#pragma once

#include "geometry.hpp"

class AcousticWavefield
{
    public:
    explicit AcousticWavefield(const ComputationalGrid &grid);
      ~AcousticWavefield();

      void swap();
      
      float* current = nullptr;
      float* previous = nullptr;
      float* next = nullptr;
};
