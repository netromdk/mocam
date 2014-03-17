#include <QDebug>
#include <QLabel>
#include <QDateTime>
#include <QVBoxLayout>
#include <QMessageBox>

#include "Util.h"
#include "Window.h"
#include "FaceDetector.h"

namespace mocam {
  Window::Window(QWidget *parent) :
    QWidget(parent), detector(nullptr), fps(0)
  {
    setupLayout();
    setupVideo();

    show();
    raise();
    activateWindow();    
  }

  Window::~Window() {
    session.stop();

    if (detector) {
      delete detector;
    }
  }

  void Window::onFrameCaptured(FramePtr frame) {
    QImage img = frame->scaledToWidth(480);
    MatPtr mat = Util::imageToMat(img);
    QList<FacePtr> faces = detector->detect(mat);
    Util::paintOverlays(img, faces);
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

    // TODO: Do this smarter about the files!
    detector = new FaceDetector("lbpcascade_frontalface.xml",
                                "haarcascade_eye_tree_eyeglasses.xml");
    if (!detector->isValid()) {
      QMessageBox::warning(this, "", tr("Could not initialize face detector!"));
      return;
    }

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
