#ifndef MOCAM_VIDEO_DEVICE_H
#define MOCAM_VIDEO_DEVICE_H

#include <QList>
#include <QDebug>
#include <QString>

#include <memory>

#include "VideoDeviceImpl.h"

namespace mocam {
  class VideoDevice;
  typedef std::shared_ptr<VideoDevice> VDPtr;

  class VideoDevice {
  public:
    VideoDevice(const QString &uniqueId);
    VideoDevice(const QString &uniqueId, const QString &name);

    static VDPtr getDefaultDevice();
    static QList<VDPtr> getSystemDevices();
    
    QString getUniqueId() const;
    QString getName() const;

    bool isNull() const;

    bool isInit() const;
    void init();

    void *getHandle() const;

  private:
    VideoDevice(ImplVDPtr ptr);

    ImplVDPtr impl;
  };

  bool operator==(VDPtr first, VDPtr second);
  QDebug operator<<(QDebug dbg, VDPtr device);
}

#endif // MOCAM_VIDEO_DEVICE_H
