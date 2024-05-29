#include <iostream>

using namespace std;

// Размер доски
const int SIZE = 3;

// Функция для отображения доски
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

// Функция для проверки победителя
char checkWinner(char board[SIZE][SIZE]) {
    // Проверка строк и столбцов
    for (int i = 0; i < SIZE; ++i) {
        if (board[i][0] == board[i][1] && board[i][1] == board[i][2] && board[i][0] != ' ') {
            return board[i][0];
        }
        if (board[0][i] == board[1][i] && board[1][i] == board[2][i] && board[0][i] != ' ') {
            return board[0][i];
        }
    }
    // Проверка диагоналей
    if (board[0][0] == board[1][1] && board[1][1] == board[2][2] && board[0][0] != ' ') {
        return board[0][0];
    }
    if (board[0][2] == board[1][1] && board[1][1] == board[2][0] && board[0][2] != ' ') {
        return board[0][2];
    }
    // Ничья
    return ' ';
}

// Функция для проверки, остались ли пустые клетки
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
    // Инициализация пустой доски
    char board[SIZE][SIZE] = {
        {' ', ' ', ' '},
        {' ', ' ', ' '},
        {' ', ' ', ' '}
    };

    char currentPlayer = 'X';  // Начинает игрок X
    bool gameOver = false;

    while (!gameOver) {
        // Отображение доски
        displayBoard(board);

        // Запрос хода у текущего игрока
        int row, col;
        cout << "Player " << currentPlayer << ", enter your move (row and column): ";
        cin >> row >> col;

        // Проверка валидности хода
        if (row >= 0 && row < SIZE && col >= 0 && col < SIZE && board[row][col] == ' ') {
            board[row][col] = currentPlayer;

            // Проверка победителя
            char winner = checkWinner(board);
            if (winner != ' ') {
                displayBoard(board);
                cout << "Player " << winner << " wins!" << endl;
                gameOver = true;
            } else if (isBoardFull(board)) {
                displayBoard(board);
                cout << "It's a draw!" << endl;
                gameOver = true;
            } else {
                // Смена игрока
                currentPlayer = (currentPlayer == 'X') ? 'O' : 'X';
            }
        } else {
            cout << "Invalid move. Try again." << endl;
        }
    }

    return 0;
}
