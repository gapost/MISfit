#include <math.h>
#define SWAP(a,b) tempr=(a);(a)=(b);(b)=tempr

#include "cppmath.h"

void four1(double datain[], unsigned long nn, int isign)
/*
Replaces data[1..2*nn] by its discrete Fourier transform, if isign is input as 1; or replaces
data[1..2*nn] by nn times its inverse discrete Fourier transform, if isign is input as -1.
data is a complex array of length nn or, equivalently, a real array of length 2*nn. nn MUST
be an integer power of 2 (this is not checked for!).
*/
{
	unsigned long n,mmax,m,j,istep,i;
	double wtemp,wr,wpr,wpi,wi,theta; //Double precision for the trigonometric recurrences.
	double tempr,tempi;

	TNT::Fortran_Array1D<double> data(2*nn,datain);

	n=nn << 1;
	j=1;
	for (i=1;i<n;i+=2) { // This is the bit-reversal section of the routine.
		if (j > i) {
			SWAP(data(j),data(i)); //Exchange the two complex numbers.
			SWAP(data(j+1),data(i+1));
		}
		m=nn;
		while (m >= 2 && j > m) {
			j -= m;
			m >>= 1;
		}
		j += m;
	}
	//Here begins the Danielson-Lanczos section of the routine.
	mmax=2;
	while (n > mmax) { //Outer loop executed log2 nn times.
		istep=mmax << 1;
		theta=isign*(6.28318530717959/mmax); //Initialize the trigonometric recurrence.
		wtemp=sin(0.5*theta);
		wpr = -2.0*wtemp*wtemp;

		wpi=sin(theta);
		wr=1.0;
		wi=0.0;
		for (m=1;m<mmax;m+=2) { //Here are the two nested inner loops.
			for (i=m;i<=n;i+=istep) {
				j=i+mmax; //This is the Danielson-Lanczos formula:
				tempr=wr*data(j)-wi*data(j+1);
				tempi=wr*data(j+1)+wi*data(j);
				data(j)=data(i)-tempr;
				data(j+1)=data(i+1)-tempi;
				data(i) += tempr;
				data(i+1) += tempi;
			}
			wr=(wtemp=wr)*wpr-wi*wpi+wr; //Trigonometric recurrence.
			wi=wi*wpr+wtemp*wpi+wi;
		}
		mmax=istep;
	}
}

void twofft(double data1in[], double data2in[], double fft1out[], double fft2out[], unsigned long n)
/*
Given two real input arrays data1[1..n] and data2[1..n], this routine calls four1 and
returns two complex output arrays, fft1[1..2n] and fft2[1..2n], each of complex length
n (i.e., real length 2*n), which contain the discrete Fourier transforms of the respective data
arrays. n MUST be an integer power of 2.
*/
{
	unsigned long nn3,nn2,jj,j;
	double rep,rem,aip,aim;

	TNT::Fortran_Array1D<double> data1(n,data1in);
	TNT::Fortran_Array1D<double> data2(n,data2in);
	TNT::Fortran_Array1D<double> fft1(2*n,data2in);
	TNT::Fortran_Array1D<double> fft2(2*n,data2in);

	nn3=1+(nn2=2+n+n);
	for (j=1,jj=2;j<=n;j++,jj+=2) { // Pack the two real arrays into one complex array.
		fft1(jj-1)=data1(j);
		fft1(jj)=data2(j);
	}
	four1(fft1out,n,1); // Transform the complex array.
	fft2(1)=fft1(2);
	fft1(2)=fft2(2)=0.0;
	for (j=3;j<=n+1;j+=2) {
		rep=0.5*(fft1(j)+fft1(nn2-j)); // Use symmetries to separate the two transforms.
		rem=0.5*(fft1(j)-fft1(nn2-j));
		aip=0.5*(fft1(j+1)+fft1(nn3-j));
		aim=0.5*(fft1(j+1)-fft1(nn3-j));
		fft1(j)=rep; // Ship them out in two complex arrays.
		fft1(j+1)=aim;
		fft1(nn2-j)=rep;
		fft1(nn3-j) = -aim;
		fft2(j)=aip;
		fft2(j+1) = -rem;
		fft2(nn2-j)=aip;
		fft2(nn3-j)=rem;
	}
}

