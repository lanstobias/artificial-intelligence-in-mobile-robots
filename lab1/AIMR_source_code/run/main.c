#include "interface.h"

#define ROBOT_NUMBER 1

void MoveOld(double mm)
{
	int steps;
	steps=mm*7.763372409;
	SetTargetSteps(steps,steps);
	printf("\nMoved %d steps left and %d steps right.\n", GetSteps().l, GetSteps().r);
}

void MoveOld2(double mm)
{
	Steps steps;
	steps.l=MM_PER_PULSE_L/mm;
	steps.r=MM_PER_PULSE_R/mm;
	SetTargetSteps(steps.l,steps.r);
	printf("\nMoved %d steps left and %d steps right.\n", GetSteps().l, GetSteps().r);
}

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
/*
	steps.l=(int)rint(degrees*3.472222222);
	steps.r=(-1)*(int)rint(degrees*3.472222222);
*/
void Turn(double degrees)
{
	Steps steps, steps_mm;
	double arc;

	arc = degrees/360*PI*ROBOT_DIAMETER;

	steps_mm.l = (int)rint(arc);
	steps_mm.r = (int)rint(arc); 
	steps = mm2enc(steps_mm);

	SetTargetSteps(steps.l,-(steps.r));
}

//==============================================================================//
//                                  main                                        //
//==============================================================================//
int main(int argc, char *argv[])
 {

//epuck connection
epuck(ROBOT_NUMBER);


Posture posture;     //get posture; float(x,y,th)

Steps steps;  // get steps: int (l,r)

Steps en_mm;    // steps to mm int (l,r)

Steps mm_en;    // mm to encoder int (l,r)

Speed speed;       // get speed int (l,r)

Speed speed_mm;       // get speed in mm int (l,r)

Sensors ir;       // get sensors vlotage unsigned int (sensor)



double vlin, vrot;
int i;


	printf("Starting...\n");
//Step 5
/*
	int left_stepcount=1000, right_stepcount=1000;
	printf("\nMoving wheels for %d step counts\n", ((left_stepcount+right_stepcount)/2));
	SetTargetSteps(left_stepcount, right_stepcount);

	printf("\nMoving right wheel for 2000 and left wheel for 3000 step counts.\n");
	SetTargetSteps(2000,3000);

*/
//Step 6
/*
	printf("\nSet speed to 100.\n");
	for (int i=0;i<50;i++)
 {
	SetSpeed(300,400);
	Sleep(1);
	Stop();
	}
*/

//Step 7
/*
for (int i=0;i<5;i++)
{
	SetTargetSteps(500,500);
	SetTargetSteps(-500,-500);
}
*/

// Step 8
/*
for (int i=0;i<5;i++)
{
	SetSpeed(500,500);
	Sleep(1000);
	Stop();
	SetSpeed(-500,-500);
	Sleep(1000);
	Stop();
}
*/

// Step 9
/*
Move(128);
*/

// Step 10;
/*
Turn(360);
SetTargetSteps((PI*WHEEL_DIAMETER_L), -(PI*WHEEL_DIAMETER_R));
SetTargetSteps(1250,-1250);
// Example 0 (Note: LEDs only works with real robot)
bool led[8] = {1,1,1,1,1,1,1,1};  // eight leds
SetRingLED (led);
*/

// Step 11
Move(300);
MoveSetSpeed(300);

//SetTargetSteps(1000, 1000);
/*

  //Example 1
  //printf("\n Moving wheels for 1000 step counts\n");
  SetTargetSteps(1000, 1000);
  Sleep(3000);


  //Example 2
  printf("\n Moving wheels for -1000 step counts\n");
  SetTargetSteps(-1000, -1000);
  Sleep(3000);


  //Example 2
  printf("\n Moving wheels for 0 step counts\n");
  SetTargetSteps(0, 0);
  Sleep(3000);


  //Example 2
  printf("\n Moving right wheel for -1000 and left for 1000 step counts\n");
  SetTargetSteps(-1000, 1000);
  Sleep(3000);


  //Example 3
  printf("\n setting speed\n");
  SetSpeed(500, 500);
  Sleep(4000);
  Stop();


  //Example 4
  printf("\n Setting linear and angular velocities\n");
  SetPolarSpeed(50, RAD(-28));
  for (i=0;i<10;i++)
  {
    Sleep(100);
    GetPolarSpeed(&vlin, &vrot);
    printf("(vlin,vrot)=(%.2lf,%.2lf)\n", vlin, DEG(vrot));
  }
  Stop();


  //Example 5
  for (i=0;i<1000;i++)
  {
    ir = GetIR();
    printf("IR values: %4d, %4d, %4d, %4d, %4d, %4d, %4d, %4d\n",
           ir.sensor[0], ir.sensor[1], ir.sensor[2], ir.sensor[3],
           ir.sensor[4], ir.sensor[5], ir.sensor[6], ir.sensor[7]);
  }
*/

  printf("Done...\n");


return (0);
 }

//==============================================================================//
//                                  end main                                    //
//==============================================================================//










