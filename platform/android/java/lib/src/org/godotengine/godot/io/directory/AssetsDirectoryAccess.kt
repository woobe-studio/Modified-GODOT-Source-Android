/**************************************************************************/
/*  AssetsDirectoryAccess.kt                                              */
/**************************************************************************/


package org.godotengine.godot.io.directory

import android.content.Context
import android.util.Log
import android.util.SparseArray
import org.godotengine.godot.io.directory.DirectoryAccessHandler.Companion.INVALID_DIR_ID
import org.godotengine.godot.io.directory.DirectoryAccessHandler.Companion.STARTING_DIR_ID
import java.io.File
import java.io.IOException

/**
 * Handles directories access within the Android assets directory.
 */
internal class AssetsDirectoryAccess(context: Context) : DirectoryAccessHandler.DirectoryAccess {

	companion object {
		private val TAG = AssetsDirectoryAccess::class.java.simpleName
	}

	private data class AssetDir(val path: String, val files: Array<String>, var current: Int = 0)

	private val assetManager = context.assets

	private var lastDirId = STARTING_DIR_ID
	private val dirs = SparseArray<AssetDir>()

	private fun getAssetsPath(originalPath: String): String {
		if (originalPath.startsWith(File.separatorChar)) {
			return originalPath.substring(1)
		}
		return originalPath
	}

	override fun hasDirId(dirId: Int) = dirs.indexOfKey(dirId) >= 0

	override fun dirOpen(path: String): Int {
		val assetsPath = getAssetsPath(path)
		try {
			val files = assetManager.list(assetsPath) ?: return INVALID_DIR_ID
			// Empty directories don't get added to the 'assets' directory, so
			// if ad.files.length > 0 ==> path is directory
			// if ad.files.length == 0 ==> path is file
			if (files.isEmpty()) {
				return INVALID_DIR_ID
			}

			val ad = AssetDir(assetsPath, files)

			dirs.put(++lastDirId, ad)
			return lastDirId
		} catch (e: IOException) {
			Log.e(TAG, "Exception on dirOpen", e)
			return INVALID_DIR_ID
		}
	}

	override fun dirExists(path: String): Boolean {
		val assetsPath = getAssetsPath(path)
		try {
			val files = assetManager.list(assetsPath) ?: return false
			// Empty directories don't get added to the 'assets' directory, so
			// if ad.files.length > 0 ==> path is directory
			// if ad.files.length == 0 ==> path is file
			return files.isNotEmpty()
		} catch (e: IOException) {
			Log.e(TAG, "Exception on dirExists", e)
			return false
		}
	}

	override fun fileExists(path: String): Boolean {
		val assetsPath = getAssetsPath(path)
		try {
			val files = assetManager.list(assetsPath) ?: return false
			// Empty directories don't get added to the 'assets' directory, so
			// if ad.files.length > 0 ==> path is directory
			// if ad.files.length == 0 ==> path is file
			return files.isEmpty()
		} catch (e: IOException) {
			Log.e(TAG, "Exception on fileExists", e)
			return false
		}
	}

	override fun dirIsDir(dirId: Int): Boolean {
		val ad: AssetDir = dirs[dirId]

		var idx = ad.current
		if (idx > 0) {
			idx--
		}

		if (idx >= ad.files.size) {
			return false
		}

		val fileName = ad.files[idx]
		// List the contents of $fileName. If it's a file, it will be empty, otherwise it'll be a
		// directory
		val filePath = if (ad.path == "") fileName else "${ad.path}/${fileName}"
		val fileContents = assetManager.list(filePath)
		return (fileContents?.size?: 0) > 0
	}

	override fun isCurrentHidden(dirId: Int): Boolean {
		val ad = dirs[dirId]

		var idx = ad.current
		if (idx > 0) {
			idx--
		}

		if (idx >= ad.files.size) {
			return false
		}

		val fileName = ad.files[idx]
		return fileName.startsWith('.')
	}

	override fun dirNext(dirId: Int): String {
		val ad: AssetDir = dirs[dirId]

		if (ad.current >= ad.files.size) {
			ad.current++
			return ""
		}

		return ad.files[ad.current++]
	}

	override fun dirClose(dirId: Int) {
		dirs.remove(dirId)
	}

	override fun getDriveCount() = 0

	override fun getDrive(drive: Int) = ""

	override fun makeDir(dir: String) = false

	override fun getSpaceLeft() = 0L

	override fun rename(from: String, to: String) = false

	override fun remove(filename: String) = false
}
