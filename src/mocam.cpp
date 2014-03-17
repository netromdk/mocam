#include <QApplication>

#include "Types.h"
#include "Window.h"

int main(int argc, char **argv) {
  QApplication app(argc, argv);

  mocam::initTypes();
  mocam::Window win;
  
  return app.exec();
}
