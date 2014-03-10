#include "MacUtil.h"
#include "CaptureSessionImpl.h"

namespace mocam {
  CaptureSessionImpl::CaptureSessionImpl()
    : handleInput(nullptr), handleOutput(nullptr)
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

  QImage CaptureSessionImpl::getSnapshot() {
    // Create the device the first time it's needed and reuse it as
    // long as possible.
    if (!handleOutput) {
      handleOutput = _setupSessionOutput(handleSession);
    }

    int len;
    _startSession(handleSession);
    const unsigned char *imgData = _getSnapshot(handleOutput, len);
    _stopSession(handleSession);

    if (!imgData) return QImage();

    QImage img = QImage::fromData(imgData, len);
    delete[] imgData;
    
    return img;
  }

  void CaptureSessionImpl::close() {
    _stopSession(handleSession);
    
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
