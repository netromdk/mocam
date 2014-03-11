#ifndef MOCAM_CAPTURE_SESSION_H
#define MOCAM_CAPTURE_SESSION_H

#include <QImage>

#include <memory>

#include "VideoDevice.h"
#include "CaptureSessionImpl.h"

namespace mocam {
  class CaptureSession;
  typedef std::shared_ptr<CaptureSession> CSPtr;
  
  class CaptureSession {
  public:
    CaptureSession();

    bool setDevice(VDPtr device);

    /**
     * Get a snapshot of the video device.
     *
     * If both width and height are given then the output is scaled to
     * match that, if one of them are specified (being not -1) then
     * that size-ratio will be respected.
     */
    QImage getSnapshot(int widht = -1, int height = -1);

    void close();

  private:
    ImplCSPtr impl;
  };
}

#endif // MOCAM_CAPTURE_SESSION_H
