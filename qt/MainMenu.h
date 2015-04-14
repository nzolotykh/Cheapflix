#ifndef MAINMENU_H
#define MAINMENU_H

#include <QtGui>
#include "../lib/Netflix.h"
#include "NewUser.h"
#include "LoggedInWindow.h"
#include "SearchWindow.h"
#include "RateMovie.h"
#include "RatingsWindow.h"

//This is the main menu window, from which all other windows and qt implementations will be called
//this is why it contains one copy of each window, and the main netflix file that contains all the
//info regarding storage
//NOTE: in my code, all the graphical objects that comprise each window are separated into their corresponding parts
//this way, it is easier to see what belongs to where and which object is responsible for which part
//this is why there are empty spaces between groups of objects

class MainMenu : public QDialog {
	Q_OBJECT
public:
	MainMenu(Netflix *netflix); //constructor needs to have a netflix file
private slots:
	void stateMachine(); //this will keep track of which window we are currently in
	void loginCheck(); //checks if it's okay to login / updates the window
	void returnMovie(); //the rest of the functions are self-explanatory
	void rentMovie();
	void rateMovie();
	void deleteMovie();
	void backQueue();
	void searchTitle(); 
	void searchKeyword();
	void searchActor();
	void nextMovie();
	void nextMovie2();
	void logout(); //logs out, recording the data back into txt file
	void addUser();
	void addToQueue();
	void viewRatings();
	void confirmChangedRatings();
signals:
	void loginSuccessful(); //self-explanatory
	void userAdded();
	void searchSuccessful();
	void canRateMovie();
	void canViewRatings();
private:
	void createLineEdit();
	void createButtons();

	QLabel *titleLabel;

	QHBoxLayout *loginHLayout;
	QLineEdit *loginLineEdit;
	QLabel *loginLineEditLabel;

	QHBoxLayout *loginButtonLayout;
	QPushButton *loginButton;
	QPushButton *newUserButton;
	QPushButton *quitButton;

	QStateMachine machine;
	QState *notLoggedIn;
	QState *newUserCreation;
	QState *loggedIn;
	QState *search;
	QState *rateMovieState;
	QState *viewRatingsState;

	NewUser *n;
	LoggedInWindow *l;
	SearchWindow *s;
	RateMovie *r;
	RatingsWindow *rw;
	Netflix *netflix;

	QString userID; //these are used to keep track of user's status
	std::map<std::string, std::set<Movie*>*>::iterator* keywordsMapIterator;
	std::map<std::string, std::set<Movie*>*>::iterator* actorsMapIterator;
	std::set<Movie*>::iterator movieSetIterator;
	Movie* currentMovie; //movie being looked at while searching
};

#endif