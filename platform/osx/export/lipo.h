/**************************************************************************/
/*  lipo.h                                                                */
/**************************************************************************/


// Universal / Universal 2 fat binary file creator and extractor.

#ifndef OSX_LIPO_H
#define OSX_LIPO_H

#include "core/os/file_access.h"
#include "core/reference.h"
#include "modules/modules_enabled.gen.h" // For regex.

#include "macho.h"

#ifdef MODULE_REGEX_ENABLED

class LipO : public Reference {
	struct FatArch {
		uint32_t cputype;
		uint32_t cpusubtype;
		uint64_t offset;
		uint64_t size;
		uint32_t align;
	};

	FileAccess *fa = nullptr;
	Vector<FatArch> archs;

	static inline size_t PAD(size_t s, size_t a) {
		return (a - s % a);
	}

public:
	static bool is_lipo(const String &p_path);

	bool create_file(const String &p_output_path, const PoolStringArray &p_files);

	bool open_file(const String &p_path);
	int get_arch_count() const;
	bool extract_arch(int p_index, const String &p_path);

	void close();

	~LipO();
};

#endif // MODULE_REGEX_ENABLED

#endif // OSX_LIPO_H
