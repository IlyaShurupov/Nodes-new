
#pragma once

#include "primitives/primitives.h"

namespace nd {

	class OpCallBack {

		friend class Operator;

		protected:
		tp::HashMap<obj::Object*, tp::string> arg_interface;
		void (*func)(class Operator* op, obj::DictObject* args) = NULL;
		const char* description = NULL;

		public:

		template <typename Type>
		static Type* getArg(obj::DictObject* args, tp::string id) {
			auto idx = args->items.presents(id);
			assert(bool(idx) && "invalid member access - not presents");
			Type* out = ((Type*) obj::ndo_cast(args->items.getSlotVal(idx), &Type::TypeData));
			assert(out && "invalid member access - type differs");
			return out;
		}

		void call(Operator* op, obj::DictObject* args);
		OpCallBack();
		const char* getDescription() { return description; }
		tp::HashMap<obj::Object*, tp::string>* getInterface() { return &arg_interface; }
	};

	class Operator {
		public:
		typedef tp::HashMap<OpCallBack*, tp::string> CallbacksDict;

		protected:
		CallbacksDict callbacks;
		tp::string description;

		public:
		Operator();
		void call(tp::string name, obj::DictObject* args);
		void setupCallbacksArgs(obj::DictObject* out);
		CallbacksDict* getCallbacks() { return &callbacks; }
		tp::string getDescription() { return description; }
	};


	class TypeOperators {
		public:
		typedef tp::HashMap<Operator*, tp::string> OperatorsDict;

		protected:
		OperatorsDict operators;

		public:
		TypeOperators() {}
		OperatorsDict* getDict() { return &operators; }
	};

	void operators_initialize();
	void operators_finalize();
};
