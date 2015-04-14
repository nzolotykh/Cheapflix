//Nikita Zolotykh
//CSCI 170
//12/9/2014
#include "../qt/MainMenu.h"

int main (int argc, char * argv[]) {
	QApplication app(argc, argv);

	if (argc != 2) {
		//number of arguments check
		QMessageBox argumentsMessage;
		argumentsMessage.setText("Incorrect number of arguments!");
		argumentsMessage.setInformativeText("There should be only one argument -- name of the text file to be opened. Try again.");
		argumentsMessage.setStandardButtons(QMessageBox::Ok);
		argumentsMessage.setDefaultButton(QMessageBox::Ok);
		argumentsMessage.exec();
		return 0;
	}

	std::ifstream myfile (argv[1]); //streams from the file specified by the first argument

	if (!myfile.is_open()) {
		//cannot open the main data file
		QMessageBox argumentsMessage;
		argumentsMessage.setText("Cannot open the main data file! Try again.");
		argumentsMessage.setStandardButtons(QMessageBox::Ok);
		argumentsMessage.setDefaultButton(QMessageBox::Ok);
		argumentsMessage.exec();
		return 0;
	} else {
		//file is opened
		std::stringstream ss;
		std::string line;
		std::string userFile;
		std::string movieFile;

//---reading in file names for users and movies---
		getline(myfile, line);
		ss << line;
		ss >> userFile;
		ss.str(std::string());
		ss.clear();
		getline(myfile, line);
		ss << line;
		ss >> movieFile;
//-----------------------------------------------

		Netflix *netflix = new Netflix(userFile, movieFile);
		if (netflix->readMovies()) {
			//reads in the movies successfully
		} else {
			//fails to read in the movies
			QMessageBox argumentsMessage;
			argumentsMessage.setText("Cannot open the movies data file! Try again.");
			argumentsMessage.setStandardButtons(QMessageBox::Ok);
			argumentsMessage.setDefaultButton(QMessageBox::Ok);
			argumentsMessage.exec();
			return 0;
		}
		if (netflix->readUsers()) {
			//reads in the users successfully
		} else {
			//fails to read in the users
			QMessageBox argumentsMessage;
			argumentsMessage.setText("Cannot open the users data file! Try again.");
			argumentsMessage.setStandardButtons(QMessageBox::Ok);
			argumentsMessage.setDefaultButton(QMessageBox::Ok);
			argumentsMessage.exec();
			return 0;
		}

		MainMenu m(netflix);
		m.show();	
		return app.exec();
	}
}