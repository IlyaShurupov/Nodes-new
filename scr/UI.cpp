#pragma once

#include "UI.h"

static void UIClass_load(File& file_self, UIClass* self) {
	NDO_CASTV(MethodObject, self->member("proc_inputs"), run_meth);
	if (!run_meth->code_flags) {
		run_meth->code.ccode.method = UIClass::proc_inputs;
	}

	NDO_CASTV(MethodObject, self->member("present_output"), foo_meth);
	if (!foo_meth->code_flags) {
		foo_meth->code.ccode.method = UIClass::present_ouput;
	}
}


ObjectStaticMethod UIMethods[] = {
	{UIClass::proc_inputs, "proc_inputs"},
	{UIClass::present_ouput, "present_output"},
	{NULL, NULL}
};

struct ObjectType UIClassType = {
	.base = &ClassObjectType,
	.constructor = UIClass::constructor,
	.destructor = UIClass::destructor,
	.copy = UIClass::copy,
	.size = sizeof(UIClass),
	.name = "UI",
	.load = (object_load)UIClass_load,
};

void UIClass::copy(Object* self, const Object* in) {}
void UIClass::destructor(Object* in) {}
void UIClass::constructor(Object* in) {}
Object* UIClass::proc_inputs(Object* self, Object*) { return NDO_NULL; };
Object* UIClass::present_ouput(Object* self, Object*) { return NDO_NULL; };


static void GUIClass_load(File& file_self, GUIClass* self) {
	NDO_CASTV(MethodObject, self->member("proc_inputs"), run_meth);
	if (!run_meth->code_flags) {
		run_meth->code.ccode.method = GUIClass::proc_inputs;
	}

	NDO_CASTV(MethodObject, self->member("present_output"), foo_meth);
	if (!foo_meth->code_flags) {
		foo_meth->code.ccode.method = GUIClass::present_ouput;
	}
}

ObjectStaticMethod GUIMethods[] = {
	{GUIClass::proc_inputs, "proc_inputs"},
	{GUIClass::present_ouput, "present_output"},
	{NULL, NULL}
};

struct ObjectType GUIClassType = {
	.base = &ClassObjectType,
	.constructor = GUIClass::constructor,
	.destructor = GUIClass::destructor,
	.copy = GUIClass::copy,
	.size = sizeof(GUIClass),
	.name = "GUI",
	.load = (object_load)GUIClass_load,
};

void GUIClass::constructor(Object* in) {
	NDO_CASTV(GUIClass, in, self);
	new (&self->window) Window();

	self->add_methods(GUIMethods);
}

void GUIClass::copy(Object* self, const Object* in) {
	NDO_CAST(GUIClass, self)->window = NDO_CAST(GUIClass, in)->window;
}

void GUIClass::destructor(Object* in) {
	NDO_CAST(GUIClass, in)->window.~Window();
}

Object* GUIClass::proc_inputs(Object* self, Object*) {
	return NDO_NULL;
};

Object* GUIClass::present_ouput(Object* in, Object*) {

	NDO_CASTV(GUIClass, in, self);

	self->window.BeginFrame();

	vec2<float> crs = self->window.GetCrsr();

	if (1) {
		int bord_size = 2;
		Color col(0.3f, 0.3f, 0.3f, 1.f);
		Color col2(0.08f, 0.08f, 0.08f, 1.f);
		Rect<float> inner(vec2<float>(bord_size, bord_size), self->window.window_dim.size - bord_size * 2.f);
		
		self->window.RRect(Rect<float>(vec2<float>(), self->window.window_dim.size), col, 11.f);
		self->window.RRect(inner, col2, 9.f);
	}

	self->window.EndFrame();
	return NDO_NULL;
};

static void TUIClass_load(File& file_self, TUIClass* self) {
	NDO_CASTV(MethodObject, self->member("proc_inputs"), run_meth);
	if (!run_meth->code_flags) {
		run_meth->code.ccode.method = TUIClass::proc_inputs;
	}

	NDO_CASTV(MethodObject, self->member("present_output"), foo_meth);
	if (!foo_meth->code_flags) {
		foo_meth->code.ccode.method = TUIClass::present_ouput;
	}
}

ObjectStaticMethod TUIMethods[] = {
	{TUIClass::proc_inputs, "proc_inputs"},
	{TUIClass::present_ouput, "present_output"},
	{NULL, NULL}
};

struct ObjectType TUIClassType = {
	.base = &ClassObjectType,
	.constructor = TUIClass::constructor,
	.destructor = TUIClass::destructor,
	.copy = TUIClass::copy,
	.size = sizeof(TUIClass),
	.name = "TUI",
	.load = (object_load)TUIClass_load,
};

void TUIClass::constructor(Object* in) {
	NDO_CASTV(TUIClass, in, self);
	new (&self->keyboard) Keyboard();

	self->add_methods(TUIMethods);
}

void TUIClass::copy(Object* self, const Object* in) {
	NDO_CAST(TUIClass, self)->keyboard = NDO_CAST(TUIClass, in)->keyboard;
}

void TUIClass::destructor(Object* in) {
	NDO_CAST(TUIClass, in)->keyboard.~Keyboard();
}

Object* TUIClass::proc_inputs(Object* in, Object*) {
	NDO_CASTV(TUIClass, in, self);

	self->keyboard.PumpEvents();

	self->keyboard.ClearEvents();
	return NDO_NULL;
};

Object* TUIClass::present_ouput(Object* in, Object*) {
	//NDO_CASTV(TUIClass, in, self);
	return NDO_NULL;
};