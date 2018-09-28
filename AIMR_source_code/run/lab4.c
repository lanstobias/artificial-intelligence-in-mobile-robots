#include "interface.h"
#include "fuzzy.h"
#include "lab1.h"
#include "lab2.h"
#include "lab3.h"
#include "lab4.h"

// Globals
float dx,dy;
double delta_position, err_th, err_pos;
Velocity dummyVelocity;

FPred Pos_Left, Pos_Right, Pos_Ahead, Pos_Here, ante;
FSet vlin, vrot;
double vel, rot;
int final_speed, final_rotation_speed;

// Macros
#define VMAX 250
#define VMIN 10
#define RMAX 8
#define RMIN 2

#define AND(x,y) (((x) < (y)) ? (x) : (y))
#define OR(x,y) (((x) < (y)) ? (y) : (x))
#define NOT(x) (1.0 - (x))

#define IF(form) (ante = (form))

#define VEL(y) if (vlin[(y)] < ante) vlin[(y)] = ante
#define ROT(y) if (vrot[(y)] < ante) vrot[(y)] = ante

//==========================================================================//
//                       GoToRules funciton (Fuzzy Logic)                   //
//==========================================================================//
void GoToRules(float xt, float yt)
{ 
    // First part: computation of the needed variables
    compute_difference_to_target_point(xt, yt, &dx, &dy);

    printf("err_th: %lf err_pos: %lf\n", err_th, err_pos);

    err_pos = calculate_epos(dx,dy);	
    //err_th = calculate_Eth(dx,dy);
    err_th = ( (calculate_Eth(dx, dy) * 180.0) / M_PI );

    // Second part: truth computation of the predicates 
    Pos_Left = RampUp(err_th, 0, 60);
    Pos_Right = RampDown(err_th, -60, 0);
    Pos_Ahead = MIN( RampUp(err_th, -30, 0), RampDown(err_th, 0, 30) );
    Pos_Here = RampDown(err_pos, 10, 50);

    // Third part: the rules
    RULESET;
        IF (AND(Pos_Left, NOT(Pos_Here))); ROT(LEFT); 
        IF (AND(Pos_Right, NOT(Pos_Here))); ROT(RIGHT); 
        IF (OR(Pos_Here, Pos_Ahead)); ROT(AHEAD);
        
        IF(AND(Pos_Ahead, NOT(Pos_Here))); VEL(FAST); 
        IF(OR(Pos_Here, NOT(Pos_Ahead))); VEL(NONE);
    RULEEND;
};

//==========================================================================//
//                      Helper function (velocity)                          //
//==========================================================================//
int ResponseToVel(float response)
{
    return (int)(VMIN + response * (VMAX - VMIN));
}

int ResponseToRot(float response)
{
    if (vrot[RIGHT] > 0)
    {
        return (int)(-RMIN - response * (RMAX - RMIN));
    }
    return (int)(RMIN + response * (RMAX - RMIN));
}

void printSets()
{
    printf("vlin[BACK]: %lf\n", vlin[BACK]);
    printf("vlin[NONE]: %lf\n", vlin[NONE]);
    printf("vlin[SLOW]: %lf\n", vlin[SLOW]);
    printf("vlin[FAST]: %lf\n", vlin[FAST]);

    printf("\n");
    printf("vrot[LEFT]: %lf\n", vrot[LEFT]);
    printf("vrot[AHEAD]: %lf\n", vrot[AHEAD]);
    printf("vrot[RIGHT]: %lf\n", vrot[RIGHT]);
    printf("\n---------------------------\n");
}

//==========================================================================//
//                      GoTo_FRB (Fuzzy Rule based)                         //
//==========================================================================//
void GoTo_FRB(float xt, float yt)
{ 
    while (1)
    {
        // Compute current position
        update_position();

        // Reset Fuzzy sets
        ClearFSet(vlin);
        ClearFSet(vrot);

        // Run the behaviour
        GoToRules(xt, yt);

        // Defuzzify and set rot/vel
        DeFuzzify(vrot, 3, &rot);
        DeFuzzify(vlin, 4, &vel);

        final_speed = ResponseToVel(vel); 
        final_rotation_speed = ResponseToRot(rot); 

        printf("vel: %d, rot: %d\n\n", vel, rot);
        printf("final_speed: %d, final_rotation_speed: %d\n", final_speed, final_rotation_speed);
        printSets();
        
        // Send commands to robot
        SetPolarSpeed(final_speed, final_rotation_speed);
        //SetPolarSpeed(-200, 0);
        Sleep(200);

        if (fabsf(err_pos) <= delta_position)
		{
			update_position();
			break;
		}      
    }

    Stop();
	update_position();
    printf("Goal has been reached.\n\n");
	
	//Compute distances to goal (dx, dy)
	compute_difference_to_target_point(xt, yt, &dx, &dy);

	//Convert (dx,dy) to errors (Eth, Epos)
	err_pos = calculate_epos(dx,dy);
	err_th = calculate_Eth(dx,dy);
	printf("---- Final values after goal has been reached: ---->\n");
}

//==========================================================================//
//                        AvoidObstacles function                           //
//==========================================================================//
/*
void AvoidObstacles () { 
    unsigned int ir[8]; 
    FPred Obs_Left, Obs_Right, Obs_Ahead;

    // First, read values of ir[0] ... ir[7] from the robot //
    Sensors ir = GetSensors();
    // Second, compute truth of predicates // 
    if (ir[0] > DangerThreshold || ir[1] > DangerThreshold) Obs_Left = 1.0; 
    else Obs_Left = 0.0;

    if (ir[2] > DangerThreshold || ir[3] > DangerThreshold) Obs_Ahead = 1.0; 
    else Obs_Ahead = 0.0;

    if (ir[4] > DangerThreshold || ir[5] > DangerThreshold) Obs_Right = 1.0; 
    else Obs_Right = 0.0;

    // Third, here come the rules // 
    RULESET;
    IF (AND(Obs_Left, NOT(Obs_Right))); ROT(RIGHT); 
    IF (AND(Obs_Right, NOT(Obs_Left))); ROT(LEFT); 
    IF (AND(Obs_Right, Obs_Left)); ROT(AHEAD);
    IF (Obs_Ahead); VEL(BACK); 
    IF (AND(OR(Obs_Right, Obs_Left)), NOT(Obs_Ahead)); VEL(SLOW); 
    IF (NOT(OR(OR(Obs_Right,Obs_Left), Obs_Ahead))); VEL(FAST);
    RULEND; 
};
*/


//==========================================================================//
//                                lab4                                      //
//==========================================================================//
void lab4()
{
    ClearSteps();
	printf("Test lab4\n\n");

    dummyVelocity.l = -1;
    dummyVelocity.r = -1;

	delta_position = 30.0;
	//delta_th = (float)(PI / 24);

	float xt = -200;
	float yt = 0;

    GoTo_FRB(xt, yt); 
}
