#include "cppmath.h"

#include <stdio.h>
#include <string.h>

#include <fstream>

struct bmp_header {
  unsigned short dum;  // for word allinment
  unsigned char c1,c2;  // file id
  int FileLength,
      Dum, // reserved
      DOffset, // offset from beginning of file to first data byte
      bmInfoL, // length of bitmap information header
      W,H; // image width and height
  unsigned short CP,Bits; //Colorplanes, Bits per pixel
  int Comp, CompSize, // compression type, compressed image size
      HorizR, VertR, // horizontal & vertical resolution (pix/meter)
      UCol,ICol; // used colors, important colors
};

#define WIDTHBYTES(bits) ((unsigned int)(((bits)+31) & (~31)) / 8)

void bmpwrite(const CArray2D& A, const char *bmpfile)
{
	int H = A.dim1();
	int W = A.dim2();
	int Stride = WIDTHBYTES((unsigned int)W*8);
	int i,j;

	double amax = A[0][0], amin = amax;
	for(i=0; i<H; i++)
		for(j=0; j<W; j++) {
			if (amax<A[i][j]) amax=A[i][j];
			if (amin>A[i][j]) amin=A[i][j];
		}
		
	TNT::Array2D<char> I(H,Stride,(char)0);
	double dd = 255./(amax-amin);
	for(i=0; i<H; i++)
		for(j=0; j<W; j++)
			I[i][j] = (char)((A[H-i-1][j] - amin)*dd);

// *** BMP HEADER ***

  bmp_header myheader={0,'B','M'};

// write file length (bytes)
  myheader.FileLength=54+4*256+Stride*H;
  myheader.DOffset=54+4*256;
  myheader.bmInfoL=40;
  myheader.W=W;
  myheader.H=H;
  myheader.CP=1;
  myheader.Bits=8;

  ofstream bmp(bmpfile, ios::out | ios::binary );

  bmp.write((char *)&myheader.c1,54);

// *** colormap ***
  unsigned char map[256][4];

  for (i=0; i<256; i++) {
    map[i][0]=i;
	map[i][1]=map[i][0];
	map[i][2]=map[i][0];
	map[i][3]=0;
  }
  bmp.write((char *)map,1024);

// *** BITMAP DATA ***

  bmp.write(I.getdata(),Stride*H);

  bmp.close();
}

void bmpread(const char *bmpfile, CArray2D& A)
{
	ifstream bmp(bmpfile, ios::in | ios::binary );
	
	bmp_header myheader;  
	bmp.read((char *)&myheader.c1,54);
	bmp.ignore(4*256);
	
	int H = myheader.H;
	int W = myheader.W;
	int Stride = WIDTHBYTES((unsigned int)W*8);
	int i,j;
	
	TNT::Array2D<char> I(H,Stride,(char)0);
	
	bmp.read(I.getdata(),Stride*H);
	
    bmp.close();
	
	A = CArray2D(H,W);
	for(i=0; i<H; i++)
		for(j=0; j<W; j++)
			A[H-i-1][j] = I[i][j];		
}