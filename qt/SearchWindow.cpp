#include "SearchWindow.h"

void SearchWindow::createMovieInfoBox() {
	//movie info box
	movieInfoBox = new QGroupBox(tr("Movie Info"));;
	keywordsLabel = new QLabel("KEYWORDS WOULD GO HERE (if they worked)");
	QVBoxLayout *layout = new QVBoxLayout;

	layout->addWidget(keywordsLabel);
	movieInfoBox->setLayout(layout);
}

void SearchWindow::createButtons() {
	//buttons on the bottom
	buttonsLayout = new QHBoxLayout;
	nextMovieButton = new QPushButton("&Next Movie");
	nextMovieButton2 = new QPushButton("&Next Movie");
	addQueueButton = new QPushButton("&Add to my Queue");
	returnMenuButton = new QPushButton("&Return to Main Menu");

	buttonsLayout->addWidget(nextMovieButton);
	buttonsLayout->addWidget(nextMovieButton2);
	buttonsLayout->addWidget(addQueueButton);
	buttonsLayout->addWidget(returnMenuButton);
}

void SearchWindow::closeEvent (QCloseEvent *event)
{
	//just need to emit a signal here when "x" is pressed
	//so that we can catch it and make the program go back to the logged in window
	//(instead of just closing)
    event->accept();
    emit aboutToClose();
}

SearchWindow::SearchWindow() {
	createMovieInfoBox(); //creators of stuff
	createButtons();
	currentMovieLabel = new QLabel("CURRENT MOVIE");
	currentMovieLabel->setAlignment(Qt::AlignCenter);

	QVBoxLayout *mainLayout = new QVBoxLayout; //setting up the window
	mainLayout->addWidget(currentMovieLabel);
	mainLayout->addWidget(movieInfoBox);
	mainLayout->addLayout(buttonsLayout);
	setLayout(mainLayout);
	setWindowTitle(tr("Cheapflix"));
}