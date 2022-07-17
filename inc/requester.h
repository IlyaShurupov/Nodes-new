
#pragma once

#include "primitives/primitives.h"

namespace nd {

	struct Requester : public obj::Object {
		static struct obj::ObjectType TypeData;

		class Operator* op;
		const obj::ObjectType* op_type;
		obj::DictObject* callbacks_arguments;

		static void constructor(Requester* self);
		static void destructor(Requester* self);
		static void copy(Requester* self, Requester* blueprint);
		static tp::alni save_size(Requester* self);
		static void save(Requester* self, tp::File& file_self);
		static void load(tp::File& file_self, Requester* self);

		void setTargetOperator(const obj::ObjectType* type, nd::Operator* op);
		void setNoOperator();

		// change later to thread requester
		void call(tp::string name);
	};
};
