//  Meagan Choo-Kang

#include <stdio.h>
#include <stdbool.h>

const int MAX =26; // Max size of board


char findOpponent(char colour);
void printBoard(char board[][MAX] ,int n);
bool positionInBounds(int n, int row, int col);
bool legalDirection(char board[][MAX], int n, int row, int col, char colour, int deltaRow, int deltaCol);
int playMove(char board[][MAX], int n, int playedRow, int playedCol, char colour);
int calcScore(char board[][MAX], int n, int playedRow, int playedCol, char colour);
bool userTurn (char board[][MAX], int n, char userColour, int userRow, int userCol);
int checkBoard(char board[][MAX], int n, char colour);
bool colourGoneOrBoardFull(char board[][MAX] , int n);
char calcWinner(char board[][MAX], int n);
int predictOpponent(char board[][MAX], int n, char colour, int possRow, int possCol);
bool opponentNearby(char board[][MAX], int n, char compColour, int possRow, int possCol);
bool compTurn(char board[][MAX], int n, char colour, int *playRow, int *playCol, int numTurns);

int main(int argc, char **argv)
{
    const int ROW = 0;
    const int COL = 1;
    
    int n, userRow, userCol, highRow, highCol;
    char outputRow, outputCol, compColour, userColour, winner;
    char userMove[2];
    char currentColour = 'W';
    bool playing = true;
    bool winnerFound = false;
    bool userPlay = true;
    bool compPlay = true;
    int numTurns = 0;
   
    // prompts for board dimensions and computer's colour
	printf("Enter the board dimension: ");
    scanf("%d", &n);
    
    printf("Computer plays (B/W): ");
    scanf(" %c", &compColour);
    
    userColour = findOpponent(compColour); // finds colour of the user
    char board[n+ 2][MAX] ;
    
    // intialize board and searches for centre positons
    for (int row = 0; row <= n ; row++){
        for (int col = 0; col <= n ; col++){
        // if sides are 0
        if (row == 0 || col == 0 ){
            board[row][col] = 'X'; // random variable
        }
        // postiton for White
        if  ((row == n/2 && col == n/2) || (row== n/2 +1 && col == n/2 + 1)){
            board[row][col] = 'W';
        // position for Black
        } else if ((row == n/2 && col == n/2 +1) || (row == n/2 + 1 && col == n/2)){
             board[row][col] = 'B';
        // default is unoccupied 
        } else {
            board[row][col] = 'U';
        }
      }
    }
    
    printBoard(board, n); 
    
    // plays game
    while (playing){
        
        numTurns++;
        
        // CHANGE IN PART 1!!!!!!!!!!
        // checks if a colour is off the board or if its full
        if (colourGoneOrBoardFull(board, n) || ((!userPlay) && (!compPlay))){
            playing = false;
            continue;
        } 
        
        // determines whose turn it is
        if (currentColour == 'W'){
            currentColour = 'B';
        }else{
            currentColour = 'W';
        }
        
        // computer's turn (starts first if black)
        if (compColour == currentColour){
                compPlay = compTurn(board, n, compColour, &highRow, &highCol, numTurns);
                if (compPlay){
                    outputRow= highRow +  'a' - 1;
                    outputCol = highCol  + 'a' - 1;
                    printf("Computer places %c at %c%c.\n", compColour, outputRow, outputCol);
                    playMove(board, n, highRow, highCol, compColour);
                    printBoard(board, n);
                } else{
                    if (userPlay){
                        printf("%c player has no valid move.\n", compColour);
                    }
                }
                
        // user's turn (starts first if black)
        }else{
            // check if the user could play 
            if (checkBoard(board, n, userColour) == 0){
                userPlay = false;
                if (compPlay){
                    printf("%c player has no valid move.\n", userColour);
                }
                //printf("%c player has no valid move.\n", userColour);
            // if user can play  after not being able play, resets
            } else{
                userPlay = true;
                printf("Enter move for colour %c (RowCol): ", userColour);
                scanf(" %c %c", &userMove[ROW], &userMove[COL]);
                // converts to int
                userRow = userMove[ROW] - 'a' + 1;
                userCol= userMove[COL] - 'a' + 1;
        
                // valid move
                if (userTurn(board, n, userColour, userRow, userCol)){
                    playMove(board, n, userRow, userCol, userColour); // plays move
                // user entered an invalid move, therefore computer wins
                }else{
                    printf("Invalid move.\n");
                    playing = false;
                    winnerFound = true;
                    winner = compColour;
                    continue; 
                }
                printBoard(board, n);
            }
        }
    } // end of game
    
    // if user didn't make error, finds which colour has the most tiles
    if (!winnerFound){
        winner = calcWinner(board, n);
    }
    // Tie
    if (winner == 'T'){
        printf("Draw!\n");
    // White or black wins
    } else{
        printf("%c player wins.\n", winner); 
    }
	return 0;
}


