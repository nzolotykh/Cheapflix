#include "Netflix.h"

User* Netflix::getUser() {
	//returns the user
	return currentUser;
}

std::map<std::string, std::set<Movie*>*>* Netflix::getKeywordsMap() {
	//returns the keywords map
	return keywordsMap;
}

std::map<std::string, std::set<Movie*>*>* Netflix::getActorsMap() {
	//returns the actors map
	return actorsMap;
}

bool Netflix::readUsers() {
	//function that reads in all the users from the text file into the map of users
	std::ifstream myfile (userFile.c_str());

	if (!myfile.is_open()) {
		//cannot open the file
		return false;
	} else {
		//file is opened
		std::string userID;
		std::string userName = "";
		std::string rentedMovieTitle;
		User* user;
		while(true) {
			std::stringstream ss;
			std::string line;
			std::string reader; //used to see what each line starts with
			getline(myfile, line);
			if (line == "BEGIN QUEUE") {
				//this is a special case, since BEGIN QUEUE is two words, and checking
				//only the first word using stringstream would not work
				std::string queueReader;
				getline(myfile, queueReader);
				while (queueReader != "END QUEUE") {
					//enqueue until we reach the end of the queue
					user->movieQueue()->push(movieMap->find(queueReader)->second);
					getline(myfile, queueReader);
				}
				continue;
			}
			if (line == "BEGIN RATINGS") {
				//this is a special case, since BEGIN RATINGS is two words, and checking
				//only the first word using stringstream would not work
				getline(myfile, line);
				while (line != "END RATINGS") {
					int score;
					std::string partToAdd;
					std::string ratedMovieTitle;
					ss << line;
					ss >> score;
					while(ss>>partToAdd) {
						//adds everything that's left on the line
						if (ratedMovieTitle.length() == 0) {
							ratedMovieTitle = partToAdd;
						} else {
							ratedMovieTitle = ratedMovieTitle + " " + partToAdd;
						}
					}
					user->getRatings()->insert(std::pair<Movie*, int> (movieMap->find(ratedMovieTitle)->second, score)); //add a new movie to the user's ratings
					user->getRatedMovies()->insert(movieMap->find(ratedMovieTitle)->second);
					getline(myfile, line);
					ss.str(std::string());
					ss.clear();
				}
				continue;
			}
			ss << line;
			ss >> reader;
			if (reader == "BEGIN") {
				//record the ID
				ss >> userID;
				continue;
			}
			if (reader == "NAME:") {
				//record the name, adding it by parts
				std::string partToAdd;
				while(ss>>partToAdd) {
					//adds everything on the line
					if (userName.length() == 0) {
						userName = partToAdd;
					} else {
						userName = userName + " " + partToAdd;
					}
				}
				user = new User(userID, userName); //creates a basic user, with no queue and no checked out movie
				continue;
			}
			if (reader == "MOVIE:") {
				//record the rented movie title, adding it by parts
				std::string partToAdd;
				while(ss>>partToAdd) {
					//adds everything on the line
					if (rentedMovieTitle.length() == 0) {
						rentedMovieTitle = partToAdd;
					} else {
						rentedMovieTitle = rentedMovieTitle + " " + partToAdd;
					}
				}
				user->rentMovie(movieMap->find(rentedMovieTitle)->second); //adds the rented movie to the user

				continue;
			}
			if (reader == "END") {
				//we are done reading in the user info
				//now, add to the map
				userMap->insert(std::pair<std::string, User*> (userID, user));
				userName = "";
				continue;
			}
			break;
		}
		myfile.close();
		return true; //finished adding successfully
	}
}