void realft(double datain[], unsigned long n, int isign)
/*
Calculates the Fourier transform of a set of n real-valued data points. Replaces this data (which
is stored in array data[1..n]) by the positive frequency half of its complex Fourier transform.
The real-valued first and last components of the complex transform are returned as elements
data[1] and data[2], respectively. n must be a power of 2. This routine also calculates the
inverse transform of a complex data array if it is the transform of real data. (Result in this case
must be multiplied by 2/n.)
*/
{
	unsigned long i,i1,i2,i3,i4,np3;
	double c1=0.5,c2,h1r,h1i,h2r,h2i;
	double wr,wi,wpr,wpi,wtemp,theta; // Double precision for the trigonometric recurrences.
	theta=3.141592653589793/(double) (n>>1); // Initialize the recurrence.
	
	TNT::Fortran_Array1D<double> data(n,datain);
	
	if (isign == 1) {
		c2 = -0.5;
		four1(datain,n>>1,1); // The forward transform is here.
	} else {
		c2=0.5; // Otherwise set up for an inverse transform.
		theta = -theta;
	}
	wtemp=sin(0.5*theta);
	wpr = -2.0*wtemp*wtemp;
	wpi=sin(theta);
	wr=1.0+wpr;
	wi=wpi;
	np3=n+3;
	for (i=2;i<=(n>>2);i++) { // Case i=1 done separately below.
		i4=1+(i3=np3-(i2=1+(i1=i+i-1)));
		h1r=c1*(data(i1)+data(i3)); // The two separate transforms are separated out of data.
		h1i=c1*(data(i2)-data(i4));
		h2r = -c2*(data(i2)+data(i4));
		h2i=c2*(data(i1)-data(i3));
		data(i1)=h1r+wr*h2r-wi*h2i; // Here they are recombined to form the true transform of the original real data.
		data(i2)=h1i+wr*h2i+wi*h2r;
		data(i3)=h1r-wr*h2r+wi*h2i;
		data(i4) = -h1i+wr*h2i+wi*h2r;
		wr=(wtemp=wr)*wpr-wi*wpi+wr; // The recurrence.
		wi=wi*wpr+wtemp*wpi+wi;
	}
	if (isign == 1) {
		data(1) = (h1r=data(1))+data(2); // Squeeze the first and last data together to get them all within the original array.
		data(2) = h1r-data(2);
	} else {
		data(1)=c1*((h1r=data(1))+data(2));
		data(2)=c1*(h1r-data(2));
		four1(datain,n>>1,-1); // This is the inverse transform for the case isign=-1. 
	}
}

