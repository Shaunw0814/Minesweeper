#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <unordered_map>
#include "TextureManager.h"
#include "Random.h"
using namespace std;
using namespace sf;

Sprite digitChanger(vector<Sprite> sp, int dCheck) {
    if (dCheck > sp.size()) {
        return sp.at(dCheck - 9);
    }
    return sp.at(dCheck);
}

vector<int> readFile_cfg(string filePath) {
    vector<int> cfg;

    ifstream inFile("boards/" + filePath + ".cfg");
    string tempVar;
    if(inFile.is_open()) {
        while (getline(inFile, tempVar)) {
            cfg.push_back(stoi(tempVar));
        }
    }

    return cfg;
}
vector<int> readFile_board(string filePath) {
    vector<int> mines;

    ifstream inFile("boards/" + filePath + ".brd");

    int counter = 0;
    string tempVar;
    string lineFromFile;

    if (inFile.is_open()) {
        while (getline(inFile, lineFromFile)) {
            for (int i = 0; i < lineFromFile.length(); i++) {
                tempVar = lineFromFile.substr(i, 1);
                mines.push_back(stoi(tempVar));
                counter++;
            }
        }
    }

    inFile.close();
    return mines;
}
vector<int> resetVector(vector<int> cfg) {
    vector<int> reset;
    for (int i = 0; i < cfg.at(0) * cfg.at(1); i++) {
        reset.push_back(0);
    }
    return reset;
}

