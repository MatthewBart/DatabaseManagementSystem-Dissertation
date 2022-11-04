#include <cstddef>
#include <string>
#include <iostream>
#include <fstream>
#include "Backend.h"

string path;
fstream file;

// create new file
void createFile(string path) {
    // we use ofstream here as ifstream throws an exception is file isn't found
    ofstream newFile(path + ".txt");
    // close new file
    newFile.close();
}

// check if a database file exists
bool dbExists(string path)
{
    // returns false is theres no stream associated with the file
    file.open(path + ".txt");
    if(file.is_open())
    {
        file.close();
        return true;
    }
    else {
        file.close();
        return false;
    }

}
// node constructor for node class
Node:: Node(int deg, bool isLeaf) {
    t = deg;
    leaf = isLeaf;

    // sets the max number of keys possible and children
    keys = new int[2 * t - 1];
    values = new string[2 * t - 1];
    children = new Node * [2 * t];
    // set starting number of keys to 0
    numKey = 0;

};

// for navigating through all nodes that derive from this node 
// and prints their keys and values
// Node:: to show navigate() belongs to the node class and isn't derived from other classes like tree
void Node::navigateTree(){
    int i;
    for (i = 0; i < numKey; i++) {
        if (leaf == false) {
            // if it isn't a leaf then navigate into it's rubroot child
            children[i]->navigateTree();
        }
        // if it's a leaf node print it's key and value
        cout << keys[i] << " " + values[i] << endl;
    }
    // print last childs key and value
    if (leaf == false) {
        children[i]->navigateTree();
    }
        
}

// populates the tree with the key and values
// function is built on the idea that the file is just a key and a value on each line
void Tree::populateTree(string path)
{
    // opens file
    file.open(path + ".txt");
    // while loop until the file is at the end
    while (!file.fail())
    {
        int key;
        string value;
        // take values from file then insert into tree
        file >> key >> value;
        insert(key, value);    
    }
    // close file
    file.close();
}

void Node::storeTree()
{
    int i;
    for (i = 0; i < numKey; i++) {
        if (leaf == false) {
            children[i]->storeTree();
        }
        // if it's a leaf node print it's key and value
        file << keys[i] << ' ' << values[i] << endl;
    }
    // print last childs key and value
    if (leaf == false) {
        children[i]->storeTree();
    }  
}

// main insert function for inserting a key/val
void Tree::insert(int k,string v)
{
    if (v.empty() == false) {
        // if tree is empty
        if (root == NULL)
        {
            // create and populate a new root
            root = new Node(t, true);
            root->keys[0] = k;  // insert key
            root->values[0] = v; // insert value
            root->numKey = 1;  // update number of keys in root
        }
        else // if tree isnt empty
        {
            // if the root is full create a new root and increase the height of the B tree
            if (root->numKey == 2 * t - 1)
            {
                // create a new root and make the current root the child of this new root
                Node* newRoot = new Node(t, false);
                newRoot->children[0] = root;
                // then split the old root/now child root and move the key
                newRoot->split(0, root);

                // allocate a new key/val to one of the child nodes
                int i = 0;
                if (newRoot->keys[0] < k) {
                    i++;
                }
                newRoot->children[i]->insertInVacant(k, v);

                // set the new root
                root = newRoot;
            }
            // insert the key val into a non full root
            else {
                root->insertInVacant(k, v);
            }
        }
    }
        
}

// inserts if node isn't full
void Node::insertInVacant(int k,string v)
{
    // rightmost element
    int i = numKey - 1;

    // if leaf node
    if (leaf == true)
    {
        // gets position for a new key and value then pushes all other keys and values up to make space
        while (i >= 0 && keys[i] > k)
        {
            keys[i + 1] = keys[i];
            values[i + 1] = values[i];
            i--;
        }

        // insert key/val at new location
        keys[i + 1] = k;
        values[i + 1] = v;
        numKey = numKey + 1;
    }
    else // if not a leaf node
    {
        // find location of child to recieve a new key
        while (i >= 0 && keys[i] > k) {
            i--;
        }
        // check if empty
        if (children[i + 1]->numKey == 2*t - 1)
        {
            // split if full
            split(i + 1, children[i + 1]);

            // find a child to insert the new key and value into
            if (keys[i + 1] < k) {
                i++;
            }
        }
        // insert key val into node with space
        children[i + 1]->insertInVacant(k,v);
    }
}

