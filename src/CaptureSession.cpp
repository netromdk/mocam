#include "CaptureSession.h"

namespace mocam {
  CaptureSession::CaptureSession() {
    impl = ImplCSPtr(new CaptureSessionImpl);
  }
}
