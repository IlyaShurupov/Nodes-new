#pragma once

#include "primitives/classobject.h"

#include "operator.h"
#include "requester.h"
#include "gui.h"
#include "tui.h"
#include "widget.h"

namespace nd {
	struct NodesCore : obj::ClassObject {
		static obj::ObjectType TypeData;
		static void constructor(NodesCore* slef);
		static void destructor(NodesCore* slef);

		void run();
	};
};