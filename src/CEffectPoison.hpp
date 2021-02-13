/**
 * @author Jakub Kučera <kucerj56@fit.cvut.cz>
 * @date 25.05.2020
 */

#ifndef TOWERDEFENCE_CEFFECTPOISON_HPP
#define TOWERDEFENCE_CEFFECTPOISON_HPP


#include "CEffect.hpp"

/// Poisonous effect. Gives damage once during attack and once transferred to an enemy it keeps damaging by the same amount for limited period of time.
class CEffectPoison : public CEffect {
public:
    CEffectPoison(int damage, int range, int towerId, bool zoneAll)
            : CEffect(damage, range, 0, 5, 0, towerId, zoneAll) {}

    /// Damages enemies by applied effects. Damage is applied only a limited amount of times.
    /// @return Effect damage
    virtual int damageByEffect() override {
        return m_Duration++ >= m_TotalDuration ? 0 : m_BaseDamage;
    }

    /// Applies effect while tower is attacking an enemy.
    /// @return Pointer to applied effect.
    virtual std::shared_ptr<CEffect> getEffect() const override {
        return std::make_shared<CEffectPoison>(m_BaseDamage, m_Range, m_TowerId, m_ZoneAll);
    };
};


#endif //TOWERDEFENCE_CEFFECTPOISON_HPP
