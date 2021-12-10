#pragma once

#include "classobject.h"

void ClassObject::copy(Object* self, const Object* in) {
	NDO.copy(NDO_CAST(ClassObject, self)->members, NDO_CAST(ClassObject, in)->members);
}

void ClassObject::destructor(Object* in) {
	NDO_CASTV(ClassObject, in, self);

	NDO.destroy(self->members);
}

void ClassObject::constructor(Object* in) {
	NDO_CASTV(ClassObject, in, self);
	self->members = NDO_CAST(DictObject, NDO.create("dict"));

}

struct ObjectType ClassObjectType = {
	.base = NULL,
	.constructor = ClassObject::constructor,
	.destructor = ClassObject::destructor,
	.copy = ClassObject::copy,
	.size = sizeof(ClassObject),
	.name = "class",
};
