# Cheapflix

*DESCRIPTION*

This is Cheapflix: a "cheap" version of Netflix, coded in C++. The basic functionality allows the user to login to the account, search for movies by keyword or title, check out the movies, add them to the queue, return the movies, and rate the returned movies. The program utilizes proper I/O techniques, maps, sets, proper object-oriented programming, and qt-based GUI. All of the data is stored in *.txt files in the data folder. For a list of existing users, see data/userinfo.txt, and for a list of existing movies, see data/moviesinfo.txt. This was my final project for CSCI 104 class (C++ Data Structures).

*INSTRUCTIONS*

In order to compile, simply launch the Makefile by typing, "make" in the command line. To execute, type, "./project_zolotykh data/maindata.txt", where "data/maindata.txt" can be any path to the text document that will be the main argument for the program.

Most of the functions and ideas are commented inside each file. The main file that has the mechanics of the program is movies.cpp, but most of functionality is implemented in Netflix.cpp and MainMenu.cpp.
