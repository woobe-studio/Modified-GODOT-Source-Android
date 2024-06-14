/**************************************************************************/
/*  register_scene_types.cpp                                              */
/**************************************************************************/


#include "register_scene_types.h"

#include "core/class_db.h"
#include "core/os/os.h"
#include "core/project_settings.h"
#include "scene/2d/animated_sprite.h"
#include "scene/2d/area_2d.h"
#include "scene/2d/camera_2d.h"
#include "scene/2d/canvas_item.h"
#include "scene/2d/collision_polygon_2d.h"
#include "scene/2d/collision_shape_2d.h"
#include "scene/2d/line_2d.h"
#include "scene/2d/navigation_2d.h"
#include "scene/2d/navigation_agent_2d.h"
#include "scene/2d/navigation_obstacle_2d.h"
#include "scene/2d/path_2d.h"
#include "scene/2d/physics_body_2d.h"
#include "scene/2d/position_2d.h"
#include "scene/2d/ray_cast_2d.h"
#include "scene/2d/sprite.h"
#include "scene/2d/tile_map.h"
#include "scene/2d/touch_screen_button.h"
#include "scene/2d/y_sort.h"
#include "scene/2d/text_label.h"
#include "scene/2d/graphic_progress.h"
#include "scene/2d/curtain.h"

#include "scene/2d/world_environment.h"
#include "scene/resources/environment.h"

#include "scene/animation/animation_player.h"
#include "scene/animation/tween.h"

#include "scene/audio/audio_stream_player.h"

#include "scene/gui/control.h"
#include "scene/gui/texture_progress.h" //

#ifndef ADVANCED_GUI_DISABLED
    #include "scene/gui/color_rect.h"
    #include "scene/gui/box_container.h"
    #include "scene/gui/button.h"
    #include "scene/gui/center_container.h" //
    #include "scene/gui/check_box.h"
    #include "scene/gui/check_button.h"
    #include "scene/gui/color_picker.h" //
    #include "scene/gui/dialogs.h"
    #include "scene/gui/file_dialog.h"
    #include "scene/gui/flow_container.h" //
    #include "scene/gui/grid_container.h" //
    #include "scene/gui/item_list.h" //
    #include "scene/gui/line_edit.h"
    #include "scene/gui/link_button.h" //
    #include "scene/gui/margin_container.h"
    #include "scene/gui/menu_button.h"
    #include "scene/gui/option_button.h"
    #include "scene/gui/panel.h" //
    #include "scene/gui/panel_container.h"
    #include "scene/gui/popup_menu.h"
    #include "scene/gui/progress_bar.h" //
    #include "scene/gui/rich_text_effect.h" //
    #include "scene/gui/rich_text_label.h" //
    #include "scene/gui/scroll_bar.h" //
    #include "scene/gui/scroll_container.h" //
    #include "scene/gui/separator.h" //
    #include "scene/gui/slider.h" //
    #include "scene/gui/spin_box.h"
    #include "scene/gui/split_container.h"
    #include "scene/gui/tab_container.h"
    #include "scene/gui/tabs.h" //
    #include "scene/gui/text_edit.h"
    #include "scene/gui/texture_button.h" //
    #include "scene/gui/texture_rect.h"
    #include "scene/gui/tool_button.h"
    #include "scene/gui/tree.h"
    #include "scene/gui/viewport_container.h" //
#endif

#include "scene/main/canvas_layer.h"
#include "scene/main/instance_placeholder.h"
#include "scene/main/resource_preloader.h"
#include "scene/main/scene_tree.h"
#include "scene/main/timer.h"
#include "scene/main/viewport.h"

#include "scene/resources/audio_stream_sample.h"
#include "scene/resources/bit_map.h"
#include "scene/resources/capsule_shape_2d.h"
#include "scene/resources/circle_shape_2d.h"
#include "scene/resources/concave_polygon_shape_2d.h"
#include "scene/resources/convex_polygon_shape_2d.h"
#include "scene/resources/default_theme/default_theme.h"
#include "scene/resources/dynamic_font.h"
#include "scene/resources/gradient.h"
#include "scene/resources/line_shape_2d.h"
#include "scene/resources/material.h"
#include "scene/resources/mesh.h"
#include "scene/resources/navigation_mesh.h"
#include "scene/resources/packed_scene.h"
#include "scene/resources/physics_material.h"
#include "scene/resources/rectangle_shape_2d.h"
#include "scene/resources/resource_format_text.h"
#include "scene/resources/segment_shape_2d.h"
#include "scene/resources/texture.h"
#include "scene/resources/tile_set.h"
#include "scene/resources/world_2d.h"

