#include<stdio.h>
#include<string.h>
#include<conio.h>
#include<cstdlib>
#include<ctype.h>

#define MIN 1
#define MAX_ROWS 4      //Make sure MAX_ROWS is at least 4 and not more than 10 so that display and stuff are proper
#define TRUE 1
#define FALSE 0
#define QUIT -1
#define ROW_FLAG 1
#define ZERO 0
#define USERNAME_LEN_LIMIT 11 // Though this is 11, only 10 chars are allowed. 11 because one holds null at the end
#define SPACE ' '
#define DEFAULT_GAME_OBJ 'o'
#define INVALID_INPUT_ERROR_MSG "\n\n<<< ERROR : Invalid input it is! Give a valid input! >>>\n"
#define ENTER_ROW_CHOICE "\n\nChoose a row to remove objects from: "
#define ENTER_REMOVE_COUNT "\nEnter the number of objects you want to remove from the selected row: "
#define QUIT_WARNING_MSG "\n<<< YOU ENTERED -1 REQUESTING TO QUIT THE GAME, IF YOU QUIT, THE OTHER PLAYER WILL BE DECLARED WINNER >>>"
#define QUIT_CONFIRM_MSG "\n\nIF YOU REALLY WANT TO QUIT, ENTER -1 AGAIN, ELSE ENTER A VALID INPUT: "
#define INVALID_NAME "\n<<< ERROR: YOUR NAME SHOULD ONLY CONTAIN ALPHABETS AND SHOULD NOT EXCEED 10 CHARACTERS! >>>\n"
#define JARVIS "JARVIS"
#define PLAYER "PLAYER"

void nimGame();
void playWithJarvis();
void playWithFriend();
void displayInstructions();
void jarvisPlay(int *rows, int *moves);
void displayGameState(const int *rows, const char gameObj);
int getValidInput(const int *rows, const int maxLimit, const char* msg, const int rowFlag);
short gameOver(const int *rows);
void declareWinner(const char *winner);
short updateProfilePreference();
char * getPlayerName(short player2);
short validName(const char *name);
char getGameObj();
void printGameObjRandom(const char gameObj);
void initRows(int *rows);
void printHomeMenu();
void showAvailableGameObjs();

int main() {
    nimGame();
    return 0;
}

/*
    * Nim Game starts here
    * Home Menu is printed and choice is taken
    * There are 4 options:
        (1) Play with computer
        (2) Play with a friend
        (3) Show Instructions
        (4) Exit
    * If an invalid input is given, the system would bash you up!
*/
void nimGame() {
    int choice;

    do {
        printHomeMenu();
        printf("\nEnter your choice: ");
        scanf("%d", &choice);

        switch(choice) {
            case 1:
                playWithJarvis();
                break;
            case 2:
                playWithFriend();
                break;
            case 3:
                system("cls");
                displayInstructions();
                break;
            case 4:
                break;
            default:
                printf(INVALID_INPUT_ERROR_MSG);
        }
        if(choice != 4) {
            printf("\nPress Enter to Continue...\t");
            fflush(stdin);
            getchar();
            fflush(stdin);
            system("cls");
        }
    } while(choice != 4);
}

