// S1D.cpp: implementation of the CS1D class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Schrodinger1D.h"

#include <fstream>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSchrodinger1D::~CSchrodinger1D()
{
	CSchrodinger1DItem *P1, *P = First;
	while (P!=0) {
		P1 = P->GetNext();
		delete P;
		P = P1;
	}
}

void CSchrodinger1D::Add(CSchrodinger1DItem *P)
{
	if (nItem==0) {
		First = P;
		P->SetNext(0);
		P->SetPrev(0);
	} else {
		Last->SetNext(P);
		P->SetPrev(Last);
		P->SetNext(0);
	}
	Last = P;
	nItem++;
}

void CSchrodinger1D::Init(double xo)
{
	assert(nItem>2 && nItem%2==1);

	CSchrodinger1DItem* P;

	M = 0; 
	P = First;
	while(P!=0) {
		M += P->get_m();
		P->SetBeta(beta);
		P = P->GetNext();
	}

	x  = CArray1D(M);
	L  = CArray1D(M);
	h  = CArray1D(M);
	q  = CArray1D(M);
	dq  = CArray1D(M);
	v  = CArray1D(M);

	T = CTridiagSym2D(M);
	d1 = CArray1D(M);

	E = CArray1D(M);
	W1 = CArray1D(M);
	W2 = CArray1D(M);
	Z = CArray2D(M,M);
	G = CArray2D(7,M);
	Md = CArray1D(M);
	absMd = CArray1D(M);
	Delta = CArray2D(M,M);

	int i = 0;
	P = First;
	while (P!=0) {
		P->Assign(this, i);
		P = P->GetNext();
	}
	assert(i==M);

	x[0] = xo;
	P = First;
	while (P!=0) {
		P->MakeGrid();
		P = P->GetNext();
	}

	for(i=0; i<M-1; i++) h[i]=x[i+1]-x[i];
	h[M-1]=h[M-2];
	for(i=1; i<M; i++) L[i]=sqrt((h[i]+h[i-1])/2);
	L[0] = sqrt(h[0]);

	P = First;
	i=0;
	while (P!=0) {
		// fix DOS mass matrices
		int m = P->get_m();
		double theMd = P->get_Md();
		for(; m>0; m--) { Md[i]=theMd; absMd[i++]=fabs(theMd); }
		// fix kinetic energy operator matrix
		P->MakeMatrix();
		P = P->GetNext();
	}
	assert(i==M);
}

void CSchrodinger1D::Init(int aM, double* ax, double* ah, double* aL, double* av, double* aq, double* adq, double* adelta)
{
	assert(nItem>2 && nItem%2==1);

	CSchrodinger1DItem* P;

	M = 0; 
	P = First;
	while(P!=0) {
		M += P->get_m();
		P->SetBeta(beta);
		P = P->GetNext();
	}

	assert( aM==M );

	x  = CArray1D(M,ax);
	h  = CArray1D(M,ah);
	L  = CArray1D(M,aL);
	q  = CArray1D(M,aq);
	dq  = CArray1D(M,adq);
	v  = CArray1D(M,av);
	Delta = CArray2D(M,M,adelta);

	T = CTridiagSym2D(M);
	d1 = CArray1D(M);

	E = CArray1D(M);
	W1 = CArray1D(M);
	W2 = CArray1D(M);
	Z = CArray2D(M,M);
	G = CArray2D(7,M);
	Md = CArray1D(M);
	absMd = CArray1D(M);

	int i = 0;
	P = First;
	while (P!=0) {
		P->Assign(this,i);
		P = P->GetNext();
	}
	assert(i==M);

	P = First;
	i=0;
	while (P!=0) {
		// fix DOS mass matrices
		int m = P->get_m();
		double theMd = P->get_Md();
		for(; m>0; m--) { Md[i]=theMd; absMd[i++]=fabs(theMd); }
		// fix kinetic energy operator matrix
		P->MakeMatrix();
		P = P->GetNext();
	}
	assert(i==M);
}

int CSchrodinger1D::Solve(double eps)
{
	double lbound = 0., ubound = 0.;
	CSchrodinger1DItem* P = First;
	while (P!=0) {
		P->CalcBounds(lbound,ubound,eps);
		P = P->GetNext();
	}

	return Solve(lbound,ubound);
}

