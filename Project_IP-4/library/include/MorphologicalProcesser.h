#pragma once
#include "Processer.h"
#include <string>
#include <vector>

class MorphologicalProcesser : public Processer
{
private:
	enum Options
	{
		dilation = 29, erosion, opening, closing, hmt, mvariant, merging
	};

	struct Window
	{
		short int x0;
		short int x1;
		short int y0;
		short int y1;

		// If the window is at (0,0) we want only some of the pixels taken from the variant - (1, 1), (1, 2), (2, 1), (2, 2). thus the window modifier
		unsigned short int modifier_x;
		unsigned short int modifier_y;

		unsigned short int iterator_x;
		unsigned short int iterator_y;
	};

	struct Coordinates
	{
		unsigned short int x;
		unsigned short int y;
	};

	const unsigned char COLOR_BLACK = 0;
	const unsigned char COLOR_WHITE = (unsigned char)255;

	int structuringElementVariant = (int)value;

	cimg_library::CImg<unsigned char> imageCopy;

	// Does not involve the elements from xi up
	const unsigned int basicStructuringElements[10][3][3] = {

		{	{0, 0, 0},
			{0, 1, 1},
			{0, 0, 0}
		},
		{	{0, 0, 0},
			{0, 1, 0},
			{0, 1, 0}
		},
		{	{1, 1, 1},
			{1, 1, 1},
			{1, 1, 1}
		},
		{	{0, 1, 0},
			{1, 1, 1},
			{0, 1, 0}
		},
		{	{0, 0, 0},
			{0, 1, 1},
			{0, 1, 0}
		},
		{	{0, 0, 0},
			{0, 0, 1},
			{0, 1, 0}
		},
		{	{0, 0, 0},
			{1, 1, 1},
			{0, 0, 0}
		},
		{	{0, 0, 0},
			{1, 0, 1},
			{0, 0, 0}
		},
		{	{0, 0, 0},
			{1, 1, 0},
			{1, 0, 0}
		},
		{	{0, 1, 1},
			{0, 1, 0},
			{0, 0, 0}
		}};


	//-1 deontes inactive element
	const int hmtStructuringElements[12][3][3] = {
		//xi
		{
			{1, -1, -1},
			{1, 0, -1},
			{1, -1, -1}
		},
		{
			{1, 1, 1},
			{-1, 0, -1},
			{-1, -1, -1}
		},
		{
			{-1, -1, 1},
			{-1, 0, 1},
			{-1, -1, 1}
		},
		{
			{-1, -1, -1},
			{-1, 0, -1},
			{1, 1, 1}
		},
		//xii
		{
			{0, 0, 0},
			{-1, 1, -1},
			{1, 1, 1}
		},
		{
			{-1, 0, 0},
			{1, 1, 0},
			{1, 1, -1}
		},
		{
			{1, -1, 0},
			{1, 1, 0},
			{1, -1, 0}
		},
		{
			{1, 1, -1},
			{1, 1, 0},
			{-1, 0, 0}
		},
		{
			{1, 1, 1},
			{-1, 1, -1},
			{0, 0, 0}
		},
		{
			{-1, 1, 1},
			{0, 1, 1},
			{0, 0, -1}
		},
		{
			{0, -1, 1},
			{0, 1, 1},
			{0, -1, 1}
		},
		{
			{0, 0, -1},
			{0, 1, 1},
			{-1, 1, 1}
		}
	};

	void printBasicStructuringElements();
	Coordinates getSeedCoordinates();

	void performDilation();
	void performErosion();
	void performOpening();
	void performClosing();
	void performHitOrMiss();
	void blacken();
	void complement();
	void intersection(cimg_library::CImg<unsigned char> reference);
	bool checkEquality();
	void performM5(bool controlRun);
	void performMerging(unsigned short int seed_x, unsigned short int seed_y, int threshold);

	//Helper methods
	double getEuclideanDistance(int r1, int g1, int b1, int r2, int b2, int g2);
	bool isInRegion(int x, int y);
	bool isPixelWithinThreshold(int x1, int y1, int x2, int y2, int threshold);
	void colorRed(int x, int y);
	std::vector<Coordinates> getSurroundingPixels(int x, int y, int seedX, int seedY, int threshold);


	// Methods developed initially with the Window (they produced different results than these, which were presented on the lectures)
	void performDilationWithWindow();
	void performErosionWithWindow();

public:
	MorphologicalProcesser(std::string imageName, int option, double value);
	~MorphologicalProcesser();

	void processImage();
};