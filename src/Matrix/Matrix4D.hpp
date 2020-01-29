#ifndef MATRIX4D_HPP
#define MATRIX4D_HPP

#include <Matrix.hpp>

#include <string>
#include <vector>
#include <utility>    // std::move()
#include <stdexcept> // runtime_error, out_of_range
#include <iostream>
#include <iomanip>  // setw(), setprecision(), fixed
#include <fstream>  // ifstream
#include <sstream>  // sstream


#define BUFFER_SIZE 4096


/*!
 * The Matrix4D class is a specialisation of the Matrix
 * class to make work with 4D matrices more easily.
 *
 * A format to save a Matrix4D objects in a binary file is defined. The following values
 * are written :
 * 1x size_t :     the number <N> of dimensions of the Matrix stored. This is the value
 *                 of _dim_size field. This value must be 4 otherwise this is not a 4D
 *                 matrix.
 * 4x size_t :     the width of the matrix in each dimension. These values correspond
 *                 to the content of the _dim vector and can be loaded inside this
 *                 vector as they are.
 * Dx <T> values : the <D> values contained in the matrix, in the _data vector.
 *                 These values can be loaded directly in this vector. <D> is equal to the
 *                 product of the 4 values stored right before. The type <T> depends on
 *                 the type of the data stored in the matrix. The 1st of the D values is
 *                 also the first value of the _data vector.
 *
 *
 * A text file format is defined to store such matrices. The specifications are as
 * follows :
 * Absolutely NO empty lines are allowed!
 * The following lines should contain :
 *
 * 1st line       : a slice header ',,,0' indicating that a slice of the 4th dimension
 *                  is beginning.
 * 3nd - Nth line : the slice of the 4th dimension. It contains slice in the 3rd dimension
 *                  which are 2D matrices separated by headers (',,0' and ',,1', in the
 *                  example below, they have 2x3 dimensions).
 * N+1th line     : ',,,1' indicating that the 2nd slice of the 4th dimension is beginning.
 * and so on...
 * Example
 * ---- start ----
 * ,,,0
 * ,,0
 * 1      2      3
 * 4      5      6
 * ,,1
 * 7      8      9
 * 10     11     12
 * ,,,1
 * ,,0
 * 21     22     23
 * 24     25     26
 * ,,1
 * 27     28     29
 * 30     31     32
 * ----- end -----
 *
 * Constructing a matrix from an empty file (0 bytes or only an EOL char) returns a null
 * matrix (0x0x0x0 dimensions). Writting a null matrix (that is with at least one null
 * dimension creates an empty file.
 *
 */
template<class T>
class Matrix4D : public Matrix<T>
{   public:
        static size_t n_instance ;
    public:
        // constructors
        /*!
         * Default constructor.
         */
        Matrix4D() = default ;

        /*!
         * \brief Constructs a matrix with the given dimensions,
         * filled with 0 values.
         * \param dim1 the first dimension.
         * \param dim2 the second dimension.
         * \param dim3 the third dimension.
         * \param dim4 the fourth dimension.
         */
        Matrix4D(size_t dim1, size_t dim2, size_t dim3, size_t dim4) ;

        /*!
         * \brief Constructs a matrix with the given dimensions and
         * initialize the values to the given value.
         * \param dim1 the first dimension.
         * \param dim2 the second dimension.
         * \param dim3 the third dimension.
         * \param dim4 the fourth dimension.
         * \param value the value to initialize the matrix content
         * with.
         */
        Matrix4D(size_t dim1, size_t dim2, size_t dim3, size_t dim4, T value) ;

        /*!
         * \brief Copy constructor
         * \param other the matrix to copy the content from.
         */
        Matrix4D(const Matrix4D& other) ;

        /*!
         * \brief Mover constructor
         * \param other the matrix to copy the content from.
         */
        Matrix4D(Matrix4D&& other) ;

        /*!
         * \brief Constructs a matrix from a text file. A matrix contructed
         * from an empty file (or a file containing only one EOL char) returns
         * an empty matrix (null dimensions).
         * \param file_address the address of the file containing the matrix.
         * \throw std::runtime_error if anything happen while reading the
         * file (format error, file not found, etc).
         */
        Matrix4D(const std::string& file_address) ;

