#define _WIN32_WINNT 0x0500 // Definiuje minimalną wymaganą wersję systemu Windows dla niektórych funkcji API.
#include <conio.h>       // Do obsługi wejścia konsoli bez buforowania (np. _getch).
#include <windows.h>     // Do funkcji API systemu Windows (obsługa konsoli, dźwięk itp.).

#include <algorithm>     // Do algorytmów takich jak std::shuffle, std::max, std::reverse.
#include <chrono>        // Do obsługi czasu (mierzenie czasu gry, opóźnienia animacji).
#include <cmath>         // Do funkcji matematycznych (np. std::fmod).
#include <ctime>         // Do uzyskania czasu (np. std::time) używanego do inicjalizacji generatora liczb losowych.
#include <fstream>       // Do operacji na plikach (zapis/odczyt danych gracza).
#include <iomanip>       // Do manipulacji strumieniem wyjściowym (np. std::setw, std::setfill).
#include <iostream>      // Do standardowego wejścia/wyjścia (nieużywane bezpośrednio do renderowania konsoli).
#include <map>           // Do potencjalnego użycia struktur danych opartych na mapach (obecnie nieużywane w tym fragmencie).
#include <random>        // Do generowania liczb losowych (tasowanie kart).
#include <sstream>       // Do operacji na strumieniach stringów (formatowanie czasu).
#include <stdexcept>     // Do obsługi standardowych wyjątków (np. std::runtime_error).
#include <string>        // Do obsługi obiektów std::string.
#include <thread>        // Do wstrzymywania wykonania wątku (np. std::this_thread::sleep_for).
#include <vector>        // Do używania dynamicznych tablic std::vector.
#include <optional>      // Do reprezentowania wartości, które mogą być obecne lub nie (obecnie nieużywane w tym fragmencie).

// Stała definiująca szerokość konsoli w znakach.
const int CONSOLE_WIDTH = 120;
// Stała definiująca wysokość konsoli w znakach.
const int CONSOLE_HEIGHT = 45;

// Definicje kolorów tekstu dla konsoli Windows API.
const WORD NEO_FOREGROUND_BLACK = 0;
const WORD NEO_FOREGROUND_BLUE = 1;
const WORD NEO_FOREGROUND_GREEN = 2;
const WORD NEO_FOREGROUND_CYAN = 3;
const WORD NEO_FOREGROUND_RED = 4;
const WORD NEO_FOREGROUND_MAGENTA = 5;
const WORD NEO_FOREGROUND_YELLOW = 6;
const WORD NEO_FOREGROUND_WHITE = 7;
// Modyfikator intensywności koloru tekstu.
const WORD NEO_FOREGROUND_INTENSITY = 8;

// Definicje kolorów tła dla konsoli Windows API.
const WORD NEO_BACKGROUND_BLACK = 0;
const WORD NEO_BACKGROUND_BLUE = 16;
const WORD NEO_BACKGROUND_GREEN = 32;
const WORD NEO_BACKGROUND_CYAN = 48;
const WORD NEO_BACKGROUND_RED = 64;
const WORD NEO_BACKGROUND_MAGENTA = 80;
const WORD NEO_BACKGROUND_YELLOW = 96;
const WORD NEO_BACKGROUND_WHITE = 112;
// Modyfikator intensywności koloru tła.
const WORD NEO_BACKGROUND_INTENSITY = 128;

// Domyślny kolor używany w konsoli (biały tekst, czarne tło).
const WORD COLOR_DEFAULT = NEO_FOREGROUND_WHITE;
// Kolor używany do rysowania ramek.
const WORD COLOR_BORDER = NEO_FOREGROUND_BLUE | NEO_FOREGROUND_INTENSITY;
// Kolor używany do wyświetlania tytułów.
const WORD COLOR_TITLE = NEO_FOREGROUND_YELLOW | NEO_FOREGROUND_INTENSITY;
// Kolor używany do wyświetlania etykiet.
const WORD COLOR_LABEL = NEO_FOREGROUND_CYAN | NEO_FOREGROUND_INTENSITY;
// Kolor używany do wyświetlania wartości (np. punktacji).
const WORD COLOR_VALUE = NEO_FOREGROUND_WHITE | NEO_FOREGROUND_INTENSITY;
// Kolor dla czerwonych kart (kier, karo).
const WORD COLOR_CARD_RED = NEO_FOREGROUND_RED | NEO_FOREGROUND_INTENSITY;
// Kolor dla czarnych kart (pik, trefl).
const WORD COLOR_CARD_BLACK = NEO_FOREGROUND_WHITE | NEO_FOREGROUND_INTENSITY;
// Kolor używany do rysowania rewersu karty.
const WORD COLOR_CARD_BACK = NEO_FOREGROUND_BLUE | NEO_FOREGROUND_INTENSITY;
// Kolor używany do rysowania pustego miejsca na kartę.
const WORD COLOR_CARD_EMPTY = NEO_FOREGROUND_WHITE;

// Kolor używany do podświetlania wybranych elementów.
const WORD COLOR_HIGHLIGHTS = NEO_BACKGROUND_YELLOW | NEO_FOREGROUND_BLACK;
// Kolor używany do podświetlania wskazówek.
const WORD COLOR_HINT_HIGHLIGHT = NEO_BACKGROUND_GREEN | NEO_FOREGROUND_BLACK;
// Kolor używany do wyświetlania komunikatów o błędach.
const WORD COLOR_ERROR = NEO_FOREGROUND_RED | NEO_FOREGROUND_INTENSITY;
// Kolor używany do wyświetlania komunikatów o sukcesie.
const WORD COLOR_SUCCESS = NEO_FOREGROUND_GREEN | NEO_FOREGROUND_INTENSITY;
// Kolor używany do wyświetlania komunikatów informacyjnych.
const WORD COLOR_INFO = NEO_FOREGROUND_YELLOW | NEO_FOREGROUND_INTENSITY;
// Kolor dla nieaktywnych elementów menu.
const WORD COLOR_MENU_ITEM = NEO_FOREGROUND_WHITE;
// Kolor dla wybranego elementu menu.
const WORD COLOR_MENU_SELECTED = NEO_BACKGROUND_WHITE | NEO_FOREGROUND_BLACK;
// Kolor dla paska statusu na dole ekranu.
const WORD COLOR_STATUS_BAR = NEO_BACKGROUND_WHITE | NEO_FOREGROUND_BLACK;

// Kody klawiszy specjalnych (strzałki).
const int KEY_UP = 256 + 72;
const int KEY_DOWN = 256 + 80;
const int KEY_LEFT = 256 + 75;
const int KEY_RIGHT = 256 + 77;
// Kody klawiszy standardowych.
const int KEY_ENTER = 13;
const int KEY_ESC = 27;
const int KEY_SPACE = 32;
// Kody dla klawisza 'h' (duża i mała litera).
const int KEY_H_LOWER = 'h';
const int KEY_H_UPPER = 'H';

// Kody klawiszy nawigacyjnych w stylu VIM (używane do poruszania się po kartach).
const int KEY_NAV_K_LOWER = 'k'; // Góra
const int KEY_NAV_K_UPPER = 'K'; // Góra
const int KEY_NAV_J_LOWER = 'j'; // Dół
const int KEY_NAV_J_UPPER = 'J'; // Dół
const int KEY_NAV_H_LOWER = 'h'; // Lewo
const int KEY_NAV_H_UPPER = 'H'; // Lewo
const int KEY_NAV_L_LOWER = 'l'; // Prawo
const int KEY_NAV_L_UPPER = 'L'; // Prawo

// Opóźnienie w milisekundach między klatkami animacji ogólnej.
const int ANIMATION_DELAY_MS = 10;
// Liczba kroków w animacji przesuwania karty.
const int MOVE_ANIMATION_STEPS = 15;
// Opóźnienie w milisekundach między rozdawaniem kolejnych kart.
const int DEAL_ANIMATION_DELAY_MS = 30;
// Opóźnienie w milisekundach między klatkami animacji wygranej.
const int WIN_ANIMATION_DELAY_MS = 50;
// Opóźnienie w milisekundach dla migotania podpowiedzi.
const int HINT_FLASH_DELAY_MS = 150;
// Liczba mignięć podpowiedzi.
const int HINT_FLASH_COUNT = 3;
// Liczba kroków w animacji użycia mocy specjalnej (np. cofnięcie).
const int POWER_ANIMATION_STEPS = 8;
// Opóźnienie w milisekundach między klatkami animacji mocy specjalnej.
const int POWERUP_ANIMATION_DELAY = 20;
// Flaga włączająca/wyłączająca animacje w grze.
const bool ENABLE_ANIMATION = true;

// Deklaracja wyprzedzająca klasy interfejsu użytkownika konsoli.
class ConsoleUI;
// Deklaracja wyprzedzająca klasy logiki gry w pasjansa.
class SolitaireGame;

// Przestrzeń nazw grupująca funkcje pomocnicze do obsługi konsoli.
namespace ConsoleUtils {
// Uchwyt do standardowego wyjścia konsoli.
HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
// Uchwyt do standardowego wejścia konsoli.
HANDLE hInput = GetStdHandle(STD_INPUT_HANDLE);
// Struktura przechowująca informacje o buforze ekranu konsoli.
CONSOLE_SCREEN_BUFFER_INFO consoleScreenBufferInfo;
// Struktura przechowująca informacje o kursorze konsoli.
CONSOLE_CURSOR_INFO cursorInfo;
// Struktura definiująca rozmiar okna konsoli.
SMALL_RECT windowSize = {0, 0, CONSOLE_WIDTH - 1, CONSOLE_HEIGHT - 1};
// Struktura definiująca rozmiar bufora konsoli.
COORD bufferSize = {CONSOLE_WIDTH, CONSOLE_HEIGHT};
// Wskaźnik na bufor znaków konsoli używany do podwójnego buforowania.
CHAR_INFO* consoleBuffer = nullptr;
// Współrzędne (0, 0) używane przy operacjach na buforze.
COORD zeroCoordinate = {0, 0};

// Czyści bufor konsoli, wypełniając go spacjami z domyślnym kolorem.
void clearBuffer() {
    // Sprawdza, czy bufor został zainicjowany.
    if (!consoleBuffer)
        return;
    // Iteruje przez wszystkie komórki bufora.
    for (int i = 0; i < CONSOLE_WIDTH * CONSOLE_HEIGHT; ++i) {
        // Ustawia znak na spację.
        consoleBuffer[i].Char.UnicodeChar = L' ';
        // Ustawia atrybuty na domyślne.
        consoleBuffer[i].Attributes = COLOR_DEFAULT;
    }
}

// Inicjalizuje konsolę do użytku przez aplikację.
void initConsole() {
    // Ustawia tytuł okna konsoli.
    SetConsoleTitle(TEXT("NeoSolitaire"));

    // Ustawia stronę kodową wyjścia i wejścia konsoli na UTF-8, aby poprawnie wyświetlać znaki Unicode.
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);

    // Usuwa stary bufor, jeśli istnieje.
    delete[] consoleBuffer;
    // Tworzy nowy bufor konsoli o odpowiednich wymiarach.
    consoleBuffer = new CHAR_INFO[CONSOLE_WIDTH * CONSOLE_HEIGHT];
    // Wypełnia nowy bufor spacjami.
    clearBuffer();

    // Ustawia rozmiar bufora ekranu konsoli.
    SetConsoleScreenBufferSize(hConsole, bufferSize);
    // Ustawia rozmiar okna konsoli.
    SetConsoleWindowInfo(hConsole, TRUE, &windowSize);

    // Pobiera uchwyt do okna konsoli.
    HWND consoleWindow = GetConsoleWindow();
    // Sprawdza, czy uchwyt jest poprawny.
    if (consoleWindow != NULL) {
        // Wyłącza możliwość maksymalizacji i zmiany rozmiaru okna konsoli.
        SetWindowLong(consoleWindow, GWL_STYLE, GetWindowLong(consoleWindow, GWL_STYLE) & ~WS_MAXIMIZEBOX & ~WS_SIZEBOX);
    }

    // Pobiera informacje o kursorze konsoli.
    GetConsoleCursorInfo(hConsole, &cursorInfo);
    // Ukrywa kursor konsoli.
    cursorInfo.bVisible = false;
    // Zastosowuje zmiany ustawień kursora.
    SetConsoleCursorInfo(hConsole, &cursorInfo);

    // Pobiera obecny tryb wejścia konsoli.
    DWORD previousMode;
    GetConsoleMode(hInput, &previousMode);
    // Ustawia nowy tryb wejścia: włączone rozszerzone flagi, przetwarzanie wejścia, wyłączony tryb QuickEdit.
    SetConsoleMode(hInput, ENABLE_EXTENDED_FLAGS | ENABLE_PROCESSED_INPUT | (previousMode & ~ENABLE_QUICK_EDIT_MODE));
}

// Przywraca oryginalne ustawienia konsoli i zwalnia zasoby.
void cleanupConsole() {
    // Struktura do przechowywania oryginalnych informacji o kursorze.
    CONSOLE_CURSOR_INFO originalCursorInfo;
    // Pobiera aktualne informacje o kursorze.
    if (GetConsoleCursorInfo(hConsole, &originalCursorInfo)) {
        // Przywraca widoczność kursora.
        originalCursorInfo.bVisible = true;
        // Zastosowuje oryginalne ustawienia kursora.
        SetConsoleCursorInfo(hConsole, &originalCursorInfo);
    }
    // Usuwa bufor konsoli.
    delete[] consoleBuffer;
    // Ustawia wskaźnik bufora na null, aby zapobiec podwójnemu zwolnieniu.
    consoleBuffer = nullptr;
}


// Renderuje zawartość bufora `consoleBuffer` na ekranie konsoli.
void renderBuffer() {
    // Sprawdza, czy bufor jest zainicjowany.
    if (!consoleBuffer)
        return;
    // Zapisuje zawartość bufora do konsoli.
    WriteConsoleOutputW(hConsole, consoleBuffer, bufferSize, zeroCoordinate, &windowSize);
}

// Ustawia pojedynczy znak w buforze konsoli pod określonymi współrzędnymi z określonymi atrybutami.
void setChar(int x, int y, wchar_t c, WORD attributes = COLOR_DEFAULT) {
    // Sprawdza, czy współrzędne mieszczą się w granicach bufora i czy bufor jest zainicjowany.
    if (consoleBuffer && x >= 0 && x < CONSOLE_WIDTH && y >= 0 && y < CONSOLE_HEIGHT) {
        // Ustawia znak Unicode w odpowiedniej komórce bufora.
        consoleBuffer[y * CONSOLE_WIDTH + x].Char.UnicodeChar = c;
        // Ustawia atrybuty (kolor) w odpowiedniej komórce bufora.
        consoleBuffer[y * CONSOLE_WIDTH + x].Attributes = attributes;
    }
}

// Wpisuje ciąg znaków (std::string, potencjalnie UTF-8) do bufora konsoli.
void writeString(int x, int y, const std::string& str, WORD attributes = COLOR_DEFAULT) {
    // Inicjalizuje bieżącą współrzędną X.
    int currentX = x;
    // Iteruje przez bajty w stringu wejściowym.
    for (size_t i = 0; i < str.length(); ++i) {
        // Domyślnie ustawia znak zastępczy.
        wchar_t currentCharacter = L'?';
        // Pobiera pierwszy bajt bieżącego znaku.
        unsigned char firstChar = str[i];

        // Sprawdza, czy znak jest zwykłym znakiem ASCII (1 bajt).
        if (firstChar < 0x80) {
            currentCharacter = (wchar_t)firstChar;
        } 
        // Sprawdza, czy znak jest 2-bajtowym znakiem UTF-8.
        else if (firstChar >= 0xC2 && firstChar <= 0xDF && i + 1 < str.length()) {
            // Pobiera drugi bajt.
            unsigned char secondChar = str[i + 1];
            // Sprawdza, czy drugi bajt jest poprawnym bajtem kontynuacyjnym.
            if (secondChar >= 0x80 && secondChar <= 0xBF) {
                // Dekoduje znak UTF-8 na wchar_t.
                currentCharacter = ((wchar_t)(firstChar & 0x1F) << 6) | (wchar_t)(secondChar & 0x3F);
                // Przesuwa indeks o dodatkowy bajt.
                i++;
            }
        } 
        // Sprawdza, czy znak jest 3-bajtowym znakiem UTF-8.
        else if (firstChar >= 0xE0 && firstChar <= 0xEF && i + 2 < str.length()) {
            // Pobiera drugi i trzeci bajt.
            unsigned char secondChar = str[i + 1];
            unsigned char thirdChar = str[i + 2];
            // Sprawdza, czy drugi i trzeci bajt są poprawnymi bajtami kontynuacyjnymi.
            if (secondChar >= 0x80 && secondChar <= 0xBF && thirdChar >= 0x80 && thirdChar <= 0xBF) {
                // Sprawdza specjalny przypadek dla symboli karcianych w bloku Miscellaneous Symbols (U+2660 - U+2667).
                if (firstChar == 0xE2 && secondChar == 0x99) { 
                    if (thirdChar == 0xA5) currentCharacter = L'♥'; // Kier
                    else if (thirdChar == 0xA6) currentCharacter = L'♦'; // Karo
                    else if (thirdChar == 0xA3) currentCharacter = L'♣'; // Trefl
                    else if (thirdChar == 0xA0) currentCharacter = L'♠'; // Pik
                    // Jeśli to nie symbol karty, dekoduje standardowo.
                    else currentCharacter = ((wchar_t)(firstChar & 0x0F) << 12) | ((wchar_t)(secondChar & 0x3F) << 6) | (wchar_t)(thirdChar & 0x3F);
                } else {
                    // Dekoduje standardowy 3-bajtowy znak UTF-8.
                    currentCharacter = ((wchar_t)(firstChar & 0x0F) << 12) | ((wchar_t)(secondChar & 0x3F) << 6) | (wchar_t)(thirdChar & 0x3F);
                }
                // Przesuwa indeks o dodatkowe dwa bajty.
                i += 2;
            }
        }
        // Ustawia zdekodowany znak w buforze i przechodzi do następnej pozycji X.
        setChar(currentX++, y, currentCharacter, attributes);
        // Przerywa pętlę, jeśli wykracza poza szerokość konsoli.
        if (currentX >= CONSOLE_WIDTH)
            break;
    }
}

// Wpisuje ciąg znaków szerokich (std::wstring) do bufora konsoli.
void writeStringWide(int x, int y, const std::wstring& wstr, WORD attributes = COLOR_DEFAULT) {
    // Iteruje przez znaki w stringu szerokim.
    for (size_t i = 0; i < wstr.length(); ++i) {
        // Sprawdza, czy bieżąca pozycja X mieści się w granicach konsoli.
        if (x + static_cast<int>(i) >= CONSOLE_WIDTH)
            break; // Przerywa, jeśli przekroczono szerokość.
        // Ustawia znak szeroki w buforze.
        setChar(x + static_cast<int>(i), y, wstr[i], attributes);
    }
}

// Wypełnia prostokątny obszar w buforze konsoli określonym znakiem i atrybutami.
void fillRectangle(int x, int y, int width, int height, wchar_t c = L' ', WORD attributes = COLOR_DEFAULT) {
    // Koryguje współrzędne i wymiary, aby mieściły się w granicach konsoli.
    if (x < 0) { width += x; x = 0; }
    if (y < 0) { height += y; y = 0; }
    if (x + width > CONSOLE_WIDTH) width = CONSOLE_WIDTH - x;
    if (y + height > CONSOLE_HEIGHT) height = CONSOLE_HEIGHT - y;
    // Sprawdza, czy wymiary są poprawne.
    if (width <= 0 || height <= 0) return;

    // Iteruje przez wiersze prostokąta.
    for (int j = y; j < y + height; ++j) {
        // Iteruje przez kolumny prostokąta.
        for (int i = x; i < x + width; ++i) {
            // Ustawia znak w bieżącej komórce bufora.
            setChar(i, j, c, attributes);
        }
    }
}

// Rysuje ramkę (pojedynczą lub podwójną) w buforze konsoli.
void drawBox(int x, int y, int width, int height, WORD attributes = COLOR_BORDER, bool doubleBorder = false) {
    // Definicje znaków do rysowania ramki.
    wchar_t topLeftChar, topRightChar, bottomLeftChar, bottomRightChar, horizontalChar, verticalChar;
    // Wybiera zestaw znaków w zależności od typu ramki.
    if (doubleBorder) {
        topLeftChar = L'╔'; topRightChar = L'╗'; bottomLeftChar = L'╚'; bottomRightChar = L'╝';
        horizontalChar = L'═'; verticalChar = L'║';
    } else {
        topLeftChar = L'┌'; topRightChar = L'┐'; bottomLeftChar = L'└'; bottomRightChar = L'┘';
        horizontalChar = L'─'; verticalChar = L'│';
    }

    // Sprawdza, czy wymiary ramki są poprawne.
    if (width <= 0 || height <= 0) return;

    // Rysuje narożniki.
    setChar(x, y, topLeftChar, attributes);
    if (width > 1) setChar(x + width - 1, y, topRightChar, attributes);
    if (height > 1) setChar(x, y + height - 1, bottomLeftChar, attributes);
    if (width > 1 && height > 1) setChar(x + width - 1, y + height - 1, bottomRightChar, attributes);

    // Rysuje linie poziome.
    for (int i = x + 1; i < x + width - 1; ++i) {
        setChar(i, y, horizontalChar, attributes);
        if (height > 1) setChar(i, y + height - 1, horizontalChar, attributes);
    }
    // Rysuje linie pionowe.
    for (int j = y + 1; j < y + height - 1; ++j) {
        setChar(x, j, verticalChar, attributes);
        if (width > 1) setChar(x + width - 1, j, verticalChar, attributes);
    }
}

// Odczytuje naciśnięty klawisz z konsoli (obsługuje klawisze specjalne).
int getKeyPress() {
    // Odczytuje pierwszy bajt kodu klawisza.
    int key = _getch();

    // Sprawdza, czy jest to klawisz specjalny (zwykle poprzedzony 0 lub 224).
    if (key == 0 || key == 224) { 
        // Odczytuje drugi bajt kodu klawisza.
        int secondKey = _getch();
        // Zwraca wartość > 255, aby odróżnić od zwykłych klawiszy ASCII.
        return 256 + secondKey;
    }
    // Zwraca kod ASCII zwykłego klawisza.
    return key;
}

// Odtwarza dźwięk o zadanej częstotliwości i czasie trwania.
void playSound(int freq, int duration) {
    // Używa funkcji Beep z Windows API.
    Beep(freq, duration);
}

// Wstrzymuje wykonanie bieżącego wątku na określoną liczbę milisekund.
void sleep(int milliseconds) {
    // Używa funkcji sleep_for z biblioteki <thread>.
    std::this_thread::sleep_for(std::chrono::milliseconds(milliseconds));
}


// Pobiera tekst wprowadzany przez użytkownika w określonym miejscu konsoli.
std::string getTextInput(int startX, int startY, int maxLength, const std::string& initialValue = "") {
    // Inicjalizuje tekst wejściowy wartością początkową.
    std::string inputText = initialValue;
    // Zapamiętuje początkową pozycję kursora.
    COORD initialCursorPos = {(SHORT)startX, (SHORT)startY};
    // Pobiera oryginalne ustawienia kursora.
    CONSOLE_CURSOR_INFO originalCursorInfo;
    GetConsoleCursorInfo(hConsole, &originalCursorInfo); 
    // Tworzy kopię ustawień kursora do edycji.
    CONSOLE_CURSOR_INFO editCursorInfo = originalCursorInfo;
    // Włącza widoczność kursora podczas edycji.
    editCursorInfo.bVisible = true; 
    // Ustawia kursor na widoczny.
    SetConsoleCursorInfo(hConsole, &editCursorInfo);

    // Lambda do odświeżania pola tekstowego w konsoli.
    auto redrawInput = [&]() {
        // Czyści obszar wprowadzania tekstu.
        fillRectangle(startX, startY, maxLength + 1, 1, L' '); 
        // Wypisuje bieżący tekst.
        writeString(startX, startY, inputText);
        // Ustawia pozycję kursora na końcu wprowadzonego tekstu.
        SetConsoleCursorPosition(hConsole, {(SHORT)(startX + inputText.length()), (SHORT)startY});
        // Renderuje zmiany na ekranie (ważne, bo operujemy na buforze).
        renderBuffer(); 
    };

    // Odświeża pole tekstowe na początku.
    redrawInput();

    // Pętla obsługi wprowadzania znaków.
    while (true) {
        // Odczytuje naciśnięty klawisz.
        int key = getKeyPress();

        // Jeśli naciśnięto Enter, kończy wprowadzanie.
        if (key == KEY_ENTER) {
            break;
        } 
        // Jeśli naciśnięto Esc, anuluje zmiany i kończy wprowadzanie.
        else if (key == KEY_ESC) {
            inputText = initialValue; // Przywraca wartość początkową.
            break;
        } 
        // Jeśli naciśnięto Backspace lub Delete (kod 127 może być używany w niektórych terminalach).
        else if (key == 8 || key == 127) { 
            // Sprawdza, czy tekst nie jest pusty.
            if (!inputText.empty()) {
                // Usuwa ostatni znak.
                inputText.pop_back();
                // Odświeża pole tekstowe.
                redrawInput();
            }
        } 
        // Jeśli naciśnięto drukowalny znak ASCII.
        else if (key >= 32 && key <= 126) { 
            // Sprawdza, czy nie przekroczono maksymalnej długości.
            if (inputText.length() < (size_t)maxLength) {
                // Dodaje znak do tekstu.
                inputText += static_cast<char>(key);
                // Odświeża pole tekstowe.
                redrawInput();
            }
        }
        // Ignoruje inne klawisze.
    }

    // Przywraca oryginalne ustawienia kursora (zazwyczaj ukryty).
    SetConsoleCursorInfo(hConsole, &originalCursorInfo); 
    // Zwraca wprowadzony tekst.
    return inputText;
}

} // Koniec przestrzeni nazw ConsoleUtils

// Klasa reprezentująca pojedynczą kartę do gry.
class Card {
private:
    // Kolor karty ('H' - Kier, 'D' - Karo, 'C' - Trefl, 'S' - Pik, '?' - nieznany/Joker).
    char suit;
    // Wartość karty (1-13, gdzie 1=As, 11=Walet, 12=Dama, 13=Król).
    int value;
    // Flaga wskazująca, czy karta jest odkryta (awers).
    bool faceUp;
    // Flaga wskazująca, czy karta jest Jokerem.
    bool isJoker;

public:
    // Domyślny konstruktor tworzący 'pustą' kartę.
    Card() : suit('?'), value(0), faceUp(false), isJoker(false) {}

    // Konstruktor tworzący kartę o podanym kolorze, wartości i opcjonalnie jako Jokera.
    Card(char s, int v, bool joker = false) : suit(s), value(v), faceUp(false), isJoker(joker) {}

    // Zwraca kolor karty.
    char getSuit() const { return suit; }
    // Zwraca wartość karty.
    int getValue() const { return value; }
    // Zwraca true, jeśli karta jest odkryta.
    bool isFaceUp() const { return faceUp; }
    // Zwraca true, jeśli karta jest Jokerem.
    bool getIsJoker() const { return isJoker; }
    // Odwraca kartę (zmienia stan faceUp).
    void flip() { faceUp = !faceUp; }
    // Ustawia stan odkrycia karty (odkryta/zakryta).
    void setFaceUp(bool up) { faceUp = up; }

    // Zwraca wartość karty jako string Unicode (np. "A", "K", "10", "*").
    std::wstring getValueAsString() const {
        if (isJoker) return L"*"; // Symbol Jokera.
        switch (value) {
        case 1: return L"A";  // As.
        case 10: return L"T"; // Dziesiątka (T zamiast 10 dla jednolitej szerokości).
        case 11: return L"J"; // Walet.
        case 12: return L"Q"; // Dama.
        case 13: return L"K"; // Król.
        default: return std::to_wstring(value); // Wartości 2-9.
        }
    }

    // Zwraca symbol koloru karty jako string Unicode (np. "♥", "♦", "♣", "♠", "J").
    std::wstring getSuitAsWString() const {
        if (isJoker) return L"J"; // Symbol koloru dla Jokera.
        switch (suit) {
        case 'H': return L"♥"; // Kier.
        case 'D': return L"♦"; // Karo.
        case 'C': return L"♣"; // Trefl.
        case 'S': return L"♠"; // Pik.
        default: return L"?"; // Nieznany kolor.
        }
    }

     // Zwraca skróconą reprezentację tekstową karty (np. "[░]", " A♥", " T♦").
     std::wstring toStringShort() const {
        if(isJoker) return L"[J*]"; // Reprezentacja Jokera.
        // Jeśli karta jest zakryta, zwraca symbol rewersu.
        if (!faceUp) {
            return L"[░]"; 
        }
        // Pobiera wartość karty jako string.
        std::wstring valueString = getValueAsString();

        // Zwraca sformatowany string z wartością i kolorem (zawsze 3 znaki).
        return L" " + (valueString.length() == 1 ? valueString : valueString.substr(0,1)) + getSuitAsWString();
    }


    // Zwraca parę stringów Unicode: wartość i kolor, używane do rysowania karty.
    std::pair<std::wstring, std::wstring> getDrawStrings() const {
        if(isJoker) return {L"*", L"J"}; // Wartość i "kolor" dla Jokera.
        if (!faceUp) return {L" ", L" "}; // Puste stringi dla zakrytej karty.
        // Zwraca parę: wartość i kolor dla odkrytej karty.
        return {getValueAsString(), getSuitAsWString()};
    }

    // Zwraca true, jeśli karta jest czerwona (Kier lub Karo) i nie jest Jokerem.
    bool isRed() const {
        return !isJoker && (suit == 'H' || suit == 'D');
    }

    // Zwraca atrybut koloru (konsola Windows) dla danej karty.
    WORD getColor() const {
        if(isJoker) return NEO_FOREGROUND_MAGENTA | NEO_FOREGROUND_INTENSITY; // Kolor dla Jokera.
        // Kolor rewersu karty.
        if (!faceUp) return COLOR_CARD_BACK;
        // Kolor dla czerwonych kart.
        if (isRed()) {
            return COLOR_CARD_RED;
        } 
        // Kolor dla czarnych kart.
        else {
            return COLOR_CARD_BLACK;
        }
    }
};


// Klasa reprezentująca gracza i jego statystyki.
class Player {
private:
    // Nazwa gracza.
    std::string name;
    // Aktualny wynik gracza w bieżącej grze.
    int score;
    // Najwyższy osiągnięty wynik gracza.
    int highScore;
    // Liczba pozostałych cofnięć.
    int undosLeft;
    // Liczba pozostałych przetasowań stosu.
    int shufflesLeft;
    // Liczba pozostałych odkryć kart zakrytych.
    int revealsLeft;
    // Liczba pozostałych Jokerów do użycia.
    int jokerCardsLeft;
    // Liczba pozostałych wskazówek.
    int hintsLeft; 
    // Flaga wskazująca, czy włączony jest tryb łatwy (więcej wspomagaczy).
    bool easyMode; 
    // Flaga wskazująca, czy włączone są wspomagacze (power-upy).
    bool powerupsEnabled;
    // Flaga wskazująca, czy gracz gra po raz pierwszy (może służyć do wyświetlania samouczka).
    bool firstTimePlayer; 
    // Punkt czasowy rozpoczęcia gry.
    std::chrono::time_point<std::chrono::steady_clock> gameStartTime;
    // Czas trwania ostatniej zakończonej gry.
    std::chrono::duration<double> gameDuration;
    // Flaga wskazująca, czy stoper gry jest aktywny.
    bool timerRunning;

public:
    // Konstruktor gracza, ustawia domyślną nazwę i inicjalizuje stan.
    Player(const std::string& n = "Player")
        : name(n), score(0), highScore(0),
          easyMode(false), powerupsEnabled(true),
          firstTimePlayer(true), // Domyślnie zakłada nowego gracza.
          timerRunning(false), gameDuration(std::chrono::duration<double>::zero())
    {
        // Resetuje liczbę wspomagaczy do wartości domyślnych.
        resetPowerups();
        // Wczytuje dane gracza z pliku (jeśli istnieje).
        loadFromFile();
    }

    // Ustawia nową nazwę gracza.
    void setName(const std::string& newName) { name = newName; }
    // Zwraca nazwę gracza.
    std::string getName() const { return name; }

