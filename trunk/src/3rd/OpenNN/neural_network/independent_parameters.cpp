/****************************************************************************************************************/
/*                                                                                                              */
/*   OpenNN: Open Neural Networks Library                                                                       */
/*   www.opennn.cimne.com                                                                                       */
/*                                                                                                              */
/*   I N D E P E N D E N T   P A R A M E T E R S   C L A S S                                                    */
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

#include "independent_parameters.h"

namespace OpenNN
{

// DEFAULT CONSTRUCTOR

/// Default constructor. 
/// It creates a independent parameters object with zero parameters.
/// This constructor also initializes the members of the object to their default values. 

IndependentParameters::IndependentParameters(void)
{
   set();
}


// INDEPENDENT PARAMETERS NUMBER CONSTRUCTOR

/// Independent parameters constructor. 
/// It creates a independent parameters object with a given number of parameters.
/// The independent parameters are initialized at random. 
/// This constructor also initializes the rest of class members to their default values.
/// @param new_parameters_number Number of independent parameters.

IndependentParameters::IndependentParameters(const unsigned int& new_parameters_number)
{
   set(new_parameters_number);
}



// COPY CONSTRUCTOR

/// Copy constructor. 
/// It creates a copy of an existing independent parameters object. 
/// @param other_independent_parameters Independent parameterse object to be copied.

IndependentParameters::IndependentParameters(const IndependentParameters& other_independent_parameters)
{
   set(other_independent_parameters);
}


// DESTRUCTOR

/// Destructor.
/// This destructor does not delete any pointer.

IndependentParameters::~IndependentParameters(void)
{
}


// ASSIGNMENT OPERATOR

/// Assignment operator. 
/// It assigns to this object the members of an existing independent parameters object.
/// @param other_independent_parameters Independent parameters object to be assigned.

IndependentParameters& IndependentParameters::operator = (const IndependentParameters& other_independent_parameters)
{
   if(this != &other_independent_parameters) 
   {
      parameters = other_independent_parameters.parameters;
      names = other_independent_parameters.names;
      units = other_independent_parameters.units;
      descriptions = other_independent_parameters.descriptions;
      minimums = other_independent_parameters.minimums;
      maximums = other_independent_parameters.maximums;
      means = other_independent_parameters.means;
      standard_deviations = other_independent_parameters.standard_deviations;
      lower_bounds = other_independent_parameters.lower_bounds;
      upper_bounds = other_independent_parameters.upper_bounds;
      scaling_method = other_independent_parameters.scaling_method;
      scaling_flag = other_independent_parameters.scaling_flag;
      bounding_flag = other_independent_parameters.bounding_flag;
      display_range_warning = other_independent_parameters.display_range_warning;
      display = other_independent_parameters.display;
   }

   return(*this);
}


// EQUAL TO OPERATOR

// bool operator == (const IndependentParameters&) const method

/// Equal to operator. 
/// It compares this object with another object of the same class. 
/// It returns true if the members of the two objects have the same values, and false otherwise.
/// @ param other_independent_parameters Independent parameters object to be compared with.

bool IndependentParameters::operator == (const IndependentParameters& other_independent_parameters) const
{
   if(parameters == other_independent_parameters.parameters
   && names == other_independent_parameters.names
   && units == other_independent_parameters.units
   && descriptions == other_independent_parameters.descriptions
   && minimums == other_independent_parameters.minimums
   && maximums == other_independent_parameters.maximums
   && means == other_independent_parameters.means
   && standard_deviations == other_independent_parameters.standard_deviations
   && lower_bounds == other_independent_parameters.lower_bounds
   && upper_bounds == other_independent_parameters.upper_bounds
   && scaling_method == other_independent_parameters.scaling_method
   && scaling_flag == other_independent_parameters.scaling_flag
   && bounding_flag == other_independent_parameters.bounding_flag
   && display_range_warning == other_independent_parameters.display_range_warning
   && display == other_independent_parameters.display)
   {
      return(true);
   }
   else
   {
      return(false);
   }
}


// METHODS

// const Vector<double> get_parameters(void) const method

/// This method returns the values of the independent parameters.

const Vector<double>& IndependentParameters::get_parameters(void) const
{   
   return(parameters);    
}


// double get_parameter(const unsigned int&) const method

/// This method returns the value of a single independent parameter.
/// @param index Index of independent parameter.

double IndependentParameters::get_parameter(const unsigned int& index) const
{
   // Control sentence (if debug)

   #ifdef _DEBUG 

   const unsigned int parameters_number = count_parameters_number();

   if(index >= parameters_number)
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: IndependentParameters class.\n" 
             << "double get_parameter(const unsigned int&) const method.\n"
             << "Index of independent parameter must be less than number of parameters.\n";

	  throw std::logic_error(buffer.str());
   }

   #endif

   return(parameters[index]);           
}


// const Vector<std::string>& get_names(void) const method

/// This method returns the names of the independent parameters. 
/// Such names are only used to give the user basic information about the problem at hand.

const Vector<std::string>& IndependentParameters::get_names(void) const
{
   return(names);    
}


// const std::string& get_name(const unsigned int&) const method

/// This method returns the name of a single independent parameter. 
/// Such name is only used to give the user basic information about the problem at hand.
/// @param index Index of independent parameter.

const std::string& IndependentParameters::get_name(const unsigned int& index) const
{
   // Control sentence (if debug)

   #ifdef _DEBUG 

   const unsigned int parameters_number = count_parameters_number();

   if(index >= parameters_number)
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: IndependentParameters class.\n" 
             << "const std::string& get_name(const unsigned int&) const method.\n"
             << "Index of independent parameter must be less than number of parameters.\n";

	  throw std::logic_error(buffer.str());
   }

   #endif

   return(names[index]);
}


// const Vector<std::string>& get_units(void) const method

/// This method returns the units of the independent parameters. 
/// Such units are only used to give the user basic information about the problem at hand.

const Vector<std::string>& IndependentParameters::get_units(void) const
{
   return(units);
}


// const std::string& get_unit(const unsigned int&) const method

/// This method returns the unit of a single independent parameter. 
/// Such units are only used to give the user basic information about the problem at hand.
/// @param index Index of independent parameter.

const std::string& IndependentParameters::get_unit(const unsigned int& index) const
{
   // Control sentence (if debug)

   #ifdef _DEBUG 

   const unsigned int parameters_number = count_parameters_number();

   if(index >= parameters_number)
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: IndependentParameters class.\n" 
             << "const std::string get_units(const unsigned int&) const method.\n"
             << "Index of independent parameter must be less than number of parameters.\n";

	  throw std::logic_error(buffer.str());
   }

   #endif

   return(units[index]);
}


// const Vector<std::string>& get_descriptions(void) const method

/// This method returns the descriptions of the independent parameters. 
/// Such descriptions are only used to give the user basic information about the problem at hand.

const Vector<std::string>& IndependentParameters::get_descriptions(void) const
{
   return(descriptions);
}


// const std::string& get_description(const unsigned int&) const method

/// This method returns the description of a single independent parameter. 
/// Such description is only used to give the user basic information about the problem at hand.
/// @param index Index of independent parameter.

const std::string& IndependentParameters::get_description(const unsigned int& index) const
{
   // Control sentence (if debug)

   #ifdef _DEBUG 

   const unsigned int parameters_number = count_parameters_number();

   if(index >= parameters_number)
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: IndependentParameters class.\n" 
             << "const std::string get_description(const unsigned int&) const method.\n"
             << "Index of independent parameter must be less than number of parameters.\n";

	  throw std::logic_error(buffer.str());
   }

   #endif

   return(descriptions[index]);
}


// Vector< Vector<std::string> > arrange_information(void) method

/// This method returns all the available information about the independent parameters. 
/// The format is a vector of subvectors of size three: 
/// <ul>
/// <li> Names of independent parameters.
/// <li> Units of independent parameters.
/// <li> Descriptions of independent parameters.
/// </ul>

Vector< Vector<std::string> > IndependentParameters::arrange_information(void) 
{
   Vector< Vector<std::string> > information(3);
 
   information[0] = names;
   information[1] = units;
   information[2] = descriptions;

   return(information);
}


// const Vector<double>& get_minimums(void) const method

/// This method returns the minimum values of all the independent parameters.
/// Such values are to be used for scaling and unscaling independent parameters with the minimum and maximum method. 

const Vector<double>& IndependentParameters::get_minimums(void) const
{
   return(minimums);
}


// double get_minimum(const unsigned int&) const method

/// This method returns the minimum value of a single independent parameter.
/// Such value is to be used for scaling and unscaling that independent parameter with the minimum and maximum method. 
/// @param index Index of independent parameter.

double IndependentParameters::get_minimum(const unsigned int& index) const
{
   // Control sentence (if debug)

   #ifdef _DEBUG 

   const unsigned int parameters_number = count_parameters_number();

   if(index >= parameters_number)
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: IndependentParameters class.\n" 
             << "double get_minimum(const unsigned int&) const method.\n"
             << "Index of independent parameter must be less than number of parameters.\n";

	  throw std::logic_error(buffer.str());
   }

   #endif

   return(minimums[index]);
}


