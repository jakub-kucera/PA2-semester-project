/**
 * @author Jakub Kučera <kucerj56@fit.cvut.cz>
 * @date 10.05.2020
 */

#include "CDFS.hpp"

bool CDFS::createDistancesRec(int distance, const CCoordinates &coords) {
    if (!checkCoords(coords, distance)) {
        return false;
    }

    distance++;
    m_IntNavigateMap[coords.m_Y][coords.m_X] = distance;
    createDistancesRec(distance, CCoordinates(coords.m_X + 1, coords.m_Y));
    createDistancesRec(distance, CCoordinates(coords.m_X - 1, coords.m_Y));
    createDistancesRec(distance, CCoordinates(coords.m_X, coords.m_Y + 1));
    createDistancesRec(distance, CCoordinates(coords.m_X, coords.m_Y - 1));
    return true;
}

void CDFS::createDistances() {
    createDistancesRec(0, CCoordinates(1, m_IntNavigateMap.size() / 2));
}