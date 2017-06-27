/**
 * fifteen.c
 *
 * Computer Science 50
 * Problem Set 3
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
 
#define _XOPEN_SOURCE 500

#include <cs50.h>
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

// prototypes
void clear(void);
void greet(void);
void init(void);
void draw(void);
bool move(int tile, int arr[9][9], int dim);
bool won(void);

int main(int argc, string argv[])
{
    // ensure proper usage
    if (argc != 2)
    {
        printf("Usage: fifteen d\n");
        return 1;
    }

    // ensure valid dimensions
    d = atoi(argv[1]);
    if (d < DIM_MIN || d > DIM_MAX)
    {
        printf("Board must be between %i x %i and %i x %i, inclusive.\n",
            DIM_MIN, DIM_MIN, DIM_MAX, DIM_MAX);
        return 2;
    }

    // open log
    FILE* file = fopen("log.txt", "w");
    if (file == NULL)
    {
        return 3;
    }

    // greet user with instructions
    greet();

    // initialize the board
    init();

    // accept moves until game is won
    while (true)
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
                }
            }
            fprintf(file, "\n");
        }
        fflush(file);

        // check for win
        if (won())
        {
            printf("ftw!\n");
            break;
        }

        // prompt for move
        printf("Tile to move: ");
        int tile = GetInt();
        
        // quit if user inputs 0 (for testing)
        if (tile == 0)
        {
            break;
        }

        // log move (for testing)
        fprintf(file, "%i\n", tile);
        fflush(file);

        // move if possible, else report illegality
        if (!move(tile, board, d))
        {
            printf("\nIllegal move.\n");
            usleep(500000);
        }

        // sleep thread for animation's sake
        usleep(500000);
    }
    
    // close log
    fclose(file);

    // success
    return 0;
}

/**
 * Clears screen using ANSI escape sequences.
 */
void clear(void)
{
    printf("\033[2J");
    printf("\033[%d;%dH", 0, 0);
}

/**
 * Greets player.
 */
void greet(void)
{
    clear();
    printf("WELCOME TO GAME OF FIFTEEN\n");
    usleep(2000000);
}

/**
 * Initializes the game's board with tiles numbered 1 through d*d - 1
 * (i.e., fills 2D array with values but does not actually print them).  
 */
void init(void)
{
    int tile_number = (d*d)-1;
    
    for (int i = 0; i < d; i++)
        {
            for (int j = 0; j < d; j++)
            {
                board[i][j] = tile_number;
                tile_number--;
            }   
        }
    
    // if modulus d is 0 -> swap the 1 and 2 tile (2nd and 3rd last tile)
    if( d % 2 == 0)
    {
        board[d-1][d-3] = 1;
        board[d-1][d-2] = 2;
    }

    
}

/**
 * Prints the board in its current state.
 */
void draw(void)
{
    printf("\n\n\n");
    for (int i = 0; i < d; i++)
        {
            for (int k = 0; k < d; k++)
            {
                printf("+----");
            }
            printf("+\n");
            for (int j = 0; j < d; j++)
            {
                if(board[i][j] == 0) // empty zero tile
                {
                    printf("|    ");
                }
                else
                printf("| %2d ", board[i][j]);
            }
            printf("|\n");
        }
    for (int k = 0; k < d; k++)
    {
        printf("+----");
    }
    printf("+\n\n\n");
 
}

/**
 * If tile borders empty space, moves tile and returns true, else
 * returns false. 
 */
bool move(int tile, int arr[9][9], int dim)
{
    //check location of tile and zero tile
    int i, j, k, l, o, p;
    for(i = 0; i<dim; i++) {
        for(j = 0; j<dim; j++) {
            if (arr[i][j] == tile) // location of tile
            {
                k = i;
                l = j;
            }
            if (arr[i][j] == 0) //location of zero tile
            {
                o = i;
                p = j;
            }
        }
        printf("\n");
    }
    int diff_row = abs(k-o);
    int diff_col = abs(l-p);
    
    // check if tile is in a moveable place and move if so
    if((diff_row == 1 && diff_col == 0) || (diff_row == 0 && diff_col == 1))
    {
        arr[k][l] = 0;
        arr[o][p] = tile;
        return true;
    }
    
    return false;
}

/**
 * Returns true if game is won (i.e., board is in winning configuration), 
 * else false.
 */
bool won(void)
{
    int tile_number = 1;
    int won = 0;
    
    // checking configuration of board
    for (int i = 0; i < d; i++)
        {
            for (int j = 0; j < d; j++)
            {
                if(board[i][j] == tile_number)
                {
                    won++;
                    tile_number++;
                }
                else if(i == d-1 && j == d-1 && board[i][j] == 0)
                {
                    won++;
                    tile_number++;   
                }
                else
                {
                    return false;
                }
            }   
        }
    
    if (won++ == d*d)
    {
        return true;
    }
    return false;
}