void fourn(TNT::Fortran_Array1D<double>& data, TNT::Fortran_Array1D<unsigned long>& nn, int ndim, int isign)
/*
Replaces data by its ndim-dimensional discrete Fourier transform, if isign is input as 1.
nn[1..ndim] is an integer array containing the lengths of each dimension (number of complex
values), which MUST all be powers of 2. data is a real array of length twice the product of
these lengths, in which the data are stored as in a multidimensional complex array: real and
imaginary parts of each element are in consecutive locations, and the rightmost index of the
array increases most rapidly as one proceeds along data. For a two-dimensional array, this is
equivalent to storing the array by rows. If isign is input as -1, data is replaced by its inverse
transform times the product of the lengths of all dimensions.
*/
{
	int idim;
	unsigned long i1,i2,i3,i2rev,i3rev,ip1,ip2,ip3,ifp1,ifp2;
	unsigned long ibit,k1,k2,n,nprev,nrem,ntot;
	double tempi,tempr;
	double theta,wi,wpi,wpr,wr,wtemp; //Double precision for trigonometric recurrences.
	for (ntot=1,idim=1;idim<=ndim;idim++) //Compute total number of complex values.
		ntot *= nn(idim);
	nprev=1;
	for (idim=ndim;idim>=1;idim--) { //Main loop over the dimensions.
		n=nn(idim);
		nrem=ntot/(n*nprev);
		ip1=nprev << 1;
		ip2=ip1*n;
		ip3=ip2*nrem;
		i2rev=1;
		for (i2=1;i2<=ip2;i2+=ip1) { //This is the bit-reversal section of the routine. 
			if (i2 < i2rev) {
				for (i1=i2;i1<=i2+ip1-2;i1+=2) {
					for (i3=i1;i3<=ip3;i3+=ip2) {
						i3rev=i2rev+i3-i2;
						SWAP(data(i3),data(i3rev));
						SWAP(data(i3+1),data(i3rev+1));
					}
				}
			}
			ibit=ip2 >> 1;
			while (ibit >= ip1 && i2rev > ibit) {
				i2rev -= ibit;
				ibit >>= 1;
			}
			i2rev += ibit;
		}
		ifp1=ip1; //Here begins the Danielson-Lanczos section of the routine. 
		while (ifp1 < ip2) {
			ifp2=ifp1 << 1;
			theta=isign*6.28318530717959/(ifp2/ip1); //Initialize for the trig. recurrence.
			wtemp=sin(0.5*theta);
			wpr = -2.0*wtemp*wtemp;
			wpi=sin(theta);
			wr=1.0;
			wi=0.0;
			for (i3=1;i3<=ifp1;i3+=ip1) {
				for (i1=i3;i1<=i3+ip1-2;i1+=2) {
					for (i2=i1;i2<=ip3;i2+=ifp2) {
						k1=i2; //Danielson-Lanczos formula:
						k2=k1+ifp1;
						tempr=(float)wr*data(k2)-(float)wi*data(k2+1);
						tempi=(float)wr*data(k2+1)+(float)wi*data(k2);
						data(k2)=data(k1)-tempr;
						data(k2+1)=data(k1+1)-tempi;
						data(k1) += tempr;
						data(k1+1) += tempi;
					}
				}
				wr=(wtemp=wr)*wpr-wi*wpi+wr; //Trigonometric recurrence.
				wi=wi*wpr+wtemp*wpi+wi;
			}
			ifp1=ifp2;
		}
		nprev *= n;
	}
}

