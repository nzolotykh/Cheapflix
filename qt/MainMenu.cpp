#include "MainMenu.h"

void MainMenu::createLineEdit() {
	//creates the top part of the window

	titleLabel = new QLabel("Welcome to Cheapflix!"); //this will be attached in the constructor
	titleLabel->setAlignment(Qt::AlignCenter);

	loginLineEdit = new QLineEdit;
	loginLineEditLabel = new QLabel("Login:");

	loginHLayout = new QHBoxLayout;
	loginHLayout->addWidget(loginLineEditLabel);
	loginHLayout->addWidget(loginLineEdit);
}

void MainMenu::createButtons() {
	//creates the bottom part of the window

	loginButton = new QPushButton ("&Login");
	newUserButton = new QPushButton ("&New User");
	quitButton = new QPushButton ("&Quit");

	loginButtonLayout = new QHBoxLayout;
	loginButtonLayout->addWidget(loginButton);
	loginButtonLayout->addWidget(newUserButton);
	loginButtonLayout->addWidget(quitButton);
}

void MainMenu::stateMachine() {
	//this is the state machine that keeps track of where we are in the program
	//it is used for closing/opening different windows
	//each window has a state assigned to it
	notLoggedIn = new QState(); //main window
	newUserCreation = new QState(); //new user window
	loggedIn = new QState(); //logged in window (after login)
	search = new QState(); //search window
	rateMovieState = new QState();
	viewRatingsState = new QState();

	machine.addState(notLoggedIn); //adding states to the machine
	machine.addState(newUserCreation);
	machine.addState(loggedIn);
	machine.addState(search);
	machine.addState(rateMovieState);
	machine.addState(viewRatingsState);

	//here, we create all sorts of transitions between states
	notLoggedIn->addTransition(newUserButton, SIGNAL(clicked()), newUserCreation); //when the New User button is pressed, we transition into the new user window
	notLoggedIn->addTransition(this, SIGNAL(loginSuccessful()), loggedIn); //when the user logs in successfully, we go to the logged in window

	newUserCreation->addTransition(n->cancelButton, SIGNAL(clicked()), notLoggedIn); //windows that can be reached from New User window
	newUserCreation->addTransition(this, SIGNAL(userAdded()), loggedIn);

	loggedIn->addTransition(l->logoutButton, SIGNAL(clicked()), notLoggedIn); //windows that can be reached from the logged in window
	loggedIn->addTransition(l, SIGNAL(aboutToClose()), notLoggedIn);
	loggedIn->addTransition(this, SIGNAL(searchSuccessful()), search);
	loggedIn->addTransition(this, SIGNAL(canRateMovie()), rateMovieState);
	loggedIn->addTransition(this, SIGNAL(canViewRatings()), viewRatingsState);

	search->addTransition(s->returnMenuButton, SIGNAL(clicked()), loggedIn); //from the search window
	search->addTransition(s, SIGNAL(aboutToClose()), loggedIn);

	rateMovieState->addTransition(r->confirmButton, SIGNAL(clicked()), loggedIn);
	rateMovieState->addTransition(r, SIGNAL(aboutToClose()), loggedIn);

	machine.setInitialState(notLoggedIn); //initial state: main window (not logged in)

	connect(notLoggedIn, SIGNAL(entered()), this, SLOT(show())); //when entering the Logged Out state, we show the main window
	connect(notLoggedIn, SIGNAL(exited()), this, SLOT(close())); //when leaving the Logged Out state, we hide the main window

	connect(newUserCreation, SIGNAL(entered()), n, SLOT(show())); //etc
	connect(newUserCreation, SIGNAL(exited()), n, SLOT(hide()));

	connect(loggedIn, SIGNAL(entered()), l, SLOT(show()));
	connect(loggedIn, SIGNAL(exited()), l, SLOT(hide()));

	connect(search, SIGNAL(entered()), s, SLOT(show()));
	connect(search, SIGNAL(exited()), s, SLOT(hide()));

	connect(rateMovieState, SIGNAL(entered()), r, SLOT(show()));
	connect(rateMovieState, SIGNAL(exited()), r, SLOT(hide()));

	machine.start(); //starting the state machine
}

