#ifndef IP_TASK_4_217862_217846_FREQUENCYPROCESSER_H
#define IP_TASK_4_217862_217846_FREQUENCYPROCESSER_H
#include "Processer.h"
#include "complex"

class FrequencyProcesser : public Processer
{
private:
	int secondValue;

    enum Options
    {
        sndft = 36, fndft, sidft, fidft, lpfilter, hpfilter, bpfilter, bcfilter, hpedgefilter, pmfilter
    };

	cimg_library::CImg<unsigned char> mask;
	void drawLine(cimg_library::CImg<unsigned char> *maskImage, int x1, int x2, int y1, int y2, double spreadIncrement, bool isSpreadingY);

    std::complex<double> **firstMatrix;
    std::complex<double> **finalMatrix;
    std::complex<double> **visualisationMatrix;
    void initializeMatrices();
    void transformFinalMatrixToVisualisationMatrix();
    void transformVisualisationMatrixToFinalMatrix();

    cimg_library::CImg<unsigned char> getFourierVisualisation();
    cimg_library::CImg<unsigned char> getFourierLogarithmicVisualisation();
    void displayFourierPreview();
    cimg_library::CImg<unsigned char> getMaskFromUser();

    void slowNormalDFT();
    void slowInverseDFT();
	void NormalFFT();
	void InverseFFT();

	

    void lowPassFilter(double distanceFromOrigin);
    void highPassFilter(double distanceFromOrigin);
	void bandPassFilter(double bandwidth, double cutoff);
	void bandCutFilter(double bandwidth, double cutoff);
	void HPFEdge();
	void PhaseFilter(double k, double l);

public:
    FrequencyProcesser();
    FrequencyProcesser(std::string imageName, int option, double value, int secondValue);
    ~FrequencyProcesser() override;
    void processImage();
};


#endif //IP_TASK_4_217862_217846_FREQUENCYPROCESSER_H
