/****************************************************************************************************************/
/*                                                                                                              */
/*   OpenNN: Open Neural Networks Library                                                                       */
/*   www.opennn.cimne.com                                                                                       */
/*                                                                                                              */
/*   S C A L I N G   L A Y E R   C L A S S                                                                      */
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

#include "scaling_layer.h"

namespace OpenNN
{

// DEFAULT CONSTRUCTOR

/// Default constructor. 
/// It creates a scaling layer object with no scaling neurons. 

ScalingLayer::ScalingLayer(void)
{   
   set();
}


// SCALING NEURONS NUMBER CONSTRUCTOR

/// Scaling neurons number constructor.
/// This constructor creates a scaling layer with a given size. 
/// The members of this object are initialized with the default values. 
/// @param new_scaling_neurons_number Number of scaling neurons in the layer. 

ScalingLayer::ScalingLayer(const unsigned int& new_scaling_neurons_number)
{
   set(new_scaling_neurons_number);
}


// STATISTICS CONSTRUCTOR

/// Statistics constructor.
/// This constructor creates a scaling layer with given minimums, maximums, means and standard deviations. 
/// The rest of members of this object are initialized with the default values. 
/// @param new_statistics Vector of vectors with the variables statistics. 

ScalingLayer::ScalingLayer(const Vector< Vector<double> >& new_statistics)
{
   set(new_statistics);
}


// COPY CONSTRUCTOR

/// Copy constructor. 

ScalingLayer::ScalingLayer(const ScalingLayer& new_scaling_layer)
{
	set(new_scaling_layer);
}


// DESTRUCTOR

/// Destructor.

ScalingLayer::~ScalingLayer(void)
{
}


// ASSIGNMENT OPERATOR

// ScalingLayer& operator = (const ScalingLayer&) method

/// Assignment operator.
/// @param other_scaling_layer Object to be copied. 

ScalingLayer& ScalingLayer::operator = (const ScalingLayer& other_scaling_layer)
{
   if(this != &other_scaling_layer) 
   {
      minimums = other_scaling_layer.minimums;

      maximums = other_scaling_layer.maximums;

      means = other_scaling_layer.means;

      standard_deviations = other_scaling_layer.standard_deviations;

      scaling_method = other_scaling_layer.scaling_method;
      display = other_scaling_layer.display;
   }

   return(*this);
}


// EQUAL TO OPERATOR

// bool operator == (const ScalingLayer&) const method

/// Equal to operator. 
/// If compares this object with another object of the same class, and returns true if they are equal, and false otherwise. 
/// @param other_scaling_layer Object to be compared with. 

bool ScalingLayer::operator == (const ScalingLayer& other_scaling_layer) const
{
   if(minimums == other_scaling_layer.minimums
   && maximums == other_scaling_layer.maximums
   && means == other_scaling_layer.means
   && standard_deviations == other_scaling_layer.standard_deviations
   && scaling_method == other_scaling_layer.scaling_method
   && display == other_scaling_layer.display)
   {
      return(true);
   }
   else
   {
      return(false);
   }
}


// unsigned int count_scaling_neurons_number(void) const method

/// This method returns the number of unscaling neurons in this layer. 

unsigned int ScalingLayer::count_scaling_neurons_number(void) const
{
   return(minimums.size());
}


// const Vector<double>& get_maximums(void) const method

/// This method returns the maximum values of the scaling layer. 
/// Such values are to be used for scaling variables with the minimums and maximums method.

const Vector<double>& ScalingLayer::get_maximums(void) const
{
   return(maximums);               
}


// double get_maximum(const unsigned int&) const method

/// This method returns the maximum value of a single variable.
/// Such value is to be used for scaling that variable with the minimums and maximums method.

double ScalingLayer::get_maximum(const unsigned int& i) const
{
   // Control sentence (if debug)

   #ifdef _DEBUG 

   const unsigned int inputs_number = count_scaling_neurons_number();

   if(inputs_number == 0)
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: ScalingLayer class.\n" 
             << "double get_maximum(const unsigned int&) const method.\n"
             << "Number of inputs is zero.\n";

	  throw std::logic_error(buffer.str());
   }
   else if(i >= inputs_number)
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: ScalingLayer class.\n" 
             << "double get_maximum(const unsigned int&) const method.\n"
             << "Index must be less than number of scaling neurons.\n";

	  throw std::logic_error(buffer.str());
   }

   #endif

   return(maximums[i]);
}


// const Vector<double> get_minimum(void) const method

/// This method returns the minimum values of all the scaling neurons.
/// Such values are to be used for scaling variables with the minimums and maximums method.

const Vector<double>& ScalingLayer::get_minimums(void) const
{
   return(minimums);               
}


// double get_minimum(const unsigned int&) const method

/// This method returns the minimum value of a single neuron in the scaling layer.
/// Such value is to be used for scaling that variable with the minimums and maximums method.

double ScalingLayer::get_minimum(const unsigned int& i) const
{
   // Control sentence (if debug)

   #ifdef _DEBUG 

   const unsigned int inputs_number = count_scaling_neurons_number();

   if(inputs_number == 0)
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: ScalingLayer class.\n" 
             << "double get_minimum(const unsigned int&) const method.\n"
             << "Number of inputs is zero.\n";

	  throw std::logic_error(buffer.str());
   }
   else if(i >= inputs_number)
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: ScalingLayer class.\n" 
             << "double get_minimum(const unsigned int&) const method.\n"
             << "Index must be less than number of scaling neurons.\n";

	  throw std::logic_error(buffer.str());
   }

   #endif

   return(minimums[i]);
}


// const Vector<double>& get_mean(void) const method

/// This method returns the mean values of all the scaling neurons of the multilayer perceptron
/// Such values are to be used for scaling inputs with the means and standard deviation method. 

const Vector<double>& ScalingLayer::get_means(void) const
{
   return(means);
}


// double get_mean(const unsigned int&) const method

/// This method returns the mean value of a single input variable of the multilayer perceptron
/// Such a value is to be used for scaling that inputs with the means and standard deviation method. 
/// @param index Index of input variable.

