//replacing these cilk headers

#include<cilk.h>

CILK_BEGIN_CPLUSPLUS_HEADERS
#include <stdio.h>
CILK_END_CPLUSPLUS_HEADERS


#include "KnowledgeBase.cilk"


class Agent {
private:
	int outputMode;
	Cave* cave;
	int xPos;
	int yPos;

	int size;
	int score;

	int direction;

	/*
	 * To Right = 0
	 * To Up    = 1
	 * To Left  = 2
	 * To Down  = 3
	 */


	KnowledgeBase kb;

	void executeActions();
	Perception chooseAction();
	int getHDirect();
	int getHLeft();
	int getHRight();
	int getHBack();
	bool rewisePosition();
public:
	Agent(Cave*, int, int);
	Agent(Cave*, int, int, int, int);
	virtual ~Agent();
};


Agent::Agent(Cave* c, int x, int y){
	// TODO Auto-generated constructor stub
	outputMode = 0;
	cave = c;
	xPos = x;
	yPos = y;

	score = 0;
	size = cave->getSize();

	direction = 0;
	kb = KnowledgeBase(size);
}

Agent::Agent(Cave* c, int x, int y, int d, int oMode) {

	cave = c;
	xPos = x;
	yPos = y;
	direction = d;
	outputMode = oMode;
	score = 0;
	size = cave->getSize();
	kb = KnowledgeBase(size);
        executeActions();
}

void Agent::executeActions(){
	int resolutionSteps = 0;
	Perception currentPerception = cave->putAgentAt(xPos, yPos);
	do{
		if( currentPerception.isGlitter() ){
		  //printf("Success!!!!! I grabbed the gold.\n");
			score += 10000;
			printf("My Score is %d \n", score );
			return;
		}
		if( currentPerception.isPit() ){
			resolutionSteps += kb.addFact(Preposition('P', true, xPos, yPos));
                        score -= 1000;
                }
		else
			resolutionSteps += kb.addFact(Preposition('P', false, xPos, yPos));
		if( currentPerception.isWumpus() ){
			resolutionSteps += kb.addFact(Preposition('W', true, xPos, yPos));
                        score -= 5000;
                }
		else
			resolutionSteps += kb.addFact(Preposition('W', false, xPos, yPos));
		if( currentPerception.isBreeze() )
			resolutionSteps += kb.addFact(Preposition('B', true, xPos, yPos));
		else
			resolutionSteps += kb.addFact(Preposition('B', false, xPos, yPos));
		if( currentPerception.isStench() )
			resolutionSteps += kb.addFact(Preposition('S', true, xPos, yPos));
		else
			resolutionSteps += kb.addFact(Preposition('S', false, xPos, yPos));
		resolutionSteps += kb.addFact(Preposition('V', true, xPos, yPos));
		if( currentPerception.isScream() )
			kb.wumpusHasBeenKilled();
		currentPerception = chooseAction();
                //printf("Perception at %d and %d : ", xPos, yPos);
                //currentPerception.printPerception();
		if(outputMode == 0) printf("%d \t %d \t %d \n" , kb.getNumberOfFacts(), kb.getNumberOfClauses(), resolutionSteps);
		resolutionSteps = 0;

	}while(true);

}


Agent::~Agent() {
	// TODO Auto-generated destructor stub
}

Perception Agent::chooseAction(){
	int d = getHDirect();
	int l = getHLeft();
	int r = getHRight();
	int b = getHBack();
	if ( ( d >= l ) && ( d >= r ) && ( d >= b ) ) {
		score--;
		if ( rewisePosition() )
		  return cave->getAction('M');
	}
	if ( ( l >= r ) && ( l >= b ) ) {
		score -=2;
		cave->getAction('L');
		direction++;
		if( direction > 3 )direction = 0;
		if ( rewisePosition() )
		  return cave->getAction('M');
	}
	if ( r >= b ) {
		score -=2;
		cave->getAction('R');
		direction--;
		if( direction < 0 )direction = 3;
		if ( rewisePosition() )
		  return cave->getAction('M');
	} else {
		score -=3;
		cave->getAction('L');
		cave->getAction('L');
		direction += 2;
                if( direction > 3 )direction -= 4;
		if ( rewisePosition() )
		  return cave->getAction('M');
	}

}

