/*
Name: Emery Morales
20 March 2019
Class: Computer Science 142-01
Project 1: It's All a Blur
Pledge: I have neither given nor received unauthorized aid on this program.
Description: Program that performs simple kernel based image processing filters
						 on an image.
*/

#include <iostream>
#include <cmath>
#include <cstring>
#include <iomanip>
#include <cstdlib>
#include "bmplib.h"

using namespace std;

//============================Function Prototypes======================
void dummy(unsigned char out[][SIZE][RGB], unsigned char in[][SIZE][RGB]);
void convolve(unsigned char out[][SIZE][RGB], unsigned char in[][SIZE][RGB],
			  int N, double kernel[][11]);
void sobel(unsigned char out[][SIZE][RGB], unsigned char in[][SIZE][RGB]);
void gaussian_filter(unsigned char out[][SIZE][RGB],
	                 unsigned char in[][SIZE][RGB], int N, double sigma);
void unsharp(unsigned char out[][SIZE][RGB], unsigned char in[][SIZE][RGB],
             int N, double sigma, double alpha);

//============================Do not change code in main()======================
int main(int argc, char* argv[])
{
	//First check argc
	if(argc < 3)
	{
		//we need at least ./filter <input file> <filter name> to continue
		cout << "usage: ./filter <input file> <filter name> <filter parameters>";
		cout << " <output file name>" << endl;
		return -1;
	}
	//then check to see if we can open the input file
	unsigned char input[SIZE][SIZE][RGB];
	unsigned char output[SIZE][SIZE][RGB];
	char* outfile;
	int N;
	double sigma, alpha;
	double kernel[11][11];

	// read file contents into input array
	int status = readRGBBMP(argv[1], input);
	if(status != 0)
	{
		cout << "unable to open " << argv[1] << " for input." << endl;
		return -1;
	}
	//Input file is good, now look at next argument
	if( strcmp("sobel", argv[2]) == 0)
	{
		sobel(output, input);
		outfile = argv[3];
	}
	else if( strcmp("blur", argv[2]) == 0)
	{
		if(argc < 6)
		{
			cout << "not enough arguments for blur." << endl;
			return -1;
		}
		N = atoi(argv[3]);
		sigma = atof(argv[4]);
		outfile = argv[5];
		gaussian_filter(output, input, N, sigma);
	}
	else if( strcmp("unsharp", argv[2]) == 0)
	{
		if(argc < 7)
		{
			cout << "not enough arguments for unsharp." << endl;
			return -1;
		}
		N = atoi(argv[3]);
		sigma = atof(argv[4]);
		alpha = atof(argv[5]);
		outfile = argv[6];
		unsharp(output, input, N, sigma, alpha);

	}
	else if( strcmp("dummy", argv[2]) == 0)
	{
		//do dummy
		dummy(output, input);
		outfile = argv[3];
	}
	else
	{
		cout << "unknown filter type." << endl;
		return -1;
	}

	if(writeRGBBMP(outfile, output) != 0)
	{
		cout << "error writing file " << argv[3] << endl;
	}

}
//=========================End Do not change code in main()=====================

// Creates an identity kernel (dummy kernel) that will simply
// copy input to output image and applies it via convolve()
//
// ** This function is complete and need not be changed.
// Use this as an example of how to create a kernel array, fill it in
// appropriately and then use it in a call to convolve.
void dummy(unsigned char out[][SIZE][RGB], unsigned char in[][SIZE][RGB]) {
	double k[11][11];
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			k[i][j] = 0;
		}
	}
	k[1][1] = 1;
	convolve(out, in, 3, k);
}


// Convolves an input image with an NxN kernel to produce the output kernel
// You will need to complete this function by following the
// instructions in the comments
void convolve(unsigned char out[][SIZE][RGB], unsigned char in[][SIZE][RGB],
              int N, double kernel[][11]) {

	// Use for input image with appropriate padding
	int padded[SIZE+10][SIZE+10][RGB];
	// Use for the unclamped output pixel values, then copy from temp to out,
	// applying clamping
	int temp[SIZE][SIZE][RGB];

	// first set all of padded to 0 (black)
	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < SIZE + 10; j++) {
			for (int k = 0; k < 3; k++) {
				//top 5 rows
				padded[i][j][k] = 0;
				// bottom 5 rows
				padded[SIZE + 10 - 1 - i][j][k] = 0;
				// left 5 columns
				padded[j][i][k] = 0;
				// right 5 columns
				padded[j][SIZE + 10 - 1 - i][k] = 0;
			}
		}
	}

	// now copy input into padding to appropriate locations
	int upperLeft = 5;
	for (int i = 0; i < SIZE; i++) {
		for (int j = 0; j < SIZE; j++) {
			for (int k = 0; k < 3; k++) {
				padded[upperLeft+i][upperLeft+j][k] = in[i][j][k];
			}
		}
	}

	//initialize temp pixels to 0 (black)
	for (int i = 0; i < SIZE; i++) {
		for (int j = 0; j < SIZE; j++) {
			for (int k = 0; k < 3; k++) {
				temp[i][j][k] = 0;
			}
		}
	}

	// now perform convolve (using convolution equation on each pixel of the
	// actual image) placing the results in temp (i.e. unclamped result)
	// Here we give you the structure of the convolve for-loops, you need
	// to figure out the loop limits
	//for(int y= ;y< ;y++)
	//  for(int x= ;x< ;x++)
	//    for(int k=0;k<RGB;k++)
	//       for(int i= ; i<= ; i++)
	//          for(int j= ; j<= ; j++)
	//                temp[][][k] += padded[][][k]*kernel[][];
	for (int y = 0; y < SIZE; y++) {
		for (int x = 0; x < SIZE; x++) {
			for (int k = 0; k < RGB; k++) {
				for (int i = 0; i < N; i++) {
					for (int j = 0; j < N; j++) {
						temp[y][x][k] +=
						padded[upperLeft+y+i-N/2][upperLeft+x+j-N/2][k]*
						kernel[i][j];
					}
				}
			}
		}
	}

	// now clamp and copy to output
	// You may need to cast to avoid warnings from the compiler:
	// (i.e. out[i][j][k] = (unsigned char) temp[i][j][k];)
	for (int i = 0; i < SIZE; i++) {
		for (int j = 0; j < SIZE; j++) {
			for (int k = 0; k < 3; k++) {
        //clamping
        int t = temp[i][j][k];
        if (t > 255) {
          t = 255;
        }
        else if (t < 0) {
          t = 0;
        }
				out[i][j][k] = (unsigned char) t;
			}
		}
	}

}

