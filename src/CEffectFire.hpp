/**
 * @author Jakub Kučera <kucerj56@fit.cvut.cz>
 * @date 25.05.2020
 */

#ifndef TOWERDEFENCE_CEFFECTFIRE_HPP
#define TOWERDEFENCE_CEFFECTFIRE_HPP


#include "CEffect.hpp"

/// Fire effect. Gives damage once during attack and once transferred to an enemy the damage gets lower and lower until it's zero.
class CEffectFire : public CEffect {//decrease damage until 0
public:
    CEffectFire(int damage, int range, int towerId, bool zoneAll)
            : CEffect(damage, range, damage / 10, 0, 0, towerId, zoneAll) {}

    /// Damages enemies by applied effects. Damage is slowly decreasing until it's zero.
    /// @return Effect damage
    virtual int damageByEffect() override {
        m_BaseDamage -= m_DecreaseDamage;
        return m_BaseDamage <= 0 ? 0 : m_BaseDamage;
    }

    /// Applies effect while tower is attacking an enemy.
    /// @return Pointer to applied effect.
    virtual std::shared_ptr<CEffect> getEffect() const override {
        return std::make_shared<CEffectFire>(m_BaseDamage, m_Range, m_TowerId, m_ZoneAll);
    };
};


#endif //TOWERDEFENCE_CEFFECTFIRE_HPP
