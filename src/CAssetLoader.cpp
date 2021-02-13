/**
 * @author Jakub Kučera <kucerj56@fit.cvut.cz>
 * @date 30.04.2020
 */
#include <stdexcept>
#include <sstream>
#include <iterator>
#include <algorithm>
#include "CAssetLoader.hpp"

CAssetLoader::~CAssetLoader() noexcept{
    for (const auto &formatter : SAVE_FILE_IDS_MAP) {
        delete formatter.second;
    }
}

bool CAssetLoader::checkStream() const {
    return inputStream.bad()
           || !inputStream.is_open()
           || inputStream.bad()
           || inputStream.fail()
           || !inputStream.good();
}

uint8_t CAssetLoader::getColor(const std::string &color) const {
    auto it = m_colorTable.find(color);
    if (it == m_colorTable.end()) {
        throw std::out_of_range("Color not found");
    }
    return it->second;
}

std::vector<CMap> CAssetLoader::loadMap() const {
    char name[32];
    std::vector<CMap> maps;
    std::vector<std::string> map;
    std::string line;
    int health, money;

    inputStream.open(FILE_MAP);
    if (checkStream()) {
        throw std::runtime_error("Cannot open map file");
    }

    while (getline(inputStream, line)) {
        if (line[0] == '-') {
            sscanf(line.c_str(), " -%[^-]- %d- %d", name, &health, &money);
            maps.emplace_back(map, name, health, money);
            map.clear();
        } else {
            if (!map.empty()) {
                if (map[0].length() != line.length()) {
                    throw std::runtime_error("Incorrect map format");
                }
            }
            map.push_back(line);
        }
    }

    inputStream.close();
    return maps;
}

std::vector<CEnemy> CAssetLoader::loadEnemy() const {
    char character, name[32], type[32], color[32];
    int health;
    std::vector<CEnemy> enemies;
    std::string line;

    inputStream.open(FILE_ENEMY);
    if (checkStream()) {
        throw std::runtime_error("Cannot open enemy file");
    }

    int i = 0;
    while (getline(inputStream, line)) {
        sscanf(line.c_str(), " %c; %[^;]; %[^;]; %[^;]; %d", &character, color, name, type, &health);
        enemies.emplace_back(character, getColor(color), name, type, i++, health);
    }

    inputStream.close();
    return enemies;
}

std::vector<CTower> CAssetLoader::loadTower() const {
    char character, name[32], type[32], color[32], attackZone[8];
    int cost, damage, range, attackSpeed;
    std::vector<CTower> towers;
    std::string line;

    inputStream.open(FILE_TOWER);
    if (checkStream()) {
        throw std::runtime_error("Cannot open tower file. Make sure, that you start this program from the main folder.");
    }
    int i = 0;
    while (getline(inputStream, line)) {
        sscanf(line.c_str(), " %c; %[^;]; %[^;]; %[^;]; %[^;]; %d; %d; %d; %d", &character, color, name, type,
               attackZone, &cost, &damage, &range, &attackSpeed);
        towers.emplace_back(character, getColor(color), name, type, attackZone, cost, damage, range, i++, attackSpeed);
    }
    inputStream.close();
    return towers;
}

bool CAssetLoader::saveGame(CLoadedGame *savedGame, const std::string &file) const {
    std::cout << "FILE" << file << std::endl;
    std::ofstream out(file, std::ofstream::out);

    if (!out) {
        std::cout << "File " << file << "couldn't have been properly opened.";
        return false;
    }
    out << ID_MAP << savedGame->m_MapIndex << ID_MAP << '\n'
        << ID_MONEY << savedGame->m_Money << ID_MONEY << '\n'
        << ID_HEALTH << savedGame->m_Health << ID_HEALTH << '\n'
        << ID_SCORE << savedGame->m_Score.first << ID_SCORE << savedGame->m_Score.second << ID_SCORE << '\n';
    for (const auto &pathCoords : savedGame->m_Path) {
        out << ID_PATH << pathCoords.m_X << ID_PATH << pathCoords.m_Y << ID_PATH << '\n';
    }
    for (const auto &tower : savedGame->m_Towers) {
        out << ID_TOWER << tower.first.m_X
            << ID_TOWER << tower.first.m_Y
            << ID_TOWER << tower.second->getId()
            << ID_TOWER << tower.second->m_AttackCharge
            << ID_TOWER << '\n';
    }
    for (const auto &enemy : savedGame->m_Enemies) {
        out << ID_ENEMY << enemy.first.m_X
            << ID_ENEMY << enemy.first.m_Y
            << ID_ENEMY << enemy.second->getId()
            << ID_ENEMY << enemy.second->m_Health;
        if (enemy.second->m_Effect != nullptr) {
            out << ID_ENEMY << enemy.second->m_Effect->getTowerId()
                << ID_ENEMY << enemy.second->m_Effect->m_BaseDamage
                << ID_ENEMY << enemy.second->m_Effect->m_Duration;
        }
        out << ID_ENEMY << '\n';
    }
    out.flush();
    out.close();
    return true;
}

CSavedGame *CAssetLoader::loadGame(const std::string &file) const {
    CSavedGame *savedGame = new CSavedGame();
    std::string line;

    inputStream.open(file);
    if (checkStream()) {
        throw std::runtime_error("Cannot open enemy file");
    }

    while (getline(inputStream, line)) {
        for (const auto &itemId : SAVE_FILE_IDS_MAP) {
            if (line.rfind(itemId.first, 0) == 0) {
                std::vector<std::string> substrings = splitString(line, itemId.first);
                CSavedFileFormatter *fileFormatter = itemId.second;
                if (!fileFormatter->loadRecord(savedGame, substrings)) {
                    std::cout << "Corrupted record, skipping record." << std::endl;
                }
                break;
            }
        }
    }
    inputStream.close();
    return savedGame;
}

std::vector<std::string> CAssetLoader::splitString(std::string string, std::string delimiter) const {
    std::vector<std::string> substrings;
    std::string token;
    size_t pos = 0;
    while ((pos = string.find(delimiter)) != std::string::npos) {
        token = string.substr(0, pos);
        if (token.length()) {
            substrings.push_back(token);
        }
        string.erase(0, pos + delimiter.length());
    }
    return substrings;
}
