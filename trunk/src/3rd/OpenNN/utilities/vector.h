/****************************************************************************************************************/
/*                                                                                                              */
/*   OpenNN: Open Neural Networks Library                                                                       */
/*   www.opennn.cimne.com                                                                                       */
/*                                                                                                              */
/*   V E C T O R   C O N T A I N E R                                                                            */
/*                                                                                                              */
/*   Roberto Lopez                                                                                              */
/*   International Center for Numerical Methods in Engineering (CIMNE)                                          */
/*   Technical University of Catalonia (UPC)                                                                    */
/*   Barcelona, Spain                                                                                           */
/*   E-mail: rlopez@cimne.upc.edu                                                                               */
/*                                                                                                              */
/****************************************************************************************************************/

#ifndef __VECTOR_H__
#define __VECTOR_H__

// System includes

#include <algorithm> 
#include <climits>
#include <cmath>
#include <cstdlib>

#include <fstream>
#include <iostream>
#include <iterator> 
#include <istream> 
#include <ostream> 
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

namespace OpenNN
{

/// Forward declaration of Matrix template

template<class Type> class Matrix;

/// This template class defines a vector for general purpose use.
/// This vector also implements some mathematical methods which can be useful. 

template <class Type> 
class Vector : public std::vector<Type>
{

public:

// CONSTRUCTORS

/// Default constructor. It creates a vector of size zero.

explicit Vector(void) : std::vector<Type>()
{
}


/// General constructor. It creates a vector of size n, containing n copies of the default value for Type.
/// @param new_size Size of vector.

explicit Vector(const unsigned int& new_size) : std::vector<Type>(new_size)
{
}


/// Constant reference initialization constructor.
/// It creates a vector of size n, containing n copies of the type value of Type. 
/// @param new_size Size of Vector.
/// @param value Initialization value of Type.

explicit Vector(const unsigned int& new_size, const Type& value) : std::vector<Type>(new_size, value)
{
}


/// File constructor. It creates a vector object by loading its members from a data file.
/// @param filename Name of vector data file.

explicit Vector(const std::string& filename) : std::vector<Type>() 
{
   load(filename);
}


/// Sequential constructor. 

explicit Vector(const Type& first, const int& step, const Type& last) : std::vector<Type>()
{
   if(first > last && step > 0 )
   {
   }
   else if(first < last && step < 0)
   {
   }
   else
   {
      const unsigned int new_size = 1 + (unsigned int)((last - first)/step + 0.5);
      
      this->resize(new_size);      

      for(unsigned int i = 0; i < new_size; i++)
      {
         (*this)[i] = first + i*step;
      }
   }
}


/// Input iterators constructor 

template<class InputIterator>
Vector(InputIterator first, InputIterator last) : std::vector<Type>(first, last)
{
}

  
/// Copy constructor. It creates a copy of an existing Vector. 
/// @param other_vector Vector to be copied.

Vector(const Vector<Type>& other_vector) : std::vector<Type>(other_vector)
{
}


// DESTRUCTOR

/// Destructor. 

~Vector(void)
{
}


// bool operator == (const Type&) const

/// Equal to operator between this vector and a Type value.
/// It produces true if all the elements of this vector are equal to the Type value, and false otherwise.
/// @param value Type value to be compared with.

inline bool operator == (const Type& value) const
{  
   const unsigned int this_size = this->size();
     
   for(unsigned int i = 0; i < this_size; i++)
   {
      if((*this)[i] != value)
      {
         return(false);
      }     
   }
   
   return(true);
}


// bool operator != (const Type&) const

/// Not equivalent relational operator between this vector and a Type value.
/// It produces true if some element of this vector is not equal to the Type value, and false otherwise.
/// @param value Type value to be compared with.

inline bool operator != (const Type& value) const
{            
   const unsigned int this_size = this->size(); 

   for(unsigned int i = 0; i < this_size; i++)
   {
      if((*this)[i] != value)
      {
         return(true);
      }     
   }

   return(false);
}


// bool operator > (const Type&) const

/// Greater than relational operator between this vector and a Type value.
/// It produces true if all the elements of this vector are greater than the Type value, and false otherwise.
/// @param value Type value to be compared with.

inline bool operator > (const Type& value) const
{
   const unsigned int this_size = this->size();      
       
   for(unsigned int i = 0; i < this_size; i++)
   {
      if((*this)[i] <= value)
      {
         return(false);
      }     
   }
   
   return(true);
}


// bool operator < (const Type&) const

/// Less than relational operator between this vector and a Type value.
/// It produces true if all the elements of this vector are less than the Type value, and false otherwise.
/// @param value Type value to be compared with.

inline bool operator < (const Type& value) const
{ 
   const unsigned int this_size = this->size(); 
       
   for(unsigned int i = 0; i < this_size; i++)
   {
      if((*this)[i] >= value)
      {
         return(false);
      }     
   }
   
   return(true);
}


// bool operator >= (const Type&) const

/// Greater than or equal to than relational operator between this vector and a Type value.
/// It produces true if all the elements of this vector are greater than or equal to the Type value, and false otherwise.
/// @param value Type value to be compared with.

inline bool operator >= (const Type& value) const
{       
   const unsigned int this_size = this->size();

   for(unsigned int i = 0; i < this_size; i++)
   {
      if((*this)[i] < value)
      {
         return(false);
      }     
   }
   
   return(true);
}


// bool operator <= (const Type&) const

/// Less than or equal to than relational operator between this vector and a Type value.
/// It produces true if all the elements of this vector are less than or equal to the Type value, and false otherwise.
/// @param value Type value to be compared with.

inline bool operator <= (const Type& value) const
{   
   const unsigned int this_size = this->size(); 

   for(unsigned int i = 0; i < this_size; i++)
   {
      if((*this)[i] > value)
      {
         return(false);
      }     
   }
   
   return(true);
}


// METHODS

// void set(void) method

/// This method sets the size of a vector to zero. 

void set(void) 
{
   this->resize(0);
}


// void set(unsigned int) method

/// This method sets a new size to the vector. It does not initialize the data.
/// @param new_size Size for the vector.

void set(const unsigned int& new_size)
{
   this->resize(new_size);
}


// void set(const unsigned int&, const Type&) method

/// This method sets a new size to the vector and initializes all its elements with a given value. 
/// @param new_size Size for the vector.
/// @param new_value Value for all the elements.

void set(const unsigned int& new_size, const Type& new_value)
{
   this->resize(new_size);

   initialize(new_value);
}


// void set(const std::string&) method

/// This method sets all the members of a vector object by loading them from a data file.
/// The format is specified in the OpenNN manual. 
/// @param filename Name of vector data file. 

void set(const std::string& filename) 
{
   load(filename);
}


// void set(const Type&, const int&, const Type&) method

/// This method makes this vector to have elements starting from a given value, continuing with a step value and finishing with a given value.
/// Depending on the starting, step and finishin values, this method can produce a variety of sizes and data. 
/// @param first Starting value. 
/// @param step Step value. 
/// @param last Finishing value. 

void set(const Type& first, const int& step, const Type& last) 
{
   if(first > last && step > 0 )
   {
      this->resize(0);
   }
   else if(first < last && step < 0)
   {
      this->resize(0);
   }
   else
   {
      const unsigned int new_size = 1 + (unsigned int)((last - first)/step + 0.5);

      this->resize(new_size);

      for(unsigned int i = 0; i < new_size; i++)
      {
         (*this)[i] = first + i*step;
      }
   }
}


// void set(const Type&, const double&, const Type&) method

/// This method makes this vector to have elements starting from a given value, continuing with a step value and finishing with a given value.
/// Depending on the starting, step and finishin values, this method can produce a variety of sizes and data. 
/// @param first Starting value. 
/// @param step Step value. 
/// @param last Finishing value. 

void set(const Type& first, const double& step, const Type& last) 
{
   if(first > last && step > 0 )
   {
      this->resize(0);
   }
   else if(first < last && step < 0)
   {
      this->resize(0);
   }
   else
   {
      const unsigned int new_size = 1 + (unsigned int)((last - first)/step + 0.5);

      this->resize(new_size);

      for(unsigned int i = 0; i < new_size; i++)
      {
         (*this)[i] = first + i*step;
      }
   }
}


// void set(const Vector&) method

/// This method sets the members of this object with the values of another vector. 
/// @param other_vector Object to set this vector. 

void set(const Vector& other_vector)
{
   *this = other_vector;
}


// void initialize(const Type&) method

/// This method initializes all the elements of the vector with a given value.
/// @param value Type value.

void initialize(const Type& value)
{
   const unsigned int this_size = this->size();

   for(unsigned int i = 0; i < this_size; i++)
   {
      (*this)[i] = value;
   }
}


// void initialize_sequential(void) method

/// This method initializes all the elements of the vector in a sequential order.

void initialize_sequential(void)
{
   const unsigned int this_size = this->size();

   for(unsigned int i = 0; i < this_size; i++)
   {
      (*this)[i] = i;
   }
}


// void initialize_uniform(void) method

/// This method assigns a random value comprised between -1 and 1 to each element in the vector. 

void initialize_uniform(void)
{   
   const unsigned int this_size = this->size();

   for(unsigned int i = 0; i < this_size; i++)
   {
      (*this)[i] = calculate_random_uniform(-1.0, 1.0);
   }
}


// void initialize_uniform(const double&, const double&) method

/// This method assigns a random value comprised between a minimum value and a maximum value to each element in
/// the vector. 
/// @param minimum Minimum initialization value.  
/// @param maximum Maximum initialization value.

void initialize_uniform(const double& minimum, double maximum)
{
   // Control sentence (if debug)

   #ifdef _DEBUG 

   if(minimum > maximum)
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: Vector Template.\n" 
             << "void initialize_uniform(const double&, const double&) method.\n"
             << "Minimum value must be less or equal than maximum value.\n";

	  throw std::logic_error(buffer.str());
   }

   #endif

   const unsigned int this_size = this->size();

