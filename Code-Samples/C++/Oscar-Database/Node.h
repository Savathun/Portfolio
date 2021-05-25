//
// Created by Daniel Schlatter on 11/8/2018.
//
#ifndef NODE_
#define NODE_
#include <iostream>
#include <string>
using namespace std;
//Binary Tree Node
template <typename DATATYPE, typename KEYTYPE>
class Node {
private:
    KEYTYPE key; //Key for sorting the tree
    DATATYPE data;
    Node<DATATYPE, KEYTYPE> * left;//ptr to left child
    Node<DATATYPE, KEYTYPE> * right;//ptr to right child
public:
    Node() {left=nullptr; right=nullptr; };//constructor
    void setKey(KEYTYPE aKey) { key = aKey; };//setter for key
    void setData(DATATYPE aData) { data = aData; }//setter for data
    void setLeft(Node<DATATYPE, KEYTYPE> * aLeft) { left = aLeft; };//setter for left child
    void setRight(Node<DATATYPE, KEYTYPE> * aRight) { right = aRight; };//setter for right child
    KEYTYPE Key() { return key; };// getter for key
    DATATYPE Data() { return data; }// getter for data
    Node<DATATYPE, KEYTYPE> * Left() { return left; };//getter for left child
    Node<DATATYPE, KEYTYPE> * Right() { return right; };//getter for right child
};

#endif
