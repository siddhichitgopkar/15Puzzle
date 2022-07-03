/**************************************************************************
 * This work originally copyright David J. Malan of Harvard University, or-
 * iginally released under:
 * CC BY-NC-SA 3.0  http://creativecommons.org/licenses/by-nc-sa/3.0/
 * licensing.
 *
 * It has been adapted for use in csci 1730.  Per the share-alike
 * clause of this license, this document is also released under the
 * same license.
 **************************************************************************/
/**
 * fifteen.c
 *
 * Implements Game of Fifteen (generalized to d x d).
 *
 * Usage: fifteen d
 *
 * whereby the board's dimensions are to be d x d,
 * where d must be in [DIM_MIN,DIM_MAX]
 *
 * Note that usleep is obsolete, but it offers more granularity than
 * sleep and is simpler to use than nanosleep; `man usleep` for more.
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// constants
#define DIM_MIN 3
#define DIM_MAX 9

// board
int board[DIM_MAX][DIM_MAX];

// dimensions
int d;

//other variables
int largest_num;

// prototypes
void clear(void);
void greet(void);
void init(void);
void draw(void);
int move(int tile);
int won(void);

int main(int argc, char* argv[])
{
    // ensure proper usage
    if (argc != 2)
    {
        printf("Usage: fifteen d\n");
        return 1;
    } //if

    // ensure valid dimensions
    d = atoi(argv[1]);
    if (d < DIM_MIN || d > DIM_MAX)
    {
        printf("Board must be between %i x %i and %i x %i, inclusive.\n",
            DIM_MIN, DIM_MIN, DIM_MAX, DIM_MAX);
        return 2;
    } //if

    // open log
    FILE* file = fopen("log.txt", "w");
    if (file == NULL)
    {
        return 3;
    } //if

    // greet user with instructions
    greet();

    // initialize the board
    init();

    // accept moves until game is won
    while (1)
    {
        // clear the screen
        clear();

        // draw the current state of the board
        draw();

        // log the current state of the board (for testing)
        for (int i = 0; i < d; i++)
        {
            for (int j = 0; j < d; j++)
            {
                fprintf(file, "%i", board[i][j]);
                if (j < d - 1)
                {
                    fprintf(file, "|");
                } //if
            } //for
            fprintf(file, "\n");
        } //for
        fflush(file);

        // check for win
        if (won())
        {
            printf("win!\n");
            break;
        } //if

        // prompt for move
        printf("Tile to move (0 to exit): ");
        int tile;
		scanf("%d", &tile);
		getchar();

        // quit if user inputs 0 (for testing)
        if (tile == 0)
        {
            break;
        } //if

        // log move (for testing)
        fprintf(file, "%i\n", tile);
        fflush(file);

        // move if possible, else report illegality
        if (!move(tile))
        {
            printf("\nIllegal move.\n");
            usleep(50000);
        } //if

        // sleep thread for animation's sake
        usleep(50000);
    } //while

    // close log
    fclose(file);

    // success
    return 0;
} //main

/**
 * Clears screen using ANSI escape sequences.
 */
void clear(void)
{
    printf("\033[2J");
    printf("\033[%d;%dH", 0, 0);
} //clear

/**
 * Greets player.
 */
void greet(void)
{
    clear();
    printf("WELCOME TO GAME OF FIFTEEN\n");
    usleep(200000);
} //greet

/**
 * Initializes the game's board with tiles numbered 1 through d*d - 1
 * (i.e., fills 2D array with values but does not actually print them).
 */
void init(void)
{
    //sets all the values in the board array
    largest_num = (d * d) - 1;
    int count = largest_num;
    for (int i = 0; i < d; i++) {
        for (int a = 0;  a < d; a++) {
                board[i][a] = count;
                count--;
        } //for
    } //for

    //setting the blank spot to a value of -1
    board[d-1][d-1] = -1;

    //if there is an odd number of tiles, swaps first and second tiles
    if (d%2 == 0) {
        board[d-1][d-3] = 1;
        board[d-1][d-2] = 2;
    } //if

} //init

/**
 * Prints the board in its current state.
 */
void draw(void)
{
    //prints out the board except for the blank space
    for (int i = 0; i < d; i++) {
        for (int a = 0; a < d; a++) {
            if (board[i][a] == -1) {
                printf(" _ ");
            } else {
                printf("%d ", board[i][a]);
            } //if
        } //for
        printf("\n");
    } //for
} //draw

/**
 * If tile borders empty space, moves tile and returns true, else
 * returns false.
 */
int move(int tile)
{
    int x = 0; //x coordinate of tile in array
    int y = 0; //y coordinate of tile in array

    //finds where in the array the tile is
    for (int i = 0; i < d; i++) {
        for (int a = 0; a < d; a++) {
            if (board[i][a] == tile) {
                y = a;
                x = i;
            } //if
        } //for
    } //for

    //checking to see if an adjacent tile is blank and if it is, moving the tile to the empty spot
    if (board[x+1][y] == -1) {
        board[x+1][y] = tile;
        board[x][y] = -1;

        return 1;
    } else if (board[x-1][y] == -1) {
        board[x-1][y] = tile;
        board[x][y] = -1;

        return 1;
    } else if (board[x][y+1] == -1) {
        board[x][y+1] = tile;
        board[x][y] = -1;

        return 1;
    } else if (board[x][y-1] == -1) {
        board[x][y-1] = tile;
        board[x][y] = -1;

        return 1;
    } else {
        return 0;
    } //if
} //move

/**
 * Returns true if game is won (i.e., board is in winning configuration),
 * else false.
 */
int won(void)
{
    int count = 1;
    for (int i = 0; i < d; i++) {
        for (int a = 0; a < d; a++) {
            if (!(board[i][a] == count)) {
                if (i == d-1 && a == d - 1 && board[i][a] == 0) {
                    return 1;
                } else {
                    return 0;
                } //if
            } //if
            count++;
        } //for
    } //for
    printf("WONNNNN");
    return 1;
} //won
