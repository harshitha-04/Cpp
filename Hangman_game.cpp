#include <iostream>
#include <fstream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <string>
#include <algorithm>

using namespace std;

// Struct to hold game name and description
struct Game {
    string name;
    string description;
};

// Function to read games and descriptions from a file
vector<Game> readGamesFromFile(const string& filename) {
    vector<Game> games;
    ifstream file(filename);

    if (file.is_open()) {
        Game game;
        while (getline(file, game.name, ':')) {
            getline(file, game.description);
            games.push_back(game);
        }
        file.close();
    } else {
        cerr << "Error: Unable to open file " << filename << endl;
    }

    return games;
}

// Function to choose a random game
Game chooseRandomGame(const vector<Game>& games) {
    int randomIndex = rand() % games.size();
    return games[randomIndex];
}

// Function to display the current state of the word with blanks for unrevealed letters
string displayWord(const string& secretWord, const vector<char>& guessedLetters) {
    string display;
    for (char letter : secretWord) {
        char lowerCaseLetter = tolower(letter); // Convert letter to lowercase
        if (find(guessedLetters.begin(), guessedLetters.end(), lowerCaseLetter) != guessedLetters.end()) {
            display += letter;
        } else {
            display += '_';
        }
    }
    return display;
}

// Function to display the hangman figure
void displayHangman(int incorrectAttempts) {
    cout << "\n";

    if (incorrectAttempts >= 1) {
        cout << "  +---+" << endl;
        cout << "  |   |" << endl;
        cout << "      |" << endl;
        cout << "      |" << endl;
    }

    if (incorrectAttempts >= 2) {
        cout << "      O" << endl;
        cout << "      |" << endl;
    }

    if (incorrectAttempts >= 3) {
        cout << "     /|\\" << endl;
        cout << "    / | \\" << endl;
    }

    if (incorrectAttempts >= 4) {
        cout << "      |" << endl;
    }

    if (incorrectAttempts >= 5) {
        cout << "     / \\" << endl;
        cout << "    /   \\" << endl;
    }

    cout << "\n";
}

void displayWelcomeMessage() {
    cout << "Welcome to Hangman!" << endl;
    cout << "Try to guess the name of the game." << endl;
    cout << "Guess one letter at a time, and be careful! You have 6 attempts." << endl;
    cout << "Let's get started!" << endl;
}

int main() {
    // Seed the random number generator
    srand(static_cast<unsigned int>(time(nullptr)));

    displayWelcomeMessage();

    do {
        // Read games and descriptions from the file
        vector<Game> games = readGamesFromFile("games.txt");

        // Check if there are games in the file
        if (games.empty()) {
            cerr << "Error: No games found in the file." << endl;
            return 1;
        }

        // Choose a random game
        Game selectedGame = chooseRandomGame(games);

        // Display the description of the selected game
        cout << "Game Description: " << selectedGame.description << endl;

        // Variables for the game
        vector<char> guessedLetters;
        // Initialize guessedLetters with the first letter of the game's name
        guessedLetters.push_back(tolower(selectedGame.name[0]));
        int maxAttempts = 6;
        int attempts = 0;
        int score = 0;

        cout << "Welcome to Hangman!" << endl;

        while (attempts < maxAttempts) {
            // Display the current state of the word
            cout << "Word: " << displayWord(selectedGame.name, guessedLetters) << endl;

            // Display guessed letters
            if (!guessedLetters.empty()) {
                cout << "Guessed Letters: ";
                for (char letter : guessedLetters) {
                    cout << letter << " ";
                }
                cout << endl;
            }

            // Get the user's guess
            cout << "Enter a letter: ";
            char guess;
            cin >> guess;

            // Convert the guess to lowercase
            guess = tolower(guess);

            // Check if the letter has already been guessed
            if (find(guessedLetters.begin(), guessedLetters.end(), guess) != guessedLetters.end()) {
                cout << "You've already guessed that letter. Try again." << endl;
                continue;
            }

            // Add the guessed letter to the vector
            guessedLetters.push_back(guess);

            // Check if the guessed letter is in the word
            if (selectedGame.name.find(guess) != string::npos) {
                cout << "Good guess!" << endl;
            } else {
                cout << "Incorrect guess. Try again." << endl;
                attempts++;
            }

            // Check if the word has been completely guessed
            if (displayWord(selectedGame.name, guessedLetters) == selectedGame.name) {
                cout << "Congratulations! You've guessed the game." << endl;
                score += (maxAttempts - attempts) * 10; // Adjust the scoring system as needed
                cout << "Your score: " << score << endl;
                break;
            }

            // Inside the main loop:
            displayHangman(attempts);
        }

        // Display the result
        if (attempts == maxAttempts) {
            cout << "Sorry, you've run out of attempts. The correct game was not revealed." << endl;
            cout << "Better luck next time!" << endl;
        }

        // Ask if the player wants to restart or exit
        cout << "Do you want to play again? (Y/N): ";
        char playAgain;
        cin >> playAgain;

        if (playAgain != 'Y' && playAgain != 'y') {
            break; // Exit the loop if the player chooses not to play again
        }

        // Clear guessedLetters vector for the next round
        guessedLetters.clear();

    } while (true);

    return 0;
}
