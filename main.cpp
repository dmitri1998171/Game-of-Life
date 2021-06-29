#include <iostream>
#include <windows.h>

using namespace std;

const int galaxySize = 25;
char currGen[galaxySize][galaxySize] = {
    "                        ",
    "                        ",
    "                        ",
    "                        ",
    "                        ",
    "                        ",
    "                        ",
    "           *            ",
    "          **            ",
    "    ********            ",
    "       ********         ",
    "                        ",
    "                        ",
    "                        ",
    "                        ",
    "                        ",
    "                        ",
    "                        ",
    "                        ",
    "                        ",
    "                        ",
    "                        ",
    "                        ",
    "                        ",
    "                        ",
};

class CellClass {
    private: 
        char prevGen[galaxySize][galaxySize];
        int neighborsCount = 0;

    public:
        void nextGen() {
            for(int i = 0; i < galaxySize; i++) 
                for(int j = 0; j < galaxySize; j++) 
                    prevGen[i][j] = currGen[i][j];
        }

        void drawCells() {
            for(int i = 0; i < galaxySize; i++) {
                for(int j = 0; j < galaxySize; j++) {
                    cout << currGen[i][j];
                }
                cout << endl;
            }
            cout << endl;
            Sleep(1000);
        }

        void changeCellState() {
            for(int i = 0; i < galaxySize; i++) {
                for(int j = 0; j < galaxySize; j++) {
                    if(prevGen[i][j] == '*') {
                        if(prevGen[i][j - 1] == '*') neighborsCount++; 
                        if(prevGen[i][j + 1] == '*') neighborsCount++;
                        if(prevGen[i + 1][j] == '*') neighborsCount++;
                        if(prevGen[i - 1][j] == '*') neighborsCount++;
                        if(prevGen[i + 1][j + 1] == '*') neighborsCount++;
                        if(prevGen[i + 1][j - 1] == '*') neighborsCount++;
                        if(prevGen[i - 1][j + 1] == '*') neighborsCount++;
                        if(prevGen[i - 1][j - 1] == '*') neighborsCount++;

                        if(neighborsCount < 2 || neighborsCount > 3) 
                            currGen[i][j] = ' ';
                        
                        neighborsCount = 0;
                    }
                    if(prevGen[i][j] == ' ') {
                        if(prevGen[i][j - 1] == '*') neighborsCount++; 
                        if(prevGen[i][j + 1] == '*') neighborsCount++;
                        if(prevGen[i + 1][j] == '*') neighborsCount++;
                        if(prevGen[i - 1][j] == '*') neighborsCount++;
                        if(prevGen[i + 1][j + 1] == '*') neighborsCount++;
                        if(prevGen[i + 1][j - 1] == '*') neighborsCount++;
                        if(prevGen[i - 1][j + 1] == '*') neighborsCount++;
                        if(prevGen[i - 1][j - 1] == '*') neighborsCount++;

                        if(neighborsCount == 3) 
                            currGen[i][j] = '*';

                        neighborsCount = 0;
                    }
                }
            }
        }

        int checkEndGame() {
            int endGameState = 0;
            int checker = 0;
            
            for(int i = 0; i < galaxySize; i++) {
                for(int j = 0; j < galaxySize; j++) {
                    if(currGen[i][j] = prevGen[i][j]) {
                        checker++;
                    }
                }
            }

            if(checker == galaxySize * galaxySize)
                endGameState = 0;
            else
                endGameState = 1;

            return endGameState;
        }

        void run() {
            bool cycle = true;

            while(cycle) {
                nextGen();
                changeCellState();
                drawCells();

                // if(checkEndGame() == 1) {
                //     cout << "\n\n\t\t\t\t\tcheckEndGame()\n\n";
                //     cycle = false;
                // }
            }
        }
};

int main(int argc, char* argv[]) {
    CellClass cell;
    cell.run();

    getchar();
    return 0;
}