#include "FrequencyProcesser.h"

void FrequencyProcesser::lowPassFilter(double maxFrequencyPercentage)
{
    double maxFrequency = std::abs(finalMatrix[0][0]);
    for (int row = 0; row < height; row++)
    {
        for (int column = 0; column < width; column++)
        {
            double percentageRatio = (std::abs(finalMatrix[row][column]) / maxFrequency) * 100;
            if (percentageRatio < maxFrequencyPercentage) finalMatrix[row][column] = 0;
        }
    }
}