#include <cilk_mutex.h>
#include <reducer_opor.h>
#include <reducer_opadd.h>
#include <reducer_list.h>

#include "Clause.cilk"

class KnowledgeBase {
private:
	Clause* clauses;
	int recentNumberOfResolutionSteps;
	int numberOfClauses;
	bool wumpusAlive;
	void initializeRules();
	void initializeRules(int environmentSize);
	bool addClause(Clause* n);
	bool containClause(Clause* n);
	void resolve();
	void removeClause(Clause* c);

public:
	KnowledgeBase();
	KnowledgeBase(int environmentSize);
	virtual ~KnowledgeBase();

	void wumpusHasBeenKilled();
	void wumpusHasBeenKilled(int environmentSize);
	bool queryFact(Preposition q);
	int getNumberOfFacts();
	int getNumberOfClauses();
	int addFact(Preposition p);
};


KnowledgeBase::KnowledgeBase() {

	recentNumberOfResolutionSteps = 0;
	numberOfClauses = 0;

	wumpusAlive = true;
	initializeRules();


}

KnowledgeBase::KnowledgeBase(int s) {

	recentNumberOfResolutionSteps = 0;
	numberOfClauses = 0;

	wumpusAlive = true;
	initializeRules(s);


}

KnowledgeBase::~KnowledgeBase() {
	// TODO Auto-generated destructor stub
}

int KnowledgeBase::addFact(Preposition p){
	recentNumberOfResolutionSteps = 0;
	Clause newP(p);
	if( wumpusAlive ){
		if( addClause(&newP ) )
			resolve();

	}else{
		if ( p.getType() != 'S' )
			if( addClause(&newP) )
				resolve();
        }
	return recentNumberOfResolutionSteps;
}

void KnowledgeBase::initializeRules(){
	int i = 0, j = 0;
/*
	for( i = 0 ; i < 4 ; ++i )     // V(x,y) => O(x,y)
		for( j = 0 ; j < 4 ; ++j ){
			Clause* temp = new Clause();
			temp->addPreposition(Preposition('V', false, i, j));
			temp->addPreposition(Preposition('O', true, i, j));
			addClause(temp);
		}
*/
	for( i = 0 ; i < 4 ; ++i )     // ~P(x,y) & ~W(x,y) => O(x,y)
		for( j = 0 ; j < 4 ; ++j ){
			Clause* temp = new Clause();
			temp->addPreposition(Preposition('P', true, i, j));
			temp->addPreposition(Preposition('W', true, i, j));
			temp->addPreposition(Preposition('O', true, i, j));
			addClause(temp);
		}


	for( i = 0 ; i < 4 ; ++i )
		for( j = 0 ; j < 4 ; ++j ){
			// ~B(x,y) => ~P(x-1,y)
			if( i > 0 ){
				Clause* temp = new Clause();
				temp->addPreposition(Preposition('B', true, i, j));
				temp->addPreposition(Preposition('P', false, ( i - 1 ), j));
				addClause(temp);
			}
			// ~B(x,y) => ~P(x+1,y)
			if( i < 3 ){
				Clause* temp = new Clause();
				temp->addPreposition(Preposition('B', true, i, j));
				temp->addPreposition(Preposition('P', false, ( i + 1 ), j));
				addClause(temp);
			}
			// ~B(x,y) => ~P(x,y-1)
			if( j > 0 ){
				Clause* temp = new Clause();
				temp->addPreposition(Preposition('B', true, i, j));
				temp->addPreposition(Preposition('P', false, i, ( j - 1 )));
				addClause(temp);
			}
			// ~B(x,y) => ~P(x,y+1)
			if( j < 3 ){
				Clause* temp = new Clause();
				temp->addPreposition(Preposition('B', true, i, j));
				temp->addPreposition(Preposition('P', false, i, ( j + 1 )));
				addClause(temp);
			}
		}

	for( i = 0 ; i < 4 ; ++i )
		for( j = 0 ; j < 4 ; ++j ){
			// ~S(x,y) => ~W(x-1,y)
			if( i > 0 ){
				Clause* temp = new Clause();
				temp->addPreposition(Preposition('S', true, i, j));
				temp->addPreposition(Preposition('W', false, ( i - 1 ), j));
				addClause(temp);
			}
			// ~S(x,y) => ~W(x+1,y)
			if( i < 3 ){
				Clause* temp = new Clause();
				temp->addPreposition(Preposition('S', true, i, j));
				temp->addPreposition(Preposition('W', false, ( i + 1 ), j));
				addClause(temp);
			}
			// ~S(x,y) => ~W(x,y-1)
			if( j > 0 ){
				Clause* temp = new Clause();
				temp->addPreposition(Preposition('S', true, i, j));
				temp->addPreposition(Preposition('W', false, i, ( j - 1 )));
				addClause(temp);
			}
			// ~S(x,y) => ~W(x,y+1)
			if( j < 3 ){
				Clause* temp = new Clause();
				temp->addPreposition(Preposition('S', true, i, j));
				temp->addPreposition(Preposition('W', false, i, ( j + 1 )));
				addClause(temp);
			}

		}

}


