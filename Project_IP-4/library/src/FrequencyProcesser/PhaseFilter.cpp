#define M_PI 3.14159265358979323846
#include "FrequencyProcesser.h"

void FrequencyProcesser::PhaseFilter(double k, double l)
{
	double halfHeight = height / 2, halfWidth = width / 2;
	for (int row = 0; row < height; row++)
	{
		for (int column = 0; column < width; column++)
		{
			std::complex<double> phase = std::polar<double> (1, (-(double)column * k * 2.0) / (double)height + (-(double)row * l * 2.0) / (double)width + (double)(k + l) * M_PI);
			visualisationMatrix[row][column] *= phase;
		}
	}
}