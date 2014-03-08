#ifndef MOCAM_VIDEO_DEVICE_H
#define MOCAM_VIDEO_DEVICE_H

#include <string>
#include <vector>
#include <memory>

#include "VideoDeviceImpl.h"

namespace mocam {
  class VideoDevice {
  public:
    VideoDevice(const std::string &uniqueId);
    VideoDevice(const std::string &uniqueId, const std::string &name);

    static std::shared_ptr<VideoDevice> getDefaultDevice();
    //static std::vector<std::shared_ptr<VideoDevice> > getSystemDevices();
    
    std::string getUniqueId() const;
    std::string getName() const;

    std::string toString() const;

  private:
    VideoDevice(std::unique_ptr<VideoDeviceImpl> &ptr);

    std::unique_ptr<VideoDeviceImpl> impl;
  };
}

#endif // MOCAM_VIDEO_DEVICE_H
