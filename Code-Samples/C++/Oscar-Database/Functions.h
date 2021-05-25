//
// Created by Daniel Schlatter on 10/28/2018.
//
#include "BSTree.h"
#include "BSTree.hpp"
#include <vector>
#include <algorithm>
#ifndef OSCARPROJECT_DATABASE_H
#define OSCARPROJECT_DATABASE_H

using namespace std;
BSTree<ActorData,string> * actorSort(BSTree<ActorData,string> * oldTree, int subMenu);// returns an actor tree sorted by the field indicated by the submenu argument
template <typename DATATYPE>
void searchKey(Node<DATATYPE, string> * node, string searchTerm, int searchType, BSTree<DATATYPE, string> *searchTree);
BSTree<ActorData, string> * actorSearch(BSTree<ActorData, string> *mainTree);
BSTree<MovieData,string> * movieSort(BSTree<MovieData,string> * oldTree, int subMenu);
BSTree<MovieData, string> * movieSearch(BSTree<MovieData, string> *mainTree);
string toLowerCase(string str);
Node<ActorData,string> * actorSearchForNode(BSTree<ActorData, string> *mainTree, string year, string name);
Node<MovieData,string> * movieSearchForNode(BSTree<MovieData, string> *mainTree, string year, string name);
template <typename DATATYPE>
void searchKeyVect(Node<DATATYPE, string> * node, string searchTerm, vector<Node<DATATYPE,string>*> &searchVect);
#endif //OSCARPROJECT_DATABASE_H
