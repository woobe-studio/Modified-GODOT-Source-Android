/**************************************************************************/
/*  mesh_editor_plugin.cpp                                                */
/**************************************************************************/


#include "mesh_editor_plugin.h"

#include "editor/editor_scale.h"

void MeshEditor::_gui_input(Ref<InputEvent> p_event) {
	Ref<InputEventMouseMotion> mm = p_event;
	if (mm.is_valid() && mm->get_button_mask() & BUTTON_MASK_LEFT) {
		rot_x -= mm->get_relative().y * 0.01;
		rot_y -= mm->get_relative().x * 0.01;
		if (rot_x < -Math_PI / 2) {
			rot_x = -Math_PI / 2;
		} else if (rot_x > Math_PI / 2) {
			rot_x = Math_PI / 2;
		}
		_update_rotation();
	}
}

void MeshEditor::_notification(int p_what) {
	if (p_what == NOTIFICATION_READY) {
		//get_scene()->connect("node_removed",this,"_node_removed");

		if (first_enter) {
			//it's in propertyeditor so. could be moved around

			light_1_switch->set_normal_texture(get_icon("MaterialPreviewLight1", "EditorIcons"));
			light_1_switch->set_pressed_texture(get_icon("MaterialPreviewLight1Off", "EditorIcons"));
			light_2_switch->set_normal_texture(get_icon("MaterialPreviewLight2", "EditorIcons"));
			light_2_switch->set_pressed_texture(get_icon("MaterialPreviewLight2Off", "EditorIcons"));
			first_enter = false;
		}
	}
}

void MeshEditor::_update_rotation() {
}

void MeshEditor::edit(Ref<Mesh> p_mesh) {
	mesh = p_mesh;

	rot_x = Math::deg2rad(-15.0);
	rot_y = Math::deg2rad(30.0);
	_update_rotation();

	AABB aabb = mesh->get_aabb();
	Vector3 ofs = aabb.position + aabb.size * 0.5;
	float m = aabb.get_longest_axis_size();
	if (m != 0) {
		m = 1.0 / m;
		m *= 0.5;
		Transform xform;
		xform.basis.scale(Vector3(m, m, m));
		xform.origin = -xform.basis.xform(ofs); //-ofs*m;
	}
}

void MeshEditor::_button_pressed(Node *p_button) {
}

void MeshEditor::_bind_methods() {
	ClassDB::bind_method(D_METHOD("_gui_input"), &MeshEditor::_gui_input);
	ClassDB::bind_method(D_METHOD("_button_pressed"), &MeshEditor::_button_pressed);
}

MeshEditor::MeshEditor() {
	viewport = memnew(Viewport);
	Ref<World> world;
	world.instance();
	viewport->set_world(world); //use own world
	add_child(viewport);
	viewport->set_disable_input(true);
	viewport->set_msaa(Viewport::MSAA_2X);
	set_stretch(true);

	set_custom_minimum_size(Size2(1, 150) * EDSCALE);

	HBoxContainer *hb = memnew(HBoxContainer);
	add_child(hb);
	hb->set_anchors_and_margins_preset(Control::PRESET_WIDE, Control::PRESET_MODE_MINSIZE, 2);

	hb->add_spacer();

	VBoxContainer *vb_light = memnew(VBoxContainer);
	hb->add_child(vb_light);

	light_1_switch = memnew(TextureButton);
	light_1_switch->set_toggle_mode(true);
	vb_light->add_child(light_1_switch);
	light_1_switch->connect("pressed", this, "_button_pressed", varray(light_1_switch));

	light_2_switch = memnew(TextureButton);
	light_2_switch->set_toggle_mode(true);
	vb_light->add_child(light_2_switch);
	light_2_switch->connect("pressed", this, "_button_pressed", varray(light_2_switch));

	first_enter = true;

	rot_x = 0;
	rot_y = 0;
}

///////////////////////

bool EditorInspectorPluginMesh::can_handle(Object *p_object) {
	return Object::cast_to<Mesh>(p_object) != nullptr;
}

void EditorInspectorPluginMesh::parse_begin(Object *p_object) {
	Mesh *mesh = Object::cast_to<Mesh>(p_object);
	if (!mesh) {
		return;
	}
	Ref<Mesh> m(mesh);

	MeshEditor *editor = memnew(MeshEditor);
	editor->edit(m);
	add_custom_control(editor);
}

MeshEditorPlugin::MeshEditorPlugin(EditorNode *p_node) {
	Ref<EditorInspectorPluginMesh> plugin;
	plugin.instance();
	add_inspector_plugin(plugin);
}
