#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "epuck.hpp"
#include "Queue.h"

void queue_init(Queue* q)
{
    q->head = NULL;
    q->tail = NULL;
    q->hog = NULL;
}

void clear_queue(Queue* q)
{
    /*
    for (int i = 0; i < (Q_MAX_NUM - 1); i++)
    {
        q->element[i] = q->element[i + 1];
    }
    */
    /* q->element[Q_MAX_NUM - 1] = NULL; */

    q->head = NULL;
    q->tail = NULL;

    //q->hog = &(q.element[0]);
}

Q_Element* get_el_queue(Queue* q)
{
    Q_Element* p = q->hog;
    q->hog = p->next;

    return p;
}

void free_el_queue(Queue* q, Q_Element* el)
{
    el->next = q->hog;
    q->hog = el;
}

bool empty_queue(Queue* q)
{
    return (q->head == NULL);
}

void push_queue(Queue* q, Cell cell)
{
    Q_Element* el;
    el = (Q_Element*)calloc(1, sizeof(Q_Element));

    el->i = cell.i;
    el->j = cell.j;
    el->next = NULL;

    if (q->tail)
    {
        // Set the current tails 'next' to the new element
        (q->tail)->next = el;
    }

    // Set the current tail to the new element
    q->tail = el;

    if (q->head == NULL)
    {
        q->head = el;
    }
}

Q_Element pop_queue(Queue* q)
{
    if (q->head == NULL)
    {
        printf("Trying to pop an empty queue.\n");
        exit(1);
    }

    Q_Element el = *(q->head);

    q->head = el.next;

    if (q->head == NULL)
    {
        q->tail = NULL;
    }

    free_el_queue(q, &el);

    return el;
}

void print_queue(Queue q)
{
    if (q.head == NULL)
    {
        printf("Trying to print an empty queue.\n");
        return;
    }
    
    Q_Element* queue_iterator;
    int i;

    for (queue_iterator = q.head, i = 0;
         queue_iterator->next != NULL;
         queue_iterator = queue_iterator->next, i++)
    {
        printf("[%d: %d, %d]->", i + 1, queue_iterator->i, queue_iterator->j);
    }
    
    printf("[%d: %d, %d]\n", i + 1, q.tail->i, q.tail->j);
}

void test_queue()
{
    Queue queue, queue2;
    queue_init(&queue2);

    printf("\n---- Test: Create a queue ----\n");
    queue_init(&queue);
    
    printf("The queue should be empty:\n");
    if (empty_queue(&queue))
    {
        printf("Queue is empty.\n");
    }
    else
    {
        printf("Queue is not empty.\n");
    }
    printf("---- Done ----\n");

    printf("\n---- Test: Adding cells to the queue ----\n");
    Cell cell1, cell2, cell3;
    
    cell1.i = 10;
    cell1.j = 5;
    cell2.i = 2;
    cell2.j = 4;
    cell3.i = 50;
    cell3.j = -50;

    printf("Add (10,5) to the queue\n");
    push_queue(&queue, cell1);

    printf("The queue should not be empty:\n");
    if (empty_queue(&queue))
    {
        printf("Queue is empty.\n");
    }
    else
    {
        printf("Queue is not empty.\n");
    }
    
    printf("Add (2,4) to the queue\n");
    push_queue(&queue, cell2);

    printf("Add (50,-50) to the queue\n");
    push_queue(&queue, cell3);

    printf("\nPrint the queue\n");
    print_queue(queue);
    printf("---- Done ----\n");

    printf("\n---- Test: Popping from the queue ----\n");
    Q_Element popped_element;
    
    printf("Popping..\n");
    popped_element = pop_queue(&queue);
    printf("The popped element: (%d, %d)\n", popped_element.i, popped_element.j);
    print_queue(queue);
    
    printf("The queue should not be empty:\n");
    if (empty_queue(&queue))
    {
        printf("Queue is empty.\n");
    }
    else
    {
        printf("Queue is not empty.\n");
    }
    
    printf("Popping..\n");
    popped_element = pop_queue(&queue);
    printf("The popped element: (%d, %d)\n", popped_element.i, popped_element.j);
    print_queue(queue);

    printf("Popping..\n");
    popped_element = pop_queue(&queue);
    printf("The popped element: (%d, %d)\n", popped_element.i, popped_element.j);
    print_queue(queue);

    printf("The queue should be empty:\n");
    if (empty_queue(&queue))
    {
        printf("Queue is empty.\n");
    }
    else
    {
        printf("Queue is not empty.\n");
    }
    printf("---- Done ----\n");

    printf("\n---- Test: Clearing the queue ----\n");
    printf("Add (2,4) to the queue\n");
    push_queue(&queue, cell2);

    printf("Add (50,-50) to the queue\n");
    push_queue(&queue, cell3);

    printf("Clearing the queue..\n");
    clear_queue(&queue);
    
    printf("The queue should be empty:\n");
    if (empty_queue(&queue))
    {
        printf("Queue is empty.\n");
    }
    else
    {
        printf("Queue is not empty.\n");
    }
    printf("---- Done ----\n");
}
