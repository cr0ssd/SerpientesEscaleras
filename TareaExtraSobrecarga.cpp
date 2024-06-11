#include <iostream>
#include <ctime>
#include <cstdlib>

using namespace std;

//turno
class Turno {
public:
    int numeroTurno;
    int jugador;
    int posicionInicial;
    int dado;
    char tipoCasilla;
    int posicionFinal;

    Turno(int numTurno, int jugadorActual, int posInicial, int valorDado, char tipoCasilla, int posFinal)
        : numeroTurno(numTurno), jugador(jugadorActual), posicionInicial(posInicial), dado(valorDado),
          tipoCasilla(tipoCasilla), posicionFinal(posFinal) {}

    // Sobrecarga << para printear el turno
    friend ostream& operator<<(ostream& os, const Turno& turno) {
        os << turno.numeroTurno << " " << turno.jugador << " " << turno.posicionInicial << " "
           << turno.dado << " " << turno.tipoCasilla << " " << turno.posicionFinal;
        return os;
    }
};

class Tile {
public:
    virtual int applyEffect(int currentPosition) = 0;
    virtual char getType() = 0;
    virtual int operator+(int pos) = 0;
};

class NormalTile : public Tile {
public:
    int applyEffect(int currentPosition) override {
        return currentPosition;
    }

    char getType() override {
        return 'N';
    }

    int operator+(int pos) override {
        return pos;
    }
};

class SnakeTile : public Tile {
private:
    int penalty;

public:
    SnakeTile(int penalty) : penalty(penalty) {}

    int applyEffect(int currentPosition) override {
        return currentPosition - penalty;
    }

    char getType() override {
        return 'S';
    }

    // Sobrecarga + para snakecase
    int operator+(int pos) override {
        return pos - abs(penalty); 
    }
};

class LadderTile : public Tile {
private:
    int reward;

public:
    LadderTile(int reward) : reward(reward) {}

    int applyEffect(int currentPosition) override {
        return currentPosition + reward;
    }

    char getType() override {
        return 'L';
    }

    int operator+(int pos) override {
        return pos + reward;
    }
};

class Game {
protected:
    int numPlayers;
    int maxTurns;

public:
    Game(int numPlayers, int maxTurns) : numPlayers(numPlayers), maxTurns(maxTurns) {}
    virtual void play(Tile** board, int boardSize) = 0;
};

class ManualGame : public Game {
public:
    ManualGame(int numPlayers, int maxTurns) : Game(numPlayers, maxTurns) {}

    void play(Tile** board, int boardSize) override {
        int* playerPositions = new int[numPlayers];
        for (int i = 0; i < numPlayers; ++i) {
            playerPositions[i] = 1;
        }

        int currentPlayer = 0;
        int turnCount = 0;
        char input;

        while (turnCount < maxTurns) {
            cout << "Press C to continue next turn, or E to end the game: ";
            cin >> input;

            if (input == 'E') {
                break;
            } else if (input != 'C') {
                cout << "Invalid option, please press C to continue next turn or E to end the game: " << endl;
                continue;
            }

            turnCount++;

            int diceRoll = (rand() % 6) + 1;
            int targetPosition = playerPositions[currentPlayer] + diceRoll;

            if (targetPosition <= boardSize) {
                targetPosition = board[targetPosition - 1]->operator+(targetPosition); 
            }

            Turno turnoActual(turnCount, currentPlayer + 1, playerPositions[currentPlayer],
                              diceRoll, board[targetPosition - 1]->getType(), targetPosition);
            cout << turnoActual << endl;

            playerPositions[currentPlayer] = targetPosition;

            if (playerPositions[currentPlayer] >= boardSize) {
                cout << "-- GAME OVER --" << endl;
                cout << "Player " << currentPlayer + 1 << " is the winner!!!" << endl;
                break;
            }

            currentPlayer = (currentPlayer + 1) % numPlayers;
        }

        if (turnCount >= maxTurns) {
            cout << "-- GAME OVER --" << endl;
            cout << "Maximum turns reached." << endl;
        }

        delete[] playerPositions;
    }
};

class AutomaticGame : public Game {
public:
    AutomaticGame(int numPlayers, int maxTurns) : Game(numPlayers, maxTurns) {}

    void play(Tile** board, int boardSize) override {
        cout << "Automatic game mode is not yet implemented." << endl;
    }
};

int main(int argc, char* argv[]) {
    int numTiles = 30;
    int numSnakes = 3;
    int numLadders = 3;
    int snakePenalty = 3;
    int ladderReward = 3;
    int numPlayers = 2;
    int maxTurns = 100;
    char gameType = 'M';

    if (argc > 1) {
        numTiles = atoi(argv[1]);
        numSnakes = atoi(argv[2]);
        numLadders = atoi(argv[3]);
        snakePenalty = atoi(argv[4]);
        ladderReward = atoi(argv[5]);
        numPlayers = atoi(argv[6]);
        maxTurns = atoi(argv[7]);
        gameType = argv[8][0];
    }

    Tile** board = new Tile*[numTiles];
    for (int i = 0; i < numTiles; ++i) {
        board[i] = new NormalTile();
    }

    srand(time(0));
    for (int i = 0; i < numSnakes; ++i) {
        int snakePos;
        do {
            snakePos = rand() % numTiles;
        } while (dynamic_cast<SnakeTile*>(board[snakePos]) != nullptr ||
                 dynamic_cast<LadderTile*>(board[snakePos]) != nullptr);
        delete board[snakePos];
        board[snakePos] = new SnakeTile(snakePenalty);
    }

    for (int i = 0; i < numLadders; ++i) {
        int ladderPos;
        do {
            ladderPos = rand() % numTiles;
        } while (dynamic_cast<SnakeTile*>(board[ladderPos]) != nullptr ||
                 dynamic_cast<LadderTile*>(board[ladderPos]) != nullptr);
        delete board[ladderPos];
        board[ladderPos] = new LadderTile(ladderReward);
    }

    Game* game;
    if (gameType == 'A') {
        game = new AutomaticGame(numPlayers, maxTurns);
    } else {
        game = new ManualGame(numPlayers, maxTurns);
    }

    game->play(board, numTiles);

    for (int i = 0; i < numTiles; ++i) {
        delete board[i];
    }
    delete[] board;
    delete game;

    return 0;
}
