#include "OnePoleHPF.hpp"
#include <cmath>

OnePoleHPF::OnePoleHPF()
    : a0(0.0f), b1(0.0f), z1(0.0f) {}

void OnePoleHPF::setCutoff_hpf(float cutoff, float sample_rate) {
    float x = std::exp(-2.0f * M_PI * cutoff / sample_rate);
    a0 = (1.0f + x) / 2.0f;
    b1 = -x;
}

float OnePoleHPF::process_hpf(float input) {
    float output = a0 * (input - z1);
    z1 = input + output * b1;
    return output;
}