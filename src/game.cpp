#include "game.h"

// External interrupt variables (defined in main.cpp)
extern volatile bool buttonPressed;

// Game variables
enum { MATRIX_ROWS = 8, MATRIX_COLS = 32 };

int blockPos = 2;          // Row position (0-7, moving block position)
int blockWidth = 3;        // Current block width
int stackPos[MATRIX_COLS]; // Position of each stacked level
int stackWidth[MATRIX_COLS]; // Width of each stacked level
int stackHeight = 0;       // Current stack height (number of levels)
int direction = 1;         // 1 = down, -1 = up
int speed = 300;           // Movement speed (ms)
int level = 1;             // Current level
bool gameRunning = true;


void moveBlock() {
  blockPos += direction;
  // Reverse direction at edges
  if (blockPos <= 0 || blockPos >= MATRIX_ROWS - blockWidth) {
    direction *= -1;
  }
}

void handleButtonPress() {
  // Check if button was pressed via interrupt
  if (buttonPressed) {
    buttonPressed = false; // Reset the flag
    
    if (stackHeight == 0) {
      // First block - place it at the bottom
      stackPos[stackHeight] = blockPos;
      stackWidth[stackHeight] = blockWidth;
      stackHeight++;
      level++;
      increaseDifficulty();
    } else {
      // Check alignment with previous block
      int prevPos = stackPos[stackHeight - 1];
      int prevWidth = stackWidth[stackHeight - 1];
      
      // Calculate overlap
      int overlapStart = max(blockPos, prevPos);
      int overlapEnd = min(blockPos + blockWidth, prevPos + prevWidth);
      int overlapWidth = overlapEnd - overlapStart;
      
      if (overlapWidth > 0) {
        // Valid placement - store the overlapping part
        stackPos[stackHeight] = overlapStart;
        stackWidth[stackHeight] = overlapWidth;
        stackHeight++;
        
        // Update block width for next level
        blockWidth = overlapWidth;
        level++;
        increaseDifficulty();
        
        // Reset block position to center for next level
        blockPos = (MATRIX_ROWS - blockWidth) / 2;
        
        // Check win condition (reached top)
        if (stackHeight >= MATRIX_COLS) {
          // Win! Reset game
          resetGame();
        }
      } else {
        // No overlap - game over
        gameRunning = false;
      }
    }
  }
}

void increaseDifficulty() {
  // Increase speed as levels progress
  speed = max(50, speed - 5);
}

void drawGame() {
  mx.clear();
  
  // Draw moving block at current stack level (from bottom)
  for (int i = 0; i < blockWidth; i++) {
    mx.setPoint(blockPos + i, stackHeight, true);
  }

  // Draw all stacked blocks (from bottom to current height)
  for (int level = 0; level < stackHeight; level++) {
    for (int i = 0; i < stackWidth[level]; i++) {
      mx.setPoint(stackPos[level] + i, level, true);
    }
  }
}

void gameOver() {
  for (int i = 0; i < 3; i++) {
    mx.clear();
    delay(300);
    drawGame();
    delay(300);
  }
  resetGame();
}

void resetGame() {
  blockPos = 2;
  blockWidth = 3;
  stackHeight = 0;
  speed = 300;
  level = 1;
  direction = 1;
  gameRunning = true;
  
  // Clear stack arrays
  for (int i = 0; i < MATRIX_COLS; i++) {
    stackPos[i] = 0;
    stackWidth[i] = 0;
  }
}
