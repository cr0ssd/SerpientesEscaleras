#include <iostream>
#include <ctime>
#include <cstdlib>

using namespace std;

const int BOARD_SIZE = 30;
const int NUM_SNAKES = 3;
const int NUM_LADDERS = 3;
const int SNAKE_PENALTY = 3;
const int LADDER_REWARD = 3;

enum TileType { NORMAL, SNAKE, LADDER };

struct Tile {
    TileType type;
};

void initializeBoard(Tile board[]) {
    for (int i = 0; i < BOARD_SIZE; ++i) {
        board[i].type = NORMAL;
    }

    srand(time(0));
    for (int i = 0; i < NUM_SNAKES; ++i) {
        int snakePos;
        do {
            snakePos = rand() % BOARD_SIZE;
        } while (board[snakePos].type != NORMAL && snakePos >= SNAKE_PENALTY); 
        board[snakePos].type = SNAKE;
    }

    for (int i = 0; i < NUM_LADDERS; ++i) {
        int ladderPos;
        do {
            ladderPos = rand() % BOARD_SIZE;
        } while (board[ladderPos].type != NORMAL && ladderPos < BOARD_SIZE - LADDER_REWARD); 
        board[ladderPos].type = LADDER;
    }
}

int rollDice() {
    return (rand() % 6) + 1;
}

int main() {
    Tile board[BOARD_SIZE];
    initializeBoard(board);

    int playerPositions[2] = {1, 1}; 
    int currentPlayer = 1;
    int turnCount = 0;

    char input;
    cout << "Press C to continue next turn, or E to end the game: ";
    cin >> input;

    while (input != 'E') {
        if (input != 'C') {
            cout << "Invalid option, please press C to continue next turn or E to end the game: " << endl;
            cin >> input;
            continue;
        }

        turnCount++;

        int diceRoll = rollDice();
        int targetPosition = playerPositions[currentPlayer - 1] + diceRoll;

        if (targetPosition <= BOARD_SIZE) {
            switch (board[targetPosition - 1].type) {
                case SNAKE:
                    targetPosition -= SNAKE_PENALTY;
                    break;
                case LADDER:
                    targetPosition += LADDER_REWARD;
                    break;
                default:
                    break;
            }
        }

        cout << turnCount << " " << currentPlayer << " " << playerPositions[currentPlayer - 1] << " "
             << diceRoll << " ";

        if (targetPosition <= BOARD_SIZE) {
            switch (board[targetPosition - 1].type) {
                case NORMAL:
                    cout << "N ";
                    break;
                case SNAKE:
                    cout << "S ";
                    break;
                case LADDER:
                    cout << "L ";
                    break;
            }
            cout << targetPosition << endl;
        } else {
            cout << "N " << BOARD_SIZE << endl; 
        }

        playerPositions[currentPlayer - 1] = targetPosition;

        if (playerPositions[currentPlayer - 1] >= BOARD_SIZE) {
            cout << "-- GAME OVER --" << endl;
            cout << "Player " << currentPlayer << " is the winner!!!" << endl;
            return 0;
        }

        currentPlayer = (currentPlayer == 1) ? 2 : 1;

        cout << "Press C to continue next turn, or E to end the game: ";
        cin >> input;
    }

    cout << "-- GAME OVER --" << endl;
    cout << "Thanks for playing!!!" << endl;

    return 0;
}