/**************************************************************************/
/*  GodotHost.java                                                        */
/**************************************************************************/


package org.godotengine.godot;

import java.util.Collections;
import java.util.List;

/**
 * Denotate a component (e.g: Activity, Fragment) that hosts the {@link Godot} fragment.
 */
public interface GodotHost {
	/**
	 * Provides a set of command line parameters to setup the engine.
	 */
	default List<String> getCommandLine() {
		return Collections.emptyList();
	}

	/**
	 * Invoked on the render thread when the Godot setup is complete.
	 */
	default void onGodotSetupCompleted() {}

	/**
	 * Invoked on the render thread when the Godot main loop has started.
	 */
	default void onGodotMainLoopStarted() {}

	/**
	 * Invoked on the UI thread as the last step of the Godot instance clean up phase.
	 */
	default void onGodotForceQuit(Godot instance) {}

	/**
	 * Invoked on the UI thread when the Godot instance wants to be restarted. It's up to the host
	 * to perform the appropriate action(s).
	 */
	default void onGodotRestartRequested(Godot instance) {}

	/**
	 * Invoked on the UI thread when a new Godot instance is requested. It's up to the host to
	 * perform the appropriate action(s).
	 *
	 * @param args Arguments used to initialize the new instance.
	 */
	default void onNewGodotInstanceRequested(String[] args) {}
}
