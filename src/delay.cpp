#include <vector>
#include <cmath>
#include <cstring>

class Delay {
public:
    Delay(size_t buffer_size);

    void write(float input);
    float read(float delay_time_ms, float sample_rate);
    void clear();

private:
    std::vector<float> buffer;
    size_t write_index;
};

Delay::Delay(size_t buffer_size)
    : buffer(buffer_size, 0.0f), write_index(0) {}

void Delay::write(float input) {
    buffer[write_index] = input;
    write_index = (write_index + 1) % buffer.size();
}

float Delay::read(float delay_time_ms, float sample_rate) {
    size_t buffer_size = buffer.size();

    // Convert delay time from milliseconds to seconds
    float delay_time_sec = delay_time_ms / 1000.0f;

    // Calculate the read position in samples
    float read_index = static_cast<float>(write_index) - (delay_time_sec * sample_rate);

    // Wrap around the buffer if the read_index is negative
    if (read_index < 0) {
        read_index += buffer_size;
    }

    // Get the integer and fractional part of the read position
    int int_index = static_cast<int>(std::floor(read_index));
    float frac = read_index - int_index;

    // Wrap the integer index to be within buffer bounds
    int_index = (int_index + buffer_size) % buffer_size;

    // Get surrounding samples for interpolation
    int prev_index = (int_index - 1 + buffer_size) % buffer_size;
    int next_index = (int_index + 1) % buffer_size;
    int next_next_index = (int_index + 2) % buffer_size;

    // Perform the four-point interpolation
    float y0 = buffer[prev_index];
    float y1 = buffer[int_index];
    float y2 = buffer[next_index];
    float y3 = buffer[next_next_index];

    // Interpolation coefficients
    float c0 = y1;
    float c1 = 0.5f * (y2 - y0);
    float c2 = y0 - 2.5f * y1 + 2.0f * y2 - 0.5f * y3;
    float c3 = 0.5f * (y3 - y0) + 1.5f * (y1 - y2);

    // Calculate interpolated value
    return ((c3 * frac + c2) * frac + c1) * frac + c0;
}

void Delay::clear() {
    std::memset(buffer.data(), 0, buffer.size() * sizeof(float));
    write_index = 0;
}