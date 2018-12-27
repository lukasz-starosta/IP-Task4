#pragma once
#include "Processer.h"

class HistogramProcesser : public Processer
{
private:
	enum Options
	{
		histogram = 17, hpower, cmean, cvariance, cstdev, cvarcoi, casyco, cflatco, cvarcoii, centropy, sedgesharp, orosenfeld
	};

	int secondValue;

	int histogramHeight[3][256] = {};
	cimg_library::CImg<unsigned char> histogramImage;
	int kernels[3][9] = { { 0,-1,0, -1,5,-1, 0,-1,0 },  { -1,-1,-1, -1,9,-1, -1,-1,-1 },  { 1,-2,1, -2,5,-2, 1,-2,1 } };

	void getImageHistograms();
	void createHistogram(int channel);
	void performHPower(int minBrightness, int maxBrightness);
	double calculateMean(int channel);
	double calculateVariance(int channel);
	double calculateStandardDeviation(int channel);
	double calculateVariationCoefficientI(int channel);
	double calculateAssymetryCoefficient(int channel);
	double calculateFlatteningCoefficient(int channel);
	double calculateVariationCoefficientII(int channel);
	double calculateInformationSourceEntropy(int channel);
	void edgeSharpening(int variant);
	void optimizedSharpen();
	void edgeDetection(int P);
public:
	HistogramProcesser();
	HistogramProcesser(std::string imageName, int option, double value, int secondValue);
	~HistogramProcesser();

	void processImage();
};

