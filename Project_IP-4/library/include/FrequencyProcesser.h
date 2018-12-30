#ifndef IP_TASK_4_217862_217846_FREQUENCYPROCESSER_H
#define IP_TASK_4_217862_217846_FREQUENCYPROCESSER_H
#include "Processer.h"

class FrequencyProcesser : public Processer
{
private:
    enum Options
    {
        sndft = 36, fndft, sidft, fidft, lpfilter, hpfilter, bpfilter, bcfilter, hpedgefilter, pmfilter
    };

public:
    FrequencyProcesser();
    FrequencyProcesser(std::string imageName, int option, double value);
    ~FrequencyProcesser();
    void processImage();
};


#endif //IP_TASK_4_217862_217846_FREQUENCYPROCESSER_H
