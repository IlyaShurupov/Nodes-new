
#pragma once

#include "object.h"

struct ListObject : Object {
	List<Object*> items;

	static void constructor(Object* self);
	static void copy(Object* self, const Object* in);
	static void destructor(Object* self);
};
 

extern ObjectType ListObjectType;