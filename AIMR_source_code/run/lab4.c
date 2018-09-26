#include "interface.h"
#include "lab1.h"
#include "lab2.h"
#include "lab3.h"
#include "lab4.h"

#define VMAX 1000
#define VMIN 75

float dx,dy;
FPred Pos_Left, Pos_Right, Pos_Ahead, Pos_Here;
int vel, rot;

// vlin, vrot & ante
int final_speed, final_rotation_speed;
double delta_position, err_th, err_pos;

FSet vlin, vrot;

// ‘NOT’, ‘AND’ and ‘OR’ are macros that compute truth values
// #define NOT(form) (1.0 - (form)) 
// #define AND(f1, f2) ((f1) * (f2)) 
// #define OR(f1, f2) ((f1) + (f2) - ((f1) * (f2)))

// ‘IF’ is a macro that sets the value of ‘ante’ 
//#define IF(form) (ante = (form))

// ‘VEL’ and ‘ROT’ are macros that set the value of ‘vlin’ and ‘vrot’
//#define VEL(value) (if (ante > 0.0) vlin = value) 
//#define ROT(value) (if (ante > 0.0) vrot = value)

//==========================================================================//
//                       GoToRules funciton (Fuzzy Logic)                   //
//==========================================================================//
void GoToRules (float xt, float yt)
{ 
    // First part: computation of the needed variables // 

    // err_th, err_pos computed as in good old GoTo() //
    /* ----- Paste & Copy lab3 ------ */
        //  Compute distances to goal (dx, dy)
        compute_difference_to_target_point(xt, yt, &dx, &dy);

        // Convert (dx,dy) to errors (Eth, Epos)
        // rename variables to fit frame of lab4
        err_pos = calculate_epos(dx,dy);	
        err_th = calculate_Eth(dx,dy);
    /* ----- Paste & Copy lab3 ------ */

    // Second part: truth computation of the predicates //
    Pos_Left = RampUp(err_th, 0, 60);
    Pos_Right = RampDown(err_th, -60, 0);
    Pos_Ahead = min(RampUp(err_th, -30, 0), RampDown(err_th, 0, 30));
    Pos_Here = RampDown(err_pos, 10, 50);

    // Third part: the rules //
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
// result is a float between 0.0 and 1.0
// convert it to in between VMIN and VMAX
// similar for rot
int ResponseToVel(float response)
{
    return (VMIN + response * (VMAX - VMIN));
}

//==========================================================================//
//                      Helper function (velocity)                          //
//==========================================================================//
int reaction_to_vel (int vel)
{
    switch (vel)
    {
        case BACK: return(-20); break; 
        case NONE: return(0); break; 
        case SLOW: return(20); break; 
        case FAST: return(50); break;
    }
}

//==========================================================================//
//                      Helper function (rotation)                          //
//==========================================================================//
int reaction_to_rot (int rot)
{ 
    switch (rot)
    { 
        case BACK: return(-20); break; 
        case NONE: return(0); break; 
        case SLOW: return(20); break; 
        case FAST: return(50); break;
    }
}

//==========================================================================//
//                      GoTo_FRB (Fuzzy Rule based)                         //
//==========================================================================//
void GoTo_FRB (float xt, float yt) { 
    while (1) {
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
        
        // Send commands to robot
        SetPolarSpeed(final_speed, final_rotation_speed);
        Sleep(200);

        if (fabsf(err_pos) <= delta_pos)
		{
			update_position();
			break;
		}      
    }

    Stop();

    printf("Goal has been reached.\n\n");

	update_position();
	
	//Compute distances to goal (dx, dy)
	compute_difference_to_target_point(xt, yt, &dx, &dy);

	//Convert (dx,dy) to errors (Eth, Epos)
	Epos = calculate_epos(dx,dy);
	Eth = calculate_Eth(dx,dy);
	printf("---- Final values after goal has been reached: ---->\n");
	printRobotValues(velocity, Epos, Eth);
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
void lab4 () {

    ClearSteps();
	printf("Test lab4\n\n");

	delta_position = 30.0;
	//delta_th = (float)(PI / 24);

	float xt = 240;
	float yt = 0;

    // GoTo_FRB(xt, yt); 
}
