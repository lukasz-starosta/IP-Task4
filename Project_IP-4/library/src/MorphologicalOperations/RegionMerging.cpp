#include "MorphologicalProcesser.h"
#include <queue>

using namespace std;

void MorphologicalProcesser::performMerging(unsigned short int seedX, unsigned short int seedY, int threshold)
{
	imageCopy = image;
	queue<Coordinates> seedPoints;
	vector<Coordinates> surroundingPixels;
	Coordinates seedPixel;
	seedPoints.push({ seedX, seedY });

	while (!seedPoints.empty())
	{
		seedPixel = seedPoints.front();
		seedPoints.pop();

		surroundingPixels = getSurroundingPixels(seedPixel.x, seedPixel.y, seedX, seedY, threshold);
		for (unsigned int i = 0; i < surroundingPixels.size(); i++)
		{
			seedPoints.push(surroundingPixels[i]);
		}
		surroundingPixels.clear();
	}

	image = imageCopy;
}

vector<MorphologicalProcesser::Coordinates>  MorphologicalProcesser::getSurroundingPixels(int x, int y, int seedX, int seedY, int threshold)
{
	vector<Coordinates> validPixels;

	for (unsigned short int i = x > 1 ? x - 1 : 0; i <= x + 1 && i < width; i++)
	{
		for (unsigned short int j = y > 1 ? y - 1 : 0; j <= y + 1 && j < height; j++)
		{
			if (isInRegion(i, j)) continue;

			if (isPixelWithinThreshold(seedX, seedY, i, j, threshold))
			{
				colorRed(i, j);
				validPixels.push_back({ i, j });
			}
		}
	}

	return validPixels;
}

bool MorphologicalProcesser::isInRegion(int x, int y)
{
	bool isRed = imageCopy(x, y, 0) == (unsigned char)255 &&
		imageCopy(x, y, 1) == 0 &&
		imageCopy(x, y, 2) == 0;
	return isRed;
}

bool MorphologicalProcesser::isPixelWithinThreshold(int x1, int y1, int x2, int y2, int threshold)
{
	double distance = getEuclideanDistance(image(x1, y1, 0), image(x1, y1, 1), image(x1, y1, 2), image(x2, y2, 0), image(x2, y2, 1), image(x2, y2, 2));
	return distance <= threshold;
}

double MorphologicalProcesser::getEuclideanDistance(int r1, int g1, int b1, int r2, int g2, int b2)
{
	return sqrt(pow(r2 - r1, 2) + pow(g2 - g1, 2) + pow(b2 - b1, 2));
}

void MorphologicalProcesser::colorRed(int x, int y)
{
	imageCopy(x, y, 0) = (unsigned char)255;
	imageCopy(x, y, 1) = 0;
	imageCopy(x, y, 2) = 0;
}

