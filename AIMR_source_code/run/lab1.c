#include "interface.h"
#include "lab1.h"

//==============================================================================//
//                            lab 1 - task 9                                    //
//==============================================================================//
void Move(double mm)
{
	ClearSteps();

	// convert mm to steps
	Steps steps, steps_mm;
	steps_mm.l=mm;
	steps_mm.r=mm;
	steps = mm2enc(steps_mm);
	printf("steps: %d\n", steps.l);

	SetTargetSteps(steps.l,steps.r);

	printf("\nMoved %d steps left and %d steps right.\n", GetSteps().l, GetSteps().r);
}

//==============================================================================//
//                             lab 1 - task 10                                  //
//==============================================================================//
void Turn(double degrees)
{
	ClearSteps();

	// calculate arc of circle in mm
	double arc;
	arc = degrees/360*PI*ROBOT_DIAMETER;

	// convert mm to steps
	Steps steps, steps_mm;
	steps_mm.l = (int)rint(arc);
	steps_mm.r = (int)rint(arc);
	steps = mm2enc(steps_mm);

	// rotate around its own axis
	SetTargetSteps(-(steps.l), steps.r);
}

//==============================================================================//
//                         lab 1 - task 11 (Optional)                           //
//==============================================================================//
void MoveSetSpeed(double mm)
{
	ClearSteps();

	// convert mm to steps
	Steps steps, steps_mm;
	steps_mm.l = mm;
	steps_mm.r = mm;
	steps = mm2enc(steps_mm);
	printf("steps: %d\n", steps.l);

	// run until amount of steps are reched
	SetSpeed(500, 500);
	while(steps.l >= abs(GetSteps().l))
	{
		printf("Current step value: %d\n", GetSteps().l);
	}

	printf("Current step value: %d\n", GetSteps().l);
	Stop();
}

//==============================================================================//
//                         lab 1 - task 12 (Optional)                           //
//==============================================================================//
void MoveSetSpeedSleep(double mm)
{
	ClearSteps();

	// convert mm to steps
	Steps steps, steps_mm;
	steps_mm.l = mm;
	steps_mm.r = mm;
	steps = mm2enc(steps_mm);

	// run & wait
	SetSpeed_mm(100, 100);
	long wait = (steps.l/100) * 1000; // calculation is not correct
	Sleep(wait);

	Stop();
}

//==============================================================================//
//                         lab 1 - task 13 (Optional)                           //
//==============================================================================//
void Airbag()
{
	SetSpeed(130, 130);

	Sensors sensors;
	int b = 0;

	while(b < 1) {
		Sleep(1000);

		// read sensor data and check activity
		sensors = GetIR();
		for(int i=0; i<8; i++) {
			printf("Sensor %d: %d | ", i+1, sensors.sensor[i]);
			if (sensors.sensor[i] > 800) {
				b = 1;
				break;
			}
		}
		printf("\n");
	}

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

	// equal stepcounts
	int left_stepcount=500, right_stepcount=500;
	printf("\nMoving wheels for %d step counts\n", ((left_stepcount+right_stepcount)/2));
	SetTargetSteps(left_stepcount, right_stepcount);
	// unequal stepcounts
	printf("\nMoving right wheel for 700 and left wheel for 800 step counts.\n");
	ClearSteps();
	SetTargetSteps(700,800);


	// Step 6
	//============================================================================//

	ClearSteps();

	printf("\nSet speed to 300 on the right wheel and 400 on the left.\n");
	for (int i=0;i<4;i++) {
		SetSpeed(300,400);
		Sleep(500);
		Stop();
		Sleep(500);
	}
	Stop();

	// Setspeed makes then car go forward at a set speed.
	// Sleep delays the execution of the program for a set amount of milliseconds.
  // Stop sets speed to 0,0.
	// If speed is set to different values, the wheels go at different speeds making the robot turn.
  // It does turn, but it takes a while to notice, and the space is not sufficent.


	// Step 7
	//============================================================================//
	//ClearSteps();

	printf("\nMake the robot go back and forth 5 times between two points (1st Option).\n");
	for (int i=0;i<5;i++)
	{

		SetTargetSteps(250,250);
		SetTargetSteps(-250,-250);
	}


	// The main difference between settargetsteps and setspeed is the acceleration.


	// Step 8
	//============================================================================//
	ClearSteps();

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


	// Step 9
	//============================================================================//
	ClearSteps();

	// see function Move
	printf("\nMove the robot a specific number of mm.\n");
	Move(128);


	// Step 10
	//============================================================================//
	ClearSteps();

	// see function Turn
	printf("\nTurn the robot the given number of degrees.\n");
	Turn(360);


	// Step 11 (Optional)
	//============================================================================//
	ClearSteps();
	// see function MoveSetSpeed
	printf("\nMove the robot with SetSpeed() and GetSteps().\n");
	Move(300);
	MoveSetSpeed(300);
	// The original Move funciton is more accurate than the one based on GetSteps() funciton.


	// Step 12 (Optional)
	//============================================================================//
	ClearSteps();
	// see function MoveSetSpeedSleep
	printf("\nMove the robot SetSpeed() and Sleep().\n");
	MoveSetSpeedSleep(100);
	// Pretty worse due to the Polling rates. Also Speed is steps per seconds.
	// There will be rounding errors.


	// Step 13 (Optional)
	//============================================================================//
	ClearSteps();
	// see function Airbag
	printf("\nRun a Sensorprogram on the robot.\n");
	Airbag();
	// Never crash into a wall again!
	// Sensor activity > 800 leads to an immediate Stop.


}
