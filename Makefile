CC := g++
ARCH :=
PLATFORM := win

BUILD_DIR := bin
OUT_DIR = $(BUILD_DIR)/$(PLATFORM)
OUT_NAME := nKaruga
FINAL_EXE = $(OUT_DIR)/$(OUT_NAME)

RELEASE_DIR := releases

SRC_DIR := src

OBJ_DIR = $(OUT_DIR)/obj
OBJS = $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o, $(wildcard $(SRC_DIR)/*.cpp))
OBJS += $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o, $(wildcard $(SRC_DIR)/*.c))

INCLUDE_DIRS = -I./SDL2/$(PLATFORM)/include/SDL2/$(ARCH) -I./SDL2_mixer/$(PLATFORM)/include/SDL2/$(ARCH)
LIB_DIRS = -L./SDL2/$(PLATFORM)/lib/$(ARCH) -L./SDL2_mixer/$(PLATFORM)/lib/$(ARCH)

LINKER_FILES_win := -lmsys-2.0 -lSDL2main -lSDL2 -lSDL2_mixer
LINKER_FILES_nix := -lSDL2 -lSDL2_mixer
FLAGS_win := -fpermissive
FLAGS_nix := -Wl,-rpath,'$$ORIGIN/lib' -fpermissive
WARNINGS := -Wno-write-strings -Wno-pointer-arith -Wno-overflow

windows: $(OBJ_DIR)/icon.o $(FINAL_EXE)
	cp */win/bin/*.dll $(OUT_DIR)

linux: PLATFORM := nix
linux: ARCH := $(shell arch)
linux: $(FINAL_EXE)
	[ -e $(OUT_DIR)/lib ] || mkdir $(OUT_DIR)/lib
	cp */nix/lib/$(ARCH)/*.so.* $(OUT_DIR)/lib

$(FINAL_EXE): $(OBJS)
	@echo Assembling into $@ ...
	$(CC) $(OBJ_DIR)/*.o $(INCLUDE_DIRS) $(LIB_DIRS) $(LINKER_FILES_$(PLATFORM)) -o $@ $(FLAGS_$(PLATFORM)) $(WARNINGS) -std=c++11
	[ -e $(OUT_DIR)/sfx ] || mkdir $(OUT_DIR)/sfx
	cp -r sfx $(OUT_DIR)

$(OBJ_DIR)/%.o: %.rc | $(OBJ_DIR)
	@echo Building icon file $< ...
	windres $< $@
	@echo

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c* | $(OBJ_DIR)
	@echo Building $< to $@ ...
	$(CC) -c $< $(INCLUDE_DIRS) -o $@ $(FLAGS_$(PLATFORM)) $(WARNINGS) -std=c++11
	@echo

$(OBJ_DIR):
	@[ -e $(OBJ_DIR) ] || mkdir -p $(OBJ_DIR)

.PHONY: clean
clean:
	-rm -r $(BUILD_DIR)

deploy: DIR := $(shell pwd)/$(RELEASE_DIR)
deploy:
	@[ -e $(DIR) ] || mkdir -p $(DIR)
	cd $(OUT_DIR) ; zip -r $(DIR)/$(OUT_NAME) ./* -xobj/*

deploy-linux: PLATFORM := nix
deploy-linux: DIR := $(shell pwd)/$(RELEASE_DIR)
deploy-linux:
	@[ -e $(DIR) ] || mkdir -p $(DIR)
	cd $(OUT_DIR) ; zip -r $(DIR)/$(OUT_NAME) ./* -xobj/*