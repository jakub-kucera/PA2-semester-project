/**
 * @author Jakub Kučera <kucerj56@fit.cvut.cz>
 * @date 30.04.2020
 */

#include <ctime>
#include <unistd.h>
#include <sys/ioctl.h>
#include <cmath>
#include <csignal>
#include <termios.h>
#include <chrono>
#include <cstdio>
#include <algorithm>
#include "CGame.hpp"
#include "CWall.hpp"
#include "CDFS.hpp"
#include "CBFS.hpp"
#include "CLoadedGame.hpp"
#include <iostream>


CGame::CGame() : m_ChosenMap(1), m_Score(0, 0), m_CloseGame(false), m_EndOfRound(true), m_Money(0), m_Health(0) {
    //finds out the dimensions of user terminal
    struct winsize size{};
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &size);
    m_TermHeight = size.ws_row;
    m_TermWidth = size.ws_col;
}

CGame::~CGame() noexcept {
    deleteLogicMap();
    delete m_Interface;
    delete m_PathFinder;
    delete m_AssetLoader;
}

void CGame::deleteLogicMap() {
    for (auto &line : m_LogicMap) {
        for (auto &area : line) {
            delete area;
        }
        line.clear();
    }
}

bool CGame::loadAssets() {
    m_TowerTypes = m_AssetLoader->loadTower();
    m_EnemyTypes = m_AssetLoader->loadEnemy();
    m_Maps = m_AssetLoader->loadMap();
    return !m_TowerTypes.empty() && !m_Maps.empty() && !m_EnemyTypes.empty();
}

void CGame::prepareGame() {
    m_TowersCoords.clear();
    m_Health = m_Maps[m_ChosenMap].getMaxHealth();
    m_Money = m_Maps[m_ChosenMap].getMoney();
    m_Interface->updateHealth(m_Health);
    m_Interface->updateMoney(m_Money);
    generateLogicMap(m_Maps[m_ChosenMap].getMap());
}

void CGame::chooseMap() {
    unsigned int mapChoice = m_Interface->getMapNumber(m_Maps);
    m_ChosenMap = mapChoice - 1;
}

void CGame::generateLogicMap(const std::vector<std::string> &map) {
    deleteLogicMap();
    m_LogicMap.resize(map.size());
    int i = 0;
    for (const auto &line : map) {
        for (const auto &character : line) {
            switch (character) {
                case ' ':
                    m_LogicMap[i].push_back(new CArea());
                    break;
                case '#':
                    m_LogicMap[i].push_back(new CWall('#'));
                    break;
                case '=':
                    m_LogicMap[i].push_back(new CWall('='));
                    break;
                default:
                    m_LogicMap[i].push_back(new CArea());
                    break;
            }
        }
        ++i;
    }
}

void CGame::getInput() {
    int input = m_Interface->getInput();
    if (input == 'c') {
        unsigned int tower = 1, x = 1, y = 1; //Initialization so that CLion doesn't show warnings.
        bool repeatInput = false;
        bool doBuild = true;
        do {
            repeatInput = false;
            std::cout << "Enter: X Y Tower_Number" << std::endl;
            std::cin >> x >> y >> tower;
            std::cin.ignore(1, '\n');//ignores \n on input
            //Checks if this tower exists and given coordinates aren't outside of map.
            if (tower > m_TowerTypes.size()
                || tower < 1
                || y < 1
                || y > m_LogicMap.size() - 2
                || x < 1
                || x > m_LogicMap[0].size() - 2) {
                std::cout << "You entered wrong coordinates, or wrong tower id." << std::endl;
                repeatInput = true;
            }
                //Checks if there already isn't something on given coordinates.
            else if (!m_LogicMap[y][x]->canPlaceTower()) {
                std::cout << "Tower cannot be placed on other towers/walls." << std::endl;
                repeatInput = true;
            }
                //Checks if path for enemies isn't on given coordinates.
            else {
                CCoordinates coords(x, y);
                for (const auto &coordsPath : m_Path) {
                    if (coordsPath == coords) {
                        std::cout << "Tower cannot be placed on path for enemies." << std::endl;
                        repeatInput = true;
                        break;
                    }
                }
            }

            //Checks if user has enough money to buy selected tower.
            if (!repeatInput && m_TowerTypes[tower - 1].getCost() > m_Money) {
                for (unsigned int i = 0; i < m_TowerTypes.size(); ++i) {
                    if (m_TowerTypes[i].getCost() <= m_Money) {
                        repeatInput = true;
                        break;
                    }
                }
                if (repeatInput) {
                    std::cout << "You cannot build this tower, it cost too much money." << std::endl;
                } else {
                    std::cout << "You don't have enough money to buy any tower." << std::endl;
                    doBuild = false;
                }
            }
        } while (repeatInput);

        if (doBuild) {
            m_Money -= m_TowerTypes[tower - 1].getCost();
            m_Interface->updateMoney(m_Money);
            delete m_LogicMap[y][x];
            CTower *newTower = new CTower(m_TowerTypes[tower - 1]);
            m_LogicMap[y][x] = newTower;
            m_TowersCoords.emplace_back(x, y);
        }
    } else if (input == 'q') {
        m_CloseGame = true;
    } else if (input == 's') {
        saveGame();
    }
}

