#include <iostream>//remove if not needed

#include "MacUtil.h"
#include "CaptureSessionImpl.h"

namespace mocam {
  CaptureSessionImpl::CaptureSessionImpl()
    : handleInput(nullptr), handleOutput(nullptr)
  {
    handleSession = _getSessionHandle();
    std::cout << "got session handle: " << handleSession << std::endl;    
  }

  CaptureSessionImpl::~CaptureSessionImpl() {
    std::cout << "session destructor" << std::endl;
    close();

    if (handleSession) {
      std::cout << "stopping session (if running)" << std::endl;
      _stopSession(handleSession);
      std::cout << "release session handle" << std::endl;      
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
      std::cout << "release session input" << std::endl;
      _releaseSessionInput(handleInput, handleSession);
      handleInput = nullptr;
    }
    
    if (handleOutput) {
      std::cout << "release session output" << std::endl;
      _releaseSessionOutput(handleOutput, handleSession);      
      handleOutput = nullptr;
    }    
  }
}
