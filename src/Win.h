#ifndef ULTRA_GAMBLER_WIN_H
#define ULTRA_GAMBLER_WIN_H
#include <vector>

#include "Column.h"


class Win {
public:
    std::vector<std::pair<int, int>> chainCoordinates;
    int chainSymbol;
    std::size_t chainLength;

    std::size_t points;

    // returns coordinates of points from the longest chain
    void CalculateBiggestChain(const std::vector<Column*>& columns);

    void CalculatePoints();
};


#endif //ULTRA_GAMBLER_WIN_H