   for(unsigned int i = 0; i < this_size; i++)
   {
      (*this)[i] = calculate_random_uniform(minimum, maximum);
   }
}


// void initialize_uniform(const Vector<double>&, const Vector<double>&) method

/// This method assigns a random value comprised between given minimum and a maximum values to every element in the 
/// vector. 
/// @param minimum Minimum initialization values.  
/// @param maximum Maximum initialization values.

void initialize_uniform(const Vector<double>& minimum, const Vector<double>& maximum)
{
   const unsigned int this_size = this->size();

   // Control sentence (if debug)

   #ifdef _DEBUG 

   const unsigned int minimum_size = minimum.size();
   const unsigned int maximum_size = maximum.size();

   if(minimum_size != this_size || maximum_size != this_size)
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: Vector Template.\n" 
             << "void initialize_uniform(const Vector<double>&, const Vector<double>&) method.\n"
             << "Minimum and maximum sizes must be equal to vector size.\n";

	  throw std::logic_error(buffer.str());
   }

   if(minimum > maximum)
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: Vector Template.\n" 
             << "void initialize_uniform(const Vector<double>&, const Vector<double>&) method.\n"
             << "Minimum values must be less or equal than their corresponding maximum values.\n";

	  throw std::logic_error(buffer.str());
   }

   #endif

   for(unsigned int i = 0; i < this_size; i++)
   {
      (*this)[i] = calculate_random_uniform(minimum[i], maximum[i]);
   }
}


// void initialize_normal(void) method

/// This method assigns random values to each element in the vector. These are taken from a normal distribution with
/// mean 0 and standard deviation 1. 

void initialize_normal(void) 
{
   const unsigned int this_size = this->size();

   for(unsigned int i = 0; i < this_size; i++)
   {
      (*this)[i] = calculate_random_normal(0.0, 1.0);
   }
}


// void initialize_normal(const double&, const double&) method

/// This method assigns random values to each element in the vector. 
/// These are taken from a normal distribution with single mean and standard deviation values for all the elements. 
/// @param mean Mean value of uniform distribution.  
/// @param standard_deviation Standard deviation value of uniform distribution.

void initialize_normal(const double& mean, double standard_deviation)
{
   // Control sentence (if debug)

   #ifdef _DEBUG 

   if(standard_deviation < 0.0)
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: Vector Template.\n" 
             << "void initialize_normal(const double&, const double&) method.\n"
             << "Standard deviation must be equal or greater than zero.\n";

	  throw std::logic_error(buffer.str());
   }

   #endif

   const unsigned int this_size = this->size();

   for(unsigned int i = 0; i < this_size; i++)
   {
      (*this)[i] = calculate_random_normal(mean, standard_deviation);
   }
}


// void initialize_normal(const Vector<double>, const Vector<double>) method

/// This method assigns random values to each element in the vector. 
/// These are taken from normal distributions with given means and standard deviations for each element. 
/// @param mean Mean values of normal distributions.  
/// @param standard_deviation Standard deviation values of normal distributions.

void initialize_normal(const Vector<double>& mean, const Vector<double>& standard_deviation)
{
   const unsigned int this_size = this->size();

   // Control sentence (if debug)

   #ifdef _DEBUG 
 
   const unsigned int mean_size = mean.size();
   const unsigned int standard_deviation_size = standard_deviation.size();

   if(mean_size != this_size || standard_deviation_size != this_size)
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: Vector Template.\n" 
             << "void initialize_normal(const Vector<double>&, const Vector<double>&) method.\n"
             << "Mean and standard deviation sizes must be equal to vector size.\n";

	  throw std::logic_error(buffer.str());
   }

   if(standard_deviation < 0.0)
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: Vector Template.\n" 
             << "void initialize_normal(const Vector<double>&, const Vector<double>&) method.\n"
             << "Standard deviations must be equal or greater than zero.\n";

	  throw std::logic_error(buffer.str());
   }

   #endif

   for(unsigned int i = 0; i < this_size; i++)
   {
      (*this)[i] = calculate_random_normal(mean[i], standard_deviation[i]);
   }
}


// Type calculate_minimum(void) const method

/// This method returns the smallest element in the vector.

Type calculate_minimum(void) const
{
   const unsigned int this_size = this->size();

   Type minimum = (*this)[0];

   for(unsigned int i = 1; i < this_size; i++)
   {
      if((*this)[i] < minimum)
      {
         minimum = (*this)[i];
      }
   }
   
   return(minimum);
}


// Type calculate_maximum(void) const method

/// This method returns the largest element in the vector.

Type calculate_maximum(void) const
{
   const unsigned int this_size = this->size();

   Type maximum = (*this)[0];

   for(unsigned int i = 1; i < this_size; i++)
   {
      if((*this)[i] > maximum)
      {
         maximum = (*this)[i];
      }
   }
   
   return(maximum);
}


// Vector<Type> calculate_minimum_maximum(void) const method

/// This method returns a vector containing the smallest and the largest elements in the vector.

Vector<Type> calculate_minimum_maximum(void) const
{
   unsigned int this_size = this->size();

   Type minimum = (*this)[0];
   Type maximum = (*this)[0];

   for(unsigned int i = 1; i < this_size; i++)
   {
      if((*this)[i] < minimum)
      {
         minimum = (*this)[i];
      }

      if((*this)[i] > maximum)
      {
         maximum = (*this)[i];
      }
   }

   Vector<Type> minimum_maximum(2);
   minimum_maximum[0] = minimum;
   minimum_maximum[1] = maximum;

   return(minimum_maximum);
}


// Vector< Vector<Type> > calculate_histogram(const unsigned int&) const method

/// This method bins the elements of the vector into a given number of equally spaced containers.
/// It returns a vector of two vectors. 
/// The size of both subvectors is the number of bins. 
/// The first subvector contains the frequency of the bins. 
/// The second subvector contains the center of the bins. 

Vector< Vector<Type> > calculate_histogram(const unsigned int& bins_number) const
{
   // Control sentence (if debug)

   #ifdef _DEBUG 

   if(bins_number < 1)
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: Vector Template.\n" 
             << "Vector< Vector<Type> > calculate_histogram(const unsigned int&) const method.\n"
             << "Number of beans must be equal or greater than one.\n";

	  throw std::logic_error(buffer.str());
   }

   #endif

   Vector<Type> bins_minimum(bins_number);
   Vector<Type> bins_maximum(bins_number);

   Vector<Type> bins_center(bins_number);
   Vector<Type> bins_frequency(bins_number, 0);

   const Vector<Type> minimum_maximum = calculate_minimum_maximum();
  
   const Type minimum = minimum_maximum[0];
   const Type maximum = minimum_maximum[1];

   const double bin_length = (maximum-minimum)/((double)bins_number);

   bins_minimum[0] = minimum;
   bins_maximum[0] = minimum + bin_length;
   bins_center[0] = (bins_maximum[0] + bins_minimum[0])/2.0;

   // Calculate bins center

   for(unsigned int i = 1; i < bins_number; i++)
   {
      bins_minimum[i] = bins_minimum[i-1] + bin_length;
      bins_maximum[i] = bins_maximum[i-1] + bin_length;

      bins_center[i] = (bins_maximum[i] + bins_minimum[i])/2.0;
   }

   // Calculate bins frequency

   const unsigned int this_size = this->size();

   for(unsigned int i = 0; i < this_size; i++)
   {
      if((*this)[i] < bins_center[0])
      {
         bins_frequency[0]++;
      }

      for(unsigned int j = 1; j < bins_number-1; j++)
      {
         if((*this)[i] >= bins_minimum[j] && (*this)[i] < bins_maximum[j])
		 {
		    bins_frequency[j]++;
		 }
	  }

      if((*this)[i] >= bins_center[bins_number-1])
      {
         bins_frequency[bins_number-1]++;
      }
   }

   Vector< Vector<Type> > histogram(2);

   histogram[0] = bins_frequency;
   histogram[1] = bins_center;

   return(histogram);
}


// Vector< Vector<Type> > calculate_histogram(void) const method

/// This method bins the elements of the vector into 10 equally spaced containers.
/// It returns a vector of two vectors. 
/// The size of both subvectors is the number of bins (10). 
/// The first subvector contains the frequency of the bins. 
/// The second subvector contains the center of the bins. 

Vector< Vector<Type> > calculate_histogram(void) const
{
	return(calculate_histogram(10));
}


// unsigned int calculate_minimal_index(void) const method

/// This method returns the index of the smallest element in the vector.

unsigned int calculate_minimal_index(void) const
{
   const unsigned int this_size = this->size();

   Type minimum = (*this)[0];
   unsigned int minimal_index = 0;

   for(unsigned int i = 1; i < this_size; i++)
   {
      if((*this)[i] < minimum)
      {
         minimum = (*this)[i];
         minimal_index = i;
      }
   }
   
   return(minimal_index);
}


// unsigned int calculate_maximal_index(void) const method

/// This method returns the index of the largest element in the vector.

unsigned int calculate_maximal_index(void) const
{
   const unsigned int this_size = this->size();

   Type maximum = (*this)[0];
   unsigned int maximal_index = 0;

   for(unsigned int i = 1; i < this_size; i++)
   {
      if((*this)[i] > maximum)
      {
         maximum = (*this)[i];
         maximal_index = i;
      }
   }
   
   return(maximal_index);
}


// Vector<unsigned int> calculate_minimal_maximal_index(void) const method

/// This method returns a vector with the indexes of the smallest and the largest elements in the vector.

Vector<unsigned int> calculate_minimal_maximal_index(void) const
{
   unsigned int this_size = this->size();

   Type minimum = (*this)[0];
   Type maximum = (*this)[0];

   unsigned int minimal_index = 0;
   unsigned int maximal_index = 0;

   for(unsigned int i = 1; i < this_size; i++)
   {
      if((*this)[i] < minimum)
      {
         minimum = (*this)[i];
         minimal_index = i;
      }
      if((*this)[i] > maximum)
      {
         maximum = (*this)[i];
         maximal_index = i;
      }
   }

   Vector<unsigned int> minimal_maximal_index(2);
   minimal_maximal_index[0] = minimal_index;
   minimal_maximal_index[1] = maximal_index;
   
   return(minimal_maximal_index);
}


