#include <QDebug>
#include <QLabel>
#include <QDateTime>
#include <QVBoxLayout>
#include <QMessageBox>

#include "Window.h"

namespace mocam {
  Window::Window(QWidget *parent) : QWidget(parent), fps(0) {
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
    QImage img = frame->scaledToWidth(640);
    frameLbl->setPixmap(QPixmap::fromImage(img));

    static int frames = 0;
    frames++;

    static QDateTime last = QDateTime::currentDateTime();
    QDateTime now(QDateTime::currentDateTime());
    if (last.msecsTo(now) >= 1000) {
      fps = frames;
      frames = 0;
      last = now;
    }

    setTitle();
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
    setTitle();

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

  void Window::setTitle() {
    QString title = tr("MoCam (%1 @ %2 fps)")
      .arg(device->getName())
      .arg(fps == 0 ? tr("N.A.") : QString::number(fps));
    setWindowTitle(title);
  }
}
