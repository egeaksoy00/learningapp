#include "raylib.h"


struct Topic {
    const char* title;
    const char* lesson;
    const char* code;
    const char* question;
    const char* answers[4];
    int correctAnswer;
    const char* hint;
};

Topic topics[] = {
    {
        "C Basics",
        "Variables store values. Conditions choose a path.\n"
        "Loops repeat code. Arrays store many values of one type.",
        "int total = 0;\n\n"
        "for (int i = 1; i <= 5; i++) {\n"
        "    total = total + i;\n"
        "}",
        "What is the last valid index of int values[5]?",
        {"5", "4", "6", "1"},
        1,
        "Array indexes start at zero."
    },
    {
        "Coding Exam 1",
        "Break a problem into input, steps, and output.\n"
        "Test normal values and boundary values.",
        "bool isLeapYear(int year) {\n"
        "    if (year % 400 == 0) return true;\n"
        "    if (year % 100 == 0) return false;\n"
        "    return year % 4 == 0;\n"
        "}",
        "Why should a prime-number divisor loop not start at 0?",
        {"It is too fast", "Modulo by zero is invalid", "Zero is prime", "Loops start at 1"},
        1,
        "Think about number % 0."
    },
    {
        "Pointers",
        "A pointer stores a memory address.\n"
        "Use & to get an address and * to access its value.",
        "int score = 15;\n"
        "int* pointer = &score;\n\n"
        "*pointer = 20;\n"
        "// score is now 20",
        "What is stored in pointer after int* pointer = &score?",
        {"The value of score", "The address of score", "A new score", "Nothing"},
        1,
        "The & operator means address of."
    },
    {
        "Linked Lists",
        "A node stores data and a pointer to the next node.\n"
        "The final node points to nullptr.",
        "struct Node {\n"
        "    int data;\n"
        "    Node* next;\n"
        "};",
        "What marks the end of a singly linked list?",
        {"A zero value", "The array size", "A nullptr link", "The head node"},
        2,
        "The last node has no next node."
    }
};

const int topicCount = 4;

#include <algorithm>
#include <array>
#include <chrono>
#include <cctype>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <future>
#include <iterator>
#include <numeric>
#include <random>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#define NOUSER
#define NOGDI
#include <windows.h>
#endif

struct Lesson {
    const char* path;
    const char* title;
    const char* body;
    const char* code;
    const char* question;
    std::array<const char*, 4> answers;
    const char* hint;
    const char* explanation;
};
const Lesson extraLessons[] = {
    {"C", "Program structure", "A hosted C program starts in main. A header declares the library functions used below. Compile with a C17 compiler, then run the executable.", "#include <stdio.h>\nint main(void) {\n    puts(\"Hello, C!\");\n    return 0;\n}", "Which function starts a hosted C program?", {"main", "puts", "printf", "include"}, "The runtime calls main.", "Headers provide declarations. They do not start the program."},
    {"C", "Types and input", "An int stores whole numbers. Pass an address to scanf so it can write a value. Check the return count before reading the result.", "int n;\nif (scanf(\"%d\", &n) == 1) {\n    printf(\"%d\\n\", n);\n}", "What does scanf return after one successful conversion?", {"1", "The input value", "The address", "Always zero"}, "Count assignments.", "The return value counts successful assignments."},
    {"C", "Operators and conditions", "Integer division truncates toward zero. Use a conditional to handle input boundaries before dividing.", "int x = 7;\nif (x > 0) {\n    printf(\"%d\", x / 2);\n}", "Predict the output.", {"3", "3.5", "4", "7"}, "Both operands are integers.", "Integer division discards the fractional part."},
    {"C", "Loops and functions", "A function packages reusable behavior. Each call has its own automatic local variables. Update the accumulator once per iteration.", "int sum_to(int n) {\n    int total = 0;\n    for (int i = 1; i <= n; ++i) total += i;\n    return total;\n}", "What does sum_to(4) return?", {"10", "6", "4", "15"}, "Add 1 + 2 + 3 + 4.", "The <= condition includes the final value."},
    {"C", "Arrays and strings", "An array stores consecutive elements. A C string ends with a null character. Reserve space for that terminator.", "char word[] = \"cat\";\nsize_t bytes = sizeof word;", "What is the value of bytes?", {"4", "3", "8", "Depends on int size"}, "Count the terminator.", "The array holds c, a, t and the null character."},
    {"C", "Pointer arithmetic", "Adding one to a pointer advances one element. A pointer may reach one past the same array, but cannot be dereferenced there.", "int values[] = {2, 4, 8};\nint *p = values;\nint value = *(p + 2);", "What is value?", {"8", "4", "2", "Undefined behavior"}, "Start at index zero.", "p + 2 points at the third element."},
    {"C", "Structures and allocation", "A structure groups fields. Check allocation before use and release ownership with free. An automatic pointer can refer to dynamically allocated data.", "struct Item { int value; };\nstruct Item *p = malloc(sizeof *p);\nif (p) {\n    p->value = 42;\n    free(p);\n}", "Which operation ends the allocation's lifetime?", {"free(p)", "p->value = 42", "sizeof *p", "Declaring p"}, "Match malloc with its release function.", "free releases the allocation; aliases must not access it afterward."},
    {"C", "Files and debugging", "Check fopen before using a stream. Read until the read operation fails, then distinguish end of file from an error when needed.", "FILE *in = fopen(\"notes.txt\", \"r\");\nif (in) {\n    int ch;\n    while ((ch = fgetc(in)) != EOF) putchar(ch);\n    fclose(in);\n}", "Why is ch an int?", {"To represent all byte values and EOF", "To make reading faster", "Because files store ints", "To skip spaces"}, "EOF is a special sentinel.", "fgetc returns an unsigned char converted to int or EOF."},
    {"C", "Recursion", "A recursive call solves a smaller instance. A base case stops expansion. Each call retains its own local state until it returns.", "int factorial(int n) {\n    if (n <= 1) return 1;\n    return n * factorial(n - 1);\n}", "What does factorial(4) return?", {"24", "10", "16", "4"}, "Multiply on the way back.", "For this small input the factors are 4, 3, 2 and 1."},
    {"C++", "Streams and references", "C++ supports stream I/O and references. A reference binds to an existing object. Assignment through a reference changes the referenced object.", "int score = 10;\nint& alias = score;\nalias += 5;\nstd::cout << score;", "Predict the output.", {"15", "10", "5", "A memory address"}, "alias names score.", "A reference aliases the original int."},
    {"C++", "Functions and overloading", "Overloads share a name with different parameter lists. Overload resolution prefers an exact match when these two functions are visible.", "int twice(int n) { return n * 2; }\ndouble twice(double n) { return n * 2; }", "Which overload receives twice(2.5)?", {"double", "int", "Both", "Neither"}, "2.5 has type double.", "The double overload needs no conversion."},
    {"C++", "Strings and vectors", "A vector owns a resizable contiguous sequence. A string owns characters. Growing a vector can invalidate pointers and iterators into it.", "std::vector<int> numbers{2, 4};\nnumbers.push_back(6);\nstd::cout << numbers.size();", "Predict the output.", {"3", "2", "6", "12"}, "size counts elements.", "push_back adds one element to the sequence."},
    {"C++", "Classes and invariants", "Use a constructor to establish valid state. A private field can be changed through public methods that enforce the class rules.", "class Counter {\n    int count = 0;\npublic:\n    void tick() { ++count; }\n    int value() const { return count; }\n};", "Which member can a caller use to read count?", {"value()", "count", "tick()", "The destructor"}, "Look for the const getter.", "value is public; count is private."},
    {"C++", "Inheritance and virtual calls", "A virtual call through a base reference selects the override for the actual object. Give polymorphic bases a virtual destructor when deleting through the base type.", "struct Base {\n    virtual ~Base() = default;\n    virtual int id() const { return 1; }\n};\nstruct Derived : Base {\n    int id() const override { return 2; }\n};", "For Derived d; Base& b = d; what is b.id()?", {"2", "1", "0", "Unspecified"}, "The actual object is Derived.", "Virtual dispatch selects Derived::id."},
    {"C++", "Templates and operators", "Templates express behavior over types. Overloaded operators should keep their familiar meaning. This template requires a usable + for T.", "template <class T>\nT add(T a, T b) { return a + b; }\n// add(2, 3) deduces T as int.", "What does add(2, 3) return?", {"5", "23", "An int pointer", "A compiler error"}, "The inputs are ints.", "The instantiated function uses built-in integer addition."},
    {"C++", "Containers and iterators", "An iterator identifies a position. Standard algorithms generally use a half-open range. The end iterator marks the boundary after the last element.", "std::vector<int> data{3, 1, 2};\nstd::sort(data.begin(), data.end());", "Which iterator must not be dereferenced?", {"data.end()", "data.begin()", "Both", "Neither"}, "One marks a boundary.", "end is outside the sequence, even when it is nonempty."},
    {"C++", "Exceptions and RAII", "RAII ties resource ownership to an object's lifetime. During normal exception unwinding, fully constructed automatic objects are destroyed.", "void work() {\n    auto value = std::make_unique<int>(42);\n    throw std::runtime_error(\"work failed\");\n}", "During unwinding to a matching handler, who frees the int?", {"The unique_ptr destructor", "The handler must call delete", "The exception object", "Nobody"}, "Follow the owner.", "The local unique_ptr releases its allocation on destruction."},
    {"C++", "Smart pointers and moves", "unique_ptr expresses exclusive ownership. Moving it transfers ownership. A moved-from unique_ptr is empty after move construction.", "auto first = std::make_unique<int>(9);\nauto second = std::move(first);\nstd::cout << *second;", "Which statement is true?", {"first is empty", "Both own the int", "second is empty", "The int was copied"}, "Only one owner is allowed.", "Ownership moves to second while the int stays in place."},
};

std::vector<Lesson> Curriculum() {
    const char* paths[] = {"C", "EXAM", "C", "C++"};
    const char* explanations[] = {
        "A five-element array has valid indices 0 through 4.",
        "The remainder operator requires a nonzero divisor. Testing divisors can start at 2.",
        "The address-of operator produces the address of score. Dereferencing the pointer accesses score.",
        "A null next pointer means no node follows. The data field can contain any valid value."
    };
    std::vector<Lesson> result;
    for (int i = 0; i < topicCount; ++i) {
        const Topic& t = topics[i];
        std::array<const char*, 4> choices{};
        for (int j = 0; j < 4; ++j) choices[j] = t.answers[(j + t.correctAnswer) % 4];
        result.push_back({paths[i], t.title, t.lesson, t.code, t.question, choices, t.hint, explanations[i]});
    }
    result.insert(result.end(), std::begin(extraLessons), std::end(extraLessons));
    return result;
}

#include "theme.inc"
Font ui{}, mono{}, heading{};
float uiScale=1;
int uiInset=0;

void Text(const char* s, float x, float y, float size, Color color = ink, bool code = false) {
    DrawTextEx(code ? mono : size>=24 ? heading : ui, s, {x,y}, size, 0.5f, color);
}
void Clip(float x,float y,float w,float h) {
    BeginScissorMode(uiInset+static_cast<int>(x*uiScale),static_cast<int>(y*uiScale),
                     static_cast<int>(w*uiScale),static_cast<int>(h*uiScale));
}
bool animateUi=true;
float animationDt=0;

Color Blend(Color from,Color to,float t) {
    return ColorLerp(from,to,std::clamp(t,0.0f,1.0f));
}

float Approach(float value,float target,float rate) {
    return animateUi ? target+(value-target)*std::exp(-rate*animationDt) : target;
}

float Arrival(float t) {
    float remainder=1-std::clamp(t,0.0f,1.0f);
    return 1-remainder*remainder*remainder;
}

void Card(Rectangle r, Color c = panel) {
    DrawRectangleRounded(r,std::min(0.16f,18.0f/std::min(r.width,r.height)),10,c);
    if(c.r==panel.r || c.r==rail.r) DrawRectangleRoundedLinesEx(r,std::min(0.16f,18.0f/std::min(r.width,r.height)),10,1,seam);
}
bool Button(Rectangle r, const char* s, bool active = false, Color tint = led) {
    bool hover = CheckCollisionPointRec(GetMousePosition(),r);
    // Track the hovered surface without moving the input target.
    static Rectangle hoveredRect{};
    static float hoverStart=-10, pressStart=-10;
    const float now=static_cast<float>(GetTime());
    bool same=r.x==hoveredRect.x && r.y==hoveredRect.y && r.width==hoveredRect.width;
    if(hover && !same) { hoveredRect=r; hoverStart=now; pressStart=-10; same=true; }
    if(hover && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) pressStart=now;
    float hoverAmount=hover ? (animateUi ? Arrival((now-hoverStart)/0.18f) : 1.0f) : 0;
    float press=animateUi && same ? 1-Arrival((now-pressStart)/0.26f) : 0;
    Color base=active ? (tint.r==errorColor.r ? errorFill : tint.r==successColor.r ? successFill : tint) : panel;
    Color fill=Blend(base,active ? (base.r>=128 ? WHITE : primary) : hoverSurface,hoverAmount*(active ? 0.12f : 1.0f));
    fill=Blend(fill,base.r>=128 ? onAccent : primary,press*0.16f);
    bool inverse=active && tint.r==led.r && tint.g==led.g;
    const Rectangle hit=r;
    r={r.x+press*1.5f,r.y+press*1.7f,r.width-press*3,r.height-press*3};
    DrawRectangleRounded(r,0.34f,10,fill);
    if(!active || hover) DrawRectangleRoundedLinesEx(r,0.34f,10,1+hoverAmount,Blend(seam,primary,hoverAmount));
    float size=18;
    while(size>13 && MeasureTextEx(ui,s,size,0.5f).x>r.width-18) --size;
    const float labelWidth=MeasureTextEx(ui,s,size,0.5f).x;
    Clip(r.x,r.y,r.width,r.height);
    Text(s,r.x+(r.width-labelWidth)/2,r.y+(r.height-size)/2,size,inverse || (active && tint.r==signal.r) ? onAccent : ink);
    EndScissorMode();
    return CheckCollisionPointRec(GetMousePosition(),hit) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT);
}
void StudioMark(Vector2 p,float size,Color base) {
    DrawRectangleRounded({p.x,p.y,size,size},0.30f,10,base);
    float unit=size/7;
    DrawLineEx({p.x+2*unit,p.y+2*unit},{p.x+3*unit,p.y+3.5f*unit},unit*0.49f,signal);
    DrawLineEx({p.x+3*unit,p.y+3.5f*unit},{p.x+2*unit,p.y+5*unit},unit*0.49f,signal);
    DrawLineEx({p.x+4*unit,p.y+5*unit},{p.x+5.4f*unit,p.y+5*unit},unit*0.49f,signal);
}

