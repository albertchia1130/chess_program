#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define X_AXIS 8
#define Y_AXIS 8
//ENUM
typedef enum x_position{
    a=0,b,c,d,e,f,g,h
}x_position;

typedef enum side{
    white=0,
    black
}side;

typedef enum ChessPiece{
    Pawn =0,
    Castle,
    Knight,
    Bishop,
    Queen,
    King
}ChessPiece;

//STRUCT
typedef struct pieces{
    x_position x;
    int y;
    side whichSide;
    ChessPiece Role;
}pieces;

//This is important as each position will contain either
// a black piece
// a white piece
// EMPTY represented by NULL pointer
pieces* Board_status[X_AXIS][Y_AXIS];
bool whiteTurn = true;

const pieces StartingPosition[]={
    {a,1,white,Pawn},{b,1,white,Pawn},{c,1,white,Pawn},{d,1,white,Pawn},{e,1,white,Pawn},{f,1,white,Pawn},{g,1,white,Pawn},{h,1,white,Pawn},
    {a,0,white,Castle},{b,0,white,Knight},{c,0,white,Bishop},{d,0,white,King},{e,0,white,Queen},{f,0,white,Bishop},{g,0,white,Knight},{h,0,white,Castle},
    {a,6,black,Pawn},{b,6,black,Pawn},{c,6,black,Pawn},{d,6,black,Pawn},{e,6,black,Pawn},{f,6,black,Pawn},{g,6,black,Pawn},{h,6,black,Pawn},
    {a,7,black,Castle},{b,7,black,Knight},{c,7,black,Bishop},{d,7,black,King},{e,7,black,Queen},{f,7,black,Bishop},{g,7,black,Knight},{h,7,black,Castle}
};


bool SetupStartingPosition();
bool MakingMove(int,int,int,int);
bool CheckRulesForKnight(pieces*,int, int);
bool CheckRulesForPawn(pieces*,int, int);
bool CheckRulesForCastle(pieces*,int, int);
bool CheckRulesForBishop(pieces*,int, int);
bool CheckRulesForQueen(pieces*,int, int);
bool CheckRulesForKing(pieces*,int, int);
void Draw();

void main(){
    SetupStartingPosition();
    bool endgame =false;
    char FromPosition[2];
    char ToPosition[2];
    int FromX,FromY,ToX,ToY;
    pieces* From;
    pieces* Destination;
    Draw();
    while(!endgame){
           
        if(whiteTurn){
            printf("white turn:\n");
        }
        else{
            printf("black turn:\n");
        }
	    printf("pieces To Move:\n");
	    scanf("%s",FromPosition);
	    printf("Move To position:\n");
	    scanf("%s",ToPosition);
        FromX = FromPosition[0] -97; //This converts the user char input to value
        FromY = FromPosition[1] -49;
        ToX = ToPosition[0] -97;
        ToY = ToPosition[1] -49;
        if(MakingMove(FromX,FromY,ToX,ToY)){
            whiteTurn= !whiteTurn; //Do not proceed if player makes illegal move
        }
        Draw();

    }
    
    printf("Testing\n");
    return;
}

//Determine whether the move is legal and make a move
bool MakingMove(int FromX,int FromY,int ToX,int ToY){
    pieces* pieceToMove;
    pieceToMove = Board_status[FromX][FromY];

    if((whiteTurn == true && pieceToMove->whichSide !=white) || 
        (whiteTurn == false && pieceToMove->whichSide !=black)){

        printf("Illegal Move: Not your pieces\n");
        return false;
    }

    if(Board_status[FromX][FromY]!=NULL){
        if(((pieceToMove->Role == Knight) && !CheckRulesForKnight(pieceToMove,ToX, ToY))||
           ((pieceToMove->Role == Castle) && !CheckRulesForCastle(pieceToMove,ToX, ToY))||
           ((pieceToMove->Role == Bishop) && !CheckRulesForBishop(pieceToMove,ToX, ToY))||
           ((pieceToMove->Role == Pawn) && !CheckRulesForPawn(pieceToMove,ToX, ToY))    ||
           ((pieceToMove->Role == Queen) && !CheckRulesForQueen(pieceToMove,ToX, ToY))||
           ((pieceToMove->Role == King) && !CheckRulesForKing(pieceToMove,ToX, ToY))){
            printf("Illegal Move:Illegal Move\n");
            return false;
        }

    }
    else{
        return false;
    }
    pieceToMove->x =ToX;
    pieceToMove->y =ToY;
    if(Board_status[ToX][ToY]!=NULL){
        printf("Captured\n");
        free(Board_status[ToX][ToY]);
    }
    Board_status[ToX][ToY] = pieceToMove;
    Board_status[FromX][FromY] = NULL;

}


