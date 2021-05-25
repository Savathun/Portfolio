//
//Daniel Schlatter
//Added structs, sortedby, header, print to file functions. deleted max and findnode functions
//
//BSTree header file
#ifndef BSTREEINT_H
#define BSTREEINT_H
#include <iostream>

using namespace std;
#include "Node.h"
struct ActorData{//struct to hold actor/actress data
    string year;
    string award;
    string winner;
    string name;
    string film;
};
struct MovieData{//struct to hold the movie data
    string name;
    string year;
    string nominations;
    string rating;
    string duration;
    string genre1;
    string genre2;
    string release;
    string metacritic;
    string synopsis;
};
// Binary Search Tree class
template <typename DATATYPE, typename KEYTYPE>
class BSTree {
private:
    Node<DATATYPE, KEYTYPE> * root;//root of tree
    void addNode(KEYTYPE key, Node<DATATYPE, KEYTYPE> * leaf, const DATATYPE& data);//adds node to one of leafs subtrees
    void freeNode(Node<DATATYPE, KEYTYPE> * leaf);//frees node
    void printInOrderConsole(Node<DATATYPE, KEYTYPE> *node);//prints tree to console
    void printInOrderToFile(Node<DATATYPE, KEYTYPE> * node, ostream & out);//prints tree to file
    Node<DATATYPE, KEYTYPE> * deleteNode(Node<DATATYPE, KEYTYPE> * node, KEYTYPE key);//deletes node with key
    string header;//holds the category headers
    int sortedBy;// indicates what field the tree is sorted by. used when tree has to be resorted to be searched so it can be sorted back to what it was before without user input
public:
    BSTree<DATATYPE, KEYTYPE>();//constructor
    ~BSTree<DATATYPE, KEYTYPE>();//destructor
    Node<DATATYPE, KEYTYPE> * Root() { return root; }// returns root of tree
    void setRoot(Node<DATATYPE, KEYTYPE> * _root) {root = _root;}// sets root to _root
    void addNode(KEYTYPE key, const DATATYPE &data);//adds node with values passed as parameters
    void printInOrderConsole();//prints tree to console
    void printInOrderToFile(ostream & out);// prints tree to file
    void printToConsole(const MovieData &data);//prints MovieData node to console
    void printToConsole(const ActorData &data);//prints ActorData node to console
    void printToFile(ostream & out, const MovieData & data);// prints MovieData node to file
    void printToFile(ostream & out, const ActorData & data);// prints ActorData node to file
    void deleteNode(KEYTYPE key);//deletes node with this key
    Node<DATATYPE, KEYTYPE> * min(Node<DATATYPE, KEYTYPE> * node);//finds node with the smallest key
    void setHeader(string nHeader){header = nHeader;}//setter for header
    void setSortedBy(int num){sortedBy = num;}//setter for sortedby
    int getSortedBy(){ return sortedBy;}//getter for sortedby

};
#endif  //BST