int Agent::getHDirect(){
		switch(direction){
		case 0:
			if(xPos<( size - 1 )){
				if ( kb.queryFact(Preposition('P', true, ( xPos + 1 ), yPos)) )
					return -1001;
				if ( kb.queryFact(Preposition('W', true, ( xPos + 1 ), yPos)) )
					return -1001;
				if ( ( kb.queryFact( (Preposition('O', true, ( xPos + 1 ), yPos)) ) && ( !kb.queryFact(Preposition('V', true, ( xPos + 1 ), yPos))) ) )
					return 999;
				if ( ( kb.queryFact( (Preposition('O', true, ( xPos + 1 ), yPos)) ) && ( kb.queryFact(Preposition('V', true, ( xPos + 1 ), yPos))) ) )
					return -1;
				if ( kb.queryFact (Preposition('O', true, ( xPos + 1 ), yPos)) )
					return -201;
			} else return -4000;
			break;
		case 1:
			if(yPos<( size - 1 )){
				if ( kb.queryFact(Preposition('P', true, xPos, ( yPos + 1 ))) )
					return -1001;
				if ( kb.queryFact(Preposition('W', true, xPos, ( yPos + 1 ))) )
					return -1001;
				if ( ( kb.queryFact( (Preposition('O', true, xPos, ( yPos + 1 ))) ) && ( !kb.queryFact(Preposition('V', true, xPos, ( yPos + 1 )))) ) )
					return 999;
				if ( ( kb.queryFact( (Preposition('O', true, xPos, ( yPos + 1 ))) ) && ( kb.queryFact(Preposition('V', true, xPos, ( yPos + 1 )))) ) )
					return -1;
				if ( kb.queryFact (Preposition('O', true, xPos, ( yPos + 1 ))) )
					return -201;
			} else return -4000;
			break;
		case 2:
			if(xPos>0){
				if ( kb.queryFact(Preposition('P', true, ( xPos - 1 ), yPos)) )
					return -1001;
				if ( kb.queryFact(Preposition('W', true, ( xPos - 1 ), yPos)) )
					return -1001;
				if ( ( kb.queryFact( (Preposition('O', true, ( xPos - 1 ), yPos)) ) && ( !kb.queryFact(Preposition('V', true, ( xPos - 1 ), yPos))) ) )
					return 999;
				if ( ( kb.queryFact( (Preposition('O', true, ( xPos - 1 ), yPos)) ) && ( kb.queryFact(Preposition('V', true, ( xPos - 1 ), yPos))) ) )
					return -1;
				if ( kb.queryFact (Preposition('O', true, ( xPos - 1 ), yPos)) )
					return -201;
			} else return -4000;
			break;
		case 3:
			if(yPos>0){
				if ( kb.queryFact(Preposition('P', true, xPos, ( yPos - 1 ))) )
					return -1001;
				if ( kb.queryFact(Preposition('W', true, xPos, ( yPos - 1 ))) )
					return -1001;
				if ( ( kb.queryFact( (Preposition('O', true, xPos, ( yPos - 1 ))) ) && ( !kb.queryFact(Preposition('V', true, xPos, ( yPos - 1 )))) ) )
					return 999;
				if ( ( kb.queryFact( (Preposition('O', true, xPos, ( yPos - 1 ))) ) && ( kb.queryFact(Preposition('V', true, xPos, ( yPos - 1 )))) ) )
					return -1;
				if ( kb.queryFact (Preposition('O', true, xPos, ( yPos - 1 ))) )
					return -201;
			} else return -4000;
			break;
		default:
			return -4000;

		}
		return -4000;
	}