#include "scene/scene_string_names.h"

#include "modules/modules_enabled.gen.h" // For freetype.

static Ref<ResourceFormatSaverText> resource_saver_text;
static Ref<ResourceFormatLoaderText> resource_loader_text;

#ifdef MODULE_FREETYPE_ENABLED
static Ref<ResourceFormatLoaderDynamicFont> resource_loader_dynamic_font;
#endif // MODULE_FREETYPE_ENABLED

static Ref<ResourceFormatLoaderStreamTexture> resource_loader_stream_texture;

static Ref<ResourceFormatLoaderBMFont> resource_loader_bmfont;

static Ref<ResourceFormatSaverShader> resource_saver_shader;
static Ref<ResourceFormatLoaderShader> resource_loader_shader;

void register_scene_types() {
	SceneStringNames::create();

	OS::get_singleton()->yield(); //may take time to init

	Node::init_node_hrcr();

#ifdef MODULE_FREETYPE_ENABLED
	resource_loader_dynamic_font.instance();
	ResourceLoader::add_resource_format_loader(resource_loader_dynamic_font);
#endif // MODULE_FREETYPE_ENABLED

	resource_loader_stream_texture.instance();
	ResourceLoader::add_resource_format_loader(resource_loader_stream_texture);

	resource_saver_text.instance();
	ResourceSaver::add_resource_format_saver(resource_saver_text, true);

	resource_loader_text.instance();
	ResourceLoader::add_resource_format_loader(resource_loader_text, true);

	resource_saver_shader.instance();
	ResourceSaver::add_resource_format_saver(resource_saver_shader, true);

	resource_loader_shader.instance();
	ResourceLoader::add_resource_format_loader(resource_loader_shader, true);

	resource_loader_bmfont.instance();
	ResourceLoader::add_resource_format_loader(resource_loader_bmfont, true);

	OS::get_singleton()->yield(); //may take time to init

	ClassDB::register_class<Object>();

	ClassDB::register_class<Node>();
	ClassDB::register_virtual_class<InstancePlaceholder>();

	ClassDB::register_class<Viewport>();
	//ClassDB::register_class<ViewportTexture>();
	ClassDB::register_class<Timer>();
	ClassDB::register_class<CanvasLayer>();
	ClassDB::register_class<ResourcePreloader>();

	/* REGISTER GUI */

	OS::get_singleton()->yield(); //may take time to init

	ClassDB::register_class<Control>();
	ClassDB::register_virtual_class<Range>();
	ClassDB::register_class<TextureProgress>();

	OS::get_singleton()->yield(); //may take time to init

	/* REGISTER 3D */
	ClassDB::register_class<AnimationPlayer>();
	ClassDB::register_class<Tween>();

	OS::get_singleton()->yield(); //may take time to init

 	ClassDB::register_class<WorldEnvironment>();

 	OS::get_singleton()->yield(); //may take time to init

    #ifndef ADVANCED_GUI_DISABLED
	    AcceptDialog::set_swap_ok_cancel(GLOBAL_DEF_NOVAL("gui/common/swap_ok_cancel", bool(OS::get_singleton()->get_swap_ok_cancel())));
    #endif

	ClassDB::register_class<Shader>();

	ClassDB::register_class<ShaderMaterial>();
	ClassDB::register_virtual_class<CanvasItem>();
	ClassDB::register_class<CanvasItemMaterial>();
	SceneTree::add_idle_callback(CanvasItemMaterial::flush_changes);
	CanvasItemMaterial::init_shaders();
	ClassDB::register_class<Node2D>();
	ClassDB::register_class<Sprite>();
	ClassDB::register_class<SpriteFrames>();
	ClassDB::register_class<AnimatedSprite>();
	ClassDB::register_class<Position2D>();
	ClassDB::register_class<Line2D>();
	ClassDB::register_virtual_class<CollisionObject2D>();
	ClassDB::register_virtual_class<PhysicsBody2D>();
	ClassDB::register_class<StaticBody2D>();
	ClassDB::register_class<RigidBody2D>();
	ClassDB::register_class<KinematicBody2D>();
	ClassDB::register_class<KinematicCollision2D>();
	ClassDB::register_class<Area2D>();
	ClassDB::register_class<CollisionShape2D>();
	ClassDB::register_class<CollisionPolygon2D>();
	ClassDB::register_class<RayCast2D>();
	ClassDB::register_class<YSort>();
    ClassDB::register_class<TextLabel>();
    ClassDB::register_class<GraphicProgress>();
    ClassDB::register_class<Curtain>();

	OS::get_singleton()->yield(); //may take time to init

	ClassDB::register_class<Camera2D>();
	ClassDB::register_class<TileSet>();
	ClassDB::register_class<TileMap>();
	ClassDB::register_class<TouchScreenButton>();

	OS::get_singleton()->yield(); //may take time to init

	 ClassDB::register_virtual_class<Mesh>();
	// ClassDB::register_class<ArrayMesh>();

	OS::get_singleton()->yield(); //may take time to init

#ifndef _3D_DISABLED
	//ClassDB::register_virtual_class<Material>();
	ClassDB::register_class<SpatialMaterial>();
	//SceneTree::add_idle_callback(SpatialMaterial::flush_changes);
	SpatialMaterial::init_shaders();

	OS::get_singleton()->yield(); //may take time to init

#endif
	ClassDB::register_class<PhysicsMaterial>();
	ClassDB::register_class<Environment>();
	ClassDB::register_class<World2D>();
	ClassDB::register_virtual_class<Texture>();
	ClassDB::register_class<StreamTexture>();
	ClassDB::register_class<ImageTexture>();
	ClassDB::register_class<AtlasTexture>();
	ClassDB::register_class<GradientTexture2D>();
	ClassDB::register_class<Animation>();
	ClassDB::register_virtual_class<Font>();
	//ClassDB::register_class<BitmapFont>();
	ClassDB::register_class<Curve>();

#ifdef MODULE_FREETYPE_ENABLED
	ClassDB::register_class<DynamicFontData>();
	ClassDB::register_class<DynamicFont>();

	DynamicFont::initialize_dynamic_fonts();
#endif // MODULE_FREETYPE_ENABLED

	ClassDB::register_virtual_class<StyleBox>();
	ClassDB::register_class<StyleBoxEmpty>();
	ClassDB::register_class<StyleBoxTexture>();
	ClassDB::register_class<StyleBoxFlat>();
	ClassDB::register_class<StyleBoxLine>();
	ClassDB::register_class<Theme>();

	ClassDB::register_class<BitMap>();
	ClassDB::register_class<Gradient>();

	OS::get_singleton()->yield(); //may take time to init

	ClassDB::register_class<AudioStreamPlayer>();
	ClassDB::register_class<AudioStreamSample>();

	OS::get_singleton()->yield(); //may take time to init

	ClassDB::register_virtual_class<Shape2D>();
	ClassDB::register_class<LineShape2D>();
	ClassDB::register_class<SegmentShape2D>();
	ClassDB::register_class<RayShape2D>();
	ClassDB::register_class<CircleShape2D>();
	ClassDB::register_class<RectangleShape2D>();
	ClassDB::register_class<CapsuleShape2D>();
	ClassDB::register_class<ConvexPolygonShape2D>();
	ClassDB::register_class<ConcavePolygonShape2D>();
	ClassDB::register_class<Curve2D>();
	ClassDB::register_class<Path2D>();
	ClassDB::register_class<PathFollow2D>();

	ClassDB::register_class<Navigation2D>();
	ClassDB::register_class<NavigationPolygon>();
	ClassDB::register_class<NavigationPolygonInstance>();
	ClassDB::register_class<NavigationAgent2D>();
	ClassDB::register_class<NavigationObstacle2D>();
	OS::get_singleton()->yield(); //may take time to init

	ClassDB::register_virtual_class<SceneState>();
	ClassDB::register_class<PackedScene>();

	ClassDB::register_class<SceneTree>();
	ClassDB::register_virtual_class<SceneTreeTimer>(); //sorry, you can't create it

	OS::get_singleton()->yield(); //may take time to init

	for (int i = 0; i < 20; i++) {
		GLOBAL_DEF(vformat("%s/layer_%d", PNAME("layer_names/2d_render"), i + 1), "");
	}

	for (int i = 0; i < 32; i++) {
		GLOBAL_DEF(vformat("%s/layer_%d", PNAME("layer_names/2d_physics"), i + 1), "");
	}

	for (int i = 0; i < 32; i++) {
		GLOBAL_DEF(vformat("%s/layer_%d", PNAME("layer_names/2d_navigation"), i + 1), "");
	};
}

