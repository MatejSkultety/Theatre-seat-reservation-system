#include "theatre_skuty.h"

THEATRE_HALL A1 = { "A1", 25, 8 }, A2 = { "A2", 28, 15 }, B1 = { "B1", 15, 10 }, B2 = { "B2", 16, 6 }; // hardcoded theatre halls

void print_theatre_hall(PLAY_HEAD current_play) {
    HANDLE  hConsole;
    hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    short int** pSeats = current_play.pSeatsTable;
    // stage
    for (int i = 0; i < current_play.played_in.seats_per_row * 3; i++) {
        if (i == (current_play.played_in.seats_per_row*3)/2) printf("\t   |STAGE|");
        else printf(" ");
    }
    // first row
    printf("\n\n\t   ");
    for (int i = 0; i < current_play.played_in.seats_per_row; i++) {
        printf("%2d ", i + 1);
    }
    // seats
    for (int i = 0; i < current_play.played_in.rows; i++) {
        printf("\n\n\t%c  ", 'A' + i);
        for (int j = 0; j < current_play.played_in.seats_per_row; j++) {
            if (pSeats[i][j]) red_print(hConsole);
            else green_print(hConsole);
        }
    }
    printf("\n\n");
}


void green_print(HANDLE hConsole) {
    printf(" ");
    SetConsoleTextAttribute(hConsole, 160);
    printf("#");
    SetConsoleTextAttribute(hConsole, 15);
    printf(" ");
}


void red_print(HANDLE hConsole) {
    printf(" ");
    SetConsoleTextAttribute(hConsole, 192);
    printf("#");
    SetConsoleTextAttribute(hConsole, 15);
    printf(" ");
}


void default_prints() {
    printf("\n");
    printf("\n\t########## SKUTY's THEATRE ##########");
    printf("\n\tWelcome! This is a seat reservation");
    printf("\n\tsystem. Pleas, choose one mode");
    printf("\n\tby typing one number below.");
    printf("\n\t############# AVAILABLE #############");
    printf("\n\t=> 1 New user registration");
    printf("\n\t=> 2 Manage and make new reservations");
    printf("\n\t=> 3 Show available plays");
    printf("\n\t=> 4 Create new play");
    printf("\n\n\tPress ESC to leave.\n");
    printf("\n\n\t############## CHOOSE ###############");
    printf("\n\tType one number: ");
}


void show() {
    char ch = 1;
    do {
        system("cls");
        printf("\n\tList of all available plays. Type corresponding number to show details.\n\tType anything alse for returning to main menu.\n");
        if (!all_plays()) break;
        printf("\n\t\tPress B for showing the list again or anything else for Main menu:");
        ch = getch();
    } while (ch == 'B' || ch == 'b');
}


int all_plays() {
    FILE* fRoot = fopen("db/root.txt", "r");
    if (fRoot == NULL) {
        printf("\nError opening root file\n");
        return 0;
    }

    char** list_of_plays = malloc(sizeof(char*));
    int i = 0;
    while (1)
    {
        list_of_plays[i] = malloc(_MAX_STR);
        if (fgets(list_of_plays[i], _MAX_STR, fRoot) == 0) break;
        printf("\n\t=> %d\tName: %s\n\n", i + 1, list_of_plays[i]);
        i++;
        list_of_plays = realloc(list_of_plays, sizeof(char*) * (i + 1));
    }
    fclose(fRoot);

    int choose;
    printf("\n\t\tYour choice: ");
    scanf(" %d", &choose);
    system("cls");
    if (choose > 0 && choose < (i + 1)) {
        char* new_line = strchr(list_of_plays[choose - 1], '\n');   // remove '\n'
        if (new_line) *new_line = '\0';
        PLAY_HEAD loaded = load_play(list_of_plays[choose - 1]);    // load chosen play

        printf("\n\tName: %s  \tHall: %s\n", loaded.play_name, loaded.played_in.hall_name);
        printf("\tStars at: %.2f  \tDuration: %d  \tAvailable seats: %d\n", loaded.start, loaded.duration, loaded.available);
        printf("\tPrice: %d$\n\n", loaded.price);

        print_theatre_hall(loaded);
        save_play(loaded);
        // free allocated memory
        for (int j = 0; j < i + 1; j++)
        {
            free(list_of_plays[j]);
        }
        free(list_of_plays);
    }
    else {
        // free allocated memory
        for (int j = 0; j < i + 1; j++)
        {
            free(list_of_plays[j]);
        }
        free(list_of_plays);
        return 0;
    }
    return choose;
}