// splits child of a node
void Node::split(int i, Node *node)
{
    // create a new node
    // nodes must have at least t-1 keys

    Node *newNode = new Node(node->t, node->leaf);
    newNode->numKey = t - 1;

    // copy last key to the new node
    for (int j = 0; j < t - 1; j++) {
        newNode->keys[j] = node->keys[j + t];
        newNode->values[j] = node->values[j + t];
    }
        
    // copy the children over to the new node
    if (node->leaf == false)
    {
        for (int j = 0; j < t; j++) {
            newNode->children[j] = node->children[j + t];
        }
            
    }

    // nodes must have at least t-1 keys 
    node->numKey = t - 1;

    // create space and set the new child node
    for (int j = numKey; j >= i + 1; j--) {
        children[j + 1] = children[j];

    }
    children[i + 1] = newNode;

    // find the new key location and move the rest of the key by 1
    for (int j = numKey - 1; j >= i; j--) {
        keys[j + 1] = keys[j];
        values[j + 1] = values[j];
    }
        

    // move the middle key to thise node
    keys[i] = node->keys[t - 1];
    values[i] = node->values[t - 1];

    // key amount ++
    numKey += 1;
}

// find a specific key
int Node::findKey(int k)
{
    int i = 0;
    while (i < numKey && keys[i] < k) {
        ++i;
    }
    return i;
}

// remove a key
void Node::remove(int k)
{
    // find the specific node
    int i = findKey(k);

    // if key is present
    if (i < numKey && keys[i] == k)
    {
        // handle removal depending depending on if it's a leaf or not
        if (leaf) {
            removeIfLeaf(i);
        }
        else {
            removeIfNotLeaf(i);
        }
            
    }
    // if key isn't present
    else
    {
        // key can't be present if it's a leaf node
        if (leaf)
        {
            return;
        }
        // indiciates if key is present in sub tree 
        bool flag = ((i == numKey) ? true : false);

        // filld child with key if less than minimum degree
        if (children[i]->numKey < t) {
            fillChildNode(i);
        }
            
        // we check the flag to see if the key is present in the subtree
        // if so then we must go to the previous child as it must have been merged with the last child
        if (flag && i > numKey) {
            children[i - 1]->remove(k);
        }
        else {
            children[i]->remove(k);
        }     
    }
    return;
}

// remove key/val if it's a leaf
void Node::removeIfLeaf(int i)
{
    //move all keys and values back by 1
    for (int j = (i + 1); j < numKey; ++j) {
        keys[j - 1] = keys[j];
        values[j - 1] = values[j];
    }
    numKey--;
    return;
}

// remove key/val if not a node
void Node::removeIfNotLeaf(int i)
{
    int k = keys[i];

    // if children[is] predecessor has t or more keys then replace keys with predecessor
    // and delete the predecessor from children
    if (children[i]->numKey >= t)
    {
        int predecessor = getPredecessor(i);
        keys[i] = predecessor;
        children[i]->remove(predecessor);
    }

    // if children[i] doesn't have enough then check children[i+1]
    // if children[i+1] has enough keys then find it's successor and replace keys[i] with the successor
    // and remove the successor
    else if (children[i + 1]->numKey >= t)
    {
        int successor = getSuccessor(i);
        keys[i] = successor;
        children[i + 1]->remove(successor);
    }

    // if neither of these options have enough keys then merge them and delete key from children[i]
    else
    {
        mergeChildren(i);
        children[i]->remove(k);
    }
    return;
}

// handle predecessor of a key
int Node::getPredecessor(int i)
{
    // loop right until a leaf is encountered 
    Node* currentNode = children[i];
    while (!currentNode->leaf) {
        currentNode = currentNode->children[currentNode->numKey];
    }
   

    // return the last key
    cout << currentNode->numKey;
    return currentNode->keys[currentNode->numKey - 1];
}

// handle successor of a key
int Node::getSuccessor(int i)
{
    // loop left from children[i+1] until a leaf is reached 
    Node* currentNode = children[i + 1];
    while (!currentNode->leaf) {
        currentNode = currentNode->children[0];
    }
        
    // return the first key
    return currentNode->keys[0];
}

// handlie filling a child node which has less than t-1 keys
void Node::fillChildNode(int i)
{
    // borrow a key from the previous child if it has more than t-1 keys
    if (i != 0 && children[i - 1]->numKey >= t) {
        borrowFromPrevious(i);
    }
    // borrow a key from the next child if it has more than t-1 keys
    else if (i != numKey && children[i + 1]->numKey >= t) {
        borrowFromNext(i);
    }

    // merge children depending on whether children[i] is the last child of a node or not
    else
    {
        if (i != numKey) {
            mergeChildren(i);
        }  
        else {
            mergeChildren(i - 1);
        }
    }
    return;
}