void printBoard(char board[][MAX], int n){
    // Outputs the current board
    
    const char letters [ ]= {'*','a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z'};
    printf("  "); 
    
    // prints row of letters for n board
    for (int i = 1; i <= n; i++){
        printf("%c", letters[i]);
    }
    printf("\n");
    
    // outputs remaning board
    for (int row = 1; row <= n; row++){
        printf("%c ", letters[row]); // prints letter at the start of each row
        for (int col =1; col <= n; col++){
            printf("%c", board[row][col]);  // prints W, B, U according to board
        }
        printf("\n");
    }
}


char findOpponent(char colour){
    // returns oppoenent of player    
    if (colour =='W'){
       return 'B';
    }else{
        return 'W';
    }
}


bool positionInBounds (int n, int row, int col){
    // lies on the board returns true, if outside of board return false
    if (((1 <= row )&& (row <= n)) && ((1 <= col )&& (col <=n))){
        return true;
    } else{
        return false;
    }
}


bool legalDirection(char board[][MAX], int n, int row, int col, char colour, int deltaRow, int deltaCol){
    // if there's a legal move in given direction, returns true, else returns false
     
     // checks if deltaRow == deltaCol == 0, which is an invalid direction
     if ((deltaRow == 0) && (deltaCol == 0)){
         return false;
     }
     
    char opponent;
    int currentRow = row + deltaRow;
    int currentCol = col + deltaCol; 
    opponent = findOpponent(colour);
    
    // move must have: valid postion on board and opponent is adjacent
    if (!positionInBounds(n, currentRow, currentCol) || board[currentRow][currentCol] != opponent){
        return false;
        
    // if it passes, then checks the next position
    } else{
        currentRow +=deltaRow;
        currentCol += deltaCol;
    }
    
    char tile;
    
    // searches through a direction, while making sure it is still in the board
    while(positionInBounds(n,currentRow, currentCol)){
        tile = board[currentRow][currentCol];
        
        // if hits own colour then it found a valid move
        if (tile == colour){
            return true;
        // no move if tile is its own colour
        }else if (tile == 'U'){
            return false; 
        // keeps on checking if the oppoent is on the tile
        }else{
            currentRow += deltaRow;
            currentCol +=deltaCol;
        }
    }
    return false; // no move
}

// combine score add parameter
int playMove(char board[][MAX], int n, int playedRow, int playedCol, char colour){
    // plays move by fliiping tiles
    
    bool flipping, checking;
    char opponent, tile;
    int row = playedRow;
    int col = playedCol;
    int numFlips = 0;
    
    opponent = findOpponent(colour); // finds colour of opponent
    board[row][col] = colour; // sets move positon to be own colour

    for (int deltaRow = -1; deltaRow <=1; deltaRow++){
        for (int deltaCol = -1; deltaCol <=1; deltaCol++){
            // row and col increase in given direction
            row = playedRow + deltaRow;
            col = playedCol + deltaCol;
            checking = true;
            
            while(checking){
                tile = board[row][col];
                // if its the oppoent colour, then keeps on checking
                if (tile == opponent){
                    row += deltaRow;
                    col += deltaCol;
                // ifit finds its own colour, then it can flip in that direction
                }else if(tile == colour){
                    flipping = true;
                    checking = false;
                // if it finds its an unoccupied spot, then it cannot flip in that direction
                }else{
                    flipping = false;
                    checking = false;
                    continue;
                }
            }
            
            // resets position
            row = playedRow + deltaRow;
            col = playedCol + deltaCol;
            
            while(flipping){
                tile = board[row][col];
                
                // if tile is opponent, then flips their colour
                if (tile == opponent){
                    board[row][col] = colour;
                    row += deltaRow;
                    col += deltaCol;   
                    numFlips++;
                 
                // if it reaches its own colour, it stops flipping
                }else{
                    flipping = false;
                }
            }
        }
    }
    return numFlips;
}


