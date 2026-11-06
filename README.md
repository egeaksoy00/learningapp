# CodePath - Interactive C/C++ Learning App

CodePath is a raylib starter application for studying C and C++ through short lessons,
visual memory models, and immediate practice feedback. The initial curriculum is organized
around the supplied lecture notes and Coding Exam 1/2 preparation material.

## Included

- Four topics: C Basics, Coding Exam 1, Pointers, and Linked Lists
- A simple lesson screen
- A four-answer quiz screen with hints and feedback
- Small pointer and linked-list drawings

The app intentionally does not call an online AI service yet. The first milestone keeps
feedback deterministic, easy to test, and safe to run offline. A later milestone can add a
provider-neutral tutor interface and connect it to an approved model endpoint.

## Code layout

```text
src/main.cpp    The entire application
```

## Build on macOS

Install raylib once:

```sh
brew install raylib pkg-config
```

Then build and run:

```sh
make
make run
```

You can also use CMake. If raylib is not installed, CMake can fetch raylib 5.5:

```sh
cmake -S . -B build
cmake --build build
./build/codepath-learning
```

## Build on Windows

Install Git, CMake 3.20 or newer, and Visual Studio 2022 with the
**Desktop development with C++** workload (including a Windows SDK).
Run these commands from the project folder in PowerShell:

```powershell
cmake -S . -B build
cmake --build build --config Debug
.\build\Debug\codepath-learning.exe
```

CMake downloads raylib 5.5 automatically if it is not installed. The first
configuration needs an internet connection. After editing `src/main.cpp`,
repeat the build and run commands.

## Good next milestones

1. Add more Coding Exam 1 exercises and unit-test each reference solution.
2. Add the ten pointer-only lab functions as interactive challenges.
3. Animate linked-list insertion and deletion one pointer update at a time.
4. Save progress locally.
5. Add a real AI tutor behind a small interface, with prompts grounded in the selected lesson.

