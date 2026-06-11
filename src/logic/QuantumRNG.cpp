#include "QuantumRNG.h"

int QuantumRNG::fetchRandomNumberFromPython(bool luckyMode) {
    PyErr_Clear();
    PyObject* pythonScript = PyImport_ImportModule("quantum_rng");
    if (!pythonScript) { PyErr_Print(); return -1; }

    const char* functionName = luckyMode ? "get_quantum_numbers_lucky" : "get_quantum_numbers";

    PyObject* randomNumber = PyObject_CallMethod(pythonScript, functionName, nullptr);

    int result = -1;
    if (randomNumber && PyLong_Check(randomNumber))
        result = static_cast<int>(PyLong_AsLong(randomNumber));

    Py_XDECREF(randomNumber);
    Py_DECREF(pythonScript);
    return result;
}