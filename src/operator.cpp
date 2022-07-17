
#include "operator.h"

#include "nodes.h"

using namespace nd;
using namespace tp;
using namespace obj;

void nd::OpCallBack::call(Operator* op, obj::DictObject* args) {
	assert(func);
	func(op, args);
}

OpCallBack::OpCallBack() {}

Operator::Operator() {}

void nd::Operator::call(tp::string name, obj::DictObject* args) {
	auto idx = callbacks.presents(name);
	assert(idx);
	OpCallBack* cb = callbacks.getSlotVal(idx);
	cb->call(this, args);
}

void nd::Operator::setupCallbacksArgs(obj::DictObject* out) {
	for (auto cb : callbacks) {
		obj::DictObject* cb_args = (obj::DictObject*) NDO->create("dict");
		out->items.put(cb->key, cb_args);

		for (auto interface_argument : cb->val->arg_interface) {
			cb_args->items.put(interface_argument->key, NDO->instatiate(interface_argument->val));
		}
	}
}

// -----------------------  Operators ------------------------------- //

class NodesCoreTypeOperators : public TypeOperators {

	struct Sleep : public Operator {
		struct Invoke : public OpCallBack {
			Invoke() {
				arg_interface.put("duration", NDO->create("int"));
				func = exec;
				description = "invoke";
			}
			static void exec(Operator* op, obj::DictObject* args) {
				tp::sleep(OpCallBack::getArg<IntObject>(args, "duration")->val);
			}
		} invoke_callback;

		Sleep() {
			description = "Freezes the programm for the given duration";
			callbacks.put("invoke", &invoke_callback);
		}
	} sleep_op;

	struct Terminate : public Operator {
		struct Invoke : public OpCallBack {
			Invoke() {
				arg_interface.put("exit code", obj::NDO->create("int"));
				func = exec;
				description = "invoker";
			}
			static void exec(Operator* op, obj::DictObject* args) {
				tp::terminate(OpCallBack::getArg<obj::IntObject>(args, "exit code")->val);
			}
		} invoke_callback;

		Terminate() {
			description = "Immediately terminates the programm";
			callbacks.put("invoke", &invoke_callback);
		}
	} terminate_op;

	public: 

	NodesCoreTypeOperators() {
		operators.put("sleep", &sleep_op);
		operators.put("terminate", &terminate_op);
	}

};

// -----------------------  Operators Initialization ------------------------------- //

void nd::operators_initialize() {
	nd::NodesCore::TypeData.nodes_custom_data = new NodesCoreTypeOperators();
}

void nd::operators_finalize() {
	delete (NodesCoreTypeOperators*)nd::NodesCore::TypeData.nodes_custom_data;
}