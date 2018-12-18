class Perception {
private:

	bool stench;
	bool breeze;
	bool glitter;
	bool bump;
	bool scream;
        bool pit;
        bool wumpus;
        bool lost;
public:
	Perception();
	Perception(bool l);
	Perception(bool st, bool br, bool gl, bool bu, bool sc, 
            bool pi, bool wu);
	virtual ~Perception();
	bool isBreeze() const;
	void setBreeze(bool breeze = false);
	bool isBump() const;
	void setBump(bool bump = false);
	bool isGlitter() const;
	void setGlitter(bool glitter = false);
	bool isScream() const;
	void setScream(bool scream = false);
	bool isStench() const;
	void setStench(bool stench = false);
	bool isPit() const;
	void setPit(bool pit = false);
	bool isWumpus() const;
	void setWumpus(bool wumpus = false);
	bool isLost() const;
	void setLost(bool lost);
        void printPerception();
};

Perception::Perception() {

	stench = false;
	breeze = false;
	glitter = false;
	bump = false;
	scream = false;
	lost = false;
}

Perception::Perception(bool l) {

	stench = false;
	breeze = false;
	glitter = false;
	bump = false;
	scream = false;
	lost = l;
}

Perception::Perception(bool st, bool br, bool gl, bool bu, bool sc, 
    bool pi, bool wu) {
		stench = st;
		breeze = br;
		glitter = gl;
		bump = bu;
		scream = sc;
		pit = pi;
                wumpus = wu;
                lost = false;
	}

bool Perception::isBreeze() const {
	return breeze;
}

void Perception::setBreeze(bool breeze) {
	this->breeze = breeze;
}

bool Perception::isBump() const {
	return bump;
}

void Perception::setBump(bool bump) {
	this->bump = bump;
}

bool Perception::isGlitter() const {
	return glitter;
}

void Perception::setGlitter(bool glitter) {
	this->glitter = glitter;
}

bool Perception::isScream() const {
	return scream;
}

void Perception::setScream(bool scream) {
	this->scream = scream;
}

bool Perception::isStench() const {
	return stench;
}

void Perception::setStench(bool stench) {
	this->stench = stench;
}

bool Perception::isPit() const {
	return pit;
}

void Perception::setPit(bool pit) {
	this->pit = pit;
}

bool Perception::isWumpus() const {
	return wumpus;
}

void Perception::setWumpus(bool wumpus) {
	this->wumpus = wumpus;
}

bool Perception::isLost() const {
	return lost;
}

void Perception::setLost(bool lost) {
	this->lost = lost;
}

void Perception::printPerception(){
  if ( breeze ) printf("b"); 
  if ( stench ) printf("s"); 
  if ( glitter ) printf("g"); 
  if ( bump ) printf("m"); 
  if ( scream ) printf("r"); 
  if ( pit ) printf("p");
  if ( wumpus ) printf("w"); 
  printf("\n");
}

Perception::~Perception() {
	// TODO Auto-generated destructor stub
}