int CSchrodinger1D::Solve(double lbound, double ubound)
{
	for(int k=0; k<M; k++) T.d(k) = d1[k] + v[k];
	
	ne = 0; 
	int ret = Tsturm(T,E,Z,lbound,ubound,ne,G);
#ifdef _DEBUG
	cout << "Eigenvalues from " << lbound/beta << " to " << ubound/beta << " : " << ne << endl;
#endif
	if (ret!=0) return -1;

	if (ne) ProcessEigenfuncs();

	return ne;
}

void CSchrodinger1D::ProcessEigenfuncs()
{
	for(int k=0; k<ne; k++) {
		double ex = sign*E[k];
		W2[k] = exp(-ex);
		if (ex<34.) {
			W1[k] = 1. + W2[k];
			W2[k] /= W1[k];
			W1[k] = log(W1[k]);
		}
		else W1[k] = W2[k];
		double* pz = Z[k];
		double* pl = &L[0];
		for(int i=0; i<M; i++) {
			double z = *pz/(*pl);
			*pz = z*z;
			pz++;
			pl++;
		}
	}
}


void CSchrodinger1D::CalcQ()
{
	//for(int i=0; i<M; i++) { q[i]=0.; dq[i]=0.; }
	//AddQ();
	double* pq = &q[0];
	double* pdq = &dq[0];
	double* qend = pq + M;
	double* pmd = &Md[0];
	double* pabsmd = &absMd[0];

	for(int i=0; i<M; i++) {
		*pq=0.; *pdq=0.;
		for(int k=0; k<ne; k++) {
			double z = Z[k][i];
			*pq -= (*pmd)*z*W1[k];
			*pdq += (*pabsmd)*z*W2[k];
		}
		pq++;
		pdq++;
		pmd++;
		pabsmd++;
	}
}
void CSchrodinger1D::CalcDelta()
{
	for(int i=0; i<M; i++) {
		for(int j=i; j<M; j++) {
			double sum = 0.;
			for(int k=0; k<ne; k++) {
				sum += absMd[i]*Z[k][i]*Z[k][j]*W2[k];
			}
			Delta[i][j] = sum; 
		}
	}
}
void CSchrodinger1D::AddDelta()
{
	for(int i=0; i<M; i++) {
		for(int j=i; j<M; j++) {
			double sum = 0.;
			for(int k=0; k<ne; k++) {
				sum += absMd[i]*Z[k][i]*Z[k][j]*W2[k];
			}
			Delta[i][j] += sum; 
		}
	}
}
void CSchrodinger1D::AddQ()
{
/*	CSchrodinger1DItem* P = First;
	while (P!=0) {
		P->CalcQ();
		P = P->GetNext();
	}*/
	double* pq = &q[0];
	double* pdq = &dq[0];
	double* qend = pq + M;
	double* pmd = &Md[0];
	double* pabsmd = &absMd[0];

	for(int i=0; i<M; i++) {
		for(int k=0; k<ne; k++) {
			double z = Z[k][i];
			*pq -= (*pmd)*z*W1[k];
			*pdq += (*pabsmd)*z*W2[k];
		}
		pq++;
		pdq++;
		pmd++;
		pabsmd++;
	}

}

void CSchrodinger1D::CalcNs(double& Ns, double& xd)
{
	Ns = 0.; xd = 0.;
	if (ne) {
		for(int i = 1; i<M-1; i++) {
			double f =  q[i]*(x[i+1] - x[i-1])/2;
			Ns += f;
			xd += f*x[i];
		}
		xd = xd/Ns;
		Ns = Ns/beta*N0*L0;
	}

#ifdef _DEBUG
	cout << "Ns = " << Ns << ", xd = " << xd << endl;
#endif
}

void CSchrodinger1D::Dump(const char *s)
{
	ofstream foo(s);
	foo << "x\tV\tQ\tdQ\n";

	CSchrodinger1DItem* P = First;
	while (P!=0) {
		P->Dump(foo);
		P = P->GetNext();
	}

	foo.close();

}

/////////////////////////////////////////////////////////////////////////////////
void COpenSchrodinger1D::Init(double xo)
{
	CSchrodinger1D::Init(xo);

	assert( ((CSchrodingerBC*)First)->get_type() == CSchrodingerBC::transmit );

	H = CComplexTridiag2D(M);
	b = CComplexArray1D(M);
	b1 = CComplexArray1D(M);
	Gamma = CArray1D(M);

}

