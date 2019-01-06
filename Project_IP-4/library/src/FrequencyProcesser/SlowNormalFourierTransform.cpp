#include "FrequencyProcesser.h"

#define _USE_MATH_DEFINES
#define M_PI 3.14159265358979323846
#include <math.h>
#include <FrequencyProcesser.h>


//PLEASE SEE the instructions if in doubt about the variable naming
// Performs a DFT on the pixels obtained from the image (only the first channel)
void oneDimensionalDFTByRow(cimg_library::CImg<unsigned char> *image, std::complex<double> **fourierMatrix, int height,
                            int width)
{
    cimg_library::CImg<unsigned char> rowData;
    std::complex<double> sum(0.0, 0.0);

    for (int row = 0; row < height; row++)
    {
        rowData = image->get_row(row);
        for (int k = 0; k < width; k++)
        {
            for (int n = 0; n < width; n++)
            {
                std::complex<double> W(cos(2 * M_PI * n * k / width), -sin(2 * M_PI * n * k / width));
                sum += (double) rowData[n] * W;
            }
            fourierMatrix[row][k] = sum;
            sum.real(0.0);
            sum.imag(0.0);
        }
    }
}

//PLEASE SEE the instructions if in doubt about the variable naming
// Performs a DFT from the previously obtained fourier matrix, yields another matrix with complex numbers
void
oneDimensionalDFTByColumn(std::complex<double> **fourierMatrix, std::complex<double> **finalMatrix, int height, int width)
{
    std::complex<double> sum(0.0, 0.0);

    for (int column = 0; column < width; column++)
    {
        for (int k = 0; k < height; k++)
        {
            for (int n = 0; n < height; n++)
            {
                std::complex<double> W(cos(2 * M_PI * n * k / height), -sin(2 * M_PI * n * k / height));
                sum += fourierMatrix[n][column] * W;
            }
            finalMatrix[k][column] = sum;
            sum.real(0.0);
            sum.imag(0.0);
        }
    }
}

void FrequencyProcesser::slowNormalDFT()
{
    // Perform the DFT based on the image, yield 'firstMatrix'
    oneDimensionalDFTByRow(&image, firstMatrix, height, width);
    // Perform the DFT based on the 'firstMatrix', yield 'finalMatrix'
    oneDimensionalDFTByColumn(firstMatrix, finalMatrix, height, width);
}