void KnowledgeBase::initializeRules(int eSize){
	int i = 0, j = 0;
/*	for( i = 0 ; i < eSize ; ++i )     // V(x,y) => O(x,y)
		for( j = 0 ; j < eSize ; ++j ){
			Clause* temp = new Clause();
			temp->addPreposition(Preposition('V', false, i, j));
			temp->addPreposition(Preposition('O', true, i, j));
			addClause(temp);
		}
*/
	for( i = 0 ; i < eSize ; ++i )     // ~P(x,y) & ~W(x,y) => O(x,y)
		for( j = 0 ; j < eSize ; ++j ){
			Clause* temp = new Clause();
			temp->addPreposition(Preposition('P', true, i, j));
			temp->addPreposition(Preposition('W', true, i, j));
			temp->addPreposition(Preposition('O', true, i, j));
			addClause(temp);
		}

/*********************************/
	for( i = 0 ; i < eSize ; ++i )
		for( j = 0 ; j < eSize ; ++j ){
			// ~B(x,y) => ~P(x-1,y)
			if( i > 0 ){
				Clause* temp = new Clause();
				temp->addPreposition(Preposition('B', true, i, j));
				temp->addPreposition(Preposition('P', false, ( i - 1 ), j));
				addClause(temp);
			}
			// ~B(x,y) => ~P(x+1,y)
			if( i < ( eSize - 1 ) ){
				Clause* temp = new Clause();
				temp->addPreposition(Preposition('B', true, i, j));
				temp->addPreposition(Preposition('P', false, ( i + 1 ), j));
				addClause(temp);
			}
			// ~B(x,y) => ~P(x,y-1)
			if( j > 0 ){
				Clause* temp = new Clause();
				temp->addPreposition(Preposition('B', true, i, j));
				temp->addPreposition(Preposition('P', false, i, ( j - 1 )));
				addClause(temp);
			}
			// ~B(x,y) => ~P(x,y+1)
			if( j < ( eSize - 1 ) ){
				Clause* temp = new Clause();
				temp->addPreposition(Preposition('B', true, i, j));
				temp->addPreposition(Preposition('P', false, i, ( j + 1 )));
				addClause(temp);
			}
            
                }

	for( i = 0 ; i < eSize ; ++i )
		for( j = 0 ; j < eSize ; ++j ){
			// ~S(x,y) => ~W(x-1,y)
			if( i > 0 ){
				Clause* temp = new Clause();
				temp->addPreposition(Preposition('S', true, i, j));
				temp->addPreposition(Preposition('W', false, ( i - 1 ), j));
				addClause(temp);
			}
			// ~S(x,y) => ~W(x+1,y)
			if( i < ( eSize - 1 ) ){
				Clause* temp = new Clause();
				temp->addPreposition(Preposition('S', true, i, j));
				temp->addPreposition(Preposition('W', false, ( i + 1 ), j));
				addClause(temp);
			}
			// ~S(x,y) => ~W(x,y-1)
			if( j > 0 ){
				Clause* temp = new Clause();
				temp->addPreposition(Preposition('S', true, i, j));
				temp->addPreposition(Preposition('W', false, i, ( j - 1 )));
				addClause(temp);
			}
			// ~S(x,y) => ~W(x,y+1)
			if( j < ( eSize - 1 ) ){
				Clause* temp = new Clause();
				temp->addPreposition(Preposition('S', true, i, j));
				temp->addPreposition(Preposition('W', false, i, ( j + 1 )));
				addClause(temp);
			}

              
                }

  
}


bool KnowledgeBase::containClause(Clause *newClause){
	if( numberOfClauses == 0 ) {
	        return false;
	}
        cilk::reducer_opor<bool> result;
        result.set_value(false);
	#pragma omp parallel
	#pragma omp for
	for( int i = 0 ; i < numberOfClauses ; ++i ) {
		if ( clauses[i].isSameClause(newClause) ) result |= true;
	}
	return result.get_value();
}



bool KnowledgeBase::addClause(Clause *newClause){
	if( numberOfClauses == 0 ) {
		clauses = new Clause[1];
		clauses[0] = *newClause;
		recentNumberOfResolutionSteps++;
		numberOfClauses = 1;
		return true;
	}

	Clause* newClauses = new Clause[numberOfClauses + 1]();

	for( int i = 0 ; i < numberOfClauses ; ++i ) {
		if ( clauses[i].isSameClause(newClause) ) return false;
		else newClauses[i] = clauses[i];
	}
	newClauses[ numberOfClauses ] = *newClause;
	delete [] clauses;
	clauses = newClauses;
	recentNumberOfResolutionSteps++;
	++numberOfClauses;
	return true;
}

