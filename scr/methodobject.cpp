
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

	if (in->code_flags == 1) {
		self->assign(in->code.pycode);
	}
	else {
		self->assign(in->code.ccode);
	}
	
	self->self = in->self;
}

Object* MethodObject::operator()() {
	if (!code_flags) {
		return NDO.call((Object*)self, code.ccode);
	}
	else if (code_flags == 1) {
		NDO.callstak.call();
		return GlobalMethod_PyInterp.exec(code.pycode, (Object*)self);
		NDO.callstak.ret();
	}
}

void MethodObject::from_string(Object* in, string code) {
	NDO_CASTV(MethodObject, in, self);
	self->assign(code);
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
