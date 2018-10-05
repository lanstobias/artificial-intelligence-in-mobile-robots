#include <stdbool.h>
#include "interface.h"
#include "fuzzy.h"
#include "lab1.h"
#include "lab2.h"
#include "lab3.h"
#include "lab4.h"
#include "lab5.h"
#include "Queue.h"
#include "map.h"

#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"
#define clear() printf("\033[H\033[J")

// Globals
Queue main_queue;
Cell robot_start_position;

int map2[16][16] = {  
    {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
    {-1, -2, -2, -3, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -1},
    {-1, -2, -2, -2, -2, -3, -2, -2, -2, -2, -3, -2, -2, -2, -3, -1},
    {-1, -2, -2, -2, -2, -3, -2, -2, -2, -2, -2, -2, -2, -2, -2, -1},
    {-1, -2, -2, -2, -2, -3, -3, -3, -3, -2, -2, -2, -2, -2, -2, -1},
    {-1, -2, -2, -2, -2, -3, -2, -2, -2, -2, -2, -2, -2, -2, -2, -1},
    {-1, -2, -2, -2, -2, -3, -2, -2, -2, -2, -2, -2, -2, -2, -2, -1},
    {-1, -2, -2, -3, -3, -3, -2, -2, -2, -2, -2, -2, -2, -2, -2, -1},
    {-1, -2, -2, -2, -2, -3, -3, -3, -3, -3, -3, -3, -3, -3, -3, -1},
    {-1, -2, -2, -3, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -1},
    {-1, -2, -2, -2, -2, -3, -3, -2, -2, -2, -2, -2, -2, -2, -2, -1},
    {-1, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -1},
    {-1, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -3, -3, -2, -1},
    {-1, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -1},
    {-1, -2, -2, -2, -2, -2, -2, -2, -2, -2, -3, -2, -2, -3, -2, -1},
    {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}
};

int map[16][16] = {  
    {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
    {-1, -2, -2, -3, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -1},
    {-1, -2, -2, -3, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -1},
    {-1, -2, -2, -2, -2, -3, -3, -3, -3, -2, -2, -2, -2, -2, -2, -1},
    {-1, -2, -2, -2, -2, -3, -3, -2, -2, -2, -2, -2, -2, -2, -2, -1},
    {-1, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -1},
    {-1, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -3, -3, -2, -1},
    {-1, -2, -2, -2, -2, -3, -3, -3, -3, -3, -2, -2, -2, -3, -3, -1},
    {-1, -2, -2, -2, -2, -3, -2, -2, -2, -2, -3, -2, -2, -2, -3, -1},
    {-1, -2, -2, -2, -2, -3, -2, -2, -2, -2, -2, -2, -2, -2, -2, -1},
    {-1, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -1},
    {-1, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -1},
    {-1, -2, -2, -3, -3, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -1},
    {-1, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -1},
    {-1, -2, -2, -2, -2, -2, -2, -2, -2, -2, -3, -2, -2, -3, -2, -1},
    {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}
};

void printMap() {
    for (int i=0; i<16; i++) {
        for (int j=0; j<16; j++) {
            
            switch(map[i][j]) {   
                case -1: printf("-1") ;break;
                case -2: printf("-2") ;break;
                case -3: printf("-3") ;break;
                case -4: printf("-4") ;break;
                default: printf(" %d", map[i][j]);
            }         
        }
        
        printf("\n");    
    }
}

void printPrettyMap() {
    for (int i=0; i<16; i++) {
        for (int j=0; j<16; j++) {
            
            switch(map[i][j]) {   
                case 0: printf(ANSI_COLOR_GREEN " G" ANSI_COLOR_RESET) ;break;
                case -1: printf(ANSI_COLOR_YELLOW "\u2586\u2586" ANSI_COLOR_RESET) ;break;
                case -2: printf("\u2591\u2591") ;break;
                case -3: printf(ANSI_COLOR_RED"\u2586\u2586" ANSI_COLOR_RESET) ;break;
                case -4: printf(ANSI_COLOR_BLUE " S" ANSI_COLOR_RESET) ;break;
                default: printf("%2d", map[i][j]);
            }         
        }
        
        printf("\n");    
    }
}

void printWaterMap() {
    for (int i=0; i<16; i++) {
        for (int j=0; j<16; j++) {
            
            switch(map[i][j]) {   
                case 0: printf(ANSI_COLOR_GREEN " G" ANSI_COLOR_RESET) ;break;
                case -1: printf(ANSI_COLOR_YELLOW "\u2586\u2586" ANSI_COLOR_RESET) ;break;
                case -2: printf("\u2591\u2591") ;break;
                case -3: printf(ANSI_COLOR_RED"\u2586\u2586" ANSI_COLOR_RESET) ;break;
                case -4: printf(ANSI_COLOR_BLUE " S" ANSI_COLOR_RESET) ;break;
                default: printf(ANSI_COLOR_CYAN "\u2591\u2591" ANSI_COLOR_RESET);
            }         
        }
        
        printf("\n");    
    }
}

bool cell_is_robot_start_postiion(Q_Element cell)
{
    return ((cell.i == robot_start_position.i) && 
            (cell.j == robot_start_position.j));
}

void place_start_and_end_on_map(Cell start_cell, Cell goal_cell)
{
    map[start_cell.i][start_cell.j] = -4;
    map[goal_cell.i][goal_cell.j] = 0;
}

void simulate_search()
{
    clear();
    printWaterMap();
    Sleep(50);
}

//==========================================================================//
//                            Search funciton                               //
//==========================================================================//
bool breadth_first_search(Cell goal_cell)
{
    Q_Element c;
    int distance;

    clear_queue(&main_queue);

    // Put the goal cell in the queue
    push_queue(&main_queue, goal_cell);

    // Repeat until the queue is empty
    while (!empty_queue(&main_queue))
    {
        // Take a cell c from the queue
        c = pop_queue(&main_queue);
        
        // If c is the robot cell, then return success
        if (cell_is_robot_start_postiion(c))
        {
            return true;
        }

        // foreach n which is a neighbor cell of c
        distance = (map[c.i][c.j] + 1);

        MarkCell(c.i, (c.j - 1), distance);
        MarkCell(c.i, (c.j + 1), distance);
        MarkCell((c.i - 1), c.j, distance);
        MarkCell((c.i + 1), c.j, distance);

        // Uncomment to see the search in action
        // simulate_search();
    }
    
    // If the queue is empty then return fail
    return false;
}

void MarkCell(int i, int j, int distance)
{
    Cell cell_to_be_marked; 
    cell_to_be_marked.i = i;
    cell_to_be_marked.j = j;

    switch (map[i][j])
    {
        case -1:
            break;

        case -2:
            map[i][j] = distance;
            push_queue(&main_queue, cell_to_be_marked);
            break;

        case -3:
            break;

        case -4:
            push_queue(&main_queue, cell_to_be_marked);
            break;
    
        default:
            // Refers to the goal cell or marked cells
            break;
    }
}

//==========================================================================//
//                                  lab5                                    //
//==========================================================================//
void lab5()
{
    queue_init(&main_queue);
    Cell goal_cell;
    goal_cell.i = 11;
    goal_cell.j = 9;

    robot_start_position.i = 4;
    robot_start_position.j = 13;

    place_start_and_end_on_map(robot_start_position, goal_cell);

    printf("Map before search:\n");
    printPrettyMap();

    printf("Searching..\n");
    if (breadth_first_search(goal_cell))
    {
        printf("Search successfull!\n");
    }
    else
    {
        printf("Search failed :(\n");
    }

    printf("Map after search:\n");
    printPrettyMap();

    //test_queue();
}