        /*!
         * \brief Destructor.
         */
        virtual ~Matrix4D() ;

        // methods overloaded from Matrix
        using Matrix<T>::get ;

        using Matrix<T>::set ;

        // methods
        /*!
         * \brief loads a matrix from the given binary
         * file.
         * \param path the path to the file to read.
         * \param dim_n the expected number of dimensions
         * of the matrix.
         * \throw std::invalid_argument if the dimensionality
         * of the matrix stored in the file is not equal to
         * the expected number of dimensions and
         * std::runtime_error if any reading error
         * occures.
         */
        void load(const std::string& file_address) ;

        /*!
         * \brief Gets the element at the given coordinates.
         * \param dim1 the first dimension coordinate.
         * \param dim2 the second dimension coordinate.
         * \param dim3 the third dimension coordinate.
         * \param dim4 the fourth dimension coordinate.
         * \throw std::out_of_range exception if the coordinates
         * are out of range.
         * \return the element.
         */
        T get(size_t dim1, size_t dim2, size_t dim3, size_t dim4) const ;

        /*!
         * \brief Sets the element at the given coordinates
         * to the given value.
         * \param dim1 the first dimension coordinate.
         * \param dim2 the second dimension coordinate.
         * \param dim3 the third dimension coordinate.
         * \param dim4 the fourth dimension coordinate.
         * \param value the new value.
         * \throw std::out_of_range exception if the coordinates
         * are out of range.
         */
        void set(size_t dim1, size_t dim2, size_t dim3, size_t dim4, T value) ;

        /*!
         * \brief Produces a nice representation of the matrix on the given
         * stream.
         * \param stream the stream.
         * \param precision the rounding precision.
         * \param width the column width in number of characters.
         * \param sep the character separator.
         */
        virtual void print(std::ostream& stream, size_t precision=4 ,size_t width=8, char sep=' ') const override ;

        // operators
        /*!
         * Assignment operator.
         * \param other an other matrix to copy the values from.
         * \return a reference to the current the instance.
         */
        Matrix4D& operator = (const Matrix4D& other) ;

        /*!
         * Move Assignment operator.
         * \param other an other matrix to use the values from.
         * \return a reference to the instance.
         */
        Matrix4D& operator = (Matrix4D&& other) ;

        /*!
         * \brief Returns a reference to the corrresponding
         * element. This method does not perform any check on
         * the coordinates.
         * \param dim1 the first dimension coordinate.
         * \param dim2 the second dimension coordinate.
         * \param dim3 the third dimension coordinate.
         * \param dim4 the third dimension coordinate.
         * \return a reference to this element.
         */
        T& operator() (size_t dim1, size_t dim2, size_t dim3, size_t dim4) ;

        /*!
         * \brief Returns a reference to the corrresponding
         * element. This method does not perform any check on
         * the coordinates.
         * \param dim1 the first dimension coordinate.
         * \param dim2 the second dimension coordinate.
         * \param dim3 the third dimension coordinate.
         * \param dim4 the third dimension coordinate.
         * \return a reference to this element.
         */
        const T& operator() (size_t dim1, size_t dim2, size_t dim3, size_t dim4) const ;

    private:
        // methods
        /*!
         * \brief Checks whether a given string is a 3D header
         * (such as ",,0"), as found in files storing Matrix4D.
         * \param str the string to check.
         * \return whether the string is such a slice header.
         */
        bool is_header_3d(const std::string& str) const ;
        /*!
         * \brief Checks whether a given string is a 4D header
         * (such as ",,,0"), as found in files storing Matrix4D.
         * \param str the string to check.
         * \return whether the string is such a slice header.
         */
        bool is_header_4d(const std::string& str) const ;

