#include "Cryption.hpp"
#include "../processes/Task.hpp"
#include "../fileHandling/ReadEnv.cpp"
#include <iostream>

int executeCryption(const std::string &taskData)
{
    Task task = Task::fromString(taskData);
    ReadEnv env;

    std::string envKey = env.getEnv();
    int key = std::stoi(envKey);

    if (task.action == Action::ENCRYPT)
    {
        char ch;
        while (task.fStream.get(ch))
        {
            ch = (ch + key) % 256;
            task.fStream.seekp(-1, std::ios::cur);
            task.fStream.put(ch);
        }
    }
    else
    {
        char ch;
        while (task.fStream.get(ch))
        {
            ch = (ch - key + 256) % 256;
            task.fStream.seekp(-1, std::ios::cur);
            task.fStream.put(ch);
        }
    }
    task.fStream.close();
    return 0;
}
