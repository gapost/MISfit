#include "cppmath.h"

#include <fstream>

int textload(const char* fname, CArray1D& A)
{
	//Count Numbers
	int n=0;
	float dd=0.;
	FILE* pfile = fopen(fname,"r");
	while(fscanf(pfile,"%e",&dd)!=EOF) n++;
	fclose(pfile);

	A = CArray1D();

	if (n==0) return n;

	A = CArray1D(n);

	pfile = fopen(fname,"r");
	for(int i=0; i<n; i++) {
		fscanf(pfile,"%e",&dd);
			A[i] = dd;
	}
	fclose(pfile);

	return n;
}

int textload(const char* fname, int nCol, CArray2D& A, int trans)
{
	assert(nCol>0);

	//Count Numbers
	int n=0;
	float dd=0.;
	FILE* pfile = fopen(fname,"r");
	while(fscanf(pfile,"%e",&dd)!=EOF) n++;
	fclose(pfile);

	int nRow = n/nCol;
	if (nRow==0 || n%nCol!=0) return 0;



	pfile = fopen(fname,"r");

	if (trans)
	{
		A = CArray2D(nCol,nRow);
		for(int i=0; i<nRow; i++)
			for(int j=0; j<nCol; j++)
			{
				fscanf(pfile,"%e",&dd);
				A[j][i] = dd;
			}
	}
	else
	{
		A = CArray2D(nRow,nCol);
		for(int i=0; i<nRow; i++)
			for(int j=0; j<nCol; j++)
			{
				fscanf(pfile,"%e",&dd);
				A[i][j] = dd;
			}
	}

	fclose(pfile);

	return nRow;
}

template<class T>
int textload(istream& is, TNT::Array1D<T>& A)
{
	streampos pos = is.tellg();

	A = TNT::Array1D<T>();

	int n = 0;
	T d;
	do
	{
		is >> d;
		if (!is.fail()) n++;
		else 
		{
			if (!is.eof()) return 0;
		}
	} while (!is.eof());

	if (n==0) return n;

	is.clear();
	is.seekg(pos);

	A = TNT::Array1D<T>(n);

	for(int i=0; i<n; i++) is >> A[i];

	return n;
}

template int textload<double> (istream& is, TNT::Array1D<double>& A);
template int textload<float> (istream& is, TNT::Array1D<float>& A);
template int textload<int> (istream& is, TNT::Array1D<int>& A);

int textsave(ostream& os, const CArray1D& A)
{

	long formatmask = os.setf(ios::scientific);

	int nRow=A.dim(); 
	for(int i=0; i<nRow; i++) {
		os << A[i] << endl;
	}

	os.setf(formatmask);

	return nRow;
}

int textsave(const char* fname, const CArray1D& A)
{
	ofstream os(fname);
	int nRow = textsave(os,A);
	os.close();
	return nRow;
}

int textsave(ostream& os, const CArray2D& A)
{

	long formatmask = os.setf(ios::scientific);

	int nRow=A.dim1(),nCol=A.dim2();
	for(int i=0; i<nRow; i++) {
		os << A[i][0];
		for(int j=1; j<nCol; j++) {
			os << "\t" << A[i][j];
		}
		os << endl;
	}

	os.setf(formatmask);

	return nRow;
}


int textsave(const char* fname, const CArray2D& A)
{
	ofstream os(fname);
	int nRow = textsave(os,A);
	os.close();
	return nRow;
}

