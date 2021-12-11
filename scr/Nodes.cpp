
#include "Nodes.h"

#include "intobject.h"
#include "dictobject.h"
#include "classobject.h"
#include "methodobject.h"
#include "listobject.h"

struct ObjectStaticMethod NodesCoreMethods[] = {
	{"run", NodesCoreClass::run },
	{"console_foo", NULL },
	{NULL, NULL}
};

struct ObjectType NodesCoreClassType = {
	.base = &ClassObjectType,
	.constructor = NodesCoreClass::constructor,
	.destructor = NodesCoreClass::destructor,
	.copy = NULL,
	.size = sizeof(NodesCoreClass),
	.name = "NodesCore",
};

void NodesCoreClass::destructor(Object* in) {

}

void NodesCoreClass::constructor(Object* in) {
	NDO_CASTV(NodesCoreClass, in, self);

	self->add_methods(NodesCoreMethods);

	NDO_CASTV(IntObject, NDO.create("int"), int1)->val = 60;
	self->members->items.Put("fps", int1);

	NDO_CASTV(ListObject, NDO.create("list"), uis);
	uis->items.PushBack(NDO.create("GUI"));
	uis->items.PushBack(NDO.create("TUI"));

	self->members->items.Put("uis", uis);
}

#include <iostream>
#include <string.h>

Object* NodesCoreClass::run(Object* in, Object* args) {

	NDO_CASTV(NodesCoreClass, in, self);

	NDO_CASTV(ListObject, self->member("uis"), uis);

	/*
	ObList& Threads = GETOBJ(ObList, this, Threads);
	ObList& Requests = GETOBJ(ObList, this, Requests);
	*/

	NDCORELOOP: {

		Timer timer = Timer((uint8)(1000.f / NDO_CAST(IntObject, self->member("fps"))->val));

		// Pump Requests From UIs
		for (auto ui : uis->items) {
			NDO_CAST(ClassObject, ui.Data())->call("proc_inputs", NULL);
		}

		std::string cmd;
		std::string line;
		
		do {
			cmd += line + "\n";
			std::getline(std::cin, line);
		} while (line != "end");

		string cmd_out;
		cmd_out = cmd.c_str();

		NDO_CASTV(MethodObject, self->member("console_foo"), foo_method);
		foo_method->code_flags = 1;
		foo_method->code.pycode = cmd_out.str;
		self->call("console_foo", NULL);

		const string* out = NdLog_read();
		if (out) {
			std::cout << out->str;
		}

		// Output to the user
		for (auto ui : uis->items) {
			NDO_CAST(ClassObject, ui.Data())->call("present_output", NULL);
		}

		timer.wait_out();

	} goto NDCORELOOP;

	return NDO_NULL;
}
