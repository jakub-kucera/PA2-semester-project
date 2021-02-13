/**
 * @author Jakub Kučera <kucerj56@fit.cvut.cz>
 * @date 11.05.2020
 */

#ifndef TOWERDEFENCE_CEFFECT_HPP
#define TOWERDEFENCE_CEFFECT_HPP


#include <string>
#include <utility>

/// Base class from which derived effects inherit from.
class CEffect {
public:
    /// @var Base(starting) damage.
    int m_BaseDamage;
    /// @var Current effects duration.
    int m_Duration;
protected:
    /// @var Attacks range.
    int m_Range;
    /// @var Taking damage from effect decreases effects damage by this amount.
    int m_DecreaseDamage;
    /// @var Total duration of an effect
    int m_TotalDuration;
    /// @var Id of the tower that has this effect been taken from.
    int m_TowerId;
    /// @var Determines whether towers with this effect attack all enemies in proximity, or just one.
    bool m_ZoneAll;
public:
    CEffect(int baseDamage, int range, int decreaseDamage, int totalDuration, int duration, int towerId, bool zoneAll)
            : m_BaseDamage(baseDamage), m_Duration(duration), m_Range(range), m_DecreaseDamage(decreaseDamage),
              m_TotalDuration(totalDuration), m_TowerId(towerId), m_ZoneAll(zoneAll) {}

    virtual ~CEffect() = default;

    /// Return range of a tower attack.
    int getRange() const {
        return m_Range;
    }

    /// Executes attack on an enemy.
    /// @return Attack damage.
    /// @return True if tower attacks on all enemies in proximity.
    virtual std::pair<int, bool> attack() { //called from tower
        return {m_BaseDamage, m_ZoneAll};
    }

    /// Damages enemies by applied effects.
    /// @return Effect damage
    virtual int damageByEffect() {
        return 0;
    }

    /// Applies effect while tower is attacking an enemy.
    /// @return Pointer to applied effect.
    virtual std::shared_ptr<CEffect> getEffect() const {
        return nullptr;
    };

    /// Return id of the tower that has this effect been taken from.
    int getTowerId() const {
        return m_TowerId;
    }
};


#endif //TOWERDEFENCE_CEFFECT_HPP
