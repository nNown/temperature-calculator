#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <windows.h>
#include <conio.h>

#define TEMPLATE_HEIGHT 6
#define TEMPLATE_WIDTH 32

float CelToFarh(float C);
float CelToKel(float C);
float FarhToCel(float F);
float FarhToKel(float F);
float KelToCel(float K);
float KelToFarh(float K);

char menuTemplate[TEMPLATE_HEIGHT][TEMPLATE_WIDTH] = {
    "------------------------------\n\r",
    "|                            |\n\r",
    "|                            |\n\r",
    "|                            |\n\r",
    "------------------------------\n\r",
    "        C( ) F( ) K( )        \n\r",
};

char menuUi[TEMPLATE_HEIGHT][TEMPLATE_WIDTH];
char *displayPointer[TEMPLATE_WIDTH - 4];
char *tempUnitPointer[3];

void renderTemplate(char template[TEMPLATE_HEIGHT][TEMPLATE_WIDTH], int row, int col, int colour);
void initializeUi(char template[TEMPLATE_HEIGHT][TEMPLATE_WIDTH], char ui[TEMPLATE_HEIGHT][TEMPLATE_WIDTH]);
void hideCursor();

enum ASCIIChars {BACKSPACE = 8, ESCAPE = 27, SPACE = 32, ASTERISK = 42, MINUS_SIGN = 45, DOT = 46, ZERO = 48, NINE = 57, ARROW_UP = 72, ARROW_LEFT = 75, ARROW_RIGHT = 77, ARROW_DOWN = 80, RESTART = 114};
enum TempUnits {CEL, FARH, KEL};

typedef struct consoleInformation {
    char sign;
    int currentRow;
    int currentCol;
    int currentColour; //Hex value
    enum TempUnits currentTempUnit;
    float currentTempValue;
} consoleInformation;


void addNumberToDisplay(char *pointer[], char number);
void deleteNumberFromDisplay(char *pointer[]);

bool setFloatingPoint(char *pointer[]);
void setSign(char *pointer[], char sign);

float getNumberFromDisplay(char *pointer[]);
void printNumberInDisplay(char *pointer[], float number);

void updateTemperaturUnitInDisplay(char *pointer[], int currentTempUnit);
void convertTemperatureToAnotherUnit(char *pointer[], char *displayPointer[], enum TempUnits *currentTempUnit, int nextTempUnit, float *currentTempValue);

int main() {
    consoleInformation consoleInfo = {32, 5, 10, 0x0070, CEL, 0};
    enum ASCIIChars currentPressedButton;
    int i;

    initializeUi(menuTemplate, menuUi);

    for(i = 0; i < TEMPLATE_WIDTH - 4; i++) {
        displayPointer[i] = &menuUi[2][i + 1];
    }
    tempUnitPointer[0] = &menuUi[5][10];
    tempUnitPointer[1] = &menuUi[5][15];
    tempUnitPointer[2] = &menuUi[5][20];

    hideCursor();
    renderTemplate(menuUi, consoleInfo.currentRow, consoleInfo.currentCol, consoleInfo.currentColour);
    
    while(currentPressedButton != ESCAPE) {
        currentPressedButton = _getch();

        if(currentPressedButton >= ZERO && currentPressedButton <= NINE) {
            for(i = ZERO; i <= NINE; i++) {
                if(currentPressedButton == i) {
                    addNumberToDisplay(displayPointer, currentPressedButton);
                    break;
                }
            }
        }

        switch(currentPressedButton) {
            case RESTART:
                initializeUi(menuTemplate, menuUi);
                consoleInfo.sign = SPACE;
                break;
            case BACKSPACE:
                deleteNumberFromDisplay(displayPointer);
                break;
            case MINUS_SIGN:
                if(consoleInfo.sign == SPACE) consoleInfo.sign = MINUS_SIGN;
                else if(consoleInfo.sign == MINUS_SIGN) consoleInfo.sign = SPACE;
                break;
            case DOT:
                if(setFloatingPoint(displayPointer)) {
                    if(*displayPointer[TEMPLATE_WIDTH - 5] == 32) {
                        *displayPointer[TEMPLATE_WIDTH - 6] = ZERO;
                        addNumberToDisplay(displayPointer, DOT);
                        break;
                    } else {
                        addNumberToDisplay(displayPointer, DOT);
                        break;
                    }
                }
            case ARROW_LEFT:
                if(consoleInfo.currentCol == 10) {
                    consoleInfo.currentCol = 20;
                }
                else if(consoleInfo.currentCol == 15) {
                    consoleInfo.currentCol = 10;
                }
                else if(consoleInfo.currentCol == 20) {
                    consoleInfo.currentCol = 15;
                }
                break;
            case ARROW_RIGHT:
                if(consoleInfo.currentCol == 10) {
                    consoleInfo.currentCol = 15;
                }
                else if(consoleInfo.currentCol == 15) {
                    consoleInfo.currentCol = 20;
                }
                else if(consoleInfo.currentCol == 20) {
                    consoleInfo.currentCol = 10;
                }
                break;
            case SPACE:
                if(consoleInfo.currentRow == 5) {
                    convertTemperatureToAnotherUnit(tempUnitPointer, displayPointer, &consoleInfo.currentTempUnit, consoleInfo.currentCol, &consoleInfo.currentTempValue);
                    if(*displayPointer[TEMPLATE_WIDTH - 5] != 32) {
                        if(consoleInfo.currentTempValue < 0) {
                            consoleInfo.sign = 45;
                        } else {
                            consoleInfo.sign = 32;
                        }
                        printNumberInDisplay(displayPointer, consoleInfo.currentTempValue);
                    } 
                }
        }

        setSign(displayPointer, consoleInfo.sign);

        consoleInfo.currentTempValue = getNumberFromDisplay(displayPointer);

        system("cls");
        
        updateTemperaturUnitInDisplay(tempUnitPointer, consoleInfo.currentTempUnit);
        renderTemplate(menuUi, consoleInfo.currentRow, consoleInfo.currentCol, consoleInfo.currentColour);
    }

    return 0;
}

