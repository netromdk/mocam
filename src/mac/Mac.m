#import <Foundation/Foundation.h>
#import <AVFoundation/AVFoundation.h>

#import "Mac.h"

void stringToCStr(NSString *str, char **cstr) {
  const char *tmp = [str UTF8String];
  int len = strlen(tmp) + 1;
  *cstr = new char[len];
  memcpy(*cstr, tmp, len);
  *cstr[len] = '\0';  
}

bool getDefaultDevice(char **id, char **name) {
  AVCaptureDevice *dev =
    [AVCaptureDevice defaultDeviceWithMediaType:AVMediaTypeVideo];
  if (!dev) return false;

  stringToCStr([dev uniqueID], id);
  stringToCStr([dev localizedName], name);  
  
  return true;
}
