
#pragma once

#include "methodobject.h"

#include "PyInterp.h"

PyInterp GlobalMethod_PyInterp;

void MethodObject::constructor(Object* in) {
	NDO_CASTV(MethodObject, in, self);

	self->self = NULL;
	self->code = NULL;
	self->code_flags = NULL;
}

void MethodObject::copy(Object* self, const Object* in) {
	NDO_CAST(MethodObject, self)->code = NDO_CAST(MethodObject, in)->code;
	NDO_CAST(MethodObject, self)->code_flags = NDO_CAST(MethodObject, in)->code_flags;
	NDO_CAST(MethodObject, self)->self = NDO_CAST(MethodObject, in)->self;
}

Object* MethodObject::operator()(MethodObjectArgument* args) {
	if (!code_flags) {
		((ndo_static_method)code)(self, args);
	}
	else if (code_flags == 1) {
		GlobalMethod_PyInterp.exec(constring((const char*)code), (Object*)self, args);
	}

	return NULL;
}

void MethodObject::from_string(Object* in, constring code) {
	NDO_CASTV(MethodObject, in, self);
	self->code_flags = 1;

	// FIXME!!!
	string bad;
	bad = code.str;
	self->code = (void*)bad.str;
	bad.str = 0;
}

struct ObjectTypeConversions MethodTypeConversions = {
	NULL,
	NULL,
	MethodObject::from_string,
	NULL,
	NULL,
	NULL,
};

struct ObjectType MethodObjectType = {
	.base = NULL,
	.constructor = MethodObject::constructor,
	.destructor = NULL,
	.copy = MethodObject::copy,
	.size = sizeof(MethodObject),
	.name = "method",
	.convesions = &MethodTypeConversions,
};
