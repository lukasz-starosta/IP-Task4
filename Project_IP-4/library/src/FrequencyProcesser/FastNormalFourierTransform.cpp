#include"FrequencyProcesser.h"
#include <math.h>
#include <vector>

#define M_PI 3.14159265358979323846 

//void oneDimensionalFFTByRow(cimg_library::CImg<unsigned char> *image, std::complex<double> **fourierMatrix, int height, int width)
//{
//	cimg_library::CImg<unsigned char> rowData;
//	std::complex<double> evenSum(0.0, 0.0);
//	std::complex<double> oddSum(0.0, 0.0);
//	
//	for (int row = 0; row < height; row++)
//	{
//		rowData = image->get_row(row);
//		for (int k = 0; k < (double)width / 2.0; k++)
//		{
//			for (int n = 0; n < (double)width / 2.0; n++)
//			{
//				std::complex<double> W(cos(2.0 * M_PI * n * k / ((double)width / 2.0)), -sin(2.0 * M_PI * n * k / ((double)width / 2.0)));
//
//				//For even numbers
//				evenSum += (double)rowData[2 * n] * W;
//
//				//For odd numbers
//				oddSum += (double)rowData[2 * n + 1] * W;
//
//			}
//			std::complex<double> R(cos(2.0 * M_PI * k / (double)width), -sin(2.0 * M_PI * k / (double)width));
//			
//			fourierMatrix[row][k] = evenSum + (oddSum * R);
//			fourierMatrix[row][(width / 2) + k] = evenSum - (oddSum * R);
//			evenSum.real(0.0);
//			evenSum.imag(0.0);
//			oddSum.real(0.0);
//			oddSum.imag(0.0);
//		}
//	}
//}

//void oneDimensionalFFTByColumn(std::complex<double> **fourierMatrix, std::complex<double> **finalMatrix, int height, int width)
//{
//	std::complex<double> evenSum(0.0, 0.0);
//	std::complex<double> oddSum(0.0, 0.0);
//	
//	for (int column = 0; column < width; column++)
//	{
//		for (int k = 0; k < (double)height / 2.0; k++)
//		{
//			for (int n = 0; n < (double)height / 2.0; n++)
//			{
//				std::complex<double> W(cos(2.0 * M_PI * n * k / ((double)height / 2.0)), -sin(2.0 * M_PI * n * k / ((double)height / 2.0)));
//
//				//For even numbers
//				evenSum += fourierMatrix[2 * n][column] * W;
//
//				//For odd numbers
//				oddSum += fourierMatrix[2 * n + 1][column] * W;
//				//std::cout << oddSum << std::endl;
//			}
//			std::complex<double> R(cos(2.0 * M_PI * k / (double)width), -sin(2.0 * M_PI * k / (double)width));
//
//			finalMatrix[k][column] = evenSum + (oddSum * R);
//			finalMatrix[(height / 2) + k][column] = evenSum - (oddSum * R);
//
//			evenSum.real(0.0);
//			evenSum.imag(0.0);
//			oddSum.real(0.0);
//			oddSum.imag(0.0);
//		}
//	}
//}

std::vector<std::complex<double>> split(std::vector<std::complex<double>> row)
{
	//stops recursive operations when vector is finally 1
	if (row.size() == 1) return row;

	int half = row.size() / 2;
	std::vector <std::complex<double>> first(half);
	std::vector<std::complex<double>> second(half);

	//divides vector into two half-sized vectors
	for (int i = 0; i < half; i++)
	{
		std::complex<double> W(cos(2 * M_PI * i / row.size()), -sin(2 * M_PI * i / row.size()));
		first[i] = row[i] + row[half + i];
		second[i] = (row[i] - row[half + i]) * W;
	}

	std::vector <std::complex<double>> partition1 = split(first);
	std::vector<std::complex<double>> partition2 = split(second);

	//orders the vector in right way
	for (int k = 0; k < half; k++)
	{
		row[2 * k] = partition1[k];
		row[2 * k + 1] = partition2[k];
	}

	return row;
}

void fft(cimg_library::CImg<unsigned char> *image, std::complex<double> **fourierMatrix, std::complex<double> **finalMatrix, double height, double width)
{
	cimg_library::CImg<unsigned char> imageData;
	std::vector<std::complex<double>> rowData(width);
	std::vector<std::complex<double>> columnData(height);
	
	//for rows
	for (int row = 0; row < height; row++)
	{
		imageData = image->get_row(row);
		for (int index = 0; index < width; index++)
		{
			rowData[index] = imageData[index];
		}
		rowData = split(rowData);
		for (int i = 0; i < width; i++)
		{
			fourierMatrix[row][i] = rowData[i];
		}
	}

	//for columns
	for (int column = 0; column < width; column++)
	{
		for (int index = 0; index < height; index++)
		{
			columnData[index] = fourierMatrix[index][column];
		}
		columnData = split(columnData);
		for (int i = 0; i < width; i++)
		{
			finalMatrix[i][column] = columnData[i];
		}
	}
}



void FrequencyProcesser::NormalFFT()
{
	fft(&image, firstMatrix, finalMatrix, height, width);
}