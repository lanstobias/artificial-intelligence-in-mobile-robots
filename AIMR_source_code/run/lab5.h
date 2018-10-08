#ifndef LAB5H
#define LAB5H

#include <stdbool.h>
#include "Queue.h"

/** @brief Search using bread first search.
 * 
 * Searches from the goal cell to the starting cell. The goal_cell
 * parameter is where the search is starting from.
 * 
 * @param goal_cell The starting cell of the search.
 * @return True if the search is successfull, false otherwise.
 */
bool breadth_first_search(Cell goal_cell);

/** @brief Mark a cell with with distance from goal cell.
 * 
 * @param i The i-index of the cell.
 * @param j The j-index of the cell.
 * @param distance The distance from the goal cell.
 * @return Void.
 */
void MarkCell(int i, int j, int distance);

/** @brief Print map with only numbers.
 * 
 * @return Void.
 */
void printMap();

/** @brief Print a map using Unicode characters.
 * 
 * @return Void.
 */
void printPrettyMap(Queue path);

/** @brief Print explored cells with blue unicode character.
 * 
 * @return Void.
 */
void printWaterMap();

/** @brief Check if the cell is the robot start position.
 * 
 * @return True or false.
 */
bool cell_is_robot_start_postiion(Q_Element cell);

/** @brief Place start cell and goal cell on the map.
 * 
 * @return Void.
 */
void place_start_and_end_on_map(Cell start_cell, Cell goal_cell);

/** @brief Visualize the search algorithm.
 * 
 * Calls the function printWaterMap() and Sleep() to show how the
 * search algorithm is expanding the unexplored cells. Clearing the
 * console before each call with the clear() funciton.
 * 
 * @return Void.
 */
void simulate_search();

/** @brief Create and store neighboring cells of the current cell.
 * 
 * @param current_cell The cell that has the neighbors to be explored.
 * @param current_cell_neighbors Array of cellsthat will hold the
 * neighboring cells.
 * @return Void.
 */
void generate_neighboring_cells(Cell current_cell, Cell* current_cell_neighbors);

/** @brief Print cell coordinates.
 * 
 * @param cell The cell to be printed.
 * @return Void.
 */
void print_cell(Cell cell);

/** @brief Print the goal cell.
 * 
 * @param goal_cell The goal cell.
 */
void print_goal(Cell goal_cell);

/** @brief Find the path to the goal cell and store in a queue.
 * 
 * @param start_cell The starting cell of the robot.
 * @param goal_cell The robot's goal position.
 * @return A queue with each cell making up the path to the goal cell.
 */
Queue Plan(Cell start_cell, Cell goal_cell);

/** @brief Check if the queue is containing a specific cell.
 * 
 * @param path The queue of cells which makes up the path to the goal.
 * @param cell The cell to look for in the queue.
 * @return True if the cell is found, otherwise false.
 */
bool cell_in_queue(Queue path, Cell cell);

/** @brief Demo function for lab 5.
 * 
 * @return Void.
 */
void lab5();

#endif
