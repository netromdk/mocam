#import <Foundation/Foundation.h>
#import <AVFoundation/AVFoundation.h>

@interface Snapper : NSObject {
  AVCaptureSession *session;
  AVCaptureDeviceInput *inputDevice;
  AVCaptureStillImageOutput *outputDevice;
}

-(id)init;
-(void)dealloc;

-(BOOL)saveFrom:(AVCaptureDevice*)device toFile:(NSString*)filename;

@end
