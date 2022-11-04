#ifndef BACKEND_H   // To make sure you don't declare the function more than once by including the header multiple times.
#define BACKEND_H
#include <string>
using namespace std;
extern string path;
extern fstream file;

// create file
void createFile(string path);
// check if file existss
bool dbExists(string path);

// node class
class Node
{
    // nodes must have at least t-1 keys and most 2t -1
    // this does not apply to roots
    // we set these bounds with t/minimum degree
    int* keys; // array of keys
    string* values; // array of values
    Node** children; // array of pointers for children 
    int t; // minimum degree
    int numKey; // number of keys
    bool leaf; // bool to check if it's a leaf node 

// constructor
// deg is to distinguish itself from int t
    public: Node(int deg, bool isLeaf);

      // traverse all nodes in a tree
      void navigateTree();

      // write tree to file
      void storeTree();

      // splits children of a node if full
      void split(int i, Node* y);
      
      // insert into node if not full
      void insertInVacant(int key, string value);

      // find a specified key
      int findKey(int k);

      // remove key from a node
      void remove(int k);

      // remove key if the node is a leaf
      void removeIfLeaf(int i);

      // remove key if the node is not a leaf
      void removeIfNotLeaf(int i);

      // get the predecessor of a key
      int getPredecessor(int i);

      // get the successor of a key
      int getSuccessor(int i);

      // fill a child node if the child contains less than 1-t keys
      void fillChildNode(int i);

      // borrow a key from the previous node and move it to the current node
      void borrowFromPrevious(int i);

      // borrow a key from the next node and move it to the current node
      void borrowFromNext(int i);

      // merge child with the next child of the node
      void mergeChildren(int i);

      // search for a key
      Node* search(int k);

      // lets us access node parameters in Tree functions
      friend class Tree;
};

// tree class
class Tree {
    Node* root; // pointer to access the root
    int t; // min degree
    // constructor
    // we use this to initialise a tree as an empty tree
    public: Tree(int deg) { root = NULL; t = deg; };
      // insert into b tree
      void insert(int k, string v);

      // remove key and it's associated val from b tree
      void remove(int k);

      // populate tree
      void populateTree(string path);

      // write tree to file by looping through the tree and storing each value to file similar to how we print the tree
      void storeTree()
      {
          if (root != NULL) root->storeTree();
      }

      Node* search(int k) {
          return (root == NULL) ? NULL : root->search(k);
      }

      void navigateTree()
      {
          if (root != NULL) root->navigateTree();
      }
};

#endif