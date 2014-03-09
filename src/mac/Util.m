#import <Foundation/Foundation.h>
#import <AVFoundation/AVFoundation.h>

#include "Util.h"

void stringToCStr(NSString *str, char **cstr) {
  const char *tmp = [str UTF8String];
  int len = strlen(tmp) + 1;
  *cstr = new char[len];
  memcpy(*cstr, tmp, len);
}

bool _getDefaultDevice(char **id, char **name) {
  AVCaptureDevice *dev =
    [AVCaptureDevice defaultDeviceWithMediaType:AVMediaTypeVideo];
  if (!dev) return false;

  stringToCStr([dev uniqueID], id);
  stringToCStr([dev localizedName], name);  

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
    stringToCStr([dev uniqueID], IDs + i);
    stringToCStr([dev localizedName], names + i);    
    [dev release];
  }
  [devs release];  
}
