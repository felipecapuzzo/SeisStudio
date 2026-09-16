#pragma once

#include "geometry.hpp"
#include "model.hpp"

class AcousticModel
{
public:
    explicit AcousticModel(const ComputationalGrid &grid);
    void fill(float value);
    ~AcousticModel();

    ComputationalGrid grid;
    float *vp = nullptr;
};

class ElasticModel
{
public:
    explicit ElasticModel(const ComputationalGrid &grid);
    ~ElasticModel();

    ComputationalGrid grid;
    float *vp = nullptr;
    float *vs = nullptr;
    float *rho = nullptr;;
};
