#include "stdafx.h"

#include <cassert>
#include <fstream>

#include "Poisson1DItem.h"
#include "Poisson1D.h"
#include "FermiFunc.h"

void CPoisson1DItem::Assign(CPoisson1D* P, int& i)
{
	x = &(P->x[i]);
	h = &(P->h[i]);
	v = &(P->V[i]);
	cp = &(P->CP[i]);

	q = (ChargeArray*)(P->Q[i]);
	dq = (ChargeArray*)(P->dQ[i]);

	b = &(P->b[i]);
	cb = &(P->cb[i]);

	Td = &(P->T.d(i)); Ad = &(P->A.d(i));
	if (i!=0) { Ts = &(P->T.s(i)); As = &(P->A.s(i)); }

	Cd = &(P->C.d(i));
	if (i!=P->T.dim()-1) Ce = &(P->C.e(i));
	if (i!=0) Cs = &(P->C.s(i));

	cCd = &(P->cC.d(i));
	if (i!=P->T.dim()-1) cCe = &(P->cC.e(i));
	if (i!=0) cCs = &(P->cC.s(i));

	pPoisson = P;

	ip = i;
	i += get_m();
}
void CPoisson1DItem::Assign(CSCPS* P, int& aip, int& ais)
{
	if (isquant) {
		is = ais;
		qe = &(P->Qe[is]);
		dqe = &(P->dQe[is]);
		qh = &(P->Qh[is]);
		dqh = &(P->dQh[is]);

		nse = &(P->Electrons.Ns);
		xse = &(P->Electrons.xs);

		nsh = &(P->Holes.Ns);
		xsh = &(P->Holes.xs);

		ais += get_m();

	}
	Assign(P,aip);
}

/******************************************************************************/
void CPoissonBC::MakeMatrix()
{
	assert(Prev!=0 || Next!=0);

	double k1,k2;
	if (Prev) k1 = ((CPoisson1DLayer*)Prev)->kappa;
	if (Next) k2 = ((CPoisson1DLayer*)Next)->kappa;

	if (Prev==0) { // front
		*Td = - k2/(*h);
		*Ad = 3*(*h)/8;
	} else if (Next==0) { // back
		*Ts = k1/(*(h-1));
		*Td = - (*Ts);
		*As = *(h-1)/8;
		*Ad = 3*(*As);
	}
	else { // interior
		*Ts = k1/(*(h-1));
		*Td = -(*Ts + k2/(*h));
		*As = *(h-1)/8;
		*Ad = 3*(*As + *h/8);
	}
}

void CPoissonBC::PrepareForSolve()
{
	if (btype==voltage) {
		*Cd = 1.; 
		*b = Vb + *cp - *v;
		if (Next) *Ce = 0.;
		else *Cs = 0;
	}
	else if (btype==cont) {
		*Cs = *Ts - *As * dq[-1][0];
		*Cd = *Td - *Ad * dq[0][0] - dVb;
		*Ce = *(Ts+1) - *(As+1) * dq[1][0];
		*b = Vb + *As*(q[-1][0]) + *Ad*q[0][0] + *(As+1)*q[1][0] -
			*Ts*(*(v-1)) - *Td*(*v) - *(Ts+1)*(*(v+1)); 
	}
	else { // field
		double k1,k2;
		if (Prev) k1 = ((CPoisson1DLayer*)Prev)->kappa;
		if (Next) k2 = ((CPoisson1DLayer*)Next)->kappa;
		if (Next) { 
			*Cd = -1/(*h); 
			*Ce = -(*Cd); 
			*b = Vb - (*(v+1) - *v)/(*h) + (*h)/k2/8*(3*q[0][0] + q[1][0]);
		}
		else { 
			*Cd = 1/(*h); 
			*Cs = -(*Cd);
			*b = Vb - (*v - *(v-1))/(*h) - (*h)/k1/8*(3*q[0][0] + q[-1][0]);
		}
	}
}

void CPoissonBC::PrepareForCalcC(int hf)
{
	if (Prev==0) {
		*Cd = 1.; *Ce = 0.; *b = 1.; //dv;
	}
	else if (Next==0) {
		*Cd = 1.; *Cs = 0.; *b = 0;
	}
	else if (btype==cont) {
		if (hf) {
			int iFreeze_1 = Prev->FreezeWhat();
			int iFreeze1 = Next->FreezeWhat();
			
			if (iFreeze_1>0) 
				*Cs = *Ts - *As * (dq[-1][0]-dq[-1][iFreeze_1]);
			else
				*Cs = *Ts - *As * dq[-1][0];
			
			if (iFreeze1>0)
				*Ce = *(Ts+1) - *(As+1) * (dq[1][0] - dq[1][iFreeze1]);
			else
				*Ce = *(Ts+1) - *(As+1) * dq[1][0];
			
			if ((iFreeze1>0) || (iFreeze_1>0)) {
				double ddq0;
				double h0 = *(h-1),hm=1/(h0+*h);
				
				if (iFreeze_1>0) ddq0 = h0*(dq0[0] - dq0[iFreeze_1]);
				else ddq0 = h0*dq0[0];
				
				if (iFreeze1>0) ddq0 += (*h)*(dq1[0] - dq1[iFreeze1]);
				else ddq0 += (*h)*dq1[0];
				
				ddq0 /= hm;
				
				
				*Cd = *Td - *Ad * ddq0; 
				
			}
			else
				*Cd = *Td - *Ad * dq[0][0];
			
			
		} else {
			*Cs = *Ts - *As * dq[-1][0];
			*Cd = *Td - *Ad * dq[0][0];
			*Ce = *(Ts+1) - *(As+1) * dq[1][0];
		}
		*b = 0.; 
	}
}