/*
    * Method to play with Jarvis (computer)
    * This method is written such that Jarvis plays intelligently
    * It is almost impossible to win against Jarvis
*/
void playWithJarvis() {
    int rows[MAX_ROWS];
    int rowChoice;
    int removeCount;
    short player = 1;
    short firstTime = 1;
    char playerName[10] = PLAYER;
    char gameObj = DEFAULT_GAME_OBJ;

    // THE PLAYER CAN UPDATE HIS/HER PROFILE BY GIVING A NAME AND SELECTING GAME OBJECT
    if(updateProfilePreference()) {
        strcpy(playerName, getPlayerName(FALSE));
        gameObj = getGameObj();
        printf("\n\n<<< EVERYTHING IS SET FOR THE GAME! ALL THE BEST %s >>>\n", playerName);
        printf("\nPress Enter to Continue...");
        fflush(stdin);
        getchar();
        fflush(stdin);
    }

    initRows(rows);
    do {
        int moves[2]; // moves[0] is row number, moves[1] is the number of removed objects
        system("cls");
        displayGameState(rows, gameObj);
        if(firstTime == 0) {
            printf("\n\n<<< JARVIS REMOVED %d OBJECTS FROM ROW %d >>>", moves[1], moves[0] + 1);
        }
        firstTime = 0;
        printf("\n\nIt is your turn %s...", playerName);

        // RECEIVING INPUT - ROWCHOICE AND REMOVECOUNT
        rowChoice = getValidInput(rows, MAX_ROWS, ENTER_ROW_CHOICE, ROW_FLAG);
        if(rowChoice == -1) {
            player = 2;
            break;
        }
        removeCount = getValidInput(rows, rows[rowChoice], ENTER_REMOVE_COUNT, ZERO);
        if(removeCount == -1) {
            player = 2;
            break;
        }
        // END OF RECEIVING INPUT - ROWCHOICE AND REMOVECOUNT

        rows[rowChoice] -= removeCount;
        player = (player == 1)? 2 : 1;

        // JARVIS TURN GOES IN THE FOLLOWING IF BLOCK
        if(player == 2 && gameOver(rows) != TRUE) {
            system("cls");
            displayGameState(rows, gameObj);
            jarvisPlay(rows, moves);
            printf("\n\n\t<<< JARVIS HAS DECIDED THE MOVE! PRESS ENTER TO FIND WHAT JARVIS IS GOING TO PLAY! >>>");
            fflush(stdin);
            getchar();
            fflush(stdin);
            player = (player == 1)? 2 : 1;
        }
    } while(gameOver(rows) != TRUE);

    if(player == 1) {
        declareWinner(playerName);
    } else {
        declareWinner(JARVIS);
    }
}

/*
    * To enable 2 player mode for Nim Game
    * Each player enters a row number and the number of objects to be removed
    * A player has to enter valid number or rows/objects or -1 for the flow to move forward
    * Else the flow keeps on looping at the same position asking the user for a valid input
    * When a player quits the game or when there are no further moves, winner is declared
    * Again the control is taken back to the main menu after the game
*/
void playWithFriend() {
    int rows[MAX_ROWS];
    int rowChoice;
    int removeCount;
    short player = 1;
    char player1[10] = "PLAYER 1";
    char player2[10] = "PLAYER 2";
    char gameObj = DEFAULT_GAME_OBJ;

    // THE PLAYER CAN UPDATE HIS/HER PROFILE BY GIVING NAMES AND SELECTING GAME OBJECT
    if(updateProfilePreference()) {
        strcpy(player1, getPlayerName(FALSE));
        do {
            strcpy(player2, getPlayerName(TRUE));
            if(strcmp(player1, player2) == 0) {
                printf("\n<<< BOTH PLAYERS CANNOT USE SAME NAME >>>\n");
            }
        } while(!strcmp(player1, player2));
        gameObj = getGameObj();
        printf("\n\n<<< EVERYTHING IS SET FOR THE GAME! ALL THE BEST %s and %s >>>\n", player1, player2);
        printf("\nPress Enter to Continue...");
        fflush(stdin);
        getchar();
        fflush(stdin);
    }

    initRows(rows);
    do {
        system("cls");
        displayGameState(rows, gameObj);
        if(player == 1) {
            printf("\n\nIt is your turn %s...", player1);
        } else {
            printf("\n\nIt is your turn %s...", player2);
        }

        // RECEIVING INPUT - ROWCHOICE AND REMOVECOUNT
        rowChoice = getValidInput(rows, MAX_ROWS, ENTER_ROW_CHOICE, ROW_FLAG);
        if(rowChoice == -1) {
            player = (player == 1)? 2 : 1;
            break;
        }
        removeCount = getValidInput(rows, rows[rowChoice], ENTER_REMOVE_COUNT, ZERO);
        if(removeCount == -1) {
            player = (player == 1)? 2 : 1;
            break;
        }
        // END OF RECEIVING INPUT - ROWCHOICE AND REMOVECOUNT

        rows[rowChoice] -= removeCount;
        player = (player == 1)? 2 : 1;
    } while(gameOver(rows) != TRUE);

    if(player == 1) {
        declareWinner(player1);
    } else {
        declareWinner(player2);
    }
}

