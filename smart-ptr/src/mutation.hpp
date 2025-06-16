#pragma once

#include <memory>
#include <string>

inline void mutate(const std::shared_ptr<std::string> &value) {
  *value = "Not what you think!";
}

inline void mutate2(const std::shared_ptr<std::string> &value) {
  std::shared_ptr<std::string> copy = value;

  *copy = "Still not what you think";
}

inline void mutate_unique(const std::unique_ptr<std::string> &value) {
  *value = "Same with unique_ptr";
}

inline int count_bad(const std::shared_ptr<std::string> &s, char d) {
  int count = 0;
  for (auto it = s->begin(); it != s->end(); ++it) {
    if (*it == d) {
      count += 1;

      // this is a surprise
      *it = '*';
    }
  }

  return count;
}

inline int count_good(const std::string &s, char d) {
  int count = 0;
  for (auto it = s.begin(); it != s.end(); ++it) {
    if (*it == d) {
      count += 1;
    }
  }

  return count;
}
