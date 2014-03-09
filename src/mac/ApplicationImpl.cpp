#include "Util.h"
#include "ApplicationImpl.h"

namespace mocam {
  ApplicationImpl::ApplicationImpl() {
    // Get default device.


    // Get system devices.
    //int count = _getNumSystemDevices();    
  }

  std::pair<std::string, std::string> ApplicationImpl::getDefaultDevice() const {
    return defDev;
  }
}
