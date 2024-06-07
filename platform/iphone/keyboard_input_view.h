/**************************************************************************/
/*  keyboard_input_view.h                                                 */
/**************************************************************************/


#import <UIKit/UIKit.h>

@interface GodotKeyboardInputView : UITextView

- (BOOL)becomeFirstResponderWithString:(NSString *)existingString multiline:(BOOL)flag cursorStart:(NSInteger)start cursorEnd:(NSInteger)end;

@end
