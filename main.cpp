#include <iostream>
#include <filesystem>
#include "./src/app/processes/ProcessManagement.hpp"
#include "./src/app/processes/Task.hpp"

namespace fs = std::filesystem;

int main(int argc, char const *argv[])
{
    std::string directory;
    std::string action;

    std::cout << "Enter the directory path: " << std::endl;
    std::getline(std::cin, directory);

    std::cout << "Enter the action (ENCRYPT/DECRYPT): " << std::endl;
    std::getline(std::cin, action);

    try
    {
        // first check if it exists and is a directory
        if (fs::exists(directory) && fs::is_directory(directory))
        {
            // create a process management object
            ProcessManagement processManagement;

            // loop over all the files inside that folder
            for (const auto &entry : fs::recursive_directory_iterator(directory))
            {
                if (entry.is_regular_file())
                {
                    //  take the filepath
                    std::string filePath = entry.path().string();

                    // io opens/closes that file and gives file stream
                    IO io(filePath);

                    // take the ownership of the file stream
                    std::fstream fstream = std::move(io.getFileStream());

                    // take the action to do with files
                    Action taskAction = (action == "ENCRYPT" ? Action::ENCRYPT : Action::DECRYPT);

                    // first check if the file is open then create a new task object and push to taskQueue
                    if (fstream.is_open())
                    {
                        auto task = std::make_unique<Task>(std::move(fstream), taskAction, filePath);
                        processManagement.submitToQueue(std::move(task));
                    }
                    else
                    {
                        std::cout << "Unable to open the file: " << filePath << std::endl;
                    }
                }
            }
            // now execute all the tasks by opening each file again
            processManagement.executeTasks();
        }
        else
        {
            std::cout << "Invalid directory path: " << std::endl;
        }
    }
    catch (const fs::filesystem_error &e)
    {
        std::cout << "FileSystem error: " << e.what() << std::endl;
    }

    return 0;
}
