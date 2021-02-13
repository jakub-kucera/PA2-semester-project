/**
 * @author Jakub Kučera <kucerj56@fit.cvut.cz>
 * @date 30.04.2020
 */

#ifndef TOWERDEFENCE_CENEMY_HPP
#define TOWERDEFENCE_CENEMY_HPP

#include <string>
#include <fstream>
#include "CArea.hpp"

/// Specifies that area on a map is an enemy, that moves across the map and can be destroyed by a tower.
class CEnemy : public CArea {
private:
    /// @var Name of CEnemy.
    std::string m_Name;
public:
    CEnemy(char mapSymbol, uint8_t symbolColorCode, std::string name, const std::string &type, int id, int health);

    /// Applies damage by taken effect.
    /// @return True if health has fallen below 0.
    virtual bool damageByEffect() override; //true for health < 0

    friend std::ostream &operator<<(std::ostream &, const CEnemy &);
};

#endif //TOWERDEFENCE_CENEMY_HPP
