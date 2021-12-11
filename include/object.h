
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

struct ndo_static_method {
	Object* (*method)(struct Object* self, Object* args);
};


struct ObjectStaticMethod {
	string name;
	ndo_static_method function_adress;
};

typedef void (*object_from_int)(Object* self, alni in);
typedef void (*object_from_float)(Object* self, alnf in);
typedef void (*object_from_string)(Object* self, string in);
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
typedef void (*object_save)(Object*, File&);
typedef Object* (*object_load)(File&);

struct ObjectType {
	const ObjectType* base;
	object_constructor constructor = NULL;
	object_destructor destructor = NULL;
	object_copy copy = NULL;
	alni size = NULL;
	string name;
	const ObjectTypeConversions* convesions = NULL;
	object_save save = NULL;
	object_load load = NULL;
	ObjectStaticMethod* type_methods = NULL;
	hmap<ndo_static_method*, string> type_methods_dict;
};

union StackSlot {
	alni arg_count;
	Object* arg;
	Object* ret;
};

struct ObjectCallStack {
	StackSlot stack[100];

	alni method_base = 0;
	alni arg_count = 0;

	void push(Object* arg) {
		stack[arg_count + 1].arg = arg;
		stack[method_base].arg_count++;
	}

	Object* get_arg(char idx) {
		return stack[method_base + 1 + idx].arg;
	}

	void call() {
		arg_count = 0;
		method_base += stack[method_base].arg_count + 1;
	}

	void ret() {
		method_base -= stack[method_base].arg_count + 1;
	}
};

struct object_types {

	hmap<const ObjectType*, string> types;

	void define(ObjectType* type);
	Object* create(string name);
	Object* copy(Object* self, const Object* in);
	
	void set(Object* self, alni val);
	void set(Object* self, alnf val);
	void set(Object* self, string val);

	alni save(Object*);
	Object* load(); 

	Object* call_type_method(string name, Object*, Object*);

	void destroy(Object* in);

};

Object* ndo_cast(const Object* in, const ObjectType* to_type);