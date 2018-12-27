#include "HistogramProcesser.h"
#include <iostream>
using namespace std;
void HistogramProcesser::performHPower(int minBrightness, int maxBrightness)
{

	int numberOfPixels = width * height;
	unsigned short int channels = image.spectrum();
	double base = 0;
	double exponent = (double)1 / 3;
	for (unsigned int x = 0; x < width; x++)
	{
		for (unsigned int y = 0; y < height; y++)
		{
			for (unsigned short int channel = 0; channel < channels; channel++)
			{
				double sum = 0;
				for (unsigned char i = 0; i < image(x, y, channel); i++)
				{
					sum += histogramHeight[channel][i];
				}
				base = pow(minBrightness, exponent) + (pow(maxBrightness, exponent) - pow(minBrightness, exponent))*sum / numberOfPixels;
				image(x, y, channel) = truncate(pow(base, 3));
			}
		}
	}
}