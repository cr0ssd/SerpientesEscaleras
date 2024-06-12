#include <iostream>
#include <ctime>
#include <cstdlib>
#include <string>

using namespace std;

class InvalidConfigurationException : public exception {
private:
    string message;

public:
    InvalidConfigurationException(const string& paramName, int paramValue) {
        message = "Invalid " + paramName + " value " + to_string(paramValue);
    }

    const char* what() const noexcept override {
        return message.c_str();
    }
};

class InvalidOptionException : public exception {
public:
    const char* what() const noexcept override {
        return "Invalid option, please press C to continue next turn or E to end the game";
    }
};

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
};    

class Game {
protected:
    int numPlayers;
    int maxTurns;

public:
    Game(int numPlayers, int maxTurns) : numPlayers(numPlayers), maxTurns(maxTurns) {
        if (numPlayers <= 0) {
            throw InvalidConfigurationException("players", numPlayers);
        }
        if (maxTurns <= 0) {
            throw InvalidConfigurationException("turns", maxTurns);
        }
    }

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
        int invalidOptionCount = 0;

        while (turnCount < maxTurns) {
            cout << "Press C to continue next turn, or E to end the game: ";
            
            try {
                cin >> input;
                if (input != 'C' && input != 'E') {
                    throw InvalidOptionException();
                }

                if (input == 'E') {
                    break; 
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

            } catch (const InvalidOptionException& e) {
                cerr << e.what() << endl;
                invalidOptionCount++;

                if (invalidOptionCount >= 5) {
                    cout << "-- GAME OVER --" << endl;
                    cout << "Invalid menu choice exceeded" << endl;
                    break;
                }
            }
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
        try {
            numTiles = stoi(argv[1]);
            if (numTiles <= 1) { 
                throw InvalidConfigurationException("tiles", numTiles);
            }
            numSnakes = stoi(argv[2]);
            numLadders = stoi(argv[3]);
            snakePenalty = stoi(argv[4]);
            ladderReward = stoi(argv[5]);
            numPlayers = stoi(argv[6]);
            maxTurns = stoi(argv[7]);
            gameType = argv[8][0];
        } catch (const InvalidConfigurationException& e) {
            cerr << e.what() << endl;
            return 1; 
        } catch (const std::exception& e) {
            cerr << "Invalid arguments provided." << endl; 
            return 1; 
        }
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

    try {
        if (gameType == 'A') {
            game = new AutomaticGame(numPlayers, maxTurns);
        } else {
            game = new ManualGame(numPlayers, maxTurns);
        }

        game->play(board, numTiles);
    } catch (const InvalidConfigurationException& e) { 
        cerr << e.what() << endl;
        return 1; 
    } 

    for (int i = 0; i < numTiles; ++i) {
        delete board[i];
    }
    delete[] board;
    delete game;

    return 0; 
}