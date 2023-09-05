#include "Board.h"
#include <SFML/Graphics.hpp>
#include <random>
#include <fstream>

Texture::Texture(string filename, float at_width, float at_height) {
    _col = at_width/32;
    _row = at_height/32;
    texture.loadFromFile(filename);
    sprite.setTexture(texture);
    sprite.setPosition(sf::Vector2f(at_width, at_height));
}

Board::Board(string filename, float width, float height) {
    _width = width;
    _height = height;
    float num_cols = width/32;
    float num_rows = ((height-100)/32);
    float temp_width = 0;
    float temp_height = 0;
    for(int i = 0; i < num_cols; i++){
        vector<Texture> colTiles;
        for(int j = 0; j < num_rows; j++){
            Texture* tempTile = new Texture(filename, temp_width, temp_height);
            colTiles.push_back(*tempTile);
            temp_height += 32;
        }
        tiles.push_back(colTiles);
        temp_width += 32;
        temp_height = 0;
    }
}

Texture* Board::overlayTexture(string filename, int col, int row) {
    float temp_width = col * 32;
    float temp_height = row * 32;
    Texture* newTexture = new Texture(filename, temp_width, temp_height);
    return newTexture;
}

void Board::updateTexture(string filename, int col, int row) {
    float temp_width = col * 32;
    float temp_height = row * 32;
    Texture* tempTile = new Texture(filename, temp_width, temp_height);
    if (tiles.at(col).at(row).mine){
        tempTile->mine = true;
    }
    tempTile->num = tiles.at(col).at(row).num;
    tempTile->adjTiles = tiles.at(col).at(row).adjTiles;
    tiles.at(col).at(row) = *tempTile;
}

void Board::addMine() {
    bool mineAdded = false;
    mt19937 random_mt(time(0));
    uniform_int_distribution<int> widthDist(0, ((_width/32) - 1));
    uniform_int_distribution<int> heightDist(0, ((_height/32) - 5));
    while (! mineAdded){
        int mineCol = widthDist(random_mt);
        int mineRow = heightDist(random_mt);
        if (! tiles.at(mineCol).at(mineRow).mine){
            tiles.at(mineCol).at(mineRow).mine = true;
            mineAdded = true;
        }
    }
}

