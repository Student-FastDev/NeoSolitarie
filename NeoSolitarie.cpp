#define _WIN32_WINNT 0x0500
#include <conio.h>
#include <windows.h>

#include <algorithm>
#include <chrono>
#include <cmath>
#include <ctime>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <map>
#include <random>
#include <sstream>
#include <stdexcept>
#include <string>
#include <thread>
#include <vector>
#include <optional> 

const int CONSOLE_WIDTH = 120;
const int CONSOLE_HEIGHT = 45;

const WORD NEO_FOREGROUND_BLACK = 0;
const WORD NEO_FOREGROUND_BLUE = 1;
const WORD NEO_FOREGROUND_GREEN = 2;
const WORD NEO_FOREGROUND_CYAN = 3;
const WORD NEO_FOREGROUND_RED = 4;
const WORD NEO_FOREGROUND_MAGENTA = 5;
const WORD NEO_FOREGROUND_YELLOW = 6;
const WORD NEO_FOREGROUND_WHITE = 7;
const WORD NEO_FOREGROUND_INTENSITY = 8;

const WORD NEO_BACKGROUND_BLACK = 0;
const WORD NEO_BACKGROUND_BLUE = 16;
const WORD NEO_BACKGROUND_GREEN = 32;
const WORD NEO_BACKGROUND_CYAN = 48;
const WORD NEO_BACKGROUND_RED = 64;
const WORD NEO_BACKGROUND_MAGENTA = 80;
const WORD NEO_BACKGROUND_YELLOW = 96;
const WORD NEO_BACKGROUND_WHITE = 112;
const WORD NEO_BACKGROUND_INTENSITY = 128;

const WORD COLOR_DEFAULT = NEO_FOREGROUND_WHITE;
const WORD COLOR_BORDER = NEO_FOREGROUND_BLUE | NEO_FOREGROUND_INTENSITY;
const WORD COLOR_TITLE = NEO_FOREGROUND_YELLOW | NEO_FOREGROUND_INTENSITY;
const WORD COLOR_LABEL = NEO_FOREGROUND_CYAN | NEO_FOREGROUND_INTENSITY;
const WORD COLOR_VALUE = NEO_FOREGROUND_WHITE | NEO_FOREGROUND_INTENSITY;
const WORD COLOR_CARD_RED = NEO_FOREGROUND_RED | NEO_FOREGROUND_INTENSITY;
const WORD COLOR_CARD_BLACK = NEO_FOREGROUND_WHITE | NEO_FOREGROUND_INTENSITY;
const WORD COLOR_CARD_BACK = NEO_FOREGROUND_BLUE | NEO_FOREGROUND_INTENSITY;
const WORD COLOR_CARD_EMPTY = NEO_FOREGROUND_WHITE;

const WORD COLOR_HIGHLIGHTS = NEO_BACKGROUND_YELLOW | NEO_FOREGROUND_BLACK;
const WORD COLOR_HINT_HIGHLIGHT = NEO_BACKGROUND_GREEN | NEO_FOREGROUND_BLACK; 
const WORD COLOR_ERROR = NEO_FOREGROUND_RED | NEO_FOREGROUND_INTENSITY;
const WORD COLOR_SUCCESS = NEO_FOREGROUND_GREEN | NEO_FOREGROUND_INTENSITY;
const WORD COLOR_INFO = NEO_FOREGROUND_YELLOW | NEO_FOREGROUND_INTENSITY;
const WORD COLOR_MENU_ITEM = NEO_FOREGROUND_WHITE;
const WORD COLOR_MENU_SELECTED = NEO_BACKGROUND_WHITE | NEO_FOREGROUND_BLACK;
const WORD COLOR_STATUS_BAR = NEO_BACKGROUND_WHITE | NEO_FOREGROUND_BLACK;

const int KEY_UP = 256 + 72;
const int KEY_DOWN = 256 + 80;
const int KEY_LEFT = 256 + 75;
const int KEY_RIGHT = 256 + 77;
const int KEY_ENTER = 13;
const int KEY_ESC = 27;
const int KEY_SPACE = 32;
const int KEY_H_LOWER = 'h'; 
const int KEY_H_UPPER = 'H'; 

const int KEY_NAV_K_LOWER = 'k';
const int KEY_NAV_K_UPPER = 'K';
const int KEY_NAV_J_LOWER = 'j';
const int KEY_NAV_J_UPPER = 'J';
const int KEY_NAV_H_LOWER = 'h'; 
const int KEY_NAV_H_UPPER = 'H'; 
const int KEY_NAV_L_LOWER = 'l';
const int KEY_NAV_L_UPPER = 'L';

const int ANIMATION_DELAY_MS = 10; 
const int MOVE_ANIMATION_STEPS = 15; 
const int DEAL_ANIMATION_DELAY_MS = 30; 
const int WIN_ANIMATION_DELAY_MS = 50; 
const int HINT_FLASH_DELAY_MS = 150;
const int HINT_FLASH_COUNT = 3;
const int POWER_ANIMATION_STEPS = 8; 
const int POWERUP_ANIMATION_DELAY = 20; 
const bool ENABLE_ANIMATION = true; 

class ConsoleUI;
class SolitaireGame;

namespace ConsoleUtils {
HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
HANDLE hInput = GetStdHandle(STD_INPUT_HANDLE);
CONSOLE_SCREEN_BUFFER_INFO consoleScreenBufferInfo;
CONSOLE_CURSOR_INFO cursorInfo;
SMALL_RECT windowSize = {0, 0, CONSOLE_WIDTH - 1, CONSOLE_HEIGHT - 1};
COORD bufferSize = {CONSOLE_WIDTH, CONSOLE_HEIGHT};
CHAR_INFO* consoleBuffer = nullptr;
COORD zeroCoordinate = {0, 0};

void clearBuffer() {
    if (!consoleBuffer)
        return;
    for (int i = 0; i < CONSOLE_WIDTH * CONSOLE_HEIGHT; ++i) {
        consoleBuffer[i].Char.UnicodeChar = L' ';
        consoleBuffer[i].Attributes = COLOR_DEFAULT;
    }
}

void initConsole() {
    SetConsoleTitle(TEXT("NeoSolitaire"));

    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);

    delete[] consoleBuffer;
    consoleBuffer = new CHAR_INFO[CONSOLE_WIDTH * CONSOLE_HEIGHT];
    clearBuffer();

    SetConsoleScreenBufferSize(hConsole, bufferSize);
    SetConsoleWindowInfo(hConsole, TRUE, &windowSize);

    HWND consoleWindow = GetConsoleWindow();
    if (consoleWindow != NULL) {
        SetWindowLong(consoleWindow, GWL_STYLE, GetWindowLong(consoleWindow, GWL_STYLE) & ~WS_MAXIMIZEBOX & ~WS_SIZEBOX);
    }

    GetConsoleCursorInfo(hConsole, &cursorInfo);
    cursorInfo.bVisible = false;
    SetConsoleCursorInfo(hConsole, &cursorInfo);

    DWORD previousMode;
    GetConsoleMode(hInput, &previousMode);
    SetConsoleMode(hInput, ENABLE_EXTENDED_FLAGS | ENABLE_PROCESSED_INPUT | (previousMode & ~ENABLE_QUICK_EDIT_MODE));
}

void cleanupConsole() {

    CONSOLE_CURSOR_INFO originalCursorInfo;
    if (GetConsoleCursorInfo(hConsole, &originalCursorInfo)) {
        originalCursorInfo.bVisible = true;
        SetConsoleCursorInfo(hConsole, &originalCursorInfo);
    }
    delete[] consoleBuffer;
    consoleBuffer = nullptr;
}


void renderBuffer() {
    if (!consoleBuffer)
        return;
    WriteConsoleOutputW(hConsole, consoleBuffer, bufferSize, zeroCoordinate, &windowSize);
}

void setChar(int x, int y, wchar_t c, WORD attributes = COLOR_DEFAULT) {
    if (consoleBuffer && x >= 0 && x < CONSOLE_WIDTH && y >= 0 && y < CONSOLE_HEIGHT) {
        consoleBuffer[y * CONSOLE_WIDTH + x].Char.UnicodeChar = c;
        consoleBuffer[y * CONSOLE_WIDTH + x].Attributes = attributes;
    }
}

void writeString(int x, int y, const std::string& str, WORD attributes = COLOR_DEFAULT) {
    int currentX = x;
    for (size_t i = 0; i < str.length(); ++i) {
        wchar_t currentCharacter = L'?';
        unsigned char firstChar = str[i];
        if (firstChar < 0x80) {
            currentCharacter = (wchar_t)firstChar;
        } else if (firstChar >= 0xC2 && firstChar <= 0xDF && i + 1 < str.length()) {
            unsigned char secondChar = str[i + 1];
            if (secondChar >= 0x80 && secondChar <= 0xBF) {
                currentCharacter = ((wchar_t)(firstChar & 0x1F) << 6) | (wchar_t)(secondChar & 0x3F);
                i++;
            }
        } else if (firstChar >= 0xE0 && firstChar <= 0xEF && i + 2 < str.length()) {
            unsigned char secondChar = str[i + 1];
            unsigned char thirdChar = str[i + 2];
            if (secondChar >= 0x80 && secondChar <= 0xBF && thirdChar >= 0x80 && thirdChar <= 0xBF) {

                if (firstChar == 0xE2 && secondChar == 0x99) { 
                    if (thirdChar == 0xA5) currentCharacter = L'♥'; 
                    else if (thirdChar == 0xA6) currentCharacter = L'♦'; 
                    else if (thirdChar == 0xA3) currentCharacter = L'♣'; 
                    else if (thirdChar == 0xA0) currentCharacter = L'♠'; 
                    else currentCharacter = ((wchar_t)(firstChar & 0x0F) << 12) | ((wchar_t)(secondChar & 0x3F) << 6) | (wchar_t)(thirdChar & 0x3F);
                } else {
                    currentCharacter = ((wchar_t)(firstChar & 0x0F) << 12) | ((wchar_t)(secondChar & 0x3F) << 6) | (wchar_t)(thirdChar & 0x3F);
                }
                i += 2;
            }
        }

        setChar(currentX++, y, currentCharacter, attributes);
        if (currentX >= CONSOLE_WIDTH)
            break;
    }
}


void writeStringWide(int x, int y, const std::wstring& wstr, WORD attributes = COLOR_DEFAULT) {
    for (size_t i = 0; i < wstr.length(); ++i) {
        if (x + static_cast<int>(i) >= CONSOLE_WIDTH)
            break;
        setChar(x + i, y, wstr[i], attributes);
    }
}


void fillRectangle(int x, int y, int width, int height, wchar_t c = L' ', WORD attributes = COLOR_DEFAULT) {
    if (x < 0) { width += x; x = 0; }
    if (y < 0) { height += y; y = 0; }
    if (x + width > CONSOLE_WIDTH) width = CONSOLE_WIDTH - x;
    if (y + height > CONSOLE_HEIGHT) height = CONSOLE_HEIGHT - y;
    if (width <= 0 || height <= 0) return;

    for (int j = y; j < y + height; ++j) {
        for (int i = x; i < x + width; ++i) {
            setChar(i, j, c, attributes);
        }
    }
}

void drawBox(int x, int y, int width, int height, WORD attributes = COLOR_BORDER, bool doubleBorder = false) {
    wchar_t topLeftChar, topRightChar, bottomLeftChar, bottomRightChar, horizontalChar, verticalChar;
    if (doubleBorder) {
        topLeftChar = L'╔'; topRightChar = L'╗'; bottomLeftChar = L'╚'; bottomRightChar = L'╝';
        horizontalChar = L'═'; verticalChar = L'║';
    } else {
        topLeftChar = L'┌'; topRightChar = L'┐'; bottomLeftChar = L'└'; bottomRightChar = L'┘';
        horizontalChar = L'─'; verticalChar = L'│';
    }

    if (width <= 0 || height <= 0) return;

    setChar(x, y, topLeftChar, attributes);
    if (width > 1) setChar(x + width - 1, y, topRightChar, attributes);
    if (height > 1) setChar(x, y + height - 1, bottomLeftChar, attributes);
    if (width > 1 && height > 1) setChar(x + width - 1, y + height - 1, bottomRightChar, attributes);

    for (int i = x + 1; i < x + width - 1; ++i) {
        setChar(i, y, horizontalChar, attributes);
        if (height > 1) setChar(i, y + height - 1, horizontalChar, attributes);
    }
    for (int j = y + 1; j < y + height - 1; ++j) {
        setChar(x, j, verticalChar, attributes);
        if (width > 1) setChar(x + width - 1, j, verticalChar, attributes);
    }
}

int getKeyPress() {
    int key = _getch();

    if (key == 0 || key == 224) { 
        int secondKey = _getch();
        return 256 + secondKey;
    }
    return key;
}

void playSound(int freq, int duration) {
    Beep(freq, duration);
}

void sleep(int milliseconds) {
    std::this_thread::sleep_for(std::chrono::milliseconds(milliseconds));
}


std::string getTextInput(int startX, int startY, int maxLength, const std::string& initialValue = "") {
    std::string inputText = initialValue;
    COORD initialCursorPos = {(SHORT)startX, (SHORT)startY};
    CONSOLE_CURSOR_INFO originalCursorInfo;
    GetConsoleCursorInfo(hConsole, &originalCursorInfo); 
    CONSOLE_CURSOR_INFO editCursorInfo = originalCursorInfo;
    editCursorInfo.bVisible = true; 
    SetConsoleCursorInfo(hConsole, &editCursorInfo);

    auto redrawInput = [&]() {
        fillRectangle(startX, startY, maxLength + 1, 1, L' '); 
        writeString(startX, startY, inputText);
        SetConsoleCursorPosition(hConsole, {(SHORT)(startX + inputText.length()), (SHORT)startY});
        renderBuffer(); 
    };

    redrawInput();

    while (true) {
        int key = getKeyPress();

        if (key == KEY_ENTER) {
            break;
        } else if (key == KEY_ESC) {
            inputText = initialValue; 
            break;
        } else if (key == 8 || key == 127) { 
            if (!inputText.empty()) {
                inputText.pop_back();
                redrawInput();
            }
        } else if (key >= 32 && key <= 126) { 
            if (inputText.length() < (size_t)maxLength) {
                inputText += static_cast<char>(key);
                redrawInput();
            }
        }

    }

    SetConsoleCursorInfo(hConsole, &originalCursorInfo); 
    return inputText;
}

} 

class Card {
private:
    char suit;
    int value;
    bool faceUp;
    bool isJoker;

public:
    Card() : suit('?'), value(0), faceUp(false), isJoker(false) {}

    Card(char s, int v, bool joker = false) : suit(s), value(v), faceUp(false), isJoker(joker) {}

    char getSuit() const { return suit; }
    int getValue() const { return value; }
    bool isFaceUp() const { return faceUp; }
    bool getIsJoker() const { return isJoker; }
    void flip() { faceUp = !faceUp; }
    void setFaceUp(bool up) { faceUp = up; }

    std::wstring getValueAsString() const {
        if (isJoker) return L"*";
        switch (value) {
        case 1: return L"A";
        case 10: return L"T";
        case 11: return L"J";
        case 12: return L"Q";
        case 13: return L"K";
        default: return std::to_wstring(value);
        }
    }

    std::wstring getSuitAsWString() const {
        if (isJoker) return L"J";
        switch (suit) {
        case 'H': return L"♥";
        case 'D': return L"♦";
        case 'C': return L"♣";
        case 'S': return L"♠";
        default: return L"?";
        }
    }

     std::wstring toStringShort() const {
        if(isJoker) return L"[J*]";
        if (!faceUp) {
            return L"[░]"; 
        }
        std::wstring valueString = getValueAsString();

        return L" " + (valueString.length() == 1 ? valueString : valueString.substr(0,1)) + getSuitAsWString();
    }


    std::pair<std::wstring, std::wstring> getDrawStrings() const {
        if(isJoker) return {L"*", L"J"};
        if (!faceUp) return {L" ", L" "}; 
        return {getValueAsString(), getSuitAsWString()};
    }

