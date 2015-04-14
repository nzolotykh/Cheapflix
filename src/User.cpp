#include "../header/User.h"

User::User (std::string ID, std::string name) {
	//simple constructor
	this->ID = ID;
	this->name = name;
	rentedMovie = NULL;
	moviesQueue = new std::queue<Movie*>;
	ratings = new std::map<Movie*, int>;
	ratedMovies = new std::set<Movie*>;
}

User::User (const User & other) {
	this->ID = other.ID;
	this->name = other.name;
}

User::~User () {
	//nothing to deallocate
}

std::string User::getID () const {
	return ID;
}

std::string User::getName () const {
	return name;
}

std::queue<Movie*>* User::movieQueue () {
	//self-explanatory
	return moviesQueue;
}

void User::rentMovie (Movie *m) {
	if (rentedMovie == NULL) {
		//if no movie, we can rent
		rentedMovie = m;
	}
	//there is a movie, so nothing happens
}

void User::returnMovie () {
	//throw out the rented movie
	rentedMovie = NULL;
}

Movie* User::currentMovie () {
	//self-explanatory
	return rentedMovie;
}

std::map<Movie*, int>* User::getRatings() {
	//self-explanatory
	return ratings;
}

std::set<Movie*>* User::getRatedMovies() {
	//self-explanatory
	return ratedMovies;
}