        /*!
         * \brief Routine to load 4D matrices from files.
         * This method reads from a std::ifstream object,
         * from the current pointer location until i) a 4D
         * header line is found (such as ',,,1') or ii) until
         * it cannot read anymore from the stream. All
         * data are pushed back into the data vector and
         * the dimensions of the data read are stored into
         * the dim vector (these data are actually a 3D
         * matrix). If the method returned because it
         * found another 4D header, it returns true, false
         * otherwise.
         * To read an entire 4D matrix from a file, simply
         * use this scheme : i) read the 1st 4D header
         * ii) call this function while it returns true.
         * \param file_name a reference to a string containing
         * the address of the file currently read (for exception
         * messages).
         * \param file a reference to the std::ifstream to read
         * from. Obviously, the stream state will be modified as
         * the method reads from it. However, it will never be
         * closed by the method.
         * \param data a reference to an empty vector where the
         * read data will be pushed back.
         * \param dim a reference to an empty vector where the
         * dimensions of the read data will be stored.
         * \return whether the last piece of data read from the
         * stream was a 4D header.
         */
        bool get_3d_slice(const std::string& file_name, std::ifstream& file,
                          std::vector<T>& data, std::vector<size_t>& dim) const ;

        /*!
         * \brief Converts a quadruplet of VALID (dim1, dim2, dim3, dim4)
         * coordinates to a the corresponding offset allowing to get an
         * element in the data vector.
         * \param dim1 the index of the 1st dimension slice.
         * \param dim2 the index of the 2nd dimension slice.
         * \param dim3 the index of the 3rd dimension slice.
         * \param dim4 the index of the 4th dimension slice.
         * \return the corresponding offset.
         */
        size_t convert_to_offset(size_t dim1,
                                 size_t dim2,
                                 size_t dim3,
                                 size_t dim4) const ;

        /*!
         * \brief Computes and stores the offsets at which
         * each slice on the 1st dimension starts.
         */
        void compute_dim1_offsets() ;

        /*!
         * \brief Computes and stores the offsets at which
         * each slice on the 2nd dimension starts.
         */
        void compute_dim2_offsets() ;

        /*!
         * \brief Computes and stores the offsets at which
         * each slice on the 3rd dimension starts.
         */
        void compute_dim3_offsets() ;

        /*!
         * \brief Computes and stores the offsets at which
         * each slice on the 4th dimension starts.
         */
        void compute_dim4_offsets() ;

        /*!
         * \brief Contains the offsets at which each dim1 slice
         * starts. Each element corresponds to the corresponding
         * dim1 slice (1st element -> 1st dim1 slice).
         */
        std::vector<size_t> _dim1_offsets ;
        /*!
         * \brief Contains the offsets at which each dim2 slice
         * starts. Each element corresponds to the corresponding
         * y slice (1st element -> 1st dim2 slice).
         */
        std::vector<size_t> _dim2_offsets ;
        /*!
         * \brief Contains the offsets at which each dim3 slice
         * starts. Each element corresponds to the corresponding
         * x slice (1st element -> 1st dim3 slice).
         */
        std::vector<size_t> _dim3_offsets ;
        /*!
         * \brief Contains the offsets at which each dim4 slice
         * starts. Each element corresponds to the corresponding
         * x slice (1st element -> 1st dim4 slice).
         */
        std::vector<size_t> _dim4_offsets ;

} ;

// operators
/*!
 * \brief Addition operator.
 * \param m the matrix of interest
 * \param value the value to add to each element.
 * \return the resulting matrix.
 */
template<class T>
const Matrix4D<T> operator + (Matrix4D<T> m, T value)
{   Matrix4D<T> other(m) ;
    m += value ;
    return m ;
}

/*!
 * \brief Substraction operator
 * \param m the matrix of interest.
 * \param value the value to substract to each element.
 * \return the resulting matrix.
 */
template<class T>
const Matrix4D<T> operator - (Matrix4D<T> m, T value)
{    Matrix4D<T> other(m) ;
     m -= value ;
     return m ;
}

/*!
 * \brief Multiplication operator.
 * \param m the matrix of interest.
 * \param value the value to multiply each elements by.
 * \return the resulting matrix.
 */
template<class T>
const Matrix4D<T> operator * (Matrix4D<T> m, T value)
{    Matrix4D<T> other(m) ;
     m *= value ;
     return m ;
}

/*!
 * \brief Division operator.
 * \param m the matrix of interest.
 * \param value the value to divide each elements by.
 * \throw std::invalid_argument if value is 0.
 * \return the resulting matrix.
 */
