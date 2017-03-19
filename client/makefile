# Get platform of OS.
UNAME := $(shell uname)

# Set up where `love` binary is located.
ifeq ($(UNAME), Darwin)
	LOVE = bin/love/love.app/Contents/MacOS/love
endif

ifeq ($(UNAME), Linux)
	LOVE = love
endif

# Set up where `lua` binary is located.
LUA = lua

# Source folders.
LOVE_MAIN_SRC_DIR = src
LOVE_TEST_DIR = tests

# Targets
# $ make target
# e.g.
# $ make
# $ make run
# $ make test

all:
	$(LOVE) $(LOVE_MAIN_SRC_DIR)

run: all

test:
	cd $(LOVE_MAIN_SRC_DIR) && $(LUA) $(LOVE_TEST_DIR)/test_suite.lua
