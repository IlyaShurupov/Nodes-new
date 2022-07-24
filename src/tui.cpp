
#include "tui.h"

using namespace nd;

struct obj::ObjectType KeyInput::TypeData = {
	.base = &obj::ClassObject::TypeData,
	.constructor = (obj::object_constructor) KeyInput::constructor,
	.destructor = (obj::object_destructor) KeyInput::destructor,
	.copy = (obj::object_copy) KeyInput::copy,
	.size = sizeof(KeyInput),
	.name = "KeyInput",
};

void KeyInput::constructor(nd::KeyInput* self) {
	self->createMember("int", "key code");

	self->createMember("enum", "state");

	self->createMember("bool", "printable");
	self->createMember("str", "char value");
	self->createMember("str", "shifted char value");

	obj::EnumObject* state = self->getMember<obj::EnumObject>("state");
	state->init({"PRESS", "RELEASE", "HOLD", "REPEAT", "NONE"});
	state->active = 4;
}

void KeyInput::destructor(nd::KeyInput* self) {}
void KeyInput::copy(nd::KeyInput* self, nd::KeyInput* blueprint) {}

KeyInput* KeyInput::create(tp::ogl::Keycode code, bool printable, char val, char shifted_val) {
	KeyInput* self = (KeyInput*) obj::NDO->create("KeyInput");
	self->getMember<obj::IntObject>("key code")->val = tp::alni(code);
	self->getMember<obj::BoolObject>("printable")->val = tp::alni(printable);
	self->getMember<obj::StringObject>("char value")->val = val;
	self->getMember<obj::StringObject>("shifted char value")->val = shifted_val;
	return self;
}

void KeyInput::updateIntermediateSates() {
	tp::uhalni& val = getMember<obj::EnumObject>("state")->active;
	tp::ogl::Keystate state = tp::ogl::Keystate(val);

	switch (state) {
		case tp::ogl::Keystate::RELEASED:
			state = tp::ogl::Keystate::NONE;
			break;

		case tp::ogl::Keystate::PRESSED:
			state = tp::ogl::Keystate::HOLD;
	}

	val = tp::uhalni(state);
}

void KeyInput::updateFromEvent(tp::ogl::KeyEvent ev) {
	tp::uhalni& val = getMember<obj::EnumObject>("state")->active;
	tp::ogl::Keystate state = tp::ogl::Keystate(val);

	switch (ev.event_state) {

		case tp::ogl::KeyEvent::EventState::PRESSED:
		{
			switch (state) {
				case tp::ogl::Keystate::RELEASED:
				case tp::ogl::Keystate::NONE:
					state = tp::ogl::Keystate::PRESSED;
					break;

				case tp::ogl::Keystate::HOLD:
				case tp::ogl::Keystate::REPEAT:
					//assert(0 && "invalid state");
					return;
			}
			break;
		}

		case tp::ogl::KeyEvent::EventState::RELEASED:
		{
			switch (state) {
				case tp::ogl::Keystate::PRESSED:
				case tp::ogl::Keystate::HOLD:
				case tp::ogl::Keystate::REPEAT:
					state = tp::ogl::Keystate::RELEASED;
					break;

				case tp::ogl::Keystate::RELEASED:
				case tp::ogl::Keystate::NONE:
					//assert(0 && "invalid state");
					return;
			}
			break;
		}

		case tp::ogl::KeyEvent::EventState::REPEAT:
		{
			break;
		}
	}

	val = tp::uhalni(state);
}

struct obj::ObjectType Comparator::TypeData = {
	.base = &obj::ClassObject::TypeData,
	.constructor = (obj::object_constructor) Comparator::constructor,
	.destructor = (obj::object_destructor) Comparator::destructor,
	.copy = (obj::object_copy) Comparator::copy,
	.size = sizeof(Comparator),
	.name = "Comparator",
};

void Comparator::constructor(nd::Comparator* self) {
	self->createMember("link", "first");
	self->createMember("link", "second");
	self->createMember("bool", "or/and");
}

void Comparator::destructor(nd::Comparator* self) {}
void Comparator::copy(nd::Comparator* self, nd::Comparator* blueprint) {}

bool Comparator::isMet() {
	bool expr = obj::NDO->compare(getMember<obj::LinkObject>("first")->link, getMember<obj::LinkObject>("second")->link);
	return expr;
}

