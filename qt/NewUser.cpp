#include "NewUser.h"

void NewUser::createLineEdit() {
	//the line edits
	titleLabel = new QLabel("Please sign up for Cheapflix");
	titleLabel->setAlignment(Qt::AlignCenter);

	loginHLayout = new QHBoxLayout;
	loginLabel = new QLabel("Login:");
	loginLineEdit = new QLineEdit;
	loginHLayout->addWidget(loginLabel);
	loginHLayout->addWidget(loginLineEdit);

	nameHLayout = new QHBoxLayout;
	nameLabel = new QLabel("Name:");
	nameLineEdit = new QLineEdit;
	nameHLayout->addWidget(nameLabel);
	nameHLayout->addWidget(nameLineEdit);
}

void NewUser::createButtons() {
	//buttons on the bottom
	buttonHLayout = new QHBoxLayout;
	confirmButton = new QPushButton("&Confirm");
	cancelButton = new QPushButton("&Cancel");
	buttonHLayout->addWidget(confirmButton);
	buttonHLayout->addWidget(cancelButton);
}

NewUser::NewUser() {
	createLineEdit(); //creating stuff
	createButtons();

	QVBoxLayout *mainLayout = new QVBoxLayout; //setting up the window
	mainLayout->addWidget(titleLabel);
	mainLayout->addLayout(loginHLayout);
	mainLayout->addLayout(nameHLayout);
	mainLayout->addLayout(buttonHLayout);
	setLayout(mainLayout);
	setWindowTitle(tr("Cheapflix"));
	loginLineEdit->setFocus();
}