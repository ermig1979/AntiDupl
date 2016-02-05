/****************************************************************************************************************/
/*                                                                                                              */
/*   OpenNN: Open Neural Networks Library                                                                       */
/*   www.opennn.cimne.com                                                                                       */
/*                                                                                                              */
/*   U N S C A L I N G   L A Y E R    C L A S S                                                                 */
/*                                                                                                              */
/*   Roberto Lopez                                                                                              */
/*   International Center for Numerical Methods in Engineering (CIMNE)                                          */
/*   Technical University of Catalonia (UPC)                                                                    */
/*   Barcelona, Spain                                                                                           */
/*   E-mail: rlopez@cimne.upc.edu                                                                               */
/*                                                                                                              */
/****************************************************************************************************************/

// System includes

#include <cmath>   
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>

// OpenNN includes

#include "unscaling_layer.h"

namespace OpenNN
{

// DEFAULT CONSTRUCTOR

/// Default constructor. 

UnscalingLayer::UnscalingLayer(void)
{
   set();
}


/// Outputs number constructor. 

UnscalingLayer::UnscalingLayer(const unsigned int& new_unscaling_neurons_number)
{
   set(new_unscaling_neurons_number);
}


/// Outputs statistics constructor. 

UnscalingLayer::UnscalingLayer(const Vector< Vector<double> > & new_statistics)
{
   set(new_statistics);
}


/// XML constructor. 

UnscalingLayer::UnscalingLayer(TiXmlElement* unscaling_layer_element)
{
   set(unscaling_layer_element);
}


/// Copy constructor. 

UnscalingLayer::UnscalingLayer(const UnscalingLayer& other_unscaling_layer)
{
   set(other_unscaling_layer);
}


// DESTRUCTOR

/// Destructor.

UnscalingLayer::~UnscalingLayer(void)
{
}


// ASSIGNMENT OPERATOR

// UnscalingLayer& operator = (const UnscalingLayer&) method

/// Assignment operator.
/// @param other_unscaling_layer Object to be copied. 

UnscalingLayer& UnscalingLayer::operator = (const UnscalingLayer& other_unscaling_layer)
{
   if(this != &other_unscaling_layer) 
   {
      minimums = other_unscaling_layer.minimums;

      maximums = other_unscaling_layer.maximums;

      means = other_unscaling_layer.means;

      standard_deviations = other_unscaling_layer.standard_deviations;

      unscaling_method = other_unscaling_layer.unscaling_method;
      display = other_unscaling_layer.display;
   }

   return(*this);
}


// EQUAL TO OPERATOR

// bool operator == (const UnscalingLayer&) const method

/// Equal to operator. 
/// If compares this object with another object of the same class, and returns true if they are equal, and false otherwise. 
/// @param other_unscaling_layer Object to be compared with. 

bool UnscalingLayer::operator == (const UnscalingLayer& other_unscaling_layer) const
{
   if(minimums == other_unscaling_layer.minimums
   && maximums == other_unscaling_layer.maximums
   && means == other_unscaling_layer.means
   && standard_deviations == other_unscaling_layer.standard_deviations
   && unscaling_method == other_unscaling_layer.unscaling_method
   && display == other_unscaling_layer.display)
   {
      return(true);
   }
   else
   {
      return(false);
   }
}


// unsigned int count_unscaling_neurons_number(void) const method

/// This method returns the number of unscaling neurons in this layer. 

unsigned int UnscalingLayer::count_unscaling_neurons_number(void) const
{
   return(minimums.size());
}


// const Vector<double>& get_minimums(void) const method

/// This method returns the minimum values of all the unscaling neurons.
/// Such values are to be used for unscaling variables with the minimums and maximums method. 

const Vector<double>& UnscalingLayer::get_minimums(void) const
{
   return(minimums);               
}


// double get_minimum(const unsigned int&) const method

/// This method returns the minimum value of a single unscaling neuron.
/// Such value is to be used for unscaling that outputs with the minimums and maximums method.

const double& UnscalingLayer::get_minimum(const unsigned int& i) const
{
   // Control sentence (if debug)

   #ifdef _DEBUG 

   std::ostringstream buffer;

   const unsigned int unscaling_neurons_number = count_unscaling_neurons_number();

   if(unscaling_neurons_number == 0)
   {
      buffer << "OpenNN Exception: UnscalingLayer class.\n" 
             << "const double& get_minimum(const unsigned int&) const method.\n"
             << "Number of outputs is zero.\n";

	  throw std::logic_error(buffer.str());
   }
   else if(i >= unscaling_neurons_number)
   {
      buffer << "OpenNN Exception: UnscalingLayer class.\n" 
             << "double get_minimum(const unsigned int&) const method.\n"
             << "Index must be less than number of unscaling neurons.\n";

	  throw std::logic_error(buffer.str());
   }

   #endif

   return(minimums[i]);
}


// const Vector<double>& get_maximum(void) const method

/// This method returns the maximum values of all the unscaling neurons.
/// Such values are to be used for unscaling variables with the minimums and maximums method. 

const Vector<double>& UnscalingLayer::get_maximums(void) const
{
   return(maximums);               
}


// const double& get_maximum(const unsigned int&) const method

/// This method returns the maximum value of a given unscaling neuron. 
/// Such value is to be used for unscaling that outputs with the minimums and maximums method.

const double& UnscalingLayer::get_maximum(const unsigned int& i) const
{
   // Control sentence (if debug)

   #ifdef _DEBUG 

   std::ostringstream buffer;

   const unsigned int unscaling_neurons_number = count_unscaling_neurons_number();

   if(unscaling_neurons_number == 0)
   {
      buffer << "OpenNN Exception: UnscalingLayer class.\n" 
             << "const double& get_maximum(const unsigned int&) const method.\n"
             << "Number of outputs is zero.\n";

	  throw std::logic_error(buffer.str());
   }
   else if(i >= unscaling_neurons_number)
   {
      buffer << "OpenNN Exception: UnscalingLayer class.\n" 
             << "const double& get_maximum(const unsigned int&) const method.\n"
             << "Index must be less than number of unscaling neurons.\n";

	  throw std::logic_error(buffer.str());
   }

   #endif

   return(maximums[i]);
}


// const Vector<double>& get_mean(void) const

/// This method returns the mean values of all the unscaling neurons.
/// Such values are to be used for unscaling the outputs with the means and standard deviation method. 

const Vector<double>& UnscalingLayer::get_means(void) const
{
   return(means);
}


// const double& get_mean(const unsigned int&) const method

/// This method returns the mean values of a single unscaling neuron.
/// @param index Index of the unscaling neuron.

const double& UnscalingLayer::get_mean(const unsigned int& index) const
{
   // Control sentence (if debug)

   #ifdef _DEBUG 

   const unsigned int unscaling_neurons_number = count_unscaling_neurons_number();

   if(index >= unscaling_neurons_number)
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: UnscalingLayer class.\n" 
             << "const double& get_mean(unsigned int) method const.\n"
             << "Index of outputs must be less than number of unscaling neurons.\n";

	  throw std::logic_error(buffer.str());
   }

