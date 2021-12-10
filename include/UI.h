#pragma once

#include "classobject.h"


extern ObjectType UIClassType;

struct UIClass : ClassObject {

	static void copy(Object* self, const Object* in);
	static void destructor(Object* in);
	static void constructor(Object* in);

	static Object* proc_inputs(ClassObject*, MethodObjectArgument*);
	static Object* present_ouput(ClassObject*, MethodObjectArgument*);
};


extern ObjectType GUIClassType;

struct GUIClass : UIClass {

	Window window;

	static void copy(Object* self, const Object* in);
	static void destructor(Object* in);
	static void constructor(Object* in);

	static Object* proc_inputs(ClassObject*, MethodObjectArgument*);
	static Object* present_ouput(ClassObject*, MethodObjectArgument*);
};

extern ObjectType TUIClassType;

struct TUIClass : UIClass {

	Keyboard keyboard;

	static void copy(Object* self, const Object* in);
	static void destructor(Object* in);
	static void constructor(Object* in);

	static Object* proc_inputs(ClassObject*, MethodObjectArgument*);
	static Object* present_ouput(ClassObject*, MethodObjectArgument*);
};
