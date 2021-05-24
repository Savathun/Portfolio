//Daniel Schlatter
// added print to file functions. deleted max and findnode functions
//BSTree functions file
#include "BSTree.h"

// Constructor
template <typename DATATYPE, typename KEYTYPE>
BSTree<DATATYPE, KEYTYPE>::BSTree() {
    root = nullptr;
}

// Destructor
template <typename DATATYPE, typename KEYTYPE>
BSTree<DATATYPE, KEYTYPE>::~BSTree() {
    if (root !=nullptr) freeNode(root);
}
// Free the node (private)
template <typename DATATYPE, typename KEYTYPE> 6 11 12 13 22 23 24 25
void BSTree<DATATYPE, KEYTYPE>::freeNode(Node<DATATYPE, KEYTYPE> * leaf){
    if(Root() == leaf) deleteNode(leaf->Key()); //if the root is the leaf, delete that leaf
    else if(leaf != nullptr){ // otherwise if the leaf is not null
        freeNode(leaf->Left()); //recursive call of the leaf's left
        freeNode(leaf->Right()); //recursive call of the leaf's right
        deleteNode(leaf, leaf->Key()); //now delete the leaf
    }
}

// Add a node
template <typename DATATYPE, typename KEYTYPE>
void BSTree<DATATYPE, KEYTYPE>::addNode(KEYTYPE key, const DATATYPE &data){// values to be passed to the node that needs to be added
    if (Root() == nullptr){//if tree is empty
        auto * newNodePtr = new Node<DATATYPE, KEYTYPE>;//create new node
        newNodePtr->setKey(key);//set key to key passed as parameter
        newNodePtr->setData(data);// set dwata to data passed as parameter
        root = newNodePtr;//set the root to the new node
    }
    else addNode(key, root, data);//otherwise, pass the root and the other parameters to the addNode function
}

// Add a node (private)
template <typename DATATYPE, typename KEYTYPE>
void BSTree<DATATYPE, KEYTYPE>::addNode(KEYTYPE key, Node<DATATYPE, KEYTYPE> * leaf, const DATATYPE& data) {//values to be passed to the node that needs to be added, current node we are checking in the tree
    if(key < leaf->Key()) {//if key is less than lefts key
        if (leaf->Left() != nullptr) addNode(key, leaf->Left(), data);//if left is not null, add to left recursively
        else {//otherwise
            auto * newNodePtr = new Node<DATATYPE, KEYTYPE>;//create new node
            newNodePtr->setKey(key);//set key to key passed as parameter
            newNodePtr->setData(data);//set data to data passed as parameter
            leaf->setLeft(newNodePtr);// set this nodes left to the new node
        }
    }
    else if (leaf->Right() != nullptr) addNode(key, leaf->Right(), data);// if right is not null, add to right recursively
    else {//otherwise
        auto * newNodePtr = new Node<DATATYPE, KEYTYPE>;//create new node
        newNodePtr->setKey(key);//set key to key passed as parameter
        newNodePtr->setData(data);//set data to data passed as parameter
        leaf->setRight(newNodePtr);// set this nodes right to the new node
    }
}
template <typename DATATYPE, typename KEYTYPE>
void BSTree<DATATYPE, KEYTYPE>::printInOrderConsole(){//prints whole tree to console
    cout << header << endl;//prints the category headers first
    printInOrderConsole(root);//then prints tree
}
//private
template <typename DATATYPE, typename KEYTYPE>
void BSTree<DATATYPE, KEYTYPE>::printInOrderConsole(Node<DATATYPE, KEYTYPE> *node){//prints node and both its subtrees to console using an in order, depth first search
    if(node != nullptr) {//if this node is not nullptr
        if (node->Left() != nullptr) printInOrderConsole(node->Left());//recursively print left subtree
        printToConsole(node->Data());//print the node
        if (node->Right() != nullptr) printInOrderConsole(node->Right());//recursively print right subtree
    }
}

