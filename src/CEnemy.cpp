/**
 * @author Jakub Kučera <kucerj56@fit.cvut.cz>
 * @date 30.04.2020
 */

#include "CEnemy.hpp"


CEnemy::CEnemy(char mapSymbol, uint8_t symbolColorCode, std::string /*name*/, const std::string &type, int id, int health)
         //CArea(mapSymbol, symbolColorCode, type, id, health), m_Name(std::move(name)),
         : CArea(mapSymbol, symbolColorCode, false, type, 0, 0, 0, id, health){}

bool CEnemy::damageByEffect() {
    if (m_Effect != nullptr) {
        m_Health -= m_Effect->damageByEffect();
        return m_Health <= 0;
    }
    return false;
}

std::ostream &operator<<(std::ostream &out, const CEnemy &self) {
    out << "Icon:" << self.printArea(true) << std::endl
        << "Char:" << self.m_Character << std::endl
        << "Color:" << (int) self.m_ColorCode << std::endl
        << "Name: " << self.m_Name << std::endl
        << "Type: " << self.m_Type << std::endl
        << "Health: " << self.m_Health << std::endl;
    return out;
}