// const Vector<double>& get_maximum(void) const method

/// This method returns the maximum values of all the independent parameters.
/// Such values are to be used for scaling and unscaling independent parameters with the minimum and maximum 
/// method. 

const Vector<double>& IndependentParameters::get_maximums(void) const
{
   return(maximums);              
}


// double get_maximum(const unsigned int&) const method

/// This method returns the maximum value of a single independent parameter.
/// Such value is to be used for scaling and unscaling that independent parameter with the minimum and maximum method. 
/// @param index Index of independent parameter.

double IndependentParameters::get_maximum(const unsigned int& index) const
{
   // Control sentence (if debug)

   #ifdef _DEBUG 

   const unsigned int parameters_number = count_parameters_number();

   if(index >= parameters_number)
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: IndependentParameters class.\n" 
             << "double get_maximum(const unsigned int&) const method.\n"
             << "Index must be less than number of parameters.\n";

	  throw std::logic_error(buffer.str());
   }

   #endif

   return(maximums[index]);
}


// const Vector<double>& get_means(void) const method

/// This method returns the mean values of all the independent parameters.
/// Such values are to be used for scaling and unscaling independent parameters with the mean and standard deviation method. 

const Vector<double>& IndependentParameters::get_means(void) const
{
   return(means);
}


// double get_mean(const unsigned int&) const method

/// This method returns the mean value of a single independent parameter.
/// Such a value is to be used for scaling and unscaling that parameter with the mean and standard deviation method. 
/// @param index Index of independent parameter.

double IndependentParameters::get_mean(const unsigned int& index) const
{
   // Control sentence (if debug)

   #ifdef _DEBUG 

   const unsigned int parameters_number = count_parameters_number();

   if(index >= parameters_number)
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: IndependentParameters class.\n" 
             << "double get_mean(const unsigned int&) const method.\n"
             << "Index must be less than number of parameters.\n";

	  throw std::logic_error(buffer.str());
   }

   #endif

   return(means[index]);
}


// const Vector<double>& get_standard_deviations(void) const method

/// This method returns the standard deviation values of all the independent parameters.
/// Such values are to be used for scaling and unscaling independent parameters with the mean and standard deviation method. 

const Vector<double>& IndependentParameters::get_standard_deviations(void) const
{
   return(standard_deviations);              
}


// double get_standard_deviation(const unsigned int&) const method

/// This method returns the standard deviation value of a single independent parameter.
/// Such a value is to be used for scaling and unscaling that parameter with the mean and standard deviation method. 
/// @param index Index of independent parameter.

double IndependentParameters::get_standard_deviation(const unsigned int& index) const
{
   // Control sentence (if debug)

   #ifdef _DEBUG 

   const unsigned int parameters_number = count_parameters_number();

   if(index >= parameters_number)
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: IndependentParameters class.\n" 
             << "double get_standard_deviation(const unsigned int&) const method.\n"
             << "Index must be less than number of parameters.\n";

	  throw std::logic_error(buffer.str());
   }

   #endif

   return(standard_deviations[index]);
}


// Vector< Vector<double> > arrange_minimum_maximum(void) method

/// This method returns the minimum and maximum values of all the independent parameters.
/// The format is a vector of two pointers to real vectors. 
/// The first element contains the minimum values of the independent parameters.
/// The second element contains the maximum values of the independent parameters.
/// Such values are to be used for scaling and unscaling independent parameters with the minimum and maximum method. 

Vector< Vector<double> > IndependentParameters::arrange_minimums_maximums(void)
{
   Vector< Vector<double> > minimums_maximums(2);

   minimums_maximums[0] = minimums;
   minimums_maximums[1] = maximums;

   return(minimums_maximums);
}


// Vector< Vector<double> > arrange_means_standard_deviations(void) method

/// This method returns the mean and the standard deviation values of all the independent parameters in a single matrix. 
/// The first row contains the mean values of the independent parameters.
/// The second row contains the standard deviation values of the independent parameters.
/// Such values are to be used for scaling and unscaling independent parameters with the mean and standard deviation method. 

Vector< Vector<double> > IndependentParameters::arrange_means_standard_deviations(void)
{
   Vector< Vector<double> > means_standard_deviations(2);

   means_standard_deviations[0] = means;
   means_standard_deviations[1] = standard_deviations;

   return(means_standard_deviations);
}


// Vector< Vector<double> > arrange_statistics(void) method

/// This method returns a vector of vectors with the basic statistics of the independent parameters 
/// (mean, standard deviation, minimum and maximum).

Vector< Vector<double> > IndependentParameters::arrange_statistics(void) 
{
   Vector< Vector<double> > statistics(4);

   statistics[0] = minimums;
   statistics[1] = minimums;
   statistics[2] = means;
   statistics[3] = standard_deviations;

   return(statistics);
}


// const ScalingMethod get_scaling_method(void) const method

/// This method returns the method used for scaling and unscaling the independent parameters.

const IndependentParameters::ScalingMethod& IndependentParameters::get_scaling_method(void) const
{
   return(scaling_method);
}


// std::string write_scaling_method(void) const method

/// This method returns a string with the method used for scaling and unscaling the independent parameters.

std::string IndependentParameters::write_scaling_method(void) const
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

      buffer << "OpenNN Exception: IndependentParameters class.\n"
             << "std::string write_scaling_method(void) const method.\n"
             << "Unknown scaling and unscaling method.\n";
 
	  throw std::logic_error(buffer.str());
   }
}


// const bool& get_scaling_flag(void) const method

/// This method returns the flag value for scaling and unscaling the independent parameters. 

const bool& IndependentParameters::get_scaling_flag(void) const
{
   return(scaling_flag);
}


// const Vector<double>& get_lower_bounds(void) const method

/// This method returns the lower bounds of all the independent parameters.
/// These values are used to postprocess the independent parameters so that they are not less than the lower bounds. 

const Vector<double>& IndependentParameters::get_lower_bounds(void) const
{
   return(lower_bounds);
}


// double get_lower_bound(const unsigned int&) const method

/// This method returns the lower bound of a single independent parameter.
/// These values are used to postprocess that independent parameter so that it is not less than the lower bound. 
/// @param index Index of independent parameter.

double IndependentParameters::get_lower_bound(const unsigned int& index) const
{
   // Control sentence (if debug)

   #ifdef _DEBUG 

   const unsigned int parameters_number = count_parameters_number();

   if(index >= parameters_number)
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: IndependentParameters class.\n" 
             << "double get_lower_bound(const unsigned int&) const method.\n"
             << "Index must be less than number of parameters.\n";

	  throw std::logic_error(buffer.str());
   }

   #endif

   return(lower_bounds[index]);
}


// const Vector<double>& get_upper_bounds(void) const method

/// This method returns the upper bounds of all the independent parameters.
/// These values are used to postprocess the independent parameters so that they are not greater than the upper bounds. 

const Vector<double>& IndependentParameters::get_upper_bounds(void) const
{
   return(upper_bounds);
}


// double get_upper_bound(const unsigned int&) const method

/// This method returns the upper bound of a single independent parameter.
/// These values are used to postprocess that independent parameter so that it is not greater than the upper bound. 
/// @param index Index of independent parameter.

double IndependentParameters::get_upper_bound(const unsigned int& index) const
{
   // Control sentence (if debug)

   #ifdef _DEBUG 

   const unsigned int parameters_number = count_parameters_number();

   if(index >= parameters_number)
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: IndependentParameters class.\n" 
             << "double get_upper_bound(const unsigned int&) const method.\n"
             << "Index must be less than number of parameters.\n";

	  throw std::logic_error(buffer.str());
   }

   #endif

   return(upper_bounds[index]);
}


// Vector< Vector<double>* > get_bounds(void) method

/// This method returns the lower and upper bounds of all the independent parameters.
/// The format is a vector of two pointers to real vectors.  
/// The first element contains the lower bounds of the independent parameters.
/// The second element contains the upper bounds of the independent parameters.
/// These values are used to postprocess the independent parameters so that they are neither less than the lower 
/// bounds nor greater than the upper bounds. 

Vector< Vector<double>* > IndependentParameters::get_bounds(void)
{
   Vector< Vector<double>* > bounds(2);

   bounds[0] = &lower_bounds;
   bounds[1] = &upper_bounds;

   return(bounds);
}


// bool get_bounding_flag(void) const method

/// This method return the scaling flag for applying lower and upper bounds to the independent parameters. 

bool IndependentParameters::get_bounding_flag(void) const
{
   return(bounding_flag);
}


// const bool& get_display(void) const method

/// This method returns true if messages from this class are to be displayed on the screen, or false if messages 
/// from this class are not to be displayed on the screen.

