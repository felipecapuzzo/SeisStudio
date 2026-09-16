#pragma once

#include <string>
#include <vector>

class io
{
    public:
//    void static deleteFile(const std::string& filename);
    void static ExportFloat32(std::string filename, size_t N_POINTS, float* vector);
};