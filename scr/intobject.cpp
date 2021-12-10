
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

void IntObject::from_int(Object* self, alni in) {
	NDO_CAST(IntObject, self)->val = in;
}

void IntObject::from_float(Object* self, alnf in) {
	NDO_CAST(IntObject, self)->val = (alni)in;
}

void IntObject::from_string(Object* self, constring in) {
	NDO_CAST(IntObject, self)->val = in.to_int();
}

string IntObject::to_string(Object* self) {
	return string(NDO_CAST(IntObject, self)->val);
}

alni IntObject::to_int(Object* self) {
	return alni(NDO_CAST(IntObject, self)->val);
}

alnf IntObject::to_float(Object* self) {
	return alnf(NDO_CAST(IntObject, self)->val);
}

struct ObjectTypeConversions IntObjectTypeConversions = {
	.from_int = IntObject::from_int,
	.from_float = IntObject::from_float,
	.from_string = IntObject::from_string,
	.to_string = IntObject::to_string,
	.to_int = IntObject::to_int,
	.to_float = IntObject::to_float,
};

struct ObjectType IntObjectType = {
	.base = NULL,
	.constructor = IntObject::constructor,
	.destructor = NULL,
	.copy = IntObject::copy,
	.size = sizeof(IntObject),
	.name = "int",
	.convesions = &IntObjectTypeConversions,
};