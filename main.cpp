#include <iostream>
#include <fstream>
using namespace std;

class treeNode {
public:
    string chStr;
    int prob;
    string code;
    treeNode* left;
    treeNode* right;
    treeNode* next;
public:
    treeNode(string, int, string, treeNode*, treeNode*, treeNode*);
    void printNode(ofstream&);
};

class linkedList {
protected:
    treeNode* listHead;
public:
    linkedList();
    treeNode* getListHead() {return listHead;};
    void setListHead(treeNode*);
    void insertNewNode(treeNode*);
    treeNode* findSpot(treeNode*);
    void insert(treeNode*, treeNode*);
    void printList(ofstream&);
};

class HuffmanBinaryTree : public linkedList {
private:
    treeNode* root;
public:
    HuffmanBinaryTree(){};
    treeNode* getRoot() {return root;};
    void constructHuffmanLList(ifstream&, ofstream&);
    void constructHuffmanBinTree(ofstream&);
    void preOrderTraversal(treeNode*, ofstream&);
    void inOrderTraversal(treeNode*, ofstream&);
    void postOrderTraversal(treeNode*, ofstream&);
    void constructCharCode(treeNode*, string, ofstream&);
    bool isLeaf(treeNode*);
};

int main(int argc, char** argv) {
    if(argc == 5) {

        // open input and output files
        ifstream inFile;
        inFile.open(argv[1]);
        ofstream outFile1, outFile2, outFile3;
        outFile1.open(argv[2]);
        outFile2.open(argv[3]);
        outFile3.open(argv[4]);

        // create Huffman object
        HuffmanBinaryTree tree;

        // construct Huffman linked list
        tree.constructHuffmanLList(inFile, outFile3);

        // construct Huffman binary tree
        tree.constructHuffmanBinTree(outFile3);

        // output pre-order traversal
        outFile2 << "The pre-order traversal of the constructed Huffman Binary tree:\n";
        tree.preOrderTraversal(tree.getRoot(), outFile2);

        // output in-order traversal
        outFile2 << "\nThe in-order traversal of the constructed Huffman Binary tree:\n";
        tree.inOrderTraversal(tree.getRoot(), outFile2);

        // output post-order traversal
        outFile2 << "\nThe post-order traversal of the constructed Huffman Binary tree:\n";
        tree.postOrderTraversal(tree.getRoot(), outFile2);

        // output character code
        tree.constructCharCode(tree.getRoot(), "", outFile1);

        // close input and output files
        inFile.close();
        outFile1.close();
        outFile2.close();
        outFile3.close();
    }
    else {
        cout << "Invalid number of arguments.";
    }
} // end main


// treeNode constructor
treeNode::treeNode(string s, int p, string c, treeNode* l, treeNode* r, treeNode* n) {
    chStr = s;
    prob = p;
    code = c;
    left = l;
    right = r;
    next = n;
} // end treeNode constructor


// linkedList constructor, set dummy node
linkedList::linkedList() {
    listHead = new treeNode("dummy", 0, "", nullptr, nullptr, nullptr);
} // end linkedList


// set listHead to newHead
void linkedList::setListHead(treeNode* newHead) {
    listHead->next = newHead;
} // end setListHead()


// inserts given node into linked list after the returned spot
void linkedList::insertNewNode(treeNode* newNode) {
    treeNode* spot = findSpot(newNode);
    insert(spot, newNode);
} // end insertNewNode()


// returns pointer where newNode will be added
treeNode* linkedList::findSpot(treeNode* newNode) {
    treeNode* spot = listHead;
    // while not at end of list, and prob of current node is less than newNode
    while(spot->next != nullptr && spot->next->prob < newNode->prob) {
        spot = spot->next;
    }
    return spot;
} // end findSpot()


// change pointers for insertNewNode
void linkedList::insert(treeNode* spot, treeNode* newNode) {
    newNode->next = spot->next;
    spot->next = newNode;
} // end insert()


