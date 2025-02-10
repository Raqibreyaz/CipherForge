#include <iostream>
#include <string>
#include <fstream>
#include "IO.hpp"
#include <sstream>

class ReadEnv
{
public:
    std::string getEnv()
    {
        std::string envPath = ".env";
        IO io(envPath);
        std::fstream fstream = io.getFileStream();
        std::stringstream buffer;
        buffer << fstream.rdbuf();
        std::string content = buffer.str();
        return content;
    }
};