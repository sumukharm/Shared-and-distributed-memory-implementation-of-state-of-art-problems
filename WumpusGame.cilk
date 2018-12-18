#include <cilk.h>
#include <cilkview.h>

CILK_BEGIN_CPLUSPLUS_HEADERS
#include <iomanip>
#include <iostream>
#include <stdlib.h>
CILK_END_CPLUSPLUS_HEADERS

#include "Cave.cilk"
#include "Agent.cilk"


int cilk_main( int argc, char* argv[] )
{
	std::cout << "The name used to start the program: " << argv[ 0 ]
	                                                             << "\nArguments are:\n";
	for (int n = 1; n < argc; n++)
		std::cout << std::setw(2) << n << ": " << argv[ n ] << '\n';
	int outputMode = atoi(argv[1]);
	cilk::cilkview cv;
	cv.start();
	if( argc < 3 ){
		Cave myCave(outputMode);
		Agent myAgent(&myCave, 0, 0, 0, outputMode);
	} else {
		Cave myCave(outputMode,argv[2]);
		Agent myAgent(&myCave, 0, 0, 0, outputMode);
	}
	cv.stop();
	cv.dump("Wumpus-result", false);
	std::cout << cv.accumulated_milliseconds() << " miliseconds" << std::endl;
	return 0;
}f
