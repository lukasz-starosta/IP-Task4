#include "HistogramProcesser.h"
#include <iostream>
#include <chrono>

using namespace std;

HistogramProcesser::HistogramProcesser()
{
}

HistogramProcesser::HistogramProcesser(std::string imageName, int option, double value, int secondValue)
	:Processer(imageName, option, value), secondValue(secondValue)
{
}


HistogramProcesser::~HistogramProcesser()
{
}

void HistogramProcesser::processImage()
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

	if (option < sedgesharp) getImageHistograms();

	chrono::high_resolution_clock::time_point t1 = chrono::high_resolution_clock::now();
	int channel = (int)value;
	switch (option)
	{
		case histogram:
			createHistogram(channel);
			break;
		case hpower:
			performHPower((int)value, secondValue);
			break;
		case cmean:
			cout << "Mean is: " << calculateMean(channel) << endl;
			break;
		case cvariance:
			cout << "Variance is: " << calculateVariance(channel) << endl;
			break;
		case cstdev:
			cout << "Standard deviation is: " << calculateStandardDeviation(channel) << endl;
			break;
		case cvarcoi:
			cout << "Variation coefficient I is: " << calculateVariationCoefficientI(channel) << endl;
			break;
		case casyco:
			cout << "Assymetry coefficient is: " << calculateAssymetryCoefficient(channel) << endl;
			break;
		case cflatco:
			cout << "Flattening coefficient is: " << calculateFlatteningCoefficient(channel) << endl;
			break;
		case cvarcoii:
			cout << "Variation coefficient I is: " << calculateVariationCoefficientII(channel) << endl;
			break;
		case centropy:
			cout << "Information source entropy is: " << calculateInformationSourceEntropy(channel) << endl;
			break;
		case sedgesharp:
			if (channel == 3) optimizedSharpen();
			else edgeSharpening((int)value);
			break;
		case orosenfeld:
			edgeDetection((int)value);
			break;
		default:
			break;
	}

	if (option > centropy || option == hpower)
	{
		chrono::high_resolution_clock::time_point t2 = chrono::high_resolution_clock::now();
		auto duration = chrono::duration_cast<chrono::microseconds>(t2 - t1).count() / (double)1000000;
		cout << "Algorithm duration: " << duration << " seconds";
		image.save("processedImage.bmp");
		image.display("Processed image", false);
	}
}
