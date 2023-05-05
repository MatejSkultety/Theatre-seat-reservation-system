# Theatre seat reservation system

### Description

Welcome to my project. My goal was to create a system in C which can interact with users and modify databases. Main point is to register with username and password and then either create, delete or view all your active reservations stored in simple database. If a seat is taken, no other reservation within that play can be made to that seat. Available plays can be shown without registration to everyone with simple graphics showing taken and free seats. With admin privileges (password: 1234) you can even create a new play in one of 4 predefined theatre halls. Entire program was written in C language and is compatible only with Windows users.

### Technologies

Project is created with:
* C programming language: C11 standard
* windows.h and conio.h libraries

### Launch

To use this reservation system, simply download the Theatre.exe file and place /db and /us folders into the same directory. If the app cannot find those database folders, it will shut down. Database contains some plays and users, but you are free to create new or delete those existing via the app itself. You can as well download the source code, header file and compile with GCC, but beware that the database must be included as well.
