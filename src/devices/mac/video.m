#import <Foundation/Foundation.h>
#import <AVFoundation/AVFoundation.h>

#include "video.h"

void dumpDevice(AVCaptureDevice *device) {
  printf("%s (%s)\n",
         [[device localizedName] UTF8String],
         [[device uniqueID] UTF8String]);
}

void dumpDevices() {
  // Get all available devices.
  for (AVCaptureDevice *device in
         [AVCaptureDevice devicesWithMediaType:AVMediaTypeVideo]) {
    dumpDevice(device);
  }
}

void dumpDefaultDevice() {
  AVCaptureDevice *device =
    [AVCaptureDevice defaultDeviceWithMediaType:AVMediaTypeVideo];
  dumpDevice(device);
}
