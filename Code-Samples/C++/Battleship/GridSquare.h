//
// Created by djsch on 10/26/2018.
//

#ifndef BATTLESHIP_GRIDSQUARE_H
#define BATTLESHIP_GRIDSQUARE_H

#include <string>
using namespace std;

class GridSquare {//Class for each gridsquare, to hold info for access in grid.cpp
private:
    string coordinate;//holds the coordinate the grid is located at
    bool hit;//holds whether or not this square has been fired on
    string shipName;//empty if it does not contain a ship
    int shipType;
    //type 1 = Destroyer (2 squares)
    //type 2 = Submarine (3 squares)
    //type 3 = Cruiser (3 squares)
    //type 4 = Battleship (4 squares)
    //type 5 = Carrier (5 squares)
public:
    GridSquare(string myCoord);//constructor
    GridSquare();//default constructor
    string getShipName();//getter for ship name
    void setShipName(string newName);//setter for ship name
    bool getHit();//getter for hit
    void setHit();//setter for hit
    string getCoordinate();//getter for coordinate
    int getShipType();//getter for the ship type
    void setShipType(int myShipType);//setter for ship type

};


#endif //BATTLESHIP_GRIDSQUARE_H
