/**************************************************************************/
/*  StorageScope.kt                                                       */
/**************************************************************************/


package org.godotengine.godot.io

import android.content.Context
import android.os.Build
import android.os.Environment
import java.io.File

/**
 * Represents the different storage scopes.
 */
internal enum class StorageScope {
	/**
	 * Covers internal and external directories accessible to the app without restrictions.
	 */
	APP,

	/**
	 * Covers shared directories (from Android 10 and higher).
	 */
	SHARED,

	/**
	 * Everything else..
	 */
	UNKNOWN;

	class Identifier(context: Context) {

		private val internalAppDir: String? = context.filesDir.canonicalPath
		private val internalCacheDir: String? = context.cacheDir.canonicalPath
		private val externalAppDir: String? = context.getExternalFilesDir(null)?.canonicalPath
		private val sharedDir : String? = Environment.getExternalStorageDirectory().canonicalPath
		private val downloadsSharedDir: String? = Environment.getExternalStoragePublicDirectory(Environment.DIRECTORY_DOWNLOADS).canonicalPath
		private val documentsSharedDir: String? = Environment.getExternalStoragePublicDirectory(Environment.DIRECTORY_DOCUMENTS).canonicalPath

		/**
		 * Determines which [StorageScope] the given path falls under.
		 */
		fun identifyStorageScope(path: String?): StorageScope {
			if (path == null) {
				return UNKNOWN
			}

			val pathFile = File(path)
			if (!pathFile.isAbsolute) {
				return UNKNOWN
			}

			// If we have 'All Files Access' permission, we can access all directories without
			// restriction.
			if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.R
				&& Environment.isExternalStorageManager()) {
				return APP
			}

			val canonicalPathFile = pathFile.canonicalPath

			if (internalAppDir != null && canonicalPathFile.startsWith(internalAppDir)) {
				return APP
			}

			if (internalCacheDir != null && canonicalPathFile.startsWith(internalCacheDir)) {
				return APP
			}

			if (externalAppDir != null && canonicalPathFile.startsWith(externalAppDir)) {
				return APP
			}

			val rootDir: String? = System.getenv("ANDROID_ROOT")
			if (rootDir != null && canonicalPathFile.startsWith(rootDir)) {
				return APP
			}

			if (sharedDir != null && canonicalPathFile.startsWith(sharedDir)) {
				if (Build.VERSION.SDK_INT < Build.VERSION_CODES.R) {
					// Before R, apps had access to shared storage so long as they have the right
					// permissions (and flag on Q).
					return APP
				}

				// Post R, access is limited based on the target destination
				// 'Downloads' and 'Documents' are still accessible
				if ((downloadsSharedDir != null && canonicalPathFile.startsWith(downloadsSharedDir))
					|| (documentsSharedDir != null && canonicalPathFile.startsWith(documentsSharedDir))) {
					return APP
				}

				return SHARED
			}

			return UNKNOWN
		}
	}
}
