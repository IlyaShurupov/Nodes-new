
#include "gui.h"

#define NANOVG_GL3_IMPLEMENTATION	// Use GL3 implementation.
#include "nanovg.h"
#include "nanovg_gl.h"
#include "nanovg_gl_utils.h"

#include "widget.h"

using namespace nd;

struct obj::ObjectType GUI::TypeData = {
	.base = &UI::TypeData,
	.constructor = (obj::object_constructor) GUI::constructor,
	.destructor = (obj::object_destructor) GUI::destructor,
	.copy = (obj::object_copy) GUI::copy,
	.size = sizeof(GUI),
	.name = "Gui",
	.save_size = (obj::object_save_size) GUI::save_size,
	.save = (obj::object_save) GUI::save,
	.load = (obj::object_load) GUI::load,
	.vtable = &GUI::vtable
};

struct GUI::Vtable GUI::vtable = {
	(void (*)(UI*)) & GUI::procInputs,
	(void (*)(UI*)) & GUI::presentOutput,
};


void GUI::constructor(GUI* self) {

	RootWidget* root = NDO_CAST(RootWidget, obj::NDO->create("RootWidget"));
	obj::LinkObject* active = NDO_CAST(obj::LinkObject, obj::NDO->create("link"));
	active->link = root;
	self->addMember(root, "root");
	self->addMember(active, "active widget");

	self->init();
};

void GUI::destructor(GUI* self) {
	self->gl.~opengl();
	self->window.~window();
	self->imgui.~DefaultWraper();
	self->debug_oedit.~ImGuiObjectEditor();

	nvgDeleteGL3(self->vg);
};

void GUI::copy(GUI* self, const GUI* in) {
	self->gl = in->gl;
	self->window = in->window;
	self->imgui = in->imgui;
	self->vg = in->vg;
}

void nd::GUI::save(GUI* self, tp::File& file_self) {}

void nd::GUI::load(tp::File& file_self, GUI* self) {
	self->init();
}

void nd::GUI::init() {
	new (&gl) tp::ogl::opengl();
	new (&window) tp::ogl::window({1, 1}, tp::ogl::window::UNDECORATED | tp::ogl::window::HIDDEN);
	new (&imgui) ImGui::DefaultWraper();
	new (&fps) tp::fpscount();
	new (&window_fps) tp::fpscount();
	new (&debug_oedit) ImGui::ImGuiObjectEditor();

	window.col_clear = tp::rgba(0, 0, 0, 0);

	vg = nvgCreateGL3(NVG_ANTIALIAS | NVG_STENCIL_STROKES);
	imgui.init(&window);

	need_update = false;
	clear_frames = true;
	whait_for_event = false;
	debug_ui = true;

	update_winrec_from_root();
	window.showWindow();
}

void GUI::procInputs(GUI* self) {
	self->window.update_event_queue(self->whait_for_event && (!self->need_update));

	if (self->window.SpecialKey()) {
		self->debug_ui = !self->debug_ui;
	}

	self->update_winrec_from_root();
};

void GUI::presentOutput(GUI* self) {

	self->window.begin_draw(self->need_update);

	if (self->window.new_frame) {
		if (self->clear_frames) {
			self->window.clear();
		}
		self->window.reset_viewport();

		nvgBeginFrame(self->vg, self->window.size.x, self->window.size.y, 1);
	}

	if (self->window.draw_event) {

		self->draw();

		nvgEndFrame(self->vg);
		
		if (self->debug_ui) {
			self->imgui.frame_start();
			self->draw_debug_info();
			self->imgui.frame_end();
		}
	}

	self->window.end_draw();

	if (self->window.draw_event) {
		self->window_fps.update(false);
	}

	self->fps.update(false);
}

void nd::GUI::draw() {
	nd::Widget* root = getMember<nd::Widget>("root");

	auto rec = root->getRect(); 
	auto pos = rec.pos; 
	rec.pos = 0;
	root->setRect(rec.x, rec.y, rec.z, rec.w);

	root->presentOutput(this, tp::rect<tp::alni>(0, 0, (tp::alni) window.size.x, (tp::alni) window.size.y));
	
	rec.pos = pos;
	root->setRect(rec.x, rec.y, rec.z, rec.w);
}

void GUI::draw_debug_info() {

	if (ImGui::WindowEditor("Profiler")) {
		ImGui::Text("PROC FPS : %i", fps.fps);
		ImGui::Text("DRAW FPS : %i", window_fps.fps);
	}
	ImGui::End();

	debug_oedit.Draw();
}

void nd::GUI::update_winrec_from_root() {
	nd::Widget* root_widget = getMember<nd::Widget>("root");
	tp::rect<tp::alni> rec = root_widget->getRect();
	window.setRectWorld(tp::rect<tp::alnf>((tp::alnf) rec.x, (tp::alnf) rec.y, (tp::alnf) rec.z, (tp::alnf) rec.w));
	root_widget->setRect(rec.x, rec.y, (tp::alni) window.size.x, (tp::alni) window.size.y);
}

void GUI::debug_init(Object* core) {
	debug_oedit.root->items.put("NodesCore", core);
	debug_oedit.cd(core, "NodesCore");
}