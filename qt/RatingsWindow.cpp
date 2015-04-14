#include "RatingsWindow.h"

void RatingsWindow::closeEvent (QCloseEvent *event) {
	//just need to emit a signal here when "x" is pressed
	//so that we can catch it and make the program go back to the logged in window
	//(instead of just closing)
    event->accept();
    emit aboutToClose();
}

void RatingsWindow::createRadioButtons(Netflix* netflix) {
	//title + radio buttons + confirm button
	titleLabel = new QLabel("Here you can modify your ratings:");
	titleLabel->setAlignment(Qt::AlignCenter);

	ratingsHLayout = new QHBoxLayout* [netflix->getUser()->getRatings()->size()];
	for (unsigned int i = 0; i < netflix->getUser()->getRatings()->size(); i ++) {
		ratingsHLayout[i] = new QHBoxLayout();
	}
	radioButtonsGroup = new QButtonGroup* [netflix->getUser()->getRatings()->size()];
	for (unsigned int i = 0; i < netflix->getUser()->getRatings()->size(); i ++) {
		radioButtonsGroup[i] = new QButtonGroup();
	}
	movieNameLabel = new QLabel* [netflix->getUser()->getRatings()->size()];
	for (unsigned int i = 0; i < netflix->getUser()->getRatings()->size(); i ++) {
		movieNameLabel[i] = new QLabel();
	}
	score1RadioButton = new QRadioButton* [netflix->getUser()->getRatings()->size()];
	for (unsigned int i = 0; i < netflix->getUser()->getRatings()->size(); i ++) {
		score1RadioButton[i] = new QRadioButton("1");
	}
	score2RadioButton = new QRadioButton* [netflix->getUser()->getRatings()->size()];
	for (unsigned int i = 0; i < netflix->getUser()->getRatings()->size(); i ++) {
		score2RadioButton[i] = new QRadioButton("2");
	}
	score3RadioButton = new QRadioButton* [netflix->getUser()->getRatings()->size()];
	for (unsigned int i = 0; i < netflix->getUser()->getRatings()->size(); i ++) {
		score3RadioButton[i] = new QRadioButton("3");
	}
	score4RadioButton = new QRadioButton* [netflix->getUser()->getRatings()->size()];
	for (unsigned int i = 0; i < netflix->getUser()->getRatings()->size(); i ++) {
		score4RadioButton[i] = new QRadioButton("4");
	}
	score5RadioButton = new QRadioButton* [netflix->getUser()->getRatings()->size()];
	for (unsigned int i = 0; i < netflix->getUser()->getRatings()->size(); i ++) {
		score5RadioButton[i] = new QRadioButton("5");
	}
	noscoreRadioButton = new QRadioButton* [netflix->getUser()->getRatings()->size()];
	for (unsigned int i = 0; i < netflix->getUser()->getRatings()->size(); i ++) {
		noscoreRadioButton[i] = new QRadioButton("No Rating");
	}

	//Set<Movie*>::Iterator movieSetIterator = netflix->getUser()->getRatedMovies()->begin();
	std::map<Movie*, int>::iterator ratingsMapIterator = netflix->getUser()->getRatings()->begin();

	int i = 0;
	while (ratingsMapIterator != netflix->getUser()->getRatings()->end()) {
		//run through all movies in the set

		movieNameLabel[i]->setText(QString::fromStdString((*ratingsMapIterator).first->getTitle()));
		if (ratingsMapIterator->second == 1)
			score1RadioButton[i]->setChecked(true);
		if (ratingsMapIterator->second == 2)
			score2RadioButton[i]->setChecked(true);
		if (ratingsMapIterator->second == 3)
			score3RadioButton[i]->setChecked(true);
		if (ratingsMapIterator->second == 4)
			score4RadioButton[i]->setChecked(true);
		if (ratingsMapIterator->second == 5)
			score5RadioButton[i]->setChecked(true);

		i++;
		++ratingsMapIterator;
	}

	for (int i = 0; i < netflix->getUser()->getRatings()->size(); i++) {
		ratingsHLayout[i]->addWidget(movieNameLabel[i]);
		ratingsHLayout[i]->addWidget(score1RadioButton[i]);
		ratingsHLayout[i]->addWidget(score2RadioButton[i]);
		ratingsHLayout[i]->addWidget(score3RadioButton[i]);
		ratingsHLayout[i]->addWidget(score4RadioButton[i]);
		ratingsHLayout[i]->addWidget(score5RadioButton[i]);
		ratingsHLayout[i]->addWidget(noscoreRadioButton[i]);
		radioButtonsGroup[i]->addButton(score1RadioButton[i]);
		radioButtonsGroup[i]->addButton(score2RadioButton[i]);
		radioButtonsGroup[i]->addButton(score3RadioButton[i]);
		radioButtonsGroup[i]->addButton(score4RadioButton[i]);
		radioButtonsGroup[i]->addButton(score5RadioButton[i]);
		radioButtonsGroup[i]->addButton(noscoreRadioButton[i]);
	}

	confirmButton = new QPushButton("&Confirm");
	cancelButton = new QPushButton("C&ancel");
}

RatingsWindow::RatingsWindow(Netflix* netflix) {
	createRadioButtons(netflix); //creating stuff

	QVBoxLayout *mainLayout = new QVBoxLayout; //setting up the window
	moviesVLayout = new QVBoxLayout;
	buttonsHLayout = new QHBoxLayout;
	for (int i = 0; i < netflix->getUser()->getRatings()->size(); i++) {
		moviesVLayout->addLayout(ratingsHLayout[i]);
	}
	buttonsHLayout->addWidget(confirmButton);
	buttonsHLayout->addWidget(cancelButton);

	mainLayout->addWidget(titleLabel);
	mainLayout->addLayout(moviesVLayout);
	mainLayout->addLayout(buttonsHLayout);

	setLayout(mainLayout);
	setWindowTitle(tr("Cheapflix"));
}