void MainMenu::loginCheck() {
	//this function will be used to update the logged in window
	//we will call it from many other functions that are supposed
	//to change the contents of the window

	if (userID == "")
		userID = loginLineEdit->text(); //since this function also updates the window, we can't just fetch contents from the login line edit
										//first we need to see whether there's already a user logged in

	if (netflix->login(userID)) {
		//login successful!
		QString userName = QString::fromStdString(netflix->getUser()->getName());
		QString currentMovie;
		QString frontQueueTitle;
		//setting the welcome label text
		l->welcomeLabel->setText("Welcome to Cheapflix, " + userName + " (" + userID + ")");

		if (netflix->getUser()->currentMovie() != NULL) {
			//the user has a movie checked out, print that out
			currentMovie = QString::fromStdString(netflix->getUser()->currentMovie()->getTitle());
			l->currentMovieLabel->setText(currentMovie);
		} else {
			//no movie checked out, so show that
			l->currentMovieLabel->setText("No movie checked out!");
		}

		if (!netflix->getUser()->movieQueue()->empty()) {
			//movie queue is not empty, so display the front movie
			frontQueueTitle = QString::fromStdString(netflix->getUser()->movieQueue()->front()->getTitle());
			l->frontQueueTitleLabel->setText(frontQueueTitle);
		} else {
			//queue is empty, so show that
			l->frontQueueTitleLabel->setText("Your queue is empty!");
		}

		netflix->similaritiesCalculation();

		//we logged in, let the world know!
		emit loginSuccessful();
	} else {
		//login failed!
		QMessageBox failedMessage;
		failedMessage.setText("User ID does not exist!");
		failedMessage.setStandardButtons(QMessageBox::Ok);
		failedMessage.setDefaultButton(QMessageBox::Ok);
		failedMessage.exec();
	}
}

void MainMenu::returnMovie() {
	//returns the currently checked out movie
	//NOTE: this does not output an error if there's no checked out movie
	//this is done on purpose, because there's always a status on the checked out movie
	
	if(netflix->getUser()->currentMovie() != NULL) {
		//there is a rented movie
		//try {
		//	if (netflix->getUser()->getRatings()->get(netflix->getUser()->currentMovie())) {
		//		//the movie is already rated, so we can't rate again :(
		//		netflix->getUser()->returnMovie();
		//		loginCheck();
		//	}
		//} catch (NoSuchElementException e) {
			emit canRateMovie();
		//}
	} else {
		//no checked out movie
		QMessageBox failedMessage;
		failedMessage.setText("You have no movie checked out!");
		failedMessage.setStandardButtons(QMessageBox::Ok);
		failedMessage.setDefaultButton(QMessageBox::Ok);
		failedMessage.exec();
	}
}

void MainMenu::rateMovie() {
	//function that rates the movie
	int score;
	if (r->score1RadioButton->isChecked()) {
		//score = 1
		score = 1;
	}
	if (r->score2RadioButton->isChecked()) {
		//score = 2
		score = 2;
	}
	if (r->score3RadioButton->isChecked()) {
		//score = 3
		score = 3;
	}
	if (r->score4RadioButton->isChecked()) {
		//score = 4
		score = 4;
	}
	if (r->score5RadioButton->isChecked()) {
		//score = 5
		score = 5;
	}
	if (r->noscoreRadioButton->isChecked()) {
		//no score, but we will use "-1" as an impossible score
		//the Netflix function will use this as a check for "no score" option
		score = -1;
	}
	netflix->returnMovie(score);
	loginCheck(); //window update
}

void MainMenu::rentMovie() {
	//function that rents the movie at the front of the queue
	if(netflix->getUser()->currentMovie() == NULL) {
		//no movie checked out, so we can try renting
		if (!netflix->getUser()->movieQueue()->empty()) {
			//queue is not empty, SO WE CAN FINALLY RENT!
			netflix->getUser()->rentMovie(netflix->getUser()->movieQueue()->front()); //rent
			netflix->getUser()->movieQueue()->pop(); //dequeue
			loginCheck(); //window update
		} else {
			//queue is empty, so we can't rent :(
			QMessageBox failedMessage;
			failedMessage.setText("There is nothing in your queue to rent!");
			failedMessage.setStandardButtons(QMessageBox::Ok);
			failedMessage.setDefaultButton(QMessageBox::Ok);
			failedMessage.exec();
		}
	} else {
		//there is a movie checked out, can't rent!
		QMessageBox failedMessage;
		failedMessage.setText("First you have to return the checked out movie!");
		failedMessage.setStandardButtons(QMessageBox::Ok);
		failedMessage.setDefaultButton(QMessageBox::Ok);
		failedMessage.exec();
	}
}

