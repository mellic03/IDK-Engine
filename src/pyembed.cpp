#include "pyembed.h"



void c2p::vec3(glm::vec3 vec, PyObject *locals)
{
  std::string str_x = "pos_x";
  std::string str_y = "pos_y";
  std::string str_z = "pos_z";

  PyDict_SetItemString(locals, str_x.c_str(), PyFloat_FromDouble(vec.x));
  PyDict_SetItemString(locals, str_y.c_str(), PyFloat_FromDouble(vec.y));
  PyDict_SetItemString(locals, str_z.c_str(), PyFloat_FromDouble(vec.z));
}


glm::vec3 p2c::vec3(PyObject *locals)
{
  std::string str_x = "pos_x";
  std::string str_y = "pos_y";
  std::string str_z = "pos_z";

  float x = PyFloat_AsDouble(PyDict_GetItemString(locals, str_x.c_str()));
  float y = PyFloat_AsDouble(PyDict_GetItemString(locals, str_y.c_str()));
  float z = PyFloat_AsDouble(PyDict_GetItemString(locals, str_z.c_str()));

  return glm::vec3(x, y, z);
}

void pembed(void)
{
  // setenv("PYTHONPATH", ".", 0);
  // PyObject *pName, *pModule, *pDict, *pFunc;
  // PyObject *pArgs, *pValue;
  // int i;


  // Py_Initialize();
  // pName = PyUnicode_FromString("multiply");
  // /* Error checking of pName left out */

  // pModule = PyImport_Import(pName);
  // Py_DECREF(pName);

  // if (pModule != NULL) {
  //     pFunc = PyObject_GetAttrString(pModule, "multiply");
  //     /* pFunc is a new reference */

  //     if (pFunc && PyCallable_Check(pFunc)) {
  //         pArgs = PyTuple_New(2);
  //         for (i = 0; i < 2; ++i) {
  //             pValue = PyLong_FromLong(atoi("5"));
  //             if (!pValue) {
  //                 Py_DECREF(pArgs);
  //                 Py_DECREF(pModule);
  //                 fprintf(stderr, "Cannot convert argument\n");
  //                 return 1;
  //             }
  //             /* pValue reference stolen here: */
  //             PyTuple_SetItem(pArgs, i, pValue);
  //         }
  //         pValue = PyObject_CallObject(pFunc, pArgs);
  //         Py_DECREF(pArgs);
  //         if (pValue != NULL) {
  //             printf("Result of call: %ld\n", PyLong_AsLong(pValue));
  //             Py_DECREF(pValue);
  //         }
  //         else {
  //             Py_DECREF(pFunc);
  //             Py_DECREF(pModule);
  //             PyErr_Print();
  //             fprintf(stderr,"Call failed\n");
  //             return 1;
  //         }
  //     }
  //     else {
  //         if (PyErr_Occurred())
  //             PyErr_Print();
  //         fprintf(stderr, "Cannot find function \"%s\"\n", "multiply");
  //     }
  //     Py_XDECREF(pFunc);
  //     Py_DECREF(pModule);
  // }
  // else {
  //     PyErr_Print();
  //     fprintf(stderr, "Failed to load \"%s\"\n", "multiply");
  //     return 1;
  // }
  // Py_Finalize();
}