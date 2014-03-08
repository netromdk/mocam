#import <Foundation/Foundation.h>
#import <AVFoundation/AVFoundation.h>

#import "Snapper.h"

#include "video.h"

AVCaptureDevice *getDefault() {
  return [AVCaptureDevice defaultDeviceWithMediaType:AVMediaTypeVideo];
}

void dumpDevice(AVCaptureDevice *device) {
  printf("%s (%s)",
         [[device localizedName] UTF8String],
         [[device uniqueID] UTF8String]);
  if (device == getDefault()) {
    printf(" (DEFAULT)");
  }
  printf("\n");
}

void dumpDevices() {
  // Get all available devices.
  for (AVCaptureDevice *device in
         [AVCaptureDevice devicesWithMediaType:AVMediaTypeVideo]) {
    dumpDevice(device);
  }
}

void confAndLockDevice(AVCaptureDevice *device) {
  AVCaptureDeviceFormat *bestFormat = nil;
  AVFrameRateRange *bestFrameRateRange = nil;
  for (AVCaptureDeviceFormat *format in [device formats]) {
    for (AVFrameRateRange *range in format.videoSupportedFrameRateRanges) {
      if (range.maxFrameRate > bestFrameRateRange.maxFrameRate) {
        bestFormat = format;
        bestFrameRateRange = range;
      }
    }
  }
  if (bestFormat) {
    printf("Found format with highest frame rate.\n");
    if ([device lockForConfiguration:NULL] == YES) {
      printf("Conf and lock device.\n");
      device.activeFormat = bestFormat;
      device.activeVideoMinFrameDuration = bestFrameRateRange.minFrameDuration;
      device.activeVideoMaxFrameDuration = bestFrameRateRange.minFrameDuration;
      [device unlockForConfiguration];
    }
  }
}

void getSnapshot() {
  AVCaptureDevice *device = getDefault();
  if (device == nil) {
    printf("Could not find default device!\n");
    return;
  }

  printf("Got default device: %s\n", [[device localizedName] UTF8String]);
  confAndLockDevice(device);

  NSString *filename = @"/tmp/camshot.jpg";
  
  Snapper *snapper = [[Snapper alloc] init];
  if ([snapper saveFrom:device toFile:filename]) {
    printf("Saved snapshot to: %s\n", [filename UTF8String]);
  }
  else {
    printf("Was unable to save snapshot.\n");
  }
  
  [snapper release];
  [device unlockForConfiguration];
  [device release];
}
