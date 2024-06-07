/**************************************************************************/
/*  GodotProjectManager.kt                                                */
/**************************************************************************/


package org.godotengine.editor

/**
 * Launcher activity for the Godot Android Editor.
 *
 * It presents the user with the project manager interface.
 * Upon selection of a project, this activity (via its parent logic) starts the
 * [GodotEditor] activity.
 */
class GodotProjectManager : GodotEditor()