float CelToFarh(float C) {
    return C * 9.0/5.0 + 32.0;
}

float CelToKel(float C) {
    return C + 273.15;
}

float FarhToCel(float F) {
    return (F - 32.0) * 5.0/9.0;
}

float FarhToKel(float F) {
    return (F + 459.67) * 5.0/9.0;
}

float KelToCel(float K) {
    return K - 273.15;
}

float KelToFarh(float K) {
    return K * 9.0/5.0 - 459.67;
}

void renderTemplate(char template[TEMPLATE_HEIGHT][TEMPLATE_WIDTH], int row, int col, int colour) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO consoleInfo;
    WORD saved_attributes;

    GetConsoleScreenBufferInfo(hConsole, &consoleInfo);
    saved_attributes = consoleInfo.wAttributes;

    int i, j;
    for(i = 0; i < TEMPLATE_HEIGHT; i++) {
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
    if(*pointer[1] != 32) return;

    int i;
    if(*pointer[TEMPLATE_WIDTH - 5] == 32) {
        *pointer[TEMPLATE_WIDTH - 5] = number;
    } else {
        for(i = 1; i < TEMPLATE_WIDTH - 4; i++) {
            if(*pointer[i] != 32) {
                *pointer[i - 1] = *pointer[i];
            }
        }
        *pointer[TEMPLATE_WIDTH - 5] = number;
    }
}

void deleteNumberFromDisplay(char *pointer[]) {
    if(*pointer[TEMPLATE_WIDTH - 5] == 32) return;

    int i;
    for(i = TEMPLATE_WIDTH - 6; i > 0; i--) {
        *pointer[i + 1] = *pointer[i];
    }
    *pointer[1] = 32;
}

bool setFloatingPoint(char *pointer[]) {
    int i;
    for(i = 1; i < TEMPLATE_WIDTH - 4; i++) {
        if(*pointer[i] == DOT) return false;
    }
    return true;
}

void setSign(char *pointer[], char sign) {
    *pointer[0] = sign;
}

float getNumberFromDisplay(char *pointer[]) {
    char number[TEMPLATE_WIDTH - 4] = {0};
    
    int i, j;
    for(i = 0; i < TEMPLATE_WIDTH - 4; i++) {
        if(*pointer[i] != 32) {
            for(j = 0; j < TEMPLATE_WIDTH - 4; j++) {
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
    char numberAsAString[TEMPLATE_WIDTH - 4];
    int i, j;

    for(i = 0; i < TEMPLATE_WIDTH - 4; i++) {
        numberAsAString[i] = 32;
        *pointer[i] = 32;
    }

    sprintf(numberAsAString, "%f", number);

    for(i = TEMPLATE_WIDTH - 5; i > 0; i--) {
        if(numberAsAString[i] >= ZERO && numberAsAString[i] <= NINE) {
            break;
        }
    }

    for(i, j = TEMPLATE_WIDTH - 5; i > 0, j > 0; i--, j--) {
        if(numberAsAString[i] == MINUS_SIGN) {
            break;
        }
        
        if((numberAsAString[i] >= ZERO && numberAsAString[i] <= NINE) || numberAsAString[i] == DOT) {
           *pointer[j] = numberAsAString[i]; 
        } else {
            *pointer[j] = 32;
        }
    }
}

void updateTemperaturUnitInDisplay(char *pointer[], int currentTempUnit) {
    *pointer[currentTempUnit] = ASTERISK;
}

void convertTemperatureToAnotherUnit(char *pointer[], char *displayPointer[], enum TempUnits *currentTempUnit, int nextTempUnit, float *currentTempValue) {
    if(*displayPointer[TEMPLATE_WIDTH - 5] != 32) {
        switch(*currentTempUnit) {
            case CEL:
                if(nextTempUnit == 15) *currentTempValue = CelToFarh(*currentTempValue);
                else if(nextTempUnit == 20) *currentTempValue = CelToKel(*currentTempValue);
                break;
            case FARH:
                if(nextTempUnit == 10) *currentTempValue = FarhToCel(*currentTempValue);
                else if(nextTempUnit == 20) *currentTempValue = FarhToKel(*currentTempValue);
                break;
            case KEL:
                if(nextTempUnit == 10) *currentTempValue = KelToCel(*currentTempValue);
                else if(nextTempUnit == 15) *currentTempValue = KelToFarh(*currentTempValue);
                break;
        }
    }

    *pointer[*currentTempUnit] = 32;
    if(nextTempUnit == 10) *currentTempUnit = CEL;
    else if(nextTempUnit == 15) *currentTempUnit = FARH;
    else if(nextTempUnit == 20) *currentTempUnit = KEL;

    if(*currentTempUnit == CEL && *currentTempValue < -273.15) *currentTempValue = -273.15;
    else if(*currentTempUnit == FARH && *currentTempValue < -459.67) *currentTempValue = -459.67;
    else if(*currentTempUnit == KEL && *currentTempValue < 0) *currentTempValue = 0;
}