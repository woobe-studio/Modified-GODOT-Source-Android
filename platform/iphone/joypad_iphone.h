/**************************************************************************/
/*  joypad_iphone.h                                                       */
/**************************************************************************/


#import <GameController/GameController.h>

@interface JoypadIPhoneObserver : NSObject

- (void)startObserving;
- (void)startProcessing;
- (void)finishObserving;

@end

class JoypadIPhone {
private:
	JoypadIPhoneObserver *observer;

public:
	JoypadIPhone();
	~JoypadIPhone();

	void start_processing();
};
