#include <cilk.h>

CILK_BEGIN_CPLUSPLUS_HEADERS
#include <stdio.h>
#include <fstream>
#include <iostream>
CILK_END_CPLUSPLUS_HEADERS

#include "Perception.cilk"


class Cave {
private:
	int outputMode; //0=Statistics 1=Verbose
	int size;
	int agentX;
	int agentY;

	int t ;

	int agentDirection;

	/*
	 * To Right = 0
	 * To Up    = 1
	 * To Left  = 2
	 * To Down  = 3
	 */

	bool agentHasArrow;
	bool agentGrabbedGold;

	bool wumpusAlive;
	int wumpusX;
	int wumpusY;

	bool** pits;

	int goldX;
	int goldY;
	bool getBreezeState(int, int);
	bool getStench(int, int);
	bool getGlitter(int, int);
	bool getPit(int, int);
	bool getWumpus(int, int);
	void drawScreen(char);

public:
	Cave();
	Cave(int);
	Cave(int, char*);
	Perception putAgentAt( int, int);
	Perception getAction(char);
	Perception stateChange(char);

	virtual ~Cave();
	int getSize() const;
};


Cave::Cave() {
	outputMode = 0;
	agentDirection = 0;
	pits = new bool*[4];
	for( int i = 0 ; i < 4 ; ++i )
		pits[i] = new bool[4];
	for( int i = 0 ; i < 4 ; ++i )
		for( int j = 0 ; j < 4 ; ++j )
			pits[i][j] = false;
	pits[2][0] = true;
	pits[2][2] = true;
	pits[3][3] = true;
	size = 4;


	agentHasArrow = true;
	agentGrabbedGold = false;

	wumpusAlive = true;

	wumpusX = 0;
	wumpusY = 2;

	agentX = 0;
	agentY = 0;

	goldX = 1;
	goldY = 2;
}

Cave::Cave(int oMode) {
	// TODO Auto-generated constructor stub
	agentDirection = 0;

	pits = new bool*[4];
	for( int i = 0 ; i < 4 ; ++i )
		pits[i] = new bool[4];
	for( int i = 0 ; i < 4 ; ++i )
		for( int j = 0 ; j < 4 ; ++j )
			pits[i][j] = false;
	pits[2][0] = true;
	pits[2][2] = true;
	pits[3][3] = true;
	size = 4;
	outputMode = oMode;

	wumpusX = 0;
	wumpusY = 2;

	agentX = 0;
	agentY = 0;

	agentHasArrow = true;
	agentGrabbedGold = false;

	wumpusAlive = true;

	goldX = 1;
	goldY = 2;
	t = 0;
}

Cave::Cave(int oMode, char* fileName){
	std::cout << "This is the file => " << fileName;

	outputMode = oMode;
	agentDirection = 0;


	wumpusX = 0;
	wumpusY = 2;

	goldX = 1;
	goldY = 2;

	agentX = 0;
	agentY = 0;

	agentHasArrow = true;
	agentGrabbedGold = false;

	t = 0;
	wumpusAlive = true;
	std::ifstream myReadFile;
	myReadFile.open(fileName);
	char data;

	int gX, gY, wX, wY;
	if (myReadFile.is_open()) {
		myReadFile >> size;
		myReadFile >> gX;
		myReadFile >> gY;
		myReadFile >> wX;
		myReadFile >> wY;
		//pits = new bool[size][size];
		pits = new bool*[size];
		for( int i = 0 ; i < size ; ++i )
				pits[i] = new bool[size];
		for( int i = 0 ; i < size ; ++i){
			for( int j =0 ; j < size ; ++j){
				myReadFile >> data;
				if( data == 'p'){
					pits[j][i] = true;
				} else {
					pits[j][i] = false;
				}
			}
			std::cout << "\n";
		}
		wumpusX = wX;
		wumpusY = wY;
		goldX = gX;
		goldY = gY;
	}
	myReadFile.close();
}

int Cave::getSize() const {
	return size;
}

Cave::~Cave() {
	// TODO Auto-generated destructor stub
}


Perception Cave::putAgentAt( int x, int y){
	agentX = x;
	agentY = y;
	drawScreen('N');
	return Perception(getStench(agentX, agentY), getBreezeState(agentX, agentY), getGlitter(agentX, agentY), false, !wumpusAlive, getPit(agentX, agentY), getWumpus(agentX, agentY));
}

Perception Cave::getAction(char action){
	Perception output = stateChange(action);
	drawScreen(action);
	return output;
}

