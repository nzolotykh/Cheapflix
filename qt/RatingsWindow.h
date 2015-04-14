#ifndef RATINGSWINDOW_H
#define RATINGSWINDOW_H

#include <QtGui>
#include "../lib/Netflix.h"

class RatingsWindow : public QWidget {
	Q_OBJECT
public:
	friend class MainMenu; //every window has to friend main menu, because main menu will access each window's members
	RatingsWindow(Netflix* netflix);
private slots:
	void closeEvent(QCloseEvent *event); //this is to specify what happens when you press "x"
signals:
	void aboutToClose();
private:
	void createRadioButtons(Netflix* netflix);

	QLabel *titleLabel;

	QHBoxLayout **ratingsHLayout;
	QButtonGroup **radioButtonsGroup;
	QLabel **movieNameLabel;
	QRadioButton **score1RadioButton;
	QRadioButton **score2RadioButton;
	QRadioButton **score3RadioButton;
	QRadioButton **score4RadioButton;
	QRadioButton **score5RadioButton;
	QRadioButton **noscoreRadioButton;

	QVBoxLayout *moviesVLayout;

	QHBoxLayout *buttonsHLayout;
	QPushButton *confirmButton;
	QPushButton *cancelButton;
};

#endif