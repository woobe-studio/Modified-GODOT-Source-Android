/**************************************************************************/
/*  GodotNetUtils.java                                                    */
/**************************************************************************/


package org.godotengine.godot.utils;

import android.app.Activity;
import android.content.Context;
import android.net.wifi.WifiManager;
import android.util.Log;

/**
 * This class handles Android-specific networking functions.
 * For now, it only provides access to WifiManager.MulticastLock, which is needed on some devices
 * to receive broadcast and multicast packets.
 */
public class GodotNetUtils {
	/* A single, reference counted, multicast lock, or null if permission CHANGE_WIFI_MULTICAST_STATE is missing */
	private WifiManager.MulticastLock multicastLock;

	public GodotNetUtils(Activity p_activity) {
		if (PermissionsUtil.hasManifestPermission(p_activity, "android.permission.CHANGE_WIFI_MULTICAST_STATE")) {
			WifiManager wifi = (WifiManager)p_activity.getApplicationContext().getSystemService(Context.WIFI_SERVICE);
			multicastLock = wifi.createMulticastLock("GodotMulticastLock");
			multicastLock.setReferenceCounted(true);
		}
	}

	/**
	 * Acquire the multicast lock. This is required on some devices to receive broadcast/multicast packets.
	 * This is done automatically by Godot when enabling broadcast or joining a multicast group on a socket.
	 */
	public void multicastLockAcquire() {
		if (multicastLock == null)
			return;
		try {
			multicastLock.acquire();
		} catch (RuntimeException e) {
			Log.e("Godot", "Exception during multicast lock acquire: " + e);
		}
	}

	/**
	 * Release the multicast lock.
	 * This is done automatically by Godot when the lock is no longer needed by a socket.
	 */
	public void multicastLockRelease() {
		if (multicastLock == null)
			return;
		try {
			multicastLock.release();
		} catch (RuntimeException e) {
			Log.e("Godot", "Exception during multicast lock release: " + e);
		}
	}
}
