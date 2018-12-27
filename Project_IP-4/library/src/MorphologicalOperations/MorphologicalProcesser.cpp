#include <MorphologicalProcesser.h>
#include <iostream>

using namespace std;

MorphologicalProcesser::MorphologicalProcesser(string imageName, int option, double value)
	: Processer(imageName, option, value)
{
}

MorphologicalProcesser::~MorphologicalProcesser()
{
}

// When in doubt whether the basic structuring elements are correct
void MorphologicalProcesser::printBasicStructuringElements()
{
	cout << endl;
	for (int i = 0; i < 10; i++)
	{
		for (int y = 0; y < 3; y++)
		{
			for (int x = 0; x < 3; x++)
			{
				cout << basicStructuringElements[i][y][x] << " ";
			}
			cout << endl;
		}

		cout << endl << endl;
	}
}


// Used to get the coordinates of the seed element
MorphologicalProcesser::Coordinates MorphologicalProcesser::getSeedCoordinates()
{
	cimg_library::CImgDisplay display(image, "Choose the seed pixel");
	Coordinates coordinates;

	// Used to check whether the display was not closed without choosing the pixel
	coordinates.x = -1;
	while (!display.is_closed())
	{
		display.wait();
		// Left mouse button
		if (display.button() & 1 && display.mouse_x() >= 0 && display.mouse_y() >= 0)
		{
			coordinates.x = (unsigned short int)display.mouse_x();
			coordinates.y = (unsigned short int)display.mouse_y();
			display.close();
		}
	}
	return coordinates;
}

void MorphologicalProcesser::processImage()
{
	cimg_library::CImg<unsigned char> initialImage;
	try
	{
		initialImage.load(imageName.c_str());
	}
	catch (cimg_library::CImgException)
	{
		cout << endl << "Image could not be loaded. Please check whether the filename is correct." << endl;
		return;
	}

	if (!initialImage) return;

	image = initialImage;
	height = image.height();
	width = image.width();

	switch (option)
	{
		case dilation:
			performDilation();
			break;
		case erosion:
			performErosion();
			break;
		case opening:
			performOpening();
			break;
		case closing:
			performClosing();
			break;
		case hmt:
			performHitOrMiss();
			break;
		case mvariant:
			performM5(false);
			break;
		case merging:

			cout << endl << "Please choose the seed pixel from the window." << endl;

			Coordinates coordinates = getSeedCoordinates();

			cout << "x: " << coordinates.x << endl << "y: " << coordinates.y << endl << "trh: " << value << endl;

			if (coordinates.x != -1)
			{
				performMerging(coordinates.x, coordinates.y, (int)value);
			}
			else
			{
				cout << endl << "You have not chosen the seed pixel. Operation has been aborted." << endl;
			}

			break;
		default: 
			break;
	}

	image.display("Result preview", false);
	image.save("result.bmp");
}