// You will need to complete this function by following the
// instructions in the comments
// Sobel operator for edge detection (using a fixed sized kernel).
void sobel(unsigned char out[][SIZE][RGB], unsigned char in[][SIZE][RGB]) {
	double k[11][11];
	double s_h1[3][3] = { {-1, 0, 1},
	{-2, 0, 2},
	{-1, 0, 1} };
	double s_h2[3][3] = { {1, 0, -1},
	{2, 0, -2},
	{1, 0, -1} };

	unsigned char h1_sobel[SIZE][SIZE][RGB]; //hold intermediate images
	unsigned char h2_sobel[SIZE][SIZE][RGB];

	for (int i = 0; i < 11; i++) {
		for (int j=0; j < 11; j++) {
			k[i][j] = 0;
		}
	}


	// Copy in 1st 3x3 horizontal sobel kernel (i.e. copy s_h1 into k)
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++) {
      k[i][j] = s_h1[i][j];
    }
  }


	// Call convolve to apply horizontal sobel kernel with result in h1_soble
  convolve(h1_sobel, in, 3, k);


	// Copy in 2nd 3x3 horizontal sobel kernel (i.e. copy s_h2 into k)
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++) {
      k[i][j] = s_h2[i][j];
    }
  }


	// Call convolve to apply horizontal sobel kernel with result in h2_soble
  convolve(h2_sobel, in, 3, k);


	// Add the two results (applying clamping) to produce the final output
  for (int i = 0; i < SIZE; i++) {
		for (int j = 0; j < SIZE; j++) {
			for (int k = 0; k < RGB; k++) {

				int t = h1_sobel[i][j][k] + h2_sobel[i][j][k];

        if (t > 255) {
          t = 255;
        }
        else if (t < 0) {
          t = 0;
        }

        out[i][j][k] = (unsigned char) t;

      }
    }
  }

}

// Add the rest of your functions here (i.e. gaussian, gaussian_filter, unsharp)

// Gaussian blur filter for low-pass filtering.
void gaussian_filter (unsigned char out[][SIZE][RGB], unsigned char in[][SIZE][RGB],
	                 int N, double sigma) {

double k[11][11];
double kSum = 0;

// Generating 11 * 11 Gaussian kernel:
// 	set A=1, and set the center point of the kernel as x0=0, y0=0. The variance,
// 	signma , is a parameter that can be used to adjust the size of the central
// 	peakr. The x,y values of the other kernel cells are set as offsets from the
//	center.
for (int i = 0; i < N; i++) {
	for (int j = 0; j < N; j++) {
		int x = i - (N/2);
		int y = j - (N/2);
		double xTerm = (((x) * (x)) / (2 * sigma * sigma));
		double yTerm = (((y) * (y)) / (2 * sigma * sigma));
		k[i][j] = exp (-1.0 * (xTerm + yTerm));

		cout << k[i][j] << " ";

	}

	cout << endl;

}

// Finding the Ksum
for (int i = 0; i < N; i++) {
	for (int j = 0; j < N; j++) {
		 kSum = (kSum + k[i][j]);
		}
 }

// Normalizing the array so brightness does not change.
for (int i = 0; i < N ; i++) {
		for (int j = 0; j < N; j++) {
				k[i][j] /= kSum;
		}
}

convolve(out, in, N, k);

}

// Unsharp mask filter for sharpening an image (which uses the Gaussian blur
// 	filter).
void unsharp (unsigned char out[][SIZE][RGB], unsigned char in[][SIZE][RGB],
        	 int N, double sigma, double alpha) {

unsigned char blur [SIZE][SIZE][RGB];
unsigned char D_map [SIZE][SIZE][RGB];

// B = blur(IM)
// Creating a blurred version B by applying the Gaussian blur function blur().
gaussian_filter(blur, in, N, sigma);

// D = IM - B
// Creating a detail map D by subtracting B from IM.
for (int i = 0; i < SIZE; i++) {
	for (int j = 0; j < SIZE; j++) {
		for (int k = 0; k < RGB; k++) {

			int t = in[i][j][k] - blur[i][j][k];

			// Clamping
			if (t > 255) {
				t = 255;
			}
			else if (t < 0) {
				t = 0;
			}

			D_map[i][j][k] = (unsigned char) t;

		}
	}
}

// S = IM + (D)alpha
// Adding some fraction of D back to IM (again this is a matrix operation
// 	where we perform the following on each pixel).
for (int i = 0; i < SIZE; i++) {
	for (int j = 0; j < SIZE; j++) {
		for (int k = 0; k < RGB; k++) {

			int t = in[i][j][k] + D_map[i][j][k] * alpha;

			// Clamping.
			if (t > 255) {
				t = 255;
			}
			else if (t < 0) {
				t = 0;
			}

			out[i][j][k] = (unsigned char) t;

		}
	}
}

}
