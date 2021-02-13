/**
 * @author Jakub Kučera <kucerj56@fit.cvut.cz>
 * @date 07.05.2020
 */

#ifndef TOWERDEFENCE_CCOORDINATES_HPP
#define TOWERDEFENCE_CCOORDINATES_HPP

#include <iostream>

/// Used to store coordinates.
struct CCoordinates {
    /// @var X coordinate.
    unsigned int m_X;
    /// @var Y coordinate.
    unsigned int m_Y;

    CCoordinates(unsigned int x, unsigned int y) : m_X(x), m_Y(y) {}

    friend std::ostream &operator<<(std::ostream &out, const CCoordinates &cords) {
        return out << "X: " << cords.m_X << " Y: " << cords.m_Y << std::endl;
    }

    friend bool operator==(const CCoordinates &leftCords, const CCoordinates &rightCords) {
        return leftCords.m_Y == rightCords.m_Y && leftCords.m_X == rightCords.m_X;
    }
};


#endif //TOWERDEFENCE_CCOORDINATES_HPP
