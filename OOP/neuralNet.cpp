/******************************
** Author: Uzay Uysal        **
** Student Number: 150180039 **
******************************/
//Please use -std=c++11 while compiling.

#include <iostream>
#include <fstream>
#include <cmath>
#include "matrix.cpp"
#include "ll.cpp"

class Neuron
{
protected:
    double z;
    double a;
public:
    Neuron(/* args */);
    void set_a(double);
    void set_z(double);
    double get_a();
    double get_z();
    virtual void activate() = 0;
    virtual ~Neuron();
};

Neuron::Neuron(/* args */)
{
}

void Neuron::set_a(double _a)
{
    a = _a;
}

void Neuron::set_z(double _z)
{
    z = _z;
}

double Neuron::get_a()
{
    return a;
}

double Neuron::get_z()
{
    return z;
}

Neuron::~Neuron()
{
}

class SigmoidNeuron : public Neuron
{
private:
    /* data */
public:
    SigmoidNeuron(/* args */);
    void activate();
    ~SigmoidNeuron();
};

SigmoidNeuron::SigmoidNeuron(/* args */)
{
}

void SigmoidNeuron::activate()
{
    a = 1 / (1 + std::exp(-z));
}

SigmoidNeuron::~SigmoidNeuron()
{
}

class ReluNeuron : public Neuron
{
private:
    /* data */
public:
    ReluNeuron(/* args */);
    void activate();
    ~ReluNeuron();
};

ReluNeuron::ReluNeuron(/* args */)
{
}

void ReluNeuron::activate()
{
    if (0 > z)
    {
        a = 0;
    }
    else
    {
        a = z;
    }
}

ReluNeuron::~ReluNeuron()
{
}

class LReluNeuron : public Neuron
{
private:
    /* data */
public:
    LReluNeuron(/* args */);
    void activate();
    ~LReluNeuron();
};

LReluNeuron::LReluNeuron(/* args */)
{
}

void LReluNeuron::activate()
{
    if (0.1 * z > z)
    {
        a = 0.1 * z;
    }
    else
    {
        a = z;
    }
}

LReluNeuron::~LReluNeuron()
{
}

class Layer
{
private:
    LinkedList<Neuron*> neuron_list;
public:
    Layer(int n_neurons, int neuron_type);
    int get_neuron_count();
    Node<Neuron*>* get_neuron_head();
    ~Layer();
};

Layer::Layer(int n_neurons, int neuron_type)
{
    for (int i = 0; i < n_neurons; i++)
    {
        if (neuron_type == 0)
        {
            neuron_list.push_back(new SigmoidNeuron);
        }
        else if (neuron_type == 1)
        {
            neuron_list.push_back(new LReluNeuron);
        }
        else if (neuron_type == 2)
        {
            neuron_list.push_back(new ReluNeuron);
        }
        
    }
    
}

Node<Neuron*>* Layer::get_neuron_head()
{
    return neuron_list[0];
}

int Layer::get_neuron_count()
{
    return neuron_list.get_count();
}

Layer::~Layer()
{
    Node<Neuron*>* temp = neuron_list.get_head();
    for (int i = 0; i < neuron_list.get_count(); i++)
    {
        delete temp->data;
        temp = temp->next;
    }
}

class Network
{
private:
    LinkedList<Layer*> layer_list;
public:
    Network(int n_layers, int* n_neurons, int* neuron_types, LinkedList<int>& in_val);
    void feed_forward();
    void print();
    ~Network();
};

Network::Network(int n_layers, int* n_neurons, int* neuron_types, LinkedList<int>& in_val)
{
    for (int i = 0; i < n_layers; i++)
    {
        Layer* temp = new Layer(n_neurons[i], neuron_types[i]);
        layer_list.push_back(temp);
    }
    Node<Neuron*>* in_layer_head = layer_list[0]->data->get_neuron_head();
    for (int i = 0; i < in_val.get_count(); i++)
    {
        in_layer_head->data->set_a(in_val[i]->data);
        in_layer_head->data->set_z(in_val[i]->data);
        in_layer_head = in_layer_head->next;
    }
    
}

void Network::feed_forward()
{
    //Initial matrix values are set to 0.1 by default in the matrix class.
    //For every layer in list, create the matrices with the right size
    //Set x matrix values to calculate next layer's z value
    //Do the calculation and set the next layer's z and a values
    for (int i = 0; i < layer_list.get_count() - 1; i++)
    {
        Matrix w(layer_list[i+1]->data->get_neuron_count(), layer_list[i]->data->get_neuron_count());
        Matrix x(layer_list[i]->data->get_neuron_count(), 1);
        Matrix b(layer_list[i+1]->data->get_neuron_count(), 1);

        Node<Neuron*>* temp = layer_list[i]->data->get_neuron_head();
        for (int j = 0; j < layer_list[i]->data->get_neuron_count(); j++)
        {
            x[j][0] = temp->data->get_a();
            temp = temp->next;
        }
        
        Matrix z = (w * x) + b;

        Node<Neuron*>* temp2 = layer_list[i+1]->data->get_neuron_head();
        for (int j = 0; j < z.get_rows(); j++)
        {
            temp2->data->set_z(z[j][0]);
            temp2->data->activate();
            temp2 = temp2->next;
        }
        
    }
    
}

void Network::print()
{
    for (int i = 0; i < layer_list.get_count(); i++)
    {   
        std::cout << "Layer " << i << ":\n";
        Node<Neuron*>* temp = layer_list[i]->data->get_neuron_head();
        for (int j = 0; j < layer_list[i]->data->get_neuron_count(); j++)
        {
            std::cout << temp->data->get_a() << '\n';
            temp = temp->next;
        }
        
    }
    
}

Network::~Network()
{
    Node<Layer*>* temp = layer_list.get_head();
    for (int i = 0; i < layer_list.get_count(); i++)
    {
        delete temp->data;
        temp = temp->next;
    }
}

int main(int argc, char const *argv[])
{
    bool err = false;
    std::fstream INFILE;
    INFILE.open(argv[1], std::ios::in);
    int n_layer;
    INFILE >> std::skipws >> n_layer;
    int n_neuron[n_layer]; int neuron_types[n_layer]; int n_val;
    for (int i = 0; i < n_layer; i++)
    {
        INFILE >> std::skipws >> n_neuron[i];
    }
    for (int i = 0; i < n_layer; i++)
    {
        try
        {
            int temp;
            INFILE >> std::skipws >> temp;
            if (temp != 0 && temp != 1 && temp != 2)
            {
                err = true;
                throw "Unidentified activation function!";
                break;
            }
            else
            {
                neuron_types[i] = temp;
            }
            
        }
        catch(char const* e)
        {
            std::cout << e << '\n';
        }
    }

    if (!err)
    {
        LinkedList<int> in_val;
        int temp;
        while(INFILE >> std::skipws >> temp){ in_val.push_back(temp); }
        try
        {
            if(in_val.get_count() != n_neuron[0])
            {
                err = true;
                throw "Input shape does not match!";
            }
        }
        catch(char const* e)
        {
            std::cout << e << '\n';
        }
        if(!err)
        {
            Network net(n_layer, n_neuron, neuron_types, in_val);
            net.feed_forward();
            net.print(); 
        }
        
    }
    
    return 0;
}
