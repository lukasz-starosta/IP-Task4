#include "FrequencyProcesser.h"

cimg_library::CImg<unsigned char> FrequencyProcesser::getFourierVisualisation()
{
    cimg_library::CImg<unsigned char> visualisation(width, height, 1, 3, 0);

    // In the final matrix, the DC component at [0][0] has the highest value
    double max = std::abs(finalMatrix[0][0]);
    unsigned char value1, value2, value3, value4;

    // Normalize the points to an interval <0, 255> so that they can be displayed and append them so that the [0][0] value is in the center
    for (int y = 0; y < height / 2; y++)
    {
        for (int x = 0; x < width / 2; x++)
        {
            value1 = (unsigned char) ((std::abs(finalMatrix[height / 2 + y][width / 2 + x]) / max) * 255);
            value2 = (unsigned char) ((std::abs(finalMatrix[y][x]) / max) * 255);
            value3 = (unsigned char) ((std::abs(finalMatrix[height / 2 + y][x]) / max) * 255);
            value4 = (unsigned char) ((std::abs(finalMatrix[y][width / 2 + x]) / max) * 255);

            //Assigns the values of the pixels to the image while swapping the places of quadrants (so that the DC is in the middle)
            for (int i = 0; i < 3; i++)
            {
                // top-left is bottom-right
                visualisation(x, y, i) = value1;
                // bottom-right is top-left
                visualisation(width / 2 + x, height / 2 + y, i) = value2;
                // top-right is bottom - left
                visualisation(width / 2 + x, y, i) = value3;
                // bottom-left is top-right
                visualisation(x, height / 2 + y, i) = value4;
            }
        }
    }

    return visualisation;
}

cimg_library::CImg<unsigned char> FrequencyProcesser::getFourierLogarithmicVisualisation()
{
    cimg_library::CImg<unsigned char> visualisation(width, height, 1, 3, 0);

    // In the final matrix, the DC component at [0][0] has the highest value
    unsigned char value1, value2, value3, value4;

    // Normalize the points to an interval <0, 255> so that they can be displayed and append them so that the [0][0] value is in the center
    for (int y = 0; y < height / 2; y++)
    {
        for (int x = 0; x < width / 2; x++)
        {
            //Transform to a logarithmic scale
            value1 = (unsigned char) std::log(std::abs(finalMatrix[height / 2 + y][width / 2 + x]) + 1);
            value2 = (unsigned char) std::log(std::abs(finalMatrix[y][x]) + 1);
            value3 = (unsigned char) std::log(std::abs(finalMatrix[height / 2 + y][x]) + 1);
            value4 = (unsigned char) std::log(std::abs(finalMatrix[y][width / 2 + x]) + 1);
            //Assigns the values of the pixels to the image while swapping the places of quadrants (so that the DC is in the middle)
            for (int i = 0; i < 3; i++)
            {
                // top-left is bottom-right
                visualisation(x, y, i) = value1;
                // bottom-right is top-left
                visualisation(width / 2 + x, height / 2 + y, i) = value2;
                // top-right is bottom - left
                visualisation(width / 2 + x, y, i) = value3;
                // bottom-left is top-right
                visualisation(x, height / 2 + y, i) = value4;
            }
        }
    }

    return visualisation;
}

