#include "interface.h"
#include "example.c"
#include "lab1.h"
#include "lab2.h"

#define ROBOT_NUMBER 1


//==============================================================================//
//                                  main                                        //
//==============================================================================//
int main(int argc, char *argv[])
{
	// epuck connection
	epuck(ROBOT_NUMBER);

	// Run program
	printf("Starting...\n");

	//example();
	//lab1();
	lab2();

	printf("Done...\n");

	return (0);
 }

//==============================================================================//
//                                  end main                                    //
//==============================================================================//
