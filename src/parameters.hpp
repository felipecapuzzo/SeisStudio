#pragma once

#include <iostream>
#include <cmath>

struct SimulationParameters
{
    float dt; // Time step in seconds
    float tmax;
    size_t boundary_size = 100;
    bool free_surface = false;

    size_t n_time_steps() const
    {
        return static_cast<size_t>(std::llround(static_cast<double>(tmax) / static_cast<double>(dt)));
    }
   
   
};
