#ifndef RATEMOVIE_H
#define RATEMOVIE_H

#include <QtGui>

class RateMovie : public QWidget {
	Q_OBJECT
public:
	friend class MainMenu; //every window has to friend main menu, because main menu will access each window's members
	RateMovie();
private slots:
	void closeEvent(QCloseEvent *event); //this is to specify what happens when you press "x"
signals:
	void aboutToClose();
private:
	void createRadioButtons();

	QLabel *titleLabel;

	QHBoxLayout *ratingsHLayout;
	QRadioButton *score1RadioButton;
	QRadioButton *score2RadioButton;
	QRadioButton *score3RadioButton;
	QRadioButton *score4RadioButton;
	QRadioButton *score5RadioButton;
	QRadioButton *noscoreRadioButton;

	QPushButton *confirmButton;
};

#endif