void CPoissonBC::PrepareForCalcC(double omega)
{
	if (Prev==0) {
		*cCd = 1.; *cCe = 0.; *cb = 1.; //cdouble(1.,0.);
	}
	else if (Next==0) {
		*cCd = 1.; *cCs = 0.; *cb = 0;
	}
	else if (btype==cont) {
		
		int iFreeze_1 = Prev->FreezeWhat();
		int iFreeze1 = Next->FreezeWhat();
		
		if (iFreeze_1>0) 
			*cCs = *Ts - *As * (dq[-1][0]-dq[-1][iFreeze_1]);
		else
			*cCs = *Ts - *As * dq[-1][0];
		
		if (iFreeze1>0)
			*cCe = *(Ts+1) - *(As+1) * (dq[1][0] - dq[1][iFreeze1]);
		else
			*cCe = *(Ts+1) - *(As+1) * dq[1][0];
		
		if ((iFreeze1>0) || (iFreeze_1>0)) {
			double ddq0;
			double h0 = *(h-1),hm=1/(h0+*h);
			
			if (iFreeze_1>0) ddq0 = h0*(dq0[0] - dq0[iFreeze_1]);
			else ddq0 = h0*dq0[0];
			
			if (iFreeze1>0) ddq0 += (*h)*(dq1[0] - dq1[iFreeze1]);
			else ddq0 += (*h)*dq1[0];
			
			ddq0 /= hm;
			
			
			*cCd = *Td - *Ad * ddq0; 
			
		}
		else
			*cCd = *Td - *Ad * dq[0][0];
		
		
		
		*cb = 0.; 	
	}
}

void CPoissonBC::PrepareForCalcC(int hf, CArray2D& De, CArray2D& Dh, CArray2D& D)
{
	if (!isquant) { PrepareForCalcC(hf); return; }
	
	if (Prev==0) {
		*Cd = 1.; *Ce = 0.; *b = 1.; return;
	}
	
	if (Next==0) {
		*Cd = 1.; *Cs = 0.; *b = 0; return;
	}
	
	// (btype==cont) 
	int iFreeze_1 = -1, iFreeze1 = -1;
	if (hf) { iFreeze_1 = Prev->FreezeWhat(); iFreeze1 = Next->FreezeWhat(); }
	int iquant_1[2],iclass_1[3],nquant_1=0,nclass_1=0;
	int iquant1[2],iclass1[3],nquant1=0,nclass1=0;
	
	if (iFreeze_1!=ELECTRONS) {
		if (Prev->isquant) { if (fabs(*(Prev->nse))>0.) iquant_1[nquant_1++]=ELECTRONS; }
		else iclass_1[nclass_1++]=ELECTRONS;
	}
	if (iFreeze_1!=HOLES) {
		if (Prev->isquant) { if (*(Prev->nsh)>0.) iquant_1[nquant_1++]=HOLES; }
		else iclass_1[nclass_1++]=HOLES;
	}
	
	if (iFreeze1!=ELECTRONS) {
		if (Next->isquant) { if (fabs(*(Next->nse))>0.) iquant1[nquant1++]=ELECTRONS; }
		else iclass1[nclass1++]=ELECTRONS;
	}
	if (iFreeze1!=HOLES) {
		if (Next->isquant) { if (*(Next->nsh)>0.) iquant1[nquant1++]=HOLES; }
		else iclass1[nclass1++]=HOLES;
	}
	
	iclass1[nclass1++]=FIXED_CHARGE;
	iclass_1[nclass_1++]=FIXED_CHARGE;
	
	int j;
	double bq;
	
	bq=0.; for(j=0; j<nclass_1; j++) bq += dq[-1][iclass_1[j]];
	*Cs = *Ts - *As * bq;
	
	bq=0.; for(j=0; j<nclass1; j++) bq += dq[1][iclass1[j]];
	*Ce = *(Ts+1) - *(As+1) * bq;
	
	double h0 = *(h-1),hm=1/(h0+*h);
	bq=0.; 
	for(j=0; j<nclass1; j++) bq += *h*dq1[iclass1[j]];
	for(j=0; j<nclass_1; j++) bq += h0*dq0[iclass_1[j]];
	bq /= hm;
	*Cd = *Td - *Ad * bq; 
	
	*b = 0.; 

	//quantum charge
	int Ms = De.dim1();
	for(j=0; j<nquant_1; j++) {
		if (iquant_1[j]==ELECTRONS) {
			for(int k=is; k<Ms; k++) {
				D[is][k] += h0*De[is][k];
			}
		}
		else if (iquant_1[j]==HOLES) {
			for(int k=is; k<Ms; k++) {
				D[is][k] += h0*Dh[is][k];
			}
		}
	}	
	for(j=0; j<nquant1; j++) {
		if (iquant1[j]==ELECTRONS) {
			for(int k=is; k<Ms; k++) {
				D[is][k] += *h*De[is][k];
			}
		}
		else if (iquant_1[j]==HOLES) {
			for(int k=is; k<Ms; k++) {
				D[is][k] += *h*Dh[is][k];
			}
		}
	}
	for(int k=is; k<Ms; k++) {
		D[is][k] /= hm;
	}
	
}

