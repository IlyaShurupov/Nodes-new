#pragma once

#include "classobject.h"

void ClassObject::copy(Object* self, const Object* in) {
	NDO.copy(NDO_CAST(ClassObject, self)->members, NDO_CAST(ClassObject, in)->members);
}

void ClassObject::destructor(Object* in) {
	NDO_CASTV(ClassObject, in, self);

	NDO.destroy(self->members);
}

void ClassObject::constructor(Object* in) {
	NDO_CASTV(ClassObject, in, self);
	self->members = NDO_CAST(DictObject, NDO.create("dict"));
}

static void save(ClassObject* self, File& file_self) {

	// only dictinary file adress
	alni save_size = 1;

	// allocate mem by moving down avk adress
	file_self.avl_adress += save_size;

	// save dict adress
	alni dict_adress = NDO.save(file_self, self->members);
	file_self.write<alni>(&dict_adress);
}

static Object* load(File& file_self) {
	ClassObject* self = (ClassObject*)NDO.create("class");

	alni dict_adress;
	file_self.read<alni>(&dict_adress);

	// find better solution
	NDO.destroy(self->members);

	Object* dictinary = NDO.load(file_self, dict_adress);

	self->members = (DictObject*)dictinary;

	return self;
}

struct ObjectType ClassObjectType = {
	.base = NULL,
	.constructor = ClassObject::constructor,
	.destructor = ClassObject::destructor,
	.copy = ClassObject::copy,
	.size = sizeof(ClassObject),
	.name = "class",
};
