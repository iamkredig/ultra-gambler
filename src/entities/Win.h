#ifndef ULTRA_GAMBLER_WIN_H
#define ULTRA_GAMBLER_WIN_H

#pragma once
#include <vector>
#include "Column.h"


class Win {
public:
    std::vector<std::pair<int, int> > chainCoordinates;
    int chainSymbol;
    std::size_t chainLength;

    std::size_t coins;

    // returns coordinates of points from the longest chain
    void CalculateBiggestChain(const std::vector<Column *> &columns);

    size_t CalculateCoins(size_t bet);
};


#endif //ULTRA_GAMBLER_WIN_H
