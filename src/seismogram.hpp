#pragma once
#include <iostream>

class Seismogram
{
    public:
    Seismogram(size_t nt, size_t n_rec, float dt);
    ~Seismogram();

    size_t n_samples;
    size_t n_rec;
    float dt_rec;

    float* data = nullptr;
    bool valid_index(size_t it, size_t ir) const
    {
        return it < n_samples && ir < n_rec;
    }

    size_t index(size_t it, size_t ir) const { return it + n_samples * ir; };
};
