//
// Created by djsch on 10/26/2018.
//

#include "GridSquare.h"
GridSquare::GridSquare(string myCoord){//constructor
    hit = false;
    shipName = "";
    shipType = 0;
    coordinate = move(myCoord);
}
GridSquare::GridSquare() = default;//default constructor


string GridSquare::getShipName(){//getter for ship name
    return shipName;
}
void GridSquare::setShipName(string newName){//setter for ship name
    shipName = move(newName);//move used since value is only entered and copied once
}
bool GridSquare::getHit(){//getter for hit
    return hit;
}
void GridSquare::setHit(){//setter for hit
    hit = true;
}
string GridSquare::getCoordinate(){//getter for coordinate
    return coordinate;
}
int GridSquare::getShipType(){//getter for the ship type
    return shipType;
}
void GridSquare::setShipType(int myShipType){//setter for ship type
    shipType = myShipType;
}