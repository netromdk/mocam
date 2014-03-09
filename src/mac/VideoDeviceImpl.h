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
    ~VideoDeviceImpl();

    static ImplVDPtr getDefaultDevice();
    static std::vector<ImplVDPtr> getSystemDevices();

    std::string getUniqueId() const { return uniqueId; }
    std::string getName() const { return name; }
    std::string toString() const;

    bool isInit() const { return inited; }
    void init();

    void *getHandle() const { return handle; }

  private:
    // Null ctor.
    VideoDeviceImpl() { }

    std::string uniqueId, name;
    bool inited;

    void *handle; // AVCaptureDevice*
  };
}

#endif // MOCAM_VIDEO_DEVICE_IMPL_H