    bool isRed() const {
        return !isJoker && (suit == 'H' || suit == 'D');
    }

    WORD getColor(bool colorblindMode) const {
        if(isJoker) return NEO_FOREGROUND_MAGENTA | NEO_FOREGROUND_INTENSITY;
        if (!faceUp) return COLOR_CARD_BACK;
        if (isRed()) {
            return colorblindMode ? COLOR_CARD_BLACK : COLOR_CARD_RED;
        } else {
            return COLOR_CARD_BLACK;
        }
    }
};


class Player {
private:
    std::string name;
    int score;
    int highScore;
    int undosLeft;
    int shufflesLeft;
    int revealsLeft;
    int jokerCardsLeft;
    int hintsLeft; 
    bool colorblindMode;
    bool easyMode; 
    bool powerupsEnabled;
    bool firstTimePlayer; 
    std::chrono::time_point<std::chrono::steady_clock> gameStartTime;
    std::chrono::duration<double> gameDuration;
    bool timerRunning;

public:
    Player(const std::string& n = "Player")
        : name(n), score(0), highScore(0),
          colorblindMode(false), easyMode(false), powerupsEnabled(true),
          firstTimePlayer(true), 
          timerRunning(false), gameDuration(std::chrono::duration<double>::zero())
    {
        resetPowerups();
        loadFromFile();
    }

    void setName(const std::string& newName) { name = newName; }
    std::string getName() const { return name; }

    int getScore() const { return score; }
    void addScore(int points) { score = std::max(0, score + points); }
    void resetScore() { score = 0; }

    int getHighScore() const { return highScore; }
    void updateHighScore() { if (score > highScore) highScore = score; }

    int getUndosLeft() const { return undosLeft; }
    bool useUndo() {
        if (undosLeft > 0) {
            undosLeft--;
            return true;
        }
        return false;
    }
    void refundUndo() { undosLeft++; } 
    void resetUndos() { undosLeft = easyMode ? 5 : 3; }
    void setUndosLeft(int count) { undosLeft = std::max(0, count); }


    int getShufflesLeft() const { return shufflesLeft; }
    bool useShuffle() {
        if (shufflesLeft > 0) {
            shufflesLeft--;
            addScore(-20); 
            return true;
        }
        return false;
    }
    void resetShuffles() { shufflesLeft = easyMode ? 3 : 2; }
    void setShufflesLeft(int count) { shufflesLeft = std::max(0, count); }

    int getRevealsLeft() const { return revealsLeft; }
    bool useReveal() {
        if (revealsLeft > 0) {
            revealsLeft--;
             addScore(-30); 
            return true;
        }
        return false;
    }
    void resetReveals() { revealsLeft = easyMode ? 5 : 3; }
    void setRevealsLeft(int count) { revealsLeft = std::max(0, count); }


    int getJokerCardsLeft() const { return jokerCardsLeft; }
    bool useJokerCard() {
        if (jokerCardsLeft > 0) {
            jokerCardsLeft--;
             addScore(-50); 
            return true;
        }
        return false;
    }
     void refundJokerCard() { jokerCardsLeft++; } 
    void resetJokerCards() { jokerCardsLeft = easyMode ? 2 : 1; }
    void setJokerCardsLeft(int count) { jokerCardsLeft = std::max(0, count); }

    int getHintsLeft() const { return hintsLeft; }
    bool useHint() {
        if (hintsLeft > 0) {
            hintsLeft--;
            addScore(-10); 
            return true;
        }
        return false;
    }

    void refundHint() {
        hintsLeft++;
        addScore(10); 
    }
    void resetHints() { hintsLeft = easyMode ? 10 : 5; }
    void setHintsLeft(int count) { hintsLeft = std::max(0, count); }


    bool getColorblindMode() const { return colorblindMode; }
    void toggleColorblindMode() { colorblindMode = !colorblindMode; }

    bool getEasyMode() const { return easyMode; }
    void toggleEasyMode() {
        easyMode = !easyMode;
        resetPowerups(); 
    }

    void resetPowerups() {
        resetUndos();
        resetShuffles();
        resetReveals();
        resetJokerCards();
        resetHints(); 
    }

    void startGameTimer() {
        gameStartTime = std::chrono::steady_clock::now();
        timerRunning = true;
        gameDuration = std::chrono::duration<double>::zero(); 
    }

    void stopGameTimer() {
        if (timerRunning) {
            gameDuration = std::chrono::steady_clock::now() - gameStartTime;
            timerRunning = false;
        }
    }

    double getElapsedSeconds() const {
        if (timerRunning) {
            return std::chrono::duration_cast<std::chrono::duration<double>>(std::chrono::steady_clock::now() - gameStartTime).count();
        } else {
            return gameDuration.count();
        }
    }

    std::string getFormattedTime() const {
        double totalSeconds = getElapsedSeconds();
        int minutes = static_cast<int>(totalSeconds / 60);
        int seconds = static_cast<int>(std::fmod(totalSeconds, 60.0));
        std::stringstream ss;
        ss << std::setfill('0') << std::setw(2) << minutes << ":" << std::setw(2) << seconds;
        return ss.str();
    }

    void saveToFile() {
        std::string filename = "player_data.txt";
        std::ofstream file(filename);
        
        if (!file.is_open()) {
            return; 
        }
        
        file << name << "\n";
        file << highScore << "\n";
        file << (colorblindMode ? "1" : "0") << "\n";
        file << (easyMode ? "1" : "0") << "\n";
        file << (powerupsEnabled ? "1" : "0") << "\n";
        file << (firstTimePlayer ? "1" : "0") << "\n"; 
        
        file.close();
    }

    void loadFromFile() {
        std::string filename = "player_data.txt";
        std::ifstream file(filename);
        
        if (!file.is_open()) {
            return; 
        }
        
        std::string line;

        if (std::getline(file, line)) {
            name = line;
        }

        if (std::getline(file, line)) {
            try {
                highScore = std::stoi(line);
            } catch (...) {
                highScore = 0;
            }
        }

        if (std::getline(file, line)) {
            colorblindMode = (line == "1");
        }

        if (std::getline(file, line)) {
            easyMode = (line == "1");
        }

        if (std::getline(file, line)) {
            powerupsEnabled = (line == "1");
        } else {
            powerupsEnabled = true; 
        }

        if (std::getline(file, line)) {
            firstTimePlayer = (line == "1");
        } else {
            firstTimePlayer = true; 
        }
        
        resetPowerups();
        file.close();
    }

    bool getPowerupsEnabled() const { return powerupsEnabled; }
    void togglePowerupsEnabled() { 
        powerupsEnabled = !powerupsEnabled; 
        if (!powerupsEnabled) {

            undosLeft = 0;
            shufflesLeft = 0;
            revealsLeft = 0;
            jokerCardsLeft = 0;
            hintsLeft = 0;
        } else {

            resetPowerups();
        }
    }

    bool isFirstTimePlayer() const { return firstTimePlayer; }
    void setFirstTimePlayer(bool isFirst) { firstTimePlayer = isFirst; }
};

struct GameState {
    std::vector<std::vector<Card>> tableau;
    std::vector<Card> stock;
    std::vector<Card> waste;
    std::vector<std::vector<Card>> foundations;
    int score;
    int undosLeft;
    int shufflesLeft;
    int revealsLeft;
    int jokerCardsLeft;
    int hintsLeft; 
    int movesMade;
};

enum class SelectableType { NONE, STOCK, WASTE, FOUNDATION, TABLEAU_PILE, TABLEAU_CARD };

struct Selection {
    SelectableType type = SelectableType::NONE;
    int index = -1;      
    int cardIndex = -1;  

    void clear() {
        type = SelectableType::NONE;
        index = -1;
        cardIndex = -1;
    }
    bool isValid() const { return type != SelectableType::NONE; }

    bool operator==(const Selection& other) const {
        return type == other.type && index == other.index && cardIndex == other.cardIndex;
    }
    bool operator!=(const Selection& other) const {
        return !(*this == other);
    }
};

struct Hint {
    Selection source;
    Selection destination;
    std::wstring description; 
};

class SolitaireGame {
private:
    Player player;
    std::vector<std::vector<Card>> tableau; 
    std::vector<Card> stock;
    std::vector<Card> waste;
    std::vector<std::vector<Card>> foundations; 
    std::vector<GameState> history; 
    bool gameOver;
    bool gameWon;
    int movesMade;
    std::mt19937 rng; 

    void saveGameState() {

        if (history.size() > 50) {
            history.erase(history.begin());
        }
        GameState currentState;
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
        history.push_back(currentState);
    }

    void revealTopTableauCard(int pileIndex) {
        if (pileIndex >= 0 && pileIndex < 7 && !tableau[pileIndex].empty()) {
            if (!tableau[pileIndex].back().isFaceUp()) {
                tableau[pileIndex].back().flip();
                player.addScore(5); 
                ConsoleUtils::playSound(800, 50); 
            }
        }
    }

public:
    SolitaireGame() : gameOver(true), gameWon(false), movesMade(0), rng(static_cast<unsigned int>(std::time(nullptr))) {
        player.loadFromFile(); 
        foundations.resize(4);
        tableau.resize(7);
    }

    Player& getPlayer() { return player; }

    const std::vector<std::vector<Card>>& getTableau() const { return tableau; }
    const std::vector<Card>& getStock() const { return stock; }
    const std::vector<Card>& getWaste() const { return waste; }
    const std::vector<std::vector<Card>>& getFoundations() const { return foundations; }
    int getMovesMade() const { return movesMade; }
    bool isGameOver() const { return gameOver; }
    bool isGameWon() const { return gameWon; }

    std::mt19937& getRng() { return rng; } 

    void newGame() {

        for (auto& pile : tableau) pile.clear();
        stock.clear();
        waste.clear();
        for (auto& f : foundations) f.clear();
        history.clear(); 

        gameOver = false;
        gameWon = false;
        movesMade = 0;
        player.resetScore();
        player.resetPowerups(); 
        player.startGameTimer();

        std::vector<Card> deck;
        char suits[] = {'H', 'D', 'C', 'S'}; 
        for (char suit : suits) {
            for (int value = 1; value <= 13; value++) { 
                deck.emplace_back(suit, value);
            }
        }

        std::shuffle(deck.begin(), deck.end(), rng);

        int deckIndex = deck.size() - 1; 
        for (int i = 0; i < 7; i++) { 
            for (int j = 0; j <= i; j++) { 
                 if(deckIndex < 0) {

                    throw std::runtime_error("Deck ran out during tableau dealing!");
                 }

                 if (static_cast<size_t>(i) >= tableau.size()) tableau.resize(i + 1);
                tableau[i].push_back(deck[deckIndex--]);
            }
             if (!tableau[i].empty()) {
                 tableau[i].back().flip(); 
             }
        }


        if (deckIndex >= -1) { 
             deck.resize(deckIndex + 1);
        } else {
             deck.clear(); 
        }

        stock = deck; 
        for(Card& card : stock) {
            card.setFaceUp(false); 
        }


        saveGameState(); 
    }

    bool undoMove() {
        if (history.size() <= 1) { 
            ConsoleUtils::playSound(200, 150); 
            return false;
        }


        if (!player.useUndo()) {
             ConsoleUtils::playSound(200, 150); 
             return false;
        }

        history.pop_back(); 
        const GameState& previousState = history.back();

        tableau = previousState.tableau;
        stock = previousState.stock;
        waste = previousState.waste;
        foundations = previousState.foundations;

        player.resetScore(); 
        player.addScore(previousState.score); 

        player.setUndosLeft(previousState.undosLeft); 
        player.setShufflesLeft(previousState.shufflesLeft);
        player.setRevealsLeft(previousState.revealsLeft);
        player.setJokerCardsLeft(previousState.jokerCardsLeft);
        player.setHintsLeft(previousState.hintsLeft); 
        movesMade = previousState.movesMade;



        ConsoleUtils::playSound(600, 80); 
        return true;
    }


