//
// Created by Daniel Schlatter on 10/26/2018.
//

#ifndef BATTLESHIP_SHIP_H
#define BATTLESHIP_SHIP_H
#include <string>
using namespace std;
class Ship{//Class for each ship to hold various info about them for access in grid.cpp
    private:
        int length;//holds ships length
        string position;//holds the grid coordinate
        string orientation;//holds the orientation
        string name;//holds the name of the ship
        string abbreviation;//holds the abbreviation for easy access when printing
        bool sunk;//bool to hold whether or not ship has been sunk
    public:
        Ship();//default constructor
        Ship(string myName);//constructor
        string getPosition();//getter for position
        int getLength();//getter for length
        string getOrientation();//getter for orientation
        string getName();//getter for name
        string getAbbrev();//getter for abbreviation
        void setPosition(string myPosition);//setter for position
        void setOrientation(string myOrientation);//setter for orientation
        void setSunk();//setter for sunk
        bool getSunk();//getter for sunk

};


#endif //BATTLESHIP_SHIP_H
