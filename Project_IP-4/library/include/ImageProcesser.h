#pragma once
#include <string>
#include "CImg.h"
#include "Processer.h"

class ImageProcesser : public Processer
{
private:
	enum Options
	{
		brightness = 1, contrast, negative, hflip, vflip, dflip, shrink, enlarge, min, max, median, mse, pmse, snr, psnr, md
	};

	std::string noisyImageName;
	std::string denoisedImageName;

	cimg_library::CImg<unsigned char> noisyImage;
	cimg_library::CImg<unsigned char> denoisedImage;

	void swapPixelsRGBValues(unsigned int x_1, unsigned int y_1, unsigned int x_2, unsigned int y_2);

	unsigned char getMedian(unsigned char* channelValues, size_t arraySize);
	unsigned char getMin(unsigned char* channelValues, size_t arraySize);
	unsigned char getMax(unsigned char* channelValues, size_t arraySize);

	void changeBrightness(int modifier);
	void changeToNegative();
	void changeContrast(int modifier);
	void shrinkImage(double modifier);
	void enlargeImage(double modifier);
	void horizontalFlip();
	void verticalFlip();
	void diagonalFlip();

	void medianFilter(int radius);
	void minFilter(int radius);
	void maxFilter(int radius);

	void calculateMSE();
	void calculatePMSE();
	void calculateSNR();
	void calculatePSNR();
	void calculateMD();

public:
	ImageProcesser(std::string imageName, int option, double value, std::string noisyImageName, std::string denoisedImageName);
	~ImageProcesser();

	void processImage();
};