#define _USE_MATH_DEFINES

#include <math.h>
#include "FrequencyProcesser.h"


//PLEASE SEE the instructions if in doubt about the variable naming
// Performs a DFT from the previously obtained fourier matrix, yields another matrix with complex numbers
void
oneDimensionalInverseDFTByColumn(std::complex<double> **finalMatrix, std::complex<double> **fourierMatrix, int height,
                                 int width)
{
    std::complex<double> sum(0.0, 0.0);

    for (int column = 0; column < width; column++)
    {
        for (int k = 0; k < height; k++)
        {
            for (int n = 0; n < height; n++)
            {
                std::complex<double> W(cos(2 * M_PI * n * k / height), sin(2 * M_PI * n * k / height));
                sum += finalMatrix[n][column] * W;
            }
            fourierMatrix[k][column] = sum / (double)height;
            sum.real(0.0);
            sum.imag(0.0);
        }
    }
}

//PLEASE SEE the instructions if in doubt about the variable naming
// Performs a DFT on the pixels obtained from the image (only the first channel)
void oneDimensionalInverseDFTByRow(std::complex<double> **fourierMatrix, cimg_library::CImg<unsigned char> *image,
                                   int height,
                                   int width)
{
    std::complex<double> sum(0.0, 0.0);

    for (int row = 0; row < height; row++)
    {
        for (int k = 0; k < width; k++)
        {
            for (int n = 0; n < width; n++)
            {
                std::complex<double> W(cos(2 * M_PI * n * k / width), sin(2 * M_PI * n * k / width));
                sum += fourierMatrix[row][n] * W;
            }
            for (int channel = 0; channel < 3; channel++)
            {
                (*image)(k, row, channel) = (unsigned char)(std::abs(sum) / (double)width);
            }
            sum.real(0.0);
            sum.imag(0.0);
        }
    }
}


void FrequencyProcesser::slowInverseDFT()
{
    oneDimensionalInverseDFTByColumn(finalMatrix, firstMatrix, height, width);
    oneDimensionalInverseDFTByRow(firstMatrix, &image, height, width);
}

