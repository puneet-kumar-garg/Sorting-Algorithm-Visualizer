CXX := clang++
CXXFLAGS := -std=c++17 -O2 $(shell sdl2-config --cflags)
LDFLAGS := $(shell sdl2-config --libs) -lSDL2_ttf

BIN := bin
SRC := src
TARGET := $(BIN)/sortviz

$(TARGET): $(SRC)/main.cpp | $(BIN)
	$(CXX) $(CXXFLAGS) $< -o $@ $(LDFLAGS)

$(BIN):
	mkdir -p $(BIN)

.PHONY: run clean
run: $(TARGET)
	./$(TARGET)

clean:
	rm -rf $(BIN)