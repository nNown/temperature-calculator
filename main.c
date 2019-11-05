// Projekt 1
// Program przeliczający temperatury

// Program po odpaleniu wyświetla menu, użytkownik wybira co chce zrobić, a po zakończeniu zadania program wraca zawsze do menu (chyba że użytkownik wybierze opcje zamknięcia programu).

// W menu znajdują się przynajmniej następujące elementy:
// * 1-6 funkcje przeliczania temperatur / dodają nowe wiersze
// * wyświelt dane historyczne
// * wypełnij wiersz losowymi wartościami
// * wypełnij tablice (n) wierszy losowymi wartościami
// * zmodyfikuj/podmień dane w wierszu
// * usuń wybrany wiersz
// * usuń wszystkie dane w tabeli
// * wyświetl dane dla podanego zakresu wierszy
// * zakończ program


// Program przechowuje dane o przeszłych przeliczeniach temperatur, które mogą zostać wyświetlone (np. numer wiersza, temperatura w C, temperatura w K, temperatura w F). Dane wprowadzone przez użytkownika są zapisywane w takiej tabeli, a puste wartości są automatycznie uzupełniane. Program posiada zabezpieczenia przez niewłaściwym wpisaniem przez użytkownika wartości (np. gdy program spodziewa się wartości int, a dostaje char albo jeśli użytkownik wprowadzi ujemne Kelviny). Program za każdym razem pokazuje użytkownikowi co dokładnie powienien wprowadzić. Usuwanie wierszy: przed usunięciem pyta o to czy użytkonik na pewno chce to zrobić, a jeśli wiersza nie ma pyta raz jeszcze o numer wiersza (a co jeśli użytkonik będzie wprowadzał same błedne numery wierszy?)

// Nie można podmienić wiersza na taki sam (wartości musą być inne). Wyświetlanie zakresu wierszy działa dla poprawnych zakresów wierszy np. od 4 do 8 (przy czym wpisanie 8 i 4 wyświetli wiersz 8, 7, 6, 5 i 4 w tej kolejności) Podmiana wiersza wyświetla całą tabelę i pyta, który wiersz zmodyfikować.

// Dla chętnych: dodanie zapisu i wyczytwania tabelki z pliku.

#include <stdio.h>
#include <stdbool.h>

float CelToFarh(float C);
float CelToKel(float C);
float FarhToCel(float F);
float FarhToKel(float F);
float KelToCel(float K);
float KelToFarh(float K);
bool isTempValid(int tempUnit, float temp);


int main() {
    while(1) {
        int choosenMenuOption;
        float tempInput;
        printf("Wybierz opcje:\n\r1.Celsjusz na Farenheit\n\r2.Celsjusz na Kelvin\n\r3.Farenheit na Celsjusz\n\r4.Farenheit na Kelvin\n\r5.Kelvin na Celsjusz\n\r6.Kelvin na Farenheit\n\r7.Zakoncz program\n\r");
        scanf("%d", &choosenMenuOption);
        fflush(stdin);
        
        if(choosenMenuOption < 1 || choosenMenuOption > 7) {
            printf("Została wybrana opcja spoza menu\n\r");
            continue;
        }

        if(choosenMenuOption == 7) {
            break;
        }

        printf("Wprowadz Temperature: ");
        scanf("%f", &tempInput);
        printf("\n\r");

        if(!isTempValid(choosenMenuOption, tempInput)) {
            printf("Wprowadzona zostala zla temperatura\n\r");
            continue;
        } else {
            switch(choosenMenuOption) {
                case 1: 
                    printf("%f\n\r", CelToFarh(tempInput));
                    break;
                case 2: 
                    printf("%f\n\r", CelToKel(tempInput));
                    break;
                case 3: 
                    printf("%f\n\r", FarhToCel(tempInput));
                    break;
                case 4: 
                    printf("%f\n\r", FarhToKel(tempInput));
                    break;
                case 5: 
                    printf("%f\n\r", KelToCel(tempInput));
                    break;
                case 6: 
                    printf("%f\n\r", KelToFarh(tempInput));
                    break;
            }
        }
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

bool isTempValid(int tempUnit, float temp) {
    if(((tempUnit == 1 || tempUnit == 2) && temp < -273.15) || ((tempUnit == 3 || tempUnit == 4) && temp < -459.67) || ((tempUnit == 5 || tempUnit == 6) && temp < 0)) {
        return false;
    }
    return true;
}