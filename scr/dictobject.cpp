#pragma once

#include "dictobject.h"


void DictObject::copy(Object* self, const Object* in) {
	NDO_CAST(DictObject, self)->items = NDO_CAST(DictObject, in)->items;
}

void DictObject::destructor(Object* self) {
	NDO_CASTV(DictObject, self, dict);

	for (auto item : dict->items) {
		NDO.destroy(item->val);
	}

	dict->items.clear();
}

void DictObject::constructor(Object* self) {
	NDO_CASTV(DictObject, self, dict);

	new (&dict->items) hmap<Object*, string>();

	dict->items.del_values = false;
}

struct ObjectType DictObjectType = {
	.base = NULL,
	.constructor = DictObject::constructor,
	.destructor = DictObject::destructor,
	.copy = DictObject::copy,
	.size = sizeof(DictObject),
	.name = "dict",
};
