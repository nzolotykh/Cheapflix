#include "LoggedInWindow.h"

void LoggedInWindow::createCurrentMovieBox() {
	//top part of the window
	currentMovieBox = new QGroupBox(tr("Your Current Movie"));
	currentMovieLabel = new QLabel;
	returnMovieButton = new QPushButton("&Return Movie");
	QVBoxLayout *layout = new QVBoxLayout;
	layout->addWidget(currentMovieLabel);
	layout->addWidget(returnMovieButton);
	currentMovieBox->setLayout(layout);
}

void LoggedInWindow::createMovieQueueBox() {
	//movie queue part of the window
	movieQueueBox = new QGroupBox("Your Movie Queue");
	QVBoxLayout *mainLayout = new QVBoxLayout;

	QHBoxLayout *queueLabelsLayout = new QHBoxLayout;
	frontQueueLabel = new QLabel("Front of Queue:");
	frontQueueTitleLabel = new QLabel;
	queueLabelsLayout->addWidget(frontQueueLabel);
	queueLabelsLayout->addWidget(frontQueueTitleLabel);

	QHBoxLayout *buttonsLayout = new QHBoxLayout;
	rentMovieButton = new QPushButton("R&ent Movie");
	deleteQueueButton = new QPushButton("&Delete from Queue");
	backQueueButton = new QPushButton("Move to &Back of Queue");
	buttonsLayout->addWidget(rentMovieButton);
	buttonsLayout->addWidget(deleteQueueButton);
	buttonsLayout->addWidget(backQueueButton);

	mainLayout->addLayout(queueLabelsLayout);
	mainLayout->addLayout(buttonsLayout);
	movieQueueBox->setLayout(mainLayout);
}

void LoggedInWindow::createSearchMovieBox() {
	//search movie box
	searchBox = new QGroupBox("Search for a Movie");
	QVBoxLayout *mainLayout = new QVBoxLayout;

	QHBoxLayout *searchLineEditLayout = new QHBoxLayout;
	searchLabel = new QLabel("Search:");
	searchLineEdit = new QLineEdit;
	searchLineEditLayout->addWidget(searchLabel);
	searchLineEditLayout->addWidget(searchLineEdit);

	QHBoxLayout *searchButtonsLayout = new QHBoxLayout;
	searchTitleButton = new QPushButton("Search by &Title");
	searchKeywordButton = new QPushButton("Search by &Keyword");
	searchActorButton = new QPushButton("Search by &Actor");
	searchButtonsLayout->addWidget(searchTitleButton);
	searchButtonsLayout->addWidget(searchKeywordButton);
	searchButtonsLayout->addWidget(searchActorButton);

	mainLayout->addLayout(searchLineEditLayout);
	mainLayout->addLayout(searchButtonsLayout);
	searchBox->setLayout(mainLayout);
}

void LoggedInWindow::closeEvent (QCloseEvent *event) {
	//when "x" is pressed, a window appears asking the user if he's sure to log out
	//note: this will not happen if the user just presses "Log Out" because
	//generally when that big button is pressed, it won't be accidental and the user
	//should of course be sure that he or she wants to log out. But when the "x" is
	//pressed, that sometimes happens accidentally, so this is necessary
	QMessageBox::StandardButton message = QMessageBox::question(this, "Cheapflix", tr("Are you sure?\n"), QMessageBox::No | QMessageBox::Yes, QMessageBox::Yes);

	if (message != QMessageBox::Yes) {
		//if No is pressed
		event->ignore();
	} else {
		//if Yes is pressed
		event->accept();
		emit aboutToClose();
	}
}

LoggedInWindow::LoggedInWindow() {
	createCurrentMovieBox(); //creating stuff
	createMovieQueueBox();
	createSearchMovieBox();

	welcomeLabel = new QLabel; //creating the top label / bottom logout button
	welcomeLabel->setAlignment(Qt::AlignCenter);
	logoutButton = new QPushButton("Log&out");
	viewRatingsButton = new QPushButton("View &Ratings");

	QVBoxLayout *mainLayout = new QVBoxLayout; //setting up the window
	mainLayout->addWidget(welcomeLabel);
	mainLayout->addWidget(currentMovieBox);
	mainLayout->addWidget(movieQueueBox);
	mainLayout->addWidget(searchBox);
	mainLayout->addWidget(viewRatingsButton);
	mainLayout->addWidget(logoutButton);
	setLayout(mainLayout);
	setWindowTitle(tr("Cheapflix"));
	searchLineEdit->setFocus();
}