    // Zwraca aktualny wynik gracza.
    int getScore() const { return score; }
    // Dodaje punkty do wyniku (wynik nie może być ujemny).
    void addScore(int points) { 
        // Upewnia się, że wynik nie spadnie poniżej zera.
        score = std::max(0, score + points); 
        // Jeśli nowy wynik jest wyższy niż rekord, aktualizuje rekord i zapisuje.
        if (score > highScore) {
            highScore = score;
            saveToFile(); // Zapisuje nowy rekord.
        }
    }
    // Resetuje wynik bieżącej gry do zera.
    void resetScore() { 
        score = 0; 
    }

    // Zwraca najwyższy wynik gracza.
    int getHighScore() const { return highScore; }
    // Sprawdza, czy obecny wynik jest nowym rekordem i ewentualnie go aktualizuje i zapisuje.
    void updateHighScore() { 
        if (score > highScore) {
            highScore = score; 
            saveToFile(); // Zapisuje nowy rekord.
        }
    }

    // Zwraca liczbę pozostałych cofnięć.
    int getUndosLeft() const { return undosLeft; }
    // Używa jednego cofnięcia, zwraca true jeśli się udało, false w przeciwnym razie.
    bool useUndo() {
        // Sprawdza, czy są dostępne cofnięcia.
        if (undosLeft > 0) {
            undosLeft--; // Zmniejsza liczbę dostępnych cofnięć.
            return true; // Zwraca sukces.
        }
        return false; // Brak dostępnych cofnięć.
    }
    // Zwraca jedno cofnięcie (np. gdy cofnięcie nie powiodło się na poziomie gry).
    void refundUndo() { undosLeft++; } 
    // Resetuje liczbę cofnięć do wartości zależnej od trybu trudności.
    void resetUndos() { undosLeft = easyMode ? 5 : 3; }
    // Ustawia konkretną liczbę cofnięć (nie mniej niż 0).
    void setUndosLeft(int count) { undosLeft = std::max(0, count); }


    // Zwraca liczbę pozostałych przetasowań.
    int getShufflesLeft() const { return shufflesLeft; }
    // Używa jednego przetasowania, zwraca true jeśli się udało, false w przeciwnym razie.
    bool useShuffle() {
        // Sprawdza, czy są dostępne przetasowania.
        if (shufflesLeft > 0) {
            shufflesLeft--; // Zmniejsza liczbę dostępnych przetasowań.
            addScore(-20); // Odejmuje punkty za użycie przetasowania.
            return true; // Zwraca sukces.
        }
        return false; // Brak dostępnych przetasowań.
    }
    // Resetuje liczbę przetasowań do wartości zależnej od trybu trudności.
    void resetShuffles() { shufflesLeft = easyMode ? 3 : 2; }
    // Ustawia konkretną liczbę przetasowań (nie mniej niż 0).
    void setShufflesLeft(int count) { shufflesLeft = std::max(0, count); }

    // Zwraca liczbę pozostałych odkryć.
    int getRevealsLeft() const { return revealsLeft; }
    // Używa jednego odkrycia, zwraca true jeśli się udało, false w przeciwnym razie.
    bool useReveal() {
        // Sprawdza, czy są dostępne odkrycia.
        if (revealsLeft > 0) {
            revealsLeft--; // Zmniejsza liczbę dostępnych odkryć.
             addScore(-30); // Odejmuje punkty za użycie odkrycia.
            return true; // Zwraca sukces.
        }
        return false; // Brak dostępnych odkryć.
    }
    // Resetuje liczbę odkryć do wartości zależnej od trybu trudności.
    void resetReveals() { revealsLeft = easyMode ? 5 : 3; }
    // Ustawia konkretną liczbę odkryć (nie mniej niż 0).
    void setRevealsLeft(int count) { revealsLeft = std::max(0, count); }


    // Zwraca liczbę pozostałych Jokerów.
    int getJokerCardsLeft() const { return jokerCardsLeft; }
    // Używa jednego Jokera, zwraca true jeśli się udało, false w przeciwnym razie.
    bool useJokerCard() {
        // Sprawdza, czy są dostępne Jokery.
        if (jokerCardsLeft > 0) {
            jokerCardsLeft--; // Zmniejsza liczbę dostępnych Jokerów.
             addScore(-50); // Odejmuje punkty za użycie Jokera.
            return true; // Zwraca sukces.
        }
        return false; // Brak dostępnych Jokerów.
    }
     // Zwraca jednego Jokera (np. gdy nie można go było umieścić).
     void refundJokerCard() { jokerCardsLeft++; } 
    // Resetuje liczbę Jokerów do wartości zależnej od trybu trudności.
    void resetJokerCards() { jokerCardsLeft = easyMode ? 2 : 1; }
    // Ustawia konkretną liczbę Jokerów (nie mniej niż 0).
    void setJokerCardsLeft(int count) { jokerCardsLeft = std::max(0, count); }

    // Zwraca liczbę pozostałych wskazówek.
    int getHintsLeft() const { return hintsLeft; }
    // Używa jednej wskazówki, zwraca true jeśli się udało, false w przeciwnym razie.
    bool useHint() {
        // Sprawdza, czy są dostępne wskazówki.
        if (hintsLeft > 0) {
            hintsLeft--; // Zmniejsza liczbę dostępnych wskazówek.
            addScore(-10); // Odejmuje punkty za użycie wskazówki.
            return true; // Zwraca sukces.
        }
        return false; // Brak dostępnych wskazówek.
    }

    // Zwraca jedną wskazówkę i punkty (np. gdy wskazówka nie została znaleziona).
    void refundHint() {
        hintsLeft++;
        addScore(10); // Dodaje punkty z powrotem.
    }
    // Resetuje liczbę wskazówek do wartości zależnej od trybu trudności.
    void resetHints() { hintsLeft = easyMode ? 10 : 5; }
    // Ustawia konkretną liczbę wskazówek (nie mniej niż 0).
    void setHintsLeft(int count) { hintsLeft = std::max(0, count); }


    // Zwraca true, jeśli tryb łatwy jest włączony.
    bool getEasyMode() const { return easyMode; }
    // Przełącza tryb łatwy i resetuje wspomagacze.
    void toggleEasyMode() {
        easyMode = !easyMode;
        resetPowerups(); // Dostosowuje liczbę wspomagaczy do nowego trybu.
    }

    // Resetuje liczbę wszystkich wspomagaczy do wartości domyślnych dla bieżącego trybu trudności.
    void resetPowerups() {
        resetUndos();
        resetShuffles();
        resetReveals();
        resetJokerCards();
        resetHints(); 
    }

    // Uruchamia stoper gry.
    void startGameTimer() {
        gameStartTime = std::chrono::steady_clock::now(); // Zapamiętuje czas startu.
        timerRunning = true; // Ustawia flagę działania stopera.
        gameDuration = std::chrono::duration<double>::zero(); // Resetuje zmierzony czas poprzedniej gry.
    }

    // Zatrzymuje stoper gry.
    void stopGameTimer() {
        // Sprawdza, czy stoper jest uruchomiony.
        if (timerRunning) {
            // Oblicza czas trwania od startu do teraz.
            gameDuration = std::chrono::steady_clock::now() - gameStartTime;
            timerRunning = false; // Zatrzymuje stoper.
        }
    }

    // Zwraca czas gry w sekundach (bieżący, jeśli stoper działa, lub zapisany, jeśli zatrzymany).
    double getElapsedSeconds() const {
        // Jeśli stoper działa, oblicza czas na bieżąco.
        if (timerRunning) {
            return std::chrono::duration_cast<std::chrono::duration<double>>(std::chrono::steady_clock::now() - gameStartTime).count();
        } 
        // Jeśli stoper jest zatrzymany, zwraca zapisany czas trwania.
        else {
            return gameDuration.count();
        }
    }

    // Zwraca sformatowany czas gry w postaci "MM:SS".
    std::string getFormattedTime() const {
        // Pobiera łączny czas w sekundach.
        double totalSeconds = getElapsedSeconds();
        // Oblicza minuty.
        int minutes = static_cast<int>(totalSeconds / 60);
        // Oblicza sekundy.
        int seconds = static_cast<int>(std::fmod(totalSeconds, 60.0));
        // Tworzy strumień stringów do formatowania.
        std::stringstream ss;
        // Formatuje czas z wiodącymi zerami (np. 01:05).
        ss << std::setfill('0') << std::setw(2) << minutes << ":" << std::setw(2) << seconds;
        // Zwraca sformatowany string.
        return ss.str();
    }

    // Zapisuje dane gracza (nazwa, rekord, ustawienia) do pliku.
    void saveToFile() {
        // Nazwa pliku do zapisu.
        std::string filename = "player_data.txt";
        // Otwiera plik do zapisu.
        std::ofstream file(filename);
        
        // Sprawdza, czy plik został poprawnie otwarty.
        if (!file.is_open()) {
            // Można dodać obsługę błędu, np. logowanie.
            return; 
        }
        
        // Zapisuje dane gracza, każda w nowej linii.
        file << name << "\n";
        file << highScore << "\n";
        file << (easyMode ? "1" : "0") << "\n"; // Zapisuje stan trybu łatwego jako 1 lub 0.
        file << (powerupsEnabled ? "1" : "0") << "\n"; // Zapisuje stan włączenia wspomagaczy.
        file << (firstTimePlayer ? "1" : "0") << "\n"; // Zapisuje stan "pierwszy raz".
        
        // Zamyka plik.
        file.close();
    }

    // Wczytuje dane gracza z pliku.
    void loadFromFile() {
        // Nazwa pliku do odczytu.
        std::string filename = "player_data.txt";
        // Otwiera plik do odczytu.
        std::ifstream file(filename);
        
        // Sprawdza, czy plik istnieje i został poprawnie otwarty.
        if (!file.is_open()) {
            // Jeśli plik nie istnieje (lub wystąpił błąd), używa domyślnych wartości.
            return; 
        }
        
        // Zmienna do przechowywania odczytanej linii.
        std::string line;

        // Odczytuje nazwę gracza.
        if (std::getline(file, line)) {
            name = line;
        }

        // Odczytuje najwyższy wynik.
        if (std::getline(file, line)) {
            try {
                // Próbuje przekonwertować string na liczbę całkowitą.
                highScore = std::stoi(line);
            } catch (...) {
                // W razie błędu konwersji ustawia rekord na 0.
                highScore = 0;
            }
        }

        // Odczytuje ustawienie trybu łatwego.
        if (std::getline(file, line)) {
            easyMode = (line == "1"); // Ustawia tryb łatwy, jeśli w pliku jest "1".
        }

        // Odczytuje ustawienie włączenia wspomagaczy.
        if (std::getline(file, line)) {
            powerupsEnabled = (line == "1"); // Ustawia flagę, jeśli w pliku jest "1".
        } else {
            // Domyślna wartość, jeśli brakuje wpisu w pliku (kompatybilność wsteczna).
            powerupsEnabled = true; 
        }

        // Odczytuje flagę "pierwszy raz".
        if (std::getline(file, line)) {
            firstTimePlayer = (line == "1"); // Ustawia flagę, jeśli w pliku jest "1".
        } else {
            // Domyślna wartość, jeśli brakuje wpisu.
            firstTimePlayer = true; 
        }
        
        // Resetuje wspomagacze zgodnie z wczytanym trybem trudności.
        resetPowerups();
        // Zamyka plik.
        file.close();
    }

    // Zwraca true, jeśli wspomagacze są włączone.
    bool getPowerupsEnabled() const { return powerupsEnabled; }
    // Przełącza stan włączenia wspomagaczy.
    void togglePowerupsEnabled() { 
        powerupsEnabled = !powerupsEnabled; 
        // Jeśli wspomagacze są wyłączone, zeruje ich liczniki.
        if (!powerupsEnabled) {
            // Zeruje wszystkie wspomagacze.
            undosLeft = 0;
            shufflesLeft = 0;
            revealsLeft = 0;
            jokerCardsLeft = 0;
            hintsLeft = 0;
        } 
        // Jeśli wspomagacze są włączone, resetuje je do wartości domyślnych.
        else {
            // Resetuje wspomagacze.
            resetPowerups();
        }
    }

    // Zwraca true, jeśli gracz gra po raz pierwszy.
    bool isFirstTimePlayer() const { return firstTimePlayer; }
    // Ustawia flagę "pierwszy raz".
    void setFirstTimePlayer(bool isFirst) { firstTimePlayer = isFirst; }

    // Resetuje najwyższy wynik gracza do zera i zapisuje zmiany.
    void resetHighScore() {
        highScore = 0;
        saveToFile(); // Zapisuje wyzerowany rekord.
    }
};

// Struktura przechowująca kompletny stan gry w danym momencie (używana do historii ruchów).
struct GameState {
    // Stan kolumn tableau.
    std::vector<std::vector<Card>> tableau;
    // Stan stosu kart zakrytych (stock).
    std::vector<Card> stock;
    // Stan stosu kart odkrytych (waste).
    std::vector<Card> waste;
    // Stan pól bazowych (fundamentów).
    std::vector<std::vector<Card>> foundations;
    // Zapisany wynik gracza.
    int score;
    // Zapisana liczba pozostałych cofnięć.
    int undosLeft;
    // Zapisana liczba pozostałych przetasowań.
    int shufflesLeft;
    // Zapisana liczba pozostałych odkryć.
    int revealsLeft;
    // Zapisana liczba pozostałych Jokerów.
    int jokerCardsLeft;
    // Zapisana liczba pozostałych wskazówek.
    int hintsLeft; 
    // Zapisana liczba wykonanych ruchów.
    int movesMade;
};

// Typ wyliczeniowy określający typ elementu, który może być wybrany przez gracza.
enum class SelectableType { 
    NONE,           // Brak zaznaczenia.
    STOCK,          // Stos kart zakrytych (stock).
    WASTE,          // Stos kart odkrytych (waste).
    FOUNDATION,     // Pole bazowe (fundament).
    TABLEAU_PILE,   // Cała kolumna tableau (gdy jest pusta).
    TABLEAU_CARD    // Pojedyncza karta (lub grupa kart) w kolumnie tableau.
};

// Struktura reprezentująca aktualne zaznaczenie gracza na planszy.
struct Selection {
    // Typ zaznaczonego elementu.
    SelectableType type = SelectableType::NONE;
    // Indeks stosu/pola (0-6 dla tableau, 0-3 dla fundamentów, -1 dla stock/waste).
    int index = -1;      
    // Indeks karty wewnątrz stosu tableau (jeśli dotyczy).
    int cardIndex = -1;  

    // Resetuje zaznaczenie do stanu początkowego (brak zaznaczenia).
    void clear() {
        type = SelectableType::NONE;
        index = -1;
        cardIndex = -1;
    }
    // Zwraca true, jeśli zaznaczenie jest aktywne (nie jest typu NONE).
    bool isValid() const { return type != SelectableType::NONE; }

    // Operator porównania równości dla zaznaczeń.
    bool operator==(const Selection& other) const {
        return type == other.type && index == other.index && cardIndex == other.cardIndex;
    }
    // Operator porównania nierówności dla zaznaczeń.
    bool operator!=(const Selection& other) const {
        return !(*this == other);
    }
};

// Struktura reprezentująca podpowiedź (możliwy ruch).
struct Hint {
    // Zaznaczenie źródłowe ruchu.
    Selection source;
    // Zaznaczenie docelowe ruchu.
    Selection destination;
    // Opis słowny podpowiedzi (opcjonalny, obecnie nieużywany).
    std::wstring description; 
};

// Główna klasa zarządzająca logiką gry w pasjansa Klondike.
class SolitaireGame {
private:
    // Obiekt gracza przechowujący statystyki i ustawienia.
    Player player;
    // Wektor wektorów kart reprezentujący 7 kolumn tableau.
    std::vector<std::vector<Card>> tableau; 
    // Wektor kart reprezentujący stos kart zakrytych (stock).
    std::vector<Card> stock;
    // Wektor kart reprezentujący stos kart odkrytych (waste).
    std::vector<Card> waste;
    // Wektor wektorów kart reprezentujący 4 pola bazowe (fundamenty).
    std::vector<std::vector<Card>> foundations; 
    // Wektor przechowujący historię stanów gry (do funkcji cofania).
    std::vector<GameState> history; 
    // Flaga wskazująca, czy gra się zakończyła (przegrana lub wygrana).
    bool gameOver;
    // Flaga wskazująca, czy gra została wygrana.
    bool gameWon;
    // Licznik wykonanych ruchów w bieżącej grze.
    int movesMade;
    // Generator liczb losowych Mersenne Twister do tasowania kart.
    std::mt19937 rng; 

    // Zapisuje bieżący stan gry do historii ruchów.
    void saveGameState() {
        // Ogranicza rozmiar historii, aby nie zużywać zbyt dużo pamięci.
        if (history.size() > 50) {
            history.erase(history.begin()); // Usuwa najstarszy stan.
        }
        // Tworzy nowy obiekt stanu gry.
        GameState currentState;
        // Kopiuje aktualny stan planszy i statystyk gracza.
        currentState.tableau = tableau;
        currentState.stock = stock;
        currentState.waste = waste;
        currentState.foundations = foundations;
        currentState.score = player.getScore();
        currentState.undosLeft = player.getUndosLeft();
        currentState.shufflesLeft = player.getShufflesLeft();
        currentState.revealsLeft = player.getRevealsLeft();
        currentState.jokerCardsLeft = player.getJokerCardsLeft();
        currentState.hintsLeft = player.getHintsLeft(); 
        currentState.movesMade = movesMade;
        // Dodaje bieżący stan na koniec historii.
        history.push_back(currentState);
    }

    // Odkrywa wierzchnią kartę w danej kolumnie tableau, jeśli jest zakryta.
    void revealTopTableauCard(int pileIndex) {
        // Sprawdza poprawność indeksu i czy stos nie jest pusty.
        if (pileIndex >= 0 && pileIndex < 7 && !tableau[pileIndex].empty()) {
            // Sprawdza, czy wierzchnia karta jest zakryta.
            if (!tableau[pileIndex].back().isFaceUp()) {
                // Odwraca kartę.
                tableau[pileIndex].back().flip();
                // Dodaje punkty za odkrycie karty.
                player.addScore(5); 
                // Odtwarza dźwięk odkrycia karty.
                ConsoleUtils::playSound(800, 50); 
            }
        }
    }

public:
    // Konstruktor klasy SolitaireGame.
    SolitaireGame() : gameOver(true), gameWon(false), movesMade(0), rng(static_cast<unsigned int>(std::time(nullptr))) {
        // Wczytuje dane gracza przy starcie.
        player.loadFromFile(); 
        // Inicjalizuje rozmiary wektorów dla fundamentów i tableau.
        foundations.resize(4);
        tableau.resize(7);
    }

    // Zwraca referencję do obiektu gracza.
    Player& getPlayer() { return player; }

    // Zwraca stałą referencję do stanu tableau.
    const std::vector<std::vector<Card>>& getTableau() const { return tableau; }
    // Zwraca stałą referencję do stanu stosu stock.
    const std::vector<Card>& getStock() const { return stock; }
    // Zwraca stałą referencję do stanu stosu waste.
    const std::vector<Card>& getWaste() const { return waste; }
    // Zwraca stałą referencję do stanu fundamentów.
    const std::vector<std::vector<Card>>& getFoundations() const { return foundations; }
    // Zwraca liczbę wykonanych ruchów.
    int getMovesMade() const { return movesMade; }
    // Zwraca true, jeśli gra jest zakończona.
    bool isGameOver() const { return gameOver; }
    // Zwraca true, jeśli gra została wygrana.
    bool isGameWon() const { return gameWon; }

    // Zwraca referencję do generatora liczb losowych (używane np. przy tasowaniu).
    std::mt19937& getRng() { return rng; } 

    // Rozpoczyna nową grę.
    void newGame() {
        // Czyści wszystkie stosy kart na planszy.
        for (auto& pile : tableau) pile.clear();
        stock.clear();
        waste.clear();
        for (auto& f : foundations) f.clear();
        // Czyści historię ruchów.
        history.clear(); 

        // Resetuje flagi stanu gry.
        gameOver = false;
        gameWon = false;
        // Resetuje licznik ruchów.
        movesMade = 0;
        // Resetuje wynik gracza.
        player.resetScore();
        // Resetuje liczniki wspomagaczy.
        player.resetPowerups(); 
        // Uruchamia stoper gry.
        player.startGameTimer();

        // Tworzy standardową talię 52 kart.
        std::vector<Card> deck;
        char suits[] = {'H', 'D', 'C', 'S'}; // Kolory kart.
        for (char suit : suits) {
            for (int value = 1; value <= 13; value++) { // Wartości kart.
                deck.emplace_back(suit, value); // Dodaje kartę do talii.
            }
        }

        // Tasuje talię kart za pomocą generatora liczb losowych.
        std::shuffle(deck.begin(), deck.end(), rng);

        // Rozdaje karty do kolumn tableau.
        int deckIndex = deck.size() - 1; // Indeks ostatniej karty w potasowanej talii.
        for (int i = 0; i < 7; i++) { // Iteruje przez kolumny tableau.
            for (int j = 0; j <= i; j++) { // Dodaje odpowiednią liczbę kart do kolumny.
                 // Sprawdza, czy talia się nie skończyła (nie powinno się zdarzyć w standardowym Klondike).
                 if(deckIndex < 0) {
                    // Rzuca wyjątek w razie problemu.
                    throw std::runtime_error("Deck ran out during tableau dealing!");
                 }
                 // Upewnia się, że wektor tableau ma wystarczający rozmiar (teoretycznie niepotrzebne po resize).
                 if (static_cast<size_t>(i) >= tableau.size()) tableau.resize(i + 1);
                // Dodaje kartę z talii do kolumny tableau.
                tableau[i].push_back(deck[deckIndex--]);
            }
             // Odkrywa wierzchnią kartę w każdej kolumnie tableau.
             if (!tableau[i].empty()) {
                 tableau[i].back().flip(); 
             }
        }

        // Przygotowuje stos kart zakrytych (stock) z pozostałych kart.
        // Sprawdza poprawność indeksu talii.
        if (deckIndex >= -1) { 
             // Zmienia rozmiar talii, aby zawierała tylko pozostałe karty.
             deck.resize(deckIndex + 1);
        } else {
             // Czyści talię, jeśli wszystkie karty zostały rozdane.
             deck.clear(); 
        }
        // Przypisuje pozostałe karty do stosu stock.
        stock = deck; 
        // Ustawia wszystkie karty w stosie stock jako zakryte.
        for(Card& card : stock) {
            card.setFaceUp(false); 
        }

        // Zapisuje początkowy stan gry do historii.
        saveGameState(); 
    }

    // Cofa ostatni ruch. Zwraca true, jeśli cofnięcie się powiodło.
    bool undoMove() {
        // Sprawdza, czy w historii jest co najmniej jeden ruch do cofnięcia (oprócz stanu początkowego).
        if (history.size() <= 1) { 
            // Odtwarza dźwięk błędu, jeśli nie można cofnąć.
            ConsoleUtils::playSound(200, 150); 
            return false; // Zwraca informację o niepowodzeniu.
        }

        // Próbuje użyć wspomagacza cofnięcia.
        if (!player.useUndo()) {
             // Odtwarza dźwięk błędu, jeśli brak dostępnych cofnięć.
             ConsoleUtils::playSound(200, 150); 
             return false; // Zwraca informację o niepowodzeniu.
        }

        // Usuwa bieżący stan z historii.
        history.pop_back(); 
        // Pobiera poprzedni stan gry z historii.
        const GameState& previousState = history.back();

        // Przywraca stan planszy z poprzedniego stanu.
        tableau = previousState.tableau;
        stock = previousState.stock;
        waste = previousState.waste;
        foundations = previousState.foundations;

        // Resetuje wynik gracza do zera przed przywróceniem.
        player.resetScore(); 
        // Przywraca wynik z poprzedniego stanu.
        player.addScore(previousState.score); 

        // Przywraca liczbę pozostałych wspomagaczy.
        player.setUndosLeft(previousState.undosLeft); // Uwaga: useUndo już zmniejszyło licznik, przywracamy poprzedni stan.
        player.setShufflesLeft(previousState.shufflesLeft);
        player.setRevealsLeft(previousState.revealsLeft);
        player.setJokerCardsLeft(previousState.jokerCardsLeft);
        player.setHintsLeft(previousState.hintsLeft); 
        // Przywraca liczbę ruchów.
        movesMade = previousState.movesMade;

        // Odtwarza dźwięk potwierdzający cofnięcie.
        ConsoleUtils::playSound(600, 80); 
        // Zwraca informację o powodzeniu.
        return true;
    }

    // Sprawdza, czy można przenieść kartę ze stosu waste na fundament.
    bool canMoveWasteToFoundation() {
        // Sprawdza, czy stos waste nie jest pusty.
        if (waste.empty()) return false;
        // Pobiera referencję do karty na wierzchu stosu waste.
        const Card& card = waste.back();
        // Inicjalizuje indeks docelowego fundamentu (-1 oznacza brak pasującego).
        int foundationIndex = -1;

        // Iteruje przez 4 fundamenty.
        for(int i=0; i<4; ++i) {
            // Jeśli fundament jest pusty.
            if (foundations[i].empty()) {
                 // Sprawdza, czy karta jest Asem (wartość 1).
                 if (card.getValue() == 1) {
                     // Sprawdza, czy ten kolor nie został już rozpoczęty na innym fundamencie.
                     bool suitStarted = false;
                     for (int j = 0; j < 4; ++j) {
                         if (i != j && !foundations[j].empty() && foundations[j][0].getSuit() == card.getSuit()) {
                             suitStarted = true;
                             break;
                         }
                     }
                     // Jeśli kolor nie jest rozpoczęty, ten fundament jest odpowiedni.
                     if (!suitStarted) {
                         foundationIndex = i;
                         break;
                     }
                 }
            } 
            // Jeśli fundament nie jest pusty i ma ten sam kolor co karta.
            else if (foundations[i].back().getSuit() == card.getSuit()) {
                // Ten fundament jest potencjalnym celem.
                foundationIndex = i;
                break;
            }
        }

        // Jeśli nie znaleziono pasującego fundamentu.
        if (foundationIndex == -1) return false;

        // Jeśli znaleziony fundament jest pusty.
        if (foundations[foundationIndex].empty()) {
            // Można przenieść tylko Asa.
            return card.getValue() == 1; 
        } 
        // Jeśli znaleziony fundament nie jest pusty.
        else {
            // Sprawdza, czy kolory się zgadzają i wartość karty jest o 1 większa niż karty na fundamencie.
            return foundations[foundationIndex].back().getSuit() == card.getSuit() &&
                   foundations[foundationIndex].back().getValue() == card.getValue() - 1;
        }
    }

    // Sprawdza, czy można przenieść kartę z wierzchu kolumny tableau na fundament.
    bool canMoveTableauToFoundation(int tableauIndex) {
        // Sprawdza poprawność indeksu tableau, czy stos nie jest pusty i czy karta jest odkryta.
        if (tableauIndex < 0 || tableauIndex >= 7 || tableau[tableauIndex].empty() || !tableau[tableauIndex].back().isFaceUp())
             return false;
        // Pobiera referencję do karty na wierzchu kolumny tableau.
        const Card& card = tableau[tableauIndex].back();
        // Inicjalizuje indeks docelowego fundamentu (-1 oznacza brak pasującego).
        int foundationIndex = -1;

        // Iteruje przez 4 fundamenty (logika identyczna jak w canMoveWasteToFoundation).
        for(int i=0; i<4; ++i) {
            if (foundations[i].empty()) {
                 if (card.getValue() == 1) {
                    bool suitStarted = false;
                     for (int j = 0; j < 4; ++j) {
                         if (i != j && !foundations[j].empty() && foundations[j][0].getSuit() == card.getSuit()) {
                             suitStarted = true;
                             break;
                         }
                     }
                     if (!suitStarted) {
                        foundationIndex = i;
                        break;
                     }
                 }
            } else if (!foundations[i].empty() && foundations[i].back().getSuit() == card.getSuit()) {
                foundationIndex = i;
                break;
            }
        }
        // Jeśli nie znaleziono pasującego fundamentu.
        if (foundationIndex == -1) return false;

        // Jeśli znaleziony fundament jest pusty.
        if (foundations[foundationIndex].empty()) {
            // Można przenieść tylko Asa.
            return card.getValue() == 1;
        } 
        // Jeśli znaleziony fundament nie jest pusty.
        else {
            // Sprawdza, czy kolory się zgadzają i wartość karty jest o 1 większa niż karty na fundamencie.
            return foundations[foundationIndex].back().getSuit() == card.getSuit() &&
                   foundations[foundationIndex].back().getValue() == card.getValue() - 1;
        }
    }

    // Sprawdza, czy można przenieść kartę ze stosu waste na kolumnę tableau.
    bool canMoveWasteToTableau(int tableauIndex) {
        // Sprawdza poprawność indeksu tableau i czy stos waste nie jest pusty.
        if (tableauIndex < 0 || tableauIndex >= 7 || waste.empty()) return false;
        // Pobiera referencję do karty na wierzchu stosu waste.
        const Card& cardToMove = waste.back();
        // Jeśli docelowa kolumna tableau jest pusta.
        if (tableau[tableauIndex].empty()) {
            // Można przenieść tylko Króla (wartość 13) lub Jokera.
            return cardToMove.getValue() == 13 || cardToMove.getIsJoker();
        } 
        // Jeśli docelowa kolumna tableau nie jest pusta.
        else {
            // Pobiera referencję do karty na wierzchu docelowej kolumny.
            const Card& topCard = tableau[tableauIndex].back();

             // Sprawdza, czy karta na wierzchu jest odkryta oraz czy:
             // 1. Wartość karty do przeniesienia jest o 1 mniejsza od karty na wierzchu ORAZ kolory są różne (czerwony/czarny).
             // 2. Karta do przeniesienia jest Jokerem.
             // 3. Karta na wierzchu docelowej kolumny jest Jokerem (Joker akceptuje każdą kartę spełniającą warunek koloru/wartości).
             return topCard.isFaceUp() &&
                   ( ( (topCard.getValue() == cardToMove.getValue() + 1) && 
                       (topCard.isRed() != cardToMove.isRed()) )           // Standardowy ruch Klondike.
                    || cardToMove.getIsJoker()                             // Przenoszona karta jest Jokerem.
                    || topCard.getIsJoker() );                             // Karta docelowa jest Jokerem.
        }
    }

    // Sprawdza, czy można przenieść kartę (lub grupę kart) z jednej kolumny tableau do drugiej.
    bool canMoveTableauToTableau(int fromIndex, int toIndex, int cardIndex) {
         // Sprawdza poprawność indeksów kolumn (muszą być różne i w zakresie 0-6).
         if (fromIndex < 0 || fromIndex >= 7 || toIndex < 0 || toIndex >= 7 || fromIndex == toIndex) return false;

         // Sprawdza, czy kolumna źródłowa nie jest pusta i czy indeks karty jest poprawny.
         if (tableau[fromIndex].empty() || cardIndex < 0 || static_cast<size_t>(cardIndex) >= tableau[fromIndex].size()) return false;

        // Pobiera referencję do pierwszej karty w przenoszonej grupie.
        const Card& cardToMove = tableau[fromIndex][cardIndex];

        // Sprawdza, czy przenoszona karta jest odkryta (wszystkie karty powyżej niej też muszą być odkryte w Klondike).
        if (!cardToMove.isFaceUp()) return false;

        // Jeśli docelowa kolumna tableau jest pusta.
        if (tableau[toIndex].empty()) {
            // Można przenieść tylko Króla (wartość 13) lub Jokera.
            return cardToMove.getValue() == 13 || cardToMove.getIsJoker();
        } 
        // Jeśli docelowa kolumna tableau nie jest pusta.
        else {
            // Pobiera referencję do karty na wierzchu docelowej kolumny.
            const Card& topCard = tableau[toIndex].back();

             // Sprawdza, czy karta na wierzchu jest odkryta oraz czy:
             // 1. Wartość karty do przeniesienia jest o 1 mniejsza od karty na wierzchu ORAZ kolory są różne.
             // 2. Karta do przeniesienia (pierwsza w grupie) jest Jokerem.
             // 3. Karta na wierzchu docelowej kolumny jest Jokerem.
             return topCard.isFaceUp() &&
                   ( ( (topCard.getValue() == cardToMove.getValue() + 1) && 
                       (topCard.isRed() != cardToMove.isRed()) )           // Standardowy ruch Klondike.
                     || cardToMove.getIsJoker()                            // Przenoszona karta jest Jokerem.
                     || topCard.getIsJoker() );                            // Karta docelowa jest Jokerem.
        }
    }

