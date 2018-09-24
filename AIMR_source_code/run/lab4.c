#include "interface.h"
#include "lab1.h"
#include "lab2.h"
#include "lab3.h"
#include "lab4.h"

// Rotational velocity rules: IF(stimulus); ROT(value)
enum dir{LEFT = -1, RIGHT = 1, AHEAD = 0};
enum vel{BACK = -1, NONE = 0, SLOW = 1, FAST = 2};

// vlin, vrot & ante
float vlin, vrot, ante, delta_pos, err_th, err_pos;

// ‘IF’ is a macro that sets the value of ‘ante’ 
#define IF(form) (ante = (form))

// ‘NOT’, ‘AND’ and ‘OR’ are macros that compute truth values
#define NOT(form) (1.0 - (form)) 
#define AND(f1, f2) ((f1) * (f2)) 
#define OR(f1, f2) ((f1) + (f2) - ((f1) * (f2)))

// ‘IF’ is a macro that sets the value of ‘ante’ 
#define IF(form) (ante = (form))

// ‘VEL’ and ‘ROT’ are macros that set the value of ‘vlin’ and ‘vrot’
#define VEL(value) (if (ante > 0.0) vlin = value) 
#define ROT(value) (if (ante > 0.0) vrot = value)

// ‘RULESET’ is a macro that initializes all values 
#define RULESET vlin = NONE; vrot = AHEAD

// Define a danger threshold
//#define DangerThreshold 0


//==========================================================================//
//                       GoToRules funciton (Fuzzy Logic)                        //
//==========================================================================//
void GoToRules (float xt, float yt) { 
 
    float dx,dy;
    FPred Pos_Left, Pos_Right, Pos_Ahead, Pos_Here;

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
    if (err_th > 10) Pos_Left = 1.0; 
    else Pos_Left = 0.0;

    if (err_th < -10) Pos_Right = 1.0; 
    else Pos_Right = 0.0;

    if (ABS(err_th) <= 10) Pos_Ahead = 1.0; 
    else Pos_Ahead = 0.0;

    if (err_pos < 20) Pos_Here = 1.0; 
    else Pos_Here = 0.0;


    // Third part: the rules //
    RULESET;
        IF (AND(Pos_Left, NOT(Pos_Here))); ROT(LEFT); 
        IF (AND(Pos_Right, NOT(Pos_Here))); ROT(RIGHT); 
        IF (OR(Pos_Here, Pos_Ahead)); ROT(AHEAD);
        
        IF(AND(Pos_Ahead, NOT(Pos_Here))); VEL(FAST); 
        IF(OR(Pos_Here, NOT(Pos_Ahead))); VEL(NONE);
    RULEND;
};

//==========================================================================//
//                      Helper function (velocity)                          //
//==========================================================================//
reaction_to_vel (int vel) { switch (vel) { 
    case BACK: return(-20); break; 
    case NONE: return(0); break; 
    case SLOW: return(20); break; 
    case FAST: return(50); break; } 
}

//==========================================================================//
//                      Helper function (rotation)                          //
//==========================================================================//
reaction_to_rot (int vel) { switch (vel) { 
    case BACK: return(-20); break; 
    case NONE: return(0); break; 
    case SLOW: return(20); break; 
    case FAST: return(50); break; } 
}

//==========================================================================//
//                      GoTo_FRB (Fuzzy Rule based)                         //
//==========================================================================//
void GoTo_FRB (float xt, float yt) { 

    while (1) {
        update_position();
        GoToRules(xt, yt);

        vlin = reaction_to_vel(vel); 
        vrot = reaction_to_rot(rot); 
        
        SetPolarSpeed(vlin, vrot);
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

	delta_pos = 30.0;
	//delta_th = (float)(PI / 24);

	float xt = 240;
	float yt = 0;

    GoTo_FRB(xt, yt); 
}