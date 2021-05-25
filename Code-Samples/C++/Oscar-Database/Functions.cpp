//
// Created by Daniel Schlatter on 11/8/2018.
//
#include "Functions.h"
BSTree<ActorData,string> * actorSort(BSTree<ActorData,string> * oldTree, int subMenu){//returns actor tree sorted by field specified by submenu argument
    auto *newTree = new BSTree<ActorData, string>;//creates new tree
    newTree->setSortedBy(oldTree->getSortedBy());//sets new tree's sortedby to the old trees sorted by
    while(oldTree->Root() != nullptr){//while the old tree still has nodes
        switch(subMenu){//each case adds the root node of the old tree to the new tree with one of the fields as the key
            case 1://sorted by name
                newTree->addNode(toLowerCase(oldTree->Root()->Data().name), oldTree->Root()->Data());
                break;
            case 2://sorted by winner
                newTree->addNode(oldTree->Root()->Data().winner, oldTree->Root()->Data());
                break;
            case 3://sorted by year
                newTree->addNode(oldTree->Root()->Data().year, oldTree->Root()->Data());
                break;
            case 4://sorted by award
                newTree->addNode(toLowerCase(oldTree->Root()->Data().award), oldTree->Root()->Data());
                break;
            case 5://sorted by film
                newTree->addNode(toLowerCase(oldTree->Root()->Data().film), oldTree->Root()->Data());
                break;
            default:
                break;
        }
        oldTree->deleteNode(oldTree->Root()->Key());//deletes root of old tree
    }
    return newTree;// returns the new tree
}
string toLowerCase(string str){//returns a lowercase copy of a string
    string strCpy = move(str);// copies string over
    transform(strCpy.begin(), strCpy.end(), strCpy.begin(), ::tolower);// transforms string to lowercase
    return strCpy;// returns lower case string
}
BSTree<MovieData,string> * movieSort(BSTree<MovieData,string> * oldTree, int subMenu){//returns movie tree sorted by field specified by submenu argument
    auto *newTree = new BSTree<MovieData, string>;//creates new tree
    newTree->setSortedBy(oldTree->getSortedBy());//sets new tree's sortedby to the old trees sorted by
    while(oldTree->Root() != nullptr){//while the old tree still has nodes
        switch(subMenu){//each case adds the root node of the old tree to the new tree with one of the fields as the key
            case 1://sorted by name
                newTree->addNode(toLowerCase(oldTree->Root()->Data().name), oldTree->Root()->Data());
                break;
            case 2://sorted by year
                newTree->addNode(oldTree->Root()->Data().year, oldTree->Root()->Data());
                break;
            case 3://sorted by nominations
                newTree->addNode(oldTree->Root()->Data().nominations, oldTree->Root()->Data());
                break;
            case 4://sorted by
                newTree->addNode(oldTree->Root()->Data().rating, oldTree->Root()->Data());
                break;
            case 5://sorted by duration
                newTree->addNode(oldTree->Root()->Data().duration, oldTree->Root()->Data());
                break;
            case 6://sorted by genre1
                newTree->addNode(toLowerCase(oldTree->Root()->Data().genre1), oldTree->Root()->Data());
                break;
            case 7://sorted by genre2
                newTree->addNode(toLowerCase(oldTree->Root()->Data().genre2), oldTree->Root()->Data());
                break;
            case 8://sorted by release
                newTree->addNode(toLowerCase(oldTree->Root()->Data().release), oldTree->Root()->Data());
                break;
            case 9://sorted by metacritic
                newTree->addNode(oldTree->Root()->Data().metacritic, oldTree->Root()->Data());
                break;
            case 10://sorted by synopsis
                newTree->addNode(toLowerCase(oldTree->Root()->Data().synopsis), oldTree->Root()->Data());
                break;
            default:
                break;
        }
        oldTree->deleteNode(oldTree->Root()->Key());//deletes root of old tree
    }
    return newTree;//returns new tree
}
BSTree<MovieData, string> * movieSearch(BSTree<MovieData, string> *mainTree){//searches maintree of movies
    auto *searchTree = new BSTree<MovieData, string>;//creates new tree
    int searchType;//for holding the type of search
    cout << "What kind of search do you wish to perform?\n"
            "1. Exact Search\n"
            "2. Contains Search\n";
    cin >> searchType;
    while(cin.fail() || searchType < 1 || searchType > 2){//ensures user enters a 1 or a 2
        cout << "Error, please enter either a 1 or a 2\n";
        cin.clear();
        cin.ignore(256, '\n');
        cin >> searchType;
    }
    string searchTerm;
    cout << "Please enter your search term:\n";
    cin.ignore();
    getline(cin,searchTerm);
    searchKey(mainTree->Root(), toLowerCase(searchTerm), searchType, searchTree);//searches tree starting at root, passes the searchtree for nodes to be added to it
    return searchTree;//returns search tree
}

