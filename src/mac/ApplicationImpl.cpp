#include "Util.h"
#include "ApplicationImpl.h"

namespace mocam {
  ApplicationImpl::ApplicationImpl() {
    // Get default device.
    char **id = new char*[1], **name = new char*[1];
    if (_getDefaultDevice(id, name)) {
      defDev = std::pair<std::string, std::string>(std::string(*id),
                                                   std::string(*name));
    }
    delete[] id;
    delete[] name;    
    
    // Get system devices.
  }

  std::pair<std::string, std::string> ApplicationImpl::getDefaultDevice() const {
    return defDev;
  }
}
