#ifndef TASK_HPP
#define TASK_HPP

#include "../fileHandling/IO.hpp"
#include <iostream>
#include <sstream>
#include <string>

enum class Action { ENCRYPT, DECRYPT };

struct Task {
  std::fstream fStream;
  Action action;
  std::string filePath;
  // & can't be used with a stream so we use && here
  Task(std::fstream &&stream, Action act, std::string file_path)
      : fStream(std::move(stream)), action(act), filePath(file_path) {
    // std::cout << "New Task Created" << std::endl;
  }

  // will return "filePath::ENCRYPT/DECRYPT"
  std::string toString() const {
    std::ostringstream oss;
    oss << filePath
        << "::" << (action == Action::ENCRYPT ? "ENCRYPT" : "DECRYPT");
    return oss.str();
  }

  // "filePath::ENCRYPT/DECRYPT"
  static Task fromString(const std::string &taskData) {
    std::istringstream iss(taskData);

    size_t doubleColon = taskData.find("::");
    if (doubleColon == std::string::npos)
      throw std::runtime_error("Invalid Task Data Format");

    std::string filePath = taskData.substr(0, doubleColon);
    std::string actionStr = taskData.substr(doubleColon + 2);

    Action action = actionStr == "ENCRYPT" ? Action::ENCRYPT : Action::DECRYPT;
    IO io(filePath);
    std::fstream fstream = io.getFileStream();
    if (fstream.is_open()) {
      return Task(std::move(fstream), action, filePath);
    } else {
      throw std::runtime_error("Failed to open file: " + filePath);
    }
  }
};

#endif