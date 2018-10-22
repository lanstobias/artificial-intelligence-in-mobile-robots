#include <stdio.h>
#include "Queue.h"
#include "fuzzy.h"
#include "lab2.h"
#include "lab4.h"
#include "lab5.h"
#include "lab6.h"
#include "final_challenge.h"

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
FPred Obs_Left, Obs_Right, Obs_Ahead;
double final_speed, final_rotation_speed;

void goto_and_avoid_monolithic_rules(float xt, float yt)
{

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

        // Send commands to robot
        SetPolarSpeed(final_speed, final_rotation_speed);

        Sleep(300);
    }
    while (!(Pos_Here >= 0.9));

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

    start_cell.i=2;
    start_cell.j=2;
    goal_cell.i=10;
    goal_cell.j=10;

    map.map = empty_map_info;

    run(start_cell, goal_cell, map);
}
