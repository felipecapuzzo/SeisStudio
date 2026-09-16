#include "seismogram.hpp"

Seismogram::Seismogram(size_t nt, size_t n_rec, float dt)
    : n_samples(nt), n_rec(n_rec), dt_rec(dt)
{
    data = new float[nt * n_rec]();
}

Seismogram::~Seismogram()
{
    delete[] data;
}