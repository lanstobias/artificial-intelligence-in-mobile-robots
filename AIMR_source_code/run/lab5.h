#ifndef LAB5H
#define LAB5H

#define Q_MAX_NUM 30

typedef struct Q_Element Q_Element;

struct Q_Element
{
    int i, j;
    Q_Element* next;
};

typedef struct
{
    Q_Element* head;
    Q_Element* tail;
    Q_Element* hog;
    Q_Element element[Q_MAX_NUM];
} Queue;

void clear_queue(Queue q);
Q_Element get_el_queue(Queue q);
void free_el_queue(Queue q, Q_Element el);
void empty_queue(Queue q);
int push_queue(Queue q, Cell cell);
Q_Element pop_queue(Queue q);
void Search();
void testQueue();


void lab5();

#endif
