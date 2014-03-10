#ifndef MOCAM_VIDEO_DEVICE_IMPL_H
#define MOCAM_VIDEO_DEVICE_IMPL_H

#include <QString>

#include <vector>
#include <memory>

namespace mocam {
  class VideoDeviceImpl;
  typedef std::shared_ptr<VideoDeviceImpl> ImplVDPtr;
  
  class VideoDeviceImpl {
  public:
    VideoDeviceImpl(const QString &uniqueId);
    VideoDeviceImpl(const QString &uniqueId, const QString &name);
    ~VideoDeviceImpl();

    static ImplVDPtr getDefaultDevice();
    static std::vector<ImplVDPtr> getSystemDevices();

    QString getUniqueId() const { return uniqueId; }
    QString getName() const { return name; }

    bool isInit() const { return inited; }
    void init();

    void *getHandle() const { return handle; }

  private:
    // Null ctor.
    VideoDeviceImpl() { }

    QString uniqueId, name;
    bool inited;

    void *handle; // AVCaptureDevice*
  };
}

#endif // MOCAM_VIDEO_DEVICE_IMPL_H
