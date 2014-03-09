#include "CaptureSession.h"

namespace mocam {
  CaptureSession::CaptureSession() {
    impl = ImplCSPtr(new CaptureSessionImpl);
  }

  void CaptureSession::setDevice(VDPtr device) {
    impl->setDevice(device);
  }

  const unsigned char *CaptureSession::getSnapshot(int &len) {
    return impl->getSnapshot(len);
  }

  void CaptureSession::close() {
    impl->close();
  }
}
