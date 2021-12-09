
#include "object.h"

#include "dictobject.h"

struct ClassObject : Object {

	DictObject*  members;

	static void copy(Object* self, Object* in) {
		NDO.copy(CCAST(ClassObject, self)->members, CCAST(ClassObject, in)->members);
	}

	static void destructor(Object* in) {
		CCASTV(ClassObject, in, self);

		NDO.destroy(self->members);
	}

	static void constructor(Object* in) {
		CCASTV(ClassObject, in, self);
		self->members = CCAST(DictObject, NDO.create("dict"));
	}

};

struct ObjectType ClassType = {
	.base = NULL,
	.constructor = ClassObject::constructor,
	.destructor = ClassObject::destructor,
	.copy = ClassObject::copy,
	.size = sizeof(ClassObject),
	.name = "class",
};
