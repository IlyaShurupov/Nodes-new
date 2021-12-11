

#include "object.h"

object_types NDO;

struct ObjectMemHead {
	ObjectMemHead* up;
	ObjectMemHead* down;
	alni flags;
};

ObjectMemHead* bottom = NULL;

void hierarchy_copy(Object* self, const Object* in, const ObjectType* type);
void hierarchy_construct(Object* self, const ObjectType* type);

void object_types::define(ObjectType* type) {

	for (ObjectStaticMethod* iter = type->type_methods; iter;  iter++) {
		type->type_methods_dict.Put(iter->name, &iter->function_adress);
	}

	types.Put(type->name, type);
}

Object* object_types::create(string name) {
	const ObjectType* type = types.Get(name);

	ObjectMemHead* memhead = (ObjectMemHead *)malloc(sizeof(ObjectMemHead) + type->size);
	Object* obj_instance = (Object*)(memhead + 1);

	if (!obj_instance) {
		return NULL;
	}

	obj_instance->type = type;

	hierarchy_construct(obj_instance, obj_instance->type);

	if (bottom) {
		bottom->down = memhead;
	}
	else {
		memhead->down = bottom;
	}
	memhead->down = NULL;
	memhead->up = bottom;
	bottom = memhead;
	
	memhead->flags = NULL;

	return obj_instance;
}

Object* object_types::copy(Object* self, const Object* in) {
	if (self->type != in->type) {
		return NULL;
	}

	hierarchy_copy(self, in, self->type);

	return self;
}

void object_types::set(Object* self, alni val) {
	if (self->type->convesions && self->type->convesions->from_int) {
		self->type->convesions->from_int(self, val);
		return;
	}
}

void object_types::set(Object* self, alnf val) {
	if (self->type->convesions && self->type->convesions->from_float) {
		self->type->convesions->from_float(self, val);
		return;
	}
}

void object_types::set(Object* self, string val) {
	if (self->type->convesions && self->type->convesions->from_string) {
		self->type->convesions->from_string(self, val);
		return;
	}
}

struct ObjectFileHead {

	ObjectFileHead(const ObjectType* in) {
		type_size = in->size;

		alni i = 0;
		for (; in->name.str[i] != '\0'; i++) {
			type_name[i] = in->name.str[i];
		}
		type_name[i] = '\0';
	}

	ObjectFileHead() {
	}

	alni type_size;
	char type_name[32];
};

alni object_types::save(Object* in) {

	for (ObjectMemHead* memh_iter = bottom; memh_iter; memh_iter = memh_iter->up) {
		memh_iter->flags = 0;
	}
	
	ObjectMemHead* memh = ((ObjectMemHead*)in) - 1;

	File ndf("save.nd");
	ObjectFileHead ofh(in->type);
	ndf.write<ObjectFileHead>(&ofh);

	in->type->save(in, ndf);

	return 0;
}

Object* object_types::load() {
	File ndf("save.nd");

	ObjectFileHead ofh;

	ndf.read<ObjectFileHead>(&ofh);

	const ObjectType* load_type = NDO.types.Get(ofh.type_name);

	Object* out = load_type->load(ndf);

	return out;
}

Object* object_types::call_type_method(string name, Object* self, Object* args) {
	ObjectType* type = ((ObjectType*)self->type);
	alni idx = type->type_methods_dict.Presents(name);
	if (idx == -1) {
		return NULL;
	}

	ndo_static_method* type_method = type->type_methods_dict.table[idx]->val;
	
	type_method->method(self, args);

	return nullptr;
}

void object_types::destroy(Object* in) {
	if (!in) {
		return;
	}

	for (const ObjectType* iter = in->type; iter && iter->destructor; iter = iter->base) {
		iter->destructor(in);
	}

	ObjectMemHead* memh = ((ObjectMemHead*)in) - 1;

	if (memh->down) {
		memh->down->up = memh->up;
	} 
	else {
		bottom = memh->up;
	}
	if (memh->up) {
		memh->up->down = memh->down;
	}

	free(memh);
}

void hierarchy_copy(Object* self, const Object* in, const ObjectType* type) {
	if (type->base) {
		hierarchy_copy(self, in, type->base);
	}

	if (type->copy) {
		type->copy(self, in);
	}
}

void hierarchy_construct(Object* self, const ObjectType* type) {
	if (type->base) {
		hierarchy_construct(self, type->base);
	}

	if (type->constructor) {
		type->constructor(self);
	}
}

Object* ndo_cast(const Object* in, const ObjectType* to_type) {
	const ObjectType* typeiter = in->type;
	while (typeiter) {
		if (typeiter == to_type) {
			return (Object*)in;
		}
		typeiter = typeiter->base;
	}
	return NULL;
}