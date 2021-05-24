//
// Created by Daniel Schlatter on 10/26/2018.
//

#include "Ship.h"
Ship::Ship() {//default constructor
    position = "";
    orientation = "";
    name = "";
    abbreviation = "";
    length = 0;
    sunk = false;
}
Ship::Ship(string myName){//constructor
    position = "";
    orientation = "";
    name = move(myName);//move used since value is only entered and copied once
    sunk = false;
    //setting all the non-changing variables (length and abbreviation) of the ships to their values
    if(name == "Carrier"){
        length = 5;
        abbreviation = "Ca";
    }
    else if(name == "Submarine"){
        length = 3;
        abbreviation = "Su";
    }
    else if(name == "Destroyer"){
        length = 2;
        abbreviation = "De";
    }
    else if(name == "Battleship"){
        length = 4;
        abbreviation = "Ba";
    }
    else {
        length = 3;
        abbreviation = "Cr";
    }
}

string Ship::getPosition(){//getter for position
    return position;
}
int Ship::getLength(){//getter for length
    return length;
}
string Ship::getOrientation(){//getter for orientation
    return orientation;
}
string Ship::getName(){//getter for name
    return name;
}
string Ship::getAbbrev(){//getter for abbreviation
    return abbreviation;
}
void Ship::setPosition(string myPosition){//setter for position
    position = move(myPosition);//move used since value is only entered and copied once
}
void Ship::setOrientation(string myOrientation){//setter for orientation
    orientation = move(myOrientation);//move used since value is only entered and copied once
}
void Ship::setSunk(){//setter for sunk
    sunk = true;
}
bool Ship::getSunk(){//getter for sunk
    return sunk;
}