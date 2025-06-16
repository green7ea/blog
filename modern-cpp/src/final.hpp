#pragma once

#include <string>

class FileWrapper {
public:
  FileWrapper(const std::string &path);
  FileWrapper(const FileWrapper &) = delete;
  FileWrapper(FileWrapper &&);
  ~FileWrapper();
  void operator=(const FileWrapper &) = delete;
  void operator=(FileWrapper &&);

  /// Reads up to 1024 characters. If
  /// something goes wrong, it returns
  /// an empty string.
  std::string read_1024() const;

private:
  int fd{-1};
};
