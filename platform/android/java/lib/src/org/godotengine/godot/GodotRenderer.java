/**************************************************************************/
/*  GodotRenderer.java                                                    */
/**************************************************************************/


package org.godotengine.godot;

import org.godotengine.godot.gl.GLSurfaceView;
import org.godotengine.godot.plugin.GodotPlugin;
import org.godotengine.godot.plugin.GodotPluginRegistry;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

/**
 * Godot's renderer implementation.
 */
class GodotRenderer implements GLSurfaceView.Renderer {
	private final GodotPluginRegistry pluginRegistry;
	private boolean activityJustResumed = false;

	GodotRenderer() {
		this.pluginRegistry = GodotPluginRegistry.getPluginRegistry();
	}

	public boolean onDrawFrame(GL10 gl) {
		if (activityJustResumed) {
			GodotLib.onRendererResumed();
			activityJustResumed = false;
		}

		boolean swapBuffers = GodotLib.step();
		for (int i = 0; i < Godot.singleton_count; i++) {
			Godot.singletons[i].onGLDrawFrame(gl);
		}
		for (GodotPlugin plugin : pluginRegistry.getAllPlugins()) {
			plugin.onGLDrawFrame(gl);
		}

		return swapBuffers;
	}

	public void onSurfaceChanged(GL10 gl, int width, int height) {
		GodotLib.resize(width, height);
		for (int i = 0; i < Godot.singleton_count; i++) {
			Godot.singletons[i].onGLSurfaceChanged(gl, width, height);
		}
		for (GodotPlugin plugin : pluginRegistry.getAllPlugins()) {
			plugin.onGLSurfaceChanged(gl, width, height);
		}
	}

	public void onSurfaceCreated(GL10 gl, EGLConfig config) {
		GodotLib.newcontext();
		for (GodotPlugin plugin : pluginRegistry.getAllPlugins()) {
			plugin.onGLSurfaceCreated(gl, config);
		}
	}

	void onActivityResumed() {
		// We defer invoking GodotLib.onRendererResumed() until the first draw frame call.
		// This ensures we have a valid GL context and surface when we do so.
		activityJustResumed = true;
	}

	void onActivityPaused() {
		GodotLib.onRendererPaused();
	}
}
