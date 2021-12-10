
#pragma once

#include "listobject.h"

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


struct ObjectType ListObjectType = {
	.base = NULL,
	.constructor = ListObject::constructor,
	.destructor = ListObject::destructor,
	.copy = ListObject::copy,
	.size = sizeof(ListObject),
	.name = "list",
};