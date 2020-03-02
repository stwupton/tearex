#include "renderer.hpp"

void Renderer::initialise() {
  RendererInitialiser initialiser;
  initialiser.run(&this->_sharedData);
}

void Renderer::terminate() {
  RendererTerminator terminator;
  terminator.run(&this->_sharedData);
}

void Renderer::update() {

}