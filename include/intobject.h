
#pragma once

#include "object.h"

struct IntObject : Object {
	alni val;

	static void constructor(Object* self);
	static void copy(Object* self, const Object* in);
	static IntObject* create(alni in);
};
 

extern ObjectType IntObjectType;