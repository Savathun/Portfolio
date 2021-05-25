//
// Created by Daniel Schlatter on 10/25/2018.
//
#include <vector>
#include <string>
#include <sstream>
#include <iostream>
#include "Grid.h"
#include "Ship.h"
#include <fstream>

using namespace std;
Grid::Grid(int x, int y){//constructor, takes grid dimensions as parameters
    grid = new GridSquare*[x];//declares grid as an array of pointers to arrays of GridSquares
    for(int i = 0; i < y; i++)
        grid[i] = new GridSquare[y];//initializes all the spaces in the 2d array to gridSquares
    width = x;//passing the dimensions
    height = y;
    for(int i = 0; i < x; i++){//loops through all the spaces in the grid
        for(int j = 0; j < y; j++){
            string coordinate;
            stringstream str;//using a stringstream to combine chars to set each coordinate on the grid to the A2 type of format
            str << char(i+65) << char(j+49);
            str >> coordinate;//passing the str value to coordinate
            GridSquare temp(coordinate);//initialize the gridsquare in this space to the coordinate value
            grid[i][j] = temp;
        }
    }

    //filling the ships vector with the static information for each one
    Ship* tempShip = new Ship("Destroyer");
    shipsVect.push_back(*tempShip);
    tempShip = new Ship("Submarine");
    shipsVect.push_back(*tempShip);
    tempShip = new Ship("Cruiser");
    shipsVect.push_back(*tempShip);
    tempShip = new Ship("Battleship");
    shipsVect.push_back(*tempShip);
    tempShip = new Ship("Carrier");
    shipsVect.push_back(*tempShip);
}
void Grid::printGrid(bool displayShips) {//prints the grid, displaySHips decide if ships should be displayed to the console or not
    cout << "Legend: X = hit, O = miss, Ca = Carrier, Cr = Cruiser, De = Destroyer, Ba = Battleship, Su = Submarine, S = Sunk\n\n";
    for(int j = -1; j < height; j++){//loop to go through each space
        for(int i = -1; i < width; i++){
            if(j<0 && i ==width-1);//check to prevent the last space on the top row getting a | at the end
            else if(j < 0) cout << "    " << char(i+66);//sets the top row to the letters
            else if (i==-1 && j < 9) cout << j + 1 << " |";//sets the first column to numbers
            else if (i==-1 && j > 8) cout << j + 1 << "|";//adjusted spacing for double digit numbers
            else if(grid[i][j].getHit() && grid[i][j].getShipType() > 0) cout << "X " << shipsVect[grid[i][j].getShipType() - 1].getAbbrev() << "|"; // prints X and the ship abbreviation if this spot has a ship and has been hit
            else if(grid[i][j].getHit() && grid[i][j].getShipType() == 0) cout << " O  |";//prints an O if this spot has been hit but lacks a ship
            else if (grid[i][j].getShipType() > 0 && shipsVect[grid[i][j].getShipType() -1].getSunk()) cout << "S " << shipsVect[grid[i][j].getShipType() - 1].getAbbrev() << "|";//prints an S and a ship abbreviation if this spot has a ship that has been sunk
            else if (displayShips && grid[i][j].getShipType() > 0) cout << " " << shipsVect[grid[i][j].getShipType() - 1].getAbbrev() << " |";//prints ship abbreviation if this spot has a ship and has been untouched, and if we want to display the ships
            else cout << "    |";//prints spaces if this spot is empty
        }
            cout << endl;
            cout << "--|----|----|----|----|----|----|----|----|----|----|-" << endl;//formatting to make grid look nice
        }
    cout << "\n\n";
}
/** Ship Placing Functions **/

void Grid::placeComputerShips(){//places all the computers ships randomly on its grid
    int shipsToPlace = 5;//number of ships needed to still be placed and the spot in the ships vector we are currently on
    vector<int> ints;//holds the spaces not randomly picked yet
    for(int i = 0; i < width*height;i++) ints.push_back(i);
    while (shipsToPlace > 0){//loops till no more ships need to be placed
        if(placeShipRandomly(ints, shipsToPlace)) shipsToPlace--;//if placeShipRandomly is successful then decrement the number of ships needing to be placed
    }
}
/**Places the players ships
 * @param infile data file holding values to be read in
 */
