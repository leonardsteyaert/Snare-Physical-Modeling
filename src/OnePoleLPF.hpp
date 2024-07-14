#ifndef ONEPOLELPF_HPP
#define ONEPOLELPF_HPP

class OnePoleLPF {
public:
    OnePoleLPF();

    void setCutoff_lpf(float cutoff, float sample_rate);
    float process_lpf(float input);

private:
    float a0, b1;  // Filter coefficients
    float z1;      // Filter state
};

#endif // ONEPOLELPF_HPP