double ScalingLayer::get_mean(const unsigned int& index) const
{
   // Control sentence (if debug)

   #ifdef _DEBUG 

   const unsigned int inputs_number = count_scaling_neurons_number();

   if(index >= inputs_number)
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: ScalingLayer class.\n" 
             << "double get_mean(const unsigned int&) const method.\n"
             << "Index of input variable must be less than number of scaling neurons.\n";

	  throw std::logic_error(buffer.str());
   }

   #endif

   return(means[index]);
}


// const Vector<double>& get_standard_deviations(void) const method

/// This method returns the standard deviation values of all the scaling neurons of the multilayer perceptron
/// Such values are to be used for scaling inputs with the means and standard deviation method. 

const Vector<double>& ScalingLayer::get_standard_deviations(void) const
{
   return(standard_deviations);
}


// double get_standard_deviation(const unsigned int&) const method

/// This method returns the standard deviation value of a single input variable of the multilayer perceptron
/// Such a value is to be used for scaling that inputs with the means and standard deviation method. 
/// @param index Index of input variable.

double ScalingLayer::get_standard_deviation(const unsigned int& index) const
{
   // Control sentence (if debug)

   #ifdef _DEBUG 

   const unsigned int inputs_number = count_scaling_neurons_number();

   if(index >= inputs_number)
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: ScalingLayer class.\n" 
             << "double get_standard_deviation(const unsigned int&) const method.\n"
             << "Index of input variable must be less than number of scaling neurons.\n";

	  throw std::logic_error(buffer.str());
   }

   #endif

   return(standard_deviations[index]);
}


// Vector< Vector<double>* > get_minimums_maximums(void) method

/// This method returns the minimums and the maximum values of all the scaling neurons. 
/// The format is a vector of pointers to vectors of size two. 
/// The first element contains the minimum values of the scaling neurons.
/// The second element contains the maximum values of the scaling neurons.
/// Such values are to be used for scaling inputs with the minimums and maximums method.

Vector< Vector<double>* > ScalingLayer::get_minimums_maximums(void)
{
   Vector< Vector<double>* > minimums_maximums(2);

   minimums_maximums[0] = &minimums;
   minimums_maximums[1] = &maximums;

   return(minimums_maximums);
}


// Vector< Vector<double>* > get_means_standard_deviations(void) method

/// This method returns the means and the standard deviation values of all the scaling neurons. 
/// The format is a vector of pointers to vectors of size two. 
/// The first element contains the mean values of the scaling neurons.
/// The second element contains the standard deviation values of the scaling neurons.
/// Such values are to be used for scaling inputs with the means and standard deviation method. 

Vector< Vector<double>* > ScalingLayer::get_means_standard_deviations(void)
{
   Vector< Vector<double>* > means_standard_deviations(2);

   means_standard_deviations[0] = &means;
   means_standard_deviations[1] = &standard_deviations;

   return(means_standard_deviations);
}


// Vector< Vector<double>* > get_statistics(void) method

/// This method returns all the available statistics of the inputs and variables. 
/// The format is a vector of pointers to vectors of size ten: 
/// <ul>
/// <li> Mean of scaling neurons.
/// <li> Standard deviation of scaling neurons.
/// <li> Minimum of scaling neurons.
/// <li> Maximum of scaling neurons.
/// </ul>

Vector< Vector<double>* > ScalingLayer::get_statistics(void)
{
   Vector< Vector<double>* > statistics(4);

   statistics[0] = &minimums;
   statistics[1] = &maximums;
   statistics[2] = &means;
   statistics[3] = &standard_deviations;

   return(statistics);
}


// const Method& get_scaling_method(void) const method

/// This method returns the method used for scaling.

const ScalingLayer::ScalingMethod& ScalingLayer::get_scaling_method(void) const
{
   return(scaling_method);
}


// std::string write_scaling_method(void) const method

/// This method returns a string with the name of the method used for scaling. 

std::string ScalingLayer::write_scaling_method(void) const
{
   if(scaling_method == MeanStandardDeviation)
   {
      return("MeanStandardDeviation");
   }
   else if(scaling_method == MinimumMaximum)
   {
      return("MinimumMaximum");
   }
   else
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: ScalingLayer class.\n"
             << "std::string write_scaling_method(void) const method.\n"
             << "Unknown scaling method.\n";
 
	  throw std::logic_error(buffer.str());
   }
}


// const bool& get_display(void) const method

/// This method returns true if messages from this class are to be displayed on the screen, or false if messages 
/// from this class are not to be displayed on the screen.

const bool& ScalingLayer::get_display(void) const
{
   return(display);
}


// void set(void) method

/// This method sets the scaling layer to be empty. 

void ScalingLayer::set(void)
{
   minimums.set();
   maximums.set();
   means.set();
   standard_deviations.set();

   set_default();
}


// void set(const unsigned int&) method

/// This method sets a new size in the scaling layer. 
/// It also sets the members to their default values. 

void ScalingLayer::set(const unsigned int& new_inputs_number)
{
   minimums.set(new_inputs_number, -1.0);
   maximums.set(new_inputs_number, 1.0);
   means.set(new_inputs_number, 0.0);
   standard_deviations.set(new_inputs_number, 1.0);

   set_default();
}


// void set(const Vector< Vector<double> >&) method

/// This method sets the size of the scaling layer and the statistics values. 
/// @param new_statistics Vector of vectors containing the minimums, maximums, means and standard deviations for the scaling layer. 
/// The size of this vector must be 4. 
/// The size of each subvector will be the size of the scaling layer. 

void ScalingLayer::set(const Vector< Vector<double> >& new_statistics)
{
   minimums = new_statistics[0];
   maximums = new_statistics[1];
   means = new_statistics[2];
   standard_deviations = new_statistics[3];

   set_default();
}


// void set(TiXmlElement*) method

/// This method sets the scaling layer members from a XML element. 
/// @param new_scaling_layer_element Pointer to a Tiny XML element containing the member data. 

