CXX = g++

JOBS ?= $(shell n=$$(nproc); j=$$((n / 2)); [ $$j -lt 1 ] && j=1; echo $$j)

ifeq ($(filter -j%,$(MAKEFLAGS)),)
	MAKEFLAGS += -j$(JOBS)
endif

WX_CXXFLAGS := $(shell wx-config --cflags)
WX_LIBS     := $(shell wx-config --libs)

CXXFLAGS = -std=c++17 -funsigned-char -D__LINUX_X86_64__ -Wall -Wextra \
           -Wno-unused-parameter -Wno-unused-variable -Wno-unused-but-set-variable \
           $(WX_CXXFLAGS)

LDFLAGS = $(WX_LIBS)

DEPFLAGS = -MMD -MP

SRC = $(wildcard src/*.cpp) $(wildcard src/tinyxml/*.cpp)

DEBUG_DIR = linux_x64/debug
RELEASE_DIR = linux_x64/release

DEBUG_OBJ_DIR = $(DEBUG_DIR)/obj/vscode
RELEASE_OBJ_DIR = $(RELEASE_DIR)/obj/vscode

DEBUG_OBJ = $(patsubst %.cpp,$(DEBUG_OBJ_DIR)/%.o,$(SRC))
RELEASE_OBJ = $(patsubst %.cpp,$(RELEASE_OBJ_DIR)/%.o,$(SRC))

.PHONY: debug release clean

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
	$(CXX) $(CXXFLAGS) $(DEPFLAGS) -c $< -o $@

$(RELEASE_OBJ_DIR)/%.o: %.cpp
	mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $(DEPFLAGS) -c $< -o $@

clean:
	rm -rf $(DEBUG_OBJ_DIR) $(RELEASE_OBJ_DIR)
	rm -f $(DEBUG_DIR)/td5mapeditor $(RELEASE_DIR)/td5mapeditor

-include $(DEBUG_OBJ:.o=.d)
-include $(RELEASE_OBJ:.o=.d)