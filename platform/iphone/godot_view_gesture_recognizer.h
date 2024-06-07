/**************************************************************************/
/*  godot_view_gesture_recognizer.h                                       */
/**************************************************************************/


// GodotViewGestureRecognizer allows iOS gestures to work correctly by
// emulating UIScrollView's UIScrollViewDelayedTouchesBeganGestureRecognizer.
// It catches all gestures incoming to UIView and delays them for 150ms
// (the same value used by UIScrollViewDelayedTouchesBeganGestureRecognizer)
// If touch cancellation or end message is fired it fires delayed
// begin touch immediately as well as last touch signal

#import <UIKit/UIKit.h>

@interface GodotViewGestureRecognizer : UIGestureRecognizer {
@private

	// Timer used to delay begin touch message.
	// Should work as simple emulation of UIDelayedAction
	NSTimer *delayTimer;

	// Delayed touch parameters
	NSSet *delayedTouches;
	UIEvent *delayedEvent;
}

@property(nonatomic, readonly, assign) NSTimeInterval delayTimeInterval;

- (instancetype)init;

@end