template<class T>
const Matrix4D<T> operator / (Matrix4D<T> m, T value)
{   if(value == static_cast<T>(0))
    {   throw std::invalid_argument("division by 0!") ; }
    Matrix4D<T> other(m) ;
    other /= value ;
    return other ;
}

/*!
 * \brief Sends a representation of the matrix to the stream.
 * \param stream the stream of interest.
 * \param m the matrix of interest.
 * \return a reference to the stream.
 */
template<class T>
std::ostream& operator << (std::ostream& stream, const Matrix4D<T>& m)
{   m.print(stream) ;
    return stream ;
}



// method implementation
template<class T>
Matrix4D<T>::Matrix4D(size_t dim1, size_t dim2, size_t dim3, size_t dim4)
    : Matrix4D<T>(dim1, dim2, dim3, dim4, 0)
{ ; }

template<class T>
Matrix4D<T>::Matrix4D(size_t dim1, size_t dim2, size_t dim3, size_t dim4, T value)
    : Matrix<T>({dim1, dim2, dim3, dim4}, value),
      _dim1_offsets(dim1),
      _dim2_offsets(dim2),
      _dim3_offsets(dim3),
      _dim4_offsets(dim4)
{   this->compute_dim1_offsets() ;
    this->compute_dim2_offsets() ;
    this->compute_dim3_offsets() ;
    this->compute_dim4_offsets() ;
}

template<class T>
Matrix4D<T>::Matrix4D(const Matrix4D &other)
    : Matrix<T>(other)
{   this->_dim1_offsets = other._dim1_offsets ;
    this->_dim2_offsets = other._dim2_offsets ;
    this->_dim3_offsets = other._dim3_offsets ;
    this->_dim4_offsets = other._dim4_offsets ;
}

template<class T>
Matrix4D<T>::Matrix4D(Matrix4D &&other)
    : Matrix<T>(std::move(other))
{   this->_dim1_offsets = other._dim1_offsets ;
    this->_dim2_offsets = other._dim2_offsets ;
    this->_dim3_offsets = other._dim3_offsets ;
    this->_dim4_offsets = other._dim4_offsets ;
}