void CGame::findPath() {
    std::pair<char, char> pathOption = m_Interface->getPathOption();
    if (pathOption.second == 'b') {
        m_PathFinder = new CBFS();
    } else {
        m_PathFinder = new CDFS();
    }
    m_PathFinder->generateMap(m_Maps[m_ChosenMap].getMap());

    //std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
    m_PathFinder->createDistances();
    //std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    //std::cout << "Creating distanced took: " << std::chrono::duration_cast<std::chrono::nanoseconds> (end - begin).count() << "[ns]" << std::endl;

    //std::chrono::steady_clock::time_point begin2 = std::chrono::steady_clock::now();
    m_PathFinder->createPath(pathOption.first);
    //std::chrono::steady_clock::time_point end2 = std::chrono::steady_clock::now();
    //std::cout << "Generating path took: " << std::chrono::duration_cast<std::chrono::nanoseconds> (end2 - begin2).count() << "[ns]" << std::endl;

    m_Path = m_PathFinder->getPath();
    // m_PathFinder->printColorMap();
}

void CGame::update() {
    attack();
    m_Interface->updateMoney(m_Money);
    m_Health -= m_LogicMap[m_Path.rbegin()->m_Y][m_Path.rbegin()->m_X]->m_Health;
    m_Interface->updateHealth(m_Health);
    if (m_Health <= 0) {
        m_EndOfRound = true;
        m_Score.second++;
        m_Interface->updateScore(m_Score);
        return;
    }
    //moves all enemies
    delete m_LogicMap[m_Path.rbegin()->m_Y][m_Path.rbegin()->m_X];
    for (auto it = m_Path.rbegin(); it != m_Path.rend() - 1; it++) {
        m_LogicMap[it->m_Y][it->m_X] = m_LogicMap[(it + 1)->m_Y][(it + 1)->m_X];
    }
    //adds new enemies
    if (rand() % (int) (2 + log2(m_Score.first + 1))) {
        m_LogicMap[m_Path[0].m_Y][m_Path[0].m_X] = new CEnemy(m_EnemyTypes[rand() % m_EnemyTypes.size()]);
    } else {
        m_LogicMap[m_Path[0].m_Y][m_Path[0].m_X] = new CArea();
    }
}

void CGame::destroyEnemy(CCoordinates coords) {
    m_Money += MONEY_PER_DESTROYED_ENEMY;
    delete m_LogicMap[coords.m_Y][coords.m_X];
    m_LogicMap[coords.m_Y][coords.m_X] = new CArea();
}

void CGame::attack() {
    //enemies take damage by towers effect from earlier games
    for (const auto &enemyCoords : m_Path) {
        if (m_LogicMap[enemyCoords.m_Y][enemyCoords.m_X]->damageByEffect()) {
            destroyEnemy(enemyCoords);
        }
    }
    //towers attack enemies
    for (const auto &towerCoords : m_TowersCoords) {
        if (m_LogicMap[towerCoords.m_Y][towerCoords.m_X]->attackChargeIncrement()) {
            int range = m_LogicMap[towerCoords.m_Y][towerCoords.m_X]->m_Effect->getRange();
            for (auto it = m_Path.rbegin(); it != m_Path.rend(); ++it) {
                //checks whether enemies are in range for an attack
                if (m_LogicMap[it->m_Y][it->m_X]->m_Health > 0
                    &&
                    pow(abs(it->m_X - towerCoords.m_X), 2) + pow(abs(it->m_Y - towerCoords.m_Y), 2) <= pow(range, 2)) {
                    auto atc = m_LogicMap[towerCoords.m_Y][towerCoords.m_X]->attack(m_LogicMap[it->m_Y][it->m_X]);
                    //checks whether an enemy has been killed
                    if (atc.first) {
                        destroyEnemy(*it);
                    } else {
                        m_LogicMap[it->m_Y][it->m_X]->m_Effect = m_LogicMap[towerCoords.m_Y][towerCoords.m_X]->m_Effect->getEffect();
                    }
                    //decides whether one tower attacks only one, or all enemies in proximity
                    if (!atc.second) {
                        break;
                    }
                }
            }
        }
    }
}

