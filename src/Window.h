#ifndef MOCAM_WINDOW_H
#define MOCAM_WINDOW_H

#include <QWidget>

namespace mocam {
  class Window : public QWidget {
  public:
    Window(QWidget *parent = nullptr);
  };
}

#endif // MOCAM_WINDOW_H
