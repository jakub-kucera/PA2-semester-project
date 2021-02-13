/**
 * @author Jakub Kučera <kucerj56@fit.cvut.cz>
 * @date 25.05.2020
 */

#ifndef TOWERDEFENCE_CEFFECTBLEED_HPP
#define TOWERDEFENCE_CEFFECTBLEED_HPP


#include "CEffect.hpp"

/// Bleeding effect. Gives damage once during attack and once transferred to an enemy it keeps damaging by the same amount of damage.
class CEffectBleed : public CEffect {
public:
    CEffectBleed(int damage, int range, int towerId, bool zoneAll)
            : CEffect(damage, range, 0, 0, 0, towerId, zoneAll) {}

    /// Damages enemies by applied effects.
    /// @return Effect damage
    virtual int damageByEffect() override {
        return m_BaseDamage;
    }

    /// Applies effect while tower is attacking an enemy.
    /// @return Pointer to applied effect.
    virtual std::shared_ptr<CEffect> getEffect() const override {
        return std::make_shared<CEffectBleed>(m_BaseDamage, m_Range, m_TowerId, m_ZoneAll);
    };
};


#endif //TOWERDEFENCE_CEFFECTBLEED_HPP
