#include "acoustic_fd.hpp"
#include "model.hpp"
#include "wavefield.hpp"
#include "geometry.hpp"
#include <algorithm>
#include <iostream>

void acoustic_fd_step(AcousticWavefield &u, const AcousticModel &model, const ComputationalGrid &grid, float dt)
{
    float dt2 = dt * dt;

    for (size_t x = 4; x < grid.nx - 4; x++)
    {
        for (size_t z = 4; z < grid.nz - 4; z++)
        {
            size_t ind = z + x * grid.nz;

            float du2dz2 = (-1.0f / 560.0f * (u.current[ind + 4] + u.current[ind - 4]) + 8.0f / 315.0f * (u.current[ind + 3] + u.current[ind - 3]) - 1.0f / 5.0f * (u.current[ind + 2] + u.current[ind - 2]) + 8.0f / 5.0f * (u.current[ind + 1] + u.current[ind - 1]) - 205.0f / 72.0f * u.current[ind]) / grid.model_grid.dz / grid.model_grid.dz;

            float du2dx2 = (-1.0f / 560.0f * (u.current[ind + 4 * grid.nz] + u.current[ind - 4 * grid.nz]) + 8.0f / 315.0f * (u.current[ind + 3 * grid.nz] + u.current[ind - 3 * grid.nz]) - 1.0f / 5.0f * (u.current[ind + 2 * grid.nz] + u.current[ind - 2 * grid.nz]) + 8.0f / 5.0f * (u.current[ind + grid.nz] + u.current[ind - grid.nz]) - 205.0f / 72.0f * u.current[ind]) / grid.model_grid.dx / grid.model_grid.dx;

            float lap = du2dx2 + du2dz2;

            u.next[ind] = 2.0f * u.current[ind] - u.previous[ind] + model.vp[ind] * model.vp[ind] * dt2 * lap;
        }
    }
}

void AcousticFDSolver::RunShot(const ShotGeometry &shot_geometry, const SourceTimeFunction &wavelet,
                               Seismogram &seismogram, float *final_snapshot)
{
    const size_t n_iterations = param.n_time_steps();

    const size_t rec_ratio = std::max<size_t>(1, get_ratio(seismogram.dt_rec, param.dt));

    // allocate wavefields
    AcousticWavefield u(model.grid);

    for (size_t n = 0; n < n_iterations; n++)
    {
        acoustic_fd_step(u, model, model.grid, param.dt); // time step acoustic

        inject_source(u.next, shot_geometry.sources, wavelet, model.grid, param.dt, n); // inject wavelet

        if (n % rec_ratio == 0)
            record_receivers(u.next, seismogram, shot_geometry.receivers, model.grid, n / rec_ratio); // recording seismogram

        apply_absorbing_boundary(u.current, model.grid);
        apply_absorbing_boundary(u.next, model.grid);

        u.swap();
    }
    if (final_snapshot)
    {
        for (size_t n = 0; n < model.grid.nx * model.grid.nz; n++)
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

void apply_absorbing_boundary(float *u, const ComputationalGrid &grid)
{
    // Keep ghost cells undamped.  They are numerical cells used by the
    // stencil; only the absorbing layers outside the physical model receive
    // the attenuation.
    constexpr float sqrt_two = 1.4142f;
    const float x_width = static_cast<float>(grid.absorbing);
    const float z_bottom_width = static_cast<float>(grid.absorbing + grid.padding_bottom);
    const float sb_x = 3.0f * x_width;
    const float sb_z = 3.0f * z_bottom_width;

    const size_t x0 = grid.model_x0;
    const size_t x1 = grid.model_x0 + grid.model_grid.nx - 1;

    const size_t z0 = grid.model_z0;
    const size_t z1 = grid.model_z0 + grid.model_grid.nz - 1;

    for (size_t x = 0; x < grid.nx; ++x)
    {
        for (size_t z = 0; z < grid.nz; ++z)
        {
            float x_abc = 1.0f;
            float z_abc = 1.0f;

            if (x >= grid.ghost && x < x0)
            {
                const float fb = static_cast<float>(x0 - x) / (sqrt_two * sb_x);
                x_abc = std::exp(-fb * fb);
            }
            else if (x > x1 && x < grid.nx - grid.ghost)
            {
                const float fb = static_cast<float>(x - x1) / (sqrt_two * sb_x);
                x_abc = std::exp(-fb * fb);
            }

            if (!grid.free_surface && z >= grid.ghost && z < z0)
            {
                const float fb = static_cast<float>(z0 - z) / (sqrt_two * sb_x);
                z_abc = std::exp(-fb * fb);
            }
            else if (z > z1 && z < grid.nz - grid.ghost)
            {
                const float fb = static_cast<float>(z - z1) / (sqrt_two * sb_z);
                z_abc = std::exp(-fb * fb);
            }

            const float damping = x_abc * z_abc;

            u[z + grid.nz * x] *= damping;
        }
    }
}
