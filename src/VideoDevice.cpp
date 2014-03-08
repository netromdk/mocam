#include "VideoDevice.h"

namespace mocam {
  class VideoDevice;
  typedef std::shared_ptr<VideoDevice> VDPtr;  
  
  VideoDevice::VideoDevice(const std::string &uniqueId) {
    impl = ImplVDPtr(new VideoDeviceImpl(uniqueId));
  }
  
  VideoDevice::VideoDevice(const std::string &uniqueId,
                           const std::string &name) {
    auto obj = new VideoDeviceImpl(uniqueId, name);
    impl = ImplVDPtr(obj);
  }

  VideoDevice::VideoDevice(ImplVDPtr ptr) {
    impl = std::move(ptr);
  }

  VDPtr VideoDevice::getDefaultDevice() {
    auto uptr = VideoDeviceImpl::getDefaultDevice();
    return VDPtr(new VideoDevice(uptr));
  }

  std::vector<VDPtr> VideoDevice::getSystemDevices() {
    auto udevs = VideoDeviceImpl::getSystemDevices();
    std::vector<VDPtr> devs;
    for (auto it = udevs.begin(); it != udevs.end(); ++it) {
      devs.push_back(VDPtr(new VideoDevice(*it)));
    }
    return devs;
  }


  std::string VideoDevice::getUniqueId() const {
    return impl->getUniqueId();
  }
  
  std::string VideoDevice::getName() const {
    return impl->getName();
  }

  std::string VideoDevice::toString() const {
    return impl->toString();
  }
}