int Agent::getHLeft(){
		switch(direction){
		case 3:
			if(xPos<( size - 1 )){
				if ( kb.queryFact(Preposition('P', true, ( xPos + 1 ), yPos)) )
					return -1002;
				if ( kb.queryFact(Preposition('W', true, ( xPos + 1 ), yPos)) )
					return -1002;
				if ( ( kb.queryFact( (Preposition('O', true, ( xPos + 1 ), yPos)) ) && ( !kb.queryFact(Preposition('V', true, ( xPos + 1 ), yPos))) ) )
					return 998;
				if ( ( kb.queryFact( (Preposition('O', true, ( xPos + 1 ), yPos)) ) && ( kb.queryFact(Preposition('V', true, ( xPos + 1 ), yPos))) ) )
					return -2;
				if ( kb.queryFact (Preposition('O', true, ( xPos + 1 ), yPos)) )
					return -202;
			} else return -4001;
			break;
		case 0:
			if(yPos<( size - 1 )){
				if ( kb.queryFact(Preposition('P', true, xPos, ( yPos + 1 ))) )
					return -1002;
				if ( kb.queryFact(Preposition('W', true, xPos, ( yPos + 1 ))) )
					return -1002;
				if ( ( kb.queryFact( (Preposition('O', true, xPos, ( yPos + 1 ))) ) && ( !kb.queryFact(Preposition('V', true, xPos, ( yPos + 1 )))) ) )
					return 998;
				if ( ( kb.queryFact( (Preposition('O', true, xPos, ( yPos + 1 ))) ) && ( kb.queryFact(Preposition('V', true, xPos, ( yPos + 1 )))) ) )
					return -2;
				if ( kb.queryFact (Preposition('O', true, xPos, ( yPos + 1 ))) )
					return -202;
			} else return -4001;
			break;
		case 1:
			if(xPos>0){
				if ( kb.queryFact(Preposition('P', true, ( xPos - 1 ), yPos)) )
					return -1002;
				if ( kb.queryFact(Preposition('W', true, ( xPos - 1 ), yPos)) )
					return -1002;
				if ( ( kb.queryFact( (Preposition('O', true, ( xPos - 1 ), yPos)) ) && ( !kb.queryFact(Preposition('V', true, ( xPos - 1 ), yPos))) ) )
					return 998;
				if ( ( kb.queryFact( (Preposition('O', true, ( xPos - 1 ), yPos)) ) && ( kb.queryFact(Preposition('V', true, ( xPos - 1 ), yPos))) ) )
					return -2;
				if ( kb.queryFact (Preposition('O', true, ( xPos - 1 ), yPos)) )
					return -202;
			} else return -4001;
			break;
		case 2:
			if(yPos>0){
				if ( kb.queryFact(Preposition('P', true, xPos, ( yPos - 1 ))) )
					return -1002;
				if ( kb.queryFact(Preposition('W', true, xPos, ( yPos - 1 ))) )
					return -1002;
				if ( ( kb.queryFact( (Preposition('O', true, xPos, ( yPos - 1 ))) ) && ( !kb.queryFact(Preposition('V', true, xPos, ( yPos - 1 )))) ) )
					return 998;
				if ( ( kb.queryFact( (Preposition('O', true, xPos, ( yPos - 1 ))) ) && ( kb.queryFact(Preposition('V', true, xPos, ( yPos - 1 )))) ) )
					return -2;
				if ( kb.queryFact (Preposition('O', true, xPos, ( yPos - 1 ))) )
					return -202;
			} else return -4001;
			break;
		default:
			return -4001;

		}
		return -4001;
	}


