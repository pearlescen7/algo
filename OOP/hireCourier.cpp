/******************************
** Author: Uzay Uysal        **
** Student Number: 150180039 **
******************************/
/*
    Since there is no information provided about this issue, I decided to upload my code combined with the main file provided.
    If you crop the main part and put another one instead, everything should work just fine.
*/

#include <iostream>
#include "ll.cpp"
#define BICYCLE_SHIPMENT_CAPACITY 10   //To make things simpler when the capacity is desired to be changed.
#define MOTORCYCLE_SHIPMENT_CAPACITY 35
#define CAR_SHIPMENT_CAPACITY 200

//Person class
class Person
{
private:
    std::string name;
    std::string surname;
public:
    Person(/* args */);
    const std::string& get_name() const           { return name; }
    const std::string& get_surname() const        { return surname; }
    void set_name(const std::string& _name)       { name = _name; }
    void set_surname(const std::string& _surname) { surname = _surname; }
    ~Person();
};

Person::Person(/* args */)
{
}

Person::~Person()
{
}

//Owner class which inherits from person class
class Owner : public Person
{
private:
    double ownership;
public:
    Owner();
    Owner(std::string, std::string);
    double get_ownership() const;
    void set_ownership(double);
    ~Owner();
};

Owner::Owner()
{

}

Owner::Owner(std::string _name, std::string _surname)
{
    set_name(_name);
    set_surname(_surname);
}

double Owner::get_ownership() const
{
   return ownership;
}

void Owner::set_ownership(double _value)
{
    ownership = _value;
}

std::ostream& operator<<(std::ostream& os, const Owner& _obj)
{
    os << _obj.get_name() << ' ' << _obj.get_surname() << ' ' << _obj.get_ownership();
    return os;
}

Owner::~Owner()
{
}

//Courier class which inherits from person class
class Courier : public Person
{
private:
    std::string vehicle_type;
public:
    Courier();
    Courier(std::string, std::string, std::string);
    const std::string& get_vehicle_type();
    bool operator==(Courier&);
    friend std::ostream& operator<<(std::ostream&, const Courier&);
    ~Courier();
};

Courier::Courier()
{
}

Courier::Courier(std::string _name, std::string _surname, std::string _vehicle_type)
{
    set_name(_name);
    set_surname(_surname);
    if(_vehicle_type != "car" && _vehicle_type != "motorcycle" && _vehicle_type != "bicycle")
    {
        std::cout << "Vehicle type " << _vehicle_type << " is invalid!" << std::endl;
    }
    vehicle_type = _vehicle_type;
}

const std::string& Courier::get_vehicle_type()
{
    return vehicle_type;
}

bool Courier::operator==(Courier& _obj)
{
    if(get_name() == _obj.get_name() && get_surname() == _obj.get_surname() && vehicle_type == _obj.vehicle_type) return true;
    else return false;
}

std::ostream& operator<<(std::ostream& os, const Courier& _obj)
{
    os << _obj.get_name() << ' ' << _obj.get_surname() << ' ' << _obj.vehicle_type;
    return os;
}

Courier::~Courier()
{
}

class Business
{
private:
    std::string name;
    std::string address;
    LinkedList<Owner> owner_list;
    LinkedList<Courier> courier_list;
public:
    Business(std::string, std::string, Owner*, int);
    void hire_courier(Courier&);
    void fire_courier(Courier&);
    void list_couriers();
    void list_owners();
    double calculate_shipment_capacity();
    void operator()();
    Courier& operator[](int);
    ~Business();
};

Business::Business(std::string _name, std::string _address, Owner* owner_arr, int owner_count)
{
    name = _name;
    address = _address;
    Owner* temp = owner_arr;
    for (int i = 0; i < owner_count; i++)
    {
        owner_list.push_back(*temp);
        temp++;
    }
    for (int i = 0; i < owner_list.get_count(); i++)
    {
        owner_list[i]->data.set_ownership(100.0/owner_count);
    }
    
}

void Business::hire_courier(Courier& _obj)
{
    courier_list.push_back(_obj);
}

void Business::fire_courier(Courier& _obj)
{
    if(!courier_list.erase(_obj))
    {
        std::cout << "Courier \"" << _obj << "\" cannot be found." << std::endl;
    }
}

void Business::list_couriers()
{
    std::cout << "The courier list is as follows: \n";
    courier_list.print_list();
}

void Business::list_owners()
{
    std::cout << "The owner list is as follows: \n";
    owner_list.print_list();
}

double Business::calculate_shipment_capacity()
{
    double sum = 0;
    for (int i = 0; i < courier_list.get_count(); i++)
    {
        if(courier_list[i]->data.get_vehicle_type() == "bicycle")
        {
            sum += BICYCLE_SHIPMENT_CAPACITY;
        }
        if(courier_list[i]->data.get_vehicle_type() == "motorcycle")
        {
            sum += MOTORCYCLE_SHIPMENT_CAPACITY;
        }
        if(courier_list[i]->data.get_vehicle_type() == "car")
        {
            sum += CAR_SHIPMENT_CAPACITY;
        }
    }
    return sum;
}

void Business::operator()()
{
    std::cout << "This business has the following properties: \n";
    std::cout << name << ' ' << address << '\n';
    list_owners();
    list_couriers();
}

Courier& Business::operator[](int index)
{
    return courier_list[index]->data;
}

Business::~Business()
{
}


int main(){
    //Create owners of the business
    //Constructor parameters: name, surname
    Owner o1 = Owner("Owner_name1", "surname1");
    Owner o2 = Owner("Owner_name2", "surname2");
    Owner* owner_arr = new Owner[2];
    owner_arr[0] = o1;
    owner_arr[1] = o2;    

    //Crete the business itself
    //Constructor parameters: name, address, owner_array, number_of_owners
    Business atlas = Business("Atlas", "Maslak Istanbul/Turkey", owner_arr, 2);
    
    // Print owner info to screen: name, surname, ownership
    atlas.list_owners();

    // Add new employees
    // Constructor parameters: name, surname, vehicle_type
    Courier c1 = Courier("Courier", "surname1", "car");
    Courier c2 = Courier("Courier", "surname2", "motorcycle");
    Courier c3 = Courier("Courier", "surname3", "motorcycle");    

    atlas.hire_courier(c1);
    atlas.hire_courier(c2);
    atlas.hire_courier(c3);


    // Print business info to screen
    // Name, address, owners, couriers
    atlas();

    // Get courier with index 1 and remove it from the list/array
    Courier cour = atlas[1];
    atlas.fire_courier(cour);
    
    // Print remaining couriers
    atlas.list_couriers();
    
    // Print current maximum shipment capacity
    std::cout << atlas.calculate_shipment_capacity() << std::endl;
    return 0;
}