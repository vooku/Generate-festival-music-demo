#include "Granulizer.h"
#include <iostream>

Granulizer::Granulizer()
    : grainIdx_(0),
      straightIdx_(0),
      grainLen_(0.0f),
      generator(std::chrono::system_clock::now().time_since_epoch().count())
{
}

bool Granulizer::loadLoop(char* filename)
{
    grainIdx_ = 0;
    grainLen_ = 0;

    SNDFILE* sfFile = sf_open(filename, SFM_READ, &sfInfo_);
    if (sfFile) {
        sf_command(sfFile, SFC_SET_NORM_FLOAT, NULL, SF_TRUE);
        loop_.resize(sfInfo_.frames * sfInfo_.channels);
        sf_read_float(sfFile, loop_.data(), sfInfo_.frames * sfInfo_.channels);
        sf_close(sfFile);
        std::cout << "Successfully loaded " << filename << "." << std::endl;
        grainLen_ = float(sfInfo_.frames) / sfInfo_.samplerate * 1000.0f;
        return true;
    }
    else {
        std::cerr << "Error: cannot open " << filename << "." << std::endl;
    }

    return false;
}

const Granulizer::Frame& Granulizer::grainedFrame()
{
    Frame frame;

    frame.first = loop_[(grainStart_ + grainIdx_) % loop_.size()];
    frame.second = loop_[(grainStart_ + grainIdx_ + 1) % loop_.size()];
    grainIdx_ += 2;

    if (grainIdx_ > grainLen_ * sfInfo_.samplerate) {
        newGrain();
    }

    return frame;
}

const Granulizer::Frame& Granulizer::straightFrame()
{
    Frame frame;

    frame.first = loop_[straightIdx_++];
    frame.second = loop_[straightIdx_++];
    straightIdx_ %= loop_.size();

    return frame;
}

void Granulizer::newGrain()
{
    grainIdx_ = 0;
    grainStart_ = (generator() % int(sfInfo_.frames * 0.5 * sfInfo_.channels)) * 2;
}