    // Sprawdza, czy można przenieść kartę z fundamentu z powrotem na kolumnę tableau.
    bool canMoveFoundationToTableau(int foundationIndex, int tableauIndex) {
        // Sprawdza poprawność indeksów fundamentu i tableau oraz czy fundament nie jest pusty.
        if (foundationIndex < 0 || foundationIndex >= 4 || tableauIndex < 0 || tableauIndex >= 7 || foundations[foundationIndex].empty())
            return false;
        // Pobiera referencję do karty na wierzchu fundamentu.
        const Card& cardToMove = foundations[foundationIndex].back();

        // Jeśli docelowa kolumna tableau jest pusta.
        if (tableau[tableauIndex].empty()) {
            // Można przenieść tylko Króla (wartość 13) lub Jokera (choć Joker na fundamencie jest nietypowy).
            return cardToMove.getValue() == 13 || cardToMove.getIsJoker();
        } 
        // Jeśli docelowa kolumna tableau nie jest pusta.
        else {
            // Pobiera referencję do karty na wierzchu docelowej kolumny.
            const Card& topCard = tableau[tableauIndex].back();

             // Sprawdza, czy karta na wierzchu jest odkryta oraz czy:
             // 1. Wartość karty do przeniesienia (z fundamentu) jest o 1 mniejsza od karty na wierzchu tableau ORAZ kolory są różne.
             // 2. Karta na wierzchu docelowej kolumny jest Jokerem (akceptuje każdą kartę spełniającą warunek koloru/wartości).
             // Karta z fundamentu zazwyczaj nie będzie Jokerem.
             return topCard.isFaceUp() &&
                   ( ( (topCard.getValue() == cardToMove.getValue() + 1) && 
                       (topCard.isRed() != cardToMove.isRed()) )           // Standardowy ruch Klondike.
                     || topCard.getIsJoker() );                            // Karta docelowa jest Jokerem.
        }
    }

    // Wykonuje akcję pociągnięcia karty ze stosu stock do waste, lub resetuje stos, jeśli stock jest pusty.
    bool dealFromStock() {
        // Zapisuje stan gry przed wykonaniem ruchu.
        saveGameState(); 
        // Jeśli stos stock jest pusty.
        if (stock.empty()) {
            // Jeśli stos waste również jest pusty, nie ma czego resetować.
            if (waste.empty()) {
                history.pop_back(); // Usuwa niepotrzebnie zapisany stan.
                ConsoleUtils::playSound(200, 150); // Dźwięk błędu/braku akcji.
                return false; // Zwraca informację, że nic się nie stało.
            }

            // Przenosi karty z waste z powrotem do stock.
            std::vector<Card> tempWaste = waste; // Kopiuje waste.
            waste.clear(); // Czyści waste.
            stock = tempWaste; // Przenosi skopiowane karty do stock.
            // Odwraca kolejność kart w nowym stosie stock.
            std::reverse(stock.begin(), stock.end());
            // Ustawia wszystkie karty w stock jako zakryte.
            for (Card& card : stock) {
                card.setFaceUp(false);
            }
            // Odejmuje punkty za zresetowanie stosu (jeśli wynik > 100).
            player.addScore(-100); 
            // Zwiększa licznik ruchów.
            movesMade++;
            // Odtwarza dźwięk resetowania stosu.
            ConsoleUtils::playSound(500, 100); 
            // Sprawdza, czy ten ruch (reset) nie prowadzi do wygranej (teoretycznie niemożliwe).
            checkWinCondition(); // To wywołanie jest prawdopodobnie nadmiarowe w tym miejscu.
            // Zwraca informację o powodzeniu resetu.
            return true;
        } 
        // Jeśli stos stock nie jest pusty.
        else {
            // Pobiera wierzchnią kartę ze stosu stock.
            Card card = stock.back();
            // Usuwa kartę ze stosu stock.
            stock.pop_back();
            // Odkrywa kartę.
            card.setFaceUp(true);
            // Dodaje odkrytą kartę na wierzch stosu waste.
            waste.push_back(card);
            // Zwiększa licznik ruchów.
            movesMade++;
            // Zgodnie z niektórymi zasadami, pociągnięcie karty nie daje punktów.
            player.addScore(0); 
            // Odtwarza dźwięk pociągnięcia karty.
            ConsoleUtils::playSound(700, 50); 
            // Sprawdza, czy ten ruch prowadzi do wygranej (np. odkrycie ostatniej potrzebnej karty).
            checkWinCondition(); // To wywołanie może być istotne.
            // Zwraca informację o powodzeniu pociągnięcia karty.
            return true;
        }
    }

    // Próbuje przenieść kartę z wierzchu stosu waste na odpowiedni fundament.
    bool moveWasteToFoundation() {
        // Sprawdza, czy ruch jest możliwy zgodnie z zasadami gry.
        if (!canMoveWasteToFoundation()) {
            // Jeśli ruch jest niemożliwy, zwraca false.
            return false;
        }
        // Zapisuje bieżący stan gry do historii przed wykonaniem ruchu.
        saveGameState();
        // Pobiera kartę z wierzchu stosu waste.
        Card card = waste.back();
        // Usuwa kartę ze stosu waste.
        waste.pop_back();

        // Znajduje indeks odpowiedniego fundamentu dla przenoszonej karty.
        int foundationIndex = -1;
        // Iteruje przez 4 fundamenty.
        for (int i = 0; i < 4; ++i) {
             // Sprawdza, czy fundament jest pusty i karta jest Asem, LUB czy fundament nie jest pusty i kolory się zgadzają.
             if ( (foundations[i].empty() && card.getValue() == 1) ||
                  (!foundations[i].empty() && foundations[i].back().getSuit() == card.getSuit()) )
            {
                // Sprawdza dodatkowo, czy ten kolor Asa nie został już rozpoczęty na innym fundamencie.
                bool suitPresentElsewhere = false;
                if (foundations[i].empty() && card.getValue() == 1) {
                    for (int j = 0; j < 4; ++j) {
                        if (i != j && !foundations[j].empty() && foundations[j][0].getSuit() == card.getSuit()) {
                            suitPresentElsewhere = true;
                            break;
                        }
                    }
                }
                // Jeśli kolor nie jest obecny gdzie indziej (lub warunek nie dotyczy Asa), to jest to prawidłowy fundament.
                if (!suitPresentElsewhere) {
                    foundationIndex = i;
                    break; // Przerywa pętlę po znalezieniu pasującego fundamentu.
                }
            }
        }

        // Jeśli znaleziono odpowiedni fundament.
        if (foundationIndex != -1) {
            // Dodaje kartę do znalezionego fundamentu.
            foundations[foundationIndex].push_back(card);
            // Dodaje punkty za przeniesienie karty na fundament.
            player.addScore(10); 
            // Zwiększa licznik ruchów.
            movesMade++;
            // Odtwarza dźwięk sukcesu.
            ConsoleUtils::playSound(900, 70); 
            // Sprawdza, czy gra została wygrana po tym ruchu.
            checkWinCondition();
            // Zwraca true, informując o powodzeniu ruchu.
            return true;
        } else {
            // Jeśli nie znaleziono odpowiedniego fundamentu (co nie powinno się zdarzyć po `canMoveWasteToFoundation`), cofa zapis stanu.
            history.pop_back(); 
            // Przywraca kartę na stos waste.
            waste.push_back(card); 
            // Odtwarza dźwięk błędu.
            ConsoleUtils::playSound(200, 150); 
            // Zwraca false, informując o niepowodzeniu (teoretycznie nieosiągalne).
            return false;
        }
    }

    // Próbuje przenieść kartę z wierzchu kolumny tableau na odpowiedni fundament.
    bool moveTableauToFoundation(int tableauIndex) {
        // Sprawdza, czy ruch jest możliwy zgodnie z zasadami gry.
        if (!canMoveTableauToFoundation(tableauIndex)) {
             // Jeśli ruch jest niemożliwy, zwraca false.
             return false;
        }
        // Zapisuje bieżący stan gry do historii.
        saveGameState();
        // Pobiera kartę z wierzchu wskazanej kolumny tableau.
        Card card = tableau[tableauIndex].back();
        // Usuwa kartę z kolumny tableau.
        tableau[tableauIndex].pop_back();

        // Znajduje indeks odpowiedniego fundamentu dla przenoszonej karty (logika identyczna jak w moveWasteToFoundation).
        int foundationIndex = -1;
         for (int i = 0; i < 4; ++i) {
            if ( (foundations[i].empty() && card.getValue() == 1) ||
                 (!foundations[i].empty() && foundations[i].back().getSuit() == card.getSuit()) )
            {
                 bool suitPresentElsewhere = false;
                if (foundations[i].empty() && card.getValue() == 1) {
                    for (int j = 0; j < 4; ++j) {
                        if (i != j && !foundations[j].empty() && foundations[j][0].getSuit() == card.getSuit()) {
                            suitPresentElsewhere = true;
                            break;
                        }
                    }
                }
                if (!suitPresentElsewhere) {
                    foundationIndex = i;
                    break;
                }
            }
        }

        // Jeśli znaleziono odpowiedni fundament.
        if (foundationIndex != -1) {
            // Dodaje kartę do znalezionego fundamentu.
            foundations[foundationIndex].push_back(card);
            // Odkrywa nową wierzchnią kartę w kolumnie tableau, jeśli była zakryta.
            revealTopTableauCard(tableauIndex); 
            // Dodaje punkty za przeniesienie karty na fundament.
            player.addScore(10); 
            // Zwiększa licznik ruchów.
            movesMade++;
            // Odtwarza dźwięk sukcesu.
            ConsoleUtils::playSound(900, 70); 
            // Sprawdza, czy gra została wygrana po tym ruchu.
            checkWinCondition();
            // Zwraca true, informując o powodzeniu ruchu.
            return true;
        } else {
            // Jeśli nie znaleziono odpowiedniego fundamentu (anomalia), cofa zapis stanu.
            history.pop_back(); 
            // Przywraca kartę do kolumny tableau.
            tableau[tableauIndex].push_back(card); 
            // Odtwarza dźwięk błędu.
            ConsoleUtils::playSound(200, 150); 
            // Zwraca false, informując o niepowodzeniu.
            return false;
        }
    }

    // Próbuje przenieść kartę z wierzchu stosu waste na wskazaną kolumnę tableau.
    bool moveWasteToTableau(int tableauIndex) {
        // Sprawdza, czy ruch jest możliwy zgodnie z zasadami gry.
        if (!canMoveWasteToTableau(tableauIndex)) {
             // Jeśli ruch jest niemożliwy, zwraca false.
             return false;
        }
        // Zapisuje bieżący stan gry do historii.
        saveGameState();
        // Pobiera kartę z wierzchu stosu waste.
        Card card = waste.back();
        // Usuwa kartę ze stosu waste.
        waste.pop_back();
        // Dodaje kartę do wskazanej kolumny tableau.
        tableau[tableauIndex].push_back(card);
        // Dodaje punkty za przeniesienie karty z waste do tableau.
        player.addScore(5); 
        // Zwiększa licznik ruchów.
        movesMade++;
        // Odtwarza dźwięk sukcesu.
        ConsoleUtils::playSound(800, 60); 
        // Sprawdza warunek zwycięstwa (mało prawdopodobne, ale dla spójności).
        checkWinCondition();
        // Zwraca true, informując o powodzeniu ruchu.
        return true;
    }

    // Próbuje przenieść kartę (lub grupę kart) z jednej kolumny tableau do drugiej.
    bool moveTableauToTableau(int fromIndex, int toIndex, int cardIndex) {
        // Sprawdza, czy ruch jest możliwy zgodnie z zasadami gry.
        if (!canMoveTableauToTableau(fromIndex, toIndex, cardIndex)) {
             // Jeśli ruch jest niemożliwy, zwraca false.
             return false;
        }
        // Zapisuje bieżący stan gry do historii.
        saveGameState();

        // Uzyskuje iterator do pierwszej przenoszonej karty w kolumnie źródłowej.
        auto itStart = tableau[fromIndex].begin() + cardIndex;
        // Uzyskuje iterator do końca kolumny źródłowej.
        auto itEnd = tableau[fromIndex].end();

        // Wstawia przenoszone karty na koniec kolumny docelowej, używając iteratorów przenoszących dla efektywności.
        tableau[toIndex].insert(tableau[toIndex].end(), std::make_move_iterator(itStart), std::make_move_iterator(itEnd));

        // Usuwa przeniesione karty z kolumny źródłowej.
        tableau[fromIndex].erase(tableau[fromIndex].begin() + cardIndex, tableau[fromIndex].end());

        // Odkrywa nową wierzchnią kartę w kolumnie źródłowej, jeśli była zakryta.
        revealTopTableauCard(fromIndex); 
        // Dodaje punkty za przeniesienie kart między kolumnami tableau.
        player.addScore(3); 
        // Zwiększa licznik ruchów.
        movesMade++;
        // Odtwarza dźwięk sukcesu.
        ConsoleUtils::playSound(800, 60); 
        // Sprawdza warunek zwycięstwa.
        checkWinCondition();
        // Zwraca true, informując o powodzeniu ruchu.
        return true;
    }

    // Próbuje przenieść kartę z wierzchu fundamentu z powrotem na kolumnę tableau.
    bool moveFoundationToTableau(int foundationIndex, int tableauIndex) {
        // Sprawdza, czy ruch jest możliwy zgodnie z zasadami gry.
        if (!canMoveFoundationToTableau(foundationIndex, tableauIndex)) {
            // Jeśli ruch jest niemożliwy, zwraca false.
            return false;
        }
        // Zapisuje bieżący stan gry do historii.
        saveGameState();
        // Pobiera kartę z wierzchu wskazanego fundamentu.
        Card card = foundations[foundationIndex].back();
        // Usuwa kartę z fundamentu.
        foundations[foundationIndex].pop_back();
        // Dodaje kartę do wskazanej kolumny tableau.
        tableau[tableauIndex].push_back(card);
        // Odejmuje punkty za przeniesienie karty z fundamentu (niekorzystny ruch).
        player.addScore(-15); 
        // Zwiększa licznik ruchów.
        movesMade++;
        // Odtwarza dźwięk (inny niż standardowy ruch).
        ConsoleUtils::playSound(400, 100); 
        // Sprawdza warunek zwycięstwa (ruch cofa postęp, więc raczej nie wygra).
        checkWinCondition(); 
        // Zwraca true, informując o powodzeniu ruchu.
        return true;
    }

    // Używa wspomagacza przetasowania stosu stock i waste.
    bool useShufflePowerup() {
        // Sprawdza, czy gracz ma dostępne przetasowania i czy wspomagacze są włączone.
        if (!player.getShufflesLeft() || !player.getPowerupsEnabled()) {
            // Zwraca false, jeśli nie można użyć przetasowania.
            return false;
        }

        // Tworzy tymczasowy wektor na karty do przetasowania.
        std::vector<Card> combinedDeck;

        // Dodaje karty ze stosu stock.
        combinedDeck.insert(combinedDeck.end(), stock.begin(), stock.end());
        // Dodaje karty ze stosu waste.
        combinedDeck.insert(combinedDeck.end(), waste.begin(), waste.end());
        
        // Jeśli nie ma kart do przetasowania, zwraca false.
        if (combinedDeck.empty()) {
            return false;
        }

        // Tasuje połączone karty.
        std::shuffle(combinedDeck.begin(), combinedDeck.end(), rng);

        // Zapisuje stan gry przed modyfikacją.
        saveGameState();

        // Zastępuje stos stock przetasowanymi kartami.
        stock = combinedDeck;
        // Czyści stos waste.
        waste.clear();
        // Wszystkie karty w stock powinny być zakryte po przetasowaniu.
        for(auto& card : stock) { card.setFaceUp(false); }

        // Rejestruje użycie wspomagacza (odejmuje punkty i zmniejsza licznik).
        bool success = player.useShuffle();
        // Zwiększa licznik ruchów.
        movesMade++;
        
        // Zwraca wynik operacji użycia wspomagacza.
        return success;
    }

    // Używa wspomagacza odkrycia karty w tableau.
    bool useRevealPowerup() {
        // Sprawdza, czy gracz ma dostępne odkrycia i czy wspomagacze są włączone.
        if (!player.getRevealsLeft() || !player.getPowerupsEnabled()) {
            // Zwraca false, jeśli nie można użyć odkrycia.
            return false;
        }

        // Flaga wskazująca, czy jakakolwiek karta została odkryta.
        bool anyRevealed = false;

        // Zapisuje stan gry przed modyfikacją.
        saveGameState();

        // Iteruje przez kolumny tableau.
        for (auto& pile : tableau) {
            // Iteruje od dołu kolumny (od wierzchu stosu wizualnie).
            for (auto it = pile.rbegin(); it != pile.rend(); ++it) {
                // Jeśli karta jest zakryta.
                if (!it->isFaceUp()) {
                    // Odkrywa kartę.
                    it->setFaceUp(true);
                    // Ustawia flagę, że dokonano odkrycia.
                    anyRevealed = true;
                    // Przerywa pętlę dla tej kolumny (odkrywa tylko jedną kartę na użycie).
                    break;  
                }
            }
             // Jeśli odkryto kartę w tej kolumnie, można by przerwać główną pętlę, jeśli chcemy odkrywać tylko jedną kartę globalnie na użycie.
             // Obecna implementacja potencjalnie odkrywa po jednej karcie w każdej kolumnie, która ma zakryte karty.
             // Dla standardowego działania (jedna karta per użycie) należałoby dodać `if (anyRevealed) break;` tutaj.
        }
        
        // Jeśli żadna karta nie została odkryta (np. wszystkie są już odkryte).
        if (!anyRevealed) {
            // W tej wersji implementacji nie zwracamy wspomagacza, ale można by to dodać:
            // player.refundReveal(); // Przykładowe zwrócenie zasobu.
            // Zwraca false, informując o braku efektu.
            return false;
        }

        // Rejestruje użycie wspomagacza.
        bool success = player.useReveal();
        // Zwiększa licznik ruchów.
        movesMade++;
        
        // Zwraca wynik operacji użycia wspomagacza.
        return success;
    }
    
    // Używa wspomagacza dodania Jokera do wskazanej kolumny tableau.
    bool useJokerPowerup(int tableauIndex) {
        // Sprawdza, czy gracz ma dostępne Jokery, czy wspomagacze są włączone i czy indeks tableau jest poprawny.
        if (!player.getJokerCardsLeft() || !player.getPowerupsEnabled() || 
            tableauIndex < 0 || tableauIndex >= static_cast<int>(tableau.size())) {
            // Zwraca false, jeśli nie można użyć Jokera.
            return false;
        }
        
        // Sprawdza dodatkowy warunek: czy można umieścić Jokera na tej kolumnie
        // (w tej implementacji Joker może być umieszczony zawsze, ale można by dodać logikę, np. tylko na pustą kolumnę lub na inną kartę)
        /*
        if (!tableau[tableauIndex].empty()) {
             const Card& topCard = tableau[tableauIndex].back();
             // Przykładowy warunek: Joker może być umieszczony tylko na karcie nie będącej Jokerem.
             if (topCard.getIsJoker()) {
                 // player.refundJokerCard(); // Opcjonalnie zwróć Jokera
                 return false;
             }
        }
        */

        // Zapisuje stan gry przed modyfikacją.
        saveGameState();

        // Tworzy kartę Jokera ('X' jako symbol koloru, wartość 1, flaga isJoker=true).
        Card joker('X', 1, true);  
        // Ustawia Jokera jako odkrytego.
        joker.setFaceUp(true);

        // Dodaje Jokera na wierzch wskazanej kolumny tableau.
        tableau[tableauIndex].push_back(joker);

        // Rejestruje użycie wspomagacza.
        bool success = player.useJokerCard();
        // Zwiększa licznik ruchów.
        movesMade++;
        
        // Zwraca wynik operacji użycia wspomagacza.
        return success;
    }

    // Sprawdza, czy warunki zwycięstwa zostały spełnione.
    void checkWinCondition() {
         // Jeśli gra już się zakończyła, nie sprawdza ponownie.
         if(gameOver) return; 

        // Licznik kart na fundamentach.
        int cardsInFoundations = 0;
        // Flaga wskazująca, czy wszystkie fundamenty są kompletne (doszły do Króla).
        bool allFoundationsComplete = true;
        // Iteruje przez fundamenty.
        for (const auto& foundation : foundations) {
            // Sprawdza, czy fundament nie jest pusty i czy na wierzchu jest Król (wartość 13).
            if (!foundation.empty() && foundation.back().getValue() == 13) { 
                 // Dodaje liczbę kart w kompletnym fundamencie.
                 cardsInFoundations += foundation.size();
            } else {
                 // Jeśli którykolwiek fundament nie jest kompletny, ustawia flagę na false.
                 allFoundationsComplete = false; 
                 // Przerywa pętlę, bo warunek zwycięstwa nie jest spełniony.
                 break;
            }
        }

         // Warunek zwycięstwa: wszystkie fundamenty są kompletne ORAZ suma kart na nich wynosi 52.
         if (allFoundationsComplete && cardsInFoundations == 52) {
             // Ustawia flagi końca gry i zwycięstwa.
             gameOver = true;
             gameWon = true;
             // Zatrzymuje stoper gry.
             player.stopGameTimer();

             // Dodaje dużą premię punktową za wygraną.
             player.addScore(1000);

             // Oblicza premię czasową (im szybciej, tym więcej punktów).
             double timeBonus = std::max(0.0, 600.0 - player.getElapsedSeconds()) * 2.0; // Mnożnik punktów za czas.
             // Dodaje premię czasową do wyniku.
             player.addScore(static_cast<int>(timeBonus));
             // Aktualizuje najwyższy wynik, jeśli obecny jest lepszy.
             player.updateHighScore();
             // Zapisuje dane gracza (w tym nowy rekord).
             player.saveToFile(); 
         } else {
             // Jeśli warunek nie jest spełniony, upewnia się, że flaga wygranej jest false.
             gameWon = false;
         }
    }

    // Sprawdza, czy istnieją jeszcze jakiekolwiek możliwe ruchy na planszy.
    bool hasValidMovesLeft() const {
        // Jeśli można pociągnąć kartę ze stosu stock, istnieje ruch.
        if (!stock.empty()) return true;

        // Jeśli stos stock jest pusty, ale można go zresetować (waste nie jest pusty), istnieje ruch.
        if (stock.empty() && !waste.empty()) return true; // Ten ruch to kliknięcie na pusty stock.

        // Sprawdza możliwość przeniesienia karty z waste na fundament. Używa const_cast, bo metody can* nie są const.
        if (const_cast<SolitaireGame*>(this)->canMoveWasteToFoundation()) return true;
        // Sprawdza możliwość przeniesienia karty z waste na każdą kolumnę tableau.
        for (int i = 0; i < 7; ++i) {
            if (const_cast<SolitaireGame*>(this)->canMoveWasteToTableau(i)) return true;
        }

        // Iteruje przez kolumny tableau.
        for (int i = 0; i < 7; ++i) {
            // Sprawdza możliwość przeniesienia karty z tableau na fundament.
            if (const_cast<SolitaireGame*>(this)->canMoveTableauToFoundation(i)) return true;

             // Jeśli kolumna tableau nie jest pusta.
             if (!tableau[i].empty()) {
                 // Iteruje przez karty w kolumnie (od dołu do góry).
                 for (size_t cardIndex = 0; cardIndex < tableau[i].size(); ++cardIndex) {
                     // Jeśli karta jest odkryta (pierwsza odkryta karta od dołu lub wyższa).
                     if (tableau[i][cardIndex].isFaceUp()) {
                         // Sprawdza możliwość przeniesienia tej karty (i tych nad nią) do innej kolumny tableau.
                         for (int j = 0; j < 7; ++j) {
                             // Pomija przenoszenie do tej samej kolumny.
                             if (i == j) continue; 
                             // Sprawdza, czy ruch jest możliwy.
                             if (const_cast<SolitaireGame*>(this)->canMoveTableauToTableau(i, j, static_cast<int>(cardIndex))) {
                                 // Jeśli tak, istnieje możliwy ruch.
                                 return true;
                             }
                         }
                         // Po sprawdzeniu pierwszej odkrytej karty, przerywa wewnętrzną pętlę, bo tylko stos od tej karty można przenieść.
                         break; // Ważne: sprawdzamy tylko ruch całego stosu odkrytych kart.
                     }
                 }
             }
        }

        // Sprawdza możliwość przeniesienia karty z fundamentu z powrotem do tableau.
        for (int i = 0; i < 4; ++i) { // Iteruje przez fundamenty.
            for (int j = 0; j < 7; ++j) { // Iteruje przez kolumny tableau.
                if (const_cast<SolitaireGame*>(this)->canMoveFoundationToTableau(i, j)) return true;
            }
        }

        // Jeśli żadna z powyższych możliwości nie istnieje, nie ma już dostępnych ruchów.
        return false;
    }

    // Sprawdza, czy gra powinna się zakończyć z powodu braku ruchów.
    void checkForGameOver() {
        // Sprawdza tylko, jeśli gra nie jest już zakończona i nie została wygrana.
        if (!gameOver && !gameWon && !hasValidMovesLeft()) {
             // Sprawdza, czy użycie wspomagaczy mogłoby potencjalnie odblokować grę.
             bool powerupsMightHelp =
                 // Przetasowanie może pomóc, jeśli jest dostępne i są karty w stock/waste.
                 (player.getPowerupsEnabled() && player.getShufflesLeft() > 0 && (!stock.empty() || !waste.empty())) 
                 // Odkrycie może pomóc, jeśli jest dostępne i są zakryte karty w tableau.
                 || (player.getPowerupsEnabled() && player.getRevealsLeft() > 0 && std::any_of(tableau.begin(), tableau.end(), [](const auto& pile){ 
                      return std::any_of(pile.begin(), pile.end(), [](const Card& c){ return !c.isFaceUp(); });
                   }))
                 // Joker może pomóc, jeśli jest dostępny.
                 || (player.getPowerupsEnabled() && player.getJokerCardsLeft() > 0)
                 // Podpowiedź sama w sobie nie odblokuje gry, ale może wskazać ruch, jeśli istnieje. Tutaj warunek jest nadmiarowy, bo `hasValidMovesLeft` już to sprawdziło.
                 || (player.getPowerupsEnabled() && player.getHintsLeft() > 0); 

             // Jeśli nie ma możliwych ruchów i wspomagacze nie mogą pomóc.
             if (!powerupsMightHelp) { 
                // Ustawia flagę końca gry (przegrana).
                gameOver = true;
                gameWon = false;
                // Zatrzymuje stoper.
                player.stopGameTimer();
                // Aktualizuje (ewentualnie) rekord.
                player.updateHighScore(); 
                // Zapisuje stan gracza.
                player.saveToFile();
             }
             // Jeśli wspomagacze mogą pomóc, gra się nie kończy automatycznie. Gracz musi zdecydować o ich użyciu.
        }
    }

    // Znajduje i zwraca możliwą podpowiedź ruchu.
    std::optional<Hint> getHint() {
        // Jeśli wspomagacze są wyłączone, nie daje podpowiedzi.
        if (!player.getPowerupsEnabled()) {
            return std::nullopt; // Zwraca pusty optional.
        }
        
        // Jeśli gracz nie ma już podpowiedzi.
        if (!player.getHintsLeft()) {
            return std::nullopt; // Zwraca pusty optional.
        }
        
        // Rejestruje użycie podpowiedzi (odejmuje punkty, zmniejsza licznik).
        // Zwrócenie podpowiedzi nastąpi, jeśli jakaś zostanie znaleziona.
        if (!player.useHint()) { // Dodatkowe sprawdzenie, czy useHint się powiodło.
             return std::nullopt;
        }

        // ---- Priorytety podpowiedzi ----
        // 1. Przeniesienie karty z Tableau na Fundament.
        for (int i = 0; i < 7; ++i) {
            if (canMoveTableauToFoundation(i)) {
                 int cardIndex = tableau[i].size() - 1; // Indeks karty to ostatnia karta.
                 int targetFoundation = -1; // Inicjalizacja indeksu docelowego fundamentu.
                 const Card& card = tableau[i].back(); // Karta do przeniesienia.
                 // Znajdź odpowiedni fundament (logika podobna do `moveTableauToFoundation`).
                 for(int f=0; f<4; ++f) { 
                     if ( (foundations[f].empty() && card.getValue() == 1) ||
                          (!foundations[f].empty() && foundations[f].back().getSuit() == card.getSuit()))
                     {
                         // Sprawdź, czy As nie jest już na innym fundamencie.
                         bool suitPresentElsewhere = false;
                         if (foundations[f].empty() && card.getValue() == 1) {
                             for(int j=0; j<4; ++j) if (f != j && !foundations[j].empty() && foundations[j][0].getSuit() == card.getSuit()) suitPresentElsewhere = true;
                         }
                         if (!suitPresentElsewhere) { targetFoundation = f; break; }
                     }
                 }
                 // Jeśli znaleziono fundament, zwróć podpowiedź.
                 if (targetFoundation != -1) {
                     return Hint{
                         {SelectableType::TABLEAU_CARD, i, cardIndex}, // Źródło: karta w tableau.
                         {SelectableType::FOUNDATION, targetFoundation, -1}, // Cel: fundament.
                         // Opis podpowiedzi.
                         L"Move " + card.toStringShort() + L" from Tableau " + std::to_wstring(i+1) + L" to Foundation."
                     };
                 }
            }
        }

        // 2. Przeniesienie karty z Waste na Fundament.
        if (canMoveWasteToFoundation()) {
            const Card& card = waste.back(); // Karta do przeniesienia.
            int targetFoundation = -1; // Inicjalizacja indeksu docelowego fundamentu.
            // Znajdź odpowiedni fundament (logika podobna do `moveWasteToFoundation`).
             for(int f=0; f<4; ++f) { 
                 if ( (foundations[f].empty() && card.getValue() == 1) ||
                      (!foundations[f].empty() && foundations[f].back().getSuit() == card.getSuit()))
                 {
                    // Sprawdź, czy As nie jest już na innym fundamencie.
                    bool suitPresentElsewhere = false;
                    if (foundations[f].empty() && card.getValue() == 1) {
                        for(int j=0; j<4; ++j) if (f != j && !foundations[j].empty() && foundations[j][0].getSuit() == card.getSuit()) suitPresentElsewhere = true;
                    }
                    if (!suitPresentElsewhere) { targetFoundation = f; break; }
                 }
             }
             // Jeśli znaleziono fundament, zwróć podpowiedź.
             if (targetFoundation != -1) {
                 return Hint{
                     {SelectableType::WASTE, 0, -1}, // Źródło: waste (indeks 0 jako placeholder).
                     {SelectableType::FOUNDATION, targetFoundation, -1}, // Cel: fundament.
                     // Opis podpowiedzi.
                     L"Move " + card.toStringShort() + L" from Waste to Foundation."
                 };
             }
        }

        // 3. Przeniesienie stosu z Tableau do Tableau, które odkrywa nową kartę.
        for (int from = 0; from < 7; ++from) {
             // Sprawdza, czy jest co najmniej jedna karta zakryta pod przenoszonym stosem.
             if (tableau[from].size() > 1) { // Musi być co najmniej 2 karty, aby móc odkryć.
                 // Znajdź pierwszą odkrytą kartę (od dołu).
                 for (int cardIdx = 0; cardIdx < (int)tableau[from].size(); ++cardIdx) {
                     if (tableau[from][cardIdx].isFaceUp()) { 
                         // Sprawdź, czy pod nią jest zakryta karta.
                         if (cardIdx > 0 && !tableau[from][cardIdx - 1].isFaceUp()) { 
                            // Sprawdź, czy można przenieść ten stos do innej kolumny.
                            for (int to = 0; to < 7; ++to) {
                                 if (from == to) continue; // Pomija tę samą kolumnę.
                                 if (canMoveTableauToTableau(from, to, cardIdx)) {
                                      // Zwraca podpowiedź o ruchu odkrywającym kartę.
                                      return Hint{
                                         {SelectableType::TABLEAU_CARD, from, cardIdx}, // Źródło: pierwsza odkryta karta stosu.
                                         {SelectableType::TABLEAU_PILE, to, -1}, // Cel: docelowa kolumna tableau.
                                         // Opis podpowiedzi.
                                         L"Move stack from Tableau " + std::to_wstring(from+1) + L" to Tableau " + std::to_wstring(to+1) + L" (reveals card)."
                                     };
                                 }
                            }
                         }
                        // Po znalezieniu pierwszej odkrytej karty, nie trzeba sprawdzać dalej w tej kolumnie dla tego priorytetu.
                        break; 
                     }
                 }
             }
        }

        // 4. Przeniesienie stosu z Tableau do Tableau (inne przypadki, np. przeniesienie króla na puste pole).
         for (int from = 0; from < 7; ++from) {
            if (!tableau[from].empty()) {
                // Znajdź pierwszą odkrytą kartę (od dołu).
                for (int cardIdx = 0; cardIdx < (int)tableau[from].size(); ++cardIdx) {
                     if (tableau[from][cardIdx].isFaceUp()) {
                        // Sprawdź, czy można przenieść ten stos do innej kolumny.
                        for (int to = 0; to < 7; ++to) {
                             if (from == to) continue; // Pomija tę samą kolumnę.
                             if (canMoveTableauToTableau(from, to, cardIdx)) {
                                // Sprawdza, czy ten ruch nie został już zasugerowany w priorytecie 3 (odkrywanie karty).
                                bool reveals = (cardIdx > 0 && !tableau[from][cardIdx-1].isFaceUp());
                                if (!reveals) { // Jeśli ruch nie odkrywa karty.
                                     // Zwraca podpowiedź.
                                     return Hint{
                                         {SelectableType::TABLEAU_CARD, from, cardIdx}, // Źródło: pierwsza odkryta karta stosu.
                                         {SelectableType::TABLEAU_PILE, to, -1}, // Cel: docelowa kolumna tableau.
                                         // Opis podpowiedzi.
                                         L"Move stack from Tableau " + std::to_wstring(from+1) + L" to Tableau " + std::to_wstring(to+1) + L"."
                                     };
                                }
                             }
                        }
                        // Po znalezieniu pierwszej odkrytej karty, przerywa dla tej kolumny.
                        break; 
                     }
                 }
             }
         }

        // 5. Przeniesienie karty z Waste do Tableau.
        for (int i = 0; i < 7; ++i) {
            if (canMoveWasteToTableau(i)) {
                const Card& card = waste.back(); // Karta do przeniesienia.
                // Zwraca podpowiedź.
                return Hint{
                    {SelectableType::WASTE, 0, -1}, // Źródło: waste.
                    {SelectableType::TABLEAU_PILE, i, -1}, // Cel: kolumna tableau.
                    // Opis podpowiedzi.
                    L"Move " + card.toStringShort() + L" from Waste to Tableau " + std::to_wstring(i+1) + L"."
                };
            }
        }

        // 6. Przeniesienie karty z Fundamentu z powrotem do Tableau (tylko jeśli inne ruchy nie są dostępne).
        for (int f = 0; f < 4; ++f) {
            for (int t = 0; t < 7; ++t) {
                if (canMoveFoundationToTableau(f, t)) {
                    const Card& card = foundations[f].back(); // Karta do przeniesienia.
                    // Zwraca podpowiedź.
                     return Hint{
                        {SelectableType::FOUNDATION, f, -1}, // Źródło: fundament.
                        {SelectableType::TABLEAU_PILE, t, -1}, // Cel: kolumna tableau.
                        // Opis podpowiedzi.
                        L"Move " + card.toStringShort() + L" from Foundation back to Tableau " + std::to_wstring(t+1) + L"."
                    };
                }
            }
        }

        // 7. Pociągnięcie karty ze Stock, jeśli nie jest pusty.
        if (!stock.empty()) {
            // Zwraca podpowiedź o pociągnięciu karty.
            return Hint{
                {SelectableType::STOCK, 0, -1}, // Źródło: stock.
                {SelectableType::WASTE, 0, -1}, // Cel: waste (symbolicznie).
                // Opis podpowiedzi.
                L"Draw card from Stock."
            };
        }

        // 8. Zresetowanie stosu Stock z Waste, jeśli Stock jest pusty, a Waste nie.
         if (stock.empty() && !waste.empty()) {
             // Zwraca podpowiedź o zresetowaniu stosu.
             return Hint{
                {SelectableType::STOCK, 0, -1}, // Źródło: stock (kliknięcie na pusty stock).
                {SelectableType::STOCK, 0, -1}, // Cel: stock (symbolicznie).
                // Opis podpowiedzi.
                L"Recycle Waste pile back to Stock."
            };
         }
         
        // Jeśli żadna podpowiedź nie została znaleziona, zwraca punkty i licznik.
        player.refundHint();

        // Zwraca pusty optional, jeśli żadna podpowiedź nie została znaleziona.
        return std::nullopt;
    }

}; // Koniec klasy SolitaireGame