template<class T>
Matrix4D<T>::Matrix4D(const std::string &file_address)
{   this->_dim       = {0,0,0,0} ;
    this->_data      = new std::vector<T>() ;
    this->_dim_size  = this->_dim.size() ;
    this->_data_size = this->_data->size() ;
    this->_dim_prod  = std::vector<size_t>(this->_dim_size, 0) ;

    std::ifstream file(file_address, std::ifstream::in) ;
    if(file.fail())
    {   char msg[BUFFER_SIZE] ;
        sprintf(msg, "error! cannot open %s", file_address.c_str()) ;
        throw std::runtime_error(msg) ;
    }

    std::string         buffer_str ;
    std::vector<T>      buffer_t ;
    std::vector<size_t> dim ;

    // read 1st line
    getline(file, buffer_str) ;

    // empty line
    if(buffer_str.size() == 0)
    {   // this file only contains one eol char and should be considered as empty,
        // -> returns empty matrix not an error
        if(file.peek() == EOF and file.eof())
        {  file.close() ;
            return ;
        }
        file.close() ;
        char msg[BUFFER_SIZE] ;
        sprintf(msg, "error! while reading %s (empty line)", file_address.c_str()) ;
        throw std::runtime_error(msg) ;
    }
    if(file.fail())
    {   file.close() ;
        char msg[BUFFER_SIZE] ;
        sprintf(msg, "error! while reading %s", file_address.c_str()) ;
        throw std::runtime_error(msg) ;
    }

    bool found_4d_header = this->is_header_4d(buffer_str) ;
    do
    {   if(file.fail())
        {   file.close() ;
            char msg[BUFFER_SIZE] ;
            sprintf(msg, "error! while reading %s", file_address.c_str()) ;
            throw std::runtime_error(msg) ;
        }
        // check empty line
        if(buffer_str.size() == 0)
        {   file.close() ;
            char msg[BUFFER_SIZE] ;
            sprintf(msg, "error! while reading %s (empty line)", file_address.c_str()) ;
            throw std::runtime_error(msg) ;
        }

        // this is the beginning of a 3D slice -> get it using routine
        if(found_4d_header)
        {   try
            {   // get slice
                buffer_t.clear() ;
                dim.clear() ;
                found_4d_header = this->get_3d_slice(file_address, file, buffer_t, dim);
                // update data
                for(const auto& i : buffer_t)
                {   this->_data->push_back(i) ;
                    this->_data_size++ ;
                }
                // update dim only for the 1st slice (the 1st slice set the dimensions)
                if(this->_dim[3] == 0)
                {   this->_dim[0] = dim[0] ;
                    this->_dim[1] = dim[1] ;
                    this->_dim[2] = dim[2] ;
                }
                // check dimensions of the slice
                else
                {   if(dim[0] != this->_dim[0] or
                       dim[1] != this->_dim[1] or
                       dim[2] != this->_dim[2])
                    {   char msg[BUFFER_SIZE] ;
                        sprintf(msg, "format error! slice have variable dimensions in %s", file_address.c_str()) ;
                        throw std::runtime_error(msg) ;
                    }
                }
                this->_dim[3]++ ;
            }
            catch(std::runtime_error& e)
            {   file.close() ;
                throw e ;
            }
        }
        // this is an error, everything between two ',,,N' header
        // should be read at once. The only way out of the loop
        // is that no more header has been read because of eof
        else if(not found_4d_header and not file.eof())
        {   file.close() ;
            char msg[BUFFER_SIZE] ;
            sprintf(msg, "error! while reading %s", file_address.c_str()) ;
            throw std::runtime_error(msg) ;
        }
    } while(found_4d_header) ;

    file.close() ;
    this->compute_dim_product() ;

    this->_dim1_offsets = std::vector<size_t>(this->_dim[1]) ;
    this->_dim2_offsets = std::vector<size_t>(this->_dim[0]) ;
    this->_dim3_offsets = std::vector<size_t>(this->_dim[2]) ;
    this->_dim4_offsets = std::vector<size_t>(this->_dim[3]) ;
    this->compute_dim1_offsets() ;
    this->compute_dim2_offsets() ;
    this->compute_dim3_offsets() ;
    this->compute_dim4_offsets() ;
}

template<class T>
Matrix4D<T>::~Matrix4D()
{   if(this->_data != nullptr)
    {   delete this->_data ;
        this->_data = nullptr ;
    }
}

template<class T>
void Matrix4D<T>::load(const std::string& file_address)
{
    Matrix<T>::load(file_address, 4) ;

    this->_dim1_offsets = std::vector<size_t>(this->_dim[1]) ;
    this->_dim2_offsets = std::vector<size_t>(this->_dim[0]) ;
    this->_dim3_offsets = std::vector<size_t>(this->_dim[2]) ;
    this->_dim4_offsets = std::vector<size_t>(this->_dim[3]) ;

    this->compute_dim1_offsets() ;
    this->compute_dim2_offsets() ;
    this->compute_dim3_offsets() ;
    this->compute_dim4_offsets() ;
}

template<class T>
T Matrix4D<T>::get(size_t dim1, size_t dim2, size_t dim3, size_t dim4) const
{   try
    {   return this->get({dim1, dim2, dim3, dim4}) ; }
    catch(std::out_of_range& e)
    {   throw e ; }
}

template<class T>
void Matrix4D<T>::set(size_t dim1, size_t dim2, size_t dim3, size_t dim4, T value)
{   try
    {   this->set({dim1, dim2, dim3, dim4}, value) ; }
    catch(std::out_of_range& e)
    {   throw e ; }
}