int calcScore(char board[][MAX], int n, int playedRow, int playedCol, char colour){
    // calculates the score of the tiles that it could flip in all directions if there is a valid move
    
    bool flipping, checking;
    char opponent, tile;
    int score = 0;
    int total = 0;
    int row, col;
    opponent = findOpponent(colour);

    for (int deltaRow = -1; deltaRow <=1; deltaRow++){
        for (int deltaCol = -1; deltaCol <=1; deltaCol++){
            row = playedRow + deltaRow;
            col = playedCol + deltaCol;
            score = 0; // resets score
            checking = true;
            
            while(checking){
                tile = board[row][col];
                
                // if own tile
                if (tile == opponent){
                    row += deltaRow;
                    col += deltaCol;
                    score++;
                // if its its own colour, then it can flip
                }else if(tile == colour){
                    flipping = true;
                    checking = false;
                // if its an unocuppied spot, then it cannot flip
                }else{
                    flipping = false;
                    checking = false;
                    continue;
                }
            }
            row = playedRow + deltaRow;
            col = playedCol + deltaCol;
            
            // if it could flip, then adds score to total
            if (flipping){
                total += score;
            }
        }
    }
    return total;
}


bool userTurn (char board[][MAX], int n, char userColour, int userRow, int userCol){
    // checks if the user gave a valid move and returns true or false, accordingly.
    
    bool userValid = false;
   
     for (int deltaRow = -1; (deltaRow <=1) && (!userValid); deltaRow++){
         for (int deltaCol = -1; (deltaCol <=1) && (!userValid); deltaCol++){
             if (legalDirection(board, n, userRow, userCol, userColour, deltaRow, deltaCol)){
                 userValid = true;
                 // add a break or something? ****************************************************************************************
             }     
         }
     }
      // user has valid move
     if (userValid){
         return true;
    // user has invalid move
     }else{
         return false;
     }
}


int checkBoard(char board[][MAX], int n, char colour){
    // checks for legal moved by calling legalDirection by checking empty postions , outputs legal move
    
    char piece;
    
    for (int row = 1; row <=n; row ++){
        for (int col = 1; col <= n; col++){
            piece = board[row][col];
            
            // checks for empty space
            if(piece == 'U'){
                for (int deltaRow = -1; deltaRow <=1; deltaRow++){
                    for (int deltaCol = -1; deltaCol <=1; deltaCol++){
                        
                        // if that is a legal move, calculates the score
                        if(legalDirection(board, n, row, col, colour, deltaRow, deltaCol)){
                            return 1;
                        }
                    }       
                }
            }   
        }
    }
    return 0; // no move
}


bool colourGoneOrBoardFull(char board[][MAX] , int n){
    // checks a colour is off the board and if the board is full
    
    bool  whiteOnBoard = false;
    bool blackOnBoard = false;
    bool boardFull = true;
    
    for(int row = 1; row <= n; row++){
        for(int col = 1; col <= n; col++){
            // if there is a white, then white is still on board
            if (board[row][col] == 'W'){
                whiteOnBoard = true;
            // if there is a black, then black is still on board
            } else if (board[row][col] == 'B'){
                blackOnBoard = true;
            // if there is an unoccupied space (U), then board is not full
            } else if (board[row][col] == 'U'){
                boardFull = false;
            }
        }
    }
    
    // if one of the colours are off the board, or if the board is full, returns true
    if ((!blackOnBoard) || (!whiteOnBoard) || (boardFull)){
        return true;
    }else{
        return false;
    }    
}