// Vector<double> calculate_pow(const Type&) const method

/// This method returns a vector with the elements of this vector raised to a power exponent.
/// @param exponent Pow exponent. 

Vector<Type> calculate_pow(const Type& exponent) const
{
   const unsigned int this_size = this->size();

   Vector<Type> power(this_size);

   for(unsigned int i = 0; i < this_size; i++)
   {
      power[i] = pow((*this)[i], exponent);
   }

   return(power);
}


// Vector<double> calculate_competitive(void) const method

/// This method returns the competitive vector of this vector, 
/// whose elements are one the bigest element of this vector, and zero for the other elements. 

Vector<Type> calculate_competitive(void) const
{
   const unsigned int this_size = this->size();

   Vector<Type> competitive(this_size, 0);

   const unsigned int maximal_index = calculate_maximal_index();

   competitive[maximal_index] = 1;

   return(competitive);
}


// Vector<Type> calculate_softmax(void) const method 

/// This method returns the softmax vector of this vector, 
/// whose elements sum one, and can be interpreted as probabilities. 

Vector<Type> calculate_softmax(void) const
{
   const unsigned int this_size = this->size();

   Vector<Type> softmax(this_size);

   Type sum = 0;

   for(unsigned int i = 0; i < this_size; i++)
   {
      sum += exp((*this)[i]);
   }

   for(unsigned int i = 0; i < this_size; i++)
   {
      softmax[i] = exp((*this)[i])/sum;
   }

   return(softmax);
}


// Matrix<Type> calculate_softmax_Jacobian(void) const method 

/// This method returns the softmax Jacobian of this vector. 
/// @todo

Matrix<Type> calculate_softmax_Jacobian(void) const
{
   const unsigned int this_size = this->size();

   Matrix<Type> softmax_Jacobian(this_size, this_size);

   return(softmax_Jacobian);
}



// Vector<bool> calculate_binary(void) const method 

/// This method converts the values of the vector to be binary. 
/// The threshold value used is 0.5.

Vector<bool> calculate_binary(void) const
{
   const unsigned int this_size = this->size();

   Vector<bool> binary(this_size);

   for(unsigned int i = 0; i < this_size; i++)
   {
      if((*this)[i] < 0.5)
	  {
	     binary[i] = false;
	  }
	  else
	  {
	     binary[i] = true;	  
	  }
   }

   return(binary);
}


// unsigned int calculate_cumulative_index(const Type&) const method

/// This method applies only to cumulative vectors. 
/// It returns the index of the first element which is greater than a given value.
/// @param value Value. 

unsigned int calculate_cumulative_index(const Type& value) const
{
   const unsigned int this_size = this->size();

   // Control sentence (if debug)

   #ifdef _DEBUG 

   if(this_size == 0)
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: Vector Template.\n" 
             << "unsigned int calculate_cumulative_index(const Type&) const.\n"
             << "Size must be greater than zero.\n";

	  throw std::logic_error(buffer.str());
   }

   Type cumulative_value = (*this)[size()-1];

   if(value > cumulative_value)
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: Vector Template.\n" 
             << "unsigned int calculate_cumulative_index(const Type&) const.\n"
             << "Value (" << value << ") must be less than cumulative value (" << cumulative_value << ").\n";

	  throw std::logic_error(buffer.str());
   }

   for(unsigned int i = 1; i < this_size; i++)
   {
      if((*this)[i] < (*this)[i-1])
      {
         std::ostringstream buffer;

         buffer << "OpenNN Exception: Vector Template.\n" 
                << "int calculate_cumulative_index(const Type&) const.\n"
                << "Vector elements must be crescent.\n";

	  throw std::logic_error(buffer.str());
      }   
   }

   #endif

   if(value <= (*this)[0])
   {
      return(0);         
   }

   for(unsigned int i = 1; i < this_size; i++)
   {
      if(value > (*this)[i-1] && value <= (*this)[i])
      {
         return(i);         
	  }
   }
   
   return(this_size-1);
}


// unsigned int calculate_closest_index(const Type&) const method

/// This method returns the index of the closest element in the vector to a given value.

unsigned int calculate_closest_index(const Type& value) const
{
   const Vector<Type> difference = (*this - value).calculate_absolute_value();

   const unsigned int closest_index = difference.calculate_minimal_index();
   
   return(closest_index);
}


// Type calculate_sum(void) const method

/// This method returns the sum of the elements in the vector.

Type calculate_sum(void) const
{
   const unsigned int this_size = this->size();

   Type sum = 0;

   for(unsigned int i = 0; i < this_size; i++)
   {
      sum += (*this)[i];
   }
   
   return(sum);
}


// Type calculate_product(void) const method

/// This method returns the product of the elements in the vector.

Type calculate_product(void) const 
{   
   const unsigned int this_size = this->size();

   Type product = 1;

   for(unsigned int i = 0; i < this_size; i++)
   {
      product *= (*this)[i];
   }
   
   return(product);
}


// double calculate_mean(void) const method

/// This method returns the mean of the elements in the vector.

double calculate_mean(void) const
{
   const unsigned int this_size = this->size();

   // Control sentence (if debug)

   #ifdef _DEBUG 

   if(this_size == 0)
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: Vector Template.\n" 
             << "double calculate_mean(void) const method.\n"
             << "Size must be greater than zero.\n";

	  throw std::logic_error(buffer.str());
   }

   #endif

   const Type sum = calculate_sum();

   const double mean = sum/(double)this_size;
   
   return(mean);
}


// double calculate_standard_deviation(void) method

/// This method returns the standard deviation of the elements in the vector.

double calculate_standard_deviation(void) const
{
   const unsigned int this_size = this->size();

   // Control sentence (if debug)

   #ifdef _DEBUG 

   if(this_size == 0)
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: Vector Template.\n" 
             << "double calculate_standard_deviation(void) const method.\n"
             << "Size must be greater than zero.\n";

	  throw std::logic_error(buffer.str());
   }

   #endif

   const double mean = calculate_mean();

   double sum = 0.0;
   
   for(unsigned int i = 0; i < this_size; i++)
   {
      sum += ((*this)[i] - mean)*((*this)[i] - mean);
   }

   const double standard_deviation = sqrt(sum/(double)this_size);
   
   return(standard_deviation);
}


// Vector<double> calculate_standard_deviation(void) const method

/// This method returns the mean and the standard deviation of the elements in the vector.

Vector<double> calculate_mean_standard_deviation(void) const
{
   const unsigned int this_size = this->size();

   // Control sentence (if debug)

   #ifdef _DEBUG 

   if(this_size == 0)
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: Vector Template.\n" 
             << "double calculate_mean_standard_deviation(void).\n"
             << "Size must be greater than zero.\n";

	  throw std::logic_error(buffer.str());
   }

   #endif

   const double mean = calculate_mean();

   double sum = 0.0;
   
   for(unsigned int i = 0; i < this_size; i++)
   {
      sum += ((*this)[i] - mean)*((*this)[i] - mean);
   }

   const double standard_deviation = sqrt(sum/(double)this_size);

   Vector<double> mean_standard_deviation(2);
   mean_standard_deviation[0] = mean;
   mean_standard_deviation[1] = standard_deviation;

   return(mean_standard_deviation);
}


// Vector<double> calculate_statistics(void) const method

/// This method returns the minimum, maximum, mean and the standard deviation of the elements in the vector.

Vector<double> calculate_statistics(void) const
{
   unsigned int this_size = this->size();

   // Control sentence (if debug)

   #ifdef _DEBUG 

   if(this_size == 0)
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: Vector Template.\n" 
             << "double calculate_statistics(void).\n"
             << "Size must be greater than zero.\n";

	  throw std::logic_error(buffer.str());
   }

   #endif

   return(calculate_minimum_maximum().get_assembly(calculate_mean_standard_deviation()));
}


// double calculate_norm(void) const method

/// This element returns the vector norm.

double calculate_norm(void) const
{
   const unsigned int this_size = this->size();

   // Control sentence (if debug)

   double norm = 0.0;

   for(unsigned int i = 0; i < this_size; i++)
   {
      norm += (*this)[i]*(*this)[i];
   }
   
   norm = sqrt(norm);
   
   return(norm);
}


// double calculate_distance(const Vector<double>&) const method

/// This method returns the distance between the elements of this vector and the elements of another vector. 
/// @param other_vector Other vector. 

double calculate_distance(const Vector<double>& other_vector) const
{
   const unsigned int this_size = this->size();

   // Control sentence (if debug)

   #ifdef _DEBUG 

   unsigned int other_size = other_vector.size();

   if(other_size != this_size)
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: Vector Template.\n" 
             << "double calculate_distance(const Vector<double>&) const method.\n"
             << "Size must be equal to this size.\n";

	  throw std::logic_error(buffer.str());
   }

   #endif

   double sum_squared_error = 0.0;

   for(unsigned int i = 0; i < this_size; i++)
   {
      sum_squared_error += ((*this)[i] - other_vector[i])*((*this)[i] - other_vector[i]);
   }

   return(sqrt(sum_squared_error));
}


// double calculate_sum_squared_error(const Vector<double>&) const method

/// This method returns the sum squared error between the elements of this vector and the elements of another vector. 
/// @param other_vector Other vector. 

double calculate_sum_squared_error(const Vector<double>& other_vector) const
{
   const unsigned int this_size = this->size();

   // Control sentence (if debug)

   #ifdef _DEBUG 

   unsigned int other_size = other_vector.size();

   if(other_size != this_size)
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: Vector Template.\n" 
             << "double calculate_sum_squared_error(const Vector<double>&) const method.\n"
             << "Size must be equal to this size.\n";

	  throw std::logic_error(buffer.str());
   }

   #endif

   double sum_squared_error = 0.0;

   for(unsigned int i = 0; i < this_size; i++)
   {
      sum_squared_error += ((*this)[i] - other_vector[i])*((*this)[i] - other_vector[i]);
   }

   return(sum_squared_error);
}


// double calculate_Minkowski_error(const Vector<double>&) const method

/// This method returns the Minkowski squared error between the elements of this vector and the elements of another vector. 
/// @param other_vector Other vector. 
/// @param Minkowski_parameter Minkowski exponent. 

