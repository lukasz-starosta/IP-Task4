#include "HistogramProcesser.h"
#include <vector>
#include <iostream>

void HistogramProcesser::edgeSharpening(int variant)
{
	unsigned short int channels = image.spectrum();
	cimg_library::CImg<unsigned char> imageCopy = image;
	struct Window
	{
		short int x0;
		short int x1;
		short int y0;
		short int y1;
	};

	Window window;
	unsigned short int x;
	unsigned short int y;
	unsigned short int channel;
	unsigned short int i;
	unsigned short int j;
	unsigned short int index;
	int value;

	for (x = 0; x < width; x++)
	{
		window.x0 = x - 1;
		window.x1 = x + 1;

		if (window.x0 < 0)
		{
			window.x0 = 0;
		}
		else if (window.x1 > width - 1)
		{
			window.x1 = width - 1;
		}

		for (y = 0; y < height; y++)
		{
			window.y0 = y - 1;
			window.y1 = y + 1;
			if (window.y0 < 0)
			{
				window.y0 = 0;
			}
			else if (window.y1 > height - 1)
			{
				window.y1 = height - 1;
			}

			for (channel = 0; channel < channels; channel++)
			{
				value = 0;
				index = 0;
				for (j = window.y0; j <= window.y1; j++)
				{
					for (i = window.x0; i <= window.x1; i++)
					{
						value += image(i, j, channel) * kernels[variant][index];
						index++;
					}
				}
				imageCopy(x, y, channel) = truncate(value);
			}
		}
	}
	image = imageCopy;
}

void HistogramProcesser::optimizedSharpen()
{
	unsigned short int channels = image.spectrum();
	cimg_library::CImg<unsigned char> imageCopy = image;

	unsigned short int x;
	unsigned short int y;
	unsigned short int channel;

	int value;

	for (x = 1; x < width - 1; x++)
	{
		for (y = 1; y < height - 1; y++)
		{
			for (channel = 0; channel < channels; channel++)
			{
				value = 0 - image(x, y - 1, channel) - image(x - 1, y, channel) + 5 * image(x, y, channel) - image(x, y + 1, channel) - image(x, y - 1, channel);
				imageCopy(x, y, channel) = truncate(value);
			}
		}
	}

	image = imageCopy;
}