char calcWinner(char board[][MAX], int n){
    // calculates the winner by counting how much tiles each colour has
    
    int numWhite = 0;
    int numBlack = 0;
    
    for(int row = 1; row <= n; row++){
        for(int col = 1; col <= n; col++){            
            // increments score of white
            if (board[row][col] == 'W'){
                numWhite++;
            // increments score of black
            } else if (board[row][col] == 'B'){
                numBlack++;
            }
        }
    }
    
    // white wins
    if (numWhite > numBlack){
        return 'W';
    // black wins
    } else if (numWhite < numBlack){
        return 'B';
    // tie
    } else{
        return 'T';
    }
}

int predictOpponent(char board[][MAX], int n, char compColour, int possRow, int possCol){
    // calculates how many times my score is bigger 
    
    char copyBoard[n+1][MAX];
    char piece, tile;
    char opponent = findOpponent(compColour);
    int myScore, opponentScore;
//    int myScoreBigger = 0;
    bool flipping;
    
    // makes a copy of the current board, so I can  temporary modify the board
    for (int row = 0; row <= n; row++){
        for (int col = 0; col <= n; col++){
            copyBoard[row][col] = board[row][col];
        }
    }
    
    myScore = playMove(copyBoard, n, possRow, possCol, compColour); // how much i would flip if I played my move, also changes the board as a copy
    
    for (int row = 1; row <= n;  row++){
        for (int col = 1; col <= n; col++){
            piece = copyBoard[row][col]; // checks current position
            opponentScore = 0; // resets
            
            // if there is any empty space, 
            if (piece == 'U'){
                for (int deltaRow = -1; deltaRow <= 1; deltaRow++){
                    for (int deltaCol = -1; deltaCol <= 1; deltaCol++){
                        
                        // checks possible moves for opppoenent
                        if (legalDirection(board, n, row, col, opponent, deltaRow, deltaCol)){
                            flipping = true;
                             
                            while(flipping){
                                tile = copyBoard[row][col];
                                
                                // "pretends" the opponent flipped my tiles, checks how many times they do that
                                if (tile == compColour){
                                    row += deltaRow;
                                    col += deltaCol;
                                    opponentScore++;
                                // cannot flip anymore (reaches its own colour -opponent's colour)
                                }else{
                                    flipping = false;
                                }
                            }
                        }                  
                    }
                }  // after it checked each direction for one piece
                // checks if my score is greater than the oppoents score for one of their moves
               /* if (myScore > opponentScore){
                    myScoreBigger++;
                }   
                 */
            }
        }
    }
   // rif its negative, return lowest score
   if (myScore - opponentScore < 0){
       return 1;
   }else{
       // plus one to account for prev statement
       return (myScore - opponentScore) + 1;
   }
}


bool opponentNearby(char board[][MAX], int n, char compColour, int possRow, int possCol){
    // calculates how many times my score is bigger 
    
    char copyBoard[n+1][MAX];
    int row, col;
    char opponent = findOpponent(compColour);
    bool checking;
    char tile;
    
    // makes a copy of the current board, so I can  temporary modify the board
    for (int row = 0; row <= n; row++){
        for (int col = 0; col <= n; col++){
            copyBoard[row][col] = board[row][col];
        }
    }
    playMove(copyBoard, n, possRow, possCol, compColour);
    
     for (int deltaRow = -1; deltaRow <=1; deltaRow++){
        for (int deltaCol = -1; deltaCol <=1; deltaCol++){
            if ((deltaRow != 0) && (deltaCol !=0)){
               // row = possRow + deltaRow;
             //   col = possCol + deltaCol;
             row = possRow + deltaRow;
             col = possCol + deltaCol;
                
              //  if ((copyBoard[row][col] == opponent) && (copyBoard[row + deltaCol][col + deltaCol] == 'U')){
                 //   return true;
              //  }
              checking = true;
              
              while(checking){
                tile = copyBoard[row][col];
                
                // if own tile
                if (tile == opponent){
                    row += deltaRow;
                   col += deltaCol;
                // if the opponent can jump
                }else if(tile == 'U'){
                    return true;
                // id there is not an empty spot for the ipponent to 
                }else{
                    checking = false;
                }
            }
            }
        }
     }
    return false;
}

