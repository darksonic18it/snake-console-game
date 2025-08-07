#include <iostream>
#include <conio.h>
#include <windows.h>
#include <vector>

using namespace std;

enum Direction { STOP = 0, LEFT, RIGHT, UP, DOWN };

// ✅ Function to clear screen without flickering
void clearScreen() {
    COORD cursorPos;
    cursorPos.X = 0;
    cursorPos.Y = 0;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), cursorPos);
}

class Snake {
public:
    int x, y;
    vector<pair<int, int>> tail;
    Direction dir;

    Snake(int startX, int startY) {
        x = startX;
        y = startY;
        dir = STOP;
    }

    void changeDirection(char key) {
        switch (key) {
            case 'a': dir = LEFT; break;
            case 'd': dir = RIGHT; break;
            case 'w': dir = UP; break;
            case 's': dir = DOWN; break;
        }
    }

    void move() {
        if (dir == STOP) return;
        tail.insert(tail.begin(), {x, y});
        switch (dir) {
            case LEFT:  x--; break;
            case RIGHT: x++; break;
            case UP:    y--; break;
            case DOWN:  y++; break;
        }
        tail.pop_back();
    }

    void grow() {
        tail.push_back({x, y});
    }
};

class Fruit {
public:
    int x, y;
    Fruit(int width, int height) {
        respawn(width, height);
    }

    void respawn(int width, int height) {
        x = rand() % width;
        y = rand() % height;
    }
};

class SnakeGame {
private:
    const int width = 20;
    const int height = 20;
    bool gameOver;
    Snake snake;
    Fruit fruit;
    int score;

public:
    SnakeGame() : snake(width / 2, height / 2), fruit(width, height) {
        gameOver = false;
        score = 0;
    }

    void draw() {
        clearScreen(); // ✅ Use flicker-free clear

        for (int i = 0; i < width + 2; i++) cout << "#";
        cout << endl;

        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                if (x == 0) cout << "#";

                if (x == snake.x && y == snake.y)
                    cout << "O"; // head
                else if (x == fruit.x && y == fruit.y)
                    cout << "F"; // fruit
                else {
                    bool isTail = false;
                    for (auto t : snake.tail) {
                        if (t.first == x && t.second == y) {
                            cout << "o"; // tail
                            isTail = true;
                            break;
                        }
                    }
                    if (!isTail) cout << " ";
                }

                if (x == width - 1) cout << "#";
            }
            cout << endl;
        }

        for (int i = 0; i < width + 2; i++) cout << "#";
        cout << endl;

        cout << "Score: " << score << endl;
    }

    void input() {
        if (_kbhit()) {
            char key = _getch();
            snake.changeDirection(key);
        }
    }

    void logic() {
        snake.move();

        if (snake.x < 0 || snake.x >= width || snake.y < 0 || snake.y >= height)
            gameOver = true;

        for (auto t : snake.tail) {
            if (t.first == snake.x && t.second == snake.y) {
                gameOver = true;
                break;
            }
        }

        if (snake.x == fruit.x && snake.y == fruit.y) {
            score += 10;
            fruit.respawn(width, height);
            snake.grow();
        }
    }

    void run() {
        while (!gameOver) {
            draw();
            input();
            logic();
            Sleep(100); // game speed
        }

        cout << "Game Over! Final Score: " << score << endl;
    }
};

int main() {
    SnakeGame game;
    game.run();
    return 0;
}