double calculate_Minkowski_error(const Vector<double>& other_vector, double Minkowski_parameter) const
{
   const unsigned int this_size = this->size();

   // Control sentence (if debug)

   #ifdef _DEBUG 

   std::ostringstream buffer;

   if(this_size == 0)
   {
      buffer << "OpenNN Exception: Vector Template.\n" 
             << "double calculate_Minkowski_error(const Vector<double>&) const method.\n"
             << "Size must be greater than zero.\n";

	  throw std::logic_error(buffer.str());
   }

   const unsigned int other_size = other_vector.size();

   if(other_size != this_size)
   {
      buffer << "OpenNN Exception: Vector Template.\n" 
             << "double calculate_Minkowski_error(const Vector<double>&) const method.\n"
             << "Other size must be equal to this size.\n";

	  throw std::logic_error(buffer.str());
   }

   // Control sentence

   if(Minkowski_parameter < 1.0 || Minkowski_parameter > 2.0)
   {
      buffer << "OpenNN Exception: Vector Template.\n" 
             << "double calculate_Minkowski_error(const Vector<double>&) const method.\n"
             << "The Minkowski parameter must be comprised between 1 and 2\n";
    
      throw std::logic_error(buffer.str().c_str());
   }

   #endif

   double Minkowski_error = 0.0;

   for(unsigned int i = 0; i < this_size; i++)
   {
      Minkowski_error += pow((*this)[i] - other_vector[i], Minkowski_parameter);
   }

   return(Minkowski_error);
}


// void calculate_absolute_value(void) const method

/// This method returns a vector with the absolute values of the current vector. 

Vector<Type> calculate_absolute_value(void) const
{
   const unsigned int this_size = this->size();

   Vector<Type> absolute_value(this_size);

   for(unsigned int i = 0; i < this_size; i++)
   {
      if((*this)[i] > 0)
	  {
	     absolute_value[i] = (*this)[i];
	  }
	  else
      {
         absolute_value[i] = -(*this)[i];
      }
   }

   return(absolute_value);
}


// void apply_absolute_value(void) method

/// This method sets the elements of the vector to their absolute values.

void apply_absolute_value(void)
{
   const unsigned int this_size = this->size();

   for(unsigned int i = 0; i < this_size; i++)
   {
      if((*this)[i] < 0)
      {
         (*this)[i] = -(*this)[i];
      }
   }
}


// Vector<Type> calculate_lower_bounded(const Type&) const method

/// This method returns a vector with the bounded elements from below of the current vector.
/// @param lower_bound Lower bound values.

Vector<Type> calculate_lower_bounded(const Type& lower_bound) const
{
   const unsigned int this_size = this->size();

   Vector<Type> bounded_vector(this_size);

   for(unsigned int i = 0; i < this_size; i++)
   {
      if((*this)[i] < lower_bound)
      {
         bounded_vector[i] = lower_bound;
      }
	  else
      {
         bounded_vector[i] = (*this)[i];
      }
   }

   return(bounded_vector);
}


// Vector<Type> calculate_lower_bounded(const Vector<Type>&) const method

/// This method returns a vector with the bounded elements from above of the current vector.
/// @param lower_bound Lower bound values.

Vector<Type> calculate_lower_bounded(const Vector<Type>& lower_bound) const
{
   const unsigned int this_size = this->size();

   // Control sentence (if debug)

   #ifdef _DEBUG 
 
   const unsigned int lower_bound_size = lower_bound.size();

   if(lower_bound_size != this_size)
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: Vector Template.\n" 
             << "Vector<Type> calculate_lower_bounded(const Vector<Type>&) const method.\n"
             << "Lower bound size must be equal to vector size.\n";

	  throw std::logic_error(buffer.str());
   }

   #endif

   Vector<Type> bounded_vector(this_size);

   // Apply lower bound

   for(unsigned int i = 0; i < this_size; i++)
   {
      if((*this)[i] < lower_bound[i])
      {
         bounded_vector[i] = lower_bound[i];
      }
      else
      {
         bounded_vector[i] = (*this)[i];
      }
   }

   return(bounded_vector);
}


// Vector<Type> calculate_upper_bounded(const Type&) const method

/// This method bounds the elements of the vector if they fall above an upper bound value.
/// @param upper_bound Upper bound value.

Vector<Type> calculate_upper_bounded(const Type& upper_bound) const
{
   const unsigned int this_size = this->size();

   Vector<Type> bounded_vector(this_size);

   for(unsigned int i = 0; i < this_size; i++)
   {
      if((*this)[i] > upper_bound)
      {
         bounded_vector[i] = upper_bound;
      }
      else
      {
         bounded_vector[i] = (*this)[i];
      }
   }

   return(bounded_vector);
}


// Vector<Type> calculate_upper_bounded(const Vector<Type>&) const method

/// This method bounds the elements of the vector if they fall above their corresponding upper bound values.
/// @param upper_bound Upper bound values.

Vector<Type> calculate_upper_bounded(const Vector<Type>& upper_bound) const
{
   const unsigned int this_size = this->size();

   // Control sentence (if debug)

   #ifdef _DEBUG 
 
   unsigned int upper_bound_size = upper_bound.size();

   if(upper_bound_size != this_size)
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: Vector Template.\n" 
             << "Vector<Type> calculate_upper_bounded(const Vector<Type>&) const method.\n"
             << "Upper bound size must be equal to vector size.\n";

	  throw std::logic_error(buffer.str());
   }

   #endif

   Vector<Type> bounded_vector(this_size);

   // Apply upper bound

   for(unsigned int i = 0; i < this_size; i++)
   {
      if((*this)[i] > upper_bound[i])
      {
         bounded_vector[i] = upper_bound[i];
      }
      else
      {
         bounded_vector[i] = (*this)[i];
      }
   }

   return(bounded_vector);
}


// Vector<Type> calculate_lower_upper_bounded(const Type&, const Type&) const method

/// This method bounds the elements of the vector if they fall above or below their lower or upper
/// bound values, respectively.
/// @param lower_bound Lower bound value.
/// @param upper_bound Upper bound value.

Vector<Type> calculate_lower_upper_bounded(const Type& lower_bound, const Type& upper_bound) const
{
   const unsigned int this_size = this->size();

   Vector<Type> bounded_vector(this_size);

   for(unsigned int i = 0; i < this_size; i++)
   {
      if((*this)[i] < lower_bound)
      {
         bounded_vector[i] = lower_bound;
      }
      else if((*this)[i] > upper_bound)
      {
         bounded_vector[i] = upper_bound;
      }
      else
      {
         bounded_vector[i] = (*this)[i];
      }
   }

   return(bounded_vector);
}


// Vector<Type> calculate_lower_upper_bounded(const Vector<Type>&, const Vector<Type>&) const method

/// This method bounds the elements of the vector if they fall above or below their corresponding lower or upper
/// bound values, respectively.
/// @param lower_bound Lower bound values.
/// @param upper_bound Upper bound values.

Vector<Type> calculate_lower_upper_bounded(const Vector<Type>& lower_bound, const Vector<Type>& upper_bound) const
{
   const unsigned int this_size = this->size();

   // Control sentence (if debug)

   #ifdef _DEBUG 
 
   unsigned int lower_bound_size = lower_bound.size();
   unsigned int upper_bound_size = upper_bound.size();

   if(lower_bound_size != this_size || upper_bound_size != this_size)
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: Vector Template.\n" 
             << "Vector<Type> calculate_lower_upper_bounded(const Vector<Type>&, const Vector<Type>&) const method.\n"
             << "Lower and upper bound sizes must be equal to vector size.\n";

	  throw std::logic_error(buffer.str());
   }

   #endif

   Vector<Type> bounded_vector(this_size);

   // Apply lower and upper bounds

   for(unsigned int i = 0; i < this_size; i++)
   {
      if((*this)[i] < lower_bound[i])
      {
         bounded_vector[i] = lower_bound[i];
      }
      else if((*this)[i] > upper_bound[i])
      {
         bounded_vector[i] = upper_bound[i];
      }
      else
      {
         bounded_vector[i] = (*this)[i];
      }
   }

   return(bounded_vector);
}


// void apply_lower_bound(const Type&) method

/// This method sets the elements of the vector to a given value if they fall below that value.
/// @param lower_bound Lower bound value.

void apply_lower_bound(const Type& lower_bound) 
{       
   const unsigned int this_size = this->size();

   for(unsigned int i = 0; i < this_size; i++)
   {
      if((*this)[i] < lower_bound)
      {
         (*this)[i] = lower_bound;
      }
   }
}


// void apply_lower_bound(const Vector<Type>&) method

/// This method sets the elements of the vector to given values if they fall below that values.
/// @param lower_bound Lower bound values.

void apply_lower_bound(const Vector<Type>& lower_bound)
{
   const unsigned int this_size = this->size();

   for(unsigned int i = 0; i < this_size; i++)
   {
      if((*this)[i] < lower_bound[i])
      {
         (*this)[i] = lower_bound[i];
      }
   }
}


// void apply_upper_bound(const Type&) method

/// This method sets the elements of the vector to a given value if they fall above that value.
/// @param upper_bound Upper bound value.

void apply_upper_bound(const Type& upper_bound)
{
   const unsigned int this_size = this->size();

   for(unsigned int i = 0; i < this_size; i++)
   {
      if((*this)[i] > upper_bound)
      {
         (*this)[i] = upper_bound;
      }
   }
}


// void apply_upper_bound(const Vector<Type>&) method

/// This method sets the elements of the vector to given values if they fall above that values.
/// @param upper_bound Upper bound values.

void apply_upper_bound(const Vector<Type>& upper_bound)
{
   const unsigned int this_size = this->size();

   for(unsigned int i = 0; i < this_size; i++)
   {
      if((*this)[i] > upper_bound[i])
      {
         (*this)[i] = upper_bound[i];
      }
   }
}


// void apply_lower_upper_bounds(const Type&, const Type&) method

