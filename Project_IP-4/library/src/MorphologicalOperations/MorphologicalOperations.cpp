#include "MorphologicalProcesser.h"

void MorphologicalProcesser::performDilation()
{
	cimg_library::CImg<unsigned char> imageCopy = image;

	unsigned short int window_x;
	unsigned short int window_y;

	for (unsigned short int x = 1; x < width - 1; x++)
	{
		for (unsigned short int y = 1; y < height - 1; y++)
		{
			if (image(x, y, 0) != COLOR_BLACK)
			{
				continue;
			}

			for (window_y = 0; window_y < 3; window_y++)
			{
				for (window_x = 0; window_x < 3; window_x++)
				{
					if (basicStructuringElements[structuringElementVariant][window_y][window_x])
					{
						// The -1 is because when we're at (x, y) we consider the window starting from coordinates (x-1, y-1) and ending at coordinates (x+1, y+1)
						short int pixelPosition_x = x - 1 + window_x;
						short int pixelPosition_y = y - 1 + window_y;

						for (unsigned char channel = 0; channel < 3; channel++)
						{
							imageCopy(pixelPosition_x, pixelPosition_y, channel) = COLOR_BLACK;
						}
					}
				}
			}
		}
	}

	image = imageCopy;
}

void MorphologicalProcesser::performErosion()
{
	cimg_library::CImg<unsigned char> imageCopy = image;

	unsigned short int window_x;
	unsigned short int window_y;

	for (unsigned short int x = 0; x < width; x++)
	{
		for (unsigned short int y = 0; y < height; y++)
		{
			bool isFullyContained = true;

			for (window_y = 0; window_y < 3; window_y++)
			{
				for (window_x = 0; window_x < 3; window_x++)
				{
					unsigned char pixelInVariant = basicStructuringElements[structuringElementVariant][window_y][window_x];

					// If the corresponding pixel in variant is black
					if (pixelInVariant)
					{
						// The -1 is because when we're at (x, y) we consider the window starting from coordinates (x-1, y-1) and ending at coordinates (x+1, y+1)
						short int pixelPosition_x = x - 1 + window_x;
						if (pixelPosition_x < 0 || pixelPosition_x > width - 1)
						{
							isFullyContained = false;
						}

						short int pixelPosition_y = y - 1 + window_y;
						if ((pixelPosition_y < 0 || pixelPosition_y > height - 1) || image(pixelPosition_x, pixelPosition_y, 0) != COLOR_BLACK)
						{
							isFullyContained = false;
						}
					}

					if (!isFullyContained) break;
				}
				if (!isFullyContained) break;
			}
			if (!isFullyContained)
			{
				for (unsigned char channel = 0; channel < 3; channel++)
				{
					imageCopy(x, y, channel) = COLOR_WHITE;
				}
			}
		}
	}

	image = imageCopy;
}

void MorphologicalProcesser::performOpening()
{
	performErosion();
	performDilation();
}

void MorphologicalProcesser::performClosing()
{
	performDilation();
	performErosion();
}

void MorphologicalProcesser::performHitOrMiss()
{
	cimg_library::CImg<unsigned char> result = image;

	unsigned int element_x, element_y;
	int index_x, index_y;
	bool hmtSatisfied;

	for (int x = 1; x < width - 1; x++)
	{
		for (int y = 1; y < height - 1; y++)
		{
			hmtSatisfied = true;

			index_y = -1;
			
			for (element_y = 0; element_y < 3; element_y++)
			{
				index_x = -1;
				
				for (element_x = 0; element_x < 3; element_x++)
				{
					if (hmtStructuringElements[structuringElementVariant][element_y][element_x] == -1)
					{
						index_x++;
						continue;
					}

					if ((image(x + index_x, y + index_y, 0) == 255 ? 1 : 0) != hmtStructuringElements[structuringElementVariant][element_y][element_x])
					{
						hmtSatisfied = false;
						goto loopExit;
					}

					index_x++;
				}

				index_y++;
			}
		
		loopExit:

			if (hmtSatisfied)
			{
				result(x, y, 0) = 255;
				result(x, y, 1) = 0;
				result(x, y, 2) = 0;
			}
		}
	}
	
	image = result;
}

//Used to leave only pixels marked by HMT (gets rid of background left by HMT for better result showcase)
void MorphologicalProcesser::blacken()
{
	for (unsigned int x = 0; x < width; x++)
	{
		for (unsigned int y = 0; y < height; y++)
		{
			if (image(x, y, 0) == 255 && image(x, y, 1) == 0 && image(x, y, 2) == 0)
			{
				image(x, y, 1) = 255;
				image(x, y, 2) = 255;
				continue;
			}

			for (int channel = 0; channel < image.spectrum(); channel++)
			{
				image(x, y, channel) = 0;
			}
		}
	}
}

void MorphologicalProcesser::complement()
{
	for (unsigned int x = 0; x < width; x++)
	{
		for (unsigned int y = 0; y < height; y++)
		{
			for (int channel = 0; channel < image.spectrum(); channel++)
			{
				image(x, y, channel) = image(x, y, channel) == 255 ? 0 : 255;
			}
		}
	}
}

