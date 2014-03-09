#include "Application.h"

namespace mocam {
  ImplAPtr Application::impl = nullptr;
  
  Application::Application() {
    impl = ImplAPtr(new ApplicationImpl);
  }

  std::pair<std::string, std::string> Application::getDefaultDevice() const {
    return impl->getDefaultDevice();
  }
}