void create_play() {
    char buff[5], password[] = "1234";
    printf("\n\n\n\tYou need admin rights to proceed. Please, enter password:\n\t");
    fgets(buff, 5, stdin);
    if (strcmp(buff, password)) {       // password check
        printf("\n\tWrong password!");
        Sleep(1000);
        return;
    }
    // correct password
    PLAY_HEAD new_play;
    system("cls");
    printf("\n\tFill in following information\n");
    printf("\n\n\tName of the play: ");
    fgets(new_play.play_name, _MAX_STR, stdin);
    fgets(new_play.play_name, _MAX_STR, stdin);
    char* new_line = strchr(new_play.play_name, '\n');  // remove '\n'
    if(new_line) *new_line = '\0';
    if (username_check(new_play.play_name)) {
        printf("\n\tInvalid name");
        Sleep(1000);
        return;
    }
    printf("\n\tName: %s\n", new_play.play_name);

    printf("\n\n\tHall (A1, A2, B1, B2): ");
    char help[3];
    fgets(help, 3, stdin);
    if (!strcmp(help, "A2")) new_play.played_in = A2;
    else if (!strcmp(help, "B1")) new_play.played_in = B1;
    else if (!strcmp(help, "B2")) new_play.played_in = B2;
    else new_play.played_in = A1;                           // default case
    printf("\n\tHall: %s\n", new_play.played_in.hall_name);

    printf("\n\n\tStart of the play ('HH.MM'): ");
    scanf(" %f", &(new_play.start));
    printf("\n\tStart %.2f\n", new_play.start);

    printf("\n\n\tDuration of the play ('MM'): ");
    scanf(" %d", &(new_play.duration));
    printf("\n\tDuration %d min\n", new_play.duration);

    printf("\n\n\tTicket price ('$'): ");
    scanf(" %d", &(new_play.price));
    printf("\n\tPrice %d$\n", new_play.price);

    new_play.available = (new_play.played_in.rows * new_play.played_in.seats_per_row);  // calculation of seats

    // create seats - allocate memory
    short int** pSeats = (short int**)malloc(sizeof(short int*) * new_play.played_in.rows);
    for (int i = 0; i < new_play.played_in.rows; i++)
    {
        pSeats[i] = (short int*)malloc(sizeof(short int) * new_play.played_in.seats_per_row);
    }

    // fill the seats
    for (int i = 0; i < new_play.played_in.rows; i++)
    {
        for (int j = 0; j < new_play.played_in.seats_per_row; j++)
        {
            pSeats[i][j] = 0;
        }
    }
    new_play.pSeatsTable = pSeats;

    printf("\n\t!!! WARNING - be sure there is no other play taking place at that time in %s !!!", new_play.played_in.hall_name);
    printf("\n\tCheck the filled information. Are you sure you want to create this play? (y/n): ");
    fgets(help, 3, stdin);
    fgets(help, 3, stdin);
    if (!strcmp(help, "y\n")) {                         // confirmed
        FILE* fRoot = fopen("db/root.txt", "a");
        fprintf(fRoot, "%s\n", new_play.play_name);
        fclose(fRoot);
        save_play(new_play);
        printf("\n\tSuccesfully created!");
    }
    else {
        printf("\n\tNothing created! Leaving...");
        // free allocated memory
        for (int i = 0; i < new_play.played_in.rows; i++)
        {
            free(pSeats[i]);
        }
        free(pSeats);
    }

    Sleep(2000);
}


