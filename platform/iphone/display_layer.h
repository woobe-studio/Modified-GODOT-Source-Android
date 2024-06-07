/**************************************************************************/
/*  display_layer.h                                                       */
/**************************************************************************/


#import <OpenGLES/EAGLDrawable.h>
#import <QuartzCore/QuartzCore.h>

@protocol DisplayLayer <NSObject>

- (void)startRenderDisplayLayer;
- (void)stopRenderDisplayLayer;
- (void)initializeDisplayLayer;
- (void)layoutDisplayLayer;

@end

@interface GodotOpenGLLayer : CAEAGLLayer <DisplayLayer>

@end
