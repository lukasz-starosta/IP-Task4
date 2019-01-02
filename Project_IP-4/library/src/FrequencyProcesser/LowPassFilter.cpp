#include "FrequencyProcesser.h"
#include <iostream>

void FrequencyProcesser::lowPassFilter(double maxFrequencyPercentage)
{
    double maxFrequency = std::abs(finalMatrix[0][0]);
    double cutOffFrequency = maxFrequency * maxFrequencyPercentage / 100;
    for (int row = 0; row < height; row++)
    {
        for (int column = 0; column < width; column++)
        {
            if (std::abs(visualisationMatrix[row][column]) > cutOffFrequency)
            {
                std::cout << row << " " << column << " " << "zeroed" << std::endl;
                visualisationMatrix[row][column].real(0);
                visualisationMatrix[row][column].imag(0);
            }
        }
    }
}