#pragma once

#include "ui.h"

#include "glw.h"
#include "ImGuiClass.h"

#define IMGUI_INCLUDE_NODES_GUI
#include "ImGuiObjectEditor.h"

struct NVGcontext;

namespace nd {
	struct GUI : UI {
		
		static obj::ObjectType TypeData;
		static struct Vtable : UI::Vtable {} vtable;
		
		static void constructor(GUI* self);
		static void destructor(GUI* self);
		static void copy(GUI* self, const GUI* in);
		static tp::alni save_size(GUI* self) { return 0; };
		static void save(GUI* self, tp::File& file_self) {};
		static void load(tp::File& file_self, GUI* self) {};

		tp::ogl::opengl gl;
		tp::ogl::window window;
		bool need_update;
		bool clear_frames;
		bool whait_for_event;
		bool debug_ui;

		tp::fpscount fps;
		tp::fpscount window_fps;
		 
		ImGui::DefaultWraper imgui;
		ImGui::ImGuiObjectEditor debug_oedit;

		NVGcontext* vg;

		static void procInputs(GUI* self);
		static void presentOutput(GUI* self);
		void draw();

		void debug_init(Object* core);
		void draw_debug_info();
	};
};