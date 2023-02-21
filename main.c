#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <windows.h>
#include <time.h>
#include <string.h>

//global variables
int fx; //x coordinate of the food
int fy; //y coordinate of the food
int i;
char c;
char s;
int n;
int x; //x coordinate of the head of the snake
int y; //y coordinate of the head of the snake
int hx; //variables to manage the movement of the snake
int hy;
int length; //length of the snake
int pressm;
int score; //variable for score
char ch;
int game; //variable for game continuity
int controlgame; //variable for initializing a new game or a previously loaded game
int day, month, year, hour, min; //variables to store the time data
char filename[20]; //string for the filename based on time data
char txt[] = ".txt"; //a string that will be used to concatenate to create a txt file
char gamenames[300]; //a sting to display the game names when the user wants to load a previously saved game

int timefunc() //getting the local time information from time.h header file
{               //accessing the structure defined inside time.h
    time_t lt;  //storing them into day, month, hour, min variables
    lt = time(NULL);
    struct tm *local = localtime(&lt);
    day = local->tm_mday;
    month = local->tm_mon + 1;
    year = local->tm_year + 1900;
    hour = local->tm_hour;
    min = local->tm_min;
    return day, month, year, hour, min;
}

char gettime() //getting the local time information and saving it into a string
{
    timefunc();
    snprintf(filename, 20, "%d_%d_%d_%d_%d", day, month, year, hour, min);
    return filename;
}

struct tail //structure to keep the tail coordinate values
{
    int tailx;
    int taily;
}*tailptr;

//function prototypes
void boardsize(int n);
void mainmenu();
void optionsmenu();
void gameini();
void printgameover();
void displayscore();
void savegame();
void loadgame(char nameofthefile[]);
void saveoptions();
void loadoptions();
void gamerules();
int newgamevariables();
void headandtailini();
void savegametocont();
void loadgametocont();
void getinput();
void eatfood();
void decidegameover();
void savegamenames();

void gotoxy(int col, int row) //function to set the cursor position to inputted coordinates
{                             //by using windows.h header file
    HANDLE a = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD position = {col - 1, row - 1};
    SetConsoleCursorPosition(a, position);
}

int generatefood(int n) //function to randomly generate a random food
{
    srand(time(NULL));
    fy = 5 + (rand() % (n-5));
    fx = 3 + (rand() % ((2*n)-6));
    return fx, fy;
}

void printfood() //function to print the food
{
    gotoxy(fx, fy);
    printf("\u23FA");
}

void main()
{
    system("chcp 65001");
    system("cls");
    mainmenu();
}

void boardsize(int n) //creating the field of play with Unicode characters
{                     //n is the input from the player to determine the board size
    gotoxy(1, 3);     //this variable is also going to be stored in a file
    int m;
    for (m = 0; m < n; m++) {
        if (m == 0) {
            for (i = 0; i < n; i++) {
                if (i == 0) {
                    printf("\u2554");
                } else if (i == (n - 1)) {
                    printf("\u2557");
                } else {
                    printf("\u2550\u2550");
                }
            }
        }
        else if (m == (n - 1)) {
            for (i = 0; i < n; i++) {
                if (i == 0) {
                    printf("\u255A");
                } else if (i == (n - 1)) {
                    printf("\u255D");
                } else {
                    printf("\u2550\u2550");
                }
            }
        }
        else {
            for (i = 0; i < n; i++) {
                if (i == 0) {
                    printf("\u2551");
                } else if (i == (n - 1)) {
                    printf("\u2551");
                } else {
                    printf("  ");
                }
            }
        }
        printf("\n");
    }
}

int newgamevariables() //these are the variables that will be called when a new game is selected
{
    score = 0;
    x = (2*(n-1))/2;
    y = n/2;
    hx = 0;
    hy = 0;
    return score, x, y, hx, hy;
}

