#include "FrequencyProcesser.h"

void FrequencyProcesser::HPFEdge(cimg_library::CImg<unsigned char> *mask)
{
//	double halfHeight = height / 2, halfWidth = width / 2, distance;
//
//	for (int row = 0; row < height; row++)
//	{
//		for (int column = 0; column < width; column++)
//		{
//			distance = std::sqrt(std::pow(row - halfHeight, 2) + std::pow(column - halfWidth, 2));
//			if (maskImage(row, column) == 0)
//			{
//				visualisationMatrix[row][column].real(0);
//				visualisationMatrix[row][column].imag(0);
//			}
//			else if (distance < )
//			{
//				visualisationMatrix[row][column].real(0);
//				visualisationMatrix[row][column].imag(0);
//			}
//		}
//	}
}