void ScalingLayer::set(TiXmlElement* new_scaling_layer_element)
{
   from_XML(new_scaling_layer_element);
}


// void set(const ScalingLayer&) method

/// This method sets the members of this object to be the members of another object of the same class. 
/// @param new_scaling_layer Object to be copied. 

void ScalingLayer::set(const ScalingLayer& new_scaling_layer)
{
   minimums = new_scaling_layer.minimums;
   maximums = new_scaling_layer.maximums;
   means = new_scaling_layer.means;
   standard_deviations = new_scaling_layer.standard_deviations;

   scaling_method = new_scaling_layer.scaling_method;

   display = new_scaling_layer.display;
}


// void set_default(void) method

/// This method sets the members to their default value: 
/// <ul>
/// <li> Minimus: -1 for all unscaling neurons.
/// <li> Maximums: 1 for al unscaling neurons. 
/// <li> Means: 0 for all unscaling neurons. 
/// <li> Standard deviations 1 for all unscaling neurons. 
/// <li> Scaling method: Minimum and maximum. 
/// <li> Display: True. 
/// </ul>

void ScalingLayer::set_default(void)
{
   minimums.initialize(-1.0);
   maximums.initialize(1.0);
   means.initialize(0.0);
   standard_deviations.initialize(1.0);

   set_scaling_method(MinimumMaximum);
 
   set_display(true);
}


// void set_minimums(const Vector<double>&) method

/// This method sets new minimum values for all the scaling neurons.
/// These values are used for scaling variables with the minimums and maximums method. 
/// @param new_minimums New set of minimum values for the scaling neurons.

void ScalingLayer::set_minimums(const Vector<double>& new_minimums)
{
   // Control sentence (if debug)

   #ifdef _DEBUG 

   const unsigned int inputs_number = count_scaling_neurons_number();

   if(new_minimums.size() != inputs_number)
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: ScalingLayer class.\n"
             << "void set_minimums(const Vector<double>&) method.\n"
             << "Size must be equal to number of scaling neurons.\n";

	  throw std::logic_error(buffer.str());
   }

   #endif

   // Set minimums of scaling neurons

   minimums = new_minimums;

}


// void set_minimum(const unsigned int&, const double&) method

/// This method sets a new minimum value for a single scaling neuron. 
/// This value is used for scaling that variable with the minimums and maximums method. 
/// @param index Index of scaling neuron.
/// @param new_minimum New minimum value for the scaling neuron with the previous index.

void ScalingLayer::set_minimum(const unsigned int& index, const double& new_minimum)
{
   const unsigned int inputs_number = count_scaling_neurons_number();

   // Control sentence (if debug)

   #ifdef _DEBUG 

   if(index >= inputs_number)
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: ScalingLayer class.\n" 
             << "void set_minimum(const unsigned int&, const double&) method.\n"
             << "Index of variable must be less than number of scaling neurons.\n";

	  throw std::logic_error(buffer.str());
   }

   #endif

   if(minimums.size() == 0)
   {
      minimums.set(inputs_number, -1.0);
   }

   // Set minimums of single input variable

   minimums[index] = new_minimum;
}


// void set_maximum(const Vector<double>&) method

/// This method sets new maximum values for all the scaling neurons.
/// These values are used for scaling the inputs to the multilayer perceptron with the minimums and maximums method. 
/// @param new_maximum New set of maximum values for the scaling neurons of the multilayer perceptron

void ScalingLayer::set_maximums(const Vector<double>& new_maximum)
{
   // Control sentence (if debug)

   #ifdef _DEBUG 

   const unsigned int inputs_number = count_scaling_neurons_number();

   if(new_maximum.size() != inputs_number)
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: ScalingLayer class.\n"
             << "void set_maximum(const Vector<double>&) method.\n"
             << "Size must be equal to number of scaling neurons.\n";

	  throw std::logic_error(buffer.str());
   }
   
   #endif

   // Set maximums of scaling neurons

   maximums = new_maximum;
}


// void set_maximum(const unsigned int&, const double&) method

/// This method sets a new maximum value for a single input variable.
/// This value is used for scaling that inputs to the multilayer perceptron with the minimums and maximums method.
/// @param index Index of input variable.
/// @param new_maximum New maximum value for that input variable.

void ScalingLayer::set_maximum(const unsigned int& index, const double& new_maximum)
{
   const unsigned int inputs_number = count_scaling_neurons_number();

   // Control sentence (if debug)

   #ifdef _DEBUG 

   if(index >= inputs_number)
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: ScalingLayer class.\n" 
             << "void set_maximum(const unsigned int&, const double&) method.\n"
             << "Index of input variable must be less than scaling neurons.\n";

	  throw std::logic_error(buffer.str());
   }

   #endif

   if(maximums.size() == 0)
   {
      maximums.set(inputs_number, 1.0);
   }

   // Set maximums of single input variable

   maximums[index] = new_maximum;
}


//void set_means(const Vector<double>&) method

/// This method sets new mean values for all the scaling neurons.
/// These values are used for scaling the inputs to the multilayer perceptron with the meand and standard 
/// deviation method. 
/// @param new_mean New set of mean values for the scaling neurons of the multilayer perceptron

void ScalingLayer::set_means(const Vector<double>& new_mean)
{
   // Control sentence (if debug)

   #ifdef _DEBUG 

   const unsigned int inputs_number = count_scaling_neurons_number();

   if(new_mean.size() != inputs_number)
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: ScalingLayer class.\n" 
             << "void set_means(const Vector<double>&) method.\n"
             << "Size must be equal to number of scaling neurons.\n";

	  throw std::logic_error(buffer.str());
   }

   #endif

   // Set means of scaling neurons

   means = new_mean;
}


// void set_mean(const unsigned int&, const double&) method

/// This method sets a new mean value for a single input variable.
/// These values are used for scaling the inputs to the multilayer perceptron with the meand and standard 
/// deviation method. 
/// @param index Index of input variable.
/// @param new_mean New mean values for the input variable with index i.

