/**
 * @author Jakub Kučera <kucerj56@fit.cvut.cz>
 * @date 01.05.2020
 */

#ifndef TOWERDEFENCE_CINTERFACE_HPP
#define TOWERDEFENCE_CINTERFACE_HPP


#include "CMap.hpp"
#include "CArea.hpp"
#include "CTower.hpp"

/// Takes care of all users input and output.
class CInterface {
private:
    /// @var Sequence of characters that clears the users terminal.
    const char * const CLEAR_COMMAND = "\x1B[2J\x1B[H";
    /// @var Path to folder, that is used for storing saved games.
    const char * const SAVE_FOLDER = "examples/saves/";
    /// @var File extension used by save files.
    const char * const SAVE_EXTENSION = ".td";

    /// @var Map that is used for game logic.
    std::vector<std::vector<CArea *>> m_LogicMap;
    /// @var Array that stores all the types of towers.
    const std::vector<CTower> m_Towers;
    /// @var Current amount of money available for tower building.
    int m_Money;
    /// @var Amount of health that the user starts with.
    int m_MaxHealth;
    /// @var Current users health.
    int m_Health;
    /// @var Width of users terminal (in number of chars).
    unsigned int m_TermWidth;
    /// @var Height of users terminal (in number of chars).
    unsigned int m_TermHeight;
    /// @var Width of the chosen map (in number of chars).
    unsigned int m_MapWidth;
    /// @var Number of decimal place of m_MapWidth.
    unsigned int m_MapWidthLength;
    /// @var Height of the chosen map (in number of chars).
    unsigned int m_MapHeight;
    /// @var Number of decimal place of m_MapHeight.
    unsigned int m_MapHeightLength;
    /// @var Stores the games score
    std::pair<int, int> m_Score;

public:
    /// Constructor for CInterface.
    /// @param [in] towers Array of all the types of towers.
    /// @param [in] money Amount of money that the users starts with.
    /// @param [in] width Width of users terminal (in number of chars).
    /// @param [in] height Height of users terminal (in number of chars).
    CInterface(std::vector<CTower> towers, int money, unsigned int width, unsigned int height);

    /// Clears users terminal.
    void clear() const;

    /// Prints line across the whole terminal.
    /// @param [in] c Specifies the character with which the line is to be printed.
    void printLine(char c) const;

    /// Prints line across the whole terminal using the alphabet letters (first upper case, then lower case letters).
    void printNumericLine() const;

    /// Reads separate characters from input
    /// @return the character from input
    int getInput() const;

    /// Prints the current state of a map, including all the enemies and towers.
    void render() const;

    /// Updates the entire map.
    /// @param [ref] map The newest version of map.
    void updateMap(const std::vector<std::vector<CArea *>> &map);

    /// Updates current users health.
    /// @param [in] health Current health.
    void updateHealth(int health);

    /// User chooses a map.
    /// @param [ref] maps Array of all the different maps.
    /// @return Index of the chosen map.
    unsigned int getMapNumber(const std::vector<CMap> &maps) const;

    /// Updates the amount of money currently available.
    /// @param [in] money The current amount of money.
    void updateMoney(int money);

    /// Updates the users score.
    /// @param [in] money The current score.
    void updateScore(std::pair<int, int> score);

    /// Reads file name from users input.
    /// @param [in] forSaving True if the file is supposed to be used to save game, false to load game.
    /// @return File name.
    std::string getFileName(bool forSaving) const;

    /// User chooses how path for enemies gets to be generated.
    /// @return Char that specifies what search algorithm is used to generate path.
    /// @return Char that specifies how to generate path (example: 'b' for closer to walls).
    std::pair<char, char> getPathOption() const;

    /// Finds out if user wants to load a saved game.
    /// @return True if user wants to load a saved game.
    bool loadGame() const;
};


#endif //TOWERDEFENCE_CINTERFACE_HPP
