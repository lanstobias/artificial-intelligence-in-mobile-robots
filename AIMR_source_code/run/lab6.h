#ifndef LAB6_H
#define LAB6_H
#include "Queue.h"
#include "lab5.h"

typedef struct
{
    float xarray[200];
    float yarray[200];
    int size;
    int number_of_targets;
}Track_arrays;

typedef struct
{
    int horizontal, vertical;
} Movement;

typedef enum {Start, Up, Right, Down, Left} Direction;

void printMenuOptions();
Track_arrays convert_path_to_robot_track(Queue path);
bool direction_change(Q_Element current_cell, Q_Element previous_cell,
                      Direction* direction, Movement* movement);
void wait_for_user();
void run(Cell* start_cell, Cell* goal_cell, Map_custom* current_map);
bool menu(int menu_choice, Cell* start_cell, Cell* goal_cell, Map_custom* current_map);
void change_start_cell(Cell* start_cell);
void change_goal_cell(Cell* goal_cell);
void change_current_map(Map_custom* current_map);
void change_all_inputs(Cell* start_cell, Cell* goal_cell, Map_custom* current_map);
void initialize_maps();
void initialize_cells(Cell* start_cell, Cell* goal_cell);
void get_user_input();

void lab6();

#endif
