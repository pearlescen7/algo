#include <iostream>
#include <vector>
using namespace std;

struct valContainer
{
    union
    {
        int x;
        int y;
        char z;
    };
    int type;
    bool operator>(const valContainer& rhs)
    {
        if (type == 0) return x > rhs.x;
        else if (type == 1) return y > rhs.y;
        else return z > rhs.z;
    }
    bool operator<(const valContainer& rhs)
    {
        if (type == 0) return x < rhs.x;
        else if (type == 1) return y < rhs.y;
        else return z < rhs.z;
    }
    bool operator==(const valContainer& rhs)
    {
        if (type == 0) return x == rhs.x;
        else if (type == 1) return y == rhs.y;
        else return z == rhs.z;
    }
};

struct Node
{
    vector<vector<valContainer> > vals; //vals[0] => x values; vals[1] => y values; vals[2] => z values
    vector<Node*> children;             //children vector to store child addresses
    //Node* parent;                     //convenience pointer to use in operations like split
    bool is_leaf;                       //to check if the node is a leaf
    Node(bool);
};

Node::Node(bool l)
{   
    //push x, y, z vectors 
    vals.push_back(vector<valContainer>());
    vals.push_back(vector<valContainer>());
    vals.push_back(vector<valContainer>());

    is_leaf = l;
}

struct BTree{
    Node* root;             //root pointer
    int type;               //type = 0 => x; type = 1 => y; type = 2 => z; criteria to use as a key
    int degree;             //degree to determine the max and min number of keys in a node

    void insert(int, int, char);                 //main insertion function
    void insert_nonfull(int, int, char, Node*);  //helper insertion function
    void split(int, Node*);                      //helper function to split children
    void preorder(Node*);                        //prints the tree in preorder
    void remove(valContainer, Node*);            //main remove function
    void remove_from_leaf(int, Node*);
    void remove_from_non_leaf(int, Node*);
    void fill(int, Node*);
    valContainer* get_pred(int, Node*);
    valContainer* get_succ(int, Node*);
    void merge(int, Node*);
    void borrow_from_prev(int, Node*);
    void borrow_from_next(int, Node*);
};

void BTree::insert(int x, int y, char z)
{
    if(!root)
    {
        root = new Node(true);
        valContainer xcon;
        valContainer ycon;
        valContainer zcon;
        xcon.x = x;
        ycon.y = y;
        zcon.z = z;
        xcon.type = 0;
        ycon.type = 1;
        zcon.type = 2; 
        root->vals[0].push_back(xcon);
        root->vals[1].push_back(ycon);
        root->vals[2].push_back(zcon);
    }
    else
    {
        if(root->vals[0].size() == (2*degree - 1))
        {
            Node* newNode = new Node(false);
            newNode->children.push_back(root);
            split(0, newNode);

            int index = 0;
            if(type == 0)
            {
                if(newNode->vals[0][0].x < x) index++;
            } else if (type == 1)
            {
                if(newNode->vals[1][0].y < y) index++;
            } else
            {
                if(newNode->vals[2][0].z < z) index++;
            }
            insert_nonfull(x, y, z, newNode->children[index]);
            root = newNode;
        }
        else
        {
            insert_nonfull(x, y ,z, root);
        }
    }

}

void BTree::split(int index, Node* parent)
{
    Node* right_child = new Node(parent->children[index]->is_leaf);
    
    //copy half of the values to the new child
    for (int i = 0; i < degree-1; i++)
    {
        right_child->vals[0].push_back(parent->children[index]->vals[0][degree + i]);
        right_child->vals[1].push_back(parent->children[index]->vals[1][degree + i]);
        right_child->vals[2].push_back(parent->children[index]->vals[2][degree + i]);
    }
    for (int i = 0; i < degree-1; i++)
    {
        parent->children[index]->vals[0].pop_back();
        parent->children[index]->vals[1].pop_back();
        parent->children[index]->vals[2].pop_back();
    }

    //if the left child isn't a leaf then copy half of its children to right child
    if(!parent->children[index]->is_leaf)
    {
        for (int i = 0; i < degree; i++)
        {
            right_child->children.push_back(parent->children[index]->children[degree + i]);
        }
        for (int i = 0; i < degree; i++)
        {
            parent->children[index]->children.pop_back();
        }
    }

    //insert the mid key to parent node
    parent->vals[0].insert(parent->vals[0].begin() + index, parent->children[index]->vals[0][degree - 1]);
    parent->vals[1].insert(parent->vals[1].begin() + index, parent->children[index]->vals[1][degree - 1]);
    parent->vals[2].insert(parent->vals[2].begin() + index, parent->children[index]->vals[2][degree - 1]);

    //delete the mid key from the child node
    parent->children[index]->vals[0].pop_back();
    parent->children[index]->vals[1].pop_back();
    parent->children[index]->vals[2].pop_back();

    //insert right child to correct position => after the mid key 
    parent->children.insert(parent->children.begin() + index + 1, right_child);
}

