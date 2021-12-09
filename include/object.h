
#pragma once

#include "Types.h"

#include "conststring.h"

#define CCAST(type, ptr) ((type*)ptr)
#define CCASTV(type, ptr, name) type* name = ((type*)ptr); name

/* Steps to create custom Object:
define name of object
define base type
define struct members
implement construct, destruct and copy methods */

typedef void (*object_constructor)(struct Object* self);
typedef void (*object_destructor)(Object* self);
typedef void (*object_copy)(Object* self, Object* target);

struct Object {
	const struct ObjectType* type;
};

struct ObjectType {
	const ObjectType* base;
	object_constructor constructor = NULL;
	object_destructor destructor = NULL;
	object_copy copy = NULL;
	alni size = NULL;
	constring name;
};

struct object_types {

	hmap<const ObjectType*, constring> types;

	void define(ObjectType* type, constring* base = NULL) {
		types.Put(type->name, type);
		if (base) {
			type->base = types.Get(*base);
		}
	}

	Object* create(constring name) {
		const ObjectType* type = types.Get(name);
		Object* obj_instance = (Object*)malloc(type->size);

		if (!obj_instance) {
			return NULL;
		}
		
		obj_instance->type = type;
		
		hierarchy_construct(obj_instance, obj_instance->type);

		return obj_instance;
	}

	Object* copy(Object* self, Object* in) {
		if (self->type != in->type) {
			return NULL;
		}

		hierarchy_copy(self, in, self->type);

		return self;
	}

	void destroy(Object* in) {
		if (!in) {
			return;
		}

		for (const ObjectType* iter = in->type; iter && iter->destructor; iter = iter->base) {
			iter->destructor(in);
		}
	}

	void hierarchy_copy(Object* self, Object* in, const ObjectType* type) {
		if (type->base) {
			hierarchy_copy(self, in, type->base);
		}

		if (type->copy) {
			type->copy(self, in);
		}
	}

	void hierarchy_construct(Object* self, const ObjectType* type) {
		if (type->base) {
			hierarchy_construct(self, type->base);
		}

		if (type->constructor) {
			type->constructor(self);
		}
	}

};

extern object_types NDO;