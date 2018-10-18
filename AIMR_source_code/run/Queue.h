#ifndef QUEUE_H
#define QUEUE_H

#include <stdbool.h>
#include <epuck.hpp>

#define Q_MAX_NUM 30

typedef struct Q_Element Q_Element;

/** @brief An element in the queue. */
struct Q_Element
{
    int i, j;
    Q_Element* next;
};

/** @brief Linked list queue. */
typedef struct
{
    Q_Element* head;
    Q_Element* tail;
    Q_Element* hog;
    Q_Element element[Q_MAX_NUM];
} Queue;

void queue_init(Queue* q);

/** @brief Clears the queue.
 *
 * @param q Pointer to the queue struct.
 * @return Void
 */
void clear_queue(Queue* q);

/** @brief ??
 *
 * @param q Pointer to the queue struct.
 * @return Pointer to the element struct.
 */
Q_Element* get_el_queue(Queue* q);

/** @brief ??
 *
 * @return Void
 */
void free_el_queue(Queue* q, Q_Element* el);

/** @brief Check if the queue is empty.
 *
 * @param q Pointer to the queue struct.
 * @return True or false.
 */
bool empty_queue(Queue* q);

/** @brief Push a cell to the queue.
 *
 * @param q Pointer to the queue struct.
 * @param cell The cell struct containing the index values.
 * @return Void
 */
void push_queue(Queue* q, Cell cell);

/** @brief Pop an element from the queue.
 *
 * @param q Pointer to the queue struct.
 * @return The element struct.
 */
Q_Element pop_queue(Queue* q);

/** @brief Print the content of the queue.
 *
 * @param q The queue struct.
 * @return Void.
 */
void print_queue(Queue q);

/** @brief Test and debugging function for the queue.
 *
 * @return Void
 */
void test_queue();

#endif