///////////////////////////////////////////////////////////////////////////
//////////////////////////////// USERS ////////////////////////////////////
///////////////////////////////////////////////////////////////////////////


void registration() {
    char username[_MAX_STR];
    printf("\n\n\tCreating a new user\n\n\tUsername: ");
    fgets(username, _MAX_STR, stdin);
    if(strlen(username) == 1) fgets(username, _MAX_STR, stdin);
    char* new_line = strchr(username, '\n');    // remove '\n'
    if(new_line) *new_line = '\0';
    if (username_check(username)) {
        printf("\n\tInvalid username format. Restarting...");
        Sleep(2000);
        return;
    }

    // create file name
    char file_name_buff[_MAX_STR + 10] = "us/";
    strcat(file_name_buff, username);
    strcat(file_name_buff, ".txt");
    FILE* fUser = fopen(file_name_buff, "r");

    if (fUser != NULL) {
        printf("\n\tThis user already exists");
        fclose(fUser);
        Sleep(2000);
        return;
    }

    char password[_MAX_STR];
    printf("\n\tPassword (max %d chars): ", _MAX_STR - 2);
    fgets(password, _MAX_STR, stdin);   // new password

    // saving user
    fUser = fopen(file_name_buff, "w");
    fprintf(fUser, "%s", password);
    fprintf(fUser, "0\n");
    fclose(fUser);

    printf("\n\tRegistration succesful!");
    Sleep(2000);
}


int username_check(char username[]) {
    for (int i = 0; i < strlen(username); i++) if (!isalnum((int)username[i]) && !isspace((int)username[i])) return 1;
    return 0;
}


void manage() {
    char username[_MAX_STR];
    printf("\n\n\tLogin\n\n\tUsername: ");
    fgets(username, _MAX_STR, stdin);           // login
    char* new_line = strchr(username, '\n');
    if(new_line) *new_line = '\0';

    // create file name
    char file_name_buff[_MAX_STR + 10] = "us/";
    strcat(file_name_buff, username);
    strcat(file_name_buff, ".txt");
    FILE* fUser = fopen(file_name_buff, "r");

    if (fUser == NULL) {
        printf("\n\tUser does not exist");
        Sleep(2000);
        return;
    }

    char password[_MAX_STR];
    char password_valid[_MAX_STR];
    printf("\n\tPassword: ");
    fgets(password, _MAX_STR, stdin);
    fgets(password_valid, _MAX_STR, fUser);
    if (strcmp(password, password_valid)) {             // password check
        printf("\n\tWrong password. Restarting...");
        fclose(fUser);
        Sleep(2000);
        return;
    }
    fclose(fUser);
    char ch = 0;
    while (ch != '4') {         // user menu
        system("cls");
        printf("\n\tWelcome, %s\n", username);
        printf("\n\tWhat action do you want to make?\n");
        printf("\n\t1.) Create new reservation\n");
        printf("\n\t2.) Show my reservations\n");
        printf("\n\t3.) Delete active reservation\n");
        printf("\n\t4.) Logout\n");
        ch = getch();
        switch (ch)
        {
        case '1':
            create_reservation(username);
            break;
        case '2':
            show_my(username);
            break;
        case '3':
            reg_del(username);
            break;
        default:
            break;
        }
    }
}