Perception Cave::stateChange(char action){
	++t;
	if ( action == 'L' ){
		++agentDirection;
		if( agentDirection > 3 )agentDirection = 0;
		return Perception(getStench(agentX, agentY), getBreezeState(agentX, agentY), getGlitter(agentX, agentY), false, !wumpusAlive, getPit(agentX, agentY), getWumpus(agentX, agentY));
	}
	if ( action == 'R' ){
		--agentDirection;
		if( agentDirection < 0 )agentDirection = 3;
		return Perception(getStench(agentX, agentY), getBreezeState(agentX, agentY), getGlitter(agentX, agentY), false, !wumpusAlive, getPit(agentX, agentY), getWumpus(agentX, agentY));
	}
	if ( action == 'M' ){
		switch (agentDirection) {
		case 0:
			if ( agentX == ( size - 1 ) )
				return Perception(getStench(agentX, agentY), getBreezeState(agentX, agentY), getGlitter(agentX, agentY), true, !wumpusAlive, getPit(agentX, agentY), getWumpus(agentX, agentY));
			else
				agentX++;
			break;
		case 1:
			if ( agentY == ( size - 1 ) )
				return Perception(getStench(agentX, agentY), getBreezeState(agentX, agentY), getGlitter(agentX, agentY), true, !wumpusAlive, getPit(agentX, agentY), getWumpus(agentX, agentY));
			else
				agentY++;
			break;
		case 2:
			if ( agentX == 0 )
				return Perception(getStench(agentX, agentY), getBreezeState(agentX, agentY), getGlitter(agentX, agentY), true, !wumpusAlive, getPit(agentX, agentY), getWumpus(agentX, agentY));
			else
				agentX--;
			break;
		case 3:
			if ( agentY == 0 )
				return Perception(getStench(agentX, agentY), getBreezeState(agentX, agentY), getGlitter(agentX, agentY), true, !wumpusAlive, getPit(agentX, agentY), getWumpus(agentX, agentY));
			else
				agentY--;
			break;
		default:
			printf("Invalid moving direction. Environment confused!!!!\n");
			break;
		};
	}
	if ( action == 'S' ){
		if ( !agentHasArrow ){
			printf("Invalid shooting action. Agent doesn't have the arrow any more!!!!\n");
		} else {
			printf("Agent is shooting. Agent doesn't have the arrow any more!!!!\n");
			agentHasArrow = false;
			switch (agentDirection) {
			case 0:
				if ( ( agentX < wumpusX ) && ( agentY == wumpusY ) )
					wumpusAlive = false;
				break;
			case 1:
				if ( ( agentY < wumpusY ) && ( agentX == wumpusX ) )
					wumpusAlive = false;
				break;
			case 2:
				if ( ( agentX > wumpusX ) && ( agentY == wumpusY ) )
					wumpusAlive = false;
				break;
			case 3:
				if ( ( agentY > wumpusY ) && ( agentX == wumpusX ) )
					wumpusAlive = false;
				break;
			default:
				printf("Invalid moving direction. Environment confused!!!!\n");
				break;
			}

		}
	}
/*	if ( pits[agentX][agentY] ){
		printf("Agent has been fallen in a pit. Game over!!!!\n");
		return Perception(true);
	}

	if ( ( agentX==wumpusX ) && ( agentY==wumpusY ) ){
		printf("Agent has been eaten with wumpus. Game over!!!!\n");
		return Perception(true);
	}*/
	return Perception(getStench(agentX, agentY), getBreezeState(agentX, agentY), getGlitter(agentX, agentY), false, !wumpusAlive, getPit(agentX, agentY), getWumpus(agentX, agentY));

}


bool Cave::getBreezeState(int x, int y){
	if( ( x + 1 ) < size ){
		if( pits[ x + 1 ][y] ) return true;
	}
	if( ( x - 1 ) >= 0 ){
		if( pits[ x - 1 ][y] ) return true;
	}
	if( ( y + 1 ) < size ){
		if( pits[x][ y + 1 ] ) return true;
	}
	if( ( y - 1 ) >= 0 ){
		if( pits[x][ y - 1 ] ) return true;
	}
	return false;
}

bool Cave::getStench(int x, int y){
	if( (( x + 1 ) < size ) && (( x + 1)==wumpusX) && (y==wumpusY) )return true;
	if( (( x - 1 ) >= 0 ) && (( x - 1)==wumpusX) && (y==wumpusY) )return true;
	if( (( y + 1 ) < size ) && (( y + 1)==wumpusY) && (x==wumpusX) )return true;
	if( (( y - 1 ) >= 0 ) && (( y - 1)==wumpusY) && (x==wumpusX) )return true;
	return false;
}

bool Cave::getWumpus(int x, int y){
  return ( x == wumpusX && y == wumpusY );
}

bool Cave::getPit(int x, int y){
  return pits[x][y];
}

bool Cave::getGlitter(int x, int y){
	return( ( x==goldX ) && ( y==goldY ) );
}

void Cave::drawScreen(char recentAction){
	if( t > 500 ){
		printf("Answer unreachable!!! Crashed\n");
		return;
	}
	if( outputMode == 1 ){
		printf("Time = %d\n", t);
		for( int j = size - 1 ; j > -1 ; --j ){
			for( int i = 0 ; i < size ; ++i ){
				printf("|");
				if( ( i == agentX ) && ( j == agentY ) ){
					if( agentDirection == 0 )printf(">");
					if( agentDirection == 1 )printf("^");
					if( agentDirection == 2 )printf("<");
					if( agentDirection == 3 )printf("v");
				} else if( ( i == wumpusX ) && ( j == wumpusY ) )printf("W");
				else if( pits[i][j] )printf("*");
				else if( ( i == goldX ) && ( j == goldY ) )printf("G");
				else printf(" ");
			
                        }
			printf("|\n");
		}
	} else if ( recentAction == 'M' ){
		printf(t+"\t");
	}else printf(t+"\t\n");

}
