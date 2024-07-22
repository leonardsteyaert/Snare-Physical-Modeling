#include "OnePoleLPF.hpp"
#include <cmath>

OnePoleLPF::OnePoleLPF()
    : feedback(0.0f), coeff(0.0f), z1(0.0f) {}

void OnePoleLPF::setCutoff_lpf(float cutoff, float sample_rate) {
    /*float x = std::exp(-2.0f * M_PI * cutoff / sample_rate);
    a0 = 1.0f - x;
    b1 = x;*/

    coeff= cutoff* (2*3.14159)/sample_rate;
    feedback = 1.0f - coeff;


}

float OnePoleLPF::process_lpf(float input) {
    z1 = input * coeff + z1 * feedback;
    return z1;
}