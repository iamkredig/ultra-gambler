#ifndef ULTRA_GAMBLER_QUANTUMRNG_H
#define ULTRA_GAMBLER_QUANTUMRNG_H

#pragma once
#include <memory>
#include <Python.h>

class QuantumRNG {
public:
    int fetchRandomNumberFromPython();
};

#endif
