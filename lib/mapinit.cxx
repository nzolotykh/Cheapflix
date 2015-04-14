// mapinit.cpp

#ifndef MAPINIT_CXX
#define MAPINIT_CXX

#include <iostream>
#include <cstring>

using namespace std;

template <class keyType, class valueType>
Map<keyType, valueType>::Map() {
	//constructor, empty map
	head = NULL;
	tail = NULL;
}

template <class keyType, class valueType>
Map<keyType, valueType>::Map (const Map<keyType, valueType> & other) {
	head = NULL;
	tail = NULL;
	MapItem <keyType, valueType> *p = other.head; //pointer to the head of the other map
	while (p != NULL) {
		this->add(p->key, p->value);
		p = p->next;
	}
}

template <class keyType, class valueType>
Map<keyType, valueType>::~Map() {
	//destructor: runs through every item and deletes it
	MapItem <keyType, valueType> *p = head, *q = NULL;
	while (p != NULL) {
		q = p->next;
		delete p;
		p = q;
	}
}

template <class keyType, class valueType>
Map<keyType, valueType>& Map<keyType, valueType>::operator= (const Map & other) {
	if (this == &other) {
		return *this;
	}
	//calls the destructor ~Map()
	MapItem <keyType, valueType> *p = head, *q = NULL;
	while (p != NULL) {
		q = p->next;
		delete p;
		p = q;
	}
	//destructor over

	//makes a deep copy
	head = NULL;
	tail = NULL;
	MapItem <keyType, valueType> *o = other.head; //pointer to the head of the other map
	while (o != NULL) {
		this->add(o->key, o->value);
		o = o->next;
	}
	return *this;
}

template <class keyType, class valueType>
int Map<keyType, valueType>::size() const {
	int size = 0;
	MapItem <keyType, valueType> *p;
	p = head;
	if (head == NULL) {
		//empty map, size = 0
		return size;
	}
	while (p != NULL) {
		//running through the items and incrementing the size
		p = p->next;
		size++;
	}
	return size;
}

template <class keyType, class valueType>
void Map<keyType, valueType>::add(const keyType &key, valueType &value) {
	MapItem <keyType, valueType> *q;
	q = head;
	bool exist = false;
	for (int i = 0; i < size(); i++) {
		//running through every item and seeing if it matches the one we want to add
		if (q->key == key) {
			//already exists
			exist = true;
			break;
		} else {
			//keep searching
			q = q->next;
		}
	}
	if (exist) {
		throw logic_error ("Key does not exist!\n");
	} else {
		//item does NOT exist, must add
		MapItem <keyType, valueType> *p = new MapItem <keyType, valueType>;
		p->key = key;
		p->value = value; //p will be added
		p->next = NULL;
		if (head == NULL) {
			//empty map, p will be the first item
			tail = p;
			head = p;
		} else {
			//add p to the back
			p->prev = tail;
			tail->next = p;
			tail = p;
		}
	}
}

template <class keyType, class valueType>
void Map<keyType, valueType>::remove (const keyType &key) {
	MapItem <keyType, valueType> *p;
	p = head;
	if (p == NULL) {
		//empty map?
		throw logic_error ("Key does not exist!\n");
	}
	if (head->key == key) {
		//if the key is the head
		head = p->next;
		delete p;
		return;
	}
	p = p->next; //move past the head
	while (p != NULL) {
		//run through items, checking if their keys are the ones we need to remove
		if (p->key == key) {
			if (p == tail) {
				//special case that the item we need to remove is the tial
				p->prev->next = NULL;
				tail = p->prev;
			} else {
				//regular case
				p->prev->next = p->next;
				p->next->prev = p->prev;
			}
			delete p;
			return;
		}
		p = p->next;
	}
	throw logic_error ("Key does not exist!\n");
}

template <class keyType, class valueType>
valueType & Map<keyType, valueType>::get (const keyType & key) const {
	MapItem <keyType, valueType> *p = head;
	for (int i = 0; i < size(); i++) {
		//runs through every item, checking if it's the one we are trying to get
		if (p->key == key) {
			//it is the one, get it!
			return p->value;
		} else {
			//not the one, check the next one!
			p = p->next;
		}
	}
	throw NoSuchElementException();
}

template <class keyType, class valueType>
void Map<keyType, valueType>::merge (const Map<keyType, valueType> & other) {
	MapItem <keyType, valueType> *p = other.head; //pointer to the head of the other map
	MapItem <keyType, valueType> *q;
	while (p != NULL) {
		//running every element of the other map through every element of our map
		//here, we iterate through the other map
		q = head;
		if (q == NULL) {
			//our map is empty, so we just need to add everything from the other map
			while (p != NULL) {
				this->add(p->key, p->value);
				p = p->next;
			}
			return;
		}
		while (q != NULL) {
			//our map is not empty
			//here, we iterate through our map
			if (p->key == q->key) {
				//match found, we do not need to add this to our map
				break;
			}
			q = q->next; //moving on..
			if (q == NULL) {
				//no match found, add the element p from the other map to our map
				this->add (p->key, p->value);
			}
		}
		p = p->next;
	}
}

template <class keyType, class valueType>
typename Map<keyType,valueType>::Iterator Map<keyType, valueType>::begin () const{
	if (head == NULL) {
		throw NoSuchElementException();
	} else {
		Map<keyType,valueType>::Iterator begin(this, head);
		begin.currentPair = new Pair<keyType,valueType>(head->key, head->value);
		return begin;
	}
}

template <class keyType, class valueType>
typename Map<keyType,valueType>::Iterator Map<keyType, valueType>::end () const {
	Map<keyType,valueType>::Iterator end(this, NULL);
	return end;
}

template <class keyType, class valueType>
Map<keyType, valueType>::Iterator::Iterator() {

}

template <class keyType, class valueType>
Map<keyType, valueType>::Iterator::Iterator(const Map<keyType,valueType> *m, MapItem <keyType, valueType> *c) {
	//constructor
	whoIBelongTo = m;
	current = c;
}

/*template <class keyType, class valueType>
Map<keyType, valueType>::Iterator::~Iterator() {
	if (currentPair != NULL) {
		delete currentPair;
	}
}*/

template <class keyType, class valueType>
Pair<keyType, valueType> Map<keyType, valueType>::Iterator::operator* () const {
	if (currentPair != NULL){
		return *currentPair;
	} else {
		throw NoSuchElementException();
	}
}

template <class keyType, class valueType>
typename Map<keyType,valueType>::Iterator Map<keyType, valueType>::Iterator::operator++ () {
	current = current->next;
	if(current != NULL) {
		currentPair->first = current->key;
		currentPair->second = current->value;
	}
	return *this;
}

template <class keyType, class valueType>
typename Map<keyType,valueType>::Iterator Map<keyType, valueType>::Iterator::operator= (const Map<keyType,valueType>::Iterator & other) {
	current = other.current;
	currentPair = other.currentPair;
	whoIBelongTo = other.whoIBelongTo;
	return *this;
}

template <class keyType, class valueType>
bool Map<keyType, valueType>::Iterator::operator== (const Map<keyType,valueType>::Iterator & other) const {
	return ((current == other.current) && (whoIBelongTo == other.whoIBelongTo));
}

template <class keyType, class valueType>
bool Map<keyType, valueType>::Iterator::operator!= (const Map<keyType,valueType>::Iterator & other) const {
	return !(*this == other);
}

#endif