void create_reservation(char username[]) {
    FILE* fRoot = fopen("db/root.txt", "r");       // plays root
    if (fRoot == NULL) {
        printf("\nError opening root file\n");
        return;
    }

    system("cls");

    char** list_of_plays = malloc(sizeof(char*));
    int i = 0;
    while (1)
    {
        list_of_plays[i] = malloc(_MAX_STR);
        if (fgets(list_of_plays[i], _MAX_STR, fRoot) == 0) break;
        printf("\n\t=> %d\tName: %s\n\n", i + 1, list_of_plays[i]);     // list of all plays
        i++;
        list_of_plays = realloc(list_of_plays, sizeof(char*) * (i + 1));
    }
    fclose(fRoot);

    int choose;
    printf("\n\t\tYour choice: ");
    scanf(" %d", &choose);
    system("cls");
    if (choose > 0 && choose < (i + 1)) {
        char* new_line = strchr(list_of_plays[choose - 1], '\n');
        if (new_line) *new_line = '\0';
        PLAY_HEAD loaded = load_play(list_of_plays[choose - 1]);

        printf("\n\tName: %s  \tHall: %s\n", loaded.play_name, loaded.played_in.hall_name);
        printf("\tStars at: %.2f  \tDuration: %d  \tAvailable seats: %d\n", loaded.start, loaded.duration, loaded.available);
        printf("\tPrice: %d$\n\n", loaded.price);

        print_theatre_hall(loaded);         // details about chosen play

        char help[_MAX_STR + 4];
        int seat = -1;
        char row = -1;
        printf("\n\tEnter your seat (eg. A 12): ");
        fgets(help, _MAX_STR + 4, stdin);
        fgets(help, _MAX_STR + 4, stdin);
        new_line = strchr(help, '\n');
        if (new_line) *new_line = '\0';
        sscanf(help, "%c", &row);     // choosing seat
        sscanf(help + 2, "%d", &seat);
        row = row - 'A';
        seat--;
        printf("\n\tYour choice: %c %d\n", help[0], seat + 1);
        if (row < 0 || row >= loaded.played_in.rows || seat < 0 || seat >= loaded.played_in.seats_per_row) {
            printf("\n\tERROR - Invalid format");       // check, wheter the seat exists
            Sleep(2000);
            for (int j = 0; j < i + 1; j++)
            {
                free(list_of_plays[j]);
            }
            free(list_of_plays);
            save_play(loaded);
            return;
        }

        if (loaded.pSeatsTable[(int)row][seat]) {
            printf("\n\tSeat is taken");
            Sleep(2000);
            for (int j = 0; j < i + 1; j++)
            {
                free(list_of_plays[j]);
            }
            free(list_of_plays);
            save_play(loaded);
            return;
        }

        printf("\n\tDo you confirm your reservation? (y/n): ");
        char ch = getch();
        if (ch == 'y') {
            USER_HEAD user = load_user(username);

            user.num_reservations++;
            loaded.available--;
            char** pActive = user.pActive_reservations;

            pActive = realloc(pActive, sizeof(char*) * user.num_reservations);
            pActive[user.num_reservations - 1] = malloc(sizeof(char) * (_MAX_STR + 4));
            strcat(help, " ");
            strcat(help, loaded.play_name);             // row in user.txt file
            loaded.pSeatsTable[(int)row][seat] = 1;     // seat is now taken
            strcpy(pActive[user.num_reservations - 1], help);   // add info into HEAD
            user.pActive_reservations = pActive;
            save_user(user);
        }
        save_play(loaded);
    }
    // free allocated memory
    for (int j = 0; j < i + 1; j++)
    {
        free(list_of_plays[j]);
    }
    free(list_of_plays);
}


void show_my(char username[]) {
    system("cls");
    USER_HEAD user = load_user(username);
    PLAY_HEAD play;
    for (int i = 0; i < user.num_reservations; i++)
    {
        char* tok1, *tok2;
        char buffer[_MAX_STR + 4];
        strcpy(buffer, user.pActive_reservations[i]);   // A 12 play_name format
        tok1 = strstr(buffer, " ");
        *tok1 = '\0';
        tok1++;
        tok2 = strstr(tok1, " ");
        *tok2 = '\0';
        tok2++;                             // gets name of the play without seat
        play = load_play(tok2);

        printf("##################################################\n");
        printf("\n\tName: %s  \tHall: %s\n", tok2, play.played_in.hall_name);
        printf("\tStars at: %.2f  \tDuration: %d\n", play.start, play.duration);
        printf("\tPrice: %d$\n", play.price);
        printf("\tMy seat: %s %s\n\n", buffer, tok1);
        printf("##################################################\n\n\n\n");

        Sleep(50);

        save_play(play);
    }

    char ch;
    printf("\n\tPress any key to leave: ");
    ch = getch();

    save_user(user);
}


