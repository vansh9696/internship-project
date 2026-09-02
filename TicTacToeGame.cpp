#include <iostream>
#include <vector>

using namespace std;

class TicTacToe {
private:
    char board[3][3];
    char currentPlayer;
    int movesCount;

public:
    TicTacToe() {
        resetGame();
    }

    void resetGame() {
        char initialChar = '1';
        for (int i = 0; i < 3; ++i) {
            for (int j = 0; j < 3; ++j) {
                board[i][j] = initialChar++;
            }
        }
        currentPlayer = 'X';
        movesCount = 0;
    }

    void displayBoard() const {
        cout << "\n";
        cout << "     |     |     \n";
        cout << "  " << board[0][0] << "  |  " << board[0][1] << "  |  " << board[0][2] << "  \n";
        cout << "_____|_____|_____\n";
        cout << "     |     |     \n";
        cout << "  " << board[1][0] << "  |  " << board[1][1] << "  |  " << board[1][2] << "  \n";
        cout << "_____|_____|_____\n";
        cout << "     |     |     \n";
        cout << "  " << board[2][0] << "  |  " << board[2][1] << "  |  " << board[2][2] << "  \n";
        cout << "     |     |     \n\n";
    }

    bool makeMove(int cellNumber) {
        if (cellNumber < 1 || cellNumber > 9) {
            cout << "[ERROR] Invalid choice! Select a cell number between 1 and 9.\n";
            return false;
        }

        int row = (cellNumber - 1) / 3;
        int col = (cellNumber - 1) % 3;

        if (board[row][col] == 'X' || board[row][col] == 'O') {
            cout << "[ERROR] Cell already occupied! Choose an empty cell.\n";
            return false;
        }

        board[row][col] = currentPlayer;
        movesCount++;
        return true;
    }

    bool checkWin() const {
        // Rows and Columns Check
        for (int i = 0; i < 3; ++i) {
            if (board[i][0] == currentPlayer && board[i][1] == currentPlayer && board[i][2] == currentPlayer)
                return true;
            if (board[0][i] == currentPlayer && board[1][i] == currentPlayer && board[2][i] == currentPlayer)
                return true;
        }

        // Diagonals Check
        if (board[0][0] == currentPlayer && board[1][1] == currentPlayer && board[2][2] == currentPlayer)
            return true;
        if (board[0][2] == currentPlayer && board[1][1] == currentPlayer && board[2][0] == currentPlayer)
            return true;

        return false;
    }

    bool isDraw() const {
        return movesCount == 9;
    }

    void switchPlayer() {
        currentPlayer = (currentPlayer == 'X') ? 'O' : 'X';
    }

    char getCurrentPlayer() const {
        return currentPlayer;
    }

    void playGame() {
        int cell;
        bool gameOver = false;

        while (!gameOver) {
            displayBoard();
            cout << "Player " << currentPlayer << "'s turn. Enter cell number (1-9): ";
            cin >> cell;

            if (cin.fail()) {
                cin.clear();
                cin.ignore(1000, '\n');
                cout << "[ERROR] Please enter a valid number.\n";
                continue;
            }

            if (!makeMove(cell)) {
                continue;
            }

            if (checkWin()) {
                displayBoard();
                cout << "==========================================\n";
                cout << " 🎉 CONGRATULATIONS! Player " << currentPlayer << " WINS! 🎉\n";
                cout << "==========================================\n";
                gameOver = true;
            } else if (isDraw()) {
                displayBoard();
                cout << "==========================================\n";
                cout << " 🤝 It's a DRAW! Well played both players.\n";
                cout << "==========================================\n";
                gameOver = true;
            } else {
                switchPlayer();
            }
        }
    }
};

int main() {
    TicTacToe game;
    char playAgain;

    do {
        cout << "\n==========================================";
        cout << "\n          TIC TAC TOE CONSOLE GAME        ";
        cout << "\n==========================================";

        game.resetGame();
        game.playGame();

        cout << "\nDo you want to play another round? (y/n): ";
        cin >> playAgain;

    } while (playAgain == 'y' || playAgain == 'Y');

    cout << "\nThanks for playing! Have a great day.\n";
    return 0;
}