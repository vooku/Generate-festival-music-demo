#include "Oscillator.h"
#include "ofMath.h"

float Oscillator::noise()
{
    return ofRandom(-1, 1);
}

float Oscillator::sine(float frequency)
{
    // sin (n) seems to have trouble when n is very large, so we
    // keep phase in the range of 0-TWO_PI like this:
    while (phase_ > TWO_PI) {
        phase_ -= TWO_PI;
    }

    float sample = sin(phase_);
    phase_ += TWO_PI * frequency / sampleRate_;
    
    return sample;
}

float Oscillator::square(float frequency)
{
    return sine(frequency) > 0 ? 1.0f : -1.0f;
}

float Oscillator::triangle(float frequency)
{
    float sample = fabs(fmod(phase_, 4.0f) - 2.0f) - 1.0f;
    phase_ += TWO_PI * frequency / sampleRate_;

    return sample;
}