const bool& IndependentParameters::get_display(void) const
{
   return(display);
}


// void set(void) method

/// This method sets the number of independent parameters to be zero. 
/// It also sets the rest of members to their default values.

void IndependentParameters::set(void)
{
   set_parameters_number(0);

   set_default();
}


// void set(const unsigned int&) method

/// This method sets a new number of independent parameters. 
/// It also sets the rest of members to their default values.
/// @param new_parameters_number Number of independent parameters. 

void IndependentParameters::set(const unsigned int& new_parameters_number)
{
   set_parameters_number(new_parameters_number);

   set_default();
}


// void set(const Vector<double>&) method

/// This method sets new independent parameters. 
/// It also sets the rest of members to their default values.
/// @param new_parameters Vector of parameters. 

void IndependentParameters::set(const Vector<double>& new_parameters)
{
   const unsigned int new_parameters_number = new_parameters.size();

   set_parameters_number(new_parameters_number);
   
   parameters = new_parameters;

   set_default();
}


// void set(const IndependentParameters&) method

/// This method sets the members of this object to be the members of another object of the same class. 
/// @param other_independent_parameters Object to be copied. 

void IndependentParameters::set(const IndependentParameters& other_independent_parameters)
{
   parameters = other_independent_parameters.parameters;

   names = other_independent_parameters.names;

   units = other_independent_parameters.units;

   descriptions = other_independent_parameters.descriptions;
 
   minimums = other_independent_parameters.minimums;

   maximums = other_independent_parameters.maximums;

   means = other_independent_parameters.means;

   standard_deviations = other_independent_parameters.standard_deviations;

   lower_bounds = other_independent_parameters.lower_bounds;

   upper_bounds = other_independent_parameters.upper_bounds;

   scaling_method = other_independent_parameters.scaling_method;

   scaling_flag = other_independent_parameters.scaling_flag;

   bounding_flag = other_independent_parameters.bounding_flag;

   display_range_warning = other_independent_parameters.display_range_warning;

   display = other_independent_parameters.display;
}


// void set_default(void) method

/// This method sets the members of this object to their default values. 

void IndependentParameters::set_default(void)
{
   set_scaling_method(MinimumMaximum);

   scaling_flag = false;

   bounding_flag = false;

   set_display(true);
}


// void set_parameters_number(const unsigned int&) method

/// This method sets a new number of independent parameters.
/// @param new_parameters_number Number of independent parameters.

void IndependentParameters::set_parameters_number(const unsigned int& new_parameters_number)
{
   parameters.set(new_parameters_number, 0.0);

   names.set(new_parameters_number);

   units.set(new_parameters_number);

   descriptions.set(new_parameters_number);

   minimums.set(new_parameters_number, -1.0);

   maximums.set(new_parameters_number, 1.0);

   means.set(new_parameters_number, 0.0);

   standard_deviations.set(new_parameters_number, 1.0);

   lower_bounds.set(new_parameters_number, -1.0e99);

   upper_bounds.set(new_parameters_number, 1.0e99);
}


// void set_parameters(const Vector<double>&) method

/// This method sets new values for all the independent parameters.
/// @param new_parameters Independent parameters values.

void IndependentParameters::set_parameters(const Vector<double>& new_parameters)
{
   // Control sentence (if debug)

   #ifdef _DEBUG 

   const unsigned int parameters_number = count_parameters_number();

   if(new_parameters.size() != parameters_number)
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: IndependentParameters class.\n"
             << "void set_parameters(const Vector<double>&) method.\n"
             << "Parameters size must be equal to number of parameters.\n";

	  throw std::logic_error(buffer.str());
   }

   #endif

   parameters = new_parameters;     

   if(bounding_flag)
   {
      bound_parameters();
   }
}


// void set_parameter(const unsigned int&, const double&) method

/// This method sets a new value for a single independent parameter.
/// @param index Index of independent parameter.
/// @param new_parameter Independent parameter value.

void IndependentParameters::set_parameter(const unsigned int& index, const double& new_parameter)
{
   // Control sentence (if debug)

   #ifdef _DEBUG 

   const unsigned int parameters_number = count_parameters_number();

   if(index >= parameters_number)
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: IndependentParameters class.\n"
             << "void set_parameter(const unsigned int&, const double&) method.\n"
             << "Index must be less than number of parameters.\n";

	  throw std::logic_error(buffer.str());
   }

   #endif

   parameters[index] = new_parameter;

   bound_parameter(index);
}


// void set_names(const Vector<std::string>&) method

/// This method sets new names for the independent parameters.
/// Such values are only used to give the user basic information on the problem at hand.
/// @param new_names New names for the independent parameters.

void IndependentParameters::set_names(const Vector<std::string>& new_names)
{
   // Control sentence (if debug)

   #ifdef _DEBUG 

   const unsigned int parameters_number = count_parameters_number();

   if(new_names.size() != parameters_number)
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: IndependentParameters class.\n"
             << "void set_name(const Vector<std::string>&) method.\n"
             << "Size of names must be equal to number of parameters.\n";

	  throw std::logic_error(buffer.str());
   }

   #endif

   // Set name of independent parameters

   names = new_names;
}


// void set_name(const unsigned int&, const std::string&) method

/// This method sets a new name for a single independent parameter.
/// Such a value is only used to give the user basic information on the problem at hand.
/// @param index Index of independent parameter.
/// @param new_name New name for the independent parameter of index i.

void IndependentParameters::set_name(const unsigned int& index, const std::string& new_name)
{
   // Control sentence (if debug)

   const unsigned int parameters_number = count_parameters_number();

   #ifdef _DEBUG 

   if(index >= parameters_number)
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: IndependentParameters class.\n"
             << "void set_name(const unsigned int&, const std::string&) method.\n"
             << "Index must be less than number of parameters.\n";

	  throw std::logic_error(buffer.str());
   }

   #endif

   if(names.size() != parameters_number)
   {
      names.set(parameters_number);
   }

   // Set name of single independent parameter

   names[index] = new_name;
}


// void set_units(const Vector<std::string>&)

/// This method sets new units for the independent parameters.
/// Such values are only used to give the user basic information on the problem at hand.
/// @param new_units New units for the independent parameters.

void IndependentParameters::set_units(const Vector<std::string>& new_units)
{
   // Control sentence (if debug)

   #ifdef _DEBUG 

   const unsigned int parameters_number = count_parameters_number();

   if(new_units.size() != parameters_number)
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: IndependentParameters class.\n" 
             << "void set_units(const Vector<std::string>&) method.\n"
             << "Size must be equal to number of parameters.\n";

	  throw std::logic_error(buffer.str());
   }

   #endif

   // Set units of independent parameters

   units = new_units;
}


// void set_unit(const unsigned int&, const std::string&) method

/// This method sets new units for a single independent parameter.
/// Such a value is only used to give the user basic information on the problem at hand.
/// @param index Index of independent parameter.
/// @param new_unit New unit for the independent parameter with the previous index. 

void IndependentParameters::set_unit(const unsigned int& index, const std::string& new_unit)
{
   const unsigned int parameters_number = count_parameters_number();

   // Control sentence (if debug)

   #ifdef _DEBUG 

   if(index >= parameters_number)
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: IndependentParameters class.\n"
             << "void set_unit(const unsigned int&, const std::string&) method.\n"
             << "Index of independent parameter must be less than number of parameters.\n";

	  throw std::logic_error(buffer.str());
   }

   #endif

   if(units.size() == 0)
   {
      units.set(parameters_number);
   }

   // Set units of single independent parameter

   units[index] = new_unit;
}


// void set_descriptions(const Vector<std::string>&) method

/// This method sets new descriptions for the independent parameters.
/// Such values are only used to give the user basic information on the problem at hand.
/// @param new_descriptions New description for the independent parameters.

void IndependentParameters::set_descriptions(const Vector<std::string>& new_descriptions)
{
   // Control sentence (if debug)

   #ifdef _DEBUG 

   const unsigned int parameters_number = count_parameters_number();

   if(new_descriptions.size() != parameters_number)
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: IndependentParameters class.\n" 
             << "void set_descriptions(const Vector<std::string>&) method.\n"
             << "Size of descriptions must be equal to number of parameters.\n";

	  throw std::logic_error(buffer.str());
   }

   #endif

   descriptions = new_descriptions;
}


// void set_description(const unsigned int&, const std::string&) method

/// This method sets a new description for a single independent parameter.
/// Such a value is only used to give the user basic information on the problem at hand.
/// @param index Index of independent parameter.
/// @param new_description New description for the independent parameter with the previous index. 

void IndependentParameters::set_description(const unsigned int& index, const std::string& new_description)
{
   const unsigned int parameters_number = count_parameters_number();

   // Control sentence (if debug)

   #ifdef _DEBUG 

   if(index >= parameters_number)
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: IndependentParameters class.\n"
             << "void set_description(const unsigned int&, const std::string&) method.\n"
             << "Index must be less than number of parameters.\n";

	  throw std::logic_error(buffer.str());
   }

   #endif

   if(descriptions.size() == 0)
   {
      descriptions.set(parameters_number);
   }

   // Set description of single independent parameter
   
   descriptions[index] = new_description;
}


