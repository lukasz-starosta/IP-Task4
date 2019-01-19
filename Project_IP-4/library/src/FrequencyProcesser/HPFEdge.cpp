#include "FrequencyProcesser.h"

void FrequencyProcesser::HPFEdge()
{
	for (int row = 0; row < height; row++)
	{
		for (int column = 0; column < width; column++)
		{
			if (mask(column, row) == 0)
			{
				visualisationMatrix[row][column].real(0);
				visualisationMatrix[row][column].imag(0);
			}
		}
	}
}