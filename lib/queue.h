// set.h
#ifndef QUEUE_H
#define QUEUE_H

#include <exception>
#include <stdexcept>

class EmptyQueueException : public exception {

};

template <class T>
struct QueueItem
{
  //linked list item of the queue
  T value;
  QueueItem<T> *next;
  QueueItem<T>(T v, QueueItem<T> *n) {value = v; next = n;}
};

template <class T>
class Queue {
  private:
    //we need both front and the back of the queue to keep everything in O(1)
    QueueItem<T> *front = NULL;
    QueueItem<T> *back = NULL;
  public:
    Queue() {front = NULL; back = NULL;}
    void enqueue (const T & item);
      /* adds the item to the end of the queue.
         The same item may be added many times. */

    void dequeue (); 
      /* removes the front element of the queue. 
         Throws an EmptyQueueException when called on an empty queue.
         EmptyQueueException should inherit from std::exception. */

    const T & peekFront ();
      /* returns the front element of the queue. 
         Throws an EmptyQueueException when called on an empty queue.
         EmptyQueueException should inherit from std::exception. */

    bool isEmpty ();
      /* returns whether the queue is empty. */

    Queue& operator= (const Queue & other);
};

#include "queueinit.cxx"

#endif