/// This method sets the elements of the vector to a given lower bound value if they fall below that value, 
/// or to a given upper bound value if they fall above that value.
/// @param lower_bound Lower bound value.
/// @param upper_bound Upper bound value.

void apply_lower_upper_bounds(const Type& lower_bound, const Type& upper_bound)
{
   const unsigned int this_size = this->size();

   for(unsigned int i = 0; i < this_size; i++)
   {
      if((*this)[i] < lower_bound)
      {
         (*this)[i] = lower_bound;
      }
      else if((*this)[i] > upper_bound)
      {
         (*this)[i] = upper_bound;
      }
   }
}


// void apply_lower_upper_bounds(const Vector<Type>&, const Vector<Type>&) method

/// This method sets the elements of the vector to given lower bound values if they fall below that values, 
/// or to given upper bound values if they fall above that values.
/// @param lower_bound Lower bound values.
/// @param upper_bound Upper bound values.

void apply_lower_upper_bounds(const Vector<Type>& lower_bound, const Vector<Type>& upper_bound)
{
   const unsigned int this_size = this->size();

   for(unsigned int i = 0; i < this_size; i++)
   {
      if((*this)[i] < lower_bound[i])
      {
         (*this)[i] = lower_bound[i];
      }
      else if((*this)[i] > upper_bound[i])
      {
         (*this)[i] = upper_bound[i];
      }
   }
}


// Vector<Type> operator + (const Type&) const method 

/// Sum vector+scalar arithmetic operator. 
/// @param scalar Scalar value to be added to this vector.

inline Vector<Type> operator + (const Type& scalar) const
{       
   const unsigned int this_size = this->size();

   Vector<Type> sum(this_size);

   for(unsigned int i = 0; i < this_size; i++)
   {
      sum[i] = (*this)[i] + scalar;
   }
   
   return(sum);
}


// Vector<Type> operator + (const Vector<Type>&) const method

/// Sum vector+vector arithmetic operator. 
/// @param other_vector Vector to be added to this vector.

inline Vector<Type> operator + (const Vector<Type>& other_vector) const
{       
   const unsigned int this_size = this->size();

   // Control sentence (if debug)

   #ifdef _DEBUG 

   unsigned int other_size = other_vector.size();

   if(other_size != this_size)
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: Vector Template.\n"
             << "Vector<Type> operator + (const Vector<Type>) const.\n"
             << "Size of vectors is " << this_size << " and " << other_size << " and they must be the same.\n";

	  throw std::logic_error(buffer.str());
   }

   #endif

   Vector<Type> sum(this_size);
  
   for(unsigned int i = 0; i < this_size; i++)
   {
      sum[i] = (*this)[i] + other_vector[i];
   }
   
   return(sum);
}


//Vector<Type> operator - (const Type&) const method 

/// Difference vector-scalar arithmetic operator.
/// @param scalar Scalar value to be subtracted to this vector.

inline Vector<Type> operator - (const Type& scalar) const
{       
   const unsigned int this_size = this->size();

   Vector<Type> difference(this_size);

   for(unsigned int i = 0; i < this_size; i++)
   {
      difference[i] = (*this)[i] - scalar;
   }
   
   return(difference);
}


// Vector<Type> operator - (const Vector<Type>&) const method

/// Difference vector-vector arithmetic operator.
/// @param other_vector vector to be subtracted to this vector.

inline Vector<Type> operator - (const Vector<Type>& other_vector) const
{
   const unsigned int this_size = this->size();
       
   // Control sentence (if debug)      
       
   #ifdef _DEBUG 

   const unsigned int other_size = other_vector.size();

   if(other_size != this_size)
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: Vector Template.\n" 
             << "Vector<Type> operator - (const Vector<Type>&) const.\n"
             << "Size of vectors is " << this_size << " and " << other_size << " and they must be the same.\n";

	  throw std::logic_error(buffer.str());
   }
      
   #endif

   Vector<Type> difference(this_size);
  
   for(unsigned int i = 0; i < this_size; i++)
   {
      difference[i] = (*this)[i] - other_vector[i];
   }
   
   return(difference);
}


// Vector<Type> operator * (const Type&) const method 

/// Product vector*scalar arithmetic operator.
/// @param scalar Scalar value to be multiplied to this vector.

inline Vector<Type> operator * (const Type& scalar) const
{       
   const unsigned int this_size = this->size();

   Vector<Type> product(this_size);

   for(unsigned int i = 0; i < this_size; i++)
   {
      product[i] = (*this)[i]*scalar;
   }
   
   return(product);
}


// Type operator * (const Vector<Type>&) const method

/// Element by element product vector*vector arithmetic operator.
/// @param other_vector vector to be multiplied to this vector.

inline Vector<Type> operator * (const Vector<Type>& other_vector) const
{  
   const unsigned int this_size = this->size();
  
   // Control sentence (if debug)      
       
   #ifdef _DEBUG 

   const unsigned int other_size = other_vector.size();

   if(other_size != this_size)
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: Vector Template.\n" 
             << "Vector<Type> operator * (const Vector<Type>&) const.\n"
             << "Size of other vector (" << other_size << ") must be equal to size of this vector (" << this_size << ").\n";

	  throw std::logic_error(buffer.str());
   }

   #endif

   Vector<Type> product(this_size);
  
   for(unsigned int i = 0; i < this_size; i++)
   {
      product[i] = (*this)[i]*other_vector[i];
   }
   
   return(product);
}


// Matrix<Type> operator * (const Matrix<Type>&) const method

/// Element by row product vector*matrix arithmetic operator.
/// @param matrix matrix to be multiplied to this vector.

inline Matrix<Type> operator * (const Matrix<Type>& matrix) const
{       
   const unsigned int rows_number = matrix.get_rows_number();
   const unsigned int columns_number = matrix.get_columns_number();

   // Control sentence (if debug)      
       
   #ifdef _DEBUG 

   const unsigned int this_size = this->size();

   if(rows_number != this_size)
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: Vector Template.\n" 
             << "Vector<Type> operator * (const Matrix<Type>&) const.\n"
             << "Number of matrix rows (" << rows_number << ") must be equal to vector size (" << this_size << ").\n";

	  throw std::logic_error(buffer.str());
   }

   #endif

   Matrix<Type> product(rows_number, columns_number);
  
   for(unsigned int i = 0; i < rows_number; i++)
   {
      for(unsigned int j = 0; j < columns_number; j++)
	  {
         product[i][j] = (*this)[i]*matrix[i][j];
	  }
   }
   
   return(product);
}


// Vector<Type> dot(const Matrix<Type>&) const method

/// This method returns the dot product of this vector with a matrix.
/// The number of rows of the matrix must be equal to the size of the vector.
/// @param matrix matrix to be multiplied to this vector.

Vector<Type> dot(const Matrix<Type>& matrix) const
{
   const unsigned int rows_number = matrix.get_rows_number();

   // Control sentence (if debug)      

   #ifdef _DEBUG 

   const unsigned int this_size = this->size();

   if(rows_number != this_size)
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: Vector Template.\n" 
             << "Vector<Type> dot(const Matrix<Type>&) const method.\n"
             << "Matrix number of rows must be equal to vector size.\n";

	  throw std::logic_error(buffer.str());
   }

   #endif

   const unsigned int columns_number = matrix.get_columns_number();

   Vector<Type> product(columns_number);
  
   for(unsigned int j = 0; j < columns_number; j++)
   {     
      product[j] = 0;      

      for(unsigned int i = 0; i < rows_number; i++)
      {
         product[j] += (*this)[i]*matrix[i][j];
      }
   }
    
   return(product);
}


// Vector<Type> dot(const Vector<Type>&) const method

/// Dot product vector*vector arithmetic operator.
/// @param other_vector vector to be multiplied to this vector.

Type dot(const Vector<Type>& other_vector) const
{
   const unsigned int this_size = this->size();
            
   // Control sentence (if debug)      

   #ifdef _DEBUG 

   const unsigned int other_size = other_vector.size();

   if(other_size != this_size)
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: Vector Template.\n" 
             << "Type dot(const Vector<Type>&) const method.\n"
             << "Both vector sizes must be the same.\n";

	  throw std::logic_error(buffer.str());
   }

   #endif

   Type dot_product = 0;
  
   for(unsigned int i = 0; i < this_size; i++)
   {
      dot_product += (*this)[i]*other_vector[i];
   }
   
   return(dot_product);
}


// Matrix<Type> direct(const Vector<Type>&) const method

/// Outer product vector*vector arithmetic operator.
/// @param other_vector vector to be multiplied to this vector.

Matrix<Type> direct(const Vector<Type>& other_vector) const
{  
   const unsigned int this_size = this->size();
          
   // Control sentence (if debug)      

   #ifdef _DEBUG 

   const unsigned int other_size = other_vector.size();

   if(other_size != this_size)
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: Vector Template.\n" 
             << "Matrix<Type> direct(const Vector<Type>&) const method.\n"
             << "Both vector sizes must be the same.\n";

	  throw std::logic_error(buffer.str());
   }

   #endif
 
   Matrix<Type> direct(this_size, this_size);

   // Upper diagonal

   for(unsigned int i = 0;  i < this_size; i++)
   {
      for(unsigned int j = i;  j < this_size; j++)
      {
         direct[i][j] = (*this)[i]*other_vector[j];
      }           
   }
   
   // Symmetry

   for(unsigned int i = 0; i < this_size; i++)
   {
      for(unsigned int j = 0; j < i; j++)
      {
         direct[i][j] = direct[j][i];
      }
   }

   return(direct);
}


//Vector<Type> operator / (const Type&) const method 

/// Cocient vector/scalar arithmetic operator.
/// @param scalar Scalar value to be divided to this vector.

Vector<Type> operator / (const Type& scalar) const
{  
   const unsigned int this_size = this->size();
   
   Vector<Type> cocient(this_size);

   for(unsigned int i = 0; i < this_size; i++)
   {
      cocient[i] = (*this)[i]/scalar;
   }
   
   return(cocient);
}


// Vector<Type> operator / (const Vector<Type>&) const method

/// Cocient vector/vector arithmetic operator.
/// @param other_vector vector to be divided to this vector.