void gameini() //initializing the game
{
    game = 1; //for game continuity
    tailptr = (struct tail*) malloc(100 * sizeof(struct tail)); //allocating memory for tail coordinates
    if (c == '1') { //determining the board size by the user input
        n = 16; //small board 16x16
    } else if (c == '2') {
        n = 32; //medium board 32x32
    } else if (c == '3') {
        n = 64; //large board 64x64
    }
    if (controlgame == 1) { //control variable to initialize the game as a new game
        newgamevariables(); //generating new game variables
        length = 1; //generating the length
        generatefood(n); //generating food coordinates
    } else if (controlgame == 0) {      //control variable to initialize the game as a previously saved game
        FILE *load;
        load = fopen("names.txt", "r"); //opening the file that contains the game names
        if (load == NULL) {
            printf("\nYou have zero saved games!");
            printf("\n\n\nPRESS ANY KEY TO GO BACK TO MAIN MENU");
            getch();
            system("cls");
            return mainmenu();
        }
        fgets(gamenames, 300, load);
        for (char *p = gamenames; (p = strchr(p, ' ')) != NULL; p++) { //replacing the spaces between game names in the file with a new line
            *p = '\n';
        }
        printf("\n");
        printf("PREVIOUSLY SAVED GAMES\n");
        printf("\n");
        printf("%s", gamenames); //displaying game names
        printf("\n");
        printf("Enter the game name: ");    //getting the user input to select the game file
        char loadgamename[20] = {0};        //user should write the game name as displayed
        int k = 0;
        ch = getchar();
        if (ch == '\n')
        {
            ch = getchar();
        }
        while(ch != '\n')
        {
            loadgamename[k] = ch;
            k++;
            ch = getchar();
        }
        strcat(loadgamename, txt); //concatenating the user input with ".txt" to open the file
        system("cls");
        loadgame(loadgamename);   //loading the exact game that the user wants
        printf("\n\n\tPRESS ANY KEY TO PLAY");
        getch();
    }
    boardsize(n); //displaying the field of play depending on game settings
    displayscore(); //displaying score
    printfood(); //printing food for the first time
    while(game == 1) {
        getinput(); //getting the keyboard input from the user
        eatfood();  //to determine what happens when the snake eats food
        decidegameover();  //to determine the conditions for losing the game
        (tailptr+0)->tailx = x;
        (tailptr+0)->taily = y;
        for (i = length; i > 0; i--) {
            (tailptr+i)->tailx = (tailptr+(i-1))->tailx; //synchronizing tail variables
            (tailptr+i)->taily = (tailptr+(i-1))->taily;
        }
        headandtailini(); //printing the head and the tails of the snake
    }
    free(tailptr); //to free the memory which was allocated before
}

void decidegameover() //to determine the conditions for losing the game
{
    for (i = 2; i <= length; i++) {
        if ((tailptr+i)->tailx == x && (tailptr+i)->taily == y) { //if the snake eats himself it is game over
            system("cls");                                        //head of the snake cannot touch tail
            printgameover();  //print game over                   //if it does, it is game over
            game = 0;
            Sleep(1000);
            system("cls");
            return mainmenu(); //return main menu
        }
    }
    if (x == 1 || x == ((2*n) - 2) || y == 3 || y == n + 2) { //if the snake hits the walls of the field of play it is game over
        system("cls");                                        //head of the snake cannot touch tail
        printgameover();  //print game over
        game = 0;
        Sleep(1000);
        system("cls");
        return mainmenu(); //return main menu
    }
}

void eatfood() //to determine what happens when the snake eats food
{
    if (fx == x && fy == y) { //if the head of the snake coordinates meet the food coordinates
        generatefood(n);  //new food will be generated
        printfood(); //new food will be printed
        length++; //length of the snake will be incremented
        score += 5; //score will be incremented by 5 points
        displayscore(); //new score will be displayed
    }
}