template<class T>
void Matrix4D<T>::print(std::ostream &stream, size_t precision, size_t width, char sep) const
{   // if the matrix has at least one 0 dimension (no data), don't do anything
    if(this->_dim[0]==0 or this->_dim[1]==0 or this->_dim[2]==0 or this->_dim[3]==0)
    {   return ; }

    stream.setf(std::ios::left) ;
    stream << std::setprecision(precision) << std::fixed ;
    std::vector<size_t> dim = this->get_dim() ;

    size_t    n  = 0 ;
    size_t n_tot = std::accumulate(dim.begin(), dim.end(), (size_t)1, std::multiplies<int>()) ;

    for(size_t dim4=0; dim4<dim[3]; dim4++)
    {   stream << ",,," << dim4 << std::endl ;
        for(size_t dim3=0; dim3<dim[2]; dim3++)
        {   stream << ",," << dim3 << std::endl ;
            for(size_t dim2=0; dim2<dim[0]; dim2++)
            {   for(size_t dim1=0; dim1<dim[1]; dim1++, n++)
                {   stream << std::setw(width) << (*this)(dim2,dim1,dim3,dim4) << sep ; }
                // avoids terminal eol
                if(n < n_tot)
                {   stream << std::endl ; }
            }
        }
    }
}

template<class T>
Matrix4D<T>& Matrix4D<T>::operator = (const Matrix4D<T>& other)
{   /*
    this->_dim           = other._dim ;
    this->_dim_size      = other._dim_size ;
    this->_data          = new std::vector<T>(*(other._data)) ;
    this->_data_size     = other._data_size ;
    this->_dim_prod      = other._dim_prod ;
    */
    Matrix<T>::operator=(other) ;
    this->_dim1_offsets = other._dim1_offsets ;
    this->_dim2_offsets = other._dim2_offsets ;
    this->_dim3_offsets = other._dim3_offsets ;
    this->_dim4_offsets = other._dim4_offsets ;
    return *this ;
}

template<class T>
Matrix4D<T>& Matrix4D<T>::operator = (Matrix4D<T>&& other)
{   Matrix<T>::operator=(std::move(other)) ;
    this->_dim1_offsets = other._dim1_offsets ;
    this->_dim2_offsets = other._dim2_offsets ;
    this->_dim3_offsets = other._dim3_offsets ;
    this->_dim4_offsets = other._dim4_offsets ;
    return *this ;
}

template<class T>
T& Matrix4D<T>::operator () (size_t dim1, size_t dim2, size_t dim3, size_t dim4)
{   return (*this->_data)[this->convert_to_offset(dim1, dim2, dim3, dim4)] ; }

template<class T>
const T& Matrix4D<T>::operator () (size_t dim1, size_t dim2, size_t dim3, size_t dim4) const
{   return (*this->_data)[this->convert_to_offset(dim1, dim2, dim3, dim4)] ; }

template<class T>
bool Matrix4D<T>::is_header_3d(const std::string &str) const
{   if(str[0] == ',' and
       str[1] == ',' and
       str.find(',', 2) == std::string::npos)
    {   return true ; }
    return false ;
}

template<class T>
bool Matrix4D<T>::is_header_4d(const std::string &str) const
{   if(str[0] == ',' and
       str[1] == ',' and
       str[2] == ',' and
       str.find(',', 3) == std::string::npos)
    {   return true ; }
    return false ;
}

