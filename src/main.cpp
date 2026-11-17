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
void Card(Rectangle r, Color c = panel) {
    float radius=std::min(0.32f,20.0f/std::min(r.width,r.height));
    DrawRectangleRounded({r.x+1,r.y+5,r.width,r.height},radius,10,Fade(BLACK,0.32f));
    DrawRectangleRounded(r,radius,10,c);
    DrawRectangleRoundedLinesEx(r,radius,10,1,Fade(seam,0.64f));
    DrawLineEx({r.x+12,r.y+2},{r.x+r.width-12,r.y+2},1,Fade(ink,0.08f));
}
bool Button(Rectangle r, const char* s, bool active = false, Color tint = led) {
    bool hover = CheckCollisionPointRec(GetMousePosition(), r);
    Card(r, active ? litSurface : hover ? hoverSurface : rail);
    if(active || hover) {
        DrawRectangleRoundedLinesEx(r,0.25f,10,1.3f,Fade(active ? tint : codeType,0.88f));
        DrawRectangleRec({r.x+5,r.y+8,2,r.height-16},active ? tint : seam);
    }
    float size=18;
    float available=r.width-20;
    while(size>13 && MeasureTextEx(ui,s,size,0.5f).x>available) --size;
    Clip(r.x,r.y,r.width,r.height);
    Text(s, r.x + 13, r.y + (r.height - size) / 2, size, active ? tint : ink);
    EndScissorMode();
    return hover && IsMouseButtonPressed(MOUSE_BUTTON_LEFT);
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
    Text(name,x,y,14,led);
    DrawRectangleRec({x,y+24,width,1},seam);
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
            if (c >= '0' && c <= '9') color = Color{255,198,120,255};
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
    bool introEnabled=!std::filesystem::exists(introSetting,introError);
    float introTime=introEnabled ? 0.0f : 4.0f;
    int current=0, path=0, offset=0, lab=0, step=0, speed=1;
    ReadProgress(save,scores,current);
    bool saved=true, playing=false, motion=true;
    bool fullscreenRequested=false, lessonDetail=false, phasePaused=false;
    MemoryBoard memoryBoard;
    int lessonScroll=0,exploreStep=0,exploreValue=15,exploreBound=4;
    bool lessonExplore=false;
    float timer=0, reward=0, displayed=0;
    float labMotion=1, quizProgress=0, reveal=1;
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
    QuizState quiz;
    Editor editor;
    editor.code=lessons[current].code;
#ifdef _WIN32
    bool cppMode=false;
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
    auto saveNow=[&]{ saved=WriteProgress(save,scores,current); };
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
    while(!WindowShouldClose()) {
        bool keyActivity=false, fullscreenKey=false;
        for(int key=GetKeyPressed();key!=0;key=GetKeyPressed()) {
            keyActivity=true;
            fullscreenKey=fullscreenKey || key==KEY_F11;
        }
        if(fullscreenKey || fullscreenRequested) ToggleBorderlessWindowed();
        fullscreenRequested=false;
        float screenW=static_cast<float>(GetScreenWidth()), screenH=static_cast<float>(GetScreenHeight());
        uiScale=std::min(screenW/1120.0f,screenH/748.0f);
        float w=std::min(1530.0f,screenW/uiScale), h=screenH/uiScale;
        uiInset=static_cast<int>((screenW-w*uiScale)/2);
        SetMouseOffset(-uiInset,0);
        SetMouseScale(1/uiScale,1/uiScale);
        float x=235, width=w-x-25, dt=GetFrameTime();
#ifdef _WIN32
        if(job.valid() && job.wait_for(std::chrono::seconds(0))==std::future_status::ready) {
            try { console=job.get(); }
            catch(const std::exception&) { console="The build workspace could not finish the request."; }
            outputScroll=0;
        }
#endif
        int mastered=0, attempts=0, correct=0;
        for(const Score& s:scores) { mastered+=s.correct>0; attempts+=s.attempts; correct+=s.correct; }
        int xp=mastered*100, level=xp/300+1;
        float fill=static_cast<float>(mastered)/lessons.size();
        displayed+= (fill-displayed)*(motion ? std::min(1.0f,dt*10) : 1.0f);
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

        BeginDrawing();
        ClearBackground(bg);
        BeginMode2D({{static_cast<float>(uiInset),0},{0,0},0,uiScale});
        if(introTime<4) {
            bool skip=IsMouseButtonPressed(MOUSE_BUTTON_LEFT) || keyActivity || !motion;
            introTime=skip ? 4 : std::min(4.0f,introTime+dt);
            float open=Ease(introTime/1.1f), fade=Ease((4-introTime)/0.65f);
            DrawRectangleGradientH(0,0,static_cast<int>(w),static_cast<int>(h),rail,bg);
            for(int i=0;i<14;++i) {
                float radius=45+i*19.0f+introTime*7;
                DrawRing({w/2,h/2-36},radius,radius+1,31+i*17+introTime*24,224+i*17+introTime*24,
                         64,Fade(i%2 ? led : codeType,(0.16f-i*0.007f)*fade));
            }
            const char* marks[]={"{ }","< >","&","*","[ ]"};
            for(int i=0;i<5;++i) {
                float angle=i*1.2566f+introTime*0.22f;
                Text(marks[i],w/2+std::cos(angle)*(108+open*103),h/2-37+std::sin(angle)*139,
                     27,Fade(i%2 ? led : codeType,open*fade*0.65f),true);
            }
            Card({w/2-190,h/2-84+(1-open)*26,380,103},Fade(rail,open*fade));
            Text("CodePath Studio",w/2-157,h/2-65+(1-open)*26,46,Fade(ink,open*fade));
            Text("C AND C++ LEARNING WORKSPACE",w/2-137,h/2+30,16,Fade(led,open*fade));
            Text("Click or press a key to skip",w/2-98,h-59,17,muted);
            EndMode2D();
            EndDrawing();
            continue;
        }
        DrawRectangleGradientV(208,0,static_cast<int>(w)-208,static_cast<int>(h),Color{13,42,45,255},bg);
        DrawRectangleGradientV(0,0,208,static_cast<int>(h),Color{13,47,50,255},Color{10,30,33,255});
        DrawLineEx({208,0},{208,h},1,seam);
        for(int i=0;i<8;++i) {
            float yy=108+i*i*77.0f;
            DrawLineEx({208,yy},{w,yy-h*0.23f},1,Fade(seam,0.11f));
        }
        Card({20,27,42,42},rail);
        DrawRectangleRoundedLinesEx({20,27,42,42},0.27f,8,2,led);
        Text(">_",27,35,22,led,true);
        Text("CodePath",73,26,24);
        Text("S T U D I O",74,53,12,led);
        const Page navOrder[]={Home,Learn,Visualize,Notebook,Assess,Progress};
        for(int i=0;i<6;++i) {
            Page destination=navOrder[i];
            Rectangle item{20,115+i*50.0f,167,41};
            bool hovered=CheckCollisionPointRec(GetMousePosition(),item);
            Card(item,page==destination ? litSurface : hovered ? hoverSurface : rail);
            if(page==destination) DrawRectangleRoundedLinesEx(item,0.25f,8,1.5f,led);
            NavIcon(destination,{item.x+21,item.y+20},page==destination ? led : muted);
            Text(nav[destination],item.x+43,item.y+11,16,page==destination ? ink : muted);
            if(hovered && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                if(page!=destination) playing=false;
                page=destination;
            }
        }
        if(Button({20,h-124,167,32},motion ? "Motion on" : "Motion off")) motion=!motion;
        if(Button({20,h-207,167,34},introEnabled ? "Intro on" : "Intro off")) {
            if(introEnabled) {
                std::filesystem::create_directories(save.parent_path(),introError);
                std::ofstream marker(introSetting);
                if(marker) introEnabled=false;
            } else {
                std::filesystem::remove(introSetting,introError);
                if(!introError) introEnabled=true;
            }
        }
        Bar(21,h-72,166,displayed);
        Text(TextFormat("LEVEL %d  /  %d XP",level,xp),21,h-55,15,led);
        Text("LOCAL PROFILE",21,h-31,12,muted);
        if(Button({20,h-167,167,34},IsWindowState(FLAG_BORDERLESS_WINDOWED_MODE) ? "Exit fullscreen" : "Fullscreen  F11"))
            fullscreenRequested=true;
        Text("CODEPATH STUDIO   /   NATIVE LEARNING WORKSPACE",x,19,14,led);
        Text(nav[page],x,43,39);
        if(!saved) Text("Progress could not be saved",x,h-25,15,errorColor);

        if(page==Home) {
            float heroBottom=h*0.63f;
            InstrumentHeading("MEMORY WORKBENCH",x,116,width);
            float sideWidth=198;
            PremiumLabs::DrawHomeMemory(memoryBoard,{x,154,width-sideWidth-22,heroBottom-205},motion);
            memoryBoard.Update(dt,motion);
            float tools=x+width-sideWidth;
            Text("EXECUTE",tools,161,15,led);
            Text(TextFormat("p = 0x%X",0x100+4*memoryBoard.target),tools,189,22,ink,true);
            Wrap("Select a cell to redirect p. Addresses are symbolic.",tools,228,sideWidth,17);
            if(Button({tools,288,sideWidth,42},memoryBoard.writing ? "Writing..." : "Execute ++*p",true) && !memoryBoard.writing) {
                memoryBoard.pendingTarget=memoryBoard.target;
                memoryBoard.travel=0;
                memoryBoard.writing=true;
            }
            if(Button({tools,338,sideWidth,40},"Open pointer lab")) {
                lab=0; step=0; playing=false; timer=0; page=Visualize;
            }
            if(Button({x,heroBottom-40,183,36},"Continue learning",true)) {
                path=0;
                auto list=filter(path);
                offset=std::max(0,static_cast<int>(std::find(list.begin(),list.end(),current)-list.begin())-2);
                page=Learn;
            }
            float routeY=heroBottom+12, routeW=width*0.65f;
            InstrumentHeading("LEARNING NETWORK",x,routeY,routeW-15);
            for(int i=0;i<2;++i) {
                float row=routeY+32+i*65;
                auto course=filter(i+1);
                int done=0;
                for(int id:course) done+=scores[id].correct>0;
                if(Button({x,row,117,40},i==0 ? "C track" : "C++ track")) {
                    path=i+1; offset=0;
                    auto next=std::find_if(course.begin(),course.end(),[&](int id){return scores[id].correct==0;});
                    pick(next==course.end() ? course.front() : *next);
                    page=Learn;
                }
                float start=x+137,end=x+routeW-15;
                for(size_t j=1;j<course.size();++j) {
                    float spacing=(end-start)/(course.size()-1);
                    DrawLineEx({start+(j-1)*spacing,row+20+10*std::sin((j-1)*0.8f)},
                               {start+j*spacing,row+20+10*std::sin(j*0.8f)},2,seam);
                }
                for(size_t j=0;j<course.size();++j) {
                    float px=start+(end-start)*j/(course.size()-1);
                    float py=row+20+10*std::sin(j*0.8f);
                    bool complete=scores[course[j]].correct>0;
                    bool hovered=CheckCollisionPointCircle(GetMousePosition(),{px,py},10);
                    if(complete || hovered) DrawCircleGradient(static_cast<int>(px),static_cast<int>(py),10,Fade(led,0.5f),Fade(led,0));
                    DrawCircleV({px,py},current==course[j] ? 6.0f : 4.0f,complete || current==course[j] || hovered ? led : muted);
                    if(hovered && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                        path=i+1; offset=static_cast<int>(j); pick(course[j]); page=Learn;
                    }
                }
                Text(TextFormat("%d / %d mastered",done,static_cast<int>(course.size())),start,row+35,14,muted);
            }
            float side=x+routeW+19;
            Text("QUICK PRACTICE",side,routeY,16,led);
            if(Button({side,routeY+32,width-routeW-19,40},"Mixed assessment")) mixed();
#ifdef _WIN32
            if(Button({side,routeY+83,width-routeW-19,40},"Coding challenges")) {
                challengePicker=true; page=Notebook;
            }
#endif
            Text(TextFormat("%d XP   /   %d of %d lessons mastered",xp,mastered,static_cast<int>(lessons.size())),
                 x,h-39,16,muted);

        } else if(page==Learn) {
            float menu=std::clamp(width*0.25f,218.0f,292.0f),bx=x+menu+14,bw=width-menu-14;
            Card({x,144,menu,h-200},rail);
            for(int i=0;i<4;++i)
                if(Button({x+i*53.0f,107,50,32},paths[i],path==i)) { path=i; offset=0; }
            auto list=filter(path);
            if(std::find(list.begin(),list.end(),current)==list.end()) pick(list.front());
            int visible=static_cast<int>((h-204)/40);
            int maxOffset=std::max(0,static_cast<int>(list.size())-visible);
            if(CheckCollisionPointRec(GetMousePosition(),{x,145,menu,h-205}))
                offset-=static_cast<int>(GetMouseWheelMove());
            offset=std::clamp(offset,0,maxOffset);
            for(int i=0;i<visible && i+offset<static_cast<int>(list.size());++i) {
                int id=list[i+offset];
                float row=149+i*40.0f;
                DrawLineEx({x+7,row},{x+7,row+40},1,seam);
                DrawCircleV({x+7,row+18},current==id ? 5.0f : 3.0f,current==id || scores[id].correct ? led : muted);
                if(Button({x+19,row,menu-19,36},lessons[id].title,current==id)) pick(id);
                if(scores[id].correct>0) DrawCircle(static_cast<int>(x+menu-6),static_cast<int>(row+18),3,current==id ? bg : led);
            }
            if(Button({x,h-52,91,34},"Up")) offset=std::max(0,offset-visible);
            if(Button({x+99,h-52,111,34},"Down")) offset=std::min(maxOffset,offset+visible);
            const Lesson& item=lessons[current];
            Card({bx,107,bw,h-146});
            Text(TextFormat("%s   /   LESSON %02d",item.path,current+1),bx+17,127,14,led);
            float titleEnd=Wrap(item.title,bx+19,152,bw-38,30,ink);
            float by=Wrap(item.body,bx+19,titleEnd+12,bw-38,20)+18;
            float actions=h-127;
            auto details=LessonDetails(current);
            if(!details.empty() && Button({bx+bw-142,115,127,32},lessonDetail ? "Code example" : "Deep dive",lessonDetail)) {
                lessonDetail=!lessonDetail; lessonExplore=false; lessonScroll=0;
            }
            if((current==2 || current==7 || current==12) &&
                Button({bx+bw-268,115,118,32},lessonExplore ? "Code example" : "Explore",lessonExplore)) {
                lessonExplore=!lessonExplore; lessonDetail=false;
            }
            if(lessonExplore) {
                LessonExplorer(current,exploreValue,exploreStep,exploreBound,{bx+17,by,bw-34,actions-by});
            } else if(lessonDetail && !details.empty()) {
                Rectangle area{bx+15,by,bw-30,actions-by-15};
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
            Text("WORKED EXAMPLE",bx+17,by,13,codeType);
            float codeHeight=actions-by-40;
            bool hasPracticeNote=codeHeight>=298;
            if(hasPracticeNote) codeHeight-=74;
            Example(item.code,{bx+15,by+24,bw-30,codeHeight},19);
            if(hasPracticeNote) {
                float noteY=by+codeHeight+37;
                Text("WATCH FOR THIS",bx+19,noteY,14,led);
                Wrap(LessonNote(current),
                     bx+19,noteY+22,bw-38,18);
            }
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
                Card({x,109,width,295},rail);
                DrawRectangleGradientH(static_cast<int>(x+2),111,static_cast<int>(width-4),180,
                                       Fade(seam,0.46f),Fade(rail,0));
                InstrumentHeading("PRACTICE MODES",x+20,130,width-40);
                Text("Put it to the test.",x+20,164,38);
                Wrap("Check the current lesson, try eight mixed questions, or focus on one track.",x+20,214,width-40,19);
                if(Button({x+20,256,161,41},"Current lesson",true)) startQuiz({current});
                if(Button({x+192,256,178,41},"Mixed assessment")) mixed();
                for(int i=1;i<=3;++i) {
                    if(Button({x+20+(i-1)*145.0f,316,133,39},TextFormat("%s practice",paths[i]))) {
                        auto pool=filter(i);
                        std::shuffle(pool.begin(),pool.end(),rng);
                        startQuiz(std::move(pool));
                    }
                }
                float lower=424, gap=13, tile=(width-2*gap)/3;
                const char* routeTitle[]={"1  Study a lesson","2  Inspect a model","3  Write and run code"};
                const char* routeDetail[]={"Review the current topic and its worked example.",
                                           "Watch a memory or data structure change step by step.",
                                           "Try an example or one of six coding challenges."};
                for(int i=0;i<3;++i) {
                    float left=x+i*(tile+gap);
                    Card({left,lower,tile,std::min(211.0f,h-lower-35)},rail);
                    Text(routeTitle[i],left+15,lower+17,20,i==1 ? led : ink);
                    Wrap(routeDetail[i],left+15,lower+51,tile-30,16,muted);
                    if(Button({left+15,lower+138,tile-30,38},i==0 ? "Open current lesson" :
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
                Card({x,109,width,239},rail);
                InstrumentHeading("ASSESSMENT COMPLETE",x+20,132,width-40);
                MasteryRing({x+width-100,228},static_cast<float>(quiz.correct)/quiz.questions.size());
                Text(TextFormat("%d / %d correct",quiz.correct,static_cast<int>(quiz.questions.size())),x+20,163,39);
                Text(quiz.mistakes.empty() ? "All answers correct. Your mastery is saved." : "Retry missed questions and revisit the explanations.",
                     x+20,222,18,muted);
                if(Button({x+20,281,110,39},"Retry",true)) quiz.Start(quiz.questions,rng);
                if(!quiz.mistakes.empty() && Button({x+139,281,155,39},"Review mistakes")) quiz.Start(quiz.mistakes,rng);
                if(Button({x+302,281,145,39},"Quiz menu")) quiz=QuizState{};
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
                float yy=Wrap(item.question,x+24,167,width-48,29,ink)+9;
                bool prediction=std::string(item.question)=="Predict the output.";
                if(prediction ||
                   std::string(item.question).find("value of bytes")!=std::string::npos ||
                   std::string(item.question)=="What is value?") {
                    Example(item.code,{x+24,yy,width-48,126},18);
                    yy+=144;
                }
                auto record=[&](int choice) {
                    if(quiz.selected!=-1) return;
                    quiz.selected=choice;
                    Score& score=scores[id];
                    if(score.attempts<1000000) ++score.attempts;
                    if(choice==0) {
                        ++quiz.correct;
                        if(score.correct==0) reward=2.4f;
                        if(score.correct<score.attempts) ++score.correct;
                    } else quiz.mistakes.push_back(id);
                    saveNow();
                };
                if(prediction) {
                    if(quiz.selected==-1) {
                        for(int c=GetCharPressed();c>0;c=GetCharPressed())
                            if((c>='0' && c<='9') && quiz.prediction.size()<12) quiz.prediction+=static_cast<char>(c);
                        if((IsKeyPressed(KEY_BACKSPACE) || IsKeyPressedRepeat(KEY_BACKSPACE)) && !quiz.prediction.empty())
                            quiz.prediction.pop_back();
                    }
                    Card({x+24,yy,286,51},rail);
                    Text(quiz.prediction.empty() ? "Type the integer output" : quiz.prediction.c_str(),
                         x+38,yy+13,21,quiz.prediction.empty() ? muted : ink);
                    if(quiz.selected==-1 && (Button({x+323,yy,141,51},"Check answer",true) || IsKeyPressed(KEY_ENTER))
                       && !quiz.prediction.empty()) record(quiz.prediction==item.answers[0] ? 0 : 1);
                    yy+=72;
                } else {
                    float answerWidth=(width-62)/2;
                    for(int i=0;i<4;++i) {
                        int choice=quiz.order[i];
                        bool marked=quiz.selected!=-1 && (quiz.selected==choice || choice==0);
                        if(Button({x+24+(i%2)*(answerWidth+14),yy+(i/2)*66,answerWidth,54},
                                  TextFormat("%c   %s",'A'+i,item.answers[choice]),
                                  marked,choice==0 ? successColor : errorColor)) record(choice);
                    }
                    yy+=139;
                }
                if(quiz.selected==-1) {
                    if(Button({x+24,yy,91,43},"Hint",quiz.hint)) quiz.hint=!quiz.hint;
                    if(quiz.hint) Wrap(item.hint,x+129,yy+9,width-155,20);
                } else {
                    DrawRectangleRec({x+14,yy,3,54},quiz.selected==0 ? successColor : errorColor);
                    Text(quiz.selected==0 ? "Correct" : TextFormat("Incorrect. Correct answer: %s",item.answers[0]),
                         x+24,yy,23,quiz.selected==0 ? successColor : errorColor);
                    yy=Wrap(item.explanation,x+24,yy+34,width-48,20);
                    if(Button({x+24,yy+10,158,43},quiz.index+1==quiz.questions.size() ? "See results" : "Next question",true))
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
            float column=std::clamp(width*0.34f,290.0f,465.0f);
            float editorWidth=width-column-15;
            float workspaceY=253;
            float workspaceHeight=h-workspaceY-59;
            DrawRectangleRec({x+editorWidth+6,workspaceY-31,1,workspaceHeight+31},seam);
            Text("SOURCE EDITOR",x,workspaceY-25,15,led);
            const char* outputTitle=running ? "COMPILING AND RUNNING" : console.empty() ? "OUTPUT  /  READY" :
                console.find("Build failed")!=std::string::npos ? "OUTPUT  /  BUILD FAILED" : "OUTPUT  /  RUN REPORT";
            Text(outputTitle,x+editorWidth+15,workspaceY-25,15,led);
            if(running) {
                float pulse=motion ? static_cast<float>(std::fmod(GetTime()*0.6,1.0)) : 0.5f;
                DrawRectangleRec({x+editorWidth+15+pulse*(column-35),workspaceY-5,35,2},led);
            }
            editor.Draw({x,workspaceY,editorWidth,workspaceHeight});
            Rectangle outputBox{x+editorWidth+15,workspaceY,column,workspaceHeight};
            Card(outputBox,rail);
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
            int outputRows=static_cast<int>((outputBox.height-24)/24);
            outputScroll=std::clamp(outputScroll,0,std::max(0,static_cast<int>(lines.size())-outputRows));
            Clip(outputBox.x+8,outputBox.y+7,outputBox.width-16,outputBox.height-14);
            for(int i=0;i<outputRows && i+outputScroll<static_cast<int>(lines.size());++i) {
                const auto& output=lines[i+outputScroll];
                Color tone=output.find("FAIL")!=std::string::npos || output.find("error ")!=std::string::npos ? errorColor :
                           output.find("PASS")!=std::string::npos || output=="Build succeeded" ? successColor : ink;
                Text(output.c_str(),outputBox.x+12,outputBox.y+12+i*24,17,tone,true);
            }
            EndScissorMode();
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
            float half=(width-14)/2,right=x+half+14;
            Card({x,112,half,155},rail);
            Card({right,112,half,155},rail);
            MasteryRing({x+half-83,187},displayed);
            Text("MASTERY SCORE",x+19,128,15,led);
            Text(TextFormat("%d XP  /  Level %d",xp,level),x+19,154,32);
            Text(TextFormat("%d lessons mastered",mastered),x+19,201,18,muted);
            Text("ASSESSMENT ACCURACY",right+19,128,15,codeType);
            Text(attempts ? TextFormat("%d%%",static_cast<int>(100.0*correct/attempts)) : "No attempts yet",right+19,154,32);
            Text(TextFormat("%d correct out of %d attempts",correct,attempts),right+19,201,18,muted);
            float start=281, area=h-start-22;
            float row=(area-57)/3;
            Card({x,start,half,area},rail);
            Card({right,start,half,area},rail);
            InstrumentHeading("SKILL MAP",x+18,start+17,half-36);
            InstrumentHeading("ACHIEVEMENTS",right+18,start+17,half-36);
            for(int p=1;p<=3;++p) {
                auto list=filter(p);
                int done=0,tried=0,correctForPath=0;
                for(int id:list) { done+=scores[id].correct>0; tried+=scores[id].attempts; correctForPath+=scores[id].correct; }
                float y=start+49+(p-1)*row;
                DrawRectangleRec({x+13,y+row-8,half-26,1},seam);
                Text(paths[p],x+26,y+11,24);
                Text(TextFormat("%d / %d mastered",done,static_cast<int>(list.size())),x+26,y+44,17,muted);
                Text(TextFormat("%d / %d correct",correctForPath,tried),x+half-123,y+44,16,muted);
                float track=(half-52)/list.size();
                for(size_t i=0;i<list.size();++i) {
                    int id=list[i];
                    Vector2 node{x+26+i*track+track/2,y+row-24-(i%2)*9};
                    bool hover=CheckCollisionPointCircle(GetMousePosition(),node,10);
                    if(i>0) DrawLineEx({node.x-track,y+row-24-((i-1)%2)*9},node,1,seam);
                    if(hover || scores[id].correct)
                        DrawCircleGradient(static_cast<int>(node.x),static_cast<int>(node.y),11,Fade(led,0.6f),Fade(led,0));
                    DrawCircleV(node,hover ? 5.0f : 4.0f,scores[id].correct ? led : muted);
                    DrawCircleLinesV(node,hover ? 8.0f : 6.0f,Fade(scores[id].correct ? led : muted,0.72f));
                    if(hover) Text(lessons[id].title,x+26,y+row-57,15,ink);
                    if(hover && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                        path=p; offset=static_cast<int>(i); pick(id); page=Learn;
                    }
                }
            }
            const char* badges[]={"First Mastery","Pointer Explorer","Curriculum Complete"};
            const char* goals[]={"Answer a lesson question correctly.","Master the Pointers quiz.","Master every lesson."};
            bool unlocked[]={mastered>0,scores[2].correct>0,mastered==static_cast<int>(lessons.size())};
            for(int i=0;i<3;++i) {
                float y=start+49+i*71;
                Card({right+13,y,half-26,62},unlocked[i] ? litSurface : rail);
                DrawCircleV({right+31,y+20},4,unlocked[i] ? led : muted);
                Text(badges[i],right+44,y+9,20,unlocked[i] ? led : ink);
                Text(goals[i],right+44,y+35,16,muted);
            }
            std::vector<int> weak;
            for(size_t i=0;i<scores.size();++i)
                if(scores[i].attempts>0 && scores[i].correct==0) weak.push_back(static_cast<int>(i));
            if(!weak.empty() && Button({right+13,h-78,190,43},"Practice weak topics")) startQuiz(std::move(weak));
        }
        if(reward>0) {
            float opacity=motion ? std::min(1.0f,reward*2.4f) : 1.0f;
            Card({w-203,31,179,38},Fade(panel,opacity));
            Text("+100 XP   /   MASTERY",w-190,42,15,Fade(led,opacity));
        }
        if(previousPage!=page) { previousPage=page; reveal=0; }
        reveal=motion ? std::min(1.0f,reveal+dt*5) : 1;
        if(reveal<1)
            DrawRectangleRec({x,100,w-x,h-100},Fade(bg,0.28f*(1-reveal)*(1-reveal)));
        EndMode2D();
        EndDrawing();
    }
    if(ui.texture.id!=GetFontDefault().texture.id) UnloadFont(ui);
    if(mono.texture.id!=GetFontDefault().texture.id) UnloadFont(mono);
    if(heading.texture.id!=GetFontDefault().texture.id) UnloadFont(heading);
    CloseWindow();
    return 0;
}
