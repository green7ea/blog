#pragma once

#include <string>

class SafeFile {
public:
  SafeFile(const std::string &path);
  SafeFile(const SafeFile &) = delete;
  ~SafeFile();
  void operator=(const SafeFile &) = delete;

  /// Reads up to 1024 characters. If
  /// something goes wrong, it returns
  /// an empty string.
  std::string read_1024() const;

private:
  int fd;
};
