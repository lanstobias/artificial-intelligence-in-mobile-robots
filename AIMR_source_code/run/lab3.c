#include "interface.h"
#include "lab1.h"
#include "lab2.h"
#include "lab3.h"

// Global variables
float Kp_pos, Kp_th, delta_pos, delta_th;
FILE *filePointer;

//==========================================================================//
//            Helper funciton (difference to target point)                  //
//==========================================================================//
void compute_difference_to_target_point(float xt, float yt, float* dx, float* dy)
{
	Posture robot_temp_posture = GetPosture();

	// Using to pointer to "return" two parameters
	*dx = (xt - robot_temp_posture.x);
	*dy = (yt - robot_temp_posture.y);
}

//==========================================================================//
//                  Helper funciton (calculate Epos)                        //
//==========================================================================//
float calculate_epos(float dx, float dy)
{
	float a2, b2, c;

	// Pythagoras: c = sqrt( a² + b²) || a² + b² = c²
	a2 = powf(dx, 2);
	b2 = powf(dy, 2);
	c= sqrtf(a2 + b2);

	return c;
}

//==========================================================================//
//                    Helper funciton (calculate Eth)                       //
//==========================================================================//
float calculate_Eth(float dx, float dy)
{
    float angle;

    angle = atan2f(dy, dx);
    angle -= GetPosture().th;
    angle = normalizeAngle(angle);

    return angle;
}

//==========================================================================//
//           Helper funciton (calculate velocity based on angle)            //
//==========================================================================//
Velocity calculateVelocity_angle(float Kp_th, float Eth)
{
	Velocity turn;

    turn.r = (Kp_th * Eth);

    //Limit max and min speeds - positive turn values
    if (turn.r> 0 && turn.r>250)
    turn.r=250;
    if (turn.r>0 && turn.r<30)
    turn.r=30;

    //Limit max and min speeds - negative turn values
    if (turn.r<0 && turn.r<-250)
    turn.r=-250;
    
    if (turn.r<0 && turn.r>-30)
    turn.r=-30;


	turn.l = -(turn.r);

	return turn;
}

//==========================================================================//
//           Helper funciton (calculate velocity based on distance)         //
//==========================================================================//
Velocity calculateVelocity_distance(float Kp_pos, float Epos)
{
	Velocity move;

	move.r = (int)((Kp_pos * Epos));

	if (move.r > 1000)
	{
		move.r = 1000;
	}
	if (move.r < 75)
	{
		move.r = 75;
	}

	move.l = move.r;

	return move;
}

//==========================================================================//
//                              GoTo funciton                               //
//==========================================================================//
void GoTo_DaC(float xt, float yt)
{
	Velocity velocity;
	float dx,dy;
	float Eth, Epos;
	while(1)
	{
		//Compute current robot position (xr, yr)
		update_position();
		
		//Compute distances to goal (dx, dy)
		compute_difference_to_target_point(xt, yt, &dx, &dy);

		//Convert (dx,dy) to errors (Eth, Epos)
		Epos = calculate_epos(dx,dy);
		
		Eth = calculate_Eth(dx,dy);
		if (fabsf(Epos) <= delta_pos)
		{
			update_position();
			break;
		}
		printRobotValues(velocity, Epos, Eth);

		if (fabsf(Eth) > delta_th)
		{
			velocity=calculateVelocity_angle(Kp_th, Eth);
		}
		else if (fabsf(Epos) > delta_pos)
		{
			velocity=calculateVelocity_distance(Kp_pos, Epos);
		}
		SetSpeed(velocity.l, velocity.r);
		Sleep(500);

	}

	Stop();
	printf("Goal has been reached.\n\n");

	update_position();
	//Compute current robot position (xr, yr)
		
	//Compute distances to goal (dx, dy)
	compute_difference_to_target_point(xt, yt, &dx, &dy);

	//Convert (dx,dy) to errors (Eth, Epos)
	Epos = calculate_epos(dx,dy);
	Eth = calculate_Eth(dx,dy);
	printf("---- Final values after goal has been reached: ---->\n");
	printRobotValues(velocity, Epos, Eth);
	
}

//==========================================================================//
//           Helper funciton (tracks the robots travelled path)             //
//==========================================================================//
void Track(float* xarray, float* yarray, int n)
{
	for (int i=0; i< n; i++)
	{
		GoTo_DaC(xarray[i], yarray[i]);
	}
	
}
void printRobotValues(Velocity velocity, float Epos, float Eth)
{
    Posture posture = GetPosture();
    printCoordinatesToFile(posture);
    float angle_rad = posture.th;
    float angle_deg = angle_rad * (180.0 / M_PI);
    printf("Robot: x: %.1f, y: %.1f, rad: %.3f, deg: %.4f\n", posture.x, posture.y, angle_rad, angle_deg);
    printf("Velocity: left: %.2f, right: %.2f\n", velocity.l, velocity.r);
    printf("Error(Epos: %.2f, Eth: %.2f)\n", Epos, (Eth*(180.0/M_PI)));
    printf("Threshhold: (Pos: %.2f, th: %.2f)\n", delta_pos, (delta_th*(180.0/M_PI)));
    printf("-----------------------\n");
}



void openFile()
{
	filePointer = fopen("robot_values.csv", "w");
	if (filePointer == NULL)
	{
		printf("Error opening file.\n");
		exit(1);
	}
}

void printCoordinatesToFile(Posture posture)
{
	fprintf(filePointer, "%f,%f\n", posture.x, posture.y);
}

void closeFile()
{
	fclose(filePointer);
}


//==========================================================================//
//                                 lab3                                     //
//==========================================================================//
void lab3()
{	
	openFile();
    ClearSteps();
	printf("Test lab3\n\n");

	Kp_pos = 10;
	Kp_th = 500;
	delta_pos = 30.0;
	delta_th = (float)(PI / 24);

	float xt = -180;
	float yt = 240;
	
	int n = 4;
	
	float xarray[10] = {240, 240, 0, 0};
	float yarray[10] = {0, 240, 240, 0};
	
	Track(xarray, yarray, n);

	closeFile();
}
