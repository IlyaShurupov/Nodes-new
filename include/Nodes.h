#pragma once

#include "UI.h"

struct NodesCoreClass : ClassObject {

	static void destructor(Object* in);
	static void constructor(Object* in);
	static Object* run(Object* self, Object* args);
};

extern ObjectType NodesCoreClassType;
