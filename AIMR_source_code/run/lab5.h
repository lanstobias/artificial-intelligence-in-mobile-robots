#ifndef LAB5H
#define LAB5H

#include <stdbool.h>
#include "Queue.h"

/** @brief
 *
 */
bool breadth_first_search(Cell goal_cell);

/** @brief
 *
 */
void MarkCell(int i, int j, int distance);

/** @brief
 * 
 */
void printMap();

void printPrettyMap(Queue path);

void printWaterMap();

bool cell_is_robot_start_postiion(Q_Element cell);

void place_start_and_end_on_map(Cell start_cell, Cell goal_cell);

void simulate_search();

void generate_neighboring_cells(Cell current_cell, Cell* current_cell_neighbors);

void print_cell(Cell cell);

void print_goal(Cell goal_cell);

Queue Plan(Cell start_cell, Cell goal_cell);

bool cell_in_queue(Queue path, Cell cell);

/** @brief
 *
 */
void lab5();

#endif