void Grid::placePlayerShips(ifstream& infile){
    string shipName;
    string position;
    string orientation;
    int letter;//holds the letter part of the position
    int number;//holds the number paart of the position
    int count = 0;//holds the number of successful ship placements made
    int place;//holds place im the shipsVect that the current hip is located if its name is valid
    while(getline(infile, shipName, ',')){//loop until no more lines of input, reads first value into ship name
        getline(infile, position, ',');//reads second value into position
        getline(infile, orientation);//reads third value into orientation
        place = -1;//resets place each time so the value doesnt get carried over
        if(shipName.empty()) {//checks if shipName is empty, continues to the next line of input if so
            cout << "Invalid ship name in CSV file. Ship will be randomly placed in grid\n";
            continue;
        }
        for(int i = 0; i < shipsVect.size();i++){//loop to check if shipName is actually a valid name
            if(shipsVect[i].getName() == shipName){
                place = i;//sets place to current index if name is valid
                break;
            }
            else if(i == shipsVect.size() -1){
                cout << "Invalid ship name in CSV file. Ship will be randomly placed in grid\n";
            }
        }
        if(place == -1) continue;//if name is invalid and no index for it was found, continues to next line of input
        if(!shipsVect[place].getPosition().empty()){//checks if this valid ship name is a duplicate of a previously inputted name, continues to next line of input if so
            cout << "Duplicate ship in CSV file. Ship will be randomly placed in grid\n";
            continue;
        }

        if(position.empty() ||position.length() > 3 || position.length() < 2) {//checks if position is either empty or of an invalid length, continues to next line of input if so
            cout << "Invalid position in CSV file. Ship will be randomly placed in grid\n";
            continue;
        }
        letter = position[0] - 65;//converts letter part of input to an int
        number = position[1] - 49;//converts number part of input to an int
        if(position.length() == 3)//if number part is a double digit number, further converts the full double digit number to an int value
            number = (number+1)*10 +  position[2] - 49;
        if(letter > width || letter < 0 || number > height || number < 0){//if either the letter or number is out of bounds then continues to the next line of input
            cout << "Position out of bounds in CSV file. Ship will be randomly placed in grid\n";
            continue;
        }

        if(orientation != "V" && orientation != "H"){//if orientation is an invalid value, continues to next line of input
            cout << "Invalid orientation in CSV file. Ship will be randomly placed in grid\n";
            continue;
        }
        for(int j = 1; j < shipsVect[place].getLength(); j++) {//for loop to check that all the spaces needed for the ship are empty, continues to next line of input if any are not
            if (shipsVect[place].getOrientation() == "V" && grid[letter][number + j].getShipType() > 0){//checking vertically
                cout << "Provided ship placement conflicts with a previously placed ship. Ship will be randomly placed in grid\n";
                continue;
            }
            else if (grid[letter + j][number].getShipType() > 0) {//checking horizontally
                cout << "Provided ship placement conflicts with a previously placed ship. Ship will be randomly placed in grid\n";
                continue;
            }
        }
        //if the line of input makes it to this point, it is a perfectly valid set of input
        shipsVect[place].setPosition(position);//sets the ships position to position
        shipsVect[place].setOrientation(orientation);//sets ships orientation to orientation
        for (int y = 0; y < shipsVect[place].getLength(); y++) {// loop to go through each space in grid needed for the ship and set each shipname and ship type to the correct value
            if (shipsVect[place].getOrientation() == "V") {//if its a vertical ship
                grid[letter][number + y].setShipName(shipsVect[place].getName());
                grid[letter][number + y].setShipType(place + 1);
            }
            else {//if it s a horizontal ship
                grid[letter + y][number].setShipName(shipsVect[place].getName());
                grid[letter + y][number].setShipType(place + 1);
            }
        }

        count++;//increment count, because a valid line of data has been successfully read in to one of the ships
        if(count == 5) break;// if 5 valid lines have been read in, breaks out of loop. no need to check further lines of data since they will be either invalid or duplicate ships
    }
    if(count == 0) cout << "CSV file either contains no valid inputs or is empty. Randomly placing all ships.\n";//if no valid lines of data were found, prints this message in cse the file was also empty and triggered no previous error messages
    if(count < 5) {//if any less than 5 valid lines of data have been read in
        vector<int> ints;//create the vector of ints to pass to the placeShipRandomly function
        for(int i = 0; i < width*height;i++) ints.push_back(i);//loads said vector
        for (int i = 0; i < shipsVect.size(); i++) {//loops through each ship in the shipsVector to find which ones did not get data read in to them
            if (shipsVect[i].getPosition().empty()) {
                bool done = false;
                while (!done) {//loops until placeShipRandomly succeeds in placing a ship
                    done = placeShipRandomly(ints, i+1);
                }

            }
        }
    }
}
/**Generates a random number and checks to see if the ship can be placed at this position in any orientation
 *
 * @param ints vector of spaces that have not been checked yet
 * @param shipToPlace the index+1 in the shipsVect of the ship needing to be placed
 * @return returns if the function was successful in placing the shup
 */
