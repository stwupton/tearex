#pragma once

#include "shared_data.hpp"
#include "../../shared_data.hpp"

class Window {
public:
  Window(SharedSystemData *sharedSystemData);

  void initialise();

  void terminate();

  void update();

private:
  SharedWindowData _sharedData;
  SharedSystemData *_sharedSystemData;
};