void rlft3(TNT::Fortran_Array3D<double>& data, TNT::Fortran_Array2D<double>& speq, unsigned long nn1, unsigned long nn2,
unsigned long nn3, int isign)
/*
Given a three-dimensional real array data[1..nn1][1..nn2][1..nn3] (where nn1 = 1 for
the case of a logically two-dimensional array), this routine returns (for isign=1) the complex
fast Fourier transform as two complex arrays: On output, data contains the zero and positive
frequency values of the third frequency component, while speq[1..nn1][1..2*nn2] contains
the Nyquist critical frequency values of the third frequency component. First (and second)
frequency components are stored for zero, positive, and negative frequencies, in standard wraparound
order. See text for description of how complex values are arranged. For isign=-1, the
inverse transform (times nn1*nn2*nn3/2 as a constant multiplicative factor) is performed,
with output data (viewed as a real array) deriving from input data (viewed as complex) and
speq. For inverse transforms on data not generated .rst by a forward transform, make sure
the complex input data array satis.es property (12.5.2). The dimensions nn1, nn2, nn3 must
always be integer powers of 2.
*/
{
	unsigned long i1,i2,i3,j1,j2,j3,ii3;
	double theta,wi,wpi,wpr,wr,wtemp;
	double c1,c2,h1r,h1i,h2r,h2i;
	TNT::Fortran_Array1D<unsigned long> nn(3);
	TNT::Fortran_Array1D<double> data1D(nn1*nn2*nn3,data.getdata());
	//if (1+&data(nn1,nn2,nn3)-&data(1,1,1) != nn1*nn2*nn3) return;
		//nrerror("rlft3: problem with dimensions or contiguity of data array\n");
	c1=0.5;
	c2 = -0.5*isign;
	theta=isign*(6.28318530717959/nn3);
	wtemp=sin(0.5*theta);
	wpr = -2.0*wtemp*wtemp;
	wpi=sin(theta);
	nn(1)=nn1;
	nn(2)=nn2;
	nn(3)=nn3 >> 1;
	if (isign == 1) { //Case of forward transform.
		fourn(data1D,nn,3,isign); //Here is where most all of the compute time is spent. 
		for (i1=1;i1<=nn1;i1++)
			for (i2=1,j2=0;i2<=nn2;i2++) { //Extend data periodically into speq.
				speq(i1,++j2)=data(i1,i2,1);
				speq(i1,++j2)=data(i1,i2,2);
			}
	}
	for (i1=1;i1<=nn1;i1++) {
		j1=(i1 != 1 ? nn1-i1+2 : 1);
		//Zero frequency is its own reflection, otherwise locate corresponding negative frequency
		//in wrap-around order.
		wr=1.0; //Initialize trigonometric recurrence.
		wi=0.0;
		for (ii3=1,i3=1;i3<=(nn3>>2)+1;i3++,ii3+=2) {
			for (i2=1;i2<=nn2;i2++) {
				if (i3 == 1) { //Equation (12.3.5).
					j2=(i2 != 1 ? ((nn2-i2)<<1)+3 : 1);
					h1r=c1*(data(i1,i2,1)+speq(j1,j2));
					h1i=c1*(data(i1,i2,2)-speq(j1,j2+1));
					h2i=c2*(data(i1,i2,1)-speq(j1,j2));
					h2r= -c2*(data(i1,i2,2)+speq(j1,j2+1));
					data(i1,i2,1)=h1r+h2r;
					data(i1,i2,2)=h1i+h2i;
					speq(j1,j2)=h1r-h2r;
					speq(j1,j2+1)=h2i-h1i;
				} else {
					j2=(i2 != 1 ? nn2-i2+2 : 1);
					j3=nn3+3-(i3<<1);
					h1r=c1*(data(i1,i2,ii3)+data(j1,j2,j3));
					h1i=c1*(data(i1,i2,ii3+1)-data(j1,j2,j3+1));
					h2i=c2*(data(i1,i2,ii3)-data(j1,j2,j3));
					h2r= -c2*(data(i1,i2,ii3+1)+data(j1,j2,j3+1));
					data(i1,i2,ii3)=h1r+wr*h2r-wi*h2i;
					data(i1,i2,ii3+1)=h1i+wr*h2i+wi*h2r;
					data(j1,j2,j3)=h1r-wr*h2r+wi*h2i;
					data(j1,j2,j3+1)= -h1i+wr*h2i+wi*h2r;
				}
			}
			wr=(wtemp=wr)*wpr-wi*wpi+wr; //Do the recurrence.
			wi=wi*wpr+wtemp*wpi+wi;
		}
	}
	if (isign == -1) //Case of reverse transform.
		fourn(data1D,nn,3,isign);
}

void imfft(const CArray2D& Image, CComplexArray2D& FFT)
{
	int N1 = Image.dim1(), N2 = Image.dim2();
	int i,j;

	TNT::Fortran_Array3D<double> fImage(1,N1,N2);
	TNT::Fortran_Array2D<double> speq(1,2*N1);

	for(i=0; i<N1; i++) 
		for(j=0; j<N2; j++)
			fImage(1,i+1,j+1) = Image[i][j];

	rlft3(fImage,speq,1,N1,N2,1);

	FFT = TNT::Array2D<cdouble>(N1,N2/2+1);

	for(i=0; i<N1; i++) 
		for(j=0; j<N2/2; j++)
			FFT[i][j] = cdouble(fImage(1,i+1,2*j+1),fImage(1,i+1,2*j+2));

	for(i=0; i<N1; i++) 
		FFT[i][N2/2] = cdouble(speq(1,2*i+1),speq(1,2*i+2));
}