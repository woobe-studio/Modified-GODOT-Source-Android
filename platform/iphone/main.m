/**************************************************************************/
/*  main.m                                                                */
/**************************************************************************/


#import "godot_app_delegate.h"

#import <UIKit/UIKit.h>
#include <stdio.h>

int gargc;
char **gargv;

int main(int argc, char *argv[]) {
	printf("*********** main.m\n");
	gargc = argc;
	gargv = argv;

	printf("running app main\n");
	@autoreleasepool {
		NSString *className = NSStringFromClass([GodotApplicalitionDelegate class]);
		UIApplicationMain(argc, argv, nil, className);
	}
	printf("main done\n");
	return 0;
}