void CPoissonBC::PrepareForCalcC(double omega, CArray2D& De, CArray2D& Dh, CArray2D& D)
{
	if (!isquant) { PrepareForCalcC(omega); return; }
	
	if (Prev==0) {
		*cCd = 1.; *cCe = 0.; *cb = 1.; return;
	}
	
	if (Next==0) {
		*cCd = 1.; *cCs = 0.; *cb = 0; return;
	}
	
	// (btype==cont) 
	int iFreeze_1 = -1, iFreeze1 = -1;
	iFreeze_1 = Prev->FreezeWhat(); iFreeze1 = Next->FreezeWhat();
	int iquant_1[2],iclass_1[3],nquant_1=0,nclass_1=0;
	int iquant1[2],iclass1[3],nquant1=0,nclass1=0;
	
	if (iFreeze_1!=ELECTRONS) {
		if (Prev->isquant) { if (fabs(*(Prev->nse))>0.) iquant_1[nquant_1++]=ELECTRONS; }
		else iclass_1[nclass_1++]=ELECTRONS;
	}
	if (iFreeze_1!=HOLES) {
		if (Prev->isquant) { if (*(Prev->nsh)>0.) iquant_1[nquant_1++]=HOLES; }
		else iclass_1[nclass_1++]=HOLES;
	}
	
	if (iFreeze1!=ELECTRONS) {
		if (Next->isquant) { if (fabs(*(Next->nse))>0.) iquant1[nquant1++]=ELECTRONS; }
		else iclass1[nclass1++]=ELECTRONS;
	}
	if (iFreeze1!=HOLES) {
		if (Next->isquant) { if (*(Next->nsh)>0.) iquant1[nquant1++]=HOLES; }
		else iclass1[nclass1++]=HOLES;
	}
	
	iclass1[nclass1++]=FIXED_CHARGE;
	iclass_1[nclass_1++]=FIXED_CHARGE;
	
	int j;
	double bq;
	
	bq=0.; for(j=0; j<nclass_1; j++) bq += dq[-1][iclass_1[j]];
	*cCs = *Ts - *As * bq;
	
	bq=0.; for(j=0; j<nclass1; j++) bq += dq[1][iclass1[j]];
	*cCe = *(Ts+1) - *(As+1) * bq;
	
	double h0 = *(h-1),hm=1/(h0+*h);
	bq=0.; 
	for(j=0; j<nclass1; j++) bq += *h*dq1[iclass1[j]];
	for(j=0; j<nclass_1; j++) bq += h0*dq0[iclass_1[j]];
	bq /= hm;
	*cCd = *Td - *Ad * bq; 
	
	*cb = 0.; 

	//quantum charge
	int Ms = De.dim1();
	for(j=0; j<nquant_1; j++) {
		if (iquant_1[j]==ELECTRONS) {
			for(int k=is; k<Ms; k++) {
				D[is][k] += h0*De[is][k];
			}
		}
		else if (iquant_1[j]==HOLES) {
			for(int k=is; k<Ms; k++) {
				D[is][k] += h0*Dh[is][k];
			}
		}
	}	
	for(j=0; j<nquant1; j++) {
		if (iquant1[j]==ELECTRONS) {
			for(int k=is; k<Ms; k++) {
				D[is][k] += *h*De[is][k];
			}
		}
		else if (iquant_1[j]==HOLES) {
			for(int k=is; k<Ms; k++) {
				D[is][k] += *h*Dh[is][k];
			}
		}
	}
	for(int k=is; k<Ms; k++) {
		D[is][k] /= hm;
	}
	
}

void CPoissonBC::FixBoundaryCharge()
{
	if (Prev==0) for(int i=0; i<N_CHARGE_TYPES; i++) { q[0][i] = q1[i]; dq[0][i] = dq1[i]; }
	else if (Next==0) for(int i=0; i<N_CHARGE_TYPES; i++) { q[0][i] = q0[i]; dq[0][i] = dq0[i]; }
	else if (btype==cont) {
		double h0 = *(h-1),hm=1/(h0+*h);
		for(int i=0; i<N_CHARGE_TYPES; i++) { 
			q[0][i] = (h0*q0[i] + *h*q1[i])*hm; 
			dq[0][i] =(h0*dq0[i] + *h*dq1[i])*hm; 
		}
	}
}

