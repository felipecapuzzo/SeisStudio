#pragma once

#include "geometry.hpp"
#include "model.hpp"
#include "wavefield.hpp"
#include "parameters.hpp"
#include "seismogram.hpp"

void acoustic_fd_step(AcousticWavefield &u, const AcousticModel &model, const ComputationalGrid &grid, float dt);

void apply_absorbing_boundary(float *u, const ComputationalGrid &grid);

void apply_free_surface(float *u, const ComputationalGrid &grid);


class AcousticFDSolver
{
public:
    AcousticFDSolver(const AcousticModel &input_model, const SimulationParameters &input_param);

    ~AcousticFDSolver() = default;

    void RunShot(const ShotGeometry &shot, const SourceTimeFunction &wavelet, Seismogram &seismogram,  float* final_snapshot = nullptr);

private:
    const AcousticModel &model;
    const SimulationParameters &param;
};

