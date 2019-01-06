#define M_PI 3.14159265358979323846

#include <math.h>
#include "FrequencyProcesser.h"

void
oneDimensionalInverseFFTByColumn(std::complex<double> **finalMatrix, std::complex<double> **fourierMatrix, int height,
	int width)
{
	std::complex<double> evenSum(0.0, 0.0);
	std::complex<double> oddSum(0.0, 0.0);

	for (int column = 0; column < width; column++)
	{
		for (int k = 0; k < (double)height / 2.0; k++)
		{
			for (int n = 0; n < (double)height / 2.0; n++)
			{
				std::complex<double> W(cos(2.0 * M_PI * n * k / ((double)height / 2.0)), sin(2.0 * M_PI * n * k / ((double)height / 2.0)));
				
				//For even numbers
				evenSum += finalMatrix[2 * n][column] * W;

				//For odd numbers
				oddSum += finalMatrix[2 * n + 1][column] * W;
			}
			std::complex<double> R(cos(2.0 * M_PI * k / (double)width), sin(2.0 * M_PI * k / (double)width));
			
			fourierMatrix[k][column] = (evenSum + (oddSum * R)) / (double)height;
			fourierMatrix[(height / 2) + k][column] = (evenSum - (oddSum * R)) / (double)height;
			
			evenSum.real(0.0);
			evenSum.imag(0.0);
			oddSum.real(0.0);
			oddSum.imag(0.0);
		}
	}
}


void oneDimensionalInverseFFTByRow(std::complex<double> **fourierMatrix, cimg_library::CImg<unsigned char> *image,
	int height,
	int width)
{
	std::complex<double> evenSum(0.0, 0.0);
	std::complex<double> oddSum(0.0, 0.0);

	for (int row = 0; row < height; row++)
	{
		for (int k = 0; k < (double)width / 2.0; k++)
		{
			for (int n = 0; n < (double)width / 2.0; n++)
			{
				std::complex<double> W(cos(2.0* M_PI * n * k / ((double)width / 2.0)), sin(2.0 * M_PI * n * k / ((double)width / 2.0)));

				//For even numbers
				evenSum += fourierMatrix[row][2 * n] * W;

				//For odd numbers
				oddSum += fourierMatrix[row][2 * n + 1] * W;
			}
			std::complex<double> R(cos(2.0 * M_PI * k / (double)width), sin(2.0 * M_PI * k / (double)width));

			double evenPixelValue = (std::abs((evenSum + (oddSum * R)) / (double)width)) > 255 ? (unsigned char)255 : (unsigned char)(std::abs(evenSum + (oddSum * R)) / (double)width);
			double oddPixelValue = (std::abs((evenSum - (oddSum * R)) / (double)width)) > 255 ? (unsigned char)255 : (unsigned char)(std::abs(evenSum - (oddSum * R)) / (double)width);
			
			for (int channel = 0; channel < 3; channel++)
			{
				(*image)(k, row, channel) = (unsigned char)evenPixelValue;
				(*image)((width / 2) + k, row, channel) = (unsigned char)oddPixelValue;
			}

			evenSum.real(0.0);
			evenSum.imag(0.0);
			oddSum.real(0.0);
			oddSum.imag(0.0);
		}
	}
}


void FrequencyProcesser::InverseFFT()
{
	oneDimensionalInverseFFTByColumn(finalMatrix, firstMatrix, height, width);
	oneDimensionalInverseFFTByRow(firstMatrix, &image, height, width);
}

