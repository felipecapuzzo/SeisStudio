#pragma once

#include "geometry.hpp"
#include "model.hpp"

class AcousticModel
{
public:
    explicit AcousticModel(RegularGrid &grid);
    void fill(float value);
    ~AcousticModel();

    RegularGrid grid;
    float *vp = nullptr;
};

class ElasticModel
{
public:
    ElasticModel(RegularGrid &grid);
    ~ElasticModel();

    RegularGrid grid;
    float *vp = nullptr;
    float *vs = nullptr;
    float *rho = nullptr;;
};