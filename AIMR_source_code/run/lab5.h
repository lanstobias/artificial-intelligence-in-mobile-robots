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
void lab5();

void printMap();
void printPrettyMap();
void printWaterMap();

#endif
