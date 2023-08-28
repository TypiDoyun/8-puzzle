#include <iostream>
#include <vector>
#include <array>
#include <queue>

using namespace std;

const int SIZE = 3;

typedef array<array<int, SIZE>, SIZE> Board;
typedef array<int, 2> Location;

Board goal = {
    1, 2, 3,
    4, 5, 6,
    7, 8, -1
};

Board board = {
    1, 2, 3,
    -1, 4, 6,
    7, 5, 8
};

queue<int> gQueue;
queue<Board> opened;
vector<Board> storage;
vector<Board> closed;

Location getLocation(int);
void open(Board);
void close(Board);
Board& getOpenedBoard();
bool doOpenedBoardExist();
bool isNotClosed(Board);
int getF(Board&, int);
int getG();
bool equals(Board&, Board&);
Location getEmptyCell(Board);
Board& swap(Board&, Location, Location);
Board& cloneAndSwap(Board, Location, Location);

int main(void) {
    open(board);
    gQueue.push(1);

    while (doOpenedBoardExist()) {
        Board currentBoard = getOpenedBoard();
        Location emptyCell = getEmptyCell(currentBoard);

        if (equals(currentBoard, goal)) {
            for (auto line : currentBoard) {
                for (int item : line) {
                    printf("%d ", item);
                }
                printf("\n");
            }
            break;
        }
        if (!isNotClosed(currentBoard)) continue;

        close(currentBoard);

        int g = getG();

        for (int y = -1; y <= 1; y++) {
            for (int x = -1; x <= 1; x++) {
                int manhattan = abs(x) + abs(y);

                if (manhattan > 1 || manhattan == 0) continue;

                int calcX = emptyCell[0] + x;
                int calcY = emptyCell[1] + y;

                if (calcX < 0 || calcY < 0 || calcX >= SIZE || calcY >= SIZE) continue;

                Board& swappedBoard = cloneAndSwap(currentBoard, emptyCell, { calcX, calcY });
                storage.push_back(swappedBoard);

                // for (auto line : swappedBoard) {
                //     for (int item : line) {
                //         printf("%d ", item);
                //     }
                //     printf("\n");
                // }
            }
        }

        sort(storage.begin(), storage.end(), [&](Board& a, Board& b) {
            return getF(a, g) < getF(b, g);
        });

        // gQueue.push(g + 1);
        // opened.push(storage[0]);

        for (Board& board : storage) {
            gQueue.push(g + 1);
            opened.push(board);
        }

        storage.clear();
    }

    printf("end");
}

Location getLocation(int value) {
    if (value == -1) return { 2, 2 };
    else return { (value - 1) % 3, (value - 1) / 3 };
}

void open(Board _board) {
    opened.push(_board);
}

void close(Board board) {
    closed.push_back(board);
}

Board& getOpenedBoard() {
    if (opened.size() == 0) throw "opened";
    Board& board = opened.front();
    opened.pop();
    return board;
}

bool doOpenedBoardExist() {
    return !opened.empty();
}

bool isNotClosed(Board board) {
    bool value = true;

    for (Board item : closed) {
        bool isSame = true;
        for (int y = 0; y < SIZE; y++) {
            for (int x = 0; x < SIZE; x++) {
                if (item[y][x] != board[y][x]) isSame = false;
            }
        }

        if (!isSame) continue;

        value = false;
        break;
    }

    return value;
}

int getF(Board& board, int g) {
    int h = 0;

    for (int y = 0; y < SIZE; y++) {
        for (int x = 0; x < SIZE; x++) {
            if (board[y][x] == goal[y][x]) continue;

            Location location = getLocation(board[y][x]);

            int manhattan = abs(location[0] - x) + abs(location[1] - y);
            h += manhattan;

        }
    }

    return h + g;
}

int getG() {
    if (gQueue.size() == 0) throw "gQueue";
    int g = gQueue.front();
    gQueue.pop();
    return g;
}

bool equals(Board& board1, Board& board2) {
    bool result = true;
    for (int y = 0; y < SIZE; y++) {
        for (int x = 0; x < SIZE; x++) {
            if (board1[y][x] == board2[y][x]) continue;

            result = false;
            break;
        }
    }
    return result;
}

Location getEmptyCell(Board board) {
    Location result = { 0, 0 };
    for (int y = 0; y < SIZE; y++) {
        for (int x = 0; x < SIZE; x++) {
            if (board[y][x] != -1) continue;

            result[0] = x;
            result[1] = y;
        }
    }

    return result;
}

Board& swap(Board& board, Location from, Location to) {
    int temp = board[from[1]][from[0]];
    board[from[1]][from[0]] = board[to[1]][to[0]];
    board[to[1]][to[0]] = temp;

    return board;
}

Board& cloneAndSwap(Board board, Location from, Location to) {
    return swap(board, from, to);
}