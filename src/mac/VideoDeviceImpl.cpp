#include <iostream>

#include "Util.h"
#include "VideoDeviceImpl.h"

namespace mocam {
  VideoDeviceImpl::VideoDeviceImpl(const std::string &uniqueId)
    : uniqueId(uniqueId)
  { }

  VideoDeviceImpl::VideoDeviceImpl(const std::string &uniqueId,
                                   const std::string &name)
    : uniqueId(uniqueId), name(name)
  { }
  
  ImplVDPtr VideoDeviceImpl::getDefaultDevice() {
    VideoDeviceImpl *dev = NULL;
    char **id = new char*[1], **name = new char*[1];
    if (!_getDefaultDevice(id, name)) {
      dev = new VideoDeviceImpl; // Null ctor.
    }
    else {
      dev = new VideoDeviceImpl(std::string(*id), std::string(*name));
    }
    delete[] id;
    delete[] name;
    return ImplVDPtr(dev);
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
