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
bool Castling = false;
bool EnPassant = false;
pieces* EnPassantPiece = NULL;

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
    printf("this\n");
    if((FromX >= X_AXIS) || (FromY >= Y_AXIS) || (FromX < 0) || (FromX < 0) ||
       (ToX >= X_AXIS) || (ToY >= Y_AXIS) || (ToX < 0) || (ToY < 0)){
            printf("Illegal Move: Illegal input\n");
            return false;
    }

    if(Board_status[FromX][FromY] == NULL) 
    {
        printf("Illegal Move: Empty\n");
        return false;
        
    }

    if((whiteTurn == true && pieceToMove->whichSide !=white) || 
        (whiteTurn == false && pieceToMove->whichSide !=black)){

        printf("Illegal Move: Not your piece\n");
        return false;
    }


    if(Board_status[FromX][FromY]!=NULL){
        if(((pieceToMove->Role == Knight) && !CheckRulesForKnight(pieceToMove,ToX, ToY))||
           ((pieceToMove->Role == Castle) && !CheckRulesForCastle(pieceToMove,ToX, ToY))||
           ((pieceToMove->Role == Bishop) && !CheckRulesForBishop(pieceToMove,ToX, ToY))||
           ((pieceToMove->Role == Pawn) && !CheckRulesForPawn(pieceToMove,ToX, ToY))    ||
           ((pieceToMove->Role == Queen) && !CheckRulesForQueen(pieceToMove,ToX, ToY))||
           ((pieceToMove->Role == King) && !CheckRulesForKing(pieceToMove,ToX, ToY))){
            printf("Illegal Move: no possible moveset\n");
            return false;
        }

    }
    else{
        return false;
    }

    if((Board_status[ToX][ToY] != NULL) && (pieceToMove->whichSide == Board_status[ToX][ToY]->whichSide) && 
       (!Castling)){
        printf("Illegal Move: Self Capture is not allowed\n");
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
    if((pieceToMove->Role == Pawn) &&
       (((pieceToMove->whichSide == white) && (ToY == 7)) ||
       ((pieceToMove->whichSide == black) && (ToY == 0)))){
        char selection;
        while(1){
            printf("Promotion: Queen[Q] Knight[K] Castle[C] Bishop[B] \n ");
            selection = getchar();
            if(selection == 'q'){
                pieceToMove->Role = Queen;
                break;
            }
            else if(selection == 'k'){
               pieceToMove->Role = Knight;
               break;
            }
            else if(selection == 'c'){
               pieceToMove->Role = Castle;
               break;
            }
            else if(selection == 'b'){
                pieceToMove->Role = Bishop;
                break;
            }
            else{
                printf("Invalid Selection\n");
            }
       }
    }
    
    return true;
}

bool IsPositionInCheck(pieces* KingPiece, int PositionX, int PositionY){
        for(int y = 0; y<Y_AXIS; y++){
	        for(int x = 0;x<X_AXIS;x++){
                if((Board_status[x][y] != NULL) && (Board_status[x][y]->whichSide != KingPiece->whichSide)){
                    if(((Board_status[x][y]->Role == Knight) && CheckRulesForKnight(Board_status[x][y],PositionX, PositionY))||
                       ((Board_status[x][y]->Role == Castle) && CheckRulesForCastle(Board_status[x][y],PositionX, PositionY))||
                       ((Board_status[x][y]->Role == Bishop) && CheckRulesForBishop(Board_status[x][y],PositionX, PositionY))||
                       ((Board_status[x][y]->Role == Pawn) && CheckRulesForPawn(Board_status[x][y],PositionX, PositionY))    ||
                       ((Board_status[x][y]->Role == Queen) && CheckRulesForQueen(Board_status[x][y],PositionX, PositionY))||
                        ((Board_status[x][y]->Role == King) && CheckRulesForKing(Board_status[x][y],PositionX, PositionY))){
                            return true;

                        }
                }

            }
        }
        return false;

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

    TempX = PositionX + 2;
    TempY = PositionY + 1;
    if((TempX == ToX) && (TempY == ToY)){
        return true;
    }
    TempX = PositionX + 2;
    TempY = PositionY - 1;
    if((TempX == ToX) && (TempY == ToY)){
        return true;
    }

    TempX = PositionX + 1;
    TempY = PositionY + 2;
    if((TempX == ToX) && (TempY == ToY)){
        return true;
    }

    TempX = PositionX + 1;
    TempY = PositionY - 2;
    if((TempX == ToX) && (TempY == ToY)){
        return true;
    }

    TempX = PositionX - 2;
    TempY = PositionY + 1;
    if((TempX == ToX) && (TempY == ToY)){
        return true;
    }

    TempX = PositionX -2 ;
    TempY = PositionY -1;
    if((TempX == ToX) && (TempY == ToY)){
        return true;
    }
    
    TempX = PositionX-1;
    TempY = PositionY-2;
    if((TempX == ToX) && (TempY == ToY)){
        return true;
    }

    TempX = PositionX - 1;
    TempY = PositionY + 2;
    if((TempX == ToX) && (TempY == ToY)){
        return true;
    }
    printf(" Knight Illegal Move:Illegal Move\n");


    return false;
}

