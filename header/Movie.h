#ifndef MOVIE_H
#define MOVIE_H
#include <iostream>
#include <algorithm>
#include <exception>
#include <stdexcept>
#include <map>
#include <set>
#include <queue>

//#include "../lib/set.h"

class Movie {
  public: 
    Movie (std::string title);       // constructor for a movie with the given title

    Movie (const Movie & other);  // copy constructor

    ~Movie ();                  // destructor

    std::string getTitle () const;   // returns the title of the movie

    void addKeyword (std::string keyword); 
      /* Adds the (free-form) keyword to this movie.
         If the exact same keyword (up to capitalization) was already
         associated with the movie, then the keyword is not added again. */

    std::set<std::string> getAllKeywords () const;
      /* Returns a set of all keywords associated with the movie. */

    void addActor (std::string actor);

    std::set<std::string> getAllActors() const;

  private:
    std::string title;
    std::set<std::string> keywords;
    std::set<std::string> actors;
    // you get to decide what goes here
};

#endif