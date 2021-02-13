/**
 * @author Jakub Kučera <kucerj56@fit.cvut.cz>
 * @date 28.05.2020
 */

#include <algorithm>
#include <map>
#include "CPathFinder.hpp"
#include "CCoordinates.hpp"

bool CPathFinder::createPathRec(int distance, const CCoordinates &coords) {
    if (m_IntNavigateMap[coords.m_Y][coords.m_X] < distance) {
        m_Path.push_back(coords);

        return m_IntNavigateMap[coords.m_Y][coords.m_X] == 1
               || createPathRec(m_IntNavigateMap[coords.m_Y][coords.m_X], CCoordinates(coords.m_X - 1, coords.m_Y))
               || createPathRec(m_IntNavigateMap[coords.m_Y][coords.m_X], CCoordinates(coords.m_X + 1, coords.m_Y))
               || createPathRec(m_IntNavigateMap[coords.m_Y][coords.m_X], CCoordinates(coords.m_X, coords.m_Y - 1))
               || createPathRec(m_IntNavigateMap[coords.m_Y][coords.m_X], CCoordinates(coords.m_X, coords.m_Y + 1));
    }
    return false;
}

std::pair<std::vector<CCoordinates>, int>
CPathFinder::createPathWallRec(int distance, const CCoordinates &coords, int nextToWalls,
                               std::vector<CCoordinates> path) {
    if (m_IntNavigateMap[coords.m_Y][coords.m_X] < distance && checkCoords(coords, 0)) {
        //increases count if next to walls.
        if (m_IntNavigateMap[coords.m_Y + 1][coords.m_X] == MAP_OBSTACLE) {
            ++nextToWalls;
        }
        if (m_IntNavigateMap[coords.m_Y - 1][coords.m_X] == MAP_OBSTACLE) {
            ++nextToWalls;
        }
        if (m_IntNavigateMap[coords.m_Y][coords.m_X + 1] == MAP_OBSTACLE) {
            ++nextToWalls;
        }
        if (m_IntNavigateMap[coords.m_Y][coords.m_X - 1] == MAP_OBSTACLE) {
            ++nextToWalls;
        }
        path.push_back(coords);

        //Recursively calls the same method and stores one of the results based on m_GoNearWalls.
        if (m_IntNavigateMap[coords.m_Y][coords.m_X] != 1) {
            std::pair<std::vector<CCoordinates>, int> path1 =
                    createPathWallRec(m_IntNavigateMap[coords.m_Y][coords.m_X],
                                      CCoordinates(coords.m_X - 1, coords.m_Y), nextToWalls, path);
            std::pair<std::vector<CCoordinates>, int> path2 =
                    createPathWallRec(m_IntNavigateMap[coords.m_Y][coords.m_X],
                                      CCoordinates(coords.m_X + 1, coords.m_Y), nextToWalls, path);
            std::pair<std::vector<CCoordinates>, int> path3 =
                    createPathWallRec(m_IntNavigateMap[coords.m_Y][coords.m_X],
                                      CCoordinates(coords.m_X, coords.m_Y - 1), nextToWalls, path);
            std::pair<std::vector<CCoordinates>, int> path4 =
                    createPathWallRec(m_IntNavigateMap[coords.m_Y][coords.m_X],
                                      CCoordinates(coords.m_X, coords.m_Y + 1), nextToWalls, path);
            std::map<int, std::vector<CCoordinates>> createdPaths =
                    {{path1.second, path1.first},
                     {path2.second, path2.first},
                     {path3.second, path3.first},
                     {path4.second, path4.first}};
            if (m_GoNearWalls) {
                return {createdPaths.rbegin()->second, createdPaths.rbegin()->first};
            }
            return {createdPaths.begin()->second, createdPaths.begin()->first};
        } else {
        }
        return {path, nextToWalls};
    }
    return {path, m_GoNearWalls ? 0 : MAP_OBSTACLE};
}

bool CPathFinder::checkCoords(CCoordinates coords, int distance) const {
    return (m_IntNavigateMap[coords.m_Y][coords.m_X] == 0
            || m_IntNavigateMap[coords.m_Y][coords.m_X] > distance)
           && m_IntNavigateMap[coords.m_Y][coords.m_X] != MAP_OBSTACLE
           && coords.m_Y != 0
           && coords.m_X != 0
           && coords.m_Y <= m_IntNavigateMap.size() - 2
           && coords.m_X <= m_IntNavigateMap[0].size() - 2;
}

void CPathFinder::generateMap(const std::vector<std::string> &map) {
    m_IntNavigateMap.resize(map.size());
    int i = 0;
    for (const auto &row : map) {
        for (const auto &character : row) {
            if (character == ' ') {
                m_IntNavigateMap[i].push_back(0);
            } else {
                m_IntNavigateMap[i].push_back(MAP_OBSTACLE);
            }
        }
        ++i;
    }
}

void CPathFinder::createPath(char option) {
    //basic path search
    if (option == 'b') {
        createPathRec(MAP_OBSTACLE, CCoordinates(m_IntNavigateMap[0].size() - 2, m_IntNavigateMap.size() / 2));
    }

    //advanced options for path search. Differs in option to choose to move more near walls, or less. Shortest path is still guaranteed.
    else {
        m_GoNearWalls = option == 'w';
        m_Path = createPathWallRec(MAP_OBSTACLE,
                                   CCoordinates(m_IntNavigateMap[0].size() - 2, m_IntNavigateMap.size() / 2), 0,
                                   {}).first;
    }
    std::reverse(m_Path.begin(), m_Path.end());
}

std::vector<CCoordinates> CPathFinder::getPath() const{
    return m_Path;
}

void CPathFinder::printColorMap() const {
    std::cout << std::endl;
    for (const auto &line : m_IntNavigateMap) {
        for (const auto &character : line) {
            std::cout << " ";
            if (character == MAP_OBSTACLE) {
                std::cout << "\033[24;7m";
            } else {
                std::cout << "\033[1;" << 41 + character % 8 << "m";
            }
            if (character < 10) {
                std::cout << "0";
            }
            if (character < 100) {
                std::cout << "0";
            }
            std::cout << character;
            std::cout << "\033[0m";
            std::cout << " ";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

void CPathFinder::addToQueue(CCoordinates coords, int distance) {
    if (checkCoords(coords, distance)) {
        m_IntNavigateMap[coords.m_Y][coords.m_X] = distance;
        m_ToVisit.push(coords);
    }
}