#include "CaptureSession.h"

namespace mocam {
  CaptureSession::CaptureSession() {
    impl = ImplCSPtr(new CaptureSessionImpl);
  }

  bool CaptureSession::setDevice(VDPtr device) {
    return impl->setDevice(device);
  }

  QImage CaptureSession::getSnapshot() {
    return impl->getSnapshot();
  }

  void CaptureSession::close() {
    impl->close();
  }
}