bool CGame::saveGame() const {
    std::string fileName = m_Interface->getFileName(true);
    std::cin.ignore(1, '\n');//ignores \n at the end of input
    CLoadedGame *loadedGame = new CLoadedGame{m_ChosenMap, m_Money, m_Health, m_Score, {}, m_Path, {}};
    //saves all towers
    for (const auto &towerCoords : m_TowersCoords) {
        loadedGame->m_Towers.emplace_back(towerCoords, m_LogicMap[towerCoords.m_Y][towerCoords.m_X]);
    }
    //saves all enemies
    for (const auto &enemyCoords : m_Path) {
        if (m_LogicMap[enemyCoords.m_Y][enemyCoords.m_X]->m_Health > 0) {
            loadedGame->m_Enemies.emplace_back(enemyCoords, m_LogicMap[enemyCoords.m_Y][enemyCoords.m_X]);
        }
    }
    bool savedOK = m_AssetLoader->saveGame(loadedGame, fileName);
    delete loadedGame;
    return savedOK;
}

bool CGame::loadGame() {
    if (!m_Interface->loadGame()) {
        return false;
    }
    std::string fileName = m_Interface->getFileName(false);
    CSavedGame *savedGame = m_AssetLoader->loadGame(fileName);
    m_ChosenMap = savedGame->m_MapIndex;
    m_Health = savedGame->m_Health;
    m_Money = savedGame->m_Money;
    m_Score = savedGame->m_Score;
    m_Path = savedGame->m_Path;

    generateLogicMap(m_Maps[m_ChosenMap].getMap());

    //loads towers to map
    for (const auto &tower : savedGame->m_Towers) {
        delete m_LogicMap[tower.m_Coords.m_Y][tower.m_Coords.m_X];
        m_LogicMap[tower.m_Coords.m_Y][tower.m_Coords.m_X] = new CTower(m_TowerTypes[tower.m_TowerId]);
        m_LogicMap[tower.m_Coords.m_Y][tower.m_Coords.m_X]->m_AttackCharge = tower.m_AttackCharge;
        m_TowersCoords.emplace_back(tower.m_Coords);
    }
    //loads enemies to maps
    for (const auto &enemy : savedGame->m_Enemies) {
        delete m_LogicMap[enemy.m_Coords.m_Y][enemy.m_Coords.m_X];
        m_LogicMap[enemy.m_Coords.m_Y][enemy.m_Coords.m_X] = new CEnemy(m_EnemyTypes[enemy.m_EnemyId]);
        m_LogicMap[enemy.m_Coords.m_Y][enemy.m_Coords.m_X]->m_Health = enemy.m_Health;
        if (enemy.m_EnemyEffect != nullptr) {//applies effects received by tower attacks
            m_LogicMap[enemy.m_Coords.m_Y][enemy.m_Coords.m_X]->m_Effect = m_TowerTypes[enemy.m_EnemyEffect->m_EffectGivenByTowerId].m_Effect->getEffect();
            m_LogicMap[enemy.m_Coords.m_Y][enemy.m_Coords.m_X]->m_Effect->m_BaseDamage = enemy.m_EnemyEffect->m_BaseDamage;
            m_LogicMap[enemy.m_Coords.m_Y][enemy.m_Coords.m_X]->m_Effect->m_Duration = enemy.m_EnemyEffect->m_Duration;
        }
    }

    m_EndOfRound = false;
    m_Interface->updateHealth(m_Health);
    m_Interface->updateMoney(m_Money);
    m_Interface->updateScore(m_Score);
    delete savedGame;
    return true;
}

void CGame::start() {
    m_AssetLoader = new CAssetLoader();
    loadAssets();
    m_Interface = new CInterface(m_TowerTypes, m_Money, m_TermWidth, m_TermHeight);
    if (!loadGame()) {
        chooseMap();
        findPath();
    }

    //Main game loop.
    while (!m_CloseGame) {
        if (m_EndOfRound) {
            prepareGame();
            m_EndOfRound = false;
        }
        //time_t start = time(nullptr);

        //gets user input and inserts tower
        getInput();
        //moves enemies, executes attacks
        update();
        //updates map
        m_Interface->updateMap(m_LogicMap);
        //renders scene
        m_Interface->render();

        //sleep(1);
        //usleep(((double)(start  - time(nullptr)) + MS_PER_FRAME) * 1000000);
    }
}