//This function is to Draw out the starting position of the board
//The routine will allocate memory for each pieces and assign pointer based on the position they occupy.
bool SetupStartingPosition(){
    x_position x_p;
    int y_p;
    int arraySize = sizeof(StartingPosition)/sizeof(StartingPosition[0]);
    for(int i =0; i<arraySize; i++){
        x_p = StartingPosition[i].x;
        y_p = StartingPosition[i].y;
        Board_status[x_p][y_p]= (pieces*)malloc(sizeof(pieces));
        Board_status[x_p][y_p]->x = x_p;
        Board_status[x_p][y_p]->y = y_p;
        Board_status[x_p][y_p]->whichSide = StartingPosition[i].whichSide;
        Board_status[x_p][y_p]->Role =  StartingPosition[i].Role;
    }

     return true;
}

bool CheckRulesForKnight(pieces* PieceToMove,int ToX, int ToY){
    int TempX;
    int TempY;
    int PositionX = (PieceToMove->x);
    int PositionY = (PieceToMove->y);

    printf("x= %d\n",(PieceToMove->x));
    printf("y= %d\n",(PieceToMove->y));
    printf("MoveTo\n");
    printf("x= %d\n",ToX);
    printf("y= %d\n",ToY);



    TempX = PositionX + 2;
    TempY = PositionY + 1;
    printf("Temp x= %d\n",TempX);
    printf("Temp y= %d\n\n",TempY);
    if((TempX == ToX) && (TempY == ToY)){
        return true;
    }
    TempX = PositionX + 2;
    TempY = PositionY - 1;
    printf("Temp x= %d\n",TempX);
    printf("Temp y= %d\n\n",TempY);
    if((TempX == ToX) && (TempY == ToY)){
        return true;
    }

    TempX = PositionX + 1;
    TempY = PositionY + 2;
    printf("Temp x= %d\n",TempX);
    printf("Temp y= %d\n\n",TempY);
    if((TempX == ToX) && (TempY == ToY)){
        return true;
    }

    TempX = PositionX + 1;
    TempY = PositionY - 2;
    printf("Temp x= %d\n",TempX);
    printf("Temp y= %d\n\n",TempY);
    if((TempX == ToX) && (TempY == ToY)){
        return true;
    }

    
    TempX = PositionX - 2;
    TempY = PositionY + 1;
    printf("Temp x= %d\n",TempX);
    printf("Temp y= %d\n\n",TempY);
    if((TempX == ToX) && (TempY == ToY)){
        return true;
    }

    
    TempX = PositionX -2 ;
    TempY = PositionY -1;
    printf("Temp x= %d\n",TempX);
    printf("Temp y= %d\n\n",TempY);
    if((TempX == ToX) && (TempY == ToY)){
        return true;
    }

    
    TempX = PositionX-1;
    TempY = PositionY-2;
    printf("Temp x= %d\n",TempX);
    printf("Temp y= %d\n\n",TempY);
    if((TempX == ToX) && (TempY == ToY)){
        return true;
    }

    
    TempX = PositionX - 1;
    TempY = PositionY + 2;
    printf("Temp x= %d\n",TempX);
    printf("Temp y= %d\n\n",TempY);
    if((TempX == ToX) && (TempY == ToY)){
        return true;
    }
    printf(" Knight Illegal Move:Illegal Move\n");


    return false;
}

bool CheckRulesForPawn(pieces* PieceToMove,int ToX, int ToY){
    return true;//To be Implemented
}
bool CheckRulesForCastle(pieces* PieceToMove,int ToX, int ToY){
    return true;
}
bool CheckRulesForBishop(pieces* PieceToMove,int ToX, int ToY){
    return true;
}
bool CheckRulesForQueen(pieces* PieceToMove,int ToX, int ToY){
    return true;
}
bool CheckRulesForKing(pieces* PieceToMove,int ToX, int ToY){
    return true;
}


void Draw(){

    for(int y =Y_AXIS-1;y >= 0 ;y--){
	    for(int x = 0;x<X_AXIS;x++){
           	if(Board_status[x][y] == NULL){
                printf(" 0 ");
            }
            else{
                if(Board_status[x][y]->whichSide == black)
			        printf("\033[0;32m");
		    	else if(Board_status[x][y]->whichSide == white)
			        printf("\033[0;35m");

                if(Board_status[x][y]->Role == Pawn){
                    printf(" P ");
                }
                else if(Board_status[x][y]->Role == Castle){
                    printf(" C ");
                }
                else if(Board_status[x][y]->Role == Knight){
                    printf(" K ");
                }
                else if(Board_status[x][y]->Role == Bishop){
                    printf(" B ");
                }
                else if(Board_status[x][y]->Role == Queen){
                    printf(" Q ");
                }
                else if(Board_status[x][y]->Role == King){
                    printf(" E ");
                }


                printf("\033[0m");
            }
	    }  
    printf("\n\n");
    }
}





