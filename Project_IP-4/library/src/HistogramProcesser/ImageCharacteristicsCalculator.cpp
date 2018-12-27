#include "HistogramProcesser.h"

double HistogramProcesser::calculateMean(int channel)
{
	int numberOfPixels = width * height;
	double numerator = 0;
	for (int i = 0; i < 256; i++)
	{
		numerator += i * histogramHeight[channel][i];
	}

	return numerator / numberOfPixels;
}

double HistogramProcesser::calculateVariance(int channel)
{
	int numberOfPixels = width * height;
	double mean = calculateMean(channel);
	double numerator = 0;
	for (int i = 0; i < 256; i++)
	{
		numerator += pow(i - mean, 2) * histogramHeight[channel][i];
	}

	return numerator / numberOfPixels;
}

double HistogramProcesser::calculateStandardDeviation(int channel)
{
	return sqrt(calculateVariance(channel));
}

double HistogramProcesser::calculateVariationCoefficientI(int channel)
{
	return calculateStandardDeviation(channel) / calculateMean(channel);
}

double HistogramProcesser::calculateAssymetryCoefficient(int channel)
{
	int numberOfPixels = width * height;
	double mean = calculateMean(channel);
	double standardDeviation = calculateStandardDeviation(channel);
	double numerator = 0;
	for (int i = 0; i < 256; i++)
	{
		numerator += pow(i - mean, 3) * histogramHeight[channel][i];
	}

	return numerator / (numberOfPixels * pow(standardDeviation, 3));
}

double HistogramProcesser::calculateFlatteningCoefficient(int channel)
{
	int numberOfPixels = width * height;
	double mean = calculateMean(channel);
	double standardDeviation = calculateStandardDeviation(channel);
	double numerator = 0;
	for (int i = 0; i < 256; i++)
	{
		numerator += pow(i - mean, 4) * histogramHeight[channel][i];
	}

	return (numerator - 3) / (numberOfPixels * pow(standardDeviation, 4));
}

double HistogramProcesser::calculateVariationCoefficientII(int channel)
{
	int numberOfPixels = width * height;
	double numerator = 0;
	for (int i = 0; i < 256; i++)
	{
		numerator += pow(histogramHeight[channel][i], 2);
	}
	return numerator / pow(numberOfPixels, 2);
}

double HistogramProcesser::calculateInformationSourceEntropy(int channel)
{
	int numberOfPixels = width * height;
	double numerator = 0;
	for (int i = 0; i < 256; i++)
	{
		if (histogramHeight[channel][i] > 0)
		{
			numerator += histogramHeight[channel][i] * log2((double)histogramHeight[channel][i] / numberOfPixels);
		}
	}
	return - numerator / numberOfPixels;
}

