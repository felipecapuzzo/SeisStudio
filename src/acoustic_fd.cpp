#include "acoustic_fd.hpp"
#include "model.hpp"
#include "wavefield.hpp"
#include "geometry.hpp"
#include <iostream>

void acoustic_fd_step(AcousticWavefield &u, const AcousticModel &model, const RegularGrid &grid, float dt)
{
    float dt2 = dt * dt;

    for (size_t x = 4; x < grid.nx - 4; x++)
    {
        for (size_t z = 4; z < grid.nz - 4; z++)
        {
            size_t ind = z + x * grid.nz;

            float du2dz2 = (-1.0f / 560.0f * (u.current[ind + 4] + u.current[ind - 4]) + 8.0f / 315.0f * (u.current[ind + 3] + u.current[ind - 3]) - 1.0f / 5.0f * (u.current[ind + 2] + u.current[ind - 2]) + 8.0f / 5.0f * (u.current[ind + 1] + u.current[ind - 1]) - 205.0f / 72.0f * u.current[ind]) / grid.dz / grid.dz;

            float du2dx2 = (-1.0f / 560.0f * (u.current[ind + 4 * grid.nz] + u.current[ind - 4 * grid.nz]) + 8.0f / 315.0f * (u.current[ind + 3 * grid.nz] + u.current[ind - 3 * grid.nz]) - 1.0f / 5.0f * (u.current[ind + 2 * grid.nz] + u.current[ind - 2 * grid.nz]) + 8.0f / 5.0f * (u.current[ind + grid.nz] + u.current[ind - grid.nz]) - 205.0f / 72.0f * u.current[ind]) / grid.dx / grid.dx;

            float lap = du2dx2 + du2dz2;

            u.next[ind] = 2.0f * u.current[ind] - u.previous[ind] + model.vp[ind] * model.vp[ind] * dt2 * lap;
        }
    }
}

void AcousticFDSolver::RunShot(const ShotGeometry &shot_geometry,                               const SourceTimeFunction &wavelet,
                               Seismogram &seismogram, float *final_snapshot)
{
    const size_t n_iterations = param.n_time_steps();

    const size_t rec_ratio = get_ratio(seismogram.dt_rec, param.dt);

    // allocate wavefields
    AcousticWavefield u(model.grid);

    for (size_t n = 0; n < n_iterations; n++)
    {
        acoustic_fd_step(u, model, model.grid, param.dt); // time step acoustic

        inject_source(
            u.next,
            shot_geometry.sources,
            wavelet,
            model.grid,
            param.dt,
            n); // inject wavelet

        if (n % rec_ratio == 0)
            record_receivers(
                u.next,
                seismogram,
                shot_geometry.receivers,
                model.grid,
                n / rec_ratio); // recording seismogram

        u.swap();
    }
    if (final_snapshot)
    {
        for ( size_t n= 0 ; n < model.grid.nx * model.grid.nz; n++)
        {
            final_snapshot[n] = u.current[n];
        }
    }
   
}

// AcousticFDSolver::~AcousticFDSolver(){}

AcousticFDSolver::AcousticFDSolver(
    const AcousticModel &input_model,
    const SimulationParameters &input_param)
    : model(input_model), param(input_param)
{
}