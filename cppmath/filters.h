#ifndef FILTERS_H
#define FILTERS_H

#include <math.h>

template<class T>
class CAverage {
	private:
		double sum,sum2;
		unsigned int n;
	public:
		CAverage() : sum(0), sum2(0), n(0) {}
		void Reset() { sum=0; sum2=0; n=0; }
		void in(T val) { 
			sum += val; sum2 += (1.0*val)*val; n++; 
		}
		T out() { return (T)(out_d()); }
		double out_d() { if (n) return (sum/n); else return 0.; }
		T var() { return (T)(var_d()); }
		double var_d() { if (n) return sqrt((sum2 - sum*sum/n)/n); else return 0.; }
};

template<class T>
class CRunningAverage {
	private:
		TNT::Array1D<T> A;
		double sum;
		unsigned int n;
		unsigned int i;
		bool transient;
	public:
		CRunningAverage() : A(10), i(0), sum(0), n(10), transient(true) {}
		CRunningAverage(unsigned int an) : A(an), i(0), sum(0), n(an), transient(true) {}
		void Reset(unsigned int an) { sum=0.; n=an; i=0; A = TNT::Array1D<T>(10); transient=true; }
		void Reset() { sum=0.; i=0; transient=true; }
		void in(T val) {
			if (transient) {
				A[i++] = val; 
				sum = 0.;
				for(int j=0; j<i; j++) sum += A[j];
				sum /= i;
				if (i==n) { transient=false; i=0; }
			}
			else {
				sum -= 1.0*A[i]/n;
				A[i] = val;
				sum += 1.0*A[i++]/n;
				i %= n;
			}
		}
		T out() { return (T)sum; }
		double out_d() { return sum; }
};

#endif