Vector<Type> operator / (const Vector<Type>& other_vector) const
{  
   const unsigned int this_size = this->size();
     
   // Control sentence (if debug)            

   #ifdef _DEBUG 

   const unsigned int other_size = other_vector.size();

   if(other_size != this_size)
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: Vector Template.\n" 
             << "Vector<Type> operator / (const Vector<Type>&) const.\n"
             << "Both vector sizes must be the same.\n";

	  throw std::logic_error(buffer.str());
   }

   #endif

   Vector<Type> cocient(this_size);
  
   for(unsigned int i = 0; i < this_size; i++)
   {
      cocient[i] = (*this)[i]/other_vector[i];
   }
   
   return(cocient);
}


// void operator += (const Type&)

/// Scalar sum and assignment operator.
/// @param value Scalar value to be added to this vector.

void operator += (const Type& value)
{         
   const unsigned int this_size = this->size();

   for(unsigned int i = 0; i < this_size; i++)
   {
      (*this)[i] += value;
   }
}


// void operator += (const Vector<Type>&)

/// Vector sum and assignment operator.
/// @param other_vector Vector to be added to this vector.

void operator += (const Vector<Type>& other_vector)
{
   const unsigned int this_size = this->size();
      
   // Control sentence (if debug)            

   #ifdef _DEBUG 

   const unsigned int other_size = other_vector.size();

   if(other_size != this_size)
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: Vector Template.\n" 
             << "void operator += (const Vector<Type>&).\n"
             << "Both vector sizes must be the same.\n";

	  throw std::logic_error(buffer.str());
   }

   #endif
  
   for(unsigned int i = 0; i < this_size; i++)
   {
      (*this)[i] += other_vector[i];
   }
}


// void operator -= (const Type&)

/// Scalar rest and assignment operator.
/// @param value Scalar value to be subtracted to this vector.

void operator -= (const Type& value)
{         
   const unsigned int this_size = this->size();

   for(unsigned int i = 0; i < this_size; i++)
   {
      (*this)[i] -= value;
   }
}


// void operator -= (const Vector<Type>&)

/// Vector rest and assignment operator.
/// @param other_vector Vector to be subtracted to this vector.

void operator -= (const Vector<Type>& other_vector)
{       
   const unsigned int this_size = this->size();

   // Control sentence (if debug)            

   #ifdef _DEBUG 

   const unsigned int other_size = other_vector.size();

   if(other_size != this_size)
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: Vector Template.\n" 
             << "void operator -= (const Vector<Type>&).\n"
             << "Both vector sizes must be the same.\n";

	  throw std::logic_error(buffer.str());
   }

   #endif
  
   for(unsigned int i = 0; i < this_size; i++)
   {
      (*this)[i] -= other_vector[i];
   }
}


// void operator *= (const Type&) 

/// Scalar product and assignment operator.
/// @param value Scalar value to be multiplied to this vector.

void operator *= (const Type& value)
{  
   const unsigned int this_size = this->size();
       
   for(unsigned int i = 0; i < this_size; i++)
   {
      (*this)[i] *= value;
   }
}


// void operator *= (const Vector<Type>&)

/// Vector product and assignment operator.
/// @param other_vector Vector to be multiplied to this vector.

void operator *= (const Vector<Type>& other_vector)
{       
   const unsigned int this_size = this->size();

   // Control sentence (if debug)            

   #ifdef _DEBUG 

   unsigned int other_size = other_vector.size();

   if(other_size != this_size)
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: Vector Template.\n" 
             << "void operator *= (const Vector<Type>&).\n"
             << "Both vector sizes must be the same.\n";

	  throw std::logic_error(buffer.str());
   }

   #endif
  
   for(unsigned int i = 0; i < this_size; i++)
   {
      (*this)[i] *= other_vector[i];
   }
}


// void operator /= (const Type&)

/// Scalar division and assignment operator.
/// @param value Scalar value to be divided to this vector.

void operator /= (const Type& value)
{         
   const unsigned int this_size = this->size();

   for(unsigned int i = 0; i < this_size; i++)
   {
      (*this)[i] /= value;
   }
}


// void operator /= (const Vector<Type>&)

/// Vector division and assignment operator.
/// @param other_vector Vector to be divided to this vector.

void operator /= (const Vector<Type>& other_vector)
{       
   const unsigned int this_size = this->size();

   // Control sentence (if debug)            

   #ifdef _DEBUG 

   unsigned int other_size = other_vector.size();

   if(other_size != this_size)
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: Vector Template.\n" 
             << "void operator /= (const Vector<Type>&).\n"
             << "Both vector sizes must be the same.\n";

	  throw std::logic_error(buffer.str());
   }

   #endif
  
   for(unsigned int i = 0; i < this_size; i++)
   {
      (*this)[i] /= other_vector[i];
   }
}


// void scale_mean_standard_deviation(const Vector<double>&, const Vector<double>&) method

/// This method scales the vector elements with given mean and standard deviation values. 
/// It updates the data in the vector.
/// The size of the mean and standard deviation vectors must be equal to the size of the vector. 
/// @param mean Mean values.
/// @param standard_deviation Standard deviation values.

void scale_mean_standard_deviation(const Vector<Type>& mean, const Vector<Type>& standard_deviation)
{
   const unsigned int this_size = this->size();

   #ifdef _DEBUG 

   const unsigned int mean_size = mean.size();

   if(mean_size != this_size)
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: Vector template." 
             << "void scale_mean_standard_deviation(const Vector<Type>&, const Vector<Type>&) method.\n" 
             << "Size of mean vector must be equal to size.\n";

	  throw std::logic_error(buffer.str());
   }

   unsigned int standard_deviation_size = standard_deviation.size();

   if(standard_deviation_size != this_size)
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: Vector template." 
             << "void scale_mean_standard_deviation(const Vector<Type>&, const Vector<Type>&) method.\n" 
             << "Size of standard deviation vector must be equal to size.\n";

	  throw std::logic_error(buffer.str());
   }   

   #endif

   // Rescale data

   for(unsigned int i = 0; i < this_size; i++)
   {
      if(standard_deviation[i] < 1e-99)
      {
         std::cout << "OpenNN Warning: Vector class.\n"
                   << "void scale_mean_standard_deviation(const Vector<Type>&, const Vector<Type>&) method.\n"
                   << "Standard deviation of variable " << i << " is zero.\n"
                   << "Those elements won't be scaled.\n";

         // Do nothing
      }
      else
      {
         (*this)[i] = ((*this)[i] - mean[i])/standard_deviation[i];
      }
   }
}



// Vector<Type> calculate_scaled_minimum_maximum(const Vector<Type>&, const Vector<Type>&) const method

/// This method returns a vector with the scaled elements of this vector acording to the minimum and maximum method. 
/// The size of the minimum and maximum vectors must be equal to the size of the vector. 
/// @param minimum Minimum values.
/// @param maximum Maximum values.

Vector<Type> calculate_scaled_minimum_maximum(const Vector<Type>& minimum, const Vector<Type>& maximum) const
{
   const unsigned int this_size = this->size();

   #ifdef _DEBUG 

   const unsigned int minimum_size = minimum.size();

   if(minimum_size != this_size)
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: Vector template." 
             << "Vector<Type> calculate_scaled_minimum_maximum(const Vector<Type>&, const Vector<Type>&) const method.\n" 
             << "Size of minimum vector must be equal to size.\n";

	  throw std::logic_error(buffer.str());
   }

   const unsigned int maximum_size = maximum.size();

   if(maximum_size != this_size)
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: Vector template." 
             << "Vector<Type> calculate_scaled_minimum_maximum(const Vector<Type>&, const Vector<Type>&) const method.\n" 
             << "Size of maximum vector must be equal to size.\n";

	  throw std::logic_error(buffer.str());
   }   

   #endif

   Vector<Type> scaled_minimum_maximum(this_size);

   // Rescale data

   for(unsigned int i = 0; i < this_size; i++)
   {
      if(maximum[i] - minimum[i] < 1e-99)
      {
         std::cout << "OpenNN Warning: Vector class.\n"
                   << "Vector<Type> calculate_scaled_minimum_maximum(const Vector<Type>&, const Vector<Type>&) const method.\n"
                   << "Minimum and maximum values of variable " << i << " are equal.\n"
                   << "Those elements won't be scaled.\n";

		 scaled_minimum_maximum[i] = (*this)[i];
      }
      else
      {
         scaled_minimum_maximum[i] = 2.0*((*this)[i] - minimum[i])/(maximum[i]-minimum[i])-1.0;
      }
   }

   return(scaled_minimum_maximum);
}


// Vector<Type> calculate_scaled_mean_standard_deviation(const Vector<Type>&, const Vector<Type>&) const method

/// This method returns a vector with the scaled elements of this vector acording to the mean and standard deviation method. 
/// The size of the mean and standard deviation vectors must be equal to the size of the vector. 
/// @param mean Mean values.
/// @param standard_deviation Standard deviation values.

Vector<Type> calculate_scaled_mean_standard_deviation(const Vector<Type>& mean, const Vector<Type>& standard_deviation) const
{
   const unsigned int this_size = this->size();

   #ifdef _DEBUG 

   std::ostringstream buffer;

   const unsigned int mean_size = mean.size();

   if(mean_size != this_size)
   {
      buffer << "OpenNN Exception: Vector template." 
             << "Vector<Type> calculate_scaled_mean_standard_deviation(const Vector<Type>&, const Vector<Type>&) const method.\n" 
             << "Size of mean vector must be equal to size.\n";

	  throw std::logic_error(buffer.str());
   }

   const unsigned int standard_deviation_size = standard_deviation.size();

   if(standard_deviation_size != this_size)
   {
      buffer << "OpenNN Exception: Vector template.\n"
             << "Vector<Type> calculate_scaled_mean_standard_deviation(const Vector<Type>&, const Vector<Type>&) const method.\n" 
             << "Size of standard deviation vector must be equal to size.\n";

	  throw std::logic_error(buffer.str());
   }   

   #endif

   Vector<Type> scaled_mean_standard_deviation(this_size);

   for(unsigned int i = 0; i < this_size; i++)
   {
      if(standard_deviation[i] < 1e-99)
      {
         std::cout << "OpenNN Warning: Vector template.\n"
                   << "Vector<Type> calculate_scaled_mean_standard_deviation(const Vector<Type>&, const Vector<Type>&) const method.\n" 
                   << "Standard deviation of variable " << i << " is zero.\n"
                   << "Those elements won't be scaled.\n";

         scaled_mean_standard_deviation = (*this)[i];
      }
      else
      {
         scaled_mean_standard_deviation[i] = (*this)[i]*standard_deviation[i] + mean[i];
      }
   }

   return(scaled_mean_standard_deviation);
}


