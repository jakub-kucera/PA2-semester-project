/**
 * @author Jakub Kučera <kucerj56@fit.cvut.cz>
 * @date 01.05.2020
 */


#include <utility>
#include <iostream>
#include <cmath>
#include <termios.h>
#include <unistd.h>
#include "CInterface.hpp"
#include "CWall.hpp"


CInterface::CInterface(std::vector<CTower> towers, int money,/* int health, */unsigned int width, unsigned int height)
        : m_Towers(std::move(towers)),
          m_Money(money),
          m_MaxHealth(0),
          m_Health(0),
          m_TermWidth(width),
          m_TermHeight(height),
          m_MapWidth(0),
          m_MapWidthLength(0),
          m_MapHeight(0),
          m_MapHeightLength(0),
          m_Score(0, 0) {}

void CInterface::clear() const {
    std::cout << CLEAR_COMMAND;
}

void CInterface::printLine(char c) const { //paints a line across the whole terminal using chosen character
    for (unsigned int i = 0; i < m_TermWidth; ++i)
        std::cout << c;
}

void CInterface::printNumericLine() const { //prints alphabet in order. First upper case, then lower case.

    int base = (int) pow(10, m_MapWidthLength - 1);
    while (base != 0 && m_MapWidth / base) {
        for (unsigned int i = 0; i <= m_MapHeightLength; ++i) {
            std::cout << " ";
        }
        for (unsigned int i = 0; i < m_MapWidth; ++i) {
            std::cout << (int) (i / base) % 10;
        }
        base /= 10;
        std::cout << '\n';
    }
}

int CInterface::getInput() const {
    //this method has been copied from http://www.cplusplus.com/forum/unices/18395/
    int character;
    struct termios oldTermios;
    struct termios newTermios;
    tcgetattr(STDIN_FILENO, &oldTermios); /*store old settings */
    newTermios = oldTermios; /* copy old settings to new settings */
    newTermios.c_lflag &= ~(ICANON | ECHO); /* make one change to old settings in new settings */
    tcsetattr(STDIN_FILENO, TCSANOW, &newTermios); /*apply the new settings immediatly */
    character = getchar(); /* standard getchar call */
    tcsetattr(STDIN_FILENO, TCSANOW, &oldTermios); /*reapply the old settings */
    //std::cin >> character; //or i could just use this.
    return character; /*return received char */
}

void CInterface::render() const {
    clear();
    printNumericLine();
    int c = 0;
    int base = (int)pow(10, m_MapHeightLength - 1);
    //prints map with towers and enemies
    for (const auto &line : m_LogicMap) {
        for (int i = 0; i < base - 1; ++i) {
            std::cout << "0";
        }
        std::cout << c << " ";
        for (const auto *character : line) {
            std::cout << character->printArea(true);
        }
        std::cout << " ";
        for (int i = 0; i < base - 1; ++i) {
            std::cout << "0";
        }
        std::cout << c << '\n';
        c++;
    }
    printNumericLine();
    printLine('-');
    //prints games statistics
    std::cout << "Number of lost games: " << m_Score.second << '\n';
    std::cout << "Health: " << m_Health << "/" << m_MaxHealth << '\n';
    std::cout << "Money:  " << "$" << m_Money << '\n';
    if (m_Health <= 0) {
        std::cout << "======YOU LOST======" << '\n';
    }
    printLine('-');
    int i = 1;
    //Prints all the towers.
    for (const auto &tower : m_Towers) {
        if (tower.getCost() > m_Money) {
            std::cout << "\033[30m";
        }
        std::cout << i++ << ": " << "\033[0m" << tower;
    }
    printLine('-');
    std::cout << std::endl;
}

void CInterface::updateMap(const std::vector<std::vector<CArea *>> &map) {
    m_LogicMap = map;
    if (m_MapHeight == 0) {
        m_MapWidth = m_LogicMap[0].size();
        m_MapHeight = m_LogicMap.size();
        m_MapWidthLength = (unsigned int) trunc(log10(m_MapWidth)) + 1;
        m_MapHeightLength = (unsigned int) trunc(log10(m_MapHeight)) + 1;
    }
}

