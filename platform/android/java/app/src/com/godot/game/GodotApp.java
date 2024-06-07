/**************************************************************************/
/*  GodotApp.java                                                         */
/**************************************************************************/


package com.godot.game;

import org.godotengine.godot.FullScreenGodotApp;

import android.os.Bundle;

/**
 * Template activity for Godot Android custom builds.
 * Feel free to extend and modify this class for your custom logic.
 */
public class GodotApp extends FullScreenGodotApp {
	@Override
	public void onCreate(Bundle savedInstanceState) {
		setTheme(R.style.GodotAppMainTheme);
		super.onCreate(savedInstanceState);
	}
}
