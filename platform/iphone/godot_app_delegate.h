/**************************************************************************/
/*  godot_app_delegate.h                                                  */
/**************************************************************************/


#import <UIKit/UIKit.h>

typedef NSObject<UIApplicationDelegate> ApplicationDelegateService;

@interface GodotApplicalitionDelegate : NSObject <UIApplicationDelegate>

@property(class, readonly, strong) NSArray<ApplicationDelegateService *> *services;

+ (void)addService:(ApplicationDelegateService *)service;

@end
