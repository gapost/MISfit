#include "stdafx.h"

#include <cassert>
#include <fstream>

#include "DielectricLayer.h"
#include "Poisson1D.h"
#include "FermiFunc.h"

/*********************************************************************/
void CDielectricLayer::CalcQ(int hf)
{
	assert(Prev);

	ChargeArray& q1 = ((CPoissonBC*)Prev)->RightQ();
	q1[ELECTRONS] = 0;
	q1[HOLES] = 0;
	q1[FIXED_CHARGE] = Qf;
	q1[Q_SUM] = Qf;
	ChargeArray& dq1 = ((CPoissonBC*)Prev)->RightdQ();
	double* p = (double*)dq1; 
	double* pend = p + N_CHARGE_TYPES;
	for( ; p!=pend; p++) *p=0.;

	int m = get_m();
	for(int i=0; i<m; i++) {
		q[i][ELECTRONS] = 0;
		q[i][HOLES] = 0;
		q[i][FIXED_CHARGE] = Qf;
		q[i][Q_SUM] = Qf;
	}


	p = (double*)dq;
	pend = p + N_CHARGE_TYPES*m;
	for( ; p!=pend; p++) *p=0.;

	assert(Next);

	ChargeArray& q0 = ((CPoissonBC*)Next)->LeftQ();
	q0[ELECTRONS] = 0;
	q0[HOLES] = 0;
	q0[FIXED_CHARGE] = Qf;
	q0[Q_SUM] = Qf;
	ChargeArray& dq0 = ((CPoissonBC*)Next)->LeftdQ();
	p = (double*)dq0; 
	pend = p + N_CHARGE_TYPES;
	for( ; p!=pend; p++) *p=0.;
}
/**********************************************************************/
void CSemiconductorLayer::CalcQ(int hf)
{
	int ifreeze = 0;

	if (hf) {
		if (Nd>=Na) ifreeze = HOLES;
		else ifreeze = ELECTRONS;
	}

	assert(Prev);

	CalcQ_(-1,
		((CPoissonBC*)Prev)->RightQ(),
		((CPoissonBC*)Prev)->RightdQ(),
		ifreeze);

	assert(Next);

	int m = get_m();
	CalcQ_(m,
		((CPoissonBC*)Next)->LeftQ(),
		((CPoissonBC*)Next)->LeftdQ(),
		ifreeze);

	for(int i=0; i<m; i++) CalcQ_(i,q[i],dq[i],ifreeze);
}
void CSemiconductorLayer::CalcQ_(int i, ChargeArray& pq, ChargeArray& pdq, int ifreeze)
{
	double ec = Ec+*(v+i), ev = Ev+*(v+i);
	double n,dn,p,dp;

	//double qpar = *(x+i)/1.21;
	//qpar = 1 - exp(-qpar*qpar);

	ImpG(fermi,ec-Ed,n,dn);
	ImpG(fermi,-ev-Ea,p,dp,4);
	pq[FIXED_CHARGE] = Nd*n - Na*p;
	pdq[FIXED_CHARGE] = Nd*dn + Na*dp;

	if (ifreeze!=ELECTRONS) {
		FermiIntegral(fermi,-ec,n,dn);
		pq[ELECTRONS] = -Nc*n; //*qpar;
		pdq[ELECTRONS] = Nc*dn; //*qpar;
	}
	else pdq[ELECTRONS] = 0.;

	if (ifreeze!=HOLES) {
		FermiIntegral(fermi,ev,p,dp);
		pq[HOLES] = Nv*p; //*qpar;
		pdq[HOLES] = Nv*dp; //*qpar;
	} 
	else pdq[HOLES] = 0.;
	
	pq[0]=0.; pdq[0]=0.;
	for(int j=1; j<N_CHARGE_TYPES; j++) {
		pq[0] += pq[j];
		pdq[0] += pdq[j];
	}
}
void CSemiconductorLayer::CalcCP()
{
	// initial estimate
	*v = (Nd-Na)/sqrt(Nv*Nc)/exp(-(Ec-Ev)/2)/2;
	*v = asinh(*v);
	*v = 0.5*log(Nv/Nc) + *v;
	*v = (Ec-Ev)/2 - Ec - *v;

	// refine
	do {
		CSemiconductorLayer::CalcQ_(0,*q,*dq);
		*v -= (*q)[0]/(*dq)[0];
	} while (fabs(*q[0])>1e-6);

	*(v-1) = *v;
	int m = get_m();
	for(int i=1; i<=m; i++) v[i] = *v;
}
void CSemiconductorLayer::SetBeta(double abeta)
{
	double c = abeta/beta;
	Ec *= c;
	Ev *= c;
	Ed *= c;
	Ea *= c;
	Nc *= c;
	Nv *= c;
	Nd *= c;
	Na *= c;
	beta = abeta;
}
/********************************************************************************************/
void CQuantumSemiconductorLayer::CalcQ_(int i, ChargeArray& pq, ChargeArray& pdq, int ifreeze)
{
	double ec = Ec+*(v+i), ev = Ev+*(v+i);
	double n,dn,p,dp;

	ImpG(fermi,ec-Ed,n,dn);
	ImpG(fermi,-ev-Ea,p,dp,4);
	pq[FIXED_CHARGE] = Nd*n - Na*p;
	pdq[FIXED_CHARGE] = Nd*dn + Na*dp;

	if (ifreeze!=ELECTRONS) {
		if (fabs(*nse)>0.) {
			pq[ELECTRONS] = qe[i];
			pdq[ELECTRONS] = dqe[i];
		}
		else {
			FermiIntegral(fermi,-ec,n,dn);
			pq[ELECTRONS] = -Nc*n; //*qpar;
			pdq[ELECTRONS] = Nc*dn; //*qpar;
		}
	}
	else pdq[ELECTRONS] = 0.;
	
	if (ifreeze!=HOLES) {
		if (*nsh>0.) {
			pq[HOLES] = qh[i];
			pdq[HOLES] = dqh[i];
		}
		else {
			FermiIntegral(fermi,ev,p,dp);
			pq[HOLES] = Nv*p; //*qpar;
			pdq[HOLES] = Nv*dp; //*qpar;
		}
	} 
	else pdq[HOLES] = 0.;
	
	pq[0]=0.; pdq[0]=0.;
	for(int j=1; j<N_CHARGE_TYPES; j++) {
		pq[0] += pq[j];
		pdq[0] += pdq[j];
	}
}