// Klasa odpowiedzialna za interfejs użytkownika w konsoli.
class ConsoleUI {
private:
    // Referencja do obiektu gry (logiki).
    SolitaireGame& game;
    // Referencja do obiektu gracza (dla łatwiejszego dostępu).
    Player& player;
    // Aktualne zaznaczenie gracza na planszy.
    Selection currentSelection; 
    // Zaznaczenie źródłowe (używane podczas przeciągania lub dwukliku).
    Selection sourceSelection;  
    // Wiadomość wyświetlana w pasku statusu.
    std::wstring statusMessage;  
    // Czas, do którego wiadomość w pasku statusu ma być widoczna.
    std::chrono::time_point<std::chrono::steady_clock> messageExpireTime; 

    // Stałe współrzędne (lewy górny róg) dla różnych elementów planszy.
    // Pozycja stosu kart zakrytych (stock).
    const COORD STOCK_POS = {5, 3};
    // Pozycja stosu kart odkrytych (waste).
    const COORD WASTE_POS = {12, 3};
    // Pozycje pól bazowych (fundamentów).
    const COORD FOUNDATION_POS[4] = {{30, 3}, {37, 3}, {44, 3}, {51, 3}};
    // Pozycje kolumn tableau.
    const COORD TABLEAU_POS[7] = {{5, 8}, {15, 8}, {25, 8}, {35, 8}, {45, 8}, {55, 8}, {65, 8}};
    // Pozycja wyświetlania wyniku.
    const COORD SCORE_POS = {75, 3};
    // Pozycja wyświetlania czasu gry.
    const COORD TIME_POS = {75, 4};
    // Pozycja wyświetlania liczby ruchów.
    const COORD MOVES_POS = {75, 5};
    // Pozycja wyświetlania najwyższego wyniku.
    const COORD HIGHSCORE_POS = {75, 6};
    // Pozycja wyświetlania informacji o wspomagaczach.
    const COORD POWERUP_POS = {75, 8}; 
    // Pozycja paska statusu na dole ekranu.
    const COORD STATUS_POS = {0, CONSOLE_HEIGHT - 2}; 
    // Pozycja linii pomocy na dole ekranu.
    const COORD HELP_POS = {0, CONSOLE_HEIGHT - 1};   

    // Szerokość wizualna karty w znakach.
    const int CARD_WIDTH = 5;
    // Wysokość wizualna karty w znakach.
    const int CARD_HEIGHT = 3;

    // Zwraca współrzędne lewego górnego rogu dla danego zaznaczenia.
    COORD getCoordsForSelection(const Selection& selection) const {
        // Wybiera odpowiednią stałą pozycję na podstawie typu zaznaczenia.
        switch (selection.type) {
        case SelectableType::STOCK: return STOCK_POS; // Zwraca pozycję stosu stock.
        case SelectableType::WASTE: return WASTE_POS; // Zwraca pozycję stosu waste.
        case SelectableType::FOUNDATION:
            // Sprawdza poprawność indeksu fundamentu.
            if (selection.index >= 0 && selection.index < 4) return FOUNDATION_POS[selection.index]; // Zwraca pozycję odpowiedniego fundamentu.
            break; // Jeśli indeks niepoprawny, przechodzi dalej.
        case SelectableType::TABLEAU_PILE: 
             // Sprawdza poprawność indeksu kolumny tableau.
             if (selection.index >= 0 && selection.index < 7) {
                 // Zwraca pozycję bazową odpowiedniej kolumny tableau.
                 return TABLEAU_POS[selection.index];
             }
             break; // Jeśli indeks niepoprawny, przechodzi dalej.
        case SelectableType::TABLEAU_CARD:
             // Sprawdza poprawność indeksu kolumny tableau.
             if (selection.index >= 0 && selection.index < 7) {
                 // Pobiera pozycję bazową kolumny.
                 COORD basePos = TABLEAU_POS[selection.index];
                 // Pobiera referencję do kolumny kart.
                 const auto& pile = game.getTableau()[selection.index];
                 // Sprawdza, czy indeks karty jest poprawny.
                 if (selection.cardIndex != -1 && static_cast<size_t>(selection.cardIndex) < pile.size()) {
                     // Oblicza przesunięcie w pionie dla zaznaczonej karty.
                     int offsetY = 0;
                     // Iteruje przez karty powyżej zaznaczonej karty.
                     for (int i = 0; i < selection.cardIndex; ++i) {
                         // Dodaje większe przesunięcie dla kart odkrytych, mniejsze dla zakrytych.
                         offsetY += (pile[i].isFaceUp() ? 2 : 1); 
                         // Ogranicza przesunięcie, aby karty nie wychodziły poza ekran.
                         if (basePos.Y + offsetY > CONSOLE_HEIGHT - CARD_HEIGHT - 3) { // -3 dla marginesu dolnego.
                             offsetY = CONSOLE_HEIGHT - CARD_HEIGHT - 3 - basePos.Y;
                             break; // Przerywa obliczanie przesunięcia.
                         }
                     }
                     // Zwraca pozycję bazową z obliczonym przesunięciem Y.
                     return {basePos.X, static_cast<SHORT>(basePos.Y + offsetY)};
                 } 
                 // Jeśli indeks karty jest niepoprawny (np. -1), ale stos nie jest pusty (zaznaczenie ostatniej karty).
                 else if (!pile.empty()) {
                     // Oblicza przesunięcie dla ostatniej karty (podobne jak wyżej).
                     int lastCardIndex = pile.size() - 1;
                     int offsetY = 0;
                      for (int i = 0; i < lastCardIndex; ++i) {
                          offsetY += (pile[i].isFaceUp() ? 2 : 1);
                           if (basePos.Y + offsetY > CONSOLE_HEIGHT - CARD_HEIGHT - 3) {
                             offsetY = CONSOLE_HEIGHT - CARD_HEIGHT - 3 - basePos.Y;
                             break;
                         }
                      }
                     // Zwraca pozycję ostatniej karty.
                     return {basePos.X, static_cast<SHORT>(basePos.Y + offsetY)};
                 } else {
                      // Jeśli stos jest pusty, a zaznaczono kartę (anomalia), zwraca pozycję bazową.
                      return basePos;
                 }
             }
             break; // Jeśli indeks kolumny niepoprawny, przechodzi dalej.
        default: break; // Dla SelectableType::NONE lub nieobsługiwanych.
        }
        // Zwraca nieprawidłowe współrzędne w razie błędu.
        return {-1, -1}; 
    }

    // Rysuje pojedynczą kartę na podanych współrzędnych.
    void drawCard(int x, int y, const Card& card, bool highlighted, bool hintHighlight = false) {
        // Sprawdza, czy współrzędne i wymiary karty mieszczą się w granicach konsoli.
        if (x < 0 || y < 0 || x + CARD_WIDTH > CONSOLE_WIDTH || y + CARD_HEIGHT > CONSOLE_HEIGHT) {
            return; // Nie rysuje karty poza ekranem.
        }

        // Domyślne atrybuty dla ramki i tła.
        WORD borderAttribute = COLOR_BORDER; // Domyślny kolor ramki.
        WORD backgroundAttribute = COLOR_DEFAULT; // Domyślny kolor tła.
        WORD cardAttribute = card.getColor(); // Pobiera kolor tekstu karty (zależny od koloru i stanu).

        // Modyfikuje atrybuty, jeśli karta ma być podświetlona jako podpowiedź.
        if (hintHighlight) {
            borderAttribute = COLOR_HINT_HIGHLIGHT; // Ramka w kolorze podpowiedzi.
            backgroundAttribute = COLOR_HINT_HIGHLIGHT; // Tło w kolorze podpowiedzi.
            // Dostosowuje kolor tekstu karty, zachowując kolor podpowiedzi jako tło.
             if (card.isRed())
                cardAttribute = COLOR_CARD_RED | (COLOR_HINT_HIGHLIGHT & 0xFFF0); // Czerwona karta na tle podpowiedzi.
             else
                cardAttribute = COLOR_CARD_BLACK | (COLOR_HINT_HIGHLIGHT & 0xFFF0); // Czarna karta na tle podpowiedzi.
             if (!card.isFaceUp())
                 cardAttribute = COLOR_CARD_BACK | (COLOR_HINT_HIGHLIGHT & 0xFFF0); // Rewers na tle podpowiedzi.
             if (card.getIsJoker())
                 cardAttribute = (NEO_FOREGROUND_MAGENTA | NEO_FOREGROUND_INTENSITY) | (COLOR_HINT_HIGHLIGHT & 0xFFF0); // Joker na tle podpowiedzi.
        } 
        // Modyfikuje atrybuty, jeśli karta ma być podświetlona jako zaznaczenie.
        else if (highlighted) {
             borderAttribute = COLOR_HIGHLIGHTS; // Ramka w kolorze zaznaczenia.
             backgroundAttribute = COLOR_HIGHLIGHTS; // Tło w kolorze zaznaczenia.
             // Dostosowuje kolor tekstu karty, zachowując kolor zaznaczenia jako tło.
             if (card.isRed())
                cardAttribute = COLOR_CARD_RED | NEO_BACKGROUND_YELLOW; // Czerwona karta na tle zaznaczenia.
             else
                cardAttribute = COLOR_CARD_BLACK | NEO_BACKGROUND_YELLOW; // Czarna karta na tle zaznaczenia.
             if (!card.isFaceUp())
                 cardAttribute = COLOR_CARD_BACK | NEO_BACKGROUND_YELLOW; // Rewers na tle zaznaczenia.
             if (card.getIsJoker())
                 cardAttribute = (NEO_FOREGROUND_MAGENTA | NEO_FOREGROUND_INTENSITY) | NEO_BACKGROUND_YELLOW; // Joker na tle zaznaczenia.
        }

        // Rysuje ramkę karty.
        ConsoleUtils::setChar(x, y, L'┌', borderAttribute); // Lewy górny róg.
        ConsoleUtils::setChar(x + CARD_WIDTH - 1, y, L'┐', borderAttribute); // Prawy górny róg.
        ConsoleUtils::setChar(x, y + CARD_HEIGHT - 1, L'└', borderAttribute); // Lewy dolny róg.
        ConsoleUtils::setChar(x + CARD_WIDTH - 1, y + CARD_HEIGHT - 1, L'┘', borderAttribute); // Prawy dolny róg.
        // Rysuje linie poziome ramki.
        for (int i = 1; i < CARD_WIDTH - 1; ++i) {
            ConsoleUtils::setChar(x + i, y, L'─', borderAttribute);
            ConsoleUtils::setChar(x + i, y + CARD_HEIGHT - 1, L'─', borderAttribute);
        }
        // Rysuje linie pionowe ramki (tylko środkowa część).
        ConsoleUtils::setChar(x, y + 1, L'│', borderAttribute);
        ConsoleUtils::setChar(x + CARD_WIDTH - 1, y + 1, L'│', borderAttribute);

        // Wypełnia środek karty.
        wchar_t fillChar = L' '; // Domyślnie spacja.
        WORD fillAttribute = backgroundAttribute; // Domyślnie atrybut tła.
        // Jeśli karta jest zakryta (i nie jest Jokerem), używa symbolu rewersu.
        if (!card.isFaceUp() && !card.getIsJoker()) {
            fillChar = L'░'; // Symbol cieniowania dla rewersu.
            // Ustawia odpowiedni atrybut dla rewersu, uwzględniając podświetlenie.
            fillAttribute = hintHighlight ? (COLOR_CARD_BACK | (COLOR_HINT_HIGHLIGHT & 0xFFF0))
                           : highlighted ? (COLOR_CARD_BACK | NEO_BACKGROUND_YELLOW)
                           : COLOR_CARD_BACK; // Standardowy kolor rewersu.
        }
        // Wypełnia środkową linię karty.
        for (int i = 1; i < CARD_WIDTH - 1; ++i) {
            ConsoleUtils::setChar(x + i, y + 1, fillChar, fillAttribute);
        }

        // Jeśli karta jest odkryta lub jest Jokerem, rysuje jej wartość i kolor.
        if (card.isFaceUp() || card.getIsJoker()) {
            // Pobiera stringi reprezentujące wartość i kolor.
            auto drawStrings = card.getDrawStrings();
            std::wstring valueString = drawStrings.first;
            std::wstring suitString = drawStrings.second;

            // Łączy stringi wartości i koloru.
            std::wstring displayString = valueString + suitString;
            // Oblicza dopełnienie, aby wyśrodkować tekst wewnątrz karty.
            int padLeft = std::max(0, (CARD_WIDTH - 2 - (int)displayString.length()) / 2); // -2 na ramkę.

            // Wypisuje wartość i kolor karty z odpowiednim atrybutem.
            ConsoleUtils::writeStringWide(x + 1 + padLeft, y + 1, displayString, cardAttribute);
        }
    }

    // Rysuje symbol pustego miejsca na stos/pole.
    void drawEmptyPile(int x, int y, bool highlighted, const std::wstring& symbol = L"[ ]", bool hintHighlight = false) {
        // Wybiera atrybut w zależności od podświetlenia.
        WORD attribute = COLOR_CARD_EMPTY; // Domyślny kolor pustego miejsca.
        if(hintHighlight) attribute = COLOR_HINT_HIGHLIGHT; // Kolor podpowiedzi.
        else if(highlighted) attribute = COLOR_HIGHLIGHTS; // Kolor zaznaczenia.

        // Rysuje ramkę pustego miejsca.
        ConsoleUtils::drawBox(x, y, CARD_WIDTH, CARD_HEIGHT, attribute);

        // Oblicza dopełnienie, aby wyśrodkować symbol.
        int padLeft = std::max(0, (CARD_WIDTH - 2 - (int)symbol.length()) / 2); // -2 na ramkę.
        // Wypisuje symbol wewnątrz ramki.
        ConsoleUtils::writeStringWide(x + 1 + padLeft, y + 1, symbol, attribute);
    }

     // Rysuje całą kolumnę tableau z odpowiednim przesunięciem pionowym kart.
     void drawPileWithOffset(int x, int y, const std::vector<Card>& pile,
                           const Selection& currentSelection,
                           const std::optional<Hint>& hint)
    {
        // Określa indeks kolumny na podstawie współrzędnej X (może być niedokładne, jeśli pozycje się zmienią).
        int pileIndex = -1; 
        for (int i = 0; i < 7; ++i) if (x == TABLEAU_POS[i].X) pileIndex = i; // Zakłada unikalne X dla każdej kolumny.

        // Sprawdza, czy cała kolumna (jako puste miejsce) jest zaznaczona.
        bool isBaseSelected = currentSelection.type == SelectableType::TABLEAU_PILE && currentSelection.index == pileIndex;
        // Sprawdza, czy ta kolumna jest celem podpowiedzi.
        bool isHintDestinationBase = hint && hint->destination.type == SelectableType::TABLEAU_PILE && hint->destination.index == pileIndex;

        // Jeśli kolumna jest pusta.
        if (pile.empty()) {
            // Rysuje symbol pustego miejsca, uwzględniając zaznaczenie/podpowiedź.
            drawEmptyPile(x, y, isBaseSelected, L"[ ]", isHintDestinationBase);
            // Czyści obszar poniżej pustego miejsca (usuwa pozostałości po kartach).
            ConsoleUtils::fillRectangle(x, y + CARD_HEIGHT, CARD_WIDTH, CONSOLE_HEIGHT - y - CARD_HEIGHT - 2, L' ');
            return; // Kończy rysowanie tej kolumny.
        }

        // Bieżąca współrzędna Y do rysowania kolejnych kart.
        int currentY = y;
        // Iteruje przez karty w kolumnie.
        for (size_t i = 0; i < pile.size(); ++i) {
            // Sprawdza, czy bieżąca karta jest zaznaczona.
            bool isCardSelected = currentSelection.type == SelectableType::TABLEAU_CARD &&
                                  currentSelection.index == pileIndex &&
                                  currentSelection.cardIndex == static_cast<int>(i);

            // Sprawdza, czy bieżąca karta jest źródłem podpowiedzi.
            bool isHintSourceCard = hint && hint->source.type == SelectableType::TABLEAU_CARD &&
                                     hint->source.index == pileIndex && hint->source.cardIndex == static_cast<int>(i);
            // Sprawdza, czy bieżąca karta jest celem podpowiedzi (mniej typowe dla tableau).
             bool isHintDestinationCard = hint && hint->destination.type == SelectableType::TABLEAU_CARD && 
                                     hint->destination.index == pileIndex && hint->destination.cardIndex == static_cast<int>(i);

             // Łączy flagi podpowiedzi dla danej karty.
             bool cardHintHighlight = isHintSourceCard || isHintDestinationCard;

            // Sprawdza, czy rysowanie kolejnej karty nie wyjdzie poza ekran.
            if (currentY + CARD_HEIGHT >= CONSOLE_HEIGHT - 1) { // -1 dla dolnej ramki.
                // Jeśli tak, rysuje wielokropek i przerywa rysowanie kolumny.
                ConsoleUtils::writeString(x + 1, currentY, "...", COLOR_LABEL); 
                break;
            }

            // Rysuje bieżącą kartę, uwzględniając zaznaczenie/podpowiedź.
            drawCard(x, currentY, pile[i], isCardSelected, cardHintHighlight);

            // Jeśli to nie jest ostatnia karta w kolumnie, oblicza przesunięcie dla następnej.
            if (i + 1 < pile.size()) {
                 // Większe przesunięcie, jeśli bieżąca karta jest odkryta.
                 if (pile[i].isFaceUp()) {
                    currentY += 2; // Kaskada dla odkrytych kart.
                 } else {
                    currentY += 1; // Mniejsze przesunięcie dla zakrytych kart.
                 }
            }
        }
         // Czyści resztę kolumny poniżej ostatniej narysowanej karty.
         int clearY = currentY + CARD_HEIGHT;
         if (clearY < CONSOLE_HEIGHT - 1) {
             ConsoleUtils::fillRectangle(x, clearY, CARD_WIDTH, CONSOLE_HEIGHT - clearY - 2, L' ');
         }
    }

    // Wykonuje animację przesuwania karty z punktu startowego do końcowego.
    void animateCardMove(const Card& card, COORD startCoord, COORD endCoord) {
        // Jeśli animacje są wyłączone w konfiguracji, natychmiast kończy.
        if (!ENABLE_ANIMATION) {
            // Można by tu dodać natychmiastowe odświeżenie planszy bez animacji.
            return;
        }
        
        // Odtwarza krótki dźwięk ruchu karty.
        ConsoleUtils::playSound(700, 50); 

        // Tworzy kopię karty do animacji (zawsze odkrytą).
        Card animatedCard = card; 
        animatedCard.setFaceUp(true); // Animowana karta jest zawsze widoczna.

        // Oblicza wektor przesunięcia na krok animacji.
        float deltaX = static_cast<float>(endCoord.X - startCoord.X) / MOVE_ANIMATION_STEPS;
        float deltaY = static_cast<float>(endCoord.Y - startCoord.Y) / MOVE_ANIMATION_STEPS;
        // Inicjalizuje bieżącą pozycję (jako float dla płynności).
        float currentX = static_cast<float>(startCoord.X);
        float currentY = static_cast<float>(startCoord.Y);

        // Zapamiętuje ostatnią pozycję, w której narysowano kartę, aby unikać przerysowywania w tej samej komórce.
        COORD lastDrawnPos = {-1, -1}; 

        // Definiuje obszar ekranu, który zostanie dotknięty przez animację (bounding box).
        SMALL_RECT affectedArea;
        affectedArea.Left   = std::min(startCoord.X, endCoord.X) - 1; // Z marginesem.
        affectedArea.Top    = std::min(startCoord.Y, endCoord.Y) - 1;
        affectedArea.Right  = std::max(startCoord.X, endCoord.X) + CARD_WIDTH + 1; // Z marginesem i szerokością karty.
        affectedArea.Bottom = std::max(startCoord.Y, endCoord.Y) + CARD_HEIGHT + 1; // Z marginesem i wysokością karty.

        // Ogranicza obszar do granic konsoli.
        affectedArea.Left = std::max((SHORT)0, affectedArea.Left);
        affectedArea.Top = std::max((SHORT)0, affectedArea.Top);
        affectedArea.Right = std::min((SHORT)(CONSOLE_WIDTH - 1), affectedArea.Right);
        affectedArea.Bottom = std::min((SHORT)(CONSOLE_HEIGHT - 1), affectedArea.Bottom);

        // Oblicza rozmiar bufora dla dotkniętego obszaru.
        COORD bufferSize = { (SHORT)(affectedArea.Right - affectedArea.Left + 1), (SHORT)(affectedArea.Bottom - affectedArea.Top + 1) };
        // Bufor do przechowywania oryginalnej zawartości dotkniętego obszaru.
        CHAR_INFO* backgroundBuffer = nullptr;

        // Jeśli obszar jest prawidłowy.
        if(bufferSize.X > 0 && bufferSize.Y > 0) {
            // Alokuje pamięć na bufor tła.
            backgroundBuffer = new CHAR_INFO[bufferSize.X * bufferSize.Y];
            // Odczytuje oryginalną zawartość konsoli z dotkniętego obszaru.
            ReadConsoleOutputW(ConsoleUtils::hConsole, backgroundBuffer, bufferSize, {0,0}, &affectedArea);
        }

        // Definicja "śladu" (motion blur) - nieużywane w tej wersji, ale przygotowane.
        const int trailLength = 0; // Wyłączony ślad.
        COORD trailPositions[trailLength]; // Tablica na pozycje śladu.
        for (int i = 0; i < trailLength; i++) {
            trailPositions[i] = {-1, -1}; // Inicjalizacja pozycji śladu.
        }

        // Pętla główna animacji (od 0 do MOVE_ANIMATION_STEPS kroków).
        for (int i = 0; i <= MOVE_ANIMATION_STEPS; ++i) {
            // Oblicza zaokrągloną pozycję karty w bieżącym kroku.
            COORD pos = {static_cast<SHORT>(std::round(currentX)), static_cast<SHORT>(std::round(currentY))};

            // Sprawdza, czy pozycja się zmieniła od ostatniego rysowania lub czy to pierwszy/ostatni krok.
            if (i == 0 || i == MOVE_ANIMATION_STEPS || pos.X != lastDrawnPos.X || pos.Y != lastDrawnPos.Y) {
                // Przywraca oryginalne tło w dotkniętym obszarze.
                if (backgroundBuffer) {
                    // Zapisuje zawartość bufora tła z powrotem do konsoli.
                    WriteConsoleOutputW(ConsoleUtils::hConsole, backgroundBuffer, bufferSize, {0,0}, &affectedArea);
                } else {
                    // Alternatywnie, jeśli bufor tła zawiódł, przerysowuje całą planszę (mniej wydajne).
                    drawBoardContent(std::nullopt); // Przekazuje std::nullopt, bo nie ma aktywnej podpowiedzi podczas animacji.
                }

                // Rysowanie śladu (obecnie wyłączone z trailLength = 0).
                for (int t = trailLength - 1; t > 0; --t) {
                    if (trailPositions[t].X != -1) { // Jeśli pozycja śladu jest ważna.
                        // Wybiera kolor "zanikającego" śladu.
                        WORD fadeAttribute = (t == 1) ? 
                            (NEO_BACKGROUND_BLUE | NEO_FOREGROUND_INTENSITY) : // Jaśniejszy ślad bliżej karty.
                            (NEO_BACKGROUND_BLACK | NEO_FOREGROUND_BLUE | NEO_FOREGROUND_INTENSITY); // Ciemniejszy dalej.
                        // Rysuje uproszczoną ramkę śladu.
                        ConsoleUtils::setChar(trailPositions[t].X, trailPositions[t].Y, L'┌', fadeAttribute);
                        ConsoleUtils::setChar(trailPositions[t].X + CARD_WIDTH - 1, trailPositions[t].Y, L'┐', fadeAttribute);
                        ConsoleUtils::setChar(trailPositions[t].X, trailPositions[t].Y + CARD_HEIGHT - 1, L'└', fadeAttribute);
                        ConsoleUtils::setChar(trailPositions[t].X + CARD_WIDTH - 1, trailPositions[t].Y + CARD_HEIGHT - 1, L'┘', fadeAttribute);
                    }
                }

                // Przesuwa pozycje w buforze śladu.
                for (int t = trailLength - 1; t > 0; --t) {
                    trailPositions[t] = trailPositions[t-1];
                }
                // Zapamiętuje ostatnią pozycję karty jako nowy element śladu.
                if (trailLength > 0) trailPositions[0] = lastDrawnPos;

                // Rysuje animowaną kartę w bieżącej pozycji (bez podświetlenia).
                drawCard(pos.X, pos.Y, animatedCard, false); 

                // Renderuje zmiany na ekranie.
                ConsoleUtils::renderBuffer();
                // Zapamiętuje pozycję, w której ostatnio narysowano kartę.
                lastDrawnPos = pos;
            }

            // Aktualizuje bieżącą pozycję float na następny krok.
            currentX += deltaX;
            currentY += deltaY;

            // Czeka przez krótki czas przed następną klatką animacji.
            ConsoleUtils::sleep(ANIMATION_DELAY_MS);
        }

        // Zwalnia pamięć bufora tła, jeśli został zaalokowany.
        if (backgroundBuffer) {
            delete[] backgroundBuffer;
        }
        // Po zakończeniu animacji, plansza zostanie przerysowana w głównym cyklu.
    }
    
    // Funkcja do rysowania zawartości planszy (wywołuje inne funkcje rysujące).
    // Przyjmuje opcjonalną podpowiedź do podświetlenia.
    void drawBoardContent(const std::optional<Hint>& hint) {
        // Rysuje statyczne elementy (ramki, etykiety).
        drawBoardStaticElements();
        // Rysuje stos stock.
        drawStock(hint);
        // Rysuje stos waste.
        drawWaste(hint);
        // Rysuje fundamenty.
        drawFoundations(hint);
        // Rysuje kolumny tableau.
        drawTableau(hint);
        // Rysuje statystyki gracza.
        drawStats();
        // Rysuje pasek statusu (wiadomości).
        drawStatusBar();
        // Rysuje linię pomocy.
        drawHelpLine();
    }

