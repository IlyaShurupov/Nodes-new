#pragma once

#include "primitives/classobject.h"

namespace nd {
	struct UI : obj::ClassObject {
		static obj::ObjectType TypeData;

		static struct Vtable {
			void (*procInputs)(UI*) = NULL;
			void (*presentOutput)(UI*) = NULL;
		} vtable;

		void procInputs() { ((Vtable*) (type->vtable))->procInputs(this); };
		void presentOutput() { ((Vtable*) (type->vtable))->presentOutput(this); };
	};
};