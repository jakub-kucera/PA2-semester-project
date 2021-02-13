/**
 * @author Jakub Kučera <kucerj56@fit.cvut.cz>
 * @date 30.04.2020
 */

#ifndef TOWERDEFENCE_CMAP_HPP
#define TOWERDEFENCE_CMAP_HPP

#include <string>
#include <vector>
#include <fstream>

/// Represent a playable map.
class CMap {
private:
    /// @var Map stores as characters.
    std::vector<std::string> m_Map;
    /// @var Maps name.
    std::string m_Name;
    /// @var Health that the user starts with.
    int m_MaxHealth;
    /// @var Amount of money that the user starts with.
    int m_Money;
    /// @var Height of man in number of characters.
    unsigned int m_Height;
    /// @var Length of man in number of characters.
    unsigned int m_Length;
public:
    /// Returns starting health.
    int getMaxHealth() const;

    /// Returns current amount of money available.
    int getMoney() const;

    /// Returns the entire map represented as a 2D array.
    const std::vector<std::string> &getMap() const;

    /// Returns maps name.
    const std::string &getName() const;

    /// Return maps height.
    unsigned int getHeight() const;

    /// Return maps length.
    unsigned int getLength() const;

public:
    CMap() = default;

    CMap(std::vector<std::string> map, std::string name, int health, int money);

    friend std::ostream &operator<<(std::ostream &, const CMap &);
};

#endif //TOWERDEFENCE_CMAP_HPP
