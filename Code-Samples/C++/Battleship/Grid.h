//
// Created by Daniel Schlatter on 10/25/2018.
//

#ifndef BATTLESHIP_GRID_H
#define BATTLESHIP_GRID_H
#include <vector>
#include <string>
#include <fstream>
#include <array>
#include "Ship.h"
#include "GridSquare.h"
using namespace std;
class Grid: public GridSquare{//class for the whole grid, child of gridsquare because it has the 2d array of gridsquares
private:
    int width;// x dimension of the grid
    int height;// y dimension of the grid
    vector<Ship> shipsVect;//vector holding the ships which in turn hold all the info needed for the functions
    GridSquare** grid;//2d array holding all the gridSquares
public:
    Grid(int x, int y);//constructor
    void printGrid(bool displayShips);//prints the grid, displaySHips decide if ships should be displayed to the console or not
    bool checkIfLost();//function to check if all the ships have been sunk
    void placeComputerShips();//places all the computers ships randomly on its grid
    bool placeShipRandomly(vector<int>& ints, int shipToPlace);//function to place a ship somewhere random on the grid, takes a vector of ints containing which coordinates have not been tried yet, and the type of ship to be placed
    /* Randomspace holds the starting position for the following functions, and shipToPlace holds the type of ship that is needing to be placed*/
    bool checkToRight(int randomSpace, int shipToPlace);//helper function for placeShipRandomly, checks to see if all the squares to the left that would be occupied by the ship are unoccupied
    bool checkToLeft(int randomSpace, int shipToPlace);//helper function for placeShipRandomly, checks to see if all the squares to the right that would be occupied by the ship are unoccupied
    bool checkAbove(int randomSpace, int shipToPlace);//helper function for placeShipRandomly, checks to see if all the squares above that would be occupied by the ship are unoccupied
    bool checkBelow(int randomSpace, int shipToPlace);//helper function for placeShipRandomly, checks to see if all the squares below that would be occupied by the ship are unoccupied
    void placePlayerShips(ifstream& infile);//takes a data file of inputs and places the players ships according to the data file. if data on a particular is in invalid format, that particular ship is placed in a random location on the grid
    void computerTurn();//function to simulate the computers turn
    bool playerTurn();//function to simulate the players turn
    void quitGame();//sinks all the players ships in the event that they choose to quit

};


#endif //BATTLESHIP_GRID_H
