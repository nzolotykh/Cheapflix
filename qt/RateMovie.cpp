#include "RateMovie.h"

void RateMovie::closeEvent (QCloseEvent *event) {
	//just need to emit a signal here when "x" is pressed
	//so that we can catch it and make the program go back to the logged in window
	//(instead of just closing)
    event->accept();
    emit aboutToClose();
}

void RateMovie::createRadioButtons() {
	//title + radio buttons + confirm button
	titleLabel = new QLabel("Thank you for returning the movie, would you like to rate it?");
	titleLabel->setAlignment(Qt::AlignCenter);

	ratingsHLayout = new QHBoxLayout;
	score1RadioButton = new QRadioButton("1");
	score2RadioButton = new QRadioButton("2");
	score3RadioButton = new QRadioButton("3");
	score4RadioButton = new QRadioButton("4");
	score5RadioButton = new QRadioButton("5");
	noscoreRadioButton = new QRadioButton("No Rating");
	noscoreRadioButton->setChecked(true);

	ratingsHLayout->addWidget(score1RadioButton);
	ratingsHLayout->addWidget(score2RadioButton);
	ratingsHLayout->addWidget(score3RadioButton);
	ratingsHLayout->addWidget(score4RadioButton);
	ratingsHLayout->addWidget(score5RadioButton);
	ratingsHLayout->addWidget(noscoreRadioButton);

	confirmButton = new QPushButton("&Confirm");
}

RateMovie::RateMovie() {
	createRadioButtons(); //creating stuff

	QVBoxLayout *mainLayout = new QVBoxLayout; //setting up the window
	mainLayout->addWidget(titleLabel);
	mainLayout->addLayout(ratingsHLayout);
	mainLayout->addWidget(confirmButton);
	setLayout(mainLayout);
	setWindowTitle(tr("Cheapflix"));
}