   #endif

   return(means[index]);
}


// const Vector<double>& get_standard_deviation(void) const method

/// This method returns the standard deviation values of all the unscaling neurons. 
/// Such values are to be used for unscaling outputs with the mean and standard deviation method. 

const Vector<double>& UnscalingLayer::get_standard_deviations(void) const
{
   return(standard_deviations);
}


// const double& get_standard_deviation(const unsigned int&) const method

/// This method returns the standard deviation value of a single neuron in the unscaling layer.
/// Such a value is to be used for unscaling outputs with the means and standard deviation method. 
/// @param index Index of output variable.

const double& UnscalingLayer::get_standard_deviation(const unsigned int& index) const
{
   // Control sentence (if debug)

   #ifdef _DEBUG 

   const unsigned int unscaling_neurons_number = count_unscaling_neurons_number();

   if(index >= unscaling_neurons_number)
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: UnscalingLayer class.\n" 
             << "double get_standard_deviation(const unsigned int&) const method.\n"
             << "Index of output variable must be less than number opf unscaling neurons.\n";

	  throw std::logic_error(buffer.str());
   }

   #endif

   return(standard_deviations[index]);
}


// Vector< Vector<double>* > get_minimum_maximum(void) method

/// This method returns the minimums and the maximum values of all the output variables. 
/// The format is a vector of pointers to vectors of size two. 
/// The first element contains the minimum values of the output variables.
/// The second element contains the maximum values of the output variables.
/// Such values are to be used for unscaling outputs with the minimums and maximums method. 

Vector< Vector<double>* > UnscalingLayer::get_minimums_maximums(void)
{
   Vector< Vector<double>* > minimums_maximums(2);

   minimums_maximums[0] = &minimums;
   minimums_maximums[1] = &maximums;

   return(minimums_maximums);
}


// Vector< Vector<double>* > get_means_standard_deviations(void) method

/// This method returns the means and the standard deviation values of all the output variables. 
/// The format is a vector of pointers to vectors of size two. 
/// The first element contains the mean values of the output variables.
/// The second element contains the standard deviation values of the output variables.
/// Such values are to be used for unscaling outputs with the means and standard deviation method. 

Vector< Vector<double>* > UnscalingLayer::get_means_standard_deviations(void)
{
   Vector< Vector<double>* > means_standard_deviations(2);

   means_standard_deviations[0] = &means;       
   means_standard_deviations[1] = &standard_deviations;

   return(means_standard_deviations);
}


// Vector< Vector<double>* > get_statistics(void) method

/// This method returns all the available statistics of the inputs and output variables. 
/// The format is a vector of pointers to vectors of size ten: 
/// <ul>
/// <li> Mean of variables.
/// <li> Standard deviation of variables.
/// <li> Minimum of variables.
/// <li> Maximum of variables.
/// </ul>

Vector< Vector<double>* > UnscalingLayer::get_statistics(void)
{
   Vector< Vector<double>* > statistics(4);

   statistics[0] = &minimums;
   statistics[1] = &maximums;
   statistics[2] = &means;
   statistics[3] = &standard_deviations;

   return(statistics);
}


// const UnscalingMethod& get_unscaling_method(void) const method

/// This method returns the method used for unscaling.

const UnscalingLayer::UnscalingMethod& UnscalingLayer::get_unscaling_method(void) const
{
   return(unscaling_method);
}


// std::string write_unscaling_method(void) const method

/// This method returns a string with the name of the method used for unscaling. 

std::string UnscalingLayer::write_unscaling_method(void) const
{
   if(unscaling_method == MeanStandardDeviation)
   {
      return("MeanStandardDeviation");
   }
   else if(unscaling_method == MinimumMaximum)
   {
      return("MinimumMaximum");
   }
   else
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: UnscalingLayer class.\n"
             << "std::string write_unscaling_method(void) const method.\n"
             << "Unknown outputs unscaling method.\n";
 
	  throw std::logic_error(buffer.str());
   }
}


// const bool& get_display(void) const method

/// This method returns true if messages from this class are to be displayed on the screen, or false if messages 
/// from this class are not to be displayed on the screen.

const bool& UnscalingLayer::get_display(void) const
{
   return(display);
}


// void set(void) method

/// This method sets the unscaling layer to be empty. 

void UnscalingLayer::set(void)
{
   minimums.set();
   maximums.set();
   means.set();
   standard_deviations.set();

   set_default();
}


// void set(const unsigned int&) method

/// This method sets a new size in the unscaling layer. 
/// It also sets the members to their default values. 

void UnscalingLayer::set(const unsigned int& new_unscaling_neurons_number)
{
   minimums.set(new_unscaling_neurons_number, -1.0);
   maximums.set(new_unscaling_neurons_number, 1.0);
   means.set(new_unscaling_neurons_number, 0.0);
   standard_deviations.set(new_unscaling_neurons_number, 1.0);

   set_default();
}


// void set(const Vector< Vector<double> >&) method

/// This method sets the size of the unscaling layer and the statistics values. 
/// @param new_statistics Vector of vectors containing the minimums, maximums, means and standard deviations for the unscaling layer. 
/// The size of this vector must be 4. 
/// The size of each subvector will be the size of the unscaling layer. 

void UnscalingLayer::set(const Vector< Vector<double> >& new_statistics)
{
   minimums = new_statistics[0];
   maximums = new_statistics[1];
   means = new_statistics[2];
   standard_deviations = new_statistics[3];

   set_default();
}


// void set(TiXmlElement*) method

/// This method sets the unscaling layer members from a XML element. 
/// @param new_unscaling_layer_element Pointer to a Tiny XML element containing the member data. 

