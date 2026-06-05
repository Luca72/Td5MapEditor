CXX = g++

CXXFLAGS = -std=c++17 -funsigned-char -D__LINUX_X86_64__ -Wall -Wextra \
           -Wno-unused-parameter -Wno-unused-variable -Wno-unused-but-set-variable \
           $(shell wx-config --cflags)		   

LDFLAGS = $(shell wx-config --libs)

SRC = $(wildcard *.cpp) $(wildcard tinyxml/*.cpp)

DEBUG_DIR = ../linux_x64/debug
RELEASE_DIR = ../linux_x64/release

DEBUG_OBJ_DIR = $(DEBUG_DIR)/obj/vscode
RELEASE_OBJ_DIR = $(RELEASE_DIR)/obj/vscode

DEBUG_OBJ = $(patsubst %.cpp,$(DEBUG_OBJ_DIR)/%.o,$(SRC))
RELEASE_OBJ = $(patsubst %.cpp,$(RELEASE_OBJ_DIR)/%.o,$(SRC))

debug: CXXFLAGS += -g -O0
debug: $(DEBUG_DIR)/td5mapeditor

release: CXXFLAGS += -O2
release: $(RELEASE_DIR)/td5mapeditor

$(DEBUG_DIR)/td5mapeditor: $(DEBUG_OBJ)
	mkdir -p $(DEBUG_DIR)
	$(CXX) $^ $(LDFLAGS) -o $@

$(RELEASE_DIR)/td5mapeditor: $(RELEASE_OBJ)
	mkdir -p $(RELEASE_DIR)
	$(CXX) $^ $(LDFLAGS) -s -o $@

$(DEBUG_OBJ_DIR)/%.o: %.cpp
	mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(RELEASE_OBJ_DIR)/%.o: %.cpp
	mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -rf $(DEBUG_OBJ_DIR) $(RELEASE_OBJ_DIR)
	rm -f $(DEBUG_DIR)/td5mapeditor $(RELEASE_DIR)/td5mapeditor