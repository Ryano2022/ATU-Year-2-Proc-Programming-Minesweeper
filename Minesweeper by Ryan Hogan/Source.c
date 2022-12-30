#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <stdbool.h>

// 8*8 Grid (64 Tiles).
// 10 Mines randomly located on the grid.
// You have to clear the grid without setting off any of the mines.

#define GRID 8
#define NUM_OF_TILES GRID * GRID
#define MINE -1
#define NUM_OF_MINES 10

FILE *savedGame;

int i, j, userX, userY, tilesRevealed = 0, playerOneTiles, playerTwoTiles, turnNumber = 0, versus = 0;
int game[GRID][GRID], visable[GRID][GRID];
bool mineExploded = false;


int generateRandomNumber() { // Generating a random number.
	int randomNum = (rand() % GRID) + 1;
	return randomNum;
}

void displayGrid(int version) {
	// Displaying the grid.
	for (i = 0; i < GRID; i++) {
		for (j = 0; j < GRID; j++) {
			if (version == 0) { // Game in progress grid.
				if (visable[i][j] == 0) {
					printf(" X ");
				}
				else if (visable[i][j] == 1) {
					if (game[i][j] == MINE) {
						printf(" M ");
					}
					else if (game[i][j] == 0) {
						printf(" . ");
					}
					else {
						printf(" %d ", game[i][j]);
					}
				}
			}
			else if (version == 1) { // Hidden grid.
				printf(" X ");
			}
			else if (version == 2) { // Fully revealed grid for debugging.
				if (game[i][j] == MINE) {
					printf(" M ");
				}
				else if (game[i][j] == 0) {
					printf(" . ");
				}
				else {
					printf(" %d ", game[i][j]);
				}
			}
		}
		printf("\n\n");
	}
}

