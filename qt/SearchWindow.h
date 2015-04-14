#ifndef SEARCHWINDOW_H
#define SEARCHWINDOW_H

#include <QtGui>

class SearchWindow : public QWidget {
Q_OBJECT
public:
	friend class MainMenu; //every window has to friend main menu, because main menu will access each window's members
	SearchWindow();
private slots:
	void closeEvent(QCloseEvent *event); //this is to specify what happens when you press "x"
signals:
	void aboutToClose();
private:
	void createMovieInfoBox(); //creators of stuff
	void createButtons();

	QLabel *currentMovieLabel;

	QGroupBox *movieInfoBox;
	QLabel *keywordsLabel; //in reality, this should be more complicated (perhaps a dynamically allocated set of labels / a vector)
							//but my keywords did not show up properly in the second part, so all keywords will be represented by
							//this dummy label

	QHBoxLayout *buttonsLayout;
	QPushButton *nextMovieButton;
	QPushButton *nextMovieButton2;
	QPushButton *addQueueButton;
	QPushButton *returnMenuButton;
};

#endif