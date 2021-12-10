
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


extern struct object_types NDO;

struct Object {
	const struct ObjectType* type;
};

typedef void (*object_from_int)(Object* self, alni in);
typedef void (*object_from_float)(Object* self, alnf in);
typedef void (*object_from_string)(Object* self, constring in);
typedef string (*object_to_string)(Object* self);
typedef alni (*object_to_int)(Object* self);
typedef alnf (*object_to_float)(Object* self);

struct ObjectTypeConversions {
	object_from_int from_int;
	object_from_float from_float;
	object_from_string from_string;
	object_to_string to_string;
	object_to_int to_int;
	object_to_float to_float;
};

typedef void (*object_constructor)(Object* self);
typedef void (*object_destructor)(Object* self);
typedef void (*object_copy)(Object* self, const Object* target);

struct ObjectType {
	const ObjectType* base;
	object_constructor constructor = NULL;
	object_destructor destructor = NULL;
	object_copy copy = NULL;
	alni size = NULL;
	constring name;
	const ObjectTypeConversions* convesions = NULL;
};

struct object_types {

	hmap<const ObjectType*, constring> types;

	void define(ObjectType* type);
	Object* create(constring name);
	Object* copy(Object* self, const Object* in);
	
	void set(Object* self, alni val);
	void set(Object* self, alnf val);
	void set(Object* self, constring val);

	void destroy(Object* in);

	void hierarchy_copy(Object* self, const Object* in, const ObjectType* type);
	void hierarchy_construct(Object* self, const ObjectType* type);

};

Object* ndo_cast(const Object* in, const ObjectType* to_type);