void BTree::insert_nonfull(int x, int y, char z, Node* node)
{
    if (node->is_leaf)
    {
        valContainer xcon;
        valContainer ycon;
        valContainer zcon;
        xcon.x = x;
        ycon.y = y;
        zcon.z = z;
        xcon.type = 0;
        ycon.type = 1;
        zcon.type = 2;  
        int pos = 0;
        if (type == 0)
        {
            while(pos < node->vals[0].size() && node->vals[0][pos].x < x)
            {
                pos++;
            }
        } 
        else if (type == 1)
        {
            while(pos < node->vals[1].size() && node->vals[1][pos].y < y)
            {
                pos++;
            }
        } 
        else
        {
            while(pos < node->vals[2].size() && node->vals[2][pos].z < z)
            {
                pos++;
            }
        }
        node->vals[0].insert(node->vals[0].begin() + pos, xcon);
        node->vals[1].insert(node->vals[1].begin() + pos, ycon);
        node->vals[2].insert(node->vals[2].begin() + pos, zcon);
    }
    else
    {
        int pos = 0;
        if (type == 0)
        {
            while(pos < node->vals[0].size() && node->vals[0][pos].x < x)
            {
                pos++;
            }
        } 
        else if (type == 1)
        {
            while(pos < node->vals[1].size() && node->vals[1][pos].y < y)
            {
                pos++;
            }
        } 
        else
        {
            while(pos < node->vals[2].size() && node->vals[2][pos].z < z)
            {
                pos++;
            }
        }

        if(node->children[pos]->vals[0].size() == (2*degree -1))
        {
            split(pos, node);
            if (type == 0)
            {
                if (node->vals[0][pos].x < x) pos++;
            }
            else if (type == 1)
            {
                if (node->vals[1][pos].y < y) pos++;
            }
            else
            {
                if (node->vals[2][pos].z < z) pos++;
            }       
        }
        insert_nonfull(x, y, z, node->children[pos]);
    }
}

valContainer* BTree::get_pred(int index, Node* node)
{
    valContainer* cvals = new valContainer[3];
    Node* curr = node->children[index];
    while (!curr->is_leaf)
    {
        curr = curr->children[curr->vals[0].size()];
    }
    cvals[0] = curr->vals[0][curr->vals[0].size()-1];
    cvals[1] = curr->vals[1][curr->vals[1].size()-1];
    cvals[2] = curr->vals[2][curr->vals[2].size()-1];
    return cvals;
}

valContainer* BTree::get_succ(int index, Node* node)
{
    valContainer* cvals = new valContainer[3];
    Node* curr = node->children[index+1];
    while (!curr->is_leaf)
    {
        curr = curr->children[0];
    }
    cvals[0] = curr->vals[0][0];
    cvals[1] = curr->vals[1][0];
    cvals[2] = curr->vals[2][0];
    return cvals;
}

void BTree::remove_from_leaf(int index, Node* node)
{
    node->vals[0].erase(node->vals[0].begin() + index);
    node->vals[1].erase(node->vals[1].begin() + index);
    node->vals[2].erase(node->vals[2].begin() + index);
}

void BTree::remove_from_non_leaf(int index, Node* node)
{
    valContainer key = node->vals[type][index];
    if (node->children[index]->vals[0].size() >= degree)
    {
        valContainer* pred = get_pred(index, node);
        node->vals[0][index] = pred[0];
        node->vals[1][index] = pred[1];
        node->vals[2][index] = pred[2];
        remove(pred[type], node->children[index]);
    }
    else if (node->children[index+1]->vals[0].size() >= degree)
    {
        valContainer* succ = get_succ(index, node);
        node->vals[0][index] = succ[0];
        node->vals[1][index] = succ[1];
        node->vals[2][index] = succ[2];
        remove(succ[type], node->children[index+1]);
    }
    else
    {
        merge(index, node);
        remove(key, node->children[index]);
    }
}

void BTree::merge(int index, Node* node)
{
    Node* child = node->children[index];
    Node* sibling = node->children[index+1];

    child->vals[0].push_back(node->vals[0][index]);
    child->vals[1].push_back(node->vals[1][index]);
    child->vals[2].push_back(node->vals[2][index]);
    node->vals[0].erase(node->vals[0].begin() + index);
    node->vals[1].erase(node->vals[1].begin() + index);
    node->vals[2].erase(node->vals[2].begin() + index);
    node->children.erase(node->children.begin() + index+1);

    for(int i = 0; i < sibling->vals[0].size(); i++)
    {
        child->vals[0].push_back(sibling->vals[0][i]);
        child->vals[1].push_back(sibling->vals[1][i]);
        child->vals[2].push_back(sibling->vals[2][i]);
    }

    if (!child->is_leaf)
    {
        for(int i = 0; i <= sibling->vals[0].size(); i++)
        {
            child->children.push_back(sibling->children[i]); 
        }
    }
    delete sibling;
}

