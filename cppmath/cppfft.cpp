#include "cppmath.h"

void realft(double datain[], unsigned long n, int isign);

int IsPowerOfTwo ( unsigned x )
{
    if ( x < 2 )
        return 0;

    if ( x & (x-1) )        // Thanks to 'byang' for this cute trick!
        return 0;

    return 1;
}

CArray1D fft_pow(const CArray1D& A)
{
	unsigned int n = A.dim();
	CArray1D B; 
	if (!IsPowerOfTwo(n)) return B;

	B = A.copy();

	realft(&B[0],n,1);

	unsigned int m = n/2;
	CArray1D Spectrum(m);
	Spectrum[0] = B[0]*B[0]; //log(B[0]);
	
	for(unsigned int i=1; i<m; i++) 
		Spectrum[i] = B[2*i]*B[2*i] + B[2*i+1]*B[2*i+1];

	return Spectrum;
}

double FindDominantFrequency (
    const CArray1D& ampl,     // power spectrum
    double samplingRate,      // sampling rate expressed in samples/second [Hz]
    int windowSize,            // width of search window (experiment with this; try 4)
	double cutoff
	)
{
    const int n = ampl.dim();   // number of positive frequencies.
	int numSamples = 2*n;

    // Zero out amplitudes for frequencies below 20 Hz.
    int low = int ( numSamples * cutoff / samplingRate );
    if (!( low>=0 && low<numSamples )) return 0.;
    /*for ( i=low; i>=0; i-- )
    {
        ampl[i] = 0.0;
    }*/

    // In the following loop, i1 is the left side of the window
    // and i2 is the right side.

    double maxWeight = -1.0;
    double centerFreq = -1.0;

    for ( int i2 = windowSize; i2 < n; i2++ )
    {
        const int i1 = i2 - windowSize;

        double moment = 0.0;
        double weight = 0.0;

        for ( int i=i1; i<=i2; i++ )
        {
			double a = ampl[i];
			if (i<=low) a = 0.;
            weight += a;
            moment += i * a;
        }

        if ( weight > maxWeight )
        {
            maxWeight = weight;
            centerFreq = moment / weight;
        }
    }

    return samplingRate * centerFreq / double(numSamples);
}



