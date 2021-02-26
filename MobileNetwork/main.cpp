/* @Author
Student Name: Uzay Uysal
Student ID: 150180039
Date: 19.12.2019 */

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

using namespace std;

template <typename T>
struct listNode{
    T data;
    listNode* next;
};

template <typename T>
struct linkedlist{
    listNode<T>* head;
    listNode<T>* tail;
    int length;
    linkedlist(){
        head = NULL;
        tail = NULL;
        length = 0;
    }
    void add(T data){
        if(head == NULL){
            head = new listNode<T>;
            head->data = data;
            head->next = NULL;
            tail = head;
        } else {
            listNode<T>* newNode = new listNode<T>;
            newNode->data = data;
            newNode->next = NULL;
            tail->next = newNode;
            tail = tail->next;
        }
        length++;
    }
};

struct Node{
    int ID;
    linkedlist<Node*> BS;
    linkedlist<Node*> MH;
    Node(){}
    Node(int x): ID(x){}
};

struct IDstack{
    listNode<int>* head;
    listNode<int>* tail;
    IDstack(){
        head = NULL;
        tail = NULL;
    }
    IDstack(int x){
        tail = new listNode<int>;
        tail->data = x;
        head = tail;
    }
    bool isEmpty(){
        if(tail == NULL){return true;}
        else {return false;}
    }
    void push(int data){
        if(this->isEmpty()){
            tail = new listNode<int>;
            tail->data = data;
            head = tail;
        } else{
            tail->next = new listNode<int>;
            tail->next->data = data;
            tail = tail->next;
        }
    }
    int pop(){
        if(this->isEmpty()){
            return -1;
        } else {
            listNode<int>* temp = head;
            int res = tail->data;
            if(temp == tail){
                delete tail;
                tail = NULL;
                head = NULL;
            } else{
                while(temp->next != tail){
                    temp = temp->next;
                }
                delete tail;
                tail = temp; 
            }
            return res;
        }
    }
    void printReversed(){
        if(this->isEmpty()){
            return;
        }
        int top = this->pop();
        printReversed();
        cout << top << " ";
    }
};

struct Network{
    Node* root;
    Network(){
        root = new Node();
        root->ID = 0;
    }

    ~Network(){
        //delete everything
    }

    //decide and add to the network
    void add(Node* newNode, string type, int parentID){
        //find the parent node to add
        Node* parentNode = bsSearch(parentID, root);
        //find the type of the node and add to corresponding list
        if(type == "BS"){
            parentNode->BS.add(newNode);
        } else if(type == "MH"){
            parentNode->MH.add(newNode);
        }
    }

    Node* bsSearch(int searchID, Node* sNode){
        //return if the ID is found
        if(searchID == sNode->ID){
            return sNode;
        }
        //call the func for every child while not found and make them do the same
        Node* res = NULL;
        listNode<Node*>* listTemp = sNode->BS.head;
        while(!res && listTemp){
            Node* temp = listTemp->data;
            listTemp = listTemp->next;
            res = bsSearch(searchID, temp);
        }
        return res;
    }

    void printSearchResult(string message, int ID){
        cout << "Traversing:";
        //using stack to solve the path problem
        IDstack* path = new IDstack;
        if(mhSearch(ID, root, path)){
            cout << endl << "Message:" << message <<" To:";
            path->printReversed();
            cout << "mh_" << ID << endl;
        } else{
            cout << endl <<"Can not be reached the mobile host mh_" << ID << " at the moment" << endl;
        }
    }

    Node* mhSearch(int searchID, Node* sNode, IDstack* path){
        //return if the ID is found
        cout << sNode->ID << " ";
        //push the node ID to the stack
        path->push(sNode->ID);
        Node* res = NULL;
        listNode<Node*>* mhTemp = sNode->MH.head;
        while (mhTemp && mhTemp->data->ID != searchID){
            mhTemp = mhTemp->next;
        }
        if(mhTemp){res = mhTemp->data;}
        //call the func for every child while not found and make them do the same
        listNode<Node*>* listTemp = sNode->BS.head;
        while(!res && listTemp){
            Node* temp = listTemp->data;
            listTemp = listTemp->next;
            res = mhSearch(searchID, temp, path);
        }
        //pop the ID if the path is wrong
        if(res == NULL){path->pop();}
        return res;
    }

};


int main(int argc, const char** argv) {
    fstream netFile;
    netFile.open(argv[1], fstream::in);
    if(!netFile.is_open()){
        cout << "Error: Network file could not be opened." << endl;
        return -1;
    }

    Network web;
    stringstream iss;
    string line;
    string type;
    int ID;
    int parentID;

    while(netFile){
        //Read the network text file line by line
        while(getline(netFile, line)){
            //insert the line into a stringstream and parse using it
            iss << line;
            iss >> type;
            iss >> ID;
            iss >> parentID;
            //create new node with the given ID and add it
            Node* newNode = new Node(ID);
            web.add(newNode, type, parentID);
            //flush the stream
            iss.clear();
        }
    }

    fstream mesFile;
    mesFile.open(argv[2], fstream::in);
    if(!mesFile.is_open()){
        cout << "Error: Message file could not be opened." << endl;
        return -1;
    }
    //read the message text line by line
    while(mesFile){
        while(getline(mesFile, line)){
            //insert line into stringstream and parse
            iss << line;
            getline(iss, line, '>');
            iss >> ID;
            web.printSearchResult(line, ID);
            //flush the stream
            iss.clear();
        }
    }
    return 0;
}