vector<Sprite> makeMap(vector<int> cfg, vector<int> revLocation, Sprite tileHidden, Sprite tileRevealed) {
    vector<Sprite> tileMap;
    for (int i = 0; i < cfg.at(0) * cfg.at(1); i++) {
        if (revLocation.at(i) != 1) {
            tileHidden.setPosition((i % cfg.at(0)) * 32, (i / cfg.at(0)) * 32);
            tileMap.push_back(tileHidden);
        }
        else {
            tileRevealed.setPosition((i % cfg.at(0)) * 32, (i / cfg.at(0)) * 32);
            tileMap.push_back(tileRevealed);
        }
    }

    return tileMap;
}
vector<Sprite> makeSprites(int columns, vector<int> location, Sprite sp) {
    vector<Sprite> spriteReturn;
    for (int i = 0; i < location.size(); i++) {
        if (location.at(i) == 1) {
            sp.setPosition((i % columns) * 32, (i / columns) * 32);
            spriteReturn.push_back(sp);
        }
    }

    return spriteReturn;
}
vector<Sprite> makeNumbers(int columns, vector<int> location, vector<Sprite> sp) {
    vector<Sprite> spriteReturn;
    for (int i = 0; i < location.size(); i++) {
        if (location.at(i) != 0) {
            sp.at(location.at(i) - 1).setPosition((i % columns) * 32, (i / columns) * 32);
            spriteReturn.push_back(sp.at(location.at(i) - 1));
        }
    }

    return spriteReturn;
}
void markClick(vector<int>& change, vector<int> cfg, int xLoc, int yLoc, int mode) {
    int indexChange = xLoc + (yLoc * cfg.at(0));
    if (mode == 1) {
        if (change.at(indexChange) != 1) {
            change.at(indexChange) = 1;
        }
        else {
            change.at(indexChange) = 0;
        }
    }
    else {
        change.at(indexChange) = 1;
    }
    
}
int checkOnes(vector<int> countThis) {
    int count = 0;
    for (int i = 0; i < countThis.size(); i++) {
        if (countThis.at(i) == 1)
        {
            count++;
        } 
    }

    return count;
}
int adjacentMines(vector<int> mines, int xLoc, int yLoc, int columns, int rows) {
    int adjMines = 0;
    //8 conditions that do not have 8 surrounding tiles
    //in a corner
    if (xLoc == 0 && yLoc == 0) {                 //top left
        if (mines.at((xLoc + 1) + (yLoc * columns)) == 1) {                              //right
            adjMines++;
        }
        if (mines.at(xLoc + ((yLoc + 1) * columns)) == 1) {         //bottom
            adjMines++;
        }
        if (mines.at((xLoc + 1) + ((yLoc + 1) * columns)) == 1) {   //bottom right
            adjMines++;
        }
    }
    else if (xLoc == 0 && yLoc == rows - 1) {      //bottom left
        if (mines.at((xLoc + 1) + (yLoc * columns)) == 1) {                              //right
            adjMines++;
        }
        if (mines.at(xLoc + ((yLoc - 1) * columns)) == 1) {         //top
            adjMines++;
        }
        if (mines.at((xLoc + 1) + ((yLoc - 1) * columns)) == 1) {   //top right
            adjMines++;
        }
    }
    else if (xLoc == columns - 1 && yLoc == 0) {              //top right
        if (mines.at((xLoc - 1) + (yLoc * columns)) == 1) {                              //left
            adjMines++;
        }
        if (mines.at(xLoc + ((yLoc + 1) * columns)) == 1) {         //bottom
            adjMines++;
        }
        if (mines.at((xLoc - 1) + ((yLoc + 1) * columns)) == 1) {   //bottom left
            adjMines++;
        }
    }
    else if (xLoc == columns - 1 && yLoc == rows - 1) {       //bottom right
        if (mines.at((xLoc - 1) + (yLoc * columns)) == 1) {                              //left
            adjMines++;
        }
        if (mines.at(xLoc + ((yLoc - 1) * columns)) == 1) {         //top
            adjMines++;
        }
        if (mines.at((xLoc - 1) + ((yLoc - 1) * columns)) == 1) {   //top left
            adjMines++;
        }
    }
    //on a side
    else if (xLoc == 0) {                    //left
        if (mines.at((xLoc + 1) + (yLoc * columns)) == 1) {                              //right
            adjMines++;
        }
        if (mines.at(xLoc + ((yLoc - 1) * columns)) == 1) {         //top
            adjMines++;
        }
        if (mines.at((xLoc + 1) + ((yLoc - 1) * columns)) == 1) {   //top right
            adjMines++;
        }
        if (mines.at(xLoc + ((yLoc + 1) * columns)) == 1) {         //bottom
            adjMines++;
        }
        if (mines.at((xLoc + 1) + ((yLoc + 1) * columns)) == 1) {   //bottom right
            adjMines++;
        }
    }
    else if (xLoc == columns - 1) {          //right
        if (mines.at((xLoc - 1) + (yLoc * columns)) == 1) {                              //left
            adjMines++;
        }
        if (mines.at(xLoc + ((yLoc - 1) * columns)) == 1) {         //top
            adjMines++;
        }
        if (mines.at((xLoc - 1) + ((yLoc - 1) * columns)) == 1) {   //top left
            adjMines++;
        }
        if (mines.at(xLoc + ((yLoc + 1) * columns)) == 1) {         //bottom
            adjMines++;
        }
        if (mines.at((xLoc - 1) + ((yLoc + 1) * columns)) == 1) {   //bottom left
            adjMines++;
        }
    }
    else if (yLoc == 0) {                    //top
        if (mines.at((xLoc - 1) + (yLoc * columns)) == 1) {                              //left
            adjMines++;
        }
        if (mines.at((xLoc - 1) + ((yLoc + 1) * columns)) == 1) {   //bottom left
            adjMines++;
        }
        if (mines.at((xLoc + 1) + (yLoc * columns)) == 1) {                              //right
            adjMines++;
        }
        if (mines.at((xLoc + 1) + ((yLoc + 1) * columns)) == 1) {   //bottom right
            adjMines++;
        }
        if (mines.at(xLoc + ((yLoc + 1) * columns)) == 1) {         //bottom
            adjMines++;
        }
    }
    else if (yLoc == rows - 1) {             //bottom
        if (mines.at((xLoc - 1) + (yLoc * columns)) == 1) {         //left
            adjMines++;
        }
        if (mines.at((xLoc - 1) + ((yLoc - 1) * columns)) == 1) {   //top left
            adjMines++;
        }
        if (mines.at((xLoc + 1) + (yLoc * columns)) == 1) {         //right
            adjMines++;
        }
        if (mines.at((xLoc + 1) + ((yLoc - 1) * columns)) == 1) {   //top right
            adjMines++;
        }
        if (mines.at(xLoc + ((yLoc - 1) * columns)) == 1) {         //top
            adjMines++;
        }
    }
    //not on a side or in a corner
    else {
        if (mines.at((xLoc - 1) + ((yLoc - 1) * columns)) == 1) {   //top left
            adjMines++;
        }
        if (mines.at((xLoc + 1) + ((yLoc - 1) * columns)) == 1) {   //top right
            adjMines++;
        }
        if (mines.at(xLoc + ((yLoc - 1) * columns)) == 1) {         //top
            adjMines++;
        }
        if (mines.at((xLoc - 1) + ((yLoc + 1) * columns)) == 1) {   //bottom left
            adjMines++;
        }
        if (mines.at((xLoc + 1) + ((yLoc + 1) * columns)) == 1) {   //bottom right
            adjMines++;
        }
        if (mines.at(xLoc + ((yLoc + 1) * columns)) == 1) {         //bottom
            adjMines++;
        }
        if (mines.at((xLoc - 1) + (yLoc * columns)) == 1) {                              //left
            adjMines++;
        }
        if (mines.at((xLoc + 1) + (yLoc * columns)) == 1) {                              //right
            adjMines++;
        }
    }

    return adjMines;
}
vector<int> randomlyGenerate(vector<int> cfg) {
    vector<int> randomBoard = resetVector(cfg);
    int indexR;
    unordered_map<int, int> randomNumList;

    while (randomNumList.size() < cfg.at(2)) {                           //no repeats because using emplace
        int randomNum = Random::Int(0, cfg.at(0) * cfg.at(1) - 1);
        randomNumList.emplace(randomNum, 1);
    }

    for (auto iter = randomNumList.begin(); iter != randomNumList.end(); iter++) {
        indexR = iter->first;
        if (indexR < randomBoard.size()) {
            randomBoard.at(indexR) = iter->second;
        }
    }

    return randomBoard;
}
void adjacentReveal(vector<int> flagLocation, vector<int>& revLocation, vector<int> adjLocation, vector<int>& adjLoc, vector<int> cfg, int index) {
    revLocation;
    revLocation.at(index) = 1;
    int top, bottom, left, right, topL, topR, botR, botL;
    int column = cfg.at(0);
    int xLoc = index % column;
    int yLoc = index / column;
    if (xLoc != 0 && xLoc != column - 1 && yLoc != 0 && yLoc != cfg.at(1) - 1) {
        // top
            top = xLoc + ((yLoc - 1) * column);
        if (revLocation.at(top) != 1 && flagLocation.at(top) != 1) {
            if (adjLocation.at(top) == 0 && revLocation.at(top) != 1) {              //top
                adjacentReveal(flagLocation, revLocation, adjLocation, adjLoc, cfg, top);
            }
            else {
                revLocation.at(top) = 1;
                adjLoc.at(top) = adjLocation.at(top);
            }
        }
        //bottom
        bottom = xLoc + ((yLoc + 1) * column);
        if (revLocation.at(bottom) != 1 && flagLocation.at(bottom) != 1) {
            if (adjLocation.at(bottom) == 0 && revLocation.at(bottom) != 1) {              //bottom
                adjacentReveal(flagLocation, revLocation, adjLocation, adjLoc, cfg, bottom);
            }
            else {
                revLocation.at(bottom) = 1;
                adjLoc.at(bottom) = adjLocation.at(bottom);
            }
        }

        //left 
        left = xLoc - 1 + (yLoc * column);
        if (revLocation.at(left) != 1 && flagLocation.at(left) != 1) {
            if (adjLocation.at(left) == 0 && revLocation.at(left) != 1) {              //left
                adjacentReveal(flagLocation, revLocation, adjLocation, adjLoc, cfg, left);
            }
            else {
                revLocation.at(left) = 1;
                adjLoc.at(left) = adjLocation.at(left);
            }
        }
        //topL
        topL = xLoc - 1 + ((yLoc - 1) * column);
        if (revLocation.at(topL) != 1 && flagLocation.at(topL) != 1) {
            if (adjLocation.at(topL) == 0 && revLocation.at(topL) != 1) {              //topL
                adjacentReveal(flagLocation, revLocation, adjLocation, adjLoc, cfg, topL);
            }
            else {
                revLocation.at(topL) = 1;
                adjLoc.at(topL) = adjLocation.at(topL);
            }
        }

        //botL
        botL = xLoc - 1 + ((yLoc + 1) * column);
        if (revLocation.at(botL) != 1 && flagLocation.at(botL) != 1) {
            if (adjLocation.at(botL) == 0 && revLocation.at(botL) != 1) {              //botL
                adjacentReveal(flagLocation, revLocation, adjLocation, adjLoc, cfg, botL);
            }
            else {
                revLocation.at(botL) = 1;
                adjLoc.at(botL) = adjLocation.at(botL);
            }
        }

        //right
        right = xLoc + 1 + (yLoc * column);
        if (revLocation.at(right) != 1 && flagLocation.at(right) != 1) {
            if (adjLocation.at(right) == 0 && revLocation.at(right) != 1) {              //right
                adjacentReveal(flagLocation, revLocation, adjLocation, adjLoc, cfg, right);
            }
            else {
                revLocation.at(right) = 1;
                adjLoc.at(right) = adjLocation.at(right);
            }
        }

        //topR
        topR = xLoc + 1 + ((yLoc - 1) * column);
        if (revLocation.at(topR) != 1 && flagLocation.at(topR) != 1) {
            if (adjLocation.at(topR) == 0 && revLocation.at(topR) != 1) {              //topR
                adjacentReveal(flagLocation, revLocation, adjLocation, adjLoc, cfg, topR);
            }
            else {
                revLocation.at(topR) = 1;
                adjLoc.at(topR) = adjLocation.at(topR);
            }
        }

        //bottomR
        botR = xLoc + 1 + ((yLoc + 1) * column);
        if (revLocation.at(botR) != 1 && flagLocation.at(botR) != 1) {
            if (adjLocation.at(botR) == 0 && revLocation.at(botR) != 1) {              //botR
                adjacentReveal(flagLocation, revLocation, adjLocation, adjLoc, cfg, botR);
            }
            else {
                revLocation.at(botR) = 1;
                adjLoc.at(botR) = adjLocation.at(botR);
            }
        }
    }
    else if (xLoc == 0 && yLoc != 0 && yLoc != cfg.at(1) - 1) {
        //top
        top = xLoc + ((yLoc - 1) * column);
        if (revLocation.at(top) != 1 && flagLocation.at(top) != 1) {
            if (adjLocation.at(top) == 0 && revLocation.at(top) != 1) {              //top
                adjacentReveal(flagLocation, revLocation, adjLocation, adjLoc, cfg, top);
            }
            else {
                revLocation.at(top) = 1;
                adjLoc.at(top) = adjLocation.at(top);
            }
        }

        //bottom
        bottom = xLoc + ((yLoc + 1) * column);
        if (revLocation.at(bottom) != 1 && flagLocation.at(bottom) != 1) {
            if (adjLocation.at(bottom) == 0 && revLocation.at(bottom) != 1) {              //bottom
                adjacentReveal(flagLocation, revLocation, adjLocation, adjLoc, cfg, bottom);
            }
            else {
                revLocation.at(bottom) = 1;
                adjLoc.at(bottom) = adjLocation.at(bottom);
            }
        }

        //right
        right = xLoc + 1 + (yLoc * column);
        if (revLocation.at(right) != 1 && flagLocation.at(right) != 1) {
            if (adjLocation.at(right) == 0 && revLocation.at(right) != 1) {              //right
                adjacentReveal(flagLocation, revLocation, adjLocation, adjLoc, cfg, right);
            }
            else {
                revLocation.at(right) = 1;
                adjLoc.at(right) = adjLocation.at(right);
            }
        }

        //topR
        topR = xLoc + 1 + ((yLoc - 1) * column);
        if (revLocation.at(topR) != 1 && flagLocation.at(topR) != 1) {
            if (adjLocation.at(topR) == 0 && revLocation.at(topR) != 1) {              //topR
                adjacentReveal(flagLocation, revLocation, adjLocation, adjLoc, cfg, topR);
            }
            else {
                revLocation.at(topR) = 1;
                adjLoc.at(topR) = adjLocation.at(topR);
            }
        }

        //bottomR
        botR = xLoc + 1 + ((yLoc + 1) * column);
        if (revLocation.at(botR) != 1 && flagLocation.at(botR) != 1) {
            if (adjLocation.at(botR) == 0 && revLocation.at(botR) != 1) {              //botR
                adjacentReveal(flagLocation, revLocation, adjLocation, adjLoc, cfg, botR);
            }
            else {
                revLocation.at(botR) = 1;
                adjLoc.at(botR) = adjLocation.at(botR);
            }
        }
    }
    else if (xLoc == cfg.at(0) - 1 && yLoc != 0 && yLoc != cfg.at(1) - 1) {
        //top
        top = xLoc + ((yLoc - 1) * column);
        if (revLocation.at(top) != 1 && flagLocation.at(top) != 1) {
            if (adjLocation.at(top) == 0 && revLocation.at(top) != 1) {              //top
                adjacentReveal(flagLocation, revLocation, adjLocation, adjLoc, cfg, top);
            }
            else {
                revLocation.at(top) = 1;
                adjLoc.at(top) = adjLocation.at(top);
            }
        }
        //bottom
        bottom = xLoc + ((yLoc + 1) * column);
        if (revLocation.at(bottom) != 1 && flagLocation.at(bottom) != 1) {
            if (adjLocation.at(bottom) == 0 && revLocation.at(bottom) != 1) {              //bottom
                adjacentReveal(flagLocation, revLocation, adjLocation, adjLoc, cfg, bottom);
            }
            else {
                revLocation.at(bottom) = 1;
                adjLoc.at(bottom) = adjLocation.at(bottom);
            }
        }

        //left 
        left = xLoc - 1 + (yLoc * column);
        if (revLocation.at(left) != 1 && flagLocation.at(left) != 1) {
            if (adjLocation.at(left) == 0 && revLocation.at(left) != 1) {              //left
                adjacentReveal(flagLocation, revLocation, adjLocation, adjLoc, cfg, left);
            }
            else {
                revLocation.at(left) = 1;
                adjLoc.at(left) = adjLocation.at(left);
            }
        }
        //topL
        topL = xLoc - 1 + ((yLoc - 1) * column);
        if (revLocation.at(topL) != 1 && flagLocation.at(topL) != 1) {
            if (adjLocation.at(topL) == 0 && revLocation.at(topL) != 1) {              //topL
                adjacentReveal(flagLocation, revLocation, adjLocation, adjLoc, cfg, topL);
            }
            else {
                revLocation.at(topL) = 1;
                adjLoc.at(topL) = adjLocation.at(topL);
            }
        }

        //botL
        botL = xLoc - 1 + ((yLoc + 1) * column);
        if (revLocation.at(botL) != 1 && flagLocation.at(botL) != 1) {
            if (adjLocation.at(botL) == 0 && revLocation.at(botL) != 1) {              //botL
                adjacentReveal(flagLocation, revLocation, adjLocation, adjLoc, cfg, botL);
            }
            else {
                revLocation.at(botL) = 1;
                adjLoc.at(botL) = adjLocation.at(botL);
            }
        }
    }
    else if (yLoc == 0 && xLoc != 0 && xLoc != cfg.at(0) - 1) {
        //bottom
        bottom = xLoc + ((yLoc + 1) * column);
        if (revLocation.at(bottom) != 1 && flagLocation.at(bottom) != 1) {
            if (adjLocation.at(bottom) == 0 && revLocation.at(bottom) != 1) {              //bottom
                adjacentReveal(flagLocation, revLocation, adjLocation, adjLoc, cfg, bottom);
            }
            else {
                revLocation.at(bottom) = 1;
                adjLoc.at(bottom) = adjLocation.at(bottom);
            }
        }

        //right
        right = xLoc + 1 + (yLoc * column);
        if (revLocation.at(right) != 1 && flagLocation.at(right) != 1) {
            if (adjLocation.at(right) == 0 && revLocation.at(right) != 1) {              //right
                adjacentReveal(flagLocation, revLocation, adjLocation, adjLoc, cfg, right);
            }
            else {
                revLocation.at(right) = 1;
                adjLoc.at(right) = adjLocation.at(right);
            }
        }

        //left 
        left = xLoc - 1 + (yLoc * column);
        if (revLocation.at(left) != 1 && flagLocation.at(left) != 1) {
            if (adjLocation.at(left) == 0 && revLocation.at(left) != 1) {              //left
                adjacentReveal(flagLocation, revLocation, adjLocation, adjLoc, cfg, left);
            }
            else {
                revLocation.at(left) = 1;
                adjLoc.at(left) = adjLocation.at(left);
            }
        }
        //botL
        botL = xLoc - 1 + ((yLoc + 1) * column);
        if (revLocation.at(botL) != 1 && flagLocation.at(botL) != 1) {
            if (adjLocation.at(botL) == 0 && revLocation.at(botL) != 1) {              //botL
                adjacentReveal(flagLocation, revLocation, adjLocation, adjLoc, cfg, botL);
            }
            else {
                revLocation.at(botL) = 1;
                adjLoc.at(botL) = adjLocation.at(botL);
            }
        }

        //botR
        botR = xLoc + 1 + ((yLoc + 1) * column);
        if (revLocation.at(botR) != 1 && flagLocation.at(botR) != 1) {
            if (adjLocation.at(botR) == 0 && revLocation.at(botR) != 1) {              //botR
                adjacentReveal(flagLocation, revLocation, adjLocation, adjLoc, cfg, botR);
            }
            else {
                revLocation.at(botR) = 1;
                adjLoc.at(botR) = adjLocation.at(botR);
            }
        }
    }
    else if (yLoc == cfg.at(1) - 1 && xLoc != 0 && xLoc != cfg.at(0) - 1) {
        //top
        top = xLoc + ((yLoc - 1) * column);
        if (revLocation.at(top) != 1 && flagLocation.at(top) != 1) {
            if (adjLocation.at(top) == 0 && revLocation.at(top) != 1) {              //top
                adjacentReveal(flagLocation, revLocation, adjLocation, adjLoc, cfg, top);
            }
            else {
                revLocation.at(top) = 1;
                adjLoc.at(top) = adjLocation.at(top);
            }
        }


        //right
        right = xLoc + 1 + (yLoc * column);
        if (revLocation.at(right) != 1 && flagLocation.at(right) != 1) {
            if (adjLocation.at(right) == 0 && revLocation.at(right) != 1) {              //right
                adjacentReveal(flagLocation, revLocation, adjLocation, adjLoc, cfg, right);
            }
            else {
                revLocation.at(right) = 1;
                adjLoc.at(right) = adjLocation.at(right);
            }
        }

        //left 
        left = xLoc - 1 + (yLoc * column);
        if (revLocation.at(left) != 1 && flagLocation.at(left) != 1) {
            if (adjLocation.at(left) == 0 && revLocation.at(left) != 1) {              //left
                adjacentReveal(flagLocation, revLocation, adjLocation, adjLoc, cfg, left);
            }
            else {
                revLocation.at(left) = 1;
                adjLoc.at(left) = adjLocation.at(left);
            }
        }

        //topL
        topL = xLoc - 1 + ((yLoc - 1) * column);
        if (revLocation.at(topL) != 1 && flagLocation.at(topL) != 1) {
            if (adjLocation.at(topL) == 0 && revLocation.at(topL) != 1) {              //topL
                adjacentReveal(flagLocation, revLocation, adjLocation, adjLoc, cfg, topL);
            }
            else {
                revLocation.at(topL) = 1;
                adjLoc.at(topL) = adjLocation.at(topL);
            }
        }

        //topR
        topR = xLoc + 1 + ((yLoc - 1) * column);
        if (revLocation.at(topR) != 1 && flagLocation.at(topR) != 1) {
            if (adjLocation.at(topR) == 0 && revLocation.at(topR) != 1) {              //topR
                adjacentReveal(flagLocation, revLocation, adjLocation, adjLoc, cfg, topR);
            }
            else {
                revLocation.at(topR) = 1;
                adjLoc.at(topR) = adjLocation.at(topR);
            }
        }
    }
    else {
        if (xLoc == 0 && yLoc == 0) {                            //top left
            right = xLoc + 1 + (yLoc * column);
            if (revLocation.at(right) != 1 && flagLocation.at(right) != 1) {
                if (adjLocation.at(right) == 0 && revLocation.at(right) != 1) {              //right
                    adjacentReveal(flagLocation, revLocation, adjLocation, adjLoc, cfg, right);
                }
                else {
                    revLocation.at(right) = 1;
                    adjLoc.at(right) = adjLocation.at(right);
                }
            }

            bottom = xLoc + ((yLoc + 1) * column);
            if (revLocation.at(bottom) != 1 && flagLocation.at(bottom) != 1) {
                if (adjLocation.at(bottom) == 0 && revLocation.at(bottom) != 1) {              //bottom
                    adjacentReveal(flagLocation, revLocation, adjLocation, adjLoc, cfg, bottom);
                }
                else {
                    revLocation.at(bottom) = 1;
                    adjLoc.at(bottom) = adjLocation.at(bottom);
                }
            }

            botR = xLoc + 1 + ((yLoc + 1) * column);
            if (revLocation.at(botR) != 1 && flagLocation.at(botR) != 1) {
                if (adjLocation.at(botR) == 0 && revLocation.at(botR) != 1) {              //botR
                    adjacentReveal(flagLocation, revLocation, adjLocation, adjLoc, cfg, botR);
                }
                else {
                    revLocation.at(botR) = 1;
                    adjLoc.at(botR) = adjLocation.at(botR);
                }
            }
        }
        else if (xLoc == 0 && yLoc == cfg.at(1) - 1) {           //bottom left
            right = xLoc + 1 + (yLoc * column);
            if (revLocation.at(right) != 1 && flagLocation.at(right) != 1) {
                if (adjLocation.at(right) == 0 && revLocation.at(right) != 1) {              //right
                    adjacentReveal(flagLocation, revLocation, adjLocation, adjLoc, cfg, right);
                }
                else {
                    revLocation.at(right) = 1;
                    adjLoc.at(right) = adjLocation.at(right);
                }
            }

            top = xLoc + ((yLoc - 1) * column);
            if (revLocation.at(top) != 1 && flagLocation.at(top) != 1) {
                if (adjLocation.at(top) == 0 && revLocation.at(top) != 1) {              //top
                    adjacentReveal(flagLocation, revLocation, adjLocation, adjLoc, cfg, top);
                }
                else {
                    revLocation.at(top) = 1;
                    adjLoc.at(top) = adjLocation.at(top);
                }
            }

            topR = xLoc + 1 + ((yLoc - 1) * column);
            if (revLocation.at(topR) && flagLocation.at(topR) != 1) {
                if (adjLocation.at(topR) == 0 && revLocation.at(topR) != 1) {              //topR
                    adjacentReveal(flagLocation, revLocation, adjLocation, adjLoc, cfg, topR);
                }
                else {
                    revLocation.at(topR) = 1;
                    adjLoc.at(topR) = adjLocation.at(topR);
                }
            }
        }
        else if (xLoc == cfg.at(0) - 1 && yLoc == 0) {           //top right
            left = xLoc - 1 + (yLoc * column);
            if (revLocation.at(left) != 1 && flagLocation.at(left) != 1) {
                if (adjLocation.at(left) == 0 && revLocation.at(left) != 1) {              //left
                    adjacentReveal(flagLocation, revLocation, adjLocation, adjLoc, cfg, left);
                }
                else {
                    revLocation.at(left) = 1;
                    adjLoc.at(left) = adjLocation.at(left);
                }
            }

            bottom = xLoc + ((yLoc + 1) * column);
            if (revLocation.at(bottom) != 1 && flagLocation.at(bottom) != 1) {
                if (adjLocation.at(bottom) == 0 && revLocation.at(bottom) != 1) {              //bottom
                    adjacentReveal(flagLocation, revLocation, adjLocation, adjLoc, cfg, bottom);
                }
                else {
                    revLocation.at(bottom) = 1;
                    adjLoc.at(bottom) = adjLocation.at(bottom);
                }
            }

            botL = xLoc - 1 + ((yLoc + 1) * column);
            if (revLocation.at(botL) != 1 && flagLocation.at(botL) != 1) {
                if (adjLocation.at(botL) == 0 && revLocation.at(botL) != 1) {              //botL
                    adjacentReveal(flagLocation, revLocation, adjLocation, adjLoc, cfg, botL);
                }
                else {
                    revLocation.at(botL) = 1;
                    adjLoc.at(botL) = adjLocation.at(botL);
                }
            }
        }
        else if (xLoc == cfg.at(0) - 1 && yLoc == cfg.at(1) - 1) {        //bottom right
            left = xLoc - 1 + (yLoc * column);
            if (revLocation.at(left) != 1 && flagLocation.at(left) != 1) {
                if (adjLocation.at(left) == 0 && revLocation.at(left) != 1) {              //left
                    adjacentReveal(flagLocation, revLocation, adjLocation, adjLoc, cfg, left);
                }
                else {
                    revLocation.at(left) = 1;
                    adjLoc.at(left) = adjLocation.at(left);
                }
            }

            top = xLoc + ((yLoc - 1) * column);
            if (revLocation.at(top) != 1 && flagLocation.at(top) != 1) {
                if (adjLocation.at(top) == 0 && revLocation.at(top) != 1) {              //top
                    adjacentReveal(flagLocation, revLocation, adjLocation, adjLoc, cfg, top);
                }
                else {
                    revLocation.at(top) = 1;
                    adjLoc.at(top) = adjLocation.at(top);
                }
            }

            topL = xLoc - 1 + ((yLoc - 1) * column);
            if (revLocation.at(topL) != 1 && flagLocation.at(topL) != 1) {
                if (adjLocation.at(topL) == 0 && revLocation.at(topL) != 1) {              //topL
                    adjacentReveal(flagLocation, revLocation, adjLocation, adjLoc, cfg, topL);
                }
                else {
                    revLocation.at(topL) = 1;
                    adjLoc.at(topL) = adjLocation.at(topL);
                }
            }
        }
    }
}

