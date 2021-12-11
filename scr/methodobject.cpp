
#pragma once

#include "methodobject.h"

#include "PyInterp.h"

PyInterp GlobalMethod_PyInterp;

void MethodObject::constructor(Object* in) {
	NDO_CASTV(MethodObject, in, self);

	self->self = NULL;
	self->code_flags = NULL;
}

void MethodObject::copy(Object* o_self, const Object* o_in) {

	NDO_CASTV(MethodObject, o_self, self);
	NDO_CASTV(MethodObject, o_in, in);

	if (self->code_flags == 1) {
		self->code.pycode.clear();
	}
	
	if (in->code_flags == 1) {
		self->code.pycode = in->code.pycode;
	}
	else {
		self->code.ccode = in->code.ccode;
	}
	
	self->code_flags = in->code_flags;
}

Object* MethodObject::operator()(Object* args) {
	if (!code_flags) {
		code.ccode.method((Object*)self, args);
	}
	else if (code_flags == 1) {
		GlobalMethod_PyInterp.exec(code.pycode, (Object*)self, args);
	}

	return NULL;
}

void MethodObject::from_string(Object* in, string code) {
	NDO_CASTV(MethodObject, in, self);
	self->code_flags = 1;

	new (&self->code.pycode) string(code);
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
