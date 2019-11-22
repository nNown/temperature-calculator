#include "display.h"

char menuTemplate[TEMPLATE_HEIGHT][TEMPLATE_WIDTH] = {
    "------------------------------\n\r",
    "|                            |\n\r",
    "|                            |\n\r",
    "|                            |\n\r",
    "------------------------------\n\r",
    "        C( ) F( ) K( )        \n\r",
};

char menuUi[TEMPLATE_HEIGHT][TEMPLATE_WIDTH];
char *displayPointer[TEMPLATE_WIDTH - 5];
char *tempUnitPointer[3];

char historyUiTemplate[HISTORY_HEIGHT][HISTORY_WIDTH] = {
    "                               |                                | E / D |\n\r"
};

char historyRowToAppend[HISTORY_HEIGHT][HISTORY_WIDTH];
char tempHistory[HISTORY_ELEMENTS][HISTORY_WIDTH];

int appendedElements = 0;
int editedRow = -1;

char *inputTempPointer[TEMPLATE_WIDTH - 5];
char *inputTempUnit;
char inputSign;
char *outputTempPointer[TEMPLATE_WIDTH - 5];
char *outputTempUnit;
char outputSign;

void renderTemplate(char template[TEMPLATE_HEIGHT][TEMPLATE_WIDTH], char history[HISTORY_ELEMENTS][HISTORY_WIDTH], int row, int col, int colour, int appendedElements) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO consoleInfo;
    WORD saved_attributes;

    GetConsoleScreenBufferInfo(hConsole, &consoleInfo);
    saved_attributes = consoleInfo.wAttributes;

    int i, j;
    for(i = 0; i < TEMPLATE_HEIGHT; i++) {
        printf("\t\t\t");
        for(j = 0; j < TEMPLATE_WIDTH; j++) {
            if(i == row && j == col) {
                SetConsoleTextAttribute(hConsole, colour);
                putchar(template[i][j]);
                SetConsoleTextAttribute(hConsole, saved_attributes);
                continue;
            }
            putchar(template[i][j]);
        }
    }

    for(i = 0; i < appendedElements; i++) {
        for(j = 0; j < HISTORY_WIDTH; j++) {
            if(i == row - 6 && j == col) {
                SetConsoleTextAttribute(hConsole, colour);
                putchar(history[i][j]);
                SetConsoleTextAttribute(hConsole, saved_attributes);
                continue;
            }
            putchar(history[i][j]);
        }
    }
}

void initializeUi(char template[TEMPLATE_HEIGHT][TEMPLATE_WIDTH], char ui[TEMPLATE_HEIGHT][TEMPLATE_WIDTH]) {
    int i, j;
    for(i = 0; i < TEMPLATE_HEIGHT; i++) {
        for(j = 0; j < TEMPLATE_WIDTH; j++) {
            ui[i][j] = template[i][j];
        }
    }
}

void hideCursor() {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO consoleInfo;
    consoleInfo.dwSize = 100;
    consoleInfo.bVisible = FALSE;
    SetConsoleCursorInfo(hConsole, &consoleInfo);
}

void addNumberToDisplay(char *pointer[], char number) {
    if(*pointer[1] != SPACE) return;

    int i;
    if(checkIfDisplayIsEmpty(pointer)) {
        *pointer[TEMPLATE_WIDTH - 6] = number;
    } else {
        for(i = 1; i < TEMPLATE_WIDTH - 5; i++) {
            if(*pointer[i] != SPACE) {
                *pointer[i - 1] = *pointer[i];
            }
        }
        *pointer[TEMPLATE_WIDTH - 6] = number;
    }
}

void deleteNumberFromDisplay(char *pointer[]) {
    if(*pointer[TEMPLATE_WIDTH - 6] == SPACE) return;

    int i;
    for(i = TEMPLATE_WIDTH - 7; i > 0; i--) {
        *pointer[i + 1] = *pointer[i];
    }
    *pointer[1] = SPACE;
}

bool setFloatingPoint(char *pointer[]) {
    int i;
    for(i = 1; i < TEMPLATE_WIDTH - 5; i++) {
        if(*pointer[i] == DOT) return false;
    }
    return true;
}

bool checkIfDisplayIsEmpty(char *pointer[]) {
    return *pointer[TEMPLATE_WIDTH - 6] == SPACE;
}

void setSign(char *pointer[], char displaySign) {
    *pointer[0] = displaySign;
}

