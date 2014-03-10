#include "VideoDevice.h"

namespace mocam {
  VideoDevice::VideoDevice(const QString &uniqueId) {
    impl = ImplVDPtr(new VideoDeviceImpl(uniqueId));
  }
  
  VideoDevice::VideoDevice(const QString &uniqueId, const QString &name) {
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

  QString VideoDevice::getUniqueId() const {
    return impl->getUniqueId();
  }
  
  QString VideoDevice::getName() const {
    return impl->getName();
  }

  bool VideoDevice::isNull() const {
    return getUniqueId().isEmpty() && getName().isEmpty();
  }

  bool VideoDevice::isInit() const {
    return impl->isInit();
  }

  void VideoDevice::init() {
    impl->init();
  }

  void *VideoDevice::getHandle() const {
    return impl->getHandle();
  }

  bool operator==(VDPtr first, VDPtr second) {
    return first->getUniqueId() == second->getUniqueId() &&
      first->getName() == second->getName();
  }

  QDebug operator<<(QDebug dbg, VDPtr device) {
    dbg << qPrintable(QString("{ name = %1, id = %2 }")
                      .arg(device->getName())
                      .arg(device->getUniqueId()));
    return dbg;
  }
}
