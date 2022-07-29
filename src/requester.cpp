
#include "requester.h"

#include "operator.h"

using namespace nd;
using namespace tp;
using namespace obj;

tp::Array<Object*> childs_retrival(nd::Requester* self) {
	tp::Array<Object*> out;
	out.pushBack(self->callbacks_arguments);
	return out;
}

alni allocated_size(nd::Requester* self) {
	return sizeof(DictObject*) + sizeof(DictObject*) + sizeof(Operator*);
}

alni allocated_size_recursive(nd::Requester* self) {
	alni out = allocated_size(self);
	out += NDO->objsize_ram_recursive_util(self->callbacks_arguments, self->callbacks_arguments->type);
	return out;
}

struct obj::ObjectType nd::Requester::TypeData = {
	.base = NULL,
	.constructor = (obj::object_constructor) Requester::constructor,
	.destructor = (obj::object_destructor) Requester::destructor,
	.copy = (obj::object_copy) Requester::copy,
	.size = sizeof(Requester),
	.name = "Requester",
	.save_size = (obj::object_save_size) Requester::save_size,
	.save = (obj::object_save) Requester::save,
	.load = (obj::object_load) Requester::load,
	.childs_retrival = (obj::object_debug_all_childs_retrival) childs_retrival,
	.allocated_size = (obj::object_allocated_size) allocated_size,
	.allocated_size_recursive = (obj::object_allocated_size_recursive) allocated_size_recursive
};

void Requester::constructor(Requester* self) {
	self->op = NULL;
	self->op_type = NULL;
	self->callbacks_arguments = (obj::DictObject*) NDO->create("dict");
}

void Requester::destructor(Requester* self) {
	NDO->destroy(self->callbacks_arguments);
}

void Requester::copy(Requester* self, Requester* blueprint) {
	NDO->copy(self->callbacks_arguments, blueprint->callbacks_arguments);
	self->op = blueprint->op;
	self->op_type = blueprint->op_type;
}

alni Requester::save_size(Requester* self) {
	alni out = sizeof(bool); // initialization state
	if (self->op_type) {

		out += sizeof(alni); // dict object adress
		out += tp::string(self->op_type->name).save_size(); // type name

		// find name of the operator
		for (auto op : *((nd::TypeOperators*) self->op_type->nodes_custom_data)->getDict()) {
			if (op->val == self->op) {
				out += op->key.save_size(); // op name
				break;
			}
		}
	}
	return out;
}

void Requester::save(Requester* self, File& file_self) {
	bool is_initialized = self->op_type != NULL;
	file_self.write<bool>(&is_initialized);

	if (!is_initialized) {
		return;
	}

	// save dictobject
	alni ndo_object_adress = NDO->save(file_self, self->callbacks_arguments);
	file_self.write<alni>(&ndo_object_adress);

	// save type name
	tp::string(self->op_type->name).save(&file_self);

	// save op name
	for (auto op : *((nd::TypeOperators*) self->op_type->nodes_custom_data)->getDict()) {
		if (op->val == self->op) {
			op->key.save(&file_self);
			break;
		}
	}
}

void Requester::load(File& file_self, Requester* self) {

	bool is_initialized;
	file_self.read<bool>(&is_initialized);

	if (!is_initialized) {
		return;
	}

	alni ndo_object_adress;
	file_self.read<alni>(&ndo_object_adress);
	self->callbacks_arguments = NDO_CAST(DictObject, NDO->load(file_self, ndo_object_adress));

	tp::string name;
	name.load(&file_self);
	self->op_type = NDO->types.get(name);
	name.load(&file_self);
	self->op = ((nd::TypeOperators*) self->op_type->nodes_custom_data)->getDict()->get(name);
}

void nd::Requester::setTargetOperator(const obj::ObjectType* type, nd::Operator* op) {

	if (this->op_type != type) {
		assert(type && type->nodes_custom_data);

		if (op_type) {
			setNoOperator();
		}

		this->op = op;
		this->op->setupCallbacksArgs(callbacks_arguments);
		this->op_type = type;
	}
}

void nd::Requester::setNoOperator() {
	if (op_type) {
		obj::NDO->destroy(callbacks_arguments);
		callbacks_arguments = (obj::DictObject*) NDO->create("dict");

		op_type = NULL;
		op = NULL;
	}
}

void nd::Requester::call(tp::string name) {
	assert(op_type && op && callbacks_arguments);
	auto idx = callbacks_arguments->items.presents(name);
	assert(idx);
	obj::DictObject* args = NDO_CAST(obj::DictObject, callbacks_arguments->items.getSlotVal(idx));
	assert(args);
	op->call(name, args);
}
