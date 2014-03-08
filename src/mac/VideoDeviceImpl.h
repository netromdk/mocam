#ifndef MOCAM_VIDEO_DEVICE_IMPL_H
#define MOCAM_VIDEO_DEVICE_IMPL_H

#include <string>
#include <vector>
#include <memory>

namespace mocam {
  class VideoDeviceImpl;
  typedef std::shared_ptr<VideoDeviceImpl> ImplVDPtr;
  
  class VideoDeviceImpl {
  public:
    VideoDeviceImpl(const std::string &uniqueId);
    VideoDeviceImpl(const std::string &uniqueId, const std::string &name);

    static ImplVDPtr getDefaultDevice();
    static std::vector<ImplVDPtr> getSystemDevices();

    std::string getUniqueId() const { return uniqueId; }    
    std::string getName() const { return name; }

    std::string toString() const;
    
  private:
    std::string uniqueId, name;
  };
}

#endif // MOCAM_VIDEO_DEVICE_IMPL_H
