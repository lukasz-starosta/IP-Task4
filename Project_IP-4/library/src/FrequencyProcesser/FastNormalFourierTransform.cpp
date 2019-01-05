#include"FrequencyProcesser.h"
#include <math.h>
#include <iostream>

#define M_PI 3.14159265358979323846 

void oneDimensionalFFTByRow(cimg_library::CImg<unsigned char> *image, std::complex<double> **fourierMatrix, int height, int width)
{
	cimg_library::CImg<unsigned char> rowData;
	std::complex<double> evenSum(0.0, 0.0);
	std::complex<double> oddSum(0.0, 0.0);
	
	for (int row = 0; row < height; row++)
	{
		rowData = image->get_row(row);
		for (int k = 0; k < (double)width / 2.0; k++)
		{
			for (int n = 0; n < (double)width / 2.0; n++)
			{
				std::complex<double> W(cos(2.0 * M_PI * n * k / ((double)width / 2.0)), -sin(2.0 * M_PI * n * k / ((double)width / 2.0)));

				//For even numbers
				evenSum += (double)rowData[2 * n] * W;

				//For odd numbers
				oddSum += (double)rowData[2 * n + 1] * W;

			}
			std::complex<double> R(cos(2.0 * M_PI * k / (double)width), -sin(2.0 * M_PI * k / (double)width));
			
			fourierMatrix[row][k] = evenSum + (oddSum * R);
			fourierMatrix[row][(width / 2) + k] = evenSum - (oddSum * R);
			evenSum.real(0.0);
			evenSum.imag(0.0);
			oddSum.real(0.0);
			oddSum.imag(0.0);
		}
	}
}


void oneDimensionalFFTByColumn(std::complex<double> **fourierMatrix, std::complex<double> **finalMatrix, int height, int width)
{
	std::complex<double> evenSum(0.0, 0.0);
	std::complex<double> oddSum(0.0, 0.0);
	
	for (int column = 0; column < width; column++)
	{
		for (int k = 0; k < (double)height / 2.0; k++)
		{
			for (int n = 0; n < (double)height / 2.0; n++)
			{
				std::complex<double> W(cos(2.0 * M_PI * n * k / ((double)height / 2.0)), -sin(2.0 * M_PI * n * k / ((double)height / 2.0)));

				//For even numbers
				evenSum += fourierMatrix[2 * n][column] * W;

				//For odd numbers
				oddSum += fourierMatrix[2 * n + 1][column] * W;
				//std::cout << oddSum << std::endl;
			}
			std::complex<double> R(cos(2.0 * M_PI * k / (double)width), -sin(2.0 * M_PI * k / (double)width));

			finalMatrix[k][column] = evenSum + (oddSum * R);
			finalMatrix[(height / 2) + k][column] = evenSum - (oddSum * R);

			evenSum.real(0.0);
			evenSum.imag(0.0);
			oddSum.real(0.0);
			oddSum.imag(0.0);
		}
	}
}

void FrequencyProcesser::NormalFFT()
{
	// Perform the DFT based on the image, yield 'firstMatrix'
	oneDimensionalFFTByRow(&image, firstMatrix, height, width);
	// Perform the DFT based on the 'firstMatrix', yield 'finalMatrix'
	oneDimensionalFFTByColumn(firstMatrix, finalMatrix, height, width);
}