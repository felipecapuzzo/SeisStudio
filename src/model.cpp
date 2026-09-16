#include "model.hpp"
#include "geometry.hpp"

AcousticModel::AcousticModel(const ComputationalGrid &grid) : grid(grid)
{
    vp = new float[grid.nz * grid.nx]();
}

void AcousticModel::fill(float value)
{
   for (size_t i = 0; i < grid.nz * grid.nx; ++i)
   {
       vp[i] = value;
   }
}

AcousticModel::~AcousticModel()
{
    delete[] vp;
}

ElasticModel::ElasticModel(const ComputationalGrid &grid) : grid(grid)
{
    vp = new float[grid.nz * grid.nx]();
    vs = new float[grid.nz * grid.nx]();
    rho = new float[grid.nz * grid.nx]();
}

ElasticModel::~ElasticModel()
{
    delete[] vp;
    delete[] vs;
    delete[] rho;
}
