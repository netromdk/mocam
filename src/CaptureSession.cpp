#include "CaptureSession.h"

namespace mocam {
  CaptureSession::CaptureSession() {
    impl = ImplCSPtr(new CaptureSessionImpl);
    connect(impl.get(), SIGNAL(frameCaptured(FramePtr)),
            SIGNAL(frameCaptured(FramePtr)));
  }

  bool CaptureSession::setDevice(VDPtr device) {
    return impl->setDevice(device);
  }

  QImage CaptureSession::getSnapshot(int width, int height) {
    QImage img = impl->getSnapshot();
    if (img.isNull()) return img;

    // Scale if necessary.
    if (width != -1 && height != -1) {
      return img.scaled(width, height, Qt::IgnoreAspectRatio,
                        Qt::SmoothTransformation);
    }
    else if (width != -1 && height == -1) {
      return img.scaledToWidth(width, Qt::SmoothTransformation);
    }
    else if (width == -1 && height != -1) {
      return img.scaledToHeight(height, Qt::SmoothTransformation);
    }
    return img;
  }

  void CaptureSession::start(bool stream) {
    impl->start(stream);
  }

  void CaptureSession::stop() {
    impl->stop();
  }

  void CaptureSession::close() {
    impl->close();
  }
}
