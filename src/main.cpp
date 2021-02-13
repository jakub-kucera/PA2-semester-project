/**
 * @author Jakub Kučera <kucerj56@fit.cvut.cz>
 * @date 30.04.2020
 */

#include <iostream>
#include "CGame.hpp"

int main() {
    CGame game;
    try {
        game.start();
    }
    catch (const std::exception &e) {
        std::cout << "Error: " << e.what() << std::endl;
    }
    return 0;
}
