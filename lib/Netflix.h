#ifndef NETFLIX_H
#define NETFLIX_H

#include <sstream>
#include <fstream>
#include <QtGui>
#include <exception>
#include "../header/User.h"

class Netflix {
public:
	Netflix();
	Netflix(std::string userFile, std::string movieFile);
	User* getUser();
	std::map<std::string, std::set<Movie*>*>* getKeywordsMap();
	bool readUsers(); //reads in the files, returning true if it is successful
	bool readMovies(); //reads in the movies, returning true if it is successful
	bool login(QString userID); //both logs in the user (assigns new current user / movie) and returns if it was successful
	void logout();
	std::map<std::string, Movie*>::iterator* searchTitle(QString movieTitle); //searches return iterators so that we could manipulate the objects
	std::map<std::string, std::set<Movie*>*>::iterator* searchKeyword(QString keyword); //inside our main menu implementation
	bool addUser(QString userID, QString userName); //similar to the login idea
	void returnMovie(int score);
	std::map<std::string, User*>* getUserMap();
	void similaritiesCalculation();
	std::map<std::string, std::set<Movie*>*>::iterator* searchActor(QString actor); //inside our main menu implementation
	std::map<std::string, std::set<Movie*>*>* getActorsMap();
	void Dijkstra (double** basicSimilarity, double** refinedSimilarity, int u);
	void traversal(int u, int v, bool* visited, double** basicSimilarity, double** refinedSimilarity);

private:
	std::map<std::string, User*>* userMap; //all self-explanatory
	std::map<std::string, Movie*>* movieMap;
	std::map<std::string, std::set<Movie*>*>* keywordsMap;
	std::map<std::string, std::set<Movie*>*>* actorsMap;
	User* currentUser;
	std::string userFile;
	std::string movieFile;
	std::string movieTitleSearch;
};

#endif