int Agent::getHRight(){
		switch(direction){
		case 1:
			if(xPos<( size - 1 )){
				if ( kb.queryFact(Preposition('P', true, ( xPos + 1 ), yPos)) )
					return -1002;
				if ( kb.queryFact(Preposition('W', true, ( xPos + 1 ), yPos)) )
					return -1002;
				if ( ( kb.queryFact( (Preposition('O', true, ( xPos + 1 ), yPos)) ) && ( !kb.queryFact(Preposition('V', true, ( xPos + 1 ), yPos))) ) )
					return 998;
				if ( ( kb.queryFact( (Preposition('O', true, ( xPos + 1 ), yPos)) ) && ( kb.queryFact(Preposition('V', true, ( xPos + 1 ), yPos))) ) )
					return -2;
				if ( kb.queryFact (Preposition('O', true, ( xPos + 1 ), yPos)) )
					return -202;
			} else return -4001;
			break;
		case 2:
			if(yPos<( size - 1 )){
				if ( kb.queryFact(Preposition('P', true, xPos, ( yPos + 1 ))) )
					return -1002;
				if ( kb.queryFact(Preposition('W', true, xPos, ( yPos + 1 ))) )
					return -1002;
				if ( ( kb.queryFact( (Preposition('O', true, xPos, ( yPos + 1 ))) ) && ( !kb.queryFact(Preposition('V', true, xPos, ( yPos + 1 )))) ) )
					return 998;
				if ( ( kb.queryFact( (Preposition('O', true, xPos, ( yPos + 1 ))) ) && ( kb.queryFact(Preposition('V', true, xPos, ( yPos + 1 )))) ) )
					return -2;
				if ( kb.queryFact (Preposition('O', true, xPos, ( yPos + 1 ))) )
					return -202;
			} else return -4001;
			break;
		case 3:
			if(xPos>0){
				if ( kb.queryFact(Preposition('P', true, ( xPos - 1 ), yPos)) )
					return -1002;
				if ( kb.queryFact(Preposition('W', true, ( xPos - 1 ), yPos)) )
					return -1002;
				if ( ( kb.queryFact( (Preposition('O', true, ( xPos - 1 ), yPos)) ) && ( !kb.queryFact(Preposition('V', true, ( xPos - 1 ), yPos))) ) )
					return 998;
				if ( ( kb.queryFact( (Preposition('O', true, ( xPos - 1 ), yPos)) ) && ( kb.queryFact(Preposition('V', true, ( xPos - 1 ), yPos))) ) )
					return -2;
				if ( kb.queryFact (Preposition('O', true, ( xPos - 1 ), yPos)) )
					return -202;
			} else return -4001;
			break;
		case 0:
			if(yPos>0){
				if ( kb.queryFact(Preposition('P', true, xPos, ( yPos - 1 ))) )
					return -1002;
				if ( kb.queryFact(Preposition('W', true, xPos, ( yPos - 1 ))) )
					return -1002;
				if ( ( kb.queryFact( (Preposition('O', true, xPos, ( yPos - 1 ))) ) && ( !kb.queryFact(Preposition('V', true, xPos, ( yPos - 1 )))) ) )
					return 998;
				if ( ( kb.queryFact( (Preposition('O', true, xPos, ( yPos - 1 ))) ) && ( kb.queryFact(Preposition('V', true, xPos, ( yPos - 1 )))) ) )
					return -2;
				if ( kb.queryFact (Preposition('O', true, xPos, ( yPos - 1 ))) )
					return -202;
			} else return -4001;
			break;
		default:
			return -4001;

		}
		return -4001;
	}


