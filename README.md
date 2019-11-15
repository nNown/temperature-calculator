# Projekt I na podstawy programowania
### Wojciech Marciniak, Bioinformatyka, I rok

Program kompiluje się tylko z użyciem kompilatora MinGW:
> gcc main.c -o main

Brakuje kompatybilności z Linuxem, program wykorzystuje bibliotekę windows.h do interakcji z konsolą.

Sam program składa się z dwóch części:
1. Wyświetlającej miejsce do wpisania liczby które przyjmuje pojedyncze litery i zamienia zawartość wyświetlanej tablicy na liczbę zmiennoprzecinkową.
2. Wyświetlającej historię wprowadzonych wartości i przeliczonych jednostek.

Zaimplementowane funkcje:
- Restart wyświetlacza, pod przyciskiem: *r*.
- Usunięcie historii temperatur pod przyciskiem: *delete*.
- Zmiana aktualnego znaku liczby na przeciwny pod przyskiem: *-*.
- Wstawienie kropki pod przyciskiem: *kropka*.
- Wprowadzenie liczby pod przyciskami: *1-9*.
- Usunięcie ostatniej liczby pod przyciskiem: *backspace*.
- Wygenerowanie losowej liczby pod przyciskiem: *tab*.
- Zmiana aktualnego przycisku przy użyciu: *strzałek*.
- Wykonanie aktualnie podświetlonej funkcji pod przyciskiem: *spacja*.

Program przelicza temperaturę w momencie wybrania innej jednostki, aby wybrać inną jednostkę bez wpisania liczby do historii należy najpierw zresetować wyświetlaną liczbę.

*E* przenosi aktywny przycisk do wiersza z jednostkami temperatur, kolejna przeliczona temperatura zostanie zmieniona w wybranym wierszu historii.

*D* usuwa wybrany wiersz z historii.
