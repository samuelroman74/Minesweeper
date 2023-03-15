#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include "TextureManager.h"
#include "Random.h"
using namespace std;


int main()
{



    string tempNumColumns = "0";
    string tempNumRows = "0";
    string tempNumMines = "0";
    //Temporary variables to read info

    ifstream file;
    file.open("boards//config.cfg");

    getline(file, tempNumColumns);
    getline(file, tempNumRows);
    getline(file, tempNumMines);

    file.close();

    int numColumns = stoi(tempNumColumns);
    int numRows = stoi(tempNumRows);
    int numMines = stoi(tempNumMines);
    //Opens boards, and reads necessary info

    int width = numColumns * 32;
    int height = (numRows * 32) + 100;
    int tileCount = numColumns * numRows;
    int mineToggle = 0;
    //Creates necessary variables. 

    sf::RenderWindow window(sf::VideoMode(width, height), "SFML works!");
    //Makes window

    vector<vector<sf::Sprite>> tiles(numRows, vector<sf::Sprite>(numColumns));
    vector<vector<int>> mineChecker(numRows, vector<int>(numColumns));
    vector<vector<sf::Sprite>> extras(numRows, vector<sf::Sprite>(numColumns));
    vector<vector<sf::Sprite>> flags(numRows, vector<sf::Sprite>(numColumns));
    vector<vector<int>> flagChecker(numRows, vector<int>(numColumns));
    //tiles, extras, and flags are vectors that contain vectors of sprites to be displayed.
    //Mine checker keeps track of numbers, flag checker keeps track of flags


    sf::Sprite hiddenTile(TextureManager::GetTexture("tile_hidden"));
    sf::Sprite mine(TextureManager::GetTexture("mine"));
    sf::Sprite flagImage(TextureManager::GetTexture("flag"));
    sf::Sprite face;
    sf::Sprite number1(TextureManager::GetTexture("digits"));
    sf::Sprite number2(TextureManager::GetTexture("digits"));
    sf::Sprite number3(TextureManager::GetTexture("digits"));
    sf::Sprite sign(TextureManager::GetTexture("digits"));
    sf::Sprite reveal(TextureManager::GetTexture("tile_revealed"));



    for (int i = 0; i < numRows; i++) {

        for (int j = 0; j < numColumns; j++) {

            hiddenTile.setPosition((j * 32), (i * 32));
            tiles[i][j] = hiddenTile;


        }

        tiles.push_back(vector<sf::Sprite>());
    }
    //Creates vector for initial board


    int prevNumMines = numMines;
    int gameNotOver = 1;

    for (int i = 0; i < numMines; i++) {
            
        sf::Vector2i pos;
        pos.x = Random::Int(16, (numColumns * 32));
        pos.y = Random::Int(16, (numRows * 32));

        int xValue = ((pos.x + 16) / 32) - 1;
        int yValue = ((pos.y + 16) / 32) - 1;

        if (mineChecker[yValue][xValue] == 9) {
            numMines++;
        }
        else {
            mineChecker[yValue][xValue] = 9;
        }

    }

    numMines = prevNumMines;

  
    //Creates appropriate number of mines



    for (int i = 0; i < numRows; i++) {
        for (int j = 0; j < numColumns; j++) {

            int mineCounter = 0;


            if (mineChecker[i][j] == 0) {
                int k = 0;
                int l = 0;
                int m = i + 1;
                int n = j + 1;

                if (i != 0)
                    k = i - 1;
                if (j != 0)
                    l = j - 1;
                if (i == numRows - 1)
                    m = numRows - 1;
                if (j == numColumns - 1)
                    n = numColumns - 1;


                while (k <= m) {
                    while (l <= n) {
                        if (mineChecker[k][l] == 9)
                            mineCounter++;
                        l++;
                    }
                    k++;
                    if (j != 0)
                        l = j - 1;
                    else
                        l = 0;
                }
                //Sets up mini loops that check surroundings to see how many mines are next to a given space
                
                mineChecker[i][j] = mineCounter;
            }
                

        }
    }
    //Sets up all the numbers

    while (window.isOpen()) {

        sf::Event event;

        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear();



       for (int i = 0; i < numRows; i++) {
            for (int j = 0; j < numColumns; j++) {
               window.draw(tiles[i][j]);
               window.draw(flags[i][j]);
               window.draw(extras[i][j]);
           }

        }
       //Prints board and revealed tiles
 



            sf::Sprite button1(TextureManager::GetTexture("test_1"));
            button1.setPosition(((numColumns / 2) * 32) + 192, numRows * 32);
            window.draw(button1);
        
            sf::Sprite button2(TextureManager::GetTexture("test_2"));
            button2.setPosition(((numColumns / 2) * 32) + 256, numRows * 32);
            window.draw(button2);

            sf::Sprite button3(TextureManager::GetTexture("test_3"));
            button3.setPosition(((numColumns / 2) * 32) + 320, numRows * 32);
            window.draw(button3);

            sf::Sprite button4(TextureManager::GetTexture("debug"));
            button4.setPosition(((numColumns/2) * 32) + 128, numRows * 32);
            window.draw(button4);
            //Draws debug buttons MUST BE CHANGED



            if (gameNotOver == 1) {
                face.setTexture(TextureManager::GetTexture("face_happy"));
            }


            //Sets up face depending on game over conditions (0 = win, 1 = game still going, 2 = loss)


            face.setPosition((numColumns / 2) * 32, numRows * 32);
            window.draw(face);
            

            int flagCounter = 0;
            string stringMine;
            numMines = 0;
            for (int i = 0; i < numRows; i++) {
                for (int j = 0; j < numColumns; j++) {
                    if (flagChecker[i][j] == 1)
                        flagCounter++;
                    if (mineChecker[i][j] == 9)
                        numMines++;
                }
            }

            if (gameNotOver == 0) {
                stringMine = "000";
            }
            else if (abs(numMines - flagCounter) < 100 && abs(numMines - flagCounter) >= 10)
                stringMine = "0" + to_string(abs(numMines - flagCounter));
            else if (abs(numMines - flagCounter < 10))
                stringMine = "00" + to_string(abs(numMines - flagCounter));
            else
                stringMine = to_string(abs(numMines - flagCounter));



            int num1 = (stringMine[0] - 48);
            int num2 = (stringMine[1] - 48);
            int num3 = (stringMine[2] - 48);
            number1.setTextureRect(sf::IntRect(21 * num1, 0, 21, 32));
            number2.setTextureRect(sf::IntRect(21 * num2, 0, 21, 32));
            number3.setTextureRect(sf::IntRect(21 * num3, 0, 21, 32));


            number1.setPosition(21, numRows * 32);
            number2.setPosition(42, numRows * 32);
            number3.setPosition(63, numRows * 32);
            window.draw(number1);
            window.draw(number2);
            window.draw(number3);

          if ((numMines - flagCounter) < 0) {
              sign.setTextureRect(sf::IntRect((21 * 10), 0, 21, 32));
              sign.setPosition(0, numRows * 32);
              window.draw(sign);
          }

          numMines = prevNumMines;

            auto pos = sf::Mouse::getPosition(window);
            int xValue = pos.x / 32;
            int yValue = pos.y / 32;

            if (event.type == sf::Event::MouseButtonPressed) {

                if (event.mouseButton.button == sf::Mouse::Left) {


                    if (yValue < numRows && flagChecker[yValue][xValue] != 1 && gameNotOver == 1) {

                        sf::Sprite reveal(TextureManager::GetTexture("tile_revealed"));

                        reveal.setPosition((xValue * 32), (yValue * 32));
                        tiles[yValue][xValue].setTexture(TextureManager::GetTexture("tile_revealed"));
                        //If the click is in valid range and there is no flag there (flag placement marked
                        //by flagChecker value being 1), reveals tile
                        
                        if (extras[yValue][xValue].getTexture() == hiddenTile.getTexture() && mineChecker[yValue][xValue] == 0) {
                            extras[yValue][xValue] = reveal;
                        }

                        if (flags[yValue][xValue].getTexture() == hiddenTile.getTexture()) {
                            flags[yValue][xValue] = reveal;
                        }



                        if (mineChecker[yValue][xValue] == 9) {
                            mine.setPosition((xValue * 32), (yValue * 32));
                            extras[yValue][xValue] = mine;


                            for (int i = 0; i < numRows; i++) {
                                for (int j = 0; j < numColumns; j++) {

                                    if (mineChecker[i][j] == 9) {
                                        mine.setPosition((j * 32), (i * 32));
                                        extras[i][j] = mine;
                                    }


                                }
                            }

                            gameNotOver = 2;

                        }
                        //If a mine is there (mineChecker = 9), displays all mines and signals game over.



                        else if (mineChecker[yValue][xValue] != 0) {
                            string adjacentImage = "number_" + to_string(mineChecker[yValue][xValue]);
                            sf::Sprite adjacent(TextureManager::GetTexture(adjacentImage));
                            adjacent.setPosition((xValue * 32), (yValue * 32));
                            extras[yValue][xValue] = adjacent;
                        }
                        //If there is a number there, displays it
                        else if (mineChecker[yValue][xValue] == 0) {

                            vector<vector<int>> zeroSpaces(numRows, vector<int>(numColumns));


                            

                            int noMoreEmptySpaces = 1;
                            zeroSpaces[yValue][xValue] = 1;


                            while (noMoreEmptySpaces != 0) {
                                noMoreEmptySpaces = 0;
                                for (int a = 0; a < numRows; a++) {
                                    for (int b = 0; b < numColumns; b++) {
                                        if (zeroSpaces[a][b] == 1) {
                                            noMoreEmptySpaces++;
                                            yValue = a;
                                            xValue = b;
                                        }

                                    }
                                }

                                if (noMoreEmptySpaces != 0) {
                                    int i = 0;
                                    int j = 0;
                                    int k = xValue + 1;
                                    int l = yValue + 1;

                                    if (xValue != 0)
                                        i = xValue - 1;
                                    if (yValue != 0)
                                        j = yValue - 1;
                                    if (xValue == numColumns - 1)
                                        k = numColumns - 1;
                                    if (yValue == numRows - 1)
                                        l = numRows - 1;

                                    zeroSpaces[yValue][xValue] = 2;

                                    while (j <= l) {
                                        while (i <= k) {
                                            reveal.setPosition((i * 32), (j * 32));
                                            if (flagChecker[j][i] != 1) {
                                                tiles[j][i] = reveal;
                                                flagChecker[j][i] = 2;

                                                if (mineChecker[j][i] == 0) {
                                                    if (extras[yValue][xValue].getTexture() == hiddenTile.getTexture()) {
                                                        extras[yValue][xValue] = reveal;
                                                    }

                                                    if (flags[yValue][xValue].getTexture() == hiddenTile.getTexture()) {
                                                        flags[yValue][xValue] = reveal;
                                                    }

                                                    if (zeroSpaces[j][i] == 0)
                                                        zeroSpaces[j][i] = 1;
                                                }
                                                else {
                                                    string adjacentImage = "number_" + to_string(mineChecker[j][i]);
                                                    sf::Sprite adjacent(TextureManager::GetTexture(adjacentImage));
                                                    adjacent.setPosition((i * 32), (j * 32));
                                                    extras[j][i] = adjacent;
                                                }
                                            }
                                            i++;
                                        }
                                        if (xValue != 0)
                                            i = xValue - 1;
                                        else
                                            i = 0;
                                        j++;
                                    }
                                }
                            }


                        }

                        //Sets mines if there is a mine in clicked location. Shows numbers if not

                        flagChecker[yValue][xValue] = 2;
                        //Sets the fact that flags can no longer be placed in revealed spot

                    }

                    //if you click on the board



                    if (face.getGlobalBounds().contains(pos.x, pos.y)) {
                        event.type = event.MouseButtonReleased;

                        for (int i = 0; i < numRows; i++) {
                            for (int j = 0; j < numColumns; j++) {
                                flags[i][j] = hiddenTile;
                                flagChecker[i][j] = 0;
                                extras[i][j] = hiddenTile;
                                mineChecker[i][j] = 0;
                                tiles[i][j].setTexture(TextureManager::GetTexture("tile_hidden"));
                            }
                        }


                        for (int i = 0; i < numRows; i++) {

                            for (int j = 0; j < numColumns; j++) {

                                hiddenTile.setPosition((j * 32), (i * 32));
                                tiles[i][j].setTexture(TextureManager::GetTexture("tile_hidden"));


                            }

                            tiles.push_back(vector<sf::Sprite>());
                        }
                        //Creates vector for initial board




                        for (int i = 0; i < numMines; i++) {

                            sf::Vector2i pos;
                            pos.x = Random::Int(16, (numColumns * 32));
                            pos.y = Random::Int(16, (numRows * 32));

                            int xValue = ((pos.x + 16) / 32) - 1;
                            int yValue = ((pos.y + 16) / 32) - 1;


                            if (mineChecker[yValue][xValue] == 9) {
                                numMines++;
                            }
                            else {
                                mineChecker[yValue][xValue] = 9;
                            }


                        }
                        numMines = prevNumMines;
                        //Creates appropriate number of mines


                        for (int i = 0; i < numRows; i++) {
                            for (int j = 0; j < numColumns; j++) {

                                int mineCounter = 0;

                                if (mineChecker[i][j] == 0) {
                                    int k = 0;
                                    int l = 0;
                                    int m = i + 1;
                                    int n = j + 1;

                                    if (i != 0)
                                        k = i - 1;
                                    if (j != 0)
                                        l = j - 1;
                                    if (i == numRows - 1)
                                        m = numRows - 1;
                                    if (j == numColumns - 1)
                                        n = numColumns - 1;


                                    while (k <= m) {
                                        while (l <= n) {
                                            if (mineChecker[k][l] == 9)
                                                mineCounter++;
                                            l++;
                                        }
                                        k++;
                                        if (j != 0)
                                            l = j - 1;
                                        else
                                            l = 0;
                                    }
                                    //Sets up mini loops that check surroundings to see how many mines are next to a given space

                                    mineChecker[i][j] = mineCounter;
                                }


                            }
                        }
                        //Sets up all the numbers

                        if (mineToggle == 1) {
                            for (int i = 0; i < numRows; i++) {
                                for (int j = 0; j < numColumns; j++) {

                                    if (mineChecker[i][j] == 9) {
                                        mine.setPosition((j * 32), (i * 32));
                                        extras[i][j] = mine;
                                    }


                                }
                            }

                        }
                        else
                        mineToggle = 0;

                        gameNotOver = 1;
                    }
                    //if you click the face


                    if (button4.getGlobalBounds().contains(pos.x, pos.y) && gameNotOver == 1) {
                        event.type = event.MouseButtonReleased;
                        if (mineToggle == 0) {
                            for (int i = 0; i < numRows; i++) {
                                for (int j = 0; j < numColumns; j++) {

                                    if (mineChecker[i][j] == 9) {
                                        mine.setPosition((j * 32), (i * 32));
                                        extras[i][j] = mine;
                                    }


                                }
                            }
                            mineToggle = 1;
                        }
                        else {

                            for (int i = 0; i < numRows; i++) {
                                for (int j = 0; j < numColumns; j++) {

                                    if (mineChecker[i][j] == 9) {
                                        mine.setPosition((j * 32), (i * 32));
                                        extras[i][j] = hiddenTile;
                                    }


                                }
                            }
                            mineToggle = 0;
                        }
                        

                    }
                    //Debug button

                    if (button1.getGlobalBounds().contains(pos.x, pos.y)) {
                        file.open("boards//testboard1.brd");
                        event.type = event.MouseButtonReleased;

                        for (int i = 0; i < numRows; i++) {
                            for (int j = 0; j < numColumns; j++) {
                                flags[i][j] = hiddenTile;
                                flagChecker[i][j] = 0;
                                extras[i][j] = hiddenTile;
                                mineChecker[i][j] = 0;
                               tiles[i][j].setTexture(TextureManager::GetTexture("tile_hidden"));
                            }
                        }
                        //Resets board


                        for (int i = 0; i < numRows; i++) {
                                string testBoard;
                                getline(file, testBoard);

                                for (int j = 0; j < testBoard.size(); j++) {
                                    int tempNum = (testBoard[j] - 48);
                                    if (tempNum == 1)
                                        tempNum = 9;
                                    mineChecker[i][j] = tempNum;
                                }                           
                        }
                        file.close();
                        //Puts mines in correct spots

                        for (int i = 0; i < numRows; i++) {
                            for (int j = 0; j < numColumns; j++) {

                                int mineCounter = 0;

                                if (mineChecker[i][j] == 0) {
                                    int k = 0;
                                    int l = 0;
                                    int m = i + 1;
                                    int n = j + 1;

                                    if (i != 0)
                                        k = i - 1;
                                    if (j != 0)
                                        l = j - 1;
                                    if (i == numRows - 1)
                                        m = numRows - 1;
                                    if (j == numColumns - 1)
                                        n = numColumns - 1;


                                    while (k <= m) {
                                        while (l <= n) {
                                            if (mineChecker[k][l] == 9)
                                                mineCounter++;
                                            l++;
                                        }
                                        k++;
                                        if (j != 0)
                                            l = j - 1;
                                        else
                                            l = 0;
                                    }
                                    //Sets up mini loops that check surroundings to see how many mines are next to a given space

                                    mineChecker[i][j] = mineCounter;
                                }


                            }
                        }
                        //Puts numbers in correct spots


                        if (mineToggle == 1) {
                            for (int i = 0; i < numRows; i++) {
                                for (int j = 0; j < numColumns; j++) {

                                    if (mineChecker[i][j] == 9) {
                                        mine.setPosition((j * 32), (i * 32));
                                        extras[i][j] = mine;
                                    }


                                }
                            }

                        }
                        else
                            mineToggle = 0;

                        gameNotOver = 1;
                    }
                    //Test 1 Button


                    if (button2.getGlobalBounds().contains(pos.x, pos.y)) {
                        file.open("boards//testboard2.brd");
                        event.type = event.MouseButtonReleased;

                        for (int i = 0; i < numRows; i++) {
                            for (int j = 0; j < numColumns; j++) {
                                flags[i][j] = hiddenTile;
                                flagChecker[i][j] = 0;
                                extras[i][j] = hiddenTile;
                                mineChecker[i][j] = 0;
                                tiles[i][j].setTexture(TextureManager::GetTexture("tile_hidden"));
                            }
                        }
                        //Resets board

                        for (int i = 0; i < numRows; i++) {
                            string testBoard;
                            getline(file, testBoard);

                            for (int j = 0; j < testBoard.size(); j++) {
                                int tempNum = (testBoard[j] - 48);
                                if (tempNum == 1)
                                    tempNum = 9;
                                mineChecker[i][j] = tempNum;
                            }
                        }
                        //Puts mines in correct spots

                        file.close();

                        for (int i = 0; i < numRows; i++) {
                            for (int j = 0; j < numColumns; j++) {

                                int mineCounter = 0;

                                if (mineChecker[i][j] == 0) {
                                    int k = 0;
                                    int l = 0;
                                    int m = i + 1;
                                    int n = j + 1;

                                    if (i != 0)
                                        k = i - 1;
                                    if (j != 0)
                                        l = j - 1;
                                    if (i == numRows - 1)
                                        m = numRows - 1;
                                    if (j == numColumns - 1)
                                        n = numColumns - 1;


                                    while (k <= m) {
                                        while (l <= n) {
                                            if (mineChecker[k][l] == 9)
                                                mineCounter++;
                                            l++;
                                        }
                                        k++;
                                        if (j != 0)
                                            l = j - 1;
                                        else
                                            l = 0;
                                    }
                                    //Sets up mini loops that check surroundings to see how many mines are next to a given space

                                    mineChecker[i][j] = mineCounter;
                                }


                            }
                        }
                        //Puts numbers in correct spots


                        if (mineToggle == 1) {
                            for (int i = 0; i < numRows; i++) {
                                for (int j = 0; j < numColumns; j++) {

                                    if (mineChecker[i][j] == 9) {
                                        mine.setPosition((j * 32), (i * 32));
                                        extras[i][j] = mine;
                                    }


                                }
                            }

                        }
                        else
                            mineToggle = 0;

                        gameNotOver = 1;
                    }
                    //Test 2 Button

                    if (button3.getGlobalBounds().contains(pos.x, pos.y)) {
                        file.open("boards//testboard3.brd");
                        event.type = event.MouseButtonReleased;

                        for (int i = 0; i < numRows; i++) {
                            for (int j = 0; j < numColumns; j++) {
                                flags[i][j] = hiddenTile;
                                flagChecker[i][j] = 0;
                                extras[i][j] = hiddenTile;
                                mineChecker[i][j] = 0;
                                tiles[i][j].setTexture(TextureManager::GetTexture("tile_hidden"));
                            }
                        }
                        //Resets board



                        for (int i = 0; i < numRows; i++) {
                            string testBoard;
                            getline(file, testBoard);

                            for (int j = 0; j < testBoard.size(); j++) {
                                int tempNum = (testBoard[j] - 48);
                                if (tempNum == 1)
                                    tempNum = 9;
                                mineChecker[i][j] = tempNum;
                            }
                        }
                        //Puts mines in correct spots
                        file.close();

                        for (int i = 0; i < numRows; i++) {
                            for (int j = 0; j < numColumns; j++) {

                                int mineCounter = 0;

                                if (mineChecker[i][j] == 0) {
                                    int k = 0;
                                    int l = 0;
                                    int m = i + 1;
                                    int n = j + 1;

                                    if (i != 0)
                                        k = i - 1;
                                    if (j != 0)
                                        l = j - 1;
                                    if (i == numRows - 1)
                                        m = numRows - 1;
                                    if (j == numColumns - 1)
                                        n = numColumns - 1;


                                    while (k <= m) {
                                        while (l <= n) {
                                            if (mineChecker[k][l] == 9)
                                                mineCounter++;
                                            l++;
                                        }
                                        k++;
                                        if (j != 0)
                                            l = j - 1;
                                        else
                                            l = 0;
                                    }
                                    //Sets up mini loops that check surroundings to see how many mines are next to a given space

                                    mineChecker[i][j] = mineCounter;
                                }


                            }
                        }
                        //Puts numbers in correct spots
                        if (mineToggle == 1) {
                            for (int i = 0; i < numRows; i++) {
                                for (int j = 0; j < numColumns; j++) {

                                    if (mineChecker[i][j] == 9) {
                                        mine.setPosition((j * 32), (i * 32));
                                        extras[i][j] = mine;
                                    }


                                }
                            }

                        }
                        else
                            mineToggle = 0;

                        gameNotOver = 1;
                    }
                    //Test 3 button
                }

                if (event.mouseButton.button == sf::Mouse::Right && gameNotOver == 1) {
                    if (yValue < numRows && flagChecker[yValue][xValue] != 2) {
                        flagImage.setPosition((xValue * 32), (yValue * 32));
                        flags[yValue][xValue] = flagImage;
                        if (flagChecker[yValue][xValue] == 1) {
                            flagChecker[yValue][xValue] = 3;
                            
                            if (mineChecker[yValue][xValue] == 0)
                                flags[yValue][xValue] = hiddenTile;
                            else
                                flags[yValue][xValue] = extras[yValue][xValue];
                        }
                        else {
                            flagChecker[yValue][xValue] = 1;
                        }
                        event.type = event.MouseButtonReleased;  
                    }
                    //Places flags where clicked (sets flagchecker to 1), removes already placed flags and replaces with previous sprite
                }
            }

            if (gameNotOver != 2) {
                gameNotOver = 0;
                for (int i = 0; i < numRows; i++) {
                    for (int j = 0; j < numColumns; j++) {
                        if (flagChecker[i][j] != 2 && mineChecker[i][j] != 9) {
                            gameNotOver = 1;
                        }
                    }
                }
            }
            //Allows win condition (if every tile has been revealed)

            if (gameNotOver == 0) {
                face.setTexture(TextureManager::GetTexture("face_win"));
                for (int i = 0; i < numRows; i++) {
                    for (int j = 0; j < numColumns; j++) {
                        if (mineChecker[i][j] == 9) {
                            flagImage.setPosition(j * 32, i * 32);
                            extras[i][j] = flagImage;
                            extras[i][j].setTexture(TextureManager::GetTexture("flag"));
                        }
                    }
                }
            }
            if (gameNotOver == 2)
                face.setTexture(TextureManager::GetTexture("face_lose"));

        window.display();
    }
    return 0;
}