#include <iostream>
#include <stdio.h>
#include <stdlib.h>
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
        Clock clock;
        Time elapsed = clock.getElapsedTime();

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
                    cell[i][j].setFillColor(deathCellColor);
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
                    if(currGen[i][j] == prevGen[i][j]) 
                        if(prevGen[i][j] == preprevGen[i][j])
                            playState = FAIL_STATE;
                }
            }

            // if(counter == gridStep) {
            //     cout << "\ncheckEndGame()\n\n";
            //     playState = 0;
            // }
        }

    // ===============================================

        int timer(int seconds) {
            Time elapsed1;

            elapsed1 = clock.getElapsedTime();
            if(elapsed1.asSeconds() > 10.0) 
                return 1;

            return 0;
        }

        void loadTextFromFile(char *path, char array[]) { 
            char tmp[255];
            FILE *file = fopen(path, "r");

            while (fgets(tmp, 255, file) != NULL)
                strcat(array, tmp);
                // array.insert(array.length(), tmp);
                // array.push_back(*tmp);

            fclose(file);
        }

        // void loadTextFromFile(char *path, string array) { 
        //     char tmp[255];
        //     FILE *file = fopen(path, "r");
        //     while (fgets(tmp, 255, file) != NULL)
        //         // strcat(array, tmp);
        //         // array.insert(array.length(), tmp);
        //         // array.push_back(*tmp);
        //     fclose(file);
        // }

        void run(RenderWindow* window) {
            bool cycle = true;
            int startCount = 0;
            int infoTextState = 1, rulesTextState = 0;
            int fontSize = 24;
            // string rulesText;
            // string infoText;
            char infoText[55];
            char rulesText[2715];

            Font font;
            Text text, info_text, rules_text;
            String str = "You win!";
            font.loadFromFile("./CyrilicOld.TTF");

            text.setFont(font);
            text.setFillColor(Color::Green);
            text.setString(str);
            text.setCharacterSize(fontSize);
            text.setPosition((WINDOW_SIZE - (str.getSize() * (fontSize / 2))) / 2, (WINDOW_SIZE - (fontSize * 2)) / 2);

            // setlocale(LC_ALL, "Rus");
            loadTextFromFile("info.txt", infoText);
            loadTextFromFile("rules.txt", rulesText);

            int tc = 75;
            info_text.setFont(font);
            info_text.setFillColor(Color(tc, tc, tc));
            info_text.setString(infoText);

            rules_text.setFont(font);
            rules_text.setFillColor(Color(tc, tc, tc));
            // rules_text.setString(String::fromUtf8(rulesText[0], rulesText[strlen(rulesText)]));
            rules_text.setString(rulesText);

            while(cycle) {
                Event event;
                while (window->pollEvent(event)) {
                    if (event.type == Event::Closed || Keyboard::isKeyPressed(Keyboard::Escape)) {
                        window->close();
                        exit(0);
                    }

                    if (event.type == sf::Event::KeyPressed) {
                        if (event.key.code == sf::Keyboard::I) {
                            infoTextState = 1;
                        }
                        if (event.key.code == sf::Keyboard::H) {
                            rulesTextState = 1;
                        }
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

                window->clear();
                
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
                }
                    
                // checkEndGame();

                for(int i = 0; i < gridStep; i++) 
                    for(int j = 0; j < gridStep; j++) 
                        window->draw(cell[i][j]);

                if(infoTextState) {
                    window->draw(info_text);

                    if(timer(5))
                        infoTextState = 0;
                }

                if(rulesTextState) {
                    window->draw(rules_text);

                    if(timer(10))
                        rulesTextState = 0;
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