    bool canMoveWasteToFoundation() {
        if (waste.empty()) return false;
        const Card& card = waste.back();
        int foundationIndex = -1;

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
            } else if (foundations[i].back().getSuit() == card.getSuit()) {

                foundationIndex = i;
                break;
            }
        }

        if (foundationIndex == -1) return false;

        if (foundations[foundationIndex].empty()) {
            return card.getValue() == 1; 
        } else {

            return foundations[foundationIndex].back().getSuit() == card.getSuit() &&
                   foundations[foundationIndex].back().getValue() == card.getValue() - 1;
        }
    }

    bool canMoveTableauToFoundation(int tableauIndex) {
        if (tableauIndex < 0 || tableauIndex >= 7 || tableau[tableauIndex].empty() || !tableau[tableauIndex].back().isFaceUp())
             return false;
        const Card& card = tableau[tableauIndex].back();
        int foundationIndex = -1;

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
        if (foundationIndex == -1) return false;

        if (foundations[foundationIndex].empty()) {
            return card.getValue() == 1;
        } else {
            return foundations[foundationIndex].back().getSuit() == card.getSuit() &&
                   foundations[foundationIndex].back().getValue() == card.getValue() - 1;
        }
    }

    bool canMoveWasteToTableau(int tableauIndex) {
        if (tableauIndex < 0 || tableauIndex >= 7 || waste.empty()) return false;
        const Card& cardToMove = waste.back();
        if (tableau[tableauIndex].empty()) {

            return cardToMove.getValue() == 13 || cardToMove.getIsJoker();
        } else {
            const Card& topCard = tableau[tableauIndex].back();


             return topCard.isFaceUp() &&
                   ( ( (topCard.getValue() == cardToMove.getValue() + 1) && 
                       (topCard.isRed() != cardToMove.isRed()) )           
                    || cardToMove.getIsJoker() 
                    || topCard.getIsJoker() ); 
        }
    }

    bool canMoveTableauToTableau(int fromIndex, int toIndex, int cardIndex) {
         if (fromIndex < 0 || fromIndex >= 7 || toIndex < 0 || toIndex >= 7 || fromIndex == toIndex) return false;

         if (tableau[fromIndex].empty() || cardIndex < 0 || static_cast<size_t>(cardIndex) >= tableau[fromIndex].size()) return false;


        const Card& cardToMove = tableau[fromIndex][cardIndex];

        if (!cardToMove.isFaceUp()) return false;

        if (tableau[toIndex].empty()) {

            return cardToMove.getValue() == 13 || cardToMove.getIsJoker();
        } else {
            const Card& topCard = tableau[toIndex].back();


             return topCard.isFaceUp() &&
                   ( ( (topCard.getValue() == cardToMove.getValue() + 1) && 
                       (topCard.isRed() != cardToMove.isRed()) )           
                     || cardToMove.getIsJoker() 
                     || topCard.getIsJoker() ); 
        }
    }

    bool canMoveFoundationToTableau(int foundationIndex, int tableauIndex) {
        if (foundationIndex < 0 || foundationIndex >= 4 || tableauIndex < 0 || tableauIndex >= 7 || foundations[foundationIndex].empty())
            return false;
        const Card& cardToMove = foundations[foundationIndex].back();

        if (tableau[tableauIndex].empty()) {

            return cardToMove.getValue() == 13 || cardToMove.getIsJoker();
        } else {
            const Card& topCard = tableau[tableauIndex].back();


             return topCard.isFaceUp() &&
                   ( ( (topCard.getValue() == cardToMove.getValue() + 1) && 
                       (topCard.isRed() != cardToMove.isRed()) )           
                     || topCard.getIsJoker() ); 
        }
    }


    bool dealFromStock() {
        saveGameState(); 
        if (stock.empty()) {

            if (waste.empty()) {
                history.pop_back(); 
                ConsoleUtils::playSound(200, 150); 
                return false; 
            }

            std::vector<Card> tempWaste = waste;
            waste.clear();
            stock = tempWaste;
            std::reverse(stock.begin(), stock.end());
            for (Card& card : stock) {
                card.setFaceUp(false);
            }
            player.addScore(-100); 
            movesMade++;
            ConsoleUtils::playSound(500, 100); 
            checkWinCondition(); 
            return true;
        } else {

            Card card = stock.back();
            stock.pop_back();
            card.setFaceUp(true);
            waste.push_back(card);
            movesMade++;
            player.addScore(0); 
            ConsoleUtils::playSound(700, 50); 
            checkWinCondition(); 
            return true;
        }
    }

    bool moveWasteToFoundation() {
        if (!canMoveWasteToFoundation()) {


            return false;
        }
        saveGameState();
        Card card = waste.back();
        waste.pop_back();

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


        if (foundationIndex != -1) {
            foundations[foundationIndex].push_back(card);
            player.addScore(10); 
            movesMade++;
            ConsoleUtils::playSound(900, 70); 
            checkWinCondition();
            return true;
        } else {

            history.pop_back(); 
            waste.push_back(card); 
            ConsoleUtils::playSound(200, 150); 
            return false;
        }
    }

    bool moveTableauToFoundation(int tableauIndex) {
        if (!canMoveTableauToFoundation(tableauIndex)) {

             return false;
        }
        saveGameState();
        Card card = tableau[tableauIndex].back();
        tableau[tableauIndex].pop_back();

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

        if (foundationIndex != -1) {
            foundations[foundationIndex].push_back(card);
            revealTopTableauCard(tableauIndex); 
            player.addScore(10); 
            movesMade++;
            ConsoleUtils::playSound(900, 70); 
            checkWinCondition();
            return true;
        } else {
            history.pop_back(); 
            tableau[tableauIndex].push_back(card); 
            ConsoleUtils::playSound(200, 150); 
            return false;
        }
    }

    bool moveWasteToTableau(int tableauIndex) {
        if (!canMoveWasteToTableau(tableauIndex)) {

             return false;
        }
        saveGameState();
        Card card = waste.back();
        waste.pop_back();
        tableau[tableauIndex].push_back(card);
        player.addScore(5); 
        movesMade++;
        ConsoleUtils::playSound(800, 60); 
        checkWinCondition();
        return true;
    }

    bool moveTableauToTableau(int fromIndex, int toIndex, int cardIndex) {
        if (!canMoveTableauToTableau(fromIndex, toIndex, cardIndex)) {

             return false;
        }
        saveGameState();

        auto itStart = tableau[fromIndex].begin() + cardIndex;
        auto itEnd = tableau[fromIndex].end();

        tableau[toIndex].insert(tableau[toIndex].end(), std::make_move_iterator(itStart), std::make_move_iterator(itEnd));

        tableau[fromIndex].erase(tableau[fromIndex].begin() + cardIndex, tableau[fromIndex].end());


        revealTopTableauCard(fromIndex); 
        player.addScore(3); 
        movesMade++;
        ConsoleUtils::playSound(800, 60); 
        checkWinCondition();
        return true;
    }

    bool moveFoundationToTableau(int foundationIndex, int tableauIndex) {
        if (!canMoveFoundationToTableau(foundationIndex, tableauIndex)) {

            return false;
        }
        saveGameState();
        Card card = foundations[foundationIndex].back();
        foundations[foundationIndex].pop_back();
        tableau[tableauIndex].push_back(card);
        player.addScore(-15); 
        movesMade++;
        ConsoleUtils::playSound(400, 100); 
        checkWinCondition(); 
        return true;
    }


    bool useShufflePowerup() {
        if (!player.getShufflesLeft() || !player.getPowerupsEnabled()) {
            return false;
        }

        std::vector<Card> combinedDeck;

        combinedDeck.insert(combinedDeck.end(), stock.begin(), stock.end());
        combinedDeck.insert(combinedDeck.end(), waste.begin(), waste.end());
        
        if (combinedDeck.empty()) {
            return false;
        }

        std::shuffle(combinedDeck.begin(), combinedDeck.end(), rng);

        saveGameState();

        stock = combinedDeck;
        waste.clear();

        bool success = player.useShuffle();
        movesMade++;
        
        return success;
    }

    bool useRevealPowerup() {
        if (!player.getRevealsLeft() || !player.getPowerupsEnabled()) {
            return false;
        }

        bool anyRevealed = false;

        saveGameState();

        for (auto& pile : tableau) {
            for (auto it = pile.rbegin(); it != pile.rend(); ++it) {
                if (!it->isFaceUp()) {
                    it->setFaceUp(true);
                    anyRevealed = true;
                    break;  
                }
            }
        }
        
        if (!anyRevealed) {

            return false;
        }

        bool success = player.useReveal();
        movesMade++;
        
        return success;
    }
    
    bool useJokerPowerup(int tableauIndex) {
        if (!player.getJokerCardsLeft() || !player.getPowerupsEnabled() || 
            tableauIndex < 0 || tableauIndex >= static_cast<int>(tableau.size())) {
            return false;
        }

        saveGameState();

        Card joker('X', 1, true);  
        joker.setFaceUp(true);

        tableau[tableauIndex].push_back(joker);

        bool success = player.useJokerCard();
        movesMade++;
        
        return success;
    }

    void checkWinCondition() {
         if(gameOver) return; 


        int cardsInFoundations = 0;
        bool allFoundationsComplete = true;
        for (const auto& foundation : foundations) {
            if (!foundation.empty() && foundation.back().getValue() == 13) { 
                 cardsInFoundations += foundation.size();
            } else {
                 allFoundationsComplete = false; 
                 break;
            }
        }

         if (allFoundationsComplete && cardsInFoundations == 52) {
             gameOver = true;
             gameWon = true;
             player.stopGameTimer();

             player.addScore(1000);

             double timeBonus = std::max(0.0, 600.0 - player.getElapsedSeconds()) * 2.0;
             player.addScore(static_cast<int>(timeBonus));
             player.updateHighScore();
             player.saveToFile(); 
         } else {
             gameWon = false;
         }
    }

    bool hasValidMovesLeft() const {

        if (!stock.empty()) return true;

         if (stock.empty() && !waste.empty()) return true;

        if (const_cast<SolitaireGame*>(this)->canMoveWasteToFoundation()) return true;
        for (int i = 0; i < 7; ++i) {
            if (const_cast<SolitaireGame*>(this)->canMoveWasteToTableau(i)) return true;
        }

        for (int i = 0; i < 7; ++i) {

            if (const_cast<SolitaireGame*>(this)->canMoveTableauToFoundation(i)) return true;

             if (!tableau[i].empty()) {
                 for (size_t cardIndex = 0; cardIndex < tableau[i].size(); ++cardIndex) {
                     if (tableau[i][cardIndex].isFaceUp()) {
                         for (int j = 0; j < 7; ++j) {
                             if (i == j) continue; 
                             if (const_cast<SolitaireGame*>(this)->canMoveTableauToTableau(i, j, static_cast<int>(cardIndex))) {
                                 return true;
                             }
                         }


                         break;
                     }
                 }
             }
        }

        for (int i = 0; i < 4; ++i) {
            for (int j = 0; j < 7; ++j) {
                if (const_cast<SolitaireGame*>(this)->canMoveFoundationToTableau(i, j)) return true;
            }
        }

        return false;
    }

    void checkForGameOver() {
        if (!gameOver && !gameWon && !hasValidMovesLeft()) {

             bool powerupsMightHelp =
                 (player.getShufflesLeft() > 0 && (!stock.empty() || !waste.empty())) 
                 || (player.getRevealsLeft() > 0 && std::any_of(tableau.begin(), tableau.end(), [](const auto& pile){ 
                      return std::any_of(pile.begin(), pile.end(), [](const Card& c){ return !c.isFaceUp(); });
                   }))
                 || player.getJokerCardsLeft() > 0 
                 || player.getHintsLeft() > 0; 


             if (!powerupsMightHelp) { 
                gameOver = true;
                gameWon = false;
                player.stopGameTimer();
                player.updateHighScore(); 
                player.saveToFile();
             }
        }
    }

    std::optional<Hint> getHint() {
        if (!player.getPowerupsEnabled()) {
            return std::nullopt;
        }
        
        if (!player.getHintsLeft()) {
            return std::nullopt;
        }
        
        player.useHint();










        for (int i = 0; i < 7; ++i) {
            if (canMoveTableauToFoundation(i)) {
                 int cardIndex = tableau[i].size() - 1;
                 int targetFoundation = -1;
                 const Card& card = tableau[i].back();
                 for(int f=0; f<4; ++f) { 
                     if ( (foundations[f].empty() && card.getValue() == 1) ||
                          (!foundations[f].empty() && foundations[f].back().getSuit() == card.getSuit()))
                     {
                         bool suitPresentElsewhere = false;
                         if (foundations[f].empty() && card.getValue() == 1) {
                             for(int j=0; j<4; ++j) if (i != j && !foundations[j].empty() && foundations[j][0].getSuit() == card.getSuit()) suitPresentElsewhere = true;
                         }
                         if (!suitPresentElsewhere) { targetFoundation = f; break; }
                     }
                 }
                 if (targetFoundation != -1) {
                     return Hint{
                         {SelectableType::TABLEAU_CARD, i, cardIndex},
                         {SelectableType::FOUNDATION, targetFoundation, -1},

                         L"Move " + card.toStringShort() + L" from Tableau " + std::to_wstring(i+1) + L" to Foundation."
                     };
                 }
            }
        }

        if (canMoveWasteToFoundation()) {
            const Card& card = waste.back();
            int targetFoundation = -1;
             for(int f=0; f<4; ++f) { 
                 if ( (foundations[f].empty() && card.getValue() == 1) ||
                      (!foundations[f].empty() && foundations[f].back().getSuit() == card.getSuit()))
                 {
                    bool suitPresentElsewhere = false;
                    if (foundations[f].empty() && card.getValue() == 1) {
                        for(int j=0; j<4; ++j) if (f != j && !foundations[j].empty() && foundations[j][0].getSuit() == card.getSuit()) suitPresentElsewhere = true;
                    }
                    if (!suitPresentElsewhere) { targetFoundation = f; break; }
                 }
             }
             if (targetFoundation != -1) {
                 return Hint{
                     {SelectableType::WASTE, 0, -1},
                     {SelectableType::FOUNDATION, targetFoundation, -1},

                     L"Move " + card.toStringShort() + L" from Waste to Foundation."
                 };
             }
        }

        for (int from = 0; from < 7; ++from) {
             if (tableau[from].size() > 1) { 
                 for (int cardIdx = 0; cardIdx < (int)tableau[from].size(); ++cardIdx) {
                     if (tableau[from][cardIdx].isFaceUp()) { 
                         if (cardIdx > 0 && !tableau[from][cardIdx - 1].isFaceUp()) { 
                            for (int to = 0; to < 7; ++to) {
                                 if (from == to) continue;
                                 if (canMoveTableauToTableau(from, to, cardIdx)) {
                                      return Hint{
                                         {SelectableType::TABLEAU_CARD, from, cardIdx},
                                         {SelectableType::TABLEAU_PILE, to, -1}, 

                                         L"Move stack from Tableau " + std::to_wstring(from+1) + L" to Tableau " + std::to_wstring(to+1) + L" (reveals card)."
                                     };
                                 }
                            }
                         }
                        break; 
                     }
                 }
             }
        }

         for (int from = 0; from < 7; ++from) {
            if (!tableau[from].empty()) {
                for (int cardIdx = 0; cardIdx < (int)tableau[from].size(); ++cardIdx) {
                     if (tableau[from][cardIdx].isFaceUp()) {
                        for (int to = 0; to < 7; ++to) {
                             if (from == to) continue;
                             if (canMoveTableauToTableau(from, to, cardIdx)) {

                                bool reveals = (cardIdx > 0 && static_cast<size_t>(cardIdx) < tableau[from].size() && !tableau[from][cardIdx-1].isFaceUp());
                                if (!reveals) { 
                                     return Hint{
                                         {SelectableType::TABLEAU_CARD, from, cardIdx},
                                         {SelectableType::TABLEAU_PILE, to, -1},

                                         L"Move stack from Tableau " + std::to_wstring(from+1) + L" to Tableau " + std::to_wstring(to+1) + L"."
                                     };
                                }
                             }
                        }
                        break; 
                     }
                 }
             }
         }

        for (int i = 0; i < 7; ++i) {
            if (canMoveWasteToTableau(i)) {
                const Card& card = waste.back();
                return Hint{
                    {SelectableType::WASTE, 0, -1},
                    {SelectableType::TABLEAU_PILE, i, -1},

                    L"Move " + card.toStringShort() + L" from Waste to Tableau " + std::to_wstring(i+1) + L"."
                };
            }
        }

        for (int f = 0; f < 4; ++f) {
            for (int t = 0; t < 7; ++t) {
                if (canMoveFoundationToTableau(f, t)) {
                    const Card& card = foundations[f].back();
                     return Hint{
                        {SelectableType::FOUNDATION, f, -1},
                        {SelectableType::TABLEAU_PILE, t, -1},

                        L"Move " + card.toStringShort() + L" from Foundation back to Tableau " + std::to_wstring(t+1) + L"."
                    };
                }
            }
        }

        if (!stock.empty()) {
            return Hint{
                {SelectableType::STOCK, 0, -1},
                {SelectableType::WASTE, 0, -1}, 

                L"Draw card from Stock."
            };
        }

         if (stock.empty() && !waste.empty()) {
             return Hint{
                {SelectableType::STOCK, 0, -1}, 
                {SelectableType::STOCK, 0, -1}, 

                L"Recycle Waste pile back to Stock."
            };
         }

        return std::nullopt;
    }

};


class ConsoleUI {
private:
    SolitaireGame& game;
    Player& player;
    Selection currentSelection; 
    Selection sourceSelection;  
    std::wstring statusMessage;  
    std::chrono::time_point<std::chrono::steady_clock> messageExpireTime; 

    const COORD STOCK_POS = {5, 3};
    const COORD WASTE_POS = {12, 3};
    const COORD FOUNDATION_POS[4] = {{30, 3}, {37, 3}, {44, 3}, {51, 3}};
    const COORD TABLEAU_POS[7] = {{5, 8}, {15, 8}, {25, 8}, {35, 8}, {45, 8}, {55, 8}, {65, 8}};
    const COORD SCORE_POS = {75, 3};
    const COORD TIME_POS = {75, 4};
    const COORD MOVES_POS = {75, 5};
    const COORD HIGHSCORE_POS = {75, 6};
    const COORD POWERUP_POS = {75, 8}; 
    const COORD STATUS_POS = {0, CONSOLE_HEIGHT - 2}; 
    const COORD HELP_POS = {0, CONSOLE_HEIGHT - 1};   

    const int CARD_WIDTH = 5;
    const int CARD_HEIGHT = 3;

    COORD getCoordsForSelection(const Selection& selection) const {
        switch (selection.type) {
        case SelectableType::STOCK: return STOCK_POS;
        case SelectableType::WASTE: return WASTE_POS;
        case SelectableType::FOUNDATION:
            if (selection.index >= 0 && selection.index < 4) return FOUNDATION_POS[selection.index];
            break;
        case SelectableType::TABLEAU_PILE: 
             if (selection.index >= 0 && selection.index < 7) {
                 return TABLEAU_POS[selection.index];
             }
             break;
        case SelectableType::TABLEAU_CARD:
             if (selection.index >= 0 && selection.index < 7) {
                 COORD basePos = TABLEAU_POS[selection.index];
                 const auto& pile = game.getTableau()[selection.index];
                 if (selection.cardIndex != -1 && static_cast<size_t>(selection.cardIndex) < pile.size()) {

                     int offsetY = 0;

                     for (int i = 0; i < selection.cardIndex; ++i) {

                         offsetY += (pile[i].isFaceUp() ? 2 : 1);

                         if (basePos.Y + offsetY > CONSOLE_HEIGHT - CARD_HEIGHT - 3) {
                             offsetY = CONSOLE_HEIGHT - CARD_HEIGHT - 3 - basePos.Y;
                             break;
                         }
                     }
                     return {basePos.X, static_cast<SHORT>(basePos.Y + offsetY)};
                 } else if (!pile.empty()) {

                     int lastCardIndex = pile.size() - 1;
                     int offsetY = 0;
                      for (int i = 0; i < lastCardIndex; ++i) {
                          offsetY += (pile[i].isFaceUp() ? 2 : 1);
                           if (basePos.Y + offsetY > CONSOLE_HEIGHT - CARD_HEIGHT - 3) {
                             offsetY = CONSOLE_HEIGHT - CARD_HEIGHT - 3 - basePos.Y;
                             break;
                         }
                      }
                     return {basePos.X, static_cast<SHORT>(basePos.Y + offsetY)};
                 } else {

                      return basePos;
                 }
             }
             break;
        default: break;
        }
        return {-1, -1}; 
    }

