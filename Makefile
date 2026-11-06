CXX ?= c++
CXXFLAGS ?= -std=c++17 -Wall -Wextra -Wpedantic -O2

APP := codepath-learning
APP_SOURCE := src/main.cpp
RAYLIB_CFLAGS := $(shell pkg-config --cflags raylib 2>/dev/null)
RAYLIB_LIBS := $(shell pkg-config --libs raylib 2>/dev/null)

.PHONY: all run clean check-raylib

all: check-raylib $(APP)

check-raylib:
	@pkg-config --exists raylib || (echo "raylib was not found. On macOS run: brew install raylib" && exit 1)

$(APP): $(APP_SOURCE)
	$(CXX) $(CXXFLAGS) $(RAYLIB_CFLAGS) $(APP_SOURCE) -o $@ $(RAYLIB_LIBS)

run: all
	./$(APP)

clean:
	rm -f $(APP)
	rm -rf build

