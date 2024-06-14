/**************************************************************************/
/*  texture.h                                                             */
/**************************************************************************/


#ifndef TEXTURE_H
#define TEXTURE_H

#include "core/io/resource_loader.h"
#include "core/math/rect2.h"
#include "core/os/mutex.h"
#include "core/os/rw_lock.h"
#include "core/os/thread_safe.h"
#include "core/resource.h"
#include "scene/resources/curve.h"
#include "scene/resources/gradient.h"
#include "core/object.h"
#include "core/os/thread_safe.h"
#include "core/reference.h"
#include "core/rid.h"
#include "core/variant.h"
#include "servers/visual_server.h"

class Texture : public Resource {
	GDCLASS(Texture, Resource);
	OBJ_SAVE_TYPE(Texture); // Saves derived classes with common type so they can be interchanged.

protected:
	static void _bind_methods();

public:
	enum Flags {
		FLAG_MIPMAPS = VisualServer::TEXTURE_FLAG_MIPMAPS,
		FLAG_REPEAT = VisualServer::TEXTURE_FLAG_REPEAT,
		FLAG_FILTER = VisualServer::TEXTURE_FLAG_FILTER,
		FLAG_ANISOTROPIC_FILTER = VisualServer::TEXTURE_FLAG_ANISOTROPIC_FILTER,
		FLAG_CONVERT_TO_LINEAR = VisualServer::TEXTURE_FLAG_CONVERT_TO_LINEAR,
		FLAG_VIDEO_SURFACE = VisualServer::TEXTURE_FLAG_USED_FOR_STREAMING,
		FLAGS_DEFAULT = FLAG_MIPMAPS | FLAG_REPEAT | FLAG_FILTER,
		FLAG_MIRRORED_REPEAT = VisualServer::TEXTURE_FLAG_MIRRORED_REPEAT
	};

	virtual int get_width() const = 0;
	virtual int get_height() const = 0;
	virtual Size2 get_size() const;
	virtual RID get_rid() const = 0;

	virtual bool is_pixel_opaque(int p_x, int p_y) const;

	virtual bool has_alpha() const = 0;

	virtual void set_flags(uint32_t p_flags) = 0;
	virtual uint32_t get_flags() const = 0;

	virtual void draw(RID p_canvas_item, const Point2 &p_pos, const Color &p_modulate = Color(1, 1, 1), bool p_transpose = false, const Ref<Texture> &p_normal_map = Ref<Texture>()) const;
	virtual void draw_rect(RID p_canvas_item, const Rect2 &p_rect, bool p_tile = false, const Color &p_modulate = Color(1, 1, 1), bool p_transpose = false, const Ref<Texture> &p_normal_map = Ref<Texture>()) const;
	virtual void draw_rect_region(RID p_canvas_item, const Rect2 &p_rect, const Rect2 &p_src_rect, const Color &p_modulate = Color(1, 1, 1), bool p_transpose = false, const Ref<Texture> &p_normal_map = Ref<Texture>(), bool p_clip_uv = true) const;
	virtual bool get_rect_region(const Rect2 &p_rect, const Rect2 &p_src_rect, Rect2 &r_rect, Rect2 &r_src_rect) const;

	virtual Ref<Image> get_data() const { return Ref<Image>(); }

	Texture();
};

VARIANT_ENUM_CAST(Texture::Flags);

class BitMap;

class ImageTexture : public Texture {
	GDCLASS(ImageTexture, Texture);
	RES_BASE_EXTENSION("tex");

public:
	enum Storage {
		STORAGE_RAW,
		STORAGE_COMPRESS_LOSSY,
		STORAGE_COMPRESS_LOSSLESS
	};

private:
	RID texture;
	Image::Format format;
	uint32_t flags;
	int w, h;
	Storage storage;
	float lossy_storage_quality;
	mutable Ref<BitMap> alpha_cache;
	bool image_stored;

protected:
	virtual void reload_from_file();

	bool _set(const StringName &p_name, const Variant &p_value);
	bool _get(const StringName &p_name, Variant &r_ret) const;
	void _get_property_list(List<PropertyInfo> *p_list) const;

	void _reload_hook(const RID &p_hook);
	virtual void _resource_path_changed();
	static void _bind_methods();

	void _set_data(Dictionary p_data);

public:
	void create(int p_width, int p_height, Image::Format p_format, uint32_t p_flags = FLAGS_DEFAULT);
	void create_from_image(const Ref<Image> &p_image, uint32_t p_flags = FLAGS_DEFAULT);

