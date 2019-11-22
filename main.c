#include "display.h"

int main() {
    consoleInformation consoleInfo = {PLUS, 5, CEL_BTN, 0x0070, CEL, 0};
    enum ASCIIChars currentPressedButton;
    int i, j;

    initializeUi(menuTemplate, menuUi);
    initializeAppendableRow(historyUiTemplate, historyRowToAppend);

    for(i = 0; i < TEMPLATE_WIDTH - 4; i++) {
        displayPointer[i] = &menuUi[2][i + 1];
    }

    tempUnitPointer[0] = &menuUi[5][10];
    tempUnitPointer[1] = &menuUi[5][15];
    tempUnitPointer[2] = &menuUi[5][20];

    for(i = 0; i < TEMPLATE_WIDTH - 4; i++) {
        inputTempPointer[i] = &historyRowToAppend[0][i];
    }

    inputTempUnit = &historyRowToAppend[0][29];

    for(i = 0, j = 33; i < TEMPLATE_WIDTH - 4, j < TEMPLATE_WIDTH - 4 + 33; i++, j++) {
        outputTempPointer[i] = &historyRowToAppend[0][j];
    }
    
    outputTempUnit = &historyRowToAppend[0][62];

    hideCursor();
    renderTemplate(menuUi, tempHistory, consoleInfo.currentRow, consoleInfo.Buttons, consoleInfo.currentColour, appendedElements);

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
                consoleInfo.displaySign = SPACE;
                break;
            case BACKSPACE:
                deleteNumberFromDisplay(displayPointer);
                break;
            case MINUS_SIGN:
                if(consoleInfo.displaySign == SPACE) consoleInfo.displaySign = MINUS_SIGN;
                else if(consoleInfo.displaySign == MINUS_SIGN) consoleInfo.displaySign = SPACE;
                break;
            case DOT:
                if(setFloatingPoint(displayPointer)) {
                    if(*displayPointer[TEMPLATE_WIDTH - 6] == SPACE) {
                        *displayPointer[TEMPLATE_WIDTH - 7] = ZERO;
                        addNumberToDisplay(displayPointer, DOT);
                        break;
                    } else {
                        addNumberToDisplay(displayPointer, DOT);
                        break;
                    }
                }
            case TAB:
                srand(time(NULL));
                float randomNumber = rand() % 1000;
                printNumberInDisplay(displayPointer, randomNumber);
                break;
            case DEL:
                for(i = 0; i < appendedElements; i++) {
                    for(j = 0; j < HISTORY_WIDTH; j++) {
                        tempHistory[i][j] = SPACE;
                    }
                }
                appendedElements = 0;
                consoleInfo.currentRow = 5;
                consoleInfo.Buttons = CEL_BTN;
                break;
            case ARROW_UP:
                if(appendedElements == 0) break;
                
                if(consoleInfo.currentRow == 5) {
                    consoleInfo.currentRow = appendedElements + 5;
                    consoleInfo.Buttons = EDIT_BTN;
                } else if(consoleInfo.currentRow == 6) {
                    consoleInfo.currentRow = 5;
                    consoleInfo.Buttons = CEL_BTN;
                } else {
                    consoleInfo.currentRow--;
                }
                break;
            case ARROW_DOWN:
                if(appendedElements == 0) break;
                
                if(consoleInfo.currentRow == appendedElements + 5) {
                    consoleInfo.currentRow = 5;
                    consoleInfo.Buttons = CEL_BTN;
                } else if(consoleInfo.currentRow == 5) {
                    consoleInfo.currentRow = 6;
                    consoleInfo.Buttons = EDIT_BTN;
                } else {
                    consoleInfo.currentRow++;
                }
                break;
            case ARROW_LEFT:
                if(consoleInfo.currentRow == 5) {
                    if(consoleInfo.Buttons == CEL_BTN) consoleInfo.Buttons = KEL_BTN;
                    else consoleInfo.Buttons -= 5;
                } else {
                    if(consoleInfo.Buttons == DELETE_BTN) consoleInfo.Buttons = EDIT_BTN;
                    else if(consoleInfo.Buttons == EDIT_BTN) consoleInfo.Buttons = EDIT_INPUT;
                    else consoleInfo.Buttons = DELETE_BTN;
                }
                break;
            case ARROW_RIGHT:
                if(consoleInfo.currentRow == 5) {
                    if(consoleInfo.Buttons == KEL_BTN) consoleInfo.Buttons = CEL_BTN;
                    else consoleInfo.Buttons += 5;
                } else {
                    if(consoleInfo.Buttons == EDIT_INPUT) consoleInfo.Buttons = EDIT_BTN;
                    else if(consoleInfo.Buttons == EDIT_BTN) consoleInfo.Buttons = DELETE_BTN;
                    else consoleInfo.Buttons = EDIT_BTN;
                }
                break;
            case SPACE:
                if(consoleInfo.currentRow == 5) {
                    initializeAppendableRow(historyUiTemplate, historyRowToAppend);

                    if(consoleInfo.currentTempUnit == CEL && consoleInfo.currentTempValue < -273.15) consoleInfo.currentTempValue = -273.15;
                    else if(consoleInfo.currentTempUnit == FARH && consoleInfo.currentTempValue < -459.67) consoleInfo.currentTempValue = -459.67;
                    else if(consoleInfo.currentTempUnit == KEL && consoleInfo.currentTempValue < 0) consoleInfo.currentTempValue = 0;

                    if(consoleInfo.currentTempValue < 0) inputSign = MINUS_SIGN;
                    else inputSign = SPACE;
                    printNumberInDisplay(inputTempPointer, consoleInfo.currentTempValue);
                    setSign(inputTempPointer, inputSign);

                    convertTemperatureToAnotherUnit(tempUnitPointer, displayPointer, &consoleInfo.currentTempUnit, consoleInfo.Buttons, &consoleInfo.currentTempValue, inputTempUnit, outputTempUnit);
                    
                    if(*displayPointer[TEMPLATE_WIDTH - 6] != SPACE) {
                        if(consoleInfo.currentTempValue < 0) {
                            consoleInfo.displaySign = MINUS_SIGN;
                            outputSign = MINUS_SIGN;
                        } else {
                            consoleInfo.displaySign = SPACE;
                            outputSign = SPACE;
                        }

                        printNumberInDisplay(displayPointer, consoleInfo.currentTempValue);

                        printNumberInDisplay(outputTempPointer, consoleInfo.currentTempValue);
                        setSign(outputTempPointer, outputSign);

                        if(editedRow != -1) {
                            for(i = editedRow, j = 0; j < HISTORY_WIDTH; j++) tempHistory[i][j] = historyRowToAppend[0][j];
                            editedRow = -1;
                        } else {
                            for(i = appendedElements, j = 0; j < HISTORY_WIDTH; j++) tempHistory[i][j] = historyRowToAppend[0][j];
                            appendedElements++;
                        }
                    }
                } else {
                    if(consoleInfo.Buttons == EDIT_BTN) {
                        editedRow = consoleInfo.currentRow - 6;
                        consoleInfo.Buttons = CEL_BTN;
                        consoleInfo.currentRow = 5;
                    }
                    else if(consoleInfo.Buttons == DELETE_BTN) {
                        editedRow = consoleInfo.currentRow - 5;
                        deleteElementFromHistory(tempHistory, editedRow);
                        editedRow = -1;
                        appendedElements--;

                        if(appendedElements == 0) {
                            consoleInfo.currentRow = 5;
                            consoleInfo.Buttons = CEL_BTN;
                        }
                    };
                }
                break;
        }

        
        system("cls");
        setSign(displayPointer, consoleInfo.displaySign);
        consoleInfo.currentTempValue = getNumberFromDisplay(displayPointer);
        
        updateTemperaturUnitInDisplay(tempUnitPointer, consoleInfo.currentTempUnit);
        renderTemplate(menuUi, tempHistory, consoleInfo.currentRow, consoleInfo.Buttons, consoleInfo.currentColour, appendedElements);
    }

    return 0;
}