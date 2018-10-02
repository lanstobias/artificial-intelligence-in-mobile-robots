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
double vel, rot, final_speed, final_rotation_speed;

// DangerThresholdS
//#define NoDanger 306
//#define FullDanger 2211
#define NoDanger 2211
#define FullDanger 306

// Macros
#define VMAX 75
#define VMIN 0
#define RMAX 2
#define RMIN -2

#define AND(x,y) (((x) < (y)) ? (x) : (y))  // min
#define OR(x,y) (((x) < (y)) ? (y) : (x))   // max
#define NOT(x) (1.0 - (x))

#define IF(form) (ante = (form))

#define VEL(y) if (vlin[(y)] < ante) vlin[(y)] = ante
#define ROT(y) if (vrot[(y)] < ante) vrot[(y)] = ante

//==========================================================================//
//                       GoToRules funciton (Fuzzy Logic)                   //
//==========================================================================//
void GoToRules(float xt, float yt)
{ 
    printf("ante: %lf\n", ante);
    printf("err_th: %lf err_pos: %lf\n", err_th, err_pos);

    // First part: computation of the needed variables
    compute_difference_to_target_point(xt, yt, &dx, &dy);

    err_pos = calculate_epos(dx,dy);	
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
double ResponseToVel(float response)
{
    return (double)(VMIN + response * (VMAX - VMIN));
}

//==========================================================================//
//                      Helper function (rotation)                          //
//==========================================================================//
double ResponseToRot(float response)
{
    printf("response: %f\n", response);

    /*
    if (vrot[RIGHT] > 0.001)
    {
        printf("Turn right\n");
        return (double)(-RMIN - response * (RMAX - RMIN));
    }
    else if (vrot[LEFT] > 0.001)
    {
        return (double)(RMIN + response * (RMAX - RMIN));
    }
    */

    double return_value = (double)(RMIN + response * (RMAX - RMIN));

    printf("rmin: %lf, rmax: %lf, return_value: %lf\n", RMIN, RMAX, return_value);

    return return_value;

    //printf("%d + %f * (%d - %d) = %lf\n", RMIN, response, RMAX, RMIN, return_value);
    // printf("Turn left, return_value: %lf\n");
}

//==========================================================================//
//                             Debug function                               //
//==========================================================================//
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
//                      Helper function (goal checker)                          //
//==========================================================================//
int goalReached()
{ 
    return (fabsf(err_pos) <= delta_position);
}

void print_ir_values(Sensors ir)
{ 
    for (int i = 0; i < 8; i++)
    { 
        printf("ir[%d]: %d\n", i, ir.sensor[i]);
    }
}

//==========================================================================//
//                        FollwoRules function                           //
//==========================================================================//
void FollowRules()
{ 
    FPred Obs_Left, Obs_Right, Obs_Ahead;

    // First, read values of ir[0] ... ir[7] from the robot //
    Sensors ir = GetIR();
    print_ir_values(ir);

    // Second, compute truth of predicates // 
    Obs_Left = RampDown(MAX(ir.sensor[5], ir.sensor[6]), FullDanger, NoDanger); 
    Obs_Right = RampDown(MAX(ir.sensor[1], ir.sensor[2]), FullDanger, NoDanger); 
    Obs_Ahead = RampDown(MAX(ir.sensor[0], ir.sensor[7]), FullDanger, NoDanger);

    // Third, the fuzzy rules //
    RULESET;
        IF (AND(Obs_Left, NOT(Obs_Right))); ROT(RIGHT); 
        IF (AND(Obs_Right, NOT(Obs_Left))); ROT(LEFT); 
        IF (AND(Obs_Right, Obs_Left)); ROT(AHEAD);

        IF (Obs_Ahead); VEL(BACK); 
        IF (AND(OR(Obs_Right, Obs_Left), NOT(Obs_Ahead))); VEL(SLOW); 
        IF (NOT(OR(OR(Obs_Right,Obs_Left), Obs_Ahead))); VEL(FAST);
    RULEEND;
};

//==========================================================================//
//                        AvoidRules function                           //
//==========================================================================//
void AvoidRules()
{ 
    FPred Obs_Left, Obs_Right, Obs_Ahead;

    // First, read values of ir[0] ... ir[7] from the robot //
    Sensors ir = GetIR();
    print_ir_values(ir);

    // Second, compute truth of predicates // 
    Obs_Left = RampDown(MAX(ir.sensor[5], ir.sensor[6]), FullDanger, NoDanger); 
    Obs_Right = RampDown(MAX(ir.sensor[1], ir.sensor[2]), FullDanger, NoDanger); 
    // Obs_Ahead = RampDown(MAX(ir.sensor[0], ir.sensor[7]), FullDanger, NoDanger);

    //Obs_Left = RampUp(MAX(ir.sensor[5], ir.sensor[6]), NoDanger, FullDanger); 
    //Obs_Right = RampUp(MAX(ir.sensor[1], ir.sensor[2]), NoDanger, FullDanger); 
    Obs_Ahead = RampUp(MAX(ir.sensor[0], ir.sensor[7]), NoDanger, FullDanger);

    printf("Obs_Left: %lf, Obs_Right: %lf, Obs_Ahead: %lf\n", Obs_Left, Obs_Right, Obs_Ahead);

    // Third, the fuzzy rules //
    RULESET;
        IF (AND(Obs_Left, NOT(Obs_Right))); ROT(RIGHT); 
        IF (AND(Obs_Right, NOT(Obs_Left))); ROT(LEFT); 
        IF (AND(Obs_Right, Obs_Left)); ROT(AHEAD);

        IF (Obs_Ahead); VEL(BACK);
        IF (AND(OR(Obs_Right, Obs_Left), NOT(Obs_Ahead))); VEL(SLOW);
        IF (NOT(OR(OR(Obs_Right,Obs_Left), Obs_Ahead))); VEL(FAST);
    RULEEND;

    printf("ante: %lf\n", ante);
};

//==========================================================================//
//                      GoTo_FRB (Fuzzy Rule based)                         //
//==========================================================================//
void GoTo_FRB(float xt, float yt)
{ 
    do
    {
        // Compute current position
        update_position();

        // Reset Fuzzy sets
        ClearFSet(vlin);
        ClearFSet(vrot);

        // run the AvoidObstacles
        AvoidRules();

        // Run the behaviour
        //GoToRules(xt, yt);

        // Defuzzify and set rot/vel
        printf("rot before DeFuzzify(): %lf\n", rot);
        DeFuzzify(vrot, 3, &rot);
        DeFuzzify(vlin, 4, &vel);
        printf("rot after DeFuzzify(): %lf\n", rot);

        final_speed = ResponseToVel(vel); 
        final_rotation_speed = ResponseToRot(rot); 

        printf("vel: %lf, rot: %lf\n\n", vel, rot);
        printf("final_speed: %lf, final_rotation_speed: %lf\n", final_speed, final_rotation_speed);
        printSets();

        // Send commands to robot
        SetPolarSpeed(final_speed, final_rotation_speed);

        Sleep(200);
    }
    while(!goalReached());

    Stop();
	update_position();
    printf("Goal has been reached.\n\n");
	
	//Compute distances to goal (dx, dy)
	//compute_difference_to_target_point(xt, yt, &dx, &dy);

	//Convert (dx,dy) to errors (Eth, Epos)
	err_pos = calculate_epos(dx,dy);
	err_th = calculate_Eth(dx,dy);

	printf("---- Final values after goal has been reached: ---->\n");
    printf("vel: %lf, rot: %lf\n\n", vel, rot);
    printf("final_speed: %lf, final_rotation_speed: %lf\n", final_speed, final_rotation_speed);

    Posture posture = GetPosture();
    printf("x: %lf, y: %lf, th: %lf\n", posture.x, posture.y, posture.th);
}

void run_AvoidRules()
{ 
    while (1)
    { 
        // Reset Fuzzy sets
        ClearFSet(vlin);
        ClearFSet(vrot);

        AvoidRules();
        printSets();

        DeFuzzify(vrot, 3, &rot);
        DeFuzzify(vlin, 4, &vel);

        final_speed = ResponseToVel(vel); 
        final_rotation_speed = ResponseToRot(rot); 
        printf("final_speed: %lf, final_rotation_speed: %lf\n", final_speed, final_rotation_speed);
        
        SetPolarSpeed(final_speed, final_rotation_speed);

        printf("---------------------------------\n");
        Sleep(200);
    }
}


//==========================================================================//
//                                lab4                                      //
//==========================================================================//
void lab4()
{
    ClearSteps();
    rot = 0;
    vel = 0;
	printf("Test lab4\n\n");
    
	delta_position = 25.0;
	//delta_th = (float)(PI / 24);

	float xt = 360.0;
	float yt = 0.0;

    //GoTo_FRB(xt, yt); 
    run_AvoidRules();
}
