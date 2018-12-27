#include "ArgumentsHandler.h"
#include "Error.h"
#include <algorithm>
#include <iostream>

using namespace std;

ArgumentsHandler::ArgumentsHandler(int argc, char* argv[])
	:argc(argc), argv(argv)
{
}

ArgumentsHandler::~ArgumentsHandler()
{
}

int ArgumentsHandler::convertToInt(std::string option) const
{
	for (int i = 0; i <= 36; i++)
	{
		if (options[i] == option) return i;
	}
	return 0;
}

bool ArgumentsHandler::optionIsValid(string option) const
{
	return find(begin(options), end(options), option) != end(options);
}

bool ArgumentsHandler::optionRequiresValue(string option) const
{
	return find(begin(optionsRequiringValues), end(optionsRequiringValues), option) != end(optionsRequiringValues);
}

bool ArgumentsHandler::optionRequiresChannel(string option) const
{
	return find(begin(optionsRequiringChannel), end(optionsRequiringChannel), option) != end(optionsRequiringChannel);
}

bool ArgumentsHandler::isPowerOfTwo(int x) const
{
	while (x != 1)
	{
		if (x % 2 != 0) return false;
		x /= 2;
	}
	return true;
}

bool ArgumentsHandler::valueIsValid(string value) const
{
	int dot_counter = 0;

	for (string::iterator iterator = value.begin(); iterator != value.end(); ++iterator)
	{
		if (*iterator < 48 || *iterator > 57)
		{
			// Options which accept floats and negative values
			if (option == "--brightness" || option == "--contrast")
			{
				if (iterator == value.begin() && *iterator == 45) continue;
				if (iterator != value.begin() && *iterator == 46) dot_counter++;
				else return false;
			}
			// Options which accept floats
			else if (option == "--enlarge" || option == "--shrink")
			{
				if (iterator != value.begin() && *iterator == 46) dot_counter++;
				else return false;
			}
			// Options which accept integrals
			else return false;
		}
		if (dot_counter > 1) return false;
	}

	if (optionRequiresChannel(option) && stoi(value) > 2) return false;
	if (option == "--hpower" && !isWithinPixelRange(stoi(value))) return false;
	if (option == "--sedgesharp" && stoi(value) > 3) return false;
	if (option == "--orosenfeld" && (stoi(value) <= 0 || !isPowerOfTwo(stoi(value)))) return false;
	if ((option == "--dilation" || option == "--erosion" || option == "--opening" || option == "--closing") && stoi(value) > 9) return false;

	return true;
}

bool ArgumentsHandler::isNameOfFile(std::string name) const
{
	return name.size() >= 4 && name.compare(name.size() - 4, 4, ".bmp") == 0;
}

bool ArgumentsHandler::isWithinPixelRange(int value) const
{
	if (value < 0 || value > 255) return false;
	return true;
}

void ArgumentsHandler::helpMessage() const
{
	std::cout << R"(Command line format:
	name --command [-argument=value [...]]

Available commands:

TASK 1:
	Elementary operations:
	--brightness
		Image brightness modification
			Adds provided value to the pixel's RGB channels keeping it within
			range from 0 to 255.
	--contrast
		Image contrast modification
			Chnages pixel's RGB values via use of linear function.
	--negative
		Negative
			Negates pixel's RGB values with the use of bitwise negation operator.
	
	Geometric operations:
	--hflip
		Horizontal flip
			Swaps every pixel's channel values from left to half the image's
			width with the corresponding pixel from the right half.
	--vflip
		Vertical flip
			Swaps every pixel's channel values from top to half the image's
			height with the corresponding pixel from the bottom half.
	--dflip
		Diagonal flip
			Swaps every pixel's channel values from top to half the image's
			height with the pixel placed diagonally on the bottom half.
	--shrink 
		Image shrinking
			Shrinks the image by interpolating pixel's RGB values in 2 x 2 grid
			by taking 0.25 of each surrounding pixel with respect to the original image.
	--enlarge
		Image enlargement
			Enlarges the image with the use of bilinear interpolation. Estimating distance 
			between original pixels in 2 x 2 grid program calculates the weighted sum of these
			pixels' RGB values and assigns it to blank pixels in enlarged image.
	Filters:
		General information:
			A moving window with side length of 2 * value + 1 is created.
			Eg. --min 1 produces a window of size 3x3.
			The image is then traversed by the window (with the pixel which
			we want to change in the center) and if the window is to
			exceed the image's height or width, it is shortened to match
			the image's size.
			Eg. a 3x3 window in pixel (0,0) will be shortened to a 2x2 window.
			For every channel, a dynamic array is filled with it's values
			and then an operation is performed according to the chosen option.
			Every operation is performed on a temporary image.
	--min value
		Min filter
			Takes the minimum value for every channel and substitutes it in place
			of the channel at the pixel's position on a temporary image.
	--max value
		Max filter
			Takes the maximum value for every channel and substitutes it in place
			of the channel at the pixel's position on a temporary image.
	--median value
		Median filter
			Sorts the values of every channel, takes the median and substitutes
			it in place of the channel at the pixel's position on a temporary image.

	Similarity measures:
		General information:
			Used to compare the effectiveness of noise removal methods.
			The correct format:
			name originalImage.bmp --option noisyImage.bmp denoisedImage.bmp
	--mse
		Mean square error
	--pmse
		Peak mean square error
	--snr
		Signal to noise ratio
	--psnr
		Peak signal to noise ratio
	--md
		Maximum difference
TASK 2:
	
	--histogram channel
		Generates and displays a histogram for the given image at a given channel.
	--hpower minBrightness maxBrightness
		Power 2/3 final probability density function (image quality enhancement).
		Used to extend the dynamic range of an image (may generate artifacts for color images).
		Both values must be within <0, 255> range.

	Image characteristics based on the histogram for a given channel:

	--cmean channel
		Calculates the mean value of pixel intensity.
	--cvariance channel
		Calculates the variance.
	--cstdev channel
		Calculates the standard deviation based on the variance.
	--cvarcoi channel
		Calculates the variation coefficient I.
	--casyco
		Calculates the asymmetry coefficient based on the mean.
	--cflatco
		Calculates the flattening coefficient based on the mean.
	--cvarcoii
		Calculates the variation coefficient II.
	--centropy
		Calculates the information source entropy.


	--sedgesharp <0, 2>
		Edge sharpening algorithm. Available variants:
		Mask 0:

		 0  -1   0
		-1   5  -1
		 0  -1   0

		Mask 1:

		-1  -1  -1
		-1   9  -1
		-1  -1  -1

		Mask 2:

		 1  -2   1
		-2   5  -2
		 1  -2   1

	--sedgesharp 3

		Optimized edge sharpening algorithm working with the mask:

		 0  -1   0
		-1   5  -1
		 0  -1   0

	--orosenfeld P
		Edge detection algorithm. P must be a power of 2.

)";
}