// Vector<Type> calculate_unscaled_minimum_maximum(const Vector<Type>&, const Vector<Type>&) const method

/// This method returns a vector with the unscaled elements of this vector acording to the minimum and maximum method. 
/// The size of the minimum and maximum vectors must be equal to the size of the vector. 
/// @param minimum Minimum values.
/// @param maximum Maximum values.

Vector<Type> calculate_unscaled_minimum_maximum(const Vector<Type>& minimum, const Vector<Type>& maximum) const 
{
   const unsigned int this_size = this->size();

   #ifdef _DEBUG 

   const unsigned int minimum_size = minimum.size();

   if(minimum_size != this_size)
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: Vector template." 
             << "Vector<Type> calculate_unscaled_minimum_maximum(const Vector<Type>&, const Vector<Type>&) const method.\n" 
             << "Size of minimum vector must be equal to size.\n";

	  throw std::logic_error(buffer.str());
   }

   const unsigned int maximum_size = maximum.size();

   if(maximum_size != this_size)
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: Vector template." 
             << "Vector<Type> calculate_unscaled_minimum_maximum(const Vector<Type>&, const Vector<Type>&) const method.\n" 
             << "Size of maximum vector must be equal to size.\n";

	  throw std::logic_error(buffer.str());
   }   

   #endif

   Vector<Type> unscaled_minimum_maximum(this_size);

   for(unsigned int i = 0; i < this_size; i++)
   {
      if(maximum[i] - minimum[i] < 1e-99)
      {
         std::cout << "OpenNN Warning: Vector template.\n"
                   << "Vector<Type> calculate_unscaled_minimum_maximum(const Vector<Type>&, const Vector<Type>&) const method.\n" 
                   << "Minimum and maximum values of variable " << i << " are equal.\n"
                   << "Those elements won't be unscaled.\n";

         unscaled_minimum_maximum[i] = (*this)[i];
	  }
      else
      {
         unscaled_minimum_maximum[i] = 0.5*((*this)[i] + 1.0)*(maximum[i]-minimum[i]) + minimum[i]; 
      }
   }

   return(unscaled_minimum_maximum);
}


// Vector<Type> calculate_unscaled_mean_standard_deviation(const Vector<Type>&, const Vector<Type>&) const method

/// This method returns a vector with the unscaled elements of this vector acording to the mean and standard deviation method. 
/// The size of the mean and standard deviation vectors must be equal to the size of the vector. 
/// @param mean Mean values.
/// @param standard_deviation Standard deviation values.

Vector<Type> calculate_unscaled_mean_standard_deviation(const Vector<Type>& mean, const Vector<Type>& standard_deviation) const
{
   const unsigned int this_size = this->size();

   #ifdef _DEBUG 

   const unsigned int mean_size = mean.size();

   if(mean_size != this_size)
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: Vector template." 
             << "Vector<Type> calculate_unscaled_mean_standard_deviation(const Vector<Type>&, const Vector<Type>&) const method.\n" 
             << "Size of mean vector must be equal to size.\n";

	  throw std::logic_error(buffer.str());
   }

   const unsigned int standard_deviation_size = standard_deviation.size();

   if(standard_deviation_size != this_size)
   {
      std::ostringstream buffer;

	  buffer << "OpenNN Exception: Vector template.\n"
             << "Vector<Type> calculate_unscaled_mean_standard_deviation(const Vector<Type>&, const Vector<Type>&) const method.\n" 
             << "Size of standard deviation vector must be equal to size.\n";

      throw std::logic_error(buffer.str());
   }   

   #endif

   Vector<Type> unscaled_mean_standard_deviation(this_size);

   for(unsigned int i = 0; i < this_size; i++)
   {
      if(standard_deviation[i] < 1e-99)
      {
         std::cout << "OpenNN Warning: Vector template.\n"
                   << "Vector<Type> calculate_unscaled_mean_standard_deviation(const Vector<Type>&, const Vector<Type>&) const method.\n" 
                   << "Standard deviation of variable " << i << " is zero.\n"
                   << "Those elements won't be scaled.\n";

         unscaled_mean_standard_deviation[i] = (*this)[i];
      }
      else
      {
         unscaled_mean_standard_deviation[i] = (*this)[i]*standard_deviation[i] + mean[i];
      }
   }

   return(unscaled_mean_standard_deviation);
}


// void scale_minimum_maximum(const Vector<Type>&, const Vector<Type>&) method

/// This method scales the vectir elements with given minimum and maximum values. 
/// It updates the data in the vector.
/// The size of the minimum and maximum vectors must be equal to the size of the vector. 
/// @param minimum Minimum values.
/// @param maximum Maximum values.

void scale_minimum_maximum(const Vector<Type>& minimum, const Vector<Type>& maximum)
{
   const unsigned int this_size = this->size();

   #ifdef _DEBUG 

   const unsigned int minimum_size = minimum.size();

   if(minimum_size != this_size)
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: Vector template." 
             << "void scale_minimum_maximum(const Vector<Type>&, const Vector<Type>&) method.\n" 
             << "Size of minimum vector must be equal to size.\n";

      throw std::logic_error(buffer.str());
   }

   const unsigned int maximum_size = maximum.size();

   if(maximum_size != this_size)
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: Vector template." 
             << "void scale_minimum_maximum(const Vector<Type>&, const Vector<Type>&) method.\n" 
             << "Size of maximum vector must be equal to size.\n";

      throw std::logic_error(buffer.str());
   }   

   #endif

   // Rescale data

   for(unsigned int i = 0; i < this_size; i++)
   {
      if(maximum[i] - minimum[i] < 1e-99)
      {
         std::cout << "OpenNN Warning: Vector class.\n"
                   << "void scale_minimum_maximum(const Vector<Type>&, const Vector<Type>&) method.\n"
                   << "Minimum and maximum values of variable " << i << " are equal.\n"
                   << "Those elements won't be scaled.\n";

		   // Do nothing
      }
      else
      {
         (*this)[i] = 2.0*((*this)[i] - minimum[i])/(maximum[i]-minimum[i])-1.0;
      }
   }
}


// void unscale_minimum_maximum(const Vector<Type>&, const Vector<Type>&) method

/// This method unscales the vector elements with given minimum and maximum values. 
/// It updates the vector elements.
/// The size of the minimum and maximum vectors must be equal to the size of the vector. 
/// @param minimum Minimum values.
/// @param maximum Maximum deviation values.

void unscale_minimum_maximum(const Vector<Type>& minimum, const Vector<Type>& maximum)
{
   const unsigned int this_size = this->size();

   #ifdef _DEBUG 

   const unsigned int minimum_size = minimum.size();

   if(minimum_size != this_size)
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: Vector template." 
             << "void unscale_minimum_maximum(const Vector<Type>&, const Vector<Type>&) method.\n" 
             << "Size of minimum vector must be equal to size.\n";

      throw std::logic_error(buffer.str());
   }

   const unsigned int maximum_size = maximum.size();

   if(maximum_size != this_size)
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: Vector template." 
             << "void unscale_minimum_maximum(const Vector<Type>&, const Vector<Type>&) method.\n" 
             << "Size of maximum vector must be equal to size.\n";

      throw std::logic_error(buffer.str());
   }   

   #endif

   for(unsigned int i = 0; i < this_size; i++)
   {
      if(maximum[i] - minimum[i] < 1e-99)
      {
         std::cout << "OpenNN Warning: Vector template.\n"
                   << "void unscale_minimum_maximum(const Vector<Type>&, const Vector<Type>&) method.\n" 
                   << "Minimum and maximum values of variable " << i << " are equal.\n"
                   << "Those elements won't be unscaled.\n";

         // Do nothing
      }
      else
      {
         (*this)[i] = 0.5*((*this)[i] + 1.0)*(maximum[i]-minimum[i]) + minimum[i]; 
      }
   }
}


// void unscale_mean_standard_deviation(const Vector<Type>&, const Vector<Type>&) method

/// This method unscales the vector elements with given mean and standard deviation values. 
/// It updates the vector elements.
/// The size of the mean and standard deviation vectors must be equal to the size of the vector. 
/// @param mean Mean values.
/// @param standard_deviation Standard deviation values.

void unscale_mean_standard_deviation(const Vector<Type>& mean, const Vector<Type>& standard_deviation)
{
   const unsigned int this_size = this->size();

   #ifdef _DEBUG 

   const unsigned int mean_size = mean.size();

   if(mean_size != this_size)
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: Vector template." 
             << "void unscale_mean_standard_deviation(const Vector<Type>&, const Vector<Type>&) method.\n" 
             << "Size of mean vector must be equal to size.\n";

      throw std::logic_error(buffer.str());
   }

   const unsigned int standard_deviation_size = standard_deviation.size();

   if(standard_deviation_size != this_size)
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: Vector template.\n"
             << "void unscale_mean_standard_deviation(const Vector<Type>&, const Vector<Type>&) method.\n" 
             << "Size of standard deviation vector must be equal to size.\n";

      throw std::logic_error(buffer.str());
   }   

   #endif

   for(unsigned int i = 0; i < this_size; i++)
   {
      if(standard_deviation[i] < 1e-99)
      {
         std::cout << "OpenNN Warning: Vector template.\n"
                   << "void unscale_mean_standard_deviation(const Vector<Type>&, const Vector<Type>&) method.\n" 
                   << "Standard deviation of variable " << i << " is zero.\n"
                   << "Those elements won't be scaled.\n";

         // Do nothing
      }
      else
      {
         (*this)[i] = (*this)[i]*standard_deviation[i] + mean[i];
      }
   }
}


