
#pragma once

#include <Types.h>

struct PyInterp {
	PyInterp();
	void exec(string cmd, struct Object* self, struct Object* args);
	~PyInterp();
};