LRU_CACHE := 0
LFU_CACHE := 0
PERFECT_CACHE := 0
DEBUG := 0

CXX := g++
CXXFLAGS := -Wall -Wextra -std=c++17
OPT_FLAGS := -O2

BUILD_DIR := build

ifeq ($(LRU_CACHE), 1)
SRC_DIR_CPP := lru_cache/source
INCLUDE_DIR := lru_cache/include
OUTPUT := lru_cache
endif 

ifeq ($(LFU_CACHE), 1)
SRC_DIR_CPP := lfu_cache/source
INCLUDE_DIR := lfu_cache/include
OUTPUT := lfu_cache
endif 

ifeq ($(PERFECT_CACHE), 1)
SRC_DIR_CPP := perfect_cache/source
INCLUDE_DIR := perfect_cache/include
OUTPUT := perfect_cache
endif 

ifeq ($(DEBUG), 1)
CXXFLAGS += -D DEBUG_CACHE
DEBUG_FLAGS := -g -fsanitize=address,undefined
OPT_FLAGS := -O0
endif

SRC_CPP_FILES := $(wildcard $(SRC_DIR_CPP)/*.cpp)
OBJ_CPP_FILES := $(patsubst $(SRC_DIR_CPP)/%.cpp, $(BUILD_DIR)/%.o, $(SRC_CPP_FILES))

$(BUILD_DIR)/$(OUTPUT): $(OBJ_CPP_FILES)
	$(CXX) $(DEBUG_FLAGS) $(OPT_FLAGS) $(OBJ_CPP_FILES) -o $@

$(BUILD_DIR)/%.o: $(SRC_DIR_CPP)/%.cpp
	@mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) $(DEBUG_FLAGS) $(OPT_FLAGS) -I $(INCLUDE_DIR) -c $< -o $@

.PHONY: clean
clean:
	rm -rf $(BUILD_DIR)

.PHONY: print
print:
	@echo "CPP source files: $(SRC_CPP_FILES)"
	@echo "CPP object files: $(OBJ_CPP_FILES)"