void COpenSchrodinger1D::Init(int aM, double* ax, double* ah, double* aL, double* av, double* aq, double* adq, double* adelta)
{
	CSchrodinger1D::Init(aM,ax,ah,aL,av,aq,adq, adelta);

	assert( ((CSchrodingerBC*)First)->get_type() == CSchrodingerBC::transmit );

	H = CComplexTridiag2D(M);
	b = CComplexArray1D(M);
	b1 = CComplexArray1D(M);
	Gamma = CArray1D(M);

}

void COpenSchrodinger1D::RefineEigenFunctions()
{
	int rne = 0, i;
	double im0 = sign*SCHROD_E0*beta;

	for(int k=0; k<ne; k++) {
		if (IsValidEigenfunction(k)) {
			// prepare vars
			cdouble w,w1(E[k],0);
			for (i=0; i<M; i++) b[i] = cdouble(Z[k][i],0);

			// refine loop
			double eps;
			int iloop=0;
			do {
				w = w1;
				// init eigenfunc
				for (i=0; i<M; i++) b1[i] = b[i];
				// init Hamiltonian
				for(i=0; i<M; i++) {
					H.d(i) = cdouble(T.d(i),0) - w;
					if (i>0) H.s(i) = cdouble(T.s(i),0);
					if (i<M-1) H.e(i) = cdouble(T.s(i+1),0);
				}
				H.d(0) -= cdouble(0,1)*std::sqrt((w-v[0])*im0)/h[0];

				// Solve
				Dgtsl(H,b1);


				// Calc eigenvalue
				{
					cdouble s(0.,0.);
					for(i=0; i<M; i++) s += std::conj(b[i])*b1[i];					
					w1 = w + 1./s;
				}

				// normalize eigenfunc
				{
					cdouble s(0.,0.);
					for(i=0; i<M; i++) s += std::conj(b1[i])*b1[i];					
					for(i=0; i<M; i++) b[i] = b1[i]/std::sqrt(s);
				}
				eps = std::abs(w1 - w);
				iloop++;

			} while (eps>1e-12 && iloop<=20);

			E[rne] = std::real(w1);
			Gamma[rne] = std::imag(w1);
			for(i=0; i<M; i++) Z[rne][i] = std::norm(b[i])/L[i]/L[i];
			rne++;
		}
	}
	ne = rne;
}

void COpenSchrodinger1D::ProcessEigenfuncs()
{
	RefineEigenFunctions();

	for(int k=0; k<ne; k++) {
		double ex = sign*E[k];
		W2[k] = exp(-ex);
		if (ex<34.) {
			W1[k] = 1. + W2[k];
			W2[k] /= W1[k];
			W1[k] = log(W1[k]);
		}
		else W1[k] = W2[k];
	}
}


#define I_CONST (ELECTRON_CHARGE*N0)/HBAR*L0

double COpenSchrodinger1D::CalcI()
{
	double I = 0;
	AddI(I);
	return I*I_CONST/beta/beta;	
}

void COpenSchrodinger1D::AddI(double& I)
{
	double Md = ( (CSchrodinger1DLayer*)(First->GetNext()) )->Md;

	for(int i=0; i<ne; i++) I -= Md*W1[i]*Gamma[i];
}

///////////////////////////////////////////////////////////////////////////////
CMultiBandSchrodinger1D::~CMultiBandSchrodinger1D()
{
	for(int i=0; i<nbands; i++) delete (Bands.at(i));
}

