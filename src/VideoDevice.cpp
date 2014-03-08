#include "VideoDevice.h"

namespace mocam {
  VideoDevice::VideoDevice(const std::string &uniqueId) {
    impl = std::unique_ptr<VideoDeviceImpl>(new VideoDeviceImpl(uniqueId));
  }
  
  VideoDevice::VideoDevice(const std::string &uniqueId,
                           const std::string &name) {
    auto obj = new VideoDeviceImpl(uniqueId, name);
    impl = std::unique_ptr<VideoDeviceImpl>(obj);
  }

  VideoDevice::VideoDevice(std::unique_ptr<VideoDeviceImpl> &ptr) {
    impl = std::move(ptr);
  }

  std::shared_ptr<VideoDevice> VideoDevice::getDefaultDevice() {
    auto uptr = VideoDeviceImpl::getDefaultDevice();
    return std::shared_ptr<VideoDevice>(new VideoDevice(uptr));
  }
  /*
  std::vector<std::shared_ptr<VideoDevice> > VideoDevice::getSystemDevices() {
  }
  */

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