	void set_flags(uint32_t p_flags);
	uint32_t get_flags() const;
	Image::Format get_format() const;
#ifndef DISABLE_DEPRECATED
	Error load(const String &p_path);
#endif
	void set_data(const Ref<Image> &p_image);
	Ref<Image> get_data() const;

	int get_width() const;
	int get_height() const;

	virtual RID get_rid() const;

	bool has_alpha() const;
	virtual void draw(RID p_canvas_item, const Point2 &p_pos, const Color &p_modulate = Color(1, 1, 1), bool p_transpose = false, const Ref<Texture> &p_normal_map = Ref<Texture>()) const;
	virtual void draw_rect(RID p_canvas_item, const Rect2 &p_rect, bool p_tile = false, const Color &p_modulate = Color(1, 1, 1), bool p_transpose = false, const Ref<Texture> &p_normal_map = Ref<Texture>()) const;
	virtual void draw_rect_region(RID p_canvas_item, const Rect2 &p_rect, const Rect2 &p_src_rect, const Color &p_modulate = Color(1, 1, 1), bool p_transpose = false, const Ref<Texture> &p_normal_map = Ref<Texture>(), bool p_clip_uv = true) const;
	void set_storage(Storage p_storage);
	Storage get_storage() const;

	bool is_pixel_opaque(int p_x, int p_y) const;

	void set_lossy_storage_quality(float p_lossy_storage_quality);
	float get_lossy_storage_quality() const;

	void set_size_override(const Size2 &p_size);

	virtual void set_path(const String &p_path, bool p_take_over = false);

	ImageTexture();
	~ImageTexture();
};

class StreamTexture : public Texture {
	GDCLASS(StreamTexture, Texture);

public:
	enum FormatBits {
		FORMAT_MASK_IMAGE_FORMAT = (1 << 20) - 1,
		FORMAT_BIT_PNG = 1 << 20,
		FORMAT_BIT_WEBP = 1 << 21,
		FORMAT_BIT_STREAM = 1 << 22,
		FORMAT_BIT_HAS_MIPMAPS = 1 << 23,
		FORMAT_BIT_DETECT_3D = 1 << 24,
		FORMAT_BIT_DETECT_SRGB = 1 << 25,
		FORMAT_BIT_DETECT_NORMAL = 1 << 26,
	};

private:
	Error _load_data(const String &p_path, int &tw, int &th, int &tw_custom, int &th_custom, int &flags, Ref<Image> &image, int p_size_limit = 0);
	String path_to_file;
	RID texture;
	Image::Format format;
	uint32_t flags;
	int w, h;
	mutable Ref<BitMap> alpha_cache;

	virtual void reload_from_file();

	static void _requested_3d(void *p_ud);
	static void _requested_srgb(void *p_ud);
	static void _requested_normal(void *p_ud);

protected:
	static void _bind_methods();
	void _validate_property(PropertyInfo &property) const;

public:
	typedef void (*TextureFormatRequestCallback)(const Ref<StreamTexture> &);

	static TextureFormatRequestCallback request_3d_callback;
	static TextureFormatRequestCallback request_srgb_callback;
	static TextureFormatRequestCallback request_normal_callback;

	uint32_t get_flags() const;
	Image::Format get_format() const;
	Error load(const String &p_path);
	String get_load_path() const;

	int get_width() const;
	int get_height() const;
	virtual RID get_rid() const;

	virtual void set_path(const String &p_path, bool p_take_over);

	virtual void draw(RID p_canvas_item, const Point2 &p_pos, const Color &p_modulate = Color(1, 1, 1), bool p_transpose = false, const Ref<Texture> &p_normal_map = Ref<Texture>()) const;
	virtual void draw_rect(RID p_canvas_item, const Rect2 &p_rect, bool p_tile = false, const Color &p_modulate = Color(1, 1, 1), bool p_transpose = false, const Ref<Texture> &p_normal_map = Ref<Texture>()) const;
	virtual void draw_rect_region(RID p_canvas_item, const Rect2 &p_rect, const Rect2 &p_src_rect, const Color &p_modulate = Color(1, 1, 1), bool p_transpose = false, const Ref<Texture> &p_normal_map = Ref<Texture>(), bool p_clip_uv = true) const;

	virtual bool has_alpha() const;
	virtual void set_flags(uint32_t p_flags);
	bool is_pixel_opaque(int p_x, int p_y) const;

	virtual Ref<Image> get_data() const;

