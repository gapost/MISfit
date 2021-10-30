#ifndef PTABLE_H
#define PTABLE_H

#include <complex>

typedef std::complex<double> cdouble;

class CPeriodicTable {
public:
	double Mass(int Z);
	double Density(int Z);
	cdouble FormFactor(int Z);
	int AnalyzeFormula(const char* strFormula, CArray1D& X, CIntArray1D& Z);
	int Z(const char* name);
	char* Name(int Z) const;
};


#endif
