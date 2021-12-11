
#include "PyInterp.h"

#include "PyNdo.h"

#include "object.h"

PyObject* pModule;

PyObject* ndo_create_object(PyObject* self, PyObject* args) {

	const char* arg;
	if (!PyArg_ParseTuple(args, "s", &arg)) {
		return NULL;
	}

	string name = arg;

	Object* out = NDO.create(name);
	
	if (!out) {
		return NULL;
	}

	return PyEmbObject_New(out);
}

static PyMethodDef ModuleMethods[] = { 
	{"ndo_create", ndo_create_object, METH_VARARGS, "creates nd object of type"},
	{NULL, NULL} 
};

static struct PyModuleDef cModPyDem = { PyModuleDef_HEAD_INIT, "npy", "", -1, ModuleMethods };

PyMODINIT_FUNC PyInit_cModPyDem() {
	return PyModule_Create(&cModPyDem);
}

PyInterp::PyInterp() {

	const char* stdOutErr =
"import sys\n\
import npy\n\
class CatchOut:\n\
	def __init__(self):\n\
			self.value = ''\n\
	def write(self, txt):\n\
			self.value += txt\n\
	def clear(self):\n\
			self.value = ''\n\
catchOut = CatchOut()\n\
sys.stdout = catchOut\n\
sys.stderr = catchOut\n\
";

	Py_SetPythonHome(L"D:/dev/src/python310");

	PyImport_AppendInittab("npy", PyInit_cModPyDem);

	Py_Initialize();

	pModule = PyImport_AddModule("__main__");

	PyRun_SimpleString(stdOutErr);
}

void PyInterp::exec(string cmd, Object* self, MethodObjectArgument* args) {

	PyDict_SetItemString(PyModule_GetDict(pModule), "ndo_self", PyEmbObject_New(self));

	PyRun_SimpleString(cmd.str);

	PyObject* catcher = PyObject_GetAttrString(pModule, "catchOut");
	PyObject* output = PyObject_GetAttrString(catcher, "value");

	string out = PyUnicode_AsUTF8(output);

	NdLog_write(out.str); 

	PyObject* function = PyObject_GetAttrString(catcher, "clear");
	PyObject_CallObject(function, NULL);
}

PyInterp::~PyInterp() {
	Py_Finalize();
}
