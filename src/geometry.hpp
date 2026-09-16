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

    size_t index(size_t ind_z, size_t ind_x) const {return( ind_x * nz + ind_z);}
    size_t index(Coordinates &coord) const 
    {
        size_t ind_z = static_cast<size_t>((coord.z - z0) / dz);
        size_t ind_x = static_cast<size_t>((coord.x - x0) / dx);
        return index(ind_z, ind_x);
    }
    
};


struct ComputationalGrid
{
    size_t nz;
    size_t nx;

    float dz;
    float dx;

    size_t ghost;
    size_t boundary;
    size_t padding_z;

    size_t model_iz0;
    size_t model_ix0;

    size_t index(size_t iz, size_t ix) const
    {
        return iz + ix * nz;
    }
};