void ArgumentsHandler::validateArguments()
{
	//Definition of type beeing a pointer to a void function within class Error
	typedef void (Error::*Error_fnc_ptr)();

	Error error;

	switch (argc)
	{
		case 2:
			option = argv[1];

			try
			{
				option == "--help" ? helpMessage() : throw error.invalidArguments;
			}
			catch (Error_fnc_ptr exception)
			{
				(error.*exception)();
			}
			break;
		case 3:
			imageName = argv[1];
			option = argv[2];

			try
			{
				if (optionIsValid(option) && option != "--help")
				{
					argumentsAreValid = optionRequiresValue(option) ? throw error.optionNeedsValue : true;
				}
				else throw error.invalidOption;
			}
			catch (Error_fnc_ptr exception)
			{
				(error.*exception)();
			}
			break;
		case 4:
			imageName = argv[1];
			option = argv[2];
			value = argv[3];

			try
			{

				if (optionIsValid(option) && option != "--help")
				{
					if (!optionRequiresValue(option) || option == "--hpower") { throw error.invalidNumberOfArguments; }
					else if (!valueIsValid(value)) { throw error.invalidValue; }
					else { argumentsAreValid = true; break; }
				}
				else throw error.invalidOption;
				break;

			}
			catch (Error_fnc_ptr exception)
			{
				(error.*exception)();
			}
			break;
		case 5:
			imageName = argv[1];
			option = argv[2];

			try
			{
				if (optionIsValid(option))
				{
					if (option == "--help") { throw error.invalidArguments; }
					if (option == "--hpower")
					{
						value = argv[3];
						secondValue = argv[4];
						if (valueIsValid(value) && valueIsValid(secondValue))
						{
							if (stoi(value) <= stoi(secondValue))
							{
								argumentsAreValid = true; break;
							}
						}
						throw error.invalidValue;
					}
					else if (optionRequiresValue(option))
					{
						noisyImageName = argv[3];
						denoisedImageName = argv[4];
						if (isNameOfFile(noisyImageName) && isNameOfFile(denoisedImageName))
						{
							argumentsAreValid = true; break;
						}
					}
					throw error.invalidValue;
				}
				else throw error.invalidArguments;
			}
			catch (Error_fnc_ptr exception)
			{
				(error.*exception)();
			}
			break;
		default:
			error.InvalidNumberOfArguments();
			break;
	}

}

ArgumentsHandler::Processers ArgumentsHandler::get_currentProcesser() const
{
	int currentOption = convertToInt(option);
	if (currentOption < 17) return Processers::ImageProcesser;
	if (currentOption < 29) return Processers::HistogramProcesser;
	if (currentOption < 36) return Processers::MorphologicalProcesser;
	return Processers::None;
}

ArgumentsHandler::Arguments ArgumentsHandler::get_arguments() const
{
	ArgumentsHandler::Arguments arguments;
	arguments.imageName = imageName;
	arguments.denoisedImageName = denoisedImageName;
	arguments.noisyImageName = noisyImageName;
	arguments.option = convertToInt(option);
	arguments.value = value.length() > 0 ? stod(value) : 0;
	arguments.secondValue = secondValue.length() > 0 ? stoi(secondValue) : 0;
	arguments.processer = get_currentProcesser();

	return arguments;
}