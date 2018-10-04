#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include "Queue.h"

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

int empty_queue(Queue* q)
{
    return (q->head == NULL);
}

void push_queue(Queue* q, Cell cell)
{
    Q_Element el;

    el.i = cell.i;
    el.j = cell.j;
    el.next = NULL;

    if (q->tail)
    {
        // Set the current tails 'next' to the new element
        (q->tail)->next = &el;
    }

    // Set the current tail to the new element
    q->tail = &el;

    if (q->head == NULL)
    {
        q->head = &el;
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

void testQueue()
{
    printf("Create queue..\n");
    Queue queue;
}
