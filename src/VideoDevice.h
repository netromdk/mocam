#ifndef MOCAM_VIDEO_DEVICE_H
#define MOCAM_VIDEO_DEVICE_H

#include <string>
#include <vector>
#include <memory>

#include "VideoDeviceImpl.h"

namespace mocam {
  class VideoDevice;
  typedef std::shared_ptr<VideoDevice> VDPtr;

  class VideoDevice {
  public:
    VideoDevice(const std::string &uniqueId);
    VideoDevice(const std::string &uniqueId, const std::string &name);

    static VDPtr getDefaultDevice();
    static std::vector<VDPtr> getSystemDevices();
    
    std::string getUniqueId() const;
    std::string getName() const;
    std::string toString() const;

    bool isNull() const;

    bool isInit() const;
    void init();

  private:
    VideoDevice(ImplVDPtr ptr);

    ImplVDPtr impl;
  };

  bool operator==(VDPtr first, VDPtr second);
}

#endif // MOCAM_VIDEO_DEVICE_H
