

#include "object.h"

object_types NDO;

void object_types::define(ObjectType* type) {
	types.Put(type->name, type);
}

Object* object_types::create(string name) {
	const ObjectType* type = types.Get(name);
	Object* obj_instance = (Object*)malloc(type->size);

	if (!obj_instance) {
		return NULL;
	}

	obj_instance->type = type;

	hierarchy_construct(obj_instance, obj_instance->type);

	return obj_instance;
}

Object* object_types::copy(Object* self, const Object* in) {
	if (self->type != in->type) {
		return NULL;
	}

	hierarchy_copy(self, in, self->type);

	return self;
}

void object_types::destroy(Object* in) {
	if (!in) {
		return;
	}

	for (const ObjectType* iter = in->type; iter && iter->destructor; iter = iter->base) {
		iter->destructor(in);
	}
}

void object_types::hierarchy_copy(Object* self, const Object* in, const ObjectType* type) {
	if (type->base) {
		hierarchy_copy(self, in, type->base);
	}

	if (type->copy) {
		type->copy(self, in);
	}
}

void object_types::hierarchy_construct(Object* self, const ObjectType* type) {
	if (type->base) {
		hierarchy_construct(self, type->base);
	}

	if (type->constructor) {
		type->constructor(self);
	}
}

Object* ndo_cast(const Object* in, const ObjectType* to_type) {
	const ObjectType* typeiter = in->type;
	while (typeiter) {
		if (typeiter == to_type) {
			return (Object*)in;
		}
		typeiter = typeiter->base;
	}
	return NULL;
}

void object_types::set(Object* self, alni val) {
	if (self->type->convesions && self->type->convesions->from_int) {
		self->type->convesions->from_int(self, val);
		return;
	}
}

void object_types::set(Object* self, alnf val) {
	if (self->type->convesions && self->type->convesions->from_float) {
		self->type->convesions->from_float(self, val);
		return;
	}
}

void object_types::set(Object* self, string val) {
	if (self->type->convesions && self->type->convesions->from_string) {
		self->type->convesions->from_string(self, val);
		return;
	}
}