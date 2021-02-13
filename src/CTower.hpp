/**
 * @author Jakub Kučera <kucerj56@fit.cvut.cz>
 * @date 30.04.2020
 */

#ifndef TOWERDEFENCE_CTOWER_HPP
#define TOWERDEFENCE_CTOWER_HPP
//#pragma once

#include <string>
#include <fstream>
#include "CArea.hpp"

/// Specifies that area on a map is a tower, which can attack enemies.
class CTower : public CArea {
private:
    /// @var Name of CTower.
    std::string m_Name;
    /// @var Name of building.
    int m_Cost;

public:
    CTower(char mapSymbol, uint8_t symbolColorCode, std::string name, const std::string &type,
           const std::string &attackZone, int cost, int damage, int range, int id, int attackSpeed);

    /// Returns cost.
    int getCost() const;

    /// Perform an attack on an enemy.
    /// @param enemy Enemy which will be attacked.
    /// @return True if health has fallen below 0.
    /// @return True if attack is supposed to be executed on all enemies in proximity.
    virtual std::pair<bool, bool> attack(CArea *enemy) override;//bool health <= 0, bool attackAll

    /// Increases attack charge and once full, then sets back to 0.
    /// @return True if can attack.
    virtual bool attackChargeIncrement() override;

    friend std::ostream &operator<<(std::ostream &, const CTower &);
};

//std::ostream & operator<< ( std::ostream &, const CTower &);

#endif //TOWERDEFENCE_CTOWER_HPP