/*********************************************************************/
void CQuantumDielectricLayer::CalcQ(int hf)
{
	assert(Prev);
	int i;

	ChargeArray& q1 = ((CPoissonBC*)Prev)->RightQ();
	ChargeArray& dq1 = ((CPoissonBC*)Prev)->RightdQ();

	if (fabs(*nse)>0.) {
		q1[ELECTRONS] = qe[-1];
		dq1[ELECTRONS] = dqe[-1];
	}
	else {
		q1[ELECTRONS] = 0.;
		dq1[ELECTRONS] = 0.;
	}
	if (*nsh>0.) {
		q1[HOLES] = qh[-1];
		dq1[HOLES] = dqh[-1];
	}
	else {
		q1[HOLES] = 0.;
		dq1[HOLES] = 0.;
	}
	q1[FIXED_CHARGE] = Qf;
	q1[0] = 0.;
	dq1[FIXED_CHARGE] = 0.;
	dq1[0] = 0.;
	for(i=1; i<N_CHARGE_TYPES; i++) {
		q1[0] += q1[i];
		dq1[0] += dq1[i];
	}

	int m = get_m();
	if (fabs(*nse)>0.) {
		for(i=0; i<m; i++) {
			q[i][ELECTRONS] = qe[i];
			dq[i][ELECTRONS] = dqe[i];
		}
	}
	else {
		for(i=0; i<m; i++) {
			q[i][ELECTRONS] = 0.;
			dq[i][ELECTRONS] = 0.;
		}
	}

	if (*nsh>0.) {
		for(i=0; i<m; i++) {
			q[i][HOLES] = qh[i];
			dq[i][HOLES] = dqh[i];
		}
	}
	else {
		for(i=0; i<m; i++) {
			q[i][HOLES] = 0.;
			dq[i][HOLES] = 0.;
		}
	}

	for(i=0; i<m; i++) {
		q[i][FIXED_CHARGE] = Qf;
		q[i][0] = 0.;
		dq[i][FIXED_CHARGE] = 0.;
		dq[i][0] = 0.;
		for(int j=1; j<N_CHARGE_TYPES; j++) {
			q[i][0] += q[i][j];
			dq[i][0] += dq[i][j];
		}
	}


	assert(Next);

	ChargeArray& q0 = ((CPoissonBC*)Next)->LeftQ();
	ChargeArray& dq0 = ((CPoissonBC*)Next)->LeftdQ();

	if (fabs(*nse)>0.) {
		q0[ELECTRONS] = qe[m];
		dq0[ELECTRONS] = dqe[m];
	}
	else {
		q0[ELECTRONS] = 0.;
		dq0[ELECTRONS] = 0.;
	}
	if (*nsh>0.) {
		q0[HOLES] = qh[m];
		dq0[HOLES] = dqh[m];
	}
	else {
		q0[HOLES] = 0.;
		dq0[HOLES] = 0.;
	}
	q0[FIXED_CHARGE] = Qf;
	q0[0] = 0.;
	dq0[FIXED_CHARGE] = 0.;
	dq0[0] = 0.;
	for(i=1; i<N_CHARGE_TYPES; i++) {
		q0[0] += q0[i];
		dq0[0] += dq0[i];
	}

}