void BTree::fill(int index, Node* node)
{
    if (index != 0 && (node->children[index-1]->vals[0].size() >= degree))
    {
        borrow_from_prev(index, node);
    }
    else if (index != node->vals[0].size() && (node->children[index+1]->vals[0].size() >= degree))
    {
        borrow_from_next(index, node);
    }
    else
    {
        if (index != node->vals[0].size()) merge(index, node);
        else merge(index-1, node);
    }
}

void BTree::borrow_from_prev(int index, Node* node)
{
    Node* child = node->children[index];
    Node* sibling = node->children[index-1];

    child->vals[0].insert(child->vals[0].begin(), node->vals[0][index-1]);
    child->vals[1].insert(child->vals[1].begin(), node->vals[1][index-1]);
    child->vals[2].insert(child->vals[2].begin(), node->vals[2][index-1]);
    if(!child->is_leaf)
    {
        child->children.insert(child->children.begin(), sibling->children[sibling->vals[0].size()]);
    }
    node->vals[0][index-1] = sibling->vals[0][sibling->vals[0].size()-1];
    node->vals[1][index-1] = sibling->vals[1][sibling->vals[1].size()-1];
    node->vals[2][index-1] = sibling->vals[2][sibling->vals[2].size()-1];
    sibling->vals[0].pop_back();
    sibling->vals[1].pop_back();
    sibling->vals[2].pop_back();
    if(!sibling->is_leaf)
    {
        sibling->children.pop_back();
    }
}

void BTree::borrow_from_next(int index, Node* node)
{
    Node* child = node->children[index];
    Node* sibling = node->children[index+1];

    child->vals[0].push_back(node->vals[0][index]);
    child->vals[1].push_back(node->vals[1][index]);
    child->vals[2].push_back(node->vals[2][index]);

    if (!child->is_leaf)
    {
        child->children.push_back(sibling->children[0]);
    }

    node->vals[0][index] = sibling->vals[0][0];
    node->vals[1][index] = sibling->vals[1][0];
    node->vals[2][index] = sibling->vals[2][0];
    sibling->vals[0].erase(sibling->vals[0].begin());
    sibling->vals[1].erase(sibling->vals[1].begin());
    sibling->vals[2].erase(sibling->vals[2].begin());

    if(!sibling->is_leaf)
    {
        sibling->children.erase(sibling->children.begin());
    }
}

void BTree::remove(valContainer val, Node* node)
{
    //find the index of the first key that is >= to the given key
    int index = 0;
    while (index < node->vals[0].size() && node->vals[type][index] < val)
    {
        index++;
    }

    //if the key is in the current node
    if (index < node->vals[0].size() && node->vals[type][index] == val)
    {
        if (node->is_leaf) remove_from_leaf(index, node);
        else remove_from_non_leaf(index, node);
    }
    else
    {
        //key is not in the tree
        if (node->is_leaf) return;

        //check if the value is in the last child subtree of the current node
        bool in_last_child = (index == node->vals[0].size()) ? true : false;
        
        //if the subtree with the key has less than degree keys, fill that child
        if (node->children[index]->vals[0].size() < degree)
        {
            fill(index, node);
        }

        //last child is merged so recurse on the child before
        if (in_last_child && (index > node->vals[0].size())) remove(val, node->children[index-1]);
        else remove(val, node->children[index]);
    }
    return;
}

void BTree::preorder(Node* root)
{
    if (root)
    {
        for(int i = 0; i < root->vals[0].size(); i++)
        {
            cout << "(" << root->vals[0][i].x << "," << root->vals[1][i].y << "," << root->vals[2][i].z << ")";
        }
        cout << "\n";
        for (int i = 0; i < root->children.size(); i++)
        {
            preorder(root->children[i]);
        }
    }
}


int main(int argc, char const *argv[])
{
    int n, deg, type;
    char ctype;
    cin >> n >> deg >> ctype;
    
    if(ctype == 'x') type = 0;
    else if (ctype == 'y') type = 1;
    else type = 2;

    BTree tree;
    tree.root = NULL;
    tree.degree = deg;
    tree.type = type;

    int x, y; char z;
    for(int i = 0; i < n; i++)
    {
        cin >> x >> y >> z;
        tree.insert(x, y, z); 
    }

    valContainer val;
    if (type == 0)
    {
        int d;
        cin >> d;
        val.type = 0;
        val.x = d;
    } 
    else if (type == 1)
    {
        int d;
        cin >> d;
        val.type = 1;
        val.y = d;
    }
    else
    {
        char d;
        cin >> d;
        val.type = 2;
        val.z = d;
    }
    tree.remove(val, tree.root);
    tree.preorder(tree.root);
    return 0;
}