void CMultiBandSchrodinger1D::Init(double xo)
{
	assert(nbands>0);

	int i = 0;
	CSchrodinger1D* pS0 = Bands.at(i);

	pS0->Init(xo);

	x = pS0->x;
	h = pS0->h;
	L = pS0->L;
	v = pS0->v;
	q = pS0->q;
	dq = pS0->dq;

	for(; i<nbands; i++) 
		Bands.at(i)->Init(x.dim(),&x[0],&h[0],&L[0],&v[0],&q[0],&dq[0],&(pS0->Delta[0][0]));

}
void CMultiBandSchrodinger1D::Init(int aM, double* ax, double* ah, double* aL, double* av, double* aq, double* adq, double* adelta)
{
	x  = CArray1D(aM,ax);
	h  = CArray1D(aM,ah);
	L  = CArray1D(aM,aL);
	q  = CArray1D(aM,aq);
	dq  = CArray1D(aM,adq);
	v  = CArray1D(aM,av);
	Delta = CArray2D(aM,aM,adelta);
	for(int i=0; i<nbands; i++) 
		Bands.at(i)->Init(x.dim(),&x[0],&h[0],&L[0],&v[0],&q[0],&dq[0],adelta);

}
int CMultiBandSchrodinger1D::Solve(double eps)
{
	ne = 0;
	for(int i=0; i<nbands; i++) {
		int ret = Bands.at(i)->Solve(eps);
		assert(ret>=0);
		if (ret>0) ne += ret;
	}
	fixed_tails = false;
	return ne;
}
double CMultiBandSchrodinger1D::CalcI()
{

	double I = 0;
	if (fabs(Ns)>0.) {
		for(int i=0; i<nbands; i++)
			Bands.at(i)->AddI(I);
	}
	return I*I_CONST/beta/beta;	
}
void CMultiBandSchrodinger1D::CalcQ()
{
	if (ne) {
		int i = 0;
		CSchrodinger1D* pS0 = Bands.at(i);
		
		pS0->CalcQ();
		for(; i<nbands; i++) 
			Bands.at(i)->AddQ();
	}
}
void CMultiBandSchrodinger1D::CalcDelta()
{
	if (ne) {
		int i = 0;
		CSchrodinger1D* pS0 = Bands.at(i);
		
		pS0->CalcDelta();
		for(; i<nbands; i++) 
			Bands.at(i)->AddDelta();
	}
}
void CMultiBandSchrodinger1D::CalcNs()
{
	Ns = 0.; xs = 0.;
	if (ne) {
		int n = x.dim()-1;
		for(int i = 1; i<n; i++) {
			double f =  q[i]*(x[i+1] - x[i-1])/2;
			Ns += f;
			xs += f*x[i];
		}
		if (Ns>0.) xs = xs/Ns;
	}

#ifdef _DEBUG
	cout << "Ns = " << Ns << ", xs = " << xs << endl;
#endif
}
void CMultiBandSchrodinger1D::Dump(const char* s)
{
	ofstream foo(s);
	CSchrodinger1DItem** P = new CSchrodinger1DItem*[nbands];
	for(int i = 0; i<nbands; i++) P[i] = Bands.at(i)->First;

	int k = 0;
	while (P[0]!=0) {
		int m = P[0]->get_m();
		if (m>1) {
			for(int j=0; j<=m; j++) {
				foo << x[k] << "\t";
				for(int i = 0; i<nbands; i++) 
					foo << (v[k] + ((CSchrodinger1DLayer*)P[i])->dE )/beta << "\t";
				foo << q[k]/beta*N0 << "\t";
				foo << dq[k]/beta*N0 << "\n";
				k++;
			}
		}
		for(int i = 0; i<nbands; i++) P[i] = P[i]->GetNext();
	}
}



void CMultiBandSchrodinger1D::FixTail(double xm, double qcl, double dqcl,
				double xm1, double qcl1, double dqcl1)
{
	if (!(fabs(Ns)>0.) || fixed_tails) return;

	int M = x.dim(), is = M-1;
	while(fabs(x[is]-xm) < 5. && is>2) is--;

	double xo = x[is];
	double q0 = q[is];
	double dq0 = dq[is];
	double aq = (qcl-q0)/(xm - xo);
	double adq = (dqcl-dq0)/(xm - xo);

	for(int i=is+1; i<M; i++) {
		double y = x[i]-xo;
		q[i] = q0 + aq*y;
		dq[i] = dq0 + adq*y;
	}

	fixed_tails = true;

}

void CMultiBandSchrodinger1D::FixTail2()
{
	if (!(fabs(Ns)>0.) || fixed_tails) return;

	int M = x.dim(), i = M-1;

	double fqmax = (q[i-1] - q[i])/(x[i] - x[i-1])/Ns, fq;

	bool found = false;

	// find max
	while (!found && i>0) {
		i--;
		fq = (q[i-1] - q[i])/(x[i] - x[i-1])/Ns;
		if (fq>=fqmax) fqmax = fq;
		else found = true;
	}

	if (i==0) return;

	found = false;
	fqmax /= 3;

	// scan to half max
	while (!found && i>0) {
		i--;
		fq = (q[i-1] - q[i])/(x[i] - x[i-1])/Ns;
		if (fq<fqmax) found = true;
	}

	if (i==0) return;

	i++;
	for(; i<M; i++) q[i] = q[i-1]; 


}
