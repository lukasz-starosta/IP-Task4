#include "FrequencyProcesser.h"


void FrequencyProcesser::transformFinalMatrixToVisualisationMatrix()
{
    for (int y = 0; y < height / 2; y++)
    {
        for (int x = 0; x < width / 2; x++)
        {
            // top-left is bottom-right
            visualisationMatrix[y][x] = finalMatrix[height / 2 + y][width / 2 + x];
            // bottom-right is top-left
            visualisationMatrix[height / 2 + y][width / 2 + x] = finalMatrix[y][x];
            // top-right is bottom - left
            visualisationMatrix[y][width / 2 + x] = finalMatrix[height / 2 + y][x];
            // bottom-left is top-right
            visualisationMatrix[height / 2 + y][x] = finalMatrix[y][width / 2 + x];
        }
    }
}


void FrequencyProcesser::transformVisualisationMatrixToFinalMatrix()
{
    for (int y = 0; y < height / 2; y++)
    {
        for (int x = 0; x < width / 2; x++)
        {
            // top-left is bottom-right
            finalMatrix[y][x] = visualisationMatrix[height / 2 + y][width / 2 + x];
            // bottom-right is top-left
            finalMatrix[height / 2 + y][width / 2 + x] = visualisationMatrix[y][x];
            // top-right is bottom - left
            finalMatrix[y][width / 2 + x] = visualisationMatrix[height / 2 + y][x];
            // bottom-left is top-right
            finalMatrix[height / 2 + y][x] = visualisationMatrix[y][width / 2 + x];
        }
    }
}

cimg_library::CImg<unsigned char> FrequencyProcesser::getFourierVisualisation()
{
    transformFinalMatrixToVisualisationMatrix();

    cimg_library::CImg<unsigned char> visualisation(width, height, 1, 3, 0);

    // In the final matrix, the DC component at [0][0] has the highest value
    double max = std::abs(finalMatrix[0][0]);
    unsigned char value;

    // Normalize the points to an interval <0, 255> so that they can be displayed and append them so that the [0][0] value is in the center
    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            value = (unsigned char) ((std::abs(visualisationMatrix[y][x]) / max) * 255);
            //Assigns the values of the pixels to the image while swapping the places of quadrants (so that the DC is in the middle)
            for (int i = 0; i < 3; i++)
            {
                // top-left is bottom-right
                visualisation(x, y, i) = value;
            }
        }
    }

    return visualisation;
}

cimg_library::CImg<unsigned char> FrequencyProcesser::getFourierLogarithmicVisualisation()
{
    transformFinalMatrixToVisualisationMatrix();

    cimg_library::CImg<unsigned char> visualisation(width, height, 1, 3, 0);

    unsigned char value;

    // Normalize the points to an interval <0, 255> so that they can be displayed and append them so that the [0][0] value is in the center
    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            value = (unsigned char) (std::log(std::abs(visualisationMatrix[y][x]) + 1));
            //Assigns the values of the pixels to the image while swapping the places of quadrants (so that the DC is in the middle)
            for (int i = 0; i < 3; i++)
            {
                // top-left is bottom-right
                visualisation(x, y, i) = value;
            }
        }
    }

    return visualisation;
}

