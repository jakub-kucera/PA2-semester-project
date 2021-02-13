/**
 * @author Jakub Kučera <kucerj56@fit.cvut.cz>
 * @date 10.05.2020
 */

#ifndef TOWERDEFENCE_CDFS_HPP
#define TOWERDEFENCE_CDFS_HPP


#include "CPathFinder.hpp"

/// Class that takes care of path finding using DFS algorithm.
class CDFS : public CPathFinder {
private:
    /// Recursively goes through every part of a map and assigns lowest distance from the start.
    /// @param distance Distance from the start.
    /// @param coords Current coordinates.
    /// @return True if coords are at an empty area.
    virtual bool createDistancesRec(int distance, const CCoordinates &coords) override;
public:
    CDFS() = default;

    /// Creates distances from the starting point for all unnocupied areas.
    virtual void createDistances() override;
};


#endif //TOWERDEFENCE_CDFS_HPP
