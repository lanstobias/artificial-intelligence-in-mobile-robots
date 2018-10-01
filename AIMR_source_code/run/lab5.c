#include "interface.h"
#include "fuzzy.h"
#include "lab1.h"
#include "lab2.h"
#include "lab3.h"
#include "lab4.h"
#include "lab5.h"

#define QMaxNum = 30

// structs
typedef struct
{
    int i, j;
    Q_Element* next;
} Q_Element;

typedef struct
{
    Q_Element* head;
    Q_Element* hog;
    Q_Element* element[QMaxNum];
} Queue;

// Globals

//==========================================================================//
//                       Helper function (clear queue)                      //
//==========================================================================//
void clear_queue (Queue q) {
    //for i from 0 to (QMaxNum-1) 
        //q.element[i] := &(q.element[i+1]) 
    //q.element[QMaxNum-1] := null 
    //q.head := null 
    //q.tail := null 
    //q.hog := &(q.element[0]) 
}

//==========================================================================//
//                    Helper function (get element queue)                   //
//==========================================================================//
Q_Element get_el_queue (Queue q) {
    //p := q.hog 
    //q.hog := p->next 
    return(p) 
}

//==========================================================================//
//                   Helper function (free element queue)                   //
//==========================================================================//
void free_el_queue (Q_Element el, Queue q) {
    //el.next := q.hog 
    //q.hog := &el
}

//==========================================================================//
//                   Helper function (emtpy queue)                          //
//==========================================================================//
Q_Element empty_queue (Queue q) {
    return(q.head = null) 
}

//==========================================================================//
//                   Helper function (push queue)                           //
//==========================================================================//
int push_queue (Cell [i,j], Queue q) {
    //el := new_el_queue(q) 
    //if (el = null) return(-1) 
    //el.i := i 
    //el.j := j 
    //el.next := nil 
    //if (q.tail) q.tail->next := &el 
    //q.tail := &el 
    //if (q.head = nil) q.head := &el
}

//==========================================================================//
//                   Helper function (pop queue)                          //
//==========================================================================//
Q_Element pop_queue (Queue q) {
    //el := q.head 
    //if (el = null) return(-1) 
    //q.head := el->next 
    //if (q.head = nil) q.tail := nil 
    //free_el_queue(el) 
    //return(el)
}

//==========================================================================//
//                            Search function                               //
//==========================================================================//
void Search () {
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
//                                lab5                                      //
//==========================================================================//
void lab5()
{
     
}