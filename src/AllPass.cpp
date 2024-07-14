#include "AllPass.hpp"

AllPass::AllPass(float max_delay_length)
    : delayTime(0.0f), theta(0.0f), delay(max_delay_length*44100.f/1000.f) {}


void AllPass::setDelayTime_allpass(float delay_time_input) {
    delayTime = delay_time_input;
}

void AllPass::setTheta_allpass(float theta_input) {
    theta = theta_input;
}

float AllPass::process_allpass(float input, float delay_time_input,float theta_input, float sample_rate) {
    // Get Input
    float a = input;

    // Get Output of Delay Line , and Delay Time
    setDelayTime_allpass(delay_time_input);
    float b = delay.read(delayTime, sample_rate);

    // Get Theta
    setTheta_allpass(theta_input);

    // Calculate first COS 
    float c = std::cos(2.f*M_PI*theta*(-0.25f));

    // Calculate second COS
    float d = std::cos(2.f*M_PI*theta);

    // Output of 1st Multiplier
    float e = a*d;

    // Output of 2nd Multiplier
    float f = b*c;

    // Output of 3rd Multiplier
    float g = a*c;

    // Output of 4th Multiplier
    float h = b*d;

    // Output of Subtraction
    float i = e-f;

    // Output of Addition 
    float j = g+h;

    // Write in Delay Line
    delay.write(j);

    // Return Output
    return i;
}