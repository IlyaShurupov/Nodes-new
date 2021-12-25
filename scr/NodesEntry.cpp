
#include "Nodes.h"

#include "nullobject.h"
#include "intobject.h"
#include "dictobject.h"
#include "classobject.h"
#include "methodobject.h"
#include "listobject.h"

int main() {

	NDO.define(&NullObjectType);
	NDO.define(&IntObjectType);
	NDO.define(&ListObjectType);
 	NDO.define(&MethodObjectType);
	NDO.define(&DictObjectType);
	NDO.define(&ClassObjectType);

	NodesCoreClass::TypeInit();
	UIClass::TypeInit();

	NDO_CAST(ClassObject, NDO.create("NodesCore"))->call("run");
}