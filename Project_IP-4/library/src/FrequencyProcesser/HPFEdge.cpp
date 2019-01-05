#include "FrequencyProcesser.h"

void FrequencyProcesser::HPFEdge(int mask, double distanceFromOrigin)
{
	cimg_library::CImg<unsigned char> maskImage;
	if (mask == 1)
	{
		maskImage.load("F5mask1.bmp");
	}
	else if (mask == 2)
	{
		maskImage.load("F5mask2.bmp");
	}

	double halfHeight = height / 2, halfWidth = width / 2, distance;

	for (int row = 0; row < height; row++)
	{
		for (int column = 0; column < width; column++)
		{
			distance = std::sqrt(std::pow(row - halfHeight, 2) + std::pow(column - halfWidth, 2));
			if (maskImage(row, column) == 0)
			{
				visualisationMatrix[row][column].real(0);
				visualisationMatrix[row][column].imag(0);
			}
			else if (distance < distanceFromOrigin)
			{
				visualisationMatrix[row][column].real(0);
				visualisationMatrix[row][column].imag(0);
			}
		}
	}
}