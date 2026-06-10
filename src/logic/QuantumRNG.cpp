#include "QuantumRNG.h"

int QuantumRNG::fetchRandomNumberFromPython() {
    PyObject* pythonScript = PyImport_ImportModule("quantum_rng");
    if (!pythonScript)
        return -1;

    PyObject* randomNumber =
        PyObject_CallMethod(pythonScript, "get_quantum_numbers", "(i)", 1);

    int castedQuantumRandomNumber = -1;

    if (randomNumber &&
        PyList_Check(randomNumber) &&
        PyList_Size(randomNumber) > 0)
    {
        PyObject* item = PyList_GetItem(randomNumber, 0); // borrowed reference
        castedQuantumRandomNumber = static_cast<int>(PyLong_AsLong(item));
    }

    Py_XDECREF(randomNumber);
    Py_XDECREF(pythonScript);

    return castedQuantumRandomNumber;
}