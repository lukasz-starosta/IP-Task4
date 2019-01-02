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
            if (std::abs(finalMatrix[row][column]) > cutOffFrequency) {
                std::cout<<row<<" " <<column<<" " << "zeroed"<<std::endl;
                    finalMatrix[row][column].real(0);
                    finalMatrix[row][column].imag(0);
            }
        }
    }
}