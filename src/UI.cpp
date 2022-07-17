
#include "ui.h"

using namespace nd;

struct obj::ObjectType UI::TypeData = {
	.base = &obj::ClassObject::TypeData,
	.constructor = NULL,
	.destructor = NULL,
	.copy = NULL,
	.size = sizeof(UI),
	.name = "Ui",
	.vtable = &UI::vtable
};

struct UI::Vtable UI::vtable = { NULL, NULL };