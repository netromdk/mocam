#include <QDebug>
#include <QLabel>
#include <QVBoxLayout>
#include <QMessageBox>

#include "Window.h"

namespace mocam {
  Window::Window(QWidget *parent) : QWidget(parent) {
    setupLayout();
    setupVideo();

    resize(512, 512);
    show();
    raise();
    activateWindow();    
  }

  Window::~Window() {
    session.stop();
  }

  void Window::onFrameCaptured(FramePtr frame) {
    session.stop();
    frameLbl->setPixmap(QPixmap::fromImage(*frame));
  }

  void Window::setupLayout() {
    frameLbl = new QLabel;
    
    auto layout = new QVBoxLayout;
    layout->addWidget(frameLbl);

    setLayout(layout);
  }

  void Window::setupVideo() {
    device = VideoDevice::getDefaultDevice();
    if (device == nullptr) {
      QMessageBox::warning(this, "", tr("No default video device found!"));
      // TODO: quit
      return;
    }
    setWindowTitle(tr("MoCam (device: %1)").arg(device->getName()));

    device->init();
    if (!session.setDevice(device)) {
      QMessageBox::warning(this, "", tr("Could not setup capture session!"));
      // TODO: quit
      return;
    }

    connect(&session, SIGNAL(frameCaptured(FramePtr)),
            SLOT(onFrameCaptured(FramePtr)));
    session.start();
  }
}
