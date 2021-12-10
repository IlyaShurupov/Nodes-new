
#pragma once

#include "Types.h"

#ifdef _DEBUG
#define NDO_CAST(cast_type, ptr) ((cast_type*)ndo_cast(ptr, &cast_type##Type))
#else
#define NDO_CAST(cast_type, ptr) (cast_type*)ptr)
#endif

#define NDO_CASTV(cast_type, ptr, var_name) cast_type* var_name = NDO_CAST(cast_type, ptr); var_name

/* Steps to create custom Object:
define name of object
define base type
define struct members
implement construct, destruct and copy methods */

typedef void (*object_constructor)(struct Object* self);
typedef void (*object_destructor)(Object* self);
typedef void (*object_copy)(Object* self, const Object* target);

extern struct object_types NDO;

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

	void define(ObjectType* type);
	Object* create(constring name);
	Object* copy(Object* self, const Object* in);
	void destroy(Object* in);

	void hierarchy_copy(Object* self, const Object* in, const ObjectType* type);
	void hierarchy_construct(Object* self, const ObjectType* type);

};

Object* ndo_cast(const Object* in, const ObjectType* to_type);