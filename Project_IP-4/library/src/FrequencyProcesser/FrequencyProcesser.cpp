#include "FrequencyProcesser.h"
#include <iostream>
#include <chrono>
#include <FrequencyProcesser.h>


using namespace std;

FrequencyProcesser::FrequencyProcesser()
{
}

FrequencyProcesser::FrequencyProcesser(std::string imageName, int option, double value, int secondValue) : Processer(imageName, option, value),
                                                                                                           secondValue(secondValue)
{
}

FrequencyProcesser::~FrequencyProcesser()
{
    // Cleanup
    for (int i = 0; i < height; i++)
    {
        delete[] firstMatrix[i];
        delete[] finalMatrix[i];
        delete[] visualisationMatrix[i];
    }
    delete[] firstMatrix;
    delete[] finalMatrix;
    delete[] visualisationMatrix;
}

void FrequencyProcesser::processImage()
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

    // Allocate memory for the matrices needed to perform the fourier transforms
    initializeMatrices();

    chrono::high_resolution_clock::time_point t1 = chrono::high_resolution_clock::now();
    switch (option)
    {
        case sndft:
            slowNormalDFT();
            break;
        case fndft:
            NormalFFT();
            break;
        case sidft:
            slowNormalDFT();
            cout << "Slow Normal DFT computed." << endl;
            displayFourierPreview();
            cout << "Computing Slow Inverse DFT." << endl;
            slowInverseDFT();
            //InverseFFT();
            break;
        case fidft:
            NormalFFT();
            cout << "Fast Fourier Transform Computed." << endl;
            displayFourierPreview();
            cout << "Computing Fast Inverse Fourier Transform." << endl;
            InverseFFT();
            break;
        case lpfilter:
            slowNormalDFT();
            transformFinalMatrixToVisualisationMatrix();
            //Optional for viewing step by step
            //displayFourierPreview();
            lowPassFilter(value);
            transformVisualisationMatrixToFinalMatrix();
            //displayFourierPreview();
            slowInverseDFT();
            break;
        case hpfilter:
            slowNormalDFT();
            transformFinalMatrixToVisualisationMatrix();
            //Optional for viewing step by step
            //displayFourierPreview();
            highPassFilter(value);
            transformVisualisationMatrixToFinalMatrix();
            //displayFourierPreview();
            slowInverseDFT();
            break;
        case bpfilter:
            NormalFFT();
            transformFinalMatrixToVisualisationMatrix();
            bandPassFilter(value, secondValue);
            transformVisualisationMatrixToFinalMatrix();
            InverseFFT();
            break;
        case bcfilter:
            NormalFFT();
            transformFinalMatrixToVisualisationMatrix();
            bandCutFilter(value, secondValue);
            transformVisualisationMatrixToFinalMatrix();
            InverseFFT();
            break;
        case hpedgefilter:
            mask = getMaskFromUser();
            mask.display("Mask", false);
//			NormalFFT();
//			transformFinalMatrixToVisualisationMatrix();
//            displayFourierPreview();
//			HPFEdge(value, secondValue);
//			transformVisualisationMatrixToFinalMatrix();
//			InverseFFT();
            break;
        case pmfilter:
            NormalFFT();
            transformFinalMatrixToVisualisationMatrix();
            PhaseFilter(value, secondValue);
            transformVisualisationMatrixToFinalMatrix();
            InverseFFT();
            break;
        default:
            break;
    }

    chrono::high_resolution_clock::time_point t2 = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::microseconds>(t2 - t1).count() / (double) 1000000;
    cout << "Algorithm duration: " << duration << " seconds";

    if (option == sndft || option == fndft)
    {
        // Visualisation
        transformFinalMatrixToVisualisationMatrix();
        if (value == 1)
        {
            image = getFourierVisualisation();
        } else
        {
            image = getFourierLogarithmicVisualisation();
        }
    }

//    image.save("processedImage.bmp");
//    image.display("Processed image", false);
}

void FrequencyProcesser::initializeMatrices()
{
    // Initialize the first matrix of the same size as the image
    firstMatrix = new std::complex<double> *[height];
    // Initialize the final matrix of the same size as the image
    finalMatrix = new std::complex<double> *[height];
    // Initialize the matrix used for visualisation of the same size as the image
    visualisationMatrix = new std::complex<double> *[height];
    // Initialize columns of the matrices
    for (int row = 0; row < height; row++)
    {
        firstMatrix[row] = new std::complex<double>[width];
        finalMatrix[row] = new std::complex<double>[width];
        visualisationMatrix[row] = new std::complex<double>[width];
    }
}

