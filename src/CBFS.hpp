/**
 * @author Jakub Kučera <kucerj56@fit.cvut.cz>
 * @date 10.05.2020
 */

#ifndef TOWERDEFENCE_CBFS_HPP
#define TOWERDEFENCE_CBFS_HPP


#include "CPathFinder.hpp"

/// Class that takes care of path finding using BFS algorithm.
class CBFS : public CPathFinder {
private:
    /// Recursively goes through every part of a map and assigns lowest distance from the start.
    /// @param distance Distance from the start.
    /// @param coords Current coordinates.
    /// @return True if coords are at an empty area.
    virtual bool createDistancesRec(int distance, const CCoordinates &coords) override;

public:
    CBFS() = default;

    virtual void createDistances() override;
};


#endif //TOWERDEFENCE_CBFS_HPP
