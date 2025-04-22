![NeoSolitaire Icon](https://i.imgur.com/1mSIKP4.png)

This README was written in two languages:
- **English** [NeoSolitaire](#neosolitaire-english)
-  **Polski** [NeoPasjans](#neopasjans-polski)

---
![NeoSolitaire Menu](https://i.imgur.com/zZusAt9.png)

<img src="https://i.imgur.com/e0d5icw.gif" width="530">

# NeoSolitaire (English)

## Table of Contents
1. [Project Overview](#project-overview)
2. [Setup and Compilation](#setup-and-compilation)
3. [Game Instructions](#game-instructions)
4. [Key Features](#key-features)

---
<img src="https://i.imgur.com/ynuNJ0i.png" width="50">

## Project Overview

NeoSolitaire is a feature-rich console-based implementation of the classic Solitaire (Klondike) card game. The game presents a polished, animated interface with smooth card movements and visual effects. It features:

- **Rich Console UI**: Colorful, well-designed console interface with card animations and visual effects.
- **Standard Solitaire Rules**: Follow traditional Klondike Solitaire gameplay.
- **Powerups System**: Special abilities to help when you're stuck.
- **Game Statistics**: Track your score, moves, and time.
- **Customization Options**: Accessibility features like colorblind mode and difficulty settings.

---
<img src="https://i.imgur.com/MBhACeb.png" width="50">

## Setup and Compilation

### Prerequisites

To run this program, you need a C++ development environment. The game is designed for Windows systems because it uses Windows-specific console functions. You will need:

- A **C++ compiler** (e.g., Visual Studio, MinGW, or g++).
- A Windows operating system.
- A terminal or command-line interface that supports UTF-8 characters.

### Compilation Instructions

1. **Download the code**:
   Clone or download the repository to your local machine.
   
2. **Open terminal**:
   Navigate to the folder where the game code is stored.

3. **Compile the code**:
   Using a terminal, run the following command to compile the program (assuming you're using g++):

   ```
   g++ -o NeoSolitaire NeoSolitaire.cpp
   ```

   This will create an executable named `NeoSolitaire`.

5. **Run the game**:
   After compiling, run the game by typing:

   ```
   NeoSolitaire.exe
   ```

---
<img src="https://i.imgur.com/Rh4y2GJ.png" width="50">

## Game Instructions

### Objective

The goal of Solitaire is to move all cards to the four foundation piles, organized by suit and in ascending order (Ace to King).

### Controls

- **Arrow Keys**: Navigate around the board.
- **Enter/Space**: Select cards and destinations.
- **D**: Draw a card from the stock.
- **U**: Undo a move (limited uses).
- **H**: Get a hint (limited uses).
- **S**: Shuffle remaining cards (limited uses).
- **V**: Reveal face-down cards (limited uses).
- **J**: Place a Joker card (limited uses).
- **O**: Options menu.
- **Q**: Quit/Main menu.

### Game Elements

- **Stock**: The pile of face-down cards in the upper-left corner.
- **Waste**: Cards turned up from the stock pile.
- **Foundations**: Four piles in the upper-right for building up each suit from Ace to King.
- **Tableau**: Seven columns where cards can be arranged in descending order and alternating colors.

### Basic Rules

1. Build tableau piles in descending order with alternating colors (red and black).
2. Only Kings can be placed on empty tableau spots.
3. Build foundation piles in ascending order (Ace to King) by suit.
4. Move single cards or sequences of face-up cards between tableau piles.
5. When stock is empty, you can recycle the waste pile (with a point penalty).

### Powerups

- **Undo**: Revert your last move.
- **Shuffle**: Remix remaining stock and waste cards.
- **Reveal**: Turn over face-down cards on the tableau.
- **Joker**: Place a wild card that can substitute for any card.
- **Hint**: Get suggestions for your next move.

---
<img src="https://i.imgur.com/H3NP7i4.png" width="50">

## Key Features

1. **Animated Card Movements**:
   - Smooth animations for card movements.
   - Visual effects for special actions.

2. **Scoring System**:
   - Points for moving cards to foundations.
   - Time bonuses for quick completion.
   - Penalties for using powerups.

3. **Game Statistics**:
   - Track time, moves, and score.
   - Persistent high score.

4. **Accessibility Features**:
   - Custom difficulty settings.

5. **Game Aids**:
   - Hint system to suggest valid moves.
   - Undo functionality to correct mistakes.

---

<img src="https://i.imgur.com/AWoRB6y.png" width="500">

# NeoPasjans (Polski)

## Spis treści
1. [Opis projektu](#opis-projektu)
2. [Konfiguracja i kompilacja](#konfiguracja-i-kompilacja)
3. [Instrukcje do gry](#instrukcje-do-gry)
4. [Kluczowe funkcje](#kluczowe-funkcje)

---
<img src="https://i.imgur.com/ynuNJ0i.png" width="50">

## Opis projektu

NeoSolitaire to bogata w funkcje konsolowa implementacja klasycznej gry karcianej Pasjans (Klondike). Gra prezentuje dopracowany, animowany interfejs z płynnymi ruchami kart i efektami wizualnymi. Posiada:

- **Bogaty interfejs konsolowy**: Kolorowy, dobrze zaprojektowany interfejs konsolowy z animacjami kart i efektami wizualnymi
- **Standardowe zasady pasjansa**: Zgodność z tradycyjną rozgrywką pasjansa Klondike
- **System wzmocnień**: Specjalne umiejętności pomagające, gdy utkniesz
- **Statystyki gry**: Śledzenie wyniku, ruchów i czasu
- **Opcje dostosowywania**: Funkcje dostępności, takie jak tryb dla daltonistów i ustawienia trudności

---
<img src="https://i.imgur.com/MBhACeb.png" width="50">

## Konfiguracja i kompilacja

### Wymagania wstępne

Aby uruchomić ten program, potrzebujesz środowiska programistycznego C++. Gra jest zaprojektowana dla systemów Windows, ponieważ wykorzystuje funkcje konsoli specyficzne dla Windows. Będziesz potrzebować:

- **Kompilatora C++** (np. Visual Studio, MinGW lub g++).
- Systemu operacyjnego Windows.
- Terminala lub interfejsu wiersza poleceń obsługującego znaki UTF-8.

### Instrukcje kompilacji

1. **Pobierz kod**:
   Sklonuj lub pobierz repozytorium na swój lokalny komputer.
   
2. **Otwórz terminal**:
   Przejdź do folderu, w którym znajduje się kod gry.

3. **Kompiluj kod**:
   W terminalu uruchom następujące polecenie, aby skompilować program (zakładając, że używasz g++):

   ```
   g++ -o NeoSolitaire NeoSolitaire.cpp
   ```

   To stworzy plik wykonywalny o nazwie `NeoSolitaire`.

5. **Uruchom grę**:
   Po skompilowaniu uruchom grę, wpisując:

   ```
   NeoSolitaire.exe
   ```

---
<img src="https://i.imgur.com/Rh4y2GJ.png" width="50">

## Instrukcje do gry

### Cel

Celem Pasjansa jest przeniesienie wszystkich kart na cztery stosy fundamentów, uporządkowane według koloru i w kolejności rosnącej (od Asa do Króla).

### Sterowanie

- **Klawisze strzałek**: Nawigacja po planszy.
- **Enter/Spacja**: Wybór kart i miejsc docelowych.
- **D**: Dobierz kartę z talii.
- **U**: Cofnij ruch (ograniczona liczba użyć).
- **H**: Uzyskaj podpowiedź (ograniczona liczba użyć).
- **S**: Przetasuj pozostałe karty (ograniczona liczba użyć).
- **V**: Odkryj zakryte karty (ograniczona liczba użyć).
- **J**: Umieść kartę Jokera (ograniczona liczba użyć).
- **O**: Menu opcji.
- **Q**: Wyjście/Menu główne.

### Elementy gry

- **Talia**: Stos zakrytych kart w lewym górnym rogu.
- **Stos odrzuconych**: Karty odkryte z talii.
- **Fundamenty**: Cztery stosy w prawym górnym rogu do budowania każdego koloru od Asa do Króla.
- **Tableau**: Siedem kolumn, gdzie karty można układać w malejącej kolejności i naprzemiennych kolorach.

### Podstawowe zasady

1. Buduj stosy tableau w malejącej kolejności z naprzemiennymi kolorami (czerwonym i czarnym).
2. Tylko Króle mogą być umieszczane na pustych miejscach tableau.
3. Buduj stosy fundamentów w rosnącej kolejności (od Asa do Króla) według koloru.
4. Przenoś pojedyncze karty lub sekwencje odkrytych kart między stosami tableau.
5. Gdy talia jest pusta, możesz przetasować stos odrzuconych (z karą punktową).

### Wzmocnienia

- **Cofnij**: Cofnij ostatni ruch.
- **Tasowanie**: Przetasuj pozostałe karty w talii i stosie odrzuconych.
- **Odkrycie**: Odwróć zakryte karty na tableau.
- **Joker**: Umieść dzikią kartę, która może zastąpić dowolną kartę.
- **Podpowiedź**: Uzyskaj sugestie dotyczące następnego ruchu.

---
<img src="https://i.imgur.com/H3NP7i4.png" width="50">

## Kluczowe funkcje

1. **Animowane ruchy kart**:
   - Płynne animacje dla ruchów kart.
   - Efekty wizualne dla specjalnych akcji.

2. **System punktacji**:
   - Punkty za przenoszenie kart na fundamenty.
   - Bonusy czasowe za szybkie ukończenie.
   - Kary za używanie wzmocnień.

3. **Statystyki gry**:
   - Śledzenie czasu, ruchów i wyniku.
   - Trwały najlepszy wynik.

4. **Funkcje dostępności**:
   - Niestandardowe ustawienia trudności.

5. **Pomoce w grze**:
   - System podpowiedzi sugerujący prawidłowe ruchy.
   - Funkcjonalność cofania do poprawiania błędów.

---
<img src="https://i.imgur.com/JJwDYbq.gif" width="530">

`Animation displayed at game start.`

`Animacja wyświetlana podczas startu gry. `

---
