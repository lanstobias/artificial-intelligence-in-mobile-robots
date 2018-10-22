#include "interface.h"
#include "example.c"
#include "lab1.h"
#include "lab2.h"
#include "lab3.h"
#include "lab4.h"
//#include "lab5.h"
#include "lab6.h"
#include "final_challenge.h"

#define ROBOT_NUMBER "2"

//==============================================================================//
//                                  main                                        //
//==============================================================================//
int main(int argc, char *argv[])
{
	// epuck connection
	//epuck(ROBOT_NUMBER);
	//ClearSteps();
	//SetPosture(0,0,0);
	// Run program
	printf("Starting...\n");

	//example();
	//lab1();
	//lab2();
	//lab3();
	//lab4();
    //lab5();
	//lab6();
	final_challenge();

	printf("\nDone...\n");

	return (0);
 }

//==============================================================================//
//                                  end main                                    //
//==============================================================================//
