#include "interface.h"
#include "fuzzy.h"
#include "lab1.h"
#include "lab2.h"
#include "lab3.h"
#include "lab4.h"

// Globals
float dx,dy;
double err_th, err_pos;
Velocity dummyVelocity;
FPred Pos_Left, Pos_Right, Pos_Ahead, Pos_Here, ante;
FSet vlin, vrot;
double vel, rot, final_speed, final_rotation_speed;
FILE* fp;

// DangerThresholdS
#define NoDanger 180
#define FullDanger 400

// Macros
#define VELMAX 160
#define VELMIN -80
#define ROTMAX 2
#define ROTMIN -2

#define AND(x,y) (((x) < (y)) ? (x) : (y))  // min
#define OR(x,y) (((x) < (y)) ? (y) : (x))   // max
#define NOT(x) (1.0 - (x))

#define IF(form) (ante = (form))

#define VEL(y) if (vlin[(y)] < ante) vlin[(y)] = ante
#define ROT(y) if (vrot[(y)] < ante) vrot[(y)] = ante

//==========================================================================//
//                       GoToRules function (Fuzzy Logic)                   //
//==========================================================================//
void GoToRules(float xt, float yt)
{ 
    //printf("ante: %lf\n", ante);
    //printf("err_th: %lf err_pos: %lf\n", err_th, err_pos);

    // System output, computation of the needed variables
    compute_difference_to_target_point(xt, yt, &dx, &dy);

    // Controller input
    err_pos = calculate_epos(dx,dy);	
    err_th = ( (calculate_Eth(dx, dy) * 180.0) / M_PI );

    // Evaluate truth of predicates (fuzzification)
    Pos_Left = RampUp(err_th, 0, 60);
    Pos_Right = RampDown(err_th, -60, 0);
    Pos_Ahead = MIN( RampUp(err_th, -30, 0), RampDown(err_th, 0, 30) );
    Pos_Here = RampDown(err_pos, 10, 50);

    // Evaluate the fuzzy rules
    RULESET;
        IF (AND(Pos_Left, NOT(Pos_Here))); ROT(LEFT); 
        IF (AND(Pos_Right, NOT(Pos_Here))); ROT(RIGHT); 
        IF (OR(Pos_Here, Pos_Ahead)); ROT(AHEAD);
        
        //IF (); VEL(SLOW);
        IF (AND(NOT(Pos_Ahead), Pos_Here)); VEL(BACK);
        IF (OR(Pos_Here, NOT(Pos_Ahead))); VEL(NONE);
        IF (AND(Pos_Ahead, NOT(Pos_Here))); VEL(FAST); 
    RULEEND;
};

//==========================================================================//
//                      Response to velocity (GoTo)                         //
//==========================================================================//
double ResponseToVel(double response)
{
    return (double)(VELMIN + response * (VELMAX - VELMIN));
}

//==========================================================================//
//                      ResponseToRot (GoTo)                                //
//==========================================================================//
double ResponseToRot(double response)
{
    return (double)(ROTMAX + response * (ROTMIN - ROTMAX));
}

//==========================================================================//
//                         Debug function printSets                         //
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
//                      Debug function (print IR-values)                    //
//==========================================================================//
void print_ir_values(Sensors ir)
{ 
    for (int i = 0; i < 8; i++)
    { 
        printf("ir[%d]: %d\n", i, ir.sensor[i]);
    }
}

//==========================================================================//
//                        FollwoRules function                              //
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
//                        AvoidRules function                               //
//==========================================================================//
void AvoidRules()
{ 
    FPred Obs_Left, Obs_Right, Obs_Ahead;

    // First, read values of ir[0] ... ir[7] from the robot //
    Sensors ir = GetIR();
    //print_ir_values(ir);

    // Second, compute truth of predicates // 
    Obs_Left = RampUp(MAX(ir.sensor[5], ir.sensor[6]), NoDanger, FullDanger); 
    Obs_Right = RampUp(MAX(ir.sensor[1], ir.sensor[2]), NoDanger, FullDanger); 
    Obs_Ahead = RampUp(MAX(ir.sensor[0], ir.sensor[7]), NoDanger, FullDanger);

    //printf("Obs_Left: %lf, Obs_Right: %lf, Obs_Ahead: %lf\n", Obs_Left, Obs_Right, Obs_Ahead);

    // Third, the fuzzy rules //
    RULESET;
    /*
        IF (OR(AND(Obs_Left, NOT(Obs_Right)), AND(Obs_Ahead, NOT(Obs_Right)))); ROT(RIGHT); 
        IF (OR(AND(Obs_Right, NOT(Obs_Left)), AND(Obs_Ahead, Obs_Right))); ROT(LEFT); 
        IF (AND(NOT(OR(Obs_Right, Obs_Left)), NOT(Obs_Ahead))); ROT(AHEAD);

        IF (OR(OR(Obs_Right, Obs_Left), Obs_Ahead)); VEL(NONE);
        IF (AND(OR(Obs_Right, Obs_Left), NOT(Obs_Ahead))); VEL(SLOW);
        IF (NOT(OR(OR(Obs_Right, Obs_Left), Obs_Ahead))); VEL(FAST);
        */

        IF (AND(Obs_Left, NOT(Obs_Right))); ROT(RIGHT);
        IF (AND(Obs_Right, NOT(Obs_Left))); ROT(LEFT);
        IF (AND(NOT(OR(Obs_Right, Obs_Left)), NOT(Obs_Ahead))); ROT(AHEAD);
        //IF (AND(Obs_Right, Obs_Left)); ROT(AHEAD);

        //IF (Obs_Ahead); VEL(BACK);
        IF (OR(OR(Obs_Right, Obs_Left), Obs_Ahead)); VEL(NONE);
        IF (AND(OR(Obs_Right, Obs_Left), NOT(Obs_Ahead))); VEL(SLOW);
        IF (NOT(OR(OR(Obs_Right, Obs_Left), Obs_Ahead))); VEL(FAST);
    RULEEND;
};

