#import <Foundation/Foundation.h>
#import <AVFoundation/AVFoundation.h>

#include "Util.h"

void stringToCStr(NSString *str, char **cstr) {
  const char *tmp = [str UTF8String];
  int len = strlen(tmp) + 1;
  *cstr = new char[len];
  memcpy(*cstr, tmp, len);
}

void getDevInfo(AVCaptureDevice *device, char **id, char **name) {
  stringToCStr([device uniqueID], id);
  stringToCStr([device localizedName], name);
}

bool _getDefaultDevice(char **id, char **name) {
  AVCaptureDevice *dev =
    [AVCaptureDevice defaultDeviceWithMediaType:AVMediaTypeVideo];
  if (!dev) return false;
  getDevInfo(dev, id, name);
  [dev release];
  return true;
}

int _getNumSystemDevices() {
  NSArray *devs = [AVCaptureDevice devicesWithMediaType:AVMediaTypeVideo];
  int count = [devs count];
  [devs release];
  return count;
}

void _getSystemDevices(char **IDs, char **names) {
  NSArray *devs = [AVCaptureDevice devicesWithMediaType:AVMediaTypeVideo];
  for (int i = 0; i < [devs count]; i++) {
    AVCaptureDevice *dev = [devs objectAtIndex:i];
    getDevInfo(dev, IDs + i, names + i);
    [dev release];
  }
  [devs release];  
}

void configureDevice(AVCaptureDevice *device) {
  AVCaptureDeviceFormat *bestFormat = nil;
  AVFrameRateRange *bestFrameRateRange = nil;
  for (AVCaptureDeviceFormat *format in [device formats]) {
    for (AVFrameRateRange *range in format.videoSupportedFrameRateRanges) {
      if (!bestFrameRateRange ||
          range.maxFrameRate > bestFrameRateRange.maxFrameRate) {
        bestFormat = format;
        bestFrameRateRange = range;
      }
    }
  }
  if (bestFormat) {
    printf("found format with highest frame rate\n");
    if ([device lockForConfiguration:NULL] == YES) {
      printf("configured device\n");
      device.activeFormat = bestFormat;
      device.activeVideoMinFrameDuration = bestFrameRateRange.minFrameDuration;
      device.activeVideoMaxFrameDuration = bestFrameRateRange.maxFrameDuration;
      [device unlockForConfiguration];
    }
  }
}

void *_getDeviceHandle(const char *id) {
  NSString *uid = [[NSString alloc] initWithUTF8String:id];
  AVCaptureDevice *dev = [AVCaptureDevice deviceWithUniqueID:uid];
  [uid release];
  if (!dev) return NULL;
  configureDevice(dev);
  return (void*) dev;
}

void _releaseHandle(void *handle) {
  if (!handle) return;
  AVCaptureDevice *dev = (AVCaptureDevice*) handle;
  [dev release];
}