struct obj::ObjectType ShortcutTrigger::TypeData = {
	.base = &obj::ClassObject::TypeData,
	.constructor = (obj::object_constructor) ShortcutTrigger::constructor,
	.destructor = (obj::object_destructor) ShortcutTrigger::destructor,
	.copy = (obj::object_copy) ShortcutTrigger::copy,
	.size = sizeof(ShortcutTrigger),
	.name = "Trigger",
};

void ShortcutTrigger::constructor(nd::ShortcutTrigger* self) {
	self->createMember("dict", "comparators");
	self->createMember("bool", "Enable");
}

void ShortcutTrigger::destructor(nd::ShortcutTrigger* self) {}
void ShortcutTrigger::copy(nd::ShortcutTrigger* self, nd::ShortcutTrigger* blueprint) {}

bool ShortcutTrigger::isMet() {
	obj::DictObject* comparators = getMember<obj::DictObject>("comparators");

	bool res = false;
	for (auto& comp_iter : comparators->items) {
		Comparator* comp = NDO_CAST(Comparator, comp_iter.iter->val);
		if (comp) {
			if (comp_iter.entry_idx == 0) {
				res = comp->isMet();
			} else {
				bool and_conj = bool(comp->getMember<obj::BoolObject>("or/and")->val);
				res = and_conj ? res && comp->isMet() : res || comp->isMet();
			}
		}
	}

	return res;
}

tp::Array<obj::Object*> childs_retrival(nd::Shortcut* self) {
	tp::Array<obj::Object*> out;
	out.pushBack(self->callbacks_triggers);
	return out;
}

tp::alni allocated_size(Shortcut* self) {
	return sizeof(obj::DictObject*);
}

tp::alni allocated_size_recursive(Shortcut* self) {
	tp::alni out = sizeof(obj::DictObject*);
	out += obj::NDO->objsize_ram_recursive_util(self->callbacks_triggers, self->callbacks_triggers->type);
	return out;
}

struct obj::ObjectType Shortcut::TypeData = {
	.base = &nd::Requester::TypeData,
	.constructor = (obj::object_constructor) Shortcut::constructor,
	.destructor = (obj::object_destructor) Shortcut::destructor,
	.copy = (obj::object_copy) Shortcut::copy,
	.size = sizeof(Shortcut),
	.name = "Shortcut",
	.save_size = (obj::object_save_size) Shortcut::save_size,
	.save = (obj::object_save) Shortcut::save,
	.load = (obj::object_load) Shortcut::load,
	.childs_retrival = (obj::object_debug_all_childs_retrival) childs_retrival,
	.allocated_size = (obj::object_allocated_size) allocated_size,
	.allocated_size_recursive = (obj::object_allocated_size_recursive) allocated_size_recursive
};

void Shortcut::constructor(nd::Shortcut* self) {
	self->callbacks_triggers = (obj::DictObject*) obj::NDO->create("dict");
}

void Shortcut::destructor(nd::Shortcut* self) {
	obj::NDO->destroy(self->callbacks_triggers);
}
void Shortcut::copy(nd::Shortcut* self, nd::Shortcut* blueprint) {
	obj::NDO->copy(self->callbacks_triggers, blueprint->callbacks_triggers);
}

tp::alni Shortcut::save_size(Shortcut* self) {
	return sizeof(tp::alni); // dict object adress
}

void Shortcut::save(Shortcut* self, tp::File& file_self) {
	// save dictobject
	tp::alni ndo_object_adress = obj::NDO->save(file_self, self->callbacks_triggers);
	file_self.write<tp::alni>(&ndo_object_adress);
}

void Shortcut::load(tp::File& file_self, Shortcut* self) {
	tp::alni ndo_object_adress;
	file_self.read<tp::alni>(&ndo_object_adress);
	self->callbacks_triggers = NDO_CAST(obj::DictObject, obj::NDO->load(file_self, ndo_object_adress));
}

void Shortcut::proc() {
	for (auto modal : callbacks_triggers->items) {
		ShortcutTrigger* expr = NDO_CAST(ShortcutTrigger, modal->val);
		if (expr && expr->isMet()) {
			if (callbacks_arguments->items.presents(modal->key)) {
				call(modal->key);
			}
		}
	}
}

