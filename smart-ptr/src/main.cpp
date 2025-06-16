#include "interfaces.hpp"
#include "mutation.hpp"

void mutation() {
  auto shared = std::make_shared<std::string>("Hello shared world!");

  printf("Good count\n");
  int good = count_good(*shared, 'l');
  printf("'%s' contains %i 'l's.\n\n", shared->c_str(), good);

  printf("Bad count\n");
  int bad = count_bad(shared, 'l');
  printf("'%s' contains %i 'l's.\n\n", shared->c_str(), bad);
}

int main() {
  mutation();

  ConfigWatcher watcher;
  std::shared_ptr<const Config> config = watcher.get_config();

  printf("Port %i is configured\n", config->port);

  // We can't update the config ourselves but the watcher can
  // internally.
  watcher.update_config();

  printf("Port %i is configured\n", config->port);
}
