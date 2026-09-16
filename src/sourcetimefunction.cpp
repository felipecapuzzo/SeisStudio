#include <math.h>
#include "sourcetimefunction.hpp"

SourceTimeFunction SourceTimeFunction::Ricker(size_t n_samples, float dt_src, float frequency_cut, float time_delay)
{
    SourceTimeFunction src;
    src.n_samples = n_samples;
    src.dt_src = dt_src;
    src.signal = new float[n_samples]();
    constexpr float pi = 3.14159265358979323846f;
    float fc = frequency_cut / (3.0f * std::sqrt(pi));

    float td = 0;

    for (size_t i = 0; i < n_samples; ++i)
    {
        td = (i*dt_src) - time_delay;
        float aux = pi * fc * td;
        aux = aux * aux;
        src.signal[i] = (1.0f - 2.0f * pi * aux) * std::exp(-pi * aux);
    }

    return src;
}

SourceTimeFunction::~SourceTimeFunction()
{
    if (signal != nullptr) delete[] signal;
        
}