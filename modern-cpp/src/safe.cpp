#include "safe.hpp"

#include <cstdio>
#include <cstring>
#include <errno.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>

SafeFile::SafeFile(const std::string &path) {
  fd = open(path.c_str(), O_RDONLY);
}

SafeFile::~SafeFile() {
  if (fd >= 0) {
    printf("~SafeFile closing file\n");

    if (close(fd) < 0) {
      fprintf(stderr, "  Couldn't close file: '%s'\n", strerror(errno));
    }
  }
}

std::string SafeFile::read_1024() const {
  if (fd < 0) {
    return "";
  }

  const int nbytes = 1024;
  char buf[nbytes] = {};

  int n = ::read(fd, buf, nbytes);
  if (n <= 0) {
    return "";
  }

  return std::string(buf, n);
}
