
#include "object.h"
#include "classobject.h"

struct MethodObject : Object {

	ClassObject* self;
	void* code;
	alni code_flags;

	static void constructor(Object* in) {
		CCASTV(MethodObject, in, self);

		self->self = NULL;
		self->code = NULL;
		self->code_flags = NULL;
	}

	static void copy(Object* self, Object* in) {
		CCAST(MethodObject, self)->code = CCAST(MethodObject, in)->code;
		CCAST(MethodObject, self)->code_flags = CCAST(MethodObject, in)->code_flags;
		CCAST(MethodObject, self)->self = CCAST(MethodObject, in)->self;
	}

};

struct ObjectType ClassType = {
	.base = NULL,
	.constructor = MethodObject::constructor,
	.destructor = NULL,
	.copy = MethodObject::copy,
	.size = sizeof(MethodObject),
	.name = "class",
};