void Board::addNums() {
    int width = _width/32, height = (_height / 32) - 3;
    for(int i = 0; i < width; i++) {
        for (int j = 0; j < height; j++) {
            if(!tiles.at(i).at(j).mine){
                int count = 0;
                if(i == 0 and j == 0){
                    // You are the top left
                    tiles[i][j].adjTiles.push_back(&tiles[i+1][j]);
                    tiles[i][j].adjTiles.push_back(&tiles[i][j+1]);
                    tiles[i][j].adjTiles.push_back(&tiles[i+1][j+1]);
                    count += tiles[i+1][j].mine + tiles[i][j+1].mine + tiles[i+1][j+1].mine;
                }else if(i == 0 and j == height-1){
                    // bottom left
                    tiles[i][j].adjTiles.push_back(&tiles[i][j-1]);
                    tiles[i][j].adjTiles.push_back(&tiles[i+1][j-1]);
                    tiles[i][j].adjTiles.push_back(&tiles[i+1][j]);
                    count += tiles[i][j-1].mine + tiles[i+1][j-1].mine + tiles[i+1][j].mine;
                }else if (i == width-1 and j == 0){
                    // top right
                    tiles[i][j].adjTiles.push_back(&tiles[i-1][j]);
                    tiles[i][j].adjTiles.push_back(&tiles[i-1][j+1]);
                    tiles[i][j].adjTiles.push_back(&tiles[i][j+1]);
                    count += tiles[i-1][j].mine + tiles[i-1][j+1].mine + tiles[i][j+1].mine;
                }else if (i == width-1 and j == height-1){
                    // bottom right
                    tiles[i][j].adjTiles.push_back(&tiles[i-1][j-1]);
                    tiles[i][j].adjTiles.push_back(&tiles[i-1][j]);
                    tiles[i][j].adjTiles.push_back(&tiles[i][j-1]);
                    count += tiles[i-1][j-1].mine + tiles[i-1][j].mine + tiles[i][j-1].mine;
                }else if (i == 0){
                    tiles[i][j].adjTiles.push_back(&tiles[i][j+1]);
                    tiles[i][j].adjTiles.push_back(&tiles[i+1][j+1]);
                    tiles[i][j].adjTiles.push_back(&tiles[i+1][j]);
                    tiles[i][j].adjTiles.push_back(&tiles[i+1][j-1]);
                    tiles[i][j].adjTiles.push_back(&tiles[i][j-1]);
                    count += tiles[i][j+1].mine + tiles[i+1][j+1].mine + tiles[i+1][j].mine + tiles[i+1][j-1].mine + tiles[i][j-1].mine;
                }else if(j == 0){
                    tiles[i][j].adjTiles.push_back(&tiles[i-1][j]);
                    tiles[i][j].adjTiles.push_back(&tiles[i-1][j+1]);
                    tiles[i][j].adjTiles.push_back(&tiles[i][j+1]);
                    tiles[i][j].adjTiles.push_back(&tiles[i+1][j+1]);
                    tiles[i][j].adjTiles.push_back(&tiles[i+1][j]);
                    count += tiles[i-1][j].mine + tiles[i-1][j+1].mine + tiles[i][j+1].mine + tiles[i+1][j+1].mine + tiles[i+1][j].mine;
                }else if(j == height - 1){
                    tiles[i][j].adjTiles.push_back(&tiles[i-1][j]);
                    tiles[i][j].adjTiles.push_back(&tiles[i-1][j-1]);
                    tiles[i][j].adjTiles.push_back(&tiles[i][j-1]);
                    tiles[i][j].adjTiles.push_back(&tiles[i+1][j-1]);
                    tiles[i][j].adjTiles.push_back(&tiles[i+1][j]);
                    count += tiles[i-1][j].mine + tiles[i-1][j-1].mine + tiles[i][j-1].mine + tiles[i+1][j-1].mine + tiles[i+1][j].mine;
                }else if(i == width - 1){
                    tiles[i][j].adjTiles.push_back(&tiles[i][j-1]);
                    tiles[i][j].adjTiles.push_back(&tiles[i-1][j-1]);
                    tiles[i][j].adjTiles.push_back(&tiles[i-1][j]);
                    tiles[i][j].adjTiles.push_back(&tiles[i-1][j+1]);
                    tiles[i][j].adjTiles.push_back(&tiles[i][j+1]);
                    count += tiles[i][j-1].mine + tiles[i-1][j-1].mine + tiles[i-1][j].mine + tiles[i-1][j+1].mine + tiles[i][j+1].mine;
                }else{
                    tiles[i][j].adjTiles.push_back(&tiles[i-1][j]);
                    tiles[i][j].adjTiles.push_back(&tiles[i-1][j+1]);
                    tiles[i][j].adjTiles.push_back(&tiles[i][j+1]);
                    tiles[i][j].adjTiles.push_back(&tiles[i+1][j+1]);
                    tiles[i][j].adjTiles.push_back(&tiles[i+1][j]);
                    tiles[i][j].adjTiles.push_back(&tiles[i][j-1]);
                    tiles[i][j].adjTiles.push_back(&tiles[i+1][j-1]);
                    tiles[i][j].adjTiles.push_back(&tiles[i-1][j-1]);
                    count += tiles[i-1][j].mine + tiles[i-1][j+1].mine + tiles[i][j+1].mine + tiles[i+1][j+1].mine + tiles[i+1][j].mine + tiles[i][j-1].mine + tiles[i+1][j-1].mine + tiles[i-1][j-1].mine;
                }
                tiles.at(i).at(j).num = count;
            }
        }
    }
}