bool Netflix::readMovies() {
	//function that reads in all the movies from the input file into the movie map
	std::ifstream myfile (movieFile.c_str());
	if (!myfile.is_open()) {
		//cannot open the file
		return false;
	} else {
		//file is opened
		std::string movieTitle = "";
		std::string movieKeyword = "";
		std::string actorName = "";
		while(true) {
			std::stringstream ss;
			std::string line;
			std::string reader; //same functionality as in the readUsers
			getline(myfile, line);
			ss << line;
			ss >> reader;
			if (reader == "BEGIN") {
				//reading the name, and creating the movie
				std::string partToAdd;
				while(ss>>partToAdd) {
					//adding everything on one line into the name
					if (movieTitle.length() == 0) {
						movieTitle = partToAdd;
					} else {
						movieTitle = movieTitle + " " + partToAdd;
					}
				}
				Movie* movie = new Movie(movieTitle);
				movieMap->insert(std::pair<std::string, Movie*> (movieTitle, movie));
				continue;
			}
			if (reader == "KEYWORD:") {
				//adding keywords to the movie
				std::string partToAdd;
				while(ss>>partToAdd) {
					if (movieKeyword.length() == 0) {
						movieKeyword = partToAdd;
					} else {
						movieKeyword = movieKeyword + " " + partToAdd;
					}
				}
				movieMap->find(movieTitle)->second->addKeyword(movieKeyword); //adds the keyword to the list of keywords for the movie
				//this is where we will add the keyword to the keywordMap and then add the movie
				std::string compareKey;
				std::string lowMovieKey = movieKeyword;
				transform(lowMovieKey.begin(), lowMovieKey.end(), lowMovieKey.begin(), ::tolower);
				std::map<std::string, std::set<Movie*>*>::iterator keywordsMapIterator;
				if (!keywordsMap->empty()) {
					keywordsMapIterator = keywordsMap->begin();
				} else {
					//map of keywords is empty
					//we need to add the first element
					std::set<Movie*>* movieSet = new std::set<Movie*>;
					movieSet->insert(movieMap->find(movieTitle)->second);
					keywordsMap->insert(std::pair<std::string, std::set<Movie*>*> (movieKeyword, movieSet));
					keywordsMapIterator = keywordsMap->begin();
				}
				while (true) {
					//this is the loop for the search
					compareKey = keywordsMapIterator->first;
					transform(compareKey.begin(), compareKey.end(), compareKey.begin(), ::tolower);
					if (compareKey == lowMovieKey) {
						//found a match
						//we need to add the movie to the set of the keyword
						keywordsMap->find(keywordsMapIterator->first)->second->insert(movieMap->find(movieTitle)->second);
						break;
					} else {
						//no match yet, move on
						if (++keywordsMapIterator != keywordsMap->end()) {
						} else {
							std::set<Movie*>* movieSet = new std::set<Movie*>;
							movieSet->insert(movieMap->find(movieTitle)->second);
							keywordsMap->insert(std::pair<std::string, std::set<Movie*>*> (movieKeyword, movieSet));
							break;
						}
						continue;
					}
				}
				movieKeyword = "";
				continue;
			}
			if (reader == "ACTOR:") {
				//adding actors to the movie
				std::string partToAdd;
				while(ss>>partToAdd) {
					if (actorName.length() == 0) {
						actorName = partToAdd;
					} else {
						actorName = actorName + " " + partToAdd;
					}
				}
				movieMap->find(movieTitle)->second->addActor(actorName); //adds the actor to the list of actors for the movie
				//this is where we will add the actor to the actorsMap and then add the movie
				std::string compareActor;
				std::string lowActorName = actorName;
				transform(lowActorName.begin(), lowActorName.end(), lowActorName.begin(), ::tolower);
				std::map<std::string, std::set<Movie*>*>::iterator actorsMapIterator;
				if (!actorsMap->empty()) {
					actorsMapIterator = actorsMap->begin();
				} else {
					//map of actors is empty
					//we need to add the first element
					std::set<Movie*>* movieSet = new std::set<Movie*>;
					movieSet->insert(movieMap->find(movieTitle)->second);
					actorsMap->insert(std::pair<std::string, std::set<Movie*>*> (actorName, movieSet));
					actorsMapIterator = actorsMap->begin();
				}

				while (true) {
					//this is the loop for the search
					compareActor = actorsMapIterator->first;
					transform(compareActor.begin(), compareActor.end(), compareActor.begin(), ::tolower);
					if (compareActor == lowActorName) {
						//found a match
						//we need to add the movie to the set of the keyword
						actorsMap->find(actorsMapIterator->first)->second->insert(movieMap->find(movieTitle)->second);
						break;
					} else {
						//no match yet, move on
						if (++actorsMapIterator != actorsMap->end()) {
						} else {
							std::set<Movie*>* movieSet = new std::set<Movie*>;
							movieSet->insert(movieMap->find(movieTitle)->second);
							actorsMap->insert(std::pair<std::string, std::set<Movie*>*> (actorName, movieSet));
							break;
						}
						continue;
					}
				}
				actorName = "";
				continue;
			}
			if (reader == "END") {
				//we are done reading in the movie info
				//since we already created the movie and added it, we don't need to do anything
				movieTitle = "";
				continue;
			}
			break;
		}
		myfile.close();
		return true; //finished adding successfully
	}
}