void CPoissonBC::Dump(ostream &sout)
{
	assert(Prev!=0 || Next!=0);

	double k1,k2;
	if (Prev) k1 = ((CPoisson1DLayer*)Prev)->kappa;
	if (Next) k2 = ((CPoisson1DLayer*)Next)->kappa;

	if (Prev==0) {
		sout << *x; sout << "\t";
		sout << (*cp)/beta; sout << "\t";
		sout << (*v - *cp)/beta; sout << "\t";
		sout << ((*(v+1)-*v)/(*h) - (*h)/8/k2*(3*q[0][0]+q[1][0]))/beta/L0; sout << "\t";
		sout << -q[0][ELECTRONS]*N0/beta; sout << "\t";
		sout << q[0][HOLES]*N0/beta; sout << "\t";
		sout << q[0][FIXED_CHARGE]*N0/beta; sout << "\t";
		sout << q[0][0]*N0/beta; sout << "\t";
		sout << -dq[0][ELECTRONS]*N0/beta; sout << "\t";
		sout << dq[0][HOLES]*N0/beta; sout << "\t";
		sout << dq[0][FIXED_CHARGE]*N0/beta; sout << "\t";
		sout << dq[0][0]*N0/beta; sout << "\n";
	} else if (Next==0) {
		sout << *x; sout << "\t";
		sout << (*cp)/beta; sout << "\t";
		sout << (*v - *cp)/beta; sout << "\t";
		sout << (((*v)-(*(v-1)))/(*h) + (*h)/8/k1*(3*q[0][0]+q[-1][0]))/beta/L0; sout << "\t";
		sout << -q[0][ELECTRONS]*N0/beta; sout << "\t";
		sout << q[0][HOLES]*N0/beta; sout << "\t";
		sout << q[0][FIXED_CHARGE]*N0/beta; sout << "\t";
		sout << q[0][0]*N0/beta; sout << "\t";
		sout << -dq[0][ELECTRONS]*N0/beta; sout << "\t";
		sout << dq[0][HOLES]*N0/beta; sout << "\t";
		sout << dq[0][FIXED_CHARGE]*N0/beta; sout << "\t";
		sout << dq[0][0]*N0/beta; sout << "\n";
	} else {
		sout << *x - (*(h-1))/100; sout << "\t";
		sout << (*cp)/beta; sout << "\t";
		sout << (*v - *cp)/beta; sout << "\t";
		sout << (((*v)-*(v-1))/(*(h-1)) + (*(h-1))/8/k1*(3*q0[0]+q[-1][0]))/beta/L0; sout << "\t";
		sout << -q0[ELECTRONS]*N0/beta; sout << "\t";
		sout << q0[HOLES]*N0/beta; sout << "\t";
		sout << q0[FIXED_CHARGE]*N0/beta; sout << "\t";
		sout << q0[0]*N0/beta; sout << "\t";
		sout << -dq0[ELECTRONS]*N0/beta; sout << "\t";
		sout << dq0[HOLES]*N0/beta; sout << "\t";
		sout << dq0[FIXED_CHARGE]*N0/beta; sout << "\t";
		sout << dq0[0]*N0/beta; sout << "\n";

		sout << *x + (*h)/100; sout << "\t";
		sout << (*cp)/beta; sout << "\t";
		sout << (*v - *cp)/beta; sout << "\t";
		sout << ((*(v+1)-*v)/(*h) - (*h)/8/k2*(3*q1[0]+q[1][0]))/beta/L0; sout << "\t";
		sout << -q1[ELECTRONS]*N0/beta; sout << "\t";
		sout << q1[HOLES]*N0/beta; sout << "\t";
		sout << q1[FIXED_CHARGE]*N0/beta; sout << "\t";
		sout << q1[0]*N0/beta; sout << "\t";
		sout << -dq1[ELECTRONS]*N0/beta; sout << "\t";
		sout << dq1[HOLES]*N0/beta; sout << "\t";
		sout << dq1[FIXED_CHARGE]*N0/beta; sout << "\t";
		sout << dq1[0]*N0/beta; sout << "\n";

	}
}

double CPoissonBC::GetRightF()
{
	assert(Next!=0);
	double k = ((CPoisson1DLayer*)Next)->kappa;
	if (Prev==0) return ((*(v+1)-*v)/(*h) - (*h)/8/k*(3*q[0][0]+q[1][0]))/beta/L0;
	else return ((*(v+1)-*v)/(*h) - (*h)/8/k*(3*q1[0]+q[1][0]))/beta/L0; 
}
double CPoissonBC::GetLeftF()
{
	assert(Prev!=0);
	double k = ((CPoisson1DLayer*)Prev)->kappa;
	if (Next==0) return (((*v)-(*(v-1)))/(*h) + (*h)/8/k*(3*q[0][0]+q[-1][0]))/beta/L0;
	else return (((*v)-*(v-1))/(*(h-1)) + (*(h-1))/8/k*(3*q0[0]+q[-1][0]))/beta/L0;
}
double CPoissonBC::GetRightD()
{
	assert(Next!=0);
	double k = ((CPoisson1DLayer*)Next)->kappa;
	if (Prev==0) return k*((*(v+1)-*v)/(*h) - (*h)/8/k*(3*q[0][0]+q[1][0]))/beta/L0;
	else return k*((*(v+1)-*v)/(*h) - (*h)/8/k*(3*q1[0]+q[1][0]))/beta/L0; 
}
double CPoissonBC::GetLeftD()
{
	assert(Prev!=0);
	double k = ((CPoisson1DLayer*)Prev)->kappa;
	if (Next==0) return k*(((*v)-(*(v-1)))/(*h) + (*h)/8/k*(3*q[0][0]+q[-1][0]))/beta/L0;
	else return k*(((*v)-*(v-1))/(*(h-1)) + (*(h-1))/8/k*(3*q0[0]+q[-1][0]))/beta/L0;
}