void DrawStudioIntro(float w,float h,float time) {
    DrawRectangleRec({0,0,w,h},terminal);
    const float cell=std::clamp(w/10,80.0f,137.0f), stride=cell+22;
    const float left=(w-5*stride+22)/2, row=h*0.50f;
    const float trace=std::clamp((time-0.60f)/1.68f,0.0f,1.0f);
    const float signalX=left+cell/2+4*stride*trace;
    const float busY=row+cell*0.5f;
    DrawLineEx({left,busY},{left+5*stride-22,busY},2,Fade(draft,0.35f));
    DrawLineEx({left,busY},{signalX,busY},3,signal);
    const char* symbols[]={"{ }","&","p","*p","++"};
    for(int i=0;i<5;++i) {
        const float px=left+i*stride;
        const float appear=Arrival((time-0.08f-i*0.13f)/0.68f);
        const float y=row+(1-appear)*54;
        const float cellPhase=std::clamp((trace*4-i)*3+1,0.0f,1.0f);
        const Color fill=Blend(hero,i==4 ? signal : litSurface,cellPhase);
        const Rectangle frame{px,y,cell,cell};
        DrawLineEx({px+cell/2,row-46},{px+cell/2,row+cell+34},1,Fade(draft,0.16f*appear));
        DrawRectangleRounded(frame,0.13f,12,Fade(fill,appear));
        DrawRectangleRoundedLinesEx(frame,0.13f,12,1,Fade(seam,appear));
        const float size=cell*0.30f, tw=MeasureTextEx(mono,symbols[i],size,0.5f).x;
        Text(symbols[i],px+(cell-tw)/2,y+(cell-size)/2,size,Fade(Blend(onDark,i==4 ? onAccent : ink,cellPhase),appear),true);
        Text(TextFormat("0x%03X",0x100+i*4),px,y+cell+12,14,Fade(draft,appear),true);
        if(time>0.8f && trace<1 && std::abs(signalX-(px+cell/2))<stride*0.55f)
            DrawLineEx({px+8,y+cell-8},{px+cell-8,y+cell-8},3,signal);
    }
    if(trace>0 && trace<1) DrawCircleV({signalX,busY},6,signal);
    const float titleReveal=Arrival((time-0.86f)/1.28f);
    const float titleSize=std::min(82.0f,w*0.071f);
    const float titleW=MeasureTextEx(heading,"CodePath Studio",titleSize,0.5f).x;
    Text("CodePath Studio",(w-titleW)/2,h*0.245f+24*(1-titleReveal),titleSize,Fade(onDark,titleReveal));
    const char* tagline="C AND C++ LEARNING WORKSPACE";
    Text(tagline,(w-MeasureTextEx(ui,tagline,18,0.5f).x)/2,h*0.395f,18,Fade(draft,Arrival((time-1.68f)/0.7f)));
    const char* skip="Click or press a key to skip";
    Rectangle badge{w/2-125,h-68,250,41};
    DrawRectangleRounded(badge,0.65f,10,litSurface);
    Text(skip,(w-MeasureTextEx(ui,skip,16,0.5f).x)/2,badge.y+11,16,ink);
}

void NavIcon(int id,Vector2 c,Color color) {
    const float x=c.x,y=c.y;
    if(id==0) {
        DrawLineEx({x-9,y-1},{x,y-9},1.8f,color); DrawLineEx({x,y-9},{x+9,y-1},1.8f,color);
        DrawRectangleLinesEx({x-7,y-2,14,12},1.7f,color);
        DrawRectangleLinesEx({x-2,y+3,4,7},1.3f,color);
    } else if(id==1) {
        DrawLineEx({x,y-7},{x,y+9},1.5f,color);
        DrawRectangleRoundedLinesEx({x-10,y-7,10,15},0.17f,5,1.6f,color);
        DrawRectangleRoundedLinesEx({x,y-7,10,15},0.17f,5,1.6f,color);
    } else if(id==2) {
        DrawCircleLinesV(c,9,color); DrawCircleLinesV(c,5,color);
        DrawLineEx({x,y},{x+9,y-9},1.7f,color);
    } else if(id==3) {
        DrawRectangleRoundedLinesEx({x-5,y-9,10,4},0.25f,5,1.6f,color);
        DrawLineEx({x-3,y-5},{x-3,y},1.5f,color);
        DrawLineEx({x+3,y-5},{x+3,y},1.5f,color);
        DrawLineEx({x-3,y},{x-9,y+8},1.7f,color);
        DrawLineEx({x+3,y},{x+9,y+8},1.7f,color);
        DrawLineEx({x-9,y+8},{x+9,y+8},1.7f,color);
    } else if(id==4) {
        DrawLineEx({x-10,y},{x-4,y-6},1.8f,color);
        DrawLineEx({x-10,y},{x-4,y+6},1.8f,color);
        DrawLineEx({x+10,y},{x+4,y-6},1.8f,color);
        DrawLineEx({x+10,y},{x+4,y+6},1.8f,color);
        DrawLineEx({x+2,y-8},{x-2,y+8},1.8f,color);
    } else {
        DrawCircleLinesV({x,y-7},3,color);
        DrawCircleLinesV({x-8,y+7},3,color);
        DrawCircleLinesV({x+8,y+7},3,color);
        DrawLineEx({x-1,y-4},{x-7,y+4},1.5f,color);
        DrawLineEx({x+1,y-4},{x+7,y+4},1.5f,color);
        DrawLineEx({x-5,y+7},{x+5,y+7},1.5f,color);
    }
}
float Wrap(const char* s, float x, float y, float width, float size, Color color = muted) {
    std::istringstream words(s);
    std::string word, line;
    while (words >> word) {
        std::string next = line.empty() ? word : line + " " + word;
        if (!line.empty() && MeasureTextEx(size>=24 ? heading : ui, next.c_str(), size, 0.5f).x > width) {
            Text(line.c_str(), x, y, size, color);
            y += size + 5;
            line = word;
        } else line = next;
    }
    if (!line.empty()) Text(line.c_str(), x, y, size, color);
    return y + size + 5;
}
void Bar(float x, float y, float w, float fill) {
    DrawRectangleRec({x,y,w,5}, seam);
    DrawRectangleRec({x,y,w * std::clamp(fill, 0.0f, 1.0f),5}, led);
}

void InstrumentHeading(const char* name,float x,float y,float width) {
    std::string label(name);
    for(size_t i=1;i<label.size();++i)
        label[i]=static_cast<char>(std::tolower(static_cast<unsigned char>(label[i])));
    Text(label.c_str(),x,y,17,ink);
    DrawRectangleRec({x,y+29,width,1},Fade(seam,0.72f));
}

void MasteryRing(Vector2 center,float fraction) {
    DrawRing(center,58,65,0,360,96,seam);
    if(fraction>0) DrawRing(center,58,65,-90,-90+360*fraction,96,accent);
    DrawCircleV({center.x+61.5f*std::cos((360*fraction-90)*DEG2RAD),
                 center.y+61.5f*std::sin((360*fraction-90)*DEG2RAD)},4,led);
    const char* label=TextFormat("%d%%",static_cast<int>(fraction*100+0.5f));
    float size=MeasureTextEx(heading,label,26,0.5f).x;
    Text(label,center.x-size/2,center.y-13,26);
}

struct Score { int attempts = 0, correct = 0; };
void ReadProgress(const std::filesystem::path& file, std::vector<Score>& scores, int& lesson) {
    std::ifstream in(file);
    std::string version;
    int index;
    if (!(in >> version >> index) || version != "CODEPATH1") return;
    std::vector<Score> data(scores.size());
    for (auto& score : data) {
        if (!(in >> score.attempts >> score.correct) || score.attempts < 0 ||
            score.attempts > 1000000 || score.correct < 0 || score.correct > score.attempts) return;
    }
    scores = data;
    lesson = std::clamp(index, 0, static_cast<int>(scores.size()) - 1);
}
bool WriteProgress(const std::filesystem::path& file, const std::vector<Score>& scores, int lesson) {
    std::error_code error;
    std::filesystem::create_directories(file.parent_path(), error);
    if (error) return false;
    auto temp = file;
    temp += ".tmp";
    std::ofstream out(temp);
    out << "CODEPATH1 " << lesson << '\n';
    for (const auto& score : scores) out << score.attempts << ' ' << score.correct << '\n';
    out.close();
    if (!out) return false;
    std::filesystem::rename(temp, file, error);
    return !error;
}

struct QuizState {
    std::vector<int> questions, mistakes;
    std::array<int,4> order{0,1,2,3};
    size_t index = 0;
    int selected = -1, correct = 0;
    bool hint = false;
    std::string prediction;
    void Advance(std::mt19937& rng) {
        ++index;
        selected = -1;
        hint = false;
        prediction.clear();
        std::shuffle(order.begin(), order.end(), rng);
    }
    void Start(std::vector<int> pool, std::mt19937& rng) {
        questions = std::move(pool);
        mistakes.clear();
        index = 0;
        selected = -1;
        hint = false;
        prediction.clear();
        correct = 0;
        std::shuffle(order.begin(), order.end(), rng);
    }
};


struct Lab {
    const char* name;
    const char* description;
    std::vector<const char*> steps;
};
const Lab visualLabs[] = {
    {"Pointer alias", "Symbolic addresses show which object the pointer refers to.",
     {"int score = 15;", "int* p = &score;", "*p = 20;", "p = nullptr;"}},
    {"Array traversal", "The accumulator starts at zero. Read each consecutive element once.",
     {"int a[] = {2, 4, 8};", "total += a[0];", "total += a[1];", "total += a[2];"}},
    {"Heap lifetime", "The pointer is local. The allocation lives until it is freed.",
     {"int *p = NULL;", "p = malloc(sizeof *p);  // assume success", "*p = 42;", "free(p);", "p = NULL;"}},
    {"Recursive calls", "Each active call has its own n. Frames disappear as calls return.",
     {"factorial(3)", "factorial(2)", "factorial(1)", "return 1", "return 2", "return 6"}},
    {"Linked insertion", "Initialize the new node's link before changing the head node's next pointer.",
     {"10 -> 30 -> nullptr", "Create node 20", "20.next points to 30", "10.next points to 20"}},
    {"Bubble sort", "Adjacent comparisons move the largest element right during the first pass.",
     {"Start with [5, 2, 4]", "Compare 5 and 2, then swap", "Compare 5 and 4, then swap", "Compare 2 and 4, no swap"}}
};
void Arrow(float x,float y,float end,float endY,float scale);
#include "memory_board.inc"
#include "scenes.inc"
#include "lesson_explorer.inc"
#include "pointer_lab.inc"

struct SortFrame {
    std::array<int,3> values;
    int left=-1;
    bool swapped=false;
};

const char* LabCaption(int lab,int step) {
    static const char* const captions[][6]={
        {"Create score and store 15. Its address is symbolic.",
         "Store the address of score in p. Both names can now reach the same object.",
         "Dereference p and write 20. The original score changes.",
         "Clear p. The local score still exists."},
        {"Start with three elements and a total of zero.","Read the first element and add it.",
         "Advance one element and add its value.","Add the last element. Stop before the end boundary."},
        {"Enter the function. The local pointer starts empty.",
         "Allocate storage on the heap. The local pointer keeps its address.",
         "Write through the pointer before reading the allocated value.",
         "Release the heap block. Access through the old pointer is invalid.",
         "Clear the local pointer. Returning from the function removes its frame."},
        {"Call factorial with n = 3.","Suspend the caller and create a frame for n = 2.",
         "Reach the base case. This call needs no further calls.",
         "Return 1 and remove the base-case frame.",
         "Multiply 2 by the returned 1, then return 2.",
         "Multiply 3 by 2. The final result is 6 and the stack unwinds."},
        {"The head points directly to the tail.","Create a detached node.",
         "Link the new node to the tail first.","Redirect the head link. The suffix is preserved."}
    };
    return captions[lab][step];
}

std::vector<SortFrame> SortTrace(std::array<int,3> values) {
    std::vector<SortFrame> trace{{values}};
    for(int end=2;end>0;--end) {
        bool changed=false;
        for(int left=0;left<end;++left) {
            trace.push_back({values,left,false});
            if(values[left]>values[left+1]) {
                std::swap(values[left],values[left+1]);
                trace.push_back({values,left,true});
                changed=true;
            }
        }
        if(!changed) break;
    }
    trace.push_back({values});
    return trace;
}