bool Netflix::login(QString userID) {
	//login check, to see if the user exists
	std::string userIDString = userID.toStdString();
	//bool userExist;

	if (userMap->find(userIDString) != userMap->end()) {
		//user exists
		currentUser = userMap->find(userIDString)->second;
		return true;
	} else {
		return false;
	}

}

void Netflix::logout() {
	//outputs to the users file when logging out
	std::map<std::string, User*>::iterator userMapIterator = userMap->begin();

	std::ofstream myfile (userFile.c_str(), std::ofstream::out | std::ofstream::trunc); //output stream

	while (userMapIterator != userMap->end()) {
		myfile << "BEGIN ";
		myfile << userMapIterator->second->getID() << "\n";
		myfile << "NAME: ";
		myfile << userMapIterator->second->getName() << "\n";
		if (userMapIterator->second->currentMovie() != NULL) {
			//user has a checked out movie
			myfile << "MOVIE: ";
			myfile << userMapIterator->second->currentMovie()->getTitle() << "\n";
		}
		myfile << "BEGIN QUEUE\n";
		std::queue<Movie*> tempQueue;
		tempQueue = *(userMapIterator->second->movieQueue()); //to implement this correctly, we had to overload the = operator for the queue
		while (!tempQueue.empty()) {
			//printing out all the movies in user's queue
			myfile << tempQueue.front()->getTitle() << "\n";
			tempQueue.pop(); // no need to try-catch since we check that it's not empty
		}

		myfile << "END QUEUE\n";

		if (!userMapIterator->second->getRatings()->empty()) {
			//there are ratings present
			myfile << "BEGIN RATINGS\n";
			std::map<Movie*, int>::iterator ratingsMapIterator = userMapIterator->second->getRatings()->begin();
			while (ratingsMapIterator != userMapIterator->second->getRatings()->end()) {
				//runs through all movies of a user's ratings map
				myfile << ratingsMapIterator->second << " " << ratingsMapIterator->first->getTitle() << '\n';
				++ratingsMapIterator;
			}
			myfile << "END RATINGS\n";
		}

		myfile << "END\n";
		++userMapIterator;
	}
	
    myfile.close();
}

bool Netflix::addUser(QString userID, QString userName) {
	//function to manually add a user
	std::string userIDString = userID.toStdString();
	std::string userNameString = userName.toStdString();
	//bool userExist = false;
	if (userMap->find(userIDString) != userMap->end()) {
		//user exists, do nothing
		return false;
	}

	//at this point, user does not exist, we can proceed with adding

	User *newUser = new User(userIDString, userNameString); //making the user and adding him/her to the map
	userMap->insert(std::pair<std::string, User*> (userIDString, newUser));

	//note: we don't need to do any checks for whether or not the file is open, since
	//it will open if we did not receive an error when loading the data in
	//(we do the check there)

	std::ofstream myfile (userFile.c_str(), std::ofstream::out | std::ofstream::app); //output stream

	//just record the data
	myfile << "BEGIN ";
	myfile << userIDString << "\n";
	myfile << "NAME: ";
	myfile << userNameString << "\n";
	myfile << "END\n";

    myfile.close();
    return true; //user was successfully added
}