void KnowledgeBase::resolve(){
	int previousClauseCount = numberOfClauses;
        cilk::reducer_opor<bool> new_clause_added;
        cilk::reducer_list_append<void*> added_clauses;
        new_clause_added.set_value(false);
	#pragma omp parallel
	#pragma omp for
	for ( int i = 0 ; i < (previousClauseCount - 1) ; ++i){
		#pragma omp parallel
		#pragma omp for		
		for ( int j = i ; j < previousClauseCount ; ++j ){
			Clause temp = clauses[i].resolvant(&clauses[j]);
			if ( !temp.isEmpty() && !containClause(&temp) ){
                            Clause* newC = new Clause();
                            temp.cloneClause(newC);
                            added_clauses.push_back((void*)newC);
			    new_clause_added |= true; 
				

			} 
		}


	}
        std::list<void*> r;
        r = added_clauses.get_value();
        while (!r.empty())
            {
                  addClause((Clause*)r.front());
                  r.pop_front();
                        }
        if ( new_clause_added.get_value() ) resolve();
}


void KnowledgeBase::wumpusHasBeenKilled(){
	for( int i = 0 ; i < 4 ; ++i )
		for( int j = 0 ; j < 4 ; ++j ){
			Clause cTemp = Clause(Preposition('W', true, i, j));
			removeClause(&cTemp);
		}
	for( int i = 0 ; i < 4 ; ++i )
		for( int j = 0 ; j < 4 ; ++j ){
			Clause cTemp = Clause(Preposition('S', true, i, j));
			removeClause(&cTemp);
		}
	for( int i = 0 ; i < 4 ; ++i )
		for( int j = 0 ; j < 4 ; ++j ){
			Clause cTemp = Clause(Preposition('W', false, i, j));
			addClause(&cTemp);
		}
	for( int i = 0 ; i < 4 ; ++i )
		for( int j = 0 ; j < 4 ; ++j ){
			Clause cTemp = Clause(Preposition('S', false, i, j));
			addClause(&cTemp);
		}
	wumpusAlive = false;
	resolve();
}


void KnowledgeBase::wumpusHasBeenKilled(int eSize){
	for( int i = 0 ; i < eSize ; ++i )
		for( int j = 0 ; j < eSize ; ++j ){
			Clause cTemp = Clause(Preposition('W', true, i, j));
			removeClause(&cTemp);
		}
	for( int i = 0 ; i < eSize ; ++i )
		for( int j = 0 ; j < eSize ; ++j ){
			Clause cTemp = Clause(Preposition('S', true, i, j));
			removeClause(&cTemp);
		}
	for( int i = 0 ; i < eSize ; ++i )
		for( int j = 0 ; j < eSize ; ++j ){
			Clause cTemp = Clause(Preposition('W', false, i, j));
			addClause(&cTemp);
		}
	for( int i = 0 ; i < eSize ; ++i )
		for( int j = 0 ; j < eSize ; ++j ){
			Clause cTemp = Clause(Preposition('S', false, i, j));
			addClause(&cTemp);
		}
	wumpusAlive = false;
	resolve();
}



void KnowledgeBase::removeClause(Clause* c){
	bool anythingRemoved = false;
	int removedItem = -1;
	for( int i = 0 ; i < numberOfClauses ; ++i )
		if ( clauses[i].isSameClause(c) ){
			removedItem = i;
			anythingRemoved = true;
		}
	if ( anythingRemoved ){
		Clause* temp = new Clause[ numberOfClauses - 1 ];
		int k = 0;
		for ( int i = 0 ; i < numberOfClauses ; ++i ){
		  if ( removedItem == i ) k++;
		  temp[k++] = clauses[i];
		}
		delete [] clauses;
		clauses = temp;
	}
	numberOfClauses--;

}

int KnowledgeBase::getNumberOfFacts(){
        cilk::reducer_opadd<int> count;
        count.set_value(0);
	#pragma omp parallel
	#pragma omp for
        for( int i = 0 ; i < numberOfClauses ; ++i )
		if( clauses[i].isFact() ) ++count;
	return count.get_value();
}

int KnowledgeBase::getNumberOfClauses(){
	return numberOfClauses;
}

bool KnowledgeBase::queryFact(Preposition q){
        cilk::reducer_opor<bool> result;
        result.set_value(false);
	#pragma omp parallel
	#pragma omp for
	for( int i = 0 ; i < numberOfClauses ; ++i )
		if( clauses[i].isFact(&q) ){
		  //printf("Some facts found!\n");
			result |= true;
		}
	return result.get_value();
}
