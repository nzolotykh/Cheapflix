// setinit.cpp

#ifndef SETINIT_CXX
#define SETINIT_CXX

#include <iostream>
#include <cstring>

using namespace std;

template <class T>
Set<T>::Set() {
	//nothing to dynamically allocate
}

template <class T>
Set<T>::Set (const Set<T> & other) {
	//no dynamic allocation, once again, so this is fine
	internalStorage = other.internalStorage;
}

template <class T>
Set<T>::~Set() {
	//nothing to delete!
}

template <class T>
Set<T>& Set<T>::operator= (const Set & other) {
	//similar to the operator overload for the Map type (but simpler)
	if (this == &other) {
		return *this;
	}
	internalStorage = other.internalStorage;
	return *this;
}

template <class T>
int Set<T>::size() const {
	//most of these functions will simply use the Map functionality
	return internalStorage.size();
}

template <class T>
void Set<T>::add(T item) {
	int x = 1; //arbitrary choice of an integer
	internalStorage.add(item, x);
}

template <class T>
void Set<T>::remove(const T &item) {
	internalStorage.remove(item);
}

template <class T>
bool Set<T>::contains (T item) const {
	bool success = false;
	try {
		if (internalStorage.get(item)) {
			//if contains, set the bool to true
			success = true;
		}
	} catch (NoSuchElementException e) {
		return success;
	}

	return success;
}

template <class T>
void Set<T>::merge (const Set<T> & other) {
	//similar to merging maps
	internalStorage.merge(other.internalStorage);
}

/*template <class T>
void Set<T>::first () {
	//maps implementation
	internalStorage.first();
}

template <class T>
void Set<T>::next () {
	internalStorage.next();
}

template <class T>
const T& Set<T>::getCurrent () {
	return internalStorage.getCurrentKey();
}*/

template <class T>
Set<T>::Iterator::Iterator (const Set<T> *s) {
	whoIBelongTo = s;
	//mapIterator = whoIBelongTo->internalStorage.begin();
}

template <class T>
T Set<T>::Iterator::operator* () const {
	return (*mapIterator).first;
}

template <class T>
typename Set<T>::Iterator Set<T>::Iterator::operator++ () {
	mapIterator = ++mapIterator;
	return *this;
}

template <class T>
typename Set<T>::Iterator Set<T>::Iterator::operator= (const Set<T>::Iterator & other) {
	whoIBelongTo = other.whoIBelongTo;
	mapIterator = other.mapIterator;
	return *this;
}
  // assigns the other iterator to this iterator and returns this

template <class T>
bool Set<T>::Iterator::operator== (const Set<T>::Iterator & other) const {
	return ((whoIBelongTo == other.whoIBelongTo) && (this->mapIterator == other.mapIterator));
}
  // returns whether this iterator is equal to the other iterator

template <class T>
bool Set<T>::Iterator::operator!= (const Set<T>::Iterator & other) const {
	return !(*this == other);
}

template <class T>
typename Set<T>::Iterator Set<T>::begin() const {
	Set<T>::Iterator *begin = new Set<T>::Iterator(this);
	begin->mapIterator = this->internalStorage.begin();
	return *begin;
}

template <class T>
typename Set<T>::Iterator Set<T>::end() const {
	Set<T>::Iterator end(this);
	end.mapIterator = this->internalStorage.end();
	return end;
}

template <class T>
Set<T> Set<T>::setUnion (const Set<T> & other) const {
	//since union is defined as the set with elements from both sets,
	//we can just merge the two sets together into one
	Set<T> unionResult;
	unionResult.merge(*this);
	unionResult.merge(other);
	return unionResult;
}
  // Returns the union of this and other. Does not alter this.

template <class T>
Set<T> Set<T>::setIntersection (const Set<T> & other) const {
	Set<T> intersectionResult;
	Set<T>::Iterator thisSetIterator;
	Set<T>::Iterator otherSetIterator;
	try {
		thisSetIterator = this->begin();
		otherSetIterator = other.begin();
	} catch (NoSuchElementException e) {
		return intersectionResult;
	}

	while(thisSetIterator != this->end()) {
		//runs through each set and compares each value to see if they match
		//if they do, those values will be added to the resulting set
		while (otherSetIterator != other.end()) {
			if ((*thisSetIterator) == (*otherSetIterator)) {
				intersectionResult.add(*thisSetIterator);
			}
			++otherSetIterator;
		}
		otherSetIterator = other.begin();
		++thisSetIterator;
	}

	//if (intersectionResult.size() == 0) {
	//	throw NoSuchElementException();
	//} else {
		return intersectionResult;
	//}
}
  // Returns the intersection of this and other. Does not alter this.

#endif