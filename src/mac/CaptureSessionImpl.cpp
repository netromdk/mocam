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
    this->device = device;

    _stopSession(handleSession);
    if (handleInput) {
      _releaseSessionInput(handleInput, handleSession);
    }
    handleInput = _setupSessionInput(this->device->getHandle(), handleSession);
    return true;
  }

  const unsigned char *CaptureSessionImpl::getSnapshot(int &len) {
    const unsigned char *img = nullptr;

    _stopSession(handleSession);
    if (handleOutput) {
      _releaseSessionOutput(handleOutput, handleSession);
    }
    handleOutput = _setupSessionOutput(handleSession);

    _startSession(handleSession);
    img = _getSnapshot(handleOutput, len);
    _stopSession(handleSession);    
    
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
