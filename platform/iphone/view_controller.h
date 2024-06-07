/**************************************************************************/
/*  view_controller.h                                                     */
/**************************************************************************/


#import <UIKit/UIKit.h>

@class GodotView;
@class GodotNativeVideoView;
@class GodotKeyboardInputView;

@interface ViewController : UIViewController

@property(nonatomic, readonly, strong) GodotView *godotView;
@property(nonatomic, readonly, strong) GodotNativeVideoView *videoView;
@property(nonatomic, readonly, strong) GodotKeyboardInputView *keyboardView;

// MARK: Native Video Player

- (BOOL)playVideoAtPath:(NSString *)filePath volume:(float)videoVolume audio:(NSString *)audioTrack subtitle:(NSString *)subtitleTrack;

@end