void ScalingLayer::set_mean(const unsigned int& index, const double& new_mean)
{
   const unsigned int inputs_number = count_scaling_neurons_number();

   // Control sentence (if debug)

   #ifdef _DEBUG 

   if(index >= inputs_number)
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: ScalingLayer class.\n"
             << "void set_mean(const unsigned int&, const double&) method.\n"
             << "Index of input variable must be less than number of scaling neurons.\n";

	  throw std::logic_error(buffer.str());
   }

   #endif

   // Set vector if necessary

   if(means.size() != inputs_number)
   {
      means.set(inputs_number, 0.0);
   }

   // Set means of single input variable

   means[index] = new_mean;
}


// void set_standard_deviations(const Vector<double>&) method

/// This method sets new standard deviation values for all the scaling neurons.
/// These values are used for scaling the inputs to the multilayer perceptron with the meand and standard deviation
/// method. 
/// @param new_standard_deviation New set of standard deviation values for the scaling neurons of 
/// the multilayer perceptron

void ScalingLayer::set_standard_deviations(const Vector<double>& new_standard_deviation)
{
   // Control sentence (if debug)

   #ifdef _DEBUG 

   const unsigned int inputs_number = count_scaling_neurons_number();

   if(new_standard_deviation.size() != inputs_number)
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: ScalingLayer class.\n"
             << "void set_standard_deviations(const Vector<double>&) method.\n"
             << "Size must be equal to number of scaling neurons.\n";

	  throw std::logic_error(buffer.str());
   }

   #endif

   // Set standard deviation of scaling neurons

   standard_deviations = new_standard_deviation;
}


// void set_standard_deviation(const unsigned int&, const double&) method

/// This method sets a new standard deviation value for a single input variable.
/// These values are used for scaling the inputs to the multilayer perceptron with the meand and standard 
/// deviation method. 
/// @param index Index of input variable.
/// @param new_standard_deviation New standard deviation value for that input variable.

void ScalingLayer::set_standard_deviation(const unsigned int& index, const double& new_standard_deviation)
{
   const unsigned int inputs_number = count_scaling_neurons_number();

   // Control sentence (if debug)

   #ifdef _DEBUG 

   if(index >= inputs_number)
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: ScalingLayer class.\n"
             << "void set_standard_deviation(const unsigned int&, const double&) method.\n"
             << "Index of input variable must be less than number of scaling neurons.\n";

	  throw std::logic_error(buffer.str());
   }

   #endif

   // Set vector if necessary

   if(standard_deviations.size() != inputs_number)
   {
      standard_deviations.set(inputs_number, 1.0);
   }

   // Set standard deviation of single input variable

   standard_deviations[index] = new_standard_deviation;
}


// void set_minimums_maximums(const Vector< Vector<double> >&) method

/// This method sets both the minimums and the maximum values of all the scaling neurons.
/// The format is a vector of two real vectors. 
/// The first element must contain the minimum values for the scaling neurons.
/// The second element must contain the maximum values for the scaling neurons.
/// These values are used for scaling variables with the minimums and maximums method. 
/// @param new_minimums_maximums 
/// New set of minimums and maximum values for the scaling neurons of the layer. 

void ScalingLayer::set_minimums_maximums(const Vector< Vector<double> >& new_minimums_maximums)
{
   // Control sentence (if debug)

   #ifdef _DEBUG 

   std::ostringstream buffer;

   const unsigned int scaling_neurons_number = count_scaling_neurons_number();

   if(new_minimums_maximums.size() != 2)
   {
      buffer << "OpenNN Exception: ScalingLayer class.\n" 
             << "void set_minimums_maximums(const Vector< Vector<double> >&) method.\n"
             << "Size of minimums maximums vector must be 2.\n";

	  throw std::logic_error(buffer.str());
   }

   if(new_minimums_maximums[0].size() != scaling_neurons_number)
   {
      buffer << "OpenNN Exception: ScalingLayer class.\n" 
             << "void set_minimum_maximum(const Vector< Vector<double> >&) method.\n"
             << "Size of minimums (" << new_minimums_maximums[0].size() << ") must be equal to number of scaling neurons (" << scaling_neurons_number << ").\n";

	  throw std::logic_error(buffer.str());
   }

   if(new_minimums_maximums[1].size() != scaling_neurons_number)
   {
      buffer << "OpenNN Exception: ScalingLayer class.\n" 
             << "void set_minimum_maximum(const Vector< Vector<double> >&) method.\n"
             << "Size of maximums " << new_minimums_maximums[1].size() << " must be equal to number of scaling neurons (" << scaling_neurons_number << ").\n";

	  throw std::logic_error(buffer.str());
   }

   // Check that minimums of scaling neurons is not greater than their maximums

   for(unsigned int i = 0; i < scaling_neurons_number; i++)
   {
      if(new_minimums_maximums[0][i] >= new_minimums_maximums[1][i])
      {
         buffer << "OpenNN Exception: ScalingLayer class.\n"
                << "void set_minimum_maximum(const Vector< Vector<double> >&) method.\n"
                << "Minimum of variable " << i << " is equal or greater than maximum of that variable.\n";

   	     throw std::logic_error(buffer.str());
      }
   }

   #endif

   // Set minimums and maximums of scaling neurons

   minimums = new_minimums_maximums[0];
   maximums = new_minimums_maximums[1];
}


// void set_means_standard_deviations(const Vector< Vector<double> >&) method

/// This method sets both the mean and the standard deviation values of all the scaling neurons.
/// The format is a vector of two real vectors.
/// The first element must contain the mean values for the scaling neurons.
/// The second elements must contain the standard deviation values for the scaling neurons.
/// These values are used for scaling variables with the meand and standard deviation method. 
/// @param new_means_standard_deviations New set of mean and standard deviation values for the scaling neurons. 