    void drawCard(int x, int y, const Card& card, bool highlighted, bool hintHighlight = false) {

        if (x < 0 || y < 0 || x + CARD_WIDTH > CONSOLE_WIDTH || y + CARD_HEIGHT > CONSOLE_HEIGHT) {
            return;
        }

        WORD borderAttribute = COLOR_BORDER;
        WORD backgroundAttribute = COLOR_DEFAULT; 
        WORD cardAttribute = card.getColor(player.getColorblindMode()); 

        if (hintHighlight) {
            borderAttribute = COLOR_HINT_HIGHLIGHT;
            backgroundAttribute = COLOR_HINT_HIGHLIGHT;

             if (card.isRed() && !player.getColorblindMode())
                cardAttribute = COLOR_CARD_RED | (COLOR_HINT_HIGHLIGHT & 0xFFF0); 
             else
                cardAttribute = COLOR_CARD_BLACK | (COLOR_HINT_HIGHLIGHT & 0xFFF0);
             if (!card.isFaceUp())
                 cardAttribute = COLOR_CARD_BACK | (COLOR_HINT_HIGHLIGHT & 0xFFF0);
             if (card.getIsJoker())
                 cardAttribute = (NEO_FOREGROUND_MAGENTA | NEO_FOREGROUND_INTENSITY) | (COLOR_HINT_HIGHLIGHT & 0xFFF0);

        } else if (highlighted) {
             borderAttribute = COLOR_HIGHLIGHTS;
             backgroundAttribute = COLOR_HIGHLIGHTS;

             if (card.isRed() && !player.getColorblindMode())
                cardAttribute = COLOR_CARD_RED | NEO_BACKGROUND_YELLOW;
             else
                cardAttribute = COLOR_CARD_BLACK | NEO_BACKGROUND_YELLOW;
             if (!card.isFaceUp())
                 cardAttribute = COLOR_CARD_BACK | NEO_BACKGROUND_YELLOW;
             if (card.getIsJoker())
                 cardAttribute = (NEO_FOREGROUND_MAGENTA | NEO_FOREGROUND_INTENSITY) | NEO_BACKGROUND_YELLOW;
        }

        ConsoleUtils::setChar(x, y, L'┌', borderAttribute);
        ConsoleUtils::setChar(x + CARD_WIDTH - 1, y, L'┐', borderAttribute);
        ConsoleUtils::setChar(x, y + CARD_HEIGHT - 1, L'└', borderAttribute);
        ConsoleUtils::setChar(x + CARD_WIDTH - 1, y + CARD_HEIGHT - 1, L'┘', borderAttribute);
        for (int i = 1; i < CARD_WIDTH - 1; ++i) {
            ConsoleUtils::setChar(x + i, y, L'─', borderAttribute);
            ConsoleUtils::setChar(x + i, y + CARD_HEIGHT - 1, L'─', borderAttribute);
        }
        ConsoleUtils::setChar(x, y + 1, L'│', borderAttribute);
        ConsoleUtils::setChar(x + CARD_WIDTH - 1, y + 1, L'│', borderAttribute);

        wchar_t fillChar = L' ';
        WORD fillAttribute = backgroundAttribute;
        if (!card.isFaceUp() && !card.getIsJoker()) {
            fillChar = L'░'; 
            fillAttribute = hintHighlight ? (COLOR_CARD_BACK | (COLOR_HINT_HIGHLIGHT & 0xFFF0))
                           : highlighted ? (COLOR_CARD_BACK | NEO_BACKGROUND_YELLOW)
                           : COLOR_CARD_BACK;
        }

        for (int i = 1; i < CARD_WIDTH - 1; ++i) {
            ConsoleUtils::setChar(x + i, y + 1, fillChar, fillAttribute);
        }

        if (card.isFaceUp() || card.getIsJoker()) {
            auto drawStrings = card.getDrawStrings();
            std::wstring valueString = drawStrings.first;
            std::wstring suitString = drawStrings.second;

            std::wstring displayString = valueString + suitString;
            int padLeft = std::max(0, (CARD_WIDTH - 2 - (int)displayString.length()) / 2); 

            ConsoleUtils::writeStringWide(x + 1 + padLeft, y + 1, displayString, cardAttribute);
        }
    }

    void drawEmptyPile(int x, int y, bool highlighted, const std::wstring& symbol = L"[ ]", bool hintHighlight = false) {
        WORD attribute = COLOR_CARD_EMPTY;
        if(hintHighlight) attribute = COLOR_HINT_HIGHLIGHT;
        else if(highlighted) attribute = COLOR_HIGHLIGHTS;

        ConsoleUtils::drawBox(x, y, CARD_WIDTH, CARD_HEIGHT, attribute);

        int padLeft = std::max(0, (CARD_WIDTH - 2 - (int)symbol.length()) / 2);
        ConsoleUtils::writeStringWide(x + 1 + padLeft, y + 1, symbol, attribute);
    }

     void drawPileWithOffset(int x, int y, const std::vector<Card>& pile,
                           const Selection& currentSelection,
                           const std::optional<Hint>& hint)
    {
        int pileIndex = -1; 
        for (int i = 0; i < 7; ++i) if (x == TABLEAU_POS[i].X) pileIndex = i;


        bool isBaseSelected = currentSelection.type == SelectableType::TABLEAU_PILE && currentSelection.index == pileIndex;
         bool isHintDestinationBase = hint && hint->destination.type == SelectableType::TABLEAU_PILE && hint->destination.index == pileIndex;


        if (pile.empty()) {
            drawEmptyPile(x, y, isBaseSelected, L"[ ]", isHintDestinationBase);
            return;
        }


        int currentY = y;
        for (size_t i = 0; i < pile.size(); ++i) {
            bool isCardSelected = currentSelection.type == SelectableType::TABLEAU_CARD &&
                                  currentSelection.index == pileIndex &&
                                  currentSelection.cardIndex == static_cast<int>(i);


            bool isHintSourceCard = hint && hint->source.type == SelectableType::TABLEAU_CARD &&
                                     hint->source.index == pileIndex && hint->source.cardIndex == static_cast<int>(i);
             bool isHintDestinationCard = hint && hint->destination.type == SelectableType::TABLEAU_CARD && 
                                     hint->destination.index == pileIndex && hint->destination.cardIndex == static_cast<int>(i);


             bool cardHintHighlight = isHintSourceCard || isHintDestinationCard;

            if (currentY + CARD_HEIGHT >= CONSOLE_HEIGHT - 1) { 
                ConsoleUtils::writeString(x + 1, currentY, "...", COLOR_LABEL); 
                break;
            }

            drawCard(x, currentY, pile[i], isCardSelected, cardHintHighlight);

            if (i + 1 < pile.size()) {

                 if (pile[i].isFaceUp()) {
                    currentY += 2;
                 } else {
                    currentY += 1;
                 }
            }
        }
    }

    void animateCardMove(const Card& card, COORD startCoord, COORD endCoord) {
        if (!ENABLE_ANIMATION) {

            return;
        }
        
        ConsoleUtils::playSound(700, 50); 

        Card animatedCard = card; 
        animatedCard.setFaceUp(true); 

        float deltaX = static_cast<float>(endCoord.X - startCoord.X) / MOVE_ANIMATION_STEPS;
        float deltaY = static_cast<float>(endCoord.Y - startCoord.Y) / MOVE_ANIMATION_STEPS;
        float currentX = static_cast<float>(startCoord.X);
        float currentY = static_cast<float>(startCoord.Y);

        COORD lastDrawnPos = {-1, -1}; 

        SMALL_RECT affectedArea;
        affectedArea.Left   = std::min(startCoord.X, endCoord.X) - 1;
        affectedArea.Top    = std::min(startCoord.Y, endCoord.Y) - 1;
        affectedArea.Right  = std::max(startCoord.X, endCoord.X) + CARD_WIDTH + 1;
        affectedArea.Bottom = std::max(startCoord.Y, endCoord.Y) + CARD_HEIGHT + 1;

        affectedArea.Left = std::max((SHORT)0, affectedArea.Left);
        affectedArea.Top = std::max((SHORT)0, affectedArea.Top);
        affectedArea.Right = std::min((SHORT)(CONSOLE_WIDTH - 1), affectedArea.Right);
        affectedArea.Bottom = std::min((SHORT)(CONSOLE_HEIGHT - 1), affectedArea.Bottom);

        COORD bufferSize = { (SHORT)(affectedArea.Right - affectedArea.Left + 1), (SHORT)(affectedArea.Bottom - affectedArea.Top + 1) };
        CHAR_INFO* backgroundBuffer = nullptr;

        if(bufferSize.X > 0 && bufferSize.Y > 0) {
            backgroundBuffer = new CHAR_INFO[bufferSize.X * bufferSize.Y];
            ReadConsoleOutputW(ConsoleUtils::hConsole, backgroundBuffer, bufferSize, {0,0}, &affectedArea);
        }

        const int trailLength = 3;
        COORD trailPositions[trailLength];
        for (int i = 0; i < trailLength; i++) {
            trailPositions[i] = {-1, -1};
        }

        for (int i = 0; i <= MOVE_ANIMATION_STEPS; ++i) {
            COORD pos = {static_cast<SHORT>(std::round(currentX)), static_cast<SHORT>(std::round(currentY))};

            if (i == 0 || i == MOVE_ANIMATION_STEPS || pos.X != lastDrawnPos.X || pos.Y != lastDrawnPos.Y) {

                if (backgroundBuffer) {
                    WriteConsoleOutputW(ConsoleUtils::hConsole, backgroundBuffer, bufferSize, {0,0}, &affectedArea);
                } else {
                    drawBoardContent(std::nullopt); 
                }

                for (int t = trailLength - 1; t > 0; --t) {
                    if (trailPositions[t].X != -1) {

                        Card trailCard = animatedCard;
                        WORD fadeAttribute = (t == 1) ? 
                            (NEO_BACKGROUND_BLUE | NEO_FOREGROUND_INTENSITY) : 
                            (NEO_BACKGROUND_BLACK | NEO_FOREGROUND_BLUE | NEO_FOREGROUND_INTENSITY);

                        ConsoleUtils::setChar(trailPositions[t].X, trailPositions[t].Y, L'┌', fadeAttribute);
                        ConsoleUtils::setChar(trailPositions[t].X + CARD_WIDTH - 1, trailPositions[t].Y, L'┐', fadeAttribute);
                        ConsoleUtils::setChar(trailPositions[t].X, trailPositions[t].Y + CARD_HEIGHT - 1, L'└', fadeAttribute);
                        ConsoleUtils::setChar(trailPositions[t].X + CARD_WIDTH - 1, trailPositions[t].Y + CARD_HEIGHT - 1, L'┘', fadeAttribute);
                    }
                }

                for (int t = trailLength - 1; t > 0; --t) {
                    trailPositions[t] = trailPositions[t-1];
                }
                trailPositions[0] = lastDrawnPos;

                drawCard(pos.X, pos.Y, animatedCard, false); 

                ConsoleUtils::renderBuffer();
                lastDrawnPos = pos;
            }

            currentX += deltaX;
            currentY += deltaY;

            ConsoleUtils::sleep(ANIMATION_DELAY_MS);
        }

        if (backgroundBuffer) {
            delete[] backgroundBuffer;
        }
    }

    void animatePowerUp(int x, int y, const std::wstring& powerUpText) {
        if (!ENABLE_ANIMATION) {
            return;
        }

        SMALL_RECT fullScreenArea = {0, 0, CONSOLE_WIDTH - 1, CONSOLE_HEIGHT - 1};
        COORD fullBufferSize = {CONSOLE_WIDTH, CONSOLE_HEIGHT};
        CHAR_INFO* fullScreenBuffer = new CHAR_INFO[CONSOLE_WIDTH * CONSOLE_HEIGHT];
        ReadConsoleOutputW(ConsoleUtils::hConsole, fullScreenBuffer, fullBufferSize, {0, 0}, &fullScreenArea);

        ConsoleUtils::playSound(900, 100);

        for (int step = 0; step < POWER_ANIMATION_STEPS; step++) {

            int size = step + 1;
            WORD attributeColor;

            switch (step % 4) {
                case 0: attributeColor = NEO_BACKGROUND_GREEN | NEO_FOREGROUND_WHITE | NEO_FOREGROUND_INTENSITY; break;
                case 1: attributeColor = NEO_BACKGROUND_BLUE | NEO_FOREGROUND_WHITE | NEO_FOREGROUND_INTENSITY; break;
                case 2: attributeColor = NEO_BACKGROUND_RED | NEO_FOREGROUND_WHITE | NEO_FOREGROUND_INTENSITY; break;
                case 3: attributeColor = NEO_BACKGROUND_MAGENTA | NEO_FOREGROUND_WHITE | NEO_FOREGROUND_INTENSITY; break;
            }

            drawBoardContent(std::nullopt);
            ConsoleUtils::drawBox(x - size, y - size, powerUpText.length() + 2 + size*2, 1 + size*2, attributeColor);

            ConsoleUtils::writeStringWide(x, y, powerUpText, attributeColor);

            ConsoleUtils::renderBuffer();
            ConsoleUtils::sleep(POWERUP_ANIMATION_DELAY);
        }

        ConsoleUtils::sleep(500);

        const int fadeSteps = 5;
        for (int step = fadeSteps; step >= 0; step--) {

            drawBoardContent(std::nullopt);

            if (step > 0) {
                float fadeRatio = static_cast<float>(step) / fadeSteps;
                int size = std::max(1, static_cast<int>(POWER_ANIMATION_STEPS * fadeRatio));
                
                WORD attributeColor = NEO_BACKGROUND_BLUE | NEO_FOREGROUND_WHITE;
                if (step <= 2) {
                    attributeColor = NEO_BACKGROUND_BLACK | NEO_FOREGROUND_BLUE;
                }

                ConsoleUtils::drawBox(x - size, y - size, powerUpText.length() + 2 + size*2, 1 + size*2, attributeColor);

                ConsoleUtils::writeStringWide(x, y, powerUpText, attributeColor);
            }

            ConsoleUtils::renderBuffer();
            ConsoleUtils::sleep(100);
        }

        drawBoardContent(std::nullopt);
        ConsoleUtils::renderBuffer();
        
        delete[] fullScreenBuffer;
    }

    void drawBoardStaticElements() {
        ConsoleUtils::drawBox(0, 0, CONSOLE_WIDTH, CONSOLE_HEIGHT, COLOR_BORDER, true); 

        std::string title = " NeoSolitaire ";
        ConsoleUtils::writeString((CONSOLE_WIDTH - title.length()) / 2, 0, title, COLOR_TITLE | NEO_BACKGROUND_BLUE); 

        ConsoleUtils::writeString(STOCK_POS.X, STOCK_POS.Y - 1, "Stock", COLOR_LABEL);
        ConsoleUtils::writeString(WASTE_POS.X, WASTE_POS.Y - 1, "Waste", COLOR_LABEL);
        ConsoleUtils::writeString(FOUNDATION_POS[0].X, FOUNDATION_POS[0].Y - 1, "Foundations", COLOR_LABEL);
        ConsoleUtils::writeString(TABLEAU_POS[0].X, TABLEAU_POS[0].Y - 1, "Tableau", COLOR_LABEL);
        ConsoleUtils::writeString(SCORE_POS.X, SCORE_POS.Y - 1, "Stats", COLOR_LABEL);

        for (int i = 0; i < 7; ++i) {
            ConsoleUtils::writeString(TABLEAU_POS[i].X + CARD_WIDTH / 2, TABLEAU_POS[i].Y - 1, std::to_string(i + 1), COLOR_LABEL);
        }

        if (player.getColorblindMode()) {
             std::wstring suitsString = L" H D C S"; 
             for (int i = 0; i < 4; ++i) {
                 ConsoleUtils::writeStringWide(FOUNDATION_POS[i].X + CARD_WIDTH / 2 -1 , FOUNDATION_POS[i].Y - 1, suitsString.substr(i*2+1, 1), COLOR_LABEL);
             }
        }
    }

