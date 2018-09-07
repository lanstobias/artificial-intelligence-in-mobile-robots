#include "interface.h"

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

void lab1()
{
	printf("Starting lab 1\n");

	Turn(360);
}
