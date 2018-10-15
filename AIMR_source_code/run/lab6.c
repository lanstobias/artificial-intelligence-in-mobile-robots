#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "Queue.h"
#include "lab5.h"
#include "lab6.h"

// Macros
#define DEFAULT_INPUT 0
#define RUN_DEFAULT_PROGRAM 1
#define CHANGE_ALL_INPUTS 2
#define CHANGE_START_POSITION 3
#define CHANGE_GOAL_POSITION 4
#define CHANGE_MAP 5
#define EXIT 9

#define NUM_OF_MAPS 3

#define CELL_MM 20

// Globals
static int hard_map_info[16][16] = {  
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

static int easy_map_info[16][16] = {  
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

Map_custom easy_map, medium_map, hard_map;
Map_custom maps[NUM_OF_MAPS];

void wait_for_user()
{
    printf("Press any key to continue..");
    getchar();
    getchar();
}

void printMenuOptions() {
    printf("Robot menu:\n");
    printf("------------------------------------------\n");
    printf("[1] Run program with default values.\n");
    printf("[2] Change all input variables.\n");
    printf("[3] Change start position.\n");
    printf("[4] Change Goal position.\n"); 
    printf("[5] Change the map.\n"); 
    printf("[9] Exit the program.\n"); 
}

void run(Cell* start_cell, Cell* goal_cell, Map_custom* current_map)
{
    Queue path, queue;
    queue_init(&path);
    queue_init(&queue);

    place_start_and_end_on_map(current_map, *start_cell, *goal_cell);
    path = Plan(current_map, &queue, *start_cell, *goal_cell);

    /* todo:
    double coordinates_for_robot[whatever_size] = convert_path_to_robot_track(path);
    Track(coordinates_for_robot);
    */

    //printPrettyMap(*current_map, path);
    //print_queue(path);

    wait_for_user();
}

bool menu(int menu_choice, Cell* start_cell, Cell* goal_cell, Map_custom* current_map)
{
    switch (menu_choice)
    {
        case RUN_DEFAULT_PROGRAM:
            clear();
            run(start_cell, goal_cell, current_map);
            break;

        case CHANGE_ALL_INPUTS:
            clear();
            change_all_inputs(start_cell, goal_cell, current_map);
            break;

        case CHANGE_START_POSITION:
            clear();
            change_start_cell(start_cell);
            break;

        case CHANGE_GOAL_POSITION:
            clear();
            change_goal_cell(goal_cell);
            break;

        case CHANGE_MAP:
            clear();
            change_current_map(current_map);
            break;

        case EXIT:
            return false;
            break;

        default:
            break;
    }
    
    return true;
}

void change_start_cell(Cell* start_cell)
{
    int user_start_i, user_start_j;
     
    printf("Enter start X-position: ");
    scanf("%d", &user_start_j);
    if ((user_start_j < 15) && (user_start_j > 0) ) {
        start_cell->j = user_start_j;
    }
    else 
    {
        printf("Value must be between 1 and 14.\n");
    }
    
    printf("Enter start Y-position: ");
    scanf("%d", &user_start_i);
    if ((user_start_i < 15) && (user_start_i > 0)) {
        start_cell->i = user_start_i;                
    }
    else 
    {
        printf("Value must be between 1 and 14.\n");
        getchar();
        getchar();
    }
}

void change_goal_cell(Cell* goal_cell)
{
    int user_goal_i, user_goal_j;
     
    printf("Enter goal X-position: ");
    scanf("%d", &user_goal_j);
    if ((user_goal_j < 15) && (user_goal_j > 0)) {
        goal_cell->j = user_goal_j;
    }
    else 
    {
        printf("Value must be between 1 and 14.\n");
    }
    
    printf("Enter goal Y-position: ");
    scanf("%d", &user_goal_i);
    if ((user_goal_i < 15) && (user_goal_i > 0)) {
        goal_cell->i = user_goal_i;
    } 
    else 
    {
        printf("Value must be between 1 and 14.\n");
        getchar();
        getchar();
    }
}

void change_current_map(Map_custom* current_map)
{
    int user_map_choice;

    for (int i = 0; i < NUM_OF_MAPS; i++)        
    {
        printf("%d. %s\n", i + 1, maps[i].name);     
    }
    printf("Choose map to run: ");
    scanf("%d", &user_map_choice);
    if ((user_map_choice < (NUM_OF_MAPS+1)) && (user_map_choice > 1)) {
        *current_map = maps[(user_map_choice - 1)];
    } 
    else 
    {
        printf("Value must be between 1 and %d.\n", NUM_OF_MAPS);
        getchar();
    }
    getchar();
}

void change_all_inputs(Cell* start_cell, Cell* goal_cell, Map_custom* current_map)
{
    change_start_cell(start_cell);                
    change_goal_cell(goal_cell);
    change_current_map(current_map);
}

void initialize_maps()
{
    easy_map.map = easy_map_info;
    strcpy(easy_map.name, "Easy map");

    medium_map.map = easy_map_info;
    strcpy(medium_map.name, "Medium map");

    hard_map.map = hard_map_info;
    strcpy(hard_map.name, "Hard map");
}

void initialize_cells(Cell* start_cell, Cell* goal_cell)
{
    start_cell->i = 1;
    start_cell->j = 1;
    
    goal_cell->i = 5;
    goal_cell->j = 5;
}

double* path_to_robot_coordiantes(Queue path)
{
    double previous_i, previous_j;
    int direction;
    unsigned int steps = 1;
    // Iterate through the path
        // Read cell values i,j

        // if (previous_i != i && previous_j == j)
            // horizontal change

        // if (previous_i == i && previous_j != j)
            // vertical change

            // store previous cell as robot coordinate as (steps * CELL_MM)
            //Set steps to 1.

        // else
            //Add to steps
}

void lab6()
{
    printf("Start lab 6\n");
    
    int menu_choice;
    Cell start_cell, goal_cell;
    //queue_init(&main_queue);
    initialize_maps();
    initialize_cells(&start_cell, &goal_cell);

    Map_custom current_map;
    maps[0] = easy_map;
    maps[1] = medium_map;
    maps[2] = hard_map;
    current_map = easy_map;

    do {
        clear();
        
        printf("-----------------------\n");
        printf("Current values:\n");
        printf("Start cell: [%2d, %2d]\n", start_cell.j, start_cell.i);
        printf("Goal cell:  [%2d, %2d]\n", goal_cell.j, goal_cell.i);
        printf("Map_custom: %s\n", current_map.name);
        printf("-----------------------\n\n");
        
        printMenuOptions();
        printf("\nEnter choice: ");
        scanf("%d", &menu_choice);

    }
    while (menu(menu_choice, &start_cell, &goal_cell, &current_map)); 
}