    void drawStock(const std::optional<Hint>& hint) {
        bool isSelected = currentSelection.type == SelectableType::STOCK;
        bool isHintSource = hint && hint->source.type == SelectableType::STOCK;
        bool isHintDest = hint && hint->destination.type == SelectableType::STOCK; 

        if (game.getStock().empty()) {

             std::wstring symbol = game.getWaste().empty() ? L"[ ]" : L"[R]";
             drawEmptyPile(STOCK_POS.X, STOCK_POS.Y, isSelected, symbol, isHintSource || isHintDest);
        } else {
            Card backCard(' ', 0); 
            backCard.setFaceUp(false);
            drawCard(STOCK_POS.X, STOCK_POS.Y, backCard, isSelected, isHintSource);

            std::string countString = std::to_string(game.getStock().size());
            ConsoleUtils::fillRectangle(STOCK_POS.X, STOCK_POS.Y + CARD_HEIGHT, CARD_WIDTH, 1, L' '); 
            ConsoleUtils::writeString(STOCK_POS.X + (CARD_WIDTH - countString.length()) / 2, STOCK_POS.Y + CARD_HEIGHT, countString, COLOR_VALUE);
        }
    }

    void drawWaste(const std::optional<Hint>& hint) {
        bool isSelected = currentSelection.type == SelectableType::WASTE;
         bool isHintSource = hint && hint->source.type == SelectableType::WASTE;
         bool isHintDest = hint && hint->destination.type == SelectableType::WASTE; 

        if (game.getWaste().empty()) {
            drawEmptyPile(WASTE_POS.X, WASTE_POS.Y, isSelected, L"[ ]", isHintDest);
        } else {
            drawCard(WASTE_POS.X, WASTE_POS.Y, game.getWaste().back(), isSelected, isHintSource || isHintDest);

             if (game.getWaste().size() > 1) {

                ConsoleUtils::setChar(WASTE_POS.X - 1, WASTE_POS.Y + 1, L'<', COLOR_BORDER); 
             }
        }
    }

    void drawFoundations(const std::optional<Hint>& hint) {
        for (int i = 0; i < 4; ++i) {
            bool isSelected = currentSelection.type == SelectableType::FOUNDATION && currentSelection.index == i;
             bool isHintSource = hint && hint->source.type == SelectableType::FOUNDATION && hint->source.index == i;
             bool isHintDest = hint && hint->destination.type == SelectableType::FOUNDATION && hint->destination.index == i;


            if (game.getFoundations()[i].empty()) {

                std::wstring suitSymbol;
                WORD color = COLOR_CARD_EMPTY;
                 switch(i) { 
                     case 0: suitSymbol = L"♥"; color = player.getColorblindMode() ? COLOR_CARD_EMPTY : COLOR_CARD_RED; break;
                     case 1: suitSymbol = L"♦"; color = player.getColorblindMode() ? COLOR_CARD_EMPTY : COLOR_CARD_RED; break;
                     case 2: suitSymbol = L"♣"; color = COLOR_CARD_BLACK; break;
                     case 3: suitSymbol = L"♠"; color = COLOR_CARD_BLACK; break;
                 }
                drawEmptyPile(FOUNDATION_POS[i].X, FOUNDATION_POS[i].Y, isSelected, L" " + suitSymbol, isHintDest);

                 ConsoleUtils::setChar(FOUNDATION_POS[i].X + 1, FOUNDATION_POS[i].Y + 1, suitSymbol[0],
                                       isSelected ? COLOR_HIGHLIGHTS : (isHintDest ? COLOR_HINT_HIGHLIGHT : color));


            } else {
                drawCard(FOUNDATION_POS[i].X, FOUNDATION_POS[i].Y, game.getFoundations()[i].back(), isSelected, isHintSource || isHintDest);
            }
        }
    }

    void drawTableau(const std::optional<Hint>& hint) {
        for (int i = 0; i < 7; ++i) {

            drawPileWithOffset(TABLEAU_POS[i].X, TABLEAU_POS[i].Y, game.getTableau()[i], currentSelection, hint);
        }
    }

    void drawStats() {
        int currentY = SCORE_POS.Y;
        int labelWidth = 11; 
        int valueX = SCORE_POS.X + labelWidth; 

        ConsoleUtils::fillRectangle(SCORE_POS.X, SCORE_POS.Y, CONSOLE_WIDTH - SCORE_POS.X -1, 15, L' ');

        ConsoleUtils::writeString(SCORE_POS.X, currentY, "Score      : ", COLOR_LABEL);
        ConsoleUtils::writeString(valueX, currentY++, std::to_string(player.getScore()), COLOR_VALUE);

        ConsoleUtils::writeString(SCORE_POS.X, currentY, "Time       : ", COLOR_LABEL);
        ConsoleUtils::writeString(valueX, currentY++, player.getFormattedTime(), COLOR_VALUE);

        ConsoleUtils::writeString(SCORE_POS.X, currentY, "Moves      : ", COLOR_LABEL);
        ConsoleUtils::writeString(valueX, currentY++, std::to_string(game.getMovesMade()), COLOR_VALUE);

        ConsoleUtils::writeString(SCORE_POS.X, currentY, "High Score : ", COLOR_LABEL);
        ConsoleUtils::writeString(valueX, currentY++, std::to_string(player.getHighScore()), COLOR_VALUE);

        currentY++; 

        ConsoleUtils::writeString(POWERUP_POS.X, currentY, "[U]ndo : ", COLOR_LABEL);
        ConsoleUtils::writeString(POWERUP_POS.X + 9, currentY++, std::to_string(player.getUndosLeft()), player.getUndosLeft() > 0 ? COLOR_VALUE : COLOR_ERROR);

        ConsoleUtils::writeString(POWERUP_POS.X, currentY, "[S]hfl : ", COLOR_LABEL);
        ConsoleUtils::writeString(POWERUP_POS.X + 9, currentY++, std::to_string(player.getShufflesLeft()), player.getShufflesLeft() > 0 ? COLOR_VALUE : COLOR_ERROR);

        ConsoleUtils::writeString(POWERUP_POS.X, currentY, "[V]eal : ", COLOR_LABEL);
        ConsoleUtils::writeString(POWERUP_POS.X + 9, currentY++, std::to_string(player.getRevealsLeft()), player.getRevealsLeft() > 0 ? COLOR_VALUE : COLOR_ERROR);

        ConsoleUtils::writeString(POWERUP_POS.X, currentY, "[J]oker: ", COLOR_LABEL);
        ConsoleUtils::writeString(POWERUP_POS.X + 9, currentY++, std::to_string(player.getJokerCardsLeft()), player.getJokerCardsLeft() > 0 ? COLOR_VALUE : COLOR_ERROR);

        ConsoleUtils::writeString(POWERUP_POS.X, currentY, "[H]int : ", COLOR_LABEL); 
        ConsoleUtils::writeString(POWERUP_POS.X + 9, currentY++, std::to_string(player.getHintsLeft()), player.getHintsLeft() > 0 ? COLOR_VALUE : COLOR_ERROR);
    }

    void drawHelpLine() {
        ConsoleUtils::fillRectangle(HELP_POS.X + 1, HELP_POS.Y, CONSOLE_WIDTH - 2, 1, L' ', COLOR_DEFAULT); 

        std::string helpText = " Arrows: Navigate | Enter/Space: Select | D: Draw | U: Undo | H: Hint | S: Shfl | V: Real | J: Jokr | O: Opts | Q: Quit "; 
        int startX = std::max(1, (CONSOLE_WIDTH - (int)helpText.length()) / 2);
        ConsoleUtils::writeString(startX, HELP_POS.Y, helpText, COLOR_LABEL | NEO_BACKGROUND_BLACK); 
    }

    void drawBoardContent(const std::optional<Hint>& hint) {
        drawStock(hint);
        drawWaste(hint);
        drawFoundations(hint);
        drawTableau(hint);
        drawStats();
        drawStatusBar();
        drawHelpLine();
    }

public:
    ConsoleUI(SolitaireGame& g) : game(g), player(g.getPlayer()) {
        ConsoleUtils::initConsole();
    }

    ~ConsoleUI() {
        ConsoleUtils::cleanupConsole();
    }

    void animateInitialDeal() {
        if (!ENABLE_ANIMATION) {
            return;
        }
        
        COORD deckPos = STOCK_POS; 
        
        ConsoleUtils::clearBuffer();
        drawBoardStaticElements();

        Card blankCard = Card();
        blankCard.setFaceUp(false);

        for (int i = 0; i < 7; ++i) {
            drawEmptyPile(TABLEAU_POS[i].X, TABLEAU_POS[i].Y, false);
        }
        for (int i = 0; i < 4; ++i) {
            drawEmptyPile(FOUNDATION_POS[i].X, FOUNDATION_POS[i].Y, false);
        }
        drawEmptyPile(WASTE_POS.X, WASTE_POS.Y, false);

        drawCard(deckPos.X, deckPos.Y, blankCard, false);

        ConsoleUtils::renderBuffer();
        ConsoleUtils::sleep(DEAL_ANIMATION_DELAY_MS * 3);

        for (int pileIdx = 0; pileIdx < 7; ++pileIdx) {
            for (int cardIdx = 0; cardIdx <= pileIdx; ++cardIdx) {

                int offsetY = 0;
                for (int i = 0; i < cardIdx; ++i) {
                    offsetY += (i == cardIdx - 1 ? 2 : 1);
                }
                
                COORD destCoord = {
                    TABLEAU_POS[pileIdx].X,
                    static_cast<SHORT>(TABLEAU_POS[pileIdx].Y + offsetY)
                };

                const Card& actualCard = game.getTableau()[pileIdx][cardIdx];

                Card animCard = actualCard;
                animCard.setFaceUp(cardIdx == pileIdx);

                ConsoleUtils::playSound(cardIdx == pileIdx ? 1000 : 800, 30);

                COORD cardCenter = {
                    static_cast<SHORT>(destCoord.X + CARD_WIDTH / 2), 
                    static_cast<SHORT>(destCoord.Y + CARD_HEIGHT / 2)
                };

                for (int step = 0; step <= MOVE_ANIMATION_STEPS / 2; ++step) {
                    float progress = static_cast<float>(step) / (MOVE_ANIMATION_STEPS / 2);

                    float arcHeight = 5.0f; 

                    float x0 = deckPos.X;
                    float y0 = deckPos.Y;
                    float x2 = destCoord.X;
                    float y2 = destCoord.Y;

                    float x1 = (x0 + x2) / 2;
                    float y1 = std::min(y0, y2) - arcHeight;

                    float t = progress;
                    float u = 1.0f - t;
                    float tt = t * t;
                    float uu = u * u;
                    
                    float x = uu * x0 + 2 * u * t * x1 + tt * x2;
                    float y = uu * y0 + 2 * u * t * y1 + tt * y2;
                    
                    COORD currentPos = {
                        static_cast<SHORT>(std::round(x)),
                        static_cast<SHORT>(std::round(y))
                    };

                    drawBoardContent(std::nullopt);

                    drawCard(currentPos.X, currentPos.Y, animCard, false);

                    if (cardIdx == pileIdx) {

                        int sparkleRadius = static_cast<int>(3 * (1.0f - progress));
                        for (int s = 0; s < 4; s++) {
                            int sx = currentPos.X + CARD_WIDTH/2 + std::cos(progress * 6.28f + s * 1.57f) * sparkleRadius;
                            int sy = currentPos.Y + CARD_HEIGHT/2 + std::sin(progress * 6.28f + s * 1.57f) * sparkleRadius;
                            
                            if (sx >= 0 && sx < CONSOLE_WIDTH && sy >= 0 && sy < CONSOLE_HEIGHT) {
                                ConsoleUtils::setChar(sx, sy, L'*', NEO_FOREGROUND_YELLOW | NEO_FOREGROUND_INTENSITY);
                            }
                        }
                    }
                    
                    ConsoleUtils::renderBuffer();
                    ConsoleUtils::sleep(DEAL_ANIMATION_DELAY_MS);
                }

                drawBoardContent(std::nullopt);
                ConsoleUtils::renderBuffer();
                
                ConsoleUtils::sleep(DEAL_ANIMATION_DELAY_MS);
            }
        }

        int stockSize = game.getStock().size();
        std::wstring countText = L"Dealing " + std::to_wstring(stockSize) + L" cards to stock...";
        
        ConsoleUtils::writeStringWide(
            (CONSOLE_WIDTH - countText.length()) / 2, 
            CONSOLE_HEIGHT - 5, 
            countText, 
            COLOR_INFO
        );
        ConsoleUtils::renderBuffer();

        for (int i = 0; i < std::min(10, stockSize); i++) {
            ConsoleUtils::playSound(600 + i * 30, 20);
            Card tempCard = blankCard;

            drawCard(deckPos.X - 1 + (i % 3), deckPos.Y - 1 + (i % 2), tempCard, false);
            ConsoleUtils::renderBuffer();
            ConsoleUtils::sleep(DEAL_ANIMATION_DELAY_MS);
        }

        drawBoardContent(std::nullopt);

        std::wstring readyText = L"Ready to play!";
        ConsoleUtils::fillRectangle(0, CONSOLE_HEIGHT - 5, CONSOLE_WIDTH, 1, L' ');
        ConsoleUtils::writeStringWide(
            (CONSOLE_WIDTH - readyText.length()) / 2, 
            CONSOLE_HEIGHT - 5, 
            readyText, 
            COLOR_SUCCESS
        );
        
        ConsoleUtils::renderBuffer();
        ConsoleUtils::sleep(500);

        drawBoardContent(std::nullopt);
        ConsoleUtils::renderBuffer();
    }

