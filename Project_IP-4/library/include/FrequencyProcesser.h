#ifndef IP_TASK_4_217862_217846_FREQUENCYPROCESSER_H
#define IP_TASK_4_217862_217846_FREQUENCYPROCESSER_H
#include "Processer.h"
#include "complex"

class FrequencyProcesser : public Processer
{
private:
    enum Options
    {
        sndft = 36, fndft, sidft, fidft, lpfilter, hpfilter, bpfilter, bcfilter, hpedgefilter, pmfilter
    };

    std::complex<double> **firstMatrix;
    std::complex<double> **finalMatrix;

    cimg_library::CImg<unsigned char> getFourierVisualisation();
    cimg_library::CImg<unsigned char> getFourierLogarithmicVisualisation();

    void slowNormalDFT();
    void initializeMatrices();
public:
    FrequencyProcesser();
    FrequencyProcesser(std::string imageName, int option, double value);
    ~FrequencyProcesser() override;
    void processImage();
};


#endif //IP_TASK_4_217862_217846_FREQUENCYPROCESSER_H