	StreamTexture();
	~StreamTexture();
};

class ResourceFormatLoaderStreamTexture : public ResourceFormatLoader {
public:
	virtual RES load(const String &p_path, const String &p_original_path = "", Error *r_error = nullptr);
	virtual void get_recognized_extensions(List<String> *p_extensions) const;
	virtual bool handles_type(const String &p_type) const;
	virtual String get_resource_type(const String &p_path) const;
};

VARIANT_ENUM_CAST(ImageTexture::Storage);

class AtlasTexture : public Texture {
	GDCLASS(AtlasTexture, Texture);
	RES_BASE_EXTENSION("atlastex");

protected:
	Ref<Texture> atlas;
	Rect2 region;
	Rect2 margin;
	bool filter_clip;

	static void _bind_methods();

public:
	virtual int get_width() const;
	virtual int get_height() const;
	virtual RID get_rid() const;

	virtual bool has_alpha() const;

	virtual void set_flags(uint32_t p_flags);
	virtual uint32_t get_flags() const;

	void set_atlas(const Ref<Texture> &p_atlas);
	Ref<Texture> get_atlas() const;

	void set_region(const Rect2 &p_region);
	Rect2 get_region() const;

	void set_margin(const Rect2 &p_margin);
	Rect2 get_margin() const;

	void set_filter_clip(const bool p_enable);
	bool has_filter_clip() const;

	virtual Ref<Image> get_data() const;

	virtual void draw(RID p_canvas_item, const Point2 &p_pos, const Color &p_modulate = Color(1, 1, 1), bool p_transpose = false, const Ref<Texture> &p_normal_map = Ref<Texture>()) const;
	virtual void draw_rect(RID p_canvas_item, const Rect2 &p_rect, bool p_tile = false, const Color &p_modulate = Color(1, 1, 1), bool p_transpose = false, const Ref<Texture> &p_normal_map = Ref<Texture>()) const;
	virtual void draw_rect_region(RID p_canvas_item, const Rect2 &p_rect, const Rect2 &p_src_rect, const Color &p_modulate = Color(1, 1, 1), bool p_transpose = false, const Ref<Texture> &p_normal_map = Ref<Texture>(), bool p_clip_uv = true) const;
	virtual bool get_rect_region(const Rect2 &p_rect, const Rect2 &p_src_rect, Rect2 &r_rect, Rect2 &r_src_rect) const;

	bool is_pixel_opaque(int p_x, int p_y) const;

	AtlasTexture();
};

class Mesh;

class GradientTexture2D : public Texture {
	GDCLASS(GradientTexture2D, Texture);

public:
	enum Fill {
		FILL_LINEAR,
		FILL_RADIAL,
	};
	enum Repeat {
		REPEAT_NONE,
		REPEAT,
		REPEAT_MIRROR,
	};

private:
	Ref<Gradient> gradient;
	mutable RID texture;

	int width = 64;
	int height = 64;

	uint32_t flags = FLAGS_DEFAULT;

	bool use_hdr = false;

	Vector2 fill_from;
	Vector2 fill_to = Vector2(1, 0);

	Fill fill = FILL_LINEAR;
	Repeat repeat = REPEAT_NONE;

	float _get_gradient_offset_at(int x, int y) const;

	bool update_pending = false;
	void _queue_update();
	void _update();

protected:
	static void _bind_methods();

public:
	void set_gradient(Ref<Gradient> p_gradient);
	Ref<Gradient> get_gradient() const;

	void set_width(int p_width);
	virtual int get_width() const;
	void set_height(int p_height);
	virtual int get_height() const;

	virtual void set_flags(uint32_t p_flags);
	virtual uint32_t get_flags() const;

	void set_use_hdr(bool p_enabled);
	bool is_using_hdr() const;

	void set_fill(Fill p_fill);
	Fill get_fill() const;
	void set_fill_from(Vector2 p_fill_from);
	Vector2 get_fill_from() const;
	void set_fill_to(Vector2 p_fill_to);
	Vector2 get_fill_to() const;

	void set_repeat(Repeat p_repeat);
	Repeat get_repeat() const;

	virtual RID get_rid() const;
	virtual bool has_alpha() const { return true; }
	virtual Ref<Image> get_data() const;

	GradientTexture2D();
	virtual ~GradientTexture2D();
};

VARIANT_ENUM_CAST(GradientTexture2D::Fill);
VARIANT_ENUM_CAST(GradientTexture2D::Repeat);

#endif // TEXTURE_H
