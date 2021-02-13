/**
 * @author Jakub Kučera <kucerj56@fit.cvut.cz>
 * @date 30.04.2020
 */



#include "CArea.hpp"
#include "CEffectFire.hpp"
#include "CEffectPoison.hpp"
#include "CEffectBleed.hpp"
#include "CEffectBasic.hpp"
#include <utility>

CArea::CArea()
        : m_Character(' '), m_ColorCode(40), m_CanPlaceTower(true), m_Damage(0), m_Range(0), m_AttackSpeed(0), m_ID(0),
          m_AttackCharge(0), m_Health(0) {
    m_ColorStart = "\033[1;" + std::to_string(m_ColorCode) + "m";
    m_ColorEnd = "\033[0m";
}

CArea::CArea(char mCharacter, uint8_t mColorCode, bool mCanPlaceTower, std::string mType,
             const int mDamage, const int mRange, const int mAttackSpeed, const int mId, int mHealth)
             : m_Character(mCharacter), m_ColorCode(mColorCode), m_CanPlaceTower(mCanPlaceTower),
             m_Type(std::move(mType)), m_Damage(mDamage), m_Range(mRange), m_AttackSpeed(mAttackSpeed),
             m_ID(mId), m_AttackCharge(0), m_Health(mHealth) {
    m_ColorStart = "\033[1;" + std::to_string(m_ColorCode) + "m";
    m_ColorEnd = "\033[0m";
}

std::string CArea::printArea() const {
    return std::string(1, m_Character);
}

std::string CArea::printArea(bool useColor) const {
    if (!useColor)
        return printArea();
    return m_ColorStart + m_Character + m_ColorEnd;
}

char CArea::getEffectCode(const std::string &effectName) {
    auto it = EFFECT_TABLE.find(effectName);
    if (it == EFFECT_TABLE.end()) {
        throw std::out_of_range("Effect not found");
    }
    return it->second;
}

std::shared_ptr<CEffect> CArea::getEffect(const std::string &effectName, int damage, int range, int towerId, bool attackAll) {
    switch (getEffectCode(effectName)) {
        case 'f':
            return std::make_shared<CEffectFire>(damage, range, towerId, attackAll);
        case 'p':
            return std::make_shared<CEffectPoison>(damage, range, towerId, attackAll);
        case 'l':
            return std::make_shared<CEffectBleed>(damage, range, towerId, attackAll);
        default:
            return std::make_shared<CEffectBasic>(damage, range, towerId, attackAll);
    }
}

bool CArea::canPlaceTower() {
    return m_CanPlaceTower;
}

std::pair<bool, bool> CArea::attack(CArea * /* enemy*/) {
    return {false, false};
}

bool CArea::damageByEffect() {
    return false;
}

bool CArea::attackChargeIncrement() {
    return false;
}

int CArea::getId() const {
    return m_ID;
}