void UnscalingLayer::set(TiXmlElement* new_unscaling_layer_element)
{
   from_XML(new_unscaling_layer_element);
}


// void set(const UnscalingLayer&) method

/// This method sets the members of this object to be the members of another object of the same class. 

void UnscalingLayer::set(const UnscalingLayer& new_unscaling_layer)
{
   minimums = new_unscaling_layer.minimums;
   maximums = new_unscaling_layer.maximums;
   means = new_unscaling_layer.means;
   standard_deviations = new_unscaling_layer.standard_deviations;

   unscaling_method = new_unscaling_layer.unscaling_method;

   display = new_unscaling_layer.display;
}


// void set_default(void) method

/// This member sets the default values for the unscaling layer:
/// <ul>
/// <li> Minimus: -1 for all unscaling neurons.
/// <li> Maximums: 1 for al unscaling neurons. 
/// <li> Means: 0 for all unscaling neurons. 
/// <li> Standard deviations 1 for all unscaling neurons. 
/// <li> Unscaling method: Minimum and maximum. 
/// <li> Display: True. 
/// </ul>

void UnscalingLayer::set_default(void)
{
   minimums.initialize(-1.0);
   maximums.initialize(1.0);
   means.initialize(0.0);
   standard_deviations.initialize(1.0);

   set_unscaling_method(MinimumMaximum);
   
   set_display(true);
}


// void set_minimums(const Vector<double>&) method

/// This method sets new minimums for all the unscaling neurons. 
/// These values are used for unscaling variables with the minimums and maximums method. 
/// @param new_minimums New set of minimum values for the unscaling neurons. 

void UnscalingLayer::set_minimums(const Vector<double>& new_minimums)
{
   // Control sentence (if debug)

   #ifdef _DEBUG 

   const unsigned int unscaling_neurons_number = count_unscaling_neurons_number();

   if(new_minimums.size() != unscaling_neurons_number)
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: UnscalingLayer class.\n"
             << "void set_minimums(const Vector<double>&) method.\n"
             << "Size must be equal to number of unscaling neurons.\n";

	  throw std::logic_error(buffer.str());
   }

   #endif

   // Set minimums of output variables

   minimums = new_minimums;
}


// void set_minimum(const unsigned int&, const double&) method

/// This method sets a new minimum value for a single unscaling neuron. 
/// This value is used for unscaling that variable with the minimums and maximums method. 
/// @param index Index of unscaling neuron.
/// @param new_minimum New minimum value for that neuron.

void UnscalingLayer::set_minimum(const unsigned int& index, const double& new_minimum)
{
   const unsigned int unscaling_neurons_number = count_unscaling_neurons_number();

   // Control sentence (if debug)

   #ifdef _DEBUG 

   if(index >= unscaling_neurons_number)
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: UnscalingLayer class.\n" 
             << "void set_minimum(const unsigned int&, const double&) method.\n"
             << "Index of unscaling neuron must be less than number of unscaling neurons.\n";

	  throw std::logic_error(buffer.str());
   }

   #endif

   if(minimums.size() == 0)
   {
      minimums.set(unscaling_neurons_number, -1.0);
   }

   // Set minimums of single output variable

   minimums[index] = new_minimum;
}


// void set_maximums(const Vector<double>&) method

/// This method sets new maximum values for all the unscaling neurons. 
/// These values are used for unscaling variables with the minimums and maximums method. 
/// @param new_maximums New set of maximum values for the unscaling neurons.

void UnscalingLayer::set_maximums(const Vector<double>& new_maximums)
{
   // Control sentence (if debug)

   #ifdef _DEBUG 

   const unsigned int unscaling_neurons_number = count_unscaling_neurons_number();

   if(new_maximums.size() != unscaling_neurons_number)
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: UnscalingLayer class.\n"
             << "void set_maximum(const Vector<double>&) method.\n"
             << "Size must be equal to number of unscaling neurons.\n";

	  throw std::logic_error(buffer.str());
   }

   #endif

   maximums = new_maximums;
}


// void set_maximums(const unsigned int&, const double&) method

/// This method sets a new maximum value for a single unscaling neuron.
/// This value is used for unscaling that variable with the minimums and maximums method. 
/// @param index Index of output variable.
/// @param new_maximum New maximum value for that output variable.

void UnscalingLayer::set_maximum(const unsigned int& index, const double& new_maximum)
{
   const unsigned int unscaling_neurons_number = count_unscaling_neurons_number();

   // Control sentence (if debug)

   #ifdef _DEBUG 

   if(index >= unscaling_neurons_number)
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: UnscalingLayer class.\n" 
             << "void new_maximum(const unsigned int&, const double&) method.\n"
             << "Index must be less than number of unscaling neurons outputs.\n";

	  throw std::logic_error(buffer.str());
   }

   #endif

   if(maximums.size() == 0)
   {
      maximums.set(unscaling_neurons_number, 1.0);
   }

   // Set maximums of single output variable

   maximums[index] = new_maximum;
}


// void set_mean(const Vector< double>&) method

/// This method sets new mean values for all the unscaling neurons.
/// These values are used for unscaling variables with the meand and standard deviation method. 
/// @param new_means New set of mean values for the usncaling neurons.

void UnscalingLayer::set_means(const Vector<double>& new_means)
{
   // Control sentence (if debug)

   #ifdef _DEBUG 

   const unsigned int unscaling_neurons_number = count_unscaling_neurons_number();

   if(new_means.size() != unscaling_neurons_number)
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: UnscalingLayer class.\n" 
             << "void set_means(const Vector<double>&) method.\n"
             << "Size must be equal to number of unscaling neurons.\n";

	  throw std::logic_error(buffer.str());
   }

   #endif

   // Set means of output variables

   means = new_means;
}


// void set_mean(const unsigned int&, const double&) method

/// This method sets a new mean value for a single unscaling neuron.
/// That value is used for unscaling a variable with the meand and standard deviation method. 
/// @param index Index of unscaling neuron.
/// @param new_mean New mean value for the unscaling neuron with the previous index.

