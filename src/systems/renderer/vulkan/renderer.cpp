#pragma once

#include "initialiser.cpp"
#include "shared_data.hpp"
#include "terminator.cpp"

class VulkanRenderer {
private:
  VulkanInitialiser _initialiser;
  VulkanSharedData _sharedData;
  VulkanTerminator _terminator;

public:
  void initialise() {
    this->_initialiser.run(&this->_sharedData);
  }

  void terminate() {
    this->_terminator.run(&this->_sharedData);
  }
};