/*
    * Displays the instructions to play Nim Game
    * After pressing enter again the main menu is displayed
*/
void displayInstructions() {
    printf("\n\n***************************************************************************************************");
    printf("\n*                       ~@~   INSTRUCTIONS FOR PLAYING NIM GAME   ~@~                             *\n");
    printf("***************************************************************************************************\n");

    printf("\n\t* This is a very simple game!");
    printf("\n\t* You have %d rows and few objects in each row", MAX_ROWS);
    printf("\n\t* You select a row with objects available");
    printf("\n\t* You say how many objects you want to remove from that row");
    printf("\n\t* Remember, in one turn, you have to remove objects from only one row");
    printf("\n\t* Also, you have to remove at least one object in each of your turns");
    printf("\n\t* This continues till there are no objects in any row");
    printf("\n\t* The one who removes the last object loses and the other player wins!");
    printf("\n\t* Any moment you want to quit the game and go back, give -1 as input");
    printf("\n\t* The one who quit the game is declared as to lose the game and the other one - Winner!");
    printf("\n\n\n\t\tNow, start the game and get your brains to work!");
    printf("\n\t\t   (; ~@~ H:a:p:p:y ;;; G:a:m:i:n:g:! ~@~ ;) ");

    printf("\n\n***************************************************************************************************");
    printf("\n*                                     END OF INSTRUCTIONS                                         *\n");
    printf("***************************************************************************************************\n");
}

/*
    * Method to implement the core logic of JarvisPlay
    * This logic makes Jarvis move intelligently and gives the most chances to win the game
    * Takes rows array and moves array as input
    * moves array is taken to communicate back what row is chosen and how many objects were removed
*/
void jarvisPlay(int *rows, int *moves) {
    int i;
    int zeroes = ZERO;
    int oneRows = ZERO;
    int targetRow = ZERO;
    int maxCount = ZERO;
    short bitArray[MAX_ROWS][3] = {0};
    int powersCount[3] = {0};
    int index = ZERO;

    for(i = 0;i < MAX_ROWS;i++) {

        int tempElement = rows[i];

        if(rows[i] == ZERO) {        // FINDS THE NO. OF ROWS CONTAINING ZERO OBJECTS
            zeroes++;
        }
        if(rows[i] > maxCount) {    // FINDS THE MAX AMONG THE ROWS
            maxCount = rows[i];
            targetRow = i;
        }
        if(rows[i] == MIN) {        //  FINDS THE NUMBER OF ROWS CONTAINING '1' OBJECT
            oneRows++;
        }

        // THIS IS A BIT TRICKY. THIS GETS THE COUNT OF POWERS OF 2 IN EACH NUM
        // EG: 7 = 111 IN BASE2. SO IT HAS ONE 4, ONE 2 AND ONE 1
        // EG: 5 = 101. IT HAS ONE 4 AND ONE 1
        // WE ARE CAPTURING THE SAME HERE
        if(tempElement >= 4) {
            bitArray[i][2] = 1;
            tempElement -= 4;
            powersCount[2]++;
        }
        if(tempElement >= 2) {
            bitArray[i][1] = 1;
            tempElement -= 2;
            powersCount[1]++;
        }
        if(tempElement >= 1) {
            bitArray[i][0] = 1;
            powersCount[0]++;
        }
    }

    // TO FIND WHICH POWER OF 2 HAS ODD COUNT STARTING FROM 4
    // INDEX IS ALREADY ZERO, SO IF NONE OF 4 NOR 2 IS ODD IT TAKES INDEX 0
    // THE BASE RULE IS THAT ONE OF 4, 2 OR 1 MUST BE ODD ACCORDING TO THE LOGIC
    // SO THIS WORKS
    if(powersCount[2] % 2) {
        index = 2;
    } else if(powersCount[1] % 2) {
        index = 1;
    }

    for(i = MAX_ROWS - 1;i >= 0;i--) {
        if(bitArray[i][index] == 1) {
            targetRow = i;
            bitArray[i][index] = 0;
            powersCount[index]--;
            index--;
            break;
        }
    }

    while(index >= 0) {
        if(powersCount[index] % 2) {
            bitArray[targetRow][index] = !bitArray[targetRow][index];
        }
        index--;
    }

    moves[1] = rows[targetRow];
    rows[targetRow] = bitArray[i][0] * 1 +
                      bitArray[i][1] * 2 +
                      bitArray[i][2] * 4;

    if(maxCount == 1 && rows[targetRow] != MIN) {
        oneRows--;
    } else if(rows[targetRow] == MIN) {
        oneRows++;
    }
    if(rows[targetRow] == ZERO) {
        zeroes++;
    }

    if((zeroes + oneRows) == MAX_ROWS && oneRows % 2 == ZERO && maxCount != MIN) {
        rows[targetRow] == ZERO ? ++rows[targetRow] : --rows[targetRow];
    }

    // SENDING BACK THE TARGETROW AND NUMBER OF OBJECTS REMOVED
    moves[0] = targetRow;
    moves[1] -= rows[targetRow];
}