std::string SortStep(const std::vector<SortFrame>& trace,int step) {
    const auto& frame=trace[step];
    if(step==0) return "Start with the input order";
    if(step+1==static_cast<int>(trace.size())) return "Sorted in ascending order";
    return std::string(frame.swapped ? "Swap positions " : "Compare positions ")+
           std::to_string(frame.left)+" and "+std::to_string(frame.left+1);
}

void DrawSort(const std::vector<SortFrame>& trace,int step,float transition,Rectangle stage) {
    const auto& frame=trace[step];
    float gap=std::min(24.0f,stage.width*0.05f);
    float barWidth=std::min(120.0f,(stage.width-4*gap)/3);
    float left=stage.x+(stage.width-3*barWidth-2*gap)/2;
    float base=stage.y+stage.height-82;
    float available=std::max(70.0f,stage.height-177);
    DrawLineEx({left-gap,base},{left+3*barWidth+3*gap,base},2,muted);
    for(int i=0;i<3;++i) {
        bool selected=frame.left>=0 && (i==frame.left || i==frame.left+1);
        int from=frame.swapped && selected ? 2*frame.left+1-i : i;
        float position=from+(i-from)*transition;
        float x=left+position*(barWidth+gap);
        float height=available*(0.22f+0.75f*frame.values[i]/9.0f);
        Color color=selected ? (frame.swapped ? led : codeType) : panel;
        DrawRectangleGradientV(static_cast<int>(x),static_cast<int>(base-height),static_cast<int>(barWidth),
                               static_cast<int>(height),color,Fade(color,0.42f));
        Text(TextFormat("%d",frame.values[i]),x+barWidth/2-9,base-height-32,24,ink,true);
        Text(TextFormat("[%d]",i),left+i*(barWidth+gap)+barWidth/2-14,base+12,16,muted,true);
    }
}
void Cell(float x, float y, const char* label, const char* value, bool active, float scale) {
    Card({x,y,125*scale,74*scale}, active ? litSurface : panel);
    DrawRectangleRec({x+10*scale,y+63*scale,105*scale,2*scale},active ? led : seam);
    Text(label,x+10*scale,y+9*scale,15*scale,muted);
    Text(value,x+10*scale,y+32*scale,22*scale,active ? led : ink,true);
}
void Arrow(float x, float y, float end, float endY, float scale) {
    Vector2 a{x,y}, b{end,endY};
    float angle = std::atan2(endY-y,end-x);
    float tip=10*scale;
    Vector2 left{end-tip*std::cos(angle-0.45f),endY-tip*std::sin(angle-0.45f)};
    Vector2 right{end-tip*std::cos(angle+0.45f),endY-tip*std::sin(angle+0.45f)};
    DrawLineEx(a,b,2.5f*scale,led);
    DrawTriangle(b,right,left,led);
}
void Model(int lab, int step, Rectangle bounds, float transition, const std::array<int,3>& arrayValues) {
    const float widths[]={345,415,345,415,563,415};
    const float heights[]={74,121,74,121,160,74};
    float scale=std::min({1.8f,(bounds.width-36)/widths[lab],(bounds.height-56)/heights[lab]});
    float originX=bounds.x+(bounds.width-widths[lab]*scale)/2;
    float originY=bounds.y+(bounds.height-heights[lab]*scale)/2+10;
    const float x=0,y=0;
    auto Cell=[&](float cx,float cy,const char* label,const char* value,bool active=false) {
        ::Cell(originX+cx*scale,originY+cy*scale,label,value,active,scale);
    };
    auto Arrow=[&](float ax,float ay,float bx,float by) {
        float progress=0.25f+0.75f*transition;
        ::Arrow(originX+ax*scale,originY+ay*scale,
                originX+(ax+(bx-ax)*progress)*scale,originY+(ay+(by-ay)*progress)*scale,scale);
    };
    auto Text=[&](const char* s,float tx,float ty,float size,Color color,bool code=false) {
        ::Text(s,originX+tx*scale,originY+ty*scale,size*scale,color,code);
    };
    if (lab == 0 || lab == 2) {
        const char* pointer[] = {"--","0x100","0x100","nullptr"};
        const char* heap[] = {"NULL","0x800","0x800","expired","NULL"};
        Cell(x,y,"local pointer",lab == 0 ? pointer[step] : heap[step],step == 1 || step == 2);
        Cell(x+220,y,lab == 0 ? "score / 0x100" : "heap / 0x800",
             lab == 0 ? (step >= 2 ? "20" : "15") : (step == 1 ? "unset" : step == 2 ? "42" : "no object"),step == 2);
        if (step == 1 || step == 2) Arrow(x+130,y+40,x+214,y+40);
    } else if (lab == 1 || lab == 5) {
        const int sorted[][3] = {{5,2,4},{2,5,4},{2,4,5},{2,4,5}};
        for (int i=0;i<3;++i) {
            Cell(x+i*145,y,TextFormat("a[%d]",i),TextFormat("%d",lab == 1 ? arrayValues[i] : sorted[step][i]),
                 lab == 1 ? step == i+1 : step == 3);
        }
        if (lab == 1) {
            int total=std::accumulate(arrayValues.begin(),arrayValues.begin()+step,0);
            Text(TextFormat("Accumulator = %d",total),x,y+96,20,led);
        }
    } else if (lab == 3) {
        const int frames[] = {1,2,3,2,1,0};
        const int returns[] = {1,2,6};
        for (int i=0;i<frames[step];++i) Cell(x+i*145,y,"stack frame",TextFormat("n = %d",3-i),i+1==frames[step]);
        if (step >= 3) Text(TextFormat("Returned %d",returns[step-3]),x,y+96,20,led);
    } else {
        Cell(x,y,"head","10");
        Cell(x+320,y,"tail","30");
        if (step >= 1) Cell(x+160,step == 3 ? y+86*(1-transition) : y+86,"new node","20",true);
        if (step == 3) {
            float newY=y+37+86*(1-transition);
            Arrow(x+128,y+37,x+156,newY);
            Arrow(x+288,newY,x+316,y+37);
        } else {
            Arrow(x+128,y+37,x+316,y+37);
            if (step == 2) Arrow(x+286,y+116,x+325,y+71);
        }
        Arrow(x+449,y+37,x+482,y+37);
        Text("nullptr",x+490,y+28,17,muted,true);
    }
}

void CodeLine(const std::string& line, float x, float y, float size) {
    const std::string keywords = " int char void bool double struct class return if for while const auto using include "
                                 "public private virtual override template throw nullptr sizeof static unsigned ";
    size_t i = 0;
    while (i < line.size()) {
        size_t start = i++;
        char c = line[start];
        Color color = ink;
        if (c == '/' && i < line.size() && line[i] == '/') {
            i = line.size();
            color = muted;
        } else if (c == '"' || c == '\'') {
            while (i < line.size()) {
                if (line[i] == '\\' && i + 1 < line.size()) i += 2;
                else if (line[i++] == c) break;
            }
            color = codeString;
        } else if ((c >= '0' && c <= '9') || (c >= 'a' && c <= 'z') ||
                   (c >= 'A' && c <= 'Z') || c == '_') {
            i = line.find_first_not_of("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789_",i);
            if (i == std::string::npos) i = line.size();
            if (c >= '0' && c <= '9') color = codeNumber;
            else if (keywords.find(" " + line.substr(start,i-start) + " ") != std::string::npos) color = codeType;
        }
        std::string token = line.substr(start,i-start);
        Text(token.c_str(),x,y,size,color,true);
        x += MeasureTextEx(mono,token.c_str(),size,0.5f).x+0.5f;
    }
}
void Example(const char* code, Rectangle r, float size = 16) {
    Card(r,rail);
    Clip(r.x+4,r.y+4,r.width-8,r.height-8);
    std::istringstream measure(code);
    std::string sample;
    float longest=0;
    while(std::getline(measure,sample))
        longest=std::max(longest,MeasureTextEx(mono,sample.c_str(),size,0.5f).x);
    if(longest>r.width-30) size=std::max(12.0f,size*(r.width-30)/longest);
    std::istringstream lines(code);
    std::string line;
    float y = r.y+12;
    while (std::getline(lines,line)) { CodeLine(line,r.x+13,y,size); y += size+4; }
    EndScissorMode();
}

struct Editor {
    std::string code;
    size_t cursor = 0, anchor = 0;
    int scroll = 0;
    float horizontal = 0;
    struct Revision { std::string code; size_t cursor, anchor; };
    std::vector<Revision> undo, redo;
    bool dragging=false;

    void Replace(const std::string& text) {
        size_t first = std::min(cursor,anchor), count = std::max(cursor,anchor)-first;
        if (code.size()-count+text.size() > 12000 || (count == 0 && text.empty())) return;
        if (undo.size() == 50) undo.erase(undo.begin());
        undo.push_back({code,cursor,anchor});
        redo.clear();
        code.replace(first,count,text);
        cursor = anchor = first+text.size();
    }
    void History(std::vector<Revision>& from, std::vector<Revision>& to) {
        if (from.empty()) return;
        to.push_back({code,cursor,anchor});
        code = std::move(from.back().code);
        cursor=from.back().cursor;
        anchor=from.back().anchor;
        from.pop_back();
    }
    size_t Start(size_t at) const {
        if (at == 0) return 0;
        size_t pos = code.rfind('\n',at-1);
        return pos == std::string::npos ? 0 : pos+1;
    }
    size_t End(size_t at) const {
        size_t pos = code.find('\n',at);
        return pos == std::string::npos ? code.size() : pos;
    }
    float Prefix(size_t start, size_t length) const {
        if (!length) return 0;
        return MeasureTextEx(mono,code.substr(start,length).c_str(),19,0.5f).x+0.5f;
    }
    void Draw(Rectangle r) {
        constexpr float rowHeight = 27;
        int rows = static_cast<int>((r.height-18)/rowHeight);
        auto rowCount = [&] { return static_cast<int>(std::count(code.begin(),code.end(),'\n'))+1; };
        auto pressed = [](int key) { return IsKeyPressed(key) || IsKeyPressedRepeat(key); };
        bool ctrl = IsKeyDown(KEY_LEFT_CONTROL) || IsKeyDown(KEY_RIGHT_CONTROL) ||
                    IsKeyDown(KEY_LEFT_SUPER) || IsKeyDown(KEY_RIGHT_SUPER);
        bool shift = IsKeyDown(KEY_LEFT_SHIFT) || IsKeyDown(KEY_RIGHT_SHIFT);
        Vector2 mouse = GetMousePosition();
        if (CheckCollisionPointRec(mouse,r)) {
            Vector2 wheel = GetMouseWheelMoveV();
            if (!shift) scroll -= static_cast<int>(wheel.y*3);
            horizontal = std::max(0.0f,horizontal-(wheel.x+(shift ? wheel.y : 0))*55);
            scroll = std::clamp(scroll,0,std::max(0,rowCount()-rows));
            bool click=IsMouseButtonPressed(MOUSE_BUTTON_LEFT);
            if (click || (dragging && IsMouseButtonDown(MOUSE_BUTTON_LEFT))) {
                int row = scroll+std::max(0,static_cast<int>((mouse.y-r.y-9)/rowHeight));
                size_t start = 0;
                while (row-- > 0 && End(start)<code.size()) start = End(start)+1;
                cursor = start;
                size_t end = End(start);
                float target = mouse.x-r.x-54+horizontal;
                while (cursor<end && target >= (Prefix(start,cursor-start)+Prefix(start,cursor-start+1))/2) ++cursor;
                if (click && !shift) anchor = cursor;
                dragging=true;
            }
        }
        if(!IsMouseButtonDown(MOUSE_BUTTON_LEFT)) dragging=false;
        size_t oldCursor = cursor;
        std::string oldCode = code;
        if (ctrl) {
            if (pressed(KEY_A)) { anchor=0; cursor=code.size(); }
            if (pressed(KEY_C) || pressed(KEY_X)) {
                size_t start=std::min(cursor,anchor);
                SetClipboardText(code.substr(start,std::max(cursor,anchor)-start).c_str());
                if (pressed(KEY_X)) Replace("");
            }
            if (pressed(KEY_V)) {
                const char* input=GetClipboardText();
                std::string cleaned;
                if (input) {
                    for (size_t i=0;input[i] && i<12001;++i) {
                        unsigned char c=static_cast<unsigned char>(input[i]);
                        if (c=='\t') cleaned+="    ";
                        else if (c=='\n' || (c>=32 && c<=126)) cleaned+=static_cast<char>(c);
                    }
                }
                Replace(cleaned);
            }
            if (pressed(KEY_Z)) {
                if (shift) History(redo,undo);
                else History(undo,redo);
            }
            if (pressed(KEY_Y)) History(redo,undo);
        } else {
            size_t start=Start(cursor), end=End(cursor);
            bool moving=true;
            if (pressed(KEY_LEFT) && cursor>0) --cursor;
            else if (pressed(KEY_RIGHT) && cursor<code.size()) ++cursor;
            else if (pressed(KEY_HOME)) cursor=start;
            else if (pressed(KEY_END)) cursor=end;
            else if (pressed(KEY_UP) && start>0) cursor=std::min(Start(start-1)+cursor-start,start-1);
            else if (pressed(KEY_DOWN) && end<code.size()) cursor=std::min(end+1+cursor-start,End(end+1));
            else moving=false;
            if (moving && !shift) anchor=cursor;
            if (pressed(KEY_BACKSPACE)) {
                if (cursor==anchor && cursor>0) --anchor;
                Replace("");
            }
            if (pressed(KEY_DELETE)) {
                if (cursor==anchor && cursor<code.size()) ++anchor;
                Replace("");
            }
            if (pressed(KEY_TAB)) Replace("    ");
            if (pressed(KEY_ENTER)) {
                std::string indent="\n";
                for (size_t i=Start(cursor);i<cursor && code[i]==' ';++i) indent+=' ';
                if (cursor>0 && code[cursor-1]=='{') indent+="    ";
                Replace(indent);
            }
        }
        for (int c=GetCharPressed();c>0;c=GetCharPressed()) {
            if (!ctrl && c>=32 && c<=126) Replace(std::string(1,static_cast<char>(c)));
        }
        if (cursor!=oldCursor || code!=oldCode) {
            int row=static_cast<int>(std::count(code.begin(),code.begin()+cursor,'\n'));
            scroll=std::clamp(scroll,std::max(0,row-rows+1),row);
            float pos=Prefix(Start(cursor),cursor-Start(cursor));
            horizontal=std::clamp(horizontal,std::max(0.0f,pos-r.width+75),pos);
        }
        scroll=std::clamp(scroll,0,std::max(0,rowCount()-rows));
        Card(r,rail);
        Clip(r.x+50,r.y+5,r.width-55,r.height-10);
        size_t start=0;
        for (int row=0;row<scroll+rows;++row) {
            size_t end=End(start);
            if (row>=scroll) {
                float x=r.x+54-horizontal, y=r.y+9+(row-scroll)*rowHeight;
                if(cursor>=start && cursor<=end)
                    DrawRectangleRec({r.x+50,y,r.width-55,rowHeight},panel);
                size_t first=std::max(start,std::min(cursor,anchor)), last=std::min(end,std::max(cursor,anchor));
                if (first<last) {
                    float left=Prefix(start,first-start), right=Prefix(start,last-start);
                    DrawRectangleRec({x+left,y,right-left,rowHeight},litSurface);
                }
                CodeLine(code.substr(start,end-start),x,y,19);
                if (cursor>=start && cursor<=end && std::fmod(GetTime(),1.0)<0.65)
                    DrawRectangleRec({x+Prefix(start,cursor-start),y,2,21},led);
            }
            if (end==code.size()) break;
            start=end+1;
        }
        EndScissorMode();
        for (int row=0;row<rows && row+scroll<rowCount();++row)
            Text(TextFormat("%d",row+scroll+1),r.x+9,r.y+9+row*rowHeight,17,muted,true);
    }
};



