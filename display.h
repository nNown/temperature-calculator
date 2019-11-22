#ifndef DISPLAY_H
#define DISPLAY_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <windows.h>
#include <conio.h>
#include <time.h>
#include "tempFunctions.h"

#define TEMPLATE_HEIGHT 6
#define TEMPLATE_WIDTH 33
#define HISTORY_HEIGHT 1
#define HISTORY_WIDTH 76
#define HISTORY_ELEMENTS 1000

extern char menuTemplate[TEMPLATE_HEIGHT][TEMPLATE_WIDTH];
extern char menuUi[TEMPLATE_HEIGHT][TEMPLATE_WIDTH];
extern char *displayPointer[TEMPLATE_WIDTH - 5];
extern char *tempUnitPointer[3];

extern char historyUiTemplate[HISTORY_HEIGHT][HISTORY_WIDTH];
extern char historyRowToAppend[HISTORY_HEIGHT][HISTORY_WIDTH];
extern char tempHistory[HISTORY_ELEMENTS][HISTORY_WIDTH];

extern int appendedElements;
extern int editedRow;

extern enum ASCIIChars {BACKSPACE = 8, TAB = 9, ESCAPE = 27, SPACE = 32, ASTERISK = 42, MINUS_SIGN = 45, DOT = 46, ZERO = 48, NINE = 57, ARROW_UP = 72, ARROW_LEFT = 75, ARROW_RIGHT = 77, ARROW_DOWN = 80, DEL = 83, RESTART = 114};
extern enum TempUnits {CEL, FARH, KEL};
extern enum Buttons {CEL_BTN = 10, FARH_BTN = 15, KEL_BTN = 20, EDIT_INPUT = 27, EDIT_BTN = 66, DELETE_BTN = 70};
extern enum DisplayedSign {MINUS = 45, PLUS = 32};

typedef struct {
    enum DisplayedSign displaySign;
    int currentRow;
    enum Buttons Buttons; // Current Column
    int currentColour; //Hex value
    enum TempUnits currentTempUnit;
    float currentTempValue;
} consoleInformation;

extern char *inputTempPointer[TEMPLATE_WIDTH - 5];
extern char *inputTempUnit;
extern char inputSign;
extern char *outputTempPointer[TEMPLATE_WIDTH - 5];
extern char *outputTempUnit;
extern char outputSign;

void renderTemplate(char menuDisplayTemplate[TEMPLATE_HEIGHT][TEMPLATE_WIDTH], char history[HISTORY_ELEMENTS][HISTORY_WIDTH], int row, int col, int colour, int appendedElements);
void initializeUi(char menuDisplayTemplate[TEMPLATE_HEIGHT][TEMPLATE_WIDTH], char ui[TEMPLATE_HEIGHT][TEMPLATE_WIDTH]);

void hideCursor();

void addNumberToDisplay(char *pointer[], char number);
void deleteNumberFromDisplay(char *pointer[]);

bool setFloatingPoint(char *pointer[]);
bool checkIfDisplayIsEmpty(char *pointer[]);
void setSign(char *pointer[], char displaySign);

float getNumberFromDisplay(char *pointer[]);
void printNumberInDisplay(char *pointer[], float number);

void updateTemperaturUnitInDisplay(char *pointer[], int currentTempUnit);
void convertTemperatureToAnotherUnit(char *pointer[], char *displayPointer[], enum TempUnits *currentTempUnit, enum Buttons nextTempUnit, float *currentTempValue, char *historyInputUnit, char *historyOutputUnit); // TODO Reduce params here

void initializeAppendableRow(char historyRowTemplate[HISTORY_HEIGHT][HISTORY_WIDTH], char row[HISTORY_HEIGHT][HISTORY_WIDTH]);
void deleteElementFromHistory(char history[HISTORY_ELEMENTS][HISTORY_WIDTH], int row);

#endif // DISPLAY_H