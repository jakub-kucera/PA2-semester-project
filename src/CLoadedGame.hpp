/**
 * @author Jakub Kučera <kucerj56@fit.cvut.cz>
 * @date 24.05.2020
 */

#ifndef TOWERDEFENCE_CLOADEDGAME_HPP
#define TOWERDEFENCE_CLOADEDGAME_HPP


#include <utility>
#include <list>
#include <vector>
#include "CCoordinates.hpp"
#include "CArea.hpp"

/// Used to save game to a file.
struct CLoadedGame {
    /// @var Index of map.
    unsigned int m_MapIndex;
    /// @var Amount of money left.
    int m_Money;
    /// @var Amount of health left.
    int m_Health;
    /// @var Users score.
    std::pair<int, int> m_Score;
    /// @var Array of all towers and their coordinates.
    std::vector<std::pair<CCoordinates, CArea *>> m_Towers;
    /// @var Path for enemies.
    std::vector<CCoordinates> m_Path;
    /// @var Array of all enemies and their coordinates.
    std::vector<std::pair<CCoordinates, CArea *>> m_Enemies;
};

/// Used to load a tower
struct CSavedTower {
    CSavedTower(CCoordinates coords, int towerId, int attackCharge)
            : m_Coords(coords), m_TowerId(towerId), m_AttackCharge(attackCharge) {}
    /// @var Coordinates of the tower.
    CCoordinates m_Coords;
    /// @var Id of tower.
    int m_TowerId;
    /// @var Attack charge.
    int m_AttackCharge;
};

/// Used to load an enemy effect from a file.
struct CSavedEnemyEffect {
    CSavedEnemyEffect(int effectGivenByTowerId, int baseDamage, int duration) : m_EffectGivenByTowerId(
            effectGivenByTowerId), m_BaseDamage(baseDamage), m_Duration(duration) {}
    /// @var Id of tower that has given this effect to the enemy.
    int m_EffectGivenByTowerId;
    /// @var Current effect damage.
    int m_BaseDamage;
    /// @var Current effect duration.
    int m_Duration;
};

/// Used to load an enemy from a file.
struct CSavedEnemy {
    /// @var Coordinates of the enemy.
    CCoordinates m_Coords;
    /// @var Id of enemies.
    int m_EnemyId;
    /// @var Amount of health left.
    int m_Health;
    /// @var Pointer to an effect that has been taken from a tower.
    std::shared_ptr<CSavedEnemyEffect> m_EnemyEffect = nullptr;


};

/// Used to load game from a file.
struct CSavedGame {
    /// @var Index of map.
    unsigned int m_MapIndex;
    /// @var Amount of money left.
    int m_Money;
    /// @var Amount of health left.
    int m_Health;
    /// @var Users score.
    std::pair<int, int> m_Score;
    /// @var Array of all towers.
    std::vector<CSavedTower> m_Towers;
    /// @var Path for enemies.
    std::vector<CCoordinates> m_Path;
    /// @var Array of all enemies.
    std::vector<CSavedEnemy> m_Enemies;
};


#endif //TOWERDEFENCE_CLOADEDGAME_HPP