    // Wykonuje animację użycia wspomagacza (np. "Shuffle!", "Undo!").
    void animatePowerUp(int x, int y, const std::wstring& powerUpText) {
        // Jeśli animacje są wyłączone, kończy.
        if (!ENABLE_ANIMATION) {
            return;
        }

        // Przygotowuje bufor do zapisania całego ekranu.
        SMALL_RECT fullScreenArea = {0, 0, CONSOLE_WIDTH - 1, CONSOLE_HEIGHT - 1}; // Obszar całego ekranu.
        COORD fullBufferSize = {CONSOLE_WIDTH, CONSOLE_HEIGHT}; // Rozmiar bufora.
        CHAR_INFO* fullScreenBuffer = new CHAR_INFO[CONSOLE_WIDTH * CONSOLE_HEIGHT]; // Alokacja bufora.
        // Odczytuje aktualną zawartość całego ekranu konsoli.
        ReadConsoleOutputW(ConsoleUtils::hConsole, fullScreenBuffer, fullBufferSize, {0, 0}, &fullScreenArea);

        // Odtwarza dźwięk aktywacji wspomagacza.
        ConsoleUtils::playSound(900, 100);

        // Animacja "rozbłysku" - powiększająca się ramka.
        for (int step = 0; step < POWER_ANIMATION_STEPS; step++) {
            // Rozmiar ramki rośnie z każdym krokiem.
            int size = step + 1;
            // Zmieniający się kolor ramki.
            WORD attributeColor;
            switch (step % 4) { // Cykl 4 kolorów tła.
                case 0: attributeColor = NEO_BACKGROUND_GREEN | NEO_FOREGROUND_WHITE | NEO_FOREGROUND_INTENSITY; break;
                case 1: attributeColor = NEO_BACKGROUND_BLUE | NEO_FOREGROUND_WHITE | NEO_FOREGROUND_INTENSITY; break;
                case 2: attributeColor = NEO_BACKGROUND_RED | NEO_FOREGROUND_WHITE | NEO_FOREGROUND_INTENSITY; break;
                case 3: attributeColor = NEO_BACKGROUND_MAGENTA | NEO_FOREGROUND_WHITE | NEO_FOREGROUND_INTENSITY; break;
                default: attributeColor = NEO_BACKGROUND_YELLOW | NEO_FOREGROUND_BLACK; break; // Fallback.
            }

            // Przerysowuje całą planszę (aby animacja była na wierzchu).
            drawBoardContent(std::nullopt); // Brak podpowiedzi podczas tej animacji.
            // Rysuje powiększającą się ramkę wokół tekstu.
            ConsoleUtils::drawBox(x - size, y - size, powerUpText.length() + 2 + size*2, 1 + size*2, attributeColor);
            // Wypisuje tekst wspomagacza w środku ramki.
            ConsoleUtils::writeStringWide(x, y, powerUpText, attributeColor);

            // Renderuje klatkę animacji.
            ConsoleUtils::renderBuffer();
            // Czeka chwilę.
            ConsoleUtils::sleep(POWERUP_ANIMATION_DELAY);
        }

        // Pauza po osiągnięciu pełnego rozmiaru.
        ConsoleUtils::sleep(500);

        // Animacja "zanikania" - zmniejszająca się ramka.
        const int fadeSteps = 5; // Liczba kroków zanikania.
        for (int step = fadeSteps; step >= 0; step--) {
            // Przerysowuje tło (całą planszę).
            drawBoardContent(std::nullopt);

            // Rysuje ramkę tylko jeśli krok > 0.
            if (step > 0) {
                // Oblicza stosunek zanikania.
                float fadeRatio = static_cast<float>(step) / fadeSteps;
                // Zmniejsza rozmiar ramki.
                int size = std::max(1, static_cast<int>(POWER_ANIMATION_STEPS * fadeRatio));
                // Zmienia kolor na bardziej stonowany podczas zanikania.
                WORD attributeColor = NEO_BACKGROUND_BLUE | NEO_FOREGROUND_WHITE; // Niebieskie tło.
                if (step <= 2) { // Ostatnie kroki.
                    attributeColor = NEO_BACKGROUND_BLACK | NEO_FOREGROUND_BLUE; // Ciemnoniebieski tekst na czarnym tle.
                }
                // Rysuje zmniejszającą się ramkę.
                ConsoleUtils::drawBox(x - size, y - size, powerUpText.length() + 2 + size*2, 1 + size*2, attributeColor);
                // Wypisuje tekst wspomagacza.
                ConsoleUtils::writeStringWide(x, y, powerUpText, attributeColor);
            }

            // Renderuje klatkę animacji zanikania.
            ConsoleUtils::renderBuffer();
            // Czeka dłużej podczas zanikania.
            ConsoleUtils::sleep(100);
        }

        // Na koniec, przerysowuje planszę do stanu oryginalnego (usuwa ostatnie ślady animacji).
        // WriteConsoleOutputW(ConsoleUtils::hConsole, fullScreenBuffer, fullBufferSize, {0, 0}, &fullScreenArea); // Przywraca zapisany ekran - alternatywa.
        drawBoardContent(std::nullopt); // Przerysowuje normalną planszę.
        ConsoleUtils::renderBuffer(); // Renderuje ostateczny stan.
        
        // Zwalnia pamięć bufora ekranu.
        delete[] fullScreenBuffer;
    }

    // Rysuje statyczne elementy interfejsu, takie jak ramki i etykiety.
    void drawBoardStaticElements() {
        // Rysuje główną, podwójną ramkę wokół całego ekranu.
        ConsoleUtils::drawBox(0, 0, CONSOLE_WIDTH, CONSOLE_HEIGHT, COLOR_BORDER, true); 

        // Wypisuje tytuł gry na środku górnej ramki.
        std::string title = " NeoSolitaire ";
        ConsoleUtils::writeString((CONSOLE_WIDTH - title.length()) / 2, 0, title, COLOR_TITLE | NEO_BACKGROUND_BLUE); // Tytuł na tle ramki.

        // Wypisuje etykiety dla głównych obszarów gry.
        ConsoleUtils::writeString(STOCK_POS.X, STOCK_POS.Y - 1, "Stock", COLOR_LABEL); // Etykieta Stock.
        ConsoleUtils::writeString(WASTE_POS.X, WASTE_POS.Y - 1, "Waste", COLOR_LABEL); // Etykieta Waste.
        ConsoleUtils::writeString(FOUNDATION_POS[0].X, FOUNDATION_POS[0].Y - 1, "Foundations", COLOR_LABEL); // Etykieta Fundamentów.
        ConsoleUtils::writeString(TABLEAU_POS[0].X, TABLEAU_POS[0].Y - 1, "Tableau", COLOR_LABEL); // Etykieta Tableau.
        ConsoleUtils::writeString(SCORE_POS.X, SCORE_POS.Y - 1, "Stats", COLOR_LABEL); // Etykieta Statystyk.

        // Rysuje numery kolumn tableau (1-7).
        for (int i = 0; i < 7; ++i) {
            // Wyśrodkowuje numer nad kolumną.
            ConsoleUtils::writeString(TABLEAU_POS[i].X + CARD_WIDTH / 2, TABLEAU_POS[i].Y - 1, std::to_string(i + 1), COLOR_LABEL);
        }
    }

    // Rysuje stos kart zakrytych (stock).
    void drawStock(const std::optional<Hint>& hint) {
        // Sprawdza, czy stock jest aktualnie zaznaczony.
        bool isSelected = currentSelection.type == SelectableType::STOCK;
        // Sprawdza, czy stock jest źródłem podpowiedzi.
        bool isHintSource = hint && hint->source.type == SelectableType::STOCK;
        // Sprawdza, czy stock jest celem podpowiedzi (np. resetowanie).
        bool isHintDest = hint && hint->destination.type == SelectableType::STOCK; 

        // Jeśli stos stock jest pusty.
        if (game.getStock().empty()) {
             // Wybiera symbol: "[ ]" jeśli waste też jest pusty, "[R]" (Recycle) jeśli można zresetować.
             std::wstring symbol = game.getWaste().empty() ? L"[ ]" : L"[R]";
             // Rysuje puste miejsce z odpowiednim symbolem i podświetleniem.
             drawEmptyPile(STOCK_POS.X, STOCK_POS.Y, isSelected, symbol, isHintSource || isHintDest);
        } else {
            // Jeśli stock nie jest pusty, rysuje symbol zakrytej karty.
            Card backCard(' ', 0); // Tworzy tymczasową kartę reprezentującą rewers.
            backCard.setFaceUp(false); // Upewnia się, że jest zakryta.
            // Rysuje kartę (rewers) z uwzględnieniem zaznaczenia/podpowiedzi.
            drawCard(STOCK_POS.X, STOCK_POS.Y, backCard, isSelected, isHintSource);

            // Wyświetla liczbę kart pozostałych w stosie stock poniżej karty.
            std::string countString = std::to_string(game.getStock().size()); // Pobiera liczbę kart jako string.
            // Czyści obszar pod kartą przed wypisaniem liczby.
            ConsoleUtils::fillRectangle(STOCK_POS.X, STOCK_POS.Y + CARD_HEIGHT, CARD_WIDTH, 1, L' '); 
            // Wypisuje liczbę kart, wyśrodkowaną pod kartą.
            ConsoleUtils::writeString(STOCK_POS.X + (CARD_WIDTH - countString.length()) / 2, STOCK_POS.Y + CARD_HEIGHT, countString, COLOR_VALUE);
        }
    }

    // Rysuje stos kart odkrytych (waste).
    void drawWaste(const std::optional<Hint>& hint) {
        // Sprawdza, czy waste jest aktualnie zaznaczony.
        bool isSelected = currentSelection.type == SelectableType::WASTE;
         // Sprawdza, czy waste jest źródłem podpowiedzi.
         bool isHintSource = hint && hint->source.type == SelectableType::WASTE;
         // Sprawdza, czy waste jest celem podpowiedzi (mniej typowe, może dla pociągnięcia karty).
         bool isHintDest = hint && hint->destination.type == SelectableType::WASTE; 

        // Jeśli stos waste jest pusty.
        if (game.getWaste().empty()) {
            // Rysuje symbol pustego miejsca.
            drawEmptyPile(WASTE_POS.X, WASTE_POS.Y, isSelected, L"[ ]", isHintDest);
            // Czyści znak "<" wskazujący na więcej kart (na lewo od waste).
            ConsoleUtils::fillRectangle(WASTE_POS.X - 1, WASTE_POS.Y + 1, 1, 1, L' ');
        } else {
            // Jeśli waste nie jest pusty, rysuje wierzchnią kartę.
            drawCard(WASTE_POS.X, WASTE_POS.Y, game.getWaste().back(), isSelected, isHintSource || isHintDest);

            // Jeśli w stosie waste jest więcej niż jedna karta, rysuje wskaźnik "<".
             if (game.getWaste().size() > 1) {
                // Wskazuje, że pod widoczną kartą są inne (chociaż nie są one bezpośrednio dostępne).
                ConsoleUtils::setChar(WASTE_POS.X - 1, WASTE_POS.Y + 1, L'<', COLOR_BORDER); 
             } else {
                 // Czyści miejsce na wskaźnik, jeśli jest tylko jedna karta.
                 ConsoleUtils::fillRectangle(WASTE_POS.X - 1, WASTE_POS.Y + 1, 1, 1, L' ');
             }
        }
    }

    // Rysuje pola bazowe (fundamenty).
    void drawFoundations(const std::optional<Hint>& hint) {
        // Iteruje przez 4 fundamenty.
        for (int i = 0; i < 4; ++i) {
            // Sprawdza, czy bieżący fundament jest zaznaczony.
            bool isSelected = currentSelection.type == SelectableType::FOUNDATION && currentSelection.index == i;
             // Sprawdza, czy bieżący fundament jest źródłem podpowiedzi.
             bool isHintSource = hint && hint->source.type == SelectableType::FOUNDATION && hint->source.index == i;
             // Sprawdza, czy bieżący fundament jest celem podpowiedzi.
             bool isHintDest = hint && hint->destination.type == SelectableType::FOUNDATION && hint->destination.index == i;

            // Jeśli fundament jest pusty.
            if (game.getFoundations()[i].empty()) {
                // Określa symbol koloru dla pustego fundamentu (orientacyjnie).
                std::wstring suitSymbol;
                WORD color = COLOR_CARD_EMPTY; // Domyślny kolor symbolu.
                 // Przypisuje symbol i kolor na podstawie indeksu (umownie).
                 switch(i) { 
                     case 0: suitSymbol = L"♥"; color = COLOR_CARD_RED; break; // Kier.
                     case 1: suitSymbol = L"♦"; color = COLOR_CARD_RED; break; // Karo.
                     case 2: suitSymbol = L"♣"; color = COLOR_CARD_BLACK; break; // Trefl.
                     case 3: suitSymbol = L"♠"; color = COLOR_CARD_BLACK; break; // Pik.
                 }
                 // Rysuje puste miejsce z ramką i symbolem koloru w środku (bezpośrednio).
                 drawEmptyPile(FOUNDATION_POS[i].X, FOUNDATION_POS[i].Y, isSelected, L" " + suitSymbol, isHintDest); // Ramka i tło.
                 // Rysuje sam symbol koloru w odpowiednim kolorze, nakładając na tło.
                 ConsoleUtils::setChar(FOUNDATION_POS[i].X + 1, FOUNDATION_POS[i].Y + 1, suitSymbol[0],
                                       isSelected ? COLOR_HIGHLIGHTS : (isHintDest ? COLOR_HINT_HIGHLIGHT : color));
            } else {
                // Jeśli fundament nie jest pusty, rysuje wierzchnią kartę.
                drawCard(FOUNDATION_POS[i].X, FOUNDATION_POS[i].Y, game.getFoundations()[i].back(), isSelected, isHintSource || isHintDest);
            }
        }
    }

    // Rysuje wszystkie kolumny tableau.
    void drawTableau(const std::optional<Hint>& hint) {
        // Iteruje przez 7 kolumn tableau.
        for (int i = 0; i < 7; ++i) {
            // Wywołuje funkcję pomocniczą do narysowania pojedynczej kolumny z przesunięciem.
            drawPileWithOffset(TABLEAU_POS[i].X, TABLEAU_POS[i].Y, game.getTableau()[i], currentSelection, hint);
        }
    }

    // Rysuje panel ze statystykami gry i informacjami o wspomagaczach.
    void drawStats() {
        // Bieżąca współrzędna Y do rysowania kolejnych linii statystyk.
        int currentY = SCORE_POS.Y;
        // Szerokość etykiet dla wyrównania wartości.
        int labelWidth = 11; 
        // Współrzędna X, od której zaczynają się wartości.
        int valueX = SCORE_POS.X + labelWidth; 

        // Czyści obszar statystyk przed rysowaniem.
        ConsoleUtils::fillRectangle(SCORE_POS.X, SCORE_POS.Y, CONSOLE_WIDTH - SCORE_POS.X -1, 15, L' '); // Wystarczająco duży prostokąt.

        // Rysuje Wynik.
        ConsoleUtils::writeString(SCORE_POS.X, currentY, "Score        ", COLOR_LABEL); // Etykieta.
        ConsoleUtils::writeString(valueX, currentY++, std::to_string(player.getScore()), COLOR_VALUE); // Wartość.

        // Rysuje Czas.
        ConsoleUtils::writeString(SCORE_POS.X, currentY, "Time         ", COLOR_LABEL); // Etykieta.
        ConsoleUtils::writeString(valueX, currentY++, player.getFormattedTime(), COLOR_VALUE); // Wartość.

        // Rysuje Liczbę Ruchów.
        ConsoleUtils::writeString(SCORE_POS.X, currentY, "Moves        ", COLOR_LABEL); // Etykieta.
        ConsoleUtils::writeString(valueX, currentY++, std::to_string(game.getMovesMade()), COLOR_VALUE); // Wartość.

        // Rysuje Najwyższy Wynik.
        ConsoleUtils::writeString(SCORE_POS.X, currentY, "High Score   ", COLOR_LABEL); // Etykieta.
        ConsoleUtils::writeString(valueX, currentY++, std::to_string(player.getHighScore()), COLOR_VALUE); // Wartość.

        // Przeskakuje linię przed wspomagaczami.
        currentY++; 

        // Rysuje informacje o wspomagaczach (Powerups).
        // Cofnięcie (Undo).
        ConsoleUtils::writeString(POWERUP_POS.X, currentY, "[U]ndo   ", COLOR_LABEL); // Etykieta z klawiszem skrótu.
        ConsoleUtils::writeString(POWERUP_POS.X + 9, currentY++, std::to_string(player.getUndosLeft()), player.getUndosLeft() > 0 ? COLOR_VALUE : COLOR_ERROR); // Liczba (kolor zależy od dostępności).

        // Przetasowanie (Shuffle).
        ConsoleUtils::writeString(POWERUP_POS.X, currentY, "[S]hfl   ", COLOR_LABEL); // Etykieta.
        ConsoleUtils::writeString(POWERUP_POS.X + 9, currentY++, std::to_string(player.getShufflesLeft()), player.getShufflesLeft() > 0 ? COLOR_VALUE : COLOR_ERROR); // Liczba.

        // Odkrycie (Reveal).
        ConsoleUtils::writeString(POWERUP_POS.X, currentY, "[V]eal   ", COLOR_LABEL); // Etykieta (V zamiast R).
        ConsoleUtils::writeString(POWERUP_POS.X + 9, currentY++, std::to_string(player.getRevealsLeft()), player.getRevealsLeft() > 0 ? COLOR_VALUE : COLOR_ERROR); // Liczba.

        // Joker.
        ConsoleUtils::writeString(POWERUP_POS.X, currentY, "[J]oker  ", COLOR_LABEL); // Etykieta.
        ConsoleUtils::writeString(POWERUP_POS.X + 9, currentY++, std::to_string(player.getJokerCardsLeft()), player.getJokerCardsLeft() > 0 ? COLOR_VALUE : COLOR_ERROR); // Liczba.

        // Podpowiedź (Hint).
        ConsoleUtils::writeString(POWERUP_POS.X, currentY, "[H]int   ", COLOR_LABEL); // Etykieta.
        ConsoleUtils::writeString(POWERUP_POS.X + 9, currentY++, std::to_string(player.getHintsLeft()), player.getHintsLeft() > 0 ? COLOR_VALUE : COLOR_ERROR); // Liczba.
    }

    // Rysuje linię pomocy na dole ekranu.
    void drawHelpLine() {
        // Czyści obszar linii pomocy.
        ConsoleUtils::fillRectangle(HELP_POS.X + 1, HELP_POS.Y, CONSOLE_WIDTH - 2, 1, L' ', COLOR_DEFAULT); // +1 i -2 aby nie nadpisać ramki.

        // Tekst pomocy ze skrótami klawiszowymi.
        std::string helpText = " Arrows: Navigate | Enter/Space: Select | D: Draw | U: Undo | H: Hint | S: Shfl | V: Real | J: Jokr | O: Opts | Q: Quit "; 
        // Wyśrodkowuje tekst pomocy.
        int startX = std::max(1, (CONSOLE_WIDTH - (int)helpText.length()) / 2);
        // Wypisuje tekst pomocy.
        ConsoleUtils::writeString(startX, HELP_POS.Y, helpText, COLOR_LABEL | NEO_BACKGROUND_BLACK); // Kolor etykiety na czarnym tle.
    }

public:
    // Konstruktor klasy interfejsu użytkownika konsoli.
    ConsoleUI(SolitaireGame& g) : game(g), player(g.getPlayer()) {
        // Inicjalizuje konsolę przy tworzeniu obiektu UI.
        ConsoleUtils::initConsole();
    }

    // Destruktor klasy interfejsu użytkownika konsoli.
    ~ConsoleUI() {
        // Przywraca oryginalne ustawienia konsoli przy niszczeniu obiektu UI.
        ConsoleUtils::cleanupConsole();
    }