#ifdef _WIN32
struct NativeHandle {
    HANDLE value;
    explicit NativeHandle(HANDLE handle) : value(handle) {}
    ~NativeHandle() { if(valid()) CloseHandle(value); }
    NativeHandle(const NativeHandle&)=delete;
    NativeHandle& operator=(const NativeHandle&)=delete;
    bool valid() const { return value && value!=INVALID_HANDLE_VALUE; }
};

struct ProcessResult {
    bool ok = false;
    std::string output;
};

ProcessResult RunLocal(const std::filesystem::path& program, std::wstring command,
                       const std::filesystem::path& folder, DWORD timeout,
                       const wchar_t* inputPath=L"NUL") {
    SECURITY_ATTRIBUTES inherit{sizeof(SECURITY_ATTRIBUTES),nullptr,TRUE};
    NativeHandle output(CreateFileW((folder/L"output.txt").c_str(),GENERIC_READ|GENERIC_WRITE,
                                   FILE_SHARE_READ,&inherit,CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL,nullptr));
    NativeHandle input(CreateFileW(inputPath,GENERIC_READ,FILE_SHARE_READ|FILE_SHARE_WRITE,
                                  &inherit,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,nullptr));
    NativeHandle job(CreateJobObjectW(nullptr,nullptr));
    if(!input.valid() || !output.valid() || !job.valid()) return {false,"Could not prepare the local process."};
    JOBOBJECT_EXTENDED_LIMIT_INFORMATION limits{};
    limits.BasicLimitInformation.LimitFlags=JOB_OBJECT_LIMIT_KILL_ON_JOB_CLOSE;
    if(!SetInformationJobObject(job.value,JobObjectExtendedLimitInformation,&limits,sizeof(limits)))
        return {false,"Could not set process cleanup limits."};

    STARTUPINFOW startup{};
    startup.cb=sizeof(startup);
    startup.dwFlags=STARTF_USESTDHANDLES;
    startup.hStdInput=input.value;
    startup.hStdOutput=startup.hStdError=output.value;
    PROCESS_INFORMATION process{};
    if(!CreateProcessW(program.c_str(),command.data(),nullptr,nullptr,TRUE,CREATE_SUSPENDED|CREATE_NO_WINDOW,
                       nullptr,folder.c_str(),&startup,&process))
        return {false,"Could not start the local process."};
    NativeHandle child(process.hProcess), thread(process.hThread);
    if(!AssignProcessToJobObject(job.value,child.value) || ResumeThread(thread.value)==static_cast<DWORD>(-1)) {
        TerminateProcess(child.value,1);
        WaitForSingleObject(child.value,1000);
        return {false,"Could not start the process with its cleanup limits."};
    }

    constexpr DWORD maxOutput=32*1024;
    ULONGLONG start=GetTickCount64();
    const char* stopped=nullptr;
    while(true) {
        DWORD status=WaitForSingleObject(child.value,20);
        LARGE_INTEGER bytes{};
        if(!GetFileSizeEx(output.value,&bytes) || bytes.QuadPart>maxOutput) {
            stopped="Stopped: output limit exceeded.";
            break;
        }
        if(status==WAIT_OBJECT_0) break;
        if(status!=WAIT_TIMEOUT || GetTickCount64()-start>=timeout) {
            stopped="Stopped: process did not finish before its time limit.";
            break;
        }
    }
    TerminateJobObject(job.value,1);
    WaitForSingleObject(child.value,1000);
    DWORD code=1;
    GetExitCodeProcess(child.value,&code);
    SetFilePointer(output.value,0,nullptr,FILE_BEGIN);
    std::string result(maxOutput,'\0');
    DWORD read=0;
    ReadFile(output.value,result.data(),maxOutput,&read,nullptr);
    result.resize(read);
    result.erase(std::remove(result.begin(),result.end(),'\r'),result.end());
    for(char& c:result) {
        unsigned char byte=static_cast<unsigned char>(c);
        if((byte<32 && c!='\n' && c!='\t') || byte>126) c='?';
    }
    if(stopped) result+=std::string("\n")+stopped;
    else if(code!=0) result+="\nExit code "+std::to_string(code)+".";
    return {!stopped && code==0,result};
}

#include "challenges.inc"

bool PrepareCompilerEnvironment() {
    wchar_t installed[32768]{};
    if(GetEnvironmentVariableW(L"VCToolsInstallDir",installed,static_cast<DWORD>(std::size(installed)))>0)
        return true;
    auto installer=std::filesystem::path(L"C:/Program Files (x86)/Microsoft Visual Studio/Installer/vswhere.exe");
    if(!std::filesystem::exists(installer)) return false;
    std::error_code error;
    auto folder=std::filesystem::temp_directory_path(error)/
                ("codepath-compiler-"+std::to_string(std::random_device{}()));
    if(error || !std::filesystem::create_directory(folder,error)) return false;
    auto cleanup=[&] {
        std::filesystem::remove(folder/"output.txt",error);
        std::filesystem::remove(folder,error);
    };
    auto lookup=RunLocal(installer,L"vswhere.exe -latest -requires Microsoft.VisualStudio.Component.VC.Tools.x86.x64 -property installationPath",
                         folder,5000);
    if(!lookup.ok) { cleanup(); return false; }
    std::string location=lookup.output.substr(0,lookup.output.find('\n'));
    auto batch=std::filesystem::path(location)/L"Common7/Tools/VsDevCmd.bat";
    if(location.empty() || !std::filesystem::exists(batch)) { cleanup(); return false; }
    wchar_t system[32768]{};
    if(!GetSystemDirectoryW(system,static_cast<UINT>(std::size(system)))) { cleanup(); return false; }
    auto command=std::filesystem::path(system)/L"cmd.exe";
    std::wstring arguments=L"cmd.exe /d /s /c \"\""+batch.wstring()+
                           L"\" -no_logo -arch=x64 -host_arch=x64 >nul && set\"";
    auto environment=RunLocal(command,arguments,folder,20000);
    if(!environment.ok) { cleanup(); return false; }
    std::istringstream lines(environment.output);
    std::string line;
    while(std::getline(lines,line)) {
        size_t separator=line.find('=');
        if(separator==std::string::npos || separator==0) continue;
        std::wstring key(line.begin(),line.begin()+separator);
        std::wstring value(line.begin()+separator+1,line.end());
        SetEnvironmentVariableW(key.c_str(),value.c_str());
    }
    cleanup();
    return GetEnvironmentVariableW(L"VCToolsInstallDir",installed,static_cast<DWORD>(std::size(installed)))>0;
}

std::string CompileAndRun(const std::string& code,bool cpp,int challenge=-1) {
    if(!PrepareCompilerEnvironment())
        return "C/C++ compiler not found. Install Visual Studio C++ build tools, then retry.";
    wchar_t path[32768]{};
    DWORD length=GetEnvironmentVariableW(L"VCToolsInstallDir",path,static_cast<DWORD>(std::size(path)));
    if(length==0 || length>=std::size(path))
        return "C/C++ compiler not found. Install Visual Studio C++ build tools, then retry.";
    auto compiler=std::filesystem::path(path)/L"bin/Hostx64/x64/cl.exe";
    std::error_code error;
    if(!std::filesystem::is_regular_file(compiler,error)) return "The Visual Studio x64 compiler was not found.";
    auto folder=std::filesystem::temp_directory_path(error)/("codepath-run-"+std::to_string(std::random_device{}()));
    if(error || !std::filesystem::create_directory(folder,error)) return "Could not create a temporary build folder.";
    auto source=folder/(cpp ? L"main.cpp" : L"main.c");
    std::ofstream file(source);
    file << code << '\n';
    file.close();
    std::string report="Could not write the temporary source file.";
    if(file) {
        auto compile=RunLocal(compiler,cpp ? L"cl.exe /nologo /W4 /EHsc /std:c++17 /Fesample.exe main.cpp" :
                                           L"cl.exe /nologo /W4 /std:c17 /Fesample.exe main.c",folder,15000);
        report=(compile.ok ? "Build succeeded\n" : "Build failed\n")+compile.output;
        if(compile.ok && challenge>=0) {
            const auto& exercise=challenges[challenge];
            int passed=0;
            report+=std::string("\n--- ")+exercise.title+" ---\n";
            auto input=folder/L"input.txt";
            for(size_t n=0;n<exercise.inputs.size();++n) {
                std::ofstream fixture(input);
                fixture << exercise.inputs[n] << '\n';
                fixture.close();
                if(!fixture) { report+="Could not write test input.\n"; break; }
                auto run=RunLocal(folder/L"sample.exe",L"sample.exe",folder,1500,input.c_str());
                int expected=exercise.expected[n];
                std::istringstream output(run.output);
                int actual=0;
                std::string extra;
                bool pass=run.ok && (output >> actual) && actual==expected && !(output >> extra);
                passed+=pass;
                report+=std::string("Input: ")+exercise.inputs[n]+"  expected "+std::to_string(expected)+"  ";
                report+=pass ? "PASS\n" : "FAIL\n";
                if(!pass && !run.output.empty()) report+=run.output.substr(0,150)+"\n";
            }
            report+=std::to_string(passed)+"/4 cases passed.";
        } else if(compile.ok) {
            auto run=RunLocal(folder/L"sample.exe",L"sample.exe",folder,3000);
            report+=run.ok ? "\n--- Program output ---\n" : "\n--- Program stopped or failed ---\n";
            report+=run.output.empty() ? "(no output)" : run.output;
        }
    }
    for(const auto* name:{L"main.c",L"main.cpp",L"main.obj",L"sample.exe",L"output.txt",L"input.txt"})
        std::filesystem::remove(folder/name,error);
    std::filesystem::remove(folder,error);
    return report;
}
#endif

enum Page { Home, Learn, Assess, Visualize, Notebook, Progress };
#include "lesson_details.inc"
const char* LessonNote(int id) {
    static const char* notes[]={
        "Trace total after each iteration. For an array of length n, stop before index n.",
        "Test 1900, 2000, and 2024. Checking divisibility by 4 alone accepts 1900 incorrectly.",
        "Dereference a pointer only while it refers to a live object. Assigning null does not destroy score.",
        "During insertion, save the successor before replacing the predecessor's link.",
        "A declaration from a header lets the compiler check arguments against the library interface.",
        "If scanf reports a failed conversion, avoid reading an uninitialized destination.",
        "Use a floating-point operand when the fractional part is required.",
        "Keep the accumulator outside the loop so its value survives each iteration.",
        "A null terminator is a character with value zero. It differs from the digit '0'.",
        "One-past pointers can mark the end of a range. Dereferencing them is invalid.",
        "After free, aliases to the allocation cannot be used to access the former object.",
        "Store fgetc's result in int and use ferror to distinguish an I/O failure from end of file.",
        "Trace the return values separately from the calls. Limit the input to avoid signed overflow.",
        "Assigning through an existing reference changes its referent. It does not rebind the reference.",
        "The parameter types distinguish the overloads. Return type alone cannot do that.",
        "After push_back, reacquire iterators if vector capacity changed.",
        "Use a const member function for operations that inspect state without changing it.",
        "A reference to the base can preserve virtual dispatch. Copying to a base object can slice the derived part.",
        "Template arguments are checked when the relevant specialization is instantiated.",
        "Use [begin, end) to express a range. It also represents an empty range when begin equals end.",
        "Prefer a resource-owning object whose destructor releases its resource during unwinding.",
        "After ownership moves, check the source before dereferencing it."
    };
    return notes[id];
}

#include "premium_labs.inc"

