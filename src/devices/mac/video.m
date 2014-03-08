#import <Foundation/Foundation.h>
#import <AVFoundation/AVFoundation.h>

#include "video.h"

void dumpDevices() {
  printf("Supported devices:\n");

  // Get all available devices.
  for (AVCaptureDevice *device in
         [AVCaptureDevice devicesWithMediaType:AVMediaTypeVideo]) {
    printf("%s (%s)\n",
           [[device localizedName] UTF8String],
           [[device uniqueID] UTF8String]);
  }
}
