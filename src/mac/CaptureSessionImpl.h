#ifndef MOCAM_CAPTURE_SESSION_IMPL_H
#define MOCAM_CAPTURE_SESSION_IMPL_H

#include <QImage>
#include <QObject>

#include <memory>

#include "../VideoDevice.h"

namespace mocam {
  class FrameGrabber;
  
  class CaptureSessionImpl;
  typedef std::shared_ptr<CaptureSessionImpl> ImplCSPtr;
  
  class CaptureSessionImpl : public QObject {
    Q_OBJECT
    
  public:
    CaptureSessionImpl();
    ~CaptureSessionImpl();

    bool setDevice(VDPtr device);

    /**
     * The 'startSession' argument dictates whether the session will
     * be started/stopped for each invocation. Should be false when
     * streaming.
     */
    QImage getSnapshot(bool startSession = true);

    void start(bool stream = true);
    void stop();    
    void close();

  signals:
    void frameCaptured(FramePtr frame);

  private:
    void *handleSession, // AVCaptureSession*
      *handleInput, // AVCaptureDeviceInput*
      *handleOutput; // AVCaptureStillImageOutput* fx
    VDPtr device;
    FrameGrabber *grabber;
  };
}

#endif // MOCAM_CAPTURE_SESSION_IMPL_H