//==========================================================================//
//                      GoTo_FRB (Fuzzy Rule based)                         //
//==========================================================================//
void GoTo_FRB(float xt, float yt, bool avoid)
{
    do {
        // Compute current position
        update_position();
        
        //printCoordinatesToFile(&fp);

        // Reset Fuzzy sets
        ClearFSet(vlin);
        ClearFSet(vrot);

        // Run the behaviour
        if (avoid)
        {
            AvoidRules();
        }
        GoToRules(xt, yt);

        // Defuzzify and set rot/vel
        DeFuzzify(vrot, 3, &rot);
        DeFuzzify(vlin, 4, &vel);

        final_speed = ResponseToVel(vel); 
        final_rotation_speed = ResponseToRot(rot); 

        //printf("vel: %lf, rot: %lf\n\n", vel, rot);
        //printf("final_speed: %lf, final_rotation_speed: %lf\n", final_speed, final_rotation_speed);
        //printSets();

        // Send commands to robot
        SetPolarSpeed(final_speed, final_rotation_speed);

        Sleep(300);
    }
    while (!(Pos_Here >= 0.9));

    Stop();
	update_position();
    //printf("Goal has been reached.\n\n");
	
	//Convert (dx,dy) to errors (Eth, Epos)
	err_pos = calculate_epos(dx,dy);
	err_th = calculate_Eth(dx,dy);

	//printf("---- Final values after goal has been reached: ---->\n");
    //printf("vel: %lf, rot: %lf\n\n", vel, rot);
    //printf("final_speed: %lf, final_rotation_speed: %lf\n", final_speed, final_rotation_speed);

    Posture posture = GetPosture();
    //printf("x: %lf, y: %lf, th: %lf\n", posture.x, posture.y, posture.th);
}

//==========================================================================//
//                      Demo function (Avoid Obsticles)                     //
//==========================================================================//
void run_AvoidRules()
{ 
    int i = 0;
    while (i < 300)
    { 
        update_position();
        //printCoordinatesToFile(&fp);

        // Reset Fuzzy sets
        ClearFSet(vlin);
        ClearFSet(vrot);

        AvoidRules();
        //printSets();

        DeFuzzify(vrot, 3, &rot);
        DeFuzzify(vlin, 4, &vel);

        final_speed = ResponseToVel(vel); 
        final_rotation_speed = ResponseToRot(rot); 
        //printf("final_speed: %lf, final_rotation_speed: %lf\n", final_speed, final_rotation_speed);
        
        SetPolarSpeed(final_speed, final_rotation_speed);

        printf("---------------------------------\n");
        Sleep(100);
        i++;
    }
}

//==========================================================================//
//                      Track Function (Avoid Obsticles)                    //
//==========================================================================//
void FuzzyTrack(float* xarray, float* yarray, int n, bool avoid)
{
	for (int i=0; i< n; i++)
	{
		GoTo_FRB(xarray[i], yarray[i], avoid);
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

	//float xt = -300.0;
	//float yt = 0.0;

    //openFile(&fp);
    //GoTo_FRB(xt, yt); 
    //closeFile(&fp);
    /*
	int n = 4;
	
	float xarray[10] = {240, 240, 0, 0};
	float yarray[10] = {0, 240, 240, 0};
	
	FuzzyTrack(xarray, yarray, n);
    */

    //openFile(&fp);
    run_AvoidRules();
    //closeFile(&fp);
}