void ScalingLayer::set_means_standard_deviations(const Vector< Vector<double> >& new_means_standard_deviations)
{
   // Control sentence (if debug)

   #ifdef _DEBUG 

   const unsigned int inputs_number = count_scaling_neurons_number();

   if(new_means_standard_deviations.size() != 2)
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: ScalingLayer class.\n"
             << "void set_means_standard_deviations(const Vector< Vector<double> >&) method.\n"
             << "Size of vector must be 2.\n";

	  throw std::logic_error(buffer.str());
   }
   else if(new_means_standard_deviations[0].size() != inputs_number && new_means_standard_deviations[1].size() != inputs_number)
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: ScalingLayer class.\n"
             << "void set_means_standard_deviations(const Vector< Vector<double> >&) method.\n"
             << "Number of columns must be equal to number of scaling neurons.\n";

	  throw std::logic_error(buffer.str());
   }

   // Check that standard deviation of scaling neurons is not zero

   for(unsigned int i = 0; i < inputs_number; i++)
   {
      if(new_means_standard_deviations[1][i] < 1.0e-99)
      {
         std::ostringstream buffer;

         buffer << "OpenNN Exception: ScalingLayer class: \n"
                << "void set_means_standard_deviations(const Vector< Vector<double> >&) method.\n"
                << "Standard deviation of scaling neuron " << i << " is zero.\n";

   	     throw std::logic_error(buffer.str());
      }
   }

   #endif

   // Set means and standard deviation of scaling neurons

   means = new_means_standard_deviations[0];
   standard_deviations = new_means_standard_deviations[1];
}


// void set_statistics(const Vector< Vector<double> >&) method

/// This method sets all the scaling layer statistics.
/// The format is a vector of four real vectors: 
/// <ul>
/// <li> Minimum of scaling neurons.
/// <li> Maximum of scaling neurons.
/// <li> Mean of scaling neurons.
/// <li> Standard deviation of scaling neurons.
/// </ul>
/// @param new_statistics Scaling layer statistics. 

void ScalingLayer::set_statistics(const Vector< Vector<double> >& new_statistics)
{
   // Control sentence (if debug)

   #ifdef _DEBUG 

   const unsigned int new_statistics_size = new_statistics.size();

   const unsigned int inputs_number = count_scaling_neurons_number();

   if(new_statistics_size != 4)
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: ScalingLayer class.\n"
             << "void set_statistics(const Vector< Vector<double> >&) method.\n"
             << "Size must be 4.\n";

	  throw std::logic_error(buffer.str());
   }

   unsigned int size;

   for(unsigned int i = 0; i < 4; i++)
   {
      size = new_statistics[i].size();

	  if(size != inputs_number)
	  {
         std::ostringstream buffer;

         buffer << "OpenNN Exception: ScalingLayer class.\n"
                << "void set_statistics(const Vector< Vector<double> >&) method.\n"
                << "Size of element " << i << " must be equal to number of scaling neurons.\n";

	     throw std::logic_error(buffer.str());
	  }
   }

   #endif

   // Set all statistics
 
   set_means(new_statistics[0]);
   set_standard_deviations(new_statistics[1]);
   set_minimums(new_statistics[2]);
   set_maximums(new_statistics[3]);
}


// void set_method(const Method&)

/// This method sets the method to be used for scaling the variables.
/// @param new_scaling_method New scaling method for the variables.

void ScalingLayer::set_scaling_method(const ScalingLayer::ScalingMethod& new_scaling_method)
{
   scaling_method = new_scaling_method;
}


// void set_scaling_method(const std::string&) method

/// This method sets the method to be used for scaling the variables.  
/// The argument is a string containing the name of the method ("None", "MeanStandardDeviation" or "MinimumMaximum").
/// @param new_scaling_method New scaling method in the layer. 

void ScalingLayer::set_scaling_method(const std::string& new_scaling_method)
{
   if(new_scaling_method == "MeanStandardDeviation")
   {
      set_scaling_method(MeanStandardDeviation);
   }
   else if(new_scaling_method == "MinimumMaximum")
   {
      set_scaling_method(MinimumMaximum);
   }
   else
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: ScalingLayer class.\n"
             << "void set_scaling_method(const std::string&) method.\n"
			 << "Unknown scaling method: " << new_scaling_method << ".\n";

	  throw std::logic_error(buffer.str());
   }
}


// void set_display(const bool&) method

/// This method sets a new display value. 
/// If it is set to true messages from this class are to be displayed on the screen;
/// if it is set to false messages from this class are not to be displayed on the screen.
/// @param new_display Display value.

void ScalingLayer::set_display(const bool& new_display)
{
   display = new_display;
}


// bool is_empty(void) const method

/// This method returns true if the number of scaling neurons is zero, and false otherwise. 

bool ScalingLayer::is_empty(void) const
{
   const unsigned int inputs_number = count_scaling_neurons_number(); 

   if(inputs_number == 0)
   {
      return(true);
   }
   else
   {
      return(false);
   }
}


// void check_range(const Vector<double>&) const method

/// This method chechs whether the inputs to the scaling layer have the right size. 
/// If not, it displays an error message and exits the program. 
/// It also checks whether the input values are inside the range defined by the minimums and maximum values, and 
/// displays a warning message if they are outside.
/// @param inputs Set of inputs to the scaling layer.

void ScalingLayer::check_range(const Vector<double>& inputs) const
{
   const unsigned int inputs_number = count_scaling_neurons_number();

   // Control sentence (if debug)

   #ifdef _DEBUG 

   const unsigned int size = inputs.size();

   if(size != inputs_number) 
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: ScalingLayer class.\n"
             << "void check_range(const Vector<double>&) const method.\n"
             << "Size of inputs must be equal to number of inputs.\n";

	  throw std::logic_error(buffer.str());
   }   

   #endif

   // Check inputs

   if(display)
   {
      if(minimums.size() != 0)
	  {
         for(unsigned int i = 0; i < inputs_number; i++)
         {
            if(inputs[i] < minimums[i])
            {
               std::cout << "OpenNN Warning: ScalingLayer class.\n"
                         << "void check_range(const Vector<double>&) const method.\n"
                         << "Input variable " << i << " is less than minimums.\n";                  
            }
		 }
	  }
      if(maximums.size() != 0)
	  {
         for(unsigned int i = 0; i < inputs_number; i++)
         {
            if(inputs[i] > maximums[i])
            {
               std::cout << "OpenNN Warning: ScalingLayer class.\n"
                         << "void check_range(const Vector<double>&) const method.\n"
                         << "Input variable " << i << " is greater than maximums.\n";
            }
         }
      }
   }
}


