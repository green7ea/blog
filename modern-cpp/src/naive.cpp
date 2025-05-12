#include "naive.hpp"

#include <cstdio>
#include <cstring>
#include <errno.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>

NaiveFile::NaiveFile(const std::string &path) {
  fd = open(path.c_str(), O_RDONLY);

  printf("(fd %i) open %s\n", fd, path.c_str());
}

NaiveFile::~NaiveFile() {
  if (fd >= 0) {
    printf("(fd %i) ~NaiveFile closing\n", fd);

    if (close(fd) < 0) {
      fprintf(stderr, "  (fd %i) Couldn't close file: '%s'\n", fd, strerror(errno));
    }
  }
}

std::string NaiveFile::read_1024() const {
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
