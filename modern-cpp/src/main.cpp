#include "final.hpp"
#include "naive.hpp"
#include "safe.hpp"

template <typename T>
void accidental_copy(T file) {
  auto content = file.read_1024();

  if (content.size() >= 1024) {
    printf("File is bigger than 1KB\n");
  } else {
    printf("File is %zu bytes long\n", content.size());
  }
}

int main() {
  const std::string filename = __FILE__;

  printf("Naive file wrapper\n");
  {
    NaiveFile naive_file = NaiveFile(filename);

    // Copy 1, since we pass by copy
    accidental_copy(naive_file);

    // Copy 2, we used the = operator that makes a copy
    auto file2 = naive_file;
  }
  printf("\n\n");

  printf("Safe file wrapper\n");
  {
    SafeFile safe_file = SafeFile(filename);

    // Gives an error "call to deleted constructor of
    // 'SafeFile'".
    // accidental_copy(safe_file);

    // Gives an error as well
    // auto file2 = safe_file;
  }
  printf("\n\n");

  printf("File file wrapper\n");
  {
    FileWrapper file = FileWrapper(filename);

    // Gives an error "call to deleted constructor of
    // 'SafeFile'".
    // accidental_copy(safe_file);

    // Gives an error as well
    // auto file2 = safe_file;
  }
  printf("\n\n");
}
