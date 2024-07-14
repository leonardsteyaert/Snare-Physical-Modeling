#ifndef WHITE_NOISE_HPP
#define WHITE_NOISE_HPP

#include <random>

class WhiteNoise {
public:
    WhiteNoise();

    // Generate a white noise sample
    float generate();

private:
    std::mt19937 generator;
    std::uniform_real_distribution<float> distribution;
};

#endif // WHITE_NOISE_HPP