float getNumberFromDisplay(char *pointer[]) {
    char number[TEMPLATE_WIDTH - 5] = {0};
    
    int i, j;

    for(i = 1; i < TEMPLATE_WIDTH - 6; i++) {
        if(*pointer[i] != SPACE && *pointer[i + 1] == SPACE) *pointer[i] = SPACE;
    }

    for(i = 0; i < TEMPLATE_WIDTH - 5; i++) {
        if(*pointer[i] != SPACE) {
            for(j = 0; j < TEMPLATE_WIDTH - 5; j++) {
                if(number[j] == 0) {
                    number[j] = *pointer[i];
                    break;
                }
            }
        }
    }

    return strtof(number, NULL);
}

void printNumberInDisplay(char *pointer[], float number) {
    char numberAsAString[TEMPLATE_WIDTH - 5];
    int i, j;

    for(i = 0; i < TEMPLATE_WIDTH - 5; i++) {
        numberAsAString[i] = SPACE;
        *pointer[i] = SPACE;
    }

    sprintf(numberAsAString, "%f", number);

    for(i = TEMPLATE_WIDTH - 6; i > 0; i--) {
        if(numberAsAString[i] >= ZERO && numberAsAString[i] <= NINE) {
            break;
        }
    }

    for(i, j = TEMPLATE_WIDTH - 6; i > 0, j > 0; i--, j--) {
        if(numberAsAString[i] == MINUS_SIGN) {
            break;
        }
        
        if((numberAsAString[i] >= ZERO && numberAsAString[i] <= NINE) || numberAsAString[i] == DOT) {
           *pointer[j] = numberAsAString[i]; 
        } else {
            *pointer[j] = SPACE;
        }
    }

    for(i = 1; i < TEMPLATE_WIDTH - 6; i++) {
        if(*pointer[i] != SPACE && *pointer[i + 1] == SPACE) *pointer[i] = SPACE;
    }
}

void updateTemperaturUnitInDisplay(char *pointer[], int currentTempUnit) {
    *pointer[currentTempUnit] = ASTERISK;
}

void convertTemperatureToAnotherUnit(char *pointer[], char *displayPointer[], enum TempUnits *currentTempUnit, enum Buttons nextTempUnit, float *currentTempValue, char *historyInputUnit, char *historyOutputUnit) {
    if(!checkIfDisplayIsEmpty(displayPointer)) {
        switch(*currentTempUnit) {
            case CEL:
                if(nextTempUnit == FARH_BTN) *currentTempValue = CelToFarh(*currentTempValue);
                else if(nextTempUnit == KEL_BTN) *currentTempValue = CelToKel(*currentTempValue);
                *historyInputUnit = 'C';
                break;
            case FARH:
                if(nextTempUnit == CEL_BTN) *currentTempValue = FarhToCel(*currentTempValue);
                else if(nextTempUnit == KEL_BTN) *currentTempValue = FarhToKel(*currentTempValue);
                *historyInputUnit = 'F';
                break;
            case KEL:
                if(nextTempUnit == CEL_BTN) *currentTempValue = KelToCel(*currentTempValue);
                else if(nextTempUnit == FARH_BTN) *currentTempValue = KelToFarh(*currentTempValue);
                *historyInputUnit = 'K';
                break;
        }
    }

    *pointer[*currentTempUnit] = SPACE;
    if(nextTempUnit == CEL_BTN) {
        *currentTempUnit = CEL;
        *historyOutputUnit = 'C';
    }
    else if(nextTempUnit == FARH_BTN) {
        *currentTempUnit = FARH;
        *historyOutputUnit = 'F';
    }
    else if(nextTempUnit == KEL_BTN) {
        *currentTempUnit = KEL;
        *historyOutputUnit = 'K';
    }

    if(*currentTempUnit == CEL && *currentTempValue < -273.15) *currentTempValue = -273.15;
    else if(*currentTempUnit == FARH && *currentTempValue < -459.67) *currentTempValue = -459.67;
    else if(*currentTempUnit == KEL && *currentTempValue < 0) *currentTempValue = 0;
}

void initializeAppendableRow(char template[HISTORY_HEIGHT][HISTORY_WIDTH], char row[HISTORY_HEIGHT][HISTORY_WIDTH]) {
    int i, j;
    for(i = 0; i < HISTORY_HEIGHT; i++) {
        for(j = 0; j < HISTORY_WIDTH; j++) {
            row[i][j] = template[i][j];
        }
    }
}

void deleteElementFromHistory(char history[HISTORY_ELEMENTS][HISTORY_WIDTH], int row) {
    int i, j;
    for(i = row - 1; i < appendedElements - 1; i++) {
        for(j = 0; j < HISTORY_WIDTH; j++) {
            history[i][j] = history[i + 1][j];
        }
    }
}