#include "FrequencyProcesser.h"

#define _USE_MATH_DEFINES
#include <math.h>

/* 1-D DFT
**  create a function which takes a row/column (array of real (1) /complex numbers (2)), the X in X-Point, performs a DFT and returns the resultant array
*/

void oneDimensionalDFTByRow(unsigned char imageRow[], std::complex<double> matrixRow[], int width)
{
    double real = 0, imag = 0;
    // for each k (pixel in a row), sum all the real and imaginary parts separately, and assign them at the end to the pixel at matrixRow[k]
    for (int k = 0; k < width; k++)
    {
        for (int n = 0; n < width; n++)
        {
            real += imageRow[n] * cos(2 * M_PI * n * k / width);
            imag -= imageRow[n] * sin(2 * M_PI * n * k / width);
        }
        matrixRow[k].real(real);
        matrixRow[k].imag(imag);
        real = 0;
        imag = 0;
    }
}

void
oneDimensionalDFTByColumn(std::complex<double> **fourierMatrix, std::complex<double> **resultantMatrix, int height,
                          int width)
{
    std::complex<double> p(0.0, 0.0);

    for (int column = 0; column < width; column++)
    {
        for (int k = 0; k < height; k++)
        {
            for (int n = 0; n < height; n++)
            {
                std::complex<double> c(cos(2 * M_PI * n * k / height), -sin(2 * M_PI * n * k / height));
                p += fourierMatrix[n][column] * c;
            }
            resultantMatrix[k][column] = p;
            p.real(0.0);
            p.imag(0.0);
        }
    }
}

void FrequencyProcesser::slowNormalDFT()
{
    cimg_library::CImg<unsigned char> imageCopy = image;

    //initialize matrix of the same size as the image
    auto **matrix = new std::complex<double> *[height];
    for (int row = 0; row < height; row++)
    {
        matrix[row] = new std::complex<double>[width];
    }

    auto **resultantMatrix = new std::complex<double> *[height];
    for (int row = 0; row < height; row++)
    {
        resultantMatrix[row] = new std::complex<double>[width];
    }

    //iterate through rows of the copy, for each row perform a discrete fourier transform
    for (int row = 0; row < height; row++)
    {
        oneDimensionalDFTByRow(imageCopy.get_row(row), matrix[row], width);
    }

    oneDimensionalDFTByColumn(matrix, resultantMatrix, height, width);

    //cleanup
    for (int i = 0; i < height; i++)
    {
        delete[] matrix[i];
        delete[] resultantMatrix[i];
    }
    delete[] matrix;
    delete[] resultantMatrix;

    image = imageCopy;
}