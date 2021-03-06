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

// Macros
// source: https://stackoverflow.com/a/3219471
#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"

#define NUM_NEIGHBOR_CELLS 4
#define LARGE_INT 1000000

// Globals
Map_custom easy_map_lab5;
Map_custom hard_map_lab5;

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

void printMap(Map_custom map) {
    for (int i=0; i<16; i++) {
        for (int j=0; j<16; j++) {
            
            switch(map.map[i][j]) {   
                case -1: printf("-1") ;break;
                case -2: printf("-2") ;break;
                case -3: printf("-3") ;break;
                case -4: printf("-4") ;break;
                default: printf(" %d", map.map[i][j]);
            }         
        }
        
        printf("\n");    
    }
}

void printPrettyMap(Map_custom map, Queue path) {
    Cell cell;

    for (int i=0; i<16; i++) {
        for (int j=0; j<16; j++)
        {
            switch(map.map[i][j])
            {
                case 0: printf(ANSI_COLOR_GREEN " G" ANSI_COLOR_RESET); break;
                case -1: printf(ANSI_COLOR_YELLOW "\u2586\u2586" ANSI_COLOR_RESET); break;
                case -2: printf("\u2591\u2591"); break;
                case -3: printf(ANSI_COLOR_RED"\u2586\u2586" ANSI_COLOR_RESET); break;
                case -4: printf(ANSI_COLOR_BLUE " S" ANSI_COLOR_RESET); break;
                default:
                    cell.i = i;
                    cell.j = j;

                    if (cell_in_queue(path, cell))
                    {
                        printf(ANSI_COLOR_CYAN "%2d" ANSI_COLOR_RESET, map.map[i][j]);
                    }
                    else
                    {
                        printf("%2d", map.map[i][j]);
                    }
                    break;
            }
        }
        
        printf("\n");    
    }
}

