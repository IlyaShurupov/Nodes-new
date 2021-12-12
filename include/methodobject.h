
#pragma once

#include "object.h"

union MethoObjectCode {
	string pycode;
	ndo_static_method ccode;

	~MethoObjectCode() {
	}
};

struct MethodObject : Object {

	struct ClassObject* self;
	MethoObjectCode code;
	alni code_flags;

	void assign(string pycode) {
		if (code_flags == 1) {
			code.pycode = pycode;
		}
		else {
			new (&code.pycode) string(pycode);
			code_flags = 1;
		}
	}

	void assign(ndo_static_method ccode) {
		if (code_flags == 1) {
			code.pycode.~string();
			code_flags = 0;
		}
		code.ccode = ccode;
	}

	static void constructor(Object* in);
	static void copy(Object* self, const Object* in);

	Object* operator()();

	static void from_string(Object* self, string in);
};

extern ObjectType MethodObjectType;