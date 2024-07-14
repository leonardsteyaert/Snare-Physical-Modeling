#include "WhiteNoise.hpp"

WhiteNoise::WhiteNoise()
    : generator(std::random_device{}()), distribution(-1.0f, 1.0f) {}

float WhiteNoise::generate() {
    return distribution(generator);
}