// void set_minimum(const Vector<double>&) method

/// This method sets the minimum values of all the independent parameters.
/// These values are used for scaling and unscaling the independent parameters with the minimum and maximum method.
/// @param new_minimums New set of minimum values for the independent parameters.

void IndependentParameters::set_minimums(const Vector<double>& new_minimums)
{
   // Control sentence (if debug)

   #ifdef _DEBUG 

   const unsigned int parameters_number = count_parameters_number();

   if(new_minimums.size() != parameters_number)
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: IndependentParameters class.\n"
             << "void set_minimums(const Vector<double>&) method.\n"
             << "Size of minimums must be equal to number of parameters.\n";

	  throw std::logic_error(buffer.str());
   }

   #endif

   // Set minimum of independent parameters

   minimums = new_minimums;                                                   
}


// void set_minimum(const unsigned int&, const double&) method

/// This method sets a minimum value for a single independent parameter.
/// Such a value is used for scaling and unscaling that independent parameter with the minimum and maximum method.
/// @param index Index of independent parameter.
/// @param new_minimum New minimum value for the independent parameter of index i.

void IndependentParameters::set_minimum(const unsigned int& index, const double& new_minimum)
{
   const unsigned int parameters_number = count_parameters_number();

   // Control sentence (if debug)

   #ifdef _DEBUG 

   if(index >= parameters_number)
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: IndependentParameters class.\n"
             << "void set_minimum(const unsigned int&, const double&) method.\n"
             << "Index of independent parameter must be less than number of parameters.\n";

	  throw std::logic_error(buffer.str());
   }

   #endif

   if(minimums.size() != parameters_number)
   {
      minimums.set(parameters_number, -1.0);
   }

   // Set minimum of single independent parameter

   minimums[index] = new_minimum;
}


// void set_maximum(const Vector<double>&) method

/// This method sets the maximum values of all the independent parameters.
/// These values are used for scaling and unscaling the independent parameters with the minimum and maximum method.
/// @param new_maximums New set of maximum values for the independent parameters.

void IndependentParameters::set_maximums(const Vector<double>& new_maximums)
{
   // Control sentence (if debug)

   #ifdef _DEBUG 

   const unsigned int parameters_number = count_parameters_number();

   if(new_maximums.size() != parameters_number)
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: IndependentParameters class.\n"
             << "void set_maximum(const Vector<double>&) method.\n"
             << "Size of maximums must be equal to number of parameters.\n";

	  throw std::logic_error(buffer.str());
   }

   #endif

   // Set maximum of independent parameters

   maximums = new_maximums;  
}


// void set_maximum(const unsigned int&, const double&) method

/// This method sets a maximum value for a single independent parameter.
/// Such a value is used for scaling and unscaling that independent parameter with the minimum and maximum method.
/// @param index Index of independent parameter.
/// @param new_maximum New maximum value for the independent parameter with the previous index.

void IndependentParameters::set_maximum(const unsigned int& index, const double& new_maximum)
{
   const unsigned int parameters_number = count_parameters_number();

   // Control sentence (if debug)

   #ifdef _DEBUG 

   if(index >= parameters_number)
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: IndependentParameters class.\n"
             << "void set_maximum(const unsigned& int, const double&) method.\n"
             << "Index must be less than number of parameters.\n";

	  throw std::logic_error(buffer.str());
   }

   #endif

   // Set maximum vector

   if(maximums.size() != parameters_number)
   {
      maximums.set(parameters_number, 1.0);
   }

   // Set maximum of single independent parameter

   maximums[index] = new_maximum;
}


// void set_mean(const Vector<double>&) method

/// This method sets the mean values of all the independent parameters.
/// These values are used for scaling and unscaling the independent parameters with the mean and standard deviation method. 
/// @param new_means New set of mean values for the independent parameters.

void IndependentParameters::set_means(const Vector<double>& new_means)
{
   // Control sentence (if debug)

   #ifdef _DEBUG 

   const unsigned int parameters_number = count_parameters_number();

   if(new_means.size() != parameters_number)
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: IndependentParameters class.\n" 
             << "void set_means(const Vector<double>&) method.\n"
             << "Size must be equal to number of parameters.\n";

	  throw std::logic_error(buffer.str());
   }

   #endif

   // Set mean of independent parameters

   means = new_means;                                                   
}


// void set_mean(const unsigned int&, const double&) method

/// This method sets a new mean value for a single independent parameter.
/// Such a value is used for scaling and unscaling the independent parameters with the mean and standard deviation method.
/// @param index Index of independent parameter.
/// @param new_mean New mean value for the independent parameter with the previous index.

void IndependentParameters::set_mean(const unsigned int& index, const double& new_mean)
{
   const unsigned int parameters_number = count_parameters_number();

   // Control sentence (if debug)

   #ifdef _DEBUG 

   if(index >= parameters_number)
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: IndependentParameters class.\n"
             << "void set_mean(const unsigned int&, const double&) method.\n"
             << "Index of must be less than number of parameters.\n";

	  throw std::logic_error(buffer.str());
   }

   #endif

   // Set independent parameters mean vector

   const unsigned int size = means.size();

   if(size != parameters_number)
   {
      means.set(parameters_number, 0.0);
   }

   // Set mean of single independent parameter

   means[index] = new_mean;
}


// void set_standard_deviations(const Vector<double>&) method

/// This method sets the standard deviation values of all the independent parameters.
/// These values are used for scaling and unscaling the independent parameters with the mean and standard deviation method. 
/// @param new_standard_deviations New set of standard deviation values for the independent parameters.

void IndependentParameters::set_standard_deviations(const Vector<double>& new_standard_deviations)
{
   // Control sentence (if debug)

   #ifdef _DEBUG 

   const unsigned int parameters_number = count_parameters_number();

   if(new_standard_deviations.size() != parameters_number)
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: IndependentParameters class.\n" 
             << "void set_standard_deviations(const Vector<double>&) method.\n"
             << "Size must be equal to number of parameters.\n";

	  throw std::logic_error(buffer.str());
   }

   #endif

   // Set standard deviation of independent parameters

   standard_deviations = new_standard_deviations;  
}


// void set_standard_deviation(const unsigned int&, const double&) method

/// This method sets a new standard deviation value for a single independent parameter.
/// Such a value is used for scaling and unscaling the independent parameters with the mean and standard deviation method.
/// @param index Index of independent parameter.
/// @param new_standard_deviation New standard deviation value for that independent parameter.

void IndependentParameters::set_standard_deviation(const unsigned int& index, const double& new_standard_deviation)
{
   const unsigned int parameters_number = count_parameters_number();

   // Control sentence (if debug)

   #ifdef _DEBUG 

   if(index >= parameters_number)
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: IndependentParameters class.\n"
             << "void set_standard_deviation(const unsigned int&, const double&) method.\n"
             << "Index must be less than number of parameters.\n";

	  throw std::logic_error(buffer.str());
   }

   #endif

   // Set independent parameters standard deviation vector

   const unsigned int size = standard_deviations.size();

   if(size != parameters_number)
   {
      standard_deviations.set(parameters_number, 1.0);
   }

   // Set standard deviation of single independent parameter

   standard_deviations[index] = new_standard_deviation;
}


// void set_minimum_maximum(const Vector< Vector<double> >&) method

/// This method sets both the minimum and the values of all the independent parameters.
/// The format is a vector of two real vectors.
/// The first element must contain the minimum values values for the independent parameters.
/// The second element must contain the maximum values for the independent parameters.
/// These values are used for scaling and unscaling the independent parameters with the minimum and maximum method.
/// @param new_minimums_maximums New set of minimum and maximum values for the independent parameters.

void IndependentParameters::set_minimums_maximums(const Vector< Vector<double> >& new_minimums_maximums)
{
   // Control sentence (if debug)

   #ifdef _DEBUG 

   const unsigned int parameters_number = count_parameters_number();

   if(new_minimums_maximums.size() != 2)
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: IndependentParameters class.\n" 
             << "void set_minimum_maximum(const Vector< Vector<double> >&) method.\n"
             << "Number of rows must be 2.\n";

	  throw std::logic_error(buffer.str());
   }
   else if(new_minimums_maximums[0].size() != parameters_number
        && new_minimums_maximums[1].size() != parameters_number)
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: IndependentParameters class.\n" 
             << "void set_minimum_maximum(const Vector< Vector<double> >&) method.\n"
             << "Number of columns must be equal to number of parameters.\n";

	  throw std::logic_error(buffer.str());
   }

   // Check that minimum of independent parameters is not greater than their maximum

   for(unsigned int i = 0; i < parameters_number; i++)
   {
      if(new_minimums_maximums[0][i] >= new_minimums_maximums[1][i])
      {
         std::ostringstream buffer;

         buffer << "OpenNN Exception: IndependentParameters class.\n"
                << "void set_minimums_maximums(const Vector< Vector<double> >&) method.\n"
                << "Minimum of parameter "<< i << " is equal or greater than maximum of that parameter.\n";

	     throw std::logic_error(buffer.str());
      }
   }

   #endif

   // Set minimum and maximum of independent parameters

   minimums = new_minimums_maximums[0];
   maximums = new_minimums_maximums[1];
}


