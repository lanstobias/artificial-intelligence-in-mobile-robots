#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "Queue.h"
#include "epuck.hpp"
#include "lab4.h"
#include "lab5.h"
#include "lab6.h"

// Macros
#define DEFAULT_INPUT 0
#define RUN_DEFAULT_PROGRAM 1
#define CHANGE_ALL_INPUTS 2
#define CHANGE_START_POSITION 3
#define CHANGE_GOAL_POSITION 4
#define CHANGE_MAP 5
#define CHANGE_AVOID 6
#define EXIT 9

#define NUM_OF_MAPS 5

#define CELL_MM 36

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

static int real_map_info[16][16] = {
    {-1, -1, -1, -1, -1, -1, -1, -3, -1, -1, -1, -1, -1, -1, -1, -1},
    {-1, -2, -2, -2, -2, -2, -2, -3, -2, -2, -2, -2, -2, -2, -2, -1},
    {-1, -2, -2, -2, -2, -2, -2, -3, -2, -2, -2, -2, -2, -2, -2, -1},
    {-1, -2, -2, -2, -2, -2, -2, -3, -2, -2, -2, -2, -2, -2, -2, -1},
    {-1, -2, -2, -2, -2, -2, -2, -3, -2, -2, -2, -2, -2, -2, -2, -1},
    {-1, -2, -2, -2, -2, -2, -2, -3, -2, -2, -2, -2, -2, -2, -2, -1},
    {-1, -2, -2, -2, -2, -2, -2, -3, -2, -2, -2, -2, -2, -2, -2, -1},
    {-1, -2, -2, -2, -2, -2, -2, -3, -2, -2, -2, -2, -2, -2, -2, -1},
    {-1, -2, -2, -2, -2, -2, -2, -3, -3, -3, -2, -2, -2, -2, -2, -1},
    {-1, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -1},
    {-1, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -1},
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

Map_custom easy_map, medium_map, hard_map, real_map, empty_map;
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
    printf("[6] Enable or disable avoid.\n");  
    printf("[9] Exit the program.\n"); 
}

bool direction_change(Q_Element current_cell, Q_Element previous_cell, Direction* direction, Movement* movement)
{
    // Move up
    if (current_cell.i < previous_cell.i && current_cell.j == previous_cell.j && *direction != Up)
    {
        *direction = Up;
        return true;
    }
    
    // Move right
    if (current_cell.i == previous_cell.i && current_cell.j > previous_cell.j && *direction != Right)
    {
        *direction = Right;
        return true;
    }
    
    // Move down
    if (current_cell.i > previous_cell.i && current_cell.j == previous_cell.j && *direction != Down)
    {
        *direction = Down;
        return true;
    }
    
    // Move left
    if (current_cell.i == previous_cell.i && current_cell.j < previous_cell.j && *direction != Left)
    {
        *direction = Left;
        return true;
    }

    return false;
}

void increse_movement(Direction direction, Movement* movement)
{
    switch (direction)
    {
        case Start:
            break;

        case Up:
            movement->vertical += CELL_MM;
            movement->horizontal += 0;
            break;
        
        case Right:
            movement->vertical += 0;
            movement->horizontal += CELL_MM;
            break;
        
        case Down:
            movement->vertical -= CELL_MM;
            movement->horizontal += 0;
            break;
        
        case Left:
            movement->vertical += 0;
            movement->horizontal -= CELL_MM;
            break;
    }
}


void store_target(Track_arrays* track, Movement movement)
{
    track->xarray[track->number_of_targets] = movement.horizontal;
    track->yarray[track->number_of_targets] = movement.vertical;
    track->number_of_targets++;
}

void add_goal_to_track(Track_arrays* track, Movement movement)
{
    store_target(track, movement);
}

void print_track(Track_arrays track)
{
    printf("[X][Y]\n");
    printf("-------\n");
    for (int i = 0; i < track.size; i++)
    {
        printf("[%4.lf][%4.lf]\n", track.xarray[i], track.yarray[i]);
    }
}

Track_arrays convert_path_to_robot_track(Queue path)
{
    //Track_arrays contains an xarray and an yarray to be used in track
    Track_arrays track;
    track.size = 0;
    track.number_of_targets = 0;
    
    //Movement - to keep track of vertical and horisontal movement of the robot
    Movement movement;
    movement.horizontal = 0;
    movement.vertical = 0;

    //Direction keeps track of which way was moved now and what was moved previously
    Direction direction = Start;

    //The current cell in the path we are observing
    Q_Element current_cell;

    // Set the the previous cell to the starting cell, to determine
    // the current_direction at the beginning.
    Q_Element previous_cell = pop_queue(&path);

    while (!empty_queue(&path))
    {
        current_cell = pop_queue(&path);

        if (direction_change(current_cell, previous_cell, &direction, &movement))
        {
            store_target(&track, movement);
        }

        increse_movement(direction, &movement);
        
        previous_cell = current_cell;
    }
    add_goal_to_track(&track, movement);

    track.size = track.number_of_targets;

    printf("\n");
    print_track(track);
    return track;
}

void run(Cell* start_cell, Cell* goal_cell, Map_custom* current_map, bool avoid)
{
    Queue path, queue;
    queue_init(&path);
    queue_init(&queue);

    place_start_and_end_on_map(current_map, *start_cell, *goal_cell);
    path = Plan(current_map, &queue, *start_cell, *goal_cell);

    Track_arrays track = convert_path_to_robot_track(path);
    printPrettyMap(*current_map, path);
    FuzzyTrack(track.xarray, track.yarray, track.size, avoid);

    wait_for_user();
}

bool menu(int menu_choice, Cell* start_cell, Cell* goal_cell, Map_custom* current_map, bool* avoid)
{
    switch (menu_choice)
    {
        case RUN_DEFAULT_PROGRAM:
            clear();
            run(start_cell, goal_cell, current_map, *avoid);
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

        case CHANGE_AVOID:
            clear();
            *avoid = !(*avoid);
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

    real_map.map = real_map_info;
    strcpy(real_map.name, "Real map");

    empty_map.map = empty_map_info;
    strcpy(empty_map.name, "Empty map");
}

void initialize_cells(Cell* start_cell, Cell* goal_cell)
{
    start_cell->i = 2;
    start_cell->j = 2;
    
    goal_cell->i = 2;
    goal_cell->j = 10;
}

void lab6()
{
    printf("Start lab 6\n");
    
    int menu_choice;
    Cell start_cell, goal_cell;
    bool avoid= true;
    //queue_init(&main_queue);
    initialize_maps();
    initialize_cells(&start_cell, &goal_cell);

    Map_custom current_map;
    maps[0] = easy_map;
    maps[1] = medium_map;
    maps[2] = hard_map;
    maps[3] = real_map;
    maps[4] = empty_map;
    current_map = empty_map;

    do {
        clear();
        
        printf("-----------------------\n");
        printf("Current values:\n");
        printf("Start cell: [%2d, %2d]\n", start_cell.j, start_cell.i);
        printf("Goal cell:  [%2d, %2d]\n", goal_cell.j, goal_cell.i);
        printf("Chosen map: %s\n", current_map.name);
        if (avoid)
        {
            printf("Avoid enabled\n");
        }
        else
        {
            printf("Avoid disabled\n");
        }
        printf("-----------------------\n\n");
        
        printMenuOptions();
        printf("\nEnter choice: ");
        scanf("%d", &menu_choice);

    }
    while (menu(menu_choice, &start_cell, &goal_cell, &current_map, &avoid)); 
}
