
#pragma once

#include "listobject.h"

#include "intobject.h"
#include "nullobject.h"

void ListObject::constructor(Object* in) {
	NDO_CASTV(ListObject, in, self);

	new (&self->items) List<Object*>(false);
}

void ListObject::copy(Object* in, const Object* target) {
	NDO_CASTV(ListObject, in, self);
	
	for (auto item : self->items) {
		NDO.destroy(item.Data());
	}

	self->items = NDO_CAST(ListObject, target)->items;
}

void ListObject::destructor(Object* in) {
	NDO_CASTV(ListObject, in, self);

	for (auto item : self->items) {
		NDO.destroy(item.Data());
	}
	
	self->items.Clear();
}

Object* ListObject::length(ListObject* self, Object* arg) {
	return IntObject::create(self->items.Len());
}

Object* ListObject::insert(ListObject* self, Object* arg) {
	Object* idx_obj = arg;
	Object* insert_obj = NDO.callstak.get_arg(1);

	alni idx = NDO_CAST(IntObject, idx_obj)->val;
	self->items.Insert(insert_obj, idx);

	return self;
}

Object* ListObject::remove(ListObject* self, Object* arg) {
	Object* rem_obj = (&arg)[0];
	self->items.DelNode(self->items.Find(rem_obj));
	return NDO_NULL;
}

Object* ListObject::get(ListObject * self, Object * arg) {
	alni idx = NDO_CAST(IntObject, arg)->val;
	return self->items[idx];
}

ObjectStaticMethod ListTypeMethods[] = {
	{(ndo_method)ListObject::get, "get"},
	{(ndo_method)ListObject::remove, "remove"},
	{(ndo_method)ListObject::insert, "insert"},
	{(ndo_method)ListObject::length, "length"},
	{ NULL , NULL},
};

struct ObjectType ListObjectType = {
	.base = NULL,
	.constructor = ListObject::constructor,
	.destructor = ListObject::destructor,
	.copy = ListObject::copy,
	.size = sizeof(ListObject),
	.name = "list",
	.convesions = NULL,
	.type_methods = ListTypeMethods,
};