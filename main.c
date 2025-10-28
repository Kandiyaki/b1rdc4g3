   #include <stdio.h>
   #include <stdlib.h>
   #include <stdbool.h>
   #include <sys/types.h>
   #include <string.h>
   #include <wchar.h>
   #include <locale.h>
   #include <windows.h>
   //#include <unistd.h>

   #define SCREEN_SIZE 4096
   #define START_CUTSCENE 

const int startCutscene[] = {0,0,0,1,2,3,4,5,6,3,4,5,7,8,9,10,11,12,12,12,12,13,13,14,13,14,14,14,14,15,16,17,17};
const int endCutscene[] = {600, 600, 601, 601, 602, 602, 603, 603, 603, 603, 604, 604, 605, 605, 606, 606,
                            607, 607, 607, 607, 608, 608, 608, 608, 609, 609, 609, 609, 610, 610, 610, 610, 611, 611, 612, 612, 612, 612, 
                            613, 613, 614, 615, 616, 616, 616, 617, 617, 617, 618, 619, 620, 621, 622, 622, 
                            623, 624, 625, 626, 627, 628, 629, 630, 631, 632, 633, 
                            634, 634, 634, 635, 635, 635, 635, 635, 635, 636, 636, 637, 638, 638, 638, 639, 
                            640, 641, 642, 643, 644, 645, 
                            646, 646, 646, 646, 647, 647, 647, 647, 648, 648, 648, 648,
                            649, 649, 649, 649, 650, 650, 650, 650, 651, 651, 651, 651, 651, 651, 651, 651, 651, 651, 651, 651,
                            652, 652, 652, 652, 652, 652, 652, 652, 652, 652, 652, 652, // i definitely could have found a better way to do this lmao
                            653, 653, 653, 653, 653, 653, 653, 653, 653, 653, 653, 653, 
                            654, 654, 654, 654, 654, 654, 654, 654, 654, 654, 654, 654,
                            655, 655, 655, 655, 655, 655, 655, 655, 655, 655, 655, 655};

//update screens on: init core transfer, change flight path, init core overload, send emergency evac signal, finalize core transfer, end game 
const int updateFlags[] = {312, 213, 233, 402, 311, 350};
int gameState = 0;
char currentLine[511];
int currentScreen;
int currentTargets[5];
FILE *fptr;
boolean completed = false;

struct screen{
    int code;
    int targets[5];
    char display[SCREEN_SIZE];
};

static const struct screen emptyScreen;

struct screen allScreens[2048];


int main(int argc, char *argv[])
{
    system("cls");
    setlocale(LC_ALL, "en_US.UTF-8");
    //PARSE ALL THE INITIAL SCREENS
    //PUT EM IN A BIG OL ARRAY
    getAllScreens("screens/intro.txt");
    getAllScreens("screens/memory.txt");
    getAllScreens("screens/mainpage.txt");
    getAllScreens("screens/household0.txt");
    getAllScreens("screens/orbital0.txt");
    getAllScreens("screens/planetary0.txt");
    getAllScreens("screens/NARF0.txt");
            getAllScreens("screens/final.txt");

    system("cls");

    //MAIN LOOP:
    //set fptr to point towards the correct screen
    //get the correct amt of characters
    //print
    //wait for input
    //updateScreen();

    //DEBUG: for easily watching the end cutscene
    //doCutscene(250, endCutscene, sizeof(endCutscene)/sizeof(endCutscene[0]));

    doCutscene(500, startCutscene, sizeof(startCutscene)/sizeof(startCutscene[0]));


    currentScreen = 50;
    displayScreen(currentScreen);

    while(!completed){
        printf("\033[2A");//move cursor up 2
        printf("\033[2C");//move cursor left 2
    
        int input = parseInput();
        
        //only accept inputs that are digits 1-5
        if(input > 48 && input <54){
            int numericInput = input - 48;
            printf("\033[H");//move cursor to top left (so no graphics flashing)
            displayScreen(currentTargets[numericInput-1]);
        }else if (input == 'x' || input == 'X'){
            system("cls");
            return 0;
        }else{
            printf("\033[H");//move cursor to top left (so no graphics flashing)
            displayScreen(currentScreen);
        }

        //once you've displayed the screen, check if getting to this point
        //should advance the game state. if so, load the updated screens
        if(updateFlags[gameState] == currentScreen){
            gameState++;
            if (loadStateScreens(gameState) == 1){
                Sleep(5000);
                system("cls");
                return 0;
            }
        }
                
    };
}