string Board::numOfSprite(int num) {
    if(num == 1){
        return "files/images/number_1.png";
    } else if (num == 2){
        return "files/images/number_2.png";
    } else if (num == 3){
        return "files/images/number_3.png";
    } else if (num == 4){
        return "files/images/number_4.png";
    } else if (num == 5){
        return "files/images/number_5.png";
    } else if (num == 6){
        return "files/images/number_6.png";
    } else if (num == 7){
        return "files/images/number_7.png";
    }
    return "files/images/number_8.png";
}

void Board::revealAdjacent(int i, int j) {
    if (tiles.at(i).at(j).num == 0){
        for (int k = 0; k < tiles.at(i).at(j).adjTiles.size(); k++){
            if(!tiles.at(i).at(j).adjTiles.at(k)->flagged){
                if(!tiles.at(i).at(j).adjTiles.at(k)->revealed) {
                    updateTexture("files/images/tile_revealed.png", tiles.at(i).at(j).adjTiles.at(k)->_col, tiles.at(i).at(j).adjTiles.at(k)->_row);
                    tiles.at(i).at(j).adjTiles.at(k)->revealed = true;
                }
                if (tiles.at(i).at(j).adjTiles.at(k)-> num == 0){
                    if(!tiles.at(i).at(j).adjTiles.at(k)->recCheck){
                        tiles.at(tiles.at(i).at(j).adjTiles.at(k)->_col).at(tiles.at(i).at(j).adjTiles.at(k)->_row).recCheck = true;
                        revealAdjacent(tiles.at(i).at(j).adjTiles.at(k)->_col, tiles.at(i).at(j).adjTiles.at(k)->_row);
                    }
                } else {
                    Texture* tempTexture = overlayTexture("files/images/number_" + to_string(tiles.at(i).at(j).adjTiles.at(k)->num) + ".png", tiles.at(i).at(j).adjTiles.at(k)->_col, tiles.at(i).at(j).adjTiles.at(k)->_row);
                    nums.push_back(tempTexture);
                }
            }
        }
    }
    tiles.at(i).at(j).recCheck = true;
}

void Board::revealMines() {
    for(int i = 0; i < (_width/32); i++) {
        for (int j = 0; j < ((_height / 32) - 4); j++) {
            if (tiles.at(i).at(j).mine){
                updateTexture("files/images/tile_revealed.png", i, j);
                mines.push_back(overlayTexture("files/images/mine.png", i, j));
            }
        }
    }
}

bool Board::checkWin() {
    for(int i = 0; i < (_width/32); i++){
        for(int j = 0; j < ((_height/32)-4); j++){
            if(!tiles.at(i).at(j).mine){
                if(!tiles.at(i).at(j).revealed){
                    return false;
                }
            } else {
                if(!tiles.at(i).at(j).flagged){
                    return false;
                }
            }
        }
    }
    return true;
}

timer::timer(int time, int cols, int rows) {
    seconds = time;
    int numPos;
    int minutes = 0;
    if(seconds >= 60){
        minutes += (seconds/60);
        seconds = seconds%60;
    }
    if(seconds >= 10){
        numPos = (seconds%10) * 21;
    } else {
        numPos = seconds * 21;
    }
    texture.loadFromFile("files/images/digits.png", sf::IntRect(numPos, 0, 21, 32));
    sprite.setTexture(texture);
    sprite.setPosition(sf::Vector2f(((cols*32)-33), 32*(rows+0.5f)+16));

    int numPos2 = (seconds/10) * 21;
    texture2.loadFromFile("files/images/digits.png", sf::IntRect(numPos2, 0, 21, 32));
    sprite2.setTexture(texture2);
    sprite2.setPosition(sf::Vector2f(((cols*32)-54), 32*(rows+0.5f)+16));

    int posMinute;
    if(minutes >= 10){
        posMinute = (minutes%10) * 21;
    } else {
        posMinute = minutes * 21;
    }
    textMin.loadFromFile("files/images/digits.png", sf::IntRect(posMinute, 0, 21, 32));
    spriteMin.setTexture(textMin);
    spriteMin.setPosition(sf::Vector2f(((cols*32)-76), 32*(rows+0.5f)+16));

    int posMinute2 = (minutes/10) * 21;
    textMin2.loadFromFile("files/images/digits.png", sf::IntRect(posMinute2, 0, 21, 32));
    spriteMin2.setTexture(textMin2);
    spriteMin2.setPosition(sf::Vector2f(((cols*32)-97), 32*(rows+0.5f)+16));

}

