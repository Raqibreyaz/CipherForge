#ifndef IO_HPP
#define IO_HPP

#include <fstream>
#include <string>

class IO
{
public:
    // constructor
    IO(const std::string &filePath);
    // destructor
    ~IO();
    // will return the filestream
    std::fstream getFileStream();

private:
    std::fstream file_stream;
};

#endif