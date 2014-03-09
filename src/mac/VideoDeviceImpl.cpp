#include "VideoDeviceImpl.h"

#include "../Application.h"

namespace mocam {
  VideoDeviceImpl::VideoDeviceImpl(const std::string &uniqueId)
    : uniqueId(uniqueId)
  { }

  VideoDeviceImpl::VideoDeviceImpl(const std::string &uniqueId,
                                   const std::string &name)
    : uniqueId(uniqueId), name(name)
  { }
  
  ImplVDPtr VideoDeviceImpl::getDefaultDevice() {
    auto p = Application::app()->getDefaultDevice();
    return ImplVDPtr(new VideoDeviceImpl(p.first, p.second));
  }

  std::vector<ImplVDPtr> VideoDeviceImpl::getSystemDevices() {
    // TODO: get from ObjC.
    std::vector<ImplVDPtr> res;
    res.push_back(ImplVDPtr(new VideoDeviceImpl("LDKFJ", "First")));
    res.push_back(ImplVDPtr(new VideoDeviceImpl("dang", "dude")));
    res.push_back(ImplVDPtr(new VideoDeviceImpl("939fF", "Third")));
    return res;
  }

  std::string VideoDeviceImpl::toString() const {
    std::string res = "{ ";
    if (!name.empty()) {
      res += "name = " + name + ", ";
    }
    return res + "id = " + uniqueId + " }";
  }
}
