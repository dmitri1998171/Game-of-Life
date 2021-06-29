#include <iostream>
#include <windows.h>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

#define WINDOW_SIZE 402

using namespace std;
using namespace sf;

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
    RenderWindow window(VideoMode(WINDOW_SIZE, WINDOW_SIZE), "Game of Life");

    const int gridStep = 10;
    int rectSize = WINDOW_SIZE / gridStep;
    RectangleShape cell[gridStep][gridStep];
    
    for(int i = 0; i < gridStep; i++) {
        for(int j = 0; j < gridStep; j++) {
            cell[i][j].setSize(Vector2f(rectSize, rectSize));
            cell[i][j].setFillColor(Color::White);
            cell[i][j].setOutlineThickness(1);
            cell[i][j].setOutlineColor(Color::Red);
            cell[i][j].setPosition((i * rectSize) + 1, (j * rectSize) + 1);
        }
    }

    while (window.isOpen()) {
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed || Keyboard::isKeyPressed(Keyboard::Escape)) {
                window.close();
                return 0;
            }
        }

        window.clear();

        for(int i = 0; i < gridStep; i++) 
            for(int j = 0; j < gridStep; j++) 
                window.draw(cell[i][j]);

        window.display();
    }
    
    // CellClass cell;
    // cell.run();

    getchar();
    return 0;
}