
#pragma once

#include "primitives/primitives.h"
#include "requester.h"

#include "gui.h"

namespace nd {

	struct KeyInput : public obj::ClassObject {
		static struct obj::ObjectType TypeData;
		static KeyInput* create(tp::ogl::Keycode code, bool printable = false, char val = ' ', char shifted_val = ' ');
		static void constructor(nd::KeyInput* self);
		static void destructor(nd::KeyInput* self);
		static void copy(nd::KeyInput* self, nd::KeyInput* blueprint);
		void updateIntermediateSates();
		void updateFromEvent(tp::ogl::KeyEvent ev);
	};

	struct Comparator : public obj::ClassObject {
		static struct obj::ObjectType TypeData;
		static void constructor(nd::Comparator* self);
		static void destructor(nd::Comparator* self);
		static void copy(nd::Comparator* self, nd::Comparator* blueprint);
		bool isMet();
	};

	struct ShortcutTrigger : public obj::ClassObject {
		static struct obj::ObjectType TypeData;
		static void constructor(nd::ShortcutTrigger* self);
		static void destructor(nd::ShortcutTrigger* self);
		static void copy(nd::ShortcutTrigger* self, nd::ShortcutTrigger* blueprint);
		bool isMet();
	};

	struct Shortcut : public nd::Requester {
		static struct obj::ObjectType TypeData;
		
		obj::DictObject* callbacks_triggers;

		static void constructor(nd::Shortcut* self);
		static void destructor(nd::Shortcut* self);
		static void copy(nd::Shortcut* self, nd::Shortcut* blueprint);

		static tp::alni save_size(Shortcut* self);
		static void save(Shortcut* self, tp::File& file_self);
		static void load(tp::File& file_self, Shortcut* self);

		void proc();
	};

	struct TUI : public UI {
		static struct obj::ObjectType TypeData;
		static struct Vtable : UI::Vtable {} vtable;
		static void constructor(nd::TUI* self);
		static void destructor(nd::TUI* self);
		static void copy(nd::TUI* self, nd::TUI* blueprint);
		static void procInputs(nd::TUI* self);
		void procEvents();
		void updateInputs();
		void procShortcuts();
		void updateInputString(KeyInput* key);
		void initializeInputs(nd::GUI* gui);
		static void presentOutput(nd::TUI* self);
	};
};
