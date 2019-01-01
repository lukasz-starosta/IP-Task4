#include "FrequencyProcesser.h"

#define _USE_MATH_DEFINES

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
oneDimensionalDFTByColumn(std::complex<double> **fourierMatrix, std::complex<double> **finalMatrix, int height,
                          int width)
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

cimg_library::CImg<unsigned char> FrequencyProcesser::getFourierVisualisation()
{
    cimg_library::CImg<unsigned char> visualisation(width, height, 1, 3, 0);

    // In the final matrix, the DC component at [0][0] has the highest value
    double max = std::abs(finalMatrix[0][0]);
    unsigned char value1, value2, value3, value4;

    // Normalize the points to an interval <0, 255> so that they can be displayed and append them so that the [0][0] value is in the center
    for (int y = 0; y < height / 2; y++)
    {
        for (int x = 0; x < width / 2; x++)
        {
            value1 = (unsigned char) ((std::abs(finalMatrix[height / 2 + y][width / 2 + x]) / max) * 255);
            value2 = (unsigned char) ((std::abs(finalMatrix[y][x]) / max) * 255);
            value3 = (unsigned char) ((std::abs(finalMatrix[height / 2 + y][x]) / max) * 255);
            value4 = (unsigned char) ((std::abs(finalMatrix[y][width / 2 + x]) / max) * 255);

            //Assigns the values of the pixels to the image while swapping the places of quadrants (so that the DC is in the middle)
            for (int i = 0; i < 3; i++)
            {
                // top-left is bottom-right
                visualisation(x, y, i) = value1;
                // bottom-right is top-left
                visualisation(width / 2 + x, height / 2 + y, i) = value2;
                // top-right is bottom - left
                visualisation(width / 2 + x, y, i) = value3;
                // bottom-left is top-right
                visualisation(x, height / 2 + y, i) = value4;
            }
        }
    }

    return visualisation;
}

cimg_library::CImg<unsigned char> FrequencyProcesser::getFourierLogarithmicVisualisation()
{
    cimg_library::CImg<unsigned char> visualisation(width, height, 1, 3, 0);

    // In the final matrix, the DC component at [0][0] has the highest value
    unsigned char value1, value2, value3, value4;

    // Normalize the points to an interval <0, 255> so that they can be displayed and append them so that the [0][0] value is in the center
    for (int y = 0; y < height / 2; y++)
    {
        for (int x = 0; x < width / 2; x++)
        {
            //Transform to a logarithmic scale
            value1 = (unsigned char) std::log(std::abs(finalMatrix[height / 2 + y][width / 2 + x]) + 1);
            value2 = (unsigned char) std::log(std::abs(finalMatrix[y][x]) + 1);
            value3 = (unsigned char) std::log(std::abs(finalMatrix[height / 2 + y][x]) + 1);
            value4 = (unsigned char) std::log(std::abs(finalMatrix[y][width / 2 + x]) + 1);
            //Assigns the values of the pixels to the image while swapping the places of quadrants (so that the DC is in the middle)
            for (int i = 0; i < 3; i++)
            {
                // top-left is bottom-right
                visualisation(x, y, i) = value1;
                // bottom-right is top-left
                visualisation(width / 2 + x, height / 2 + y, i) = value2;
                // top-right is bottom - left
                visualisation(width / 2 + x, y, i) = value3;
                // bottom-left is top-right
                visualisation(x, height / 2 + y, i) = value4;
            }
        }
    }

    return visualisation;
}


