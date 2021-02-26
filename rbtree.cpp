/****************************
* Author: Uzay Uysal        *
* Student Number: 150180039 *
****************************/


#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <assert.h>
using namespace std;


//Simple node structure to represent players in a RBTree.
struct Node
{
    //Player related 
    string name;
    int points;
    int rebounds;
    int assists;
    
    //RBTree related
    Node* left;
    Node* right;
    Node* parent;
    int color;      //0 => black, 1 => red
    Node();
    ~Node();
};

Node::Node()
{
    left = NULL;
    right = NULL;
    parent = NULL;
}

Node::~Node()
{
    delete left;
    delete right;
}

//Red-Black Tree to insert player nodes which uses player names to compare the nodes.
struct RBTree
{
    RBTree();
    Node* root;
    void insert(string, int, int, int);
    void insertfix(Node*);
    Node* search(string);
    Node* search_helper(Node*, string);
    void preorder();
    void preorder_helper(Node*, int);
    void inorder();
    void inorder_helper(Node*);
    Node* get_parent(Node*);
    Node* get_grandparent(Node*);
    Node* get_uncle(Node*);
    Node* get_sibling(Node*);
    void rotate_left(Node*);
    void rotate_right(Node*);
    ~RBTree();
};

Node* RBTree::get_parent(Node* node)
{
    return node == NULL ? NULL : node->parent;
}

Node* RBTree::get_grandparent(Node* node)
{
    return get_parent(get_parent(node));
}

Node* RBTree::get_uncle(Node* node)
{
    return get_sibling(get_parent(node));
}

Node* RBTree::get_sibling(Node* node)
{
    Node* parent = get_parent(node);
    if(parent == NULL) return NULL;
    if(node == parent->left) return parent->right;
    else return parent->left;
}

void RBTree::rotate_left(Node* node)
{
    Node* temp = node->right;
    Node* parent = get_parent(node);
    //assert(temp != NULL); assertion to check if something impossible is happening
    node->right = temp->left;
    temp->left = node;
    node->parent = temp;

    if(node->right != NULL) node->right->parent = node;

    if(parent != NULL)
    {
        if(node == parent->left) parent->left = temp;
        else if (node == parent->right) parent->right = temp;
    }
    temp->parent = parent;
}   

void RBTree::rotate_right(Node* node)
{
    Node* temp = node->left;
    Node* parent = get_parent(node);
    //assert(temp != NULL); assertion to check if something impossible is happening

    node->left = temp->right;
    temp->right = node;
    node->parent = temp;

    if(node->left != NULL) node->left->parent = node;

    if(parent != NULL)
    {
        if(node == parent->left) parent->left = temp; 
        else if (node == parent->right) parent->right = temp;
    }
    temp->parent = parent;
}

//Search function that calls a helper function that does BST Search over the tree.
//Since RBTrees are a special case of BST trees this works.
Node* RBTree::search(string name)
{
    Node* result = search_helper(root, name);
    if (result) return result;
    else return NULL;  //Later on it's possible to do something else if we can't find the value (like printing something to the console)
}

//Helper function that does BST Search.
Node* RBTree::search_helper(Node* root, string name)
{
    if ((root == NULL) || (root->name == name)) return root;    //Check if the root exists or we found the right value.

    if (root->name < name)                          //If the key is bigger than the root then search in the right subtree.
    {
        return search_helper(root->right, name);
    }
    return search_helper(root->left, name);         //If not search in the left subtree.
}

void RBTree::preorder()
{
    preorder_helper(root, 0);
}

void RBTree::preorder_helper(Node* root, int level)
{
    if(!root) return;
    for (int i = 0; i < level; i++)
    {
        cout << '-';
    }
    if(root->color == 0)
    {
        cout << "(BLACK) " << root->name << endl;
    }
    else
    {
        cout << "(RED) " << root->name << endl;
    }
    preorder_helper(root->left, level+1);
    preorder_helper(root->right, level+1); 
}

