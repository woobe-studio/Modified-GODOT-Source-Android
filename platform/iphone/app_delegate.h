/**************************************************************************/
/*  app_delegate.h                                                        */
/**************************************************************************/


#import <UIKit/UIKit.h>

@class ViewController;

@interface AppDelegate : NSObject <UIApplicationDelegate>

@property(strong, nonatomic) UIWindow *window;
@property(strong, class, readonly, nonatomic) ViewController *viewController;

@end
