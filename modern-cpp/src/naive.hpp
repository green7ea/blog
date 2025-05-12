#pragma once

#include <string>

class NaiveFile {
public:
  NaiveFile(const std::string &path);
  ~NaiveFile();

  /// Reads up to 1024 characters. If
  /// something goes wrong, it returns
  /// an empty string.
  std::string read_1024() const;

private:
  int fd;
};
