#ifndef LAB6_H
#define LAB6_H
#include "Queue.h"
#include "lab5.h"

void printMenuOptions();
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