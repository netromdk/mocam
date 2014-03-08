#include "VideoDevice.h"

namespace mocam {
  VideoDevice::VideoDevice(const std::string &uniqueId) {
    impl = ImplVDPtr(new VideoDeviceImpl(uniqueId));
  }
  
  VideoDevice::VideoDevice(const std::string &uniqueId,
                           const std::string &name) {
    impl = ImplVDPtr(new VideoDeviceImpl(uniqueId, name));
  }

  VideoDevice::VideoDevice(ImplVDPtr ptr) {
    impl = std::move(ptr);
  }

  VDPtr VideoDevice::getDefaultDevice() {
    return VDPtr(new VideoDevice(VideoDeviceImpl::getDefaultDevice()));
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
