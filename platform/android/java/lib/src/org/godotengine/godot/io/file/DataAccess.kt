/**************************************************************************/
/*  DataAccess.kt                                                         */
/**************************************************************************/


package org.godotengine.godot.io.file

import android.content.Context
import android.os.Build
import android.util.Log
import org.godotengine.godot.io.StorageScope
import java.io.IOException
import java.nio.ByteBuffer
import java.nio.channels.FileChannel
import kotlin.math.max

/**
 * Base class for file IO operations.
 *
 * Its derived instances provide concrete implementations to handle regular file access, as well
 * as file access through the media store API on versions of Android were scoped storage is enabled.
 */
internal abstract class DataAccess(private val filePath: String) {

	companion object {
		private val TAG = DataAccess::class.java.simpleName

		fun generateDataAccess(
			storageScope: StorageScope,
			context: Context,
			filePath: String,
			accessFlag: FileAccessFlags
		): DataAccess? {
			return when (storageScope) {
				StorageScope.APP -> FileData(filePath, accessFlag)

				StorageScope.SHARED -> if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.Q) {
					MediaStoreData(context, filePath, accessFlag)
				} else {
					null
				}

				StorageScope.UNKNOWN -> null
			}
		}

		fun fileExists(storageScope: StorageScope, context: Context, path: String): Boolean {
			return when(storageScope) {
				StorageScope.APP -> FileData.fileExists(path)
				StorageScope.SHARED -> MediaStoreData.fileExists(context, path)
				StorageScope.UNKNOWN -> false
			}
		}

		fun fileLastModified(storageScope: StorageScope, context: Context, path: String): Long {
			return when(storageScope) {
				StorageScope.APP -> FileData.fileLastModified(path)
				StorageScope.SHARED -> MediaStoreData.fileLastModified(context, path)
				StorageScope.UNKNOWN -> 0L
			}
		}

		fun removeFile(storageScope: StorageScope, context: Context, path: String): Boolean {
			return when(storageScope) {
				StorageScope.APP -> FileData.delete(path)
				StorageScope.SHARED -> MediaStoreData.delete(context, path)
				StorageScope.UNKNOWN -> false
			}
		}

		fun renameFile(storageScope: StorageScope, context: Context, from: String, to: String): Boolean {
			return when(storageScope) {
				StorageScope.APP -> FileData.rename(from, to)
				StorageScope.SHARED -> MediaStoreData.rename(context, from, to)
				StorageScope.UNKNOWN -> false
			}
		}
	}

	protected abstract val fileChannel: FileChannel
	internal var endOfFile = false

	fun close() {
		try {
			fileChannel.close()
		} catch (e: IOException) {
			Log.w(TAG, "Exception when closing file $filePath.", e)
		}
	}

	fun flush() {
		try {
			fileChannel.force(false)
		} catch (e: IOException) {
			Log.w(TAG, "Exception when flushing file $filePath.", e)
		}
	}

	fun seek(position: Long) {
		try {
			fileChannel.position(position)
			endOfFile = position >= fileChannel.size()
		} catch (e: Exception) {
			Log.w(TAG, "Exception when seeking file $filePath.", e)
		}
	}

	fun seekFromEnd(positionFromEnd: Long) {
		val positionFromBeginning = max(0, size() - positionFromEnd)
		seek(positionFromBeginning)
	}

	fun position(): Long {
		return try {
			fileChannel.position()
		} catch (e: IOException) {
			Log.w(
				TAG,
				"Exception when retrieving position for file $filePath.",
				e
			)
			0L
		}
	}

	fun size() = try {
		fileChannel.size()
	} catch (e: IOException) {
		Log.w(TAG, "Exception when retrieving size for file $filePath.", e)
		0L
	}

	fun read(buffer: ByteBuffer): Int {
		return try {
			val readBytes = fileChannel.read(buffer)
			endOfFile = readBytes == -1 || (fileChannel.position() >= fileChannel.size())
			if (readBytes == -1) {
				0
			} else {
				readBytes
			}
		} catch (e: IOException) {
			Log.w(TAG, "Exception while reading from file $filePath.", e)
			0
		}
	}

	fun write(buffer: ByteBuffer) {
		try {
			val writtenBytes = fileChannel.write(buffer)
			if (writtenBytes > 0) {
				endOfFile = false
			}
		} catch (e: IOException) {
			Log.w(TAG, "Exception while writing to file $filePath.", e)
		}
	}
}
