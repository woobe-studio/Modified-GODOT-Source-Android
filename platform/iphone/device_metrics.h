/**************************************************************************/
/*  device_metrics.h                                                      */
/**************************************************************************/


#import <Foundation/Foundation.h>

@interface GodotDeviceMetrics : NSObject

@property(nonatomic, class, readonly, strong) NSDictionary<NSArray *, NSNumber *> *dpiList;

@end