/*
    Displays the game state with the available number of objects in each row as well as in pictorial form
    Arguments : 'rows' array
    Return : void
*/
void displayGameState(const int *rows, const char gameObj) {
    int i;
    int j;

    printf("\n\n\t\t*******************************************************");
    printf("\n\t\t*\t~@~   N:I:M  G:A:M:E  I:S  O:N:!   ~@~        *\n");
    printf("\t\t*******************************************************\n");
    for(i = 0;i < MAX_ROWS;i++) {
        printf("\n\t\t  Row %d : Available %d --->", i + 1, rows[i]);
        printf("\t");
        for(j = 1;j < MAX_ROWS - i;j++) {
            printf("   ");
        }
        for(j = 0;j < rows[i];j++) {
            printf("%c  ", gameObj);
        }
    }

    printf("\n\n\t\t*******************************************************");
    printf("\n\n");
    printf("\t\t*******************************************************\n");
}

/*
    * METHOD TO GET THE INPUT, VALIDATE IT AND RETURN IT
    * Following do-while takes input from the user
    * If input == -1 then the user has opted to quit the game
    * A confirmation regarding the same is taken and then the game is quit
    * Each time a check is made to see if the input is valid or not
    * If the input is invalid the same repeats till either valid input is given or quit
*/
int getValidInput(const int *rows, const int maxLimit, const char* msg, const int rowFlag) {
    int input;
    short invalidInput = TRUE;

    do {
            printf(msg);
            fflush(stdin);
            scanf("%d", &input);
            if(input == QUIT) {
                printf(QUIT_WARNING_MSG);
                printf(QUIT_CONFIRM_MSG);
                fflush(stdin);
                scanf("%d", &input);
            }
            if(input == QUIT) {
                invalidInput = FALSE;
            } else if(input < MIN || input > maxLimit || (rowFlag == ROW_FLAG && rows[--input] < MIN)) {
                // In the above condition "--input" is done because, if the choice is for rows, then the input is to be reduced to work as the index
                printf(INVALID_INPUT_ERROR_MSG);
            } else {
                invalidInput = FALSE;
            }
    } while(invalidInput == TRUE);

    return input;
}

/*
    * Method to find if the game is over
    * Argument : Takes rows array as an argument
    * Return : Returns 1 if game over, else returns 0
*/
short gameOver(const int *rows) {
    int i;
    for(i = 0;i < MAX_ROWS; i++) {
        if(rows[i] > 0) {
            return FALSE;
        }
    }
    return TRUE;
}

