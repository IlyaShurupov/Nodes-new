
#pragma once

#include "PyNdo.h"

#include "object.h"
#include "classobject.h"

typedef struct {
	PyObject_HEAD;
	Object* ndo_ptr;
} Py_EmbObj;

static PyObject* Emb_new(PyTypeObject* type, PyObject* args, PyObject* kwargs) {

	Py_EmbObj* out = (Py_EmbObj*)PyObject_Malloc(sizeof(Py_EmbObj));

	out->ob_base.ob_type = type;
	out->ob_base.ob_refcnt = 1;

	out->ndo_ptr = 0;

	return (PyObject*)out;
}

PyObject* Emb_vectorcall(PyObject* callable, PyObject* const* args, size_t nargsf, PyObject* kwnames) {
	return  PyEmbObject_New(0);
}

void Emb_dealloc(Py_EmbObj* self) {
}

PyObject* Emb_repr(PyObject* self) {
	Object* ndo = ((Py_EmbObj*)self)->ndo_ptr;
	return PyUnicode_FromString(ndo->type->name.str);
}

static PyObject* EmbObj_get_child(PyObject* self, PyObject* args) {

	Py_EmbObj* me = ((Py_EmbObj*)self);

	const char* arg;
	if (!PyArg_ParseTuple(args, "s", &arg)) {
		return NULL;
	}

	constring name = arg;

	NDO_CASTV(ClassObject, me->ndo_ptr, ndo_class);

	int child_idx = ndo_class->members->items.Presents(name);

	if (child_idx == -1) {
		return NULL;
	}

	PyObject* out = PyEmbObject_New(ndo_class->members->items.table[child_idx]->val);
	return out;
}

static PyObject* EmbObj_call(PyObject* self, PyObject* args) {

	Py_EmbObj* me = ((Py_EmbObj*)self);

	const char* arg;
	if (!PyArg_ParseTuple(args, "s", &arg)) {
		return NULL;
	}

	constring name = arg;

	NDO_CASTV(ClassObject, me->ndo_ptr, ndo_class);

	Object* ndo_out = ndo_class->call(name, NULL);

	return PyEmbObject_New(ndo_out);
}

static PyMethodDef Emb_methods[] = {
	{"child", EmbObj_get_child, METH_VARARGS | METH_KEYWORDS, "doc get_info"},
	{"call", EmbObj_call, METH_VARARGS | METH_KEYWORDS, "doc get_info"},
	{NULL, NULL},
};

PyTypeObject Py_EmbObjType;

void init_Py_EmbObjType() {
	Py_EmbObjType =
	{

			PyVarObject_HEAD_INIT(&PyType_Type, 0)

			"EmbObj",                                   /* tp_name */
			sizeof(Py_EmbObj),                          /* tp_basicsize */
			0,                                          /* tp_itemsize */
			(destructor)Emb_dealloc,                    /* tp_dealloc */
			0,                                          /* tp_print */
			0,                                          /* tp_getattr */
			0,                                          /* tp_setattr */
			0,                                          /* tp_reserved */
			(reprfunc)Emb_repr,                         /* tp_repr */
			0,                                          /* tp_as_number */
			0,                                          /* tp_as_sequence */
			0,                                          /* tp_as_mapping */
			0,                                          /* tp_hash */
			0,                                          /* tp_call */
			0,                                          /* tp_str */
			0,                                          /* tp_getattro */
			0,                                          /* tp_setattro */
			0,                                          /* tp_as_buffer */
			Py_TPFLAGS_DEFAULT,                         /* tp_flags */
			"emb Obj doc",                              /* tp_doc */
			0,                                          /* tp_traverse */
			0,                                          /* tp_clear */
			0,                                          /* tp_richcompare */
			0,                                          /* tp_weaklistoffset */
			0,                                          /* tp_iter */
			0,                                          /* tp_iternext */
			Emb_methods,                                /* tp_methods */
			0,                                          /* tp_members */
			0,                                          /* tp_getset */
			0,                                          /* tp_base */
			0,                                          /* tp_dict */
			0,                                          /* tp_descr_get */
			0,                                          /* tp_descr_set */
			0,                                          /* tp_dictoffset */
			0,                                          /* tp_init */
			0,                                          /* tp_alloc */
			(newfunc)Emb_new,                           /* tp_new */
			0,                                          /* tp_free */
			0,                                          /* tp_is_gc */
			0,                                          /* tp_bases; */
			0,                                          /* tp_mro; */
			0,                                          /* tp_cache */
			0,                                          /* tp_subclasses */
			0,                                          /* tp_weaklist */
			0,                                          /* tp_del */
			0,                                          /* tp_version_tag */
			0,																					 /* tp_finalize */
			(vectorcallfunc)Emb_vectorcall,             /* vectorcallfunc */
	};
}

PyObject* PyEmbObject_New(Object* self) {

	init_Py_EmbObjType();

	if (PyType_Ready(&Py_EmbObjType) < 0) {
		return NULL;
	}

	Py_EmbObj* out = (Py_EmbObj*)Py_EmbObjType.tp_new(&Py_EmbObjType, NULL, NULL);
	out->ndo_ptr = self;
	return (PyObject*)out;
}