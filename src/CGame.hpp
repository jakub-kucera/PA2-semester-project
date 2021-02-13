/**
 * @author Jakub Kučera <kucerj56@fit.cvut.cz>
 * @date 30.04.2020
 */

#ifndef TOWERDEFENCE_CGAME_HPP
#define TOWERDEFENCE_CGAME_HPP

#include <vector>
#include <list>
#include "CTower.hpp"
#include "CEnemy.hpp"
#include "CMap.hpp"
#include "CInterface.hpp"
#include "CCoordinates.hpp"
#include "CPathFinder.hpp"
#include "CAssetLoader.hpp"

/// Takes care of core functionality of the game. Connects all of the different modules.
class CGame {
private:
    /// @var Number of seconds per frame.
    const double S_PER_FRAME = 1;
    /// @var Constant that determines the amount money, that the user will receive for destroying an enemy.
    const int MONEY_PER_DESTROYED_ENEMY = 20;
    /// @var Object that takes care of input/output from/to files.
    CAssetLoader *m_AssetLoader = nullptr;
    /// @var Interface that takes care of users I/O.
    CInterface *m_Interface = nullptr;
    /// @var Object that takes case of everything concerning paths for enemies.
    CPathFinder *m_PathFinder = nullptr;
    /// @var Array that stores all the types of towers.
    std::vector<CTower> m_TowerTypes;
    /// @var Array that stores all the types of enemies.
    std::vector<CEnemy> m_EnemyTypes;
    /// @var Array that stores all the maps.
    std::vector<CMap> m_Maps;
    /// @var Array that stores all the coordinates of the correct path from start to the destination.
    std::vector<CCoordinates> m_Path;
    /// @var Map that is used for game logic.
    std::vector<std::vector<CArea *>> m_LogicMap;
    /// @var List that includes coordinates of all towers that have been built.
    std::list<CCoordinates> m_TowersCoords;
    /// @var Index of map chosen by user.
    unsigned int m_ChosenMap;
    /// @var Stores the games score.(user : game)
    std::pair<int, int> m_Score;
    /// @var Determines whether the game is supposed to get closed.
    bool m_CloseGame;
    /// @var Determines whether the current round has ended.
    bool m_EndOfRound;
    /// @var Current amount of money available for tower building.
    int m_Money;
    /// @var Current users health.
    int m_Health;
    /// @var Width of users terminal (in number of chars).
    unsigned int m_TermWidth;
    /// @var Height of users terminal (in number of chars).
    unsigned int m_TermHeight;

    /// Deletes all pointers stored in m_LogicMap.
    void deleteLogicMap();

    /// Loads assets such as maps, towers and enemies from given files. Can also save/load started games.
    /// @return True if all the assets were loaded correctly.
    bool loadAssets();

    /// sets certain attributes, so that game can be started
    void prepareGame();

    /// Sets the correct map based on user input.
    void chooseMap();

    /// Generates map that is used for game logic.
    /// @param [ref] map Original map chosen by user.
    void generateLogicMap(const std::vector<std::string> &map);

    /// Processes user input and adds new towers.
    void getInput();

    /// Creates path for enemies.
    void findPath();
    /// Saves current state of game to a file.
    /// @return True if game has been saved correctly.
    bool saveGame() const;

    /// Load current state of game from a file.
    /// @return True if game has been loaded correctly.
    bool loadGame();

    /// Destroys an enemy on given coordinates.
    /// @param [in] coords Coordinates of enemy.
    void destroyEnemy(CCoordinates coords);

    /// Executes tower attacks on nearby enemies.
    void attack();

    /// Moves enemies, executes tower attacks.
    void update();

public:
    CGame();

    ~CGame() noexcept;

    /// Starts the entire game.
    void start();
};


#endif //TOWERDEFENCE_CGAME_HPP
