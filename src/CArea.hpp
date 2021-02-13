/**
 * @author Jakub Kučera <kucerj56@fit.cvut.cz>
 * @date 30.04.2020
 */

#ifndef TOWERDEFENCE_CAREA_HPP
#define TOWERDEFENCE_CAREA_HPP


#include <string>
#include <iostream>
#include <memory>
#include <unordered_map>
#include "CEffect.hpp"

/// Represents a single area(/point/place) on a map.
class CArea {
protected:
    /// @var Assigns code values based on effect names.
    const std::unordered_map<std::string, char> EFFECT_TABLE = {{"basic",  'b'},
                                                                {"fire",   'f'},
                                                                {"poison", 'p'},
                                                                {"bleed",  'l'}};
    /// @var Character under which CArea object will be represented by on a map.
    char m_Character;
    /// @var Special sequence of characters, which ensure, that characters writen out afterwards, will be in given format.
    std::string m_ColorStart;
    /// @var Special sequence of characters, reset formatting to normal.
    std::string m_ColorEnd;
    /// @var Code which represents certain color.
    uint8_t m_ColorCode;
    /// @var Value that determines whether user can place tower on this CArea.
    bool m_CanPlaceTower;
    /// @var Type of CArea.
    std::string m_Type;
    /// @var Damage given by an attack.
    const int m_Damage;
    /// @var Attack range.
    const int m_Range;
    /// @var Attack speed - determines how often can be attack executed.
    const int m_AttackSpeed;
    /// @var Id that determines in what order were CArea instances loaded from files.
    const int m_ID;
public:
    /// @var Current attack charge. If it is same as m_AttackSpeed, then attack can be executed.
    int m_AttackCharge;
    /// @var Current health.
    int m_Health;
    /// @var Effect that CArea can either have, or copy to other CArea during an attack.
    std::shared_ptr<CEffect> m_Effect = nullptr;

    CArea();

    /// Constructor with more advanced options.
    CArea(char mCharacter, uint8_t mColorCode, bool mCanPlaceTower, std::string mType,
          int mDamage, int mRange, int mAttackSpeed, int mId, int mHealth);

    virtual ~CArea() = default;

    /// Returns string which will be used to represent CArea on a map.
    std::string printArea() const;
    /// Returns string which will be used to represent CArea on a map.
    /// @param useColor If true then it also return special characters, which when printed into terminal will be in set color.
    std::string printArea(bool useColor) const;

    /// Returns information whether CTower can be placed instead of this CArea.
    bool canPlaceTower();

    /// Increases attack charge and once full, then sets back to 0.
    /// @return True if can attack.
    virtual bool attackChargeIncrement();

    /// Gets effect code based on effects name.
    /// @param [ref] effect Effect name in english.
    /// @return Effect code.
    char getEffectCode(const std::string &effect);

    /// Creates effect.
    /// @param effectName Effects name.
    /// @param damage Attack damage.
    /// @param range Attack range.
    /// @param towerId Id which determines in what order has been this CArea created.
    /// @param attackAll Information whether effect applies to only one, or all enemies in proximity.
    /// @return Pointer to created effect.
    std::shared_ptr<CEffect> getEffect(const std::string &effectName, int damage, int range, int towerId, bool attackAll);

    /// Applies damage by taken effect.
    /// @return True if health has fallen below 0.
    virtual bool damageByEffect();

    /// Perform an attack on an enemy.
    /// @param enemy Enemy which will be attacked.
    /// @return True if health has fallen below 0.
    /// @return True if attack is supposed to be executed on all enemies in proximity.
    virtual std::pair<bool, bool> attack(CArea *enemy);

    /// Return id, which determines in what order has this CArea been created.
    int getId() const;
};


#endif //TOWERDEFENCE_CAREA_HPP
