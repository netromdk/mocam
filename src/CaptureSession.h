#ifndef MOCAM_CAPTURE_SESSION_H
#define MOCAM_CAPTURE_SESSION_H

#include <QImage>
#include <QObject>

#include <memory>

#include "Types.h"
#include "VideoDevice.h"
#include "CaptureSessionImpl.h"

namespace mocam {
  class CaptureSession;
  typedef std::shared_ptr<CaptureSession> CSPtr;
  
  class CaptureSession : public QObject {
    Q_OBJECT
    
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

    void start(bool stream = true);
    void stop();

    void close();

  signals:
    void frameCaptured(FramePtr frame);

  private:
    ImplCSPtr impl;
  };
}

#endif // MOCAM_CAPTURE_SESSION_H
