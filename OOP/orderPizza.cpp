/******************************
** Author: Uzay Uysal        **
** Student Number: 150180039 **
******************************/

#include <iostream>
#include <string>
#include <sstream>
#include <iomanip>

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

        void erase(T item)
        {
            if(count == 0)  //If item count is 0 there is nothing to be found
            {
                std::cout << "Item cannot be found" << std::endl;
                return;
            }

            count--;  //Decrease the size by one
            Node<T>* temp = head;
            Node<T>* temp_to_del;
            if((head != NULL) && (head->data == item)){head = head->next; delete temp; return;}  //If head has the right value, move head and delete the node
            while((temp->next != NULL) && (temp->next->data != item)){ temp = temp->next; }      //If not search until the next element gets the right value
            temp_to_del = temp->next;

            if (temp_to_del == NULL)  //If the item is not in the list
            {
                std::cout << "Item cannot be found" << std::endl;
                return;
            }         

            temp->next = temp->next->next;        //Make the new connections and delete the found element
            temp->next->prev = temp;              //If it is the last element, then don't set its prev since it cannot be reached
            delete temp_to_del;
            temp_to_del = NULL;
            temp = NULL;
            return;
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

class Pizza
{
private:
    std::string name;
    std::string size;
    LinkedList<std::string> ingredients;
    std::string crust_type;
public:
    Pizza();
    Pizza(Pizza&);
    Pizza(std::string, std::string, int);
    const std::string get_name();
    const std::string get_size();
    LinkedList<std::string>& get_ingredients();
    const std::string get_crust_type();
    void set_name(const std::string&);
    void set_size(const std::string&);
    void set_ingredients(const LinkedList<std::string>&);
    void set_crust_type(const std::string&);
    friend void remove_ingredients(Pizza&);
    ~Pizza();
};

//Default ctor creates pizza with medium size, normal crust and mozarella as the ingredient
Pizza::Pizza()
{
    size = "medium";
    crust_type = "normal";
    ingredients.push_back("mozarella");
}

//Creates a new pizza from the copied one without the unwanted ingredients
Pizza::Pizza(Pizza& P)
{
    name = P.name;
    size = P.size;
    crust_type = P.crust_type;
    ingredients = P.ingredients;  //Assignment operator is already overloaded
    remove_ingredients(*this);
}

//Creates pizza according to the given parameters
Pizza::Pizza(std::string _size, std::string _crust_type, int _pizza_type)
{
    size = _size;
    crust_type = _crust_type;
    switch (_pizza_type)
    {
    case 1:
        name = "Chicken Pizza";
        ingredients.push_back("mozarella");
        ingredients.push_back("chicken");
        ingredients.push_back("mushroom");
        ingredients.push_back("corn");
        ingredients.push_back("onion");
        ingredients.push_back("tomato");
        break;
    case 2:
        name = "Broccoli Pizza";
        ingredients.push_back("mozarella");
        ingredients.push_back("broccoli");
        ingredients.push_back("pepperoni");
        ingredients.push_back("olive");
        ingredients.push_back("corn");
        ingredients.push_back("onion");
        break;
    case 3:
        name = "Sausage Pizza";
        ingredients.push_back("mozarella");
        ingredients.push_back("sausage");
        ingredients.push_back("tomato");
        ingredients.push_back("olive");
        ingredients.push_back("mushroom");
        ingredients.push_back("corn");
        break;
    case 4:
        name = "";
        crust_type = "";
        size = "";
    }
}
//Getters to calculate the price and print out the orders
const std::string Pizza::get_name()
{
    return name;
}

const std::string Pizza::get_size()
{
    return size;
}

const std::string Pizza::get_crust_type()
{
    return crust_type;
}

LinkedList<std::string>& Pizza::get_ingredients()
{
    return ingredients;
}

void Pizza::set_name(const std::string& _name)
{
    name = _name;
}

void Pizza::set_size(const std::string& _size)
{
    size = _size;
}

void Pizza::set_ingredients(const LinkedList<std::string>& _ingredients)
{
    ingredients = _ingredients;
}

void Pizza::set_crust_type(const std::string& _crust_type)
{
    crust_type = _crust_type;
}

void remove_ingredients(Pizza& P)
{
    std::cout << "Please enter the number of the ingredient you want to remove from your pizza." << std::endl;
    P.ingredients.print_list();
    std::cout << "Enter \"0\" to save your changes." << std::endl;

    int input;
    bool done = false;
    LinkedList<int> to_remove;
    while (!done)
    {
        std::string line;
        while(std::getline(std::cin, line))
        {
            bool found = false;
            std::stringstream ss(line);
            if(ss >> input)
            {
                if ((ss.eof()) && (input >= 0) && (input <= P.ingredients.get_count()))
                {
                    for (int i = 0; i < to_remove.get_count(); i++)
                    {
                        if(to_remove[i]->data == input - 1)
                        {
                            found = true;
                            break;
                        }
                    }
                    
                    if(!found)
                    { 
                        break;
                    }
                }   
            }
            std::cout << "Invalid input. Please try again." << std::endl;
        }
        if (input == 0)
        {
            done = true;
        } 
        else
        {
            to_remove.push_back(input-1);
        }    
    }
    for (int i = 0; i < to_remove.get_count(); i++)
    {
        P.ingredients.erase_at(to_remove[i]->data-i);
    }
    //do the loop to take multiple inputs
    //USE ERASEAT
}

//No need to delete anything since nothing is allocated using new in Pizza class. LinkedList will call its own.
Pizza::~Pizza()
{
}

template <>
struct Node<Pizza*>
{
    Pizza* data;
    Node<Pizza*>* next;
    Node<Pizza*>* prev;
    Node(Pizza* _data)
    {
        data = new Pizza("", "", 4);
        data->set_name(_data->get_name());
        data->set_size(_data->get_size());
        data->set_ingredients(_data->get_ingredients());
        data->set_crust_type(_data->get_crust_type());
        next = NULL;
        prev = NULL;
    }
};

class Order
{
private:
    std::string customer_name;
    LinkedList<Pizza*> pizza_list;
    LinkedList<std::string> drink_list;
public:
    Order();
    Order(const std::string, const LinkedList<Pizza*>&, const LinkedList<std::string>&);
    Order(const std::string, const LinkedList<Pizza*>&);
    Order* operator=(const Order&);
    std::string& get_customer_name();
    float get_price();
    void print_order();
    ~Order();
};

Order::Order()
{
}

std::string& Order::get_customer_name()
{
    return customer_name;
}

Order::Order(const std::string _customer_name, const LinkedList<Pizza*>& _pizza_list, const LinkedList<std::string>& _drink_list)
{
    customer_name = _customer_name;
    pizza_list = _pizza_list;
    drink_list = _drink_list;
}

Order::Order(const std::string _customer_name, const LinkedList<Pizza*>& _pizza_list)
{
    customer_name = _customer_name;
    pizza_list = _pizza_list;
    LinkedList<std::string> empty_list;
    empty_list.push_back("");
    drink_list = empty_list;
}
//Calculates the price of the order
float Order::get_price()
{
    float total_price = 0;
    int pizza_count = pizza_list.get_count();
    int drink_count = drink_list.get_count();
    for (int i = 0; i < pizza_count; i++)
    {
        std::string pizza_size = pizza_list[i]->data->get_size();
        if (pizza_size == "small")
        {
            total_price += 15;
        }
        else if (pizza_size == "medium")
        {
            total_price += 20;
        }
        else if (pizza_size == "big")
        {
            total_price += 25;
        }
        else
        {
            return -1;
        }
    }
    for (int i = 0; i < drink_count; i++)
    {
        std::string drink = drink_list[i]->data;
        if (drink == "cola")
        {
            total_price += 4;
        }
        else if (drink == "soda")
        {
            total_price += 2;
        }
        else if (drink == "ice tea")
        {
            total_price += 3;
        }
        else if (drink == "fruit juice")
        {
            total_price += 3.5;
        }
    }
    return total_price;
}
//Prints the order in a formatted way
void Order::print_order()
{
    std::cout << std::setfill('-') << std::setw(20) << std::endl; 
    std::cout << "\nName: " << customer_name << std::endl << std::endl;
    for(int i = 0; i < pizza_list.get_count(); i++)
    {
        std::cout << pizza_list[i]->data->get_name() << '('; 
        for (int j = 0; j < pizza_list[i]->data->get_ingredients().get_count(); j++)
        {
            std::cout << pizza_list[i]->data->get_ingredients()[j]->data;
            if(j != pizza_list[i]->data->get_ingredients().get_count() - 1){ std::cout << ','; }
        }
        std::cout << ')' << std::endl;
        std::cout << "Size: " << pizza_list[i]->data->get_size() << ", Crust: " << pizza_list[i]->data->get_crust_type() << std::endl << std::endl;
    }
    int cola_ctr = 0, soda_ctr = 0, tea_ctr = 0, fruit_ctr = 0;
    for (int i = 0; i < drink_list.get_count(); i++)
    {
        if(drink_list[i]->data == "cola"){cola_ctr++;}
        if(drink_list[i]->data == "soda"){soda_ctr++;}
        if(drink_list[i]->data == "ice tea"){tea_ctr++;}
        if(drink_list[i]->data == "fruit juice"){fruit_ctr++;}
    }
    if(cola_ctr){std::cout << cola_ctr << " cola" << ", ";}
    if(soda_ctr){std::cout << soda_ctr << " soda" << ", ";}
    if(tea_ctr){std::cout << tea_ctr << " ice tea" << ", ";}
    if(fruit_ctr){std::cout << fruit_ctr << " fruit juice";}
    std::cout << "\n" << std::setfill('-') << std::setw(20) << std::endl;
}

Order* Order::operator=(const Order& _order)
{
    customer_name = _order.customer_name;
    pizza_list = *(new LinkedList<Pizza*>(_order.pizza_list));
    drink_list = *(new LinkedList<std::string>(_order.drink_list));
}

//Same with pizza class, nothing is allocated with new and linked list will call its own
Order::~Order()
{
}

template <>
struct Node<Order*>
{
    Order* data;
    Node<Order*>* next;
    Node<Order*>* prev;
    Node(Order* _data)
    {
        data = new Order;
        *data = *_data;
        next = NULL;
        prev = NULL;
    }
};

class OrderList
{
private:
    LinkedList<Order*> list;
public:
    OrderList();
    void take_order();
    void list_orders();
    void deliver_orders();
    ~OrderList();
};

OrderList::OrderList()
{
}

void OrderList::take_order()
{
    
    std::cout << "\nPizza Menu" << std::endl;
    std::cout << "1. Chicken Pizza (mozarella, chicken, mushroom, corn, olive, onion, tomato" << std::endl;
    std::cout << "2. Broccoli Pizza (mozarella, broccoli, pepperoni, olive, corn, onion" << std::endl;
    std::cout << "3. Sausage Pizza (mozarella, sausage, tomato, olive, mushroom, corn" << std::endl;
    std::cout << "0. Back to main menu" << std::endl;
    int type_input;
    bool valid = false;
    while(!valid)
    {
        std::string line;
        while(std::getline(std::cin, line))
        {
            std::stringstream ss(line);
            if(ss >> type_input)
            {
                if ((ss.eof()) && ((type_input == 0) || (type_input == 1) || (type_input == 2) || (type_input == 3)))
                {
                    valid = true;
                    break;
                }   
            }
            std::cout << "Invalid input. Please try again." << std::endl;
        }
    }
    if (type_input == 0)
    {
        return;
    }
    std::cout << "Select size: small (15 TL), medium (20 TL), big (25 TL)" << std::endl;
    std::string size_input;
    valid = false;
    while(!valid)
    {
        std::string line;
        while(std::getline(std::cin, line))
        {
            std::stringstream ss(line);
            if(ss >> size_input)
            {
                if ((ss.eof()) && ((size_input == "small") || (size_input == "medium") || (size_input == "big")))
                {
                    valid = true;
                    break;
                }   
            }
            std::cout << "Invalid input. Please try again." << std::endl;
        }
    }
    std::cout << "Select crust type: thin, normal, thick" << std::endl;
    std::string crust_input;
    valid = false;
    while(!valid)
    {
        std::string line;
        while(std::getline(std::cin, line))
        {
            std::stringstream ss(line);
            if(ss >> crust_input)
            {
                if ((ss.eof()) && ((crust_input == "thin") || (crust_input == "normal") || (crust_input == "thick")))
                {
                    valid = true;
                    break;
                }   
            }
            std::cout << "Invalid input. Please try again." << std::endl;
        }
    }
    std::cout << "Enter the amount: ";
    int amount;
    valid = false;
    while(!valid)
    {
        std::string line;
        while(std::getline(std::cin, line))
        {
            std::stringstream ss(line);
            if(ss >> amount)
            {
                if ((ss.eof()) && (amount > 0))
                {
                    valid = true;
                    break;
                }   
            }
            std::cout << "Invalid input. Please try again." << std::endl;
        }
    }
    LinkedList<Pizza*> _pizza_list;
    Pizza temp(size_input, crust_input, type_input);
    _pizza_list.push_back(&temp);
    if (amount > 1)
    {
        for (int i = 0; i < amount - 1; i++)
        {
            Pizza temp2(temp);
            _pizza_list.push_back(&temp2);
        } 
    }

    std::cout << "Please choose a drink:" << std::endl;
    std::cout<< "0. no drink" << std::endl;
    std::cout<< "1. cola 4 TL" << std::endl;
    std::cout<< "2. soda 2 TL" << std::endl;
    std::cout<< "3. ice tea 3 TL" << std::endl;
    std::cout<< "4. fruit juice 3.5 TL" << std::endl;
    std::cout << "Enter \"-1\" to save your changes." << std::endl;

    LinkedList<std::string> _drink_list;
    int drink_input;
    valid = false;
    bool no_drinks = false;
    while (!valid)
    {
        std::string line;
        while(std::getline(std::cin, line))
        {
            std::stringstream ss(line);
            if(ss >> drink_input)
            {
                if (ss.eof() && (drink_input == 0) || (drink_input == 1) ||(drink_input == 2) || (drink_input == 3) || (drink_input == 4) || (drink_input == -1))
                {
                    break;
                }   
            }
            std::cout << "Invalid input. Please try again." << std::endl;
        }
        if (drink_input == -1)
        {
            if(_drink_list.get_count() != 0){ valid = true; }
            else{ std::cout << "No option selected. Please select an option and then save." << std::endl; }
        } 
        else
        {
            if (drink_input == 1)
            {
                _drink_list.push_back("cola");
            }
            else if (drink_input == 2)
            {
                _drink_list.push_back("soda");
            }
            else if (drink_input == 3)
            {
                _drink_list.push_back("ice tea");
            }
            else if (drink_input == 4)
            {
                _drink_list.push_back("fruit juice");
            }
            else if (drink_input == 0)
            {
                while(_drink_list.get_count() != 0)
                {
                    _drink_list.erase_at(0);
                }
                _drink_list.push_back("");
                no_drinks = true;
                valid = true;
            }
        }
    }
    std::cout << "Please enter your name:" << std::endl;
    std::string _customer_name;
    std::string line;
    while(std::getline(std::cin, line))
    {
        std::stringstream ss(line);
        if(ss >> _customer_name)
        {
            if(ss.eof())
            {
                valid = true;
                break;
            }   
        }
        std::cout << "Invalid input. Please try again." << std::endl;
    }
    if(!no_drinks)
    {
        Order O(_customer_name, _pizza_list, _drink_list);
        Order* OP = &O;
        list.push_back(OP);
        std::cout << "Your order is saved, it will be delivered when it is ready..." << std::endl;
        OP->print_order();
    }
    else
    {
        Order O(_customer_name, _pizza_list);
        Order* OP = &O;
        list.push_back(OP);
        std::cout << "Your order is saved, it will be delivered when it is ready..." << std::endl;
        OP->print_order();
    }
    
}

void OrderList::list_orders()
{
    if(list.get_count() == 0)
    {
        std::cout << std::setfill('-') << std::setw(20) << std::endl; 
        std::cout << "No orders available." << std::endl;
        std::cout << std::setfill('-') << std::setw(20) << '\n' << std::endl;
        return;
    }
    for (int i = 0; i < list.get_count(); i++)
    {
        std::cout << "\n" << i + 1;
        list[i]->data->print_order();
    }
}

void OrderList::deliver_orders()
{
    list_orders();
    std::cout << "Please write the customer name in order to deliver his/her order: ";
    bool valid = false;
    bool code_valid = false;
    std::string _customer_name;
    std::string line;
    while(std::getline(std::cin, line))
    {
        std::stringstream ss(line);
        if(ss >> _customer_name)
        {
            if(ss.eof())
            {
                for (int i = 0; i < list.get_count(); i++)
                {
                    if(list[i]->data->get_customer_name() == _customer_name)
                    {
                        std::cout << "Following order is delivering..." << std::endl;
                        list[i]->data->print_order();
                        std::cout << "\nTotal price: " << list[i]->data->get_price() << std::endl;
                        std::cout << "Do you have a promotion code? (y/n)" << std::endl;
                        char code;
                        while(!code_valid)
                        {
                            std::cin >> code;
                            if((code != 'y') && (code != 'n'))
                            {
                                std::cout << "Invalid input. Please try again." << std::endl;
                            }
                            else
                            {
                                if(code == 'n')
                                {
                                    std::cout << "The order is delivered successfully!" << std::endl;
                                    std::cout << std::setfill('-') << std::setw(20) << '\n' << std::endl; 
                                    list.erase_at(i);
                                    code_valid = true;
                                }
                                else if (code == 'y')
                                {
                                    std::cout << "Please enter the promotion code: ";
                                    std::string code_line;
                                    std::cin.ignore(1000, '\n');
                                    while(std::getline(std::cin, code_line))
                                    {
                                        if(code_line == "I am a student")
                                        {
                                            std::cout << "\nNew total price: " << list[i]->data->get_price()* 0.9 << std::endl;
                                            std::cout << "The order is delivered successfully!" << std::endl;
                                            std::cout << std::setfill('-') << std::setw(20) << '\n' << std::endl; 
                                            list.erase_at(i);
                                            code_valid = true;
                                            break;
                                        }
                                        else
                                        {
                                            std::cout << "Invalid promotion code. Please try again." << std::endl;
                                        }

                                    }
                                }
                            }
                            
                        }
                        valid = true;
                        break;
                    }
                }
                if(valid){break;}
            }   
        }
        std::cout << "Invalid input. Please try again. Available orders are the following:" << std::endl;
        for (int i = 0; i < list.get_count(); i++)
        {
            std::cout << list[i]->data->get_customer_name();
            if(i != list.get_count() - 1) {std::cout << ", ";}
        }
        std::cout << "\n";
    }
}

OrderList::~OrderList()
{
}



int main(int argc, char const *argv[])
{
    bool exit_flag = false;
    OrderList OL;
    while (!exit_flag)
    {
        std::cout << "Welcome to Unicorn Pizza!\n1. Add an order\n2. List orders\n3. Deliver order\n4. Exit\nChoose what to do: ";
        int choice;
        std::string line;
        while(std::getline(std::cin, line))
        {
            std::stringstream ss(line);
            if(ss >> choice)
            {
                if (ss.eof() && ((choice == 1) ||(choice == 2) || (choice == 3) || (choice == 4)))
                {
                    break;
                }   
            }
            std::cout << "Invalid input. Please try again." << std::endl;
        }
        if (choice == 4)
        {
            std::cout << "Goodbye..." << std::endl;
            exit_flag = true;
        } 
        else if (choice == 1)
        {
            OL.take_order();
        }
        else if (choice == 2)
        {
            OL.list_orders();
        }
        else if (choice == 3)
        {
            OL.deliver_orders();
        }   

    }
    return 0;
}
