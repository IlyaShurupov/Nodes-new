#pragma once

#include "object.h"

struct DictObject : Object {
	hmap<Object*, constring> items;

	static void copy(Object* self, const Object* in);
	static void destructor(Object* self);
	static void constructor(Object* self);
};

extern ObjectType DictObjectType;
