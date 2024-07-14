#ifndef DELAY_HPP
#define DELAY_HPP

#include <vector>

class Delay {
public:
    Delay(size_t buffer_size);

    void write(float input);
    float read(float delay_time, float sample_rate);
    void clear();

private:
    std::vector<float> buffer;
    size_t write_index;
};

#endif // DELAY_HPP