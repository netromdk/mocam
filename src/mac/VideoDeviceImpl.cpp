#include <iostream> // remove if not used

#include "Util.h"
#include "VideoDeviceImpl.h"

namespace mocam {
  VideoDeviceImpl::VideoDeviceImpl(const std::string &uniqueId)
    : uniqueId(uniqueId), inited(false), handle(nullptr)
  { }

  VideoDeviceImpl::VideoDeviceImpl(const std::string &uniqueId,
                                   const std::string &name)
    : uniqueId(uniqueId), name(name), inited(false), handle(nullptr)
  { }

  VideoDeviceImpl::~VideoDeviceImpl() {
    if (inited && handle) {
      std::cout << "releasing handle" << std::endl;
      _releaseDeviceHandle(handle);
      handle = nullptr;
    }
  }
  
  ImplVDPtr VideoDeviceImpl::getDefaultDevice() {
    VideoDeviceImpl *dev = nullptr;
    char **id = new char*[1], **name = new char*[1];
    if (_getDefaultDevice(id, name)) {
      dev = new VideoDeviceImpl(std::string(*id), std::string(*name));
    }
    else {
      dev = new VideoDeviceImpl; // null ctor
    }
    
    delete id[0]; delete[] id;
    delete name[0]; delete[] name;
    return ImplVDPtr(dev);
  }

  std::vector<ImplVDPtr> VideoDeviceImpl::getSystemDevices() {
    int count = _getNumSystemDevices();
    char **IDs = new char*[count], **names = new char*[count];
    _getSystemDevices(IDs, names);

    std::vector<ImplVDPtr> res;
    res.reserve(count);
    for (int i = 0; i < count; i++) {
      res.push_back(ImplVDPtr(new VideoDeviceImpl(IDs[i], names[i])));
      delete IDs[i];
      delete names[i];      
    }
    
    delete[] IDs;
    delete[] names;
    return res;
  }

  std::string VideoDeviceImpl::toString() const {
    std::string res = "{ ";
    if (!name.empty()) {
      res += "name = " + name + ", ";
    }
    return res + "id = " + uniqueId + " }";
  }

  void VideoDeviceImpl::init() {
    if (inited) return;

    handle = _getDeviceHandle(uniqueId.c_str());
    std::cout << "acquired device handle: " << handle << std::endl;

    inited = true;
  }
}
