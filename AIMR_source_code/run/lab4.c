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
#define NoDanger 1500       // if reading above this, no danger 
#define FullDanger 650      // if reading below this, max danger

// Macros
#define VMAX 250
#define VMIN 0
#define RMAX 3
#define RMIN 0

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

    if (vrot[RIGHT] > 0.0)
    {
        return (double)(-RMIN - response * (RMAX - RMIN));
    }

    return (double)(RMIN + response * (RMAX - RMIN));
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

//==========================================================================//
//                        AvoidRules function                           //
//==========================================================================//
/*void AvoidRules() { 

    unsigned int ir[8]; 
    FPred Obs_Left, Obs_Right, Obs_Ahead;

    // First, read values of ir[0] ... ir[7] from the robot //
    Sensors ir = GetSensors();

    // Second, compute truth of predicates // 
    ObsLeft = RampDown(max(ir[5],ir[6]), FullDanger, NoDanger); 
    ObsRight = RampDown(max(ir[1],ir[2]), FullDanger, NoDanger); 
    ObsAhead = RampDown(max(ir[0],ir[7]), FullDanger, NoDanger);

    // Third, the fuzzy rules //
    RULESET;
        IF (AND(Obs_Left, NOT(Obs_Right))) ROT(RIGHT); 
        IF (AND(Obs_Right, NOT(Obs_Left))) ROT(LEFT); 
        IF (AND(Obs_Right, Obs_Left)) ROT(AHEAD);

        IF (Obs_Ahead) VEL(BACK); 
        IF (AND(OR(Obs_Right, Obs_Left)), NOT(Obs_Ahead)) VEL(SLOW); 
        IF (NOT(OR(OR(Obs_Right,Obs_Left), Obs_Ahead))) VEL(FAST);
    RULEND;
};*/

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

        // Run the behaviour
        GoToRules(xt, yt);

        // run the AvoidObstacles
        // AvoidRules();

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

	float xt = -120.0;
	float yt = -240;

    GoTo_FRB(xt, yt); 
}
