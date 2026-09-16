#include "geometry.hpp"
#include "seismogram.hpp"
#include <iostream>
#include <cmath>

void record_receivers(const float *u,
                      Seismogram &seismogram,
                      const ReceiverPosition &receivers,
                      const ComputationalGrid &grid,
                      size_t n)
{
    for (size_t r = 0; r < receivers.size(); r++)
    {
        if (!seismogram.valid_index(n, r))
            continue;

        const size_t ind_r = grid.index(receivers[r]);
        // const size_t ind_sismo = it + r*nt;

        seismogram.data[seismogram.index(n, r)] = u[ind_r];
    }
}

void inject_source(float *u, const SourcePosition &sources, const SourceTimeFunction &wavelet, const ComputationalGrid &grid, float dt, size_t n)
{
    for (size_t s = 0; s < sources.size(); s++)
    {
        const size_t ind_s = grid.index(sources[s]);

        u[ind_s] += wavelet.signal[n] * dt * dt;
    }
}

size_t get_ratio(float a, float b)
{
    float ratio = a / b;
    size_t int_ratio = static_cast<size_t>(std::round(ratio));
    return (int_ratio);
}

float kaiser_window_weight(float x, float r)
{
    if (std::abs(x) <= r)
    {
        float aux = 6.31f * std::sqrt(1 - (x / r) * (x / r));
        return static_cast<float>(std::cyl_bessel_i(0, aux) /
                                  std::cyl_bessel_i(0, 6.31f));
    }

    return 0.0f;
}

float hicks_weight(float x, float r)
{
    float sinc;
    float pi = 3.14159265358979323846f;

    if (std::abs(x) < 1e-6f)
    {
        sinc = 1.0f;
    }
    else
    {
        sinc = std::sin(x * pi) / (x * pi);
    }

    return (sinc * kaiser_window_weight(x, r));
}