counter::counter(int num, int rows) {
    int numPos;
    int numPos2 = 0;
    int numPos3 = 0;
    if(num < 0){
        numPos3 = 210;
        num *= -1;
        negative = true;
    }
    if(num >= 10){
        numPos = (num%10) * 21;
        numPos2 += (num/10);
    } else {
        numPos = num * 21;
    }

    if(numPos2 >= 10){
        numPos2 = (numPos2%10) * 21;
        if(!negative){
            numPos3 += (numPos2/10);
        }
    } else {
        numPos2 = numPos2 * 21;
    }

    if(!negative){
        numPos3 = numPos3 * 21;
    }
    texture.loadFromFile("files/images/digits.png", sf::IntRect(numPos, 0, 21, 32));
    sprite.setTexture(texture);
    sprite.setPosition(sf::Vector2f(54, 32*(rows+0.5f)+16));

    texture2.loadFromFile("files/images/digits.png", sf::IntRect(numPos2, 0, 21, 32));
    sprite2.setTexture(texture2);
    sprite2.setPosition(sf::Vector2f(33, 32*(rows+0.5f)+16));

    texture3.loadFromFile("files/images/digits.png", sf::IntRect(numPos3, 0, 21, 32));
    sprite3.setTexture(texture3);
    sprite3.setPosition(sf::Vector2f(12, 32*(rows+0.5f)+16));
}

leaderboard::leaderboard(int _width, int _height, int updated) {
    fstream leaderFile("files/leaderboard.txt", ios_base::in);
    string temp;
    string time;
    string name;
    string delimiter = ",";
    string final = "";
    int num = 0;
    if(updated == 1){
        newFirst = true;
    }
    if(updated == 2){
        newSecond = true;
    }
    if(updated == 3){
        newThird = true;
    }
    if(updated == 4){
        newFourth = true;
    }
    if(updated == 5){
        newFifth = true;
    }
    while(getline(leaderFile, temp)){
        num += 1;
        time = temp.substr(0, temp.find(delimiter));
        string min = time.substr(0, temp.find(":"));
        string sec = time.substr(temp.find(":")+1, time.length()-min.length()-1);
        string storeTime = min + sec;
        name = temp.substr(temp.find(delimiter)+1, temp.length()-time.length()-1);
        times.push_back(stoi(storeTime));
        names.push_back(name);

        if(newFirst and num == 1){
            name += "*";
        }
        if(newSecond and num == 2){
            name += "*";
        }
        if(newThird and num == 3){
            name += "*";
        }
        if(newFourth and num == 4){
            name += "*";
        }
        if(newFifth and num == 5){
            name += "*";
        }
        final += to_string(num) + "." + "\t" + time + "\t" + name + "\n\n";
    }
    newFirst = false;
    newSecond = false;
    newThird = false;
    newFourth = false;
    newFifth = false;

    font.loadFromFile("files/font.ttf");
    title.setFont(font);
    title.setString("LEADERBOARD");
    title.setCharacterSize(20);
    title.setStyle(sf::Text::Underlined | sf::Text::Bold);

    sf::FloatRect titleRect = title.getLocalBounds();
    title.setOrigin(titleRect.left + titleRect.width/2.0f, titleRect.top + titleRect.height/2.0f);
    title.setPosition(sf::Vector2f(_width/4.0f, (_height/4.0f)-120));

    ranks.setFont(font);
    ranks.setString(final);
    ranks.setCharacterSize(18);
    ranks.setStyle(sf::Text::Bold);

    sf::FloatRect rankRect = ranks.getLocalBounds();
    ranks.setOrigin(rankRect.left + rankRect.width/2.0f, rankRect.top + rankRect.height/2.0f);
    ranks.setPosition(sf::Vector2f(_width/4.0f, (_height/4.0f)+20));
}

