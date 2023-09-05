#include <iostream>
#include <vector>
#include <SFML/Graphics.hpp>

using namespace std;

class Texture{
public:
    sf::Texture texture;
    sf::Sprite sprite;
    bool revealed = false;
    bool flagged = false;
    bool mine = false;
    bool recCheck = false;
    int num;
    int _col;
    int _row;
    vector<Texture*> adjTiles;
    Texture(string filename, float at_width, float at_height);
};

class Board {
    float _width;
    float _height;
public:
    bool gameOver = false;
    bool gameWon = false;
    bool debugging = false;
    bool paused = false;
    bool firstRun = false;
    bool leaderboardDisplayed = false;
    bool leaderboardOpen = false;
    bool leaderboardOpening = false;
    bool done = false;
    int boardSec = 0;
    int pausedTime = 0;
    vector<vector<Texture>> tiles;
    vector<Texture*> mines;
    vector<Texture*> debugMines;
    vector<Texture*> flags;
    vector<Texture*> nums;
    Board(string filename, float width, float height);
    Texture* overlayTexture(string filename, int col, int row);
    void updateTexture(string filename, int col, int row);
    void addMine();
    void addNums();
    string numOfSprite(int num);
    void revealAdjacent(int i, int j);
    void revealMines();
    bool checkWin();
};

class timer{
public:
    int seconds = 0;
    sf::Texture texture;
    sf::Texture texture2;
    sf::Texture textMin;
    sf::Sprite sprite;
    sf::Sprite sprite2;
    sf::Sprite spriteMin;
    sf::Texture textMin2;
    sf::Sprite spriteMin2;
    timer(int time, int cols, int rows);
};

class counter{
public:
    bool negative = false;
    sf::Texture texture;
    sf::Texture texture2;
    sf::Texture texture3;
    sf::Sprite sprite;
    sf::Sprite sprite2;
    sf::Sprite sprite3;
    counter(int num, int rows);
};

class leaderboard{
public:
    sf::Text title;
    sf::Font font;
    sf::Text ranks;
    bool newFirst = false;
    bool newSecond = false;
    bool newThird = false;
    bool newFourth = false;
    bool newFifth = false;
    vector<int> times;
    vector<string> names;
    leaderboard(int _width, int _height, int updated);
    void updateLeaderboard(int time, string name);
};