int main()
{
    vector<int> cfg = readFile_cfg("config");
    vector<Sprite> tiles;
    vector<Sprite> mines;
    vector<Sprite> flags;
    vector<Sprite> adjacents;
    vector<int> mineLocation;
    vector<int> flagLocation;
    vector<int> revLocation;
    vector<int> adjLocation;
    vector<int> adjLocationV2;       //this one will have the already complete map and is used for 
    int mineCount = 0;
    int flagCount = 0;
    

    sf::RenderWindow window(sf::VideoMode(cfg.at(0) * 32, cfg.at(1) * 32 + 88), "Minesweeper");

    //loading sprites
    Sprite test1Button(TextureManager::GetTexture("test_1"));
    Sprite test2Button(TextureManager::GetTexture("test_2"));
    Sprite test3Button(TextureManager::GetTexture("test_3"));
    Sprite debugButton(TextureManager::GetTexture("debug"));
    Sprite tileHidden(TextureManager::GetTexture("tile_hidden"));
    Sprite tileRevealed(TextureManager::GetTexture("tile_revealed"));
    Sprite flag(TextureManager::GetTexture("flag"));
    Sprite mine(TextureManager::GetTexture("mine"));
    Sprite happyFace(TextureManager::GetTexture("face_happy"));
    Sprite sadFace(TextureManager::GetTexture("face_lose"));
    Sprite winFace(TextureManager::GetTexture("face_win"));
    Sprite number1(TextureManager::GetTexture("number_1"));
    Sprite number2(TextureManager::GetTexture("number_2"));
    Sprite number3(TextureManager::GetTexture("number_3"));
    Sprite number4(TextureManager::GetTexture("number_4"));
    Sprite number5(TextureManager::GetTexture("number_5"));
    Sprite number6(TextureManager::GetTexture("number_6"));
    Sprite number7(TextureManager::GetTexture("number_7"));
    Sprite number8(TextureManager::GetTexture("number_8"));
    Sprite d0(TextureManager::GetTexture("digits"));
    Sprite d1(TextureManager::GetTexture("digits"));
    Sprite d2(TextureManager::GetTexture("digits"));
    Sprite d3(TextureManager::GetTexture("digits"));
    Sprite d4(TextureManager::GetTexture("digits"));
    Sprite d5(TextureManager::GetTexture("digits"));
    Sprite d6(TextureManager::GetTexture("digits"));
    Sprite d7(TextureManager::GetTexture("digits"));
    Sprite d8(TextureManager::GetTexture("digits"));
    Sprite d9(TextureManager::GetTexture("digits"));
    Sprite d10(TextureManager::GetTexture("digits"));

    d0.setTextureRect(IntRect(0, 0, 21, 32));
    d1.setTextureRect(IntRect(21, 0, 21, 32));
    d2.setTextureRect(IntRect(42, 0, 21, 32));
    d3.setTextureRect(IntRect(63, 0, 21, 32));
    d4.setTextureRect(IntRect(84, 0, 21, 32));
    d5.setTextureRect(IntRect(105, 0, 21, 32));
    d6.setTextureRect(IntRect(126, 0, 21, 32));
    d7.setTextureRect(IntRect(147, 0, 21, 32));
    d8.setTextureRect(IntRect(168, 0, 21, 32));
    d9.setTextureRect(IntRect(189, 0, 21, 32));
    d10.setTextureRect(IntRect(210, 0, 21, 32));

    vector<Sprite> digits;
    digits.push_back(d0);
    digits.push_back(d1);
    digits.push_back(d2);
    digits.push_back(d3);
    digits.push_back(d4);
    digits.push_back(d5);
    digits.push_back(d6);
    digits.push_back(d7);
    digits.push_back(d8);
    digits.push_back(d9);
    digits.push_back(d10);

    Sprite negative = d10;
    Sprite first = d0;
    Sprite second = d0;
    Sprite third = d0;

    vector<Sprite> numbers;
    numbers.push_back(number1);
    numbers.push_back(number2);
    numbers.push_back(number3);
    numbers.push_back(number4);
    numbers.push_back(number5);
    numbers.push_back(number6);
    numbers.push_back(number7);
    numbers.push_back(number8);

    //setting button positions
    test1Button.setPosition(17 * 32, cfg[1] * 32);
    test2Button.setPosition(19 * 32, cfg[1] * 32);
    test3Button.setPosition(21 * 32, cfg[1] * 32);
    debugButton.setPosition(15 * 32, cfg[1] * 32);
    happyFace.setPosition(11 * 32, cfg[1] * 32);
    sadFace.setPosition(11 * 32, cfg[1] * 32);
    winFace.setPosition(11 * 32, cfg[1] * 32);
    Sprite resetButton = happyFace;

    //prep work
    mineLocation = randomlyGenerate(cfg);
    flagLocation = resetVector(cfg);
    revLocation = resetVector(cfg);
    adjLocation = resetVector(cfg);
    adjLocationV2 = resetVector(cfg);
    tiles = makeMap(cfg, revLocation, tileHidden, tileRevealed);
    mines = makeSprites(cfg.at(0), mineLocation, mine);
    adjacents = makeNumbers(cfg.at(0), adjLocation, numbers);
    mineCount = cfg.at(2);

    for (int i = 0; i < mineLocation.size(); i++) {
        adjLocationV2.at(i) = adjacentMines(mineLocation, i % cfg.at(0), i / cfg.at(0), cfg.at(0), cfg.at(1));
    }


    //variables for tracking conditions
    int xLoc = 0;
    int yLoc = 0;

    int firstD = 0;
    int secondD = 0;
    int thirdD = 0;

    bool gameOver = false;
    bool gameWon = false;
    bool debugMode = false;
    bool nOOOOOO = false;

    firstD = (mineCount - flagCount) / 100;
    secondD = (mineCount - flagCount) / 10;
    thirdD = (mineCount - flagCount) % 10;

    if (mineCount - flagCount < 0) {
        secondD = (-1)* (mineCount - flagCount) / 10;
        thirdD = (-1) * (mineCount - flagCount) % 10;
        if (firstD == 10) {
            firstD = 0;
        }
        if (secondD == 10) {
            secondD = 0;
        }
        if (thirdD == 10) {
            thirdD = 0;
        }
        first = digitChanger(digits, 10);
        second = digitChanger(digits, secondD);
        third = digitChanger(digits, thirdD);
    }
    else {
        first = digitChanger(digits, firstD);
        second = digitChanger(digits, secondD);
        third = digitChanger(digits, thirdD);
    }    

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            switch (event.type)
            {
            case Event::Closed:
                window.close();
                break;
            case Event::MouseButtonPressed:
                if (event.mouseButton.button == Mouse::Right) {
                    xLoc = event.mouseButton.x / 32;
                    yLoc = event.mouseButton.y / 32;
                    if (yLoc < cfg.at(1) && gameOver != true && gameWon != true) {
                        if (revLocation.at(xLoc + (yLoc * cfg.at(0))) == 1) {
                            continue;
                        }
                        else {
                            markClick(flagLocation, cfg, xLoc, yLoc, 1);
                        }
                        flags.clear();
                        flags = makeSprites(cfg.at(0), flagLocation, flag);

                        flagCount = checkOnes(flagLocation);
                        mineCount = checkOnes(mineLocation);

                        firstD = (mineCount - flagCount) / 100;
                        secondD = (mineCount - flagCount) / 10;
                        thirdD = (mineCount - flagCount) % 10;

                        if (firstD < 0) {
                            firstD *= -1;
                        }
                        else {
                            nOOOOOO = false;
                        }

                        if (secondD < 0) {
                            secondD *= -1;
                        }
                        if (thirdD < 0) {
                            thirdD *= -1;
                        }

                        if (secondD > 9) {
                            secondD =  secondD % 100;
                            if (secondD > 9) {
                                secondD = secondD % 10;
                            }
                        }

                        first = digitChanger(digits, firstD);
                        second = digitChanger(digits, secondD);
                        third = digitChanger(digits, thirdD);

                        if (mineCount - flagCount < -99){
                            nOOOOOO = true;
                        }
                        else if (mineCount - flagCount < 0) {
                            first = digitChanger(digits, 10);
                        }

                    }
                    else if (gameWon) {
                        first = d0;
                        second = d0;
                        third = d0;
                    }
                }
                if (event.mouseButton.button == Mouse::Left) {
                    xLoc = event.mouseButton.x / 32;
                    yLoc = event.mouseButton.y / 32;
                    if (yLoc < cfg.at(1)) {
                        if (flagLocation.at(xLoc + (yLoc * cfg.at(0))) == 1) {
                            continue;
                        }
                        else if (mineLocation.at(xLoc + (yLoc * cfg.at(0))) == 1) {
                            for (int i = 0; i < mineLocation.size(); i++) {
                                if (mineLocation.at(i) == 1) {
                                    revLocation.at(i) = 1;
                                    flagLocation.at(i) = 0;
                                }
                            }
                            gameOver = true;
                            resetButton = sadFace;
                        }
                        else if (gameOver == true) {

                        }
                        // ABLE TO CLICK TILES OCCURS HERE

                        else if (gameOver != true || gameWon != true) {
                            adjLocation.at(xLoc + (yLoc * cfg.at(0))) = adjacentMines(mineLocation, xLoc, yLoc, cfg.at(0), cfg.at(1));
                            if (adjLocation.at(xLoc + (yLoc * cfg.at(0))) == 0) {
                                adjacentReveal(flagLocation, revLocation, adjLocationV2, adjLocation, cfg, xLoc + (yLoc * cfg.at(0)));
                            }
                            markClick(revLocation, cfg, xLoc, yLoc, 0);
                            if (checkOnes(mineLocation) == cfg.at(0) * cfg.at(1) - checkOnes(revLocation)) {
                                gameWon = true;
                                resetButton = winFace;
                                for (int i = 0; i < revLocation.size(); i++) {
                                    if (revLocation.at(i) == 1) {
                                        flagLocation.at(i) = 0;
                                    }
                                    else {
                                        flagLocation.at(i) = 1;
                                    }
                                }
                                flags = makeSprites(cfg.at(0), flagLocation, flag);
                                first = digitChanger(digits, 0);
                                second = digitChanger(digits, 0);
                                third = digitChanger(digits, 0);
                            }
                        }

                        tiles.clear();
                        adjacents.clear();
                        flags.clear();
                        tiles = makeMap(cfg, revLocation, tileHidden, tileRevealed);
                        adjacents = makeNumbers(cfg.at(0), adjLocation, numbers);
                        flags = makeSprites(cfg.at(0), flagLocation, flag);
                    }
                    else if (yLoc > cfg.at(1) - 1){
                        Vector2i mousePos = Mouse::getPosition(window);
                        auto bounds1 = test1Button.getGlobalBounds();
                        auto bounds2 = test2Button.getGlobalBounds();
                        auto bounds3 = test3Button.getGlobalBounds();
                        auto boundsR = resetButton.getGlobalBounds();
                        auto boundsD = debugButton.getGlobalBounds();
                        if (bounds1.contains(mousePos.x, mousePos.y)) {
                            tiles.clear();
                            mines.clear();
                            adjacents.clear();
                            flags.clear();
                            resetButton = happyFace;

                            mineLocation.clear();
                            mineLocation = readFile_board("testboard1");
                            for (int i = 0; i < mineLocation.size(); i++) {
                                adjLocationV2.at(i) = adjacentMines(mineLocation, i % cfg.at(0), i / cfg.at(0), cfg.at(0), cfg.at(1));
                            }
                            mines = makeSprites(cfg.at(0), mineLocation, mine);
                            gameOver = false;
                            gameWon = false;
                            nOOOOOO = false;

                            //resetting tiles and flags
                            for (int i = 0; i < mineLocation.size(); i++) {
                                revLocation.at(i) = 0;
                            }
                            for (int i = 0; i < mineLocation.size(); i++) {
                                flagLocation.at(i) = 0;
                            }
                            for (int i = 0; i < cfg.at(0) * cfg.at(1); i++) {
                                adjLocation.at(i) = 0;
                            }
                            tiles = makeMap(cfg, revLocation, tileHidden, tileRevealed);
                            flags = makeSprites(cfg.at(0), flagLocation, flag);

                            mineCount = checkOnes(mineLocation);
                            flagCount = checkOnes(flagLocation);
                            
                            firstD = (mineCount - flagCount) / 100;
                            secondD = (mineCount - flagCount) / 10;
                            thirdD = (mineCount - flagCount) % 10;

                            if (secondD > 9) {
                                secondD = secondD % 100;
                                if (secondD > 9) {
                                    secondD = secondD % 10;
                                }
                            }

                            if (mineCount - flagCount < 0) {
                                secondD = (-1) * (mineCount - flagCount) / 10;
                                thirdD = (-1) * (mineCount - flagCount) % 10;
                                first = digitChanger(digits, 10);
                                second = digitChanger(digits, secondD);
                                third = digitChanger(digits, thirdD);
                            }
                            
                            else {
                                first = digitChanger(digits, firstD);
                                second = digitChanger(digits, secondD);
                                third = digitChanger(digits, thirdD);
                            }
                            
                        }
                        else if (bounds2.contains(mousePos.x, mousePos.y)) {
                            tiles.clear();
                            mines.clear();
                            adjacents.clear();
                            flags.clear();
                            resetButton = happyFace;

                            mineLocation.clear();
                            mineLocation = readFile_board("testboard2");
                            for (int i = 0; i < mineLocation.size(); i++) {
                                adjLocationV2.at(i) = adjacentMines(mineLocation, i % cfg.at(0), i / cfg.at(0), cfg.at(0), cfg.at(1));
                            }
                            mines = makeSprites(cfg.at(0), mineLocation, mine);
                            gameOver = false;
                            gameWon = false;
                            nOOOOOO = false;

                            //resetting tiles and flags
                            for (int i = 0; i < mineLocation.size(); i++) {
                                revLocation.at(i) = 0;
                            }
                            for (int i = 0; i < mineLocation.size(); i++) {
                                flagLocation.at(i) = 0;
                            }
                            for (int i = 0; i < cfg.at(0) * cfg.at(1); i++) {
                                adjLocation.at(i) = 0;
                            }
                            tiles = makeMap(cfg, revLocation, tileHidden, tileRevealed);
                            flags = makeSprites(cfg.at(0), flagLocation, flag);

                            mineCount = checkOnes(mineLocation);
                            flagCount = checkOnes(flagLocation);

                            firstD = (mineCount - flagCount) / 100;
                            secondD = (mineCount - flagCount) / 10;
                            thirdD = (mineCount - flagCount) % 10;

                            if (secondD > 9) {
                                secondD = secondD % 100;
                                if (secondD > 9) {
                                    secondD = secondD % 10;
                                }
                            }

                            if (mineCount - flagCount < 0) {
                                secondD = (-1) * (mineCount - flagCount) / 10;
                                thirdD = (-1) * (mineCount - flagCount) % 10;
                                first = digitChanger(digits, 10);
                                second = digitChanger(digits, secondD);
                                third = digitChanger(digits, thirdD);
                            }
                            else {
                                first = digitChanger(digits, firstD);
                                second = digitChanger(digits, secondD);
                                third = digitChanger(digits, thirdD);
                            }
                        }
                        else if (bounds3.contains(mousePos.x, mousePos.y)) {
                            tiles.clear();
                            mines.clear();
                            adjacents.clear();
                            flags.clear();
                            resetButton = happyFace;

                            mineLocation.clear();
                            mineLocation = readFile_board("testboard3");
                            for (int i = 0; i < mineLocation.size(); i++) {
                                adjLocationV2.at(i) = adjacentMines(mineLocation, i % cfg.at(0), i / cfg.at(0), cfg.at(0), cfg.at(1));
                            }
                            mines = makeSprites(cfg.at(0), mineLocation, mine);
                            gameOver = false;
                            gameWon = false;
                            nOOOOOO = false;

                            //resetting tiles and flags
                            for (int i = 0; i < mineLocation.size(); i++) {
                                revLocation.at(i) = 0;
                            }
                            for (int i = 0; i < mineLocation.size(); i++) {
                                flagLocation.at(i) = 0;
                            }
                            for (int i = 0; i < cfg.at(0) * cfg.at(1); i++) {
                                adjLocation.at(i) = 0;
                            }
                            tiles = makeMap(cfg, revLocation, tileHidden, tileRevealed);
                            flags = makeSprites(cfg.at(0), flagLocation, flag);

                            mineCount = checkOnes(mineLocation);
                            flagCount = checkOnes(flagLocation);

                            firstD = (mineCount - flagCount) / 100;
                            secondD = (mineCount - flagCount) / 10;
                            thirdD = (mineCount - flagCount) % 10;

                            if (secondD > 9) {
                                secondD = secondD % 100;
                                if (secondD > 9) {
                                    secondD = secondD % 10;
                                }
                            }

                            if (mineCount - flagCount < 0) {
                                secondD = (-1) * (mineCount - flagCount) / 10;
                                thirdD = (-1) * (mineCount - flagCount) % 10;
                                first = digitChanger(digits, 10);
                                second = digitChanger(digits, secondD);
                                third = digitChanger(digits, thirdD);
                            }
                            else {
                                first = digitChanger(digits, firstD);
                                second = digitChanger(digits, secondD);
                                third = digitChanger(digits, thirdD);
                            }
                        }
                        else if (boundsR.contains(mousePos.x, mousePos.y)) {
                            tiles.clear();
                            mines.clear();
                            flags.clear();
                            adjacents.clear();
                            resetButton = happyFace;

                            mineLocation = randomlyGenerate(cfg);
                            for (int i = 0; i < mineLocation.size(); i++) {
                                adjLocationV2.at(i) = adjacentMines(mineLocation, i % cfg.at(0), i / cfg.at(0), cfg.at(0), cfg.at(1));
                            }
                            mines = makeSprites(cfg.at(0), mineLocation, mine);
                            gameOver = false;
                            gameWon = false;
                            nOOOOOO = false;

                            //resetting tiles and flags
                            for (int i = 0; i < mineLocation.size(); i++) {
                                revLocation.at(i) = 0;
                            }
                            for (int i = 0; i < mineLocation.size(); i++) {
                                flagLocation.at(i) = 0;
                            }
                            for (int i = 0; i < cfg.at(0) * cfg.at(1); i++) {
                                adjLocation.at(i) = 0;
                            }
                            tiles = makeMap(cfg, revLocation, tileHidden, tileRevealed);
                            flags = makeSprites(cfg.at(0), flagLocation, flag);

                            mineCount = checkOnes(mineLocation);
                            flagCount = checkOnes(flagLocation);

                            firstD = (mineCount - flagCount) / 100;
                            secondD = (mineCount - flagCount) / 10;
                            thirdD = (mineCount - flagCount) % 10;


                            if (mineCount - flagCount < 0) {
                                secondD = (-1) * (mineCount - flagCount) / 10;
                                thirdD = (-1) * (mineCount - flagCount) % 10;
                                first = digitChanger(digits, 10);
                                second = digitChanger(digits, secondD);
                                third = digitChanger(digits, thirdD);
                            }
                            else {
                                first = digitChanger(digits, firstD);
                                second = digitChanger(digits, secondD);
                                third = digitChanger(digits, thirdD);
                            }

                        }
                        else if (boundsD.contains(mousePos.x, mousePos.y)) {
                            if (debugMode == true) {
                                debugMode = false;
                            }
                            else {
                                debugMode = true;
                            }
                        }
                    }
                }
            }
        }
        window.clear(Color::White);

        for (int i = 0; i < tiles.size(); i++) {
            window.draw(tiles.at(i));
        }
        for (int i = 0; i < adjacents.size(); i++) {
            window.draw(adjacents.at(i));
        }
        for (int i = 0; i < flags.size(); i++) {
            window.draw(flags.at(i));
        }
        if (debugMode == true || gameOver == true) {
            for (int i = 0; i < mines.size(); i++) {
                window.draw(mines.at(i));
            }
        }

        negative.setPosition(0 * 32, cfg[1] * 32);
        first.setPosition(1 * 21, cfg[1] * 32);
        second.setPosition(2 * 21, cfg[1] * 32);
        third.setPosition(3 * 21, cfg[1] * 32);

        window.draw(resetButton);
        window.draw(debugButton);
        window.draw(test3Button);
        window.draw(test2Button);
        window.draw(test1Button);   

        if (nOOOOOO == true) {
            window.draw(negative);
        }
        window.draw(first);
        window.draw(second);
        window.draw(third);
       
        window.display();
    }

    TextureManager::Clear();
    return 0;
}