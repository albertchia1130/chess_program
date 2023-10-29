#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

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

const pieces StartingPosition[]={
    {a,1,white,Pawn},{b,1,white,Pawn},{c,1,white,Pawn},{d,1,white,Pawn},{e,1,white,Pawn},{f,1,white,Pawn},{g,1,white,Pawn},{h,1,white,Pawn},
    {a,0,white,Castle},{b,0,white,Knight},{c,0,white,Bishop},{d,0,white,King},{e,0,white,Queen},{f,0,white,Bishop},{g,0,white,Knight},{h,0,white,Castle},
    {a,6,black,Pawn},{b,6,black,Pawn},{c,6,black,Pawn},{d,6,black,Pawn},{e,6,black,Pawn},{f,6,black,Pawn},{g,6,black,Pawn},{h,6,black,Pawn},
    {a,7,black,Castle},{b,7,black,Knight},{c,7,black,Bishop},{d,7,black,King},{e,7,black,Queen},{f,7,black,Bishop},{g,7,black,Knight},{h,7,black,Castle}
};


bool SetupStartingPosition();
void Draw();

void main(){
    SetupStartingPosition();
    Draw();
    printf("Testing\n");
    return;
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

void Draw(){

    for(int y =0;y < Y_AXIS;y++){
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
