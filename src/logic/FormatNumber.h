#ifndef ULTRA_GAMBLER_FORMATNUMBER_H
#define ULTRA_GAMBLER_FORMATNUMBER_H

#pragma once
#include <string>
#include <sstream>
#include <iomanip>

inline std::string FormatNumber(size_t value) {

    std::ostringstream oss;

    if (value >= 1'000'000'000'000) {
        double newValue = value / 1'000'000'000'000.0;
        oss << std::fixed << std::setprecision(newValue < 10 ? 2 : 1) << newValue << "T";
    }
    else if (value >= 1'000'000'000) {
        double newValue = value / 1'000'000'000.0;
        oss << std::fixed << std::setprecision(newValue < 10 ? 2 : 1) << newValue << "B";
    }
    else if (value >= 1'000'000) {
        double newValue = value / 1'000'000.0;
        oss << std::fixed << std::setprecision(newValue < 10 ? 2 : 1) << newValue << "M";
    }
    else if (value >= 1'000) {
        double newValue = value / 1'000.0;
        oss << std::fixed << std::setprecision(newValue < 10 ? 1 : 0) << newValue << "K";
    }
    else {
        oss << value;
    }

    return oss.str();
}

#endif