void CPoissonBC_Dit::FixBoundaryCharge()
{
	CPoissonBC::FixBoundaryCharge();
		
	pQit->Nit(*v,Vb,dVb);
}

void CPoissonBC_Dit::PrepareForCalcC(double omega)
{
	CPoissonBC::PrepareForCalcC(omega);

	*cCd -= pQit->dNit(*v,omega);
}

void CPoissonBC_Dit::PrepareForCalcC(double omega, CArray2D& De, CArray2D& Dh, CArray2D& D)
{
	CPoissonBC::PrepareForCalcC(omega,De,Dh,D);

	*cCd -= pQit->dNit(*v,omega);
}

/*********************************************************************/

void CPoisson1DLayer::MakeMatrix()
{
	int m = get_m();
	for(int i=0; i<m; i++) {
		Ts[i] = kappa/h[i-1];
		Td[i] = -Ts[i]-kappa/h[i];
		As[i] = h[i-1]/8;
		Ad[i] = 3*(h[i-1]+h[i])/8;
	}
}
void CPoisson1DLayer::CalcQ(int hf)
{
	assert(Prev);

	ChargeArray& q1 = ((CPoissonBC*)Prev)->RightQ();
	double* p = (double*)q1, *pend = p + N_CHARGE_TYPES;
	for( ; p!=pend; p++) *p=0.;
	ChargeArray& dq1 = ((CPoissonBC*)Prev)->RightdQ();
	p = (double*)dq1; 
	pend = p + N_CHARGE_TYPES;
	for( ; p!=pend; p++) *p=0.;

	p = (double*)q;
	int m = get_m();
	pend = p + N_CHARGE_TYPES*m;
	for( ; p!=pend; p++) *p=0.;
	p = (double*)dq;
	pend = p + N_CHARGE_TYPES*m;
	for( ; p!=pend; p++) *p=0.;

	assert(Next);

	ChargeArray& q0 = ((CPoissonBC*)Next)->LeftQ();
	p = (double*)q0; 
	pend = p + N_CHARGE_TYPES;
	for( ; p!=pend; p++) *p=0.;
	ChargeArray& dq0 = ((CPoissonBC*)Next)->LeftdQ();
	p = (double*)dq0; 
	pend = p + N_CHARGE_TYPES;
	for( ; p!=pend; p++) *p=0.;
}

void CPoisson1DLayer::CalcCP()
{
	double* vend = v + get_m() + 1;
	double *pv = v-1;

	for( ; pv!=vend; pv++) *pv = 0.;
}

void CPoisson1DLayer::PrepareForSolve()
{
	assert(Prev);
	ChargeArray& q0 = ((CPoissonBC*)Prev)->RightQ();
	ChargeArray& dq0 = ((CPoissonBC*)Prev)->RightdQ();

	assert(Next);
	ChargeArray& q1 = ((CPoissonBC*)Next)->LeftQ();
	ChargeArray& dq1 = ((CPoissonBC*)Next)->LeftdQ();

	int m = get_m();

	if (m==1) {
		int i = 0;
		Cd[i] = Td[i] - Ad[i]*dq[0][0]; 
		Ce[i] = Ts[i+1] - As[i+1]*dq1[0];
		Cs[i] = Ts[i] - As[i]*dq0[0];
		b[i] = - Ts[i]*v[i-1] - Td[i]*v[i] - Ts[i+1]*v[i+1] + 
			As[i]*q0[0] + Ad[i]*q[0][0] + As[i+1]*q1[0];

	}
	else {
		int i=0, mp=m-1;
		Cd[i] = Td[i] - Ad[i]*dq[i][0]; 
		Ce[i] = Ts[i+1] - As[i+1]*dq[i+1][0];
		Cs[i] = Ts[i] - As[i]*dq0[0];
		b[i] = - Ts[i]*v[i-1] - Td[i]*v[i] - Ts[i+1]*v[i+1] + 
			As[i]*q0[0] + Ad[i]*q[i][0] + As[i+1]*q[i+1][0];
		
		for(i=1; i<mp; i++) {
			Cd[i] = Td[i] - Ad[i]*dq[i][0]; 
			Ce[i] = Ts[i+1] - As[i+1]*dq[i+1][0];
			Cs[i] = Ts[i] - As[i]*dq[i-1][0];
			b[i] = - Ts[i]*v[i-1] - Td[i]*v[i] - Ts[i+1]*v[i+1] + 
				As[i]*q[i-1][0] + Ad[i]*q[i][0] + As[i+1]*q[i+1][0];
		}
		Cd[i] = Td[i] - Ad[i]*dq[i][0]; 
		Ce[i] = Ts[i+1] - As[i+1]*dq1[0];
		Cs[i] = Ts[i] - As[i]*dq[i-1][0];
		b[i] = - Ts[i]*v[i-1] - Td[i]*v[i] - Ts[i+1]*v[i+1] + 
			As[i]*q[i-1][0] + Ad[i]*q[i][0] + As[i+1]*q1[0];
	}
	
}

