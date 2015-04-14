// set.h
#ifndef SET_H
#define SET_H

#include "map.h"
//#include "mergesort.h"

template <class T>
class Set
{
  public:
    Set ();  // constructor for a new empty set

    Set (const Set<T> & other); // deep copy constructor

    ~Set (); // destructor

    Set& operator= (const Set & other);

    int size () const; // returns the number of elements in the set

    void add (T item); 
      /* Adds the item to the set.
         If the item is already in the set, it should do nothing.
      */

    void remove (const T &item);
     /* Removes the item from the set.
        If the item was not in the set, it should do nothing. */

    bool contains (T item) const;
     /* Returns whether the item is in the set. */

    void merge (const Set<T> & other);
     /* Adds all elements of other to this set. 
        However, will not create duplicates, i.e., it ignores elements
        already in the set. */
    class Iterator {
         /* add any constructors that you feel will be helpful,
            and choose whether to make them public or private. */

         public:
          Iterator(){}
          friend class Set<T>;
           T operator* () const;
              // return the item the iterator is at

           Set<T>::Iterator operator++ ();
              // advances the iterator (pre-increment)

           Set<T>::Iterator operator= (const Set<T>::Iterator & other);
              // assigns the other iterator to this iterator and returns this

           bool operator== (const Set<T>::Iterator & other) const;
              // returns whether this iterator is equal to the other iterator

           bool operator!= (const Set<T>::Iterator & other) const;
              // returns whether this iterator is not equal to the other iterator

         private:
          Iterator(const Set<T> *s);
          //Map<T, int> *current;
          const Set<T> *whoIBelongTo;
          typename Map<T, int>::Iterator mapIterator;
     };

     Set<T>::Iterator begin () const;
       // returns an iterator initialized to the first element

     Set<T>::Iterator end () const;
       /* returns an iterator initialized past the last element,
          to designate that the end of the map has been reached. */

    Set<T> setUnion (const Set<T> & other) const;
      // Returns the union of this and other. Does not alter this.

    Set<T> setIntersection (const Set<T> & other) const;
      // Returns the intersection of this and other. Does not alter this.

  private:
    Map<T, int> internalStorage;
};

#include "setinit.cxx"

#endif