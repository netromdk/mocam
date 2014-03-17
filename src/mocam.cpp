#include <QApplication>

#include "Window.h"

int main(int argc, char **argv) {
  QApplication app(argc, argv);

  mocam::Window win;
  win.resize(512, 512);
  win.show();
  win.raise();
  win.activateWindow();
  
  return app.exec();
}
