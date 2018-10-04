#ifndef QUEUE_H
#define QUEUE_H

#define Q_MAX_NUM 30

typedef struct
{
    int i, j;
} Cell;

typedef struct Q_Element Q_Element;

/** @brief
 * 
 */
struct Q_Element
{
    int i, j;
    Q_Element* next;
};

/** @brief Queue struct.
 * 
 */
typedef struct
{
    Q_Element* head;
    Q_Element* tail;
    Q_Element* hog;
    Q_Element element[Q_MAX_NUM];
} Queue;

/** @brief Clears the queue.
 * 
 * @param q The queue.
 * @return Void
 */
void clear_queue(Queue* q);

/** @brief
 *
 * @param q The queue.
 */
Q_Element* get_el_queue(Queue* q);

/** @brief
 *
 * @return Void
 */
void free_el_queue(Queue* q, Q_Element* el);

/** @brief Check if the queue is empty.
 *
 * @param q The queue.
 * @return True or false as 0 or 1.
 */
int empty_queue(Queue* q);

/** @brief
 *
 */
void push_queue(Queue* q, Cell cell);

/** @brief
 *
 * @param q The queue.
 */
Q_Element pop_queue(Queue* q);

/** @brief
 *
 * @return Void
 */
void testQueue();

#endif
