#pragma once

#include "UI.h"

struct NodesCoreClass : ClassObject {

	static void destructor(Object* in);
	static void constructor(Object* in);
	static Object* run(ClassObject* self, MethodObjectArgument* args);
};

extern ObjectType NodesCoreClassType;