void CPoisson1DLayer::PrepareForCalcC(int hf)
{
	assert(Prev);
	ChargeArray& dq0 = ((CPoissonBC*)Prev)->RightdQ();
	assert(Next);
	ChargeArray& dq1 = ((CPoissonBC*)Next)->LeftdQ();
	int iFreeze = -1;
	if (hf) iFreeze = FreezeWhat();

	int m = get_m();
	if (m==1) {
		int i = 0;
		if (iFreeze>0) {
			Cd[i] = Td[i] - Ad[i]*(dq[i][0]-dq[i][iFreeze]); 
			Ce[i] = Ts[i+1] - As[i+1]*(dq1[0]-dq1[iFreeze]);
			Cs[i] = Ts[i] - As[i]*(dq0[0]-dq0[iFreeze]);
		}
		else {
			Cd[i] = Td[i] - Ad[i]*dq[i][0]; 
			Ce[i] = Ts[i+1] - As[i+1]*dq1[0];
			Cs[i] = Ts[i] - As[i]*dq0[0];
		}
		b[i] = 0.;

	}
	else {
		int i=0, mp=m-1;

		if (iFreeze>0) {
			Cd[i] = Td[i] - Ad[i]*(dq[i][0]-dq[i][iFreeze]); 
			Ce[i] = Ts[i+1] - As[i+1]*(dq[i+1][0]-dq[i+1][iFreeze]);
			Cs[i] = Ts[i] - As[i]*(dq0[0]-dq0[iFreeze]);
		}
		else {
			Cd[i] = Td[i] - Ad[i]*dq[i][0]; 
			Ce[i] = Ts[i+1] - As[i+1]*dq[i+1][0];
			Cs[i] = Ts[i] - As[i]*dq0[0];
		}
		b[i] = 0.;
		
		for(i=1; i<mp; i++) {
			if (iFreeze>0) {
				Cd[i] = Td[i] - Ad[i]*(dq[i][0]-dq[i][iFreeze]); 
				Ce[i] = Ts[i+1] - As[i+1]*(dq[i+1][0]-dq[i+1][iFreeze]);
				Cs[i] = Ts[i] - As[i]*(dq[i-1][0]-dq[i-1][iFreeze]);
			}
			else {
				Cd[i] = Td[i] - Ad[i]*dq[i][0]; 
				Ce[i] = Ts[i+1] - As[i+1]*dq[i+1][0];
				Cs[i] = Ts[i] - As[i]*dq[i-1][0];
			}
			b[i] = 0.;
		}
		
		
		if (iFreeze>0) {
			Cd[i] = Td[i] - Ad[i]*(dq[i][0]-dq[i][iFreeze]); 
			Ce[i] = Ts[i+1] - As[i+1]*(dq1[0]-dq1[iFreeze]);
			Cs[i] = Ts[i] - As[i]*(dq[i-1][0]-dq[i-1][iFreeze]);
		}
		else {
			Cd[i] = Td[i] - Ad[i]*dq[i][0]; 
			Ce[i] = Ts[i+1] - As[i+1]*dq1[0];
			Cs[i] = Ts[i] - As[i]*dq[i-1][0];
		}
		b[i] = 0.;
	}
	
}

void CPoisson1DLayer::PrepareForCalcC(int hf, CArray2D& De, CArray2D& Dh, CArray2D& D)
{
	if (!isquant) { PrepareForCalcC(hf); return; }

	assert(Prev);
	ChargeArray& dq0 = ((CPoissonBC*)Prev)->RightdQ();
	assert(Next);
	ChargeArray& dq1 = ((CPoissonBC*)Next)->LeftdQ();
	
	int iFreeze = -1;
	if (hf) iFreeze = FreezeWhat();
	int iquant[2],iclass[3],nquant=0,nclass=0;

	if (iFreeze!=ELECTRONS) {
		if (fabs(*nse)>0.) iquant[nquant++]=ELECTRONS;
		else iclass[nclass++]=ELECTRONS;
	}
	if (iFreeze!=HOLES) {
		if (*nsh>0.) iquant[nquant++]=HOLES;
		else iclass[nclass++]=HOLES;
	}
	iclass[nclass++]=FIXED_CHARGE;

	// classical charge
	int m = get_m();
	double bq;
	int j;
	if (m==1) {
		int i = 0;
		bq=0.; for(j=0; j<nclass; j++) bq += dq[i][iclass[j]];
		Cd[i] = Td[i] - Ad[i]*bq; 
		bq=0.; for(j=0; j<nclass; j++) bq += dq1[iclass[j]];
		Ce[i] = Ts[i+1] - As[i+1]*bq;
		bq=0.; for(j=0; j<nclass; j++) bq += dq0[iclass[j]];
		Cs[i] = Ts[i] - As[i]*bq;
		b[i] = 0.;
	}
	else {
		int i=0, mp=m-1;

		bq=0.; for(j=0; j<nclass; j++) bq += dq[i][iclass[j]];
		Cd[i] = Td[i] - Ad[i]*bq; 
		bq=0.; for(j=0; j<nclass; j++) bq += dq[i+1][iclass[j]];
		Ce[i] = Ts[i+1] - As[i+1]*bq;
		bq=0.; for(j=0; j<nclass; j++) bq += dq0[iclass[j]];
		Cs[i] = Ts[i] - As[i]*bq;
		b[i] = 0.;
		
		for(i=1; i<mp; i++) {
			bq=0.; for(j=0; j<nclass; j++) bq += dq[i][iclass[j]];
			Cd[i] = Td[i] - Ad[i]*bq; 
			bq=0.; for(j=0; j<nclass; j++) bq += dq[i+1][iclass[j]];
			Ce[i] = Ts[i+1] - As[i+1]*bq;
			bq=0.; for(j=0; j<nclass; j++) bq += dq[i-1][iclass[j]];
			Cs[i] = Ts[i] - As[i]*bq;
			b[i] = 0.;
		}
		
		
		bq=0.; for(j=0; j<nclass; j++) bq += dq[i][iclass[j]];
		Cd[i] = Td[i] - Ad[i]*bq; 
		bq=0.; for(j=0; j<nclass; j++) bq += dq1[iclass[j]];
		Ce[i] = Ts[i+1] - As[i+1]*bq;
		bq=0.; for(j=0; j<nclass; j++) bq += dq[i-1][iclass[j]];
		Cs[i] = Ts[i] - As[i]*bq;
		b[i] = 0.;
	}

	//quantum charge
	int mms = is+m;
	int Ms = De.dim1();
	for(j=0; j<nquant; j++) {
		if (iquant[j]==ELECTRONS) {
			for(int i=is; i<mms; i++) {
				for(int k=i; k<Ms; k++) {
					D[i][k] += De[i][k];
				}
			}
		}
		else if (iquant[j]==HOLES) {
			for(int i=is; i<mms; i++) {
				for(int k=i; k<Ms; k++) {
					D[i][k] += Dh[i][k];
				}
			}
		}
	}	
}

