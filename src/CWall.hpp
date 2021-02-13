/**
 * @author Jakub Kučera <kucerj56@fit.cvut.cz>
 * @date 01.05.2020
 */

#ifndef TOWERDEFENCE_CWALL_HPP
#define TOWERDEFENCE_CWALL_HPP


#include "CArea.hpp"

/// Specifies that area on a map is a wall and cannot be replaced by anything else.
class CWall : public CArea {
public:
    //CWall() : CArea('#', 40){};
    explicit CWall(char c)
            : CArea(c, 40, false, " ", 0, 0, 0, 0, 0) {};
};



#endif //TOWERDEFENCE_CWALL_HPP
