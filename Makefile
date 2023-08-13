TARGET = grachale

CXX = g++
MKDIR = mkdir -p

CXXFLAGS = -Wall -pedantic -Wextra -std=c++11

LDFLAGS= -lSDL2 -lSDL2_image

SOURCE_DIR = src
BUILD_DIR = build

SOURCES = $(wildcard $(SOURCE_DIR)/*.cpp)
HEADERS = $(wildcard $(SOURCE_DIR)/*.h)

OBJECTS = $(SOURCES:$(SOURCE_DIR)/%.cpp=$(BUILD_DIR)/%.o)


.PHONY: all
all: compile doc


.PHONY: compile
compile: $(TARGET)


.PHONY: run
run: $(TARGET)
	./$(TARGET)


$(TARGET): $(OBJECTS)
	$(CXX) $^ -o $@ $(LDFLAGS)


$(BUILD_DIR)/%.o: $(SOURCE_DIR)/%.cpp
	$(MKDIR) $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) $< -c -o $@


doc: Doxyfile README.md $(HEADERS)
	doxygen Doxyfile


.PHONY: clean
clean:
	rm -rf $(TARGET) $(TARGET).zip doc/ .archive/ $(BUILD_DIR)/ 2>/dev/null


build/BlinkyGhost.o: src/BlinkyGhost.cpp src/BlinkyGhost.h src/Ghost.h src/TextureManager.h src/Game.h src/Map.h \
src/TextureManager.h src/Game.h src/Map.h

build/ClydeGhost.o: src/ClydeGhost.cpp src/ClydeGhost.h src/Ghost.h \
src/TextureManager.h src/Game.h src/Map.h

build/Game.o: src/Game.cpp src/Game.h \
src/Pacman.h src/TextureManager.h src/BlinkyGhost.h src/Ghost.h \
src/PinkyGhost.h src/ClydeGhost.h

build/Ghost.o: src/Ghost.cpp src/Ghost.h \
src/TextureManager.h src/Game.h src/Map.h

build/Map.o: src/Map.cpp src/Map.h \
src/TextureManager.h src/Game.h

build/Pacman.o: src/Pacman.cpp src/Pacman.h \
src/TextureManager.h src/Game.h src/Map.h

build/PinkyGhost.o: src/PinkyGhost.cpp src/PinkyGhost.h src/Ghost.h \
src/TextureManager.h src/Game.h src/Map.h

build/TextureManager.o: src/TextureManager.cpp src/TextureManager.h src/Game.h \

build/main.o: src/main.cpp src/Game.h