void leaderboard::updateLeaderboard(int time, string name) {
    if(time < times.at(0)){
        times.at(4) = times.at(3);
        times.at(3) = times.at(2);
        times.at(2) = times.at(1);
        times.at(1) = times.at(0);
        times.at(0) = time;
        names.at(4) = names.at(3);
        names.at(3) = names.at(2);
        names.at(2) = names.at(1);
        names.at(1) = names.at(0);
        names.at(0) = name;
        newFirst = true;
    } else if (time < times.at(1)){
        times.at(4) = times.at(3);
        times.at(3) = times.at(2);
        times.at(2) = times.at(1);
        times.at(1) = time;
        names.at(4) = names.at(3);
        names.at(3) = names.at(2);
        names.at(2) = names.at(1);
        names.at(1) = name;
        newSecond = true;
    } else if (time < times.at(2)){
        times.at(4) = times.at(3);
        times.at(3) = times.at(2);
        times.at(2) = time;
        names.at(4) = names.at(3);
        names.at(3) = names.at(2);
        names.at(2) = name;
        newThird = true;
    } else if (time < times.at(3)){
        times.at(4) = times.at(3);
        times.at(3) = time;
        names.at(4) = names.at(3);
        names.at(3) = name;
        newFourth = true;
    } else if (time < times.at(4)){
        times.at(4) = time;
        names.at(4) = name;
        newFifth = true;
    }

    string finalString0;
    if(times.at(0)/100 < 10){
        finalString0 = "0" + to_string(times.at(0)/100);
    } else {
        finalString0 = to_string(times.at(0)/100);
    }
    string finalString1;
    if(times.at(1)/100 < 10){
        finalString1 = "0" + to_string(times.at(1)/100);
    } else {
        finalString1 = to_string(times.at(1)/100);
    }
    string finalString2;
    if(times.at(2)/100 < 10){
        finalString2 = "0" + to_string(times.at(2)/100);
    } else {
        finalString2 = to_string(times.at(2)/100);
    }
    string finalString3;
    if(times.at(3)/100 < 10){
        finalString3 = "0" + to_string(times.at(3)/100);
    } else {
        finalString3 = to_string(times.at(3)/100);
    }
    string finalString4;
    if(times.at(4)/100 < 10){
        finalString4 = "0" + to_string(times.at(4)/100);
    } else {
        finalString4 = to_string(times.at(4)/100);
    }

    string String0;
    if(times.at(0)%100 < 10){
        String0 = "0" + to_string(times.at(0)%100);
    } else {
        String0 = to_string(times.at(0)%100);
    }
    string String1;
    if(times.at(1)%100 < 10){
        String1 = "0" + to_string(times.at(1)%100);
    } else {
        String1 = to_string(times.at(1)%100);
    }
    string String2;
    if(times.at(2)%100 < 10){
        String2 = "0" + to_string(times.at(2)%100);
    } else {
        String2 = to_string(times.at(2)%100);
    }
    string String3;
    if(times.at(3)%100 < 10){
        String3 = "0" + to_string(times.at(3)%100);
    } else {
        String3 = to_string(times.at(3)%100);
    }
    string String4;
    if(times.at(4)%100 < 10){
        String4 = "0" + to_string(times.at(4)%100);
    } else {
        String4 = to_string(times.at(4)%100);
    }

    fstream updateFile("files/leaderboard.txt", ios_base::out);
    updateFile << finalString0 << ":" << String0 << "," << names.at(0) << endl;
    updateFile << finalString1 << ":" << String1 << "," << names.at(1) << endl;
    updateFile << finalString2 << ":" << String2 << "," << names.at(2) << endl;
    updateFile << finalString3 << ":" << String3 << "," << names.at(3) << endl;
    updateFile << finalString4 << ":" << String4 << "," << names.at(4) << endl;
}
