/**************************************************************************/
/*  native_video_view.h                                                   */
/**************************************************************************/


#import <UIKit/UIKit.h>

@interface GodotNativeVideoView : UIView

- (BOOL)playVideoAtPath:(NSString *)filePath volume:(float)videoVolume audio:(NSString *)audioTrack subtitle:(NSString *)subtitleTrack;
- (BOOL)isVideoPlaying;
- (void)pauseVideo;
- (void)unfocusVideo;
- (void)unpauseVideo;
- (void)stopVideo;

@end