// void initialize_random(void) method

/// @todo

void ScalingLayer::initialize_random(void)
{

}


// Vector<double> calculate_outputs(const Vector<double>&) const method

/// This method scales some values to produce some scaled values. 
/// @param inputs Set of inputs to the scaling layer.

Vector<double> ScalingLayer::calculate_outputs(const Vector<double>& inputs) const
{
   // Control sentence (if debug)

   #ifdef _DEBUG 

   std::ostringstream buffer;

   const unsigned int inputs_number = count_scaling_neurons_number();

   const unsigned int size = inputs.size();

   if(size != inputs_number) 
   {
      buffer << "OpenNN Exception: ScalingLayer class.\n"
             << "Vector<double> calculate_outputs(const Vector<double>&) const method.\n"
             << "Size of inputs must be equal to number of scaling neurons.\n";

	  throw std::logic_error(buffer.str());
   }   

   #endif

   switch(scaling_method)
   {
      case MinimumMaximum:
      {
         return(calculate_minimum_maximum_output(inputs));
      }            
      break;

      case MeanStandardDeviation:
      {
         return(calculate_mean_standard_deviation_output(inputs));
      }
      break;

      default:
      {
         std::ostringstream buffer;

         buffer << "OpenNN Exception: ScalingLayer class\n"
                << "Vector<double> calculate_outputs(const Vector<double>&) const method.\n"               
                << "Unknown scaling and unscaling method.\n";

	     throw std::logic_error(buffer.str());
      }
      break;
   }
}  


// Vector<double> calculate_derivative(const Vector<double>&) const method

/// This method retuns the derivatives of the scaled inputs with respect to the raw inputs.
/// That derivatives depend on the inputs scaling method to be used. 

Vector<double> ScalingLayer::calculate_derivative(const Vector<double>& dummy) const
{
   switch(scaling_method)
   {
      case MinimumMaximum:
      {
         return(calculate_minimum_maximum_derivative(dummy));
      }// end minimums and maximums
      break;       

      case MeanStandardDeviation:
      {
         return(calculate_mean_standard_deviation_derivative(dummy));
      }// end means and standard deviation
      break;

      default:
      {
         std::ostringstream buffer;

         buffer << "OpenNN Exception: ScalingLayer class.\n"
                << "Vector<double> calculate_derivative(const Vector<double>&) const method.\n"               
                << "Unknown scaling and unscaling method.\n";
 
  	     throw std::logic_error(buffer.str());
      }// end default
      break;

   }// end switch
}


// Vector<double> calculate_second_derivative(const Vector<double>&) const method

/// This method retuns the second derivatives of the scaled inputs with respect to the raw inputs.
/// That second derivatives depend on the inputs scaling method to be used. 

Vector<double> ScalingLayer::calculate_second_derivative(const Vector<double>& dummy) const
{
   switch(scaling_method)
   {
      case MinimumMaximum:
      {
         return(calculate_minimum_maximum_second_derivative(dummy));
      }// end minimums and maximums
      break;       

      case MeanStandardDeviation:
      {
         return(calculate_mean_standard_deviation_second_derivative(dummy));
      }// end means and standard deviation
      break;

      default:
      {
         std::ostringstream buffer;

         buffer << "OpenNN Exception: ScalingLayer class.\n"
                << "Vector<double> calculate_second_derivative(const Vector<double>&) const method.\n"               
                << "Unknown scaling and unscaling method.\n";
 
	     throw std::logic_error(buffer.str());
      }// end default
      break;

   }// end switch
}


// Vector<double> calculate_minimum_maximum_output(const Vector<double>&) const method

/// This method calculates the outputs from the scaling layer with the minimum and maximum method for a set of inputs.
/// @param inputs Vector of input values to the scaling layer. The size must be equal to the number of scaling neurons. 

Vector<double> ScalingLayer::calculate_minimum_maximum_output(const Vector<double>& inputs) const
{
   const unsigned int scaling_neurons_number = count_scaling_neurons_number();

   Vector<double> outputs(scaling_neurons_number);

   for(unsigned int i = 0; i < scaling_neurons_number; i++)
   {
      if(maximums[i]-minimums[i] < 1e-99)
      {      
         if(display)
         {
            std::cout << "OpenNN Warning: ScalingLayer class\n"
                      << "Vector<double> calculate_minimum_maximum_output(Vector<double>&) const method.\n"               
                      << "Minimum and maximum values of variable " << i << " are equal.\n"
                      << "Those inputs won't be scaled.\n";
         }
               
         outputs[i] = inputs[i];
      }      
      else
      {             
         outputs[i] = 2.0*(inputs[i] - minimums[i])/(maximums[i]-minimums[i]) - 1.0;
      }
   }

    return(outputs);
}


// Vector<double> calculate_minimum_maximum_derivative(const Vector<double>&) const method

/// This method calculates the derivatives of the outputs from the scaling layer with the minimum and maximum method.
/// As the minimum and maximum method is a linear method, the derivatives will not depend on the inputs. 

Vector<double> ScalingLayer::calculate_minimum_maximum_derivative(const Vector<double>&) const
{
   const unsigned int scaling_neurons_number = count_scaling_neurons_number();

   Vector<double> scaled_derivative(scaling_neurons_number);

   for(unsigned int i = 0; i < scaling_neurons_number; i++)
   {
      if(maximums[i]-minimums[i] < 1e-99)
      {      
         if(display)
         {
            std::cout << "OpenNN Warning: ScalingLayer class.\n"
                      << "Vector<double> calculate_minimum_maximum_derivative(const Vector<double>&) const method.\n"               
                      << "Minimum and maximum values of variable " << i << " are equal.\n"
                      << "That inputs is not scaled.\n";
         }

         scaled_derivative[i] = 1.0;
      }      
      else
      {
         scaled_derivative[i] = 2.0/(maximums[i]-minimums[i]); 
      }
   }

   return(scaled_derivative);
}


