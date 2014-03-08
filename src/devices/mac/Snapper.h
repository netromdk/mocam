#import <Foundation/Foundation.h>
#import <AVFoundation/AVFoundation.h>

@interface Snapper : NSObject {
  AVCaptureSession *session;
  AVCaptureDeviceInput *inputDevice;
  AVCaptureStillImageOutput *outputDevice;
}

-(id)init;
-(void)dealloc;
-(void)cleanup; // TODO: make private!

-(BOOL)saveFrom:(AVCaptureDevice*)device toFile:(NSString*)filename;

+(AVCaptureDevice*)getDefaultDevice;
+(NSArray*)getDevices;
@end