std::map<std::string, Movie*>::iterator* Netflix::searchTitle(QString movieTitle) {
	//searching by title
	movieTitleSearch = movieTitle.toStdString();
	std::string compareTitle; //will be used for searching
	transform(movieTitleSearch.begin(), movieTitleSearch.end(), movieTitleSearch.begin(), ::tolower);
	std::map<std::string, Movie*>::iterator* movieMapIterator;
	if (!movieMap->empty()) {
		movieMapIterator = new std::map<std::string, Movie*>::iterator;
		*movieMapIterator = movieMap->begin();
	} else {
		//no movies to search (the map is empty)
		movieMapIterator = NULL; //returning NULL if it fails
		return movieMapIterator; //same will be done for the other search
	}

	while (true) {
		//this is the loop for the search
		//the searched name is forced to be lower-case, and every movie name that it is
		//being compared to is also forced to be lower-case, so that a proper comparison
		//(case-insensitive) can be made
		compareTitle = (*movieMapIterator)->first;
		transform(compareTitle.begin(), compareTitle.end(), compareTitle.begin(), ::tolower);
		if (compareTitle == movieTitleSearch) {
			//found a match!
			return movieMapIterator;

		} else {
			//moves on to the next movie to be searched if no match was found
			if (++(*movieMapIterator) != movieMap->end()) {
			} else {
				movieMapIterator = NULL; //search failed
				return movieMapIterator;
			}
		}
	}
}

std::map<std::string, std::set<Movie*>*>::iterator* Netflix::searchKeyword(QString keyword) {
	//function to search by keyword
	std::string movieKey = keyword.toStdString();
	std::string compareKey; //will be used for searching

	transform(movieKey.begin(), movieKey.end(), movieKey.begin(), ::tolower);
	std::map<std::string, std::set<Movie*>*>::iterator* keywordsMapIterator;
	std::set<Movie*>::iterator movieSetIterator;

	if (!keywordsMap->empty()) {
		keywordsMapIterator = new std::map<std::string, std::set<Movie*>*>::iterator;
		*keywordsMapIterator = keywordsMap->begin();
	} else {
		//nothing in the keywords map
		keywordsMapIterator = NULL; //search failed
		return keywordsMapIterator;
	}

	while (true) {
		//this is the loop for the search
		//the searched key is forced to be lower-case, and every movie key that it is
		//being compared to is also forced to be lower-case, so that a proper comparison
		//(case-insensitive) can be made
		if (!keywordsMap->find((*keywordsMapIterator)->first)->second->empty()) {
			movieSetIterator = keywordsMap->find((*keywordsMapIterator)->first)->second->begin();
		} else {
			//nothing in the movies set of the keyword map
			//let's move on and check the next keyword
			//(technically, this situation should never happen since whenever a keyword)
			//(is added to the map, at least one movie will be assigned to it)
			if(++(*keywordsMapIterator) != keywordsMap->end()) {

			} else {
				keywordsMapIterator = NULL; //search failed
				return keywordsMapIterator;
			}
			continue;
		}

		compareKey = (*keywordsMapIterator)->first;
		transform(compareKey.begin(), compareKey.end(), compareKey.begin(), ::tolower);
		if (compareKey == movieKey) {
			//found a match
			return keywordsMapIterator; //search successful!
		} else {
			//moves on to the next keyword to be searched if no match was found
			if(++(*keywordsMapIterator) != keywordsMap->end()) {

			} else {
				//finished running through all the keywords
				keywordsMapIterator = NULL; //search failed
				return keywordsMapIterator;
			}
			continue;
		}
	}
}

std::map<std::string, std::set<Movie*>*>::iterator* Netflix::searchActor(QString actor) {
	//function to search by actor
	std::string actorName = actor.toStdString();
	std::string compareActor; //will be used for searching

	transform(actorName.begin(), actorName.end(), actorName.begin(), ::tolower);
	std::map<std::string, std::set<Movie*>*>::iterator* actorsMapIterator;
	std::set<Movie*>::iterator movieSetIterator;

	if (!actorsMap->empty()) {
		actorsMapIterator = new std::map<std::string, std::set<Movie*>*>::iterator;
		*actorsMapIterator = actorsMap->begin();
	} else {
		//nothing in the actors map
		actorsMapIterator = NULL; //search failed
		return actorsMapIterator;
	}

	while (true) {
		//this is the loop for the search
		//the searched key is forced to be lower-case, and every movie key that it is
		//being compared to is also forced to be lower-case, so that a proper comparison
		//(case-insensitive) can be made
		if (!actorsMap->find((*actorsMapIterator)->first)->second->empty()) {
			movieSetIterator = actorsMap->find((*actorsMapIterator)->first)->second->begin();
		} else {
			//nothing in the movies set of the keyword map
			//let's move on and check the next keyword
			//(technically, this situation should never happen since whenever a keyword)
			//(is added to the map, at least one movie will be assigned to it)
			if(++(*actorsMapIterator) != actorsMap->end()) {

			} else {
				actorsMapIterator = NULL; //search failed
				return actorsMapIterator;
			}
			continue;
		}

		compareActor = (*actorsMapIterator)->first;
		transform(compareActor.begin(), compareActor.end(), compareActor.begin(), ::tolower);
		if (compareActor == actorName) {
			//found a match
			return actorsMapIterator; //search successful!
		} else {
			//moves on to the next keyword to be searched if no match was found
			if(++(*actorsMapIterator) != actorsMap->end()) {

			} else {
				//finished running through all the actors
				actorsMapIterator = NULL; //search failed
				return actorsMapIterator;
			}
			continue;
		}
	}
}

