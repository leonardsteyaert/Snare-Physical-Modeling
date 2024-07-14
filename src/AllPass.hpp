#ifndef ALLPASS_HPP
#define ALLPASS_HPP

#include "delay.hpp"

class AllPass {
public:
    AllPass(float max_delay_length);

    void setDelayTime_allpass(float delayTime);
    void setTheta_allpass(float theta);
    float process_allpass(float input, float delay_time_input,float theta_input, float sample_rate);

private:
    float delayTime;
    float theta;
    Delay delay;
};

#endif // ALLPASS_HPP