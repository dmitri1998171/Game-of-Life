#include <iostream>
#include <windows.h>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

#define WINDOW_SIZE 402

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
        int playState = 0;
        static const int gridStep = 25;
        char prevGen[gridStep][gridStep];
        char currGen[gridStep][gridStep];
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
                if(!playState) {
                    Vector2i mousePos = Mouse::getPosition(*window);

                    for(int i = 0; i < gridStep; i++) 
                        for(int j = 0; j < gridStep; j++) 
                            if(cell[i][j].getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                                cout << "click on cell[" << i << "][" << j << "]" << endl;
                                currGen[i][j] = '*';
                                cell[i][j].setFillColor(liveCellColor);
                            }
                }
            }
        }

    // ==== Logic methods ============================

        void cleanCurrGen() {
            for(int i = 0; i < gridStep; i++) 
                for(int j = 0; j < gridStep; j++)
                    currGen[i][j] = ' ';
        }

        void nextGen() {
            int counter = 0;

            for(int i = 0; i < gridStep; i++) {
                for(int j = 0; j < gridStep; j++) 
                    prevGen[i][j] = currGen[i][j];
             
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
            int counter = 0;
            
            for(int i = 0; i < gridStep; i++) {
                for(int j = 0; j < gridStep; j++) {
<<<<<<< HEAD
                    // if(strcmp(currGen[i], prevGen[i])) {
                    //     counter++;
                    // }
                    if(currGen[i][j] == prevGen[i][j]) 
                        if(prevGen[i][j] == preprevGen[i][j])
                            playState = FAIL_STATE;
=======
                    if(strcmp(currGen[i], prevGen[i])) {
                        counter++;
                    }
>>>>>>> parent of 92f66a5 (add game states, restart game)
                }
            }

            if(counter == gridStep) {
                cout << "\ncheckEndGame()\n\n";
                playState = 0;
            }
        }

        void run(RenderWindow* window) {
            bool cycle = true;
            int pauseCount = 0;

            Font font;
            Text text;
            String str = "You win!";
            font.loadFromFile("./CyrilicOld.TTF");

            text.setFont(font);
            text.setFillColor(Color::Green);
            text.setString(str);
            // text.setPosition((WINDOW_SIZE - str.getSize()) / 2, WINDOW_SIZE / 2);

            while(cycle) {
                Event event;
                while (window->pollEvent(event)) {
                    if (event.type == Event::Closed || Keyboard::isKeyPressed(Keyboard::Escape)) {
                        window->close();
                        exit(0);
                    }

                    if (event.type == sf::Event::KeyPressed) {
                        if (event.key.code == sf::Keyboard::Enter) playState = 1;
                        if (event.key.code == sf::Keyboard::Space) playState = 0;
                    }
                }

                window->clear();
                
<<<<<<< HEAD
                switch(playState) {
                    case START_STATE:
                        str.clear();
                        str.insert(0, "You win!");

                        if(startCount == 0) {
                            cleanCurrGen();
                            startCount++;
                        }

                        drawFirstGen(window);
                        break;

                    case PLAY_STATE:
                        nextGen();
                        changeCellState();
                        Sleep(750);
                        break;

                    case WIN_STATE:
                        window->draw(text);
                        break;

                    case FAIL_STATE:
                        str.clear();
                        str.insert(0, "Game over!");

                        window->draw(text);
                        break;
=======
                drawFirstGen(window);

                if(playState) {
                    nextGen();
                    changeCellState();
>>>>>>> parent of 92f66a5 (add game states, restart game)
                }

                // checkEndGame();
<<<<<<< HEAD
=======
                    
                window->clear();
>>>>>>> parent of 92f66a5 (add game states, restart game)

                for(int i = 0; i < gridStep; i++) 
                    for(int j = 0; j < gridStep; j++) 
                        window->draw(cell[i][j]);
<<<<<<< HEAD
                
=======

                // window->draw(text);
>>>>>>> parent of 92f66a5 (add game states, restart game)
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