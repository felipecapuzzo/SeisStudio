#include <iostream>
#include <fstream>
#include "io.hpp"

void io::ExportFloat32(std::string filename, size_t N_POINTS, float* vector)
{
    std::ofstream file(filename, std::ios_base::binary | std::ios_base::out);
    if (!file.is_open())
    {
        std::cerr << "Error writing file: " << filename << std::endl;
        return;
    }
    else
    {
        file.write(reinterpret_cast<const char*>(vector), N_POINTS * sizeof(float));
        file.close();
    }
}