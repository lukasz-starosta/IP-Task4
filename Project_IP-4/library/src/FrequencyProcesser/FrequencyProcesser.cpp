#include "FrequencyProcesser.h"
#include <iostream>
#include <chrono>
#include <FrequencyProcesser.h>


using namespace std;
FrequencyProcesser::FrequencyProcesser()
{
}

FrequencyProcesser::FrequencyProcesser(std::string imageName, int option, double value) : Processer(imageName, option, value)
{

}

FrequencyProcesser::~FrequencyProcesser()
{

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

    chrono::high_resolution_clock::time_point t1 = chrono::high_resolution_clock::now();
    int channel = (int)value;
    switch (option)
    {
        case sndft:
            slowNormalDFT();
            break;
        case fndft:
            cout << "fndft" << endl;
            break;
        case sidft:
            cout << "sidft" << endl;
            break;
        case fidft:
            cout << "fidft" << endl;
            break;
        case lpfilter:
            cout << "lpfilter" << endl;
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
    auto duration = chrono::duration_cast<chrono::microseconds>(t2 - t1).count() / (double)1000000;
    cout << "Algorithm duration: " << duration << " seconds";
    image.save("processedImage.bmp");
    image.display("Processed image", false);
}

