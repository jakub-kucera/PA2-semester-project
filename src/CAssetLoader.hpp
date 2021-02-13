/**
 * @author Jakub Kučera <kucerj56@fit.cvut.cz>
 * @date 30.04.2020
 */

#ifndef TOWERDEFENCE_CASSETLOADER_HPP
#define TOWERDEFENCE_CASSETLOADER_HPP
//#pragma once

#include <string>
#include <vector>
#include <map>
#include "CTower.hpp"
#include "CEnemy.hpp"
#include "CMap.hpp"
#include "CLoadedGame.hpp"
#include "CSavedFileFormatter.hpp"

/// Takes care of working with files.
class CAssetLoader {
private:
    /// @var Stream that is used to read assets from files.
    mutable std::ifstream inputStream;
    /// @var Path to file containing map.
    const char *const FILE_MAP = "examples/map.txt";
    /// @var Path to file containing enemy types.
    const char *const FILE_ENEMY = "examples/enemy.txt";
    /// @var Path to file containing tower types.
    const char *const FILE_TOWER = "examples/tower.txt";
    /// @var Map identifier.
    const char *const ID_MAP = "-ID_MAP-";
    /// @var Money identifier
    const char *const ID_MONEY = "-MONEY-";
    /// @var Health identifier
    const char *const ID_HEALTH = "-HEALTH-";
    /// @var Score identifier
    const char *const ID_SCORE = "-SCORE-";
    /// @var Score identifier
    const char *const ID_PATH = "-PATH-";
    /// @var Path identifier
    const char *const ID_TOWER = "-TOWER-";
    /// @var Tower identifier
    const char *const ID_ENEMY = "-ENEMY-";
    /// @var Assign pointer to object which will correctly load a saved record.
    std::map<std::string, CSavedFileFormatter *> SAVE_FILE_IDS_MAP{{ID_MAP,    new CSavedFileFormatterMap()},
                                                                   {ID_MONEY,  new CSavedFileFormatterMoney()},
                                                                   {ID_HEALTH, new CSavedFileFormatterHealth()},
                                                                   {ID_SCORE,  new CSavedFileFormatterScore()},
                                                                   {ID_PATH,   new CSavedFileFormatterPath()},
                                                                   {ID_TOWER,  new CSavedFileFormatterTower()},
                                                                   {ID_ENEMY,  new CSavedFileFormatterEnemy()}};
    /// @var Assigns code values based on color names.
    const std::unordered_map<std::string, uint8_t> m_colorTable = {{"black",   40},
                                                                   {"red",     41},
                                                                   {"green",   42},
                                                                   {"yellow",  43},
                                                                   {"blue",    44},
                                                                   {"magenta", 45},
                                                                   {"cyan",    46},
                                                                   {"white",   47}};
public:
    ~CAssetLoader() noexcept;

    /// Checks whether the stream correctly opened a file.
    /// @return False if everything is alright.
    bool checkStream(/*std::iofstream*/) const;

    /// Gets color code based on colors name.
    /// @param [ref] color Color name in english.
    /// @return Color code.
    uint8_t getColor(const std::string &color) const;

    /// Loads maps from file.
    /// @return Array of maps.
    std::vector<CMap> loadMap() const;

    /// Loads tower types from file.
    /// @return Array of tower types.
    std::vector<CTower> loadTower() const;

    /// Loads enemy types from file.
    /// @return Array of enemy types.
    std::vector<CEnemy> loadEnemy() const;

    /// Saves current state of game to a file.
    /// @param [in] savedGame Includes all the necessary information of the current game state.
    /// @param [ref] file Name of file where the game is suppose to save.
    /// @return True if game has been loaded correctly.
    bool saveGame(CLoadedGame *savedGame, const std::string &file) const;

    /// Loads a saved game from a file.
    /// @param [ref] file Name of file where the game is suppose to save.
    /// @return Includes all the necessary information of the saved game to be loaded.
    CSavedGame *loadGame(const std::string &file) const;

    /// Splits string into smaller strings by separating it with delimiter.
    /// @param string String that is to be separated.
    /// @param delimiter Delimiter which is used to separate string.
    /// @return Separated string.
    std::vector<std::string> splitString(std::string string, std::string delimiter) const;
};


#endif //TOWERDEFENCE_CASSETLOADER_HPP