void MorphologicalProcesser::intersection(cimg_library::CImg<unsigned char> reference)
{
	for (unsigned int x = 0; x < width; x++)
	{
		for (unsigned int y = 0; y < height; y++)
		{
			if (reference(x, y) != image(x, y))
			{
				for (int channel = 0; channel < image.spectrum(); channel++)
				{
					image(x, y, channel) = 0;
				}
			}
		}
	}
}

//Used to check if M5 should stop performing as there are no possible changes to be made
bool MorphologicalProcesser::checkEquality()
{
	cimg_library::CImg<unsigned char> reference = image;
	performM5(true);
	for (unsigned int x = 0; x < width; x++)
	{
		for (unsigned int y = 0; y < height; y++)
		{
			for (int channel = 0; channel < image.spectrum(); channel++)
			{
				if (image(x, y, channel) != reference(x, y, channel))
				{
					return false;
				}
			}
		}
	}

	return true;
}

//controlRun is used to denote if call of performM5 is for the purpose of checking if it should stop performing operations on the image or a normal run
void MorphologicalProcesser::performM5(bool controlRun)
{
	cimg_library::CImg<unsigned char> reference;

	for (structuringElementVariant = 4; structuringElementVariant <= 12; structuringElementVariant++)
	{
		reference = image;
		performHitOrMiss();
		blacken();
		complement();
		intersection(reference);
	}

	if (controlRun == false && !checkEquality()) performM5(false);
}


///////////////////////////////


/*
 *This method can be used instead of the simpler dilation - it introduces a window
 */
void MorphologicalProcesser::performDilationWithWindow()
{

	cimg_library::CImg<unsigned char> imageCopy = image;

	Window window;

	for (unsigned short int x = 0; x < width; x++)
	{
		window.x0 = x - 1;
		window.x1 = x + 1;
		window.modifier_x = 0;

		if (window.x0 < 0)
		{
			window.x0 = 0;
			window.modifier_x = 1;
		}
		else if (window.x1 > width - 1)
		{
			window.x1 = width - 1;
		}

		for (unsigned short int y = 0; y < height; y++)
		{

			window.y0 = y - 1;
			window.y1 = y + 1;
			window.modifier_y = 0;

			if (window.y0 < 0)
			{
				window.y0 = 0;
				window.modifier_y = 1;
			}
			else if (window.y1 > height - 1)
			{
				window.y1 = height - 1;
			}

			if (image(x, y, 0) == COLOR_BLACK)
			{
				for (window.iterator_y = 0; window.iterator_y <= window.y1 - window.y0; window.iterator_y++)
				{
					for (window.iterator_x = 0; window.iterator_x <= window.x1 - window.x0; window.iterator_x++)
					{
						if (basicStructuringElements[structuringElementVariant][window.iterator_y + window.modifier_y][window.iterator_x + window.modifier_x])
						{
							short int pixelPosition_x = window.x0 + window.iterator_x;
							short int pixelPosition_y = window.y0 + window.iterator_y;

							for (unsigned char channel = 0; channel < 3; channel++)
							{
								imageCopy(pixelPosition_x, pixelPosition_y, channel) = COLOR_BLACK;
							}
						}
					}
				}
			}
		}
	}

	image = imageCopy;
}

/*
 *This method can be used instead of the simpler erosion - it introduces a window
 */
void MorphologicalProcesser::performErosionWithWindow()
{
	cimg_library::CImg<unsigned char> imageCopy = image;

	Window window;

	for (unsigned short int x = 0; x < width; x++)
	{
		window.x0 = x - 1;
		window.x1 = x + 1;
		window.modifier_x = 0;

		if (window.x0 < 0)
		{
			window.x0 = 0;
			window.modifier_x = 1;
		}
		else if (window.x1 > width - 1)
		{
			window.x1 = width - 1;
		}

		for (unsigned short int y = 0; y < height; y++)
		{

			window.y0 = y - 1;
			window.y1 = y + 1;
			window.modifier_y = 0;

			if (window.y0 < 0)
			{
				window.y0 = 0;
				window.modifier_y = 1;
			}
			else if (window.y1 > height - 1)
			{
				window.y1 = height - 1;
			}

			bool isFullyContained = true;
			for (window.iterator_y = 0; window.iterator_y <= window.y1 - window.y0; window.iterator_y++)
			{
				for (window.iterator_x = 0; window.iterator_x <= window.x1 - window.x0; window.iterator_x++)
				{
					unsigned char pixelInVariant = basicStructuringElements[structuringElementVariant][window.iterator_y + window.modifier_y][window.iterator_x + window.modifier_x];
					// If the corresponding pixel in variant is black
					if (pixelInVariant)
					{
						short int pixelPosition_x = window.x0 + window.iterator_x;
						short int pixelPosition_y = window.y0 + window.iterator_y;

						if (image(pixelPosition_x, pixelPosition_y, 0) != COLOR_BLACK)
						{
							isFullyContained = false;
						}
					}


					if (!isFullyContained) break;
				}
				if (!isFullyContained) break;
			}

			if (!isFullyContained)
			{
				for (unsigned char channel = 0; channel < 3; channel++)
				{
					imageCopy(x, y, channel) = COLOR_WHITE;
				}
			}


		}
	}

	image = imageCopy;
}