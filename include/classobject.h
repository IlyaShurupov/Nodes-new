#pragma once

#include "object.h"

#include "nullobject.h"
#include "dictobject.h"
#include "methodobject.h"

struct ClassObject : Object {

	DictObject*  members;

	static void copy(Object* self, const Object* in);
	static void destructor(Object* in);
	static void constructor(Object* in);

	void add_methods(const ObjectStaticMethod* methods) {

		for (; *((alni*)methods); methods++) {
			NDO_CASTV(MethodObject, NDO.create("method"), class_method);
			class_method->self = this;
			class_method->code.ccode = methods->function_adress;

			members->items.Put(methods->name, class_method);
		}
	}

	Object* call(string name) {

		NDO_CASTV(MethodObject, members->items.Get(name), method);
		
		if (method->type != &MethodObjectType) {
			return NULL;
		}

		return (*method)();
	}

	Object* member(string name) {
		return members->items.Get(name);
	}
};

extern ObjectType ClassObjectType;
