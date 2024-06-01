#include <iostream>
#include <fstream>
#include <string>
#include <ctime>
#include <iomanip>
#include <json/json.h>

using namespace std;
#pragma warning(disable : 4996)
// Board size
const int SIZE = 3;
const char PLAYER_X = 'X';
const char PLAYER_O = 'O';

// Structure to store win history
struct Score {
    int xWins;
    int oWins;
    int draws;
};

// Function to read win history from a file
Score readScoreFromFile(const string& filename) {
    ifstream file(filename);
    Score score = { 0, 0, 0 };

    if (file.is_open()) {
        file >> score.xWins >> score.oWins >> score.draws;
        file.close();
    }

    return score;
}

// Function to write win history to a file
void writeScoreToFile(const string& filename, const Score& score) {
    ofstream file(filename);

    if (file.is_open()) {
        file << score.xWins << " " << score.oWins << " " << score.draws;
        file.close();
    }
}

// Function to read win history from a JSON file
Score readScoreFromJson(const string& filename) {
    ifstream file(filename);
    Score score = { 0, 0, 0 };

    if (file.is_open()) {
        Json::Value root;
        file >> root;
        score.xWins = root["wins"]["X"].asInt();
        score.oWins = root["wins"]["O"].asInt();
        score.draws = root["wins"]["draws"].asInt();
        file.close();
    }

    return score;
}

// Function to write win history to a JSON file
void writeScoreToJson(const string& filename, const Score& score) {
    ofstream file(filename);

    if (file.is_open()) {
        Json::Value root;
        root["wins"]["X"] = score.xWins;
        root["wins"]["O"] = score.oWins;
        root["wins"]["draws"] = score.draws;

        file << root;
        file.close();
    }
}

// Function to display the board
void displayBoard(char board[SIZE][SIZE]) {
    for (int i = 0; i < SIZE; ++i) {
        for (int j = 0; j < SIZE; ++j) {
            cout << board[i][j];
            if (j < SIZE - 1) cout << " | ";
        }
        cout << endl;
        if (i < SIZE - 1) cout << "--+---+--" << endl;
    }
}

// Function to check the winner
char checkWinner(char board[SIZE][SIZE]) {
    // Check rows and columns
    for (int i = 0; i < SIZE; ++i) {
        if (board[i][0] == board[i][1] && board[i][1] == board[i][2] && board[i][0] != ' ') {
            return board[i][0];
        }
        if (board[0][i] == board[1][i] && board[1][i] == board[2][i] && board[0][i] != ' ') {
            return board[0][i];
        }
    }
    // Check diagonals
    if (board[0][0] == board[1][1] && board[1][1] == board[2][2] && board[0][0] != ' ') {
        return board[0][0];
    }
    if (board[0][2] == board[1][1] && board[1][1] == board[2][0] && board[0][2] != ' ') {
        return board[0][2];
    }
    // No winner
    return ' ';
}

// Function to check if the board is full
bool isBoardFull(char board[SIZE][SIZE]) {
    for (int i = 0; i < SIZE; ++i) {
        for (int j = 0; j < SIZE; ++j) {
            if (board[i][j] == ' ') {
                return false;
            }
        }
    }
    return true;
}

// Function to validate a move
bool isValidMove(int row, int col, char board[SIZE][SIZE]) {
    return row >= 0 && row < SIZE && col >= 0 && col < SIZE && board[row][col] == ' ';
}

// Function to get the current timestamp as a string
string currentTimestamp() {
    auto t = time(nullptr);
    auto tm = localtime(&t);
    ostringstream oss;
    oss << put_time(tm, "%Y-%m-%d %H:%M:%S");
    return oss.str();
}

// Function to write logs to a file
void logToFile(const string& filename, const string& message) {
    ofstream file(filename, ios::app);

    if (file.is_open()) {
        file << currentTimestamp() << "," << message << endl;
        file.close();
    }
}

int main() {
    // Filename for storing win history
    const string scoreFilename = "score.txt";
    const string logFilename = "logs.csv";
    const string jsonFilename = "wins.json";

    // Read win history from file
    Score score = readScoreFromJson(jsonFilename);

    // Log game start
    logToFile(logFilename, "User started the Game");

    // Initialize an empty board
    char board[SIZE][SIZE] = {
        {' ', ' ', ' '},
        {' ', ' ', ' '},
        {' ', ' ', ' '}
    };

    char currentPlayer = PLAYER_X;  // Player X starts
    bool gameOver = false;

    while (!gameOver) {
        // Display the board
        displayBoard(board);

        // Prompt the current player for a move
        int row, col;
        cout << "Player " << currentPlayer << ", enter your move (row and column): ";
        cin >> row >> col;

        // Validate the move
        if (isValidMove(row, col, board)) {
            board[row][col] = currentPlayer;

            // Log the move
            ostringstream moveLog;
            moveLog << "User " << currentPlayer << " chose the (" << row + 1 << "," << col + 1 << ") cell";
            logToFile(logFilename, moveLog.str());

            // Check for a winner
            char winner = checkWinner(board);
            if (winner != ' ') {
                displayBoard(board);
                cout << "Player " << winner << " wins!" << endl;
                if (winner == PLAYER_X) {
                    score.xWins++;
                }
                else {
                    score.oWins++;
                }
                gameOver = true;

                // Log the game result
                logToFile(logFilename, string("Player ") + winner + " wins");
            }
            else if (isBoardFull(board)) {
                displayBoard(board);
                cout << "It's a draw!" << endl;
                score.draws++;
                gameOver = true;

                // Log the game result
                logToFile(logFilename, "It's a draw");
            }
            else {
                // Switch player
                currentPlayer = (currentPlayer == PLAYER_X) ? PLAYER_O : PLAYER_X;
            }
        }
        else {
            cout << "Invalid move. Try again." << endl;
        }
        cout << currentTimestamp() << endl;
    }

    // Write win history to file
    writeScoreToJson(jsonFilename, score);

    // Display overall win history
    cout << "X wins: " << score.xWins << endl;
    cout << "O wins: " << score.oWins << endl;
    cout << "Draws: " << score.draws << endl;

    return 0;
}
