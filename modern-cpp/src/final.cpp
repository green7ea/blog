#include "final.hpp"

#include <cstdio>
#include <cstring>
#include <errno.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>

FileWrapper::FileWrapper(const std::string &path) {
  fd = open(path.c_str(), O_RDONLY);
}

FileWrapper::FileWrapper(FileWrapper &&wrapper)
    : fd(wrapper.fd) {
  // 'wrapper' won't close a fd with -1 in its
  // destructor.
  wrapper.fd = -1;
}

FileWrapper::~FileWrapper() {
  if (fd >= 0) {
    if (close(fd) < 0) {
      fprintf(stderr, "  Couldn't close file: '%s'\n", strerror(errno));
    }
  }
}

void FileWrapper::operator=(FileWrapper &&wrapper) {
  this->fd = wrapper.fd;
  // 'wrapper' won't close a fd with -1 in its
  // destructor.
  wrapper.fd = -1;
}

std::string FileWrapper::read_1024() const {
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
