#include "HistogramProcesser.h"
#include <iostream>

void HistogramProcesser::edgeDetection(int P)
{
	unsigned short int channels = image.spectrum();
	cimg_library::CImg<unsigned char> imageCopy = image;
	
	unsigned short int x;
	unsigned short int y;
	unsigned short int channel;
	unsigned short int iterator;
	double value;
	double factor = 1 / (double)P;

	for (x = 0; x < width; x++)
	{
		for (y = 0; y < height; y++)
		{
			for (channel = 0; channel < channels; channel++)
			{
				value = 0;
				iterator = 0;
				for (iterator; iterator <= P; iterator++)
				{
					value = value + image((x + P - iterator <= width - 1) ? x + P - iterator : width - 1, y, channel) - image((x - iterator >= 0) ? x - iterator : 0, y, channel);
				}
				value *= factor;
				if (value < 0) value *= -1;
				imageCopy(x, y, channel) = truncate((int)value);
			}
		}
	}
	image = imageCopy;
}