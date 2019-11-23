#include "src/util/debug.h"
#include "src/app.hpp"

int main() {
  app app;
  TRY("APPLICATION START", app.start(), return EXIT_FAILURE)
  return EXIT_SUCCESS;
}
