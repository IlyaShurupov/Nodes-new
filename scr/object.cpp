

#include "object.h"

object_types NDO;

struct ObjectMemHead {
	ObjectMemHead* up;
	ObjectMemHead* down;
	alni flags;
};

ObjectMemHead* bottom = NULL;


Object* Object::call_type_method(string name) {
	alni idx = ((ObjectType*)type)->type_methods_dict.Presents(name);
	if (idx == -1) {
		return NULL;
	}
	ndo_static_method* type_method = type->type_methods_dict.table[idx]->val;
	return NDO.call(this, *type_method);
}

void hierarchy_copy(Object* self, const Object* in, const ObjectType* type);
void hierarchy_construct(Object* self, const ObjectType* type);

void object_types::define(ObjectType* type) {

	ObjectStaticMethod* iter = type->type_methods;
	for (; iter && iter->function_adress.method;  iter++) {
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
		alni i = 0;
		for (; in->name.str[i] != '\0'; i++) {
			type_name[i] = in->name.str[i];
		}
		type_name[i] = '\0';
	}

	ObjectFileHead() {
		type_name[15] = 0;
	}

	char type_name[16];
};

alni object_types::save(File& ndf, Object* in) {

	// save write adress for parent save function call 
	alni tmp_adress = ndf.adress;

	// save requested object to first avaliable adress
	alni save_adress = ndf.avl_adress;

	// update write adress
	ndf.adress = save_adress;

	// save file object header
	ObjectFileHead ofh(in->type);
	ndf.write<ObjectFileHead>(&ofh);

	// allocate for object file header
	ndf.avl_adress += sizeof(ObjectFileHead);

	in->type->save(in, ndf);

	// restore adress for parent save function
	ndf.adress = tmp_adress;

	// return addres of saved object in file space
	return save_adress;
}

Object* object_types::load(File& ndf, alni file_adress) {

	alni parent_file_adress = ndf.adress;
	ndf.adress = file_adress;

	ObjectFileHead ofh;
	ndf.read<ObjectFileHead>(&ofh);

	const ObjectType* load_type = NDO.types.Get(ofh.type_name);
	Object* out = load_type->load(ndf);

	ndf.adress = parent_file_adress;

	return out;
}

void object_types::save(Object* in) {
	for (ObjectMemHead* memh_iter = bottom; memh_iter; memh_iter = memh_iter->up) {
		memh_iter->flags = 0;
	}

	File ndf("save.nd", FileOpenFlags::SAVE);

	save(ndf, in);
}

Object* object_types::load() {
	File ndf("save.nd", FileOpenFlags::LOAD);

	return load(ndf, 0);
}

void object_types::push(Object* in) {
	callstak.push(in);
}

Object* object_types::call(Object* self, ndo_static_method meth) {
	
	callstak.call();
	Object* out = meth.method(self, callstak.get_arg(0));
	callstak.ret();
	return out;
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