void UnscalingLayer::set_mean(const unsigned int& index, const double& new_mean)
{
   const unsigned int unscaling_neurons_number = count_unscaling_neurons_number();

   // Control sentence (if debug)

   #ifdef _DEBUG 

   if(index >= unscaling_neurons_number)
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: UnscalingLayer class.\n"
             << "void set_mean(const unsigned int&, const double&) method.\n"
             << "Index must be less than number of unscaling neurons.\n";

	  throw std::logic_error(buffer.str());
   }

   #endif

   // Set vector if necessary

   if(means.size() != unscaling_neurons_number)
   {
      means.set(unscaling_neurons_number, 0.0);
   }

   // Set means of single output variable

   means[index] = new_mean;
}


// void set_standard_deviations(const Vector<double>&) method

/// This method sets new standard deviation values for all the variables.
/// These values are used for unscaling outputs with the meand and standard deviation method. 
/// @param new_standard_deviations New set of standard deviation values for the variables.

void UnscalingLayer::set_standard_deviations(const Vector<double>& new_standard_deviations) 
{
   // Control sentence (if debug)

   #ifdef _DEBUG 

   const unsigned int unscaling_neurons_number = count_unscaling_neurons_number();

   if(new_standard_deviations.size() != unscaling_neurons_number)
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: UnscalingLayer class.\n"
             << "void set_standard_deviations(const Vector<double>&) method.\n"
             << "Size must be equal to number of unscaling neurons.\n";

	  throw std::logic_error(buffer.str());
   }

   #endif

   // Set standard deviation of output variables

   standard_deviations = new_standard_deviations;
}


// void set_standard_deviation(const unsigned int&, const double&) method

/// This method sets a new standard deviation value for a single output variable.
/// These values are used for unscaling the outputs form the multilayer perceptron with the meand and 
/// standard deviation method. 
/// @param index Index of output variable.
/// @param new_standard_deviation New standard deviation value for that output variable.

void UnscalingLayer::set_standard_deviation(const unsigned int& index, const double& new_standard_deviation)
{
   const unsigned int unscaling_neurons_number = count_unscaling_neurons_number();

   // Control sentence (if debug)

   #ifdef _DEBUG 

   if(index >= unscaling_neurons_number)
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: UnscalingLayer class.\n"
             << "void set_standard_deviation(const unsigned int&, const double&) method.\n"
             << "Index must be less than number of unscaling neurons.\n";

	  throw std::logic_error(buffer.str());
   }

   #endif

   // Set vector if necessary

   if(standard_deviations.size() != unscaling_neurons_number)
   {
      standard_deviations.set(unscaling_neurons_number, 1.0);
   }

   // Set standard deviation of single output variable

   standard_deviations[index] = new_standard_deviation;
}


// void set_minimums_maximums(const Vector< Vector<double> >&) method

/// This method sets both the minimums and the maximum values of all the unscaling neurons.
/// The format is a vector of two real vectors. 
/// The first element must contain the minimum values for the variables.
/// The second element must contain the maximum values for the variables.
/// These values are used for unscaling variables with the minimums and maximums method. 
/// @param new_minimums_maximums Set of minimum and maximum values for the variables.

void UnscalingLayer::set_minimums_maximums(const Vector< Vector<double> >& new_minimums_maximums)
{
   // Control sentence (if debug)

   #ifdef _DEBUG 

   const unsigned int unscaling_neurons_number = count_unscaling_neurons_number();

   if(new_minimums_maximums.size() != 2)
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: UnscalingLayer class.\n" 
             << "void set_minimums_maximums(const Vector< Vector<double> >&) method.\n"
             << "Number of rows must be 2.\n";

	  throw std::logic_error(buffer.str());
   }
   else if(new_minimums_maximums[0].size() != unscaling_neurons_number
        && new_minimums_maximums[1].size() != unscaling_neurons_number)
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: UnscalingLayer class.\n" 
             << "void set_minimums_maximums(const Vector< Vector<double> >&) method.\n"
             << "Number of columns must be equal to number of unscaling neurons.\n";

	  throw std::logic_error(buffer.str());
   }

   // Check that minimums of output variables is not greater than their maximums

   for(unsigned int i = 0; i < unscaling_neurons_number; i++)
   {
      if(new_minimums_maximums[0][i] > new_minimums_maximums[1][i])
      {
         std::ostringstream buffer;

         buffer << "OpenNN Exception: UnscalingLayer class.\n" 
                << "void set_minimum_maximum(const Vector< Vector<double> >&) method.\n"
                << "Minimum of output variable " << i << " is greater than maximums of that variable.\n";

	     throw std::logic_error(buffer.str());
      }
   }

   #endif

   // Set minimums and maximums of output variables

   minimums = new_minimums_maximums[0];
   maximums = new_minimums_maximums[1];
}


// void set_means_standard_deviations(const Vector< Vector<double> >&) method

/// This method sets both the means and the standard deviation values of all the variables.
/// The format is a vector of two real vectors.
/// The first element must contain the mean values for the variables.
/// The second element must contain the standard deviation values for the variables.
/// @param new_means_standard_deviations Set of mean and standard deviation values for all the variables.

void UnscalingLayer::set_means_standard_deviations(const Vector< Vector<double> >& new_means_standard_deviations)
{
   // Control sentence (if debug)

   #ifdef _DEBUG 

   const unsigned int unscaling_neurons_number = count_unscaling_neurons_number();

   if(new_means_standard_deviations.size() != 2)
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: UnscalingLayer class.\n"
             << "void set_mean_standard_deviation(const Vector< Vector<double> >&) method.\n"
             << "Size of vector must be 2.\n";

	  throw std::logic_error(buffer.str());
   }
   else if(new_means_standard_deviations[0].size() != unscaling_neurons_number && new_means_standard_deviations[1].size() != unscaling_neurons_number)
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: UnscalingLayer class.\n"
             << "void set_mean_standard_deviation(const Vector< Vector<double> >&) method.\n"
             << "Number of columns must be equal to number of unscaling neurons.\n";

	  throw std::logic_error(buffer.str());
   }

   #endif

   // Set means and standard deviation of output variables

   means = new_means_standard_deviations[0];   
   standard_deviations = new_means_standard_deviations[1];
}


// void set_statistics(const Vector< Vector<double> >&) method

/// This method sets all the available statistics about the inputs and output variables.
/// The format is a vector of ten real vectors: 
/// <ul>
/// <li> Mean of input variables.
/// <li> Standard deviation of input variables.
/// <li> Minimum of input variables.
/// <li> Maximum of input variables.
/// <li> Mean of output variables.
/// <li> Standard deviation of output variables.
/// <li> Minimum of output variables.
/// <li> Maximum of output variables.
/// </ul>
/// @param new_statistics Variables statistics. 

