#import <Foundation/Foundation.h>
#import <AVFoundation/AVFoundation.h>

#include "MacUtil.h"

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

void _releaseDeviceHandle(void *handle) {
  if (!handle) return;
  AVCaptureDevice *dev = (AVCaptureDevice*) handle;
  [dev release];
}

void *_getSessionHandle() {
  AVCaptureSession *session = [[AVCaptureSession alloc] init];
  return (void*) session;
}

void _releaseSessionHandle(void *handle) {
  if (!handle) return;
  AVCaptureSession *session = (AVCaptureSession*) handle;
  [session release];
}

void _startSession(void *handle) {
  if (!handle) return;
  AVCaptureSession *session = (AVCaptureSession*) handle;
  [session startRunning];
}

void _stopSession(void *handle) {
  if (!handle) return;
  AVCaptureSession *session = (AVCaptureSession*) handle;  
  [session stopRunning];  
}

void *_setupSessionInput(void *device, void *session) {
  if (!device || !session) {
    return nil;
  }

  AVCaptureDevice *dev = (AVCaptureDevice*) device;
  AVCaptureSession *ses = (AVCaptureSession*) session;
  AVCaptureDeviceInput *inputDevice = nil;

  NSError *error = nil;
  
  printf("Adding input device to session.\n");  
  inputDevice = [AVCaptureDeviceInput deviceInputWithDevice:dev error:&error];
  if (!inputDevice || ![ses canAddInput:inputDevice]) {
    printf("Could not get input device, or could not add to session.\n");
    return nil;
  }
  
  [ses addInput:inputDevice];
  return (void*) inputDevice;
}

void _releaseSessionInput(void *input, void *session) {
  if (!input) return;
  AVCaptureDeviceInput *devInput = (AVCaptureDeviceInput*) input;
  
  if (session) {
    AVCaptureSession *ses = (AVCaptureSession*) session;
    [ses removeInput:devInput];
  }

  [devInput release];
}

void *_setupSessionOutput(void *session) {
  if (!session) return nil;

  AVCaptureSession *ses = (AVCaptureSession*) session;
  AVCaptureStillImageOutput *output = [[AVCaptureStillImageOutput alloc] init];
  if (![ses canAddOutput:output]) {
    printf("Can't add output.\n");
    return nil;
  }

  [output setOutputSettings:@{AVVideoCodecKey : AVVideoCodecJPEG}];
  [ses addOutput:output];
  return (void*) output;
}

void _releaseSessionOutput(void *output, void *session) {
  if (!output) return;
  AVCaptureStillImageOutput *devOutput = (AVCaptureStillImageOutput*) output;
  
  if (session) {
    AVCaptureSession *ses = (AVCaptureSession*) session;
    [ses removeOutput:devOutput];
  }

  [devOutput release];  
}

const unsigned char *_getSnapshot(void *output, int &len) {
  if (!output) return nil;

  AVCaptureStillImageOutput *outputDevice = (AVCaptureStillImageOutput*) output;

  __block int gotStill = 0, length = 0;
  __block unsigned char *buf = nil;

  // Capture a still image.  
  AVCaptureConnection *conn =
    [outputDevice connectionWithMediaType:AVMediaTypeVideo];
  [outputDevice captureStillImageAsynchronouslyFromConnection:conn
    completionHandler:^(CMSampleBufferRef buffer, NSError *error) {
      if (buffer) {
        NSData *imageData =
          [AVCaptureStillImageOutput jpegStillImageNSDataRepresentation:buffer];

        length = [imageData length];
        buf = new unsigned char[length];
        [imageData getBytes:(void*)buf length:length];
        
        gotStill = 1;
      }
      else {
        printf("Failed to capture still!\n");
        gotStill = 2;
      }
    }];

  // Wait for finish.
  while (gotStill == 0) {
    NSDate *date = [NSDate dateWithTimeIntervalSinceNow: 0.1];
    [[NSRunLoop currentRunLoop] runUntilDate:date];
  }  

  len = length;
  return buf;
}
