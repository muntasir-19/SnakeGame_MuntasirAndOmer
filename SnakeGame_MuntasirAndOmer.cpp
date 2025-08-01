#include <iostream>       // For input/output operations
#include <conio.h>        // For console input functions (_kbhit, _getch)
#include <windows.h>      // For Windows API functions (Sleep, console handling)
#include <vector>         // For vector container
#include <cstdlib>        // For standard library functions (rand, srand)
#include <ctime>          // For time functions (time)
#include <fstream>        // For file stream operations
#include <string>         // For string support
using namespace std;

// Game configuration constants
const int WIDTH = 20;      // Width of the game grid
const int HEIGHT = 10;     // Height of the game grid
const int INIT_DELAY = 200; // Initial delay between game updates (controls speed)
const int MIN_DELAY = 50;   // Minimum delay (maximum speed)

// Enumeration for  directions
enum Direction { STOP, UP, DOWN, LEFT, RIGHT };

// Handles movement, growth, and collision detection of the Snake
class Snake {
private:
    vector<pair<int, int> > body; // Stores snake segments as (x,y) pairs
    Direction dir;                // Current movement direction
    bool growNext;                // variable to grow snake on next move

public:
    // Constructor: initializes snake at position (x,y) facing right
    Snake(int x, int y) : dir(RIGHT), growNext(false) {
        // Create initial snake body (3 segments)
        body.push_back(pair<int, int>(x, y));      // Head
        body.push_back(pair<int, int>(x-1, y));    
        body.push_back(pair<int, int>(x-2, y));    
    }

    // Change direction with collision prevention
    void changeDir(Direction d) {
        // Prevent 180-degree turns
        if ((dir == LEFT && d != RIGHT) || (dir == RIGHT && d != LEFT) ||
            (dir == UP && d != DOWN) || (dir == DOWN && d != UP))
            dir = d;
    }

    // Move the snake by adding new head and removing tail
    void move() {
        pair<int, int> head = body.front(); // Get current head position
        
        // Calculate new head position based on direction
        switch(dir) {
            case UP:    head.second--; break; // decrease y
            case DOWN:  head.second++; break; // increase y
            case LEFT:  head.first--;  break; // decrease x
            case RIGHT: head.first++;  break; // increase x
        }
        
        body.insert(body.begin(), head); // Add new head position
        
        // Remove tail segment unless growing
        if (!growNext) body.pop_back();
        else growNext = false; // Reset growth var
    }

    // tell the snake to grow on next move
    void grow() { growNext = true; }
    
    // Check if snake occupies given position
    bool isCollide(int x, int y) {
        for (int i = 0; i < body.size(); i++)
            if (body[i].first == x && body[i].second == y)
                return true;
        return false;
    }
    
    // Get head position
    pair<int, int> getHead() { return body[0]; }
    