// Vector<double> calculate_minimum_maximum_second_derivative(const Vector<double>&) const method

/// This method calculates the second derivatives of the outputs from the scaling layer with the minimum and maximum method.
/// As the minimum and maximum method is a linear method, the second derivatives will be always zero. 

Vector<double> ScalingLayer::calculate_minimum_maximum_second_derivative(const Vector<double>&) const
{
   const unsigned int scaling_neurons_number = count_scaling_neurons_number();

   const Vector<double> scaled_second_derivative(scaling_neurons_number, 0.0);

   return(scaled_second_derivative);
}


// Vector<double> calculate_mean_standard_deviation_output(const Vector<double>&) const method

/// This method calculates the outputs from the scaling layer with the mean and standard deviation method for a set of inputs.
/// @param inputs Vector of input values to the scaling layer. The size must be equal to the number of scaling neurons. 

Vector<double> ScalingLayer::calculate_mean_standard_deviation_output(const Vector<double>& inputs) const
{

   const unsigned int scaling_neurons_number = count_scaling_neurons_number();

   Vector<double> outputs(scaling_neurons_number);

   for(unsigned int i = 0; i < scaling_neurons_number; i++)
   {            
      if(standard_deviations[i] < 1e-99)
      {      
         if(display)
         {
            std::cout << "OpenNN Warning: ScalingLayer class.\n"
                      << "Vector<double> calculate_mean_standard_deviation_output(const Vector<double>&) const method.\n"
                      << "Standard deviation of variable " << i << " is zero.\n"
                      << "Those variables won't be scaled.\n";
         }
               
         outputs[i] = inputs[i];
      }      
      else
      {             
         outputs[i] = (inputs[i] - means[i])/standard_deviations[i];
      }
   }       

   return(outputs);
}


// Vector<double> calculate_mean_standard_deviation_derivative(const Vector<double>&) const method

/// This method calculates the derivatives of the outputs from the scaling layer with the mean and standard deviation method.
/// As the minimum and maximum method is a linear method, the derivatives will not depend on the inputs. 

Vector<double> ScalingLayer::calculate_mean_standard_deviation_derivative(const Vector<double>&) const
{
   const unsigned int scaling_neurons_number = count_scaling_neurons_number();

   Vector<double> derivative(scaling_neurons_number);

   for(unsigned int i = 0; i < scaling_neurons_number; i++)
   {
      if(standard_deviations[i] < 1e-99)
      {      
         if(display)
         {
            std::cout << "OpenNN Warning: ScalingLayer class.\n"
                      << "Vector<double> calculate_mean_standard_deviation_derivative(const Vector<double>&) const method.\n"               
                      << "Standard deviation of input variable " << i << " is zero.\n" 
                      << "That inputs is not be scaled.\n";
         }
               
         derivative[i] = 1.0;
      }      
      else
      {
         derivative[i] = 1.0/standard_deviations[i];
      }
   }

    return(derivative);
}


// Vector<double> calculate_mean_standard_deviation_second_derivative(const Vector<double>&) const method

/// This method calculates the second derivatives of the outputs from the scaling layer with the mean and standard deviation method.
/// As the minimum and maximum method is a linear method, the second derivatives will be always zero. 

Vector<double> ScalingLayer::calculate_mean_standard_deviation_second_derivative(const Vector<double>&) const
{
   const unsigned int scaling_neurons_number = count_scaling_neurons_number();

   const Vector<double> second_derivative(scaling_neurons_number, 0.0);

   return(second_derivative);
}


// Matrix<double> arrange_Jacobian(const Vector<double>&) const method

/// This method arranges a "Jacobian" matrix from the vector of derivatives. 

Matrix<double> ScalingLayer::arrange_Jacobian(const Vector<double>& derivative) const
{
   const unsigned int scaling_neurons_number = count_scaling_neurons_number();

   Matrix<double> Jacobian(scaling_neurons_number, scaling_neurons_number, 0.0);

   Jacobian.set_diagonal(derivative);

   return(Jacobian);
}


// Vector< Matrix<double> > arrange_Hessian_form(const Vector<double>&) const method

/// This method arranges a "Hessian form" vector of matrices from the vector of second derivatives. 

Vector< Matrix<double> > ScalingLayer::arrange_Hessian_form(const Vector<double>& second_derivative) const
{
   const unsigned int scaling_neurons_number = count_scaling_neurons_number();

   Vector< Matrix<double> > Hessian_form(scaling_neurons_number);

   for(unsigned int i = 0; i < scaling_neurons_number; i++)
   {
      Hessian_form[i].set(scaling_neurons_number, scaling_neurons_number, 0.0);

      Hessian_form[i][i][i] = second_derivative[i];
   }

   return(Hessian_form);
}


// std::string write_minimum_maximum_expression(const Vector<std::string>&, const Vector<std::string>&) const method

/// This method returns a string with the expression of the scaling process with the minimum and maximum method. 
/// @param inputs_name Name of inputs to the scaling layer. The size of this vector must be equal to the number of scaling neurons. 
/// @param outputs_name Name of outputs from the scaling layer. The size of this vector must be equal to the number of scaling neurons. 

std::string ScalingLayer::write_minimum_maximum_expression(const Vector<std::string>& inputs_name, const Vector<std::string>& outputs_name) const
{
   const unsigned int inputs_number = count_scaling_neurons_number();

   std::ostringstream buffer;

   for(unsigned int i = 0; i < inputs_number; i++)
   {
      buffer << outputs_name[i] << "=2*(" << inputs_name[i] << "-" << minimums[i] << ")/(" << maximums[i] << "-" << minimums[i] << ")-1;\n";
   }

   return(buffer.str());
}   