    // Rysuje pasek statusu z aktualną wiadomością.
    void drawStatusBar() {
        // Wypełnia cały pasek statusu kolorem tła.
        ConsoleUtils::fillRectangle(STATUS_POS.X + 1, STATUS_POS.Y, CONSOLE_WIDTH - 2, 1, L' ', COLOR_STATUS_BAR);
        // Sprawdza, czy obecna wiadomość nie wygasła.
        if (std::chrono::steady_clock::now() < messageExpireTime) {
            // Jeśli nie wygasła, wypisuje ją.
             int startX = std::max(1, (CONSOLE_WIDTH - (int)statusMessage.length()) / 2);
            ConsoleUtils::writeStringWide(startX, STATUS_POS.Y, statusMessage, COLOR_STATUS_BAR);
        } else {
            // Jeśli wygasła, czyści tekst wiadomości.
            statusMessage.clear();
        }
    }

// Wykonuje animację początkowego rozdania kart.
void animateInitialDeal() {
    // Jeśli animacje są wyłączone, funkcja nic nie robi.
    if (!ENABLE_ANIMATION) {
        return;
    }
    
    // Pozycja początkowa dla animacji (stos stock).
    COORD deckPos = STOCK_POS; 
    
    // Czyści bufor konsoli przed rozpoczęciem animacji.
    ConsoleUtils::clearBuffer();
    // Rysuje statyczne elementy tła (ramki, etykiety).
    drawBoardStaticElements();

    // Tworzy pustą kartę (rewers) do rysowania w miejscach, gdzie karty jeszcze nie dotarły.
    Card blankCard = Card();
    // Ustawia ją jako zakrytą.
    blankCard.setFaceUp(false);

    // Rysuje puste miejsca dla kolumn tableau.
    for (int i = 0; i < 7; ++i) {
        drawEmptyPile(TABLEAU_POS[i].X, TABLEAU_POS[i].Y, false);
    }
    // Rysuje puste miejsca dla fundamentów.
    for (int i = 0; i < 4; ++i) {
        drawEmptyPile(FOUNDATION_POS[i].X, FOUNDATION_POS[i].Y, false);
    }
    // Rysuje puste miejsce dla stosu waste.
    drawEmptyPile(WASTE_POS.X, WASTE_POS.Y, false);

    // Rysuje symbol stosu stock (rewers karty).
    drawCard(deckPos.X, deckPos.Y, blankCard, false);

    // Renderuje początkowy stan planszy.
    ConsoleUtils::renderBuffer();
    // Czeka chwilę przed rozpoczęciem rozdawania.
    ConsoleUtils::sleep(DEAL_ANIMATION_DELAY_MS * 3);

    // Pętla rozdawania kart do kolumn tableau.
    for (int pileIdx = 0; pileIdx < 7; ++pileIdx) {
        // Bieżąca współrzędna Y dla kart w danej kolumnie.
        int currentY = TABLEAU_POS[pileIdx].Y;
        
        // Pętla rozdawania odpowiedniej liczby kart do kolumny.
        for (int cardIdx = 0; cardIdx <= pileIdx; ++cardIdx) {
            // Współrzędne docelowe dla bieżącej karty.
            COORD destCoord = {
                TABLEAU_POS[pileIdx].X,
                static_cast<SHORT>(currentY)
            };

            // Pobiera referencję do rzeczywistej karty z logiki gry.
            const Card& actualCard = game.getTableau()[pileIdx][cardIdx];

            // Tworzy kartę do animacji (odkryta tylko jeśli to ostatnia karta w kolumnie).
            Card animCard = actualCard;
            animCard.setFaceUp(cardIdx == pileIdx);

            // Odtwarza dźwięk rozdania karty (inny dla odkrywanej karty).
            ConsoleUtils::playSound(cardIdx == pileIdx ? 1000 : 800, 30);

            // Środek karty (nieużywane w tej wersji animacji).
            COORD cardCenter = {
                static_cast<SHORT>(destCoord.X + CARD_WIDTH / 2), 
                static_cast<SHORT>(destCoord.Y + CARD_HEIGHT / 2)
            };

            // Pętla animacji ruchu pojedynczej karty (po łuku).
            for (int step = 0; step <= MOVE_ANIMATION_STEPS / 2; ++step) {
                // Postęp animacji (0.0 do 1.0).
                float progress = static_cast<float>(step) / (MOVE_ANIMATION_STEPS / 2);

                // Parametry łuku (krzywa Beziera kwadratowa).
                float arcHeight = 5.0f; // Wysokość łuku.

                // Punkty kontrolne krzywej Beziera.
                float x0 = static_cast<float>(deckPos.X); // Początek X.
                float y0 = static_cast<float>(deckPos.Y); // Początek Y.
                float x2 = static_cast<float>(destCoord.X); // Koniec X.
                float y2 = static_cast<float>(destCoord.Y); // Koniec Y.
                float x1 = (x0 + x2) / 2; // Punkt kontrolny X (środek).
                float y1 = std::min(y0, y2) - arcHeight; // Punkt kontrolny Y (powyżej).

                // Parametry interpolacji.
                float t = progress;
                float u = 1.0f - t;
                float tt = t * t;
                float uu = u * u;
                
                // Oblicza bieżącą pozycję (x, y) na krzywej Beziera.
                float x = uu * x0 + 2 * u * t * x1 + tt * x2;
                float y = uu * y0 + 2 * u * t * y1 + tt * y2;
                
                // Konwertuje pozycję float na współrzędne konsoli.
                COORD currentPos = {
                    static_cast<SHORT>(std::round(x)),
                    static_cast<SHORT>(std::round(y))
                };

                // Przerysowuje całą planszę jako tło.
                drawBoardContent(std::nullopt); // Bez podpowiedzi.

                // Rysuje animowaną kartę w bieżącej pozycji.
                drawCard(currentPos.X, currentPos.Y, animCard, false);

                // Dodaje efekt "iskrzenia" dla odkrywanej karty.
                if (cardIdx == pileIdx) {
                    // Promień efektu maleje wraz z postępem.
                    int sparkleRadius = static_cast<int>(3 * (1.0f - progress));
                    // Rysuje 4 iskierki wokół karty.
                    for (int s = 0; s < 4; s++) {
                        // Oblicza pozycję iskierki na okręgu.
                        int sx = currentPos.X + CARD_WIDTH/2 + static_cast<int>(std::cos(progress * 6.28f + s * 1.57f) * sparkleRadius);
                        int sy = currentPos.Y + CARD_HEIGHT/2 + static_cast<int>(std::sin(progress * 6.28f + s * 1.57f) * sparkleRadius);
                        
                        // Sprawdza, czy pozycja iskierki jest w granicach konsoli.
                        if (sx >= 0 && sx < CONSOLE_WIDTH && sy >= 0 && sy < CONSOLE_HEIGHT) {
                            // Rysuje iskierkę.
                            ConsoleUtils::setChar(sx, sy, L'*', NEO_FOREGROUND_YELLOW | NEO_FOREGROUND_INTENSITY);
                        }
                    }
                }
                
                // Renderuje klatkę animacji.
                ConsoleUtils::renderBuffer();
                // Czeka chwilę.
                ConsoleUtils::sleep(DEAL_ANIMATION_DELAY_MS);
            }

            // Po zakończeniu animacji ruchu karty, przerysowuje planszę ze statyczną kartą w docelowym miejscu.
            drawBoardContent(std::nullopt);
            ConsoleUtils::renderBuffer();
            
            // Czeka chwilę przed rozpoczęciem animacji następnej karty.
            ConsoleUtils::sleep(DEAL_ANIMATION_DELAY_MS);
            
            // Jeśli to nie jest ostatnia karta w kolumnie, oblicza przesunięcie dla następnej.
            if (cardIdx < pileIdx) {
                // Używa rzeczywistego stanu karty (odkryta/zakryta) do obliczenia przesunięcia.
                // W tej implementacji karty poprzedzające ostatnią są zawsze zakryte po rozdaniu.
                if (animCard.isFaceUp()) { // Powinno być false.
                    currentY += 2;
                } else {
                    currentY += 1; // Zakryte karty mają mniejsze przesunięcie.
                }
            }
        }
    }

    // Animacja "rozdawania" pozostałych kart do stosu stock (tylko wizualna).
    int stockSize = static_cast<int>(game.getStock().size()); // Pobiera liczbę kart w stock.
    std::wstring countText = L"Dealing " + std::to_wstring(stockSize) + L" cards to stock..."; // Tekst informacyjny.
    
    // Wypisuje tekst informacyjny na dole ekranu.
    ConsoleUtils::writeStringWide(
        (CONSOLE_WIDTH - static_cast<int>(countText.length())) / 2, 
        CONSOLE_HEIGHT - 5, // Pozycja Y.
        countText, 
        COLOR_INFO // Kolor informacyjny.
    );
    // Renderuje zmiany.
    ConsoleUtils::renderBuffer();

    // Krótka animacja wizualna "sypania" kart na stos stock.
    for (int i = 0; i < std::min(10, stockSize); i++) { // Ogranicza liczbę kroków animacji.
        // Odtwarza narastający dźwięk.
        ConsoleUtils::playSound(600 + i * 30, 20);
        // Używa karty rewersu do animacji.
        Card tempCard = blankCard;
        // Rysuje kartę w lekko przesuniętej pozycji, aby symulować stos.
        drawCard(deckPos.X - 1 + (i % 3), deckPos.Y - 1 + (i % 2), tempCard, false);
        // Renderuje klatkę.
        ConsoleUtils::renderBuffer();
        // Czeka chwilę.
        ConsoleUtils::sleep(DEAL_ANIMATION_DELAY_MS);
    }

    // Po animacji "sypania", przerysowuje całą planszę w jej finalnym stanie początkowym.
    drawBoardContent(std::nullopt);

    // Wyświetla komunikat "Ready to play!".
    std::wstring readyText = L"Ready to play!";
    // Czyści obszar poprzedniego komunikatu.
    ConsoleUtils::fillRectangle(0, CONSOLE_HEIGHT - 5, CONSOLE_WIDTH, 1, L' ');
    // Wypisuje nowy komunikat.
    ConsoleUtils::writeStringWide(
        (CONSOLE_WIDTH - static_cast<int>(readyText.length())) / 2, 
        CONSOLE_HEIGHT - 5, 
        readyText, 
        COLOR_SUCCESS // Kolor sukcesu.
    );
    
    // Renderuje komunikat.
    ConsoleUtils::renderBuffer();
    // Czeka chwilę, aby gracz go zobaczył.
    ConsoleUtils::sleep(500);

    // Ostatecznie przerysowuje planszę bez komunikatu na dole.
    drawBoardContent(std::nullopt);
    ConsoleUtils::renderBuffer();
}

// Wykonuje animację wygranej gry.
void animateWin() {
    // Odtwarza dźwięk fanfar.
    ConsoleUtils::playSound(1500, 500); 

    // Zbiera wszystkie karty z fundamentów (zakładając, że tam są wszystkie przy wygranej).
    std::vector<Card> allCards;
    for (const auto& foundation : game.getFoundations()) {
        allCards.insert(allCards.end(), foundation.begin(), foundation.end());
    }

    // Zabezpieczenie: jeśli na fundamentach nie ma 52 kart, zbiera je ze wszystkich miejsc.
    if (allCards.size() != 52) {
        allCards.clear(); // Czyści wektor.
        // Zbiera karty ze wszystkich możliwych miejsc.
        for (const auto& f : game.getFoundations()) allCards.insert(allCards.end(), f.begin(), f.end());
        for (const auto& t : game.getTableau()) allCards.insert(allCards.end(), t.begin(), t.end());
        allCards.insert(allCards.end(), game.getStock().begin(), game.getStock().end());
        allCards.insert(allCards.end(), game.getWaste().begin(), game.getWaste().end());
    }

    // Upewnia się, że wszystkie karty są odkryte na potrzeby animacji.
    for(auto& cardRef : allCards) cardRef.setFaceUp(true); 
    // Tasuje zebrane karty, aby animacja była bardziej losowa.
    std::shuffle(allCards.begin(), allCards.end(), game.getRng()); 

    // Struktura przechowująca stan pojedynczej odbijającej się karty.
    struct BouncingCard {
        Card card; // Karta.
        float x, y; // Pozycja (float dla płynności).
        float vx, vy; // Prędkość (wektorowa).
    };
    // Wektor przechowujący wszystkie karty biorące udział w animacji.
    std::vector<BouncingCard> bouncingCards;

    // Inicjalizuje generator liczb losowych dla fizyki animacji.
    std::mt19937 bounceRng(static_cast<unsigned int>(std::time(nullptr)));
    // Rozkład dla losowej prędkości początkowej.
    std::uniform_real_distribution<float> speedDistribution(1.5f, 3.0f);
    // Rozkład dla losowego kąta początkowego.
    std::uniform_real_distribution<float> angleDistribution(0.0f, 2.0f * 3.14159f);
    // Rozkład dla losowej pozycji startowej X.
    std::uniform_int_distribution<int> startPosXDistribution(CARD_WIDTH, CONSOLE_WIDTH - CARD_WIDTH*2);
    // Rozkład dla losowej pozycji startowej Y.
    std::uniform_int_distribution<int> startPosYDistribution(CARD_HEIGHT, CARD_HEIGHT*2); 

    // Parametry fizyki animacji.
    float gravity = 0.1f; // Przyspieszenie grawitacyjne.
    float bounceFactor = -0.6f; // Współczynnik odbicia (ujemny i mniejszy od 1, aby tracić energię).

    // Rozpoczyna pomiar czasu trwania animacji.
    auto startTime = std::chrono::steady_clock::now();
    // Indeks następnej karty do dodania do animacji.
    int cardIndex = 0; 

    // Pętla główna animacji wygranej (trwa określoną liczbę sekund).
    while (std::chrono::duration_cast<std::chrono::seconds>(std::chrono::steady_clock::now() - startTime).count() < 8) { // Czas trwania animacji.
        // Czyści bufor konsoli w każdej klatce.
        ConsoleUtils::clearBuffer(); 
        // Rysuje statyczne tło (ramki, etykiety).
        drawBoardStaticElements(); 

        // Dodaje nową kartę do animacji, jeśli nie wszystkie zostały dodane i nie przekroczono limitu.
        if (cardIndex < static_cast<int>(allCards.size()) && bouncingCards.size() < 52) { // Limit 52 kart jednocześnie.
            // Losuje kąt i prędkość początkową.
            float angle = angleDistribution(bounceRng);
            float speed = speedDistribution(bounceRng);
             // Dodaje nową odbijającą się kartę do wektora.
             bouncingCards.push_back({
                allCards[cardIndex++], // Następna karta z potasowanej talii.
                static_cast<float>(startPosXDistribution(bounceRng)), static_cast<float>(startPosYDistribution(bounceRng)), // Losowa pozycja startowa.
                speed * std::cos(angle), speed * std::sin(angle) * 0.5f // Początkowa prędkość (mniejsza składowa Y).
             });
             // Odtwarza dźwięk dodania karty.
             ConsoleUtils::playSound(1000 + cardIndex*20, 30); 
        }

        // Aktualizuje pozycję i prędkość każdej karty w animacji.
        for (auto& bouncingCard : bouncingCards) {
            // Stosuje grawitację do prędkości pionowej.
            bouncingCard.vy += gravity;

            // Aktualizuje pozycję na podstawie prędkości.
            bouncingCard.x += bouncingCard.vx;
            bouncingCard.y += bouncingCard.vy;

            // Sprawdza kolizje ze ścianami poziomymi.
            if (bouncingCard.x <= 1 || bouncingCard.x + CARD_WIDTH >= CONSOLE_WIDTH - 1) {
                bouncingCard.vx *= bounceFactor; // Odwraca i tłumi prędkość X.
                 // Koryguje pozycję, aby karta nie utknęła w ścianie.
                 bouncingCard.x = std::max(1.0f, std::min(bouncingCard.x, static_cast<float>(CONSOLE_WIDTH) - 1.0f - CARD_WIDTH)); 
                 // Odtwarza dźwięk odbicia.
                 ConsoleUtils::playSound(600, 20); 
            }
            // Sprawdza kolizje ze ścianami pionowymi (góra/dół).
            if (bouncingCard.y <= 1 || bouncingCard.y + CARD_HEIGHT >= CONSOLE_HEIGHT - 1) {
                bouncingCard.vy *= bounceFactor; // Odwraca i tłumi prędkość Y.
                // Koryguje pozycję.
                bouncingCard.y = std::max(1.0f, std::min(bouncingCard.y, static_cast<float>(CONSOLE_HEIGHT) - 1.0f - CARD_HEIGHT)); 

                // Dodatkowo spowalnia ruch poziomy przy odbiciu od podłogi (symulacja tarcia).
                if (bouncingCard.y + CARD_HEIGHT >= CONSOLE_HEIGHT - 1) bouncingCard.vx *= 0.9f;
                 // Odtwarza dźwięk odbicia.
                 ConsoleUtils::playSound(500, 20); 
            }

            // Rysuje kartę w jej nowej pozycji.
            drawCard(static_cast<int>(bouncingCard.x), static_cast<int>(bouncingCard.y), bouncingCard.card, false);
        }

        // Renderuje klatkę animacji.
        ConsoleUtils::renderBuffer();
        // Czeka chwilę przed następną klatką.
        ConsoleUtils::sleep(ANIMATION_DELAY_MS * 2); // Dłuższe opóźnienie dla płynniejszego wyglądu.
    }

    // Po zakończeniu animacji odbijania, ustawia finalny komunikat o wygranej w pasku statusu.
    setStatusMessage(L"!!! C O N G R A T U L A T I O N S !!! YOU WIN !!!", 0); // 0 oznacza brak wygasania.
    // Rysuje pasek statusu z nowym komunikatem.
    drawStatusBar(); 
    // Renderuje zmiany.
    ConsoleUtils::renderBuffer();
    // Czeka kilka sekund, aby gracz mógł przeczytać komunikat.
    ConsoleUtils::sleep(3000); 
}

// Ustawia wiadomość w pasku statusu na określony czas.
void setStatusMessage(const std::wstring& message, int durationMilliseconds = 1500) {
    // Ustawia tekst wiadomości.
    statusMessage = message;
    // Jeśli czas trwania jest dodatni, ustawia czas wygaśnięcia.
    if (durationMilliseconds > 0) {
        messageExpireTime = std::chrono::steady_clock::now() + std::chrono::milliseconds(durationMilliseconds);
    } else {
        // Jeśli czas trwania jest 0 lub ujemny, wiadomość jest stała (nie wygasa).
        // Używa zerowego punktu czasowego jako flagi stałej wiadomości.
        messageExpireTime = std::chrono::time_point<std::chrono::steady_clock>(); 
    }

    // Natychmiast odświeża pasek statusu z nową wiadomością.
    drawStatusBar();
    // Renderuje zmiany.
    ConsoleUtils::renderBuffer();
}

// Natychmiast czyści wiadomość w pasku statusu.
void clearStatusMessage() {
    // Czyści tekst wiadomości.
    statusMessage.clear();
    // Ustawia czas wygaśnięcia na "teraz", aby natychmiast wygasła przy następnym rysowaniu.
    messageExpireTime = std::chrono::steady_clock::now();
}

// Rysuje całą planszę od nowa (bez renderowania). Używane np. przy inicjalizacji.
void drawBoard() {
    // Czyści bufor konsoli.
    ConsoleUtils::clearBuffer();
    // Rysuje statyczne elementy tła.
    drawBoardStaticElements();
    // Rysuje dynamiczną zawartość planszy (karty, statystyki itp.).
    drawBoardContent(std::nullopt); // Rysuje bez aktywnej podpowiedzi.
}

// Aktualizuje i renderuje całą planszę. Używane po większości akcji gracza.
void updateBoard() {
    // Czyści bufor konsoli.
    ConsoleUtils::clearBuffer();
    // Rysuje statyczne elementy tła.
    drawBoardStaticElements();
    // Rysuje dynamiczną zawartość planszy.
    drawBoardContent(std::nullopt); // Rysuje bez aktywnej podpowiedzi.
    // Renderuje zawartość bufora na ekranie.
    ConsoleUtils::renderBuffer();
}

// Wykonuje animację migotania dla wskazanej podpowiedzi.
void flashHint(const Hint& hint) {
    // Odtwarza dźwięk podpowiedzi.
    ConsoleUtils::playSound(1000, 50); 

    // Powtarza migotanie zadaną liczbę razy.
    for (int i = 0; i < HINT_FLASH_COUNT; ++i) {
        // Rysuje planszę z podświetloną podpowiedzią.
        drawBoardContent(hint);
        // Renderuje zmiany.
        ConsoleUtils::renderBuffer();
        // Czeka chwilę.
        ConsoleUtils::sleep(HINT_FLASH_DELAY_MS);

        // Rysuje planszę bez podświetlenia podpowiedzi.
        drawBoardContent(std::nullopt);
        // Renderuje zmiany.
        ConsoleUtils::renderBuffer();
        // Czeka chwilę.
        ConsoleUtils::sleep(HINT_FLASH_DELAY_MS);
    }

    // Na koniec upewnia się, że plansza jest narysowana bez podświetlenia.
    drawBoardContent(std::nullopt);
    ConsoleUtils::renderBuffer();
}

// Zwraca stałą referencję do aktualnego zaznaczenia interfejsu.
const Selection& getCurrentSelection() const { return currentSelection; }
// Zwraca true, jeśli gracz zaznaczył źródło ruchu.
bool isSourceSelected() const { return sourceSelection.isValid(); }

// Czyści zarówno bieżące zaznaczenie, jak i zaznaczenie źródła.
void clearSelections() {
    currentSelection.clear(); // Resetuje bieżące zaznaczenie.
    sourceSelection.clear(); // Resetuje zaznaczenie źródła.
}

// Czyści tylko zaznaczenie źródła (używane np. po wykonaniu ruchu).
void clearSourceSelection() {
    sourceSelection.clear();
}

// Ustawia zaznaczenie źródła (używane przez kontroler gry).
void setSourceSelection(const Selection& sel) {
    sourceSelection = sel;
}

// Obsługuje nawigację kursora (strzałki) po elementach planszy.
void navigate(int deltaX, int deltaY) {
     // Pobiera potencjalne następne zaznaczenie (początkowo kopia bieżącego).
     Selection nextSelection = currentSelection; 
     // Pobiera współrzędne bieżącego zaznaczenia.
     COORD currentPosition = getCoordsForSelection(currentSelection);

     // Jeśli nic nie jest zaznaczone lub pozycja jest nieprawidłowa, domyślnie zaznacza Stock.
     if (!currentSelection.isValid() || currentPosition.X == -1) {
         nextSelection = {SelectableType::STOCK, 0, -1}; // Zaczyna od stosu stock.
     } else {
         // Logika nawigacji w pionie (deltaY).
         if (deltaY == -1) { // Ruch w górę.
              // Jeśli zaznaczona jest karta w tableau.
              if (currentSelection.type == SelectableType::TABLEAU_CARD) {
                  // Szuka poprzedniej odkrytej karty w tej samej kolumnie.
                  int previousFaceUpIndex = -1;
                  const auto& pile = game.getTableau()[currentSelection.index];
                   for (int i = currentSelection.cardIndex - 1; i >= 0; --i) {
                       if (pile[i].isFaceUp()) {
                           previousFaceUpIndex = i;
                           break; // Znaleziono poprzednią odkrytą kartę.
                       }
                   }
                   // Jeśli znaleziono poprzednią odkrytą kartę, zaznacza ją.
                   if (previousFaceUpIndex != -1) {
                       nextSelection.cardIndex = previousFaceUpIndex; 
                   } 
                   // Jeśli nie ma poprzedniej odkrytej, ale jest to karta > 0, zaznacza pierwszą kartę (zakrytą).
                   else if (currentSelection.cardIndex > 0) {
                        // Zaznacza najniższą kartę (może być zakryta).
                        nextSelection.cardIndex = 0; 
                   } 
                   // Jeśli to była najniższa karta (index 0), przechodzi do górnego rzędu (Stock/Waste/Foundation).
                   else { 
                         // Wybiera element w górnym rzędzie na podstawie pozycji X.
                         if (currentPosition.X < WASTE_POS.X + CARD_WIDTH / 2) // Bliżej Stock.
                            nextSelection = {SelectableType::STOCK, 0, -1};
                         else if (currentPosition.X < FOUNDATION_POS[0].X + CARD_WIDTH / 2) // Bliżej Waste.
                            nextSelection = {SelectableType::WASTE, 0, -1};
                         else { // Bliżej Fundamentów.
                             // Znajduje najbliższy fundament.
                             int closestFoundation = 0;
                             int minDistance = 1000;
                             for(int f=0; f<4; ++f) {
                                 int distance = std::abs(currentPosition.X - FOUNDATION_POS[f].X);
                                 if (distance < minDistance) { minDistance = distance; closestFoundation = f;}
                             }
                             nextSelection = {SelectableType::FOUNDATION, closestFoundation, -1};
                         }
                   }
              }
              // Jeśli zaznaczona jest pusta kolumna tableau.
              else if (currentSelection.type == SelectableType::TABLEAU_PILE) {
                   // Przechodzi do górnego rzędu (logika jak wyżej).
                   if (currentPosition.X < WASTE_POS.X + CARD_WIDTH / 2)
                      nextSelection = {SelectableType::STOCK, 0, -1};
                   else if (currentPosition.X < FOUNDATION_POS[0].X + CARD_WIDTH / 2)
                      nextSelection = {SelectableType::WASTE, 0, -1};
                   else {
                       int closestFoundation = 0; int minDistance = 1000;
                       for(int f=0; f<4; ++f) {int d = std::abs(currentPosition.X - FOUNDATION_POS[f].X); if (d < minDistance) { minDistance = d; closestFoundation = f;}}
                       nextSelection = {SelectableType::FOUNDATION, closestFoundation, -1};
                   }
              }
              // Z innych miejsc (Stock, Waste, Foundation) ruch w górę nie zmienia zaznaczenia.
         }
         else if (deltaY == 1) { // Ruch w dół.
             // Jeśli zaznaczony jest element w górnym rzędzie.
             if (currentSelection.type == SelectableType::STOCK || currentSelection.type == SelectableType::WASTE || currentSelection.type == SelectableType::FOUNDATION) {
                 // Znajduje najbliższą kolumnę tableau poniżej.
                 int closestTableau = 0;
                 int minDistance = 1000;
                 for (int t = 0; t < 7; ++t) {
                     int distance = std::abs(currentPosition.X - TABLEAU_POS[t].X);
                     if (distance < minDistance) { minDistance = distance; closestTableau = t;}
                 }
                 const auto& targetPile = game.getTableau()[closestTableau]; // Pobiera docelową kolumnę.
                  // Jeśli kolumna nie jest pusta.
                  if (!targetPile.empty()) {
                       // Znajduje pierwszą odkrytą kartę od góry (od najniższego indeksu).
                       int firstFaceUpIndex = -1;
                       for (size_t c = 0; c < targetPile.size(); ++c) {
                           if (targetPile[c].isFaceUp()) { firstFaceUpIndex = static_cast<int>(c); break; }
                       }
                       // Jeśli znaleziono odkrytą kartę, zaznacza ją.
                       if (firstFaceUpIndex != -1)
                          nextSelection = {SelectableType::TABLEAU_CARD, closestTableau, firstFaceUpIndex};
                       // Jeśli nie ma odkrytych kart, zaznacza ostatnią (najwyższą zakrytą).
                       else 
                          nextSelection = {SelectableType::TABLEAU_CARD, closestTableau, static_cast<int>(targetPile.size() - 1)};
                  } else {
                       // Jeśli kolumna jest pusta, zaznacza ją jako całość.
                       nextSelection = {SelectableType::TABLEAU_PILE, closestTableau, -1};
                  }
             }
             // Jeśli zaznaczona jest karta w tableau.
             else if (currentSelection.type == SelectableType::TABLEAU_CARD) {
                 const auto& currentPile = game.getTableau()[currentSelection.index]; // Pobiera bieżącą kolumnę.
                 // Szuka następnej odkrytej karty poniżej bieżącej.
                 int nextFaceUpIndex = -1;
                 for (size_t i = static_cast<size_t>(currentSelection.cardIndex) + 1; i < currentPile.size(); ++i) { // Bezpieczne rzutowanie.
                     if (currentPile[i].isFaceUp()) {
                         nextFaceUpIndex = static_cast<int>(i);
                         break; // Znaleziono następną odkrytą kartę.
                     }
                 }
                 // Jeśli znaleziono, zaznacza ją.
                 if (nextFaceUpIndex != -1)
                     nextSelection.cardIndex = nextFaceUpIndex; 
                 // Jeśli nie znaleziono, ale bieżąca karta nie jest ostatnią, zaznacza ostatnią kartę.
                 else if (static_cast<size_t>(currentSelection.cardIndex) < currentPile.size() - 1) // Bezpieczne rzutowanie.
                    // Zaznacza ostatnią kartę w kolumnie.
                    nextSelection.cardIndex = static_cast<int>(currentPile.size() - 1);
                 // Jeśli to już była ostatnia karta, ruch w dół nie zmienia zaznaczenia w tej kolumnie.
             }
             // Z pustej kolumny tableau ruch w dół nie jest zdefiniowany.
         }
         // Logika nawigacji w poziomie (deltaX).
         else if (deltaX == -1) { // Ruch w lewo.
              // Z Waste na Stock.
              if (currentSelection.type == SelectableType::WASTE) nextSelection = {SelectableType::STOCK, 0, -1};
              // Między fundamentami.
              else if (currentSelection.type == SelectableType::FOUNDATION && currentSelection.index > 0) nextSelection.index--;
              // Z pierwszego fundamentu na Waste.
              else if (currentSelection.type == SelectableType::FOUNDATION && currentSelection.index == 0) nextSelection = {SelectableType::WASTE, 0, -1}; 
              // Między kolumnami tableau.
              else if ((currentSelection.type == SelectableType::TABLEAU_PILE || currentSelection.type == SelectableType::TABLEAU_CARD) && currentSelection.index > 0) {
                   // Wybiera kolumnę na lewo.
                   int targetIndex = currentSelection.index - 1;
                   const auto& targetPile = game.getTableau()[targetIndex]; // Pobiera docelową kolumnę.
                   // Jeśli kolumna docelowa nie jest pusta.
                   if (!targetPile.empty()) {
                        // Domyślnie zaznacza ostatnią kartę.
                        int targetCardIndex = static_cast<int>(targetPile.size())-1; 
                        // Szuka pierwszej odkrytej karty, aby ją zaznaczyć.
                        int firstFaceUp = -1;
                        for(size_t c=0; c<targetPile.size(); ++c) if(targetPile[c].isFaceUp()){ firstFaceUp=static_cast<int>(c); break;}
                        // Jeśli znaleziono odkrytą kartę, używa jej indeksu.
                        if(firstFaceUp != -1) targetCardIndex = firstFaceUp; 
                        // Ustawia nowe zaznaczenie na kartę w kolumnie docelowej.
                        nextSelection = {SelectableType::TABLEAU_CARD, targetIndex, targetCardIndex};
                   } else {
                        // Jeśli kolumna docelowa jest pusta, zaznacza ją jako całość.
                        nextSelection = {SelectableType::TABLEAU_PILE, targetIndex, -1};
                   }
              }
              // Z innych miejsc (Stock) ruch w lewo nie jest zdefiniowany.
         }
         else if (deltaX == 1) { // Ruch w prawo.
             // Ze Stock na Waste.
             if (currentSelection.type == SelectableType::STOCK) nextSelection = {SelectableType::WASTE, 0, -1};
             // Z Waste na pierwszy fundament.
             else if (currentSelection.type == SelectableType::WASTE) nextSelection = {SelectableType::FOUNDATION, 0, -1}; 
             // Między fundamentami.
             else if (currentSelection.type == SelectableType::FOUNDATION && currentSelection.index < 3) nextSelection.index++;
             // Między kolumnami tableau.
             else if ((currentSelection.type == SelectableType::TABLEAU_PILE || currentSelection.type == SelectableType::TABLEAU_CARD) && currentSelection.index < 6) {
                   // Wybiera kolumnę na prawo.
                   int targetIndex = currentSelection.index + 1;
                   const auto& targetPile = game.getTableau()[targetIndex]; // Pobiera docelową kolumnę.
                   // Jeśli kolumna docelowa nie jest pusta (logika jak dla ruchu w lewo).
                   if (!targetPile.empty()) {
                        int targetCardIndex = static_cast<int>(targetPile.size())-1;
                        int firstFaceUp = -1;
                        for(size_t c=0; c<targetPile.size(); ++c) if(targetPile[c].isFaceUp()){ firstFaceUp=static_cast<int>(c); break;}
                        if(firstFaceUp != -1) targetCardIndex = firstFaceUp;
                       nextSelection = {SelectableType::TABLEAU_CARD, targetIndex, targetCardIndex};
                   } else {
                        // Jeśli kolumna docelowa jest pusta.
                        nextSelection = {SelectableType::TABLEAU_PILE, targetIndex, -1};
                   }
             }
             // Z innych miejsc (ostatni fundament, ostatnia kolumna tableau) ruch w prawo nie jest zdefiniowany.
         }
     }

    // Jeśli obliczone następne zaznaczenie jest prawidłowe i inne niż bieżące.
    if (nextSelection.isValid() && nextSelection != currentSelection) {
        // Aktualizuje bieżące zaznaczenie.
        currentSelection = nextSelection;
        // Odtwarza krótki dźwięk nawigacji.
        ConsoleUtils::playSound(1000, 20); 
    } 
    // Jeśli następne zaznaczenie jest takie samo jak bieżące (brak zmiany).
    else if (nextSelection.isValid() && nextSelection == currentSelection) {
        // Nic nie robi, nie odtwarza dźwięku.
    }
    // Jeśli następne zaznaczenie jest nieprawidłowe (np. ruch poza granice).
    else {
         // Odtwarza dźwięk błędu/blokady.
         ConsoleUtils::playSound(150, 50); 
     }
}

// Wyświetla proste menu tekstowe i zwraca indeks wybranej opcji lub -1 przy anulowaniu.
int runMenu(const std::string& title, const std::vector<std::string>& options, int startX = -1, int startY = -1) {
    // Początkowo zaznaczona jest pierwsza opcja.
    int selectedOption = 0;
    // Liczba opcji w menu.
    int numOptions = static_cast<int>(options.size()); // Bezpieczne rzutowanie.
    // Jeśli nie ma opcji, zwraca -1.
    if (numOptions == 0) return -1; 

    // Oblicza szerokość menu na podstawie najdłuższej opcji i tytułu.
    int menuWidth = static_cast<int>(title.length()); // Bezpieczne rzutowanie.
    for (const auto& option : options) {
        menuWidth = std::max(menuWidth, static_cast<int>(option.length())); // Bierze pod uwagę szerokość opcji.
    }
    menuWidth += 4; // Dodaje marginesy (2 znaki po każdej stronie).
    // Oblicza wysokość menu (tytuł + opcje + ramki i odstępy).
    int menuHeight = numOptions + 4; // Tytuł(1) + Pusty(1) + Opcje(numOptions) + Pusty(1) + Ramki Pionowe(2) = numOptions + 4.

    // Jeśli współrzędne startowe nie są podane, centruje menu.
    if (startX < 0) startX = (CONSOLE_WIDTH - menuWidth) / 2;
    if (startY < 0) startY = (CONSOLE_HEIGHT - menuHeight) / 2;

    // Zapobiega wyjściu menu poza ekran.
    if (startX + menuWidth >= CONSOLE_WIDTH) startX = CONSOLE_WIDTH - menuWidth - 1;
    if (startY + menuHeight >= CONSOLE_HEIGHT) startY = CONSOLE_HEIGHT - menuHeight - 1;
    if (startX < 0) startX = 0; // Upewnia się, że X nie jest ujemne.
    if (startY < 0) startY = 0; // Upewnia się, że Y nie jest ujemne.

    // Pętla obsługi menu.
    while (true) {
        // Rysuje podwójną ramkę wokół menu.
        ConsoleUtils::drawBox(startX, startY, menuWidth, menuHeight, COLOR_BORDER, true); 
        // Wypełnia wnętrze menu domyślnym tłem.
        ConsoleUtils::fillRectangle(startX + 1, startY + 1, menuWidth - 2, menuHeight - 2, L' ', COLOR_DEFAULT); 

        // Wypisuje tytuł menu, wyśrodkowany.
        ConsoleUtils::writeString(startX + (menuWidth - static_cast<int>(title.length())) / 2, startY + 1, title, COLOR_TITLE); // Bezpieczne rzutowanie.

        // Rysuje opcje menu.
        for (int i = 0; i < numOptions; ++i) {
            // Wybiera atrybut w zależności od tego, czy opcja jest zaznaczona.
            WORD attribute = (i == selectedOption) ? COLOR_MENU_SELECTED : COLOR_MENU_ITEM;
            // Tworzy tekst opcji z dopełnieniem spacjami dla jednolitego tła przy zaznaczeniu.
            // Używa rzutowania na int, aby uniknąć problemów z typami przy odejmowaniu.
            std::string padding(std::max(0, menuWidth - 4 - static_cast<int>(options[i].length())), ' ');
            std::string optionText = " " + options[i] + padding; 
            // Wypisuje tekst opcji.
            ConsoleUtils::writeString(startX + 2, startY + 3 + i, optionText, attribute); // +3 dla tytułu, pustej linii i początku opcji.
        }

        // Renderuje narysowane menu.
        ConsoleUtils::renderBuffer(); 

        // Odczytuje naciśnięty klawisz.
        int key = ConsoleUtils::getKeyPress();

        // Obsługuje nawigację i wybór w menu.
        switch (key) {
        case KEY_UP: // Strzałka w górę.
        case KEY_NAV_K_LOWER: // Klawisz 'k'.
        case KEY_NAV_K_UPPER: // Klawisz 'K'.
            // Przechodzi do poprzedniej opcji (z zawijaniem).
            selectedOption = (selectedOption - 1 + numOptions) % numOptions;
            // Odtwarza dźwięk nawigacji.
            ConsoleUtils::playSound(800, 30); 
            break;
        case KEY_DOWN: // Strzałka w dół.
        case KEY_NAV_J_LOWER: // Klawisz 'j'.
        case KEY_NAV_J_UPPER: // Klawisz 'J'.
            // Przechodzi do następnej opcji (z zawijaniem).
            selectedOption = (selectedOption + 1) % numOptions;
            // Odtwarza dźwięk nawigacji.
            ConsoleUtils::playSound(800, 30); 
            break;
        case KEY_ENTER: // Enter.
        case KEY_SPACE: // Spacja.
            // Odtwarza dźwięk potwierdzenia.
            ConsoleUtils::playSound(1000, 50); 
            // Zwraca indeks wybranej opcji.
            return selectedOption; 
        case KEY_ESC: // Escape.
        case 'q': // Klawisz 'q'.
        case 'Q': // Klawisz 'Q'.
            // Odtwarza dźwięk anulowania.
            ConsoleUtils::playSound(400, 50); 
            // Zwraca -1, sygnalizując anulowanie.
            return -1; 
        }
    }
}

// Wywołuje animację dla efektu wspomagacza.
void animatePowerUpEffect(int x, int y, const std::wstring& powerUpText) {
    // Wywołuje główną funkcję animacji wspomagacza.
    animatePowerUp(x, y, powerUpText);
}
}; // Koniec klasy ConsoleUI.


// Klasa kontrolera gry, łącząca logikę (SolitaireGame) z interfejsem (ConsoleUI).
class GameController {
private:
    // Obiekt logiki gry.
    SolitaireGame game;
    // Obiekt interfejsu użytkownika.
    ConsoleUI ui;
    // Flaga określająca, czy główna pętla gry ma być kontynuowana.
    bool running;
    // Zmienna przechowująca zaznaczenie źródłowe (potrzebna w kontrolerze do logiki wyboru).
    Selection sourceSelection; 

    // Przetwarza pojedyncze wejście (naciśnięcie klawisza) od użytkownika.
    void processInput() {
        // Odczytuje naciśnięty klawisz.
        int key = ConsoleUtils::getKeyPress();

        // Specjalna obsługa wejścia, gdy gra jest zakończona (wygrana lub przegrana).
        if (game.isGameOver() || game.isGameWon()) {
             // Klawisz 'N' - nowa gra.
             if (key == 'n' || key == 'N') {
                startGame(); // Rozpoczyna nową grę (implementacja poza fragmentem).
             } 
             // Klawisz 'Q' lub Escape - powrót do menu głównego.
             else if (key == 'q' || key == 'Q' || key == KEY_ESC) {
                 showMainMenu(); // Wyświetla menu główne (implementacja poza fragmentem).
             } 
             // Klawisz 'O' - opcje.
             else if (key == 'o' || key == 'O') {
                 showOptions(); // Wyświetla menu opcji (implementacja poza fragmentem).
                 // Po powrocie z opcji, jeśli gra nadal jest zakończona, ponownie obsługuje ten stan.
                 if (game.isGameOver() || game.isGameWon()) {
                    handleGameOver(); // Ponownie wyświetla komunikat końca gry.
                 }
             }
             // Ignoruje inne klawisze w stanie końca gry.
             return; 
        }

        // Flaga wskazująca, czy wykonano próbę ruchu (np. przeniesienie karty, pociągnięcie).
        bool moveAttempted = false; 
        // Flaga wskazująca, czy plansza wymaga pełnego przerysowania po obsłudze klawisza.
        bool requiresRedraw = true; 

        // Główna instrukcja switch obsługująca różne klawisze.
        switch (key) {
            // Klawisze nawigacyjne (strzałki).
            case KEY_UP: ui.navigate(0, -1); requiresRedraw = true; break; // Góra.
            case KEY_DOWN: ui.navigate(0, 1); requiresRedraw = true; break; // Dół.
            case KEY_LEFT: ui.navigate(-1, 0); requiresRedraw = true; break; // Lewo.
            case KEY_RIGHT: ui.navigate(1, 0); requiresRedraw = true; break; // Prawo.

            // Klawisze wyboru (Enter, Spacja).
            case KEY_ENTER:
            case KEY_SPACE:
                handleSelection(); // Obsługuje logikę zaznaczania/przenoszenia.
                moveAttempted = true; // Zaznaczenie/próba ruchu.
                break;

            // Klawisz 'D' - pociągnięcie karty (Draw).
            case 'd': case 'D': 
                // Próbuje pociągnąć kartę lub zresetować stos.
                if (game.dealFromStock()) {
                    ui.clearSelections(); // Czyści zaznaczenie interfejsu.
                    sourceSelection.clear(); // Czyści zaznaczenie źródła w kontrolerze.
                    ui.setStatusMessage(L"Drew from stock.", 1000); // Wyświetla komunikat.
                } else {
                    // Jeśli nie można pociągnąć/zresetować.
                    ui.setStatusMessage(L"Cannot draw or recycle.", 1500); // Wyświetla błąd.
                }
                moveAttempted = true; // Zaznacza próbę ruchu.
                break;
            // Klawisz 'U' - cofnięcie (Undo).
            case 'u': case 'U':
                // Sprawdza, czy wspomagacze są włączone i czy są dostępne cofnięcia.
                if (game.getPlayer().getPowerupsEnabled() && game.getPlayer().getUndosLeft() > 0) {
                    // Próbuje cofnąć ruch w logice gry.
                    if (game.undoMove()) {
                        // Jeśli cofnięcie się udało.
                        ui.clearSelections(); // Czyści zaznaczenia.
                        sourceSelection.clear();
                        // Wykonuje animację cofnięcia.
                        int centerX = CONSOLE_WIDTH / 2 - 2; // Pozycja animacji.
                        int centerY = CONSOLE_HEIGHT / 2;
                        ui.animatePowerUpEffect(centerX, centerY, L"UNDO!");
                        // Aktualizuje planszę po animacji.
                        ui.updateBoard();
                    }
                    // Jeśli game.undoMove() zwróci false (anomalia).
                } else {
                    // Jeśli brak cofnięć lub wspomagacze wyłączone.
                    ui.setStatusMessage(L"No undos left or powerups disabled!", 1500); // Wyświetla błąd.
                }
                break;

            // Klawisz 'S' - przetasowanie (Shuffle).
            case 's': case 'S':
                // Sprawdza dostępność wspomagacza.
                if (game.getPlayer().getPowerupsEnabled() && game.getPlayer().getShufflesLeft() > 0) {
                    // Próbuje użyć przetasowania w logice gry.
                    if (game.useShufflePowerup()) {
                        // Jeśli się udało.
                        ui.clearSelections(); // Czyści zaznaczenia.
                        sourceSelection.clear();
                        // Wykonuje animację przetasowania.
                        int centerX = CONSOLE_WIDTH / 2 - 4; // Pozycja animacji.
                        int centerY = CONSOLE_HEIGHT / 2;
                        ui.animatePowerUpEffect(centerX, centerY, L"SHUFFLE!");
                        // Aktualizuje planszę.
                        ui.updateBoard();
                    } else {
                        // Jeśli nie było czego tasować.
                        ui.setStatusMessage(L"Nothing to shuffle!", 1500); // Wyświetla komunikat.
                    }
                } else {
                    // Jeśli brak wspomagacza.
                    ui.setStatusMessage(L"No shuffles left or powerups disabled!", 1500); // Wyświetla błąd.
                }
                break;

            // Klawisz 'V' - odkrycie (Reveal).
            case 'v': case 'V':
                // Sprawdza dostępność wspomagacza.
                if (game.getPlayer().getPowerupsEnabled() && game.getPlayer().getRevealsLeft() > 0) {
                    // Próbuje użyć odkrycia w logice gry.
                    if (game.useRevealPowerup()) {
                        // Jeśli się udało.
                        ui.clearSelections(); // Czyści zaznaczenia.
                        sourceSelection.clear();
                        // Wykonuje animację odkrycia.
                        int centerX = CONSOLE_WIDTH / 2 - 3; // Pozycja animacji.
                        int centerY = CONSOLE_HEIGHT / 2;
                        ui.animatePowerUpEffect(centerX, centerY, L"REVEAL!");
                        // Aktualizuje planszę.
                        ui.updateBoard();
                    } else {
                        // Jeśli nie było nic do odkrycia.
                        ui.setStatusMessage(L"No face-down cards to reveal!", 1500); // Wyświetla komunikat.
                    }
                } else {
                    // Jeśli brak wspomagacza.
                    ui.setStatusMessage(L"No reveals left or powerups disabled!", 1500); // Wyświetla błąd.
                }
                break;

            // Klawisz 'J' - Joker.
            case 'j': case 'J':
                // Sprawdza dostępność wspomagacza.
                if (game.getPlayer().getPowerupsEnabled() && game.getPlayer().getJokerCardsLeft() > 0) {
                    // Wyświetla menu wyboru kolumny tableau dla Jokera.
                    std::vector<std::string> options = {"Tableau 1", "Tableau 2", "Tableau 3", "Tableau 4", "Tableau 5", "Tableau 6", "Tableau 7"};
                    int selected = ui.runMenu("Select Tableau for Joker Card", options);
                    
                    // Jeśli wybrano poprawną kolumnę (0-6).
                    if (selected >= 0 && selected < 7) {
                        // Próbuje umieścić Jokera w logice gry.
                        if (game.useJokerPowerup(selected)) {
                            // Jeśli się udało.
                            ui.clearSelections(); // Czyści zaznaczenia.
                            sourceSelection.clear();
                            // Wykonuje animację Jokera.
                            int centerX = CONSOLE_WIDTH / 2 - 3; // Pozycja animacji.
                            int centerY = CONSOLE_HEIGHT / 2;
                            ui.animatePowerUpEffect(centerX, centerY, L"JOKER!");
                            // Aktualizuje planszę.
                            ui.updateBoard();
                        } else {
                            // Jeśli nie można było umieścić Jokera.
                            ui.setStatusMessage(L"Can't place joker there!", 1500); // Wyświetla błąd.
                        }
                    }
                    // Jeśli anulowano wybór kolumny.
                } else {
                    // Jeśli brak wspomagacza.
                    ui.setStatusMessage(L"No jokers left or powerups disabled!", 1500); // Wyświetla błąd.
                }
                break;

            // Klawisz 'H' - podpowiedź (Hint).
            case KEY_H_LOWER: // Małe 'h'.
            case KEY_H_UPPER: // Duże 'H'.
                // Sprawdza dostępność wspomagacza.
                if (game.getPlayer().getPowerupsEnabled() && game.getPlayer().getHintsLeft() > 0) {
                    // Pobiera podpowiedź z logiki gry (zużywa licznik i punkty).
                    auto hint = game.getHint();
                    // Jeśli znaleziono podpowiedź.
                    if (hint) {
                        ui.clearSelections(); // Czyści zaznaczenia.
                        sourceSelection.clear();
                        ui.flashHint(*hint); // Wykonuje animację migotania podpowiedzi.
                        ui.setStatusMessage(hint->description, 3000); // Wyświetla opis podpowiedzi.
                    } else {
                        // Jeśli nie znaleziono podpowiedzi.
                        ui.setStatusMessage(L"No hints available!", 1500); // Wyświetla komunikat.
                        game.getPlayer().refundHint(); // Zwraca zużytą podpowiedź i punkty.
                    }
                } else {
                    // Jeśli brak wspomagacza.
                    ui.setStatusMessage(L"No hints left or powerups disabled!", 1500); // Wyświetla błąd.
                }
                break;

            // Klawisz 'O' - opcje (Options).
            case 'o': case 'O': 
                 showOptions(); // Wyświetla menu opcji (implementacja poza fragmentem).
                 requiresRedraw = true; // Wymaga przerysowania po powrocie z opcji.
                 break;
            // Klawisz 'Q' - wyjście (Quit) do menu głównego.
            case 'q': case 'Q': 
                 game.getPlayer().updateHighScore(); // Aktualizuje rekord przed wyjściem.
                 game.getPlayer().saveToFile(); // Zapisuje stan gracza.
                 showMainMenu(); // Wyświetla menu główne (implementacja poza fragmentem).
                 return; // Kończy przetwarzanie wejścia dla tej klatki.
            // Klawisz Escape - anulowanie zaznaczenia źródła lub czyszczenie zaznaczenia.
            case KEY_ESC: 
                // Jeśli zaznaczono źródło ruchu.
                if (sourceSelection.isValid()) { 
                    sourceSelection.clear(); // Czyści zaznaczenie źródła w kontrolerze.
                    ui.clearSelections(); // Czyści zaznaczenie w UI.
                    ui.setStatusMessage(L"Move cancelled.", 1000); // Wyświetla komunikat.
                } else {
                    // Jeśli nic nie zaznaczono jako źródło, czyści tylko bieżące zaznaczenie w UI.
                    ui.clearSelections(); 
                }
                break;
            // Domyślna obsługa - inne klawisze.
            default:
                requiresRedraw = false; // Inne klawisze nie wymagają przerysowania planszy.
                break;
        }

        // Po przetworzeniu klawisza, jeśli wykonano próbę ruchu.
        if (moveAttempted) {
            game.checkWinCondition(); // Sprawdza, czy gra została wygrana.
             // Jeśli gra nie została wygrana, sprawdza, czy nie ma już ruchów.
             if (!game.isGameWon()) {
                 game.checkForGameOver(); 
             }
             // Jeśli gra się zakończyła (wygrana lub przegrana przez brak ruchów).
             if (game.isGameOver() || game.isGameWon()) {
                 handleGameOver(); // Obsługuje stan końca gry.
                 return; // Kończy przetwarzanie wejścia dla tej klatki.
             }
        }

        // Jeśli plansza wymaga przerysowania (np. po nawigacji, ruchu, użyciu wspomagacza).
        if (requiresRedraw) {
             ui.updateBoard(); // Aktualizuje i renderuje całą planszę.
        } else {
             // Jeśli plansza nie wymaga pełnego przerysowania, aktualizuje tylko pasek statusu.
             ui.drawStatusBar(); // Rysuje pasek statusu (może zawierać nowe komunikaty).
             ConsoleUtils::renderBuffer(); // Renderuje tylko zmiany w pasku statusu.
        }
    }