struct obj::ObjectType TUI::TypeData = {
	.base = &UI::TypeData,
	.constructor = (obj::object_constructor) TUI::constructor,
	.destructor = (obj::object_destructor) TUI::destructor,
	.copy = (obj::object_copy) TUI::copy,
	.size = sizeof(TUI),
	.name = "Tui",
	.vtable = &TUI::vtable
};

struct TUI::Vtable TUI::vtable = {
	(void (*)(UI*)) & TUI::procInputs,
	(void (*)(UI*)) & TUI::presentOutput,
};

void TUI::constructor(nd::TUI* self) {
	self->createMember("link", "gui");
	self->createMember("list", "shortcuts");
	self->createMember("dict", "inputs");
	self->createMember("link", "shift modifier Key");

	self->createMember("bool", "capture input");
	self->createMember("str", "input string");
}

void TUI::destructor(nd::TUI* self) {}
void TUI::copy(nd::TUI* self, nd::TUI* blueprint) {}

void TUI::procInputs(nd::TUI* self) {
	self->updateInputs();
	self->procEvents();
}

void TUI::procEvents() {
	obj::LinkObject* gui_link = getMember<obj::LinkObject>("gui");
	if (!gui_link) {
		return;
	}

	nd::GUI* gui = NDO_CAST(nd::GUI, gui_link->link);
	if (!gui) {
		return;
	}

	obj::DictObject* inputs = getMember<obj::DictObject>("inputs");

	tp::List<tp::ogl::KeyEvent>& events = gui->window.event_queue.keys;

	for (auto ev : events) {
		int key_code = (int) ev.data().code;
		auto idx = inputs->items.presents(key_code);
		if (idx) {
			KeyInput* key = NDO_CAST(KeyInput, inputs->items.getSlotVal(idx));
			key->updateFromEvent(ev.data());
			updateInputString(key);
			procShortcuts();
		}
	}
}

void TUI::updateInputs() {
	obj::DictObject* inputs = getMember<obj::DictObject>("inputs");
	for (auto& input : inputs->items) {
		NDO_CAST(KeyInput, input.iter->val)->updateIntermediateSates();
	}
}

void TUI::procShortcuts() {
	obj::ListObject* inputs = getMember<obj::ListObject>("shortcuts");
	for (auto shcut_iter : inputs->items) {
		Shortcut* shcut = NDO_CAST(Shortcut, shcut_iter.data());
		if (shcut) {
			shcut->proc();
		}
	}
}

void TUI::updateInputString(KeyInput* key) {
	if (!getMember<obj::BoolObject>("capture input")->val) {
		return;
	}

	if (!key->getMember<obj::BoolObject>("printable")->val) {
		return;
	}

	if (tp::ogl::Keystate(key->getMember<obj::EnumObject>("state")->active) != tp::ogl::Keystate::PRESSED) {
		return;
	}

	tp::string val;
	KeyInput* shift_key = NDO_CAST(KeyInput, getMember<obj::LinkObject>("shift modifier Key")->link);
	if (tp::ogl::Keystate(shift_key->getMember<obj::EnumObject>("state")->active) != tp::ogl::Keystate::HOLD) {
		val = key->getMember<obj::StringObject>("char value")->val;
	} else {
		val = key->getMember<obj::StringObject>("shifted char value")->val;
	}

	tp::string& str = getMember<obj::StringObject>("input string")->val;
	str += val;
}

void TUI::initializeInputs(nd::GUI* gui) {
	getMember<obj::LinkObject>("gui")->link = gui;
	obj::DictObject* inputs = getMember<obj::DictObject>("inputs");

	/* a - z */
	for (auto i : tp::Range(65, 91)) {
		inputs->items.put(i.index(), KeyInput::create(tp::ogl::Keycode(i.index()), true, char(i.index() + 32), char(i.index())));
	}

	KeyInput* shift_key = KeyInput::create(tp::ogl::Keycode::LEFT_SHIFT);
	getMember<obj::LinkObject>("shift modifier Key")->link = shift_key;
	inputs->items.put(int(tp::ogl::Keycode::LEFT_SHIFT), shift_key);
}

void TUI::presentOutput(nd::TUI* self) {}