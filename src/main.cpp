#include "color.hpp"
#include "math/Ray.hpp"
#include "app/App.hpp"



int main() {

  App *app = new App();
  app->run();
  delete app;
  return 0;
}