void UnscalingLayer::set_statistics(const Vector< Vector<double> >& new_statistics)
{
   // Control sentence (if debug)

   #ifdef _DEBUG 

   const unsigned int unscaling_neurons_number = count_unscaling_neurons_number();

   const unsigned int new_statistics_size = new_statistics.size();

   if(new_statistics_size != 4)
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: UnscalingLayer class.\n"
             << "void set_statistics(const Vector< Vector<double> >&) method.\n"
             << "Size must be 4.\n";

	  throw std::logic_error(buffer.str());
   }

   unsigned int size;

   for(unsigned int i = 0; i < 4; i++)
   {
      size = new_statistics[i].size();

	  if(size != unscaling_neurons_number)
 	  {
         std::ostringstream buffer;

         buffer << "OpenNN Exception: UnscalingLayer class.\n"
                << "void set_statistics(const Vector< Vector<double> >&) method.\n"
                << "Size of element " << i << " must be equal to number of unscaling neurons.\n";

	     throw std::logic_error(buffer.str());
	  }
   }

   #endif

   // Set all statistics
 
   set_minimums(new_statistics[0]);
   set_maximums(new_statistics[1]);
   set_means(new_statistics[2]);
   set_standard_deviations(new_statistics[3]);
}


// void set_unscaling_method(const UnscalingMethod&) method

/// This method sets the method to be used for unscaling the outputs from the multilayer perceptron
/// @param new_unscaling_method New unscaling method for the output variables.

void UnscalingLayer::set_unscaling_method(const UnscalingLayer::UnscalingMethod& new_unscaling_method)
{
   unscaling_method = new_unscaling_method;
}


// void set_unscaling_method(const std::string&) method

/// This method sets the method to be used for unscaling the outputs from the multilayer perceptron
/// The argument is a string containing the name of the method ("None", "MeanStandardDeviation" or "MinimumMaximum").
/// @param new_unscaling_method New unscaling method for the output variables.

void UnscalingLayer::set_unscaling_method(const std::string& new_unscaling_method)
{
   if(new_unscaling_method == "MeanStandardDeviation")
   {
      set_unscaling_method(MeanStandardDeviation);
   }
   else if(new_unscaling_method == "MinimumMaximum")
   {
      set_unscaling_method(MinimumMaximum);
   }
   else
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: UnscalingLayer class.\n"
             << "void set_unscaling_method(const std::string&) method.\n"
			 << "Unknown unscaling method: " << new_unscaling_method << ".\n";

	  throw std::logic_error(buffer.str());
   }
}


// void set_display(const bool&) method

/// This method sets a new display value. 
/// If it is set to true messages from this class are to be displayed on the screen;
/// if it is set to false messages from this class are not to be displayed on the screen.
/// @param new_display Display value.

void UnscalingLayer::set_display(const bool& new_display)
{
   display = new_display;
}


// void check_range(const Vector<double>&) const method

/// This method checks whether the outptus from the unscaling layer are inside the range defined by the minimums and maximum values. 
/// It displays a warning message if they are outside.
/// @param outputs Set of outptus from the unscaling layer.

void UnscalingLayer::check_range(const Vector<double>& outputs) const
{
   const unsigned int unscaling_neurons_number = count_unscaling_neurons_number();

   // Control sentence (if debug)

   #ifdef _DEBUG 

   const unsigned int size = outputs.size();

   if(size != unscaling_neurons_number) 
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: UnscalingLayer class.\n"
             << "void check_range(const Vector<double>&) const method.\n"
             << "Size of outputs must be equal to number of unscaling neurons.\n";

	  throw std::logic_error(buffer.str());
   }   

   #endif

   // Check outputs

   if(display)
   {
      if(minimums.size() != 0)
	  {
         for(unsigned int i = 0; i < unscaling_neurons_number; i++)
         {
            if(outputs[i] < minimums[i])
            {
               std::cout << "OpenNN Warning: UnscalingLayer class.\n"
                         << "void check_range(const Vector<double>&) const method.\n"
                         << "Output variable " << i << " is less than outputs.\n";                  
            }
		 }
	  }
      if(maximums.size() != 0)
	  {
         for(unsigned int i = 0; i < unscaling_neurons_number; i++)
         {
            if(outputs[i] > maximums[i])
            {
               std::cout << "OpenNN Warning: UnscalingLayer class.\n"
                         << "void check_range(const Vector<double>&) const method.\n"
                         << "Output variable " << i << " is greater than maximum.\n";
            }
         }
      }
   }
}


// bool is_empty(void) const method

/// This method returns true if the number of unscaling neurons is zero, and false otherwise. 

bool UnscalingLayer::is_empty(void) const
{
	const unsigned int unscaling_neurons_number = count_unscaling_neurons_number(); 

   if(unscaling_neurons_number == 0)
   {
      return(true);
   }
   else
   {
      return(false);
   }
}


// void initialize_random(void) method

/// @todo

void UnscalingLayer::initialize_random(void)
{

}


// Vector<double> calculate_outputs(const Vector<double>&) const method

/// This method calculates the outputs from the unscaling layer for a given set of inputs to that layer.  
/// @param inputs Set of inputs to the unscaling layer.

Vector<double> UnscalingLayer::calculate_outputs(const Vector<double>& inputs) const
{
   // Control sentence (if debug)

   #ifdef _DEBUG 

   const unsigned int unscaling_neurons_number = count_unscaling_neurons_number();

   const unsigned int size = inputs.size();

   if(size != unscaling_neurons_number) 
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: UnscalingLayer class.\n"
             << "Vector<double> calculate_outputs(const Vector<double>&) const method.\n"
             << "Size must be equal to number of unscaling neurons.\n";

	  throw std::logic_error(buffer.str());
   }   

   #endif

   // Unscale 

   switch(unscaling_method)
   {
      case MinimumMaximum:
      {
         return(calculate_minimum_maximum_output(inputs));
      }// end minimums and maximums
      break;       

      case MeanStandardDeviation:
      {
         return(calculate_mean_standard_deviation_output(inputs));
      }// end means and standard deviation
      break;

      default:
      {
         std::ostringstream buffer;

         buffer << "OpenNN Exception: UnscalingLayer class.\n"
                << "Vector<double> calculate_outputs(const Vector<double>&) const method.\n"               
                << "Unknown unscaling method.\n";
 
	     throw std::logic_error(buffer.str());
      }// end default
      break;

   }// end switch
}  


