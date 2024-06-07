/**************************************************************************/
/*  file_access_filesystem_jandroid.h                                     */
/**************************************************************************/


#ifndef FILE_ACCESS_FILESYSTEM_JANDROID_H
#define FILE_ACCESS_FILESYSTEM_JANDROID_H

#include "core/os/file_access.h"
#include "java_godot_lib_jni.h"

class FileAccessFilesystemJAndroid : public FileAccess {
	static jobject file_access_handler;
	static jclass cls;

	static jmethodID _file_open;
	static jmethodID _file_get_size;
	static jmethodID _file_seek;
	static jmethodID _file_seek_end;
	static jmethodID _file_tell;
	static jmethodID _file_eof;
	static jmethodID _file_set_eof;
	static jmethodID _file_read;
	static jmethodID _file_write;
	static jmethodID _file_flush;
	static jmethodID _file_close;
	static jmethodID _file_exists;
	static jmethodID _file_last_modified;

	int id;
	String absolute_path;
	String path_src;

	void _set_eof(bool eof);

public:
	virtual Error _open(const String &p_path, int p_mode_flags) override; ///< open a file
	virtual void close() override; ///< close a file
	virtual bool is_open() const override; ///< true when file is open

	/// returns the path for the current open file
	virtual String get_path() const override;
	/// returns the absolute path for the current open file
	virtual String get_path_absolute() const override;

	virtual void seek(uint64_t p_position) override; ///< seek to a given position
	virtual void seek_end(int64_t p_position = 0) override; ///< seek from the end of file
	virtual uint64_t get_position() const override; ///< get position in the file
	virtual uint64_t get_len() const override; ///< get size of the file

	virtual bool eof_reached() const override; ///< reading passed EOF

	virtual uint8_t get_8() const override; ///< get a byte
	virtual String get_line() const override; ///< get a line
	virtual uint64_t get_buffer(uint8_t *p_dst, uint64_t p_length) const override;

	virtual Error get_error() const override; ///< get last error

	virtual void flush() override;
	virtual void store_8(uint8_t p_dest) override; ///< store a byte
	virtual void store_buffer(const uint8_t *p_src, uint64_t p_length) override;

	virtual bool file_exists(const String &p_path) override; ///< return true if a file exists

	static void setup(jobject p_file_access_handler);

	virtual uint64_t _get_modified_time(const String &p_file) override;
	virtual uint32_t _get_unix_permissions(const String &p_file) override { return 0; }
	virtual Error _set_unix_permissions(const String &p_file, uint32_t p_permissions) override { return FAILED; }

	FileAccessFilesystemJAndroid();
	~FileAccessFilesystemJAndroid();
};

#endif // FILE_ACCESS_FILESYSTEM_JANDROID_H
