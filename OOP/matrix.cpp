/******************************
** Author: Uzay Uysal        **
** Student Number: 150180039 **
******************************/


#pragma once
#include <exception>
#include <iostream>

class bad_matrix_size: public std::exception
{
    public:
    virtual const char* what() const throw()
    {
        return "Invalid matrix size for the asked operation.";
    }
};
class bad_index: public std::exception
{
    public:
    virtual const char* what() const throw()
    {
        return "Index out of bounds.";
    }
};

//Simple 2D array based matrix class for small sized matrices.
class Matrix
{
    class MRow
    {
        friend class Matrix;
        private:
            Matrix& parent;
            int row;
            MRow(Matrix& _parent, int _row) : parent(_parent), row(_row)
            {
                try
                {
                    if((row < 0) || row >= parent.n_rows) { throw bad_index(); }
                }
                catch(const std::exception& e)
                {
                    std::cerr << e.what() << '\n';
                    
                }
                
            }
        public:
            double& operator[](int col)
            {
                try
                {
                    if((col < 0) || col >= parent.n_cols) { throw bad_index(); }
                }
                catch(const std::exception& e)
                {
                    std::cerr << e.what() << '\n';
                    
                }
                
                return parent.values[row][col];
            }
    };
    private:
        unsigned n_rows;
        unsigned n_cols;
        double** values;
    public:
        Matrix(unsigned _rows, unsigned _cols, double init_val = 0.1) : n_rows(_rows) , n_cols(_cols)
        {
            values = new double*[n_rows];
            for (int i = 0; i < n_rows; i++)
            {
                values[i] = new double[n_cols];
                for (int j = 0; j < n_cols; j++)
                {
                    values[i][j] = init_val;
                }
            }
        }

        Matrix(Matrix& _obj) : n_rows(_obj.get_rows()) , n_cols(_obj.get_cols())
        {   
            values = new double*[n_rows];
            for (int i = 0; i < n_rows; i++)
            {
                values[i] = new double[n_cols];
                for (int j = 0; j < n_cols; j++)
                {
                    values[i][j] = _obj[i][j];
                }
            }
        }

        Matrix(Matrix&& _obj) : n_rows(_obj.get_rows()) , n_cols(_obj.get_cols())
        {   
            values = new double*[n_rows];
            for (int i = 0; i < n_rows; i++)
            {
                values[i] = new double[n_cols];
                for (int j = 0; j < n_cols; j++)
                {
                    values[i][j] = _obj[i][j];
                }
            }
        }

        int get_rows(){return n_rows;}
        int get_cols(){return n_cols;}

        MRow operator[](int i)
        {   
            return MRow(*this, i);
        }

        Matrix operator+(Matrix& _obj)
        {
            try
            {
                if((_obj.get_rows() != n_rows) || (_obj.get_cols() != n_cols)) 
                { 
                    throw bad_matrix_size();
                }
            }
            catch(const std::exception& e)
            {
                std::cerr << e.what() << '\n';
                
            }

            Matrix res(n_rows, n_cols, 0);

            for (int i = 0; i < n_rows; i++)
            {
                for (int j = 0; j < n_cols; j++)
                {
                    res[i][j] = (*this)[i][j] + _obj[i][j];
                }
                
            }
            
            return res;
        }

        Matrix operator+=(Matrix& _obj)
        {
            try
            {
                if((_obj.get_rows() != n_rows) || (_obj.get_cols() != n_cols)) 
                { 
                    throw bad_matrix_size();
                }
            }
            catch(const std::exception& e)
            {
                std::cerr << e.what() << '\n';
                
            }


            for (int i = 0; i < n_rows; i++)
            {
                for (int j = 0; j < n_cols; j++)
                {
                    (*this)[i][j] += _obj[i][j];
                }
                
            }
            
            return (*this);
        }

        Matrix operator-(Matrix& _obj)
        {
            try
            {
                if((_obj.get_rows() != n_rows) || (_obj.get_cols() != n_cols)) 
                { 
                    throw bad_matrix_size();
                }
            }
            catch(const std::exception& e)
            {
                std::cerr << e.what() << '\n';
                
            }

            Matrix res(n_rows, n_cols, 0);

            for (int i = 0; i < n_rows; i++)
            {
                for (int j = 0; j < n_cols; j++)
                {
                    res[i][j] = (*this)[i][j] - _obj[i][j];
                }
                
            }
            
            return res;
        }

        Matrix operator*(Matrix& _obj)
        {
            try
            {
                if(_obj.get_rows() != n_cols) 
                { 
                    throw bad_matrix_size();
                }
            }
            catch(const std::exception& e)
            {
                std::cerr << e.what() << '\n';
                
            }

            Matrix res(n_rows, _obj.get_cols(), 0);
            
            for (int i = 0; i < n_rows; i++)
            {
                for (int j = 0; j < _obj.get_cols(); j++) 
                {
                    for (int k = 0; k < n_cols; k++)
                    {
                        res[i][j] += (*this)[i][k] * _obj[k][j];
                    }  
                } 
            }
            return res;
        }

        void print()
        {
            for (int i = 0; i < n_rows; i++)
            {
                for (int j = 0; j < n_cols; j++)
                {
                    std::cout << values[i][j] << ' ';   
                }
                std::cout << '\n';
                std::cout.flush();
            }
        }

        ~Matrix()
        {
            for (int i = 0; i < n_rows; i++)
            {
                delete [] values[i];
            }
            delete [] values;
        }
};