template <typename DATATYPE, typename KEYTYPE>
void BSTree<DATATYPE, KEYTYPE>::printToConsole(const ActorData &data){//function for printing ActorData nodes' data to the console
    cout << data.year << "\t" << data.award;
    if(data.award == "Actor in a Leading Role") cout << "\t";//formatting to make everything  in the same column
    cout << "\t" << data.winner << "\t" << data.name;
    if(data.name.length() <= 23) cout << "\t";//formatting to make everything  in the same column
    if(data.name.length() <= 15) cout << "\t";//formatting to make everything  in the same column
    if(data.name.length() <= 7) cout << "\t";//formatting to make everything  in the same column
    cout << "\t" << data.film.substr(0,36);
    if(data.film.length() >= 37) cout << "...";//formatting to make everything  in the same column
    cout << endl;
}
template <typename DATATYPE, typename KEYTYPE>
void BSTree<DATATYPE, KEYTYPE>::printToConsole(const MovieData &data){//function for printing MovieData nodes' data to the console
    if(data.name.length() >= 29) cout << data.name.substr(0, 27) << "...";//formatting to make everything  in the same column
    else cout << data.name;
    if(data.name.length() <= 23) cout << "\t";//formatting to make everything  in the same column
    if(data.name.length() <= 15) cout << "\t";//formatting to make everything  in the same column
    if(data.name.length() <= 7) cout << "\t";//formatting to make everything  in the same column
    cout << "\t" << data.year << "\t" << data.nominations << "\t" << data.rating << "\t" << data.duration << "\t" << data.genre1;
    if(data.genre1.length() <= 7) cout << "\t";//formatting to make everything  in the same column
    cout << "\t" << data.genre2;
    if(data.genre2.length() <= 7) cout << "\t";//formatting to make everything  in the same column
    cout << "\t" << data.release.substr(0, 3) << "\t" << data.metacritic << "\t";
    if(data.synopsis.length() > 3) cout << data.synopsis.substr(0, 4) << "..." << endl;//formatting to make everything  in the same column
    else cout << data.synopsis << endl;
}
template <typename DATATYPE, typename KEYTYPE>
void BSTree<DATATYPE, KEYTYPE>::printInOrderToFile(ostream & out){//prints whole tree to file passed as parameter
    out << header << endl;//prints header first
    printInOrderToFile(root, out);//prints tree to file, passing root and file as parameter
}
//private
template <typename DATATYPE, typename KEYTYPE>
void BSTree<DATATYPE, KEYTYPE>::printInOrderToFile(Node<DATATYPE, KEYTYPE> * node, ostream & out){//prints node and both its subtrees to file passed as parameter using an in order, depth first search
    if(node != nullptr) {//if nodes not nullptr
        if (node->Left() != nullptr) printInOrderToFile(node->Left(), out);//recursively print left subtree
        printToFile(out, node->Data());//print node
        if (node->Right() != nullptr) printInOrderToFile(node->Right(), out);//recursively print right subtree
    }
}

template <typename DATATYPE, typename KEYTYPE>
void BSTree<DATATYPE, KEYTYPE>::printToFile(ostream & out, const ActorData & data){//prints ActorData nodes to file passed as parameter
    out << data.year << "," << data.award << "," << data.winner << ",";
    if(data.name.find(',') != string::npos) out << '"' << data.name << '"';//adds in quotation marks if name has a comma in it
    else out << data.name;
    out << ",";
    if(data.film.find(',') != string::npos) out << '"' << data.film << '"';// adds in quotation marks if the film has a comma in it
    else out << data.film;
    out << endl;
}
template <typename DATATYPE, typename KEYTYPE>
void BSTree<DATATYPE, KEYTYPE>::printToFile(ostream & out, const MovieData & data){//prints MovieData nodes to file passed as parameter
    out << data.name << "," << data.year << "," << data.nominations << "," << data.rating << "," << data.duration << "," << data.genre1 << "," << data.genre2 << "," << data.release << "," << data.metacritic << ",";
    if(data.synopsis.find('"') != string::npos) out << '"' << data.synopsis << '"';//adds quotes if synopsis has quotes in it
    else out << data.synopsis;
    out << endl;
}
template <typename DATATYPE, typename KEYTYPE>
void BSTree<DATATYPE, KEYTYPE>::deleteNode(KEYTYPE key){//deletes a node with this key
    setRoot(deleteNode(Root(), key));//sets the root to the output of the delete node function
}

//deleteNode (Private)
template <typename DATATYPE, typename KEYTYPE>
Node<DATATYPE, KEYTYPE> * BSTree<DATATYPE, KEYTYPE>::deleteNode(Node<DATATYPE, KEYTYPE> * aRoot,KEYTYPE value){//starting at node passed as parameter, this function will find a node with this ket and delete it
    if (aRoot == nullptr) return aRoot; // base case
    if (value < aRoot->Key()) aRoot->setLeft(deleteNode(aRoot->Left(), value));//if key to be deleted is smaller than aroots key, it lies in left subtree
    else if (value > aRoot->Key()) aRoot->setRight(deleteNode(aRoot->Right(), value));//if the key to be deleted is greater than the aroots key, it lies in right subtree
    else{//if key is same as aroots key, then this is the node to be deleted
        if (aRoot->Left() == nullptr){// node with only one child or no child
            Node<DATATYPE, KEYTYPE> *temp = aRoot->Right();
            delete aRoot;
            return temp;
        }
        else if (aRoot->Right() == nullptr){// node with one child
            Node<DATATYPE, KEYTYPE> *temp = aRoot->Left();
            delete aRoot;
            return temp;
        }
        // node with two children: Get the InOrder successor (smallest in the right subtree)
        Node<DATATYPE, KEYTYPE> * temp = min(aRoot->Right());
        // Copy the InOrder successor's content to this node
        aRoot->setKey(temp->Key());
        aRoot->setData(temp->Data());
        // Delete the InOrder successor
        aRoot->setRight(deleteNode(aRoot->Right(), temp->Key()));
    }
    return aRoot;
}
// Find the node with min key
// Traverse the left sub-BSTree recursively
// till left sub-BSTree is empty to get min
template <typename DATATYPE, typename KEYTYPE>
Node<DATATYPE, KEYTYPE> * BSTree<DATATYPE, KEYTYPE>::min(Node<DATATYPE, KEYTYPE> * node){
    Node<DATATYPE, KEYTYPE> * current = node;
    /* loop down to find the leftmost leaf */
    while (current->Left() != nullptr) current = current->Left();
    return current;
}