    void animateWin() {
        ConsoleUtils::playSound(1500, 500); 

        std::vector<Card> allCards;
        for (const auto& foundation : game.getFoundations()) {
            allCards.insert(allCards.end(), foundation.begin(), foundation.end());
        }

        if (allCards.size() != 52) {
            allCards.clear();

            for (const auto& f : game.getFoundations()) allCards.insert(allCards.end(), f.begin(), f.end());
            for (const auto& t : game.getTableau()) allCards.insert(allCards.end(), t.begin(), t.end());
            allCards.insert(allCards.end(), game.getStock().begin(), game.getStock().end());
            allCards.insert(allCards.end(), game.getWaste().begin(), game.getWaste().end());
        }


        for(auto& cardRef : allCards) cardRef.setFaceUp(true); 
        std::shuffle(allCards.begin(), allCards.end(), game.getRng()); 

        struct BouncingCard {
            Card card;
            float x, y; 
            float vx, vy; 
        };
        std::vector<BouncingCard> bouncingCards;

        std::mt19937 bounceRng(static_cast<unsigned int>(std::time(nullptr)));
        std::uniform_real_distribution<float> speedDistribution(1.5f, 3.0f);
        std::uniform_real_distribution<float> angleDistribution(0.0f, 2.0f * 3.14159f);
        std::uniform_int_distribution<int> startPosXDistribution(CARD_WIDTH, CONSOLE_WIDTH - CARD_WIDTH*2);
        std::uniform_int_distribution<int> startPosYDistribution(CARD_HEIGHT, CARD_HEIGHT*2); 


        float gravity = 0.1f;
        float bounceFactor = -0.6f; 


        auto startTime = std::chrono::steady_clock::now();
        int cardIndex = 0; 

        while (std::chrono::duration_cast<std::chrono::seconds>(std::chrono::steady_clock::now() - startTime).count() < 8) {
            ConsoleUtils::clearBuffer(); 
            drawBoardStaticElements(); 

            if (cardIndex < (int)allCards.size() && bouncingCards.size() < 52) { 
                float angle = angleDistribution(bounceRng);
                float speed = speedDistribution(bounceRng);
                 bouncingCards.push_back({
                    allCards[cardIndex++],
                    (float)startPosXDistribution(bounceRng), (float)startPosYDistribution(bounceRng), 
                    speed * std::cos(angle), speed * std::sin(angle) * 0.5f 
                 });
                 ConsoleUtils::playSound(1000 + cardIndex*20, 30); 
            }

            for (auto& bouncingCard : bouncingCards) {

                bouncingCard.vy += gravity;

                bouncingCard.x += bouncingCard.vx;
                bouncingCard.y += bouncingCard.vy;

                if (bouncingCard.x <= 1 || bouncingCard.x + CARD_WIDTH >= CONSOLE_WIDTH - 1) {
                    bouncingCard.vx *= bounceFactor; 
                     bouncingCard.x = std::max(1.0f, std::min(bouncingCard.x, (float)CONSOLE_WIDTH - 1 - CARD_WIDTH)); 
                     ConsoleUtils::playSound(600, 20); 
                }
                if (bouncingCard.y <= 1 || bouncingCard.y + CARD_HEIGHT >= CONSOLE_HEIGHT - 1) {
                    bouncingCard.vy *= bounceFactor; 
                    bouncingCard.y = std::max(1.0f, std::min(bouncingCard.y, (float)CONSOLE_HEIGHT - 1 - CARD_HEIGHT)); 

                    if (bouncingCard.y + CARD_HEIGHT >= CONSOLE_HEIGHT - 1) bouncingCard.vx *= 0.9f;
                     ConsoleUtils::playSound(500, 20); 
                }

                drawCard(static_cast<int>(bouncingCard.x), static_cast<int>(bouncingCard.y), bouncingCard.card, false);
            }


            ConsoleUtils::renderBuffer();
            ConsoleUtils::sleep(ANIMATION_DELAY_MS * 2); 
        }

        setStatusMessage(L"!!! C O N G R A T U L A T I O N S !!! YOU WIN !!!", 0); 
        drawStatusBar(); 
        ConsoleUtils::renderBuffer();
        ConsoleUtils::sleep(3000); 
    }

    void drawStatusBar() {
        WORD statusBarBackground = COLOR_STATUS_BAR & 0xFFF0; 
        WORD currentAttribute = COLOR_STATUS_BAR; 

        if (!statusMessage.empty() && std::chrono::steady_clock::now() > messageExpireTime && messageExpireTime.time_since_epoch().count() != 0) {
            statusMessage.clear(); 
        }

        std::wstring modeText; 
        if (!statusMessage.empty()) {
            modeText = statusMessage;

            if (statusMessage.find(L"Error") != std::wstring::npos || statusMessage.find(L"Invalid") != std::wstring::npos ||
                statusMessage.find(L"Cannot") != std::wstring::npos || statusMessage.find(L"OVER") != std::wstring::npos || statusMessage.find(L"No ") != std::wstring::npos) {
                currentAttribute = statusBarBackground | COLOR_ERROR;
            } else if (statusMessage.find(L"Success") != std::wstring::npos || statusMessage.find(L"WIN") != std::wstring::npos ||
                       statusMessage.find(L"saved") != std::wstring::npos || statusMessage.find(L"Moved") != std::wstring::npos ||
                       statusMessage.find(L"Drew") != std::wstring::npos || statusMessage.find(L"shuffled") != std::wstring::npos ||
                       statusMessage.find(L"Revealed") != std::wstring::npos || statusMessage.find(L"placed") != std::wstring::npos ||
                       statusMessage.find(L"Hint") != std::wstring::npos) {
                currentAttribute = statusBarBackground | COLOR_SUCCESS;
            } else { 
                currentAttribute = statusBarBackground | COLOR_INFO;
            }
        } else if (sourceSelection.isValid()) {
            modeText = L"SELECT DESTINATION (Arrows/kj, Enter/Space to select | Esc to cancel)"; 
            currentAttribute = statusBarBackground | COLOR_INFO;
        } else {

            modeText = L"SELECT SOURCE (Arrows/kj, Enter/Space) | D: Draw | U: Undo | H: Hint | S: Shfl | V: Real | J: Jokr | O: Opts | Q: Quit"; 
            currentAttribute = COLOR_STATUS_BAR;
        }

        ConsoleUtils::fillRectangle(STATUS_POS.X + 1, STATUS_POS.Y, CONSOLE_WIDTH - 2, 1, L' ', currentAttribute);
        ConsoleUtils::writeStringWide(STATUS_POS.X + 2, STATUS_POS.Y, modeText, currentAttribute); 
    }

    void setStatusMessage(const std::wstring& message, int durationMilliseconds = 1500) {
        statusMessage = message;
        if (durationMilliseconds > 0) {
            messageExpireTime = std::chrono::steady_clock::now() + std::chrono::milliseconds(durationMilliseconds);
        } else {

            messageExpireTime = std::chrono::time_point<std::chrono::steady_clock>(); 
        }

        drawStatusBar();
        ConsoleUtils::renderBuffer();
    }

    void clearStatusMessage() {
        statusMessage.clear();

        messageExpireTime = std::chrono::steady_clock::now();
    }

    void drawBoard() {
        ConsoleUtils::clearBuffer();
        drawBoardStaticElements();
        drawBoardContent(std::nullopt); 
    }

    void updateBoard() {

        drawBoardContent(std::nullopt);
    }

    void flashHint(const Hint& hint) {
        ConsoleUtils::playSound(1000, 50); 

        for (int i = 0; i < HINT_FLASH_COUNT; ++i) {

            drawBoardContent(hint);
            ConsoleUtils::renderBuffer();
            ConsoleUtils::sleep(HINT_FLASH_DELAY_MS);

            drawBoardContent(std::nullopt);
            ConsoleUtils::renderBuffer();
            ConsoleUtils::sleep(HINT_FLASH_DELAY_MS);
        }

        drawBoardContent(std::nullopt);
        ConsoleUtils::renderBuffer();
    }


    const Selection& getCurrentSelection() const { return currentSelection; }
    bool isSourceSelected() const { return sourceSelection.isValid(); }

    void clearSelections() {
        currentSelection.clear();
        sourceSelection.clear();
    }

    void clearSourceSelection() {
        sourceSelection.clear();
    }

    void setSourceSelection(const Selection& sel) {
        sourceSelection = sel;
    }

