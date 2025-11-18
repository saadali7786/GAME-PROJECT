#include "game.h"

// ----------------------------------------------------------------------
// GLOBAL VARIABLE DEFINITIONS (Initializes the extern variables from game.h)
// ----------------------------------------------------------------------
int playerRow = 1;      // Starting coordinates
int playerCol = 1;
int keysCollected = 0;
int gameOver = 0;
const int totalKeys = 6;
int exitSpawned = 0;
int playerHealth = 5;   // Player starts with 5 health
DWORD startTime;        // Timer starting point

// ----------------------------------------------------------------------
// FUNCTION IMPLEMENTATIONS
// ----------------------------------------------------------------------

/**
 * @brief Displays the game instructions, story, controls, and rules.
 */
void print_Instructions() {
    printf("-------------------------------------------------------------\n");
    printf("  ESCAPE THE CURSED TEMPLE: MYSTERY BOX CHALLENGE!   \n");
    printf("-------------------------------------------------------------\n\n");
    printf("  STORY:\n");
    printf("You are trapped in an ancient temple filled with mystery boxes.\n");
    printf("Each box could contain a KEY or be a TRAP that hurts you!\n");
    printf("Find all %d keys to escape, but be careful - you only have %d health!\n\n", totalKeys, playerHealth);

    printf("  CONTROLS:\n");
    printf("  W = Move Up\n");
    printf("  S = Move Down\n");
    printf("  A = Move Left\n");
    printf("  D = Move Right\n\n");

    printf("  SYMBOLS:\n");
    printf("  @ = You\n");
    printf("  ? = Mystery Box (50%% KEY or 50%% -1 Health)\n");
    printf("  O = Portal (teleports you randomly)\n");
    printf("  E = Exit (appears after collecting all keys)\n");
    printf("  . = Empty space\n");
    printf("  # = Wall\n\n");

    printf("  RULES:\n");
    printf(" - You have 5 HEALTH points.\n");
    printf(" - Mystery boxes have 50%% chance: KEY or lose 1 health.\n");
    printf(" - Some boxes are in secret rooms - use portals to reach them!\n");
    printf(" - The EXIT will ONLY appear in one of the 3 SECRET AREAS!\n");
    printf(" - If health reaches 0, game over!\n");
    printf(" - Collect all %d keys to make the exit appear.\n", totalKeys);
    printf("Good luck, adventurer! Choose your boxes wisely!\n");
}

/**
 * @brief Prints the current state of the temple map, stats, and handles exit spawning.
 * @param str The game map array.
 * @return The remaining time in seconds.
 */
int print_Temple(char str[Size][Size]){
    // Calculate remaining time
    DWORD currentTime = GetTickCount();
    DWORD elapsedTime = currentTime - startTime;
    int timeLimit = 90000; // 90 seconds
    int remainingTime = (timeLimit - elapsedTime) / 1000; // Convert to seconds
    
    if (remainingTime < 0) remainingTime = 0;
    
    int minutes = remainingTime / 60;
    int seconds = remainingTime % 60;
    
    printf("\n -------------------------------------------------------\n");
    printf(" | Health: %d/5 | Keys: %d/%d | Time: %02d:%02d |\n", 
           playerHealth, keysCollected, totalKeys, minutes, seconds);
    printf(" -------------------------------------------------------\n\n");
    
    if (exitSpawned == 0) 
    {
         if (keysCollected == totalKeys)
         {
            // Exit gate appears in one of the secret areas accessible by portal
            int Row_choice[] = {3, 5, 17}; 
            int Col_choice[] = {3, 3, 2}; 
            
            int index = rand() % 3;
            int exitRow = Row_choice[index];
            int exitCol = Col_choice[index];
            
            str[exitRow][exitCol] = 'E';
            exitSpawned = 1;
            printf("\n🚪 --------- The exit has appeared! Find it to escape! ---------🚪\n\n");
         }
    }
    
    // Print the map
    for (int i = 0; i < Size; i++)
    {
        for (int j = 0; j < Size; j++)
        {
            printf("%c  " , str[i][j]);
        }
        printf("\n");       
    }
    return remainingTime;
}

/**
 * @brief Takes movement input from the user and validates it.
 * @param str The game map array.
 */
