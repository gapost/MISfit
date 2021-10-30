#ifndef UTILS_H
#define UTILS_H

inline double fabs(cdouble& c) { return std::abs(c); }
inline float fabs(cfloat& c) { return std::abs(c); }

inline float fsqr(float x)
{ return x == 0.0f ? 0.0f : x*x; }
inline double fsqr(double x)
{ return x == 0.0 ? 0.0 : x*x; }

inline ostream& operator<<(ostream &s, const cdouble &C)
{
	s << std::real(C);
	double im = std::imag(C);
	if (im>=0.) s << " + i";
	else s << " - i";
	s << fabs(im);
    return s;
}

inline ostream& operator<<(ostream &s, const cfloat &C)
{
	s << std::real(C);
	double im = std::imag(C);
	if (im>=0.) s << " + i";
	else s << " - i";
	s << (float)fabs(im);
    return s;
}



#endif