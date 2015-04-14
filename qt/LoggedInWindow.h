#ifndef LOGGEDINWINDOW_H
#define LOGGEDINWINDOW_H

#include <QtGui>

class LoggedInWindow : public QWidget {
Q_OBJECT
public:
	friend class MainMenu; //every window has to friend main menu, because main menu will access each window's members
	LoggedInWindow();
private slots:
	void closeEvent(QCloseEvent *event); //this is to specify what happens when you press "x"
signals:
	void aboutToClose();
private:
	void createCurrentMovieBox(); //creators of stuff
	void createMovieQueueBox();
	void createSearchMovieBox();
	void createWelcomeLabels();

	QLabel *welcomeLabel;

	QGroupBox *currentMovieBox;
	QLabel *currentMovieLabel;
	QPushButton *returnMovieButton;

	QGroupBox *movieQueueBox;
	QLabel *frontQueueLabel;
	QLabel *frontQueueTitleLabel;
	QPushButton *rentMovieButton;
	QPushButton *deleteQueueButton;
	QPushButton *backQueueButton;

	QGroupBox *searchBox;
	QLabel *searchLabel;
	QLineEdit *searchLineEdit;
	QPushButton *searchTitleButton;
	QPushButton *searchKeywordButton;
	QPushButton *searchActorButton;

	QPushButton *viewRatingsButton;
	QPushButton *logoutButton;
};

#endif