// Matrix<Type> arrange_diagonal_matrix(void) const method

/// This method returns a squared matrix in which the entries outside the main diagonal are all zero. 
/// The elements in the diagonal are the elements in this vector. 

Matrix<Type> arrange_diagonal_matrix(void) const
{
   const unsigned int this_size = this->size();

   Matrix<Type> matrix(this_size, this_size, 0.0);

   for(unsigned int i = 0; i < this_size; i++)
   {
      matrix[i][i] = (*this)[i];
   }

   return(matrix);
}


// void load(const std::string&) method

/// This method loads the members of a vector from an data file.
/// Please be careful with the file format, which is specified in the OpenNN manual.
/// @param filename Name of vector file.

void load(const std::string& filename) 
{
   std::ifstream file(filename.c_str()); 

   std::stringstream buffer; 

   std::string line;

   while(file.good())
   {
      getline(file, line);

      buffer << line;
   }
     
   std::istream_iterator<std::string> it(buffer); 
   std::istream_iterator<std::string> end; 

   const std::vector<std::string> results(it, end); 

   const unsigned int new_size = results.size();

   this->resize(new_size);

   file.clear();
   file.seekg(0, std::ios::beg); 

   // Read data

   for(unsigned int i = 0; i < new_size; i++)
   {
      file >> (*this)[i];
   }

   file.close();
}


// void save(const std::string&) const method

/// This method saves to a data file the elements of the vector.
/// The file format is as follows:
/// element_0 element_1 ... element_N-1
/// @param filename Name of vector data file.

void save(const std::string& filename) const
{
   std::ofstream file(filename.c_str()); 

   if(!file.is_open())
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: Vector template.\n"
             << "void save(const std::string&) const method.\n"
             << "Cannot open vector data file.\n";

      throw std::logic_error(buffer.str());
   }

   // Write file

   const unsigned int this_size = this->size();

   for(unsigned int i = 0; i < this_size; i++)
   {
      file << (*this)[i] << " ";   
   }

   file << std::endl;

   // Close file

   file.close();
}


// void tuck_in(const unsigned int&, const Vector<Type>&) const method

/// Insert another vector starting from a given position.
/// @param position Insertion position.
/// @param other_vector Vector to be inserted.

void tuck_in(const unsigned int& position, const Vector<Type>& other_vector)
{
   const unsigned int other_size = other_vector.size();

   // Control sentence (if debug)            

   #ifdef _DEBUG 

   const unsigned int this_size = this->size();

   if(position + other_size > this_size)
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: Vector Template.\n" 
             << "void tuck_in(const unsigned int&, const Vector<Type>&) const method.\n"
             << "Cannot tuck in vector.\n";

      throw std::logic_error(buffer.str());
   }

   #endif

   for(unsigned int i = 0; i < other_size; i++)
   {
      (*this)[position + i] = other_vector[i];
   }
}


// Vector<Type> take_out(const unsigned int&, const unsigned int&) method

/// Extract a vector of a given size from a given position
/// @param position Extraction position.
/// @param other_size Size of vector to be extracted.

Vector<Type> take_out(const unsigned int& position, const unsigned int& other_size) const
{
   // Control sentence (if debug)            

   #ifdef _DEBUG 

   const unsigned int this_size = this->size();

   if(position + other_size > this_size)
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: Vector Template.\n" 
             << "Vector<Type> take_out(const unsigned int&, const unsigned int&) method.\n"
             << "Cannot take out vector.\n";

      throw std::logic_error(buffer.str());
   }

   #endif

   Vector<Type> other_vector(other_size);

   for(unsigned int i = 0; i < other_size; i++)
   {
      other_vector[i] = (*this)[position + i];
   }

   return(other_vector);
}


// Vector<Type> get_assembly(const Vector<Type>&) const method

/// Assemble two vectors.
/// @param other_vector Vector to be get_assemblyd to this vector. 

Vector<Type> get_assembly(const Vector<Type>& other_vector) const
{
   const unsigned int this_size = this->size();
   const unsigned int other_size = other_vector.size();

   if(this_size == 0 && other_size == 0)
   {
      Vector<Type> assembly;

      return(assembly);
   }
   else if(this_size == 0)
   {
      return(other_vector);
   }
   else if(other_size == 0)
   {
      return(*this);
   }
   else
   {
      Vector<Type> assembly(this_size + other_size);

      for(unsigned int i = 0; i < this_size; i++)
      {
         assembly[i] = (*this)[i];
      }

      for(unsigned int i = 0; i < other_size; i++)
      {
         assembly[this_size+i] = other_vector[i];
      } 
   
      return(assembly);
   }
}


// void parse(const std::string&) method

/// This method takes a string representation of a vector and sets this vector to have size equal to the number of words and values equal to that words.
/// @param str String to be parsed. 

void parse(const std::string& str)
{
   if(!str.empty())
   {
      std::istringstream buffer(str); 
        
      std::istream_iterator<std::string> first(buffer); 
      std::istream_iterator<std::string> last; 

      Vector<std::string> str_vector(first, last); 

      const  unsigned int new_size = str_vector.size();

      if(new_size > 0)
      {
         this->resize(new_size);

         buffer.clear();
         buffer.seekg(0, std::ios::beg); 

         for(unsigned int i = 0; i < new_size; i++)
         {
            buffer >> (*this)[i];
         }
      }
   }
}


// std::string to_string(void)

/// This method returns a string representation of this vector. 

std::string to_string(void) const
{
   std::ostringstream buffer;
   
   buffer << *this;

   return(buffer.str());
}


// Vector<std::string> get_string_vector(void) const

/// This method retuns a vector of strings with size equal to the size of this vector and elements equal to string representations of the elements of this vector. 

Vector<std::string> get_string_vector(void) const
{
   const unsigned int this_size = this->size();

   Vector<std::string> string_vector(this_size);

   std::ostringstream buffer;

   for(unsigned int i = 0; i < this_size; i++)
   {
      buffer.str("");
      buffer << (*this)[i];
   
      string_vector[i] = buffer.str();
   }

   return(string_vector);
}


// Matrix<Type> to_matrix(const unsigned int&, const unsigned int&) method

/// This method returns a matrix with given numbers of rows and columns and with the elements of this vector ordered by rows. 
/// The number of rows multiplied by the number of columns must be equal to the size of this vector. 
/// @param rows_number Number of rows in the new matrix. 
/// @param columns_number Number of columns in the new matrix. 


Matrix<Type> to_matrix(const unsigned int& rows_number, const unsigned int& columns_number) const
{
   // Control sentence (if debug)            

   #ifdef _DEBUG 

   const unsigned int this_size = this->size();

   if(rows_number*columns_number != this_size)
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: Vector Template.\n" 
             << "Matrix<Type> to_matrix(const unsigned int&, const unsigned int&) method.\n"
             << "The number of rows times the number of colums of the matrix must be equal to the size of the vector.\n";

      throw std::logic_error(buffer.str());
   }

   #endif

   Matrix<Type> matrix(rows_number, columns_number);

   unsigned int index = 0;

   for(unsigned int i = 0; i < rows_number; i++)
   {
      for(unsigned int j = 0; j < rows_number; j++)
      {
         matrix[i][j] = (*this)[index];
         index++;
      }   
   }

   return(matrix);
}


private:

// double calculate_random_uniform(const double&, const double&) method

/// This method returns a random number chosen from a uniform distribution.
/// @param minimum Minimum value.
/// @param maximum Maximum value.

double calculate_random_uniform(const double& minimum, double maximum) const
{
   const double random = (double)rand()/(RAND_MAX+1.0);

   const double random_uniform = minimum + (maximum-minimum)*random;

   return(random_uniform);
}


// double calculate_random_normal(const double&, const double&) method

/// This method returns a random number chosen from a normal distribution.
/// @param mean Mean value of normal distribution.
/// @param standard_deviation Standard deviation value of normal distribution.

double calculate_random_normal(const double& mean, double standard_deviation) const
{
   const double pi = 4.0*atan(1.0);

   double random_uniform_1;

   do
   {
      random_uniform_1 = (double)rand()/(RAND_MAX+1.0);

   }while(random_uniform_1 == 0.0);

   const double random_uniform_2 = (double)rand()/(RAND_MAX+1.0);

   // Box-Muller transformation

   const double random_normal = mean + sqrt(-2.0*log(random_uniform_1))*sin(2.0*pi*random_uniform_2)*standard_deviation;

   return(random_normal);
}

};


// Input operator

/// This method re-writes the inputs operator >> for the Vector template. 
/// @param is Input stream.
/// @param v Input vector.

template<typename Type>
std::istream& operator >> (std::istream& is, Vector<Type>& v)
{
   const unsigned int size = v.size();
   
   for(unsigned int i = 0; i < size; i++)
   {
      is >> v[i];
   }

   return(is);
}


// Output operator

/// This method re-writes the outputs operator << for the Vector template. 
/// @param os Output stream.
/// @param v Output vector.

template<typename Type>
inline std::ostream& operator << (std::ostream& os, const Vector<Type>& v) 
{
   const unsigned int size = v.size();
 
   if(size > 0)
   {
      for(unsigned int i = 0; i < size-1; i++)
      {
         os << v[i] << " ";
      }

      os << v[size-1];
   }  

   return(os);
}


template<typename Type>
inline std::ostream& operator << (std::ostream& os, const Vector< Vector<Type> >& v)
{ 
   for(unsigned int i = 0; i < v.size(); i++)
   {
      os << "Subvector " << i  << "\n"
         << v[i] << std::endl;
   }

   return(os);
}


template<typename Type>
inline std::ostream& operator << (std::ostream& os, const Vector< Matrix<Type> >& v)
{  
   for(unsigned int i = 0; i < v.size(); i++)
   {
      os << "Submatrix" << i  << "\n"
         << v[i];
   }

   return(os);
}

}

#endif


// OpenNN: Open Neural Networks Library.
// Copyright (C) 2005-2012 Roberto Lopez 
//
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 2.1 of the License, or any later version.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.

// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
