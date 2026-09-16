#pragma once

#include <iostream>

class SourceTimeFunction
{
    public:
    SourceTimeFunction(){};
    ~SourceTimeFunction();
    static SourceTimeFunction Ricker(size_t n_samples, float dt_src, float frequency_cut, float time_delay);

    float* signal = nullptr;
    float dt_src;
    size_t n_samples;

};