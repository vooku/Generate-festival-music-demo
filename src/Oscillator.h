#pragma once

class Oscillator
{
public:
    Oscillator() : phase_(0.0f) { };

    void setSampleRate(int sampleRate) { sampleRate_ = sampleRate; }
    int getSampleRate() const { return sampleRate_; };

    float noise();
    float sine(float frequency);
    float square(float frequency);
    float triangle(float frequency);

private:
    int sampleRate_;
    float phase_;

};
