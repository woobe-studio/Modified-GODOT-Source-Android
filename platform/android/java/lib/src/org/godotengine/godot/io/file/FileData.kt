/**************************************************************************/
/*  FileData.kt                                                           */
/**************************************************************************/


package org.godotengine.godot.io.file

import java.io.File
import java.io.FileOutputStream
import java.io.RandomAccessFile
import java.nio.channels.FileChannel

/**
 * Implementation of [DataAccess] which handles regular (not scoped) file access and interactions.
 */
internal class FileData(filePath: String, accessFlag: FileAccessFlags) : DataAccess(filePath) {

	companion object {
		private val TAG = FileData::class.java.simpleName

		fun fileExists(path: String): Boolean {
			return try {
				File(path).isFile
			} catch (e: SecurityException) {
				false
			}
		}

		fun fileLastModified(filepath: String): Long {
			return try {
				File(filepath).lastModified()
			} catch (e: SecurityException) {
				0L
			}
		}

		fun delete(filepath: String): Boolean {
			return try {
				File(filepath).delete()
			} catch (e: Exception) {
				false
			}
		}

		fun rename(from: String, to: String): Boolean {
			return try {
				val fromFile = File(from)
				fromFile.renameTo(File(to))
			} catch (e: Exception) {
				false
			}
		}
	}

	override val fileChannel: FileChannel

	init {
		if (accessFlag == FileAccessFlags.WRITE) {
			fileChannel = FileOutputStream(filePath, !accessFlag.shouldTruncate()).channel
		} else {
			fileChannel = RandomAccessFile(filePath, accessFlag.getMode()).channel
		}

		if (accessFlag.shouldTruncate()) {
			fileChannel.truncate(0)
		}
	}
}