int SelfTest() {
    auto lessons=Curriculum();
    bool passed=lessons.size()==22;
    for(auto input:{std::array<int,3>{5,2,4},std::array<int,3>{9,5,1},
                    std::array<int,3>{4,4,2},std::array<int,3>{1,4,9}}) {
        auto trace=SortTrace(input);
        passed=passed && trace.front().values==input;
        std::sort(input.begin(),input.end());
        passed=passed && trace.back().values==input;
        for(size_t i=1;i<trace.size();++i) {
            auto expected=trace[i-1].values;
            if(trace[i].swapped) {
                int left=trace[i].left;
                passed=passed && left>=0 && left<2 && expected[left]>expected[left+1];
                std::swap(expected[left],expected[left+1]);
            }
            passed=passed && trace[i].values==expected;
        }
    }
    for(const Lesson& lesson:lessons) {
        passed=passed && *lesson.title && *lesson.body && *lesson.code && *lesson.question &&
               *lesson.hint && *lesson.explanation;
        for(size_t i=0;i<lesson.answers.size();++i) {
            passed=passed && *lesson.answers[i];
            for(size_t j=i+1;j<lesson.answers.size();++j)
                passed=passed && std::string(lesson.answers[i])!=lesson.answers[j];
        }
    }
    std::mt19937 rng(12);
    QuizState quiz;
    quiz.Start({0,2,3},rng);
    quiz.hint=true;
    quiz.selected=2;
    quiz.prediction="12";
    quiz.Advance(rng);
    auto order=quiz.order;
    std::sort(order.begin(),order.end());
    passed=passed && quiz.index==1 && !quiz.hint && quiz.selected==-1 && quiz.prediction.empty() &&
           order==std::array<int,4>{0,1,2,3};
    Editor editor;
    editor.code="int x = 1;";
    editor.anchor=8;
    editor.cursor=9;
    editor.Replace("42");
    passed=passed && editor.code=="int x = 42;";
    editor.History(editor.undo,editor.redo);
    passed=passed && editor.code=="int x = 1;";
    editor.History(editor.redo,editor.undo);
    passed=passed && editor.code=="int x = 42;";

    std::error_code error;
    auto directory=std::filesystem::temp_directory_path(error);
    if(error) return 1;
    directory/="codepath-self-test-"+std::to_string(std::random_device{}());
    if(!std::filesystem::create_directory(directory,error)) return 1;
    auto file=directory/"progress.txt";
    std::vector<Score> scores(lessons.size()), loaded(lessons.size());
    scores[2]={1,1};
    passed=WriteProgress(file,scores,2) && passed;
    scores[2]={2,1};
    passed=WriteProgress(file,scores,3) && passed;
    int lesson=0;
    ReadProgress(file,loaded,lesson);
    passed=passed && loaded[2].attempts==2 && loaded[2].correct==1 && lesson==3;
    { std::ofstream invalid(file); invalid << "CODEPATH1 2\n1 4\n"; }
    ReadProgress(file,loaded,lesson);
    passed=passed && loaded[2].attempts==2 && lesson==3;
    std::filesystem::remove(file,error);
    std::filesystem::remove(directory/"progress.txt.tmp",error);
    std::filesystem::remove(directory,error);
    std::puts(passed ? "CodePath self-test passed" : "CodePath self-test failed");
    return passed ? 0 : 1;
}

int main(int argc, char** argv) {
    if(argc==2 && std::string(argv[1])=="--self-test") return SelfTest();
    const bool captureMotion=argc==3 && std::string(argv[1])=="--capture-motion";
    const bool captureWindow=argc==3 && std::string(argv[1])=="--capture-windowed";
    const bool captureScreens=captureWindow || (argc==3 && std::string(argv[1])=="--capture-screenshots");
    std::filesystem::path captureDirectory;
    if(captureScreens || captureMotion) {
        captureDirectory=std::filesystem::u8path(argv[2]);
        std::filesystem::create_directories(captureDirectory);
    }
#ifdef _WIN32
    if(argc==2 && std::string(argv[1])=="--self-test-compiler") {
        std::string c=CompileAndRun("#include <stdio.h>\nint main(void) { puts(\"CodePath C output\"); }",false);
        std::string cpp=CompileAndRun("#include <iostream>\nint main() { std::cout << \"CodePath CPP output\"; }",true);
        std::string challenge=CompileAndRun("#include <stdio.h>\nint main(void) { int n; "
            "if (scanf(\"%d\", &n) == 1) printf(\"%d\\n\", n*(n+1)/2); }",false,0);
        for(int i=1;i<static_cast<int>(std::size(challenges));++i) {
            auto result=CompileAndRun(challenges[i].reference,challenges[i].cpp,i);
            std::puts(result.c_str());
            if(result.find("4/4 cases passed")==std::string::npos) return 1;
        }
        std::string invalid=CompileAndRun("this is not C++;",true);
        std::string timeout=CompileAndRun("int main(void) { volatile int n = 0; while (n == 0) {} }",false);
        std::puts((c+"\n"+cpp+"\n"+challenge+"\n"+invalid+"\n"+timeout).c_str());
        return c.find("CodePath C output")!=std::string::npos &&
               cpp.find("CodePath CPP output")!=std::string::npos &&
               challenge.find("4/4 cases passed")!=std::string::npos &&
               invalid.find("Build failed")!=std::string::npos &&
               timeout.find("time limit")!=std::string::npos ? 0 : 1;
    }
#endif
    SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_MSAA_4X_HINT);
    InitWindow(1100,700,"CodePath Studio | C and C++");
#ifdef _WIN32
    // Keep the 1100x700 layout readable on Windows displays with enlarged text scaling.
    Vector2 dpi=GetWindowScaleDPI();
    int monitor=GetCurrentMonitor();
    float windowScale=std::min({std::max(1.0f,dpi.x),std::max(1.0f,dpi.y),
                                (GetMonitorWidth(monitor)-100)/1100.0f,
                                (GetMonitorHeight(monitor)-100)/700.0f});
    if(windowScale>1.01f) {
        int startWidth=static_cast<int>(1100*windowScale), startHeight=static_cast<int>(700*windowScale);
        SetWindowSize(startWidth,startHeight);
        Vector2 monitorPosition=GetMonitorPosition(monitor);
        SetWindowPosition(static_cast<int>(monitorPosition.x+(GetMonitorWidth(monitor)-startWidth)/2),
                          static_cast<int>(monitorPosition.y+(GetMonitorHeight(monitor)-startHeight)/2));
    }
    SetWindowMinSize(static_cast<int>(1000*windowScale),static_cast<int>(700*windowScale));
#else
    SetWindowMinSize(1000,700);
#endif
    SetTargetFPS(60);
    if(captureScreens && !captureWindow) ToggleBorderlessWindowed();
    ui=mono=heading=GetFontDefault();
    auto fontPath=[&](const char* name) {
        auto path=std::filesystem::path(GetApplicationDirectory())/"assets"/"fonts"/name;
        if(!std::filesystem::exists(path)) path=std::filesystem::path("assets")/"fonts"/name;
        return path.string();
    };
    auto uiFont=fontPath("Manrope-Regular.ttf"), headingFont=fontPath("Manrope-Bold.ttf");
    auto codeFont=fontPath("JetBrainsMono-Regular.ttf");
    if(FileExists(uiFont.c_str())) {
        ui=LoadFontEx(uiFont.c_str(),64,nullptr,0);
    }
    if(FileExists(headingFont.c_str())) heading=LoadFontEx(headingFont.c_str(),84,nullptr,0);
    if(FileExists(codeFont.c_str())) mono=LoadFontEx(codeFont.c_str(),64,nullptr,0);
    SetTextureFilter(ui.texture,TEXTURE_FILTER_BILINEAR);
    SetTextureFilter(mono.texture,TEXTURE_FILTER_BILINEAR);
    SetTextureFilter(heading.texture,TEXTURE_FILTER_BILINEAR);

    auto lessons=Curriculum();
    std::vector<Score> scores(lessons.size());
    const char* profile=nullptr;
#ifdef _MSC_VER
    char* windowsProfile=nullptr;
    _dupenv_s(&windowsProfile,nullptr,"LOCALAPPDATA");
    profile=windowsProfile;
#else
    profile=std::getenv("HOME");
#endif
    std::filesystem::path save=(profile ? std::filesystem::u8path(profile) : std::filesystem::path(GetApplicationDirectory()))/
                                "CodePathStudio"/"progress-v1.txt";
#ifdef _MSC_VER
    std::free(windowsProfile);
#endif
    auto introSetting=save.parent_path()/"skip-intro";
    std::error_code introError;
    bool introEnabled=captureMotion || !std::filesystem::exists(introSetting,introError);
    float introTime=(introEnabled || captureMotion) ? 0.0f : 4.0f;
    float introExit=1;
    int current=0, path=0, offset=0, lab=0, step=0, speed=1;
    ReadProgress(save,scores,current);
    bool saved=true, playing=false, motion=true;
    bool fullscreenRequested=false, lessonDetail=false, phasePaused=false;
    MemoryBoard memoryBoard;
    int lessonScroll=0,exploreStep=0,exploreValue=15,exploreBound=4;
    bool lessonExplore=false;
    float timer=0, reward=0, displayed=0;
    float labMotion=1, quizProgress=0;
    float pageAge=1, selectionX=0, lessonAge=1, quizAge=1, answerAge=1;
    int lessonKey=-1, questionKey=-1, lastAnswer=-1;
    float outputAge=1;
    Page previousPage=Home;
    int observedLab=-1, observedStep=-1, observedIndex=-1;
    const std::array<int,3> sortInputs[]={{5,2,4},{9,5,1},{4,4,2},{1,4,9}};
    int sortInput=0;
    int arrayInput=0;
    const std::array<int,3> arrayInputs[]={{2,4,8},{-3,7,2},{9,0,-9}};
    auto sortTrace=SortTrace(sortInputs[sortInput]);
    bool pointerTrace=false;
    auto labCount=[&]{
        if(lab==0) return static_cast<int>(std::size(PointerLab::operations));
        Scene scene=FindScene(lab);
        if(scene.count) return scene.count;
        return static_cast<int>(lab==5 ? sortTrace.size() : visualLabs[lab].steps.size());
    };
    Page page=Home;
    int captureIndex=0, captureFrame=0;
    if(captureScreens) { introTime=4; motion=false; }
    QuizState quiz;
    Editor editor;
    editor.code=lessons[current].code;
#ifdef _WIN32
    bool cppMode=std::string(lessons[current].path)=="C++";
    int challenge=-1;
    bool challengePicker=false;
    std::future<std::string> job;
    std::string console;
    int outputScroll=0;
    auto loadCode=[&](const char* code) {
        editor.cursor=0;
        editor.anchor=editor.code.size();
        editor.Replace(code);
        editor.cursor=editor.anchor=0;
        editor.scroll=0;
        editor.horizontal=0;
    };
    auto selectChallenge=[&](int id) {
        challenge=id;
        cppMode=challenges[id].cpp;
        loadCode(challenges[id].starter);
        if(!job.valid()) console.clear();
        outputScroll=0;
        challengePicker=false;
        page=Notebook;
    };
