/**
 * @author Jakub Kučera <kucerj56@fit.cvut.cz>
 * @date 10.05.2020
 */

#ifndef TOWERDEFENCE_CPATHFINDER_HPP
#define TOWERDEFENCE_CPATHFINDER_HPP

#include <list>
#include <vector>
#include <string>
#include <queue>
#include <climits>
#include "CCoordinates.hpp"

/// Class that takes care of finding the correct shortest path for enemies.
class CPathFinder {
protected:
    /// @var Number under which obstacles are represented.
    const int MAP_OBSTACLE = INT_MAX;
    /// @var Determines whether we want the path to be as close to walls as possible.
    bool m_GoNearWalls = false;
    /// @var Map that is used to create the path. Stores values of distances from the starting point.
    std::vector<std::vector<int>> m_IntNavigateMap;
    /// @var Generated map.
    std::vector<CCoordinates> m_Path;
    /// @var Queue of coordinates to visit. Is used by some search algorithms.
    std::queue<CCoordinates> m_ToVisit;


    virtual bool createDistancesRec(int distance, const CCoordinates &coords) = 0;

    /// Recursively goes from the destination to start based on distance from start.
    /// @param [in] distance Distance from the start.
    /// @param [ref] coords Current coordinates.
    /// @return True if coords are at the start.
    bool createPathRec(int distance, const CCoordinates &coords);

    /// Recursively goes from the destination to start based on distance from start
    /// and whether the path is supposed to be as close to walls as possible, while still guaranteeing shortest path.
    /// @param [in] distance Distance from the start.
    /// @param [ref] coords Current coordinates.
    /// @return Path.
    /// @return Number of areas next to walls.
    std::pair<std::vector<CCoordinates>, int> createPathWallRec(int distance, const CCoordinates &coords, int nextToWalls, std::vector<CCoordinates> path);

public:
    CPathFinder() = default;

    virtual ~CPathFinder() = default;

    virtual void createDistances() = 0;

    /// Creates path for enemies.
    /// @param option Specifies special options.
    void createPath(char option);

    /// Return path for enemies.
    std::vector<CCoordinates> getPath() const;

    /// Performs a check on given coordinates whether they are inside map and their distance from start is smaller than given distance
    bool checkCoords(CCoordinates coords, int distance) const;

    ///Adds given coordinates to a queue to be processed.
    /// @param coords Coordinates to be put in a queue.
    /// @param distance Distance from the starting point.
    void addToQueue(CCoordinates coords, int distance);

    /// Generates map that is used to create path for enemies.
    /// @param map Original map chosen by user.
    void generateMap(const std::vector<std::string> &map);

    /// Prints map with different colors for different distances from the starting point.
    void printColorMap() const;
};


#endif //TOWERDEFENCE_CPATHFINDER_HPP
