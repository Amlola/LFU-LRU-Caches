SRC_DIR_CPP := source
BUILD_DIR := build

CXX := g++
CXXFLAGS := -O2 -Wall -Wextra -std=c++17

DEBUG_FLAGS := -g -O0 -fsanitize=address,undefined

OUTPUT := main

SRC_CPP_FILES := $(wildcard $(SRC_DIR_CPP)/*.cpp)
OBJ_CPP_FILES := $(patsubst $(SRC_DIR_CPP)/%.cpp, $(BUILD_DIR)/%.o, $(SRC_CPP_FILES))

$(BUILD_DIR)/$(OUTPUT): $(OBJ_CPP_FILES)
	$(CXX) $(OBJ_CPP_FILES) -o $@

$(BUILD_DIR)/%.o: $(SRC_DIR_CPP)/%.cpp
	@mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

.PHONY: clean
clean:
	rm -rf $(BUILD_DIR)

.PHONY: print
print:
	@echo "CPP source files: $(SRC_CPP_FILES)"
	@echo "CPP object files: $(OBJ_CPP_FILES)"