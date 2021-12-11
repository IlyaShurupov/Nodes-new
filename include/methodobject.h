
#pragma once

#include "object.h"

union MethoObjectCode {
	string pycode;
	ndo_static_method ccode = NULL;

	~MethoObjectCode() {
	}
};

struct MethodObject : Object {

	struct ClassObject* self;
	MethoObjectCode code;
	alni code_flags;

	static void constructor(Object* in);
	static void copy(Object* self, const Object* in);

	Object* operator()(Object* args);

	static void from_string(Object* self, string in);
};

extern ObjectType MethodObjectType;