bool Grid::placeShipRandomly(vector<int>& ints, int shipToPlace){
    int randomSpace = ints[rand() % ints.size()];//generates a random index within the ints vector
    int temp = ints.back();//swaops said elements value with the back value to allow for it to ber popped off now that it has been generated so it doesnt get generated again
    ints.back() = ints[randomSpace];
    ints.pop_back();
    ints[randomSpace] = temp;
    int x = randomSpace/height;//sets the x coordinate of the random space to x
    int y = randomSpace%height;//sets the y coordinate of the random space to u
    if(grid[x][y].getShipType() == 0){//if there js no ship at this random space
        if(x + shipsVect[shipToPlace - 1].getLength() < width && grid[x + shipsVect[shipToPlace-1].getLength()][y].getShipType() == 0) {//checks to the right if the far end is still in bounds and has no ship either
            if(checkToRight(randomSpace, shipToPlace)) {//check to see if any of the spaces in between have ships
                for (int i = x; i < x + shipsVect[shipToPlace - 1].getLength() ; i++) {//if they dont, loop through and set each squeares ship type and name to the type and name of the ship we are placing
                    grid[i][y].setShipType(shipToPlace);
                    grid[i][y].setShipName(shipsVect[shipToPlace - 1].getName());
                }
                return true;
            }
        }
        else if(x - shipsVect[shipToPlace - 1].getLength()  > width && grid[x - shipsVect[shipToPlace-1].getLength() ][y].getShipType() == 0) {//checks to the left if the far end is still in bounds and has no ship either
            if(checkToLeft(randomSpace, shipToPlace)) {//check to see if any of the spaces in between have ships
                for (int i = x; i > x - shipsVect[shipToPlace - 1].getLength() ; i--){//if they dont, loop through and set each squeares ship type and name to the type and name of the ship we are placing
                    grid[i][y].setShipType(shipToPlace);
                    grid[i][y].setShipName(shipsVect[shipToPlace - 1].getName());
                }
                return true;
            }
        }
        else if(y + shipsVect[shipToPlace - 1].getLength() < height && grid[x][y + shipsVect[shipToPlace-1].getLength() ].getShipType() == 0) {//checks above if the far end is still in bounds and has no ship either
            if(checkAbove(randomSpace, shipToPlace)) {//check to see if any of the spaces in between have ships
                for (int i = y; i < y + shipsVect[shipToPlace - 1].getLength() ; i++){//if they dont, loop through and set each squeares ship type and name to the type and name of the ship we are placing
                    grid[x][i].setShipType(shipToPlace);
                    grid[x][i].setShipName(shipsVect[shipToPlace - 1].getName());
                }
                return true;
            }
        }
        else if(y - shipsVect[shipToPlace - 1].getLength()  > height && grid[x][y - shipsVect[shipToPlace-1].getLength() ].getShipType() == 0) {//checks below if the far end is still in bounds and has no ship either
            if(checkBelow(randomSpace, shipToPlace)) {//check to see if any of the spaces in between have ships
                for (int i = y; i > y - shipsVect[shipToPlace - 1].getLength() ; i--) {//if they dont, loop through and set each squeares ship type and name to the type and name of the ship we are placing
                    grid[x][i].setShipType(shipToPlace);
                    grid[x][i].setShipName(shipsVect[shipToPlace - 1].getName());
                }
                return true;
            }
        }
    }
    return false;//returns false if at least one space  less than or equal to the ships length in every direction is occupied
}
/** Helper Functions for PlaceShipsRandomly
 *
 * @param randomSpace holds space to be used as a starting position
 * @param shipToPlace holds the current index +1 in the shipsVector of the ship we are trying to place
 * @return returns if any of the squares checked are occupied by other ships, true if all spots are unoccupied
 * randomSpace%height is the y coordinate of the random space
 * randomSpace/height is the x coordinate
 */
bool Grid::checkToRight(int randomSpace, int shipToPlace){//checks to see if all the squares to the right that would be occupied by the ship are unoccupied
    for (int i = randomSpace / height; i <= randomSpace / height + shipsVect[shipToPlace - 1].getLength() ; i++)
        if (grid[i][randomSpace % height].getShipType() != 0)
            return false;
    return true;
}
bool Grid::checkToLeft(int randomSpace, int shipToPlace) {//checks to see if all the squares to the left that would be occupied by the ship are unoccupied
    for (int i = randomSpace / height; i >= randomSpace / height - shipsVect[shipToPlace - 1].getLength() ; i--)
        if (grid[i][randomSpace % height].getShipType() != 0)
            return false;
    return true;
}
bool Grid::checkAbove(int randomSpace, int shipToPlace) {//checks to see if all the squares above that would be occupied by the ship are unoccupied
    for (int i = randomSpace % height; i <= randomSpace % height + shipsVect[shipToPlace - 1].getLength() ; i++)
        if (grid[randomSpace/height][i].getShipType() != 0)
            return false;
    return true;
}
bool Grid::checkBelow(int randomSpace, int shipToPlace) {//checks to see if all the squares below that would be occupied by the ship are unoccupied
    for (int i = randomSpace % height; i >= randomSpace % height - shipsVect[shipToPlace - 1].getLength();  i--)
        if (grid[randomSpace/height][i].getShipType() != 0)
            return false;
    return true;
}