// Vector<double> calculate_derivative(const Vector<double>&) const method

/// This method retuns the derivatives of the unscaled outputs with respect to the scaled outputs.
/// That derivatives depend on the method for unscaling the outputs to be used. 

Vector<double> UnscalingLayer::calculate_derivative(const Vector<double>& inputs) const
{
   // Control sentence (if debug)

//   #ifdef _DEBUG 
//
//   unsigned int size = scaled_output.size();
//
//   if(size != unscaling_neurons_number) 
//   {
//      std::ostringstream buffer;

//      buffer << "OpenNN Exception: UnscalingLayer class.\n"
//             << "Vector<double> calculate_unscaled_derivative(const Vector<double>&) const method.\n"
//             << "Size must be equal to number of unscaling neurons.\n";
//
//	  throw std::logic_error(buffer.str());
//   }   
//
//   #endif

   switch(unscaling_method)
   {
      case MinimumMaximum:
      {
         return(calculate_minimum_maximum_derivative(inputs));
      }// end minimums and maximums
      break;       

      case MeanStandardDeviation:
      {
         return(calculate_mean_standard_deviation_derivative(inputs));
      }// end means and standard deviation
      break;

      default:
      {
         std::ostringstream buffer;

         buffer << "OpenNN Exception: UnscalingLayer class.\n"
                << "Vector<double> calculate_derivative(const Vector<double>&) const.\n"               
                << "Unknown scaling and unscaling method.\n";
 
	     throw std::logic_error(buffer.str());
      }// end default
      break;

   }// end switch
}


// Vector<double> calculate_second_derivative(const Vector<double>&) const

/// This method retuns the second derivatives of the unscaled outputs with respect to the scaled outputs.
/// That second derivatives depend on the method for unscaling the outputs to be used. 

Vector<double> UnscalingLayer::calculate_second_derivative(const Vector<double>& inputs) const
{
   switch(unscaling_method)
   {
      case MinimumMaximum:
      {
         return(calculate_minimum_maximum_second_derivative(inputs));
      }// end minimums and maximums
      break;       

      case MeanStandardDeviation:
      {
         return(calculate_mean_standard_deviation_second_derivative(inputs));
      }// end means and standard deviation
      break;

      default:
      {
         std::ostringstream buffer;

         buffer << "OpenNN Exception: UnscalingLayer class.\n"
                << "Vector<double> calculate_second_derivative(const Vector<double>&) const.\n"               
                << "Unknown scaling and unscaling method.\n";
 
	     throw std::logic_error(buffer.str());
      }// end default
      break;

   }// end switch
}


// Vector<double> calculate_minimum_maximum_output(const Vector<double>&) const method

/// This method calculates the outputs from the unscaling layer with the minimum and maximum method for a set of inputs.
/// @param inputs Vector of input values to the unscaling layer. The size must be equal to the number of unscaling neurons. 

Vector<double> UnscalingLayer::calculate_minimum_maximum_output(const Vector<double>& inputs) const
{
   const unsigned int unscaling_neurons_number = count_unscaling_neurons_number();

   Vector<double> outputs(unscaling_neurons_number);

   for(unsigned int i = 0; i < unscaling_neurons_number; i++)
   {
      if(maximums[i]-minimums[i] < 1e-99)
      {      
         if(display)
         {
            std::cout << "OpenNN Warning: UnscalingLayer class.\n"
                      << "Vector<double> calculate_minimum_maximum_output(Vector<double>&) const method.\n"               
                      << "Minimum and maximum values of output variable " << i << " are equal.\n"
                      << "Those outputs won't be unscaled.\n";
         }

         outputs[i] = inputs[i];
      }      
      else
      {
         outputs[i] = 0.5*(inputs[i] + 1.0)*(maximums[i]-minimums[i]) + minimums[i]; 
      }
   }

   return(outputs);
}


// Vector<double> calculate_minimum_maximum_derivative(const Vector<double>&) const method

/// This method calculates the derivatives of the outputs from the unscaling layer with the minimum and maximum method.
/// As the minimum and maximum method is a linear method, the derivatives will not depend on the inputs. 

Vector<double> UnscalingLayer::calculate_minimum_maximum_derivative(const Vector<double>&) const
{
   const unsigned int unscaling_neurons_number = count_unscaling_neurons_number();

   Vector<double> derivative(unscaling_neurons_number);

   for(unsigned int i = 0; i < unscaling_neurons_number; i++)
   {
      if(maximums[i]-minimums[i] < 1e-99)
      {      
         if(display)
         {
             std::cout << "OpenNN Warning: UnscalingLayer class.\n"
                       << "Vector<double> calculate_minimum_maximum_derivative(const Vector<double>&) const.\n"               
                       << "Minimum and maximum values of output variable " << i << " are equal.\n"
                       << "Those derivatives won't be unscaled.\n";
         }

         derivative[i] = 1.0;
      }      
      else
      {
         derivative[i] = 0.5*(maximums[i]-minimums[i]);
      }
   }

   return(derivative);
}


// Vector<double> calculate_minimum_maximum_second_derivative(const Vector<double>&) const method

/// This method calculates the second derivatives of the outputs from the unscaling layer with the minimum and maximum method.
/// As the minimum and maximum method is a linear method, the second derivatives will be always zero. 

Vector<double> UnscalingLayer::calculate_minimum_maximum_second_derivative(const Vector<double>&) const
{
   const unsigned int unscaling_neurons_number = count_unscaling_neurons_number();

   const Vector<double> unscaled_second_derivative(unscaling_neurons_number, 0.0);

   return(unscaled_second_derivative);
}


// Vector<double> calculate_mean_standard_deviation_output(const Vector<double>&) const method

/// This method calculates the outputs from the unscaling layer with the mean and standard deviation method for a set of inputs.
/// @param inputs Vector of input values to the unscaling layer. The size must be equal to the number of unscaling neurons. 

