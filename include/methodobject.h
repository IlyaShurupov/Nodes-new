
#pragma once

#include "object.h"

struct MethodObjectArgument {
	constring name;
	Object* arg;
};

typedef Object* (*ndo_static_method)(struct ClassObject* self, MethodObjectArgument* args);

struct MethodObject : Object {

	struct ClassObject* self;
	void* code;
	alni code_flags;

	static void constructor(Object* in);
	static void copy(Object* self, const Object* in);

	Object* operator()(MethodObjectArgument* args);
};

extern ObjectType MethodObjectType;