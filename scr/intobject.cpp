
#pragma once

#include "intobject.h"

void IntObject::constructor(Object* self) {
	NDO_CAST(IntObject, self)->val = 0;
}

void IntObject::copy(Object* self, const Object* in) {
	NDO_CAST(IntObject, self)->val = NDO_CAST(IntObject, in)->val;
}

IntObject* IntObject::create(alni in) {
	NDO_CASTV(IntObject, NDO.create("int"), out)->val = in;
	return out;
}

struct ObjectType IntObjectType = {
	.base = NULL,
	.constructor = IntObject::constructor,
	.destructor = NULL,
	.copy = IntObject::copy,
	.size = sizeof(IntObject),
	.name = "int",
};