#endif
    std::mt19937 rng(std::random_device{}());
    const char* paths[]={"ALL","C","C++","EXAM"};
    const char* nav[]={"Overview","Curriculum","Assessments","Visual labs","Code notebook","Progress"};
    auto saveNow=[&]{ if(!captureScreens && !captureMotion) saved=WriteProgress(save,scores,current); };
    auto pick=[&](int id){
        if(current!=id) {
            current=id; lessonScroll=0; exploreStep=0; exploreValue=15;
            lessonExplore=false; lessonDetail=false; saveNow();
        }
    };
    auto filter=[&](int p){
        std::vector<int> list;
        const int sequence[]={4,0,5,6,7,8,2,9,10,11,12,13,14,15,16,17,18,19,20,21,3,1};
        for(int id:sequence)
            if(p==0 || std::string(lessons[id].path)==paths[p]) list.push_back(id);
        return list;
    };
    auto startQuiz=[&](std::vector<int> pool){
        quiz.Start(std::move(pool),rng);
        page=Assess;
    };
    auto mixed=[&]{
        auto pool=filter(0);
        std::shuffle(pool.begin(),pool.end(),rng);
        pool.resize(8);
        startQuiz(std::move(pool));
    };
    // Record the exact frame presented in the window at a fixed readback resolution.
    RenderTexture2D recording{};
    if(captureMotion) recording=LoadRenderTexture(960,611);
    const double recordingStart=GetTime();
    double nextFrame=0;
    float demoElapsed=0;
    int demoScene=-1;
    auto beginFrame=[&](Color background) {
        BeginDrawing();
        if(captureMotion) BeginTextureMode(recording);
        ClearBackground(background);
        BeginMode2D({{static_cast<float>(uiInset),0},{0,0},0,uiScale});
    };
    auto endFrame=[&] {
        EndMode2D();
        if(captureMotion) {
            EndTextureMode();
            DrawTexturePro(recording.texture,{0,0,960,-611},
                           {0,0,static_cast<float>(GetScreenWidth()),static_cast<float>(GetScreenHeight())},
                           {0,0},0,WHITE);
            if(demoElapsed>=nextFrame) {
                nextFrame=demoElapsed+1.0/14;
                Image shot=LoadImageFromTexture(recording.texture);
                ImageFlipVertical(&shot);
                auto path=(captureDirectory/TextFormat("%06d.bmp",static_cast<int>(demoElapsed*1000))).string();
                ExportImage(shot,path.c_str());
                UnloadImage(shot);
            }
        }
        EndDrawing();
    };
    while(!WindowShouldClose()) {
        if(captureMotion) {
            demoElapsed=static_cast<float>(GetTime()-recordingStart);
            const float sceneEnds[]={5.0f,6.2f,7.5f,12.8f,15.8f,19.5f,22.6f,25.8f,31.7f,36.0f};
            const int scene=static_cast<int>(std::upper_bound(std::begin(sceneEnds),std::end(sceneEnds),demoElapsed)
                                             -std::begin(sceneEnds));
            if(scene!=demoScene) {
                demoScene=scene;
                if(scene==1) page=Learn;
                if(scene==2) page=Notebook;
                if(scene>=3 && scene<=8) {
                    const int firstSteps[]={2,1,2,3,1,0};
                    page=Visualize;
                    lab=scene-3;
                    step=firstSteps[lab];
                    speed=lab==0 ? 1 : 2;
                    playing=true;
                    timer=0;
                }
                if(scene==9) { playing=false; startQuiz({4}); }
                if(scene==10) break;
            }
        }
        if(captureScreens) {
            const Page capturePages[]={Home,Learn,Notebook,Progress,Assess,Visualize};
            page=captureIndex<5 ? capturePages[captureIndex] : captureIndex<=6 ? Assess : Visualize;
            if(captureIndex==5 && quiz.questions.empty()) quiz.Start({current},rng);
            if(captureIndex==6) { quiz.index=quiz.questions.size(); quiz.correct=1; }
            if(captureIndex>=7 && captureIndex<13) {
                const int stages[]={3,2,3,4,3,2};
                lab=captureIndex-7; step=stages[lab];
            }
        }
        bool keyActivity=false, fullscreenKey=false;
        for(int key=GetKeyPressed();key!=0;key=GetKeyPressed()) {
            keyActivity=true;
            fullscreenKey=fullscreenKey || key==KEY_F11;
        }
        if(fullscreenKey || fullscreenRequested) ToggleBorderlessWindowed();
        fullscreenRequested=false;
        float screenW=captureMotion ? 960.0f : static_cast<float>(GetScreenWidth());
        float screenH=captureMotion ? 611.0f : static_cast<float>(GetScreenHeight());
        uiScale=std::min(screenW/1120.0f,screenH/790.0f);
        float w=std::min(1530.0f,screenW/uiScale), h=screenH/uiScale;
        uiInset=static_cast<int>((screenW-w*uiScale)/2);
        SetMouseOffset(-uiInset,0);
        SetMouseScale(screenW/GetScreenWidth()/uiScale,screenH/GetScreenHeight()/uiScale);
        float x=32, width=w-64, dt=GetFrameTime();
        animationDt=std::min(dt,0.05f);
        animateUi=motion;
        introExit=motion ? std::min(1.0f,introExit+dt/0.78f) : 1.0f;
        if(introTime<4 && !captureScreens) {
            bool skip=!captureMotion && (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) || keyActivity || !motion);
            introTime=skip ? 4 : std::min(4.0f,introTime+dt);
            if(introTime<4 || skip) {
                beginFrame(terminal);
                DrawStudioIntro(w,h,introTime);
                endFrame();
                continue;
            }
            pageAge=0; introExit=0;
        }
#ifdef _WIN32
        if(job.valid() && job.wait_for(std::chrono::seconds(0))==std::future_status::ready) {
            try { console=job.get(); }
            catch(const std::exception&) { console="The build workspace could not finish the request."; }
            outputScroll=0;
            outputAge=0;
        }
