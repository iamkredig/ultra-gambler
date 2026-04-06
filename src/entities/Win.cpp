#include "../Win.h"

#include <iostream>

int symbolsGrid[5][3];

void FindChain(const int column, const int row, const int symbol, bool visited[5][3], int& currentChainSize, std::vector<std::pair<int, int>>& currentChain) {
    // do not search neighbors when out of bonds or already visited
    if (column < 0 || column >= 5 || row < 0 || row >= 3) return;
    if (visited[column][row] || symbolsGrid[column][row] != symbol) return;

    // mark column visited
    visited[column][row] = true;
    currentChain.emplace_back(column, row);
    currentChainSize++;

    // check neighbors top, bottom, left, right
    FindChain(column + 1, row, symbol, visited, currentChainSize,currentChain);
    FindChain(column - 1, row, symbol, visited, currentChainSize,currentChain);
    FindChain(column, row + 1, symbol, visited, currentChainSize,currentChain);
    FindChain(column, row - 1, symbol, visited, currentChainSize,currentChain);
    // check diag. neighbors
    FindChain(column + 1, row + 1, symbol, visited, currentChainSize,currentChain);
    FindChain(column - 1, row + 1, symbol, visited, currentChainSize,currentChain);
    FindChain(column - 1, row - 1, symbol, visited, currentChainSize,currentChain);
    FindChain(column +1, row - 1, symbol, visited, currentChainSize,currentChain);
}

void fillGridWithSymbols(const std::vector<Column *> &columns) {
    for (int column = 0; column < 5; column++) { // grid with all column symbols
        for (int row = 0; row < 3; row++) {
            symbolsGrid[column][row] = columns[column]->symbols[row + 1];
        }
    }

    for (int row = 0; row < 3; row++) { //debugging print
        for (const auto & column : symbolsGrid) {
            std::cout << column[row] << " ";
        }
        std::cout << std::endl;
    }
}

void Win::CalculateBiggestChain(const std::vector<Column*>& columns) {
    std::vector<std::pair<int, int>> bestChain;
    int bestSymbol = -1;

    fillGridWithSymbols(columns);

    bool symbolVisited[5][3] = { false };

    for (int column = 0; column < 5; column++) {
        for (int row = 0; row < 3; row++) {
            int currentChainSize = 0;
            int currentSymbol = symbolsGrid[column][row];
            std::vector<std::pair<int, int>> currentChain;

            // search symbol chain of current element
            FindChain(column, row, currentSymbol, symbolVisited, currentChainSize, currentChain);

            if (currentChainSize >= 4 && currentChain.size() > bestChain.size()) {
                std::cout << "new best chain with symbol: " << currentSymbol
                << " length: " << currentChainSize << std::endl;
                bestChain = currentChain;
                bestSymbol = currentSymbol;
            }
        }
    }
    chainCoordinates = bestChain;
    chainSymbol = bestSymbol;
    chainLength = bestChain.size();

}

size_t Win::CalculateCoins(size_t bet) {
    if (chainLength > 2) {
        coins = bet * (chainLength - 3);
    } else {
        coins = 0;
    }

    return coins;
}