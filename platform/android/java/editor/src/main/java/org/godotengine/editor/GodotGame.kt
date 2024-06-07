/**************************************************************************/
/*  GodotGame.kt                                                          */
/**************************************************************************/


package org.godotengine.editor

/**
 * Drives the 'run project' window of the Godot Editor.
 */
class GodotGame : GodotEditor() {
	override fun overrideOrientationRequest() = false
}