void MainMenu::deleteMovie() {
	//function that deletes the movie from the queue
		if (!netflix->getUser()->movieQueue()->empty()) {
			//queue is not empty, so we can delete
			netflix->getUser()->movieQueue()->pop();
			loginCheck(); //window update
		} else {
			//queue is empty, so we can't delete
			QMessageBox failedMessage;
			failedMessage.setText("There is nothing in your queue to delete!");
			failedMessage.setStandardButtons(QMessageBox::Ok);
			failedMessage.setDefaultButton(QMessageBox::Ok);
			failedMessage.exec();
		}
}

void MainMenu::backQueue() {
	//function that puts the movie from the front to the back of the queue
		if (!netflix->getUser()->movieQueue()->empty()) {
			//queue is not empty, so we can move it
			netflix->getUser()->movieQueue()->push(netflix->getUser()->movieQueue()->front());
			netflix->getUser()->movieQueue()->pop();
			loginCheck(); //window update
		} else {
			//queue is empty, so there's nothing to move
			QMessageBox failedMessage;
			failedMessage.setText("There is nothing in your queue!");
			failedMessage.setStandardButtons(QMessageBox::Ok);
			failedMessage.setDefaultButton(QMessageBox::Ok);
			failedMessage.exec();
		}
}

void MainMenu::logout() {
	userID = ""; //clear the current user ID
	netflix->logout();
	loginLineEdit->setText(""); //clear whatever we had when we were logging in
	loginLineEdit->setFocus(); //set focus back to the field for login
}

void MainMenu::addUser() {
	//adding the user to the map, adding to the text file, and logging in

	userID = n->loginLineEdit->text();
	QString userName = n->nameLineEdit->text();

	if(userID.isEmpty() || userName.isEmpty()) {
		//empty user ID or user name, not good
		QMessageBox failedMessage;
		failedMessage.setText("All the fields must be filled out!");
		failedMessage.setStandardButtons(QMessageBox::Ok);
		failedMessage.setDefaultButton(QMessageBox::Ok);
		failedMessage.exec();
		return;
	}

	if(netflix->addUser(userID, userName)) {
		//user can be added
		loginCheck();
		n->loginLineEdit->setText("");
		n->nameLineEdit->setText("");
		n->loginLineEdit->setFocus();
		emit userAdded();
	} else {
		//user can't be added
		QMessageBox failedMessage;
		failedMessage.setText("User ID already exists!");
		failedMessage.setStandardButtons(QMessageBox::Ok);
		failedMessage.setDefaultButton(QMessageBox::Ok);
		failedMessage.exec();
	}
}

void MainMenu::searchKeyword() {
	QString keywordToSearch = l->searchLineEdit->text();
	keywordsMapIterator = netflix->searchKeyword(keywordToSearch);

	s->nextMovieButton2->hide();

	if (keywordsMapIterator != NULL) {
		//found some movies!
		movieSetIterator = (*keywordsMapIterator)->second->begin();
		if (netflix->getKeywordsMap()->find((*keywordsMapIterator)->first)->second->size() == 1) {
			//size of the set of movies of the keyword is 1; i.e., there is only one movie to display
			s->nextMovieButton->hide();
		} else {
			//more than one movie to show
			s->nextMovieButton->show();
		}
		currentMovie = *movieSetIterator; //update the current movie so that other functions can use it
		s->currentMovieLabel->setText(QString::fromStdString((*movieSetIterator)->getTitle()));
		emit searchSuccessful();
	} else {
		//no movies were found!
		QMessageBox failedMessage;
		failedMessage.setText("No movies were found!");
		failedMessage.setStandardButtons(QMessageBox::Ok);
		failedMessage.setDefaultButton(QMessageBox::Ok);
		failedMessage.exec();
	}
}

void MainMenu::searchActor() {
	//basically the same implementation as searching by keyword
	QString actorToSearch = l->searchLineEdit->text();
	actorsMapIterator = netflix->searchActor(actorToSearch);

	s->nextMovieButton->hide();

	if (actorsMapIterator != NULL) {
		//found some movies!
		movieSetIterator = (*actorsMapIterator)->second->begin();
		if (netflix->getActorsMap()->find((*actorsMapIterator)->first)->second->size() == 1) {
			//size of the set of movies of the keyword is 1; i.e., there is only one movie to display
			s->nextMovieButton2->hide();
		} else {
			//more than one movie to show
			s->nextMovieButton2->show();
		}
		currentMovie = *movieSetIterator; //update the current movie so that other functions can use it
		s->currentMovieLabel->setText(QString::fromStdString((*movieSetIterator)->getTitle()));
		emit searchSuccessful();
	} else {
		//no movies were found!
		QMessageBox failedMessage;
		failedMessage.setText("No movies were found!");
		failedMessage.setStandardButtons(QMessageBox::Ok);
		failedMessage.setDefaultButton(QMessageBox::Ok);
		failedMessage.exec();
	}
}

