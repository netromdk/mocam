#import <Foundation/Foundation.h>
#import <AVFoundation/AVFoundation.h>

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

//@interface Snapper : NSObject <AVCaptureFileOutputRecordingDelegate>
//@interface Snapper : NSObject <AVCaptureDataOutputRecordingDelegate> {
@interface Snapper : NSObject {
  AVCaptureSession *session;
  dispatch_queue_t sessionQueue;
  AVCaptureDeviceInput *inputDevice;
  AVCaptureStillImageOutput *outputDevice;
}

/*
@property (nonatomic, retain) AVCaptureSession *session;
@property (nonatomic, retain) dispatch_queue_t sessionQueue;
@property (nonatomic, retain) AVCaptureDeviceInput *inputDevice;
@property (nonatomic, retain) AVCaptureVideoDataOutput *outputDevice;
*/

-(id)init;
-(void)dealloc;

-(BOOL)saveFrom:(AVCaptureDevice*)device toFile:(NSString*)filename;

@end

@implementation Snapper

- (id)init {
	self = [super init];
  session = [[AVCaptureSession alloc] init];
  inputDevice = nil;
  outputDevice = nil;
	return self;
}

- (void)dealloc {
	if (session) [session release];
	if (inputDevice) [inputDevice release];
	if (outputDevice)	[outputDevice release];
  [super dealloc];
}

-(BOOL)saveFrom:(AVCaptureDevice*)device
         toFile:(NSString*)filename {
  if (!device) {
    printf("Device is invalid!\n");
    return NO;
  }

  NSError *error = nil;  
  
  printf("Adding input device to session.\n");  
  inputDevice = [AVCaptureDeviceInput deviceInputWithDevice:device error:&error];
  if (!inputDevice || ![session canAddInput:inputDevice]) {
    printf("Could not get input device, or could not add to session.\n");
    return NO;
  }
  [session addInput:inputDevice];

  printf("Adding output device to session.\n");
  outputDevice = [[AVCaptureStillImageOutput alloc] init];
  if (![session canAddOutput:outputDevice]) {
    printf("Can't add output.\n");
    return NO;
  }
  [outputDevice setOutputSettings:@{AVVideoCodecKey : AVVideoCodecJPEG}];
  [session addOutput:outputDevice];

  printf("Start recording...\n");
  [session startRunning];

  // Capture a still image.
  __block int gotStill = 0;
  AVCaptureConnection *conn = [outputDevice connectionWithMediaType:AVMediaTypeVideo];
  [outputDevice captureStillImageAsynchronouslyFromConnection:conn
    completionHandler:^(CMSampleBufferRef buffer, NSError *error) {
      if (buffer) {
        printf("Captured still!\n");
        NSData *imageData = [AVCaptureStillImageOutput jpegStillImageNSDataRepresentation:buffer];
        [imageData writeToFile:filename atomically:NO];
      }
      else {
        printf("Failed to capture still!\n");
      }
      gotStill = 1;
    }];

  while (gotStill == 0) {
    [[NSRunLoop currentRunLoop] runUntilDate:[NSDate dateWithTimeIntervalSinceNow: 0.1]];
  }

  printf("Stop recording...\n");
  [session stopRunning];
  
  return YES;
}

@end

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