void reg_del(char username[]) {
    system("cls");
    USER_HEAD user = load_user(username);
    for (int i = 0; i < user.num_reservations; i++)
    {
        printf("\t%d.) %s\n\n", i + 1, user.pActive_reservations[i]);   // prints active reservations
    }
    printf("\n\n\tType number of reservation you want to remove: ");
    int index;
    scanf(" %d", &index);
    index--;

    char file_name_buff[_MAX_STR + 4];
    strcpy(file_name_buff, user.pActive_reservations[index]);
    printf("\n\tDo you want to delete: %s? (y/n)  ", file_name_buff);   // prints chosen reservation
    char ch = getch();
    if (ch == 'y') {
        char* tok1;
        tok1 = strstr(file_name_buff, " ");
        tok1++;
        tok1 = strstr(tok1, " ");
        tok1++;                                 // gets name of the play without seat
        PLAY_HEAD play = load_play(tok1);       

        int seat;
        char row;
        sscanf(file_name_buff, "%c %d ", &row, &seat);
        row = row - 'A';
        seat--;

        if (row < 0 || row >= play.played_in.rows || seat < 0 || seat >= play.played_in.seats_per_row) {
            printf("\n\tERROR - Invalid format");   // internal check
            Sleep(2000);
            save_user(user);
            save_play(play);
            return;
        }

        user.num_reservations--;
        play.available++;
        play.pSeatsTable[(int)row][seat] = 0;       // free the seat
        save_play(play);

        strcpy(user.pActive_reservations[index], user.pActive_reservations[user.num_reservations]); // move the last reservation into deleted slot
        free(user.pActive_reservations[user.num_reservations]);
    }

    save_user(user);
}


///////////////////////////////////////////////////////////////////////////
//////////////////////////////// DATA /////////////////////////////////////
///////////////////////////////////////////////////////////////////////////


int db_check() {
    FILE* fRoot = fopen("db/root.txt", "r");
    if (!fRoot)
    {
        printf("\n\nERROR! It seems like \"/db\" directory or \"db/root.txt\" file is missing in your installation folder. Try reinstalling the program\n");
        return 1;
    }

    fclose(fRoot);
    return 0;
}


int us_check() {
    FILE* fRoot = fopen("us/root.txt", "r");
    if (!fRoot)
    {
        printf("\n\nERROR! It seems like \"/us\" directory or \"us/root.txt\" file is missing in your installation folder. Try reinstalling the program\n");
        return 1;
    }

    fclose(fRoot);
    return 0;
}


void save_user(USER_HEAD user) {
    // save header
    // create file name
    char file_name_buff[_MAX_STR + 10] = "us/";
    strcat(file_name_buff, user.username);
    strcat(file_name_buff, ".txt");

    FILE* fUser = fopen(file_name_buff, "w");

    if (fUser == NULL) {
        printf("\nError opening file\n");
        return;
    }

    fprintf(fUser, "%s\n", user.password);              // 1st line - password
    fprintf(fUser, "%d\n", user.num_reservations);      // 2nd line - num of reservations

    // save active reservations
    // create file name
    for (int i = 0; i < user.num_reservations; i++)
    {
        fprintf(fUser, "%s\n", user.pActive_reservations[i]);
    }

    // free
    for (int i = 0; i < user.num_reservations; i++)
    {
        free(user.pActive_reservations[i]);
    }
    free(user.pActive_reservations);
    fclose(fUser);
}