void points() {
	if (versus == 1) {
		if (turnNumber % 2 == 0) {
			playerTwoTiles++;
		}
		else {
			playerOneTiles++;
		}
	}
}
void main() {
	srand(time(NULL)); // Making sure the random numbers are indeed random.

	// Variables
	int listOfMinesRow[NUM_OF_MINES], listOfMinesCol[NUM_OF_MINES];
	int randomNum1, randomNum2, drawnNumbers = 0, duplicateCount = 0, yesOrNo = 0;
	bool duplicate;

	// Ask to load a previously saved game or if there is none, just continue.
	savedGame = fopen("SavedGame.txt", "r");
	if (savedGame != NULL) {
		printf("Would you like to load your previously saved game? (Y = 1/N = 0) ");
		scanf("%d", &yesOrNo);
	}
	fclose(savedGame);

	if (yesOrNo == 1) {
		savedGame = fopen("SavedGame.txt", "r");
		for (i = 0; i < GRID; i++) {
			for (j = 0; j < GRID; j++) {
				fscanf(savedGame, "%d\n", &game[i][j]); // Loading game[GRID][GRID].
			}
		}

		for (i = 0; i < GRID; i++) {
			for (j = 0; j < GRID; j++) {
				fscanf(savedGame, "%d\n", &visable[i][j]); // Loading visibility[GRID][GRID].
			}
		}

		for (i = 0; i < NUM_OF_MINES; i++) {
			fscanf(savedGame, "%d\n", &listOfMinesRow[i]); // Loading listOfMinesRow[NUM_OF_MINES].
		}

		for (i = 0; i < NUM_OF_MINES; i++) {
			fscanf(savedGame, "%d\n", &listOfMinesCol[i]); // Loading listOfMinesCol[NUM_OF_MINES].
		}

		fscanf(savedGame, "%d\n", &tilesRevealed); // Loading tilesRevealed.
		fscanf(savedGame, "%d\n", &playerOneTiles); // Loading playerOneTiles.
		fscanf(savedGame, "%d\n", &playerTwoTiles); // Loading playerTwoTiles.
		fscanf(savedGame, "%d\n", &turnNumber); // Loading turnNumber.
		fscanf(savedGame, "%d\n", &versus); // Loading versus.

		fclose(savedGame);
		displayGrid(0);
	}
	else {
		// Generate 10 random locations for the mines.
		for (i = 0; i < NUM_OF_MINES; i++) {
			randomNum1 = generateRandomNumber();
			randomNum2 = generateRandomNumber();

			duplicate = false;

			for (j = 0; j < drawnNumbers; j++) { // Checking for duplicates.
				if (listOfMinesRow[j] == randomNum1 && listOfMinesCol[j] == randomNum2) {
					duplicate = true;

					/* Debug
					duplicateCount++;
					printf("Duplicates found: %d\n", duplicateCount);
					*/
				}
			}
			
			if (duplicate == false) { // If not, lay the mine.
				game[randomNum1 - 1][randomNum2 - 1] = MINE;
				drawnNumbers++;
				
				/* Debug
				listOfMinesRow[i] = randomNum1;
				listOfMinesCol[i] = randomNum2;
				*/
			}
			else if (duplicate == true) { // If it is, go back and try again.
				i -= 1;
			}
		}

		// Set the number of mines number on all the tiles.
		for (i = 0; i < GRID; i++) {
			for (j = 0; j < GRID; j++) {
				// Checking for mines. Kind of long but I couldn't think of any other way sorry.
				int mineCount = 0;

				// -1 = Out of bounds (left).
				// GRID = Out of bounds (right).
				if (i - 1 != -1 && j - 1 != -1) {
					if (game[i - 1][j - 1] == MINE)
						mineCount++; // Top Left
				}
				if (i - 1 != -1) {
					if (game[i - 1][j] == MINE)
						mineCount++; // Top Middle
				}
				if (i - 1 != -1 && j + 1 != GRID) {
					if (game[i - 1][j + 1] == MINE)
						mineCount++; // Top Right
				}
				if (j - 1 != -1) {
					if (game[i][j - 1] == MINE)
						mineCount++; // Middle Left
				}
				if (j + 1 != GRID) {
					if (game[i][j + 1] == MINE)
						mineCount++; // Middle Right
				}
				if (i + 1 != GRID && j - 1 != -1) {
					if (game[i + 1][j - 1] == MINE)
						mineCount++; // Bottom Left
				}
				if (i + 1 != GRID) {
					if (game[i + 1][j] == MINE)
						mineCount++; // Bottom Middle
				}
				if (i + 1 != GRID && j + 1 != GRID) {
					if (game[i + 1][j + 1] == MINE)
						mineCount++; // Bottom Right
				}

				if (game[i][j] != MINE) {
					game[i][j] = mineCount;

				}
			}
		}

		/* Debug
		for (i = 0; i < NUM_OF_MINES; i++) { // Displaying all the mines at the beginning just for testing.
			printf("Mine #%d is on tile %d, %d\n", i + 1, listOfMinesRow[i], listOfMinesCol[i]);
		}
		printf("\n");
		*/

		printf("Would you like to play Versus mode? (2 Players) (Y = 1/N = 0) ");
		versus = 0;
		scanf("%d", &versus);

		if (versus == 1) {
			playerOneTiles = 0;
			playerTwoTiles = 0;
			turnNumber = 1;
		}
		tilesRevealed = 0;

		printf("\n\n");
		displayGrid(1); // Display a hidden grid at the beginning of the game.
		printf("\n");
	}

	/* Debug
	printf("\n\n");
	displayGrid(2); 
	printf("\n");
	*/
	
	while (mineExploded == false || tilesRevealed != (NUM_OF_TILES - NUM_OF_MINES)) { // Repeat until the mine explodes or all tiles have been revealed.
		if (versus == 1) {
			if (turnNumber % 2 == 0) {
				printf("P2: "); // Player two.
			}
			else {
				printf("P1: "); // Player one.
			}
		}

		printf("Which row and column would you like to select? "); // Take in user input for which tile to reveal.
		scanf("%d %d", &userX, &userY);

		while (userX > GRID || userX < 1 || userY > GRID || userY < 1) { // Keep asking until it's within bounds.
			printf("That row and column is outside the grid. Please select another row and column: ");
			scanf("%d %d", &userX, &userY);
		}
		
		// Decrementing so it's accurately displayed on the game grid.
		userX -= 1;
		userY -= 1;
		
		for (i = 0; i < GRID; i++) { // Setting up the visability of the tiles.
			for (j = 0; j < GRID; j++) {
				if (i == userX && j == userY) {
					visable[userX][userY] = 1; // 0 for hidden, 1 for revealed.
					
					if (game[userX][userY] == 0) {
						int num = 1; // I've spent 4 nearly 5 hours trying to figure out how to reveal all the 0s around a selected 0.
						if (game[userX + num][userY] == 0) { // I've tried all sorts of different ideas but I just cannot think of how
							visable[userX + num][userY] = 1; // To make it work so I'm going to have to skip this part.
							//points();    // Points not working as intended so commented out.
						}
						if (game[userX - num][userY] == 0) {
							visable[userX - num][userY] = 1;
							//points();
						}
						if (game[userX][userY + num] == 0) {
							visable[userX][userY + num] = 1;
							//points();
						}
						if (game[userX][userY - num] == 0) {
							visable[userX][userY - num] == 0;
							//points();
						}
					}

					if (versus == 1) {
						turnNumber++;
						if (turnNumber % 2 == 0) {
							playerOneTiles++;
						}
						else {
							playerTwoTiles++;
						}
					}

					tilesRevealed++;
				}
			}
		}

		printf("\n");
		displayGrid(0);
		printf("\n");
		if (versus == 1) {
			printf("P1 Points: %d\nP2 Points: %d\n", playerOneTiles, playerTwoTiles);
		}

		if (game[userX][userY] == MINE) // Triggering the loss.
			mineExploded = true;

		if (mineExploded == true) { // Loss
			if (versus == 1) {
				if (turnNumber % 2 == 0) {
					printf("P1: "); // Player one
				}
				else {
					printf("P2: "); // Player two.
				}
			}
			printf("You selected a mine! You lose. Better luck next time!\n\n");
			exit(0);
		}

		if (tilesRevealed == (NUM_OF_TILES - NUM_OF_MINES)) { // Victory 
			if (versus == 1) {
				if (playerOneTiles > playerTwoTiles) { // Backwards but it works.
					printf("P1: You had the most tiles revealed without setting off a mine! You win! Good job!");
				}
				else {
					printf("P2: You had the most tiles revealed without setting off a mine! You win! Good job!");
				}
			}
			else {
				printf("You revealed all the tiles without setting off a mine! You win! Good job!\n\n");
			}
			exit(0);
		}

		printf("Would you like to save? (Y = 1/N = 0) "); // Saving the game to SavedGame.txt
		yesOrNo = 0;
		scanf("%d", &yesOrNo);

		if (yesOrNo == 1) {
			savedGame = fopen("SavedGame.txt", "w");
			for (i = 0; i < GRID; i++) {
				for (j = 0; j < GRID; j++) {
					fprintf(savedGame, "%d\n", game[i][j]); // Saving game[GRID][GRID].
				}
			}

			for (i = 0; i < GRID; i++) {
				for (j = 0; j < GRID; j++) {
					fprintf(savedGame, "%d\n", visable[i][j]); // Saving visibility[GRID][GRID].
				}
			}

			for (i = 0; i < NUM_OF_MINES; i++) {
				fprintf(savedGame, "%d\n", listOfMinesRow[i]); // Saving listOfMinesRow[NUM_OF_MINES].
			}

			for (i = 0; i < NUM_OF_MINES; i++) {
				fprintf(savedGame, "%d\n", listOfMinesCol[i]); // Saving listOfMinesCol[NUM_OF_MINES].
			}

			fprintf(savedGame, "%d\n", tilesRevealed); // Saving tilesRevealed.
			fprintf(savedGame, "%d\n", playerOneTiles); // Saving playerOneTiles.
			fprintf(savedGame, "%d\n", playerTwoTiles); // Saving playerTwoTiles.
			fprintf(savedGame, "%d\n", turnNumber); // Saving turnNumber.
			fprintf(savedGame, "%d\n", versus); // Saving versus.

			fclose(savedGame);
		}
	}
}