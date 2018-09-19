#include "interface.h"
#include "example.c"
#include "lab1.h"
#include "lab2.h"
#include "lab3.h"

#define ROBOT_NUMBER "12"


//==============================================================================//
//                                  main                                        //
//==============================================================================//
int main(int argc, char *argv[])
{
	// epuck connection
	epuck(ROBOT_NUMBER);
	ClearSteps();
	SetPosture(0,0,0);
	// Run program
	printf("Starting...\n");
	/*
	printf("Initial position:\n");
	update_position();
	print_position();
	
	printf("Moving 120 mm forward...\n");
	Move(120);
	
	update_position();
	print_position();
	
	printf("Turning 90 degrees...\n");
	Turn(90);
	
	update_position();
	print_position();
	
	printf("Moving 120 mm forward...\n");
	Move(120);
	
	update_position();
	print_position();
	 
	update_position();
	print_position();
	printf("Moving 120 mm with movesetspeed");
	MoveSetSpeed(120);
	update_position();
	print_position();
	* */
	
	//example();
	//lab1();
	//lab2();
	lab3();

	printf("Done...\n");

	return (0);
 }

//==============================================================================//
//                                  end main                                    //
//==============================================================================//