/*
    * Displays the Winner and Congratulates!
    * Argument : Takes a string winner as argument
*/
void declareWinner(const char *winner) {

    int i;
    int padding;
    int len = strlen(winner);
    char leftPadding[6];
    char rightPadding[6];

    padding = (USERNAME_LEN_LIMIT - 1 - len) / 2;
    for(i = 0;i < padding;i++) {
        leftPadding[i] = SPACE;
    }
    leftPadding[i] = '\0';

    padding = (USERNAME_LEN_LIMIT - 1 - len) - padding;
    for(i = 0;i < padding;i++) {
        rightPadding[i] = SPACE;
    }
    rightPadding[i] = '\0';

    system("cls");
    printf("\n\n\t\t\t\t************************************************");
    printf("\n\t\t\t\t\t  *\t\t\t     *");
    printf("\n\t\t\t\t\t  *\t\t\t     *");
    printf("\n\t\t\t\t\t  *\t\t\t     *");
    printf("\n\t\t\t\t\t  *\t\t\t     *");
    printf("\n\t\t\t\t\t  *\t\t\t     *");
    printf("\n\t\t\t\t\t  *\t\t\t     *");
    printf("\n\t\t\t\t************************************************");
    printf("\n\t\t\t\t************************************************");
    printf("\n\t\t\t\t **\t\t\t\t\t     **");
    printf("\n\t\t\t\t **\t\t\t\t\t     **");
    printf("\n\t\t\t\t **\t      %s%s WINS!%s               **", leftPadding, winner, rightPadding);
    printf("\n\t\t\t\t **\t\t\t\t\t     **");
    printf("\n\t\t\t\t **\t %sCONGRATULATIONS %s!%s         **", leftPadding, winner, rightPadding);
    printf("\n\t\t\t\t **\t\t\t\t\t     **");
    printf("\n\t\t\t\t **\t\t\t\t\t     **");
    printf("\n\t\t\t\t************************************************");
    printf("\n\t\t\t\t************************************************\n\n\n\n\n");
}

/*
    * Method to get the preference of the player regarding updating the player profile
    * Returns 1 if the user is willing, else returns 0
*/
short updateProfilePreference() {
    char choice;
    short invalidInput = TRUE;
    printf("\nWould you like to tell us your name and select the game object? ");
    do {
        printf("\n\nPress Y for Yes and N for No : ");
        fflush(stdin);
        scanf("%c", &choice);
        if(choice == 'N' || choice == 'n' || choice == 'Y' || choice == 'y') {
            invalidInput = FALSE;
        } else {
            printf(INVALID_INPUT_ERROR_MSG);
        }
    } while(invalidInput == TRUE);

    if(choice == 'n' || choice == 'N') {
        return FALSE;
    }
    return TRUE;
}

/*
    * Method to obtain the player name
    * Takes a short int player2 as argument
    * The argument is to check if the entry is for 2nd player so that appropriate message can be displayed
    * Returns the name entered after validation
*/
char * getPlayerName(short player2) {
    char *name;
    short validInput = FALSE;
    int i = 0;

    name = (char *) malloc(sizeof(char) * USERNAME_LEN_LIMIT);

    do {
        if(player2) {
        printf("\nOther player name: ");
        } else {
            printf("\nEnter your name: ");
        }
        fflush(stdin);
        scanf("%s", name);
        fflush(stdin);
        validInput = validName(name);
        if(validInput == FALSE) {
            printf(INVALID_NAME);
        }
    } while(validInput == FALSE);

    while(name[i]) {
        name[i] = toupper(name[i]);
        i++;
    }
    return name;
}

/*
    * Method to validate the name entered by the user
    * Takes char * as argument
    * Returns 1 if the entered name is valid, else returns 0
*/
short validName(const char *name) {
    int i;

    for(i = 0;name[i];i++) {
        if(name[i] < 'A' || (name[i] > 'Z' && name[i] < 'a') || name[i] > 'z') {
            return FALSE;
        }
    }
    if(i < 1 || i > USERNAME_LEN_LIMIT - 1) {
        return FALSE;
    }
    return TRUE;
}

