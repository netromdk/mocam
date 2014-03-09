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

  void CaptureSessionImpl::setDevice(VDPtr device) {
    this->device = device;
    handleInput = _setupSessionInput(this->device->getHandle(), handleSession);
  }

  const unsigned char *CaptureSessionImpl::getSnapshot(int &len) {
    const unsigned char *img = nullptr;

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
