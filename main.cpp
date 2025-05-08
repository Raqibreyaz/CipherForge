#include "./src/app/encryptDecrypt/Cryption.hpp"
#include "./src/app/fileHandling/IO.hpp"
#include "./src/app/processes/ProcessManagement.hpp"
#include "./src/threadPool/threadPool.hpp"
#include "src/app/processes/Task.hpp"
#include <algorithm>
#include <chrono>
#include <filesystem>
#include <iostream>
#include <memory>

namespace fs = std::filesystem;

int main(int argc, char const *argv[]) {

  std::string directory;
  std::string action;

  std::cout << "Enter the directory path: " << std::endl;
  std::getline(std::cin, directory);

  std::cout << "Enter the action (ENCRYPT/DECRYPT): " << std::endl;
  std::getline(std::cin, action);

  // create an instance of the thread pool
  ThreadPool pool;

  ProcessManagement pm;

  auto start = std::chrono::high_resolution_clock::now();

  if (action == "DECRYPT") {
    try {
      // first check if it exists and is a directory
      if (fs::exists(directory) && fs::is_directory(directory)) {

        // loop over all the files inside that folder
        for (const auto &entry : fs::recursive_directory_iterator(directory)) {
          if (entry.is_regular_file()) {
            //  take the filepath
            std::string filePath = entry.path().string();

            // io opens/closes that file and gives file stream
            IO io(filePath);

            // take the ownership of the file stream
            std::fstream fstream = io.getFileStream();

            if (fstream.is_open()) {
              std::unique_ptr<Task> task = std::make_unique<Task>(
                  std::move(fstream), Action::DECRYPT, filePath);

              pm.submitToQueue(std::move(task));
            } else {
              std::cout << "Unable to open the file: " << filePath << std::endl;
            }
          }
        }
      } else {
        std::cout << "Invalid directory path: " << std::endl;
      }

      pm.executeTasks();
    } catch (const fs::filesystem_error &e) {
      std::cout << "FileSystem error: " << e.what() << std::endl;
    }
  } else {
    try {
      // first check if it exists and is a directory
      if (fs::exists(directory) && fs::is_directory(directory)) {

        // loop over all the files inside that folder
        for (const auto &entry : fs::recursive_directory_iterator(directory)) {
          if (entry.is_regular_file()) {
            //  take the filepath
            std::string filePath = entry.path().string();

            // io opens/closes that file and gives file stream
            IO io(filePath);

            // take the ownership of the file stream
            std::fstream fstream = io.getFileStream();

            if (fstream.is_open()) {

              std::string taskData = filePath + "::" + action;

              pool.addTask([taskData]() mutable { executeCryption(taskData); });

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
  }

  auto end = std::chrono::high_resolution_clock::now();

  auto duration =
      std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

  std::clog << "Took " << duration.count() << " ms" << " for " << action
            << std ::endl;

  return 0;
}