    void navigate(int deltaX, int deltaY) {
         Selection nextSelection = currentSelection; 
         COORD currentPosition = getCoordsForSelection(currentSelection);

         if (!currentSelection.isValid() || currentPosition.X == -1) {
             nextSelection = {SelectableType::STOCK, 0, -1};
         } else {


             if (deltaY == -1) {

                  if (currentSelection.type == SelectableType::TABLEAU_CARD) {

                      int previousFaceUpIndex = -1;
                      const auto& pile = game.getTableau()[currentSelection.index];
                       for (int i = currentSelection.cardIndex - 1; i >= 0; --i) {
                           if (pile[i].isFaceUp()) {
                               previousFaceUpIndex = i;
                               break;
                           }
                       }
                       if (previousFaceUpIndex != -1) {
                           nextSelection.cardIndex = previousFaceUpIndex; 
                       } else if (currentSelection.cardIndex > 0) {
                            nextSelection.cardIndex = 0; 
                       } else { 

                             if (currentPosition.X < WASTE_POS.X + CARD_WIDTH / 2)
                                nextSelection = {SelectableType::STOCK, 0, -1};
                             else if (currentPosition.X < FOUNDATION_POS[0].X + CARD_WIDTH / 2)
                                nextSelection = {SelectableType::WASTE, 0, -1};
                             else {
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

                  else if (currentSelection.type == SelectableType::TABLEAU_PILE) {
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

             }

             else if (deltaY == 1) {

                 if (currentSelection.type == SelectableType::STOCK || currentSelection.type == SelectableType::WASTE || currentSelection.type == SelectableType::FOUNDATION) {

                     int closestTableau = 0;
                     int minDistance = 1000;
                     for (int t = 0; t < 7; ++t) {
                         int distance = std::abs(currentPosition.X - TABLEAU_POS[t].X);
                         if (distance < minDistance) { minDistance = distance; closestTableau = t;}
                     }
                     const auto& targetPile = game.getTableau()[closestTableau];
                      if (!targetPile.empty()) {

                           int firstFaceUpIndex = -1;
                           for (size_t c = 0; c < targetPile.size(); ++c) {
                               if (targetPile[c].isFaceUp()) { firstFaceUpIndex = static_cast<int>(c); break; }
                           }
                           if (firstFaceUpIndex != -1)
                              nextSelection = {SelectableType::TABLEAU_CARD, closestTableau, firstFaceUpIndex};
                           else 
                              nextSelection = {SelectableType::TABLEAU_CARD, closestTableau, static_cast<int>(targetPile.size() - 1)};
                      } else {

                           nextSelection = {SelectableType::TABLEAU_PILE, closestTableau, -1};
                      }
                 }

                 else if (currentSelection.type == SelectableType::TABLEAU_CARD) {
                     const auto& currentPile = game.getTableau()[currentSelection.index];

                     int nextFaceUpIndex = -1;
                     for (size_t i = currentSelection.cardIndex + 1; i < currentPile.size(); ++i) {
                         if (currentPile[i].isFaceUp()) {
                             nextFaceUpIndex = static_cast<int>(i);
                             break;
                         }
                     }
                     if (nextFaceUpIndex != -1)
                         nextSelection.cardIndex = nextFaceUpIndex; 
                     else if (static_cast<size_t>(currentSelection.cardIndex) < currentPile.size() - 1)

                        nextSelection.cardIndex = static_cast<int>(currentPile.size() - 1);

                 }

             }

             else if (deltaX == -1) {
                  if (currentSelection.type == SelectableType::WASTE) nextSelection = {SelectableType::STOCK, 0, -1};
                  else if (currentSelection.type == SelectableType::FOUNDATION && currentSelection.index > 0) nextSelection.index--;
                  else if (currentSelection.type == SelectableType::FOUNDATION && currentSelection.index == 0) nextSelection = {SelectableType::WASTE, 0, -1}; 
                  else if ((currentSelection.type == SelectableType::TABLEAU_PILE || currentSelection.type == SelectableType::TABLEAU_CARD) && currentSelection.index > 0) {

                       int targetIndex = currentSelection.index - 1;
                       const auto& targetPile = game.getTableau()[targetIndex];
                       if (!targetPile.empty()) {
                            int targetCardIndex = static_cast<int>(targetPile.size())-1; 
                            int firstFaceUp = -1;
                            for(size_t c=0; c<targetPile.size(); ++c) if(targetPile[c].isFaceUp()){ firstFaceUp=c; break;}
                            if(firstFaceUp != -1) targetCardIndex = firstFaceUp; 
                            nextSelection = {SelectableType::TABLEAU_CARD, targetIndex, targetCardIndex};
                       } else {
                            nextSelection = {SelectableType::TABLEAU_PILE, targetIndex, -1};
                       }
                  }
             }

             else if (deltaX == 1) {
                 if (currentSelection.type == SelectableType::STOCK) nextSelection = {SelectableType::WASTE, 0, -1};
                 else if (currentSelection.type == SelectableType::WASTE) nextSelection = {SelectableType::FOUNDATION, 0, -1}; 
                 else if (currentSelection.type == SelectableType::FOUNDATION && currentSelection.index < 3) nextSelection.index++;
                 else if ((currentSelection.type == SelectableType::TABLEAU_PILE || currentSelection.type == SelectableType::TABLEAU_CARD) && currentSelection.index < 6) {

                       int targetIndex = currentSelection.index + 1;
                       const auto& targetPile = game.getTableau()[targetIndex];
                       if (!targetPile.empty()) {
                            int targetCardIndex = static_cast<int>(targetPile.size())-1;
                            int firstFaceUp = -1;
                            for(size_t c=0; c<targetPile.size(); ++c) if(targetPile[c].isFaceUp()){ firstFaceUp=c; break;}
                            if(firstFaceUp != -1) targetCardIndex = firstFaceUp;
                           nextSelection = {SelectableType::TABLEAU_CARD, targetIndex, targetCardIndex};
                       } else {
                            nextSelection = {SelectableType::TABLEAU_PILE, targetIndex, -1};
                       }
                 }
             }
         }

        if (nextSelection.isValid() && nextSelection != currentSelection) {
            currentSelection = nextSelection;
            ConsoleUtils::playSound(1000, 20); 
        } else if (nextSelection.isValid() && nextSelection == currentSelection) {

        }
         else {
             ConsoleUtils::playSound(150, 50); 
         }
    }

     int runMenu(const std::string& title, const std::vector<std::string>& options, int startX = -1, int startY = -1) {
        int selectedOption = 0;
        int numOptions = options.size();
        if (numOptions == 0) return -1; 

        int menuWidth = 0;
        for (const auto& option : options) {
            menuWidth = std::max(menuWidth, (int)option.length());
        }
        menuWidth += 4; 
        int menuHeight = numOptions + 4; 

        if (startX < 0) startX = (CONSOLE_WIDTH - menuWidth) / 2;
        if (startY < 0) startY = (CONSOLE_HEIGHT - menuHeight) / 2;

        if (startX + menuWidth >= CONSOLE_WIDTH) startX = CONSOLE_WIDTH - menuWidth - 1;
        if (startY + menuHeight >= CONSOLE_HEIGHT) startY = CONSOLE_HEIGHT - menuHeight - 1;
        if (startX < 0) startX = 0;
        if (startY < 0) startY = 0;

        while (true) {

            ConsoleUtils::drawBox(startX, startY, menuWidth, menuHeight, COLOR_BORDER, true); 
            ConsoleUtils::fillRectangle(startX + 1, startY + 1, menuWidth - 2, menuHeight - 2, L' ', COLOR_DEFAULT); 

            ConsoleUtils::writeString(startX + (menuWidth - title.length()) / 2, startY + 1, title, COLOR_TITLE);

            for (int i = 0; i < numOptions; ++i) {
                WORD attribute = (i == selectedOption) ? COLOR_MENU_SELECTED : COLOR_MENU_ITEM;

                std::string optionText = " " + options[i] + std::string(menuWidth - 4 - options[i].length(), ' '); 
                ConsoleUtils::writeString(startX + 2, startY + 3 + i, optionText, attribute);
            }


            ConsoleUtils::renderBuffer(); 

            int key = ConsoleUtils::getKeyPress();

            switch (key) {
            case KEY_UP:
            case KEY_NAV_K_LOWER: 
            case KEY_NAV_K_UPPER:
                selectedOption = (selectedOption - 1 + numOptions) % numOptions;
                ConsoleUtils::playSound(800, 30); 
                break;
            case KEY_DOWN:
            case KEY_NAV_J_LOWER: 
            case KEY_NAV_J_UPPER:
                selectedOption = (selectedOption + 1) % numOptions;
                ConsoleUtils::playSound(800, 30); 
                break;
            case KEY_ENTER:
            case KEY_SPACE:
                ConsoleUtils::playSound(1000, 50); 

                return selectedOption; 
            case KEY_ESC:
            case 'q': 
            case 'Q':
                ConsoleUtils::playSound(400, 50); 

                return -1; 
            }
        }
    }

    void animatePowerUpEffect(int x, int y, const std::wstring& powerUpText) {

        animatePowerUp(x, y, powerUpText);
    }
};


class GameController {
private:
    SolitaireGame game;
    ConsoleUI ui;
    bool running;
    Selection sourceSelection; 

    void processInput() {
        int key = ConsoleUtils::getKeyPress();

        if (game.isGameOver() || game.isGameWon()) {
             if (key == 'n' || key == 'N') {
                startGame(); 
             } else if (key == 'q' || key == 'Q' || key == KEY_ESC) {
                 showMainMenu(); 
             } else if (key == 'o' || key == 'O') {
                 showOptions();

                 if (game.isGameOver() || game.isGameWon()) {
                    handleGameOver(); 
                 }
             }
             return; 
        }

        bool moveAttempted = false; 
        bool requiresRedraw = true; 

        switch (key) {

            case KEY_UP: ui.navigate(0, -1); requiresRedraw = true; break;
            case KEY_DOWN: ui.navigate(0, 1); requiresRedraw = true; break;
            case KEY_LEFT: ui.navigate(-1, 0); requiresRedraw = true; break; 
            case KEY_RIGHT: ui.navigate(1, 0); requiresRedraw = true; break;

            case KEY_ENTER:
            case KEY_SPACE:
                handleSelection(); 
                moveAttempted = true;
                break;

            case 'd': case 'D': 
                if (game.dealFromStock()) {
                    ui.clearSelections();
                    sourceSelection.clear(); 
                    ui.setStatusMessage(L"Drew from stock.", 1000); 
                } else {
                    ui.setStatusMessage(L"Cannot draw or recycle.", 1500); 
                }
                moveAttempted = true;
                break;
            case 'u': case 'U':
                if (game.getPlayer().getPowerupsEnabled() && game.getPlayer().getUndosLeft() > 0) {
                    if (game.undoMove()) {

                        ui.clearSelections();
                        sourceSelection.clear();
                        int centerX = CONSOLE_WIDTH / 2 - 2;
                        int centerY = CONSOLE_HEIGHT / 2;
                        ui.animatePowerUpEffect(centerX, centerY, L"UNDO!");
                        ui.updateBoard();
                    }
                } else {
                    ui.setStatusMessage(L"No undos left or powerups disabled!", 1500);
                }
                break;

            case 's': case 'S':
                if (game.getPlayer().getPowerupsEnabled() && game.getPlayer().getShufflesLeft() > 0) {
                    if (game.useShufflePowerup()) {

                        ui.clearSelections();
                        sourceSelection.clear();
                        int centerX = CONSOLE_WIDTH / 2 - 4;
                        int centerY = CONSOLE_HEIGHT / 2;
                        ui.animatePowerUpEffect(centerX, centerY, L"SHUFFLE!");
                        ui.updateBoard();
                    } else {
                        ui.setStatusMessage(L"Nothing to shuffle!", 1500);
                    }
                } else {
                    ui.setStatusMessage(L"No shuffles left or powerups disabled!", 1500);
                }
                break;

            case 'v': case 'V':
                if (game.getPlayer().getPowerupsEnabled() && game.getPlayer().getRevealsLeft() > 0) {
                    if (game.useRevealPowerup()) {

                        ui.clearSelections();
                        sourceSelection.clear();
                        int centerX = CONSOLE_WIDTH / 2 - 3;
                        int centerY = CONSOLE_HEIGHT / 2;
                        ui.animatePowerUpEffect(centerX, centerY, L"REVEAL!");
                        ui.updateBoard();
                    } else {
                        ui.setStatusMessage(L"No face-down cards to reveal!", 1500);
                    }
                } else {
                    ui.setStatusMessage(L"No reveals left or powerups disabled!", 1500);
                }
                break;

            case 'j': case 'J':
                if (game.getPlayer().getPowerupsEnabled() && game.getPlayer().getJokerCardsLeft() > 0) {

                    std::vector<std::string> options = {"Tableau 1", "Tableau 2", "Tableau 3", "Tableau 4", "Tableau 5", "Tableau 6", "Tableau 7"};
                    int selected = ui.runMenu("Select Tableau for Joker Card", options);
                    
                    if (selected >= 0 && selected < 7) {
                        if (game.useJokerPowerup(selected)) {

                            ui.clearSelections();
                            sourceSelection.clear();
                            int centerX = CONSOLE_WIDTH / 2 - 3;
                            int centerY = CONSOLE_HEIGHT / 2;
                            ui.animatePowerUpEffect(centerX, centerY, L"JOKER!");
                            ui.updateBoard();
                        } else {
                            ui.setStatusMessage(L"Can't place joker there!", 1500);
                        }
                    }
                } else {
                    ui.setStatusMessage(L"No jokers left or powerups disabled!", 1500);
                }
                break;

            case KEY_H_LOWER:
            case KEY_H_UPPER:
                if (game.getPlayer().getPowerupsEnabled() && game.getPlayer().getHintsLeft() > 0) {
                    auto hint = game.getHint();
                    if (hint) {
                        ui.clearSelections();
                        sourceSelection.clear();
                        ui.flashHint(*hint);
                        ui.setStatusMessage(hint->description, 3000);
                    } else {
                        ui.setStatusMessage(L"No hints available!", 1500);
                        game.getPlayer().refundHint(); 
                    }
                } else {
                    ui.setStatusMessage(L"No hints left or powerups disabled!", 1500);
                }
                break;

            case 'o': case 'O': 
                 showOptions();

                 requiresRedraw = true;
                 break;
            case 'q': case 'Q': 
                 showMainMenu(); 
                 return; 
            case KEY_ESC: 
                if (sourceSelection.isValid()) { 
                    sourceSelection.clear();
                    ui.clearSelections(); 
                    ui.setStatusMessage(L"Move cancelled.", 1000); 
                } else {

                    ui.clearSelections(); 
                }
                break;


            default:

                requiresRedraw = false; 
                break;
        }

        if (moveAttempted) {
            game.checkWinCondition(); 
             if (!game.isGameWon()) {
                 game.checkForGameOver(); 
             }
             if (game.isGameOver() || game.isGameWon()) {
                 handleGameOver(); 
                 return; 
             }
        }

        if (requiresRedraw) {
             ui.updateBoard(); 
             ConsoleUtils::renderBuffer();
        } else {

             ui.drawStatusBar();
             ConsoleUtils::renderBuffer();
        }
    }

    void handleSelection() {
        const Selection& currentSelection = ui.getCurrentSelection();
        if (!currentSelection.isValid()) return; 

        bool autoMoved = false;

        if (!sourceSelection.isValid()) {

             if (currentSelection.type == SelectableType::WASTE && !game.getWaste().empty() && game.canMoveWasteToFoundation()) {
                autoMoved = game.moveWasteToFoundation();
                if(autoMoved) ui.setStatusMessage(L"Moved Waste to Foundation.", 1000); 
             }

             else if (currentSelection.type == SelectableType::TABLEAU_CARD && currentSelection.index >= 0 && currentSelection.index < 7 &&
                       !game.getTableau()[currentSelection.index].empty() &&
                       currentSelection.cardIndex == static_cast<int>(game.getTableau()[currentSelection.index].size() - 1))
            {
                 if (game.canMoveTableauToFoundation(currentSelection.index)) {
                     autoMoved = game.moveTableauToFoundation(currentSelection.index);
                     if(autoMoved) ui.setStatusMessage(L"Moved Tableau to Foundation.", 1000); 
                 }
            }
        }

        if (autoMoved) {
            ui.clearSelections();
            sourceSelection.clear();
            return;
        }

        if (!sourceSelection.isValid()) {

            bool isValidSource = false;
            switch (currentSelection.type) {
            case SelectableType::STOCK:

                 if (game.dealFromStock()) {
                    ui.setStatusMessage(L"Drew from stock.", 1000); 
                 } else {
                    ui.setStatusMessage(L"Cannot draw or recycle.", 1500); 
                 }
                 ui.clearSelections(); 
                 sourceSelection.clear();
                 return; 
            case SelectableType::WASTE:
                isValidSource = !game.getWaste().empty();
                break;
            case SelectableType::FOUNDATION:
                 isValidSource = !game.getFoundations()[currentSelection.index].empty();
                 break;
            case SelectableType::TABLEAU_PILE: 
                 isValidSource = true; 
                 break;
            case SelectableType::TABLEAU_CARD:

                 isValidSource = currentSelection.index >= 0 && currentSelection.index < 7 &&
                                 !game.getTableau()[currentSelection.index].empty() &&
                                 currentSelection.cardIndex >= 0 &&
                                 static_cast<size_t>(currentSelection.cardIndex) < game.getTableau()[currentSelection.index].size() &&
                                 game.getTableau()[currentSelection.index][currentSelection.cardIndex].isFaceUp();
                 break;
            default: break;
            }


            if (isValidSource) {
                sourceSelection = currentSelection; 
                ui.setSourceSelection(sourceSelection); 
                ui.setStatusMessage(L"Source selected. Select destination.", 0); 
                ConsoleUtils::playSound(900, 40); 
            } else {
                ui.setStatusMessage(L"Invalid source selected.", 1500); 
                ConsoleUtils::playSound(200, 100); 
            }


        } else {

            Selection source = sourceSelection; 
            const Selection& destination = currentSelection;
            bool moveMade = false;

            if (source.type == SelectableType::WASTE) {
                if (destination.type == SelectableType::FOUNDATION) {
                     moveMade = game.moveWasteToFoundation(); 
                } else if (destination.type == SelectableType::TABLEAU_PILE || destination.type == SelectableType::TABLEAU_CARD) {
                     int targetIndex = destination.index; 
                     moveMade = game.moveWasteToTableau(targetIndex);
                }
            } else if (source.type == SelectableType::TABLEAU_CARD) {
                if (destination.type == SelectableType::FOUNDATION) {

                     if (source.index >= 0 && source.index < 7 && !game.getTableau()[source.index].empty() &&
                         source.cardIndex == static_cast<int>(game.getTableau()[source.index].size() - 1)) {
                         moveMade = game.moveTableauToFoundation(source.index); 
                     } else {
                          ui.setStatusMessage(L"Can only move top card to foundation.", 1500); 
                     }
                } else if (destination.type == SelectableType::TABLEAU_PILE || destination.type == SelectableType::TABLEAU_CARD) {
                     int targetIndex = destination.index;
                     moveMade = game.moveTableauToTableau(source.index, targetIndex, source.cardIndex);
                }
            } else if (source.type == SelectableType::FOUNDATION) {
                if (destination.type == SelectableType::TABLEAU_PILE || destination.type == SelectableType::TABLEAU_CARD) {
                     int targetIndex = destination.index;
                     moveMade = game.moveFoundationToTableau(source.index, targetIndex);
                }
            }

            if (moveMade) {
                 ui.setStatusMessage(L"Move successful!", 1000); 

            } else {

                 if (source != destination && destination.type != SelectableType::STOCK) {
                    ui.setStatusMessage(L"Invalid move.", 1500); 
                    ConsoleUtils::playSound(200, 150); 
                 }
            }

            ui.clearSelections();
            sourceSelection.clear();
        }
    }

    void handleGameOver() {
        ui.clearSelections(); 
        sourceSelection.clear();
        ui.drawBoard(); 
        ConsoleUtils::renderBuffer();
        ConsoleUtils::sleep(500); 


        std::wstring finalMessage; 
        if (game.isGameWon()) {

             finalMessage = L"!!! YOU WIN !!! Score: " + std::to_wstring(game.getPlayer().getScore()) +
                           L" Time: " + std::wstring(game.getPlayer().getFormattedTime().begin(), game.getPlayer().getFormattedTime().end());

             ConsoleUtils::playSound(1200, 200); ConsoleUtils::sleep(100);
             ConsoleUtils::playSound(1500, 300); ConsoleUtils::sleep(100);
             ConsoleUtils::playSound(1800, 500);

             ui.animateWin();
        } else {

             ConsoleUtils::playSound(300, 300); ConsoleUtils::sleep(100);
             ConsoleUtils::playSound(250, 300); ConsoleUtils::sleep(100);
             ConsoleUtils::playSound(200, 500);

             finalMessage = L"GAME OVER! Score: " + std::to_wstring(game.getPlayer().getScore());
        }

        finalMessage += L" | Press N for New Game, O for Options, Q for Main Menu."; 
        ui.setStatusMessage(finalMessage, 0); 

        ui.drawStatusBar();
        ConsoleUtils::renderBuffer();

    }

    void displaySplash() {
        ConsoleUtils::clearBuffer();

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
        int startY = (CONSOLE_HEIGHT - splashArt.size()) / 2 - 2;
        int maxWidth = 0;
        for(const auto& line : splashArt) maxWidth = std::max(maxWidth, (int)line.length());
        int startX = (CONSOLE_WIDTH - maxWidth) / 2;

        for (size_t i = 0; i < splashArt.size(); ++i) {
             for (size_t j = 0; j < splashArt[i].length(); ++j) {
                 ConsoleUtils::setChar(startX + j, startY + i, splashArt[i][j], COLOR_TITLE);
                 if (j % 5 == 0) { 
                      ConsoleUtils::renderBuffer();
                      ConsoleUtils::sleep(1);
                 }
             }
            ConsoleUtils::renderBuffer(); 
            ConsoleUtils::sleep(50);
        }

        std::string loadingText = "Loading...";
        ConsoleUtils::writeString((CONSOLE_WIDTH - loadingText.length()) / 2, startY + splashArt.size() + 2, loadingText, COLOR_INFO);


        ConsoleUtils::renderBuffer();
        ConsoleUtils::sleep(1500); 
    }

    void showMainMenu() {
        const std::vector<std::string> options = {"New Game", "Tutorial", "Options", "How to Play", "Exit"};
        int choice = -1;
        bool needsRedraw = true; 


        while (choice == -1 && running) { 
             if (needsRedraw) {


                 ui.drawBoard();
                 ConsoleUtils::renderBuffer();
                 needsRedraw = false;
             }

            choice = ui.runMenu("Main Menu", options, -1, -1); 


            bool returnToGame = false;
            switch (choice) {
                case 0: 
                    startGame();
                    return; 
                case 1:
                    showTutorial();
                    needsRedraw = true;
                    choice = -1;
                    break;
                case 2: 
                    showOptions();
                    needsRedraw = true; 
                    choice = -1; 
                    break;
                case 3: 
                    showHelpScreen();
                    needsRedraw = true; 
                    choice = -1; 
                    break;
                case 4: 
                    running = false; 
                    break;
                default: 

                     if (game.isGameOver() || game.isGameWon()) {
                         choice = -1; 
                         needsRedraw = true;
                     } else {
                         returnToGame = true; 
                     }
                    break;
            }
             if (returnToGame) break; 
        }

        if (running && choice != 0) { 
             ui.drawBoard();
             ConsoleUtils::renderBuffer();
        }
    }
    
    void showTutorial() {
        ConsoleUtils::clearBuffer();
        ConsoleUtils::drawBox(0, 0, CONSOLE_WIDTH, CONSOLE_HEIGHT, COLOR_BORDER);
        ConsoleUtils::writeString((CONSOLE_WIDTH - 21) / 2, 1, "SOLITAIRE TUTORIAL", COLOR_TITLE | NEO_BACKGROUND_BLUE);
        
        int currentLesson = 0;
        bool tutorialActive = true;
        
        std::vector<std::wstring> lessons = {
            L"Welcome to the Solitaire Tutorial! In this tutorial, you'll learn how to play step by step.",
            L"OBJECTIVE: Move all cards to the 4 Foundations (top right) sorted by suit from Ace to King.",
            L"TABLEAU: Cards in the middle area are built DOWN in alternating colors (red on black, black on red).",
            L"STOCK & WASTE: When stuck, draw cards from the Stock (top left) by pressing D or clicking it.",
            L"FOUNDATIONS: Built UP by suit from Ace to King. Cards can be moved here by selecting them and pressing Enter.",
            L"MOVING CARDS: Select a card with Enter/Space, then select destination and press Enter/Space again.",
            L"POWER-UPS: Use special abilities like Undo (U), Shuffle (S), Reveal (V), Joker (J) and Hint (H).",
            L"BASIC STRATEGY: Try to reveal face-down cards and create empty spots for Kings.",
            L"TIP: Auto-move to foundations by clicking/selecting a valid card once (no destination needed).",
            L"Now you're ready to play! Start a new game from the main menu, and have fun!"
        };
        
        auto renderTutorialPage = [&](int page) {
            ConsoleUtils::clearBuffer();
            ConsoleUtils::drawBox(0, 0, CONSOLE_WIDTH, CONSOLE_HEIGHT, COLOR_BORDER);
            ConsoleUtils::writeString((CONSOLE_WIDTH - 21) / 2, 1, "SOLITAIRE TUTORIAL", COLOR_TITLE | NEO_BACKGROUND_BLUE);

            std::string pageInfo = "Lesson " + std::to_string(page + 1) + " of " + std::to_string(lessons.size());
            ConsoleUtils::writeString((CONSOLE_WIDTH - pageInfo.length()) / 2, 3, pageInfo, COLOR_LABEL);

            switch (page) {
                case 0: {

                    ConsoleUtils::drawBox(15, 8, 50, 10, COLOR_BORDER);
                    ConsoleUtils::writeStringWide(20, 10, L"♠ ♥ Welcome to Solitaire! ♦ ♣", COLOR_SUCCESS);
                    ConsoleUtils::writeStringWide(20, 12, L"Let's learn how to play step by step.", COLOR_DEFAULT);
                    ConsoleUtils::writeStringWide(20, 14, L"Use ← → keys to navigate the tutorial", COLOR_INFO);
                    break;
                }
                case 1: {

                    for (int i = 0; i < 4; i++) {
                        ConsoleUtils::drawBox(20 + i*15, 8, 6, 4, COLOR_BORDER);
                        switch (i) {
                            case 0: ConsoleUtils::writeStringWide(22 + i*15, 9, L"A♠", COLOR_CARD_BLACK); break;
                            case 1: ConsoleUtils::writeStringWide(22 + i*15, 9, L"A♥", COLOR_CARD_RED); break;
                            case 2: ConsoleUtils::writeStringWide(22 + i*15, 9, L"A♣", COLOR_CARD_BLACK); break;
                            case 3: ConsoleUtils::writeStringWide(22 + i*15, 9, L"A♦", COLOR_CARD_RED); break;
                        }
                        ConsoleUtils::writeString(20 + i*15, 13, "↑ Up to K", COLOR_INFO);
                    }
                    break;
                }
                case 2: {

                    int x = 20;
                    ConsoleUtils::drawBox(x, 8, 6, 4, COLOR_BORDER);
                    ConsoleUtils::writeStringWide(x+2, 9, L"5♠", COLOR_CARD_BLACK);
                    
                    ConsoleUtils::drawBox(x, 11, 6, 4, COLOR_BORDER);
                    ConsoleUtils::writeStringWide(x+2, 12, L"4♥", COLOR_CARD_RED);
                    
                    ConsoleUtils::drawBox(x, 14, 6, 4, COLOR_BORDER);
                    ConsoleUtils::writeStringWide(x+2, 15, L"3♠", COLOR_CARD_BLACK);
                    
                    ConsoleUtils::writeString(x+10, 11, "← Build DOWN in", COLOR_INFO);
                    ConsoleUtils::writeString(x+10, 12, "  alternating colors", COLOR_INFO);
                    break;
                }

                case 3: {

                    ConsoleUtils::drawBox(20, 8, 6, 4, COLOR_BORDER);
                    ConsoleUtils::writeString(22, 9, "[ ]", COLOR_CARD_BACK);
                    ConsoleUtils::writeString(19, 12, "STOCK", COLOR_LABEL);
                    
                    ConsoleUtils::drawBox(30, 8, 6, 4, COLOR_BORDER);
                    ConsoleUtils::writeStringWide(32, 9, L"7♥", COLOR_CARD_RED);
                    ConsoleUtils::writeString(29, 12, "WASTE", COLOR_LABEL);
                    
                    ConsoleUtils::writeString(40, 10, "Press D to", COLOR_INFO);
                    ConsoleUtils::writeString(40, 11, "draw cards", COLOR_INFO);
                    break;
                }
                case 6: {

                    int x = 20, y = 8;
                    std::vector<std::pair<std::string, std::string>> powerups = {
                        {"[U]ndo", "Take back a move"},
                        {"[S]huffle", "Shuffle stock & waste"},
                        {"Re[V]eal", "Flip hidden cards"},
                        {"[J]oker", "Create a wild card"},
                        {"[H]int", "Show possible move"}
                    };
                    
                    for (const auto& powerup : powerups) {
                        ConsoleUtils::writeString(x, y, powerup.first, COLOR_LABEL);
                        ConsoleUtils::writeString(x + 10, y, powerup.second, COLOR_DEFAULT);
                        y += 2;
                    }
                    break;
                }
            }

            ConsoleUtils::writeStringWide(10, 20, lessons[page], COLOR_DEFAULT);

            std::string navHelp = "Use ← → arrow keys to navigate, ESC to exit tutorial";
            ConsoleUtils::writeString((CONSOLE_WIDTH - navHelp.length()) / 2, CONSOLE_HEIGHT - 2, navHelp, COLOR_INFO);
            
            ConsoleUtils::renderBuffer();
        };
        
        renderTutorialPage(currentLesson);
        
        while (tutorialActive) {
            int key = ConsoleUtils::getKeyPress();
            
            switch (key) {
                case KEY_LEFT:
                    if (currentLesson > 0) {
                        currentLesson--;
                        renderTutorialPage(currentLesson);
                    }
                    break;
                case KEY_RIGHT:
                    if (currentLesson < static_cast<int>(lessons.size()) - 1) {
                        currentLesson++;
                        renderTutorialPage(currentLesson);
                    }
                    break;
                case KEY_ESC:
                    tutorialActive = false;
                    break;
            }
        }
    }

    void showOptions() {
         bool optionsOpen = true;
         while (optionsOpen) {
             Player& playerRef = game.getPlayer(); 

             std::vector<std::string> options = {
                "Player Name: " + playerRef.getName(),
                std::string("Colorblind Mode [") + (playerRef.getColorblindMode() ? "ON" : "OFF") + "]",
                std::string("Easy Mode       [") + (playerRef.getEasyMode() ? "ON" : "OFF") + "]",
                std::string("Powerups        [") + (playerRef.getPowerupsEnabled() ? "ON" : "OFF") + "]",
                std::string("Animations      [") + (ENABLE_ANIMATION ? "ON" : "OFF") + "]",
                "Save Settings",
                "Back"
             };

             ui.drawBoard();

             int choice = ui.runMenu("Options", options, -1, -1);


             switch (choice) {
                case 0: { 
                    std::string prompt = "Enter new name (max 20 chars): ";

                     int inputX = (CONSOLE_WIDTH - prompt.length() - 20) / 2;
                     int inputY = CONSOLE_HEIGHT / 2 + options.size() + 2; 

                     ConsoleUtils::fillRectangle(1, inputY, CONSOLE_WIDTH - 2, 1, L' ', COLOR_DEFAULT); 
                     ConsoleUtils::writeString(inputX, inputY, prompt, COLOR_LABEL);
                     ConsoleUtils::renderBuffer();

                     std::string newName = ConsoleUtils::getTextInput(inputX + prompt.length(), inputY, 20, playerRef.getName());

                     if (!newName.empty()) { 
                         playerRef.setName(newName);
                     }

                     ConsoleUtils::fillRectangle(1, inputY, CONSOLE_WIDTH - 2, 1, L' ', COLOR_DEFAULT);


                     break;
                 }
                case 1: 
                    playerRef.toggleColorblindMode();
                    ConsoleUtils::playSound(700, 50);
                    break;
                case 2: 
                     playerRef.toggleEasyMode(); 
                     ConsoleUtils::playSound(700, 50);


                     ui.setStatusMessage(std::wstring(L"Easy Mode ") + (playerRef.getEasyMode() ? L"ON" : L"OFF") + L". Powerups reset.", 1500);
                     break;
                case 3:
                     playerRef.togglePowerupsEnabled();
                     ConsoleUtils::playSound(700, 50);
                     ui.setStatusMessage(std::wstring(L"Powerups ") + (playerRef.getPowerupsEnabled() ? L"ON" : L"OFF"), 1500);
                     break;
                case 4:


                     ui.setStatusMessage(L"Animation settings aren't persistent yet.", 1500);
                     break;
                case 5: 
                    playerRef.saveToFile();
                    ui.setStatusMessage(L"Settings saved.", 1500); 
                    ConsoleUtils::playSound(1000, 80);
                    break;
                case 6: 
                case -1: 
                    optionsOpen = false; 
                    break;
             }

         }

         ui.drawBoard();
         ConsoleUtils::renderBuffer();
    }

    void showHelpScreen() {
        ConsoleUtils::clearBuffer();
        ConsoleUtils::drawBox(0, 0, CONSOLE_WIDTH, CONSOLE_HEIGHT, COLOR_BORDER); 
        ConsoleUtils::writeString((CONSOLE_WIDTH - 13) / 2, 1, "HOW TO PLAY", COLOR_TITLE);

        int currentY = 4;
        int leftColX = 5;
        int rightColX = CONSOLE_WIDTH / 2 + 3;

        ConsoleUtils::writeString(leftColX, currentY++, "Objective:", COLOR_LABEL);
        ConsoleUtils::writeString(leftColX, currentY++, "- Move all 52 cards to the 4 Foundation piles (top right).", COLOR_DEFAULT);
        ConsoleUtils::writeString(leftColX, currentY++, "- Foundations build UP by suit (A, 2... K).", COLOR_DEFAULT);
        currentY++;
        ConsoleUtils::writeString(leftColX, currentY++, "Tableau (Middle):", COLOR_LABEL);
        ConsoleUtils::writeString(leftColX, currentY++, "- Build DOWN by alternating colors (Red on Black, Black on Red).", COLOR_DEFAULT);
        ConsoleUtils::writeString(leftColX, currentY++, "- Move single cards or valid sequences between piles.", COLOR_DEFAULT);
        ConsoleUtils::writeString(leftColX, currentY++, "- Empty piles accept only Kings (or sequences starting with K/Joker).", COLOR_DEFAULT);
        currentY++;
        ConsoleUtils::writeString(leftColX, currentY++, "Stock & Waste (Top Left):", COLOR_LABEL);
        ConsoleUtils::writeString(leftColX, currentY++, "- Click Stock or press [D] to deal 1 card to Waste.", COLOR_DEFAULT);
        ConsoleUtils::writeString(leftColX, currentY++, "- Click empty Stock [R] to recycle Waste back to Stock (penalty).", COLOR_DEFAULT);
        ConsoleUtils::writeString(leftColX, currentY++, "- Move top Waste card to Foundations or Tableau.", COLOR_DEFAULT);

        currentY = 4;
        ConsoleUtils::writeString(rightColX, currentY++, "Controls:", COLOR_LABEL);
        ConsoleUtils::writeString(rightColX, currentY++, "- Arrow Keys : Navigate Selection (Up/Down)", COLOR_DEFAULT);
        ConsoleUtils::writeString(rightColX, currentY++, "- Arrow Keys       : Navigate Selection (Left/Right)", COLOR_DEFAULT);
        ConsoleUtils::writeString(rightColX, currentY++, "- Enter / Spacebar : Select Source / Destination", COLOR_DEFAULT);
        ConsoleUtils::writeString(rightColX, currentY++, "  (Auto-moves to Foundation if possible on first click)", COLOR_DEFAULT);
        ConsoleUtils::writeString(rightColX, currentY++, "- [D]: Draw from Stock / Recycle Waste", COLOR_DEFAULT);
        ConsoleUtils::writeString(rightColX, currentY++, "- [U]: Undo Last Move", COLOR_DEFAULT);
        currentY++;
        ConsoleUtils::writeString(rightColX, currentY++, "Powerups:", COLOR_LABEL);
        ConsoleUtils::writeString(rightColX, currentY++, "- [S]: Shuffle Stock & Waste", COLOR_DEFAULT);
        ConsoleUtils::writeString(rightColX, currentY++, "- [V]: Reveal 1 face-down card per pile", COLOR_DEFAULT);
        ConsoleUtils::writeString(rightColX, currentY++, "- [J]: Place Joker on selected Tableau pile", COLOR_DEFAULT);
        ConsoleUtils::writeString(rightColX, currentY++, "- [H]: Show a Hint for a possible move", COLOR_DEFAULT);
        currentY++;
        ConsoleUtils::writeString(rightColX, currentY++, "Other:", COLOR_LABEL);
        ConsoleUtils::writeString(rightColX, currentY++, "- [O]: Options Menu", COLOR_DEFAULT);
        ConsoleUtils::writeString(rightColX, currentY++, "- [Q]: Quit Game to Main Menu", COLOR_DEFAULT);
        ConsoleUtils::writeString(rightColX, currentY++, "- [Esc]: Cancel Selection / Back (Menus)", COLOR_DEFAULT);

        ConsoleUtils::writeString((CONSOLE_WIDTH - 21) / 2, CONSOLE_HEIGHT - 3, "Press any key to return", COLOR_INFO);

        ConsoleUtils::renderBuffer();
        ConsoleUtils::getKeyPress(); 
    }

    void startGame() {
         try {
             game.newGame(); 
             ui.clearSelections(); 
             sourceSelection.clear();
             ui.clearStatusMessage();

             if (game.getPlayer().isFirstTimePlayer()) {

                 ConsoleUtils::clearBuffer();
                 ConsoleUtils::drawBox(0, 0, CONSOLE_WIDTH, CONSOLE_HEIGHT, COLOR_BORDER);
                 
                 std::string title = " Welcome to NeoSolitaire! ";
                 ConsoleUtils::writeString((CONSOLE_WIDTH - title.length()) / 2, 1, title, COLOR_TITLE | NEO_BACKGROUND_BLUE);
                 
                 int centerY = CONSOLE_HEIGHT / 2 - 4;
                 std::vector<std::string> messages = {
                     "It looks like this is your first time playing.",
                     "Would you like to go through the tutorial first?",
                     "This will help you understand the rules and controls."
                 };
                 
                 for (size_t i = 0; i < messages.size(); i++) {
                     ConsoleUtils::writeString((CONSOLE_WIDTH - messages[i].length()) / 2, centerY + i, messages[i], COLOR_DEFAULT);
                 }
                 
                 std::vector<std::string> options = {"Yes, show tutorial", "No, start playing"};
                 int choice = ui.runMenu("", options, -1, centerY + 5);
                 
                 if (choice == 0) {

                     showTutorial();
                 }

                 game.getPlayer().setFirstTimePlayer(false);
                 game.getPlayer().saveToFile();
             }

             ui.animateInitialDeal();

             ui.drawBoard();
             ConsoleUtils::renderBuffer();
         } catch (const std::exception& e) {

             ConsoleUtils::cleanupConsole();
             std::cerr << "FATAL ERROR during game setup: " << e.what() << std::endl;
             running = false; 
         }
    }

public:
    GameController() : game(), ui(game), running(true) {}

    void run() {
        displaySplash(); 
        showMainMenu();

        while (running) {
            if (game.isGameOver() || game.isGameWon()) {
                processInput(); 

            } else {
                 processInput();
            }

            ConsoleUtils::sleep(16); 
        }

        ConsoleUtils::cleanupConsole();
        std::cout << "\nThanks for playing NeoSolitaire!\n";
    }
};


int main() {
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);

    try {
        GameController controller;
        controller.run(); 
    } catch (const std::exception& e) {

        ConsoleUtils::cleanupConsole(); 
        std::cerr << "\n\nAn unexpected error occurred: " << e.what() << std::endl;
        std::cerr << "Exiting." << std::endl;
        return 1; 
    } catch (...) {

        ConsoleUtils::cleanupConsole();
        std::cerr << "\n\nAn unknown fatal error occurred. Exiting." << std::endl;
        return 2; 
    }

    return 0; 
}
