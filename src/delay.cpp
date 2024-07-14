#include "delay.hpp"
#include <cstring> // for memset
#include <cmath> // for floor
#include <algorithm> // for clamp

Delay::Delay(size_t buffer_size)
    : buffer(buffer_size, 0.0f), write_index(0) {}

void Delay::write(float input) {
    buffer[write_index] = input;
    write_index = (write_index + 1) % buffer.size();
}

float Delay::read(float delay_time_ms, float sample_rate)  {
    size_t buffer_size = buffer.size();

    // Convert delay time from milliseconds to seconds
    float delay_time_sec = delay_time_ms / 1000.0f;

    // Calculate the read position
    float read_index = static_cast<float>(write_index) - delay_time_sec * sample_rate;
    if (read_index < 0) {
        read_index += buffer_size;
    }

    // Get the integer and fractional part of the read position
    int int_index = static_cast<int>(std::floor(read_index));
    float frac = read_index - int_index;

    // Get the samples for interpolation
    float y0 = buffer[(int_index - 1 + buffer_size) % buffer_size];
    float y1 = buffer[int_index % buffer_size];
    float y2 = buffer[(int_index + 1) % buffer_size];
    float y3 = buffer[(int_index + 2) % buffer_size];

    // Perform the four-point interpolation
    float c0 = y1;
    float c1 = 0.5f * (y2 - y0);
    float c2 = y0 - 2.5f * y1 + 2.0f * y2 - 0.5f * y3;
    float c3 = 0.5f * (y3 - y0) + 1.5f * (y1 - y2);

    return ((c3 * frac + c2) * frac + c1) * frac + c0;
}

void Delay::clear() {
    std::memset(buffer.data(), 0, buffer.size() * sizeof(float));
    write_index = 0;
}