// void set_means_standard_deviations(const Vector< Vector<double> >&) method

/// This method sets both the mean and the standard deviation values of all the independent parameters.
/// The format is a vector of two real vectors. 
/// The first element must contain the mean values values for the independent parameters.
/// The second element must contain the standard deviation values for the independent parameters.
/// These values are used for scaling and unscaling the independent parameters with the mean and standard deviation method. 
/// @param new_means_standard_deviations New set of mean and standard deviation values for the independent parameters.

void IndependentParameters::set_means_standard_deviations(const Vector< Vector<double> >& new_means_standard_deviations)
{
   // Control sentence (if debug)

   #ifdef _DEBUG 

   std::ostringstream buffer;

   const unsigned int parameters_number = count_parameters_number();

   const unsigned int size = new_means_standard_deviations.size();

   if(size != 2)
   {
      buffer << "OpenNN Exception: IndependentParameters class.\n"
             << "void set_means_standard_deviations(const Vector< Vector<double> >& ) method.\n"
             << "Number of rows must be 2.\n";

	  throw std::logic_error(buffer.str());
   }
   else if(new_means_standard_deviations[0].size() != parameters_number
	    && new_means_standard_deviations[1].size() != parameters_number)
   {
      buffer << "OpenNN Exception: IndependentParameters class.\n"
             << "void set_means_standard_deviations(const Vector< Vector<double> >& ) method.\n"
             << "Number of columns must be equal to number of parameters.\n";

	  throw std::logic_error(buffer.str());
   }

   // Check that standard deviation of independent parameters is not zero

   if(display)
   {
      for(unsigned int i = 0; i < parameters_number; i++)
      {
         if(new_means_standard_deviations[1][i] < 1.0e-99)
         {
            std::ostringstream buffer;

            buffer << "OpenNN Exception: IndependentParameters class: \n"
                   << "void set_means_standard_deviations(const Vector< Vector<double> >& ) method.\n"
                   << "Standard deviation of independent parameter " << i << " is zero.\n";
         }
      }
   }

   #endif

   // Set mean and standard deviation of independent parameters

   means = new_means_standard_deviations[0];
   standard_deviations = new_means_standard_deviations[1];
}


// void set_statistics(const Vector< Vector<double> >&) method

/// This method sets all the statistics of the independent parameters.
/// The format is a vector of four real vectors. 
/// <ul>
/// <li> Mean of independent parameters.
/// <li> Standard deviation of independent parameters.
/// <li> Minimum of independent parameters.
/// <li> Maximum of independent parameters.
/// </ul>
/// @param new_statistics New statistics values for the independent parameters.

void IndependentParameters::set_statistics(const Vector< Vector<double> >& new_statistics)
{
   // Control sentence (if debug)

   #ifdef _DEBUG 

   if(new_statistics.size() != 4)
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: IndependentParameters class.\n"
             << "void set_statistics(const Vector< Vector<double> >&) method.\n"
             << "Size must be 6.\n";

	  throw std::logic_error(buffer.str());
   }

   #endif

   // Set all statistics

   set_minimums(new_statistics[0]);
   set_maximums(new_statistics[1]);
 
   set_means(new_statistics[2]);
   set_standard_deviations(new_statistics[3]);

}


// void set_scaling_method(const ScalingMethod&) method

/// This method sets the method to be used for scaling and unscaling the independent parameters.
/// @param new_scaling_method New scaling and unscaling method for the independent parameters.

void IndependentParameters::set_scaling_method
(const IndependentParameters::ScalingMethod& new_scaling_method)
{
   scaling_method = new_scaling_method;
}


// void set_scaling_method(const std::string&) method

/// This method sets the method to be used for scaling and unscaling the independent parameters.
/// The argument is a string containing the name of the method ("None", "MeanStandardDeviation" or "MinimumMaximum").
/// @param new_scaling_method Scaling and unscaling method for the independent parameters.

void IndependentParameters::set_scaling_method(const std::string& new_scaling_method)
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

      buffer << "OpenNN Exception: IndependentParameters class.\n"
             << "void set_scaling_method(const std::string&) method.\n"
			 << "Unknown independent parameters scaling method: " << new_scaling_method << ".\n";

	  throw std::logic_error(buffer.str());
   }
}


// void set_scaling_flag(const bool&) method

/// This method sets the flag for scaling and unscaling the independent parameters. 
/// @param new_scaling_flag True if the independent parameters are to be scaled and unscaled, and false otherwise. 

void IndependentParameters::set_scaling_flag(const bool& new_scaling_flag)
{
   scaling_flag = new_scaling_flag;
}


// void set_lower_bound(void) method

/// This method sets the lower bound of the independent parameters to an empty vector. 

void IndependentParameters::set_lower_bounds(void)
{
   const unsigned int parameters_number = count_parameters_number();

   lower_bounds.set(parameters_number, 1.0e-99);
}


// void set_lower_bound(const Vector<double>&) method

/// This method sets the lower bound of all the independent parameters.
/// These values are used for unscaling the independent parameters so that they are not less than the lower bounds. 
/// @param new_lower_bounds New set of lower bounds for the independent parameters.

void IndependentParameters::set_lower_bounds(const Vector<double>& new_lower_bounds)
{
   // Control sentence (if debug)

   #ifdef _DEBUG 

   const unsigned int parameters_number = count_parameters_number();

   if(new_lower_bounds.size() != parameters_number)
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: IndependentParameters class.\n" 
             << "void set_lower_bounds(const Vector<double>&) method.\n"
             << "Size must be equal to number of parameters.\n";

	  throw std::logic_error(buffer.str());
   }

   #endif

   // Set lower bound of independent parameters

   lower_bounds = new_lower_bounds; 
}


// void set_lower_bound(const unsigned int&, const double&) method

/// This method sets the lower bound of a single independent parameter.
/// Such a value is used for unscaling that independent parameter so that it is not less than its lower bound. 
/// @param index Index of independent parameter.
/// @param new_lower_bound New lower bound for that independent parameter.

void IndependentParameters::set_lower_bound(const unsigned int& index, const double& new_lower_bound)
{
   const unsigned int parameters_number = count_parameters_number();

   // Control sentence (if debug)

   #ifdef _DEBUG 

   if(index >= parameters_number)
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: IndependentParameters class.\n"
             << "void set_lower_bound(const unsigned int&, const double&) method.\n"
             << "Index must be less than number of parameters.\n";

	  throw std::logic_error(buffer.str());
   }

   #endif

   // Set lower bound vector

   if(lower_bounds.size() != parameters_number)
   {
      lower_bounds.set(parameters_number, -1.0e99);
   }

   // Set lower bound of single independent parameter

   lower_bounds[index] = new_lower_bound;
}


// void set_upper_bounds(void) method

/// This method sets the vector of upper bounds for the independent parameters to have size zero. 

void IndependentParameters::set_upper_bounds(void)
{
   const unsigned int parameters_number = count_parameters_number();

   upper_bounds.set(parameters_number, 1.0e99);
}


// void set_upper_bound(const Vector<double>&) method

/// This method sets the upper bound of all the independent parameters.
/// These values are used for unscaling the independent parameters so that they are not greater than the 
/// upper bounds. 
/// @param new_upper_bounds New set of upper bounds for the independent parameters.

void IndependentParameters::set_upper_bounds(const Vector<double>& new_upper_bounds)
{
   // Control sentence (if debug)

   #ifdef _DEBUG 

   const unsigned int parameters_number = count_parameters_number();

   if(new_upper_bounds.size() != parameters_number)
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: IndependentParameters class.\n" 
             << "void set_upper_bound(const Vector<double>&) method.\n"
             << "Size of upper bounds must be equal to number of parameters.\n";

	  throw std::logic_error(buffer.str());
   }

   #endif

   upper_bounds = new_upper_bounds;
}


// void set_upper_bound(const unsigned int&, const double&) method

/// This method sets the upper bound of a single independent parameter.
/// Such a value is used for unscaling that independent parameter so that it is not greater than its upper bound. 
/// @param index Index of independent parameter.
/// @param new_upper_bound New upper bound for the independent parameter of index i.

void IndependentParameters::set_upper_bound(const unsigned int& index, const double& new_upper_bound)
{
   // Control sentence (if debug)

   #ifdef _DEBUG 

   const unsigned int parameters_number = count_parameters_number();

   if(index >= parameters_number)
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: IndependentParameters class.\n"
             << "void set_upper_bound(const unsigned int&, const double&) method.\n"
             << "Index must be less than number of parameters.\n";

	  throw std::logic_error(buffer.str());
   }

   #endif

   upper_bounds[index] = new_upper_bound;
}