    // Get entire snake body
    vector<pair<int, int> > getBody() { return body; }
};
// Food class Handles food position and spawning 
class Food {
private:
    pair<int, int> pos; // Current position of food

public:
    // Initialize position
    Food() : pos(-1, -1) {}
    // Spawn food at random position not occupied by snake
    void spawn(Snake &s) {
        do {
            pos = pair<int, int>(rand() % WIDTH, rand() % HEIGHT);
        } while (s.isCollide(pos.first, pos.second));
    }
    // Get current food position
    pair<int, int> getPos() { return pos; }
};
// Handles game loop, rendering, and input processing 
class Game {
private:
    Snake snake;        // Player's snake
    Food food;          // Food object
    int score;          // Current score
    int highScore;      // Highest recorded score
    bool gameOver;      // Game state Var
    bool paused;        // Pause state Var
    int delay;          // Current delay between updates
    HANDLE console;     // Console handle for text attributes
    string HIGHSCORE_FILE; // Filename for highscore storage
    // Load high score from file
    void loadHighScore() {
        ifstream file(HIGHSCORE_FILE.c_str());
        if (file) file >> highScore;
        file.close();
    }
    // Save high score to file
    void saveHighScore() {
        ofstream file(HIGHSCORE_FILE.c_str());
        file << highScore;
        file.close();
    }
    // Draw the game state to console
    void draw() {
        COORD cursor = {0,0}; // Position cursor at top-left
        SetConsoleCursorPosition(console, cursor);
        // Draw top wall
        for (int i = 0; i < WIDTH+2; i++) cout << "*";
        cout << endl;
        // Draw game grid
        for (int y = 0; y < HEIGHT; y++) {
            cout << "*"; // Left wall
            
            for (int x = 0; x < WIDTH; x++) {
                // Check what to draw at position (x,y)
                if (snake.getHead().first == x && snake.getHead().second == y) {
                    SetConsoleTextAttribute(console, 9); // Blue head
                    cout << "O";
                }
                else if (snake.isCollide(x, y)) {
                    SetConsoleTextAttribute(console, 2); // Green body
                    cout << "o";
                }
                else if (food.getPos().first == x && food.getPos().second == y) {
                    SetConsoleTextAttribute(console, 12); // Red food
                    cout << "F";
                }
                else cout << " "; // Empty space
                
                SetConsoleTextAttribute(console, 7); // Reset to default
            }
            
            cout << "*" << endl; // Right wall
        }
        // Draw bottom wall
        for (int i = 0; i < WIDTH+2; i++) cout << "*";
        cout << endl;
        
        // Display game information
        cout << "Score: " << score << "  High Score: " << highScore;
        cout << "\n\nPress X to end the game";
        
        // Show pause status
        if (paused) cout << "\n\nPAUSED - Press P to resume";
    }
    // Handle keyboard input
    void input() {
        if (_kbhit()) {
            switch (_getch()) {
                case 'w': snake.changeDir(UP); break;
                case 's': snake.changeDir(DOWN); break;
                case 'a': snake.changeDir(LEFT); break;
                case 'd': snake.changeDir(RIGHT); break;
                case 'p': paused = !paused; break; //  pause game
                case 'x': gameOver = true;        // End game
            }
        }
    }
    // Update game state
    void update() {
        snake.move(); // Move snake
        pair<int, int> head = snake.getHead();
        vector<pair<int, int> > body = snake.getBody();
        // Wall collision detection
        if (head.first < 0 || head.first >= WIDTH || 
            head.second < 0 || head.second >= HEIGHT)
            gameOver = true;
        // Self-collision detection
        for (int i = 1; i < body.size(); i++)
            if (head.first == body[i].first && head.second == body[i].second)
                gameOver = true;
        // Food collection logic
        if (head.first == food.getPos().first && head.second == food.getPos().second) {
            snake.grow();        // Grow snake
            food.spawn(snake);   // Generate new food
            score += 10;         // Increase score
            // Increase speed 
            if (delay > MIN_DELAY) delay -= 5;
        }
    }
public:
    // Game constructor
    Game() : snake(0, 0), score(0), gameOver(false), 
             paused(false), delay(INIT_DELAY) {
        console = GetStdHandle(STD_OUTPUT_HANDLE); // Get console handle
        HIGHSCORE_FILE = "highscore.txt";          // Set highscore filename
        loadHighScore();   // Load saved highscore
        food.spawn(snake); // Generate initial food
    }
    // Main game loop
    void run() {
        while (!gameOver) {
            input();  // Process player input
            
            if (!paused) {
                update(); // Update game state
                draw();   // Render game
                Sleep(delay); // Control game speed
            }
            else {
                draw(); // Keep rendering while paused
                Sleep(100); // Shorter delay during pause
            }
        }
        // Game over handling
        if (score > highScore) {
            highScore = score; // Update high score
            saveHighScore();  // Save to file
        }
        // Clear screen and display final score
        system("cls");
        cout << "GAME OVER\nFinal Score: " << score 
             << "\nHigh Score: " << highScore;
    }
};
// Entry point of the program
int main() {
    srand(static_cast<unsigned>(time(0))); // Seeds the random number generator with current time to ensure different food spawn patterns each game run
    Game game;      // Create game instance
    game.run();     // Start game loop
    return 0;
}
