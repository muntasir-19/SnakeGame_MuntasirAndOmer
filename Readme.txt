### Snake Game in C++ - Readme

Authors: Muntasir Ahmed and Omer Seif
Group name: Muntasir and Omer

Overview  
This program implements a classic Snake game in C++ using Windows console functions. The player controls a snake that grows longer when it eats food, with collision detection, score tracking, and high score persistence.

Features  
- Snake movement  
- Random food generation  
- Score tracking
- high score saving  
- Pause/resume functionality  
- Wall collision and self-collision detection  
- snake speed increases with score  
- Game over screen with final score display  

Requirements  
- Windows OS  
- C++ compiler

Controls  
- W: Move Up  
- A: Move Left  
- S: Move Down  
- D: Move Right  
- P: Pause/Resume game  
- X: End game  

Game Rules  
1. Control the snake  to eat food 
2. Each food item increases:  
   - Score by 10 point  
   - Snake length  
   - Game speed by 5 milliseconds  
3. Avoid:  
   - Walls  
   - Snake's body   
4. Game ends on collision
  
High Scores 
- Current score shown during gameplay  
- All-time high score saved in `highscore.txt`  
- New high scores replace previous records  

Code Structure  
- `Snake` class: Handles movement, growth, and collision detection  
- `Food` class: Handles food position and spawning  
- `Game` class: Handles game loop, rendering, and input processing  