USER_HEAD load_user(char file_name[]) {
    USER_HEAD result;
    strcpy(result.username, file_name);

    // load header
    // create file name
    char file_name_buff[_MAX_STR + 10] = "us/";
    strcat(file_name_buff, file_name);
    strcat(file_name_buff, ".txt");
    FILE* fUser = fopen(file_name_buff, "r");

    if (fUser == NULL) {
        printf("\nUser not found\n");
        return result;
    }

    char help[_MAX_STR + 4];
    fgets(help, _MAX_STR + 4, fUser);
    char* new_line = strchr(help, '\n');
    if(new_line) *new_line = '\0';
    strcpy(result.password, help);          // 1st line - password
            
    fgets(help, _MAX_STR + 4, fUser);       // 2nd line - num of reservations
    new_line = strchr(help, '\n');
    if(new_line) *new_line = '\0';
    sscanf(help, "%d", &result.num_reservations);

    // create reservations
    char** pActive = (char**)malloc(sizeof(char*) * result.num_reservations);
    for (int i = 0; i < result.num_reservations; i++)
    {
        pActive[i] = (char*)malloc(sizeof(char) * (_MAX_STR + 4));
    }

    // fill the reservations
    for (int i = 0; i < result.num_reservations; i++)
    {
        fgets(help, _MAX_STR + 4, fUser);
        new_line = strchr(help, '\n');
        if(new_line) *new_line = '\0';
        strcpy(pActive[i], help);
    }
    result.pActive_reservations = pActive;

    fclose(fUser);

    return result;
}


void save_play(PLAY_HEAD play) {
    // save header
    // create file name
    char file_name_buff[_MAX_STR + 10] = "db/";
    strcat(file_name_buff, play.play_name);
    strcat(file_name_buff, ".txt");
    FILE* fPlay = fopen(file_name_buff, "wb");

    if (fPlay == NULL) {
        printf("\nError opening file\n");
        return;
    }

    if (!fwrite(&play, sizeof(PLAY_HEAD), 1, fPlay)) {  // copy the header
        printf("\nError writing to file\n");
    }

    fclose(fPlay);

    // save available seats
    // create file name
    strcpy(file_name_buff, "db/");
    strcat(file_name_buff, play.play_name);
    strcat(file_name_buff, "_st.txt");
    FILE* fSeats = fopen(file_name_buff, "w");

    if (fSeats == NULL) {
        printf("\nError opening file\n");
        return;
    }

    for (int i = 0; i < play.played_in.rows; i++)
    {
        for (int j = 0; j < play.played_in.seats_per_row; j++){
            fprintf(fSeats, "%d", play.pSeatsTable[i][j]);
        }
        fprintf(fSeats, "\n"); // end of row
    }

    // free
    for (int i = 0; i < play.played_in.rows; i++)
    {
        free(play.pSeatsTable[i]);
    }
    free(play.pSeatsTable);
    fclose(fSeats);
}


PLAY_HEAD load_play(char file_name[]) {
    PLAY_HEAD result;

    // load header
    // create file name
    char file_name_buff[_MAX_STR + 10] = "db/";
    strcat(file_name_buff, file_name);
    strcat(file_name_buff, ".txt");
    FILE* fPlay = fopen(file_name_buff, "rb");

    if (fPlay == NULL) {
        printf("\nPlay not found\n");
        return result;
    }

    fread(&result, sizeof(PLAY_HEAD), 1, fPlay);    // read header  

    fclose(fPlay);
 
    // load available seats
    // create file name
    strcpy(file_name_buff, "db/");
    strcat(file_name_buff, file_name);
    strcat(file_name_buff, "_st.txt");
    FILE* fSeats = fopen(file_name_buff, "r");

    if (fSeats == NULL) {
        printf("\nError loading file\n");
        return result;
    }

    // allocate memory
    short int** pSeats = (short int**)malloc(sizeof(short int*) * result.played_in.rows);
    for (int i = 0; i < result.played_in.rows; i++)
    {
        pSeats[i] = (short int*)malloc(sizeof(short int) * result.played_in.seats_per_row);
    }

    // fill the seats
    for (int i = 0; i < result.played_in.rows; i++)
    {
        for (int j = 0; j < result.played_in.seats_per_row; j++)
        {
            pSeats[i][j] = (short int)(fgetc(fSeats) - '0');
        }
        fgetc(fSeats); // '\n'
    }
    result.pSeatsTable = pSeats;

    fclose(fSeats);

    return result;
}