/*
    Function to return the game object
*/
char getGameObj() {
    int randomGen = 0;
    int choice;
    char gameObj;
    short invalidInput;
    char *objects = "*oi$#^!|";

    system("cls");
    showAvailableGameObjs();
    do {
        invalidInput = FALSE;
        printf("\nSelect an option from the above: ");
        fflush(stdin);
        scanf("%d", &choice);
        if(choice < 1 || choice > 9) {
            invalidInput = TRUE;
            printf(INVALID_INPUT_ERROR_MSG);
        } else if (choice == 9){
            system("cls");
            printf("\n\t\t<<< PRESS ANY KEY TO GET A RANDOM OBJECT >>>");
            char randomStr[] = "0123456789ABCDEFGHIJ";
            printf("\n\n\n\n\t\t\t    ");
            while(!kbhit()) {
                int i;
                randomGen++;
                for(i = 0;i < 20;i++) {
                    printf("%c", randomStr[i]++);
                    if(randomStr[i] > 254 || randomStr[i] < 40) {
                        randomStr[i] = 40;
                    }
                }
                printf("\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b");
            }
            system("cls");
            gameObj = objects[randomGen % 8];
            printGameObjRandom(gameObj);
            fflush(stdin);
            getchar();
        } else {
            gameObj = objects[choice - 1];
            printf("\n\n<<< YOU HAVE CHOSEN \" %c \" AS YOUR GAME OBJECT >>>\n", gameObj);
        }
    } while(invalidInput == TRUE);
    return gameObj;
}

// Function to print that random game object is generated
void printGameObjRandom(const char gameObj) {
    printf("\n\t\t\t\t\t<<< YOUR GAME OBJECT GENERATED >>>\n");
    printf("\n\t\t\t\t************************************************");
    printf("\n\t\t\t\t************************************************");
    printf("\n\t\t\t\t **\t\t\t\t\t     **");
    printf("\n\t\t\t\t **\t\t\t\t\t     **");
    printf("\n\t\t\t\t **\t        YOUR GAME OBJECT IS          **");
    printf("\n\t\t\t\t **\t\t\t\t\t     **");
    printf("\n\t\t\t\t **\t    ~@<<<(((-> \" %c \" <-)))>>>@~      **", gameObj);
    printf("\n\t\t\t\t **\t\t\t\t\t     **");
    printf("\n\t\t\t\t **\t\t\t\t\t     **");
    printf("\n\t\t\t\t************************************************");
    printf("\n\t\t\t\t************************************************");
}
/*
    * Initializes the rows array with appropriate number of objects in each row
    * Takes rows array as argument
*/
void initRows(int *rows) {
    int i;

    rows[0] = 1;
    for(i = 1;i < MAX_ROWS;i++) {
        rows[i] = rows[i - 1] + 2;
    }
}

/*
    Displays the Home Menu of Nim Game
*/
void printHomeMenu() {
    printf("\n\n\t***********************************");
    printf("\n\t*\t    NIM GAME MENU         *\n");
    printf("\t***********************************\n");

    printf("\t*\t\t\t          *");
    printf("\n\t*    1. Play against Jarvis       *");
    printf("\n\t*    2. Play against a friend     *");
    printf("\n\t*    3. Instructions              *");
    printf("\n\t*    4. Exit                      *");

    printf("\n\t*\t\t\t          *");
    printf("\n\t***********************************\n");
    printf("\t*\t                          *");
    printf("\n\t***********************************\n");
}

/*
    Function to print the available game objects
*/
void showAvailableGameObjs() {
    printf("\n\n\t***********************************");
    printf("\n\t*     AVAILABLE GAME OBJECTS      *\n");
    printf("\t***********************************\n");
    printf("\n\t\t1. *\t\t2. o");
    printf("\n\t\t3. i\t\t4. $");
    printf("\n\t\t5. #\t\t6. ^");
    printf("\n\t\t7. !\t\t8. |");
    printf("\n\t\t9. RANDOM\n");

    printf("\n\t***********************************\n");
    printf("\t*\t                          *");
    printf("\n\t***********************************\n");
    printf("\n<<< DEFAULT OBJECT : o, PRESS 9 TO GENERATE A RANDOM OBJECT >>>\n");
}
