#pragma once

#include "initialiser.hpp"
#include "terminator.hpp"
#include "shared_data.hpp"

class Renderer {
public:
  void initialise();

  void terminate();

  void update();

private:
  SharedRendererData _sharedData;
};