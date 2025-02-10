#include <iostream>
#include "IO.hpp"
#include <fstream>

IO::IO(const std::string &filePath)
{
    // open the file in read & write with binary mode
    file_stream.open(filePath, std::ios::in | std::ios::out | std::ios::binary);

    // if failed to open the file
    if (!file_stream.is_open())
    {
        std::cout << "unable to open the file " << filePath << std::endl;
    }
}

std::fstream IO::getFileStream()
{
    return std::move(file_stream);
}

IO::~IO()
{
    // close the file if it was open
    if (file_stream.is_open())
    {
        file_stream.close();
    }
}