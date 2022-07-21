
#include "Nodes.h"

#include "gui.h"
#include "tui.h"

#include "timer.h"

using namespace nd;

namespace nd {
	bool terminate_flag;
	int terminate_code;
};

struct obj::ObjectType NodesCore::TypeData = {
	.base = &obj::ClassObject::TypeData,
	.constructor = (obj::object_constructor) NodesCore::constructor,
	.destructor = (obj::object_destructor) NodesCore::destructor,
	.copy = NULL,
	.size = sizeof(NodesCore),
	.name = "NodesCore",
	.save_size = (obj::object_save_size) NodesCore::save_size,
	.save = (obj::object_save) NodesCore::save,
	.load = (obj::object_load) NodesCore::load,
};

void NodesCore::constructor(NodesCore* self) {

	NDO_CASTV(obj::IntObject, obj::NDO->create("int"), int1)->val = 160;
	NDO_CASTV(obj::ListObject, obj::NDO->create("list"), uis);

	nd::GUI* gui = (nd::GUI*)obj::NDO->create("Gui");
	gui->debug_init(self);

	nd::TUI* tui = (nd::TUI*) obj::NDO->create("Tui");
	tui->initializeInputs(gui);

	uis->items.pushBack(gui);
	uis->items.pushBack(tui);

	self->addMember(uis, "uis");
	self->addMember(int1, "fps");
}

void NodesCore::destructor(NodesCore* in) {}

void nd::NodesCore::save(NodesCore* self, tp::File& file_self) {}

void nd::NodesCore::load(tp::File& file_self, NodesCore* self) {
	obj::ListObject* uis = self->getMember<obj::ListObject>("uis");

	for (auto ui : uis->items) {
		nd::GUI* gui = NDO_CAST(nd::GUI, ui.data());
		if (gui) {
			gui->debug_init(self);
		}
	}
}

void NodesCore::run() {

	obj::ListObject* uis = getMember<obj::ListObject>("uis");

	/*
	ObList& Threads = GETOBJ(ObList, this, Threads);
	ObList& Requests = GETOBJ(ObList, this, Requests);
	*/

	while (!nd::terminate_flag) {

		tp::Timer timer(tp::time_ms(1000.f / getMember<obj::IntObject>("fps")->val));

		// Pump Requests From UIs
		for (auto ui : uis->items) {
			NDO_CASTV(UI, ui.data(), ui_component);
			if (ui_component) ui_component->procInputs();
		}


		// Output to the user
		for (auto ui : uis->items) {
			NDO_CASTV(UI, ui.data(), ui_component);
			if (ui_component) ui_component->presentOutput();
		}

		timer.wait();

	}
}