void getinput() //getting the keyboard inputs from the user
{
    if (kbhit())
        {
            ch = getch(); //get the key
            switch (ch) {
                case 75: //if the key is left arrow key
                    hy = 0;
                    hx = -1; //go left
                    break;
                case 80: //if the key is upper arrow key
                    hy = 1; //go up
                    hx = 0;
                    break;
                case 77: //if the key is right arrow key
                    hy = 0;
                    hx = 1; //move right
                    break;
                case 72: //if the key is lower arrow key
                    hy = -1; //move down
                    hx = 0;
                    break;
                case 109: //pressing 'm'
                    savegametocont(); //save game to continue to play when the user presses 2
                    system("cls");
                    gotoxy(10,9);
                    printf("Are you sure?(Press 1 to exit or press 2 to continue):");
                    scanf("%d", &pressm);
                    if (pressm == 1) {
                        savegame(); //saves game and creates a file depends on time
                        savegamenames();
                        game = 0;
                        system("cls");
                        return mainmenu(); //returns back to main menu
                    } else if (pressm == 2) { //loads game to continue to play
                        system("cls");
                        printf("\n\n");
                        loadgametocont(); //loads the game variables before pressing m
                        boardsize(n); //prints the board
                        printfood(); //prints food
                        displayscore(); //prints the score
                    }
                    break;
                case 115: //press 's' to save
                    savegame(); //saves the game data
                    savegamenames();
                    break;
                case 83: //press 'S' to save
                    savegame(); //saves the game data
                    savegamenames();
                    break;
                default: //pressing any other key will have no effect
                    break;
            }
    }
    x += hx; //moving the head of the snake by changing the coordinates
    y += hy;
}

void headandtailini() //initializing head and the tail
{
        for (i = 2; i <= length; i++) { //printing the tail of the snake
            gotoxy((tailptr+i)->tailx, (tailptr+i)->taily);
            printf("\u23F9");
        }
        gotoxy(x, y); //printing the head of the snake
        printf("\u23F9");
        if (s == '1') { //determines the game speed based on the game settings
            Sleep(40);
        } else if (s == '2') {
            Sleep(70);
        } else if (s == '3') {
            Sleep(100);
        }
        gotoxy(x, y); //deleting the head of the snake for continuity of the snake
        printf(" ");
        for (i = 2; i <= length; i++) { //deleting the tail of the snake for continuity of the snake
            gotoxy((tailptr+i)->tailx, (tailptr+i)->taily);
            printf(" ");
        }
}

void printgameover() //printing game over
{
    gotoxy(((2*(n-1))/2), n/2);
    printf("GAME OVER\n\n\n");
}

void displayscore() //displaying score
{
    gotoxy(((2*n) - 12), 1);
    printf("Score: %d", score);
}

void gamerules() //printing game rules
{
    printf("\n\t-----------------SNAKEGAME-----------------");
    printf("\n\t 1-Use arrow keys to move the snake around");
    printf("\n\t 2-Snake gets bigger when it eats food");
    printf("\n\t 3-When snake eats, you gain 5 points");
    printf("\n\t 4-If snake touches the walls, GAME OVER");
    printf("\n\t 5-If snake eats itself, GAME OVER");
    printf("\n\t 6-The game continues until the snake dies");
    printf("\n\t 7-You can press 'm' to save the game and then decide whether to quit or continue");
    printf("\n\t 8-You can press either 's' or 'S' to save the game while playing");
    printf("\n\t 9-You can set the board size and the game speed from the game settings");
    printf("\n\t-----------------ENJOY---------------------");
    printf("\n\n\t PRESS ANY KEY TO CONTINUE");
    getch();
    system("cls");
}

void mainmenu()
{
    FILE *checker;
    checker = fopen("gameoptions.txt", "r"); //checking if the user set the game settings to create a new game
    int choice = 0;
    while (choice != 4) { //printing the menu
        printf("Press 1 for Game Settings\n");
        printf("Press 2 for Load game\n");
        printf("Press 3 for New Game\n");
        printf("Press 4 to Exit the game\n");
        printf("Press 5 to See the game rules\n");
        scanf("%d", &choice);
        system("cls");
        switch (choice) {
            case 1:
                optionsmenu(); //goes to game settings menu
                break;
            case 2: //initializing previously saved game
                controlgame = 0;
                gameini();
                break;
            case 3:
                if (checker == NULL) { //checking if the user set the game settings to create a new game
                    gotoxy(10,9);
                    printf("ADJUST GAME SETTINGS!");
                    Sleep(1000);
                    system("cls");
                    return mainmenu(); //returns back to main menu
                } else { //if the user set the game settings
                    controlgame = 1;
                    loadoptions(); //load the game settings
                    gameini(); //initialize a new game
                }
                break;
            case 4: //exiting the game
                exit(0);
                break;
            case 5: //printing the game rules
                gamerules();
                break;
            default: //if the user enters a nonvalid key
                system("cls");
                gotoxy(10,9);
                printf("PRESS A VALID KEY!"); //make a warning
                Sleep(3);
                system("cls");
                return mainmenu(); //return back to main menu
                break;
        }
    }
}

