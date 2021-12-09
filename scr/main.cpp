
#include "object.h"

#include "intobject.h"
#include "dictobject.h"
#include "classobject.h"

#include <types.h>

struct NodesCoreClass : ClassObject {

	static void copy(Object* self, Object* in) {
		CCASTV(NodesCoreClass, in, self);

	}

	static void destructor(Object* in) {
		CCASTV(NodesCoreClass, in, self);
		
	}

	static void constructor(Object* in) {
		CCASTV(NodesCoreClass, in, self);
		
	}

};

struct ObjectType NodesCoreType = {
	.base = &ClassType,
	.constructor = NodesCoreClass::constructor,
	.destructor = NodesCoreClass::destructor,
	.copy = NodesCoreClass::copy,
	.size = sizeof(NodesCoreClass),
	.name = "NodesCore",
};


int main() {

	NDO.define(&IntType);
	NDO.define(&DictType);
	NDO.define(&ClassType);
	NDO.define(&NodesCoreType);

	Object* nodes = NDO.create("NodesCore");
	
	

	CCASTV(DictObject, NDO.create("dict"), dict);
	dict->items.Put("int1", int1);
	dict->items.Put("int2", int2);

	CCASTV(ClassObject, NDO.create("class"), class1);
	class1->members->items.Put("dictinary", dict);

	NDO.destroy(class1);
}