void CInterface::updateHealth(int health) {
    m_Health = health;
    if (m_MaxHealth == 0) {
        m_MaxHealth = m_Health;
    }
}

unsigned int CInterface::getMapNumber(const std::vector<CMap> &maps) const {
    int i = 1;
    //prints maps
    for (const auto &map : maps) {
        std::cout << i++ << ": " << std::endl;
        if (map.getLength() > m_TermWidth || map.getHeight() > m_TermHeight) {
            std::cout << "This map is not fully supported by your device, game might not work properly!" << std::endl;
        } else {
            std::cout << map;
        }
        printLine('-');
    }
    std::cout
            << "!BEWARE: CURRENT IMPLEMENTATION OF PATH FINDING ALGORITHM IS VERY SLOW, IF YOU CHOOSE #1, IT WILL TAKE AROUND 3 MINUTES!"
            << std::endl;
    std::cout << "Write map number: " << std::endl;

    //user chooses map
    while (true) {
        unsigned int mapChoice = 0;
        std::string input;
        std::cin >> input;
        try{
            mapChoice = stoi(input);
        }
        catch (const std::exception & e){
            mapChoice = 0;
        }

        if (mapChoice <= maps.size() && mapChoice > 0) {
            std::cout << "\033[7mYou chose map number: " << mapChoice
                      << " named: " << maps[mapChoice - 1].getName()
                      << "\033[0m\n" << std::endl;
            return mapChoice;
        } else {
            std::cout << "\033[7mMap number: "
                      << mapChoice
                      << " does not exist, please choose map number again. "
                      << "\033[0m\n" << std::endl;
        }
    }
}

void CInterface::updateMoney(int money) {
    m_Money = money;
}

void CInterface::updateScore(std::pair<int, int> score) {
    m_Score = score;
}

std::string CInterface::getFileName(bool forSaving) const {
    std::string fileName;
    if (forSaving) {
        std::cout << "Write name of your new save: " << std::endl;
        std::cin >> fileName;
        return SAVE_FOLDER + fileName + SAVE_EXTENSION;
    }
    std::cout << "Write name of an existing save, that you would like to load: " << std::endl;
    std::ifstream f;
    do {
        std::cin >> fileName;
        fileName = SAVE_FOLDER + fileName + SAVE_EXTENSION;
        f.open(fileName.c_str());
        if (!f.good()) {
            std::cout << "File name that you have entered is invalid, please write a valid one: " << std::endl;
        }
    } while (!f.good());
    return fileName;
}

std::pair<char, char> CInterface::getPathOption() const {
    std::string pathOption;
    char type, algorithmChoice;
    bool correctInput = true;
    std::cout << "Choose how you want to generate path for enemies: " << std::endl;
    std::cout
            << "Enter 2 or 1 characters (example: BD, W). First character defines how the path is supposed to look (B for basic, W for path to be as close as possible to walls, and N for it to be as far as possible from walls)."
            << std::endl;
    std::cout
            << "Second parameter is optional. It defines what kind of search algorithm is used. (B is for BFS(default), D is for DFS)."
            << std::endl;
    do {
        if (!correctInput) {
            std::cout << "Your input is invalid, please write a valid one: " << std::endl;
        }
        algorithmChoice = 'b';
        std::cin >> pathOption;
        type = (char)tolower(pathOption[0]);
        if (pathOption.size() == 2) {
            algorithmChoice = (char)tolower(pathOption[1]);
        }
        correctInput = pathOption.size() <= 2
                       && (algorithmChoice == 'b' || algorithmChoice == 'd')
                       && (type == 'w' || type == 'b' || type == 'n');
    } while (!correctInput);
    return {type, algorithmChoice};
}

bool CInterface::loadGame() const {
    std::cout << "Would you like to load a saved game. Enter yes/no: " << std::endl;
    bool correctInput = false;
    std::string loadInput;
    do {
        std::cin >> loadInput;
        std::cin.ignore(1, '\n');
        correctInput = loadInput == "yes" || loadInput == "no" || loadInput == "y" || loadInput == "n";
        if (!correctInput) {
            std::cout << "Not a valid option, write again: " << std::endl;
        }

    } while (!correctInput);

    return loadInput == "yes" || loadInput == "y";
}