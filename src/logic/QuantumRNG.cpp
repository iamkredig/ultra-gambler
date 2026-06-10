#include "QuantumRNG.h"

int QuantumRNG::fetchRandomNumberFromPython() {
    PyErr_Clear(); // clear .py errors from previous run

    PyObject* pythonScript = PyImport_ImportModule("quantum_rng");
    if (!pythonScript) {
        PyErr_Print();  // print error
        return -1;
    }

    PyObject* randomNumber = PyObject_CallMethod(pythonScript, "get_quantum_numbers", nullptr);

    int castedQuantumRandomNumber = -1;

    if (randomNumber && PyLong_Check(randomNumber)) {
        castedQuantumRandomNumber = static_cast<int>(PyLong_AsLong(randomNumber));
    }

    Py_XDECREF(randomNumber);
    Py_DECREF(pythonScript);

    return castedQuantumRandomNumber;
}