void MainMenu::nextMovie() {
	//when pressing the next movie button
	if(++movieSetIterator != netflix->getKeywordsMap()->find((*keywordsMapIterator)->first)->second->end()) {
		//try to increment the movie set iterator, update the current movie, update the window label
		currentMovie = *movieSetIterator;
		s->currentMovieLabel->setText(QString::fromStdString((*movieSetIterator)->getTitle()));
	} else {
		//no more movies to show!
		QMessageBox failedMessage;
		failedMessage.setText("No more movies to show!");
		failedMessage.setStandardButtons(QMessageBox::Ok);
		failedMessage.setDefaultButton(QMessageBox::Ok);
		failedMessage.exec();
		s->nextMovieButton->hide(); //hide the next movie button
	}
}

void MainMenu::nextMovie2() {
	//when pressing the next movie button WHEN SEARCHING FOR AN ACTOR
	if(++movieSetIterator != netflix->getActorsMap()->find((*actorsMapIterator)->first)->second->end()) {
		//try to increment the movie set iterator, update the current movie, update the window label
		currentMovie = *movieSetIterator;
		s->currentMovieLabel->setText(QString::fromStdString((*movieSetIterator)->getTitle()));
	} else {
		//no more movies to show!
		QMessageBox failedMessage;
		failedMessage.setText("No more movies to show!");
		failedMessage.setStandardButtons(QMessageBox::Ok);
		failedMessage.setDefaultButton(QMessageBox::Ok);
		failedMessage.exec();
		s->nextMovieButton2->hide(); //hide the next movie button
	}
}

void MainMenu::searchTitle() {

	QString movieToSearch = l->searchLineEdit->text();
	s->nextMovieButton->hide(); //no need for next movie button
	s->nextMovieButton2->hide();

	if (netflix->searchTitle(movieToSearch) != NULL) {
		//found a movie!
		currentMovie = (**netflix->searchTitle(movieToSearch)).second; //update the current movie
		s->currentMovieLabel->setText(QString::fromStdString((**netflix->searchTitle(movieToSearch)).first)); //change the window label
		emit searchSuccessful();
	} else {
		//movie was not found!
		QMessageBox failedMessage;
		failedMessage.setText("Movie with this title does not exist!");
		failedMessage.setStandardButtons(QMessageBox::Ok);
		failedMessage.setDefaultButton(QMessageBox::Ok);
		failedMessage.exec();
	}
}

void MainMenu::addToQueue() {
	//adding the current movie (the movie we are considering now) to the queue
	netflix->getUser()->movieQueue()->push(currentMovie);
	loginCheck(); //window update
}

void MainMenu::confirmChangedRatings() {
	//triggered when the confirm button is pressed after viewing ratings
	//reads in the results from all radio buttons and updates the user's ratings
	std::set<Movie*>::iterator ratingsSetIterator = netflix->getUser()->getRatedMovies()->begin();

	Movie* tempMovie;
	int i = 0;

	while (ratingsSetIterator != netflix->getUser()->getRatedMovies()->end()) {
		//runs through all movies, updating all the ratings
		tempMovie = (*ratingsSetIterator);
		netflix->getUser()->getRatings()->erase((*ratingsSetIterator)); //remove so we could add a new value

		if (rw->score1RadioButton[i]->isChecked()) {
			//radio button 1
			int one = 1;
			netflix->getUser()->getRatings()->insert(std::pair<Movie*, int> (tempMovie, one));
		}
		if (rw->score2RadioButton[i]->isChecked()) {
			//radio button 2
			int two = 2;
			netflix->getUser()->getRatings()->insert(std::pair<Movie*, int> (tempMovie, two));
		}
		if (rw->score3RadioButton[i]->isChecked()) {
			//radio button 3
			int three = 3;
			netflix->getUser()->getRatings()->insert(std::pair<Movie*, int> (tempMovie, three));
		}
		if (rw->score4RadioButton[i]->isChecked()) {
			//radio button 4
			int four = 4;
			netflix->getUser()->getRatings()->insert(std::pair<Movie*, int> (tempMovie, four));
		}
		if (rw->score5RadioButton[i]->isChecked()) {
			//radio button 5
			int five = 5;
			netflix->getUser()->getRatings()->insert(std::pair<Movie*, int> (tempMovie, five));
		}
		if (rw->noscoreRadioButton[i]->isChecked()) {
			//no rating button
			netflix->getUser()->getRatedMovies()->erase(tempMovie);
		}
		++ratingsSetIterator;
		i++;
	}
	loginCheck();
}