void printWaterMap(Map_custom map) {
    for (int i=0; i<16; i++) {
        for (int j=0; j<16; j++) {
            
            switch(map.map[i][j]) {   
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

bool cell_is_robot_start_position(Q_Element cell, Cell start_cell)
{
    return ((cell.i == start_cell.i) && 
            (cell.j == start_cell.j));
}

void place_start_and_end_on_map(Map_custom* map, Cell start_cell, Cell goal_cell)
{
    map->map[start_cell.i][start_cell.j] = -4;
    map->map[goal_cell.i][goal_cell.j] = 0;
}

void simulate_search(Map_custom map)
{
    clear();
    printWaterMap(map);
    Sleep(20);
}

bool breadth_first_search(Map_custom* map, Queue* queue, Cell start_cell, Cell goal_cell)
{
    Q_Element c;
    int distance;

    clear_queue(queue);

    // Put the goal cell in the queue
    push_queue(queue, goal_cell);

    // Repeat until the queue is empty
    while (!empty_queue(queue))
    {
        // Take a cell c from the queue
        c = pop_queue(queue);
        
        // If c is the robot cell, then return success
        if (cell_is_robot_start_position(c, start_cell))
        {
            return true;
        }

        // foreach n which is a neighbor cell of c
        distance = (map->map[c.i][c.j] + 1);

        MarkCell(map, queue, c.i, (c.j - 1), distance);
        MarkCell(map, queue, c.i, (c.j + 1), distance);
        MarkCell(map, queue, (c.i - 1), c.j, distance);
        MarkCell(map, queue, (c.i + 1), c.j, distance);

        // Uncomment to see the search in action
        simulate_search(*map);
    }
    
    // If the queue is empty then return fail
    return false;
}

void MarkCell(Map_custom* map, Queue* queue, int i, int j, int distance)
{
    Cell cell_to_be_marked; 
    cell_to_be_marked.i = i;
    cell_to_be_marked.j = j;

    switch (map->map[i][j])
    {
        case -1:
            break;

        case -2:
            map->map[i][j] = distance;
            push_queue(queue, cell_to_be_marked);
            break;

        case -3:
            break;

        case -4:
            push_queue(queue, cell_to_be_marked);
            break;
    
        default:
            // Refers to the goal cell or marked cells
            break;
    }
}

void generate_neighboring_cells(Cell current_cell, Cell* current_cell_neighbors)
{
    current_cell_neighbors[0].i = (current_cell.i - 1);
    current_cell_neighbors[0].j = (current_cell.j);

    current_cell_neighbors[1].i = (current_cell.i + 1);
    current_cell_neighbors[1].j = (current_cell.j);

    current_cell_neighbors[2].i = (current_cell.i);
    current_cell_neighbors[2].j = (current_cell.j - 1);

    current_cell_neighbors[3].i = (current_cell.i);
    current_cell_neighbors[3].j = (current_cell.j + 1);
}

void print_cell(Cell cell)
{
    printf("[%d, %d]->", cell.i, cell.j);
}

void print_goal(Cell goal_cell)
{
    printf("[%d, %d]\n", goal_cell.i, goal_cell.j);
}

Queue Plan(Map_custom* map, Queue* queue, Cell start_cell, Cell goal_cell)
{
    Queue path;
    queue_init(&path);

    if (!breadth_first_search(map, queue, start_cell, goal_cell))
    {
        printf("Did not find the goal :(\n");
        exit(1);
    }

    Cell current_cell, best_cell, neighbor_cell;
    Cell current_cell_neighbors[NUM_NEIGHBOR_CELLS] ;
    int best_value, neighbor_cell_value;

    current_cell = start_cell;
    push_queue(&path, current_cell);

    while (map->map[current_cell.i][current_cell.j] != 0)
    {
        generate_neighboring_cells(current_cell, current_cell_neighbors);
        best_value = LARGE_INT;
    
        for (int i = 0; i < NUM_NEIGHBOR_CELLS; i++)
        {
            neighbor_cell = current_cell_neighbors[i];
            neighbor_cell_value = map->map[neighbor_cell.i][neighbor_cell.j];

            if (neighbor_cell_value < best_value && neighbor_cell_value >= 0)
            {
                best_value = neighbor_cell_value;
                best_cell = neighbor_cell;
            }
        }

        current_cell = best_cell;
        push_queue(&path, current_cell);
    }

    return path;
}

bool cell_in_queue(Queue path, Cell cell)
{
    if (path.head == NULL)
    {
        return false;
    }
    
    Q_Element* queue_iterator;

    for (queue_iterator = path.head;
         queue_iterator->next != NULL;
         queue_iterator = queue_iterator->next)
    {
        if (cell.i == queue_iterator->i && cell.j == queue_iterator->j)
        {
            return true;
        }
    }
    
    return false;
}

void initialize_maps_lab5()
{
    easy_map_lab5.map = easy_map_info;
    strcpy(easy_map_lab5.name, "Easy map");

    hard_map_lab5.map = hard_map_info;
    strcpy(hard_map_lab5.name, "Hard map");
}

void lab5()
{
    printf("Start of lab5\n");
    Queue path, queue;
    Cell goal_cell, robot_start_position;
    Map_custom current_map;
    initialize_maps_lab5();

    current_map=easy_map_lab5;

    queue_init(&path);
    queue_init(&queue);

    // Easy Map
    
    robot_start_position.i = 4;
    robot_start_position.j = 13;
    goal_cell.i = 11;
    goal_cell.j = 9;
    

    // Easy map with different start/goal
    /* 
    robot_start_position.i = 14;
    robot_start_position.j = 14;
    goal_cell.i = 1;
    goal_cell.j = 1;
    */

    // Hard map
    /*
    robot_start_position.i = 7;
    robot_start_position.j = 5;
    goal_cell.i = 14;
    goal_cell.j = 14;
    */

    place_start_and_end_on_map(&current_map, robot_start_position, goal_cell);

    // Planning
    printPrettyMap(current_map, path);
    printf("\n");
    path = Plan(&current_map, &queue, robot_start_position, goal_cell);
    printPrettyMap(current_map, path);

    print_queue(path);
}
