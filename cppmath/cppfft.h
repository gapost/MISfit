#ifndef CPPFFT_H
#define CPPFFT_H

#include <tnt110\tnt.h>

typedef TNT::Array1D<double> CArray1D;

CArray1D fft_pow(const CArray1D& A);

double FindDominantFrequency (
    const CArray1D& ampl,          // power spectrum
    double samplingRate,      // sampling rate expressed in samples/second [Hz]
    int windowSize,           // width of search window (experiment with this; try 4)
	double cutoff = 20.
	);


#endif