bool Grid::checkIfLost() {//checks if all the ships on this grid have been sunk, return true if so and false if any remains afloat
    for (int j = 0; j < shipsVect.size(); j++)
            if(!shipsVect[j].getSunk()) return false;
    return true;
}

void Grid::quitGame(){//sinks all the ships on the grid if called, allows for player to quit midgame and let the checkiflost function still be usable
    for (int i = 0; i < shipsVect.size(); i++) shipsVect[i].setSunk();
}

/** Turn Functions **/

bool Grid::playerTurn() {//returns false if the player quits or if the computer loses

    string input;
    cout << "Enter the coordinate of the square you wish to hit, or Q if you would like to quit the game: ";
    bool validInput =false;
    while(!validInput){
        cin >> input;
        cout << endl;
        if(input == "Q" || input == "q") {//exits function if player wants to quit
            return false;
        }
        /*Checks if input is the correct length, if the character is a letter within the appropriate range, and if the next character(s) are numbers within the appropriate range */
        else if ((input.length() > 3 || input.length() < 2)||(toupper(input[0]) - 65 >= width || toupper(input[0]) - 65 < 0)||(input[1] -48 > height-1 || input[1] - 48 < 1)||(input.length() >2 && height/10 < input[1]-48)||(input.length() >2 && (input[2] -48 > height%10 || input[2] - 48 < 0)))
            cout << "Please enter a valid input. Valid inputs are inputs you have not previously entered and are either the letter Q by itself or a letter between A and "
                 << char(width + 64) << " followed by a number between 1 and " << height << ": ";
        else validInput = true;//exits loop if input is valid
        cin.clear();//clears buffer and goes to next line so inputs dont get mixed together
        cin.ignore(256, '\n');
    }

    int letter = (toupper(input[0])-65);//converts letter part of number to an int
    int number = (input[1]-49);//converts number part of inoput to an int
    if(input.length() > 2) number = (number+1)*10 + input[2]-49;//conversion for if number part is a double digit number
    if(grid[letter][number].getHit() || (grid[letter][number].getShipType() > 0 && shipsVect[grid[letter][number].getShipType() -1].getSunk())){//checks to see if you have already guessed said coordinate or if you have already sunk a ship loactaed at this coordinate
        cout << "You have either sunk the ship at this coordinate already or you have already guessed this coordinate! Please guess a new coordinate.\n\n";
        return playerTurn();//recursively calls the function if so
    }
    grid[letter][number].setHit();
    if(grid[letter][number].getShipType() > 0) {//if you hit one of their ships
        cout << "\nHit! You sunk their " << grid[letter][number].getShipName() << endl;
        grid[letter][number].setHit();
        shipsVect[grid[letter][number].getShipType() -1].setSunk();//sink ship since only one hit is needed to do so
    }
    else {//if you missed
        cout << "\nMiss. No ship located at " << input << endl;
        grid[letter][number].setHit();
    }
    if(checkIfLost()) return false;//check if computer lost
    return true;
}
void Grid::computerTurn() {//simulates computers turn
    int randomSpace;
    do {
        randomSpace = rand() % (width * height);//randomly generates a space to hit
    }
    while(grid[randomSpace/height][randomSpace%height].getHit() || (grid[randomSpace/height][randomSpace%height].getShipType() > 0 && shipsVect[grid[randomSpace/height][randomSpace%height].getShipType() -1].getSunk()));// iterates until it finds that it hasnt hit already and is not part of a ship it has sunken already
    grid[randomSpace/height][randomSpace%height].setHit();//sets hit to true
    cout << "The computer guessed "<< grid[randomSpace/height][randomSpace%height].getCoordinate();
    if(grid[randomSpace/height][randomSpace%height].getShipType() > 0) {//if the computer hit one of your ships
        cout << " and sunk your " << grid[randomSpace / height][randomSpace % height].getShipName() << "!\n";
        shipsVect[grid[randomSpace / height][randomSpace % height].getShipType()-1].setSunk();//sinks ship
    }
    else cout << " and missed. \n";//if the computer missed
    cout << "Here is your grid: \n\n";
}
