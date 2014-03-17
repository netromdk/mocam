#include "MacUtil.h"
#include "FrameGrabber.h"
#include "CaptureSessionImpl.h"

namespace mocam {
  CaptureSessionImpl::CaptureSessionImpl()
    : handleInput(nullptr), handleOutput(nullptr), grabber(nullptr)
  {
    handleSession = _getSessionHandle();
  }

  CaptureSessionImpl::~CaptureSessionImpl() {
    close();

    if (handleSession) {
      _stopSession(handleSession);
      _releaseSessionHandle(handleSession);
      handleSession = nullptr;
    }
  }

  bool CaptureSessionImpl::setDevice(VDPtr device) {
    if (!device->isInit()) {
      return false;
    }

    // Close and cleanup everything at this point because we are
    // setting/changing device.
    close();

    this->device = device;
    handleInput = _setupSessionInput(this->device->getHandle(), handleSession);
    return true;
  }

  QImage CaptureSessionImpl::getSnapshot(bool startSession) {
    if (startSession) {
      start(false);
    }
    
    int len;
    const unsigned char *imgData = _getSnapshot(handleOutput, len);

    if (startSession) {
      stop();
    }

    if (!imgData) return QImage();

    QImage img = QImage::fromData(imgData, len);
    delete[] imgData;
    
    return img;
  }

  void CaptureSessionImpl::start(bool stream) {
    // Create the device the first time it's needed and reuse it as
    // long as possible.
    if (!handleOutput) {
      handleOutput = _setupSessionOutput(handleSession);
    }
    
    _startSession(handleSession);

    if (stream) {
      if (grabber) return;      
      grabber = new FrameGrabber(this);
      connect(grabber, SIGNAL(frameCaptured(FramePtr)),
              SIGNAL(frameCaptured(FramePtr)));
      connect(grabber, SIGNAL(finished()), grabber, SLOT(deleteLater()));
      grabber->start();
    }
  }
  
  void CaptureSessionImpl::stop() {
    if (grabber) {
      grabber->requestInterruption();
      grabber->wait();
      grabber->deleteLater();
      grabber = nullptr;
    }
    
    _stopSession(handleSession);
  }

  void CaptureSessionImpl::close() {
    stop();
    
    if (handleInput) {
      _releaseSessionInput(handleInput, handleSession);
      handleInput = nullptr;
    }
    
    if (handleOutput) {
      _releaseSessionOutput(handleOutput, handleSession);      
      handleOutput = nullptr;
    }    
  }
}
