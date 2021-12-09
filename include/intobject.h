
#include "object.h"

struct IntObject : Object {
	alni val;

	static void constructor(Object* self) {
		CCAST(IntObject, self)->val = 0;
	}

	static void copy(Object* self, Object* in) {
		CCAST(IntObject, self)->val = CCAST(IntObject, in)->val;
	}
};
 

struct ObjectType IntType = {
	.base = NULL,
	.constructor = IntObject::constructor,
	.destructor = NULL,
	.copy = IntObject::copy,
	.size = sizeof(IntObject),
	.name = "int",
};