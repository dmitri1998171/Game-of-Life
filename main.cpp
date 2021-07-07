#include <iostream>
#include <windows.h>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

#define WINDOW_SIZE 402

enum GAME_STATES {
    START_STATE = 0,
    PAUSE_STATE = 1,
    PLAY_STATE,
    WIN_STATE,
    FAIL_STATE
};

using namespace std;
using namespace sf;

// char currGen[gridStep][gridStep] = {
//     "                        ",
//     "                        ",
//     "                        ",
//     "                        ",
//     "                        ",
//     "                        ",
//     "                        ",
//     "           *            ",
//     "          **            ",
//     "    ********            ",
//     "       ********         ",
//     "                        ",
//     "                        ",
//     "                        ",
//     "                        ",
//     "                        ",
//     "                        ",
//     "                        ",
//     "                        ",
//     "                        ",
//     "                        ",
//     "                        ",
//     "                        ",
//     "                        ",
//     "                        ",
// };

class CellClass {
    private: 
        int neighborsCount = 0;
        int playState = START_STATE;
        static const int gridStep = 25;
        char prevGen[gridStep][gridStep];
        char currGen[gridStep][gridStep];
        char preprevGen[gridStep][gridStep];
        int rectSize = WINDOW_SIZE / gridStep;
        RectangleShape cell[gridStep][gridStep];
        Color liveCellColor = Color::Red;
        Color deathCellColor = Color::White;
        
    public:
    // ==== SFML methods =============================
       
        void drawGrid() {
            for(int i = 0; i < gridStep; i++) {
                for(int j = 0; j < gridStep; j++) {
                    cell[i][j].setSize(Vector2f(rectSize, rectSize));
                    cell[i][j].setFillColor(deathCellColor);
                    cell[i][j].setOutlineThickness(1);
                    cell[i][j].setOutlineColor(Color::Black);
                    cell[i][j].setPosition((i * rectSize) + 1, (j * rectSize) + 1);
                }
            }
        }

        void drawFirstGen(RenderWindow* window) {
            if(Mouse::isButtonPressed(Mouse::Left)) {
                if(playState == START_STATE) {
                    Vector2i mousePos = Mouse::getPosition(*window);

                    for(int i = 0; i < gridStep; i++) 
                        for(int j = 0; j < gridStep; j++) 
                            if(cell[i][j].getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                                currGen[i][j] = '*';
                                cell[i][j].setFillColor(liveCellColor);
                            }
                }
            }
        }

    // ==== Logic methods ============================

        void cleanCurrGen() {
            for(int i = 0; i < gridStep; i++) 
                for(int j = 0; j < gridStep; j++) {
                    currGen[i][j] = ' ';
                    cell[i][j].setFillColor(Color::White);
                }
        }

        void nextGen() {
            int counter = 0;

            for(int i = 0; i < gridStep; i++) {
                for(int j = 0; j < gridStep; j++) {
                    preprevGen[i][j] = prevGen[i][j];
                    prevGen[i][j] = currGen[i][j];
                }
             
                // if(strcmp(currGen[i], prevGen[i])) 
                //     counter++;
            }

            // if(counter == gridStep) {
            //     cout << "\ncheckEndGame()\n\n";
            //     playState = 0;
            // }
        }

        void changeCellState() {
            for(int i = 0; i < gridStep; i++) {
                for(int j = 0; j < gridStep; j++) {
                    if(prevGen[i][j] == '*') {
                        if(prevGen[i][j - 1] == '*') neighborsCount++; 
                        if(prevGen[i][j + 1] == '*') neighborsCount++;
                        if(prevGen[i + 1][j] == '*') neighborsCount++;
                        if(prevGen[i - 1][j] == '*') neighborsCount++;
                        if(prevGen[i + 1][j + 1] == '*') neighborsCount++;
                        if(prevGen[i + 1][j - 1] == '*') neighborsCount++;
                        if(prevGen[i - 1][j + 1] == '*') neighborsCount++;
                        if(prevGen[i - 1][j - 1] == '*') neighborsCount++;

                        if(neighborsCount < 2 || neighborsCount > 3) {
                            currGen[i][j] = ' ';
                            cell[i][j].setFillColor(deathCellColor);
                        }
                        
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

                        if(neighborsCount == 3) {
                            currGen[i][j] = '*';
                            cell[i][j].setFillColor(liveCellColor);
                        }

                        neighborsCount = 0;
                    }
                }
            }
        }

        void checkEndGame() {
            // int counter = 0;
            
            for(int i = 0; i < gridStep; i++) {
                for(int j = 0; j < gridStep; j++) {
                    // if(strcmp(currGen[i], prevGen[i])) {
                    //     counter++;
                    // }
                    if(prevGen[i][j] == currGen[i][j]) 
                        if(prevGen[i][j] == preprevGen[i][j])
                            playState = FAIL_STATE;
                }
            }

            // if(counter == gridStep) {
            //     cout << "\ncheckEndGame()\n\n";
            //     playState = 0;
            // }
        }

        void run(RenderWindow* window) {
            bool cycle = true;
            int startCount = 0;

            Font font;
            Text text;
            int fontSize = 24;
            String str = "You win!";
            font.loadFromFile("./CyrilicOld.TTF");

            text.setFont(font);
            text.setFillColor(Color::Green);
            text.setString(str);
            text.setCharacterSize(fontSize);
            text.setPosition((WINDOW_SIZE - (str.getSize() * (fontSize / 2))) / 2, (WINDOW_SIZE - (fontSize * 2)) / 2);

            while(cycle) {
                Event event;
                while (window->pollEvent(event)) {
                    if (event.type == Event::Closed || Keyboard::isKeyPressed(Keyboard::Escape)) {
                        window->close();
                        exit(0);
                    }

                    if (event.type == sf::Event::KeyPressed) {
                        if (event.key.code == sf::Keyboard::C) {
                            if(playState == PLAY_STATE || playState == PAUSE_STATE) {
                                playState = START_STATE;
                                startCount = 0;
                            }
                        }
                        if (event.key.code == sf::Keyboard::Enter) playState = PLAY_STATE;
                        if (event.key.code == sf::Keyboard::Space) playState = PAUSE_STATE;
                    }
                }
                
                if(playState == START_STATE) {
                    str.clear();
                    str.insert(0, "You win!");

                    if(startCount == 0) {
                        cleanCurrGen();
                        startCount++;
                    }

                    drawFirstGen(window);
                }

                if(playState == PLAY_STATE) {
                    nextGen();
                    changeCellState();
                    Sleep(750);
                }
                    
                // checkEndGame();
                
                window->clear();

                for(int i = 0; i < gridStep; i++) 
                    for(int j = 0; j < gridStep; j++) 
                        window->draw(cell[i][j]);

                if(playState == WIN_STATE) {
                    window->draw(text);
                }

                if(playState == FAIL_STATE) {
                    // text.setString("Game over!");
                    str.clear();
                    str.insert(0, "Game over!");

                    window->draw(text);
                }
                
                window->display();
            }
        }
};

int main(int argc, char* argv[]) {
    RenderWindow window(VideoMode(WINDOW_SIZE, WINDOW_SIZE), "Game of Life");

    CellClass cell;
    cell.cleanCurrGen();
    cell.drawGrid();
    cell.run(&window);
    
    getchar();
    return 0;
}