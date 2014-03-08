#import "Snapper.h"

@interface Snapper ()

-(void)cleanup;

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
  [super dealloc];
}

- (void)cleanup {
  if (inputDevice) {
    [session removeInput:inputDevice];
    [inputDevice release];
    inputDevice = nil;
  }
  if (outputDevice)	{
    [session removeOutput:outputDevice];
    [outputDevice release];
    outputDevice = nil;
  }
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
    [self cleanup];
    return NO;
  }
  [session addInput:inputDevice];

  printf("Adding output device to session.\n");
  outputDevice = [[AVCaptureStillImageOutput alloc] init];
  if (![session canAddOutput:outputDevice]) {
    printf("Can't add output.\n");
    [self cleanup];
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
        gotStill = 1;
      }
      else {
        printf("Failed to capture still!\n");
        gotStill = 2;
      }
    }];

  while (gotStill == 0) {
    [[NSRunLoop currentRunLoop] runUntilDate:[NSDate dateWithTimeIntervalSinceNow: 0.1]];
  }

  printf("Stop recording...\n");
  [session stopRunning];

  [self cleanup];
  return (gotStill == 1 ? YES : NO);
}

+(AVCaptureDevice*)getDefaultDevice {
  return [AVCaptureDevice defaultDeviceWithMediaType:AVMediaTypeVideo];
}

+(NSArray*)getDevices {
  return [AVCaptureDevice devicesWithMediaType:AVMediaTypeVideo];
}

@end
