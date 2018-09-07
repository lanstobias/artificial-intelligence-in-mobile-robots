#include "interface.h"

//==============================================================================//
//                            lab 1 - task 9                                    //
//==============================================================================//
void Move(double mm)
{
	ClearSteps();

	Steps steps, steps_mm;
	steps_mm.l=mm;
	steps_mm.r=mm;
	steps = mm2enc(steps_mm);
	printf("steps.l: %d\n", steps.l);

	SetTargetSteps(steps.l,steps.r);

	printf("\nMoved %d steps left and %d steps right.\n", GetSteps().l, GetSteps().r);
}

//==============================================================================//
//                             lab 1 - task 10                                  //
//==============================================================================//
void Turn(double degrees)
{
	ClearSteps();

	Steps steps, steps_mm;
	double arc;

	arc = degrees/360*PI*ROBOT_DIAMETER;

	steps_mm.l = (int)rint(arc);
	steps_mm.r = (int)rint(arc);
	steps = mm2enc(steps_mm);

	SetTargetSteps(steps.l, -(steps.r));
}

//==============================================================================//
//                         lab 1 - task 11 (Optional)                           //
//==============================================================================//
void MoveSetSpeed(double mm)
{
	ClearSteps();

	Steps steps, steps_mm;
	steps_mm.l = mm;
	steps_mm.r = mm;
	steps = mm2enc(steps_mm);
	printf("steps.l: %d\n", steps.l);


	SetSpeed(500, 500);
	while(steps.l >= abs(GetSteps().l))

	{
		printf("Current step value: %d\n", GetSteps().l);
	}

	printf("Current step value: %d\n", GetSteps().l);
	Stop();
}

//==============================================================================//
//                                  lab 1                                       //
//==============================================================================//
void lab1()
{
	printf("Starting labb 1 ...\n");

	// Step 1-4
	//============================================================================//
	// no code required

	// Step 5
	//============================================================================//
	/*
	// equal stepcounts
	int left_stepcount=1000, right_stepcount=1000;
	printf("\nMoving wheels for %d step counts\n", ((left_stepcount+right_stepcount)/2));
	SetTargetSteps(left_stepcount, right_stepcount);
	// unequal stepcounts
	printf("\nMoving right wheel for 2000 and left wheel for 3000 step counts.\n");
	SetTargetSteps(2000,3000);
	*/

	// Step 6
	//============================================================================//
	/*
	printf("\nSet speed to 300 on the right wheel and 400 on the left.\n");
	for (int i=0;i<50;i++) {
		SetSpeed(300,400);
		Sleep(1);
		Stop();
	}
	// Setspeed makes then car go forward at a set speed.
	// Sleep delays the execution of the program for a set amount of milliseconds.
  // Stop sets speed to 0,0.
	// If speed is set to different values, the wheels go at different speeds making the robot turn.
  // It does turn, but it takes a while to notice, and the space is not sufficent.
	*/

	// Step 7
	//============================================================================//
	/*
	printf("\nMake the robot go back and forth 5 times between two points (1st Option).\n");
	for (int i=0;i<5;i++)
	{
		SetTargetSteps(500,500);
		SetTargetSteps(-500,-500);
	}
	// The main difference between settargetsteps and setspeed is the acceleration.
	*/

	// Step 8
	//============================================================================//
	/*
	printf("\nMake the robot go back and forth 5 times between two points (2nd Option).\n");
	for (int i=0;i<5;i++)
	{
		SetSpeed(500,500);
		Sleep(1000);
		Stop();
		SetSpeed(-500,-500);
		Sleep(1000);
		Stop();
	}
	// The main difference between settargetsteps and setspeed is the acceleration.
	*/

	// Step 9
	//============================================================================//
	/*
	// see function Move
	printf("\nMove the robot a specific number of mm.\n");
	Move(128);
	*/

	// Step 10
	//============================================================================//
	/*
	// see function Turn
	printf("\nTurn the robot the given number of degrees.\n");
	Turn(360);
	*/

	// Step 11 (Optional)
	//============================================================================//
	/*
	// see function MoveSetSpeed
	printf("\nMove the robot with SetSpeed() and GetSteps().\n");
	Move(300);
	MoveSetSpeed(300);
	// The original Move funciton is more accurate than the one based on GetSteps() funciton.
	*/

	// Step 12 (Optional)
	//============================================================================//
	/*
	// see function
	printf("\nMove the robot SetSpeed() and Sleep().\n");

	*/

	// Step 13 (Optional)
	//============================================================================//
	/*
	// see function
	printf("\nRun a Sensorprogram on the robot.\n");

	*/

}
