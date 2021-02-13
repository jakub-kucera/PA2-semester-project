/**
 * @author Jakub Kučera <kucerj56@fit.cvut.cz>
 * @date 30.04.2020
 */

#include "CTower.hpp"
#include "CEffectBasic.hpp"
#include "CEffectFire.hpp"
#include "CEffectBleed.hpp"
#include "CEffectPoison.hpp"

#include <utility>


CTower::CTower(char mapSymbol, uint8_t symbolColorCode, std::string name, const std::string &type,
               const std::string &attackZone, int cost, int damage, int range, int id, int attackSpeed)
        : CArea(mapSymbol, symbolColorCode, false, type,
                damage, range, attackSpeed, id, 0), m_Name(std::move(name)), m_Cost(cost) {
    bool attackAll = attackZone == "all";
    m_Effect = getEffect(type, damage, range, m_ID, attackAll);
}


std::ostream &operator<<(std::ostream &out, const CTower &self) {
    out << self.printArea(true) << " " << self.m_Name << "; "
        << self.m_Type << "; " << self.m_Cost
        << "$; Damage " << self.m_Damage
        << "; Targets " << ((self.m_Effect->attack().second == 1) ? "all enemies" : "one enemy")
        << "; Range " << self.m_Range
        << "; Speed " << self.m_AttackSpeed << '\n';
    return out;
    /*out << "Icon:" << self.printArea(true) << std::endl
        << "Char:" << self.m_Character << std::endl
        << "Color:" << (int)self.m_ColorCode << std::endl
        << "Name: " << self.m_Name << std::endl
        << "Type: " << self.m_Type << std::endl
        << "Cost: " << self.m_Cost << std::endl
        << "Damage: " << self.m_Damage << std::endl
        << "Range: " << self.m_Range << std::endl
        << "AttackSpeed: " << self.m_AttackSpeed << std::endl;
    return out;*/
}

int CTower::getCost() const {
    return m_Cost;
}

std::pair<bool, bool> CTower::attack(CArea *enemy) {
    std::pair<int, bool> attack = m_Effect->attack();
    enemy->m_Health -= attack.first;
    return {enemy->m_Health <= 0, attack.second};
}

bool CTower::attackChargeIncrement() {
    if (++m_AttackCharge >= m_AttackSpeed) {
        m_AttackCharge = 0;
        return true;
    }
    return false;
}


