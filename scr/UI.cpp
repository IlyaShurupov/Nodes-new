#pragma once

#include "UI.h"

ObjectStaticMethod UIMethods[] = {
	{"proc_inputs", UIClass::proc_inputs},
	{"present_outut", UIClass::present_ouput},
	{NULL, NULL}
};

struct ObjectType UIClassType = {
	.base = &ClassObjectType,
	.constructor = UIClass::constructor,
	.destructor = UIClass::destructor,
	.copy = UIClass::copy,
	.size = sizeof(UIClass),
	.name = "UI",
};

void UIClass::copy(Object* self, const Object* in) {}
void UIClass::destructor(Object* in) {}
void UIClass::constructor(Object* in) {}
Object* UIClass::proc_inputs(Object* self, Object* args) { return NDO_NULL; };
Object* UIClass::present_ouput(Object* self, Object* args) { return NDO_NULL; };


ObjectStaticMethod GUIMethods[] = {
	{"proc_inputs", GUIClass::proc_inputs},
	{"present_output", GUIClass::present_ouput},
	{NULL, NULL}
};

struct ObjectType GUIClassType = {
	.base = &ClassObjectType,
	.constructor = GUIClass::constructor,
	.destructor = GUIClass::destructor,
	.copy = GUIClass::copy,
	.size = sizeof(GUIClass),
	.name = "GUI",
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

Object* GUIClass::proc_inputs(Object* self, Object* args) {
	return NDO_NULL;
};

Object* GUIClass::present_ouput(Object* in, Object* args) {

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


ObjectStaticMethod TUIMethods[] = {
	{"proc_inputs", TUIClass::proc_inputs},
	{"present_output", TUIClass::present_ouput},
	{NULL, NULL}
};

struct ObjectType TUIClassType = {
	.base = &ClassObjectType,
	.constructor = TUIClass::constructor,
	.destructor = TUIClass::destructor,
	.copy = TUIClass::copy,
	.size = sizeof(TUIClass),
	.name = "TUI",
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

Object* TUIClass::proc_inputs(Object* in, Object* args) {
	NDO_CASTV(TUIClass, in, self);

	self->keyboard.PumpEvents();

	self->keyboard.ClearEvents();

	return NDO_NULL;
};

Object* TUIClass::present_ouput(Object* in, Object* args) {
	//NDO_CASTV(TUIClass, in, self);
	return NDO_NULL;
};