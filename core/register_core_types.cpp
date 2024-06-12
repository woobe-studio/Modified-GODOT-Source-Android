/**************************************************************************/
/*  register_core_types.cpp                                               */
/**************************************************************************/


#include "register_core_types.h"

#include "core/bind/core_bind.h"
#include "core/class_db.h"
#include "core/compressed_translation.h"
#include "core/core_string_names.h"
#include "core/crypto/aes_context.h"
#include "core/crypto/crypto.h"
#include "core/engine.h"
#include "core/func_ref.h"
#include "core/input_map.h"
#include "core/io/config_file.h"
#include "core/io/image_loader.h"
#include "core/io/marshalls.h"
#include "core/io/packet_peer.h"
#include "core/io/resource_format_binary.h"
#include "core/io/resource_importer.h"
#include "core/io/tcp_server.h"
#include "core/io/translation_loader_po.h"
#include "core/io/xml_parser.h"
#include "core/math/expression.h"
#include "core/math/random_number_generator.h"
#include "core/os/input.h"
#include "core/os/main_loop.h"
#include "core/os/time.h"
#include "core/packed_data_container.h"
#include "core/project_settings.h"
#include "core/translation.h"
#include "core/undo_redo.h"

static Ref<ResourceFormatSaverBinary> resource_saver_binary;
static Ref<ResourceFormatLoaderBinary> resource_loader_binary;
static Ref<ResourceFormatImporter> resource_format_importer;
static Ref<ResourceFormatLoaderImage> resource_format_image;
static Ref<TranslationLoaderPO> resource_format_po;
static Ref<ResourceFormatSaverCrypto> resource_format_saver_crypto;
static Ref<ResourceFormatLoaderCrypto> resource_format_loader_crypto;

static _ResourceLoader *_resource_loader = nullptr;
static _ResourceSaver *_resource_saver = nullptr;
static _OS *_os = nullptr;
static _Engine *_engine = nullptr;
static _ClassDB *_classdb = nullptr;
static _Marshalls *_marshalls = nullptr;
static _JSON *_json = nullptr;

static IP *ip = nullptr;

static _Geometry *_geometry = nullptr;

extern Mutex _global_mutex;

extern void register_global_constants();
extern void unregister_global_constants();
extern void register_variant_methods();
extern void unregister_variant_methods();

void register_core_types() {
	MemoryPool::setup();

	StringName::setup();

	register_global_constants();
	register_variant_methods();

	CoreStringNames::create();

	resource_format_po.instance();
	ResourceLoader::add_resource_format_loader(resource_format_po);

	resource_saver_binary.instance();
	ResourceSaver::add_resource_format_saver(resource_saver_binary);
	resource_loader_binary.instance();
	ResourceLoader::add_resource_format_loader(resource_loader_binary);

	resource_format_importer.instance();
	ResourceLoader::add_resource_format_loader(resource_format_importer);

	resource_format_image.instance();
	ResourceLoader::add_resource_format_loader(resource_format_image);

	ClassDB::register_class<Object>();

	ClassDB::register_virtual_class<Script>();

	ClassDB::register_class<Reference>();
	ClassDB::register_class<WeakRef>();
	ClassDB::register_class<Resource>();
	ClassDB::register_class<Image>();

	ClassDB::register_virtual_class<InputEvent>();
	ClassDB::register_virtual_class<InputEventWithModifiers>();
	ClassDB::register_class<InputEventKey>();
	ClassDB::register_virtual_class<InputEventMouse>();
	ClassDB::register_class<InputEventMouseButton>();
	ClassDB::register_class<InputEventMouseMotion>();
	ClassDB::register_class<InputEventJoypadButton>();
	ClassDB::register_class<InputEventJoypadMotion>();
	ClassDB::register_class<InputEventScreenDrag>();
	ClassDB::register_class<InputEventScreenTouch>();
	ClassDB::register_class<InputEventAction>();
	ClassDB::register_virtual_class<InputEventGesture>();
	ClassDB::register_class<InputEventMagnifyGesture>();
	ClassDB::register_class<InputEventPanGesture>();
	ClassDB::register_class<InputEventMIDI>();

	ClassDB::register_class<FuncRef>();
	ClassDB::register_virtual_class<StreamPeer>();
	ClassDB::register_class<StreamPeerTCP>();
	ClassDB::register_class<TCP_Server>();

	// Crypto
	ClassDB::register_class<AESContext>();
	ClassDB::register_custom_instance_class<X509Certificate>();
	ClassDB::register_custom_instance_class<CryptoKey>();
	ClassDB::register_custom_instance_class<Crypto>();

	resource_format_saver_crypto.instance();
	ResourceSaver::add_resource_format_saver(resource_format_saver_crypto);
	resource_format_loader_crypto.instance();
	ResourceLoader::add_resource_format_loader(resource_format_loader_crypto);

	ClassDB::register_virtual_class<IP>();
	ClassDB::register_virtual_class<PacketPeer>();
	ClassDB::register_class<PacketPeerStream>();
	ClassDB::register_class<MainLoop>();
	ClassDB::register_class<Translation>();
	ClassDB::register_class<PHashTranslation>();
	ClassDB::register_class<UndoRedo>();

	ClassDB::register_virtual_class<ResourceInteractiveLoader>();

	ClassDB::register_class<ResourceFormatLoader>();
	ClassDB::register_class<ResourceFormatSaver>();

	ClassDB::register_class<_File>();
	ClassDB::register_class<_Directory>();
	ClassDB::register_class<_Thread>();
	ClassDB::register_class<_Mutex>();
	ClassDB::register_class<_Semaphore>();

	ClassDB::register_class<XMLParser>();

	ClassDB::register_class<ConfigFile>();

	ClassDB::register_class<PackedDataContainer>();
	ClassDB::register_virtual_class<PackedDataContainerRef>();
	ClassDB::register_class<EncodedObjectAsID>();
	ClassDB::register_class<RandomNumberGenerator>();

	ClassDB::register_class<JSONParseResult>();

	ClassDB::register_virtual_class<ResourceImporter>();

	ip = IP::create();

	_geometry = memnew(_Geometry);

	_resource_loader = memnew(_ResourceLoader);
	_resource_saver = memnew(_ResourceSaver);
	_os = memnew(_OS);
	_engine = memnew(_Engine);
	_classdb = memnew(_ClassDB);
	_marshalls = memnew(_Marshalls);
	_json = memnew(_JSON);
}