void CPoisson1DLayer::PrepareForCalcC(double omega)
{
	assert(Prev);
	ChargeArray& dq0 = ((CPoissonBC*)Prev)->RightdQ();
	assert(Next);
	ChargeArray& dq1 = ((CPoissonBC*)Next)->LeftdQ();

	int iFreeze = FreezeWhat();
	int m = get_m();
	if (m==1) {
		int i = 0;
		if (iFreeze>0) {
			cCd[i] = Td[i] - Ad[i]*(dq[i][0]-dq[i][iFreeze]); 
			cCe[i] = Ts[i+1] - As[i+1]*(dq1[0]-dq1[iFreeze]);
			cCs[i] = Ts[i] - As[i]*(dq0[0]-dq0[iFreeze]);
		}
		else {
			cCd[i] = Td[i] - Ad[i]*dq[i][0]; 
			cCe[i] = Ts[i+1] - As[i+1]*dq1[0];
			cCs[i] = Ts[i] - As[i]*dq0[0];
		}
		cb[i] = 0.;

	}
	else {
		int i=0, mp=m-1;

		if (iFreeze>0) {
			cCd[i] = Td[i] - Ad[i]*(dq[i][0]-dq[i][iFreeze]); 
			cCe[i] = Ts[i+1] - As[i+1]*(dq[i+1][0]-dq[i+1][iFreeze]);
			cCs[i] = Ts[i] - As[i]*(dq0[0]-dq0[iFreeze]);
		}
		else {
			cCd[i] = Td[i] - Ad[i]*dq[i][0]; 
			cCe[i] = Ts[i+1] - As[i+1]*dq[i+1][0];
			cCs[i] = Ts[i] - As[i]*dq0[0];
		}
		cb[i] = 0.;
		
		for(i=1; i<mp; i++) {
			if (iFreeze>0) {
				cCd[i] = Td[i] - Ad[i]*(dq[i][0]-dq[i][iFreeze]); 
				cCe[i] = Ts[i+1] - As[i+1]*(dq[i+1][0]-dq[i+1][iFreeze]);
				cCs[i] = Ts[i] - As[i]*(dq[i-1][0]-dq[i-1][iFreeze]);
			}
			else {
				cCd[i] = Td[i] - Ad[i]*dq[i][0]; 
				cCe[i] = Ts[i+1] - As[i+1]*dq[i+1][0];
				cCs[i] = Ts[i] - As[i]*dq[i-1][0];
			}
			cb[i] = 0.;
		}
		
		
		if (iFreeze>0) {
			cCd[i] = Td[i] - Ad[i]*(dq[i][0]-dq[i][iFreeze]); 
			cCe[i] = Ts[i+1] - As[i+1]*(dq1[0]-dq1[iFreeze]);
			cCs[i] = Ts[i] - As[i]*(dq[i-1][0]-dq[i-1][iFreeze]);
		}
		else {
			cCd[i] = Td[i] - Ad[i]*dq[i][0]; 
			cCe[i] = Ts[i+1] - As[i+1]*dq1[0];
			cCs[i] = Ts[i] - As[i]*dq[i-1][0];
		}
		cb[i] = 0.;
	}	
}

