#include <iostream>

#include "src/util/debug.h"
#include "src/systems/systems.hpp"
#include "src/systems/shared_data.hpp"

int main() {
  SharedSystemData *sharedSystemData = new SharedSystemData {};

  Window window(sharedSystemData);
  Renderer renderer;

  window.initialise();
  renderer.initialise();

  while (!sharedSystemData->applicationShouldClose) {
    window.update();
  }

  renderer.terminate();
  window.terminate();

  delete sharedSystemData;

  return EXIT_SUCCESS;
}
