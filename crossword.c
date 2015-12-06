
// crossword.c
// Katie Schermerhorn and Courtney Kelly

#define _GNU_SOURCE
#include <stdio.h>
#include <string.h>
#define place_success 1
#define place_failure 0

typedef struct clues {
	int clueRow;
	int clueCol;
	char location[7];
	char hint[16];
} clues_t;

int userInput( char [][16] );
void initializeBoard( char [][15] );
void sortWords( char [][16] , int);
int compare(char * a, char * b);
void initialPlace(char [][16], char [][15]);
void placeWord(char [], char [][15], int, clues_t clues[]);
int can_place_word_horiz(char [], char [][15], int, int, int, int);
int can_place_word_vert(char [], char [][15], int, int, int, int);
void place_horiz(char[], char[][15], int, int, int, int);
void place_vert(char[], char[][15], int, int, int, int);
void jumbleWords( char * a, clues_t clues[], int);

int main() {

// Get User Input
	char words[21][16];
	int i;

	i = userInput(words);

// Initialize Board
	char board[15][15] = {0};
	initializeBoard(board);
	int clueCount=0;
	clues_t clues[20];
	
// Sort Words
	sortWords(words,i);

// Generate Puzzle
	initialPlace(words,board);
   	int n;

	for(n=1; n<(i-1); n++) {
		placeWord(words[n], board, clueCount, clues);
	}

// Print Solution
	int row,col;
	printf("\nSolution: \n\n");
	for (row=0; row<15; row++) {
        	for (col=0; col<15; col++) {
                	printf("%c", board[row][col]);
                }
                printf("\n");
        }

// Print Crossword Puzzle
	char key = '.';
	printf("\nCrossword Puzzle:\n\n");
	for (row=0; row<15; row++) {
        	for (col=0; col<15; col++) {
                	if (board[row][col]==key) {
				printf("#");
                	}
			else {
				printf(" ");
			}
		}
               	printf("\n");
        }

// Display Hints 
	for (n=0; n<i; n++) {
		printf("%s", clues[n].hint);
	}
}
int userInput( char words[21][16] ) {
	char key[2] = ".";
	int  i=0, loop=1;

        printf("Anagram Crossword Puzzle Generator\n----------------------------------\n");
        printf("Enter a list of words:\n");
	while (loop) {
		fscanf(stdin, "%s", words[i]);
		if (strcasestr(words[i], key) || i>19) {
                        loop=0;
                }
		i++;
        }

	return i;
}
void initializeBoard(char board[15][15]) {
	int row, col;
	char key = '.';
	for (row=0; row<15; row++) {
		for (col=0; col<15; col++) {
			board[row][col] = key;
		}
	}
}
void sortWords(char words[21][16],int i) {
	
	qsort(words[0], i, 16, compare);
}
int compare(char * a, char * b) {
	if (strlen(a) > strlen(b)) return -1;
	if (strlen(a) == strlen(b)) return 0;
  	if (strlen(a) < strlen(b)) return 1;
}
void initialPlace(char words[21][16], char board[15][15]) {

  	int length = strlen(words[0]);
  	int startPoint = (15 - length)/2;
  	int n=0, i;

  	for (i=startPoint;i<(startPoint+length);i++){
    		board[7][i] = words[0][n];
    		n++;
  	}
}
void  placeWord(char * word, char board[15][15], int clueCount, clues_t clues[]) {

   	int row,col,i;
   	int length= strlen(word);

   	for (i=0; i<length; i++) {
     		for(row=0; row<15; row++){
       			for(col=0; col<15; col++){
	 			if (board[row][col]==word[i]) {
	   				if (can_place_word_horiz(word, board, row, col, i, length)) {
	   					// Get info for Clues
							clues[clueCount].clueRow=row;
							clues[clueCount].clueCol=col;
							strcpy(clues[clueCount].location, "across");
							char temp[16]={0};
							strcpy(temp, word);
							jumbleWords( temp, clues, clueCount);
							clueCount++;
						// Place solution onto board
							place_horiz(word, board, row, col, i, length);
						return;
	   				}
					else if (can_place_word_vert(word, board, row, col, i, length)) {
	 					// Get Info for Clues
	 						clues[clueCount].clueRow=row;
                                                        clues[clueCount].clueCol=col;
                                                        strcpy(clues[clueCount].location, "down");
							char temp[16]={0};
                                                        strcpy(temp, word);
                                                        jumbleWords( temp, clues, clueCount);
							clueCount++;
                                                // Place solution onto board
							place_vert(word, board, row, col, i, length);
						return;
					}
				}
       			}
     		}
   	}
     	return;
 }
int can_place_word_horiz(char * word, char board[15][15], int row, int col, int i, int length) {

  	char key= '.';
	int n,k;

	// Checking against width of board
		if ((length-i) > (15-col) || ((col-i+1)< 0)) {
			return place_failure;
		}
	// Checking immedietly above through immedietly below the potential place for the word
		for (n=(row-1); n<=(row+1); n++) {
			for (k=(col-i+1); k<=(col+(length-i)); k++) {
				if (k==col) {
					// disregards column bc letters can be there
				}
				else if (board[n][k] != key) {
					return place_failure;
				}
			}
		}
	// Checking the endpoints of the word
		if ((board[row][col-i] != key) || (board[row][col+(length-i)+1] != key)) {
			return place_failure;
		}
	return place_success;
}
int can_place_word_vert(char * word, char board[15][15], int row, int col, int i, int length) {

	char key= '.';
        int n,k;
	
	// Checking against height of the board
		if ((length-i) > (15-row) || ((row-i+1)< 0)) {
                        return place_failure;
                }
	// Checking immediently to the left through immedietly to the right of potenital place for word
		for (n=(col-1); n<=(col+1); n++) {
                        for (k=(row-i+1); k<=(row+(length-i)); k++) {
                                if (k==row) {
                                        // disregards column bc letters can be there
                                }
                                else if (board[k][n] != key) {
					return place_failure;
				}
			}
		}
	// Checking the enpoints of the word
		if ((board[row-i][col] != key) || (board[row+(length-i+1)][col] != key)) {
                        return place_failure;
                }
        return place_success;
}
void place_horiz(char * word, char board[15][15], int row, int col, int i, int length) {
	int n;
	for (n=0; n<length; n++) {
		board[row][col-i+n]=word[n];
	}
}
void place_vert(char * word, char board[15][15], int row, int col, int i, int length) {
	int n;
	for (n=0; n<length; n++) {
		board[row-i+n][col]=word[n];
	}
}
void jumbleWords( char * a, clues_t clues[], int clueCount) {
	strcpy(clues[clueCount].hint, (char*) strfry(a));
}