void Input_User(char str[Size][Size]){
    char m;
    int valid = 0;
    while (!valid) {
        printf("Enter movement (W/A/S/D): ");
        scanf(" %c", &m);

        if (m == 'W' || m == 'w' || m == 'A' || m == 'a' ||
            m == 'S' || m == 's' || m == 'D' || m == 'd') 
        {
            valid = 1;
            movePlayer(str , m);
        } else {
            printf("Invalid input! Please use W, A, S, or D only.\n");
        }
    }
}

/**
 * @brief Handles player movement and interaction with map cells.
 * @param str The game map array.
 * @param m The movement character (W, A, S, D).
 */
void movePlayer(char str[Size][Size], char m){
    int newRow=playerRow, newCol=playerCol;

    // Calculate new position
    if (m == 'W' || m == 'w') newRow--;
    else if (m == 'S' || m == 's') newRow++;
    else if (m == 'A' || m == 'a') newCol--;
    else if (m == 'D' || m == 'd') newCol++;

    // Check for wall collision
    if (str[newRow][newCol]=='#')   
    {
        return; // Do nothing if it's a wall
    }
    
    // Clear old position and update player coordinates
    str[playerRow][playerCol]='.';
    playerRow=newRow; 
    playerCol=newCol; 
    
    char cell = str[playerRow][playerCol];

    // Handle cell interaction
    if (cell == '?') { 
        if (keysCollected == totalKeys) { // After all keys, all boxes are traps
             playerHealth--;
             printf("\n🎉 ========== UNLUCKY! Box was a TRAP! ==========\n");
             printf("   Keys: %d/%d | Health: %d/5\n\n", keysCollected, totalKeys, playerHealth);
        } 
        else {
            int random = rand() % 2; // 50% chance
            if (random == 0) { // Key
                keysCollected++;
                printf("\n🎉 ========== LUCKY! Box contained a KEY! ==========\n");
                printf("   Keys: %d/%d | Health: %d/5\n\n", keysCollected, totalKeys, playerHealth);
            } 
            else { // Trap
                playerHealth--;
                printf("\n💀 ========== UNLUCKY! Box was a TRAP! ==========\n");
                printf("   Lost 1 health! Health: %d/5\n\n", playerHealth);
            }
        }
        
        // Check game over
        if (playerHealth <= 0) {
            gameOver = 1;
            printf("\n💀 -------------- GAME OVER! You ran out of health --------------💀\n\n");
        }
    }
    
    else if (cell == 'E' && keysCollected == totalKeys) { // Successful Escape
        gameOver = 1;
        printf("\n🎊 ============== CONGRATULATIONS! YOU ESCAPED! ==============🎊\n");
        printf("   Final Stats - Keys: %d/%d | Health: %d/5\n\n", keysCollected, totalKeys, playerHealth);
    }
    
    else if (cell == 'O') { // Portal
        printf("\n🌀 --------------- Entered portal! Teleporting... ---------------🌀\n\n");
        Teleport(str, m);  
    }
    
    // Update player position on the map
    str[playerRow][playerCol] = '@';
}

/**
 * @brief Teleports the player to a new random location based on the portal type.
 * @param str The game map array.
 * @param m The movement character (unused, kept for signature consistency).
 */
void Teleport(char str[Size][Size] , char m){
    // Portals leading to secret box areas
    if((playerRow == 2 && playerCol == 5) || (playerRow == 7 && playerCol == 7) || (playerRow == 15 && playerCol == 8)){
        // Destination: Near secret boxes
        int Row_choices[] = {18, 1, 4}; 
        int Col_choices[] = {1, 16, 3}; 
        int index = rand() % 3;
        playerRow = Row_choices[index];
        playerCol = Col_choices[index];
    }
    
    // Portals leading back to main area
    else if((playerRow == 4 && playerCol == 4) || (playerRow == 2 && playerCol == 15) || (playerRow == 16 && playerCol == 3)){
        // Destination: Random empty spot in the main area
        int Row_choices[] = {5, 5, 6, 7, 8, 9, 10, 11, 12, 14};
        int Col_choices[] = {5,10, 7, 5, 8,12, 7, 6, 9, 5};
        int index = rand() % 10;

        playerRow = Row_choices[index];
        playerCol = Col_choices[index];
    } 
}