bool CheckRulesForPawn(pieces* PieceToMove,int ToX, int ToY){
    int TempX;
    int TempY;
    int PositionX = (PieceToMove->x);
    int PositionY = (PieceToMove->y);


    if((PieceToMove->whichSide == white) &&
        (ToY == (PositionY+1)) &&
        (ToX == PositionX)){
            if(Board_status[ToX][ToY] == NULL){
               return true;
            }
    }
    else if((PieceToMove->whichSide == black) &&
        (ToY == (PositionY-1)) &&
        (ToX == PositionX)){
            if(Board_status[ToX][ToY] == NULL){
               return true;
            }
    }
    else if((PieceToMove->whichSide == white) &&
        (ToY == (PositionY+2)) &&
        (ToX == PositionX)){
            if((Board_status[ToX][ToY] == NULL) && (PositionY == 1)){
               return true;
            }
    }
    else if((PieceToMove->whichSide == black) &&
        (ToY == (PositionY-2)) &&
        (ToX == PositionX)){
            if((Board_status[ToX][ToY] == NULL) && (PositionY == 6)){
               return true;
            }
    }
    else if((PieceToMove->whichSide == white) &&
        (ToY == (PositionY+1)) &&
        ((ToX == (PositionX+1)) || (ToX == (PositionX-1)))){
            if(Board_status[ToX][ToY] != NULL){
               return true;
            }
    }
    else if((PieceToMove->whichSide == black) &&
        (ToY == (PositionY-1)) &&
        ((ToX == (PositionX+1)) || (ToX == (PositionX-1)))){
            if(Board_status[ToX][ToY] != NULL){
               return true;
            }
    }

    return false;//To be Implemented
}
bool CheckRulesForCastle(pieces* PieceToMove,int ToX, int ToY){

    int PositionX = (PieceToMove->x);
    int PositionY = (PieceToMove->y);
    int TempX=PositionX;
    int TempY=PositionY;
    int Direction; // Indicate the direction of search (up down left right)

    if(PositionX == ToX){
        if(PositionY < ToY){
            Direction = 0;
        }
        else if(PositionY > ToY){
            Direction = 1;
        }
        else{
            return false;
        }
    }
    else if(PositionY == ToY){
        if(PositionX < ToX){
            Direction = 2;
        }
        else if(PositionX > ToX){
            Direction = 3;
        }
        else{
            return false;
        }
    }
    else{
        return false;
    }


    while(true){
        if(Direction == 0){
            TempY++;
        }
        else if(Direction ==1){
            TempY--;
        }
       else if(Direction ==2){
            TempX++;
        }
       else if(Direction ==3){
            TempX--;
        }
        if(TempX ==ToX && TempY ==ToY){
            break;
        }
        else if((Board_status[TempX][TempY] != NULL) && (TempX !=ToX || TempY !=ToY)){
            return false; // return false if there is a piece blocking in the way thus illegal
        }
    }

    return true;
}
bool CheckRulesForBishop(pieces* PieceToMove,int ToX, int ToY){

    int PositionX = (PieceToMove->x);
    int PositionY = (PieceToMove->y);
    int TempX=PositionX;
    int TempY=PositionY;
    int Direction; // Indicate the direction of search (up down left right)

    if((abs(ToX - PositionX)) != (abs(ToY - PositionY))){
        return false;
    }
    else if(PositionX > ToX){
        if(PositionY < ToY){ //315 degree
            Direction = 0; 
        }
        else if(PositionY > ToY){ //225 degree
            Direction = 1; 
        }
        else{
            return false;
        }
    }
    else if(PositionX < ToX){
        if(PositionY < ToY){ //45 degree
            Direction = 2; 
        }
        else if(PositionY > ToY){ // 135 degree
            Direction = 3;
        }
        else{
            return false;
        }
    }

    while(true){
        if(Direction == 0){
            TempX--;
            TempY++;
        }
        else if(Direction ==1){
            TempX--;
            TempY--;
        }
       else if(Direction ==2){
            TempX++;
            TempY++;
        }
       else if(Direction ==3){
            TempX++;
            TempY--;
        }
        if(TempX ==ToX && TempY ==ToY){
            break;
        }
        else if((Board_status[TempX][TempY] != NULL) && (TempX !=ToX || TempY !=ToY)){
            return false; // return false if there is a piece blocking in the way thus illegal
        }
    }
    return true;
}
bool CheckRulesForQueen(pieces* PieceToMove,int ToX, int ToY){
    if((CheckRulesForBishop(PieceToMove,ToX, ToY) == false) && (CheckRulesForCastle(PieceToMove,ToX, ToY) == false)){ // Queen share the same moveset as bishop and castle
        return false;
    }
    return true;
}
bool CheckRulesForKing(pieces* PieceToMove,int ToX, int ToY){
    int PositionX = (PieceToMove->x);
    int PositionY = (PieceToMove->y);
    if((abs(ToX - PositionX) > 1) ||
        (abs(ToY - PositionY) > 1)){
            return false;
        }
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