int loadStateScreens(int newState){
    //printf("new state!!!");
    switch (newState)
    {
    case 1: //just started core transfer
        /* code */
        //update orbital core - warning about unauthorized data transfer
        getAllScreens("screens/orbital1.txt");
        //update planetary core - 12% transfer
        getAllScreens("screens/planetary1.txt");
        break;
    case 2: //just started flight path change
        /* code */
        //update orbital core - new flight path
        getAllScreens("screens/orbital2.txt");
        //update planetary core - 32% transfer
        getAllScreens("screens/planetary2.txt");
        //update NARF hub - a computer is beeping, nobody notices
        getAllScreens("screens/NARF2.txt");

        break;
    case 3: //just started reactor overload
        /* code */
        //update orbital core - reactor warnings
        getAllScreens("screens/orbital3.txt");
        //update planetary core - 67% transfer
        getAllScreens("screens/planetary3.txt");
        //update NARF - allow emergency evac signal
        getAllScreens("screens/NARF3.txt");
        break;
    case 4: //just started emergency evac
        /* code */
        //update NARF hub - people scrambling
        //update orbital core - losing connection
        //update planetary core - 98% transfer, ready to finalize
        //update household - ohhhhh 9/11
        getAllScreens("screens/NARF4.txt");
        getAllScreens("screens/orbital4.txt");
        getAllScreens("screens/planetary4.txt");
        getAllScreens("screens/household4.txt");

        break;
    case 5: //just finalized transfer
        /* code */
        //update NARF hub - no connection
        //update orbital core - no connection
        //update household - explosion on da TV
        //update planetary core - allow body creation for final cutscene
        getAllScreens("screens/NARF5.txt");
        getAllScreens("screens/orbital5.txt");
        getAllScreens("screens/planetary5.txt");
        getAllScreens("screens/household5.txt");

        break;
    case 6: //final cutscene
        doCutscene(500, endCutscene, sizeof(endCutscene)/sizeof(endCutscene[0]));
        return 1;
        break;
    default: //something else?
        break;
    }

    return 0;
}

//frames will last n milliseconds
int doCutscene(int frameTime, int* frameList, int numFrames){
    //hide cursor
    printf("\33[?25l");
    for(int i=0; i<numFrames; i++){
        printf("\033[H");//move cursor to top left (so no graphics flashing)
        Sleep(frameTime);
        displayScreen(frameList[i]);
    }

    //printf("\033[2D");//move cursor left 2

    printf("\033[H");//move cursor to top left (so no graphics flashing)
    //show cursor
    //printf("\33[?25h");
    return 0;
}

int parseInput(){
    int input = getchar();
    //printf("key: %c (code %d)\n", input, input);
    return input;
}

int displayScreen(int ID){
    //printf("called displayScreen with ID %d\n", ID);
    
    if(ID>=0){ //if we got an ID for a real page, change current page to that
        currentScreen = ID;
        //set targets in a loop here
         for(int i=0; i<5; i++){
            currentTargets[i] = allScreens[currentScreen].targets[i];
        }
    }

    struct screen toDisplay = allScreens[currentScreen];

    //display current page
    wchar_t *line = L"";
    printf("%ls%s", line, toDisplay.display);
    //DEBUG
    //printf("current screen: %d\n", currentScreen);
    //printf("targets: %d %d %d %d %d\n", currentTargets[0], currentTargets[1],currentTargets[2],currentTargets[3],currentTargets[4]);


    return 0;
}

void getTargets(char* line, int* output){ //christ

    int linePos = 0; //
    for(int targetNum=0; targetNum<5; targetNum++){
        char current[8];
        int targetPos=0;
        while(line[linePos] != ',' && line[linePos] != '\0'){ //why did i choose to do this in C
            current[targetPos] = line[linePos];
            linePos++;
            targetPos++;
        }
        linePos++; //skip comma
        output[targetNum] = atoi(current);
    }
}

int getAllScreens(char *file){
    fptr = fopen(file, "r");
    struct screen currentlyParsing;
    int lineNum = 1;

    while(fgets(currentLine, 511, fptr)){
        if(lineNum % 30 == 1){ //screen's ID
            currentlyParsing.code = atoi(currentLine+3);
            //printf("got code %d\n", currentlyParsing.code);
        }else if (lineNum % 30 == 2){ //screen's links
            int start = 0;
            int output[5];
            getTargets(currentLine, output);
            for(int i=0; i<5; i++){
                currentlyParsing.targets[i] = output[i];
            }
            memset(currentlyParsing.display, 0, strlen(currentlyParsing.display));

        }else if(lineNum % 30 < 28){
            wchar_t *line = L"";
            char lineBuf[512];
            snprintf(lineBuf, 511, "%ls%s", line, currentLine);
            strcat(currentlyParsing.display, lineBuf);
        }
        if(lineNum % 30 == 28){
            allScreens[currentlyParsing.code] = emptyScreen;
            allScreens[currentlyParsing.code] = currentlyParsing;
            char newDisplay[SCREEN_SIZE];
            strcpy(currentlyParsing.display, newDisplay);
        }
        lineNum++;
    }
    return 0;
}