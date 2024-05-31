#include <iostream>
#include <fstream>
#include <string>

using namespace std;

// Розмір дошки
const int SIZE = 3;

// Структура для зберігання історії перемог
struct Score {
    int xWins;
    int oWins;
    int draws;
};

// Функція для зчитування історії перемог з файлу
Score readScoreFromFile(const string& filename) {
    ifstream file(filename);
    Score score = { 0, 0, 0 };

    if (file.is_open()) {
        file >> score.xWins >> score.oWins >> score.draws;
        file.close();
    }

    return score;
}

// Функція для запису історії перемог у файл
void writeScoreToFile(const string& filename, const Score& score) {
    ofstream file(filename);

    if (file.is_open()) {
        file << score.xWins << " " << score.oWins << " " << score.draws;
        file.close();
    }
}

// Функція для відображення дошки
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

// Функція для перевірки переможця
char checkWinner(char board[SIZE][SIZE]) {
    // Перевірка рядків і стовпців
    for (int i = 0; i < SIZE; ++i) {
        if (board[i][0] == board[i][1] && board[i][1] == board[i][2] && board[i][0] != ' ') {
            return board[i][0];
        }
        if (board[0][i] == board[1][i] && board[1][i] == board[2][i] && board[0][i] != ' ') {
            return board[0][i];
        }
    }
    // Перевірка діагоналей
    if (board[0][0] == board[1][1] && board[1][1] == board[2][2] && board[0][0] != ' ') {
        return board[0][0];
    }
    if (board[0][2] == board[1][1] && board[1][1] == board[2][0] && board[0][2] != ' ') {
        return board[0][2];
    }
    // Нічия
    return ' ';
}

// Функція для перевірки, чи залишилися пусті клітинки
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

int main() {
    // Ім'я файлу для зберігання історії перемог
    const string filename = "score.txt";

    // Зчитування історії перемог з файлу
    Score score = readScoreFromFile(filename);

    // Ініціалізація пустої дошки
    char board[SIZE][SIZE] = {
        {' ', ' ', ' '},
        {' ', ' ', ' '},
        {' ', ' ', ' '}
    };

    char currentPlayer = 'X';  // Починає гравець X
    bool gameOver = false;

    while (!gameOver) {
        // Відображення дошки
        displayBoard(board);

        // Запит ходу у поточного гравця
        int row, col;
        cout << "Player " << currentPlayer << ", enter your move (row and column): ";
        cin >> row >> col;

        // Перевірка валідності ходу
        if (row >= 0 && row < SIZE && col >= 0 && col < SIZE && board[row][col] == ' ') {
            board[row][col] = currentPlayer;

            // Перевірка переможця
            char winner = checkWinner(board);
            if (winner != ' ') {
                displayBoard(board);
                cout << "Player " << winner << " wins!" << endl;
                if (winner == 'X') {
                    score.xWins++;
                }
                else {
                    score.oWins++;
                }
                gameOver = true;
            }
            else if (isBoardFull(board)) {
                displayBoard(board);
                cout << "It's a draw!" << endl;
                score.draws++;
                gameOver = true;
            }
            else {т
                // Зміна гравця
                currentPlayer = (currentPlayer == 'X') ? 'O' : 'X';
            }
        }
        else {
            cout << "Invalid move. Try again." << endl;
        }
    }

    // Запис історії перемог у файл
    writeScoreToFile(filename, score);

    // Відображення загальної історії перемог
    cout << "X wins: " << score.xWins << endl;
    cout << "O wins: " << score.oWins << endl;
    cout << "Draws: " << score.draws << endl;

    return 0;
}
