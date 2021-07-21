#include <iostream>
#include <fstream>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

#define WINDOW_SIZE 800

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
        static const int rectSize = 16;
        static const int gridStep = WINDOW_SIZE / rectSize;
        char prevGen[gridStep][gridStep];
        char currGen[gridStep][gridStep];
        char preprevGen[gridStep][gridStep];
        RectangleShape cell[gridStep][gridStep];
        Color liveCellColor = Color::Red;
        Color deathCellColor = Color::White;
        
    public:
        Clock clock;
        Time elapsed = clock.getElapsedTime();

    // ==== SFML methods =============================
       
        void CreateGrid() {
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

        void addText(Text *text, Font *font, string *str, Color color, int fontSize) {
            text->setFont(*font);
            text->setFillColor(color);
            text->setCharacterSize(fontSize);
            text->setString(*str);
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
            for(int i = 0; i < gridStep; i++) {
                for(int j = 0; j < gridStep; j++) {
                    preprevGen[i][j] = prevGen[i][j];
                    prevGen[i][j] = currGen[i][j];
                }
            }
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

    // ==== Other metods =============================

        int timer(float seconds) {
            Time elapsed1;

            elapsed1 = clock.getElapsedTime();
            if(elapsed1.asSeconds() > seconds) 
                return 1;

            return 0;
        }

        void loadTextFromFile(char *path, string *array) { 
            string line;
 
            ifstream file(path);
            if (file.is_open()) {
                while (getline(file, line)) {
                    array->append(line);
                    array->append("\n");
                }
            }

            file.close(); 
        }

        void drawGrid(RenderWindow *window) {
            for(int i = 0; i < gridStep; i++) 
                for(int j = 0; j < gridStep; j++) 
                    window->draw(cell[i][j]);
        }

    // ===============================================

        void run(RenderWindow* window) {
            int startCount = 0;
            int fontSize = 24;
            int textColor = 75;
            int infoTextState = 1, rulesTextState = 0;
            string infoText, rulesText;
            Text info_text, rules_text;
            Font font;
            font.loadFromFile("./CyrilicOld.TTF");

            loadTextFromFile("info.txt", &infoText);
            loadTextFromFile("rules.txt", &rulesText);

            addText(&info_text, &font, &infoText, Color(textColor, textColor, textColor), fontSize);
            addText(&rules_text, &font, &rulesText, Color(textColor, textColor, textColor), fontSize * 0.75);

            while(true) {
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
                
                drawGrid(window);

                switch(playState) {
                    case START_STATE:
                        if(startCount == 0) {
                            cleanCurrGen();
                            startCount++;
                        }

                        drawFirstGen(window);
                        
                    case PAUSE_STATE:
                        if(infoTextState) {
                            window->draw(info_text);

                            if(timer(5))
                                infoTextState = 0;
                        }

                        if(rulesTextState) {
                            window->draw(rules_text);

                            if(timer(30))
                                rulesTextState = 0;
                        }
                        break;

                    case PLAY_STATE:
                        nextGen();
                        changeCellState();
                        Sleep(750);
                        break;
                }
                                
                window->display();
            }
        }
};

int main(int argc, char* argv[]) {
    RenderWindow window(VideoMode(WINDOW_SIZE, WINDOW_SIZE), "Game of Life");

    CellClass cell;
    cell.cleanCurrGen();
    cell.CreateGrid();
    cell.run(&window);
    
    getchar();
    return 0;
}