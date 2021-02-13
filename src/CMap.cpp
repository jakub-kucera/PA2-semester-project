/**
 * @author Jakub Kučera <kucerj56@fit.cvut.cz>
 * @date 30.04.2020
 */

#include "CMap.hpp"
#include <utility>

CMap::CMap(std::vector<std::string> map, std::string name, int health, int money)
        : m_Map(std::move(map)), m_Name(std::move(name)), m_MaxHealth(health),
          m_Money(money), m_Height(m_Map.size()), m_Length(m_Map[0].size()) {}

std::ostream &operator<<(std::ostream &out, const CMap &self) {
    for (const auto &line : self.m_Map) {
        out << line << std::endl;
    }
    out << "Name: " << self.m_Name << std::endl
        << "Health: " << self.m_MaxHealth << std::endl
        << "Start Money: " << self.m_Money << std::endl
        << "Height: " << self.m_Height << std::endl
        << "Length: " << self.m_Length << std::endl;
    return out;
}

const std::vector<std::string> &CMap::getMap() const {
    return m_Map;
}

const std::string &CMap::getName() const {
    return m_Name;
}

unsigned int CMap::getHeight() const {
    return m_Height;
}

unsigned int CMap::getLength() const {
    return m_Length;
}

int CMap::getMaxHealth() const {
    return m_MaxHealth;
}

int CMap::getMoney() const {
    return m_Money;
}
