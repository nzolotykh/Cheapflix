// setinit.cpp

#ifndef QUEUEINIT_CXX
#define QUEUEINIT_CXX

#include <iostream>
#include <cstring>

using namespace std;

template <class T>
void Queue<T>::enqueue (const T & item) {
	QueueItem<T> *temp = new QueueItem<T>(item, NULL); //item to be added
	if (front == NULL) {
		//empty queue, make the new item the first item
		front = temp;
	} else {
		//otherwise, put it in the back
		back->next = temp;
	}
	//declare a new back
	back = temp;
}

template <class T>
void Queue<T>::dequeue () {
	if (front == NULL) {
		//empty queue
		throw EmptyQueueException();
	} else {
		//remove from the front, making sure to deallocate memory
		QueueItem<T> *temp;
		temp = front;
		front = front->next;
		delete temp;
	}
}

template <class T>
const T & Queue<T>::peekFront () {
	if (front == NULL) {
		//empty queue
		throw EmptyQueueException();
	} else {
		//self-explanatory
		return front->value;
	}
}

template <class T>
bool Queue<T>::isEmpty () {
	if (front == NULL) {
		//empty queue
		return true;
	} else {
		//not empty queue
		return false;
	}
}

template <class T>
Queue<T>& Queue<T>::operator= (const Queue & other) {
	if (this == &other) {
		return *this;
	}
	//calls the destructor ~Queue()
	while (!this->isEmpty()) {
		this->dequeue();
	}
	//destructor over

	//makes a deep copy
	front = NULL;
	back = NULL;
	QueueItem <T> *o = other.front; //pointer to the head of the other map
	while (o != NULL) {
		this->enqueue(o->value);
		o = o->next;
	}
	return *this;
}

#endif