void FrequencyProcesser::displayFourierPreview()
{
    transformFinalMatrixToVisualisationMatrix();
    if (value == 1)
    {
        getFourierVisualisation().display("DFT preview", false);
    } else
    {
        getFourierLogarithmicVisualisation().display("DFT preview", false);
    }
}


int truncateCoord(int coord, int limit)
{
    if (coord >= limit) coord = limit - 1;
    else if (coord < 0) coord = 0;
    return coord;
}

cimg_library::CImg<unsigned char> FrequencyProcesser::getMaskFromUser()
{
    double radius, angleInRadians, distance;
    short int angle;
    double spreadIncrement = 0.2;

    cimg_library::CImg<unsigned char> maskImage(width, height, 1, 3, 0);
    cout << "Please create a mask you would like to use." << endl;
    cout << "Radius: ";
    cin >> radius;
    cout << "Angle in degrees: ";
    cin >> angle;

    angleInRadians = angle * 3.14 / 180;

    cout << "Spread coefficient: ";
    cin >> spreadIncrement;

    int halfHeight = height / 2, halfWidth = width / 2;
    int x1 = halfWidth, x2, y1 = halfHeight, y2, opposite_x1 = halfWidth, opposite_x2, opposite_y1 = halfHeight, opposite_y2;

    if ((angle >= 0 && angle <= 90) || (angle >= 180 && angle <= 270))
    {
        x2 = width - 1;
        y2 = truncateCoord((int) (tan(angleInRadians) * halfWidth), height);

        opposite_x2 = 0;
        opposite_y2 = truncateCoord((int) (height - tan(angleInRadians) * halfWidth), height);

        drawLine(&maskImage, x1, x2, y1, y2, spreadIncrement, true);
        drawLine(&maskImage, opposite_x1, opposite_x2, opposite_y1, opposite_y2, spreadIncrement, true);
    } else if ((angle > 90 && angle < 180) || (angle > 270 && angle <= 360))
    {
        angleInRadians = (angle - 90) * 3.14 / 180;
        x2 = truncateCoord((int) (width - tan(angleInRadians) * halfHeight), width);
        y2 = height - 1;

        opposite_x2 = truncateCoord((int) (tan(angleInRadians) * halfHeight), width);
        opposite_y2 = 0;

        drawLine(&maskImage, x1, x2, y1, y2, spreadIncrement, false);
        drawLine(&maskImage, opposite_x1, opposite_x2, opposite_y1, opposite_y2, spreadIncrement, false);
    }
    else {
        cout << "Wrong angle. Only a circle will be generated." << endl;
    }

    for (int row = 0; row < height; row++)
    {
        for (int column = 0; column < width; column++)
        {
            distance = std::sqrt(std::pow(row - halfHeight, 2) + std::pow(column - halfWidth, 2));
            if (distance < radius)
            {
                for (int channel = 0; channel < 3; channel++)
                {
                    maskImage(column, row, channel) = 0;
                }
            }
        }
    }

    return maskImage;
}

void FrequencyProcesser::drawLine(cimg_library::CImg<unsigned char> *maskImage, int x1, int x2, int y1, int y2, double spreadIncrement, bool isSpreadingY)
{
    unsigned char COLOR_WHITE = (unsigned char) 255;
    double spread = 1;

    int dx = x2 - x1;
    int dy = y2 - y1;

    // calculate steps required for generating pixels
    int steps = abs(dx) > abs(dy) ? abs(dx) : abs(dy);

    // calculate increment in x & y for each steps
    float xIncrement = dx / (float) steps;
    float yIncrement = dy / (float) steps;

    // Put pixel for each step
    float x = (float)x1;
    float y = (float)y1;

    unsigned int channel;
    for (int i = 0; i <= steps; i++)
    {
        for (int s = 0; s < spread; s++)
        {
            for (channel = 0; channel < 3; channel++)
            {
                if (isSpreadingY)
                {
                    if (y + s < height)
                        (*maskImage)((unsigned int)x, (unsigned int)(y + s), channel) = COLOR_WHITE;
                    if (y - s > 0)
                        (*maskImage)((unsigned int)x, (unsigned int)(y - s), channel) = COLOR_WHITE;
                } else
                {
                    if (x + s < width)
                        (*maskImage)((unsigned int)(x + s), (unsigned int)y, channel) = COLOR_WHITE;
                    if (x - s > 0)
                        (*maskImage)((unsigned int)(x - s), (unsigned int)y, channel) = COLOR_WHITE;
                }
            }

        }
        x += xIncrement;
        y += yIncrement;
        spread += spreadIncrement;
    }
}