#include "interface.h"
#include "fuzzy.h"
#include "lab1.h"
#include "lab2.h"
#include "lab3.h"
#include "lab4.h"
#include "lab5.h"

//==========================================================================//
//                       Helper function (clear queue)                      //
//==========================================================================//
void clear_queue(Queue q)
{
    for (int i = 0; i < (Q_MAX_NUM - 1); i++)
    {
        q.element[i] = &(q.element[i + 1]);
    }

    q.element[Q_MAX_NUM - 1] = NULL;
    q.head = NULL;
    q.tail = NULL;
    q.hog = &(q.element[0]);
}

//==========================================================================//
//                    Helper function (get element queue)                   //
//==========================================================================//
Q_Element get_el_queue(Queue q)
{
    Q_Element element_to_uenqueue = q.hog;
    q.hog = p->next;

    return element_to_uenqueue;
}

//==========================================================================//
//                   Helper function (free element queue)                   //
//==========================================================================//
void free_el_queue(Queue q, Q_Element el)
{
    el.next = q.hog;
    q.hog = &el;
}

//==========================================================================//
//                   Helper function (emtpy queue)                          //
//==========================================================================//
void empty_queue(Queue q)
{
    q.head = NULL;
}

Q_Element new_el_queue(Queue q)
{
    // TODO: Implement
    Q_Element el;

    return el;
}

//==========================================================================//
//                   Helper function (push queue)                           //
//==========================================================================//
int push_queue(Queue q, Cell cell)
{
    Q_Element el = new_el_queue(q);

    if (el == NULL)
    {
        return -1;
    }

    el.i = i;
    el.j = j;
    el.next = NULL;

    if (q.tail)
    {
        q.tail->next = &el;
    }

    q.tail = &el;

    if (q.head == NULL)
    {
        q.head = &el;
    }
}

//==========================================================================//
//                   Helper function (pop queue)                          //
//==========================================================================//
Q_Element pop_queue(Queue q)
{
    Q_Element el = *q.head;

    if (el == NULL)
    {
        return -1;
    }

    q.head = el->next;

    if (q.head == NULL)
    {
        q.tail = NULL;
    }

    free_el_queue(el);

    return el;
}

//==========================================================================//
//                            Search function                               //
//==========================================================================//
void Search ()
{
    //put the goal cell in the queue

    //repeat until the queue is empty

        // take a cell c from the queue
        // (depending on how we perform these operations we obtain search strategies)

        //if c is the robot cell then return(success)

        //foreach n which is a neighbor cell of c
            //if n is not an obstacle and n has label -2
                //label n by grid(c)+1
                //insert n in the queue
                // (depending on how we perform these operations we obtain search strategies)

    //if the queue is empty then return(failure)
}

//==========================================================================//
//                            Test Queue                                    //
//==========================================================================//
void testQueue()
{
    printf("Create queue..\n");
    Queue queue;
}

//==========================================================================//
//                                lab5                                      //
//==========================================================================//
void lab5()
{
    printf("-- Labb 5 --\n");
    testQueue();
}