// construct Huffman linked list
void HuffmanBinaryTree::constructHuffmanLList(ifstream& inFile, ofstream& outFile) {
    string chr;
    int prob;
    while(inFile >> chr >> prob) {
        treeNode* newNode = new treeNode(chr, prob, "", nullptr, nullptr, nullptr);

        insertNewNode(newNode);

        printList(outFile);
    }
} // end constructHuffmanLList()


// construct Huffman binary tree
void HuffmanBinaryTree::constructHuffmanBinTree(ofstream& outFile) {
    while(listHead->next->next != nullptr) {
        treeNode* newNode = new treeNode(listHead->next->chStr + listHead->next->next->chStr,
                                        listHead->next->prob + listHead->next->next->prob,
                                        "",
                                        listHead->next,
                                        listHead->next->next,
                                        nullptr);
        insertNewNode(newNode);
        setListHead(listHead->next->next->next);

        printList(outFile);
    }
    root = listHead->next;
} // end constructHuffmanBinTree()


// returns true if given node is a leaf, false otherwise
bool HuffmanBinaryTree::isLeaf(treeNode* T) {
    if(T->left == nullptr && T->right == nullptr) return true;
    else return false;
} // end isLeaf()


// outputs pre-order traversal
void HuffmanBinaryTree::preOrderTraversal(treeNode* T, ofstream& outFile) {
    if(isLeaf(T)) {
        T->printNode(outFile);
    } else {
        T->printNode(outFile);
        if(T->left != nullptr) preOrderTraversal(T->left, outFile);
        if(T->right != nullptr) preOrderTraversal(T->right, outFile);
    }
} // end preOrderTraversal()


// outputs in-order traversal
void HuffmanBinaryTree::inOrderTraversal(treeNode* T, ofstream& outFile) {
    if(isLeaf(T)) {
        T->printNode(outFile);
    } else {
        if(T->left != nullptr) inOrderTraversal(T->left, outFile);
        T->printNode(outFile);
        if(T->right != nullptr) inOrderTraversal(T->right, outFile);
    }
} // end inOrderTraversal()


// outputs post-order traversal
void HuffmanBinaryTree::postOrderTraversal(treeNode* T, ofstream& outFile) {
    if(isLeaf(T)) {
        T->printNode(outFile);
    } else {
        if(T->left != nullptr) postOrderTraversal(T->left, outFile);
        if(T->right != nullptr) postOrderTraversal(T->right, outFile);
        T->printNode(outFile);
    }
} // end postOrderTraversal()


// construct and output character codes
void HuffmanBinaryTree::constructCharCode(treeNode* T, string c, ofstream& outFile) {
    if(isLeaf(T)) {
        T->code = c;
        outFile << T->chStr << " " << T->code << endl;
    } else {
        constructCharCode(T->left, c+"0", outFile);
        constructCharCode(T->right, c+"1", outFile);
    }
} // end constructCharCode()


// prints linked list to outFile starting at head
void linkedList::printList(ofstream& outFile) {
    // list navigation pointer
    treeNode* nav = listHead;

    outFile << "listHead--> ";
    while(nav->next != nullptr) {
        outFile << "(\"" << nav->chStr << "\", " << nav->prob << ", \"" <<
            nav->next->chStr << "\")--> ";

        nav = nav->next;
    }
    outFile << "(\"" << nav->chStr << "\", " << nav->prob << ", NULL)--> NULL\n";
} // end printList()


// print node information in format:
// (chStr, prob, next, left, right)
void treeNode::printNode(ofstream& outFile) {
    outFile << "(\"" << chStr << "\", ";
    outFile << prob << ", ";
    if(next != nullptr) outFile << "\"" << next->chStr << "\", ";
    else outFile << "NULL, ";
    if(left != nullptr) outFile << "\"" << left->chStr << "\", ";
    else outFile << "NULL, ";
    if(right != nullptr) outFile << "\"" << right->chStr << "\")\n";
    else outFile << "NULL)\n";
} // end printNode()