void Netflix::returnMovie(int score) {
	if (score == -1) {
		//user chose NOT to rate
		getUser()->returnMovie();
	} else {
		//user inputted a rating from 1 to 5
		if (getUser()->getRatedMovies()->find(getUser()->currentMovie()) != getUser()->getRatedMovies()->end()) {
			//the movie has been rated before
			getUser()->getRatings()->erase(getUser()->currentMovie());
			getUser()->getRatings()->insert(std::pair<Movie*, int> (getUser()->currentMovie(), score));
		} else {
			getUser()->getRatings()->insert(std::pair<Movie*, int> (getUser()->currentMovie(), score));
			getUser()->getRatedMovies()->insert(getUser()->currentMovie());
		}
		getUser()->returnMovie();
	}
}

std::map<std::string, User*>* Netflix::getUserMap() {
	//self-explanatory
	return userMap;
}

void Netflix::similaritiesCalculation() {
	//BASIC SIMILARITY CALCULATION
	double** basicSimilarity = new double* [userMap->size()]; //final similarity will be made up of as many values as there are users, minus 1 (the current user)
	for (unsigned int i = 0; i < userMap->size(); i++) {
		basicSimilarity[i] = new double [userMap->size()];
	}
	int userCounter1 = 0;
	if (userMap->empty()) {
		//no users, don't need to calculate anything
		//just terminate the function
		return;
	}
	std::map<std::string, User*>::iterator userMapIteratorA = userMap->begin();

	while (userMapIteratorA != userMap->end()) {
		//runs through all users
		std::map<std::string, User*>::iterator userMapIteratorB = userMap->begin();
		int userCounter2 = 0;
		while (userMapIteratorB != userMap->end()) {
			//also runs through all users
			if (userMapIteratorB->second != userMapIteratorA->second) {
				//it is not the same user
				std::set<Movie*> bothRated;
				set_intersection(userMapIteratorA->second->getRatedMovies()->begin(), userMapIteratorA->second->getRatedMovies()->end(), 
								userMapIteratorB->second->getRatedMovies()->begin(), userMapIteratorB->second->getRatedMovies()->end(),
								std::inserter(bothRated, bothRated.begin()));
				if (bothRated.size() == 0) {
					//two users have not rated the same movies
					basicSimilarity[userCounter1][userCounter2] = 1;
					userCounter2++;
				} else {
					//two users have movies that they both rated!
					double* ratingsSimilarity = new double[bothRated.size()]; //stores similarity scores for all movies they both rated
					int bothRatedCounter = 0;
					basicSimilarity[userCounter1][userCounter2] = 0;
					std::set<Movie*>::iterator bothRatedSetIterator = bothRated.begin();
					while (bothRatedSetIterator != bothRated.end()) {
						//runs through all movies both have rated
						int scoreA = userMapIteratorA->second->getRatings()->find(*bothRatedSetIterator)->second;
						int scoreB = userMapIteratorB->second->getRatings()->find(*bothRatedSetIterator)->second;
						ratingsSimilarity[bothRatedCounter] = abs(scoreA - scoreB)/4.0;
						bothRatedCounter++;
						++bothRatedSetIterator;
					}
					for (unsigned int i = 0; i < bothRated.size(); i++) {
						//summing all the basic similarities
						basicSimilarity[userCounter1][userCounter2] = basicSimilarity[userCounter1][userCounter2] + ratingsSimilarity[i];
					}
					//averaging them
					basicSimilarity[userCounter1][userCounter2] = basicSimilarity[userCounter1][userCounter2]/bothRated.size();
					userCounter2++;
				}
			} else {
				//iterators are pointing to the same user
				basicSimilarity[userCounter1][userCounter2] = 0;
				userCounter2++;
			}
			++userMapIteratorB;
		}
		++userMapIteratorA;
		userCounter1++;
	}
	std::cout << "Basic Similarity ratings: " << std::endl;
	for (unsigned int i = 0; i < userMap->size(); i ++) {
		for (unsigned int j = 0; j < userMap->size(); j++) {
			std::cout << "(" << i << ", " << j << "): " << basicSimilarity[i][j] << std::endl;
		}
	}

	double** refinedSimilarity = new double* [userMap->size()];
	for (unsigned int i = 0; i < userMap->size(); i++) {
		//initiation of stuff
		refinedSimilarity[i] = new double [userMap->size()];
	}
	for (unsigned int i = 0; i < userMap->size(); i++) {
		for (unsigned int j = 0; j < userMap->size(); j++) {
			//first all values of refined similarity are the same as the basic ones
			refinedSimilarity[i][j] = basicSimilarity[i][j];
		}
	}
	
	for (unsigned int j = 0; j < userMap->size(); j++) {
		//first all values of refined similarity are the same as the basic ones
		Dijkstra(basicSimilarity, refinedSimilarity, j);
	}

	std::cout << "Refined Similarity ratings: " << std::endl;
	for (unsigned int i = 0; i < userMap->size(); i ++) {
		for (unsigned int j = 0; j < userMap->size(); j++) {
			std::cout << "(" << i << ", " << j << "): " << refinedSimilarity[i][j] << std::endl;
		}
	}

}

