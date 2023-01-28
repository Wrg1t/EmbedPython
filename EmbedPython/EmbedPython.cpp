#include <Python.h>

int main(int argc, char *argv[])
{
	PyObject * pName{}, * pModule{}, * pDict{}, * pClass{}, * pInstance{}, * pValue{};
	int arg[2]{};

	if (argc < 4) {
		fprintf(stderr, "Usage: call module_name class_name method_name [arguments]\n");
	}

	Py_Initialize();

	pName = PyUnicode_DecodeFSDefault(argv[1]);
	pModule = PyImport_Import(pName);
	Py_DECREF(pName);

	if (pModule != NULL) {
		pDict = PyModule_GetDict(pModule);
		pClass = PyDict_GetItemString(pDict, argv[2]);
	}
	else {
		PyErr_Print();
	}

	if (pClass && PyCallable_Check(pClass)) {
		pInstance = PyObject_CallObject(pClass, NULL);
	}
	else {
		if (PyErr_Occurred())
			PyErr_Print();	
		fprintf(stderr, "Cannot find class \"%s\"\n", argv[2]);
	}

	if (pInstance != NULL) {
		if (argc == 6) {
			for (int i = 0; i < argc - 4; i++) {
				arg[i] = atoi(argv[i + 4]);
			}
			pValue = PyObject_CallMethod(pInstance, argv[3], "(ii)", arg[0], arg[1]);
		}
		else if (argc == 4) {
			pValue = PyObject_CallMethod(pInstance, argv[3], NULL);
		}
		else {
			fprintf(stderr, "Usage: call module_name class_name method_name [arguments]\n");
		}
	}
	else {
		PyErr_Print();
	}

	if (pValue != NULL) {
		printf("Return of call : %d\n", PyLong_AsLong(pValue));
	}
	else {
		PyErr_Print();
	}

	Py_DECREF(pValue);
	Py_DECREF(pInstance);
	Py_DECREF(pClass);
	Py_DECREF(pDict);
	Py_DECREF(pModule);

	Py_Finalize();
	return 0;
}