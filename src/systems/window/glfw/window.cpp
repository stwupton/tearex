#include "window.hpp"

#include "initialiser.hpp"
#include "terminator.hpp"
#include "updater.hpp"

Window::Window(SharedSystemData *sharedSystemData) {
  this->_sharedSystemData = sharedSystemData;
}

void Window::initialise() {
  WindowInitialiser initialiser;
  initialiser.run(&this->_sharedData, this->_sharedSystemData);
}

void Window::terminate() {
  WindowTerminator terminator;
  terminator.run(&this->_sharedData, this->_sharedSystemData);
}

void Window::update() {
  WindowUpdater updater;
  updater.run(&this->_sharedData, this->_sharedSystemData);
}