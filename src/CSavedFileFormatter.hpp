/**
 * @author Jakub Kučera <kucerj56@fit.cvut.cz>
 * @date 28.05.2020
 */

#ifndef TOWERDEFENCE_CSAVEDFILEFORMATTER_HPP
#define TOWERDEFENCE_CSAVEDFILEFORMATTER_HPP


#include <string>
#include "CLoadedGame.hpp"

/// Abstract class. Loads all of the types of saved records.
struct CSavedFileFormatter {
    virtual ~CSavedFileFormatter() = default;

    /// Loads given records.
    /// @param savedGame Pointer to structure where all the loaded records are saved.
    /// @param substrings Array of all the necessary information.
    /// @return True if record has been correctly loaded.
    virtual bool loadRecord(CSavedGame */* savedGame*/, const std::vector<std::string> & /*substrings*/) = 0;
};

/// Takes care of loading maps from saved records.
struct CSavedFileFormatterMap : public CSavedFileFormatter {
    /// Loads map.
    /// @param savedGame Pointer to structure where all the loaded records are saved.
    /// @param substrings Array of all the necessary information.
    /// @return True if record has been correctly loaded.
    virtual bool loadRecord(CSavedGame *savedGame, const std::vector<std::string> &substrings) override {
        if (substrings.size() != 1) {
            return false;
        }
        savedGame->m_MapIndex = std::stoi(substrings[0]);
        return true;
    }
};

/// Takes care of loading maps from saved records.
struct CSavedFileFormatterMoney : public CSavedFileFormatter {
    /// Loads amount of money.
    /// @param savedGame Pointer to structure where all the loaded records are saved.
    /// @param substrings Array of all the necessary information.
    /// @return True if record has been correctly loaded.
    virtual bool loadRecord(CSavedGame *savedGame, const std::vector<std::string> &substrings) override {
        if (substrings.size() != 1) {
            return false;
        }
        savedGame->m_Money = std::stoi(substrings[0]);
        return true;
    }
};

/// Takes care of loading health from saved records.
struct CSavedFileFormatterHealth : public CSavedFileFormatter {
    /// Loads health.
    /// @param savedGame Pointer to structure where all the loaded records are saved.
    /// @param substrings Array of all the necessary information.
    /// @return True if record has been correctly loaded.
    virtual bool loadRecord(CSavedGame *savedGame, const std::vector<std::string> &substrings) override {
        if (substrings.size() != 1) {
            return false;
        }
        savedGame->m_Health = std::stoi(substrings[0]);
        return true;
    }
};

/// Takes care of users score from saved records.
struct CSavedFileFormatterScore : public CSavedFileFormatter {
    /// Loads users score.
    /// @param savedGame Pointer to structure where all the loaded records are saved.
    /// @param substrings Array of all the necessary information.
    /// @return True if record has been correctly loaded.
    virtual bool loadRecord(CSavedGame *savedGame, const std::vector<std::string> &substrings) override {
        if (substrings.size() != 2) {
            return false;
        }
        savedGame->m_Score = std::pair<int, int>(std::stoi(substrings[0]), std::stoi(substrings[1]));
        return true;
    }
};

/// Takes care of loading path for enemies from saved records.
struct CSavedFileFormatterPath : public CSavedFileFormatter {
    /// Loads path for enemies.
    /// @param savedGame Pointer to structure where all the loaded records are saved.
    /// @param substrings Array of all the necessary information.
    /// @return True if record has been correctly loaded.
    virtual bool loadRecord(CSavedGame *savedGame, const std::vector<std::string> &substrings) override {
        if (substrings.size() != 2) {
            return false;
        }
        savedGame->m_Path.emplace_back(std::stoi(substrings[0]), std::stoi(substrings[1]));
        return true;
    }
};

/// Takes care of loading placed towers on map from saved records.
struct CSavedFileFormatterTower : public CSavedFileFormatter {
    /// Loads towers.
    /// @param savedGame Pointer to structure where all the loaded records are saved.
    /// @param substrings Array of all the necessary information.
    /// @return True if record has been correctly loaded.
    virtual bool loadRecord(CSavedGame *savedGame, const std::vector<std::string> &substrings) override {
        if (substrings.size() != 4) {
            return false;
        }
        savedGame->m_Towers.emplace_back(CCoordinates(std::stoi(substrings[0]), std::stoi(substrings[1])),
                                         std::stoi(substrings[2]),
                                         std::stoi(substrings[3]));
        return true;
    }
};

/// Takes care of loading enemies from saved records.
struct CSavedFileFormatterEnemy : public CSavedFileFormatter {
    /// Loads enemies.
    /// @param savedGame Pointer to structure where all the loaded records are saved.
    /// @param substrings Array of all the necessary information.
    /// @return True if record has been correctly loaded.
    virtual bool loadRecord(CSavedGame *savedGame, const std::vector<std::string> &substrings) override {
        int size = substrings.size();
        if (size != 4 && size != 7) {
            return false;
        }
        CSavedEnemy enemy{CCoordinates(std::stoi(substrings[0]), std::stoi(substrings[1])),
                          std::stoi(substrings[2]),
                          std::stoi(substrings[3])};
        if (size == 7) {
            enemy.m_EnemyEffect = std::make_shared<CSavedEnemyEffect>(std::stoi(substrings[4]),
                                                                      std::stoi(substrings[5]),
                                                                      std::stoi(substrings[6]));
        }
        savedGame->m_Enemies.push_back(enemy);
        return true;
    }
};

#endif //TOWERDEFENCE_CSAVEDFILEFORMATTER_HPP