Vector<double> UnscalingLayer::calculate_mean_standard_deviation_output(const Vector<double>& inputs) const
{
   const unsigned int unscaling_neurons_number = count_unscaling_neurons_number();

   Vector<double> outputs(unscaling_neurons_number);

   for(unsigned int i = 0; i < unscaling_neurons_number; i++)
   {
      if(standard_deviations[i] < 1e-99)
      {      
         if(display)
         {
            std::cout << "OpenNN Warning: UnscalingLayer class.\n"
                      << "Vector<double> calculate_mean_standard_deviation_output(const Vector<double>&) const method.\n"               
                      << "Standard deviation of output variable " << i << " is zero.\n" 
                      << "Those outputs won't be unscaled.\n";
         }
               
         outputs[i] = inputs[i];
      }      
      else
      {
         outputs[i] = inputs[i]*standard_deviations[i] + means[i];
      }
   }

   return(outputs);
}


// Vector<double> calculate_mean_standard_deviation_derivative(const Vector<double>&) const method

/// This method calculates the derivatives of the outputs from the unscaling layer with the mean and standard deviation method.
/// As the minimum and maximum method is a linear method, the derivatives will not depend on the inputs. 

Vector<double> UnscalingLayer::calculate_mean_standard_deviation_derivative(const Vector<double>&) const
{
   const unsigned int unscaling_neurons_number = count_unscaling_neurons_number();

   Vector<double> unscaled_derivative(unscaling_neurons_number, 0.0);

   for(unsigned int i = 0; i < unscaling_neurons_number; i++)
   {
      if(standard_deviations[i] < 1e-99)
      {      
         if(display)
         {
            std::cout << "OpenNN Warning: UnscalingLayer class.\n"
                      << "Vector<double> calculate_derivative(const Vector<double>&) const.\n"               
                      << "Standard deviation of output variable " << i << " is zero.\n" 
                      << "Those derivatives won't be unscaled.\n";
         }
         
         unscaled_derivative[i] = 1.0;
      }      
      else
      {
         unscaled_derivative[i] = standard_deviations[i];
      }
   }

   return(unscaled_derivative);
}


// Vector<double> calculate_mean_standard_deviation_second_derivative(const Vector<double>&) const method

/// This method calculates the second derivatives of the outputs from the unscaling layer with the mean and standard deviation method.
/// As the minimum and maximum method is a linear method, the second derivatives will be always zero. 

Vector<double> UnscalingLayer::calculate_mean_standard_deviation_second_derivative(const Vector<double>&) const
{
   const unsigned int unscaling_neurons_number = count_unscaling_neurons_number();

   const Vector<double> unscaled_second_derivative(unscaling_neurons_number, 0.0);

   return(unscaled_second_derivative);
}


// Matrix<double> arrange_Jacobian(const Vector<double>&) const method

/// This method arranges a "Jacobian" matrix from the vector of derivatives. 

Matrix<double> UnscalingLayer::arrange_Jacobian(const Vector<double>& derivatives) const
{
   const unsigned int unscaling_neurons_number = count_unscaling_neurons_number();

   Matrix<double> Jacobian(unscaling_neurons_number, unscaling_neurons_number, 0.0);

   Jacobian.set_diagonal(derivatives);

   return(Jacobian);
}


// Vector< Matrix<double> > arrange_Hessian_form(const Vector<double>&) const method

/// This method arranges a "Hessian form" vector of matrices from the vector of second derivatives. 

Vector< Matrix<double> > UnscalingLayer::arrange_Hessian_form(const Vector<double>& second_derivative) const
{
   const unsigned int unscaling_neurons_number = count_unscaling_neurons_number();

   Vector< Matrix<double> > Hessian_form(unscaling_neurons_number);

   for(unsigned int i = 0; i < unscaling_neurons_number; i++)
   {
      Hessian_form[i].set(unscaling_neurons_number, unscaling_neurons_number, 0.0);

      Hessian_form[i][i][i] = second_derivative[i];
   }
   return(Hessian_form);
}


// std::string to_string(void) const method

/// This method returns a string representation of the current unscaling layer object. 

std::string UnscalingLayer::to_string(void) const
{
   std::ostringstream buffer;

   buffer << "Unscaling layer\n"
          << "Minimums: " << minimums << "\n"
          << "Maximums: " << maximums << "\n"
          << "Means: " << means << "\n"
          << "Standard deviations: " << standard_deviations << "\n"
          << "Unscaling method: " << write_unscaling_method() << "\n"
          << "Display: " << display << "\n";

   return(buffer.str());
}


// TiXmlElement* to_XML(void) const method

/// This method serializes this unscaling layer object into a TinyXML element. 
/// Please read the OpenNN manual for more information about this. 

TiXmlElement* UnscalingLayer::to_XML(void) const
{
   std::ostringstream buffer;

   TiXmlElement* unscaling_layer_element = new TiXmlElement("UnscalingLayer");
   unscaling_layer_element->SetAttribute("Version", 4);

   // Minimums
   {
      TiXmlElement* element = new TiXmlElement("Minimums");
      unscaling_layer_element->LinkEndChild(element);

      TiXmlText* text = new TiXmlText(minimums.to_string().c_str());
      element->LinkEndChild(text);
   }

   // Maximums
   {
      TiXmlElement* element = new TiXmlElement("Maximums");
      unscaling_layer_element->LinkEndChild(element);

      TiXmlText* text = new TiXmlText(maximums.to_string().c_str());
      element->LinkEndChild(text);
   }

   // Means
   {
      TiXmlElement* element = new TiXmlElement("Means");
      unscaling_layer_element->LinkEndChild(element);

      TiXmlText* text = new TiXmlText(means.to_string().c_str());
      element->LinkEndChild(text);
   }

   // Standard deviations
   {
      TiXmlElement* element = new TiXmlElement("StandardDeviations");
      unscaling_layer_element->LinkEndChild(element);

      TiXmlText* text = new TiXmlText(standard_deviations.to_string().c_str());
      element->LinkEndChild(text);
   }

   // Unscaling method
   {
      TiXmlElement* element = new TiXmlElement("UnscalingMethod");
      unscaling_layer_element->LinkEndChild(element);

      TiXmlText* text = new TiXmlText(write_unscaling_method().c_str());
      element->LinkEndChild(text);
   }

   // Display
   {
      TiXmlElement* element = new TiXmlElement("Display");
      unscaling_layer_element->LinkEndChild(element);

      buffer.str("");
      buffer << display;

      TiXmlText* text = new TiXmlText(buffer.str().c_str());
      element->LinkEndChild(text);
   }

   return(unscaling_layer_element);
}


