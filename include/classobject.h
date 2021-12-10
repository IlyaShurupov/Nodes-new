#pragma once

#include "object.h"

#include "nullobject.h"
#include "dictobject.h"
#include "methodobject.h"

struct ClassStaticMethod {
	constring name;
	ndo_static_method function_adress;
};

struct ClassObject : Object {

	DictObject*  members;

	static void copy(Object* self, const Object* in);
	static void destructor(Object* in);
	static void constructor(Object* in);

	void add_methods(const ClassStaticMethod* methods) {

		for (; *((alni*)methods); methods++) {
			NDO_CASTV(MethodObject, NDO.create("method"), class_method);
			class_method->self = this;
			class_method->code = (void*)methods->function_adress;

			members->items.Put(methods->name, class_method);
		}
	}

	Object* call(constring name, MethodObjectArgument* args) {

		NDO_CASTV(MethodObject, members->items.Get(name), method);
		
		if (method->type != &MethodObjectType) {
			return NULL;
		}

		return (*method)(args);
	}

	Object* member(constring name) {
		return members->items.Get(name);
	}
};

extern ObjectType ClassObjectType;
