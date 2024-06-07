/**************************************************************************/
/*  XRMode.java                                                           */
/**************************************************************************/


package org.godotengine.godot.xr;

/**
 * Godot available XR modes.
 */
public enum XRMode {
	REGULAR(0, "Regular", "--xr_mode_regular", "Default Android Gamepad"), // Regular/flatscreen
	OVR(1, "Oculus Mobile VrApi (Deprecated)", "--xr_mode_ovr", ""),
	OPENXR(2, "OpenXR", "--xr_mode_openxr", "");

	final int index;
	final String label;
	public final String cmdLineArg;
	public final String inputFallbackMapping;

	XRMode(int index, String label, String cmdLineArg, String inputFallbackMapping) {
		this.index = index;
		this.label = label;
		this.cmdLineArg = cmdLineArg;
		this.inputFallbackMapping = inputFallbackMapping;
	}
}