// void set_bounds(void) method

/// This method sets the vectors of lower and upper bounds for the independent parameters to have size zero. 

void IndependentParameters::set_bounds(void)
{
   set_lower_bounds();
   set_upper_bounds();
}


// void set_bounds(const Vector< Vector<double> >&) method

/// This method sets both the lower and the upper bounds of all the independent parameters.
/// The format is a vector of two real vectors. 
/// The first element must contain the lower bound values values for the independent parameters.
/// The second element must contain the upper bound values for the independent parameters.
/// These values are used for unscaling the independent parameters so that they are neither less than the 
/// lower bounds nor greater than the upper bounds. 
/// @param new_bounds New set of lower and upper bounds for the independent parameters.

void IndependentParameters::set_bounds(const Vector< Vector<double> >& new_bounds)
{
   // Control sentence (if debug)

   #ifdef _DEBUG 

   const unsigned int parameters_number = count_parameters_number();

   if(new_bounds.size() != 2)
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: IndependentParameters class.\n" 
             << "void set_bounds(const Vector< Vector<double> >&) method.\n"
             << "Number of rows must be 2.\n";

	  throw std::logic_error(buffer.str());
   }
   
   
   if(new_bounds[0].size() != parameters_number
	    && new_bounds[1].size() != parameters_number)
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: IndependentParameters class.\n" 
             << "void set_bounds(const Vector< Vector<double> >&) method.\n"
             << "Number of columns must be equal to number of parameters.\n";

	  throw std::logic_error(buffer.str());
   }

   #endif

   // Set lower and upper bounds of independent parameters

   lower_bounds = new_bounds[0];
   upper_bounds = new_bounds[1];
}


// void set_bounding_flag(const bool&) method

/// This method sets the flag for applying lower and upper bounds to the independent parameters. 
/// @param new_bounding_flag True if the independent parameters are to be bounded, and false otherwise. 

void IndependentParameters::set_bounding_flag(const bool& new_bounding_flag)
{
   bounding_flag = new_bounding_flag;
}


// void set_display(const bool&) method

/// This method sets a new display value. 
/// If it is set to true messages from this class are to be displayed on the screen;
/// if it is set to false messages from this class are not to be displayed on the screen.
/// @param new_display Display value.

void IndependentParameters::set_display(const bool& new_display)
{
   display = new_display;
}


// bool is_empty(void) const method

/// This method returns true if the number of parameters is zero, and false otherwise. 

bool IndependentParameters::is_empty(void) const
{
   if(parameters.empty())
   {
      return(true);
   }
   else
   {
      return(false);
   }
}


// void initialize_parameters(const double&) method

/// This method initializes the independent parameters with a given value. 
/// @param value Initialization value. 

void IndependentParameters::initialize_parameters(const double& value)
{
   parameters.initialize(value);

   if(bounding_flag)
   {
      bound_parameters();
   }
}


// void initialize_parameters_uniform(void) method

/// This method initializes the independent parameters with values comprised between -1 and +1.

void IndependentParameters::initialize_parameters_uniform(void) 
{
   parameters.initialize_uniform();

   if(bounding_flag)
   {
      bound_parameters();
   }
}


// void initialize_parameters_uniform(const double&, const double&) method

/// This method initializes the independent parameters at random with values comprised between a minimum and a maximum values.
/// @param minimum Minimum initialization value.
/// @param maximum Maximum initialization value.

void IndependentParameters::initialize_parameters_uniform(const double& minimum, const double& maximum)
{
   parameters.initialize_uniform(minimum, maximum);

   if(bounding_flag)
   {
      bound_parameters();
   }
}


// void initialize_parameters_uniform(const Vector<double>&, const Vector<double>&) method

/// This method initializes the independent parameters at random with values 
/// comprised between different minimum and maximum numbers for each independent parameter.
/// @param minimum Vector of minimum initialization values.
/// @param maximum Vector of maximum initialization values.

void IndependentParameters::initialize_parameters_uniform(const Vector<double>& minimum, const Vector<double>& maximum)
{
   parameters.initialize_uniform(minimum, maximum);

   if(bounding_flag)
   {
      bound_parameters();
   }
}


// void initialize_parameters_uniform(const Vector< Vector<double> >&) method

/// This method initializes the independent parameters at random values comprised
/// between different minimum and maximum numbers for each independent parameter.
/// All minimum and maximum values are given from a vector of two real vectors.
/// The first element must contain the minimum inizizalization value for each independent parameter.
/// The second element must contain the maximum inizizalization value for each independent parameter.
/// @param minimum_maximum Matrix of minimum and maximum initialization values.

void IndependentParameters::initialize_parameters_uniform(const Vector< Vector<double> >& minimum_maximum)
{
   parameters.initialize_uniform(minimum_maximum[0], minimum_maximum[1]);

   if(bounding_flag)
   {
      bound_parameters();
   }
}


// void initialize_parameters_normal(void) method

/// This method initializes the independent parameters with random values chosen from a 
/// normal distribution with mean 0 and standard deviation 1.

void IndependentParameters::initialize_parameters_normal(void)
{
   parameters.initialize_normal();

   if(bounding_flag)
   {
      bound_parameters();
   }
}


// void initialize_parameters_normal(const double&, const double&) method

/// This method initializes the independent parameters with random values chosen 
/// from a normal distribution with a given mean and a given standard deviation.
/// @param mean Mean of normal distribution.
/// @param standard_deviation Standard deviation of normal distribution.

void IndependentParameters::initialize_parameters_normal(const double& mean, const double& standard_deviation)
{
   parameters.initialize_normal(mean, standard_deviation);

   if(bounding_flag)
   {
      bound_parameters();
   }
}


// void initialize_parameters_normal(const Vector<double>&, const Vector<double>&) method

/// This method initializes the independent parameters with random values chosen 
/// from normal distributions with different mean and standard deviation for each independent parameter.
/// @param mean Vector of mean values.
/// @param standard_deviation Vector of standard deviation values.

void IndependentParameters::initialize_parameters_normal(const Vector<double>& mean, const Vector<double>& standard_deviation)
{
   parameters.initialize_normal(mean, standard_deviation);

   if(bounding_flag)
   {
      bound_parameters();
   }
}


// void initialize_parameters_normal(const Vector< Vector<double> >&) method

/// This method initializes the independent parameters with random values chosen 
/// from normal distributions with different mean and standard deviation for each independent parameter.
/// All mean and standard deviation values are given from a vector of two real vectors.
/// The first element must contain the mean value for each independent parameter.
/// The second element must contain the standard deviation value for each independent parameter.
/// @param mean_standard_deviation Vector of mean and standard deviation vectors.

void IndependentParameters::initialize_parameters_normal(const Vector< Vector<double> >& mean_standard_deviation)
{
   parameters.initialize_normal(mean_standard_deviation[0], mean_standard_deviation[1]);

   if(bounding_flag)
   {
      bound_parameters();
   }
}


// void initialize_random(void) method

/// This method initializes all the membes of this object with random values. 
/// This is useful for testing purposes. 

void IndependentParameters::initialize_random(void)
{
   const unsigned int parameters_number = rand()%10 + 2;

   set(parameters_number);

   parameters.initialize_normal();

   minimums.set(parameters_number);
   minimums.initialize_normal();

   maximums.set(parameters_number);
   maximums.initialize_normal();

   means.set(parameters_number);
   means.initialize_normal();

   standard_deviations.set(parameters_number);
   standard_deviations.initialize_normal();

   if(rand()%2)
   {
      set_scaling_method(MinimumMaximum);

   }
   else
   {
      set_scaling_method(MeanStandardDeviation);
   }

   if(rand()%2)
   {
      set_scaling_flag(false);
   }
   else
   {
      set_scaling_flag(true);
   }

   lower_bounds.set(parameters_number);
   lower_bounds.initialize_uniform(-1.0, 0.0);

   upper_bounds.set(parameters_number);
   upper_bounds.initialize_uniform(0.0, 1.0);

   if(rand()%2)
   {
      set_bounding_flag(false);
   }
   else
   {
      set_bounding_flag(true);
   }

   if(rand()%2)
   {
      set_display(false);
   }
   else
   {
      set_display(true);
   }

   if(bounding_flag)
   {
      bound_parameters();
   }
}


// Vector<double> calculate_scaled_parameters(void) const method

/// This method preprocesses the independendent parameters according to their scaling and unscaling method.
/// This form of scaling is used prior when getting the vector of parameters.

