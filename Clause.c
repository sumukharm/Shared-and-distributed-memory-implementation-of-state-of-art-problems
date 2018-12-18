#define null '\0'


#include <cilk.h>
#include <reducer_opor.h>

CILK_BEGIN_CPLUSPLUS_HEADERS
#include <stdio.h>
CILK_END_CPLUSPLUS_HEADERS

#include "Preposition.cilk"

class Clause {
private:
	Preposition* prepositions;
	int prepositionsLength;
	void writeClause();

public:
	Clause();
	Clause(Preposition* prepList, int prepListLength);
	Clause(Preposition p);
	void cloneClause(Clause* c);
        bool isEmpty();
	void addPreposition(Preposition n);
	bool containPreposition(Preposition* p);
	bool containNegationOfPreposition(Preposition* p);
	bool containOnlyPreposition(Preposition* p);
	bool containOnlyNegationOfPreposition(Preposition* p);
	bool isSameClause(Clause* c);
	Clause resolvant(Clause* c);
	Clause removePreposition(Preposition* p);
	bool isFact(Preposition* p);
	bool isFact();
	Preposition GetPrepositionAt(int i);
        void printClause();


	virtual ~Clause();
	int getPrepositionsLength() const;
};

Clause::Clause(){
	prepositionsLength = 0;
}

Clause::Clause(Preposition p) {

	prepositions = new Preposition[1];
	prepositions[0] = p;
	prepositionsLength = 1;

}

Clause::Clause(Preposition *p, int pLength) {

	prepositions = new Preposition[pLength];
	for ( int i = 0 ; i < pLength ; ++i ){
		prepositions[i] = p[i];
	}
	prepositionsLength = pLength;

}

void Clause::cloneClause(Clause* c){
  for(int i = 0 ; i < prepositionsLength ; ++i ){
    c->addPreposition(prepositions[i]);
  }
}

void Clause::printClause(){
  for(int i = 0 ; i < prepositionsLength ; ++i ){
    prepositions[i].writePreposition();
  }
  printf("\n");
}

int Clause::getPrepositionsLength() const {
	return prepositionsLength;
}

Clause::~Clause() {
  //if ( prepositionsLength != 0 ) delete [] prepositions;
}

bool Clause::isEmpty(){
	if ( prepositionsLength == 0 ) return true;
	else return false;
}

void Clause::addPreposition(Preposition newPreposition){
	if( prepositionsLength == 0 ) {
		prepositions = new Preposition[1];
		prepositions[0] = newPreposition;
		prepositionsLength = 1;
		return;
	}

	if ( !containPreposition(&newPreposition) ){
		Preposition* newPrepositions = new Preposition[prepositionsLength + 1];

		for( int i = 0 ; i < prepositionsLength ; ++i ) newPrepositions[i] = prepositions[i];
		newPrepositions[ prepositionsLength ] = newPreposition;
		prepositions = newPrepositions;
		prepositionsLength++;
	}

}

bool Clause::containPreposition(Preposition* p){
  if ( prepositions == null ) return false;
  cilk::reducer_opor<bool> result;
  result.set_value(false);
  #pragma omp parallel
  #pragma omp for
  for( int i = 0 ; i < prepositionsLength ; ++i ){
    if ( prepositions[i].isSame(p) )result |= true;
  }
  return result.get_value();
}

bool Clause::containNegationOfPreposition(Preposition* p){
  if ( prepositions == null ) return false;
  cilk::reducer_opor<bool> result;
  result.set_value(false);
  #pragma omp parallel
  #pragma omp for
  for( int i = 0 ; i < prepositionsLength ; ++i ){
    if ( prepositions[i].isNegation(p) )result |= true;
  }
  return result.get_value();
}

bool Clause::containOnlyPreposition(Preposition* p){
	if ( prepositions == null ) return false;
	if ( prepositionsLength == 1 && prepositions[0].isSame(p) )return true;
	else return false;

}

bool Clause::containOnlyNegationOfPreposition(Preposition* p){
	if ( prepositions == null ) return false;
	if ( prepositionsLength == 1 && prepositions[0].isNegation(p) )return true;
	else return false;
}

bool Clause::isSameClause(Clause* c){
        bool result = true;
	for ( int i = 0 ; i < prepositionsLength ; ++i ){
		if ( !c->containPreposition(&prepositions[i]) )result = false;
	}
	return result;
}

Clause Clause::resolvant(Clause* c){
	for ( int i = 0 ; i < prepositionsLength ; ++i ){
		if ( c->containNegationOfPreposition(&prepositions[i]) ){
			if ( containOnlyPreposition(&prepositions[i]) && ( c->containOnlyNegationOfPreposition(&prepositions[i]) ) ) {
				writeClause();
				printf("*****");
				c->writeClause();
			} else {
				Clause cNew;
				for( int j = 0 ; j < prepositionsLength ; ++j ){
					if (!prepositions[j].isSame(&prepositions[i]) )cNew.addPreposition(prepositions[j]);
				}
				for( int j = 0 ; j < c->getPrepositionsLength() ; ++j ){
					if (!c->GetPrepositionAt(j).isNegation(&prepositions[i]) ) cNew.addPreposition(c->GetPrepositionAt(j));
				}

				return cNew;
			}
		}
	}
	return Clause();
}

bool Clause::isFact(Preposition* p){
	return containOnlyPreposition(p);
}

Preposition Clause::GetPrepositionAt(int index){
	return prepositions[index];
}

void Clause::writeClause(){
	printf("\n");
	for( int i = 0 ; i < prepositionsLength ; ++i ){
		prepositions[i].writePreposition();
		printf(",");
	}

}


bool Clause::isFact(){
	if( prepositions == null )return false;
	if( prepositionsLength == 1 )return true;
	else return false;
}