// void from_XML(TiXmlElement*) method

/// This method deserializes a TinyXML element into this unscaling layer object. 
/// @param unscaling_layer_element Pointer to a XML element containing the member data. 

void UnscalingLayer::from_XML(TiXmlElement* unscaling_layer_element)
{
   if(unscaling_layer_element)
   { 
   
      // Unscaling method
      {
         TiXmlElement* unscaling_method_element = unscaling_layer_element->FirstChildElement("UnscalingMethod");

         if(unscaling_method_element)
         {
            std::string new_method = unscaling_method_element->GetText();

            try
            {
               set_unscaling_method(new_method);
            }
            catch(std::exception& e)
            {
               std::cout << e.what() << std::endl;		 
            }
         }
     }

      // Minimums
      {
         TiXmlElement* minimums_element = unscaling_layer_element->FirstChildElement("Minimums");

         if(minimums_element)
         {
            const char* minimums_text = minimums_element->GetText();

            if(minimums_text)
            {
               Vector<double> new_minimums;                  
               new_minimums.parse(minimums_text);

               try
               {
                  set_minimums(new_minimums);
               }
               catch(std::exception& e)
               {
                  std::cout << e.what() << std::endl;		 
               }
            }      
         }
      }

      // Maximums
      {
         TiXmlElement* maximums_element = unscaling_layer_element->FirstChildElement("Maximums");

         if(maximums_element)
         {
            const char* maximums_text = maximums_element->GetText();

            if(maximums_text)
            {
               Vector<double> new_maximums;                  
               new_maximums.parse(maximums_text);

               try
               {
                  set_maximums(new_maximums);
               }
               catch(std::exception& e)
               {
                  std::cout << e.what() << std::endl;		 
               }
            }
         }
      }

      // Means
      {
         TiXmlElement* means_element = unscaling_layer_element->FirstChildElement("Means");

         if(means_element)
         {
            const char* means_text = means_element->GetText();

            if(means_text)
            {
               Vector<double> new_means;                  
               new_means.parse(means_text);

               try
               {
                  set_means(new_means);
               }
               catch(std::exception& e)
               {
                  std::cout << e.what() << std::endl;		 
               }
            }
         }
      }

      // Standard deviations
      {
         TiXmlElement* standard_deviations_element = unscaling_layer_element->FirstChildElement("StandardDeviations");

         if(standard_deviations_element)
         {
            const char* standard_deviations_text = standard_deviations_element->GetText();

            if(standard_deviations_text)
            {
               Vector<double> new_standard_deviations;                  
               new_standard_deviations.parse(standard_deviations_text);

               try
               {
                  set_standard_deviations(new_standard_deviations);
               }
               catch(std::exception& e)
               {
                  std::cout << e.what() << std::endl;		 
               }
            }
         }
      }

      // Display
      {
         TiXmlElement* display_element = unscaling_layer_element->FirstChildElement("Display");

         if(display_element)
         {
            std::string new_display_string = display_element->GetText();     

            try
            {
               set_display(new_display_string != "0");
            }
            catch(std::exception& e)
            {
               std::cout << e.what() << std::endl;		 
            }
         }
      }
   }
}


// std::string write_minimum_maximum_expression(const Vector<std::string>&, const Vector<std::string>&) const method

/// This method returns a string with the expression of the unscaling process with the minimum and maximum method. 
/// @param inputs_name Name of inputs to the unscaling layer. The size of this vector must be equal to the number of unscaling neurons. 
/// @param outputs_name Name of outputs from the unscaling layer. The size of this vector must be equal to the number of unscaling neurons. 

std::string UnscalingLayer::write_minimum_maximum_expression(const Vector<std::string>& inputs_name, const Vector<std::string>& outputs_name) const
{
   const unsigned int unscaling_neurons_number = count_unscaling_neurons_number();

   std::ostringstream buffer;

   for(unsigned int i = 0; i < unscaling_neurons_number; i++)
   {
      buffer << outputs_name[i] << "=0.5*(" << inputs_name[i] << "+1.0)*(" << maximums[i] << "-" << minimums[i] << ")+" << minimums[i] << ";\n";       
   }

   return(buffer.str());
}


// std::string write_mean_stadard_deviation_expression(const Vector<std::string>&, const Vector<std::string>&) const method

/// This method returns a string with the expression of the unscaling process with the mean and standard deviation method. 
/// @param inputs_name Name of inputs to the unscaling layer. The size of this vector must be equal to the number of unscaling neurons. 
/// @param outputs_name Name of outputs from the unscaling layer. The size of this vector must be equal to the number of unscaling neurons. 

std::string UnscalingLayer::write_mean_stadard_deviation_expression(const Vector<std::string>& inputs_name, const Vector<std::string>& outputs_name) const
{
   const unsigned int unscaling_neurons_number = count_unscaling_neurons_number();

   std::ostringstream buffer;

   for(unsigned int i = 0; i < unscaling_neurons_number; i++)
   {
      buffer << outputs_name[i] << "=" <<  means[i] << "+" << standard_deviations[i] << "*" << inputs_name[i] << ";\n";
   }

   return(buffer.str());
}


// std::string write_expression(const Vector<std::string>&, const Vector<std::string>&) const method

/// This method returns a string with the expression of the unscaling process in this layer. 
/// @param inputs_name Name of inputs to the unscaling layer. The size of this vector must be equal to the number of unscaling neurons. 
/// @param outputs_name Name of outputs from the unscaling layer. The size of this vector must be equal to the number of unscaling neurons. 

std::string UnscalingLayer::write_expression(const Vector<std::string>& inputs_name, const Vector<std::string>& outputs_name) const
{
   switch(unscaling_method)   
   {
      case MinimumMaximum:
      {   
         return(write_minimum_maximum_expression(inputs_name, outputs_name));
      }
      break;

      case MeanStandardDeviation:
      {
         return(write_mean_stadard_deviation_expression(inputs_name, outputs_name));
      }
      break;

      default:
      {   
         std::ostringstream buffer;

         buffer << "OpenNN Exception: UnscalingLayer class.\n"
                << "std::string write_expression(const Vector<std::string>&, const Vector<std::string>&) const method.\n"
                << "Unknown unscaling method.\n";
 
         throw std::logic_error(buffer.str());
      }// end default
      break;
   }

//   return("");
}

}


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
