#include "FrequencyProcesser.h"

void FrequencyProcesser::highPassFilter(double distanceFromOrigin)
{
    double halfHeight = height / 2, halfWidth = width / 2, distance;
    for (int row = 0; row < height; row++)
    {
        for (int column = 0; column < width; column++)
        {
            distance = std::sqrt(std::pow(row - halfHeight, 2) + std::pow(column - halfWidth, 2));
            if (distance </ distanceFromOrigin)
            {
                visualisationMatrix[row][column].real(0);
                visualisationMatrix[row][column].imag(0);
            }
        }
    }
}