    // Obsługuje logikę wyboru/kliknięcia na element planszy (Enter/Spacja).
    void handleSelection() {
        // Pobiera aktualne zaznaczenie z interfejsu użytkownika.
        const Selection& currentSelection = ui.getCurrentSelection();
        // Jeśli nic nie jest zaznaczone, nic nie robi.
        if (!currentSelection.isValid()) return; 

        // Flaga wskazująca, czy wykonano automatyczny ruch (np. dwuklik na kartę do fundamentu).
        bool autoMoved = false;

        // Sprawdza możliwość automatycznego ruchu tylko jeśli źródło NIE jest jeszcze zaznaczone.
        if (!sourceSelection.isValid()) {
             // Auto-ruch z Waste na Fundament.
             if (currentSelection.type == SelectableType::WASTE && !game.getWaste().empty() && game.canMoveWasteToFoundation()) {
                autoMoved = game.moveWasteToFoundation(); // Wykonuje ruch.
                if(autoMoved) ui.setStatusMessage(L"Moved Waste to Foundation.", 1000); // Komunikat sukcesu.
             }
             // Auto-ruch z Tableau na Fundament (tylko wierzchnia karta).
             else if (currentSelection.type == SelectableType::TABLEAU_CARD && currentSelection.index >= 0 && currentSelection.index < 7 &&
                       !game.getTableau()[currentSelection.index].empty() &&
                       // Sprawdza, czy zaznaczona jest dokładnie ostatnia karta w kolumnie.
                       currentSelection.cardIndex == static_cast<int>(game.getTableau()[currentSelection.index].size() - 1))
            {
                 // Sprawdza, czy ruch jest możliwy.
                 if (game.canMoveTableauToFoundation(currentSelection.index)) {
                     autoMoved = game.moveTableauToFoundation(currentSelection.index); // Wykonuje ruch.
                     if(autoMoved) ui.setStatusMessage(L"Moved Tableau to Foundation.", 1000); // Komunikat sukcesu.
                 }
            }
        }

        // Jeśli wykonano automatyczny ruch, czyści zaznaczenia i kończy obsługę.
        if (autoMoved) {
            ui.clearSelections(); // Czyści zaznaczenie w UI.
            sourceSelection.clear(); // Czyści zaznaczenie źródła w kontrolerze.
            return; // Kończy funkcję.
        }

        // Jeśli NIE wykonano automatycznego ruchu i źródło NIE jest zaznaczone - próba zaznaczenia źródła.
        if (!sourceSelection.isValid()) {
            // Sprawdza, czy bieżące zaznaczenie może być prawidłowym źródłem ruchu.
            bool isValidSource = false;
            switch (currentSelection.type) {
            case SelectableType::STOCK:
                 // Kliknięcie na Stock zawsze wykonuje akcję pociągnięcia/resetu.
                 if (game.dealFromStock()) {
                    ui.setStatusMessage(L"Drew from stock.", 1000); 
                 } else {
                    ui.setStatusMessage(L"Cannot draw or recycle.", 1500); 
                 }
                 ui.clearSelections(); // Czyści zaznaczenie po akcji.
                 sourceSelection.clear();
                 return; // Kończy funkcję, bo akcja została wykonana.
            case SelectableType::WASTE:
                // Waste jest źródłem, jeśli nie jest puste.
                isValidSource = !game.getWaste().empty();
                break;
            case SelectableType::FOUNDATION:
                // Fundament jest źródłem, jeśli nie jest pusty.
                 isValidSource = !game.getFoundations()[currentSelection.index].empty();
                 break;
            case SelectableType::TABLEAU_PILE: 
                 // Pusta kolumna tableau nie może być źródłem ruchu.
                 isValidSource = false; 
                 break;
            case SelectableType::TABLEAU_CARD:
                // Karta w tableau jest źródłem, jeśli istnieje, jest w granicach i jest odkryta.
                 isValidSource = currentSelection.index >= 0 && currentSelection.index < 7 &&
                                 !game.getTableau()[currentSelection.index].empty() &&
                                 currentSelection.cardIndex >= 0 &&
                                 static_cast<size_t>(currentSelection.cardIndex) < game.getTableau()[currentSelection.index].size() &&
                                 game.getTableau()[currentSelection.index][currentSelection.cardIndex].isFaceUp();
                 break;
            default: break; // Inne typy nie są źródłem.
            }

            // Jeśli zaznaczenie jest prawidłowym źródłem.
            if (isValidSource) {
                sourceSelection = currentSelection; // Zapisuje zaznaczenie jako źródło w kontrolerze.
                ui.setSourceSelection(sourceSelection); // Informuje UI o zaznaczeniu źródła (dla podświetlenia).
                ui.setStatusMessage(L"Source selected. Select destination.", 0); // Wyświetla instrukcję (stała).
                ConsoleUtils::playSound(900, 40); // Dźwięk zaznaczenia źródła.
            } else {
                // Jeśli wybrano nieprawidłowe źródło.
                ui.setStatusMessage(L"Invalid source selected.", 1500); // Wyświetla błąd.
                ConsoleUtils::playSound(200, 100); // Dźwięk błędu.
            }
        // Jeśli źródło JEST już zaznaczone - próba wykonania ruchu do bieżącego zaznaczenia (celu).
        } else {
            // Pobiera zapisane źródło.
            Selection source = sourceSelection; 
            // Bieżące zaznaczenie jest celem.
            const Selection& destination = currentSelection;
            // Flaga wskazująca, czy ruch został wykonany.
            bool moveMade = false;

            // Logika sprawdzania i wykonywania ruchu w zależności od typu źródła i celu.
            // Ruch z Waste.
            if (source.type == SelectableType::WASTE) {
                // Do Fundamentu.
                if (destination.type == SelectableType::FOUNDATION) {
                     moveMade = game.moveWasteToFoundation(); 
                } 
                // Do Tableau (pusta kolumna lub karta).
                else if (destination.type == SelectableType::TABLEAU_PILE || destination.type == SelectableType::TABLEAU_CARD) {
                     int targetIndex = destination.index; // Pobiera indeks docelowej kolumny.
                     moveMade = game.moveWasteToTableau(targetIndex);
                }
            } 
            // Ruch z Tableau (karta).
            else if (source.type == SelectableType::TABLEAU_CARD) {
                // Do Fundamentu.
                if (destination.type == SelectableType::FOUNDATION) {
                     // Sprawdza dodatkowo, czy przenoszona jest tylko jedna, wierzchnia karta.
                     if (source.index >= 0 && source.index < 7 && !game.getTableau()[source.index].empty() &&
                         source.cardIndex == static_cast<int>(game.getTableau()[source.index].size() - 1)) {
                         moveMade = game.moveTableauToFoundation(source.index); 
                     } else {
                          // Jeśli próbowano przenieść kartę inną niż wierzchnia lub kilka kart.
                          ui.setStatusMessage(L"Can only move top card to foundation.", 1500); 
                     }
                } 
                // Do innej kolumny Tableau.
                else if (destination.type == SelectableType::TABLEAU_PILE || destination.type == SelectableType::TABLEAU_CARD) {
                     int targetIndex = destination.index; // Pobiera indeks docelowej kolumny.
                     // Wykonuje ruch tableau-tableau, podając indeks pierwszej przenoszonej karty.
                     moveMade = game.moveTableauToTableau(source.index, targetIndex, source.cardIndex);
                }
            } 
            // Ruch z Fundamentu.
            else if (source.type == SelectableType::FOUNDATION) {
                // Do Tableau.
                if (destination.type == SelectableType::TABLEAU_PILE || destination.type == SelectableType::TABLEAU_CARD) {
                     int targetIndex = destination.index; // Pobiera indeks docelowej kolumny.
                     moveMade = game.moveFoundationToTableau(source.index, targetIndex);
                }
            }

            // Po próbie wykonania ruchu.
            if (moveMade) {
                 ui.setStatusMessage(L"Move successful!", 1000); // Komunikat sukcesu.
                 game.getPlayer().updateHighScore(); // Sprawdza i ewentualnie aktualizuje rekord.
            } else {
                 // Jeśli ruch się nie powiódł (ale nie z powodu próby przeniesienia na Stock lub na samego siebie).
                 if (source != destination && destination.type != SelectableType::STOCK) {
                    ui.setStatusMessage(L"Invalid move.", 1500); // Wyświetla błąd.
                    ConsoleUtils::playSound(200, 150); // Dźwięk błędu.
                 }
            }

            // Niezależnie od tego, czy ruch się udał, czy nie, czyści zaznaczenia.
            ui.clearSelections(); // Czyści zaznaczenie w UI.
            sourceSelection.clear(); // Czyści zaznaczenie źródła w kontrolerze.
        }
    }

    // Obsługuje wyświetlanie stanu końca gry (komunikaty, animacje).
    void handleGameOver() {
        // Czyści zaznaczenia na wszelki wypadek.
        ui.clearSelections(); 
        sourceSelection.clear();
        // Rysuje finalny stan planszy.
        ui.drawBoard(); 
        ConsoleUtils::renderBuffer();
        // Czeka chwilę przed wyświetleniem komunikatu/animacji.
        ConsoleUtils::sleep(500); 

        // Tekst finalnego komunikatu.
        std::wstring finalMessage; 
        // Jeśli gra została wygrana.
        if (game.isGameWon()) {
             // Tworzy komunikat o wygranej z wynikiem i czasem.
             finalMessage = L"!!! YOU WIN !!! Score: " + std::to_wstring(game.getPlayer().getScore()) +
                           L" Time: " + std::wstring(game.getPlayer().getFormattedTime().begin(), game.getPlayer().getFormattedTime().end());
             // Odtwarza dźwięki fanfar.
             ConsoleUtils::playSound(1200, 200); ConsoleUtils::sleep(100);
             ConsoleUtils::playSound(1500, 300); ConsoleUtils::sleep(100);
             ConsoleUtils::playSound(1800, 500);
             // Wykonuje animację wygranej.
             ui.animateWin(); // Ta funkcja może blokować na kilka sekund.
        } 
        // Jeśli gra została przegrana (brak ruchów).
        else {
             // Odtwarza dźwięki przegranej.
             ConsoleUtils::playSound(300, 300); ConsoleUtils::sleep(100);
             ConsoleUtils::playSound(250, 300); ConsoleUtils::sleep(100);
             ConsoleUtils::playSound(200, 500);
             // Tworzy komunikat o przegranej z wynikiem.
             finalMessage = L"GAME OVER! Score: " + std::to_wstring(game.getPlayer().getScore());
        }

        // Dodaje instrukcje do finalnego komunikatu.
        finalMessage += L" | Press N for New Game, O for Options, Q for Main Menu."; 
        // Ustawia finalny komunikat jako stały w pasku statusu.
        ui.setStatusMessage(finalMessage, 0); 

        // Rysuje pasek statusu z finalnym komunikatem.
        ui.drawStatusBar();
        // Renderuje zmiany.
        ConsoleUtils::renderBuffer();
    }

    // Wyświetla ekran powitalny (splash screen).
    void displaySplash() {
        // Czyści bufor konsoli.
        ConsoleUtils::clearBuffer();

        // Definicja grafiki ASCII dla ekranu powitalnego.
        std::vector<std::string> splashArt = {
            " ",
            " ███╗   ██╗███████╗ ██████╗ ███████╗██╗ ██████╗ ████████╗ ██████╗ ███████╗ ",
            " ████╗  ██║██╔════╝██╔═══██╗██╔════╝██║██╔════╝    ██╔══╝██╔════╝ ██╔════╝ ",
            " ██╔██╗ ██║███████╗██║   ██║███████╗██║██║         ██║   ██║  ███╗███████╗ ",
            " ██║╚██╗██║╚════██║██║   ██║╚════██║██║██║         ██║   ██║   ██║╚════██║ ",
            " ██║ ╚████║███████║╚██████╔╝███████║██║╚██████╗    ██║   ╚██████╔╝███████║ ",
            " ╚═╝  ╚═══╝╚══════╝ ╚═════╝ ╚══════╝╚═╝ ╚═════╝    ╚═╝    ╚═════╝ ╚══════╝ ",
            " ",
            "                                                                           T H E   U L T I M A T E   C O N S O L E   S O L I T A I R E "
        };
        // Oblicza pozycję startową Y dla wyśrodkowania grafiki.
        int startY = (CONSOLE_HEIGHT - static_cast<int>(splashArt.size())) / 2 - 2; // Bezpieczne rzutowanie.
        // Znajduje maksymalną szerokość linii w grafice.
        int maxWidth = 0;
        for(const auto& line : splashArt) maxWidth = std::max(maxWidth, static_cast<int>(line.length())); // Bezpieczne rzutowanie.
        // Oblicza pozycję startową X dla wyśrodkowania grafiki.
        int startX = (CONSOLE_WIDTH - maxWidth) / 2;

        // Animacja pojawiania się grafiki (znak po znaku).
        for (size_t i = 0; i < splashArt.size(); ++i) { // Iteruje przez linie.
             for (size_t j = 0; j < splashArt[i].length(); ++j) { // Iteruje przez znaki w linii.
                 // Ustawia znak w buforze z kolorem tytułu.
                 ConsoleUtils::setChar(startX + static_cast<int>(j), startY + static_cast<int>(i), splashArt[i][j], COLOR_TITLE); // Bezpieczne rzutowanie.
                 // Renderuje co kilka znaków dla płynniejszej animacji.
                 if (j % 5 == 0) { 
                      ConsoleUtils::renderBuffer();
                      ConsoleUtils::sleep(1); // Bardzo krótkie opóźnienie.
                 }
             }
            // Renderuje całą linię po jej narysowaniu.
            ConsoleUtils::renderBuffer(); 
            // Czeka chwilę między liniami.
            ConsoleUtils::sleep(50);
        }

        // Wyświetla tekst "Loading...".
        std::string loadingText = "Loading...";
        ConsoleUtils::writeString((CONSOLE_WIDTH - static_cast<int>(loadingText.length())) / 2, startY + static_cast<int>(splashArt.size()) + 2, loadingText, COLOR_INFO); // Bezpieczne rzutowanie.

        // Renderuje tekst ładowania.
        ConsoleUtils::renderBuffer();
        // Czeka, symulując ładowanie.
        ConsoleUtils::sleep(1500); 
    }

    // Wyświetla menu główne gry.
    void showMainMenu() {
        // Definicja opcji menu głównego.
        const std::vector<std::string> options = {"New Game   ", "Tutorial", "Options", "Exit"};
        // Początkowy wybór (-1 oznacza brak wyboru).
        int choice = -1;
        // Flaga wskazująca, czy tło (plansza gry) wymaga przerysowania.
        bool needsRedraw = true; 

        // Pętla menu głównego. Działa dopóki nie zostanie wybrana opcja inna niż -1 lub gra nie zostanie zamknięta.
        while (choice == -1 && running) { 
             // Jeśli wymagane jest przerysowanie tła (np. po powrocie z opcji).
             if (needsRedraw) {
                 // Rysuje aktualny stan planszy (może być pusta lub zakończona gra).
                 ui.drawBoard();
                 // Renderuje zmiany.
                 ConsoleUtils::renderBuffer();
                 // Resetuje flagę przerysowania.
                 needsRedraw = false;
             }

            // Wyświetla menu i pobiera wybór gracza.
            choice = ui.runMenu("Main Menu", options, -1, -1); // -1, -1 centruje menu.

            // Flaga wskazująca, czy należy wrócić do aktywnej gry (jeśli była).
            bool returnToGame = false;
            // Obsługuje wybór gracza.
            switch (choice) {
                case 0: // Nowa Gra.
                    startGame(); // Rozpoczyna nową grę.
                    return; // Wychodzi z showMainMenu, aby przejść do pętli gry.
                case 1: // Samouczek.
                    showTutorial(); // Wyświetla samouczek.
                    needsRedraw = true; // Wymaga przerysowania tła po powrocie.
                    choice = -1; // Resetuje wybór, aby menu pojawiło się ponownie.
                    break;
                case 2: // Opcje.
                    showOptions(); // Wyświetla menu opcji.
                    needsRedraw = true; // Wymaga przerysowania tła.
                    choice = -1; // Resetuje wybór.
                    break;
                case 3: // Wyjście.
                    game.getPlayer().updateHighScore(); // Aktualizuje rekord przed wyjściem.
                    game.getPlayer().saveToFile(); // Zapisuje stan gracza.
                    running = false; // Ustawia flagę zakończenia głównej pętli gry.
                    break;
                default: // Anulowanie (Escape) lub nieprawidłowy wybór.
                     // Jeśli gra jest zakończona, anulowanie ponownie wyświetla menu.
                     if (game.isGameOver() || game.isGameWon()) {
                         choice = -1; // Resetuje wybór.
                         needsRedraw = true; // Wymaga przerysowania tła.
                     } 
                     // Jeśli gra jest w toku, anulowanie wraca do gry.
                     else {
                         returnToGame = true; // Ustawia flagę powrotu do gry.
                     }
                    break;
            }
             // Jeśli ustawiono flagę powrotu do gry, przerywa pętlę menu.
             if (returnToGame) break; 
        }

        // Po wyjściu z pętli menu (jeśli gra nadal działa i nie wybrano Nowej Gry),
        // upewnia się, że plansza jest poprawnie narysowana.
        if (running && choice != 0) { 
             ui.drawBoard();
             ConsoleUtils::renderBuffer();
        }
    }
    
