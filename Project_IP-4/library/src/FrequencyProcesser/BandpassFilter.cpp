#include "FrequencyProcesser.h"

void FrequencyProcesser::bandPassFilter(double bandwidth, double cutoff)
{
	double halfHeight = height / 2, halfWidth = width / 2, distance;
	for (int row = 0; row < width; row++)
	{
		for (int column = 0; column < height; column++)
		{
			distance = std::sqrt(std::pow(row - halfHeight, 2) + std::pow(column - halfWidth, 2));
			if (distance < (cutoff - bandwidth / 2) || distance > (cutoff + bandwidth / 2))
			{
				visualisationMatrix[row][column].real(0);
				visualisationMatrix[row][column].imag(0);
			}
		}
	}
}
