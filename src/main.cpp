#include "raylib.h"

// Everything is kept in this one C++ file to make the project easy to follow.

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

bool Clicked(Rectangle button) {
    return CheckCollisionPointRec(GetMousePosition(), button) &&
           IsMouseButtonPressed(MOUSE_BUTTON_LEFT);
}

void DrawButton(Rectangle button, const char* text, bool selected) {
    Color color = selected ? Color{80, 90, 230, 255} : Color{230, 232, 240, 255};
    Color textColor = selected ? WHITE : DARKGRAY;

    DrawRectangleRounded(button, 0.2f, 8, color);
    DrawText(text, (int)button.x + 14, (int)button.y + 12, 18, textColor);
}

void DrawPointerPicture() {
    DrawText("pointer", 330, 500, 18, DARKGRAY);
    DrawRectangleRounded({330, 530, 150, 65}, 0.15f, 8, Color{225, 226, 255, 255});
    DrawText("&score", 355, 550, 22, DARKBLUE);

    DrawText("score", 650, 500, 18, DARKGRAY);
    DrawRectangleRounded({650, 530, 150, 65}, 0.15f, 8, Color{220, 245, 235, 255});
    DrawText("20", 710, 550, 22, DARKGREEN);

    DrawLine(490, 562, 640, 562, Color{80, 90, 230, 255});
    DrawTriangle({640, 562}, {625, 554}, {625, 570}, Color{80, 90, 230, 255});
}

void DrawLinkedListPicture() {
    int x = 320;

    for (int i = 0; i < 3; i++) {
        DrawRectangleRounded({(float)x, 525, 120, 65}, 0.15f, 8,
                             Color{225, 226, 255, 255});
        DrawText(TextFormat("%d", (i + 1) * 10), x + 42, 547, 22, DARKBLUE);

        if (i < 2) {
            DrawLine(x + 120, 557, x + 165, 557, Color{80, 90, 230, 255});
            DrawTriangle({(float)x + 165, 557}, {(float)x + 153, 551},
                         {(float)x + 153, 563}, Color{80, 90, 230, 255});
        }

        x = x + 165;
    }

    DrawText("nullptr", x - 35, 548, 17, DARKGRAY);
}

int main() {
    InitWindow(1100, 700, "C and C++ Learning App");
    SetTargetFPS(60);

    int currentTopic = 0;
    int screen = 0;          // 0 = lesson, 1 = quiz
    int selectedAnswer = -1;
    bool showHint = false;

    while (!WindowShouldClose()) {
        if (Clicked({270, 25, 120, 45})) {
            screen = 0;
            selectedAnswer = -1;
        }

        if (Clicked({405, 25, 120, 45})) {
            screen = 1;
            selectedAnswer = -1;
        }

        for (int i = 0; i < topicCount; i++) {
            if (Clicked({20, (float)110 + i * 70, 210, 50})) {
                currentTopic = i;
                selectedAnswer = -1;
                showHint = false;
            }
        }

        Topic topic = topics[currentTopic];

        BeginDrawing();
        ClearBackground(Color{245, 246, 250, 255});

        DrawRectangle(0, 0, 250, 700, Color{32, 42, 68, 255});
        DrawText("CODE LAB", 28, 30, 28, WHITE);
        DrawText("C / C++", 30, 66, 17, LIGHTGRAY);

        for (int i = 0; i < topicCount; i++) {
            DrawButton({20, (float)110 + i * 70, 210, 50}, topics[i].title,
                       currentTopic == i);
        }

        DrawButton({270, 25, 120, 45}, "Lesson", screen == 0);
        DrawButton({405, 25, 120, 45}, "Quiz", screen == 1);

        DrawRectangleRounded({270, 90, 800, 580}, 0.03f, 8, WHITE);
        DrawText(topic.title, 305, 120, 32, Color{30, 35, 50, 255});

        if (screen == 0) {
            DrawText(topic.lesson, 305, 175, 21, DARKGRAY);

            DrawRectangleRounded({305, 255, 730, 210}, 0.05f, 8,
                                 Color{25, 30, 45, 255});
            DrawText("EXAMPLE", 330, 275, 15, GRAY);
            DrawText(topic.code, 330, 310, 20, RAYWHITE);

            if (currentTopic == 2) DrawPointerPicture();
            if (currentTopic == 3) DrawLinkedListPicture();
        } else {
            DrawText(topic.question, 305, 180, 22, DARKGRAY);

            for (int i = 0; i < 4; i++) {
                Rectangle answer{305, (float)235 + i * 68, 600, 50};
                DrawButton(answer, topic.answers[i], selectedAnswer == i);

                if (Clicked(answer) && selectedAnswer == -1) {
                    selectedAnswer = i;
                }
            }

            Rectangle hintButton{925, 235, 110, 45};
            DrawButton(hintButton, "Hint", showHint);
            if (Clicked(hintButton)) showHint = true;

            if (showHint && selectedAnswer == -1) {
                DrawText(topic.hint, 305, 535, 19, DARKBLUE);
            }

            if (selectedAnswer != -1) {
                if (selectedAnswer == topic.correctAnswer) {
                    DrawText("Correct!", 305, 535, 24, DARKGREEN);
                } else {
                    DrawText("Not quite. Try the lesson again.", 305, 535, 22, MAROON);
                }
            }
        }

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