void MainMenu::viewRatings() {
	//triggers the ratings view window
	//since it is a QT window with dynamically allocated objects, we need to create
	//and assign different parameters of it here, and not in the constructor, like we
	//did for the other QT objects
	if (rw != NULL) {
		delete rw;
	}
	if (netflix->getUser()->getRatedMovies()->size() != 0) {
		//there are rated movies, we can view their ratings!
		rw = new RatingsWindow(netflix);

		viewRatingsState->addTransition(rw->confirmButton, SIGNAL(clicked()), loggedIn);
		viewRatingsState->addTransition(rw->cancelButton, SIGNAL(clicked()), loggedIn);
		viewRatingsState->addTransition(rw, SIGNAL(aboutToClose()), loggedIn);

		connect(viewRatingsState, SIGNAL(entered()), rw, SLOT(show()));
		connect(viewRatingsState, SIGNAL(exited()), rw, SLOT(hide()));

		connect(rw->confirmButton, SIGNAL(clicked()), this, SLOT(confirmChangedRatings()));
		connect(rw->cancelButton, SIGNAL(clicked()), rw, SLOT(close()));
		connect(rw, SIGNAL(aboutToClose()), rw, SLOT(close()));

		emit canViewRatings();
	} else {
		//no ratings
		QMessageBox failedMessage;
		failedMessage.setText("You have not rated any movies!");
		failedMessage.setStandardButtons(QMessageBox::Ok);
		failedMessage.setDefaultButton(QMessageBox::Ok);
		failedMessage.exec();
	}
}

MainMenu::MainMenu(Netflix *netflix) {
	n = new NewUser; //constructing stuff
	l = new LoggedInWindow;
	s = new SearchWindow;
	r = new RateMovie;
	rw = NULL;
	this->netflix = netflix;
	createLineEdit();
	createButtons();
	stateMachine();

	connect(quitButton, SIGNAL(clicked()), this, SLOT(close())); //proper connections from the main window
	connect(loginButton, SIGNAL(clicked()), this, SLOT(loginCheck())); //note: the new user button is taken care of in the state machine

	connect(l->logoutButton, SIGNAL(clicked()), this, SLOT(logout())); //proper connections from the logged in window
	connect(l->viewRatingsButton, SIGNAL(clicked()), this, SLOT(viewRatings())); //proper connections from the logged in window
	connect(l->rentMovieButton, SIGNAL(clicked()), this, SLOT(rentMovie())); //various function calls for all buttons
	connect(l->deleteQueueButton, SIGNAL(clicked()), this, SLOT(deleteMovie()));
	connect(l->backQueueButton, SIGNAL(clicked()), this, SLOT(backQueue()));
	connect(l->returnMovieButton, SIGNAL(clicked()), this, SLOT(returnMovie()));
	connect(l->searchTitleButton, SIGNAL(clicked()), this, SLOT(searchTitle()));
	connect(l->searchLineEdit, SIGNAL(returnPressed()), this, SLOT(searchTitle()));
	connect(l->searchKeywordButton, SIGNAL(clicked()), this, SLOT(searchKeyword()));
	connect(l->searchActorButton, SIGNAL(clicked()), this, SLOT(searchActor()));
	connect(l, SIGNAL(aboutToClose()), this, SLOT(logout()));

	connect(n->confirmButton, SIGNAL(clicked()), this, SLOT(addUser())); //etc
	connect(n->nameLineEdit, SIGNAL(returnPressed()), this, SLOT(addUser()));
	connect(n->cancelButton, SIGNAL(clicked()), n, SLOT(close()));

	connect(s->nextMovieButton, SIGNAL(clicked()), this, SLOT(nextMovie()));
	connect(s->nextMovieButton2, SIGNAL(clicked()), this, SLOT(nextMovie2()));
	connect(s->addQueueButton, SIGNAL(clicked()), this, SLOT(addToQueue()));
	connect(s, SIGNAL(aboutToClose()), s, SLOT(close()));

	connect(r->confirmButton, SIGNAL(clicked()), this, SLOT(rateMovie()));
	connect(r, SIGNAL(aboutToClose()), r, SLOT(close()));

	QVBoxLayout *mainLayout = new QVBoxLayout; //setting up the window
	mainLayout->addWidget(titleLabel);
	mainLayout->addLayout(loginHLayout);
	mainLayout->addLayout(loginButtonLayout);
	setLayout(mainLayout);
	setWindowTitle(tr("Cheapflix"));
	loginLineEdit->setFocus();
}
