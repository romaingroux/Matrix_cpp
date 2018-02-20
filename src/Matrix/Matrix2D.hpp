#ifndef MATRIX2D_HPP
#define MATRIX2D_HPP

#include "Matrix.hpp"

#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <sstream>

#define BUFFER_SIZE 4096

/*! The Matrix2D class implements 2D matrices.
 */
template<class T>
class Matrix2D : public Matrix<T>
{
    public:
        // constructors
        Matrix2D() = delete ;
        /*!
         * \brief Constructs a matrix with the given dimension with
         * 0 values.
         * \param dim the dimensions.
         */
        Matrix2D(size_t nrow, size_t ncol) ;
        /*!
         * \brief Constructs a matrix with the given dimensions and
         * initialize the values to the given value.
         * \param dim the dimensions.
         * \param value the value to initialize the matrix content
         * with.
         */
        Matrix2D(size_t nrow, size_t ncol, T value) ;

        /*!
         * \brief Constructs a matrix from a text file.
         * \param file_address the address of the file containing the matrix.
         */
        Matrix2D(const std::string& file_address) throw (std::runtime_error) ;

        // methods overloaded in Matrix
        using Matrix<T>::get ;
        using Matrix<T>::set ;

        // methods
        /*!
         * \brief Gets the element at the given coordinates.
         * \param row the row number of the element to set.
         * \param col the column number of the element to set.
         * \throw std::out_of_range exception if the coordinates
         * are out of range.
         * \return the element.
         */
        T get(size_t row, size_t col) const throw(std::out_of_range) ;

        /*!
         * \brief Sets the element at the given coordinates
         * to the given value.
         * \param row the row number of the element to set.
         * \param col the column number of the element to set.
         * \param value the new value.
         * \throw std::out_of_range exception if the coordinates
         * are out of range.
         */
        void set(size_t row, size_t col, T value) throw (std::out_of_range) ;

        // operators
        /*!
         * \brief Returns a reference to the corrresponding
         * element. This method does not perform any check on
         * the coordinates.
         * \param row the row number of the element to set.
         * \param col the column number of the element to set.
         * \return a reference to this element.
         */
        T& operator () (size_t row, size_t col) ;

        /*!
         * \brief Returns a const reference to the corrresponding
         * element. This method does not perform any check on
         * the coordinates.
         * \param row the row number of the element to set.
         * \param col the column number of the element to set.
         * \return a const reference to this element.
         */
        const T& operator () (size_t row, size_t col) const ;

} ;

/*!
 * \brief Produces a transpose of the given matrix.
 * \param m a matrix.
 */
template<class T>
Matrix2D<T> transpose(const Matrix2D<T>& m) ;




template<class T>
Matrix2D<T> transpose(const Matrix2D<T>& m)
{   std::vector<size_t> dim = m.get_dim() ;
    size_t nrow = dim[0] ;
    size_t ncol = dim[1] ;
    Matrix2D<T> m2(ncol, nrow, 0) ;
    for(size_t i=0; i<ncol; i++)
    {   for(size_t j=0; j<nrow; j++)
        {   m2(i,j) = m(j,i) ; }
    }
    return m2 ;
}


template<class T>
Matrix2D<T>::Matrix2D(size_t nrow, size_t ncol)
    : Matrix2D<T>(nrow, ncol, 0)
{}

template<class T>
Matrix2D<T>::Matrix2D(size_t nrow, size_t ncol, T value)
     : Matrix<T>({nrow, ncol}, value)
{}

template<class T>
Matrix2D<T>::Matrix2D(const std::string &file_address) throw (std::runtime_error)
    : Matrix<T>({0,0})
{
    this->_dim       = {0,0} ;
    this->_data      = std::vector<T>() ;
    this->_dim_size  = this->_dim.size() ;
    this->_data_size = this->_data.size() ;
    this->_dim_prod  = std::vector<size_t>(this->_dim_size, 0) ;

    std::ifstream file(file_address, std::ifstream::in) ;
    if(file.fail())
    {   char msg[BUFFER_SIZE] ;
        sprintf(msg, "error! cannot open %s", file_address.c_str()) ;
        throw std::runtime_error(msg) ;
    }

    std::string         buffer_str ;
    std::vector<T>      buffer_vec ;
    T                   buffer_T ;

    // read file
    size_t i = 0 ;
    size_t row_len = 0 ;
    while(getline(file, buffer_str))
    {   // check stream status and read content
        if(file.eof())
        {   break ; }
        if(buffer_str.size() == 0)
        {   char msg[BUFFER_SIZE] ;
            sprintf(msg, "error! while reading %s (empty line)", file_address.c_str()) ;
            throw std::runtime_error(msg) ;
        }
        if(file.fail())
        {   char msg[BUFFER_SIZE] ;
            sprintf(msg, "error! while reading %s", file_address.c_str()) ;
            throw std::runtime_error(msg) ;
        }
        if(buffer_str.size() == 0)
        {   continue ; }
        else
        {   // parse line
            buffer_vec.clear() ;
            std::istringstream buffer_ss(buffer_str) ;

            while(buffer_ss >> buffer_T)
            {   buffer_vec.push_back(buffer_T) ; }

            // check that number of column is constant
            if(i == 0)
            {  row_len = buffer_vec.size() ; }
            else if(buffer_vec.size() != row_len)
            {   char msg[BUFFER_SIZE] ;
                sprintf(msg, "error! variable number of columns in %s", file_address.c_str()) ;
                throw std::runtime_error(msg) ;
            }
            // update matrix content
            for(auto i : buffer_vec)
            {   this->_data.push_back(i) ;
                this->_data_size++ ;
            }
            this->_dim[1]++ ;
            // this->_data.push_back(buffer_vec);
        }
        i++ ;
    }
    file.close() ;

    this->_dim[0] = row_len ;
    this->compute_dim_product() ;
}



template<class T>
T Matrix2D<T>::get(size_t row, size_t col) const throw(std::out_of_range)
{   try
    {   return this->get({row, col}) ; }
    catch(std::out_of_range& e)
    {   throw e ; }
}


template<class T>
void Matrix2D<T>::set(size_t row, size_t col, T value) throw(std::out_of_range)
{   try
    {   this->get({row, col}) ; }
    catch(std::out_of_range& e)
    {   throw e ; }
}


template<class T>
T& Matrix2D<T>::operator () (size_t row, size_t col)
{   std::vector<size_t> coord = {col, row} ;
    return this->_data[this->convert_to_offset(coord)] ;
}


template<class T>
const T& Matrix2D<T>::operator () (size_t row, size_t col) const
{   std::vector<size_t> coord = {col, row} ;
    return this->_data[this->convert_to_offset(coord)] ;
}


template<class T>
std::ostream& operator << (std::ostream& stream, const Matrix2D<T>& m)
{   size_t n_data = m.get_data_size() ;
    std::vector<size_t> dim = m.get_dim() ;

    for(size_t i=0; i<n_data; i++)
    {   stream << m.get(i) << ' ' ;
        if((i != 0) and ((i+1) % dim[1] == 0))
        {   stream << std::endl ; }
    }
    return stream ;
}


#endif // MATRIX2D_HPP


