#include "wavefield.hpp"
#include "geometry.hpp"

AcousticWavefield::AcousticWavefield(const ComputationalGrid &grid)
{
    size_t n_points = grid.nx * grid.nz;
    current = new float[n_points]();
    previous = new float[n_points]();
    next = new float[n_points]();
}

AcousticWavefield::~AcousticWavefield()
{
    delete[] current;
    delete[] previous;
    delete[] next;
}

void AcousticWavefield::swap()
{
   std::swap(previous, current);
   std::swap(current, next);
}
