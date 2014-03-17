#include <QDebug>

#include "FrameGrabber.h"
#include "CaptureSessionImpl.h"

namespace mocam {
  FrameGrabber::FrameGrabber(CaptureSessionImpl *session) : session(session) { }

  void FrameGrabber::run() {
    for (;;) {
      // Stop when requested.
      if (isInterruptionRequested()) {
        return;
      }

      QImage img = session->getSnapshot(false);
      emit frameCaptured(FramePtr(new QImage(img)));
      usleep(512);
    }
  }
}