Vector<double> IndependentParameters::calculate_scaled_parameters(void) const
{
   const unsigned int parameters_number = count_parameters_number();

   switch(scaling_method)   
   {
      case MinimumMaximum:
      {
         Vector<double> scaled_parameters(parameters_number);

         for(unsigned int i = 0; i < parameters_number; i++)
         {
            if(maximums[i] - minimums[i] < 1.0e-99)
            {
               if(display)
               {
                  std::cout << "OpenNN Warning: IndependentParameters class.\n"
                            << "Vector<double> calculate_scaled_parameters(void) const method.\n"
                            << "Maximum and minimum of parameter " << i << " are equal.\n"
                            << "That parameter won't be scaled.\n"; 
               }

               scaled_parameters[i] = parameters[i];
            }
            else
            {
               scaled_parameters[i] = 2.0*(parameters[i] - minimums[i])/(maximums[i] - minimums[i])-1.0;
            }
         }

		   return(scaled_parameters);	  
	   }
      break;

      case MeanStandardDeviation:
      {
         Vector<double> scaled_parameters(parameters_number);

         for(unsigned int i = 0; i < parameters_number; i++)
         {
            if(standard_deviations[i] < 1.0e-99)
            {
               if(display)
               {
                  std::cout << "OpenNN Warning: IndependentParameters class.\n"
                            << "Vector<double> calculate_scaled_parameters(void) method.\n"
                            << "Standard deviation of parameter " << i << " is zero.\n"
                            << "That won't be unscaled.\n"; 
               }
               
               scaled_parameters[i] = parameters[i];
            }
            else
            {
               scaled_parameters[i] = (parameters[i] - means[i])/standard_deviations[i];
            }
         }

         return(scaled_parameters);
	  }
      break;

      default:
      {
         std::ostringstream buffer;

         buffer << "OpenNN Exception: IndependentParameters class\n"
                << "Vector<double> calculate_scaled_parameters(void) method.\n" 
                << "Unknown independent parameters scaling and unscaling method.\n";

	     throw std::logic_error(buffer.str());
      }
      break;
   }// end switch
}


// void unscale_parameters(void) method

/// This method postprocesses the independendent parameters according to their scaling and unscaling method.
/// This form of scaling is used when setting a new vector of parameters.

void IndependentParameters::unscale_parameters(const Vector<double>& scaled_parameters)
{
   const unsigned int parameters_number = count_parameters_number();

   switch(scaling_method)   
   {
      case MeanStandardDeviation:
      {
         for(unsigned int i = 0; i < parameters_number; i++)
	     {
            if(standard_deviations[i] < 1e-99)
            {      
               if(display)
               {
                  std::cout << "OpenNN Warning: IndependentParameters class\n"
                            << "void unscale_parameters(void) method.\n"
                            << "Standard deviation of parameter " << i << " is zero.\n" 
                            << "That parameter won't be scaled.\n";
               }
               
               parameters[i] = scaled_parameters[i];
            }      
            else
            {
               parameters[i] = means[i] + scaled_parameters[i]*standard_deviations[i]; 
            } 
         }
      }
      break;

      case MinimumMaximum:
      {
         for(unsigned int i = 0; i < parameters_number; i++)
	     {
            if(maximums[i] - minimums[i] < 1e-99)
            {      
               if(display)
               {
                  std::cout << "OpenNN Warning: IndependentParameters class\n"
                            << "void unscale_parameters(void) method.\n"
                            << "Maximum and minimum of parameter " << i << " are equal.\n"
                            << "That parameter won't be scaled.\n"; 
               }
               
               parameters[i] = scaled_parameters[i];
            }      
            else
            {
               parameters[i] = 0.5*(scaled_parameters[i] + 1.0)*(maximums[i]-minimums[i]) + minimums[i]; 
            }
         }
      }
      break;

      default:
      {
         std::ostringstream buffer;

         buffer << "OpenNN Exception: IndependentParameters class\n"
                << "void unscale_parameters(void) method.\n" 
                << "Unknown scaling and unscaling method.\n";

	     throw std::logic_error(buffer.str());
      }
      break;
   }// end switch       

   if(bounding_flag)
   {
      bound_parameters();
   }
}


// void bound_parameters(void) const method

/// This method makes the independent parameters to fall in the range defined by their lower and the upper bounds. 

void IndependentParameters::bound_parameters(void)
{
   const unsigned int parameters_number = count_parameters_number();

   const unsigned int lower_bounds_size = lower_bounds.size();  
   const unsigned int upper_bounds_size = upper_bounds.size();  
 
   if(lower_bounds_size == parameters_number && upper_bounds_size == parameters_number)
   {
      parameters.apply_lower_upper_bounds(lower_bounds, upper_bounds);
   }
   else if(lower_bounds_size == parameters_number)
   {
      parameters.apply_lower_bound(lower_bounds);   
   }
   else if(upper_bounds_size == parameters_number)
   {
      parameters.apply_upper_bound(upper_bounds);   
   }
}


// void bound_parameter(const unsigned int&) method

/// This method makes a single independent parameter to fall in the range defined by its lower and the upper bounds. 
/// @param index Index of independent parameters. 

void IndependentParameters::bound_parameter(const unsigned int& index)
{
   if(lower_bounds != 0 && upper_bounds != 0)
   {
      if(parameters[index] < lower_bounds[index])
      {
         parameters[index] = lower_bounds[index];
      }
      else if(parameters[index] > upper_bounds[index])
      {
         parameters[index] = upper_bounds[index];
      }
   }
   else if(lower_bounds != 0)
   {
      if(parameters[index] < lower_bounds[index])
      {
         parameters[index] = lower_bounds[index];
      }
   }
   else if(upper_bounds != 0)
   {
      if(parameters[index] > upper_bounds[index])
      {
         parameters[index] = upper_bounds[index];
      }
   }
}


// std::string to_string(void) const method

/// This method returns a string representation of the current independent parameters object. 

std::string IndependentParameters::to_string(void) const
{
   std::ostringstream buffer;

   buffer << "Independent parameters\n"
          << "Parameters: " << parameters << "\n"
          << "Names: " << names << "\n"
          << "Units: " << units << "\n"
          << "Descriptions: " << descriptions << "\n"
          << "Minimums: " << minimums << "\n"
          << "Maximums: " << maximums << "\n"
          << "Means: " << means << "\n"
          << "Standard deviations: " << standard_deviations << "\n"
          << "Lower bounds: " << lower_bounds << "\n"
          << "Upper bounds: " << upper_bounds << "\n"
          << "Scaling method: " << scaling_method << "\n"
          << "Scaling flag: " << scaling_flag << "\n"
          << "Bounding flag: " << bounding_flag << "\n"
          << "Display range warning: " << display_range_warning << "\n"
          << "Display: " << display << "\n";

   return(buffer.str());
}


// TiXmlElement* to_XML(void) const method

/// This method serializes the independent parameters object into a XML element of the TinyXML library. 
/// See the OpenNN manual for more information about the format of this element. 

