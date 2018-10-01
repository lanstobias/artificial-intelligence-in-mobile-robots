#ifndef LAB5H
#define LAB5H

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


void lab5();

#endif
