#include "OnePoleLPF.hpp"
#include <cmath>

OnePoleLPF::OnePoleLPF()
    : a0(0.0f), b1(0.0f), z1(0.0f) {}

void OnePoleLPF::setCutoff_lpf(float cutoff, float sample_rate) {
    float x = std::exp(-2.0f * M_PI * cutoff / sample_rate);
    a0 = 1.0f - x;
    b1 = x;
}

float OnePoleLPF::process_lpf(float input) {
    z1 = input * a0 + z1 * b1;
    return z1;
}