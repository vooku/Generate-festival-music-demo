#pragma once

#include <vector>
#include <chrono>
#include <random>
#include <sndfile.h>

class Granulizer
{
public:
    typedef std::pair<float, float> Frame;

    Granulizer();

    bool loadLoop(char* filename);
    
    const Frame& grainedFrame();
    const Frame& straightFrame();

    void setGrainLen(float grainLen) { grainLen_ = grainLen; }
    float getGrainLen() const { return grainLen_; }
    float getLoopLen() const { return (float)loop_.size() / (sfInfo_.samplerate * sfInfo_.channels); }

private:
    void newGrain();

    std::minstd_rand0 generator;  // minstd_rand0 is a standard linear_congruential_engine

    SF_INFO sfInfo_;
    std::vector<float> loop_;
    int grainStart_;
    int grainIdx_;
    int straightIdx_;
    float grainLen_;

};