bool compTurn(char board[][MAX], int n, char colour, int *playRow, int *playCol, int numTurns){
    // determines move for computer, if there is no move returns false, else returns true and hanges value of row and col that it will play in main
    
  //  const int EARLY_GAME = 0; // change - 5-10 was okay
    // prev version that won against smartest only, get rifd of oppoennt nearby
    
    char piece;
    int cornerScore = 0;
   int edgeScore = 0;
    int betterScore = 0;
  //  int badScore = 0;
    int score;
    int possRow = -1, possCol = -1;
    bool cornerMove = false;
    bool edgeMove = false;
    bool closeToEdge = true;
    bool oppClose;
    bool canPlay;

    *playRow = -1; 
    *playCol = -1;
    char opponent = findOpponent(colour);
    
    for (int row = 1; row <=n; row ++){
        for (int col = 1; col <= n; col++){
            piece = board[row][col];
            canPlay = false;
            
            // checks for empty space
            if(piece == 'U'){
                for (int deltaRow = -1; (deltaRow <=1) && (!canPlay); deltaRow++){
                    for (int deltaCol = -1; (deltaCol <=1)  && (!canPlay); deltaCol++){
                        
                        // if that is a legal move, calculates the score
                        if(legalDirection(board, n, row, col, colour, deltaRow, deltaCol)){
                            canPlay = true; // exits for loops
                        }
                    }       
                }
                
                // if the position is a possible move
                if (canPlay){
                    //numMineBetter = predictOpponent(board, n, colour, row, col);
                   /* if (numTurns <= EARLY_GAME ){
                        score = predictOpponent(board, n, colour, row, col);
                    }else{
                        score = calcScore(board, n, row, col, colour);
                    }*/
                    score = calcScore(board, n, row, col, colour);
                    oppClose = opponentNearby(board, n, colour, row, col);
                    
                    // checks if a possible move is a corner
                    if (((row ==1) && (col ==1))  || ((row ==1) && (col == n))  || ((row == n) && (col == 1))  || ((row == n) && (col == n))){
                
                        if ((score > cornerScore)){
                            *playRow = row; 
                            *playCol = col;
                            cornerScore = score;  
                            closeToEdge = false;    
                        }
                        cornerMove = true;
                        
                    // if it is an edge
                    } else if (((row == 1) || (col == 1) || (row ==n) || (col ==n)) && (!cornerMove)){
                        // checks above row and below if the oppponent is there and if there is an empty space -Ffor the oponent to jump
                        if (((board[row][col+1] != opponent) && (board[row][col-1] != 'U')) || ((board[row][col -1] != opponent) && (board[row][col +1] != 'U')) || (!edgeMove)){
                            
                            // detrmine which edge move is better, but does not take into account bad edges
                            if ((score > edgeScore) && ((row != 2) && (row != n-1) && (col != 2) && (col != n-1))){
                                *playRow = row; 
                                *playCol = col;
                                edgeScore = score;
                                edgeMove = true;   
                               closeToEdge = false;
 
                            }
                        }
                        
                    // regular move, if there has not already have been another move that is an edge or corner move
                    }else if ((score > betterScore) && (!cornerMove) && (!edgeMove) && (row != 2) && (row != n-1) && (col != 2) && (col != n-1) && (!oppClose)){
                        // if its greater, then changes the row and col asscociated with the highscore in main
                        *playRow = row; 
                        *playCol = col;
                        betterScore = score;
                       closeToEdge = false;
                    } 
                    
                    // iff didnt change move
                    if (closeToEdge){ 
                        if ((possRow== -1) || (!oppClose)){
                            possRow = row; 
                            possCol = col;
                        }

                    }
                }
            }   
        }
    }
    
     // if it didnt change a move
    if (*playRow == -1){
        *playRow = possRow; // if no valid move, keeps playRow and co;  as -1
        *playCol = possCol;
    }
        
    if ((*playRow == -1) && (*playCol == -1)){
        return false; // no move
    } else {
        return true; //made a move
    }
}