#endif
        int mastered=0, attempts=0, correct=0;
        for(const Score& s:scores) { mastered+=s.correct>0; attempts+=s.attempts; correct+=s.correct; }
        int xp=mastered*100, level=xp/300+1;
        float fill=static_cast<float>(mastered)/lessons.size();
        displayed+= (fill-displayed)*(motion ? std::min(1.0f,dt*10) : 1.0f);
        outputAge=motion ? outputAge+dt : 1;
        reward=std::max(0.0f,reward-dt);
        if(page==Visualize && playing) {
            timer+=dt*(speed==0 ? 0.5f : speed==1 ? 1.0f : 2.0f);
            if(timer>=2.2f) {
                timer=0;
                if(step+1<labCount()) ++step;
                else { playing=false; labMotion=1; }
            }
        }
        if(observedLab!=lab || observedStep!=step) {
            labMotion=0;
            phasePaused=false;
            observedLab=lab;
            observedStep=step;
        }
        if(!motion) labMotion=1;
        else if(page==Visualize && playing) labMotion=std::clamp(timer/2.2f,0.0f,1.0f);
        else if(page==Visualize && !phasePaused) labMotion=std::min(1.0f,labMotion+dt/2.2f);
        if(page!=Assess || quiz.questions.empty() || observedIndex>static_cast<int>(quiz.index)) quizProgress=0;
        observedIndex=static_cast<int>(quiz.index);
        if(page==Assess && !quiz.questions.empty()) {
            float target=(quiz.index+(quiz.selected!=-1 ? 1.0f : 0.0f))/quiz.questions.size();
            quizProgress+=(target-quizProgress)*(motion ? std::min(1.0f,dt*12) : 1);
        }

        beginFrame(bg);
        DrawRectangleRec({0,0,w,57},panel);
        DrawRectangleRec({0,56,w,1},seam);
        StudioMark({x,13},29,litSurface);
        Text("CodePath Studio",x+41,17,20);
        const Page navOrder[]={Home,Learn,Visualize,Notebook,Assess,Progress};
        float tabW=std::min(144.0f,(width-236)/6), navX=x+width-tabW*6;
        for(int i=0;i<6;++i) {
            Rectangle tab{navX+i*tabW,8,tabW-4,41};
            if(CheckCollisionPointRec(GetMousePosition(),tab) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                if(page!=navOrder[i]) playing=false;
                page=navOrder[i];
            }
        }
        if(page!=previousPage) pageAge=0;
        pageAge=motion ? pageAge+dt : 1;
        int selectedTab=static_cast<int>(std::find(std::begin(navOrder),std::end(navOrder),page)-std::begin(navOrder));
        float selectionTarget=navX+selectedTab*tabW;
        selectionX=selectionX==0 ? selectionTarget : Approach(selectionX,selectionTarget,13);
        DrawRectangleRounded({selectionTarget,8,tabW-4,41},0.38f,10,
                             Blend(panel,litSurface,0.30f+0.70f*Arrival(pageAge/0.30f)));
        for(int i=0;i<6;++i) {
            Page destination=navOrder[i];
            Rectangle item{navX+i*tabW,8,tabW-4,41};
            bool selected=page==destination;
            bool hovered=CheckCollisionPointRec(GetMousePosition(),item);
            if(hovered && !selected)
                DrawRectangleRounded(item,0.38f,10,Fade(hoverSurface,0.62f));
            NavIcon(destination,{item.x+16,item.y+20},selected ? primary : muted);
            Text(nav[destination],item.x+32,item.y+11,16,selected ? primary : ink);
        }
        DrawRectangleRounded({selectionX+14,47,tabW-32,2},1,6,primary);
        float footerY=h-42;
        DrawRectangleRec({0,footerY,w,42},panel);
        DrawRectangleRec({0,footerY,w,1},seam);
        Text("LOCAL PROFILE",x,footerY+12,13,muted);
        Text(TextFormat("LEVEL %d  /  %d XP",level,xp),x+121,footerY+11,15,primary);
        Bar(x+251,footerY+18,101,displayed);
        if(Button({w-x-347,footerY+5,96,32},introEnabled ? "Intro on" : "Intro off")
           && !captureMotion && !captureScreens) {
            if(introEnabled) {
                std::filesystem::create_directories(save.parent_path(),introError);
                std::ofstream marker(introSetting);
                if(marker) introEnabled=false;
            } else {
                std::filesystem::remove(introSetting,introError);
                if(!introError) introEnabled=true;
            }
        }
        if(Button({w-x-242,footerY+5,98,32},motion ? "Motion on" : "Motion off")) motion=!motion;
        if(Button({w-x-135,footerY+5,135,32},IsWindowState(FLAG_BORDERLESS_WINDOWED_MODE) ? "Exit fullscreen" : "Fullscreen  F11"))
            fullscreenRequested=true;
        h=footerY;
        const float headingIn=Arrival(pageAge/0.34f);
        Text(nav[page],x+13*(1-headingIn),66+4*(1-headingIn),32,Blend(muted,ink,headingIn));
        if(!saved) Text("Progress could not be saved",x,h-25,15,errorColor);

        const Page renderedPage=page;
        if(page==Home) {
            // The start page is a live demonstration, with the courses as the next stop.
            float heroY=112, heroH=h-162, heroW=width*0.67f;
            DrawRectangleRounded({x,heroY,heroW,heroH},0.09f,12,hero);
            Text("Learn by changing",x+29,heroY+25,48,WHITE);
            Text("the state.",x+29,heroY+77,48,signal);
            Text("A live memory experiment",x+32,heroY+145,18,draft);
            float boardY=heroY+187, boardH=heroH-297;
            PremiumLabs::DrawHomeMemory(memoryBoard,{x+23,boardY,heroW-46,boardH},motion);
            memoryBoard.Update(dt,motion);
            Text("POINTER ADDRESS",x+28,heroY+heroH-92,13,draft);
            Text(TextFormat("p = 0x%X",0x100+4*memoryBoard.target),x+28,heroY+heroH-70,25,WHITE,true);
            Wrap("Select a memory cell, then write through the pointer.",x+29,heroY+heroH-36,heroW-50,15,draft);
            if(Button({x+heroW-322,heroY+heroH-90,153,40},memoryBoard.writing ? "Writing..." : "Execute ++*p",true,signal) && !memoryBoard.writing) {
                memoryBoard.pendingTarget=memoryBoard.target;
                memoryBoard.travel=0;
                memoryBoard.writing=true;
            }
            if(Button({x+heroW-159,heroY+heroH-90,132,40},"Open pointer lab")) {
                lab=0; step=0; playing=false; timer=0; page=Visualize;
            }
            float routeX=x+heroW+24, routesY=heroY+20, tile=width-heroW-24;
            Text("Continue your path",routeX,routesY,26);
            if(Button({routeX,routesY+47,tile,42},"Continue learning",true)) {
                path=0;
                auto list=filter(path);
                offset=std::max(0,static_cast<int>(std::find(list.begin(),list.end(),current)-list.begin())-2);
                page=Learn;
            }
            const char* routeName[]={"C fundamentals","C++ systems","Practice"};
            float startY=routesY+112, stride=(heroY+heroH-startY)/3;
            for(int i=0;i<3;++i) {
                float left=routeX, cardY=startY+i*stride;
                DrawRectangleRec({left,cardY,tile,1},seam);
                Text(routeName[i],left+17,cardY+14,21);
                if(i==2) {
                    Wrap("Test a topic or compile a short program.",left+17,cardY+43,tile-34,16);
                    if(Button({left+17,cardY+82,tile-34,35},"Mixed assessment")) mixed();
                    continue;
                }
                auto course=filter(i+1);
                int done=0;
                for(int id:course) done+=scores[id].correct>0;
                Text(TextFormat("%d / %d lessons mastered",done,static_cast<int>(course.size())),left+17,cardY+43,16,muted);
                Bar(left+17,cardY+69,tile-34,static_cast<float>(done)/course.size());
                if(Button({left+17,cardY+82,tile-34,35},"Open course")) {
                    path=i+1; offset=0;
                    auto next=std::find_if(course.begin(),course.end(),[&](int id){return scores[id].correct==0;});
                    pick(next==course.end() ? course.front() : *next);
                    page=Learn;
                }
            }
            Text(TextFormat("%d XP   /   %d of %d lessons mastered",xp,mastered,static_cast<int>(lessons.size())),
                 x,h-29,15,muted);

        } else if(page==Learn) {
            // Lessons use a horizontal syllabus above a full-width reading desk.
            float bx=x,bw=width;
            for(int i=0;i<4;++i)
                if(Button({x+i*102.0f,105,94,32},paths[i],path==i)) { path=i; offset=0; }
            auto list=filter(path);
            if(std::find(list.begin(),list.end(),current)==list.end()) pick(list.front());
            int visible=std::max(2,static_cast<int>((width-100)/185));
            int maxOffset=std::max(0,static_cast<int>(list.size())-visible);
            if(previousPage!=Learn) {
                int selected=static_cast<int>(std::find(list.begin(),list.end(),current)-list.begin());
                offset=std::clamp(selected-visible/2,0,maxOffset);
            }
            if(CheckCollisionPointRec(GetMousePosition(),{x,145,width,66}))
                offset-=static_cast<int>(GetMouseWheelMove());
            offset=std::clamp(offset,0,maxOffset);
            Card({x,145,width,68},rail);
            if(Button({x+7,160,35,38},"<")) offset=std::max(0,offset-1);
            if(Button({x+width-43,160,35,38},">")) offset=std::min(maxOffset,offset+1);
            float lessonTile=(width-102)/visible;
            for(int i=0;i<visible && i+offset<static_cast<int>(list.size());++i) {
                int id=list[i+offset];
                float left=x+50+i*lessonTile;
                if(Button({left,155,lessonTile-8,48},lessons[id].title,current==id)) pick(id);
                if(scores[id].correct>0) DrawCircle(static_cast<int>(left+lessonTile-21),163,3,led);
            }
            const Lesson& item=lessons[current];
            Card({bx,226,bw,h-244},panel);
            float readingW=bw*0.38f;
            DrawRectangleRec({bx+readingW,279,1,h-424},seam);
            Text(TextFormat("%s   /   LESSON %02d",item.path,current+1),bx+25,241,14,led);
            float titleEnd=Wrap(item.title,bx+26,271,readingW-52,36,ink);
            float bodyEnd=Wrap(item.body,bx+26,titleEnd+19,readingW-55,20);
            float contentX=bx+readingW+24, contentW=bw-readingW-49, by=284;
            float actions=h-127;
            auto details=LessonDetails(current);
            if(!details.empty() && Button({bx+bw-142,236,127,32},lessonDetail ? "Code example" : "Deep dive",lessonDetail)) {
                lessonDetail=!lessonDetail; lessonExplore=false; lessonScroll=0;
            }
            if((current==2 || current==7 || current==12) &&
                Button({bx+bw-268,236,118,32},lessonExplore ? "Code example" : "Explore",lessonExplore)) {
                lessonExplore=!lessonExplore; lessonDetail=false;
            }
            const int viewKey=current*3+(lessonExplore ? 2 : lessonDetail ? 1 : 0);
            if(viewKey!=lessonKey) { lessonKey=viewKey; lessonAge=0; }
            lessonAge=motion ? lessonAge+dt : 1;
            if(lessonExplore) {
                LessonExplorer(current,exploreValue,exploreStep,exploreBound,{contentX,by,contentW,actions-by});
            } else if(lessonDetail && !details.empty()) {
                Rectangle area{contentX,by,contentW,actions-by-15};
                Card(area,rail);
                if(CheckCollisionPointRec(GetMousePosition(),area))
                    lessonScroll-=static_cast<int>(GetMouseWheelMove()*38);
                lessonScroll=std::max(0,lessonScroll);
                Clip(area.x+5,area.y+5,area.width-10,area.height-10);
                float dy=area.y+15-lessonScroll;
                for(size_t i=0;i<details.size();i+=2) {
                    Text(details[i],area.x+15,dy,21,led);
                    dy=Wrap(details[i+1],area.x+15,dy+29,area.width-30,19)+18;
                }
                EndScissorMode();
                lessonScroll=std::min(lessonScroll,std::max(0,static_cast<int>(dy+lessonScroll-area.y-area.height)));
            } else {
                Text("WORKED EXAMPLE",contentX,by,14,codeType);
                Example(item.code,{contentX,by+28,contentW,actions-by-40},18);
                if(bodyEnd+90<actions) {
                    Text("WATCH FOR THIS",bx+26,bodyEnd+25,14,led);
                    Wrap(LessonNote(current),bx+26,bodyEnd+51,readingW-55,17);
                }
            }
            if(lessonAge<0.38f) {
                const float settle=Arrival(lessonAge/0.38f);
                DrawRectangleGradientV(static_cast<int>(contentX),static_cast<int>(by),static_cast<int>(contentW),
                                       static_cast<int>(actions-by),Fade(panel,0.38f*(1-settle)),Fade(panel,0.80f*(1-settle)));
                DrawRectangleRec({contentX+4,by+1,(contentW-8)*settle,2},Fade(primary,1-settle));
            }
            if(Button({bx+15,actions,112,37},"Practice",true)) startQuiz({current});
            if(Button({bx+134,actions,127,37},"Edit example")) {
#ifdef _WIN32
                loadCode(item.code);
                cppMode=std::string(item.path)!="C";
                challenge=-1;
                challengePicker=false;
#else
                editor.cursor=0;
                editor.anchor=editor.code.size();
                editor.Replace(item.code);
                editor.scroll=0;
                editor.horizontal=0;
                editor.cursor=editor.anchor=0;
#endif
                page=Notebook;
            }
            int linkedLab=-1;
            const std::string title=item.title;
            if(current==2 || title=="Pointer arithmetic") linkedLab=0;
            else if(current==3) linkedLab=4;
            else if(title=="Recursion") linkedLab=3;
            else if(title=="Structures and allocation") linkedLab=2;
            else if(title=="Arrays and strings") linkedLab=1;
            if(linkedLab>=0 && Button({bx+268,actions,117,37},"Visual lab")) {
                lab=linkedLab; step=0; playing=false; timer=0; page=Visualize;
            }
#ifdef _WIN32
            int related=current==2 ? 4 : current==8 ? 2 : current==7 ? 1 : current==15 ? 5 : -1;
            if(related>=0 && Button({bx+15,actions+43,144,32},"Coding challenge"))
                selectChallenge(related);
#endif
            auto position=std::find(list.begin(),list.end(),current);
            if(position!=list.begin() && Button({bx+bw-199,actions+43,96,32},"Previous")) {
                pick(*(position-1));
                offset=std::min(offset,static_cast<int>(position-list.begin())-1);
            }
            if(position+1!=list.end() && Button({bx+bw-95,actions+43,86,32},"Next")) {
                pick(*(position+1));
                offset=std::max(offset,static_cast<int>(position-list.begin())+2-visible);
            }
        } else if(page==Assess) {
            if(quiz.questions.empty()) {
                float leftW=width*0.55f, rightX=x+leftW+14, rightW=width-leftW-14;
                Card({x,109,leftW,h-132},panel);
                DrawRectangleGradientV(static_cast<int>(x+1),110,static_cast<int>(leftW-2),
                                       static_cast<int>((h-132)*0.55f),litSurface,panel);
                Text("Practice changes understanding.",x+25,146,32);
                Wrap("Choose a lesson to check what you know, or take a mixed assessment across the curriculum.",
                     x+26,205,leftW-52,19);
                DrawRectangleRec({x+25,288,leftW-50,1},seam);
                Text("YOUR NEXT CHECKPOINT",x+26,310,14,led);
                Text(lessons[current].title,x+26,341,25);
                if(Button({x+25,405,leftW-50,49},"Practice this lesson",true)) startQuiz({current});
                if(Button({x+25,467,leftW-50,47},"Start mixed assessment")) mixed();
                Text("Hints are available before answering; explanations follow.",x+26,541,16,muted);
                Card({rightX,109,rightW,h-132},panel);
                Text("Focused practice",rightX+19,132,24);
                Wrap("Questions from one track",rightX+20,168,rightW-40,17);
                for(int i=1;i<=3;++i) {
                    if(Button({rightX+19,206+(i-1)*53.0f,rightW-38,43},TextFormat("%s practice",paths[i]))) {
                        auto pool=filter(i);
                        std::shuffle(pool.begin(),pool.end(),rng);
                        startQuiz(std::move(pool));
                    }
                }
                DrawRectangleRec({rightX+19,383,rightW-38,1},seam);
                Text("Learning tools",rightX+19,401,21);
                for(int i=0;i<3;++i) {
                    if(Button({rightX+19,441+i*53.0f,rightW-38,43},i==0 ? "Open current lesson" :
                              i==1 ? "Open visual lab" : "Open notebook")) {
                        if(i==0) { path=0; page=Learn; }
                        else if(i==1) {
                            const std::string topic=lessons[current].title;
                            lab=current==3 ? 4 : topic=="Structures and allocation" ? 2 :
                                topic=="Recursion" ? 3 :
                                topic=="Arrays and strings" || topic=="Loops and functions" ? 1 : 0;
                            step=0; playing=false; timer=0; page=Visualize;
                        } else page=Notebook;
                    }
                }
            } else if(quiz.index>=quiz.questions.size()) {
                Card({x,109,width,h-132},rail);
                DrawRectangleGradientH(static_cast<int>(x+1),110,static_cast<int>(width-2),
                                       static_cast<int>((h-132)*0.55f),litSurface,panel);
                Text("Assessment complete",x+30,145,32);
                MasteryRing({x+width*0.72f,329},static_cast<float>(quiz.correct)/quiz.questions.size());
                Text(TextFormat("%d / %d",quiz.correct,static_cast<int>(quiz.questions.size())),x+35,239,51);
                Text("questions correct",x+37,306,21,muted);
                Wrap(quiz.mistakes.empty() ? "All answers correct. Your mastery is saved." :
                     "Retry missed questions and revisit the explanations.",x+36,383,width*0.53f,20);
                DrawRectangleRec({x+34,464,width-68,1},seam);
                Text("Keep learning",x+36,489,22);
                if(Button({x+35,544,155,45},"Retry",true)) quiz.Start(quiz.questions,rng);
                if(!quiz.mistakes.empty() && Button({x+202,544,171,45},"Review mistakes")) quiz.Start(quiz.mistakes,rng);
                if(Button({x+385,544,159,45},"Quiz menu")) quiz=QuizState{};
            } else {
                int id=quiz.questions[quiz.index];
                const Lesson& item=lessons[id];
                Card({x,109,width,h-173},rail);
                DrawRectangleGradientH(static_cast<int>(x+2),111,static_cast<int>(width-4),76,
                                       Fade(seam,0.42f),Fade(rail,0));
                Text(TextFormat("%s   /   QUESTION %d OF %d",item.title,static_cast<int>(quiz.index)+1,
                     static_cast<int>(quiz.questions.size())),x+20,130,15,led);
                float segment=(width-48)/quiz.questions.size();
                for(size_t i=0;i<quiz.questions.size();++i)
                    Bar(x+24+i*segment,154,segment-4,std::clamp(quizProgress*quiz.questions.size()-i,0.0f,1.0f));
                float questionW=width*0.51f;
                float answerX=x+questionW+17, answerW=width-questionW-40;
                DrawRectangleRec({answerX-12,181,1,h-297},seam);
                int nextQuestionKey=id*100+static_cast<int>(quiz.index);
                if(questionKey!=nextQuestionKey || previousPage!=Assess) {
                    questionKey=nextQuestionKey; quizAge=0;
                }
                quizAge=motion ? quizAge+dt : 1;
                float yy=Wrap(item.question,x+24,186+13*(1-Arrival(quizAge/0.36f)),questionW-48,28,ink)+19;
                bool prediction=std::string(item.question)=="Predict the output.";
                if(prediction ||
                   std::string(item.question).find("value of bytes")!=std::string::npos ||
                   std::string(item.question)=="What is value?") {
                    Example(item.code,{x+24,yy,questionW-48,175},18);
                    yy+=144;
                }
                auto record=[&](int choice) {
                    if(quiz.selected!=-1) return;
                    quiz.selected=choice;
                    answerAge=0;
                    Score& score=scores[id];
                    if(score.attempts<1000000) ++score.attempts;
                    if(choice==0) {
                        ++quiz.correct;
                        if(score.correct==0) reward=2.4f;
                        if(score.correct<score.attempts) ++score.correct;
                    } else quiz.mistakes.push_back(id);
                    saveNow();
                };
                if(captureMotion && demoElapsed>=33.0f && quiz.selected==-1) record(0);
                if(prediction) {
                    if(quiz.selected==-1) {
                        for(int c=GetCharPressed();c>0;c=GetCharPressed())
                            if((c>='0' && c<='9') && quiz.prediction.size()<12) quiz.prediction+=static_cast<char>(c);
                        if((IsKeyPressed(KEY_BACKSPACE) || IsKeyPressedRepeat(KEY_BACKSPACE)) && !quiz.prediction.empty())
                            quiz.prediction.pop_back();
                    }
                    float answerY=230;
                    Card({answerX,answerY,answerW,51},rail);
                    Text(quiz.prediction.empty() ? "Type the integer output" : quiz.prediction.c_str(),
                         answerX+14,answerY+13,21,quiz.prediction.empty() ? muted : ink);
                    if(quiz.selected==-1 && (Button({answerX,answerY+64,answerW,46},"Check answer",true) || IsKeyPressed(KEY_ENTER))
                       && !quiz.prediction.empty()) record(quiz.prediction==item.answers[0] ? 0 : 1);
                    yy=answerY+128;
                } else {
                    yy=192;
                    for(int i=0;i<4;++i) {
                        int choice=quiz.order[i];
                        bool marked=quiz.selected!=-1 && (quiz.selected==choice || choice==0);
                        if(Button({answerX,yy+i*66,answerW,54},
                                  TextFormat("%c   %s",'A'+i,item.answers[choice]),
                                  marked,choice==0 ? successColor : errorColor)) record(choice);
                    }
                    yy+=278;
                }
                if(quiz.selected==-1) {
                    if(Button({answerX,yy,91,43},"Hint",quiz.hint)) quiz.hint=!quiz.hint;
                    if(quiz.hint) Wrap(item.hint,answerX,yy+53,answerW,18);
                } else {
                    if(lastAnswer!=quiz.selected) answerAge=0;
                    lastAnswer=quiz.selected;
                    answerAge=motion ? answerAge+dt : 1;
                    const float feedback=Ease(answerAge/0.56f);
                    const Color resultTone=quiz.selected==0 ? successColor : errorColor;
                    DrawRectangleRec({answerX-10,yy,4,54*feedback},resultTone);
                    DrawRectangleRounded({answerX-5,yy-5,answerW+5,55},0.19f,8,
                                          Fade(quiz.selected==0 ? successFill : errorFill,0.30f+0.58f*(1-feedback)));
                    if(feedback<1) {
                        const float edge=answerX+answerW*feedback;
                        DrawLineEx({answerX,yy-6},{edge,yy-6},2,Fade(resultTone,1-feedback));
                    }
                    yy=Wrap(quiz.selected==0 ? "Correct" : TextFormat("Incorrect. Correct answer: %s",item.answers[0]),
                            answerX,yy,answerW,20,
                            quiz.selected==0 ? successColor : errorColor);
                    yy=Wrap(item.explanation,answerX,yy+5,answerW,17);
                    if(Button({answerX,yy+8,158,40},quiz.index+1==quiz.questions.size() ? "See results" : "Next question",true))
                        quiz.Advance(rng);
                }
                if(Button({x+width-123,h-89,104,33},"Exit quiz")) quiz=QuizState{};
            }


        } else if(page==Visualize) {
#include "visual_labs_ui.inc"
        } else if(page==Notebook) {
            Text("C / C++ SYNTAX NOTEBOOK",x,108,14,led);
#ifdef _WIN32
            if(Button({x,135,91,35},cppMode ? "C++17" : "C17",true)) cppMode=!cppMode;
            if(Button({x+99,135,126,35},"Hello sample")) {
                loadCode(cppMode ? "#include <iostream>\n\nint main() {\n    std::cout << \"Hello, C++!\\n\";\n}"
                                 : "#include <stdio.h>\n\nint main(void) {\n    puts(\"Hello, C!\");\n}");
                challenge=-1;
            }
            if(Button({x+233,135,137,35},"Challenges",challengePicker)) challengePicker=!challengePicker;
            if(challengePicker) {
                Text("Choose a challenge",x,191,30);
                float tile=(width-18)/3;
                for(int i=0;i<static_cast<int>(std::size(challenges));++i) {
                    float left=x+(i%3)*(tile+9), top=245+(i/3)*152.0f;
                    Card({left,top,tile,138});
                    Wrap(challenges[i].instruction,left+12,top+57,tile-24,17);
                    if(Button({left+10,top+10,tile-20,38},challenges[i].title,challenge==i)) {
                        selectChallenge(i);
                    }
                }
            } else {
            bool running=job.valid();
            if(Button({x+378,135,181,35},running ? "Working..." : challenge>=0 ? "Run four tests" : "Build and run",!running) && !running) {
                console="Compiling...";
                outputScroll=0;
                job=std::async(std::launch::async,CompileAndRun,editor.code,cppMode,challenge);
            }
            if(challenge>=0) {
                Wrap(challenges[challenge].instruction,x,180,width,17);
                Text(TextFormat("Example input: %s   |   Expected output: %d",challenges[challenge].inputs[0],
                                challenges[challenge].expected[0]),x,205,15,led);
            } else Text("Run only code you trust. It executes locally with your account permissions.",x,182,16,muted);
            float editorWidth=width*0.62f;
            float workspaceY=253;
            float workspaceHeight=h-workspaceY-59;
            float editorHeight=workspaceHeight;
            float outputY=workspaceY;
            float outputHeight=editorHeight;
            float consoleX=x+editorWidth+14;
            DrawRectangleRec({x,workspaceY-35,width,27},rail);
            DrawRectangleRec({x,workspaceY-35,3,27},led);
            Text(cppMode ? "main.cpp" : "main.c",x+13,workspaceY-31,16,ink,true);
            Text("SOURCE",x+editorWidth-82,workspaceY-29,13,muted);
            const char* outputTitle=running ? "COMPILING AND RUNNING" : console.empty() ? "OUTPUT  /  READY" :
                console.find("Build failed")!=std::string::npos ? "OUTPUT  /  BUILD FAILED" : "OUTPUT  /  RUN REPORT";
            Text(outputTitle,consoleX+10,outputY-29,15,led);
            if(running) {
                float pulse=motion ? static_cast<float>(std::fmod(GetTime()*0.6,1.0)) : 0.5f;
                const float track=width-editorWidth-20;
                DrawRectangleRec({consoleX,outputY-5,track,3},seam);
                const float sweep=motion ? 1-std::abs(2*pulse-1) : 0.5f;
                DrawRectangleRec({consoleX+sweep*(track-49),outputY-5,49,3},signal);
            }
            editor.Draw({x,workspaceY,editorWidth,editorHeight});
            Rectangle outputBox{consoleX,outputY,width-editorWidth-14,outputHeight};
            DrawRectangleRounded(outputBox,0.10f,10,terminal);
            std::istringstream stream(console.empty() ? "Compiler output appears here.\nRun an example or choose\na coding challenge." : console);
            std::vector<std::string> lines;
            std::string line;
            float glyph=MeasureTextEx(mono,"M",17,0.5f).x+0.5f;
            size_t columns=static_cast<size_t>((outputBox.width-26)/glyph);
            while(std::getline(stream,line)) {
                size_t pos=0;
                while(pos<line.size()) {
                    size_t end=std::min(pos+columns,line.size());
                    if(end<line.size()) {
                        size_t space=line.rfind(' ',end);
                        if(space!=std::string::npos && space>pos+columns/2) end=space+1;
                    }
                    lines.push_back(line.substr(pos,end-pos));
                    pos=end;
                }
                if(line.empty()) lines.emplace_back();
            }
            if(CheckCollisionPointRec(GetMousePosition(),outputBox)) outputScroll-=static_cast<int>(GetMouseWheelMove())*3;
            int outputRows=static_cast<int>((outputBox.height-18)/20);
            outputScroll=std::clamp(outputScroll,0,std::max(0,static_cast<int>(lines.size())-outputRows));
            Clip(outputBox.x+8,outputBox.y+7,outputBox.width-16,outputBox.height-14);
            for(int i=0;i<outputRows && i+outputScroll<static_cast<int>(lines.size());++i) {
                const auto& output=lines[i+outputScroll];
                Color tone=output.find("FAIL")!=std::string::npos || output.find("error ")!=std::string::npos ? terminalError :
                           output.find("PASS")!=std::string::npos || output=="Build succeeded" ? signal : draft;
                Text(output.c_str(),outputBox.x+12,outputBox.y+10+i*20,16,tone,true);
            }
            EndScissorMode();
            if(outputAge<0.55f && !running) {
                const float sweep=Arrival(outputAge/0.55f);
                const Color completed=console.find("Build succeeded")!=std::string::npos ? signal : errorColor;
                DrawRectangleRoundedLinesEx(outputBox,0.10f,10,2,Fade(completed,1-sweep));
                DrawRectangleRec({outputBox.x+13,outputY+4,(outputBox.width-26)*sweep,2},Fade(completed,1-sweep));
            }
            Text("Ctrl+A/C/X/V  Select/copy/cut/paste   Ctrl+Z/Y  Undo/redo   Shift+wheel  Scroll sideways",
                 x,h-37,16,muted);
            }
#else
            Text("Edit examples or draft your own program. Copy it into your local compiler to run it.",x,137,17,muted);
            editor.Draw({x,171,width,h-272});
            Text("Ctrl+A/C/X/V  Select/copy/cut/paste    Ctrl+Z/Y  Undo/redo    Shift+wheel  Scroll sideways",
                 x,h-81,15,muted);
            Text("Notebook edits stay in memory for this session. This screen has no compiler.",x,h-54,15,muted);
#endif
        } else if(page==Progress) {
            // A single progress instrument anchors the page; tracks and awards follow.
            float top=115, featureW=width*0.29f, tracksX=x+featureW+22;
            float tracksW=width-featureW-22, upperH=h-top-193;
            DrawRectangleRounded({x,top,featureW,upperH},0.12f,10,hero);
            Text("Your learning",x+23,top+25,26,WHITE);
            Text(TextFormat("%d%%",static_cast<int>(displayed*100+0.5f)),x+21,top+86,72,signal,true);
            DrawRectangleRec({x+24,top+178,featureW-48,7},draft);
            DrawRectangleRec({x+24,top+178,(featureW-48)*displayed,7},signal);
            Text(TextFormat("Level %d",level),x+24,top+220,29,WHITE);
            Text(TextFormat("%d XP earned",xp),x+25,top+266,18,draft);
            Text(TextFormat("%d / %d lessons mastered",mastered,static_cast<int>(lessons.size())),x+25,top+324,17,WHITE);
            Text(attempts ? TextFormat("%d%% answer accuracy",static_cast<int>(100.0*correct/attempts)) :
                            "No answers recorded yet",x+25,top+352,17,draft);
            Card({tracksX,top,tracksW,upperH},panel);
            Text("Curriculum tracks",tracksX+19,top+16,24);
            float row=(upperH-57)/3;
            for(int p=1;p<=3;++p) {
                auto list=filter(p);
                int done=0,tried=0,correctForPath=0;
                for(int id:list) { done+=scores[id].correct>0; tried+=scores[id].attempts; correctForPath+=scores[id].correct; }
                float y=top+49+(p-1)*row;
                if(p>1) DrawRectangleRec({tracksX+15,y-5,tracksW-30,1},seam);
                Text(paths[p],tracksX+21,y+8,22);
                Text(TextFormat("%d / %d mastered",done,static_cast<int>(list.size())),tracksX+21,y+39,16,muted);
                Text(TextFormat("%d / %d correct",correctForPath,tried),tracksX+tracksW-124,y+40,14,muted);
                float track=(tracksW-48)/list.size();
                for(size_t i=0;i<list.size();++i) {
                    int id=list[i];
                    Vector2 node{tracksX+23+i*track+track/2,y+row-25};
                    bool hover=CheckCollisionPointCircle(GetMousePosition(),node,10);
                    if(i>0) DrawLineEx({node.x-track,node.y},node,2,seam);
                    if(hover || scores[id].correct)
                        DrawCircleGradient(static_cast<int>(node.x),static_cast<int>(node.y),11,Fade(led,0.6f),Fade(led,0));
                    DrawCircleV(node,hover ? 5.0f : 4.0f,scores[id].correct ? led : muted);
                    DrawCircleLinesV(node,hover ? 8.0f : 6.0f,Fade(scores[id].correct ? led : muted,0.72f));
                    if(hover) Text(lessons[id].title,tracksX+23,y+57,15,ink);
                    if(hover && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                        path=p; offset=static_cast<int>(i); pick(id); page=Learn;
                    }
                }
            }
            float awardsY=top+upperH+15;
            Text("Achievements",x,awardsY,23);
            const char* badges[]={"First Mastery","Pointer Explorer","Curriculum Complete"};
            const char* goals[]={"Answer a lesson question correctly.","Master the Pointers quiz.","Master every lesson."};
            bool unlocked[]={mastered>0,scores[2].correct>0,mastered==static_cast<int>(lessons.size())};
            float awardGap=12, awardW=(width-2*awardGap)/3;
            for(int i=0;i<3;++i) {
                float left=x+i*(awardW+awardGap), y=awardsY+35;
                DrawRectangleRec({left,y,awardW,2},unlocked[i] ? primary : seam);
                Vector2 badge{left+23,y+26};
                if(motion && reward>0 && unlocked[i] && (i==0 ? mastered==1 : i==1 ? current==2 : mastered==static_cast<int>(lessons.size()))) {
                    float halo=Arrival((2.4f-reward)/0.72f);
                    DrawCircleLinesV(badge,8+12*halo,Fade(signalInk,1-halo));
                }
                DrawCircleV(badge,7,unlocked[i] ? led : muted);
                Text(badges[i],left+40,y+14,19,unlocked[i] ? led : ink);
                Wrap(goals[i],left+17,y+53,awardW-34,16,muted);
            }
            std::vector<int> weak;
            for(size_t i=0;i<scores.size();++i)
                if(scores[i].attempts>0 && scores[i].correct==0) weak.push_back(static_cast<int>(i));
            if(!weak.empty() && Button({x+width-198,awardsY-3,198,32},"Practice weak topics")) startQuiz(std::move(weak));
        }
        if(reward>0) {
            float lift=motion ? 1-Arrival((2.4f-reward)/0.42f) : 0;
            float opacity=motion ? std::min(1.0f,reward*3.5f) : 1;
            float toastX=w-214+26*lift;
            DrawRectangleRounded({toastX,62,181,38},0.34f,10,Fade(hero,opacity));
            DrawCircleV({toastX+17,81},5,Fade(signal,opacity));
            if(motion && reward>1.65f) {
                float ring=Ease((2.4f-reward)/0.75f);
                DrawCircleLinesV({toastX+17,81},6+8*ring,Fade(signal,opacity*(1-ring)));
            }
            Text("+100 XP   /   MASTERY",toastX+29,73,15,Fade(onDark,opacity));
        }
        previousPage=renderedPage;
        if(pageAge<0.38f) {
            float cover=1-Arrival(pageAge/0.38f);
            DrawRectangleGradientH(static_cast<int>(x),103,static_cast<int>(width),static_cast<int>(h-103),
                                   Fade(bg,0.18f*cover),Fade(bg,0.64f*cover));
        }
        if(introExit<1) {
            float curtain=w*(1-Ease(introExit));
            Clip(0,0,curtain,h+42);
            DrawStudioIntro(w,h+42,4);
            EndScissorMode();
            DrawRectangleRec({curtain-2,0,2,h+42},signal);
        }
        if(captureScreens && captureIndex>=13) DrawStudioIntro(w,h+42,captureIndex==13 ? 2.75f : 1.15f);
        endFrame();
        if(captureScreens && ++captureFrame>=18) {
            const char* names[]={"home","lessons","notebook","progress","assessments",
                                 "quiz","quiz-result","lab-pointer","lab-array","lab-heap",
                                 "lab-recursion","lab-linked","lab-bubble","intro","intro-in-motion"};
            Image shot=LoadImageFromScreen();
            if(shot.width>GetScreenWidth() || shot.height>GetScreenHeight())
                ImageCrop(&shot,{0,static_cast<float>(GetScreenHeight()),
                                 static_cast<float>(GetScreenWidth()),static_cast<float>(GetScreenHeight())});
            auto filename=(captureDirectory/(std::string(names[captureIndex])+".png")).string();
            ExportImage(shot,filename.c_str());
            UnloadImage(shot);
            ++captureIndex;
            captureFrame=0;
            if(captureIndex==15) break;
        }
    }
    if(captureMotion) UnloadRenderTexture(recording);
    if(ui.texture.id!=GetFontDefault().texture.id) UnloadFont(ui);
    if(mono.texture.id!=GetFontDefault().texture.id) UnloadFont(mono);
    if(heading.texture.id!=GetFontDefault().texture.id) UnloadFont(heading);
    CloseWindow();
    return 0;
}
