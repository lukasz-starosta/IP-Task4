#include "FrequencyProcesser.h"
#include <iostream>
#include <chrono>
#include <FrequencyProcesser.h>


using namespace std;

FrequencyProcesser::FrequencyProcesser()
{
}

FrequencyProcesser::FrequencyProcesser(std::string imageName, int option, double value) : Processer(imageName, option,
                                                                                                    value)
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
            cout << "fndft" << endl;
            break;
        case sidft:
            slowNormalDFT();
            cout << "Slow Normal DFT computed." << endl;
            displayFourierPreview();
            cout << "Computing Slow Inverse DFT." << endl;
            slowInverseDFT();
            break;
        case fidft:
            cout << "fidft" << endl;
            break;
        case lpfilter:
            slowNormalDFT();
            transformFinalMatrixToVisualisationMatrix();
            displayFourierPreview();
            lowPassFilter(value);
            displayFourierPreview();
            transformVisualisationMatrixToFinalMatrix();
            slowInverseDFT();
            break;
        case hpfilter:
            cout << "hpfilter" << endl;
            break;
        case bpfilter:
            cout << "bpfilter" << endl;
            break;
        case bcfilter:
            cout << "bcfilter" << endl;
            break;
        case hpedgefilter:
            cout << "hpedgefilter" << endl;
            break;
        case pmfilter:
            cout << "pmfilter" << endl;
            break;
        default:
            break;
    }
    chrono::high_resolution_clock::time_point t2 = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::microseconds>(t2 - t1).count() / (double) 1000000;
    cout << "Algorithm duration: " << duration << " seconds";

    if (option == sndft || option == fndft)
    {
        if (value == 1) {
            image = getFourierVisualisation();
        } else {
            image = getFourierLogarithmicVisualisation();
        }
    }

    image.save("processedImage.bmp");
    image.display("Processed image", false);
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
    if (value == 1) {
        getFourierVisualisation().display("DFT preview", false);
    } else {
        getFourierLogarithmicVisualisation().display("DFT preview", false);
    }
}