TiXmlElement* IndependentParameters::to_XML(void) const
{
   std::ostringstream buffer;

   TiXmlElement* independent_parameters_element = new TiXmlElement("IndependentParameters");
   independent_parameters_element->SetAttribute("Version", 4);

   const unsigned int parameters_number = count_parameters_number();

   // Parameters

   TiXmlElement* parameters_element = new TiXmlElement("Parameters");
   independent_parameters_element->LinkEndChild(parameters_element);

   buffer.str("");
   buffer << parameters;

   TiXmlText* parameters_text = new TiXmlText(buffer.str().c_str());
   parameters_element->LinkEndChild(parameters_text);

   // Names
   {
      TiXmlElement* names_element = new TiXmlElement("Names");
      independent_parameters_element->LinkEndChild(names_element);

      Vector<TiXmlElement*> elements(parameters_number);
      Vector<TiXmlText*> texts(parameters_number);

      for(unsigned int i = 0; i < parameters_number; i++)
      {
         elements[i] = new TiXmlElement("Name");
         elements[i]->SetAttribute("Index", i+1);
         names_element->LinkEndChild(elements[i]);
      
         texts[i] = new TiXmlText(names[i].c_str());
         elements[i]->LinkEndChild(texts[i]);
      }
   }

   // Units 
   
   {
      TiXmlElement* units_element = new TiXmlElement("Units");
      independent_parameters_element->LinkEndChild(units_element);

      Vector<TiXmlElement*> elements(parameters_number);
      Vector<TiXmlText*> texts(parameters_number);

      for(unsigned int i = 0; i < parameters_number; i++)
      {
         elements[i] = new TiXmlElement("Unit");
         elements[i]->SetAttribute("Index", i+1);
         units_element->LinkEndChild(elements[i]);
      
         texts[i] = new TiXmlText(units[i].c_str());
         elements[i]->LinkEndChild(texts[i]);
      }
   }

   // Descriptions
   {
      TiXmlElement* descriptions_element = new TiXmlElement("Descriptions");
      independent_parameters_element->LinkEndChild(descriptions_element);

      Vector<TiXmlElement*> elements(parameters_number);
      Vector<TiXmlText*> texts(parameters_number);

      for(unsigned int i = 0; i < parameters_number; i++)
      {
         elements[i] = new TiXmlElement("Description");
         elements[i]->SetAttribute("Index", i+1);
         descriptions_element->LinkEndChild(elements[i]);
      
         texts[i] = new TiXmlText(descriptions[i].c_str());
         elements[i]->LinkEndChild(texts[i]);
      }
   }

   // Minimums
   {
      TiXmlElement* element = new TiXmlElement("Minimums");
      independent_parameters_element->LinkEndChild(element);

      buffer.str("");
      buffer << minimums;

      TiXmlText* text = new TiXmlText(buffer.str().c_str());
      element->LinkEndChild(text);
   }

   // Maximums 

   {
      TiXmlElement* element = new TiXmlElement("Maximums");
      independent_parameters_element->LinkEndChild(element);

      buffer.str("");
      buffer << maximums;

      TiXmlText* text = new TiXmlText(buffer.str().c_str());
      element->LinkEndChild(text);
   }

   // Means
   {
      TiXmlElement* element = new TiXmlElement("Means");
      independent_parameters_element->LinkEndChild(element);

      buffer.str("");
      buffer << means;

      TiXmlText* text = new TiXmlText(buffer.str().c_str());
      element->LinkEndChild(text);
   }

   // Standard deviations

   {
      TiXmlElement* element = new TiXmlElement("StandardDeviations");
      independent_parameters_element->LinkEndChild(element);

      buffer.str("");
      buffer << standard_deviations;

      TiXmlText* text = new TiXmlText(buffer.str().c_str());
      element->LinkEndChild(text);
   }

   // Lower bounds
   {
      TiXmlElement* element = new TiXmlElement("LowerBounds");
      independent_parameters_element->LinkEndChild(element);

      buffer.str("");
      buffer << lower_bounds;

      TiXmlText* text = new TiXmlText(buffer.str().c_str());
      element->LinkEndChild(text);
   }

   // Upper bounds
   {
      TiXmlElement* element = new TiXmlElement("UpperBounds");
      independent_parameters_element->LinkEndChild(element);

      buffer.str("");
      buffer << upper_bounds;

      TiXmlText* text = new TiXmlText(buffer.str().c_str());
      element->LinkEndChild(text);
   }

   // Scaling method
   {
      TiXmlElement* element = new TiXmlElement("ScalingMethod");
      independent_parameters_element->LinkEndChild(element);

      TiXmlText* text = new TiXmlText(write_scaling_method().c_str());
      element->LinkEndChild(text);
   }

   // Scaling flag

   {
      TiXmlElement* element = new TiXmlElement("ScalingFlag");
      independent_parameters_element->LinkEndChild(element);

      buffer.str("");
      buffer << scaling_flag;

      TiXmlText* text = new TiXmlText(buffer.str().c_str());
      element->LinkEndChild(text);
   }

   // Bounding flag

   {
      TiXmlElement* element = new TiXmlElement("BoundingFlag");
      independent_parameters_element->LinkEndChild(element);

      buffer.str("");
      buffer << bounding_flag;

      TiXmlText* text = new TiXmlText(buffer.str().c_str());
      element->LinkEndChild(text);
   }

   // Display range warning
   {
      TiXmlElement* element = new TiXmlElement("DisplayRangeWarning");
      independent_parameters_element->LinkEndChild(element);

      buffer.str("");
      buffer << display_range_warning;

      TiXmlText* text = new TiXmlText(buffer.str().c_str());
      element->LinkEndChild(text);
   }

   // Display
   {
      TiXmlElement* display_element = new TiXmlElement("Display");
      independent_parameters_element->LinkEndChild(display_element);

      buffer.str("");
      buffer << display;

      TiXmlText* display_text = new TiXmlText(buffer.str().c_str());
      display_element->LinkEndChild(display_text);
   }

   return(independent_parameters_element);
}


// void from_XML(TiXmlElement*) method

/// This method deserializes a TinyXML element into this independent parameters object. 
/// @param independent_parameters_element Pointer to a XML element containing the member data. 

void IndependentParameters::from_XML(TiXmlElement* independent_parameters_element)
{
   if(independent_parameters_element)
   {
      int index = 0; // unsigned int does not work

      unsigned int parameters_number = count_parameters_number();

      // Parameters
      {
         TiXmlElement* parameters_element = independent_parameters_element->FirstChildElement("Parameters");

         if(parameters_element)
         {    
         }

         set(parameters);

         parameters_number = count_parameters_number();
      }

      // Names
      {
         TiXmlElement* names_element = independent_parameters_element->FirstChildElement("Names");

         if(names_element)
         {        
            Vector<std::string> new_names(parameters_number);
      
            TiXmlElement* name_element = names_element->FirstChildElement("Name");

            if(name_element)
            {
               name_element->QueryIntAttribute("Index", &index);

			   if(name_element->GetText())
			   { 
                  new_names[index-1] = name_element->GetText();
			   } 

               for( ; name_element; name_element=name_element->NextSiblingElement())
               {
                  name_element->QueryIntAttribute("Index", &index);

				  if(name_element->GetText())
				  {
                     new_names[index-1] = name_element->GetText();
				  }
               }
            }
         
            try
            {
               set_names(new_names);
            }
            catch(std::exception& e)
            {
               std::cout << e.what() << std::endl;		 
            }
         }
      }

      // Units
      {
         TiXmlElement* units_element = independent_parameters_element->FirstChildElement("VariablesUnits");

         if(units_element)
         {        
            Vector<std::string> new_units(parameters_number);
      
            TiXmlElement* variable_units_element = units_element->FirstChildElement("VariableUnits");

            if(variable_units_element)
            {
               variable_units_element->QueryIntAttribute("Index", &index);

			   if(variable_units_element->GetText())
			   {
                  new_units[index-1] = variable_units_element->GetText();
			   }

               for( ; variable_units_element; variable_units_element=variable_units_element->NextSiblingElement())
               {
                  variable_units_element->QueryIntAttribute("Index", &index);

				  if(variable_units_element->GetText())
				  { 
                     new_units[index-1] = variable_units_element->GetText();
				  } 
               }
            }
         
            try
            {
               set_units(new_units);
            }
            catch(std::exception& e)
            {
               std::cout << e.what() << std::endl;		 
            }
         }
      }

      // Descriptions
      {
         TiXmlElement* descriptions_element = independent_parameters_element->FirstChildElement("Descriptions");

         if(descriptions_element)
         {        
            Vector<std::string> new_descriptions(parameters_number);
      
            TiXmlElement* variable_description_element = descriptions_element->FirstChildElement("Description");

            if(variable_description_element)
            {
               variable_description_element->QueryIntAttribute("Index", &index);

			   if(variable_description_element->GetText())
			   {
                  new_descriptions[index-1] = variable_description_element->GetText();
			   }

               for( ; variable_description_element; variable_description_element=variable_description_element->NextSiblingElement())
               {
                  variable_description_element->QueryIntAttribute("Index", &index);

				  if(variable_description_element->GetText())
				  { 
                     new_descriptions[index-1] = variable_description_element->GetText();
				  }
               }
            }

            try
            {
               set_descriptions(new_descriptions);
            }
            catch(std::exception& e)
            {
               std::cout << e.what() << std::endl;		 
            }
         }
      }

      // Minimums
      {
         TiXmlElement* minimums_element = independent_parameters_element->FirstChildElement("Minimums");

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
         TiXmlElement* maximums_element = independent_parameters_element->FirstChildElement("Maximums");

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
         TiXmlElement* means_element = independent_parameters_element->FirstChildElement("Means");

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
         TiXmlElement* standard_deviations_element = independent_parameters_element->FirstChildElement("StandardDeviations");

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

      // Lower bounds
      {
         TiXmlElement* lower_bounds_element = independent_parameters_element->FirstChildElement("LowerBounds");

         if(lower_bounds_element)
         {
            const char* lower_bounds_text = lower_bounds_element->GetText();

            if(lower_bounds_text)
            {
               Vector<double> new_lower_bounds;                  
               new_lower_bounds.parse(lower_bounds_text);

               try
               {
                  set_lower_bounds(new_lower_bounds);
               }
               catch(std::exception& e)
               {
                  std::cout << e.what() << std::endl;		 
               }
            }
         }
      }

      // Upper bounds
      {
         TiXmlElement* upper_bounds_element = independent_parameters_element->FirstChildElement("UpperBounds");

         if(upper_bounds_element)
         {
            const char* upper_bounds_text = upper_bounds_element->GetText();

            if(upper_bounds_text)
            {
               Vector<double> new_upper_bounds;                  
               new_upper_bounds.parse(upper_bounds_text);

               try
               {
                  set_upper_bounds(new_upper_bounds);
               }
               catch(std::exception& e)
               {
                  std::cout << e.what() << std::endl;		 
               }
            }
         }
      }

      // Display warnings
      {
         TiXmlElement* display_element = independent_parameters_element->FirstChildElement("Display");

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