int Agent::getHBack(){
		switch(direction){
		case 2:
			if(xPos<( size - 1 )){
				if ( kb.queryFact(Preposition('P', true, ( xPos + 1 ), yPos)) )
					return -1003;
				if ( kb.queryFact(Preposition('W', true, ( xPos + 1 ), yPos)) )
					return -1003;
				if ( ( kb.queryFact( (Preposition('O', true, ( xPos + 1 ), yPos)) ) && ( !kb.queryFact(Preposition('V', true, ( xPos + 1 ), yPos))) ) )
					return 997;
				if ( ( kb.queryFact( (Preposition('O', true, ( xPos + 1 ), yPos)) ) && ( kb.queryFact(Preposition('V', true, ( xPos + 1 ), yPos))) ) )
					return -3;
				if ( kb.queryFact (Preposition('O', true, ( xPos + 1 ), yPos)) )
					return -203;
			} else return -4002;
			break;
		case 3:
			if(yPos<( size - 1 )){
				if ( kb.queryFact(Preposition('P', true, xPos, ( yPos + 1 ))) )
					return -1003;
				if ( kb.queryFact(Preposition('W', true, xPos, ( yPos + 1 ))) )
					return -1003;
				if ( ( kb.queryFact( (Preposition('O', true, xPos, ( yPos + 1 ))) ) && ( !kb.queryFact(Preposition('V', true, xPos, ( yPos + 1 )))) ) )
					return 997;
				if ( ( kb.queryFact( (Preposition('O', true, xPos, ( yPos + 1 ))) ) && ( kb.queryFact(Preposition('V', true, xPos, ( yPos + 1 )))) ) )
					return -3;
				if ( kb.queryFact (Preposition('O', true, xPos, ( yPos + 1 ))) )
					return -203;
			} else return -4002;
			break;
		case 0:
			if(xPos>0){
				if ( kb.queryFact(Preposition('P', true, ( xPos - 1 ), yPos)) )
					return -1003;
				if ( kb.queryFact(Preposition('W', true, ( xPos - 1 ), yPos)) )
					return -1003;
				if ( ( kb.queryFact( (Preposition('O', true, ( xPos - 1 ), yPos)) ) && ( !kb.queryFact(Preposition('V', true, ( xPos - 1 ), yPos))) ) )
					return 997;
				if ( ( kb.queryFact( (Preposition('O', true, ( xPos - 1 ), yPos)) ) && ( kb.queryFact(Preposition('V', true, ( xPos - 1 ), yPos))) ) )
					return -3;
				if ( kb.queryFact (Preposition('O', true, ( xPos - 1 ), yPos)) )
					return -203;
			} else return -4002;
			break;
		case 1:
			if(yPos>0){
				if ( kb.queryFact(Preposition('P', true, xPos, ( yPos - 1 ))) )
					return -1003;
				if ( kb.queryFact(Preposition('W', true, xPos, ( yPos - 1 ))) )
					return -1003;
				if ( ( kb.queryFact( (Preposition('O', true, xPos, ( yPos - 1 ))) ) && ( !kb.queryFact(Preposition('V', true, xPos, ( yPos - 1 )))) ) )
					return 997;
				if ( ( kb.queryFact( (Preposition('O', true, xPos, ( yPos - 1 ))) ) && ( kb.queryFact(Preposition('V', true, xPos, ( yPos - 1 )))) ) )
					return -3;
				if ( kb.queryFact (Preposition('O', true, xPos, ( yPos - 1 ))) )
					return -203;
			} else return -4002;
			break;
		default:
			return -4002;

		}
		return -4002;
	}


bool Agent::rewisePosition(){
		//printf("Changing Agent's Direction");
		switch( direction ){
		case 0:
			if( xPos < ( size - 1 ) ){
                          ++xPos;
                          return true;
                        } else {
                          printf("Agent is confused and trying move toward x and it is wall!\n");
                          return false;
                        }
			break;
		case 1:
			if( yPos < ( size - 1 ) ){
                          ++yPos;
                          return true;
                        } else {
                          printf("Agent is confused and trying move toward y and it is wall!\n");
                          return false;
                        }
			break;
		case 2:
			if( xPos > 0 ){
                          --xPos;
                          return true;
                        } else {
                          printf("Agent is confused and trying move toward -x and it is wall!\n");
                          return false;
                        }
			break;
		case 3:
			if( yPos > 0 ){
                          --yPos;
                          return true;
                        } else {
                          printf("Agent is confused and trying move toward -y and it is wall!\n");
                          return false;
                        }
			break;
		default:
			printf("Agent is confused trying move toward nowhere!\n");
                        return false;
			break;
		}
	}
