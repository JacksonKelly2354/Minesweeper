#include <iostream>
#include <fstream>
#include <SFML/Graphics.hpp>
#include "Board.h"
using namespace std;

int main() {
    string temp;
    float width;
    float height;
    float numMines;

    fstream cFile ("files/board_config.cfg", ios_base::in);
    getline(cFile, temp);
    width = stoi(temp) * 32;
    getline(cFile, temp);
    height = (stoi(temp) * 32) + 100;
    getline(cFile, temp);
    numMines = stoi(temp);

    sf::RenderWindow window(sf::VideoMode(width, height), "Minesweeper", sf::Style::Close);

    sf::Text text;
    sf::Font font;
    font.loadFromFile("files/font.ttf");
    text.setFont(font);
    text.setString("WELCOME TO MINESWEEPER!");
    text.setCharacterSize(24);
    text.setStyle(sf::Text::Underlined | sf::Text::Bold);

    sf::FloatRect textRect = text.getLocalBounds();
    text.setOrigin(textRect.left + textRect.width/2.0f, textRect.top + textRect.height/2.0f);
    text.setPosition(sf::Vector2f(width/2.0f, (height/2.0f)-150));

    sf::Text text2;
    text2.setFont(font);
    text2.setString("Enter your name:");
    text2.setCharacterSize(20);
    text2.setStyle(sf::Text::Bold);

    sf::FloatRect textRect2 = text2.getLocalBounds();
    text2.setOrigin(textRect2.left + textRect2.width/2.0f, textRect2.top + textRect2.height/2.0f);
    text2.setPosition(sf::Vector2f(width/2.0f, (height/2.0f)-75));

    sf::String playerInput;
    sf::Text playerName;
    int lenName = 0;

    playerName.setFont(font);
    playerName.setString("|");
    playerName.setCharacterSize(18);
    playerName.setStyle(sf::Text::Bold);
    playerName.setFillColor(sf::Color::Yellow);

    sf::FloatRect nameRect = playerName.getLocalBounds();
    playerName.setOrigin(nameRect.left + nameRect.width/2.0f, nameRect.top + nameRect.height/2.0f);
    playerName.setPosition(sf::Vector2f(width/2.0f, (height/2.0f)-45));

    while (window.isOpen()) {
        sf::Event event;
        while(window.pollEvent(event)){
            if (event.type == sf::Event::Closed){
                window.close();
            } else if (event.type == sf::Event::TextEntered){
                if(std::isalpha(event.text.unicode)){
                    if(lenName == 0){
                        playerInput +=((char)std::toupper(event.text.unicode));
                        playerInput +="|";
                        playerName.setString(playerInput);
                        playerInput.erase(playerInput.getSize() - 1, 1);

                        nameRect = playerName.getLocalBounds();
                        playerName.setOrigin(nameRect.left + nameRect.width/2.0f, nameRect.top + nameRect.height/2.0f);
                        playerName.setPosition(sf::Vector2f(width/2.0f, (height/2.0f)-45));
                        lenName += 1;
                    } else if (lenName < 10){
                        playerInput +=((char)std::tolower(event.text.unicode));
                        playerInput +="|";
                        playerName.setString(playerInput);
                        playerInput.erase(playerInput.getSize() - 1, 1);
                        nameRect = playerName.getLocalBounds();
                        playerName.setOrigin(nameRect.left + nameRect.width/2.0f, nameRect.top + nameRect.height/2.0f);
                        playerName.setPosition(sf::Vector2f(width/2.0f, (height/2.0f)-45));
                        lenName += 1;
                    }
                }
            } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter)){
                if(lenName != 0){
                    window.close();
                    sf::RenderWindow gameWindow(sf::VideoMode(width, height), "Minesweeper", sf::Style::Close);
                    gameWindow.setFramerateLimit(30);
                    int numCol = width/32;
                    int numRow = height/32-3;
                    Texture smile = Texture("files/images/face_happy.png", ((numCol / 2.0) * 32)-32, 32 * (numRow+0.5f));
                    Board board = Board("files/images/tile_hidden.png", width, height);
                    Board tempBoard = Board("files/images/tile_revealed.png", width, height);
                    for (int i = 0; i < numMines; i++){
                        board.addMine();
                    }
                    board.addNums();
                    Texture debugButton = Texture("files/images/debug.png", width-304, 32 * (numRow + 0.5f));
                    Texture pauseButton = Texture("files/images/pause.png", width - 240, 32 * (numRow+0.5f));
                    Texture leaderButton = Texture("files/images/leaderboard.png", width - 176, 32 * (numRow+0.5f));
                    sf::Clock clock;
                    sf::Time elapsed;
                    int tempSeconds = 0;
                    while (gameWindow.isOpen()) {
                        if(board.leaderboardOpening){
                            tempSeconds = board.boardSec;
                            board.leaderboardOpen = true;
                            sf::RenderWindow finalWindow(sf::VideoMode(width/2, height/2), "Minesweeper", sf::Style::Close);
                            leaderboard _leaderboard = leaderboard(width, height, 0);
                            while (finalWindow.isOpen()) {
                                while (finalWindow.pollEvent(event)) {
                                    if (event.type == sf::Event::Closed) {
                                        finalWindow.close();
                                    }
                                }
                                finalWindow.clear(sf::Color::Blue);
                                finalWindow.draw(_leaderboard.title);
                                finalWindow.draw(_leaderboard.ranks);
                                finalWindow.display();
                            }
                            clock.restart();
                            board.leaderboardOpening = false;
                        }
                        while (gameWindow.pollEvent(event)) {
                            if (event.type == sf::Event::Closed) {
                                board.gameOver = true;
                                gameWindow.close();
                            } else if (event.type == sf::Event::MouseButtonPressed){
                                int currentSize = board.flags.size();
                                auto mouse_pos = sf::Mouse::getPosition(gameWindow);
                                auto final_pos = gameWindow.mapPixelToCoords(mouse_pos);
                                if(sf::Mouse::isButtonPressed(sf::Mouse::Button::Right) and !board.gameOver and !board.paused){
                                    for(int i = 0; i < (width/32); i++){
                                        for(int j = 0; j < ((height/32)-4); j++){
                                            if (! board.tiles.at(i).at(j).revealed){
                                                if (board.tiles.at(i).at(j).sprite.getGlobalBounds().contains(final_pos)){
                                                    for(int k = 0; k < board.flags.size(); k++){
                                                        if (board.flags.at(k)->sprite.getGlobalBounds().contains(final_pos)){
                                                            board.flags.erase(board.flags.begin() + k);
                                                            board.tiles.at(i).at(j).flagged = false;
                                                        }
                                                    }
                                                    if (currentSize == board.flags.size()){
                                                        board.flags.push_back(board.overlayTexture("files/images/flag.png", i, j));
                                                        board.tiles.at(i).at(j).flagged = true;
                                                    }
                                                }
                                            }
                                        }
                                    }
                                } else if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)){
                                    for(int i = 0; i < (width/32); i++){
                                        for(int j = 0; j < ((height/32)-4); j++) {
                                            if (! board.tiles.at(i).at(j).flagged){
                                                if (board.tiles.at(i).at(j).sprite.getGlobalBounds().contains(final_pos) and !board.gameOver and !board.paused) {
                                                    board.updateTexture("files/images/tile_revealed.png", i, j);
                                                    board.tiles.at(i).at(j).revealed = true;
                                                    if (board.tiles.at(i).at(j).mine){
                                                        board.revealMines();
                                                        board.gameOver = true;
                                                    } else {
                                                        if (board.tiles.at(i).at(j).num != 0){
                                                            string _filename = board.numOfSprite(board.tiles.at(i).at(j).num);
                                                            Texture* tempTexture = board.overlayTexture(_filename, i, j);
                                                            board.nums.push_back(tempTexture);
                                                        } else {
                                                            board.revealAdjacent(i, j);
                                                        }
                                                    }
                                                }
                                            }
                                        }
                                    }
                                    if (debugButton.sprite.getGlobalBounds().contains(final_pos)){
                                        if (! board.debugging and !board.paused and !board.leaderboardOpen){
                                            board.debugging = true;
                                            for(int i = 0; i < (width/32); i++) {
                                                for (int j = 0; j < ((height / 32) - 4); j++) {
                                                    if (board.tiles.at(i).at(j).mine){
                                                        board.debugMines.push_back(board.overlayTexture("files/images/mine.png", i, j));
                                                    }
                                                }
                                            }
                                        } else {
                                            board.debugging = false;
                                        }
                                    }
                                    if (smile.sprite.getGlobalBounds().contains(final_pos)){
                                        Board newBoard = Board("files/images/tile_hidden.png", width, height);
                                        board = newBoard;
                                        for (int i = 0; i < numMines; i++){
                                            board.addMine();
                                        }
                                        board.addNums();
                                        tempSeconds = 0;
                                        clock.restart();
                                    }
                                    if (pauseButton.sprite.getGlobalBounds().contains(final_pos)){
                                        if(!board.gameOver){
                                            if (!board.paused){
                                                board.paused = true;
                                                board.firstRun = true;
                                            } else {
                                                board.paused = false;
                                            }
                                        }
                                    }
                                    if (leaderButton.sprite.getGlobalBounds().contains(final_pos)){
                                        board.leaderboardOpening = true;
                                    }
                                }
                            }
                        }

                        gameWindow.clear(sf::Color::White);
                        for(int i = 0; i < (width/32); i++){
                            for(int j = 0; j < ((height/32)-4); j++){
                                if(!board.paused and !board.leaderboardOpening){
                                    gameWindow.draw(board.tiles.at(i).at(j).sprite);
                                } else {
                                    gameWindow.draw(tempBoard.tiles.at(i).at(j).sprite);
                                }
                            }
                        }
                        for(int i = 0; i < board.flags.size(); i++){
                            if(!board.paused){
                                gameWindow.draw(board.flags.at(i)->sprite);
                            }
                        }

                        for(int i = 0; i < board.mines.size(); i++) {
                            gameWindow.draw(board.mines.at(i)->sprite);
                        }

                        if (board.debugging){
                            for(int i = 0; i < board.debugMines.size(); i++){
                                if(!board.paused and !board.leaderboardOpening){
                                    gameWindow.draw(board.debugMines.at(i)->sprite);
                                }
                            }
                        }

                        for(int i = 0; i < board.nums.size(); i++){
                            if(!board.paused and !board.leaderboardOpening){
                                gameWindow.draw(board.nums.at(i)->sprite);
                            }
                        }
                        gameWindow.draw(debugButton.sprite);
                        gameWindow.draw(leaderButton.sprite);

                        if(!board.paused){
                            gameWindow.draw(pauseButton.sprite);
                        } else {
                            Texture playButton = Texture("files/images/play.png", width-240, 32 * (numRow+0.5f));
                            gameWindow.draw(playButton.sprite);
                        }

                        if (board.gameOver){
                            if(!board.gameWon){
                                Texture faceLose = Texture("files/images/face_lose.png", ((numCol / 2.0) * 32)-32, 32 * (numRow+0.5f));
                                gameWindow.draw(faceLose.sprite);
                            } else {
                                Texture faceWin = Texture("files/images/face_win.png", ((numCol / 2.0) * 32)-32, 32 * (numRow+0.5f));
                                gameWindow.draw(faceWin.sprite);
                            }
                        } else {
                            gameWindow.draw(smile.sprite);
                        }


                        elapsed = clock.getElapsedTime();
                        int seconds = tempSeconds + elapsed.asSeconds();
                        if(!board.leaderboardOpen and !board.paused){
                            seconds += board.pausedTime;
                        }
                        if(board.paused and board.firstRun){
                            tempSeconds = seconds;
                            board.firstRun = false;
                            //cout << seconds << " " << board.pausedTime << endl;

                        }
                        if(!board.gameOver and !board.paused){
                            board.boardSec = seconds;
                        } else {
                            if(board.gameOver){
                                if(!board.done){
                                    board.boardSec = seconds;
                                    board.done = true;
                                }
                            } else {
                                board.boardSec = tempSeconds;
                                clock.restart();
                            }
                        }
                        if(board.leaderboardOpen){
                            board.leaderboardOpen = false;
                        }
                        timer timeSec = timer(board.boardSec, numCol, numRow);
                        gameWindow.draw(timeSec.sprite);
                        gameWindow.draw(timeSec.sprite2);
                        gameWindow.draw(timeSec.spriteMin);
                        gameWindow.draw(timeSec.spriteMin2);

                        int remainingMines = numMines - board.flags.size();
                        counter count = counter(remainingMines, numRow);
                        gameWindow.draw(count.sprite);
                        gameWindow.draw(count.sprite2);
                        gameWindow.draw(count.sprite3);

                        gameWindow.display();
                        if(board.gameWon and !board.leaderboardDisplayed){
                            board.gameOver = true;
                            leaderboard newLeaderboard = leaderboard(width, height, 0);
                            string updateName = playerName.getString();
                            updateName.erase(updateName.length() - 1, 1);
                            newLeaderboard.updateLeaderboard(board.boardSec, updateName);

                            int update = 0;
                            if(newLeaderboard.newFirst){
                                update = 1;
                            }
                            if(newLeaderboard.newSecond){
                                update = 2;
                            }
                            if(newLeaderboard.newThird){
                                update = 3;
                            }
                            if(newLeaderboard.newFourth){
                                update = 4;
                            }
                            if(newLeaderboard.newFifth){
                                update = 5;
                            }

                            sf::RenderWindow finalWindowWin(sf::VideoMode(width/2, height/2), "Minesweeper", sf::Style::Close);
                            leaderboard finalLeaderboard = leaderboard(width, height, update);
                            while (finalWindowWin.isOpen()) {
                                while (finalWindowWin.pollEvent(event)) {
                                    if (event.type == sf::Event::Closed) {
                                        finalWindowWin.close();
                                        board.leaderboardDisplayed = true;
                                    }
                                }
                                finalWindowWin.clear(sf::Color::Blue);
                                finalWindowWin.draw(finalLeaderboard.title);
                                finalWindowWin.draw(finalLeaderboard.ranks);
                                finalWindowWin.display();
                            }
                        }

                        board.gameWon = board.checkWin();
                        if(board.gameWon){
                            board.gameOver = true;
                        }
                    }
                }
            } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::BackSpace)){
                if(lenName != 0){
                    playerInput.erase(playerInput.getSize() - 1, 1);
                    playerInput +="|";
                    playerName.setString(playerInput);
                    playerInput.erase(playerInput.getSize() - 1, 1);
                    lenName -= 1;

                    nameRect = playerName.getLocalBounds();
                    playerName.setOrigin(nameRect.left + nameRect.width/2.0f, nameRect.top + nameRect.height/2.0f);
                    playerName.setPosition(sf::Vector2f(width/2.0f, (height/2.0f)-45));
                }
            }
        }
        window.clear(sf::Color::Blue);
        window.draw(text);
        window.draw(text2);
        window.draw(playerName);
        window.display();
    }
}