void savegamenames() //saving the game names to display them after
{
    FILE *save;
    gettime(); //gets the current time into the filename string
    save = fopen("names.txt", "a");
    fprintf(save, "%s ", filename); //writing it into the file
    fclose(save);
}

void savegame() //saves the game based on time
{
    FILE *save;
    gettime(); //gets the current time into the filename string
    strcat(filename, txt); //concatenating the filename with ".txt"
    save = fopen(filename, "w"); //writing the game variables into the file
    fprintf(save, "%d %d %d %d %d %c %d %d %d %d ", fx, fy ,score, length, n, s, x, y, hx, hy);
    for (i = 0; i <= length; i++) {
        fprintf(save, "%d %d ", (tailptr+i)->tailx, (tailptr+i)->taily);
    }
    fclose(save);
}

void loadgame(char nameofthefile[]) //loads the previously saved game by the user input
{
    FILE *load;
    load = fopen(nameofthefile, "r"); //loads the game variables of that game from the file
    fscanf(load, "%d %d %d %d %d %c %d %d %d %d ", &fx, &fy, &score, &length, &n, &s, &x, &y, &hx, &hy);
    for (i = 0; i <= length; i++) {
        fscanf(load, "%d %d", &((tailptr+i)->tailx), &((tailptr+i)->taily));
    }
    fclose(load);
}

void savegametocont() //saves the game variables to continue the game
{
    FILE *save;
    save = fopen("continue.txt", "w"); //writes the game variables into the file
    fprintf(save, "%d %d %d %d %d %c %d %d %d %d ", fx, fy ,score, length, n, s, x, y, hx, hy);
    for (i = 0; i <= length; i++) {
        fprintf(save, "%d %d ", (tailptr+i)->tailx, (tailptr+i)->taily);
    }
    fclose(save);
}

void loadgametocont() //loads the game variables to continue the game
{
    FILE *load;
    load = fopen("continue.txt", "r"); //reads the game variables from the file
    fscanf(load, "%d %d %d %d %d %c %d %d %d %d ", &fx, &fy, &score, &length, &n, &s, &x, &y, &hx, &hy);
    for (i = 0; i <= length; i++) {
        fscanf(load, "%d %d", &((tailptr+i)->tailx), &((tailptr+i)->taily));
    }
    fclose(load);
}

void saveoptions() //saves the game options(board size and the speed of the game) to the file
{
    FILE *options;
    options = fopen("gameoptions.txt", "w");
    fprintf(options, "%c %c", s, c);
    fclose(options);
}

void loadoptions() //loads the game options(board size and the speed of the game) from the file
{
    FILE *loadoptions;
    loadoptions = fopen("gameoptions.txt", "r");
    fscanf(loadoptions, "%c %c", &s, &c);
    fclose(loadoptions);
}

void optionsmenu() //game options menu
{
    printf("\nEnter the speed of the game (Press 1 for high speed, 2 for medium speed or 3 for low speed): ");
    s = getch(); //gets the variable to determine the speed of the game
    printf("\n");
    printf("\nChoose the board size\nPress 1 for (16x16)\nPress 2 for (32x32)\nPress 3 for (64x64): ");
    c = getch(); //gets the variable to determine the board size
    saveoptions(); //saves these options
    system("cls");
    return mainmenu(); //returns back to main menu
}
