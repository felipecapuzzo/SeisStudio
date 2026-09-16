#pragma once

#include <vector>
#include <iostream>
#include "sourcetimefunction.hpp"
#include "seismogram.hpp"
#include "geometry.hpp"

struct Coordinates
{
    float z;
    float x;
    float y = 0.0f;
};

struct RegularGrid
{
    size_t nx;
    size_t nz;
    size_t ny = 0;

    float dx;
    float dz;
    float dy = 0.0f;

    float z0 = 0.0f;
    float x0 = 0.0f;
    float y0 = 0.0f;

    size_t index(size_t ind_z, size_t ind_x) const { return (ind_x * nz + ind_z); }
    size_t index(const Coordinates &coord) const
    {
        size_t ind_z = static_cast<size_t>((coord.z - z0) / dz);
        size_t ind_x = static_cast<size_t>((coord.x - x0) / dx);
        return index(ind_z, ind_x);
    }
};

class ReceiverPosition
{
public:
    void add(const Coordinates &coord) { return (coordinate.push_back(coord)); };

    std::size_t size() const { return coordinate.size(); }
    const Coordinates &operator[](std::size_t index) const { return coordinate[index]; }

private:
    std::vector<Coordinates> coordinate;
};

class SourcePosition
{
public:
    void add(const Coordinates &coord) { return (coordinate.push_back(coord)); };

    std::size_t size() const { return coordinate.size(); }
    const Coordinates &operator[](std::size_t index) const { return coordinate[index]; }

private:
    std::vector<Coordinates> coordinate;
};

class ShotGeometry
{
public:
    ShotGeometry() {};
    SourcePosition sources;
    ReceiverPosition receivers;
};

class ComputationalGrid;

void inject_source(float *u, const SourcePosition &sources, const SourceTimeFunction &wavelet, const ComputationalGrid &grid, float dt, size_t n);
size_t get_ratio(float a, float b);
float hicks_weight(float x, float r);
void record_receivers(const float *u, Seismogram &seismogram, const ReceiverPosition &receivers, const ComputationalGrid &grid, size_t n);

#pragma once

class ComputationalGrid
{
public:
    ComputationalGrid(const RegularGrid &model_grid, size_t absorbing, bool free_surface) : model_grid(model_grid), absorbing(absorbing), free_surface(free_surface)
    {
        ghost = 4;

        absorbing_top = free_surface ? 0 : absorbing;

        model_x0 = ghost + absorbing;
        model_z0 = ghost + absorbing_top;

        nx = model_grid.nx + 2 * ghost + 2 * absorbing;

        nz = model_grid.nz + 2 * ghost + absorbing_top + absorbing;

        // Extra padding only at the bottom so that
        // the z dimension is divisible by 4.
        padding_bottom = (4 - nz % 4) % 4;
        nz += padding_bottom;
    }

    size_t index(size_t z, size_t x) const
    {
        return z + nz * x;
    }

    size_t model_index(size_t z, size_t x) const
    {
        return (z + model_z0) + nz * (x + model_x0);
    }

    RegularGrid model_grid;

    size_t index(const Coordinates &coord) const
    {
        size_t iz = static_cast<size_t>((coord.z - model_grid.z0) / model_grid.dz);
        size_t ix = static_cast<size_t>((coord.x - model_grid.x0) / model_grid.dx);
        return model_index(iz, ix);
    }

    size_t nx;
    size_t nz;

    size_t ghost;
    size_t absorbing;

    size_t absorbing_top;
    size_t padding_bottom;

    size_t model_x0;
    size_t model_z0;

    bool free_surface;
};
