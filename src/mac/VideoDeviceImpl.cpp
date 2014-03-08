#include "VideoDeviceImpl.h"

namespace mocam {
  VideoDeviceImpl::VideoDeviceImpl(const std::string &uniqueId)
    : uniqueId(uniqueId)
  {
    
  }

  VideoDeviceImpl::VideoDeviceImpl(const std::string &uniqueId,
                                   const std::string &name)
    : uniqueId(uniqueId), name(name)
  {
    
  }
  
  std::unique_ptr<VideoDeviceImpl> VideoDeviceImpl::getDefaultDevice() {
    // TODO: get default from ObjC
    return std::unique_ptr<VideoDeviceImpl>(new VideoDeviceImpl("dang", "dude"));
  }

  std::string VideoDeviceImpl::toString() const {
    std::string res = "{ ";
    if (!name.empty()) {
      res += "name=" + name + ", ";
    }
    return res + "id=" + uniqueId + " }";
  }
}
