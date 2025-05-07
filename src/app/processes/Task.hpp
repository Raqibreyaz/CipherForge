#ifndef TASK_HPP
#define TASK_HPP

#include <iostream>
#include <string>
#include <sstream>
#include "../fileHandling/IO.hpp"

enum class Action
{
    ENCRYPT,
    DECRYPT
};

struct Task
{
    std::fstream fStream;
    Action action;
    std::string filePath;
    // & can't be used with a stream so we use && here
    Task(std::fstream &&stream, Action act, std::string file_path) : fStream(std::move(stream)), action(act), filePath(file_path)
    {
        // std::cout << "New Task Created" << std::endl;
    }

    // will return "filePath,ENCRYPT/DECRYPT"
    std::string toString()
    {
        std::ostringstream oss;
        oss << filePath << "," << (action == Action::ENCRYPT ? "ENCRYPT" : "DECRYPT");
        return oss.str();
    }

    static Task fromString(const std::string &taskData)
    {
        std::istringstream iss(taskData);
        std::string filePath;
        std::string actionStr;

        if (std::getline(iss, filePath, ',') && std::getline(iss, actionStr))
        {
            Action action = actionStr == "ENCRYPT" ? Action::ENCRYPT : Action::DECRYPT;
            IO io(filePath);
            std::fstream fstream = io.getFileStream();
            if (fstream.is_open())
            {
                return Task(std::move(fstream), action, filePath);
            }
            else
            {
                throw std::runtime_error("Failed to open file: " + filePath);
            }
        }
        else
        {
            throw std::runtime_error("Invalid Task Data Format");
        }
    }
};

#endif