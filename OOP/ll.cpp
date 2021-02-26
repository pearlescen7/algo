/******************************
** Author: Uzay Uysal        **
** Student Number: 150180039 **
******************************/

#include <iostream>

//Simple node struct to use in linked list.
template <typename T>
struct Node
{
    T data;
    Node<T>* next;
    Node<T>* prev;
    Node(T _data)
    {   
        data = _data;
        next = NULL;
        prev = NULL;
    }
};

//Linked list class for general use.
template <typename T> 
struct LinkedList
{
    private:
        Node<T>* head;
        int count;
    
    public:
        LinkedList(){ head = NULL; count = 0; }

        LinkedList(const LinkedList& list)
        {
            head = NULL; count = 0;
            Node<T>* temp = list.head;
            for (int i = 0; i < list.count; i++)
            {
                push_back(temp->data);
                temp = temp->next;
            }
        }

        const int get_count() 
        {
            return count;
        }

        Node<T>* get_head()
        {
            return head;
        }

        Node<T>* operator[](int index)
        {
            if(index < 0 || index > count)
            {
                std::cout << "Invalid index!" << std::endl;
            }
            Node<T>* temp = head;
            for (int i = 0; i < index; i++)
            {
                temp = temp->next;
            }
            return temp;
        }

        LinkedList& operator=(const LinkedList& list)  //Overloading assignment operator to do a deep copy
        {
            Node<T>* temp = list.head;
            for (int i = 0; i < list.count; i++)
            {
                push_back(temp->data);
                temp = temp->next;
            }
            return *this;
        }

        void push_back(T item)
        {
            count++;  //Increase the size by one
            if(head == NULL){ head = new Node<T>(item); return; } //Adding the first item
            
            Node<T>* temp = head;                           //If not the first element
            while(temp->next != NULL){ temp = temp->next; } //Iterate until the last element
            temp->next = new Node<T>(item);                 //Allocate memory for the null element
            temp->next->prev = temp;                        //Connect it to the previous element
            temp->next->next = NULL;                        //Set the new element's next to null                       
        }

        bool erase(T item)
        {
            if(count == 0)  //If item count is 0 there is nothing to be found
            {
                //std::cout << "Item cannot be found" << std::endl;
                return false;
            }

            count--;  //Decrease the size by one
            Node<T>* temp = head;
            Node<T>* temp_to_del;
            if((head != NULL) && (head->data == item)){head = head->next; delete temp; return true;}  //If head has the right value, move head and delete the node
            while((temp->next != NULL) && !(temp->next->data == item)){ temp = temp->next; }      //If not search until the next element gets the right value
            temp_to_del = temp->next;

            if (temp_to_del == NULL)  //If the item is not in the list
            {
                //std::cout << "Item cannot be found" << std::endl;
                return false;
            }         

            temp->next = temp->next->next;        //Make the new connections and delete the found element
            temp->next->prev = temp;              //If it is the last element, then don't set its prev since it cannot be reached
            delete temp_to_del;
            temp_to_del = NULL;
            temp = NULL;
            return true;
        }
        void erase_at(int index)
        {
            if ((index >= 0) && (index < count) && (count != 0))   //Check if the index is valid
            {
                count--;
                Node<T>* temp = head;
                Node<T>* temp_to_del;
                for (int i = 0; i < index; i++)
                {
                    temp = temp->next;
                }
                temp_to_del = temp;                                    //Found the node to delete
                if(temp != head)                                       //Get the previous node to make the connections, if head is being deleted move it forward
                {                                     
                    temp = temp->prev;
                    temp->next = temp->next->next;
                    if(temp->next != NULL){ temp->next->prev = temp; } //If it is the last element, then don't set its prev since it cannot be reached
                } 
                else 
                { 
                    head = head->next; 
                    if(head != NULL){ head->prev = NULL; }
                }
                delete temp_to_del;
                temp_to_del = NULL;
                temp = NULL;
                return;
            }
            else
            {
                std::cout << "Element" << index + 1 << "could not be deleted." << std::endl;
            }
            
        }
        void print_list()
        {   
            Node<T>* temp = head;
            for (int i = 0; i < count; i++)
            {
                std::cout << i+1 << ". " << temp->data << std::endl;
                temp = temp->next;
            }
            
        }
        ~LinkedList()
        {
            while(head != NULL)  //Move head until everything is deleted
            {
                count--;
                Node<T>* temp = head;
                head = head->next;
                delete temp;
                temp = NULL;
            }
            
        }
};