    // Wyświetla interaktywny samouczek gry.
    void showTutorial() {
        // Czyści bufor i rysuje ramkę.
        ConsoleUtils::clearBuffer();
        ConsoleUtils::drawBox(0, 0, CONSOLE_WIDTH, CONSOLE_HEIGHT, COLOR_BORDER);
        // Rysuje tytuł samouczka.
        ConsoleUtils::writeString((CONSOLE_WIDTH - 21) / 2, 1, "SOLITAIRE TUTORIAL", COLOR_TITLE | NEO_BACKGROUND_BLUE);
        
        // Zmienne stanu samouczka.
        int currentLesson = 0; // Bieżący numer lekcji.
        bool tutorialActive = true; // Flaga aktywności pętli samouczka.
        int animationFrame = 0; // Licznik klatek dla animacji w lekcjach.
        
        // Teksty poszczególnych lekcji.
        std::vector<std::wstring> lessons = {
            L"Welcome to the Solitaire Tutorial! Interact with the examples below to learn how to play.",
            L"OBJECTIVE: Move all cards to the 4 Foundation piles (♥♦♣♠) in ascending order (A→K) by suit.",
            L"TABLEAU: Build columns in descending order (K→A) with alternating colors (red on black, black on red).",
            L"STOCK & WASTE: Draw cards from Stock (left pile) to Waste (middle pile) when you need more options.",
            L"FOUNDATIONS: Build upward by suit from Ace to King. Only cards of matching suit can be placed here.",
            L"CARD MOVEMENT: Select a card or sequence with Enter/Space, then select a valid destination.",
            L"SPECIAL MOVES: Empty tableau spots can only be filled with Kings or King sequences.",
            L"POWER-UPS: Special abilities to help when stuck. Each has limited uses per game.",
            L"WINNING STRATEGY: Focus on uncovering face-down cards and creating empty tableau spots for Kings.",
            L"SCORING: Earn points for each move to foundation. Try to win with the highest score possible!",
            L"You're ready to play! Remember to have fun and use power-ups when needed."
        };
        
        // Funkcja pomocnicza do rysowania pełnej karty w samouczku (większa niż w grze).
        auto drawFullCard = [](int x, int y, char suit, int value, bool faceUp, bool highlighted = false) {
            // Tworzy obiekt karty na podstawie parametrów.
            Card card(suit, value);
            card.setFaceUp(faceUp);
            // Rysuje ramkę karty (6x4 znaki).
            ConsoleUtils::drawBox(x, y, 6, 4, highlighted ? COLOR_HIGHLIGHTS : COLOR_BORDER);
            
            // Jeśli karta jest odkryta, rysuje jej wartość i kolor.
            if (faceUp) {
                // Konwertuje wartość liczbową na string (A, J, Q, K).
                std::wstring valueStr;
                switch (value) {
                    case 1: valueStr = L"A"; break;
                    case 11: valueStr = L"J"; break;
                    case 12: valueStr = L"Q"; break;
                    case 13: valueStr = L"K"; break;
                    default: valueStr = std::to_wstring(value); break;
                }
                
                // Wybiera symbol i kolor konsoli na podstawie koloru karty.
                std::wstring suitStr;
                WORD color;
                switch (suit) {
                    case 'H': suitStr = L"♥"; color = COLOR_CARD_RED; break;
                    case 'D': suitStr = L"♦"; color = COLOR_CARD_RED; break;
                    case 'C': suitStr = L"♣"; color = COLOR_CARD_BLACK; break;
                    case 'S': suitStr = L"♠"; color = COLOR_CARD_BLACK; break;
                    default: suitStr = L"?"; color = COLOR_DEFAULT; break;
                }
                
                // Oblicza przesunięcie X, aby wyśrodkować tekst.
                int xOffset = (valueStr.length() == 1) ? 2 : 1;
                // Wypisuje wartość i kolor karty.
                ConsoleUtils::writeStringWide(x + xOffset, y + 1, valueStr + suitStr, highlighted ? COLOR_HIGHLIGHTS : color);
            } else {
                // Jeśli karta jest zakryta, wypełnia ją symbolem rewersu.
                ConsoleUtils::fillRectangle(x + 1, y + 1, 4, 2, L'░', COLOR_CARD_BACK);
            }
        };
        
        // Funkcja pomocnicza do rysowania animowanej strzałki.
        auto drawAnimatedArrow = [&animationFrame](int x, int y, bool horizontal, int length, WORD color = COLOR_INFO) {
            // Wybiera znak strzałki (pozioma lub pionowa).
            std::wstring arrowChars = horizontal ? L"→" : L"↓";
            // Oblicza pozycję animowanego znaku strzałki.
            int pos = animationFrame % length;
            
            // Rysuje strzałkę (jeden znak aktywny, reszta spacje).
            for (int i = 0; i < length; i++) {
                // Jeśli to pozycja aktywnego znaku.
                if (i == pos) {
                    // Rysuje znak strzałki z podświetleniem.
                    ConsoleUtils::writeStringWide(x + (horizontal ? i : 0), y + (horizontal ? 0 : i), arrowChars, color | NEO_FOREGROUND_INTENSITY);
                } else {
                    // Rysuje spację, aby wymazać poprzednią pozycję znaku.
                    ConsoleUtils::writeStringWide(x + (horizontal ? i : 0), y + (horizontal ? 0 : i), L" ", COLOR_DEFAULT);
                }
            }
        };
        
        // Lambda do renderowania strony samouczka.
        auto renderTutorialPage = [&](int page) {
            // Czyści bufor i rysuje ramkę.
            ConsoleUtils::clearBuffer();
            ConsoleUtils::drawBox(0, 0, CONSOLE_WIDTH, CONSOLE_HEIGHT, COLOR_BORDER);
            // Rysuje tytuł.
            ConsoleUtils::writeString((CONSOLE_WIDTH - 21) / 2, 1, "SOLITAIRE TUTORIAL", COLOR_TITLE | NEO_BACKGROUND_BLUE);

            // Wyświetla informację o numerze strony.
            std::string pageInfo = "Lesson " + std::to_string(page + 1) + " of " + std::to_string(lessons.size());
            ConsoleUtils::writeString((CONSOLE_WIDTH - static_cast<int>(pageInfo.length())) / 2, 3, pageInfo, COLOR_LABEL); // Bezpieczne rzutowanie.
            
            // Definiuje tekst podpowiedzi interakcji dla niektórych stron.
            std::wstring interactPrompt = L"";
            if (page == 2 || page == 5) { // Strony z demonstracją tableau i ruchu.
                interactPrompt = L"Press Enter to see a demonstration.";
            } else if (page == 7) { // Strona o wspomagaczach.
                interactPrompt = L"Press P, U, S, V, J, or H to learn about each power-up.";
            }
            
            // Jeśli jest podpowiedź interakcji, wyświetla ją na dole.
            if (!interactPrompt.empty()) {
                ConsoleUtils::writeStringWide((CONSOLE_WIDTH - static_cast<int>(interactPrompt.length())) / 2, CONSOLE_HEIGHT - 4, interactPrompt, COLOR_SUCCESS); // Bezpieczne rzutowanie.
            }

            // Rysuje zawartość specyficzną dla danej strony samouczka.
            switch (page) {
                case 0: { // Strona powitalna.
                    // Rysuje ramkę wewnętrzną.
                    ConsoleUtils::drawBox(15, 8, 50, 10, COLOR_BORDER);
                    
                    // Definicja kolorów kart.
                    std::vector<std::pair<char, WORD>> suits = {
                        {'H', COLOR_CARD_RED}, {'D', COLOR_CARD_RED},
                        {'C', COLOR_CARD_BLACK}, {'S', COLOR_CARD_BLACK}
                    };
                    
                    // Rysuje 4 animowane karty różnych kolorów.
                    for (int i = 0; i < 4; i++) {
                        char suit = "HDCS"[i];
                        int x = 20 + i * 10;
                        // Wartość karty zmienia się w czasie animacji.
                        drawFullCard(x, 9, suit, (animationFrame / 4) % 13 + 1, true);
                    }
                    
                    // Wyświetla tekst powitalny.
                    ConsoleUtils::writeStringWide(20, 14, L"Learn to play Solitaire step by step!", COLOR_SUCCESS);
                    break;
                }
                case 1: { // Cel gry - Fundamenty.
                    // Wyświetla tytuł sekcji.
                    ConsoleUtils::writeString(20, 6, "OBJECTIVE: Build four ordered foundation piles.", COLOR_TITLE);
                    
                    // Pozycja startowa dla rysowania fundamentów.
                    int startX = 20;
                    // Iteruje przez 4 kolory.
                    for (int suit = 0; suit < 4; suit++) {
                        // Pobiera znak i symbol koloru.
                        char suitChar = "HDCS"[suit];
                        std::wstring suitSymbol;
                        suitSymbol += L"♥♦♣♠"[suit]; // Pobiera znak Unicode.
                        // Wybiera kolor konsoli.
                        WORD color = (suit < 2) ? COLOR_CARD_RED : COLOR_CARD_BLACK;
                        
                        // Rysuje pustą ramkę fundamentu z symbolem koloru.
                        ConsoleUtils::drawBox(startX + suit*15, 8, 6, 4, COLOR_BORDER);
                        ConsoleUtils::writeStringWide(startX + suit*15 + 2, 9, suitSymbol, color);
                        
                        // Animuje budowanie stosu na fundamencie (od Asa w górę).
                        int topCard = (animationFrame / 10) % 14; // Wartość rośnie do 13, potem zaczyna od nowa.
                        for (int i = 1; i <= std::min(13, topCard); i++) {
                            // Rysuje karty jedna nad drugą z lekkim przesunięciem.
                            int offset = i * 1; // Przesunięcie Y.
                            drawFullCard(startX + suit*15, 8 - offset, suitChar, i, true);
                        }
                    }
                    
                    // Wyświetla opisy celu gry.
                    ConsoleUtils::writeString(20, 14, "Each foundation begins with Ace (A).", COLOR_INFO);
                    ConsoleUtils::writeString(20, 15, "and builds up to King (K) by suit.", COLOR_INFO);
                    ConsoleUtils::writeString(20, 17, "Game is won when all four piles are complete.", COLOR_SUCCESS);
                    break;
                }
                case 2: { // Zasady Tableau.
                    // Wyświetla tytuł sekcji.
                    ConsoleUtils::writeString(15, 6, "TABLEAU RULES: Cards build DOWN with ALTERNATING COLORS.", COLOR_TITLE);
                    
                    // Rysuje pierwszy przykład budowania kolumny (Pik->Kier->Pik).
                    int x = 15;
                    drawFullCard(x, 8, 'S', 5, true);
                    drawFullCard(x, 12, 'H', 4, true);
                    drawFullCard(x, 16, 'S', 3, true);
                    
                    // Dodaje opisy strzałek.
                    ConsoleUtils::writeStringWide(x+7, 10, L"↓ Black to Red.", COLOR_INFO);
                    ConsoleUtils::writeStringWide(x+7, 14, L"↓ Red to Black.", COLOR_INFO);
                    
                    // Rysuje drugi przykład (Karo->Trefl->Kier).
                    x = 45;
                    drawFullCard(x, 8, 'D', 10, true);
                    drawFullCard(x, 12, 'C', 9, true);
                    drawFullCard(x, 16, 'H', 8, true);
                    
                    // Dodaje opisy strzałek.
                    ConsoleUtils::writeStringWide(x+7, 10, L"↓ Red to Black.", COLOR_INFO);
                    ConsoleUtils::writeStringWide(x+7, 14, L"↓ Black to Red.", COLOR_INFO);
                    
                    break;
                }
                case 3: { // Stock i Waste.
                    // Wyświetla tytuł sekcji.
                    ConsoleUtils::writeString(20, 6, "STOCK & WASTE: Drawing new cards.", COLOR_TITLE);
                    
                    // Współrzędne X dla Stock i Waste.
                    int stockX = 20, wasteX = 40;
                    
                    // Rysuje stos Stock (kilka kart nałożonych na siebie).
                    for (int i = 0; i < 3; i++) {
                        ConsoleUtils::drawBox(stockX - i/2, 8 - i/2, 6, 4, COLOR_BORDER);
                    }
                    // Wypełnia wierzchnią kartę Stock symbolem rewersu.
                    ConsoleUtils::fillRectangle(stockX + 1, 8 + 1, 4, 2, L'░', COLOR_CARD_BACK);
                    // Dodaje etykietę "STOCK".
                    ConsoleUtils::writeString(stockX + 1, 13, "STOCK", COLOR_LABEL);
                    
                    // Animuje ruch karty ze Stock do Waste.
                    int movePhase = animationFrame % 30; // Cykl animacji.
                    if (movePhase < 15) { // Faza ruchu.
                        // Oblicza pozycję karty w trakcie ruchu.
                        int cardX = stockX + (wasteX - stockX) * movePhase / 15;
                        // Rysuje kartę (odkrywana w połowie ruchu).
                        drawFullCard(cardX, 8, 'H', 7, movePhase > 7);
                    } else { // Faza po ruchu.
                        // Rysuje kartę statycznie na stosie Waste.
                        drawFullCard(wasteX, 8, 'H', 7, true);
                    }
                    
                    // Dodaje etykietę "WASTE".
                    ConsoleUtils::writeString(wasteX + 1, 13, "WASTE", COLOR_LABEL);
                    
                    // Wyświetla opisy działania Stock i Waste.
                    ConsoleUtils::writeString(20, 17, "Press [D] to draw card from Stock to Waste.", COLOR_INFO);
                    ConsoleUtils::writeString(20, 19, "When Stock is empty, you can use [S] to shuffle (limited uses).", COLOR_INFO);
                    break;
                }
                case 4: { // Budowanie Fundamentów.
                    // Wyświetla tytuł sekcji.
                    ConsoleUtils::writeString(15, 6, "FOUNDATION BUILDING: Same suit, ascending order (A→K).", COLOR_TITLE);
                    
                    // Pozycja X fundamentu.
                    int foundationX = 35;
                    
                    // Przykładowy kolor (Pik).
                    char suit = 'S';
                    std::wstring suitSymbol = L"♠";
                    
                    // Rysuje pustą ramkę fundamentu z symbolem koloru.
                    ConsoleUtils::drawBox(foundationX, 8, 6, 4, COLOR_BORDER);
                    ConsoleUtils::writeStringWide(foundationX + 2, 9, suitSymbol, COLOR_CARD_BLACK);
                    
                    // Animuje dodawanie kart na fundament (od Asa do 5).
                    int topCard = 1 + (animationFrame / 20) % 5; // Wartość rośnie od 1 do 5.
                    for (int i = 1; i <= topCard; i++) {
                        // Rysuje karty jedna nad drugą.
                        drawFullCard(foundationX, 8 - (i-1), suit, i, true);
                    }
                    
                    // Rysuje następną kartę (która ma być dodana) obok, podświetloną.
                    drawFullCard(20, 12, suit, topCard + 1, true, true);
                    
                    // Rysuje animowaną strzałkę wskazującą ruch.
                    drawAnimatedArrow(27, 12, true, 7);
                    
                    // Wyświetla zasady budowania fundamentów.
                    ConsoleUtils::writeString(15, 17, "● You can only place cards on foundations in ascending order (A,2,3...).", COLOR_INFO);
                    ConsoleUtils::writeString(15, 18, "● Each foundation accepts only one suit (♥, ♦, ♣, or ♠).", COLOR_INFO);
                    break;
                }
                case 5: { // Przenoszenie Kart.
                    // Wyświetla tytuł sekcji.
                    ConsoleUtils::writeString(15, 6, "MOVING CARDS: Select source, then destination.", COLOR_TITLE);
                    
                    // Faza animacji ruchu sekwencji kart.
                    int movePhase = animationFrame % 40; // Cykl animacji.
                    
                    // Rysuje kolumnę źródłową.
                    int srcX = 20;
                    // Karta, która pozostanie w kolumnie źródłowej.
                    drawFullCard(srcX, 8, 'H', 6, true, movePhase < 10); // Podświetlona na początku.
                    // Karty, które będą przenoszone (podświetlone razem z pierwszą).
                    drawFullCard(srcX, 12, 'S', 5, true, movePhase < 10 && movePhase >= 5);
                    drawFullCard(srcX, 16, 'H', 4, true, movePhase < 10 && movePhase >= 5);
                    
                    // Rysuje kolumnę docelową.
                    int dstX = 45;
                    // Karta na wierzchu kolumny docelowej (podświetlona w fazie wyboru celu).
                    drawFullCard(dstX, 8, 'C', 7, true, movePhase >= 10 && movePhase < 20);
                    
                    // Faza animacji przesuwania kart.
                    if (movePhase >= 20 && movePhase < 30) {
                        int progress = movePhase - 20; // Postęp w tej fazie.
                        // Oblicza bieżącą pozycję X przenoszonych kart.
                        int cardX = srcX + (dstX - srcX) * progress / 10;
                        // Rysuje przenoszone karty w bieżącej pozycji.
                        drawFullCard(cardX, 12, 'S', 5, true);
                        drawFullCard(cardX, 16, 'H', 4, true);
                    } 
                    // Faza po zakończeniu ruchu.
                    else if (movePhase >= 30) {
                        // Rysuje przeniesione karty statycznie w kolumnie docelowej.
                        drawFullCard(dstX, 12, 'S', 5, true);
                        drawFullCard(dstX, 16, 'H', 4, true);
                    }
                    
                    // Wyświetla instrukcje dotyczące przenoszenia kart.
                    ConsoleUtils::writeString(15, 20, "1. Select a card or sequence with Enter/Space (it will highlight).", COLOR_INFO);
                    ConsoleUtils::writeString(15, 21, "2. Then select a valid destination and press Enter/Space again.", COLOR_INFO);
                    break;
                }
                 case 6: { // Ruchy specjalne - pusty spot.
                    // Wyświetla tytuł sekcji.
                    ConsoleUtils::writeString(20, 6, "SPECIAL MOVES & RULES", COLOR_TITLE);
                    
                    // Rysuje pusty spot w tableau.
                    int tableauX = 30;
                    ConsoleUtils::drawBox(tableauX, 8, 6, 4, COLOR_BORDER);
                    ConsoleUtils::writeString(tableauX + 2, 9, "[ ]", COLOR_CARD_EMPTY);
                    ConsoleUtils::writeString(tableauX-5, 13, "EMPTY SPOT", COLOR_LABEL);
                
                    // Rysuje Króla, który ma być przeniesiony na pusty spot.
                    drawFullCard(tableauX - 15, 8, 'S', 13, true);
                    
                    // Rysuje animowaną strzałkę wskazującą ruch Króla.
                    drawAnimatedArrow(tableauX - 8, 9, true, 7);
                    
                    // Wyświetla zasady specjalne.
                    ConsoleUtils::writeString(20, 16, "● Only Kings can be placed on empty tableau spots.", COLOR_INFO);
                    ConsoleUtils::writeString(20, 17, "● You can move sequences of cards as a single unit.", COLOR_INFO);
                    ConsoleUtils::writeString(20, 18, "● Creating empty spots is a key strategy for organizing cards.", COLOR_INFO);
                    break;
                }
                case 7: { // Wspomagacze (Power-ups).
                    // Wyświetla tytuł sekcji.
                    ConsoleUtils::writeString(20, 6, "POWER-UPS: Special abilities to help when stuck.", COLOR_TITLE);
                    
                    // Pozycja startowa dla listy wspomagaczy.
                    int x = 20, y = 8;
                    // Definicje nazw i opisów wspomagaczy.
                    std::vector<std::pair<std::string, std::string>> powerups = {
                        {"[U]ndo", "Take back your last move."},
                        {"[S]huffle", "Reshuffle stock & waste piles."},
                        {"Re[V]eal", "Flip over hidden cards on tableau."},
                        {"[J]oker", "Create a wild card that matches any card."},
                        {"[H]int", "Highlights a possible move you can make."}
                    };
                    
                    // Animuje podświetlanie kolejnych wspomagaczy na liście.
                    int highlightIndex = (animationFrame / 30) % static_cast<int>(powerups.size()); // Bezpieczne rzutowanie.
                    
                    // Rysuje listę wspomagaczy.
                    for (size_t i = 0; i < powerups.size(); i++) {
                        // Wybiera kolor w zależności od podświetlenia.
                        WORD color = (static_cast<int>(i) == highlightIndex) ? COLOR_SUCCESS : COLOR_LABEL; // Bezpieczne rzutowanie.
                        // Wypisuje nazwę wspomagacza (z klawiszem skrótu).
                        ConsoleUtils::writeString(x, y + static_cast<int>(i)*2, powerups[i].first, color); // Bezpieczne rzutowanie.
                        // Wypisuje opis wspomagacza.
                        ConsoleUtils::writeString(x + 10, y + static_cast<int>(i)*2, powerups[i].second, (static_cast<int>(i) == highlightIndex) ? COLOR_SUCCESS : COLOR_DEFAULT); // Bezpieczne rzutowanie.
                    }
                    
                    // Wyświetla informację o ograniczonym użyciu wspomagaczy.
                    ConsoleUtils::writeString(20, 18, "Each power-up has limited uses per game. Use them wisely!", COLOR_INFO);
                    break;
                }
                case 8: { // Strategie wygrywania.
                    // Wyświetla tytuł sekcji.
                    ConsoleUtils::writeString(20, 6, "WINNING STRATEGIES", COLOR_TITLE);
                    
                    // Pozycja X dla pierwszego przykładu.
                    int x = 15;
                    
                    // Rysuje przykład kolumny z zakrytą kartą na dole.
                    drawFullCard(x, 8, 'S', 10, true); // Karta odkryta.
                    drawFullCard(x, 12, 'H', 9, true); // Karta odkryta.
                    drawFullCard(x, 16, 'X', 0, false); // Karta zakryta (X jako placeholder).
                    
                    // Dodaje opisy strategii.
                    ConsoleUtils::writeString(x + 10, 10, "Focus on revealing face-down cards first.", COLOR_SUCCESS);
                    ConsoleUtils::writeString(x + 10, 14, "This card is still unknown - try to reveal it.", COLOR_INFO);
                    
                    // Pozycja Y dla drugiego przykładu.
                    x = 15;
                    int strategyY = 20;
                    // Rysuje przykład pustego miejsca.
                    ConsoleUtils::drawBox(x, strategyY, 6, 4, COLOR_BORDER);
                    ConsoleUtils::writeString(x + 2, strategyY + 1, "[ ]", COLOR_CARD_EMPTY);
                    
                    // Rysuje Króla obok.
                    drawFullCard(x + 25, strategyY, 'D', 13, true);
                    // Dodaje opis strategii pustych miejsc.
                    ConsoleUtils::writeString(x + 10, strategyY + 2, "Empty spots are valuable! Use for Kings.", COLOR_SUCCESS);
                    
                    break;
                }
                 case 9: { // Punktacja.
                    // Wyświetla tytuł sekcji.
                    ConsoleUtils::writeString(20, 6, "SCORING & POINTS", COLOR_TITLE);
                    
                    // Pozycja Y dla tabeli punktacji.
                    int y = 8;
                    // Nagłówki tabeli.
                    ConsoleUtils::writeString(15, y, "Action", COLOR_LABEL);
                    ConsoleUtils::writeString(40, y, "Points", COLOR_LABEL);
                    y += 2; // Przesunięcie do pierwszej akcji.
                    
                    // Definicja akcji i ich punktacji.
                    std::vector<std::pair<std::string, int>> scoreActions = {
                        {"Move card to Foundation.", 10},
                        {"Flip tableau card face up.", 5},
                        {"Move King to empty spot.", 5}, // Punktacja może się różnić w zależności od zasad.
                        {"Using Shuffle power-up.", -20},
                        {"Game Completion Bonus.", 100} // Przykładowa wartość.
                    };
                    
                    // Animuje podświetlanie kolejnych wierszy tabeli.
                    int highlight = (animationFrame / 20) % static_cast<int>(scoreActions.size()); // Bezpieczne rzutowanie.
                    
                    // Rysuje tabelę punktacji.
                    for (size_t i = 0; i < scoreActions.size(); i++) {
                        // Wybiera kolor w zależności od podświetlenia.
                        WORD color = (static_cast<int>(i) == highlight) ? COLOR_SUCCESS : COLOR_DEFAULT; // Bezpieczne rzutowanie.
                        // Wypisuje nazwę akcji.
                        ConsoleUtils::writeString(15, y + static_cast<int>(i)*2, scoreActions[i].first, color); // Bezpieczne rzutowanie.
                        // Wypisuje liczbę punktów.
                        ConsoleUtils::writeString(40, y + static_cast<int>(i)*2, std::to_string(scoreActions[i].second), color); // Bezpieczne rzutowanie.
                    }
                    
                    // Przesuwa Y poniżej tabeli.
                    y += static_cast<int>(scoreActions.size()) * 2 + 1; // Bezpieczne rzutowanie.
                    // Wyświetla zachętę do zdobywania punktów.
                    ConsoleUtils::writeString(15, y, "Try to win with highest possible score!", COLOR_INFO);
                    break;
                }
                case 10: { // Strona końcowa samouczka.
                    // Rysuje ramkę wewnętrzną.
                    ConsoleUtils::drawBox(15, 8, 50, 10, COLOR_BORDER);
                    
                    // Wyświetla komunikat o gotowości do gry.
                    ConsoleUtils::writeStringWide(25, 10, L"♠ ♥ You're Ready to Play! ♦ ♣", COLOR_SUCCESS);
                    ConsoleUtils::writeStringWide(20, 12, L"   Press ESC to return to the main menu", COLOR_DEFAULT);
                    ConsoleUtils::writeStringWide(20, 14, L"and start your first Solitaire game.", COLOR_DEFAULT);
                    
                    // Rysuje 4 Asy w rogach ramki.
                    drawFullCard(17, 9, 'S', 1, true);
                    drawFullCard(60, 9, 'H', 1, true);
                    drawFullCard(17, 16, 'C', 1, true);
                    drawFullCard(60, 16, 'D', 1, true);
                    break;
                }
            }

            // Wyświetla tekst bieżącej lekcji poniżej przykładów.
            ConsoleUtils::writeStringWide(10, 20, lessons[page], COLOR_DEFAULT);

            // Wyświetla pomoc nawigacyjną na dole ekranu.
            std::string navHelp = "Use ← → arrow keys to navigate, ESC to exit tutorial.";
            ConsoleUtils::writeString((CONSOLE_WIDTH - static_cast<int>(navHelp.length())) / 2, CONSOLE_HEIGHT - 2, navHelp, COLOR_INFO); // Bezpieczne rzutowanie.
            
            // Renderuje całą stronę samouczka.
            ConsoleUtils::renderBuffer();
        };
        
        // Renderuje pierwszą stronę samouczka na początku.
        renderTutorialPage(currentLesson);
        // Zapamiętuje czas ostatniej klatki dla kontroli szybkości animacji.
        auto lastFrameTime = std::chrono::steady_clock::now();
        
        // Główna pętla samouczka.
        while (tutorialActive) {
            // Pobiera bieżący czas.
            auto currentTime = std::chrono::steady_clock::now();
            // Oblicza czas, jaki upłynął od ostatniej klatki.
            auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - lastFrameTime).count();
            
            // Jeśli upłynął wystarczający czas (np. > 100ms), aktualizuje animację.
            if (elapsed > 100) {
                animationFrame++; // Zwiększa licznik klatek animacji.
                renderTutorialPage(currentLesson); // Przerysowuje stronę z nową klatką animacji.
                lastFrameTime = currentTime; // Aktualizuje czas ostatniej klatki.
            }
            
            // Sprawdza, czy naciśnięto klawisz (nieblokujące).
            if (_kbhit()) {
                // Odczytuje naciśnięty klawisz.
                int key = ConsoleUtils::getKeyPress();
                
                // Obsługuje klawisze nawigacyjne i interakcji.
                switch (key) {
                    case KEY_LEFT: // Strzałka w lewo.
                        // Przechodzi do poprzedniej lekcji, jeśli to nie pierwsza.
                        if (currentLesson > 0) {
                            currentLesson--;
                            renderTutorialPage(currentLesson); // Renderuje nową stronę.
                        }
                        break;
                    case KEY_RIGHT: // Strzałka w prawo.
                        // Przechodzi do następnej lekcji, jeśli to nie ostatnia.
                        if (currentLesson < static_cast<int>(lessons.size()) - 1) { // Bezpieczne rzutowanie.
                            currentLesson++;
                            renderTutorialPage(currentLesson); // Renderuje nową stronę.
                        }
                        break;
                    case KEY_ESC: // Escape.
                        // Kończy samouczek.
                        tutorialActive = false;
                        break;
                    case KEY_ENTER: // Enter.
                        // Uruchamia demonstrację na stronach 2 i 5.
                        if (currentLesson == 2 || currentLesson == 5) {
                            // Przewija animację o 40 klatek.
                            for (int i = 0; i < 40; i++) {
                                animationFrame++;
                                renderTutorialPage(currentLesson);
                                ConsoleUtils::sleep(50); // Opóźnienie między klatkami demonstracji.
                            }
                        }
                        break;
                    // Klawisze wspomagaczy (P, U, S, V, J, H).
                    case 'P': case 'p': case 'U': case 'u': case 'S': case 's':
                    case 'V': case 'v': case 'J': case 'j': case 'H': case 'h':
                        // Jeśli jest na stronie o wspomagaczach (lekcja 7).
                        if (currentLesson == 7) {
                            // Pobiera dużą literę naciśniętego klawisza.
                            char powerup = static_cast<char>(toupper(key)); // Bezpieczne rzutowanie.
                            // Definicje szczegółów wspomagaczy.
                            std::map<char, std::pair<std::string, std::wstring>> powerupDetails = {
                                {'U', {"UNDO", L"Takes back your last move. Useful when you make a mistake."}},
                                {'S', {"SHUFFLE", L"Reshuffles stock and waste piles. Helpful when you run out of moves."}},
                                {'V', {"REVEAL", L"Reveals face-down cards in tableau. Great for planning your next moves."}},
                                {'J', {"JOKER", L"Creates a wild card that can substitute for any card you need."}},
                                {'H', {"HINT", L"Highlights a valid move you can make when you're stuck."}}
                            };
                            
                            // Sprawdza, czy naciśnięty klawisz odpowiada znanemu wspomagaczowi.
                            if (powerupDetails.find(powerup) != powerupDetails.end()) {
                                // Wyświetla szczegółowy opis wspomagacza.
                                ConsoleUtils::clearBuffer(); // Czyści ekran.
                                ConsoleUtils::drawBox(10, 8, 60, 10, COLOR_BORDER); // Rysuje ramkę.
                                // Wypisuje nazwę wspomagacza.
                                ConsoleUtils::writeString(12, 9, powerupDetails[powerup].first + " POWER-UP", COLOR_TITLE);
                                // Wypisuje opis.
                                ConsoleUtils::writeStringWide(12, 11, powerupDetails[powerup].second, COLOR_DEFAULT);
                                // Wyświetla instrukcję powrotu.
                                ConsoleUtils::writeString(12, 16, "Press any key to return to power-ups page", COLOR_INFO);
                                ConsoleUtils::renderBuffer(); // Renderuje ekran opisu.
                                
                                // Czeka na naciśnięcie dowolnego klawisza.
                                ConsoleUtils::getKeyPress();
                                // Renderuje ponownie stronę samouczka o wspomagaczach.
                                renderTutorialPage(currentLesson);
                            }
                        }
                        break;
                }
            }
            
            // Krótkie opóźnienie w pętli, aby nie obciążać procesora.
            ConsoleUtils::sleep(10);
        }
    }

    // Wyświetla menu opcji gry.
    void showOptions() {
         // Flaga kontrolująca pętlę menu opcji.
         bool optionsOpen = true;
         // Referencja do obiektu gracza dla łatwiejszego dostępu.
         Player& playerRef = game.getPlayer();
         
         // Pętla menu opcji.
         while (optionsOpen && running) { // Działa dopóki opcje są otwarte i gra działa.
             // Czyści bufor i rysuje ramkę.
             ConsoleUtils::clearBuffer();
             ConsoleUtils::drawBox(0, 0, CONSOLE_WIDTH, CONSOLE_HEIGHT, COLOR_BORDER);
             
             // Wyświetla tytuł menu opcji.
             std::string title = " SETTINGS ";
             ConsoleUtils::writeString((CONSOLE_WIDTH - static_cast<int>(title.length())) / 2, 1, title, COLOR_TITLE | NEO_BACKGROUND_BLUE); // Bezpieczne rzutowanie.
             
             // Tworzy dynamicznie listę opcji na podstawie bieżących ustawień gracza.
             std::vector<std::string> options = {
                 "Easy Mode: " + std::string(playerRef.getEasyMode() ? "ON" : "OFF"), // Opcja trybu łatwego.
                 "Powerups: " + std::string(playerRef.getPowerupsEnabled() ? "ON" : "OFF"), // Opcja wspomagaczy.
                 "Change Player Name", // Opcja zmiany nazwy gracza.
                 "Reset High Score", // Opcja resetowania rekordu.
                 "Save Settings", // Opcja zapisu ustawień.
                 "Back" // Opcja powrotu.
             };
             
             // Wyświetla menu opcji i pobiera wybór gracza.
             int choice = ui.runMenu("Settings", options, -1, -1); // Centruje menu.
             
             // Obsługuje wybór gracza.
             switch (choice) {
                case 0: // Przełącz Tryb Łatwy.
                    playerRef.toggleEasyMode(); // Zmienia ustawienie w obiekcie gracza.
                    // Wyświetla komunikat potwierdzający zmianę.
                    ui.setStatusMessage(L"Easy mode " + std::wstring(playerRef.getEasyMode() ? L"enabled." : L"disabled."), 1500);
                    // Odtwarza dźwięk potwierdzenia.
                    ConsoleUtils::playSound(1000, 80);
                    break;
                case 1: // Przełącz Wspomagacze.
                    playerRef.togglePowerupsEnabled(); // Zmienia ustawienie.
                    // Wyświetla komunikat.
                    ui.setStatusMessage(L"Powerups " + std::wstring(playerRef.getPowerupsEnabled() ? L"enabled." : L"disabled."), 1500);
                    // Odtwarza dźwięk.
                    ConsoleUtils::playSound(1000, 80);
                    break;
                case 2: // Zmień Nazwę Gracza.
                    { // Tworzy lokalny zakres dla zmiennej newName.
                        // Wyświetla pole do wprowadzania tekstu i pobiera nową nazwę.
                        std::string newName = ConsoleUtils::getTextInput(CONSOLE_WIDTH / 2 - 10, CONSOLE_HEIGHT / 2, 20, playerRef.getName()); 
                        // Jeśli nowa nazwa nie jest pusta (użytkownik nie anulował).
                        if (!newName.empty()) {
                            playerRef.setName(newName); // Ustawia nową nazwę.
                            // Wyświetla komunikat z nową nazwą.
                            ui.setStatusMessage(L"Name changed to " + std::wstring(newName.begin(), newName.end()), 1500);
                            // Odtwarza dźwięk.
                            ConsoleUtils::playSound(1000, 80);
                        }
                        // Jeśli nowa nazwa jest pusta, nic nie robi.
                    }
                    break;
                case 3: // Resetuj Rekord.
                    { // Tworzy lokalny zakres.
                        // Wyświetla menu potwierdzenia.
                        std::vector<std::string> confirmOptions = {"Yes, reset score.", "No, keep score."};
                        int confirm = ui.runMenu("Reset High Score?", confirmOptions, -1, -1);
                        // Jeśli wybrano "Tak".
                        if (confirm == 0) {
                            playerRef.resetHighScore(); // Resetuje rekord.
                            // Wyświetla komunikat.
                            ui.setStatusMessage(L"High score reset to 0.", 1500);
                            // Odtwarza dźwięk (niższy, bo to akcja potencjalnie negatywna).
                            ConsoleUtils::playSound(200, 150);
                        }
                        // Jeśli wybrano "Nie", nic nie robi.
                    }
                    break;
                case 4: // Zapisz Ustawienia.
                    playerRef.saveToFile(); // Zapisuje bieżące ustawienia gracza do pliku.
                    ui.setStatusMessage(L"Settings saved.", 1500); // Wyświetla komunikat.
                    ConsoleUtils::playSound(1000, 80); // Dźwięk potwierdzenia.
                    break;
                case 5: // Powrót.
                case -1: // Anulowanie (Escape).
                    optionsOpen = false; // Kończy pętlę menu opcji.
                    break;
             }
             // Pętla kontynuuje, dopóki optionsOpen jest true.
         }
         // Po wyjściu z menu opcji, przerysowuje planszę gry.
         ui.drawBoard();
         ConsoleUtils::renderBuffer();
    }

    // Rozpoczyna nową grę (inicjalizuje stan, obsługuje samouczek dla nowych graczy, animuje rozdanie).
    void startGame() {
         try {
             game.newGame(); // Inicjalizuje nową grę w logice (tasowanie, rozdawanie).
             ui.clearSelections(); // Czyści zaznaczenia w UI.
             sourceSelection.clear(); // Czyści zaznaczenie źródła w kontrolerze.
             ui.clearStatusMessage(); // Czyści pasek statusu.

             // Sprawdza, czy gracz gra po raz pierwszy.
             if (game.getPlayer().isFirstTimePlayer()) {
                 // Wyświetla ekran powitalny z pytaniem o samouczek.
                 ConsoleUtils::clearBuffer();
                 ConsoleUtils::drawBox(0, 0, CONSOLE_WIDTH, CONSOLE_HEIGHT, COLOR_BORDER);
                 
                 std::string title = " Welcome to NeoSolitaire! ";
                 ConsoleUtils::writeString((CONSOLE_WIDTH - static_cast<int>(title.length())) / 2, 1, title, COLOR_TITLE | NEO_BACKGROUND_BLUE); // Bezpieczne rzutowanie.
                 
                 int centerY = CONSOLE_HEIGHT / 2 - 4; // Pozycja Y dla tekstu.
                 std::vector<std::string> messages = {
                     "It looks like this is your first time playing.",
                     "Would you like to go through the tutorial first?",
                     "This will help you understand the rules and controls."
                 };
                 
                 // Wypisuje wiadomości powitalne.
                 for (size_t i = 0; i < messages.size(); i++) {
                     ConsoleUtils::writeString((CONSOLE_WIDTH - static_cast<int>(messages[i].length())) / 2, centerY + static_cast<int>(i), messages[i], COLOR_DEFAULT); // Bezpieczne rzutowanie.
                 }
                 
                 // Wyświetla menu z pytaniem o samouczek.
                 std::vector<std::string> options = {"Yes, show tutorial.", "No, start playing."};
                 int choice = ui.runMenu("Welcome", options, -1, centerY + 5); // Centruje menu poniżej tekstu.
                 
                 // Jeśli wybrano "Tak".
                 if (choice == 0) {
                     // Wyświetla samouczek.
                     showTutorial();
                 }
                 // Jeśli wybrano "Nie" lub samouczek się zakończył.

                 // Oznacza gracza jako nie-nowego i zapisuje zmianę.
                 game.getPlayer().setFirstTimePlayer(false);
                 game.getPlayer().saveToFile();
             }

             // Wykonuje animację początkowego rozdania kart.
             ui.animateInitialDeal();

             // Rysuje finalny stan planszy po rozdaniu.
             ui.drawBoard();
             ConsoleUtils::renderBuffer();
         } catch (const std::exception& e) {
             // Obsługuje błędy krytyczne podczas inicjalizacji gry.
             ConsoleUtils::cleanupConsole(); // Sprząta konsolę.
             // Wypisuje błąd do standardowego wyjścia błędów.
             std::cerr << "FATAL ERROR during game setup: " << e.what() << std::endl;
             running = false; // Zatrzymuje główną pętlę gry.
         }
    }

public:
    // Konstruktor kontrolera gry. Inicjalizuje logikę i interfejs.
    GameController() : game(), ui(game), running(true) {}

    // Główna funkcja uruchamiająca grę.
    void run() {
        // Wyświetla ekran powitalny.
        displaySplash(); 

        // Wyświetla menu główne na starcie.
        showMainMenu();

        // Główna pętla gry. Działa dopóki flaga `running` jest true.
        while (running) {
            // Sprawdza, czy gra jest zakończona.
            if (game.isGameOver() || game.isGameWon()) {
                // Jeśli tak, używa specjalnej logiki przetwarzania wejścia dla stanu końca gry.
                processInput(); 
            } else {
                // Jeśli gra jest w toku, używa standardowej logiki przetwarzania wejścia.
                 processInput();
            }

            // Krótkie opóźnienie w pętli, aby ograniczyć użycie procesora (ok. 60 FPS).
            ConsoleUtils::sleep(16); 
        }

        // Po zakończeniu głównej pętli (np. przez wybór "Exit" w menu).
        game.getPlayer().updateHighScore(); // Ostateczna aktualizacja rekordu.
        game.getPlayer().saveToFile(); // Zapisuje stan gracza.

        // Przywraca oryginalne ustawienia konsoli.
        ConsoleUtils::cleanupConsole();
        // Wyświetla komunikat pożegnalny.
        std::cout << "\nThanks for playing NeoSolitaire!\n";
    }
}; // Koniec klasy GameController.


// Główna funkcja programu.
int main() {
    // Ustawia stronę kodową konsoli na UTF-8 dla poprawnego wyświetlania znaków.
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);

    try {
        // Tworzy obiekt kontrolera gry.
        GameController controller;
        // Uruchamia główną pętlę gry.
        controller.run(); 
    } catch (const std::exception& e) {
        // Obsługuje znane wyjątki (np. std::runtime_error).
        ConsoleUtils::cleanupConsole(); // Sprząta konsolę w razie błędu.
        // Wypisuje komunikat o błędzie do standardowego wyjścia błędów.
        std::cerr << "\n\nAn unexpected error occurred: " << e.what() << std::endl;
        std::cerr << "Exiting." << std::endl;
        return 1; // Zwraca kod błędu.
    } catch (...) {
        // Obsługuje nieznane wyjątki (np. błędy systemowe).
        ConsoleUtils::cleanupConsole(); // Sprząta konsolę.
        // Wypisuje ogólny komunikat o błędzie krytycznym.
        std::cerr << "\n\nAn unknown fatal error occurred. Exiting." << std::endl;
        return 2; // Zwraca inny kod błędu.
    }

    // Jeśli program zakończył się pomyślnie, zwraca 0.
    return 0; 
}