void initialize_theme() {
	bool default_theme_hidpi = GLOBAL_DEF("gui/theme/use_hidpi", false);
	ProjectSettings::get_singleton()->set_custom_property_info("gui/theme/use_hidpi", PropertyInfo(Variant::BOOL, "gui/theme/use_hidpi", PROPERTY_HINT_NONE, "", PROPERTY_USAGE_DEFAULT | PROPERTY_USAGE_RESTART_IF_CHANGED));
	String theme_path = GLOBAL_DEF_RST("gui/theme/custom", "");
	ProjectSettings::get_singleton()->set_custom_property_info("gui/theme/custom", PropertyInfo(Variant::STRING, "gui/theme/custom", PROPERTY_HINT_FILE, "*.tres,*.res,*.theme", PROPERTY_USAGE_DEFAULT | PROPERTY_USAGE_RESTART_IF_CHANGED));
	String font_path = GLOBAL_DEF_RST("gui/theme/custom_font", "");
	ProjectSettings::get_singleton()->set_custom_property_info("gui/theme/custom_font", PropertyInfo(Variant::STRING, "gui/theme/custom_font", PROPERTY_HINT_FILE, "*.tres,*.res,*.font", PROPERTY_USAGE_DEFAULT | PROPERTY_USAGE_RESTART_IF_CHANGED));

	Ref<Font> font;
	if (font_path != String()) {
		font = ResourceLoader::load(font_path);
		if (!font.is_valid()) {
			ERR_PRINT("Error loading custom font '" + font_path + "'");
		}
	}

	// Always make the default theme to avoid invalid default font/icon/style in the given theme
	make_default_theme(default_theme_hidpi, font);

	if (theme_path != String()) {
		Ref<Theme> theme = ResourceLoader::load(theme_path);
		if (theme.is_valid()) {
			Theme::set_project_default(theme);
			if (font.is_valid()) {
				Theme::set_default_font(font);
			}
		} else {
			ERR_PRINT("Error loading custom theme '" + theme_path + "'");
		}
	}
}

void unregister_scene_types() {
	clear_default_theme();

#ifdef MODULE_FREETYPE_ENABLED
	ResourceLoader::remove_resource_format_loader(resource_loader_dynamic_font);
	resource_loader_dynamic_font.unref();

	DynamicFont::finish_dynamic_fonts();
#endif // MODULE_FREETYPE_ENABLED

	ResourceLoader::remove_resource_format_loader(resource_loader_stream_texture);
	resource_loader_stream_texture.unref();

	ResourceSaver::remove_resource_format_saver(resource_saver_text);
	resource_saver_text.unref();

	ResourceLoader::remove_resource_format_loader(resource_loader_text);
	resource_loader_text.unref();

	ResourceSaver::remove_resource_format_saver(resource_saver_shader);
	resource_saver_shader.unref();

	ResourceLoader::remove_resource_format_loader(resource_loader_shader);
	resource_loader_shader.unref();

	ResourceLoader::remove_resource_format_loader(resource_loader_bmfont);
	resource_loader_bmfont.unref();

	CanvasItemMaterial::finish_shaders();
	SceneStringNames::free();
}
