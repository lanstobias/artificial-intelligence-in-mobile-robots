#include <stdio.h>
#include "interface.h"
#include "Queue.h"
#include "fuzzy.h"
#include "lab2.h"
#include "lab3.h"
#include "lab4.h"
#include "lab5.h"
#include "lab6.h"
#include "final_challenge.h"

// Macros
#define NO_DANGER 120
#define FULL_DANGER 300

#define NO_DANGER_FRONT 120
#define FULL_DANGER_FRONT 300

#define NO_DANGER_SIDES 120
#define FULL_DANGER_SIDES 300

#define GOAL_THRESHOLD 0.85

#define VELMAX 160
#define VELMIN -80
#define ROTMAX 20
#define ROTMIN -20

#define AND(x,y) (((x) < (y)) ? (x) : (y))  // min
#define OR(x,y) (((x) < (y)) ? (y) : (x))   // max
#define NOT(x) (1.0 - (x))

#define IF(form) (ante = (form))

#define VEL(y) if (vlin[(y)] < ante) vlin[(y)] = ante
#define ROT(y) if (vrot[(y)] < ante) vrot[(y)] = ante

// Lab 5 macros
#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"

#define NUM_NEIGHBOR_CELLS 4
#define LARGE_INT 1000000

// Global variables
static int custom_map_info[16][16] = {
    {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
    {-1, -2, -2, -2, -2, -2, -2, -2, -3, -2, -2, -2, -2, -2, -2, -1},
    {-1, -2, -2, -2, -2, -2, -2, -2, -3, -2, -2, -2, -2, -2, -2, -1},
    {-1, -2, -2, -2, -2, -2, -2, -2, -3, -2, -2, -2, -2, -2, -2, -1},
    {-1, -2, -2, -2, -2, -2, -2, -2, -3, -2, -2, -2, -2, -2, -2, -1},
    {-1, -2, -2, -2, -2, -2, -2, -2, -3, -2, -2, -2, -2, -2, -2, -1},
    {-1, -2, -2, -2, -2, -2, -3, -3, -3, -2, -2, -2, -2, -2, -2, -1},
    {-1, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -1},
    {-1, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -1},
    {-1, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -1},
    {-1, -3, -3, -3, -3, -3, -3, -2, -2, -2, -2, -2, -2, -2, -2, -1},
    {-1, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -1},
    {-1, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -1},
    {-1, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -1},
    {-1, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -1},
    {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}
};

static int empty_map_info[16][16] = {
    {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
    {-1, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -1},
    {-1, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -1},
    {-1, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -1},
    {-1, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -1},
    {-1, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -1},
    {-1, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -1},
    {-1, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -1},
    {-1, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -1},
    {-1, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -1},
    {-1, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -1},
    {-1, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -1},
    {-1, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -1},
    {-1, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -1},
    {-1, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -1},
    {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}
};

static float dx, dy;
static double err_th, err_pos;
FPred ante;
static FSet vlin, vrot;
static double vel, rot;
FPred Pos_Left, Pos_Right, Pos_Ahead, Pos_Here;
FPred Obs_Left, Obs_Right, Obs_Ahead, Obs_Diagonal_Left, Obs_Diagonal_Right, Obs_Ahead_Right, Obs_Ahead_Left;
double final_speed, final_rotation_speed;

void goto_and_avoid_monolithic_rules(float xt, float yt)
{
    compute_difference_to_target_point(xt, yt, &dx, &dy);

    // Controller input
    err_pos = calculate_epos(dx,dy);	
    err_th = ( (calculate_Eth(dx, dy) * 180.0) / M_PI );
    Sensors ir = GetIR();
    //print_ir_values(ir);

    //printf("ante: %lf\n", ante);
    printf("err_th: %lf err_pos: %lf\n", err_th, err_pos);
    Posture posture = GetPosture();
    printf("x: %lf, y: %lf, th: %lf\n", posture.x, posture.y, posture.th);
    printf("---------------------------\n\n");
    
    // Compute fuzzy predicates
    Obs_Left = RampUp(MAX(ir.sensor[5], ir.sensor[6]), NO_DANGER_SIDES, FULL_DANGER_SIDES); 
    Obs_Right = RampUp(MAX(ir.sensor[1], ir.sensor[2]), NO_DANGER_SIDES, FULL_DANGER_SIDES); 
    Obs_Ahead = RampUp(MAX(ir.sensor[0], ir.sensor[7]), NO_DANGER_FRONT, FULL_DANGER_FRONT);

    Pos_Left = RampUp(err_th, 0, 60);
    Pos_Right = RampDown(err_th, -60, 0);
    Pos_Ahead = MIN( RampUp(err_th, -30, 0), RampDown(err_th, 0, 30) );
    Pos_Here = RampDown(err_pos, 10, 50);

    // Fuzzy rules
    RULESET;
        // Rotate towards the goal unless an obstacle is in the way and unless
        // we are at the goal position.
        IF (AND(AND(Pos_Left, NOT(Obs_Left)), NOT(Pos_Here))); ROT(LEFT);
        IF (AND(AND(Pos_Right, NOT(Obs_Right)), NOT(Pos_Here))); ROT(RIGHT);
        IF (OR(AND(Pos_Ahead, NOT(Obs_Ahead)), Pos_Here)); ROT(AHEAD);

        // If goal is to the side but there is an obstacle there and not 
        // an obstacle ahead, move forward.
        IF (AND(Pos_Left, AND(Obs_Left, NOT(Obs_Ahead)))); ROT(AHEAD);
        IF (AND(Pos_Right, AND(Obs_Right, NOT(Obs_Ahead)))); ROT(AHEAD);
        
        // If goal is ahead of you but there is an obstacle ahead of you
        // and not to the sides, turn to either side.
        IF (AND(Pos_Ahead, AND(Obs_Ahead, NOT(Obs_Left)))); ROT(LEFT);
        IF (AND(Pos_Ahead, AND(Obs_Ahead, NOT(Obs_Right)))); ROT(RIGHT);

        IF (AND(Obs_Ahead, AND(Obs_Left, Obs_Right))); ROT(RIGHT);
        
        // Goto vel.
        IF (OR(Pos_Here, NOT(Pos_Ahead))); VEL(NONE);

        // Avoid vev.
        IF (Obs_Ahead); VEL(NONE);
        IF (AND(OR(Obs_Right, Obs_Left), NOT(Obs_Ahead))); VEL(SLOW);
        IF (AND(NOT(OR(OR(Obs_Right, Obs_Left), Obs_Ahead)), AND(Pos_Ahead, NOT(Pos_Here)) )); VEL(FAST);
    RULEEND;
}

void print_Sets()
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

void go_to(float xt, float yt)
{
    do {
        update_position();

        // Reset Fuzzy sets
        ClearFSet(vlin);
        ClearFSet(vrot);

        goto_and_avoid_monolithic_rules(xt, yt);

        // Defuzzify and set rot/vel
        DeFuzzify(vrot, 3, &rot);
        DeFuzzify(vlin, 4, &vel);

        final_speed = ResponseToVel(vel); 
        final_rotation_speed = ResponseToRot(rot); 
        /*
        printf("vel: %lf, rot: %lf\n\n", vel, rot);
        printf("final_speed: %lf, final_rotation_speed: %lf\n", final_speed, final_rotation_speed);
        print_Sets();
        */
        // Send commands to robot
        SetPolarSpeed(final_speed, final_rotation_speed);

        Sleep(200);
    }
    while (!(Pos_Here >= GOAL_THRESHOLD));

    Stop();
	update_position();
}

void run_trajectory(float* xarray, float* yarray, int n)
{
    for (int i = 0; i < n; i++)
	{
		go_to(xarray[i], yarray[i]);
	}
}

void run(Cell start_cell, Cell goal_cell, Map_custom map)
{
    Queue path, search_queue;
    queue_init(&path);
    queue_init(&search_queue);

    place_start_and_end_on_map(&map, start_cell, goal_cell);
    path = Plan(&map, &search_queue, start_cell, goal_cell);

    Track_arrays track = convert_path_to_robot_track(path);
    printPrettyMap(map, path);
    run_trajectory(track.xarray, track.yarray, track.size);
}

void final_challenge()
{
    printf("Final challenge!\n");

    //Run with start, goal and map
    Cell start_cell, goal_cell;
    Map_custom map;

    start_cell.i = 4; start_cell.j = 4;
    goal_cell.i = 12; goal_cell.j = 12;
    map.map = empty_map_info;

    run(start_cell, goal_cell, map);
}