void Netflix::Dijkstra (double** basicSimilarity, double** refinedSimilarity, int u) {
	bool visited[userMap->size()];
	for (unsigned int i = 0; i < userMap->size(); i++) {
		visited[i] = false; //no node is visited yet
	}

	int smallest = 1;
	int index; //index of the smallest outgoing edge of u
	visited[u] = true;

	//here we find the smallest edge emanating from u, and then start traversing there
	for (unsigned int i = 0; i < userMap->size(); i++) {
		if (!visited[i]) {
			//all non-visited nodes
			if (basicSimilarity[u][i] <= smallest) {
				smallest = basicSimilarity[u][i];
				index = i;
			}
		}
	}
	traversal(u, index, visited, basicSimilarity, refinedSimilarity);

}

void Netflix::traversal(int u, int v, bool* visited, double** basicSimilarity, double** refinedSimilarity) {
	//u is the index of the node we are running Dijkstra on, should never change
	//v is the index of the node we are currently looking at
	bool allVisited = true;
	for (unsigned int i = 0; i < userMap->size(); i++) {
		if (!visited[i]) {
			allVisited = false;
			break;
		}
	}
	if (allVisited) {
		//all nodes have been visited, terminate!
		return;
	}
	int smallest = 1;
	int index;
	visited[v] = true;
	for (unsigned int i = 0; i < userMap->size(); i++) {
		if (!visited[i]) {
			//all non-visited nodes
			if (basicSimilarity[v][i] <= smallest) {
				smallest = basicSimilarity[v][i];
				index = i;
			}
		}
	}
	if (refinedSimilarity[u][v] + basicSimilarity[v][index] < refinedSimilarity[u][index]) {
		refinedSimilarity[u][index] = refinedSimilarity[u][v] + basicSimilarity[v][index];
	}

	traversal(u, index, visited, basicSimilarity, refinedSimilarity);
}

Netflix::Netflix(std::string userFile, std::string movieFile) {
	//constructs stuff
	this->userFile = userFile;
	this->movieFile = movieFile;
	userMap = new std::map<std::string, User*>;
	movieMap = new std::map<std::string, Movie*>;
	keywordsMap = new std::map<std::string, std::set<Movie*>*>;
	actorsMap = new std::map<std::string, std::set<Movie*>*>;
}