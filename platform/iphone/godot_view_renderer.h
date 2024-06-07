/**************************************************************************/
/*  godot_view_renderer.h                                                 */
/**************************************************************************/


#import <UIKit/UIKit.h>

@protocol GodotViewRendererProtocol <NSObject>

@property(assign, readonly, nonatomic) BOOL hasFinishedSetup;

- (BOOL)setupView:(UIView *)view;
- (void)renderOnView:(UIView *)view;

@end

@interface GodotViewRenderer : NSObject <GodotViewRendererProtocol>

@end
