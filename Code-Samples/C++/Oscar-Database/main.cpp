// Created by Daniel Schlatter on 10/28/2018.

#include <iostream>
#include "Node.h"
#include <fstream>
#include <sstream>
#include "Functions.h"
#include "BSTree.h"
using namespace std;

int main() {
    ifstream infile;
    //infile.open("C:\\Users\\djsch\\CLionProjects\\OscarProject\\actor-actress.csv");
    infile.open("actor-actress.csv");
    auto* actorDB  = new BSTree<ActorData,string>;//tree for the actor/actresses
    string line;//string to hold each line of input
    string extra;//string to hold part of a fields input if it has a comma in it
    getline(infile, line);//gets first line
    actorDB->setHeader(line);//to set it to the header
    while(getline(infile, line)){//while there are lines left in the file
        ActorData temp;//temporary actor object
        istringstream linestream(line);//make a stream of each line so getline can be delimited by commas
        getline(linestream, temp.year, ',');//sets year value
        getline(linestream, temp.award, ',');//sets award value
        getline(linestream, temp.winner, ',');//sets winner value
        getline(linestream, temp.name, ',');//sets name value
        if(temp.name[0] == '"') {//ife name has quotes in it because of a comma
            getline(linestream,extra, ',');//get the input on the other side of th comma
            temp.name = temp.name.substr(1, temp.name.length()-1) + extra.substr(0,extra.length()-2);// remove the quotes and combine the strings
        }
        getline(linestream, temp.film);//sets film value
        if(temp.film[0] == '"') temp.film = temp.film.substr(1, temp.film.length()-2);// if it has quotes in it, remove them
        actorDB->addNode(temp.year, temp);//adds the node to the database
    }
    actorDB->setSortedBy(3);//sets it to sorted by year to start
    infile.close();
    //infile.open("C:\\Users\\djsch\\CLionProjects\\OscarProject\\pictures.csv");
    infile.open("pictures.csv");
    auto* movieDB = new BSTree<MovieData,string>;//tree for the movies
    getline(infile, line);//gets first line
    movieDB->setHeader(line);//to set it to the header
    while(getline(infile, line)){//while there are lines left in the file
        MovieData temp;//temporary movie object
        istringstream linestream(line);//stream of each line so getline can be delimited by commas
        getline(linestream, temp.name, ',');//sets name value
        getline(linestream, temp.year, ',');//sets year value
        getline(linestream, temp.nominations, ',');//sets nominations value
        getline(linestream, temp.rating, ',');//sets rating value
        getline(linestream, temp.duration, ',');//sets duration value
        getline(linestream, temp.genre1, ',');//sets genre1 value
        getline(linestream, temp.genre2, ',');//sets genre2 value
        getline(linestream, temp.release, ',');//sets release value
        getline(linestream,temp.metacritic, ',');//sets metacritic value
        getline(linestream, temp.synopsis);//sets synopsis value
        if(temp.synopsis[0] == '"') temp.synopsis = temp.synopsis.substr(1, temp.synopsis.length()-2);// removes quotes if synopsis has quotes in it
        movieDB->addNode(toLowerCase(temp.name), temp);// adds the node to the database
    }
    movieDB->setSortedBy(1);//sets it to sorted by the name to start
    infile.close();
    int menu = 0;//menu variable
    string input;//holds various inputs
    while(menu != 3){// while the user does not select quit option
        cout << "Welcome to the Oscars database! Please enter the number of the database you wish to access\n";
        cout << "1. Actor-Actress Database\n"
                "2. Pictures Database\n"
                "3. Quit\n";
        cin >> menu;
        while (cin.fail()) { //check to make sure user is entering an int
            cout << "Error, please enter an integer between 1 and 3" << endl;
            cin.clear();
            cin.ignore(256, '\n');
            cin >> menu;
        }
        switch(menu){
            case 1: {//access the actor database functions
                int subMenu = 0;
                while (subMenu != 8) {
                    cout << "You have accessed the Actor-Actress Database. Please enter the number of the operation you wish to perform\n"
                            "1. Display Database\n"
                            "2. Add a Record\n"
                            "3. Search for a record and modify the fields\n"
                            "4. Search for a record and delete the fields\n"
                            "5. Sort by any single sortable field\n"
                            "6. Search any (searchable) field\n"
                            "7. Write database info to a file\n"
                            "8. Exit this database\n";
                    cin >> subMenu;
                    while (cin.fail()) { //check to make sure user is entering an int
                        cout << "Error, please enter an integer between 1 and 7" << endl;
                        cin.clear();
                        cin.ignore(256, '\n');
                        cin >> subMenu;
                    }
                    switch (subMenu) {
                        case 1: {// displays the database
                            actorDB->printInOrderConsole();
                            break;
                        }
                        case 2: {//adds record to database
                            cin.ignore();
                            ActorData temp;
                            cout << "Name: \n";
                            getline(cin,temp.name);
                            cout << "Year: \n";
                            getline(cin,temp.year);
                            cout << "Award: \n";
                            getline(cin,temp.award);
                            cout << "Winner: \n";
                            getline(cin,temp.winner);
                            cout << "Film: \n";
                            getline(cin,temp.film);
                            actorDB->addNode(temp.year, temp);//adds the node to the database
                            actorDB = actorSort(actorDB, actorDB->getSortedBy());//re-sorts database to whatever it is currently sorted by
                            cout << "New Record successfully added.\n";
                            break;
                        }
                        case 3: {// search and modify record
                            actorDB = actorSort(actorDB, 1);// sorts database by name
                            cin.ignore();
                            string nameIn, yearIn, newData;
                            do{
                                cout << "Enter the name and year of the record you wish to modify\nName:\n";
                                getline(cin,nameIn);
                                cout << "Year: \n";
                                getline(cin, yearIn);
                                Node<ActorData,string> *tempNode;// node to hold the node to be modified
                                tempNode = actorSearchForNode(actorDB, toLowerCase(yearIn), toLowerCase(nameIn));//finds node with name and year user specified
                                if (tempNode == nullptr) cout << "Record not found.\n";// not found if the node is a nullptr
                                else {
                                    cout << "Record found.\n";
                                    do {
                                        ActorData tempAD;// temporary actor object
                                        actorDB->printToConsole(tempNode->Data());//prints the nodes data as it currently is to console
                                        cout << "Enter the number of the field you wish to modify, or 6 to exit\n"
                                                "1. Name\n"
                                                "2. Winner\n"
                                                "3. Year\n"
                                                "4. Award\n"
                                                "5. Film\n";
                                        cin >> subMenu;
                                        while (cin.fail()) { //check to make sure user is entering an int
                                            cout << "Error, please enter an integer between 1 and 6" << endl;
                                            cin.clear();
                                            cin.ignore(256, '\n');
                                            cin >> subMenu;
                                        }
                                        cin.ignore();
                                        if(subMenu != 6) {//have to have check before switch so user doesnt get asked for new data if they want to exit
                                            cout << "Enter your new value for this field\n";
                                            getline(cin, newData);
                                        }
                                        switch (subMenu) {//each case passes the users input to that specific field of the new actor object, and copies over the rest of the fields from the old actor object
                                            case 1://modify name
                                                tempAD.name = newData;
                                                tempAD.winner = tempNode->Data().winner;
                                                tempAD.year = tempNode->Data().year;
                                                tempAD.award = tempNode->Data().award;
                                                tempAD.film = tempNode->Data().film;
                                                tempNode->setData(tempAD);
                                                cout << "Updated record:\n";
                                                break;
                                            case 2://modify winner
                                                tempAD.name = tempNode->Data().name;
                                                tempAD.winner = newData;
                                                tempAD.year = tempNode->Data().year;
                                                tempAD.award = tempNode->Data().award;
                                                tempAD.film = tempNode->Data().film;
                                                tempNode->setData(tempAD);
                                                cout << "Updated record:\n";
                                                break;
                                            case 3://modify year
                                                tempAD.name = tempNode->Data().name;
                                                tempAD.winner = tempNode->Data().winner;
                                                tempAD.year = newData;
                                                tempAD.award = tempNode->Data().award;
                                                tempAD.film = tempNode->Data().film;
                                                tempNode->setData(tempAD);
                                                cout << "Updated record:\n";
                                                break;
                                            case 4://modify awaard
                                                tempAD.name = tempNode->Data().name;
                                                tempAD.winner = tempNode->Data().winner;
                                                tempAD.year = tempNode->Data().year;
                                                tempAD.award = newData;
                                                tempAD.film = tempNode->Data().film;
                                                tempNode->setData(tempAD);
                                                cout << "Updated record:\n";
                                                break;
                                            case 5://modify film
                                                tempAD.name = tempNode->Data().name;
                                                tempAD.winner = tempNode->Data().winner;
                                                tempAD.year = tempNode->Data().year;
                                                tempAD.award = tempNode->Data().award;
                                                tempAD.film = newData;
                                                tempNode->setData(tempAD);
                                                cout << "Updated record:\n";
                                                break;
                                            case 6://exit loop
                                                break;
                                            default:
                                                cout << "Please enter a number between 1 and 6";
                                                break;
                                        }

                                    }
                                    while(subMenu != 6);// loops until user enters a 6, allowing user to modify as many fields as they wish before exiting
                                    actorDB->printToConsole(tempNode->Data());//prints updated record to console
                                    cout << "Modification finished.\n";
                                }
                                cout << "Enter 1 if you would like to search again, or anything else if you would like  to exit: \n";
                                getline(cin,input);
                            }
                            while(input == "1");// loops until user enters soemthing other than a 1, in case they wish to modify multiple records
                            actorDB = actorSort(actorDB, actorDB->getSortedBy());// re-sorts the database to what it was sorted by before the method was called
                            break;
                        }
                        case 4: {// search and delete
                            actorDB = actorSort(actorDB, 1);// sorts database by name
                            cin.ignore();
                            string nameIn, yearIn;
                            do{
                                cout << "Enter the name and year of the record you wish to delete\nName:\n";
                                getline(cin,nameIn);
                                cout << "Year: \n";
                                getline(cin, yearIn);
                                Node<ActorData,string> *tempNode;
                                tempNode = actorSearchForNode(actorDB, toLowerCase(yearIn), toLowerCase(nameIn));//finds the node with the name and year user inputted
                                if (tempNode == nullptr) cout << "Record not found.";
                                else {
                                    cout << "Record found.\n";
                                    actorDB->printToConsole(tempNode->Data());//prints node to console
                                    cout << "Press 1 to confirm the deletion of this record, or anything else to cancel";
                                    getline(cin,input);
                                    if (input == "1") {
                                        actorDB->deleteNode(tempNode->Key());//deletes node if user presses 1
                                        cout << "Record deleted.\n";
                                    }
                                    else cout << "Deletion canceled.";//cancels otherwise
                                }
                                cout << "Enter 1 if you would like to search again, or anything else if you would like  to exit: \n";
                                getline(cin,input);
                            }
                            while(input == "1");//loops till user presses something other than 1, in case they wish to delete multiple records
                            actorDB = actorSort(actorDB, actorDB->getSortedBy());// re-sorts database by what it was sorted by before method was called
                            break;
                        }

                        case 5: {//sort
                            cout << "Which field do you wish to sort by?\n"
                                    "1. Name\n"
                                    "2. Winner\n"
                                    "3. Year\n"
                                    "4. Award\n"
                                    "5. Film\n";
                            cin >> subMenu;
                            while(cin.fail() || subMenu < 1 || subMenu > 5){//ensures user enters an int between 1 and 5
                                cout << "Error, please enter a number between 1 and 5\n";
                                cin.clear();
                                cin.ignore(256, '\n');
                                cin >> subMenu;
                            }
                            actorDB->setSortedBy(subMenu);//changes the set sorted by to the option the user chose
                            actorDB = actorSort(actorDB, subMenu);//sorts database
                            cout << "Successfully resorted.\n";
                            break;
                        }
                        case 6: {//search
                            cin.ignore();
                            int count = 0;
                            cout << "Which field do you wish to search?\n"
                                    "1. Name\n"
                                    "2. Winner\n"
                                    "3. Year\n"
                                    "4. Award\n"
                                    "5. Film\n";
                            cin >> subMenu;
                            while(cin.fail() || subMenu < 1 || subMenu > 5){//ensures user enters a valid input
                                cout << "Error, please enter a number between 1 and 5\n";
                                cin.clear();
                                cin.ignore(256, '\n');
                                cin >> subMenu;
                            }
                            actorDB = actorSort(actorDB, subMenu);//sorts database by whichever field the user wishes to search
                            BSTree<ActorData,string> *searchTree = actorDB;// creates a new tree of just the search results
                            do{
                                if(count > 0) {// this part is skipped on the first iteration. needed it so that the search tree wasnt being re set to the main database every time it looped
                                    cout << "Which field do you wish to search?\n"
                                            "1. Name\n"
                                            "2. Winner\n"
                                            "3. Year\n"
                                            "4. Award\n"
                                            "5. Film\n";
                                    cin >> subMenu;
                                    while(cin.fail() || subMenu < 1 || subMenu > 5){//ensures user enters a valid input
                                        cout << "Error, please enter a number between 1 and 5\n";
                                        cin.clear();
                                        cin.ignore(256, '\n');
                                        cin >> subMenu;
                                    }
                                    searchTree = actorSort(searchTree, subMenu);//sorts search tree by user-inputted field
                                }
                                searchTree = actorSearch(searchTree);//searches the search tree, returns a tree of all the results
                                if(searchTree->Root() == nullptr){//if returned tree is empty, nothing was found
                                    input = "2";
                                    cout << "No results found.\n";
                                }
                                else {//otherwise
                                    cout << "\n\n Search Results\n\n";
                                    searchTree->printInOrderConsole();//print results to console
                                    cout << "Enter 1 to search within these search results, or anything else to return to the main menu:\n";
                                    getline(cin, input);
                                }
                                count ++;
                            }
                            while(input == "1");//loops till user enters a 1, in case they wish to search within their search results
                            actorDB = actorSort(actorDB, actorDB->getSortedBy());// sorts database back to what it was sorted by originally before the method was called
                            break;
                        }
                        case 7: {//output database to file
                            cin.ignore();
                            cout << "Enter the name of the file you would like the database information written to:\n";
                            getline(cin,input);//opens file inputted by user
                            ofstream outfile;
                            outfile.open(input);
                            actorDB->printInOrderToFile(outfile);//prints database to file
                            cout << "Database successfully written to file.\n";
                            outfile.close();
                            break;
                        }
                        case 8:{
                            cout << "Database Exited.\n";
                            break;
                        }
                        default: {
                            cout << "Please enter a number between 1 and 7\n";
                            break;
                        }
                    }
                }
                break;
            }
            case 2: {
                int subMenu = 0;
                while (subMenu != 8) {
                    cout << "You have accessed the Movie Database. Please enter the number of the operation you wish to perform\n"
                            "1. Display Database\n"
                            "2. Add a Record\n"
                            "3. Search for a record and modify the fields\n"
                            "4. Search for a record and delete the fields\n"
                            "5. Sort by any single sortable field\n"
                            "7. Write database info to a file\n"
                            "8. Exit this database\n";
                    cin >> subMenu;
                    while (cin.fail()) { //check to make sure user is entering an int
                        cout << "Error, please enter an integer between 1 and 7" << endl;
                        cin.clear();
                        cin.ignore(256, '\n');
                        cin >> subMenu;
                    }
                    switch (subMenu) {
                        case 1: {//display
                            movieDB->printInOrderConsole();//prints DB to console
                            break;
                        }
                        case 2: {// add record
                            cin.ignore();
                            MovieData temp;
                            cout << "Name: \n";
                            getline(cin, temp.name);
                            cout << "Year: \n";
                            getline(cin,temp.year);
                            cout << "Nominations: \n";
                            getline(cin,temp.nominations);
                            cout << "Rating: \n";
                            getline(cin,temp.rating);
                            cout << "Duration: \n";
                            getline(cin,temp.duration);
                            cout << "Genre 1: \n";
                            getline(cin,temp.genre1);
                            cout << "Genre 2: \n";
                            getline(cin,temp.genre2);
                            cout << "Release: \n";
                            getline(cin,temp.release);
                            cout << "Metacritic: \n";
                            getline(cin,temp.metacritic);
                            cout << "Synopsis: \n";
                            getline(cin,temp.synopsis);
                            movieDB->addNode(toLowerCase(temp.name), temp);//adds record to DB
                            movieDB = movieSort(movieDB, movieDB->getSortedBy());//re-sorts DB by whatever it was already sorted by
                            cout << "New Record successfully added.\n";
                            break;
                        }
                        case 3:{//search and modify
                            movieDB = movieSort(movieDB, 1);// sorts DB by name
                            cin.ignore();
                            string nameIn, yearIn, newData;
                            do{
                                cout << "Enter the name and year of the record you wish to modify\nName:\n";
                                getline(cin,nameIn);
                                cout << "Year: \n";
                                getline(cin, yearIn);
                                Node<MovieData,string> *tempNode;
                                tempNode = movieSearchForNode(movieDB, toLowerCase(yearIn), toLowerCase(nameIn));//finds record with use inputted name and year
                                if (tempNode == nullptr) cout << "Record not found.\n";
                                else {
                                    cout << "Record found.\n";
                                    do {
                                        MovieData tempAD;
                                        movieDB->printToConsole(tempNode->Data());//prints current state of record to console
                                        cout << "Enter the number of the field you wish to modify, or 11 to exit\n"
                                                "1. Name\n"
                                                "2. Year\n"
                                                "3. Nominations\n"
                                                "4. Rating\n"
                                                "5. Duration\n"
                                                "6. Genre 1\n"
                                                "7. Genre 2\n"
                                                "8. Release\n"
                                                "9. Metacritic\n"
                                                "10. Synopsis\n";
                                        cin >> subMenu;
                                        while(cin.fail()){//ensures user enters a valid input
                                            cout << "Error, please enter a number between 1 and 11\n";
                                            cin.clear();
                                            cin.ignore(256, '\n');
                                            cin >> subMenu;
                                        }
                                        cin.ignore();
                                        if(subMenu != 11) {// asks for data if the user doesnt want to quit
                                            cout << "Enter your new value for this field\n";
                                            getline(cin, newData);
                                        }
                                        switch (subMenu) {//each case passes the users input to that specific field of the new movie object, and copies over the rest of the fields from the old movie object
                                            case 1://modifies name
                                                tempAD.name = newData;
                                                tempAD.year = tempNode->Data().year;
                                                tempAD.nominations = tempNode->Data().nominations;
                                                tempAD.rating = tempNode->Data().rating;
                                                tempAD.duration = tempNode->Data().duration;
                                                tempAD.genre1 = tempNode->Data().genre1;
                                                tempAD.genre2 = tempNode->Data().genre2;
                                                tempAD.release = tempNode->Data().release;
                                                tempAD.metacritic = tempNode->Data().metacritic;
                                                tempAD.synopsis = tempNode->Data().synopsis;
                                                tempNode->setData(tempAD);
                                                cout << "Updated record:\n";
                                                break;
                                            case 2://modifies year
                                                tempAD.name = tempNode->Data().name;
                                                tempAD.year = newData;
                                                tempAD.nominations = tempNode->Data().nominations;
                                                tempAD.rating = tempNode->Data().rating;
                                                tempAD.duration = tempNode->Data().duration;
                                                tempAD.genre1 = tempNode->Data().genre1;
                                                tempAD.genre2 = tempNode->Data().genre2;
                                                tempAD.release = tempNode->Data().release;
                                                tempAD.metacritic = tempNode->Data().metacritic;
                                                tempAD.synopsis = tempNode->Data().synopsis;
                                                tempNode->setData(tempAD);
                                                cout << "Updated record:\n";
                                                break;
                                            case 3://modifies nominations
                                                tempAD.name = tempNode->Data().name;
                                                tempAD.year = tempNode->Data().year;
                                                tempAD.nominations = newData;
                                                tempAD.rating = tempNode->Data().rating;
                                                tempAD.duration = tempNode->Data().duration;
                                                tempAD.genre1 = tempNode->Data().genre1;
                                                tempAD.genre2 = tempNode->Data().genre2;
                                                tempAD.release = tempNode->Data().release;
                                                tempAD.metacritic = tempNode->Data().metacritic;
                                                tempAD.synopsis = tempNode->Data().synopsis;
                                                tempNode->setData(tempAD);
                                                cout << "Updated record:\n";
                                                break;
                                            case 4://modifies rating
                                                tempAD.name = tempNode->Data().name;
                                                tempAD.year = tempNode->Data().year;
                                                tempAD.nominations = tempNode->Data().nominations;
                                                tempAD.rating = newData;
                                                tempAD.duration = tempNode->Data().duration;
                                                tempAD.genre1 = tempNode->Data().genre1;
                                                tempAD.genre2 = tempNode->Data().genre2;
                                                tempAD.release = tempNode->Data().release;
                                                tempAD.metacritic = tempNode->Data().metacritic;
                                                tempAD.synopsis = tempNode->Data().synopsis;
                                                tempNode->setData(tempAD);
                                                cout << "Updated record:\n";
                                                break;
                                            case 5://modifies duration
                                                tempAD.name = tempNode->Data().name;
                                                tempAD.year = tempNode->Data().year;
                                                tempAD.nominations = tempNode->Data().nominations;
                                                tempAD.rating = tempNode->Data().rating;
                                                tempAD.duration = newData;
                                                tempAD.genre1 = tempNode->Data().genre1;
                                                tempAD.genre2 = tempNode->Data().genre2;
                                                tempAD.release = tempNode->Data().release;
                                                tempAD.metacritic = tempNode->Data().metacritic;
                                                tempAD.synopsis = tempNode->Data().synopsis;
                                                tempNode->setData(tempAD);
                                                cout << "Updated record:\n";
                                                break;
                                            case 6://modifies genre1
                                                tempAD.name = tempNode->Data().name;
                                                tempAD.year = tempNode->Data().year;
                                                tempAD.nominations = tempNode->Data().nominations;
                                                tempAD.rating = tempNode->Data().rating;
                                                tempAD.duration = tempNode->Data().duration;
                                                tempAD.genre1 = newData;
                                                tempAD.genre2 = tempNode->Data().genre2;
                                                tempAD.release = tempNode->Data().release;
                                                tempAD.metacritic = tempNode->Data().metacritic;
                                                tempAD.synopsis = tempNode->Data().synopsis;
                                                tempNode->setData(tempAD);
                                                cout << "Updated record:\n";
                                                break;
                                            case 7://modifies genre 2
                                                tempAD.name = tempNode->Data().name;
                                                tempAD.year = tempNode->Data().year;
                                                tempAD.nominations = tempNode->Data().nominations;
                                                tempAD.rating = tempNode->Data().rating;
                                                tempAD.duration = tempNode->Data().duration;
                                                tempAD.genre1 = tempNode->Data().genre1;
                                                tempAD.genre2 = newData;
                                                tempAD.release = tempNode->Data().release;
                                                tempAD.metacritic = tempNode->Data().metacritic;
                                                tempAD.synopsis = tempNode->Data().synopsis;
                                                tempNode->setData(tempAD);
                                                cout << "Updated record:\n";
                                                break;
                                            case 8://modifies release
                                                tempAD.name = tempNode->Data().name;
                                                tempAD.year = tempNode->Data().year;
                                                tempAD.nominations = tempNode->Data().nominations;
                                                tempAD.rating = tempNode->Data().rating;
                                                tempAD.duration = tempNode->Data().duration;
                                                tempAD.genre1 = tempNode->Data().genre1;
                                                tempAD.genre2 = tempNode->Data().genre2;
                                                tempAD.release = newData;
                                                tempAD.metacritic = tempNode->Data().metacritic;
                                                tempAD.synopsis = tempNode->Data().synopsis;
                                                tempNode->setData(tempAD);
                                                cout << "Updated record:\n";
                                                break;
                                            case 9://modifies metacritic
                                                tempAD.name = tempNode->Data().name;
                                                tempAD.year = tempNode->Data().year;
                                                tempAD.nominations = tempNode->Data().nominations;
                                                tempAD.rating = tempNode->Data().rating;
                                                tempAD.duration = tempNode->Data().duration;
                                                tempAD.genre1 = tempNode->Data().genre1;
                                                tempAD.genre2 = tempNode->Data().genre2;
                                                tempAD.release = tempNode->Data().release;
                                                tempAD.metacritic = newData;
                                                tempAD.synopsis = tempNode->Data().synopsis;
                                                tempNode->setData(tempAD);
                                                cout << "Updated record:\n";
                                                break;
                                            case 10://modifies synopsis
                                                tempAD.name = tempNode->Data().name;
                                                tempAD.year = tempNode->Data().year;
                                                tempAD.nominations = tempNode->Data().nominations;
                                                tempAD.rating = tempNode->Data().rating;
                                                tempAD.duration = tempNode->Data().duration;
                                                tempAD.genre1 = tempNode->Data().genre1;
                                                tempAD.genre2 = tempNode->Data().genre2;
                                                tempAD.release = tempNode->Data().release;
                                                tempAD.metacritic = tempNode->Data().metacritic;
                                                tempAD.synopsis = newData;
                                                tempNode->setData(tempAD);
                                                cout << "Updated record:\n";
                                                break;
                                            case 11://exits mod menu
                                                break;
                                            default:
                                                cout << "Please enter a number between 1 and 10";
                                                break;
                                        }

                                    }
                                    while(subMenu != 11);//loops till user exits
                                    movieDB->printToConsole(tempNode->Data());//prints updated record to console
                                    cout << "Modification finished.\n";
                                }
                                cout << "Enter 1 if you would like to search again, or anything else if you would like  to exit: \n";
                                getline(cin,input);
                            }
                            while(input == "1");//loops till user exits
                            movieDB = movieSort(movieDB, movieDB->getSortedBy());//re-sorts
                            break;
                        }
                         case 4:{//search and delete
                            movieDB = movieSort(movieDB, 1);//sorts by name
                            cin.ignore();
                            string nameIn, yearIn;
                            do{
                                cout << "Enter the name and year of the record you wish to delete\nName:\n";
                                getline(cin,nameIn);
                                cout << "Year: \n";
                                getline(cin, yearIn);
                                Node<MovieData,string> *tempNode;
                                tempNode = movieSearchForNode(movieDB, toLowerCase(yearIn), toLowerCase(nameIn));//finds node with values use inputted
                                if (tempNode == nullptr) cout << "Record not found.";
                                else {
                                    cout << "Record found.\n";
                                    movieDB->printToConsole(tempNode->Data());//prints node
                                    cout << "Press 1 to confirm the deletion of this record, or anything else to cancel\n";
                                    getline(cin,input);
                                    if (input == "1") movieDB->deleteNode(tempNode->Key());//deletes node if user presses 1
                                    else cout << "Deletion canceled.\n";//cancels otherwise
                                }
                                cout << "Enter 1 if you would like to search again, or anything else if you would like  to exit: \n";
                                getline(cin,input);
                            }
                            while(input == "1");
                            movieDB = movieSort(movieDB, movieDB->getSortedBy());//re-sorts
                            break;
                        }
                         case 5: {//sort
                             cout << "Which field do you wish to sort by?\n"
                                     "1. Name\n"
                                     "2. Year\n"
                                     "3. Nominations\n"
                                     "4. Rating\n"
                                     "5. Duration\n"
                                     "6. Genre 1\n"
                                     "7. Genre 2\n"
                                     "8. Release\n"
                                     "9. Metacritic\n"
                                     "10. Synopsis\n";
                             cin >> subMenu;
                             while(cin.fail() || subMenu < 1 || subMenu > 10){//ensures user enters valid input
                                 cout << "Error, please enter a number between 1 and 11\n";
                                 cin.clear();
                                 cin.ignore(256, '\n');
                                 cin >> subMenu;
                             }
                             movieDB->setSortedBy(subMenu);//sets sorted by to field user chpse
                             movieDB = movieSort(movieDB, subMenu);//sorts db
                             cout << "Successfully resorted.\n";
                             break;
                         }
                         case 6: {//search
                             cin.ignore();
                             int count = 0;
                             cout << "Which field do you wish to search?\n"
                                     "1. Name\n"
                                     "2. Year\n"
                                     "3. Nominations\n"
                                     "4. Rating\n"
                                     "5. Duration\n"
                                     "6. Genre 1\n"
                                     "7. Genre 2\n"
                                     "8. Release\n"
                                     "9. Metacritic\n"
                                     "10. Synopsis\n";
                             cin >> subMenu;
                             while(cin.fail() || subMenu < 1 || subMenu > 10){//ensures user enters valid input
                                 cout << "Error, please enter a number between 1 and 11\n";
                                 cin.clear();
                                 cin.ignore(256, '\n');
                                 cin >> subMenu;
                             }
                             movieDB = movieSort(movieDB, subMenu);//sorts by field user chose
                             BSTree<MovieData,string> *searchTree = movieDB;//initializes search tree
                             do{
                                 if(count > 0) {//skips on first iteration
                                     cout << "Which field do you wish to search?\n"
                                             "1. Name\n"
                                             "2. Year\n"
                                             "3. Nominations\n"
                                             "4. Rating\n"
                                             "5. Duration\n"
                                             "6. Genre 1\n"
                                             "7. Genre 2\n"
                                             "8. Release\n"
                                             "9. Metacritic\n"
                                             "10. Synopsis\n";
                                     cin >> subMenu;
                                     while(cin.fail() || subMenu < 1 || subMenu > 10){//ensures user enters valid input
                                         cout << "Error, please enter a number between 1 and 11\n";
                                         cin.clear();
                                         cin.ignore(256, '\n');
                                         cin >> subMenu;
                                     }
                                     searchTree = movieSort(searchTree, subMenu);//sorts by field user chose
                                 }
                                 searchTree = movieSearch(searchTree);//searches search tree for value
                                 if(searchTree->Root() == nullptr){//if tree is empty
                                     input = "2";//exit loop
                                     cout << "No results found.\n";
                                 }
                                 else {
                                     cout << "\n\n Search Results\n\n";
                                     searchTree->printInOrderConsole();//print search results to console
                                     cout << "Enter 1 to search within these search results, or anything else to return to the main menu:\n";
                                     getline(cin, input);
                                 }
                                 count ++;
                             }
                             while(input == "1");//loops till user exits
                             movieDB = movieSort(movieDB, movieDB->getSortedBy());//re-sorts
                             break;
                         }
                        case 7: {//output to file
                            cin.ignore();
                            cout << "Enter the name of the file you would like the database information written to:\n";
                            getline(cin,input);
                            ofstream outfile;
                            outfile.open(input);
                            movieDB->printInOrderToFile(outfile);//prints it to file
                            cout << "Database successfully written to file.\n";
                            outfile.close();
                            break;
                        }
                        case 8:{
                            cout << "Database Exited.\n";
                            break;
                        }
                        default: {
                            cout << "Please enter a number between 1 and 7\n";
                            break;
                        }
                    }
                }
                break;
            }
            case 3: {
                cout << "Program exited! Thank you" << endl;
                break;
            }
            default: {
                cout << "Please enter either a 1,2 or a 3" << endl;
                break;
            }
        }
    }

    return 0;
}
