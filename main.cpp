#include <filesystem>
#include <iostream>
#include <memory>
// #include "./src/app/processes/ProcessManagement.hpp"
#include "./src/app/encryptDecrypt/Cryption.hpp"
#include "./src/app/processes/Task.hpp"
#include "./src/threadPool/threadPool.hpp"

namespace fs = std::filesystem;

int main(int argc, char const *argv[]) {

  std::string directory;
  std::string action;

  std::cout << "Enter the directory path: " << std::endl;
  std::getline(std::cin, directory);

  std::cout << "Enter the action (ENCRYPT/DECRYPT): " << std::endl;
  std::getline(std::cin, action);

  try {
    // first check if it exists and is a directory
    if (fs::exists(directory) && fs::is_directory(directory)) {

      // create an instance of the thread pool
      ThreadPool pool;

      // loop over all the files inside that folder
      for (const auto &entry : fs::recursive_directory_iterator(directory)) {
        if (entry.is_regular_file()) {
          //  take the filepath
          std::string filePath = entry.path().string();

          std::clog << "file: " << filePath << std::endl;

          // io opens/closes that file and gives file stream
          IO io(filePath);

          // take the ownership of the file stream
          std::fstream fstream = io.getFileStream();

          // take the action to do with files
          Action taskAction =
              (action == "ENCRYPT" ? Action::ENCRYPT : Action::DECRYPT);

          if (fstream.is_open()) {

            auto task = std::make_shared<Task>(std::move(fstream), taskAction,
                                               filePath);

            pool.addTask(
                [&task]() mutable { executeCryption(task->toString()); });

          } else {
            std::cout << "Unable to open the file: " << filePath << std::endl;
          }
        }
      }
    } else {
      std::cout << "Invalid directory path: " << std::endl;
    }
  } catch (const fs::filesystem_error &e) {
    std::cout << "FileSystem error: " << e.what() << std::endl;
  }

  return 0;
}