// handle borrowing a key/val from the previous child and insert it into the current child
void Node::borrowFromPrevious(int i)
{
    // set the child and sibling
    Node* child = children[i];
    Node* sibling = children[i - 1];

    // move keys and values from children by one position
    for (int i = child->numKey - 1; i >= 0; --i) {
        child->keys[i + 1] = child->keys[i];
        child->values[i + 1] = child->values[i];
    }

    // if not a leaf move all children one position ahead
    if (!child->leaf)
    {
        for (int j = child->numKey; j >= 0; --j) {
            child->children[j + 1] = child->children[j];
        }
            
    }

    // set child's first key/vale to [i-1]
    child->keys[0] = keys[i - 1];
    child->values[0] = values[i - 1];

    // move the siblings last child to the childrens[i] first child
    if (!child->leaf) {
        child->children[0] = sibling->children[sibling->numKey];
    }
        
    // Moving the keys and values from the sibling to the parent
    keys[i - 1] = sibling->keys[sibling->numKey - 1];
    values[i - 1] = sibling->values[sibling->numKey - 1];


    // increase childs key
    // decrease siblings key
    child->numKey += 1;
    sibling->numKey -= 1;

    return;
}

// handle borrowing a key/val from children[i+1]
void Node::borrowFromNext(int i)
{

    Node* child = children[i];
    Node* sibling = children[i + 1];

    // keys[i] and values[i] are inserted as the last values in C[i]
    child->keys[(child->numKey)] = keys[i];
    child->values[(child->numKey)] = values[i];

    // insert first child of sibling as the last child of children[]
    if (!(child->leaf)) {
        child->children[(child->numKey) + 1] = sibling->children[0];
    }
        

    // insert first key/val of sibling into keys[i]
    keys[i] = sibling->keys[0];
    values[i] = sibling->values[0];

    // move all sibling keys and values back by one
    for (int i = 1; i < sibling->numKey; ++i) {
        sibling->keys[i - 1] = sibling->keys[i];
        sibling->values[i - 1] = sibling->values[i];
    }
        
    // move all child pointers back bby 1
    if (!sibling->leaf)
    {
        for (int i = 1; i <= sibling->numKey; ++i) {
            sibling->children[i - 1] = sibling->children[i];
        }
            
    }

    // increase child key count and reduce sibling key count
    child->numKey += 1;
    sibling->numKey -= 1;

    return;
}

// handles merging children[i] and [i+1]
void Node::mergeChildren(int i)
{
    // define both children
    Node* child = children[i];
    Node* sibling = children[i + 1];

    // insert key and value from current node and insert into the t-1 position of children[i]
    child->keys[t - 1] = keys[i];
    child->values[t - 1] = values[i];

    // copy the keys and values from the children[i+1] to current children[i]
    for (int i = 0; i < sibling->numKey; ++i) {
        child->keys[i + t] = sibling->keys[i];
        child->values[i + t] = sibling->values[i];
    }
        

    // copy over the child pointers
    if (!child->leaf)
    {
        for (int i = 0; i <= sibling->numKey; ++i)
            child->children[i + t] = sibling->children[i];
    }

    // move all keys/values after i one step ahead to fill gaps created from moving the keys
    for (int j = i + 1; j < numKey; ++j) {
        keys[j - 1] = keys[j];
        values[j - 1] = values[j];
    }
        

    // move child pointers a step back 
        for (int j = i + 2; j <= numKey; ++j) {
            children[j - 1] = children[j];
    }
        

    // handle key count for both the child and sibling
    child->numKey += sibling->numKey + 1;
    numKey--;

    // delete the sibling
    delete(sibling);
    return;
}

// handles searching for a key in the nodes subtree
Node* Node::search(int k)
{
    // find a key greater than or equal to k
    int i = 0;
    while (i < numKey && k > keys[i]) {
        i++;
    }
        
    // if key is found then return this node
    if (keys[i] == k) {
        cout << endl << "Key: " << keys[i] << " Value: " << values[i];
        return this;
    }
        
    // if the key isn't found and it's a leaf then return null
    if (leaf == true) {
        return NULL;
    }
        

    // search for the key in ths next child
    return children[i]->search(k);
}

// remove a key/val
void Tree::remove(int k)
{
    if (!root)
    {
        return;
    }

    // call the function to remove from root
    root->remove(k);

    // if the root contains 0 keys then replace the root with it's first child if there is on otherwise set it to NULL
    if (root->numKey == 0)
    {
        Node* tmp = root;
        if (root->leaf)
            root = NULL;
        else
            root = root->children[0];

        // delete the old root
        delete tmp;
    }
    return;
}

