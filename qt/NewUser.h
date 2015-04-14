#ifndef NEWUSER_H
#define NEWUSER_H

#include <QtGui>

class NewUser : public QWidget {
	Q_OBJECT
public:
	friend class MainMenu; //every window has to friend main menu, because main menu will access each window's members
	NewUser();
private:
	void createLineEdit();
	void createButtons();

	QLabel *titleLabel;

	QHBoxLayout *loginHLayout;
	QLabel *loginLabel;
	QLineEdit *loginLineEdit;

	QHBoxLayout *nameHLayout;
	QLabel *nameLabel;
	QLineEdit *nameLineEdit;

	QHBoxLayout *buttonHLayout;
	QPushButton *confirmButton;
	QPushButton *cancelButton;
};

#endif