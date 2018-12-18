#include <cilk.h>

CILK_BEGIN_CPLUSPLUS_HEADERS
#include <stdio.h>
CILK_END_CPLUSPLUS_HEADERS



class Preposition {
private:
	/**
	 * type can be  'B' => Breeze
	 * 			    'G' => Glitter
	 * 			    'O' => OK, it is safe
	 * 			    'P' => Pit
	 * 			    'S' => Stench
	 * 			    'V' => Visited
	 * 			    'W' => Wumpus
	 */
	char type;

	bool negation;

	int xPos;
	int yPos;

public:
	Preposition();
	Preposition(char t, bool n, int x, int y);
	virtual ~Preposition();
	void writePreposition();
	bool isSame(Preposition* p);
	bool isNegation(Preposition* p);
	Preposition toggleNegation();
	bool isNegation() const;
	char getType() const;
	int getxPos() const;
	int getyPos() const;
};

Preposition::Preposition() {
	type = 'O';
	negation = false;
	xPos = 0;
	yPos = 0;
}

Preposition::Preposition(char t, bool n, int x, int y) {
	type = t;
	negation = n;
	xPos = x;
	yPos = y;
}

Preposition::~Preposition() {
	// TODO Auto-generated destructor stub
}

bool Preposition::isNegation() const {
	return negation;
}

char Preposition::getType() const {
	return type;
}

int Preposition::getxPos() const {
	return xPos;
}

int Preposition::getyPos() const {
	return yPos;
}

bool Preposition::isSame(Preposition* p) {
	if( ( p->getxPos() == xPos ) && ( p->getyPos() == yPos ) && ( p->getType() == type ) && ( p->isNegation() == negation ) )return true;
	else return false;
}

bool Preposition::isNegation(Preposition* p){
	if( ( p->getxPos() == xPos ) && ( p->getyPos() == yPos ) && ( p->getType() == type ) && ( p->isNegation() != negation ) )return true;
	else return false;
}

Preposition Preposition::toggleNegation(){
		return Preposition(type, !negation, xPos, yPos);
	}

void Preposition::writePreposition(){
	printf("(");
	if( !negation )printf("~");
	printf("%c, %d, %d )" ,type, xPos, yPos);
	if( ( type != 'B' ) && ( type != 'G' ) && ( type != 'O' ) && ( type != 'P' ) && ( type != 'S' ) && ( type != 'V' ) && ( type != 'W' )  )
		printf("Error publishing the preposition\n");
}