void register_core_settings() {
	// Since in register core types, globals may not be present.
	GLOBAL_DEF("network/limits/tcp/connect_timeout_seconds", (30));
	ProjectSettings::get_singleton()->set_custom_property_info("network/limits/tcp/connect_timeout_seconds", PropertyInfo(Variant::INT, "network/limits/tcp/connect_timeout_seconds", PROPERTY_HINT_RANGE, "1,1800,1"));
	GLOBAL_DEF_RST("network/limits/packet_peer_stream/max_buffer_po2", (16));
	ProjectSettings::get_singleton()->set_custom_property_info("network/limits/packet_peer_stream/max_buffer_po2", PropertyInfo(Variant::INT, "network/limits/packet_peer_stream/max_buffer_po2", PROPERTY_HINT_RANGE, "0,64,1,or_greater"));

	GLOBAL_DEF("network/ssl/certificates", "");
	ProjectSettings::get_singleton()->set_custom_property_info("network/ssl/certificates", PropertyInfo(Variant::STRING, "network/ssl/certificates", PROPERTY_HINT_FILE, "*.crt"));
}

void register_core_singletons() {
	ClassDB::register_class<ProjectSettings>();
	ClassDB::register_virtual_class<IP>();
	ClassDB::register_class<_Geometry>();
	ClassDB::register_class<_ResourceLoader>();
	ClassDB::register_class<_ResourceSaver>();
	ClassDB::register_class<_OS>();
	ClassDB::register_class<_Engine>();
	ClassDB::register_class<_ClassDB>();
	ClassDB::register_class<_Marshalls>();
	ClassDB::register_class<TranslationServer>();
	ClassDB::register_virtual_class<Input>();
	ClassDB::register_class<InputMap>();
	ClassDB::register_class<_JSON>();
	ClassDB::register_class<Expression>();
	ClassDB::register_class<Time>();

	Engine::get_singleton()->add_singleton(Engine::Singleton("ProjectSettings", ProjectSettings::get_singleton()));
	Engine::get_singleton()->add_singleton(Engine::Singleton("IP", IP::get_singleton()));
	Engine::get_singleton()->add_singleton(Engine::Singleton("Geometry", _Geometry::get_singleton()));
	Engine::get_singleton()->add_singleton(Engine::Singleton("ResourceLoader", _ResourceLoader::get_singleton()));
	Engine::get_singleton()->add_singleton(Engine::Singleton("ResourceSaver", _ResourceSaver::get_singleton()));
	Engine::get_singleton()->add_singleton(Engine::Singleton("OS", _OS::get_singleton()));
	Engine::get_singleton()->add_singleton(Engine::Singleton("Engine", _Engine::get_singleton()));
	Engine::get_singleton()->add_singleton(Engine::Singleton("ClassDB", _classdb));
	Engine::get_singleton()->add_singleton(Engine::Singleton("Marshalls", _Marshalls::get_singleton()));
	Engine::get_singleton()->add_singleton(Engine::Singleton("TranslationServer", TranslationServer::get_singleton()));
	Engine::get_singleton()->add_singleton(Engine::Singleton("Input", Input::get_singleton()));
	Engine::get_singleton()->add_singleton(Engine::Singleton("InputMap", InputMap::get_singleton()));
	Engine::get_singleton()->add_singleton(Engine::Singleton("JSON", _JSON::get_singleton()));
	Engine::get_singleton()->add_singleton(Engine::Singleton("Time", Time::get_singleton()));
}

void unregister_core_types() {
	memdelete(_resource_loader);
	memdelete(_resource_saver);
	memdelete(_os);
	memdelete(_engine);
	memdelete(_classdb);
	memdelete(_marshalls);
	memdelete(_json);

	memdelete(_geometry);

	ResourceLoader::remove_resource_format_loader(resource_format_image);
	resource_format_image.unref();

	ResourceSaver::remove_resource_format_saver(resource_saver_binary);
	resource_saver_binary.unref();

	ResourceLoader::remove_resource_format_loader(resource_loader_binary);
	resource_loader_binary.unref();

	ResourceLoader::remove_resource_format_loader(resource_format_importer);
	resource_format_importer.unref();

	ResourceLoader::remove_resource_format_loader(resource_format_po);
	resource_format_po.unref();

	ResourceSaver::remove_resource_format_saver(resource_format_saver_crypto);
	resource_format_saver_crypto.unref();
	ResourceLoader::remove_resource_format_loader(resource_format_loader_crypto);
	resource_format_loader_crypto.unref();

	if (ip) {
		memdelete(ip);
	}

	ResourceLoader::finalize();

	ClassDB::cleanup_defaults();
	ObjectDB::cleanup();

	unregister_variant_methods();
	unregister_global_constants();

	ClassDB::cleanup();
	ResourceCache::clear();
	CoreStringNames::free();
	StringName::cleanup();

	MemoryPool::cleanup();
}
