
#pragma once

#include "object.h"

struct MethodObjectArgument {
	string name;
	Object* arg;
};

typedef Object* (*ndo_static_method)(struct ClassObject* self, MethodObjectArgument* args);

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

	Object* operator()(MethodObjectArgument* args);

	static void from_string(Object* self, string in);
};

extern ObjectType MethodObjectType;