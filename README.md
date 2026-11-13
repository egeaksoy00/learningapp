# CodePath Studio

CodePath Studio is an offline C and C++ learning application built with C++17 and raylib.
The application and curriculum are contained in src/main.cpp.

- 22 lessons across C, C++, and exam practice, including the four original topics
- Lesson, path, and mixed assessments with hints, explanations, typed output predictions,
  results, retries, and review of missed questions
- Six visual labs covering pointer aliasing, arrays, allocation lifetime, recursion,
  linked insertion, and bubble sort
- A syntax notebook with line numbers, indentation, selection, clipboard, and undo/redo
- Actual C17 and C++17 compilation, diagnostics, and a four-case coding challenge on
  Windows with Visual Studio 2022
- Local progress, per-path statistics, XP, and mastery achievements

Visual labs model fixed examples and use symbolic addresses. Notebook edits stay in
memory for the current session. After building, the app works offline without an account.

Use **F11** or the sidebar fullscreen button to enter and leave borderless
fullscreen. The diagrams, typography, and controls scale with the window.

In Visual Labs, click a step in the guide to jump to it. Bubble Sort has
four input presets and shows comparisons separately from swaps. The sidebar
motion control also applies to lab transitions. On Windows, open the sum
exercise directly from the Overview using **Coding challenge**.

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

### Compile programs in the notebook

Launch the executable from an **x64 Developer PowerShell for VS 2022** so
the compiler and Windows SDK environment are available. In Code notebook,
choose C17 or C++17, load Hello sample, and select Build and run.
Sum challenge supplies four inputs and checks the program's numeric output.
The free-form runner closes standard input. Lesson excerpts may need includes
and a main function before they can be compiled as programs.

Run only code you trust. A Windows job object manages child process cleanup,
and the runner stops on time or output limits. The program still runs with
your account's permissions. This is not a sandbox. The macOS notebook supports
editing and copying code for use with an external compiler.

## Verify the build

Pass --self-test to the executable to check curriculum data, quiz resets,
editor history, and progress save/load with a temporary profile.
From an x64 Developer PowerShell, pass --self-test-compiler to verify C and C++
output, all four challenge cases, compiler error capture, and the execution timeout.

The app saves mastery, attempts, and the last lesson in CodePathStudio/progress-v1.txt
under the local application data folder on MSVC Windows or the home directory on macOS.
