# Projekt I na podstawy programowania
### Wojciech Marciniak, Bioinformatyka, I rok

Program kompiluje się tylko z użyciem kompilatora MinGW:
> gcc -o main main.c display.c tempFunctions.c

Brakuje kompatybilności z Unixiem, program wykorzystuje bibliotekę windows.h do interakcji z konsolą.

Sam program składa się z dwóch części:
1. Wyświetlającej miejsce do wpisania liczby które przyjmuje pojedyncze litery i zamienia zawartość wyświetlanej tablicy na liczbę zmiennoprzecinkową.
2. Wyświetlającej historię wprowadzonych wartości i przeliczonych jednostek.

Zaimplementowane funkcje:

| Przycisk | Funkcja |
|----------|---------|
| r | Restart wyświetlacza |
| delete | Usunięcie historii temperatur |
| . | Wstawienie kropki |
| 1 - 9 | Wstawienie liczby |
| backspace | Usunięcie ostatniej liczby |
| tab | Wygenerowanie losowej liczby w wyświetlaczu |
| strzałki | Sterowanie kursorem |
| spacja | Wykonanie aktualnej funkcji |

Program przelicza temperaturę w momencie wybrania innej jednostki, aby wybrać inną jednostkę bez wpisania liczby do historii należy najpierw zresetować wyświetlaną liczbę.

*E* przenosi aktywny przycisk do wiersza z jednostkami temperatur, kolejna przeliczona temperatura zostanie zmieniona w wybranym wierszu historii.

*D* usuwa wybrany wiersz z historii.
