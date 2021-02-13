TARGET =  kucerj56

CXX = g++
CXX_FLAGS = -Wall -pedantic -Wextra -std=c++14

MKDIR = mkdir -p
BUILD_DIR = build
HEADERS = $(wildcard $(SOURCE_DIR)/*.h)

.PHONY: all
all: compile

.PHONY: compile
compile: $(TARGET)

.PHONY:  run
run: $(TARGET)
	./$(TARGET)

#.PHONY:  doc
#doc: doxygen Doxyfile
#doc: Doxyfile README.md $(BUILD_DIR)/*.hpp
#	doxygen Doxyfile
doc: Doxyfile zadani.txt $(wildcard $(SOURCE_DIR)/*.h)
	doxygen Doxyfile


$(TARGET): $(BUILD_DIR)/CArea.o $(BUILD_DIR)/CAssetLoader.o $(BUILD_DIR)/CBFS.o $(BUILD_DIR)/CDFS.o $(BUILD_DIR)/CEnemy.o $(BUILD_DIR)/CGame.o $(BUILD_DIR)/CInterface.o $(BUILD_DIR)/CMap.o $(BUILD_DIR)/CPathFinder.o $(BUILD_DIR)/CTower.o $(BUILD_DIR)/main.o
	$(CXX) $(CXX_FLAGS) $^ -o $@


$(BUILD_DIR)/%.o: src/%.cpp
	$(MKDIR) $(BUILD_DIR)
	$(CXX) $(CXX_FLAGS) $< -c -o $@

.PHONY: clean
clean:
	rm -rf doc $(TARGET) $(BUILD_DIR)/ 2>/dev/null

#Dependencies
$(BUILD_DIR)/CArea.o: src/CArea.cpp src/CArea.hpp src/CEffect.hpp src/CEffectFire.hpp src/CEffectPoison.hpp src/CEffectBleed.hpp src/CEffectBasic.hpp
$(BUILD_DIR)/CAssetLoader.o: src/CAssetLoader.cpp src/CAssetLoader.hpp src/CTower.hpp src/CArea.hpp src/CEffect.hpp src/CEnemy.hpp src/CMap.hpp src/CLoadedGame.hpp src/CCoordinates.hpp src/CSavedFileFormatter.hpp
$(BUILD_DIR)/CBFS.o: src/CBFS.cpp src/CBFS.hpp src/CPathFinder.hpp src/CCoordinates.hpp
$(BUILD_DIR)/CDFS.o: src/CDFS.cpp src/CDFS.hpp src/CPathFinder.hpp src/CCoordinates.hpp
$(BUILD_DIR)/CEnemy.o: src/CEnemy.cpp src/CEnemy.hpp src/CArea.hpp src/CEffect.hpp
$(BUILD_DIR)/CGame.o: src/CGame.cpp src/CGame.hpp src/CTower.hpp src/CArea.hpp src/CEffect.hpp src/CEnemy.hpp src/CMap.hpp src/CInterface.hpp src/CCoordinates.hpp src/CPathFinder.hpp src/CAssetLoader.hpp src/CLoadedGame.hpp src/CSavedFileFormatter.hpp src/CWall.hpp src/CDFS.hpp src/CBFS.hpp
$(BUILD_DIR)/CInterface.o: src/CInterface.cpp src/CInterface.hpp src/CMap.hpp src/CArea.hpp src/CEffect.hpp src/CTower.hpp src/CWall.hpp
$(BUILD_DIR)/CMap.o: src/CMap.cpp src/CMap.hpp
$(BUILD_DIR)/CPathFinder.o: src/CPathFinder.cpp src/CPathFinder.hpp src/CCoordinates.hpp
$(BUILD_DIR)/CTower.o: src/CTower.cpp src/CTower.hpp src/CArea.hpp src/CEffect.hpp src/CEffectBasic.hpp src/CEffectFire.hpp src/CEffectBleed.hpp src/CEffectPoison.hpp
$(BUILD_DIR)/main.o: src/main.cpp src/CGame.hpp src/CTower.hpp src/CArea.hpp src/CEffect.hpp src/CEnemy.hpp src/CMap.hpp src/CInterface.hpp src/CCoordinates.hpp src/CPathFinder.hpp src/CAssetLoader.hpp src/CLoadedGame.hpp src/CSavedFileFormatter.hpp