#include <stdio.h>
#include <Windows.h>
#include <string.h>
#include <conio.h>
#include <ctype.h>
#include <stdlib.h>

#pragma warning(disable:4996)

// DEFINITIONS

#define _MAX_THEATRE_WIDTH 28
#define _MAX_THEATRE_HEIGHT 15
#define _MAX_STR 64

// STRUCTS

typedef struct _theatre_hall {
	char hall_name[3];
	int seats_per_row;
	int rows;
} THEATRE_HALL;

typedef struct _play_head {
	char play_name[_MAX_STR];
	float start;
	int duration;		// in minutes
	int price;
	int available;
	THEATRE_HALL played_in;
	short int** pSeatsTable;
} PLAY_HEAD;

typedef struct _user_head {
	char username[_MAX_STR];
	char password[_MAX_STR];
	int num_reservations;
	char** pActive_reservations;	// format "A12 Play_name"
} USER_HEAD;

// FUNCTIONS

void default_prints();
void print_theatre_hall(PLAY_HEAD current_play);
void green_print(HANDLE hConsloe);
void red_print(HANDLE hConsole);
void show();
int all_plays();
void create_play();

// USERS

void registration();
int username_check(char username[]);
void manage();
void create_reservation(char username[]);
void show_my(char username[]);
void reg_del(char username[]);

// DATABASE

int db_check();
int us_check();
void save_user(USER_HEAD user);
USER_HEAD load_user(char file_name[]);
void save_play(PLAY_HEAD play);
PLAY_HEAD load_play(char file_name[]);