BSTree<ActorData, string> * actorSearch(BSTree<ActorData, string> *mainTree){//searches maintree of actors
    auto *searchTree = new BSTree<ActorData, string>;//creates new tree
    int searchType;
    cout << "What kind of search do you wish to perform?\n"
            "1. Exact Search\n"
            "2. Contains Search\n";
    cin >> searchType;
    while(cin.fail() || searchType < 1 || searchType > 2){//ensures the user enters a 1 or a 2
        cout << "Error, please enter either a 1 or a 2\n";
        cin.clear();
        cin.ignore(256, '\n');
        cin >> searchType;
    }
    string searchTerm;
    cout << "Please enter your search term:\n";
    cin.ignore();
    getline(cin,searchTerm);
    searchKey(mainTree->Root(), toLowerCase(searchTerm), searchType, searchTree);//searches tree starting at root, passes the searchtree for nodes to be added to it
    return searchTree;//returns search tree
}
template <typename DATATYPE>
void searchKey(Node<DATATYPE, string> * node, string searchTerm, int searchType, BSTree<DATATYPE, string> *searchTree){//searches a tree for a search term, starting at node, adding positive results to searchtree
    if(node != nullptr) {//if the node is not nullptr
        //if key matches this nodes key, or if it is a contains search and the key is in this nodes key, adds it to the search tree
        if(node->Key() == searchTerm || (searchType == 2 && node->Key().find(searchTerm) != string::npos)) searchTree->addNode(node->Key(), node->Data());
        if (node->Left() != nullptr) searchKey(node->Left(), searchTerm, searchType, searchTree);//searches left subtree recursively
        if (node->Right() != nullptr) searchKey(node->Right(), searchTerm, searchType, searchTree);//searches right subtree recursively
    }
}
Node<ActorData,string> * actorSearchForNode(BSTree<ActorData, string> *mainTree, string year, string name){//finds a node with this specific year and name
    vector<Node<ActorData,string>*> searchVect;// vector of nodes with the same name as the name passed as parameter
    searchKeyVect(mainTree->Root(), move(name), searchVect);// starting at root, searches a tree recursively for the name, adds positive results to searchVect
    if(searchVect.empty()) return nullptr;// if no results were found, return nullptr
    else{//otherwise
        for(auto i : searchVect){//check each result in the searchVect to see if the year matches
            if(toLowerCase(i->Data().year) == year) return i;// returns the node if the year matches

        }
        return nullptr;//if no matches were found return nullptr
    }
}
Node<MovieData,string> * movieSearchForNode(BSTree<MovieData, string> *mainTree, string year, string name){//finds a node with this specific year and name
    vector<Node<MovieData,string>*> searchVect;// vector of nodes with the same name as the name passed as parameter
    searchKeyVect(mainTree->Root(), move(name), searchVect);// starting at root, searches a tree recursively for the name, adds positive results to searchVect
    if(searchVect.empty()) return nullptr;// if no results were found, return nullptr
    else{//otherwise
        for(auto i : searchVect){//check each result in the searchVect to see if the year matches
            if(toLowerCase(i->Data().year) == year) return i;// returns the node if the year matches

        }
        return nullptr;//if no matches were found return nullptr
    }
}
template <typename DATATYPE>
void searchKeyVect(Node<DATATYPE, string> * node, string searchTerm, vector<Node<DATATYPE,string>*> &searchVect){//starting at root, searches a tree recursively for the name, adds positive results to searchVect
    if(node != nullptr) {//if this node is not nullptr
        if(node->Key() == searchTerm) searchVect.push_back(node);//if the key matches the search term, add it to the vector
        if (node->Left() != nullptr) searchKeyVect(node->Left(), searchTerm, searchVect);//recursively search left subtree
        if (node->Right() != nullptr) searchKeyVect(node->Right(), searchTerm,searchVect);//recursively search right subtree
    }
}