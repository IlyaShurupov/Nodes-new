#pragma once

#include "primitives/classobject.h"

#include "operator.h"
#include "requester.h"
#include "gui.h"
#include "tui.h"
#include "widget.h"

namespace nd {

	extern bool terminate_flag;
	extern int terminate_code;

	struct NodesCore : obj::ClassObject {
		static obj::ObjectType TypeData;
		static void constructor(NodesCore* slef);
		static void destructor(NodesCore* slef);
		static tp::alni save_size(GUI* self) { return 0; };
		static void save(NodesCore* self, tp::File& file_self);
		static void load(tp::File& file_self, NodesCore* self);

		void run();
	};
};