template<class T>
bool Matrix4D<T>::get_3d_slice(const std::string& file_name, std::ifstream& file,
                               std::vector<T> &data, std::vector<size_t> &dim) const
{
    bool found_4d_header = false ; // the flag to return

    dim = {0,0,0} ;

    std::string         buffer_str ;
    std::vector<T>      buffer_vec ;
    T                   buffer_T ;

    size_t n_line      = 0, n_line_data = 0 ; // number of line and of data line read
    size_t row_len     = 0, col_len = 0 ;     // length of row and column in nber of values
    size_t row_len_cur = 0, col_len_cur = 0 ; // current number of values read in row and col

    while(getline(file, buffer_str))
    {   if(file.fail())
        {   char msg[BUFFER_SIZE] ;
            sprintf(msg, "error! while reading %s", file_name.c_str()) ;
            throw std::runtime_error(msg) ;
        }
        // check empty line
        if(buffer_str.size() == 0)
        {   char msg[BUFFER_SIZE] ;
            sprintf(msg, "error! while reading %s (empty line)", file_name.c_str()) ;
            throw std::runtime_error(msg) ;
        }
        // check whether this is the beginning of a 4D slice header, if so
        // break
        if(this->is_header_4d(buffer_str))
        {   found_4d_header = true ;
            break ;
        }
        // check whether it is the beginning of a slice
        // 1st line in file should be
        if(this->is_header_3d(buffer_str))
        {   // check that slice have a constant number of rows
            if(dim[2] == 1)
            {   col_len = col_len_cur ;
                // dim[0] = row_len ;
                // dim[1]  = col_len ;
            }
            else if(col_len_cur != col_len)
            {   char msg[BUFFER_SIZE] ;
                sprintf(msg, "format error! slice have variable dimensions in %s", file_name.c_str()) ;
                throw std::runtime_error(msg) ;
            }
            dim[2]++ ;
            col_len_cur = 0 ;
            n_line++ ;
            continue ;
        }
        // 1st line in file should be a header and entering
        // this block is forbidden
        if(n_line == 0)
        {   char msg[BUFFER_SIZE] ;
            sprintf(msg, "format error! first line is not a slice header in %s", file_name.c_str()) ;
            throw std::runtime_error(msg) ;
        }

        // parse line
        row_len_cur = 0 ;
        buffer_vec.clear() ;
        std::istringstream buffer_ss(buffer_str) ;
        while(buffer_ss >> buffer_T)
        {   buffer_vec.push_back(buffer_T) ;
            row_len_cur++ ;
        }
        // check for an error which likely indicates that a value could not be
        // casted into a type T (mixed data types in the file)
        if(buffer_ss.fail() and not buffer_ss.eof())
        {   char msg[BUFFER_SIZE] ;
            sprintf(msg, "format error! could not read a line in %s (incompatible data types)", file_name.c_str()) ;
            throw std::runtime_error(msg) ;
        }

        // check that number of column is constant
        if(n_line_data == 0)
        {  row_len = row_len_cur ; }
        else if(row_len_cur != row_len)
        {   char msg[BUFFER_SIZE] ;
            sprintf(msg, "format error! slice have variable dimensions in %s", file_name.c_str()) ;
            throw std::runtime_error(msg) ;
        }

        // update matrix content
        for(auto i : buffer_vec)
        {   data.push_back(i) ; }
        col_len_cur++ ;
        n_line_data++ ;
        n_line++ ;
        // update dimension
        dim[0] = row_len_cur ;
        dim[1] = col_len_cur ;
    }

    // check dimensions of last slice
    if(col_len_cur != dim[1])
    {   char msg[BUFFER_SIZE] ;
        sprintf(msg, "format error! slice have variable dimensions 333 in %s", file_name.c_str()) ;
        throw std::runtime_error(msg) ;
    }

    return found_4d_header ;
}

template<class T>
void Matrix4D<T>::compute_dim1_offsets()
{   for(size_t i=0; i<this->_dim[1]; i++)
    {   this->_dim1_offsets[i] = i * this->_dim_prod[1] ; }
}

template<class T>
void Matrix4D<T>::compute_dim2_offsets()
{   for(size_t i=0; i<this->_dim[0]; i++)
    {   this->_dim2_offsets[i] = i * this->_dim_prod[0] ; }
}

template<class T>
void Matrix4D<T>::compute_dim3_offsets()
{   for(size_t i=0; i<this->_dim[2]; i++)
    {   this->_dim3_offsets[i] = i * this->_dim_prod[2] ; }
}

template<class T>
void Matrix4D<T>::compute_dim4_offsets()
{   for(size_t i=0; i<this->_dim[3]; i++)
    {   this->_dim4_offsets[i] = i * this->_dim_prod[3] ; }
}

template<class T>
size_t Matrix4D<T>::convert_to_offset(size_t dim1,
                                      size_t dim2,
                                      size_t dim3,
                                      size_t dim4) const
{   /*
    size_t offset = 0 ;

    for(size_t i=0; i<this->_dim_size; i++)
    {   offset += coord[i] * this->_dim_prod[i] ; }

    return offset ;
    */
    size_t offset = this->_dim1_offsets[dim1] +
                    this->_dim2_offsets[dim2] +
                    this->_dim3_offsets[dim3] +
                    this->_dim4_offsets[dim4] ;
    return offset ;
}

#endif // MATRIX4D_HPP