// std::string write_mean_standard_deviation_expression(const Vector<std::string>&, const Vector<std::string>&) const method

/// This method returns a string with the expression of the scaling process with the mean and standard deviation method. 
/// @param inputs_name Name of inputs to the scaling layer. The size of this vector must be equal to the number of scaling neurons. 
/// @param outputs_name Name of outputs from the scaling layer. The size of this vector must be equal to the number of scaling neurons. 

std::string ScalingLayer::write_mean_standard_deviation_expression(const Vector<std::string>& inputs_name, const Vector<std::string>& outputs_name) const
{
   const unsigned int inputs_number = count_scaling_neurons_number();

   std::ostringstream buffer;

   for(unsigned int i = 0; i < inputs_number; i++)
   {
      buffer << outputs_name[i] << "=(" << inputs_name[i] << "-" << means[i] << ")/" << standard_deviations[i] << ";\n";
   }

   return(buffer.str());
}


// std::string write_expression(const Vector<std::string>&, const Vector<std::string>&) const method

/// This method returns a string with the expression of the inputs scaling process. 

std::string ScalingLayer::write_expression(const Vector<std::string>& inputs_name, const Vector<std::string>& outputs_name) const
{
   std::ostringstream buffer;

   switch(scaling_method)
   { 
      case MinimumMaximum:
      {
         return(write_minimum_maximum_expression(inputs_name, outputs_name));
      }
      break;

      case MeanStandardDeviation:
      {
         return(write_mean_standard_deviation_expression(inputs_name, outputs_name));
      }
      break;
   
      default:
      {
         std::ostringstream buffer;

         buffer << "OpenNN Exception: ScalingLayer class.\n"
                << "std::string write_expression(void) const method.\n"
                << "Unknown inputs scaling method.\n";
 
	     throw std::logic_error(buffer.str());
      }// end default
      break;
   }

//   return("");
}


// std::string to_string(void) const method

/// This method returns a string representation of the current scaling layer object. 

std::string ScalingLayer::to_string(void) const
{
   std::ostringstream buffer;

   buffer << "Scaling layer\n"
          << "Minimums: " << minimums << "\n"
          << "Maximums: " << maximums << "\n"
          << "Means: " << means << "\n"
          << "Standard deviations: " << standard_deviations << "\n"
          << "Scaling method: " << write_scaling_method() << "\n"
          << "Display: " << display << "\n";

   return(buffer.str());
}


// TiXmlElement* to_XML(void) const method

/// This method serializes the scaling layer object into a XML element of the TinyXML library. 
/// See the OpenNN manual for more information about the format of this element. 

TiXmlElement* ScalingLayer::to_XML(void) const
{
   std::ostringstream buffer;

   TiXmlElement* scaling_layer_element = new TiXmlElement("ScalingLayer");
   scaling_layer_element->SetAttribute("Version", 4);

   // Minimums

   TiXmlElement* minimums_element = new TiXmlElement("Minimums");
   scaling_layer_element->LinkEndChild(minimums_element);

   buffer.str("");
   buffer << minimums;

   TiXmlText* minimums_text = new TiXmlText(buffer.str().c_str());
   minimums_element->LinkEndChild(minimums_text);

   // Maximums

   TiXmlElement* maximums_element = new TiXmlElement("Maximums");
   scaling_layer_element->LinkEndChild(maximums_element);

   buffer.str("");
   buffer << maximums;

   TiXmlText* maximums_text = new TiXmlText(buffer.str().c_str());
   maximums_element->LinkEndChild(maximums_text);

   // Means

   TiXmlElement* means_element = new TiXmlElement("Means");
   scaling_layer_element->LinkEndChild(means_element);

   buffer.str("");
   buffer << means;

   TiXmlText* means_text = new TiXmlText(buffer.str().c_str());
   means_element->LinkEndChild(means_text);

   // Standard deviations

   TiXmlElement* standard_deviations_element = new TiXmlElement("StandardDeviations");
   scaling_layer_element->LinkEndChild(standard_deviations_element);

   buffer.str("");
   buffer << standard_deviations;

   TiXmlText* standard_deviations_text = new TiXmlText(buffer.str().c_str());
   standard_deviations_element->LinkEndChild(standard_deviations_text);

   // Scaling method

   TiXmlElement* method_element = new TiXmlElement("ScalingMethod");
   scaling_layer_element->LinkEndChild(method_element);

   TiXmlText* method_text = new TiXmlText(write_scaling_method().c_str());
   method_element->LinkEndChild(method_text);

   // Display warnings

   TiXmlElement* display_element = new TiXmlElement("Display");
   scaling_layer_element->LinkEndChild(display_element);

   buffer.str("");
   buffer << display;

   TiXmlText* display_text = new TiXmlText(buffer.str().c_str());
   display_element->LinkEndChild(display_text);

   return(scaling_layer_element);
}


// void from_XML(TiXmlElement*) method

/// This method deserializes a TinyXML element into this scaling layer object. 
/// @param scaling_layer_element Pointer to a XML element containing the member data. 

void ScalingLayer::from_XML(TiXmlElement* scaling_layer_element)
{
   if(scaling_layer_element)
   {
      // Scaling method
      {
         TiXmlElement* scaling_method_element = scaling_layer_element->FirstChildElement("ScalingMethod");

         if(scaling_method_element)
         {
            std::string new_method = scaling_method_element->GetText();

            try
            {
               set_scaling_method(new_method);
            }
            catch(std::exception& e)
            {
               std::cout << e.what() << std::endl;		 
            }
         }
      }

      // Minimums
      {
         TiXmlElement* minimums_element = scaling_layer_element->FirstChildElement("Minimums");

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
         TiXmlElement* maximums_element = scaling_layer_element->FirstChildElement("Maximums");

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
         TiXmlElement* means_element = scaling_layer_element->FirstChildElement("Means");

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
         TiXmlElement* standard_deviations_element = scaling_layer_element->FirstChildElement("StandardDeviations");

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
         TiXmlElement* display_element = scaling_layer_element->FirstChildElement("Display");

         if(display)
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
