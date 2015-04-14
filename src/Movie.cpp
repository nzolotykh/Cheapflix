#include "../header/Movie.h"

Movie::Movie(std::string title) {
	this->title = title;
}

Movie::Movie (const Movie & other) {
	this->title = other.title;
	this->keywords = other.keywords;
	this->actors = other.actors;
}

Movie::~Movie() {
	//nothing to deallocate
}

std::string Movie::getTitle () const {
	return title;
}

void Movie::addKeyword (std::string keyword) {
	//use the set implementation
	keywords.insert(keyword);
}

std::set<std::string> Movie::getAllKeywords () const {
	return keywords;
}

void Movie::addActor (std::string actor) {
	//use the set implementation
	actors.insert(actor);
}

std::set<std::string> Movie::getAllActors () const {
	return actors;
}