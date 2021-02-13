/**
 * @author Jakub Kučera <kucerj56@fit.cvut.cz>
 * @date 10.05.2020
 */

#include "CBFS.hpp"

bool CBFS::createDistancesRec(int /*distance*/, const CCoordinates &/* coords*/) {
    return false;
}

void CBFS::createDistances() {
    m_ToVisit.push(CCoordinates(1, m_IntNavigateMap.size() / 2));
    m_IntNavigateMap[m_ToVisit.front().m_Y][m_ToVisit.front().m_X] = 1;
    while (!m_ToVisit.empty()) {
        CCoordinates coords = m_ToVisit.front();
        int distance = m_IntNavigateMap[coords.m_Y][coords.m_X];
        m_ToVisit.pop();
        distance++;

        addToQueue(CCoordinates(coords.m_X + 1, coords.m_Y), distance);
        addToQueue(CCoordinates(coords.m_X, coords.m_Y + 1), distance);
        addToQueue(CCoordinates(coords.m_X, coords.m_Y - 1), distance);
        addToQueue(CCoordinates(coords.m_X - 1, coords.m_Y), distance);
    }
}