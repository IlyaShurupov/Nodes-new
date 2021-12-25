#pragma once

#include "classobject.h"


extern ObjectType UIClassType;

struct UIClass : ClassObject {

	static void copy(Object* self, const Object* in);
	static void destructor(Object* in);
	static void constructor(Object* in);

	static Object* proc_inputs(Object*, Object*);
	static Object* present_ouput(Object*, Object*);

	static void TypeInit();
};


extern ObjectType GUIClassType;

struct GUIClass : UIClass {

	Window window;

	static void copy(Object* self, const Object* in);
	static void destructor(Object* in);
	static void constructor(Object* in);

	static Object* proc_inputs(Object*, Object*);
	static Object* present_ouput(Object*, Object*);
};

extern ObjectType TUIClassType;

struct TUIClass : UIClass {

	Keyboard keyboard;

	static void copy(Object* self, const Object* in);
	static void destructor(Object* in);
	static void constructor(Object* in);

	static Object* proc_inputs(Object*, Object*);
	static Object* present_ouput(Object*, Object*);
};
