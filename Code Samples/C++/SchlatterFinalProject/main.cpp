//
// Created by Daniel Schlatter on 10/25/2018.
//
#include <iostream>
#include <fstream>
#include <time.h>
#include "Grid.h"


using namespace std;

int main(){
    ifstream infile;
    infile.open("ship_placement.csv");//opening the data input file
    cout << "Welcome to Battleship\n";
    srand(time(0));//seeding for the random numbers generated in grid.cpp
    Grid playerGrid(10,10), computerGrid(10,10);//objects for the players and computers grids, respectively
    playerGrid.placePlayerShips(infile);// using the data file to place the ships
    cout << "Your ships have been successfully placed";
    cout << "\n\nHere is your starting grid: \n\n";
    playerGrid.printGrid(true);//displaying the players grid
    infile.close();
    computerGrid.placeComputerShips();//placing the computers ships
    bool endgame;//boolean to tell us if the game ends before the computer takes its turn or if the player wants to quit
    while(!playerGrid.checkIfLost() && !computerGrid.checkIfLost()){//loop until someone loses
        cout << "\n\nHere is the current state of the computers grid: \n\n";
        computerGrid.printGrid(false);//prints the computers grid with ships hidden
        endgame = computerGrid.playerTurn();// player takes their turn
        if(!endgame && !computerGrid.checkIfLost()){//checking if the players turn returned false but the computer hasnt lost, if so, quit the game
            playerGrid.quitGame();
            break;
        }
        else if (!endgame) break;//check to see if the players turn returned false and the computer has lost
        playerGrid.computerTurn();//computer takes its turn
        playerGrid.printGrid(true);//displays players grid
    }
    if(playerGrid.checkIfLost()) cout << "The computer won! Game over. Here is the computers grid: \n";
    else cout << "You won! Game over. Here is the computers grid: \n";
    computerGrid.printGrid(true);//displays computers grid with ships visible
return 0;
}