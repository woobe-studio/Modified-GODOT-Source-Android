/**************************************************************************/
/*  GodotGestureHandler.java                                              */
/**************************************************************************/


package org.godotengine.godot.input;

import org.godotengine.godot.GodotLib;
import org.godotengine.godot.GodotView;

import android.view.GestureDetector;
import android.view.MotionEvent;

/**
 * Handles gesture input related events for the {@link GodotView} view.
 * https://developer.android.com/reference/android/view/GestureDetector.SimpleOnGestureListener
 */
public class GodotGestureHandler extends GestureDetector.SimpleOnGestureListener {
	private final GodotView godotView;

	public GodotGestureHandler(GodotView godotView) {
		this.godotView = godotView;
	}

	private void queueEvent(Runnable task) {
		godotView.queueEvent(task);
	}

	@Override
	public boolean onDown(MotionEvent event) {
		super.onDown(event);
		//Log.i("GodotGesture", "onDown");
		return true;
	}

	@Override
	public boolean onSingleTapConfirmed(MotionEvent event) {
		super.onSingleTapConfirmed(event);
		return true;
	}

	@Override
	public void onLongPress(MotionEvent event) {
		//Log.i("GodotGesture", "onLongPress");
	}

	@Override
	public boolean onDoubleTap(MotionEvent event) {
		//Log.i("GodotGesture", "onDoubleTap");
		final int x = Math.round(event.getX());
		final int y = Math.round(event.getY());
		final int buttonMask = event.getButtonState();
		GodotLib.doubleTap(buttonMask, x, y);
		return true;
	}

	@Override
	public boolean onFling(MotionEvent event1, MotionEvent event2, float velocityX, float velocityY) {
		//Log.i("GodotGesture", "onFling");
		return true;
	}
}
