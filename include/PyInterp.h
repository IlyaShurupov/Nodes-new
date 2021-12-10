
#pragma once

#include <Types.h>

struct PyInterp {
	PyInterp();
	void exec(constring cmd, struct Object* self, struct MethodObjectArgument* args);
	~PyInterp();
};