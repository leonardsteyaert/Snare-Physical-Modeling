#ifndef ONEPOLEHPF_HPP
#define ONEPOLEHPF_HPP

class OnePoleHPF {
public:
    OnePoleHPF();

    void setCutoff_hpf(float cutoff, float sample_rate);
    float process_hpf(float input);

private:
    float a0, b1;  // Filter coefficients
    float z1;      // Filter state
};

#endif // ONEPOLEHPF_HPP