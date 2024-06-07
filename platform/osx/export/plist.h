/**************************************************************************/
/*  plist.h                                                               */
/**************************************************************************/


// Property list file format (application/x-plist) parser, property list ASN-1 serialization.

#ifndef OSX_PLIST_H
#define OSX_PLIST_H

#include "core/crypto/crypto_core.h"
#include "core/os/file_access.h"
#include "modules/modules_enabled.gen.h" // For regex.

#ifdef MODULE_REGEX_ENABLED

class PListNode;

class PList : public Reference {
	friend class PListNode;

public:
	enum PLNodeType {
		PL_NODE_TYPE_NIL,
		PL_NODE_TYPE_STRING,
		PL_NODE_TYPE_ARRAY,
		PL_NODE_TYPE_DICT,
		PL_NODE_TYPE_BOOLEAN,
		PL_NODE_TYPE_INTEGER,
		PL_NODE_TYPE_REAL,
		PL_NODE_TYPE_DATA,
		PL_NODE_TYPE_DATE,
	};

private:
	Ref<PListNode> root;

public:
	PList();
	PList(const String &p_string);

	bool load_file(const String &p_filename);
	bool load_string(const String &p_string);

	PoolByteArray save_asn1() const;
	String save_text() const;

	Ref<PListNode> get_root();
};

/*************************************************************************/

class PListNode : public Reference {
	static int _asn1_size_len(uint8_t p_len_octets);

public:
	PList::PLNodeType data_type = PList::PLNodeType::PL_NODE_TYPE_NIL;

	CharString data_string;
	Vector<Ref<PListNode>> data_array;
	Map<String, Ref<PListNode>> data_dict;
	union {
		int32_t data_int;
		bool data_bool;
		float data_real;
	};

	static Ref<PListNode> new_array();
	static Ref<PListNode> new_dict();
	static Ref<PListNode> new_string(const String &p_string);
	static Ref<PListNode> new_data(const String &p_string);
	static Ref<PListNode> new_date(const String &p_string);
	static Ref<PListNode> new_bool(bool p_bool);
	static Ref<PListNode> new_int(int32_t p_int);
	static Ref<PListNode> new_real(float p_real);

	bool push_subnode(const Ref<PListNode> &p_node, const String &p_key = "");

	size_t get_asn1_size(uint8_t p_len_octets) const;

	void store_asn1_size(PoolByteArray &p_stream, uint8_t p_len_octets) const;
	bool store_asn1(PoolByteArray &p_stream, uint8_t p_len_octets) const;
	void store_text(String &p_stream, uint8_t p_indent) const;

	PListNode() {}
	~PListNode() {}
};

#endif // MODULE_REGEX_ENABLED

#endif // OSX_PLIST_H