void CPoisson1DLayer::PrepareForCalcC(double omega, CArray2D& De, CArray2D& Dh, CArray2D& D)
{
	if (!isquant) { PrepareForCalcC(omega); return; }

	assert(Prev);
	ChargeArray& dq0 = ((CPoissonBC*)Prev)->RightdQ();
	assert(Next);
	ChargeArray& dq1 = ((CPoissonBC*)Next)->LeftdQ();
	
	int iFreeze = -1;
	iFreeze = FreezeWhat();
	int iquant[2],iclass[3],nquant=0,nclass=0;

	if (iFreeze!=ELECTRONS) {
		if (fabs(*nse)>0.) iquant[nquant++]=ELECTRONS;
		else iclass[nclass++]=ELECTRONS;
	}
	if (iFreeze!=HOLES) {
		if (*nsh>0.) iquant[nquant++]=HOLES;
		else iclass[nclass++]=HOLES;
	}
	iclass[nclass++]=FIXED_CHARGE;

	// classical charge
	int m = get_m();
	double bq;
	int j;
	if (m==1) {
		int i = 0;
		bq=0.; for(j=0; j<nclass; j++) bq += dq[i][iclass[j]];
		cCd[i] = Td[i] - Ad[i]*bq; 
		bq=0.; for(j=0; j<nclass; j++) bq += dq1[iclass[j]];
		cCe[i] = Ts[i+1] - As[i+1]*bq;
		bq=0.; for(j=0; j<nclass; j++) bq += dq0[iclass[j]];
		cCs[i] = Ts[i] - As[i]*bq;
		cb[i] = 0.;
	}
	else {
		int i=0, mp=m-1;

		bq=0.; for(j=0; j<nclass; j++) bq += dq[i][iclass[j]];
		cCd[i] = Td[i] - Ad[i]*bq; 
		bq=0.; for(j=0; j<nclass; j++) bq += dq[i+1][iclass[j]];
		cCe[i] = Ts[i+1] - As[i+1]*bq;
		bq=0.; for(j=0; j<nclass; j++) bq += dq0[iclass[j]];
		cCs[i] = Ts[i] - As[i]*bq;
		cb[i] = 0.;
		
		for(i=1; i<mp; i++) {
			bq=0.; for(j=0; j<nclass; j++) bq += dq[i][iclass[j]];
			cCd[i] = Td[i] - Ad[i]*bq; 
			bq=0.; for(j=0; j<nclass; j++) bq += dq[i+1][iclass[j]];
			cCe[i] = Ts[i+1] - As[i+1]*bq;
			bq=0.; for(j=0; j<nclass; j++) bq += dq[i-1][iclass[j]];
			cCs[i] = Ts[i] - As[i]*bq;
			cb[i] = 0.;
		}
		
		
		bq=0.; for(j=0; j<nclass; j++) bq += dq[i][iclass[j]];
		cCd[i] = Td[i] - Ad[i]*bq; 
		bq=0.; for(j=0; j<nclass; j++) bq += dq1[iclass[j]];
		cCe[i] = Ts[i+1] - As[i+1]*bq;
		bq=0.; for(j=0; j<nclass; j++) bq += dq[i-1][iclass[j]];
		cCs[i] = Ts[i] - As[i]*bq;
		cb[i] = 0.;
	}

	//quantum charge
	int mms = is+m;
	int Ms = De.dim1();
	for(j=0; j<nquant; j++) {
		if (iquant[j]==ELECTRONS) {
			for(int i=is; i<mms; i++) {
				for(int k=i; k<Ms; k++) {
					D[i][k] += De[i][k];
				}
			}
		}
		else if (iquant[j]==HOLES) {
			for(int i=is; i<mms; i++) {
				for(int k=i; k<Ms; k++) {
					D[i][k] += Dh[i][k];
				}
			}
		}
	}	
}

void CPoisson1DLayer::CalcF()
{
	int i=-1,mp=get_m()-1;

	assert(Prev);
	ChargeArray& q0 = ((CPoissonBC*)Prev)->RightQ();
	
	F[i+1] = (v[i+1]-v[i])/h[i] - h[i]/8/kappa*(3*q0[0]+q[i+1][0]);	

	for(i=0; i<mp; i++) 
		F[i+1] = (v[i+1]-v[i])/h[i] - h[i]/8/kappa*(3*q[i][0]+q[i+1][0]);

	assert(Next);
	ChargeArray& q1 = ((CPoissonBC*)Next)->LeftQ();

	F[i+1] = (v[i+1]-v[i])/h[i] - h[i]/8/kappa*(3*q[i][0]+q1[0]);
	i++;
	F[i+1] = (v[i]-v[i+1])/h[i-1] + h[i-1]/8/kappa*(3*q[i-1][0]+q1[0]);

}


void CPoisson1DLayer::Dump(ostream &sout)
{
	CalcF();
	int i, mp=get_m()-1;
	for(i=0; i<=mp; i++) {
		sout << x[i]; sout << "\t";
		sout << cp[i]/beta; sout << "\t";
		sout << (v[i] - cp[i])/beta; sout << "\t";
		sout << F[i+1]/beta/L0; sout << "\t";
		sout << -q[i][ELECTRONS]*N0/beta; sout << "\t";
		sout << q[i][HOLES]*N0/beta; sout << "\t";
		sout << q[i][FIXED_CHARGE]*N0/beta; sout << "\t";
		sout << q[i][0]*N0/beta; sout << "\t";
		sout << -dq[i][ELECTRONS]*N0/beta; sout << "\t";
		sout << dq[i][HOLES]*N0/beta; sout << "\t";
		sout << dq[i][FIXED_CHARGE]*N0/beta; sout << "\t";
		sout << dq[i][0]*N0/beta; sout << "\n";
	} 
}