void RBTree::insert(string name, int points, int rebounds, int assists)
{
    Node* temp = new Node;
    temp->name = name;
    temp->points = points;
    temp->rebounds = rebounds;
    temp->assists = assists;
    temp->color = 1;

    Node* front = root;
    Node* behind = root;
    
    while(front)
    {
        behind = front;
        if(front->name == name)
        {
            front->points += points;
            front->rebounds += rebounds;
            front->assists += assists;
            return;
        }
        if(name > front->name) front = front->right;
        else front = front->left;
    }
    if(behind == NULL) root = temp;
    else if (behind->name > name) behind->left = temp;
    else behind->right = temp;
    temp->parent = behind;
    
    insertfix(temp);

    while(get_parent(root) != NULL) root = get_parent(root);
}

void RBTree::insertfix(Node* node)
{
    if (get_parent(node) == NULL) node->color = 0; //If node is root fix the color
    else if (get_parent(node)->color == 0) return; //If node's parent is black, no violation
    else if (get_uncle(node) != NULL && get_uncle(node)->color == 1) //If node's uncle exists and it is red do recolor
    {
        get_parent(node)->color = 0;
        get_uncle(node)->color = 0;
        get_grandparent(node)->color = 1;
        insertfix(get_grandparent(node));
    }
    else //If the uncle is black do rotation fixes
    {
        Node* parent = get_parent(node);
        Node* grandparent = get_grandparent(node);

        //           G
        //         // \\
        //        P     U
        //         \\
        //           N
        if(node == parent->right && parent == grandparent->left) 
        {
            rotate_left(parent);
            node = node->left;
        }
        //           G
        //         // \\
        //        U     P
        //            //
        //           N
        else if (node == parent->left && parent == grandparent->right)
        {
            rotate_right(parent);
            node = node->right;
        }

        parent = get_parent(node);
        grandparent = get_grandparent(node);

        if(node == parent->left) rotate_right(grandparent);
        else rotate_left(grandparent);
        parent->color = 0;
        grandparent->color = 1;
    }
}

void RBTree::inorder()
{
    inorder_helper(root);
}

void RBTree::inorder_helper(Node* root)
{
    if(root == NULL) return;
    inorder_helper(root->left);
    cout << root->name << " ";
    inorder_helper(root->right);
}

RBTree::RBTree()
{
    root = NULL;
}

RBTree::~RBTree()
{
    delete root;
}

int main(int argc, const char** argv) {

    string filename = argv[1];
    fstream file;
    file.open(filename, ios_base::in);

    bool once = true, sonce = true; 
    int points, assists, rebounds, mp = 0, ma = 0, mr = 0;
    string line, oldseason, season, name, team, mps, mas, mrs;  
    getline(file, line);      //Read the first dump line

    RBTree* tree = new RBTree();
    while(getline(file, line))
    {
        stringstream ss(line);    //Pass the line to the ss
        getline(ss, season, ','); //Read from ss using comma as delimiter
        getline(ss, name, ','); 
        getline(ss, team, ',');
        getline(ss, line, ',');  rebounds = stoi(line);
        getline(ss, line, ',');  assists = stoi(line);
        getline(ss, line); points = stoi(line);
        if (sonce)
        {
            oldseason = season;
            sonce = false;
        }
        if (oldseason != season)
        {
            cout << "End of the " << oldseason << " Season" << endl;
            cout << "Max Points: " << mp << " - Player Name: " << mps << endl;
            cout << "Max Assists: " << ma << " - Player Name: " << mas << endl;
            cout << "Max Rebs: " << mr << " - Player Name: " << mrs << endl;
            if(once) 
            {
                tree->preorder();
                once = false;
            }
        }
        Node* search_res = tree->search(name);
        if(search_res)
        {
            points += search_res->points;
            rebounds += search_res->rebounds;
            assists += search_res->assists; 
        }
        if(points > mp)
        {
            mp = points;
            mps = name;
        }
        if(assists > ma)
        {
            ma = assists;
            mas = name;
        }
        if(rebounds > mr)
        {
            mr = rebounds;
            mrs = name;
        }
        if(search_res)
        {
            points -= search_res->points;
            rebounds -= search_res->rebounds;
            assists -= search_res->assists; 
        }
        tree->insert(name, points, rebounds, assists);
        oldseason = season;
    }

    cout << "End of the " << oldseason << " Season" << endl;
    cout << "Max Points: " << mp << " - Player Name: " << mps << endl;
    cout << "Max Assists: " << ma << " - Player Name: " << mas << endl;
    cout << "Max Rebs: " << mr << " - Player Name: " << mrs << endl;
    //tree->inorder();
    cout << endl;
    delete tree;
       
    return 0;
}