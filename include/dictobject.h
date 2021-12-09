
#include "object.h"

struct DictObject : Object {
	hmap<Object*, constring> items;

	static void copy(Object* self, Object* in) {
		CCAST(DictObject, self)->items = CCAST(DictObject, in)->items;
	}

	static void destructor(Object* self) {
		CCASTV(DictObject, self, dict);

		for (auto item : dict->items) {
			NDO.destroy(item->val);
		}

		dict->items.clear();
	}

	static void constructor(Object* self) {
		CCASTV(DictObject, self, dict);

		new (&dict->items) hmap<Object*, constring>();

		dict->items.del_values = false;
	}
};

struct ObjectType DictType = {
	.base = NULL,
	.constructor = DictObject::constructor,
	.destructor = DictObject::destructor,
	.copy = DictObject::copy,
	.size = sizeof(DictObject),
	.name = "dict",
};
