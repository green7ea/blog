#pragma once

#include <memory>
#include <string>

struct Config {
  std::string hostname;
  int port;
  std::string url;
};

class ConfigWatcher {
public:
  ConfigWatcher()
      : config(
            std::make_shared<Config>(Config{"localhost", 80, "/index.html"})) {
  }

  /**
   * By returning a shared_ptr to a const Config, we make sure that
   * we're the only ones that can modify the underlying configuration.
   */
  inline std::shared_ptr<const Config> get_config() {
    return config;
  }

  /**
   * This could be triggered when the config is updated.
   */
  inline void update_config() {
    config->port += 1;
  }

private:
  std::shared_ptr<Config> config;
};
