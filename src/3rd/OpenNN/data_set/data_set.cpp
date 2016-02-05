/****************************************************************************************************************/
/*                                                                                                              */
/*   OpenNN: Open Neural Networks Library                                                                       */
/*   www.opennn.cimne.com                                                                                       */
/*                                                                                                              */
/*   D A T A   S E T   C L A S S                                                                                */
/*                                                                                                              */ 
/*   Roberto Lopez                                                                                              */ 
/*   International Center for Numerical Methods in Engineering (CIMNE)                                          */
/*   Technical University of Catalonia (UPC)                                                                    */
/*   Barcelona, Spain                                                                                           */
/*   E-mail: rlopez@cimne.upc.edu                                                                               */ 
/*                                                                                                              */
/****************************************************************************************************************/

// System includes

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <cmath>
#include <algorithm>
#include <cstdlib>
#include <stdexcept>
#include <ctime>
#include <exception>

// OpenNN includes

#include "data_set.h"

// TinyXml includes

#include "../../tinyxml/tinyxml.h"

namespace OpenNN
{


// DEFAULT CONSTRUCTOR

/// Default constructor. It creates a data set object with zero instances and zero inputs and target variables. 
/// It also initializes the rest of class members to their default values.

DataSet::DataSet(void)
{
   set();  

   set_default();
}


// GENERAL CONSTRUCTOR

/// Instances and variables number constructor. 
/// It creates a data set object with given instances and variables numbers.
/// All the variables are set as inputs. 
/// It also initializes the rest of class members to their default values.
/// @param new_instances_number Number of instances in the data set.
/// @param new_variables_number Number of variables.

DataSet::DataSet(const unsigned int& new_instances_number, const unsigned int& new_variables_number)
{
   set(new_instances_number, new_variables_number);

   set_default();
}


// INSTANCES, INPUTS AND TARGETS NUMBERS CONSTRUCTORS

/// Instances number, input variables number and target variables number constructor. 
/// It creates a data set object with given instances and inputs and target variables numbers.
/// It also initializes the rest of class members to their default values.
/// @param new_instances_number Number of instances in the data set.
/// @param new_inputs_number Number of input variables.
/// @param new_targets_number Number of target variables.

DataSet::DataSet(const unsigned int& new_instances_number, const unsigned int& new_inputs_number, const unsigned int& new_targets_number)
{
   set(new_instances_number, new_inputs_number, new_targets_number);

   set_default();
}


// XML CONSTRUCTOR

/// This method sets the data set members from a XML element. 
/// @param data_set_element Pointer to a Tiny XML element containing the member data. 

DataSet::DataSet(TiXmlElement* data_set_element)
{
   set_default();

   from_XML(data_set_element);
}


// FILE CONSTRUCTOR

/// File constructor. It creates a data set object by loading the object members from a XML-type file. 
/// Please mind about the file format. This is specified in the User's Guide.
/// @param filename Data set file name.

DataSet::DataSet(const std::string& filename)
{
   set();

   set_default();

   load(filename);
}


// COPY CONSTRUCTOR

/// Copy constructor. 
/// It creates a copy of an existing inputs targets data set object. 
/// @param other_data_set Data set object to be copied.

DataSet::DataSet(const DataSet& other_data_set)
{
   set_default();

   set(other_data_set);
}


// DESTRUCTOR

/// Destructor. 

DataSet::~DataSet(void)
{
}


// ASSIGNMENT OPERATOR

/// Assignment operator. 
/// It assigns to the current object the members of an existing data set object.
/// @param other_data_set Data set object to be assigned.

DataSet& DataSet::operator = (const DataSet& other_data_set)
{
   if(this != &other_data_set) 
   {
      data_filename = other_data_set.data_filename;

      // Data matrix

      data = other_data_set.data;

      // Variables information

      variables_information = other_data_set.variables_information;

      // Instances information

      instances_information = other_data_set.instances_information;

      // Utilities

      display = other_data_set.display;
   }

   return(*this);
}


// EQUAL TO OPERATOR

// bool operator == (const DataSet&) const method

/// Equal to operator. 
/// It compares this object with another object of the same class. 
/// It returns true if the members of the two objects have the same values, and false otherwise.
/// @ param other_data_set Data set object to be compared with.

bool DataSet::operator == (const DataSet& other_data_set) const
{
   if(data_filename == other_data_set.data_filename
   && data == other_data_set.data
   && variables_information == other_data_set.variables_information
   && instances_information == other_data_set.instances_information
   && display == other_data_set.display)
   {
      return(true);
   }
   else
   {
      return(false);
   }

}


// METHODS

// const VariablesInformation& get_variables_information(void) const

/// This method returns a constant reference to the variables information object composing this data set object. 

const VariablesInformation& DataSet::get_variables_information(void) const
{
   return(variables_information);
}


// VariablesInformation* get_variables_information_pointer(void) const

/// This method returns a pointer to the variables information object composing this data set object. 

VariablesInformation* DataSet::get_variables_information_pointer(void) 
{
   return(&variables_information);
}


// const InstancesInformation& get_instances_information(void) const

/// This method returns a constant reference to the instances information object composing this data set object. 

const InstancesInformation& DataSet::get_instances_information(void) const
{
   return(instances_information);
}


// InstancesInformation* get_instances_information_pointer(void)

/// This method returns a pointer to the variables information object composing this data set object. 

InstancesInformation* DataSet::get_instances_information_pointer(void) 
{
   return(&instances_information);
}


// const ScalingUnscalingMethod& get_scaling_unscaling_method(void) const method

/// This method returns the method used for scaling and unscaling the data. 

const DataSet::ScalingUnscalingMethod& DataSet::get_scaling_unscaling_method(void) const
{
   return(scaling_unscaling_method);
}


// std::string write_scaling_unscaling_method(void) const method

/// This method returns a string with the name of the method used for scaling and unscaling the data. 

std::string DataSet::write_scaling_unscaling_method(void) const
{
   if(scaling_unscaling_method == MinimumMaximum)
   {
      return("MinimumMaximum");
   }
   else if(scaling_unscaling_method == MeanStandardDeviation)
   {
      return("MeanStandardDeviation");
   }
   else
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: DataSet class.\n"
             << "std::string write_scaling_unscaling_method(void) const method.\n"
             << "Unknown scaling and unscaling method.\n";
 
	  throw std::logic_error(buffer.str());
   }
}


// const bool& get_display(void) const method

/// This method returns true if messages from this class can be displayed on the screen,
/// or false if messages from this class can't be displayed on the screen.

const bool& DataSet::get_display(void) const
{
   return(display);   
}


// const Matrix<double>& get_data(void) const method

/// This method returns a reference to the data matrix in the data set. 
/// The number of rows is equal to the number of instances.
/// The number of columns is equal to the number of variables. 

const Matrix<double>& DataSet::get_data(void) const
{
   return(data);
}


// const std::string& get_data_filename(void) const method

/// This method returns the name of the data file. 

const std::string& DataSet::get_data_filename(void) const
{
   return(data_filename);
}


// Matrix<double> arrange_training_data(void) const method

/// This method returns a matrix with the training instances in the data set. 
/// The number of rows is the number of training instances.
/// The number of columns is the number of variables. 

Matrix<double> DataSet::arrange_training_data(void) const
{
   const unsigned int variables_number = get_variables_number();
   
   Vector<unsigned int> variables_indices(0, 1, variables_number-1);

   const Vector<unsigned int>& training_indices = instances_information.get_training_indices();

   return(data.arrange_submatrix(training_indices, variables_indices));
}


// Matrix<double> arrange_generalization_data(void) const method

/// This method returns a matrix with the generalization instances in the data set. 
/// The number of rows is the number of generalization instances.
/// The number of columns is the number of variables. 

Matrix<double> DataSet::arrange_generalization_data(void) const
{
   const unsigned int variables_number = get_variables_number();

   const Vector<unsigned int>& generalization_indices = instances_information.get_generalization_indices();

   Vector<unsigned int> variables_indices(0, 1, variables_number-1);

   return(data.arrange_submatrix(generalization_indices, variables_indices));
}


// Matrix<double> arrange_testing_data(void) const method

/// This method returns a matrix with the testing instances in the data set. 
/// The number of rows is the number of testing instances.
/// The number of columns is the number of variables. 

Matrix<double> DataSet::arrange_testing_data(void) const
{
   const unsigned int variables_number = get_variables_number();
   Vector<unsigned int> variables_indices(0, 1, variables_number-1);

   const Vector<unsigned int>& testing_indices = instances_information.get_testing_indices();

   return(data.arrange_submatrix(testing_indices, variables_indices));
}


// Matrix<double> arrange_input_data(void) const method

/// This method returns a matrix with the input variables in the data set.
/// The number of rows is the number of instances.
/// The number of columns is the number of input variables. 

Matrix<double> DataSet::arrange_input_data(void) const
{
   const unsigned int instances_number = get_instances_number();   
   Vector<unsigned int> instances_indices(0, 1, instances_number-1);

   const Vector<unsigned int>& inputs_indices = variables_information.get_inputs_indices();

   return(data.arrange_submatrix(instances_indices, inputs_indices));
}


// Matrix<double> arrange_target_data(void) const method

/// This method returns a matrix with the target variables in the data set.
/// The number of rows is the number of instances.
/// The number of columns is the number of target variables. 

Matrix<double> DataSet::arrange_target_data(void) const
{
   const unsigned int instances_number = get_instances_number();
   Vector<unsigned int> instances_indices(0, 1, instances_number-1);

   const Vector<unsigned int>& targets_indices = variables_information.get_targets_indices();

   return(data.arrange_submatrix(instances_indices, targets_indices));
}


// Matrix<double> arrange_training_input_data(void) const method

/// This method returns a matrix with training instances and input variables.
/// The number of rows is the number of training instances.
/// The number of columns is the number of input variables. 

Matrix<double> DataSet::arrange_training_input_data(void) const
{
   const Vector<unsigned int>& inputs_indices = variables_information.get_inputs_indices();

   const Vector<unsigned int>& training_indices = instances_information.get_training_indices();

   return(data.arrange_submatrix(training_indices, inputs_indices));
}


// Matrix<double> arrange_training_target_data(void) const method

/// This method returns a matrix with training instances and target variables.
/// The number of rows is the number of training instances.
/// The number of columns is the number of target variables. 

Matrix<double> DataSet::arrange_training_target_data(void) const 
{
   const Vector<unsigned int>& training_indices = instances_information.get_training_indices();

   const Vector<unsigned int>& targets_indices = variables_information.get_targets_indices();

   return(data.arrange_submatrix(training_indices, targets_indices));
}


// Matrix<double> get_generalization_input_data(void) const method

/// This method returns a matrix with generalization instances and input variables.
/// The number of rows is the number of generalization instances.
/// The number of columns is the number of input variables. 

Matrix<double> DataSet::get_generalization_input_data(void) const
{
   const Vector<unsigned int>& generalization_indices = instances_information.get_generalization_indices();

   const Vector<unsigned int>& inputs_indices = variables_information.get_inputs_indices();

   return(data.arrange_submatrix(generalization_indices, inputs_indices));
}


// Matrix<double> get_generalization_target_data(void) const method

/// This method returns a matrix with generalization instances and target variables.
/// The number of rows is the number of generalization instances.
/// The number of columns is the number of target variables. 

Matrix<double> DataSet::get_generalization_target_data(void) const
{
   const Vector<unsigned int>& generalization_indices = instances_information.get_generalization_indices();

   const Vector<unsigned int>& targets_indices = variables_information.get_targets_indices();

   return(data.arrange_submatrix(generalization_indices, targets_indices));
}


// Matrix<double> arrange_testing_input_data(void) const method

/// This method returns a matrix with testing instances and input variables.
/// The number of rows is the number of testing instances.
/// The number of columns is the number of input variables. 

Matrix<double> DataSet::arrange_testing_input_data(void) const
{
   const Vector<unsigned int>& inputs_indices = variables_information.get_inputs_indices();

   const Vector<unsigned int>& testing_indices = instances_information.get_testing_indices();

   return(data.arrange_submatrix(testing_indices, inputs_indices));
}


// Matrix<double> arrange_testing_target_data(void) const method

/// This method returns a matrix with testing instances and target variables.
/// The number of rows is the number of testing instances.
/// The number of columns is the number of target variables. 

Matrix<double> DataSet::arrange_testing_target_data(void) const
{
   const Vector<unsigned int>& targets_indices = variables_information.get_targets_indices();

   const Vector<unsigned int>& testing_indices = instances_information.get_testing_indices();

   return(data.arrange_submatrix(testing_indices, targets_indices));
}


// Vector<double> get_instance(const unsigned int&) const method

/// This method returns the inputs and target values of a single instance in the data set. 
/// @param i Index of the instance. 

Vector<double> DataSet::get_instance(const unsigned int& i) const
{
   // Control sentence (if debug)

   #ifdef _DEBUG 

   unsigned int instances_number = get_instances_number();

   if(i >= instances_number)
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: DataSet class.\n"
             << "Vector<double> get_instance(const unsigned int&) const method.\n"
             << "Index of instance must be less than number of instances.\n";

	  throw std::logic_error(buffer.str());
   }

   #endif

   // Get instance

   return(data.arrange_row(i));
}


// Vector<double> get_training_instance(const unsigned int&) const method

/// This method returns the inputs and target values of a single training instance in the data set. 
/// @param training_instance_index Index of the training instance. 

Vector<double> DataSet::get_training_instance(const unsigned int& training_instance_index) const
{
   // Control sentence (if debug)

   #ifdef _DEBUG 
  
   const unsigned int training_instances_number = instances_information.count_training_instances_number();

   if(training_instance_index >= training_instances_number)
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: DataSet class.\n"
             << "Vector<double> get_training_instance(const unsigned int&) const method.\n"
             << "Index of training instance must be less than number of training instances.\n";

	  throw std::logic_error(buffer.str());
   }

   #endif

   const Vector<unsigned int>& training_indices = instances_information.get_training_indices();

   return(data.arrange_row(training_indices[training_instance_index]));
}


// Vector<double> get_generalization_instance(const unsigned int&) const method

/// This method returns the inputs and target values of a single generalization instance in the data set. 
/// @param generalization_instance_index Index of the generalization instance. 

Vector<double> DataSet::get_generalization_instance(const unsigned int& generalization_instance_index) const
{
   // Control sentence (if debug)

   #ifdef _DEBUG 
 
   const unsigned int generalization_instances_number = instances_information.count_generalization_instances_number();

   if(generalization_instance_index >= generalization_instances_number)
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: DataSet class.\n"
             << "Vector<double> get_generalization_instance(const unsigned int&) const method.\n"
             << "Index of generalization instance must be less than number of generalization instances.\n";

	  throw std::logic_error(buffer.str());
   }

   #endif

   const Vector<unsigned int>& generalization_indices = instances_information.get_generalization_indices();

   return(data.arrange_row(generalization_indices[generalization_instance_index]));
}


// Vector<double> get_testing_instance(const unsigned int&) const method

/// This method returns the inputs and target values of a single testing instance in the data set. 
/// @param testing_instance_index Index of the testing instance. 

Vector<double> DataSet::get_testing_instance(const unsigned int& testing_instance_index) const
{
   // Control sentence (if debug)

   #ifdef _DEBUG 

   const unsigned int testing_instances_number = instances_information.count_testing_instances_number();

   if(testing_instance_index >= testing_instances_number)
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: DataSet class.\n"
             << "Vector<double> get_testing_instance(const unsigned int&) const method.\n"
             << "Index of testing instance must be less than number of testing instances.\n";

	  throw std::logic_error(buffer.str());
   }

   #endif

   const Vector<unsigned int>& testing_indices = instances_information.get_testing_indices();

   return(data.arrange_row(testing_indices[testing_instance_index]));
}


// Vector<double> get_input_instance(const unsigned int&) const method

/// This method returns a vector with the input values of a single instance in the data set. 
/// @param instance_index Index of the instance. 

Vector<double> DataSet::get_input_instance(const unsigned int& instance_index) const
{
   // Control sentence (if debug)

   #ifdef _DEBUG 

   unsigned int instances_number = get_instances_number();

   if(instance_index >= instances_number)
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: DataSet class.\n"
             << "Vector<double> get_input_instance(const unsigned int&) const method.\n"
             << "Index of instance must be less than number of instances.\n";

	  throw std::logic_error(buffer.str());
   }

   #endif

   const Vector<unsigned int>& inputs_indices = variables_information.get_inputs_indices();

   return(data.arrange_row(instance_index, inputs_indices));
}


// Vector<double> get_target_instance(const unsigned int&) const method

/// This method returns a vector with the target values of a single instance in the data set. 
/// @param instance_index Index of the instance. 

Vector<double> DataSet::get_target_instance(const unsigned int& instance_index) const
{
   // Control sentence (if debug)

   #ifdef _DEBUG 

   unsigned int instances_number = get_instances_number();

   if(instance_index >= instances_number)
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: DataSet class.\n"
             << "Vector<double> get_target_instance(const unsigned int&) const method.\n"
             << "Index of instance must be less than number of instances.\n";

	  throw std::logic_error(buffer.str());
   }

   #endif

   const Vector<unsigned int>& targets_indices = variables_information.get_targets_indices();

   return(data.arrange_row(instance_index, targets_indices));
}



// Vector<double> get_training_input_instance(const unsigned int&) const method

/// This method returns the input values of a given training instance.
/// @param training_instance_index Index of training instance.

Vector<double> DataSet::get_training_input_instance(const unsigned int& training_instance_index) const
{
   // Control sentence (if debug)

   #ifdef _DEBUG 

   unsigned int training_instances_number = instances_information.count_training_instances_number();  

   if(training_instance_index >= training_instances_number)
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: DataSet class.\n"
             << "Vector<double> get_training_input_instance(const unsigned int&) const method.\n"
             << "Index of training instance (" << training_instance_index << ") must be less than number of training instances (" << training_instances_number << ").\n";

	  throw std::logic_error(buffer.str());
   }

   #endif

   const Vector<unsigned int>& inputs_indices = variables_information.get_inputs_indices();

   const Vector<unsigned int>& training_indices = instances_information.get_training_indices();

   return(data.arrange_row(training_indices[training_instance_index], inputs_indices));
}


// Vector<double> get_generalization_input_instance(const unsigned int&) const method

/// This method returns the input values of a given generalization instance.
/// @param generalization_instance_index Index of generalization instance.

Vector<double> DataSet::get_generalization_input_instance(const unsigned int& generalization_instance_index) const
{
   // Control sentence (if debug)

   #ifdef _DEBUG 

   const unsigned int generalization_instances_number = instances_information.count_generalization_instances_number();
 
   if(generalization_instance_index >= generalization_instances_number)
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: DataSet class.\n"
             << "Vector<double> get_generalization_input_instance(const unsigned int&) const method.\n"
             << "Index of generalization instance must be less than number of generalization instances.\n";

	  throw std::logic_error(buffer.str());
   }

   #endif

   const Vector<unsigned int>& inputs_indices = variables_information.get_inputs_indices();

   const Vector<unsigned int>& generalization_indices = instances_information.get_generalization_indices();

   return(data.arrange_row(generalization_indices[generalization_instance_index], inputs_indices));
}


// Vector<double> get_testing_input_instance(const unsigned int&) const method

/// This method returns the input values of a given testing instance.
/// @param testing_instance_index Index of testing instance.

Vector<double> DataSet::get_testing_input_instance(const unsigned int& testing_instance_index) const
{
   // Control sentence (if debug)

   #ifdef _DEBUG 

   const unsigned int testing_instances_number = instances_information.count_testing_instances_number();

   if(testing_instance_index >= testing_instances_number)
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: DataSet class.\n"
             << "Vector<double> get_testing_input_instance(const unsigned int&) const method.\n"
             << "Index of testing instance must be less than number of testing instances.\n";

	  throw std::logic_error(buffer.str());
   }

   #endif

   const Vector<unsigned int>& testing_indices = instances_information.get_testing_indices();

   const Vector<unsigned int>& inputs_indices = variables_information.get_inputs_indices();

   return(data.arrange_row(testing_indices[testing_instance_index], inputs_indices));
}

   
// Vector<double> get_training_target_instance(const unsigned int&) const method

/// This method returns the target values of a given training instance.
/// @param training_instance_index Index of training instance.

Vector<double> DataSet::get_training_target_instance(const unsigned int& training_instance_index) const
{
   // Control sentence (if debug)

   #ifdef _DEBUG 

   unsigned int training_instances_number = instances_information.count_training_instances_number();

   if(training_instance_index >= training_instances_number)
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: DataSet class.\n"
             << "Vector<double> get_training_target_instance(const unsigned int&) const method.\n"
             << "Index of training instance must be less than number of training instances.\n";

	  throw std::logic_error(buffer.str());
   }

   #endif

   const Vector<unsigned int>& targets_indices = variables_information.get_targets_indices();

   const Vector<unsigned int>& training_indices = instances_information.get_training_indices();

   return(data.arrange_row(training_indices[training_instance_index], targets_indices));
}


// Vector<double> get_generalization_target_instance(const unsigned int&) const method

/// This method returns the target values of a given generalization instance.
/// @param generalization_instance_index Index of generalization instance.

Vector<double> DataSet::get_generalization_target_instance(const unsigned int& generalization_instance_index) const
{
   // Control sentence (if debug)

   #ifdef _DEBUG 

   const unsigned int generalization_instances_number = instances_information.count_generalization_instances_number();

   if(generalization_instance_index >= generalization_instances_number)
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: DataSet class.\n"
             << "Vector<double> get_generalization_target_instance(const unsigned int&) const method.\n"
             << "Index of generalization instance must be less than number of generalization instances.\n";

	  throw std::logic_error(buffer.str());
   }

   #endif

   const Vector<unsigned int>& targets_indices = variables_information.get_targets_indices();

   const Vector<unsigned int>& generalization_indices = instances_information.get_generalization_indices();

   return(data.arrange_row(generalization_indices[generalization_instance_index], targets_indices));
}


// Vector<double> get_testing_target_instance(const unsigned int&) const method

/// This method returns the target values of a given testing instance.
/// @param testing_instance_index Index of testing instance.

Vector<double> DataSet::get_testing_target_instance(const unsigned int& testing_instance_index) const
{
   // Control sentence (if debug)

   #ifdef _DEBUG 

   const unsigned int testing_instances_number = instances_information.count_testing_instances_number();

   if(testing_instance_index >= testing_instances_number)
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: DataSet class.\n"
             << "Vector<double> get_testing_target_instance(const unsigned int&) const method.\n"
             << "Index of testing instance must be less than number of testing instances.\n";

	  throw std::logic_error(buffer.str());
   }

   #endif

   const Vector<unsigned int>& targets_indices = variables_information.get_targets_indices();

   const Vector<unsigned int>& testing_indices = instances_information.get_testing_indices();

   return(data.arrange_row(testing_indices[testing_instance_index], targets_indices));
}


// Vector<double> get_variable(const unsigned int&) const method

/// This method returns all the instances of a single variable in the data set. 
/// @param i Index of the variable. 

Vector<double> DataSet::get_variable(const unsigned int& i) const
{
   // Control sentence (if debug)

   #ifdef _DEBUG 

   unsigned int variables_number = get_variables_number();

   if(i >= variables_number)
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: DataSet class.\n"
             << "Vector<double> get_variable(const unsigned int&) const method.\n"
             << "Index of variable must be less than number of instances.\n";

	  throw std::logic_error(buffer.str());
   }

   #endif

   // Get variable

   return(data.arrange_column(i));
}


// void set(void) method

/// This method sets zero instances and zero variables in the data set. 

void DataSet::set(void)
{
   data_filename = "";

   data.set();

   instances_information.set();
   variables_information.set();

   display = true;
}


// void set(const unsigned int&, const unsigned int&) method

/// This method sets new numbers of instances and variables in the inputs targets data set. 
/// All the instances are set for training. 
/// All the variables are set as inputs. 
/// @param new_instances_number Number of instances.
/// @param new_variables_number Number of variables.

void DataSet::set(const unsigned int& new_instances_number, const unsigned int& new_variables_number)
{
   data_filename = "";

   data.set(new_instances_number, new_variables_number);

   instances_information.set(new_instances_number);

   variables_information.set(new_variables_number);

   display = true;
}


// void set(const unsigned int&, const unsigned int&, const unsigned int&) method

/// This method sets new numbers of instances and inputs and target variables in the data set.
/// All the instances are set for training. 
/// The variables in the data set are the number of inputs plus the number of targets. 
/// The first variables in the data matrix correspond to the input variables.
/// The last variables in the data matrix correspond to the target variables. 
///
/// @param new_instances_number Number of instances.
/// @param new_inputs_number Number of input variables.
/// @param new_targets_number Number of target variables.

void DataSet::set(const unsigned int& new_instances_number, const unsigned int& new_inputs_number, const unsigned int& new_targets_number)
{   
   data_filename = "";

   const unsigned int new_variables_number = new_inputs_number + new_targets_number; 

   data.set(new_instances_number, new_variables_number);

   instances_information.set(new_instances_number);

   variables_information.set(new_inputs_number, new_targets_number);

   display = true;
}


// void set(const DataSet& other_data_set)

/// This method sets the members of this data set object with those from another data set object. 
/// @param other_data_set Data set object to be copied. 

void DataSet::set(const DataSet& other_data_set)
{
   data_filename = other_data_set.data_filename;

   data = other_data_set.data;

   variables_information = other_data_set.variables_information;

   instances_information = other_data_set.instances_information;

   display = other_data_set.display;
}


// void set(TiXmlElement*) method

/// This method sets the data set members from a XML element. 
/// @param data_set_element Pointer to a Tiny XML element containing the member data. 

void DataSet::set(TiXmlElement* data_set_element)
{
   from_XML(data_set_element);
}


// void set(const std::string&) method

/// This method sets the data set members by loading them from a XML file. 
/// @param filename Data set XML filename.

void DataSet::set(const std::string& filename)
{
   load(filename);
}


// void set_scaling_unscaling_method(const ScalingUnscalingMethod&) method

/// This method sets the method for scaling and uscaling the data. 
/// @param new_scaling_unscaling_method Scaling and unscaling method. 

void DataSet::set_scaling_unscaling_method(const ScalingUnscalingMethod& new_scaling_unscaling_method)
{
   scaling_unscaling_method = new_scaling_unscaling_method;
}


// void set_scaling_unscaling_method(const std::string&) method

/// This method sets the method for scaling and uscaling the data from a string containing the name of the method. 
/// @param new_scaling_unscaling_method Scaling and unscaling method. 

void DataSet::set_scaling_unscaling_method(const std::string& new_scaling_unscaling_method)
{
   if(new_scaling_unscaling_method == "MinimumMaximum")
   {
      set_scaling_unscaling_method(MinimumMaximum);
   }
   else if(new_scaling_unscaling_method == "MeanStandardDeviation")
   {
      set_scaling_unscaling_method(MinimumMaximum);
   }
   else
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: DataSet class.\n"
             << "void set_conditions_method(const std::string&) method.\n"
			 << "Unknown scaling and unscaling method: " << new_scaling_unscaling_method << ".\n";

	  throw std::logic_error(buffer.str());
   }
}


// void set_display(const bool&) method

/// This method sets a new display value. 
/// If it is set to true messages from this class are to be displayed on the screen;
/// if it is set to false messages from this class are not to be displayed on the screen.
/// @param new_display Display value.

void DataSet::set_display(const bool& new_display)
{
   display = new_display;
}


// void set_default(void) method

/// This method sets the default member values:
/// <ul>
/// <li> Scaling and uscaling method: Minimum and maximum.
/// <li> Display: True.
/// </ul>

void DataSet::set_default(void)
{
   scaling_unscaling_method = MinimumMaximum;   
  
   display = true;
}


// void set_data(const Matrix<double>&) method

/// This method sets a new data matrix. 
/// The number of rows must be equal to the number of instances.
/// The number of columns must be equal to the number of variables. 
/// Indices of all training, generalization and testing instances and inputs and target variables do not change.
/// @param new_data Data matrix.

void DataSet::set_data(const Matrix<double>& new_data)
{
   // Control sentence (if debug)

   #ifdef _DEBUG 

   const unsigned int rows_number = new_data.get_rows_number();
   const unsigned int instances_number = get_instances_number();

   if(rows_number != instances_number)
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: DataSet class.\n"
             << "void set_data(const Matrix<double>&) method.\n"
             << "Number of rows must be equal to number of instances.\n";

	  throw std::logic_error(buffer.str());
   }

   const unsigned int columns_number = new_data.get_columns_number();
   const unsigned int variables_number = get_variables_number();

   if(columns_number != variables_number)
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: DataSet class.\n"
             << "void set_data(const Matrix<double>&) method.\n"
             << "Number of columns must be equal to number of variables.\n";

	  throw std::logic_error(buffer.str());
   }

   #endif

   // Set data
   
   data = new_data;   
}


// void set_data_filename(const std::string&) method

/// This method sets the name of the data file.
/// It also loads the data from that file. 
/// Moreover, it sets the variables and instances information objects. 
/// @param new_data_filename Name of the file containing the data. 

void DataSet::set_data_filename(const std::string& new_data_filename)
{   
   data_filename = new_data_filename;   

   try
   {
      data.load(data_filename);

      const unsigned int variables_number = get_variables_number();

      const unsigned int instances_number = get_instances_number();

      variables_information.set(variables_number);
      instances_information.set(instances_number);
   }
   catch(std::exception& e)
   {
      std::cout << e.what() << std::endl;		 
   }   
}


// void set_instances_number(unsigned int) method

/// This method sets a new number of instances in the data set. 
/// All instances are also set for training. 
/// The indices of the inputs and target variables do not change. 
/// @param new_instances_number Number of instances. 

void DataSet::set_instances_number(const unsigned int& new_instances_number)
{
   const unsigned int variables_number = get_variables_number();

   data.set(new_instances_number, variables_number);

   instances_information.set(new_instances_number);
}


// void set_variables_number(unsigned int) method

/// This method sets a new number of input variables in the data set. 
/// The indices of the training, generalization and testing instances do not change. 
/// All variables are set as inputs. 
/// @param new_variables_number Number of variables. 

void DataSet::set_variables_number(const unsigned int& new_variables_number)
{
   const unsigned int instances_number = get_instances_number();

   data.set(instances_number, new_variables_number);

   variables_information.set(new_variables_number);
}


// void set_instance(const unsigned int&, const Vector<double>&)

/// This method sets new inputs and target values of a single instance in the data set. 
/// @param instance_index Index of the instance. 
/// @param instance New inputs and target values of the instance.

void DataSet::set_instance(const unsigned int& instance_index, const Vector<double>& instance)
{
   // Control sentence (if debug)

   #ifdef _DEBUG 

   const unsigned int instances_number = get_instances_number();

   if(instance_index >= instances_number)
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: DataSet class.\n"
             << "void set_instance(const unsigned int&, const Vector<double>&) method.\n"
             << "Index of instance must be less than number of instances.\n";

	  throw std::logic_error(buffer.str());
   }

   const unsigned int size = instance.size();
   const unsigned int variables_number = get_variables_number();

   if(size != variables_number)
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: DataSet class.\n"
             << "void set_instance(const unsigned int&, const Vector<double>&) method.\n"
             << "Size (" << size << ") must be equal to number of variables (" << variables_number << ").\n";

	  throw std::logic_error(buffer.str());
   } 

   #endif

   // Set instance

   data.set_row(instance_index, instance);
}


// void set_training_instance(const unsigned int&, const Vector<double>&) method

/// This method sets new inputs and target values of a single training instance in the data set. 
/// @param i Index of the training instance. 
/// @param new_training_instance New inputs and target values of the training instance.

void DataSet::set_training_instance(const unsigned int& i, const Vector<double>& new_training_instance)
{
   // Control sentence (if debug)

   #ifdef _DEBUG 

   const unsigned int training_instances_number = instances_information.count_training_instances_number();

   if(i >= training_instances_number)
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: DataSet class.\n"
             << "void set_training_instance(const unsigned int&, const Vector<double>&) method.\n"
             << "Training instance index must be less than number of training instances.\n";

	  throw std::logic_error(buffer.str());
   }
   
   #endif

   const Vector<unsigned int>& training_indices = instances_information.get_training_indices();

   const unsigned int index = training_indices[i];

   set_instance(index, new_training_instance);
}


// void set_generalization_instance(const unsigned int&, const Vector<double>&) method

/// This method sets new inputs and target values of a single generalization instance in the data set. 
/// @param i Index of the generalization instance. 
/// @param new_generalization_instance New inputs and target values of the generalization instance.

void DataSet::set_generalization_instance(const unsigned int& i, const Vector<double>& new_generalization_instance)
{
   // Control sentence (if debug)

   #ifdef _DEBUG 

   const unsigned int generalization_instances_number = instances_information.count_generalization_instances_number();

   if(i >= generalization_instances_number)
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: DataSet class.\n"
             << "void set_generalization_instance(const unsigned int&, const Vector<double>&) method.\n"
             << "Generalization instance index must be less than number of generalization instances.\n";

	  throw std::logic_error(buffer.str());
   }
   
   #endif

   const Vector<unsigned int>& generalization_indices = instances_information.get_generalization_indices();

   const unsigned int index = generalization_indices[i];

   set_instance(index, new_generalization_instance);
}


// void set_testing_instance(const unsigned int&, const Vector<double>&) method

/// This method sets new inputs and target values of a single testing instance in the data set. 
/// @param i Index of the testing instance. 
/// @param new_testing_instance New inputs and target values of the testing instance.

void DataSet::set_testing_instance(const unsigned int& i, const Vector<double>& new_testing_instance)
{
   // Control sentence (if debug)

   #ifdef _DEBUG 

   const unsigned int testing_instances_number = instances_information.count_testing_instances_number();

   if(i >= testing_instances_number)
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: DataSet class.\n"
             << "void set_testing_instance(const unsigned int&, const Vector<double>&) method.\n"
             << "Testing instance index must be less than number of testing instances.\n";

	  throw std::logic_error(buffer.str());
   }
   
   #endif

   const Vector<unsigned int>& testing_indices = instances_information.get_testing_indices();

   const unsigned int index = testing_indices[i];

   set_instance(index, new_testing_instance);
}


// void set_input_instance(const unsigned int&, const Vector<double>&) method

/// This method sets new input values of a single instance in the data set. 
/// @param instance_index Index of the instance. 
/// @param input_instance New input values of the instance.

void DataSet::set_input_instance(const unsigned int& instance_index, const Vector<double>& input_instance)
{
   // Control sentence (if debug)

   #ifdef _DEBUG 

   const unsigned int instances_number = get_instances_number();

   if(instance_index >= instances_number)
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: DataSet class.\n"
             << "void set_input_instance(const unsigned int&, const Vector<double>&) method.\n"
             << "Index of instance must be less than number of instances.\n";

	  throw std::logic_error(buffer.str());
   }

   #endif

   const unsigned int inputs_number = variables_information.count_inputs_number();

   #ifdef _DEBUG 

   const unsigned int size = input_instance.size();

   if(size != inputs_number)
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: DataSet class.\n"
             << "void set_input_instance(const unsigned int&, const Vector<double>&) method.\n"
             << "Size of inputs instance must be equal to number of input variables.\n";

	  throw std::logic_error(buffer.str());
   }

   #endif

   const Vector<unsigned int>& inputs_indices = variables_information.get_inputs_indices();

   unsigned int input_variable_index;

   for(unsigned int i = 0; i < inputs_number; i++)
   {
      input_variable_index = inputs_indices[i];

      data[instance_index][input_variable_index] = input_instance[i];
   }
}


// void set_target_instance(const unsigned int&, const Vector<double>&) method

/// This method sets new target values of a single instance in the data set. 
/// @param instance_index Index of the instance. 
/// @param target_instance New target values of the instance.

void DataSet::set_target_instance(const unsigned int& instance_index, const Vector<double>& target_instance)
{
   // Control sentence (if debug)

   #ifdef _DEBUG 

   const unsigned int instances_number = get_instances_number();

   if(instance_index >= instances_number)
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: DataSet class.\n"
             << "void set_target_instance(const unsigned int&, const Vector<double>&) method.\n"
             << "Index of instance must be less than number of instances.\n";

	  throw std::logic_error(buffer.str());
   }

   #endif

   const unsigned int targets_number = variables_information.count_targets_number();

   #ifdef _DEBUG 

   const unsigned int size = target_instance.size();

   if(size != targets_number)
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: DataSet class.\n"
             << "void set_target_instance(const unsigned int&, const Vector<double>&) method.\n"
             << "Size of targets instance must be equal to number of target variables.\n";

	  throw std::logic_error(buffer.str());
   }

   #endif

   const Vector<unsigned int>& targets_indices = variables_information.get_targets_indices();

   unsigned int target_variable_index;

   for(unsigned int i = 0; i < targets_number; i++)
   {
      target_variable_index = targets_indices[i];

      data[instance_index][target_variable_index] = target_instance[i];
   }
}


// void set_training_input_instance(const unsigned int&, const Vector<double>&) method

/// This method sets new input values of a single training instance in the data set. 
/// @param i Index of the training instance. 
/// @param new_training_input_instance New input values of that instance.

void DataSet::set_training_input_instance(const unsigned int& i, const Vector<double>& new_training_input_instance)
{
   // Control sentence (if debug)

   #ifdef _DEBUG 

   const unsigned int training_instances_number = instances_information.count_training_instances_number();

   if(i >= training_instances_number)
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: DataSet class.\n"
             << "void set_training_input_instance(const unsigned int&, const Vector<double>&) method.\n"
             << "Training instance index must be less than number of training instances.\n";

	  throw std::logic_error(buffer.str());
   }
   
   #endif

   const Vector<unsigned int>& training_indices = instances_information.get_training_indices();

   const unsigned int index = training_indices[i];

   set_input_instance(index, new_training_input_instance);
}


// void set_generalization_input_instance(const unsigned int&, const Vector<double>&) method

/// This method sets new input values of a single generalization instance in the data set. 
/// @param i Index of the generalization instance. 
/// @param new_generalization_input_instance New input values of that instance.

void DataSet::set_generalization_input_instance(const unsigned int& i, const Vector<double>& new_generalization_input_instance)
{
   // Control sentence (if debug)

   #ifdef _DEBUG 

   const unsigned int generalization_instances_number = instances_information.count_generalization_instances_number();

   if(i >= generalization_instances_number)
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: DataSet class.\n"
             << "void set_generalization_input_instance(const unsigned int&, const Vector<double>&) method.\n"
             << "Generalization instance index must be less than number of generalization instances.\n";

	  throw std::logic_error(buffer.str());
   }
   
   #endif

   const Vector<unsigned int>& generalization_indices = instances_information.get_generalization_indices();

   const unsigned int index = generalization_indices[i];

   set_input_instance(index, new_generalization_input_instance);
}


// void set_testing_input_instance(const unsigned int&, const Vector<double>&) method

/// This method sets new input values of a single testing instance in the data set. 
/// @param i Index of the testing instance. 
/// @param new_testing_input_instance New input values of that instance.

void DataSet::set_testing_input_instance(const unsigned int& i, const Vector<double>& new_testing_input_instance)
{   
   // Control sentence (if debug)

   #ifdef _DEBUG 

   const unsigned int testing_instances_number = instances_information.count_testing_instances_number();

   if(i >= testing_instances_number)
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: DataSet class.\n"
             << "void set_testing_input_instance(const unsigned int&, const Vector<double>&) method.\n"
             << "Testing instance index must be less than number of testing instances.\n";

	  throw std::logic_error(buffer.str());
   }
   
   #endif

   const Vector<unsigned int>& testing_indices = instances_information.get_testing_indices();

   const unsigned int index = testing_indices[i];

   set_input_instance(index, new_testing_input_instance);
}


// void set_training_target_instance(const unsigned int&, const Vector<double>&) method

/// This method sets new target values of a single training instance in the data set. 
/// @param i Index of the training instance. 
/// @param new_training_target_instance New target values of that instance.

void DataSet::set_training_target_instance(const unsigned int& i, const Vector<double>& new_training_target_instance)
{
   // Control sentence (if debug)

   #ifdef _DEBUG 

   const unsigned int training_instances_number = instances_information.count_training_instances_number();

   if(i >= training_instances_number)
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: DataSet class.\n"
             << "void set_training_target_instance(const unsigned int&, const Vector<double>&) method.\n"
             << "Training instance index must be less than number of training instances.\n";

	  throw std::logic_error(buffer.str());
   }
   
   #endif

   const Vector<unsigned int>& training_indices = instances_information.get_training_indices();

   const unsigned int index = training_indices[i];

   set_target_instance(index, new_training_target_instance);
}


// void set_generalization_target_instance(const unsigned int&, const Vector<double>&) method

/// This method sets new target values of a single generalization instance in the data set. 
/// @param i Index of the generalization instance. 
/// @param new_generalization_target_instance New target values of that instance.

void DataSet::set_generalization_target_instance(const unsigned int& i, const Vector<double>& new_generalization_target_instance)
{
   // Control sentence (if debug)

   #ifdef _DEBUG 

   const unsigned int generalization_instances_number = instances_information.count_generalization_instances_number();

   if(i >= generalization_instances_number)
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: DataSet class.\n"
             << "void set_generalization_target_instance(const unsigned int&, const Vector<double>&) method.\n"
             << "Generalization instance index must be less than number of generalization instances.\n";

	  throw std::logic_error(buffer.str());
   }
   
   #endif

   const Vector<unsigned int>& generalization_indices = instances_information.get_generalization_indices();

   const unsigned int index = generalization_indices[i];

   set_target_instance(index, new_generalization_target_instance);
}


// void set_testing_target_instance(const unsigned int&, const Vector<double>&) method

/// This method sets new target values of a single testing instance in the data set. 
/// @param i Index of the testing instance. 
/// @param new_testing_target_instance New target values of that instance.

void DataSet::set_testing_target_instance(const unsigned int& i, const Vector<double>& new_testing_target_instance)
{
   // Control sentence (if debug)

   #ifdef _DEBUG 

   const unsigned int testing_instances_number = instances_information.count_testing_instances_number();

   if(i >= testing_instances_number)
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: DataSet class.\n"
             << "void set_testing_target_instance(const unsigned int&, const Vector<double>&) method.\n"
             << "Testing instance index must be less than number of testing instances.\n";

	  throw std::logic_error(buffer.str());
   }
   
   #endif

   const Vector<unsigned int>& testing_indices = instances_information.get_testing_indices();

   const unsigned int index = testing_indices[i];

   set_target_instance(index, new_testing_target_instance);
}


// void add_instance(const Vector<double>&) method

/// This method adds a new instance to the data matrix from a vector of real numbers.
/// The size of that vector must be equal to the number of variables. 
/// Note that resizing is here necessary and therefore computationally expensive. 
/// All instances are also set for training. 
/// @param instance Input and target values of the instance to be added. 

void DataSet::add_instance(const Vector<double>& instance)
{
   // Control sentence (if debug)

   #ifdef _DEBUG 

   const unsigned int size = instance.size();
   const unsigned int variables_number = get_variables_number();

   if(size != variables_number)
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: DataSet class.\n"
             << "void add_instance(const Vector<double>&) method.\n"
             << "Size of instance must be equal to number of variables.\n";

	  throw std::logic_error(buffer.str());
   }

   #endif

   data.append_row(instance);

   instances_information.set_training();
}


// void subtract_instance(unsigned int) method

/// This method substract the inputs-targets instance with a given index from the data set.
/// All instances are also set for training. 
/// Note that resizing is here necessary and therefore computationally expensive. 
/// @param instance_index Index of instance to be removed. 

void DataSet::subtract_instance(const unsigned int& instance_index)
{
   // Control sentence (if debug)

   #ifdef _DEBUG 

   const unsigned int instances_number = get_instances_number();

   if(instance_index >= instances_number)
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: DataSet class.\n"
             << "void subtract_instance(unsigned int) method.\n"
             << "Index of instance must be less than number of instances.\n";

	  throw std::logic_error(buffer.str());
   }

   #endif

   data.subtract_row(instance_index);

   instances_information.set_training();
}


// void append_variable(const Vector<double>&) method

/// This method appends a variable with given values to the data matrix.
/// @param variable Vector of values. The size must be equal to the number of instances. 

void DataSet::append_variable(const Vector<double>& variable)
{
   // Control sentence (if debug)

   #ifdef _DEBUG 

   const unsigned int size = variable.size();
   const unsigned int instances_number = get_instances_number();

   if(size != instances_number)
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: DataSet class.\n"
             << "void append_variable(const Vector<double>&) method.\n"
             << "Size of variable must be equal to number of instances.\n";

	  throw std::logic_error(buffer.str());
   }

   #endif

   const unsigned int variables_number = get_variables_number();

   data.append_column(variable);

   Matrix<double> new_data(data);

   const unsigned int new_variables_number = variables_number + 1;

   set_variables_number(new_variables_number);

   set_data(new_data);
}


// void subtract_variable(unsigned int) method

/// This method removes a variable with given index from the data matrix.
/// @param variable_index Index of variable to be subtracted. 

void DataSet::subtract_variable(const unsigned int& variable_index)
{
   const unsigned int variables_number = get_variables_number();

   // Control sentence (if debug)

   #ifdef _DEBUG 

   if(variable_index >= variables_number)
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: DataSet class.\n"
             << "void subtract_variable(unsigned int) method.\n"
             << "Index of variable must be less than number of variables.\n";

	  throw std::logic_error(buffer.str());
   }

   #endif

   data.subtract_column(variable_index);

   Matrix<double> new_data(data);

   const unsigned int new_variables_number = variables_number - 1;

   set_variables_number(new_variables_number);

   set_data(new_data);
}


// void subtract_constant_variables(void) method

/// @todo

void DataSet::subtract_constant_variables(void)
{
}


// void subtract_repeated_instances(void) method

/// @todo

void DataSet::subtract_repeated_instances(void)
{

}


// Vector< Vector< Vector<double> > > calculate_data_histogram(const unsigned int&) const method

/// This method returns a histogram for each variable with a given number of bins. 
/// The format is a vector of subvectors of subsubvectors.
/// The size of the vector is the number of variables. 
/// @param bins_number Number of bins.

Vector< Vector< Vector<double> > > DataSet::calculate_data_histogram(const unsigned int& bins_number) const
{
   return(data.calculate_histogram(bins_number));
}


// Vector< Vector< Vector<double> > > calculate_data_histogram(void) const method

/// This method returns a histogram for each variable with a default number of bins. 
/// The format is a vector of subvectors of subsubvectors.
/// The size of the vector is the number of variables. 

Vector< Vector< Vector<double> > > DataSet::calculate_data_histogram(void) const
{
   return(data.calculate_histogram());  
}


// Vector< Vector<double> > calculate_data_statistics(void) const method

/// This method returns a vector of vectors containing some basic statistics of all the variables in the data set. 
/// The size of this vector is four. The subvectors are:
/// <ul>
/// <li> Minimum.
/// <li> Maximum.
/// <li> Mean.
/// <li> Standard deviation.
/// </ul> 

Vector< Vector<double> > DataSet::calculate_data_statistics(void) const
{
   Vector< Vector<double> > minimum_maximum = data.calculate_minimum_maximum();
   Vector< Vector<double> > mean_standard_deviation = data.calculate_mean_standard_deviation();

   Vector< Vector<double> > statistics(4);

   statistics[0] = minimum_maximum[0];
   statistics[1] = minimum_maximum[1];
   statistics[2] = mean_standard_deviation[0];
   statistics[3] = mean_standard_deviation[1];

   return(statistics);
}


// Vector< Vector<double> > calculate_training_instances_statistics(void) const method

/// This method returns a vector of vectors containing some basic statistics of all variables on the training instances. 
/// The size of this vector is four. The subvectors are:
/// <ul>
/// <li> Training data mean.

/// <li> Training data standard deviation.
/// <li> Training data minimum.
/// <li> Training data maximum.
/// </ul> 

Vector< Vector<double> > DataSet::calculate_training_instances_statistics(void) const
{
   const unsigned int variables_number = get_variables_number();
   const Vector<unsigned int> variables_indices(0, 1, variables_number-1);

   const Vector<unsigned int>& training_indices = instances_information.get_training_indices();

   const Vector< Vector<double> > mean_standard_deviation = data.calculate_mean_standard_deviation(training_indices, variables_indices);

   const Vector< Vector<double> > minimum_maximum = data.calculate_minimum_maximum(training_indices, variables_indices);

   Vector< Vector<double> > statistics(4);

   statistics[0] = mean_standard_deviation[0];
   statistics[1] = mean_standard_deviation[1];
   statistics[2] = minimum_maximum[0];
   statistics[3] = minimum_maximum[1];

   return(statistics);
}


// Vector< Vector<double> > calculate_generalization_instances_statistics(void) const method

/// This method returns a vector of vectors containing some basic statistics of all variables on the generalization instances. 
/// The size of this vector is four. The subvectors are:
/// <ul>
/// <li> Generalization data mean.
/// <li> Generalization data standard deviation.
/// <li> Generalization data minimum.
/// <li> Generalization data maximum.
/// </ul> 

Vector< Vector<double> > DataSet::calculate_generalization_instances_statistics(void) const
{
   const unsigned int variables_number = get_variables_number();
   const Vector<unsigned int> variables_indices(0, 1, variables_number-1);

   const Vector<unsigned int>& generalization_indices = instances_information.get_generalization_indices();

   const Vector< Vector<double> > mean_standard_deviation = data.calculate_mean_standard_deviation(generalization_indices, variables_indices);

   const Vector< Vector<double> > minimum_maximum = data.calculate_minimum_maximum(generalization_indices, variables_indices);

   Vector< Vector<double> > statistics(4);

   statistics[0] = mean_standard_deviation[0];
   statistics[1] = mean_standard_deviation[1];
   statistics[2] = minimum_maximum[0];
   statistics[3] = minimum_maximum[1];

   return(statistics);
}


// Vector< Vector<double> > calculate_testing_instances_statistics(void) const method

/// This method returns a vector of vectors containing some basic statistics of all variables on the testing instances. 
/// The size of this vector is four. The subvectors are:
/// <ul>
/// <li> Testing data mean.
/// <li> Testing data standard deviation.
/// <li> Testing data minimum.
/// <li> Testing data maximum.
/// </ul> 

Vector< Vector<double> > DataSet::calculate_testing_instances_statistics(void) const
{
   const unsigned int variables_number = get_variables_number();
   const Vector<unsigned int> variables_indices(0, 1, variables_number-1);

   const Vector<unsigned int>& testing_indices = instances_information.get_testing_indices();

   const Vector< Vector<double> > mean_standard_deviation = data.calculate_mean_standard_deviation(testing_indices, variables_indices);

   const Vector< Vector<double> > minimum_maximum = data.calculate_minimum_maximum(testing_indices, variables_indices);

   Vector< Vector<double> > statistics(4);

   statistics[0] = mean_standard_deviation[0];
   statistics[1] = mean_standard_deviation[1];
   statistics[2] = minimum_maximum[0];
   statistics[3] = minimum_maximum[1];

   return(statistics);
}


// Vector< Vector<double> > calculate_instances_statistics(void) const method

/// This method returns a vector of vectors containing some basic statistics of all variables on all data, 
/// training, generalization and testing instances. 
/// The size of this vector is twelve. The subvectors are:
/// <ul>
/// <li> Data mean.
/// <li> Data standard deviation.
/// <li> Data minimum.
/// <li> Data maximum.
/// <li> Training instances mean.
/// <li> Training instances standard deviation.
/// <li> Training instances minimum.
/// <li> Training instances maximum.
/// <li> Generalization instances mean.
/// <li> Generalization instances standard deviation.
/// <li> Generalization instances minimum.
/// <li> Generalization instances maximum.
/// <li> Testing instances mean.
/// <li> Testing instances standard deviation.
/// <li> Testing instances minimum.
/// <li> Testing instances maximum.
/// </ul> 

Vector< Vector<double> > DataSet::calculate_instances_statistics(void) const
{
   Vector< Vector<double> > data_statistics = calculate_data_statistics();
   Vector< Vector<double> > training_instances_statistics = calculate_training_instances_statistics();
   Vector< Vector<double> > generalization_instances_statistics = calculate_generalization_instances_statistics();
   Vector< Vector<double> > testing_instances_statistics = calculate_testing_instances_statistics();

   return(data_statistics.get_assembly(training_instances_statistics).get_assembly(generalization_instances_statistics).get_assembly(testing_instances_statistics));
}


// Vector< Vector<double> > calculate_inputs_statistics(void) const method

/// This method returns a vector of vectors with some basic statistics of the input variables on all instances. 
/// The size of this vector is four. The subvectors are:
/// <ul>
/// <li> Input variables mean.
/// <li> Input variables standard deviation.
/// <li> Input variables minimum.
/// <li> Input variables maximum.
/// </ul> 

Vector< Vector<double> > DataSet::calculate_inputs_statistics(void) const
{
   const Vector<unsigned int>& inputs_indices = variables_information.get_inputs_indices();

   const Vector< Vector<double> > mean_standard_deviation = data.calculate_mean_standard_deviation(inputs_indices);
   const Vector< Vector<double> > minimum_maximum = data.calculate_minimum_maximum(inputs_indices);

   Vector< Vector<double> > statistics(4);

   statistics[0] = mean_standard_deviation[0];
   statistics[1] = mean_standard_deviation[1];
   statistics[2] = minimum_maximum[0];
   statistics[3] = minimum_maximum[1];

   return(statistics);
}


// Vector< Vector<double> > calculate_targets_statistics(void) const method

/// This method returns a vector of vectors with some basic statistics of the target variables on all instances. 
/// The size of this vector is four. The subvectors are:
/// <ul>
/// <li> Target variables mean.
/// <li> Target variables standard deviation.
/// <li> Target variables minimum.
/// <li> Target variables maximum.
/// </ul> 

Vector< Vector<double> > DataSet::calculate_targets_statistics(void) const
{
   const Vector<unsigned int>& targets_indices = variables_information.get_targets_indices();

   const Vector< Vector<double> > mean_standard_deviation = data.calculate_mean_standard_deviation(targets_indices);
   const Vector< Vector<double> > minimum_maximum = data.calculate_minimum_maximum(targets_indices);

   Vector< Vector<double> > statistics(4);

   statistics[0] = mean_standard_deviation[0];
   statistics[1] = mean_standard_deviation[1];
   statistics[2] = minimum_maximum[0];
   statistics[3] = minimum_maximum[1];

   return(statistics);
}


// Vector< Vector<double> > calculate_inputs_targets_minimum_maximum(void) const method

/// This method returns a vector of vectors with some the minimum and maximum values of the inputs and target variables on all instances. 
/// The size of this vector is four. 
/// The subvectors are:
/// <ul>
/// <li> Input variables minimum.
/// <li> Input variables maximum.
/// <li> Target variables minimum.
/// <li> Target variables maximum.
/// </ul> 

Vector< Vector<double> > DataSet::calculate_inputs_targets_minimum_maximum(void) const
{
   const Vector<unsigned int>& inputs_indices = variables_information.get_inputs_indices();
   const Vector<unsigned int>& targets_indices = variables_information.get_targets_indices();

   const Vector< Vector<double> > inputs_minimum_maximum = data.calculate_minimum_maximum(inputs_indices);
   const Vector< Vector<double> > targets_minimum_maximum = data.calculate_minimum_maximum(targets_indices);
   
   return(inputs_minimum_maximum.get_assembly(targets_minimum_maximum));
}


// Vector< Vector<double> > calculate_inputs_targets_mean_standard_deviation(void) const method

/// This method returns a vector of vectors with the mean and standard deviation values of the inputs and target variables on all instances. 
/// The size of this vector is four. The subvectors are:
/// <ul>
/// <li> Input variables mean.
/// <li> Input variables standard deviation.
/// <li> Target variables mean.
/// <li> Target variables standard deviation.
/// </ul> 

Vector< Vector<double> > DataSet::calculate_inputs_targets_mean_standard_deviation(void) const
{
   const Vector<unsigned int>& inputs_indices = variables_information.get_inputs_indices();
   const Vector<unsigned int>& targets_indices = variables_information.get_targets_indices();

   const Vector< Vector<double> > inputs_mean_standard_deviation = data.calculate_mean_standard_deviation(inputs_indices);
   const Vector< Vector<double> > targets_mean_standard_deviation = data.calculate_mean_standard_deviation(targets_indices);

   return(inputs_mean_standard_deviation.get_assembly(targets_mean_standard_deviation));
}


// Vector< Vector<double> > calculate_inputs_targets_statistics(void) const method

/// This method returns a vector of vectors with some basic statistics of the inputs and target variables on all instances. 
/// The size of this vector is eight. The subvectors are:
/// <ul>
/// <li> Input variables mean.
/// <li> Input variables standard deviation.
/// <li> Input variables minimum.
/// <li> Input variables maximum.
/// <li> Target variables mean.
/// <li> Target variables standard deviation.
/// <li> Target variables minimum.
/// <li> Target variables maximum.
/// </ul> 

Vector< Vector<double> > DataSet::calculate_inputs_targets_statistics(void) const
{
   const Vector<unsigned int>& inputs_indices = variables_information.get_inputs_indices();
   const Vector<unsigned int>& targets_indices = variables_information.get_targets_indices();

   Vector< Vector<double> > statistics(8);

   // Input

   const Vector< Vector<double> > inputs_minimum_maximum = data.calculate_minimum_maximum(inputs_indices);
   const Vector< Vector<double> > inputs_mean_standard_deviation = data.calculate_mean_standard_deviation(inputs_indices);

   statistics[0] = inputs_minimum_maximum[0];
   statistics[1] = inputs_minimum_maximum[1];
   statistics[2] = inputs_mean_standard_deviation[0];
   statistics[3] = inputs_mean_standard_deviation[1];

   // Target

   const Vector< Vector<double> > targets_minimum_maximum = data.calculate_minimum_maximum(targets_indices);
   const Vector< Vector<double> > targets_mean_standard_deviation = data.calculate_mean_standard_deviation(targets_indices);
   
   statistics[4] = targets_minimum_maximum[0];
   statistics[5] = targets_minimum_maximum[1];
   statistics[6] = targets_mean_standard_deviation[0];
   statistics[7] = targets_mean_standard_deviation[1];

   return(statistics);
}


// Vector<double> calculate_training_target_data_mean(void) const method

/// This method returns the mean values of the target variables on the training instances. 

Vector<double> DataSet::calculate_training_target_data_mean(void) const
{
   const Vector<unsigned int>& targets_indices = variables_information.get_targets_indices();

   const Vector<unsigned int>& training_indices = instances_information.get_training_indices();

   return(data.calculate_mean(training_indices, targets_indices));
}


// Vector<double> calculate_generalization_target_data_mean(void) const method

/// This method returns the mean values of the target variables on the generalization instances. 

Vector<double> DataSet::calculate_generalization_target_data_mean(void) const
{
   const Vector<unsigned int>& targets_indices = variables_information.get_targets_indices();

   const Vector<unsigned int>& generalization_indices = instances_information.get_generalization_indices();

   return(data.calculate_mean(generalization_indices, targets_indices));
}


// Vector<double> calculate_testing_target_data_mean(void) const method

/// This method returns the mean values of the target variables on the testing instances. 

Vector<double> DataSet::calculate_testing_target_data_mean(void) const
{
   const Vector<unsigned int>& testing_indices = instances_information.get_testing_indices();

   const Vector<unsigned int>& targets_indices = variables_information.get_targets_indices();

   return(data.calculate_mean(testing_indices, targets_indices));
}


// Matrix<double> calculate_variables_correlation(void) const method

/// @todo

Matrix<double> DataSet::calculate_variables_correlation(void) const
{
   Matrix<double> variables_correlation;

   return(variables_correlation);
}


// void scale_data_mean_standard_deviation(const Vector<double>&, const Vector<double>&) const method

/// This method scales the data matrix with given mean and standard deviation values. 
/// It updates the data matrix.
/// @param means Mean of columns.
/// @param standard_deviations Standard deviation of columns.

void DataSet::scale_data_mean_standard_deviation(const Vector<double>& means, const Vector<double>& standard_deviations)
{
   // Control sentence (if debug)

   #ifdef _DEBUG 

   std::ostringstream buffer;

   const unsigned int columns_number = data.get_columns_number();

   const unsigned int means_size = means.size();
   const unsigned int standard_deviations_size = standard_deviations.size();

   if(means_size != columns_number)
   {
      buffer << "OpenNN Exception: DataSet class.\n"
             << "void scale_data_mean_standard_deviation(const Vector<double>&, const Vector<double>&) method.\n" 
             << "Size of means must be equal to number of columns.\n";

	  throw std::logic_error(buffer.str());
   }

   if(standard_deviations_size != columns_number)
   {
      buffer << "OpenNN Exception: DataSet class.\n"
             << "void scale_data_mean_standard_deviation(const Vector<double>&, const Vector<double>&) method.\n" 
             << "Size of standard deviations must be equal to number of columns.\n";

	  throw std::logic_error(buffer.str());
   }

   #endif

   data.scale_mean_standard_deviation(means, standard_deviations);
}


// void scale_data_minimum_maximum(const Vector<double>&, const Vector<double>&) method

/// This method scales the data matrix with given minimum and maximum values. 
/// It updates the data matrix.
/// @param minimums Minimum values for all variables.
/// @param maximums Maximum values for all variables.

void DataSet::scale_data_minimum_maximum(const Vector<double>& minimums, const Vector<double>& maximums)
{
   // Control sentence (if debug)

   #ifdef _DEBUG 

   std::ostringstream buffer;

   const unsigned int columns_number = data.get_columns_number();

   const unsigned int minimums_size = minimums.size();
   const unsigned int maximums_size = maximums.size();

   if(minimums_size != columns_number)
   {
      buffer << "OpenNN Exception: DataSet class.\n"
             << "void scale_data_minimum_maximum(const Vector<double>&, const Vector<double>&) method.\n" 
             << "Size of minimums must be equal to number of columns.\n";

	  throw std::logic_error(buffer.str());
   }

   if(maximums_size != columns_number)
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: DataSet class.\n"
             << "void scale_data_minimumn_maximum(const Vector<double>&, const Vector<double>&) method.\n" 
             << "Size of maximums must be equal to number of columns.\n";

	  throw std::logic_error(buffer.str());
   }

   #endif

   data.scale_minimum_maximum(minimums, maximums);
}


// void scale_data(const Vector< Vector<double> >&) method

/// This method scales the data matrix. 
/// The method to be used is that in the scaling and unscaling method variable. 
/// @param statistics Vector of vectors with the basic statistics. 

void DataSet::scale_data(const Vector< Vector<double> >& statistics)
{
   const Vector<double>& minimums = statistics[0];   
   const Vector<double>& maximums = statistics[0];   

   const Vector<double>& means = statistics[0];   
   const Vector<double>& standard_deviations = statistics[0];   

   switch(scaling_unscaling_method)
   {
      case MinimumMaximum:
      {
         scale_data_minimum_maximum(minimums, maximums);
      }            
      break;

      case MeanStandardDeviation:
      {
         scale_data_mean_standard_deviation(means, standard_deviations);
      }
      break;

      default:
      {
         std::ostringstream buffer;

         buffer << "OpenNN Exception: DataSet class\n"
                << "void scale_data(const Vector< Vector<double> >&) method.\n"
                << "Unknown scaling and unscaling method.\n";

	     throw std::logic_error(buffer.str());
      }
      break;
   }
}


// Vector< Vector<double> > scale_data(void) method

/// This method calculates the data statistics, scales the data with that values and returns the statistics. 
/// The method to be used is that in the scaling and unscaling method variable. 

Vector< Vector<double> > DataSet::scale_data(void)
{
   const Vector< Vector<double> >& statistics = data.calculate_statistics();

   const Vector<double>& minimums = statistics[0];   
   const Vector<double>& maximums = statistics[0];   

   const Vector<double>& means = statistics[0];   
   const Vector<double>& standard_deviations = statistics[0];   

   switch(scaling_unscaling_method)
   {
      case MinimumMaximum:
      {
         scale_data_minimum_maximum(minimums, maximums);
      }            
      break;

      case MeanStandardDeviation:
      {
         scale_data_mean_standard_deviation(means, standard_deviations);
      }
      break;

      default:
      {
         std::ostringstream buffer;

         buffer << "OpenNN Exception: DataSet class\n"
                << "const Vector< Vector<double> > scale_data(void) method.\n"
                << "Unknown scaling and unscaling method.\n";

	     throw std::logic_error(buffer.str());
      }
      break;
   }

   return(statistics);
}


// void scale_inputs_mean_standard_deviation(const Vector<double>&, const Vector<double>&) method

/// This method scales the input variables with given mean and standard deviation values.
/// It updates the input variables of the data matrix.
/// @param inputs_mean Mean values for the input variables to be used for scaling.
/// @param inputs_standard_deviation Standard deviation values for the input variables to be used for scaling.

void DataSet::scale_inputs_mean_standard_deviation(const Vector<double>& inputs_mean, const Vector<double>& inputs_standard_deviation)
{
   const unsigned int inputs_number = variables_information.count_inputs_number();

   // Control sentence (if debug)

   #ifdef _DEBUG 

   const unsigned int inputs_mean_size = inputs_mean.size();
   const unsigned int inputs_standard_deviation_size = inputs_standard_deviation.size();

   if(inputs_mean_size != inputs_number)
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: DataSet class.\n"
             << "void scale_inputs_mean_standard_deviation(const Vector<double>&, const Vector<double>&) method.\n" 
             << "Size of input variables mean must be equal to number of input variables.\n";

	  throw std::logic_error(buffer.str());

	  return;
   }

   if(inputs_standard_deviation_size != inputs_number)
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: DataSet class.\n"
             << "void scale_inputs_mean_standard_deviation(const Vector<double>&, const Vector<double>&) method.\n" 
             << "Size of input variables standard deviation must be equal to number of input variables.\n";

	  throw std::logic_error(buffer.str());

	  return;
   }

   #endif

   const Vector<unsigned int>& inputs_indices = variables_information.get_inputs_indices();

   const unsigned int instances_number = get_instances_number();

   unsigned int variable_index;

   // Rescale inputs data

   for(unsigned int j = 0; j < inputs_number; j++)
   {
      variable_index = inputs_indices[j];

      if(inputs_standard_deviation[j] < 1e-99)
      {
         if(display)
         {                                          
            std::cout << "OpenNN Warning: DataSet class.\n"
                      << "void scale_inputs_mean_standard_deviation(const Vector<double>&, const Vector<double>&) method.\n"
                      << "Standard deviation of input variable " << j << " is zero.\n"
                      << "Those inputs won't be scaled.\n";
         }

         // Do nothing
      }
      else
      {
         for(unsigned int i = 0; i < instances_number; i++)
         {
            data[i][variable_index] = (data[i][variable_index] - inputs_mean[j])/inputs_standard_deviation[j];
         }
      }
   }
}


// Vector< Vector<double> > scale_inputs_mean_standard_deviation(void) method

/// This method scales the input variables with the calculated mean and standard deviation values from the data matrix.
/// It updates the input variables of the data matrix.
/// It also returns a vector of vectors with the variables statistics. 

Vector< Vector<double> > DataSet::scale_inputs_mean_standard_deviation(void)
{
   const Vector< Vector<double> > inputs_targets_statistics = calculate_inputs_targets_statistics();

   const Vector<double>& inputs_means = inputs_targets_statistics[2];
   const Vector<double>& inputs_standard_deviations = inputs_targets_statistics[3];

   scale_inputs_mean_standard_deviation(inputs_means, inputs_standard_deviations);

   return(inputs_targets_statistics);
}


// void scale_inputs_minimum_maximum(const Vector<double>&, const Vector<double>&) method

/// This method scales the input variables with given minimum and maximum values.
/// It updates the input variables of the data matrix.
/// @param inputs_minimum Minimum values for the input variables to be used for scaling.
/// @param inputs_maximum Maximum values for the input variables to be used for scaling.

void DataSet::scale_inputs_minimum_maximum(const Vector<double>& inputs_minimum, const Vector<double>& inputs_maximum)
{
   const Vector<unsigned int>& inputs_indices = variables_information.get_inputs_indices();

   const unsigned int instances_number = get_instances_number();
   const unsigned int inputs_number = variables_information.count_inputs_number();

   unsigned int variable_index;

   for(unsigned int j = 0; j < inputs_number; j++)
   {
      variable_index = inputs_indices[j];

      if(inputs_maximum[j] - inputs_minimum[j] < 1e-99)
      {
         if(display)
         {
            std::cout << "OpenNN Warning: DataSet class.\n"
                      << "void scale_inputs_minimum_maximum(const Vector<double>&, const Vector<double>&) method.\n"
                      << "Minimum and maximum values of input variable " << j << " are equal. " 
                      << "Those inputs won't be scaled.\n";
         }
         
         // Do nothing
      }
      else
      {
         for(unsigned int i = 0; i < instances_number; i++)
         {
            data[i][variable_index] = 2.0*(data[i][variable_index] - inputs_minimum[j])/(inputs_maximum[j]-inputs_minimum[j])-1.0;
         }
      }
   }
}


// Vector< Vector<double> > scale_inputs_minimum_maximum(void) method

/// This method scales the input variables with the calculated minimum and maximum values from the data matrix.
/// It updates the input variables of the data matrix.
/// It also returns a vector of vectors with the minimum and maximum values of the input variables. 

Vector< Vector<double> > DataSet::scale_inputs_minimum_maximum(void)
{
   const Vector< Vector<double> > inputs_targets_statistics = calculate_inputs_targets_statistics();

   const Vector<double> & inputs_minimums = inputs_targets_statistics[0];
   const Vector<double> & inputs_maximums = inputs_targets_statistics[1];

   scale_inputs_minimum_maximum(inputs_minimums, inputs_maximums);

   return(inputs_targets_statistics);
}


// Vector< Vector<double> > scale_inputs(void) method

/// This method calculates the input and target variables statistics. 
/// Then it scales the input variables with that values.
/// The method to be used is that in the scaling and unscaling method variable. 
/// Finally, it returns the statistics. 

Vector< Vector<double> > DataSet::scale_inputs(void)
{
   switch(scaling_unscaling_method)
   {
      case MinimumMaximum:
      {
         return(scale_inputs_minimum_maximum());
      }            
      break;

      case MeanStandardDeviation:
      {
         return(scale_inputs_mean_standard_deviation());
      }
      break;

      default:
      {
         std::ostringstream buffer;

         buffer << "OpenNN Exception: DataSet class\n"
                << "Vector< Vector<double> > scale_inputs(void) method.\n"
                << "Unknown scaling and unscaling method.\n";

	     throw std::logic_error(buffer.str());
      }
      break;
   }
}


// void scale_targets_mean_standard_deviation(const Vector<double>&, const Vector<double>&)

/// This method scales the target variables with given mean and standard deviation values.
/// It updates the target variables of the data matrix.
///
/// @param targets_mean Mean values for the target variables to be used for scaling.
/// @param targets_standard_deviation Standard deviation values for the target variables to be used for scaling.

void DataSet::scale_targets_mean_standard_deviation(const Vector<double>& targets_mean, const Vector<double>& targets_standard_deviation)
{
   const unsigned int targets_number = variables_information.count_targets_number();

   // Control sentence (if debug)

   #ifdef _DEBUG 

   const unsigned int targets_mean_size = targets_mean.size();
   const unsigned int targets_standard_deviation_size = targets_standard_deviation.size();

   if(targets_mean_size != targets_number)
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: DataSet class.\n"
             << "void scale_targets_mean_standard_deviation(const Vector<double>&, const Vector<double>&) method.\n" 
             << "Size of target variables mean must be equal to number of target variables.\n";

	  throw std::logic_error(buffer.str());
   }

   if(targets_standard_deviation_size != targets_number)
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: DataSet class.\n"
             << "void scale_targets_mean_standard_deviation(const Vector<double>&, const Vector<double>&) method.\n" 
             << "Size of target variables standard deviation must be equal to number of target variables.\n";

	  throw std::logic_error(buffer.str());
   }

   #endif

   const Vector<unsigned int>& targets_indices = variables_information.get_targets_indices();

   const unsigned int instances_number = get_instances_number();

   unsigned int variable_index;

   // Rescale targets data

   for(unsigned int j = 0; j < targets_number; j++)
   {
      variable_index = targets_indices[j];

      if(targets_standard_deviation[j] < 1e-99)
      {
         if(display)
         {
            std::cout << "OpenNN Warning: DataSet class.\n"
                      << "void scale_targets_mean_standard_deviation(const Vector<double>&, const Vector<double>&) method.\n"   
                      << "Standard deviation of target variable " <<  j << " is zero.\n"
                      << "Those targets won't be scaled.\n";
         }
         
         // Do nothing
      }
      else
      {
         for(unsigned int i = 0; i < instances_number; i++)
         {
            data[i][variable_index] = (data[i][variable_index] - targets_mean[j])/targets_standard_deviation[j];
         }
      }
   }
}


// Vector< Vector<double> > scale_targets_mean_standard_deviation(void) method

/// This method scales the target variables with the calculated mean and standard deviation values from the data matrix.
/// It updates the target variables of the data matrix.
/// It also returns a vector of vectors with the basic statistics of the inputs and target variables. 
/// <ul>
/// <li> Input data mean.
/// <li> Input data standard deviation.
/// <li> Input data minimum.
/// <li> Input data maximum.
/// <li> Target data mean.
/// <li> Target data standard deviation.
/// <li> Target data minimum.
/// <li> Target data maximum.
/// </ul> 

Vector< Vector<double> > DataSet::scale_targets_mean_standard_deviation(void)
{
   Vector< Vector<double> > inputs_targets_statistics = calculate_inputs_targets_statistics();

   const Vector<double>& targets_means = inputs_targets_statistics[4];
   const Vector<double>& targets_standard_deviations = inputs_targets_statistics[5];

   scale_targets_mean_standard_deviation(targets_means, targets_standard_deviations);

   return(inputs_targets_statistics);
}


// void scale_targets_minimum_maximum(const Vector<double>&, const Vector<double>&) method

/// This method scales the target variables with given minimum and maximum values.
/// It updates the target variables of the data matrix.
///
/// @param targets_minimum Minimum values for the target variables to be used for scaling.
/// @param targets_maximum Maximum values for the target variables to be used for scaling.

void DataSet::scale_targets_minimum_maximum(const Vector<double>& targets_minimum, const Vector<double>& targets_maximum)
{
   const unsigned int instances_number = get_instances_number();
   const unsigned int targets_number = variables_information.count_targets_number();

   const Vector<unsigned int>& targets_indices = variables_information.get_targets_indices();

   unsigned int variable_index;

   // Rescale targets data

   for(unsigned int j = 0; j < targets_number; j++)
   {
	   variable_index = targets_indices[j];

      if(targets_maximum[j] - targets_minimum[j] < 1e-99)
      {
         if(display)
         {
            std::cout << "OpenNN Warning: DataSet class.\n"
                      << "void scale_targets_minimum_maximum(const Vector<double>&, const Vector<double>&) method.\n"   
                      << "Minimum and maximum values of target variable " << j << " are equal. "
                      << "Those targets won't be scaled.\n"
			          << "Minimum: " << targets_minimum[j] << "\n"
			          << "Maximum: " << targets_maximum[j] << std::endl;

         }
         
         // Do nothing 
      }
      else
      {
         for(unsigned int i = 0; i < instances_number; i++)
         {
            data[i][variable_index] = 2.0*(data[i][variable_index] - targets_minimum[j])/(targets_maximum[j]-targets_minimum[j]) - 1.0;
         }
      }
   }
}


// Vector< Vector<double> > scale_targets_minimum_maximum(void) method

/// This method scales the target variables with the calculated minimum and maximum values from the data matrix.
/// It updates the target variables of the data matrix.
/// It also returns a vector of vectors with the statistics of the input target variables. 

Vector< Vector<double> > DataSet::scale_targets_minimum_maximum(void)
{
   const Vector< Vector<double> > inputs_targets_statistics = calculate_inputs_targets_statistics();

   const Vector<double>& targets_minimums = inputs_targets_statistics[4];
   const Vector<double>& targets_maximums = inputs_targets_statistics[5];

   scale_targets_minimum_maximum(targets_minimums, targets_maximums);

   return(inputs_targets_statistics);
}


// Vector< Vector<double> > scale_targets(void) method

/// This method calculates the input and target variables statistics. 
/// Then it scales the target variables with that values.
/// The method to be used is that in the scaling and unscaling method variable. 
/// Finally, it returns the statistics. 

Vector< Vector<double> > DataSet::scale_targets(void)
{
   switch(scaling_unscaling_method)
   {
      case MinimumMaximum:
      {
         return(scale_targets_minimum_maximum());
      }            
      break;

      case MeanStandardDeviation:
      {
         return(scale_targets_mean_standard_deviation());
      }
      break;

      default:
      {
         std::ostringstream buffer;

         buffer << "OpenNN Exception: DataSet class\n"
                << "Vector< Vector<double> > scale_targets(void) method.\n"
                << "Unknown scaling and unscaling method.\n";

	     throw std::logic_error(buffer.str());
      }
      break;
   }
}


// void scale_inputs_targets_mean_standard_deviation(const Vector<double>&, const Vector<double>&, const Vector<double>&, const Vector<double>&) method

/// This method scales the inputs and the target variables with given mean and standard deviation values.
/// Please note that it takes as argument a vector of vectors of all basic statistics, with size eight.  
/// It updates the inputs and the target variables of the data matrix.
/// @param inputs_means Mean values used for scaling the input variables.
/// @param inputs_standard_deviations Standard deviation values used for scaling the input variables.
/// @param targets_means Mean values used for scaling the target variables.
/// @param targets_standard_deviations Standard deviation values used for scaling the target variables.

void DataSet::scale_inputs_targets_mean_standard_deviation(const Vector<double>& inputs_means, const Vector<double>& inputs_standard_deviations, const Vector<double>& targets_means, const Vector<double>& targets_standard_deviations)
{
   scale_inputs_mean_standard_deviation(inputs_means, inputs_standard_deviations);

   scale_targets_mean_standard_deviation(targets_means, targets_standard_deviations);
}


// Vector< Vector<double> > scale_inputs_targets_mean_standard_deviation(void) method

/// This method scales the inputs and the target variables with the mean and standard deviation values calculated from 
/// the data matrix. 
/// It updates the inputs and target variables of the data.
/// It also returns the vector of vectors containing the computed statistics of the data set:
/// <ul>
/// <li> Input variables mean.
/// <li> Input variables standard deviation.
/// <li> Input variables minimum.
/// <li> Input variables maximum.
/// <li> Target variables mean.
/// <li> Target variables standard deviation.
/// <li> Target variables minimum.
/// <li> Target variables maximum.
/// </ul> 

Vector< Vector<double> > DataSet::scale_inputs_targets_mean_standard_deviation(void)
{
   const Vector< Vector<double> > inputs_targets_statistics = calculate_inputs_targets_statistics();

   const Vector<double>& inputs_means = inputs_targets_statistics[2];
   const Vector<double>& inputs_standard_deviations = inputs_targets_statistics[3];

   const Vector<double>& targets_means = inputs_targets_statistics[6];
   const Vector<double>& targets_standard_deviations = inputs_targets_statistics[7];

   scale_inputs_targets_mean_standard_deviation(inputs_means, inputs_standard_deviations, targets_means, targets_standard_deviations);

   return(inputs_targets_statistics);
}


// void scale_inputs_targets_minimum_maximum(const Vector<double>&, const Vector<double>&, const Vector<double>&, const Vector<double>&) method

/// This method scales the inputs and the target variables with given minimum and maximum values.
/// Please note that it takes as argument a vector of vectors of all basic statistics, with size eight.  
/// It updates the inputs and the target variables of the data matrix.
/// @param inputs_minimums Minimum values used for scaling the input variables.
/// @param inputs_maximums Maximums values used for scaling the input variables.
/// @param targets_minimums Minimum values used for scaling the target variables.
/// @param targets_maximums Maximum values used for scaling the target variables.

void DataSet::scale_inputs_targets_minimum_maximum(const Vector<double>& inputs_minimums, const Vector<double>& inputs_maximums, const Vector<double>& targets_minimums, const Vector<double>& targets_maximums)
{
   scale_inputs_minimum_maximum(inputs_minimums, inputs_maximums);
   scale_targets_minimum_maximum(targets_minimums, targets_maximums);
}


// Vector< Vector<double> > scale_inputs_targets_minimum_maximum(void) method

/// This method scales the input and the target variables with the minumum and maximum values calculated from the data matrix. 
/// It updates the inputs and target variables of the data.
/// It also returns the vector of vectors containing the computed statistics of the data set:
/// <ul>
/// <li> Input variables mean.
/// <li> Input variables standard deviation.
/// <li> Input variables minimum.
/// <li> Input variables maximum.
/// <li> Target variables mean.
/// <li> Target variables standard deviation.
/// <li> Target variables minimum.
/// <li> Target variables maximum.
/// </ul> 

Vector< Vector<double> > DataSet::scale_inputs_targets_minimum_maximum(void)
{
   const Vector< Vector<double> > inputs_targets_statistics = calculate_inputs_targets_statistics();

   const Vector<double> inputs_minimums = inputs_targets_statistics[0];
   const Vector<double> inputs_maximums = inputs_targets_statistics[1];

   const Vector<double> targets_minimums = inputs_targets_statistics[4];
   const Vector<double> targets_maximums = inputs_targets_statistics[5];

   scale_inputs_targets_minimum_maximum(inputs_minimums, inputs_maximums, targets_minimums, targets_maximums);

   return(inputs_targets_statistics);
}


// Vector< Vector<double> > scale_inputs_targets(void) method

/// This method calculates the input and target variables statistics. 
/// Then it scales the data with that values.
/// The method to be used is that in the scaling and unscaling method variable. 
/// Finally, it returns the statistics. 

Vector< Vector<double> > DataSet::scale_inputs_targets(void)
{
   switch(scaling_unscaling_method)
   {
      case MinimumMaximum:
      {
         return(scale_inputs_targets_minimum_maximum());
      }            
      break;

      case MeanStandardDeviation:
      {
         return(scale_inputs_targets_mean_standard_deviation());
      }
      break;

      default:
      {
         std::ostringstream buffer;

         buffer << "OpenNN Exception: DataSet class\n"
                << "Vector< Vector<double> > scale_inputs_targets(void) method.\n"
                << "Unknown scaling and unscaling method.\n";

	     throw std::logic_error(buffer.str());
      }
      break;
   }
}


// void unscale_data_mean_standard_deviation(const Vector<double>&, const Vector<double>&) method

/// This method unscales the data matrix with given mean and standard deviation values. 
/// It updates the data matrix.
/// @param mean Mean of variables.
/// @param standard_deviation Standard deviation of variables.

void DataSet::unscale_data_mean_standard_deviation(const Vector<double>& mean, const Vector<double>& standard_deviation)
{
   data.unscale_mean_standard_deviation(mean, standard_deviation);
}


// void unscale_data_minimum_maximum(const Vector<double>&, const Vector<double>&) method

/// This method unscales the data matrix with given minimum and maximum values. 
/// It updates the data matrix.
///
/// @param minimum Minimum of variables.
/// @param maximum Maximum of variables.

void DataSet::unscale_data_minimum_maximum(const Vector<double>& minimum, const Vector<double>& maximum)
{
   data.unscale_minimum_maximum(minimum, maximum);
}


// void unscale_inputs_mean_standard_deviation(const Vector<double>&, const Vector<double>&) method

/// This method unscales the input variables with given mean and standard deviation values.
/// It updates the input variables of the data matrix.
/// @param inputs_mean Mean values for the input variables to be used for unscaling.
/// @param inputs_standard_deviation Standard deviation values for the input variables to be used for unscaling.

void DataSet::unscale_inputs_mean_standard_deviation(const Vector<double>& inputs_mean,
const Vector<double>& inputs_standard_deviation)
{
   const unsigned int instances_number = get_instances_number();
   const unsigned int inputs_number = variables_information.count_inputs_number();

   // Postprocess inputs data

   for(unsigned int j = 0; j < inputs_number; j++)
   {
      if(inputs_standard_deviation[j] < 1e-99)
      {
         if(display)
         {                                          
            std::cout << "OpenNN Warning: DataSet class.\n"
                      << "void unscale_inputs_mean_standard_deviation(const Vector<double>&, const Vector<double>&) method.\n"
                      << "Standard deviation of input variable " << j << " is zero.\n"
                      << "Those inputs won't be scaled.\n";
         }

         // Do nothing
      }
      else
      {
         for(unsigned int i = 0; i < instances_number; i++)
         {
            data[i][j] = data[i][j]*inputs_standard_deviation[j] + inputs_mean[j];
         }
      }
   }
}


// void unscale_inputs_minimum_maximum(const Vector<double>&, const Vector<double>&) method

/// This method unscales the input variables with given minimum and maximum values.
/// It updates the input variables of the data matrix.
///
/// @param inputs_minimum Minimum values for the input variables to be used for unscaling.
/// @param inputs_maximum Maximum values for the input variables to be used for unscaling.

void DataSet::unscale_inputs_minimum_maximum(const Vector<double>& inputs_minimum, const Vector<double>& inputs_maximum)
{
   const unsigned int instances_number = get_instances_number();
   const unsigned int inputs_number = variables_information.count_inputs_number();

   // Postprocess inputs data

   for(unsigned int j = 0; j < inputs_number; j++)
   {
      if(inputs_maximum[j] - inputs_minimum[j] < 1e-99)
      {
         if(display)
         {
            std::cout << "OpenNN Warning: DataSet class.\n"
                      << "void unscale_inputs_minimum_maximum(const Vector<double>&, const Vector<double>&) method.\n"
                      << "Minimum and maximum values of input variable " << j << " are equal.\n"
                      << "Those inputs won't be unscaled.\n";
         }

         // Do nothing
      }
      else
      {
         for(unsigned int i = 0; i < instances_number; i++)
         {
            data[i][j] = 0.5*(data[i][j] + 1.0)*(inputs_maximum[j]-inputs_minimum[j]) 
            + inputs_minimum[j]; 
         }
      }
   }
}


// void unscale_targets_mean_standard_deviation(const Vector<double>&, const Vector<double>&) method

/// This method unscales the target variables with given mean and standard deviation values.
/// It updates the target variables of the data matrix.
/// @param targets_mean Mean values for the target variables to be used for unscaling.
/// @param targets_standard_deviation Standard deviation values for the target variables to be used for unscaling.

void DataSet::unscale_targets_mean_standard_deviation(const Vector<double>& targets_mean, 
const Vector<double>& targets_standard_deviation)
{
   const unsigned int instances_number = get_instances_number();
   const unsigned int targets_number = variables_information.count_targets_number();

   // Rescale targets data

   for(unsigned int j = 0; j < targets_number; j++)
   {
      if(targets_standard_deviation[j] < 1e-99)
      {
         if(display)
         {
            std::cout << "OpenNN Warning: DataSet class.\n"
                      << "void unscale_targets_mean_standard_deviation(const Vector<double>&) method.\n"   
                      << "Standard deviation of target variable " <<  j << " is zero.\n"
                      << "Those targets won't be scaled.\n";
         }
         
        // Do nothing
      }
      else
      {
         for(unsigned int i = 0; i < instances_number; i++)
         {
            data[i][j] = data[i][j]*targets_standard_deviation[j] + targets_mean[j];
         }
      }
   }
}


// void unscale_targets_minimum_maximum(const Vector<double>&, const Vector<double>&) method

/// This method unscales the target variables with given minimum and maximum values.
/// It updates the target variables of the data matrix.
///
/// @param targets_minimum Minimum values for the target variables to be used for unscaling.
/// @param targets_maximum Maximum values for the target variables to be used for unscaling.

void DataSet::unscale_targets_minimum_maximum(const Vector<double>& targets_minimum,
const Vector<double>& targets_maximum)
{
   unsigned int instances_number = get_instances_number();
   unsigned int targets_number = variables_information.count_targets_number();

   // Postprocess targets data

   for(unsigned int j = 0; j < targets_number; j++)
   {
      if(targets_maximum[j] - targets_minimum[j] < 1e-99)
      {
         if(display)
         {
            std::cout << "OpenNN Warning: DataSet class.\n"
                      << "void unscale_targets_minimum_maximum(const Vector<double>&, const Vector<double>&) method.\n"   
                      << "Minimum and maximum values of target variable " << j << " are equal.\n"
                      << "Those targets won't be unscaled.\n";
         }
         
         // Do nothing
      }
      else
      {
         for(unsigned int i = 0; i < instances_number; i++)
         {
            data[i][j] = 0.5*(data[i][j] + 1.0)*(targets_maximum[j]-targets_minimum[j]) 
            + targets_minimum[j]; 
         }
      }
   }
}


// void unscale_inputs_targets_mean_standard_deviation(const Vector< Vector<double> >&) method 

/// This method unscales the inputs and the target variables with given mean and standard deviation values.
/// Please note that it takes as argument a vector of vectors of all basic statistics, with size eight.  
/// It updates the inputs and the target variables of the data matrix.
///
/// @param variables_statistics Statistics for the inputs and target variables to be used for unscaling.

void DataSet::unscale_inputs_targets_mean_standard_deviation(const Vector< Vector<double> >& variables_statistics)
{
   // Control sentence (if debug)      
       
   #ifdef _DEBUG 

   unsigned int size = variables_statistics.size();

   if(size != 8)
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: DataSet class.\n" 
             << "void unscale_inputs_targets_mean_standard_deviation(const Vector< Vector<double> >&) method.\n"
             << "Size of statistics must be 8.\n";

	  throw std::logic_error(buffer.str());

   }     

   for(unsigned int i = 0; i < 8; i++)
   {
      size = variables_statistics[i].size();

      if(size != 8)
      {
         std::ostringstream buffer;

         buffer << "OpenNN Exception: DataSet class.\n" 
                << "void unscale_inputs_targets_mean_standard_deviation(const Vector< Vector<double> >&) method.\n"
                << "Size of statistics element " << i << " must be equal to number of variables.\n";

   	     throw std::logic_error(buffer.str());
      }     
   }

   #endif

   unscale_inputs_mean_standard_deviation(variables_statistics[2], variables_statistics[3]);
   unscale_targets_mean_standard_deviation(variables_statistics[6], variables_statistics[7]);
}


// void unscale_inputs_targets_minimum_maximum(const Vector< Vector<double> >&) method

/// This method unscales the inputs and the target variables with given minimum and maximum values.
/// Please note that it takes as argument a vector of vectors of all basic statistics, with size eight.  
/// It updates the inputs and the target variables of the data matrix.
///
/// @param variables_statistics Statistics for the inputs and target variables to be used for unscaling.

void DataSet::unscale_inputs_targets_minimum_maximum(const Vector< Vector<double> >& variables_statistics)
{
   // Control sentence (if debug)      
       
   #ifdef _DEBUG 

   unsigned int size = variables_statistics.size();

   if(size != 8)
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: DataSet class.\n" 
             << "void unscale_inputs_targets_minimum_maximum(const Vector< Vector<double> >&) method.\n"
             << "Size of statistics must be 8.\n";

	  throw std::logic_error(buffer.str());
   }     

   #endif

   unscale_inputs_minimum_maximum(variables_statistics[0], variables_statistics[1]);

   unscale_targets_minimum_maximum(variables_statistics[4], variables_statistics[5]);
}


// void initialize_data(const double& value) method

/// This methods initializes the data matrix with a given value. 
/// @param new_value Initialization value. 

void DataSet::initialize_data(const double& new_value)
{
   data.initialize(new_value);
}


// void initialize_data_normal(void) method

/// This methods initializes the data matrix with random given values chosen from a normal distribution with mean 0 and 
/// standard deviation 1. 

void DataSet::initialize_data_normal(void)
{
   data.initialize_normal();
}


// TiXmlElement* to_XML(void) const method

/// This method serializes the data set object into a XML element of the TinyXML library. 

TiXmlElement* DataSet::to_XML(void) const
{
   std::ostringstream buffer;

   // Data set

   TiXmlElement* data_set_element = new TiXmlElement("DataSet");
   data_set_element->SetAttribute("Version", 4);

   // Data filename

   if(!data_filename.empty())
   {
      TiXmlElement* data_filename_element = new TiXmlElement("DataFilename");
      data_set_element->LinkEndChild(data_filename_element);

      TiXmlText* data_filename_text = new TiXmlText(data_filename.c_str());
      data_filename_element->LinkEndChild(data_filename_text);
   }

   // Variables information 

   TiXmlElement* variables_information_element = variables_information.to_XML();
   data_set_element->LinkEndChild(variables_information_element);

   // Instances information 

   TiXmlElement* instances_information_element = instances_information.to_XML();
   data_set_element->LinkEndChild(instances_information_element);

   return(data_set_element);
}


// void from_XML(TiXmlElement*) method

/// This method deserializes a TinyXML element into this data set object. 
/// @param data_set_element Pointer to a XML element containing the member data. 

void DataSet::from_XML(TiXmlElement* data_set_element)
{
   if(!data_set_element)
   {
      return;
   }

   // Data filename

   TiXmlElement* data_filename_element = data_set_element->FirstChildElement("DataFilename");

   if(data_filename_element)
   {
      std::string new_data_filename = data_filename_element->GetText();           

      try
      {
         set_data_filename(new_data_filename);
         load_data(new_data_filename);
      }
      catch(std::exception& e)
      {
         std::cout << e.what() << std::endl;		 
      }
   }

   // Variables information 

   TiXmlElement* variables_information_element = data_set_element->FirstChildElement("VariablesInformation");

   if(variables_information_element)
   {
      variables_information.from_XML(variables_information_element);
   }

   // Instances information

   TiXmlElement* instances_information_element = data_set_element->FirstChildElement("InstancesInformation");

   if(instances_information_element)
   {
      instances_information.from_XML(instances_information_element);
   }

}


// std::string to_string(void) const method

/// This method returns a string representation of the current data set object. 

std::string DataSet::to_string(void) const
{
   std::ostringstream buffer;

   buffer << "Data filename: " << data_filename << "\n"
          << variables_information.to_string()
          << instances_information.to_string()
          << "Display: " << display << "\n"
          << "Data:\n" << data;

   return(buffer.str());
}

// void print(void) const method

/// This method prints to the screen in a XML-type format the members of the data set object. 

void DataSet::print(void) const
{
   if(display)
   {
      std::cout << to_string();
   }
}


// void save(const std::string&) const method

/// This method saves the members of a data set object to a XML-type file in an XML-type format.
/// @param filename Name of data set XML-type file.

void DataSet::save(const std::string& filename) const
{
   TiXmlDocument document;

   TiXmlDeclaration* declaration = new TiXmlDeclaration("1.0", "", "");
   document.LinkEndChild(declaration);

   TiXmlElement* data_set_element = to_XML();
   document.LinkEndChild(data_set_element);

   document.SaveFile(filename.c_str());
}


// void load(const std::string&) method

/// This method loads the members of a data set object from a XML-type file:
/// <ul>
/// <li> Instances number.
/// <li> Training instances number.
/// <li> Training instances indices.
/// <li> Generalization instances number.

/// <li> Generalization instances indices.
/// <li> Testing instances number.
/// <li> Testing instances indices.
/// <li> Input variables number.
/// <li> Input variables indices.
/// <li> Target variables number.
/// <li> Target variables indices.
/// <li> Input variables name. 
/// <li> Target variables name.
/// <li> Input variables description. 
/// <li> Target variables description.
/// <li> Display.
/// <li> Data.
/// </ul> 
/// Please mind about the file format. This is specified in the User's Guide.
/// @param filename Name of data set XML-type file.

void DataSet::load(const std::string& filename)
{
   std::ostringstream buffer;

   TiXmlDocument document(filename.c_str());

   if(!document.LoadFile())
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: DataSet class.\n"
             << "void load(const std::string&) method.\n"
             << "Cannot load XML file " << filename << ".\n";

      throw std::logic_error(buffer.str());
   }

   // Root

   TiXmlElement* data_set_element = document.FirstChildElement("DataSet");

   if(!data_set_element)
   {
      buffer << "OpenNN Exception: DataSet class.\n"
             << "void load(const std::string&) method.\n"
             << "File " << filename << " is not a valid data set file.\n";

      throw std::logic_error(buffer.str());
   }

   from_XML(data_set_element);
}


// void print_data(void) const method

/// This method prints to the sceen the values of the data matrix. 

void DataSet::print_data(void) const
{
   if(display)
   {
      std::cout << data;
   }
}


// void save_data(const std::string&) const method

/// This method saves to a file the values of the data matrix. 
/// @param filename Name of data file. 

void DataSet::save_data(const std::string& filename) const
{
   std::ofstream file(filename.c_str());

   if(!file.is_open())
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: DataSet class.\n" 
             << "void save_data(const std::string&) const method.\n"
             << "Cannot open data file.\n";

      throw std::logic_error(buffer.str().c_str());	  
   }
  
   // Write data

   file << data;

   // Close file

   file.close();
}


// void load_data(const std::string& filename)

/// This method loads from a file the values of the data matrix. 
/// The number of rows must be equal to the number of instances.
/// The number of columns must be equal to the number of variables.

void DataSet::load_data(const std::string& new_data_filename)
{
   data_filename = new_data_filename;

   data.load(data_filename);

   unsigned int variables_number = data.get_columns_number();

   variables_information.set(variables_number);

   unsigned int instances_number = data.get_rows_number();

   instances_information.set(instances_number);
}


// Vector<int> calculate_target_class_distribution(void) const method

/// This method returns a vector containing the number of instances of each class in the data set.
/// If the number of target variables is one then the number of classes is two.
/// If the number of target variables is greater than one then the number of classes is equal to the number 
/// of target variables.

Vector<unsigned int> DataSet::calculate_target_class_distribution(void) const
{ 
   // Control sentence (if debug)

   const unsigned int instances_number = get_instances_number();   
   const unsigned int targets_number = variables_information.count_targets_number();
   const Vector<unsigned int>& targets_indices = variables_information.get_targets_indices();

   Vector<unsigned int> class_distribution;

   if(targets_number == 1) // Two classes
   {
      class_distribution.set(2, 0);

	  int target_index = targets_indices[0];

      for(unsigned int instance_index = 0; instance_index < instances_number; instance_index++)
      {      
         if(data[instance_index][target_index] < 0.5)
         {
            class_distribution[0]++;
         }   
		 else
		 {
            class_distribution[1]++;		 
		 }
      }
   }
   else // More than two classes
   {
      class_distribution.set(targets_number, 0);
   
      for(unsigned int i = 0; i < instances_number; i++)
      {     
         for(unsigned int j = 0; j < targets_number; j++)
         {         
            if(data[i][targets_indices[j]] > 0.5)
            {
               class_distribution[j]++;
            }   
         } 
      }
   }

   // Check data consistency

   if(class_distribution.calculate_sum() != instances_number)
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: DataSet class.\n" 
             << "Vector<int> calculate_target_class_distribution(void) const method.\n"
             << "Sum of class distributions is not equal to number of instances.\n"
             << "Class distribution: " << class_distribution << std::endl; 
      throw std::logic_error(buffer.str().c_str());   
   }

   return(class_distribution);
}


// TiXmlElement* report_variables_information_XML(void) const method

//TiXmlElement* DataSet::report_variables_information_XML(void) const
//{
//   return(NULL);
//}


// TiXmlElement* report_instances_information_XML(void) const method

//TiXmlElement* DataSet::report_instances_information_XML(void) const
//{
//   return(NULL);
//}


// TiXmlElement* report_data_XML(void) const method

//TiXmlElement* DataSet::report_data_XML(void) const
//{
//   TiXmlElement* report_data_element = new TiXmlElement("ReportData");

//   TiXmlElement* data_element = new TiXmlElement("Data");
//   report_data_element->LinkEndChild(data_element);

//   std::string data_string;// = data.to_string();

//   TiXmlText* data_text = new TiXmlText(data_string.c_str());
//   data_element->LinkEndChild(data_text);

//   return(report_data_element);
//}


// TiXmlElement* calculate_data_statistics_XML(void) const method

/// This method returns a HTML formated string with the basic statistics of the data matrix. 

//TiXmlElement* DataSet::calculate_data_statistics_XML(void) const
//{
//   const Vector< Vector<double> > data_statistics = calculate_data_statistics();

//   const Vector<std::string> default_variables_name = variables_information.get_default_names();

//   TiXmlElement* calculate_data_statistics_element = new TiXmlElement("CalculateDataStatistics");

   // Variables name

//   TiXmlElement* variables_name_element = new TiXmlElement("VariablesName");
//   calculate_data_statistics_element->LinkEndChild(variables_name_element);

//   TiXmlText* variables_name_text = new TiXmlText("data_string");
//   variables_name_element->LinkEndChild(variables_name_text);

   // Variables minimum

//   TiXmlElement* variables_minimum_element = new TiXmlElement("VariablesMinimum");
//   calculate_data_statistics_element->LinkEndChild(variables_minimum_element);

//   TiXmlText* variables_minimum_text = new TiXmlText("variables_minimum");
//   variables_minimum_element->LinkEndChild(variables_minimum_text);

   // Variables maximum

//   TiXmlElement* variables_maximum_element = new TiXmlElement("VariablesMaximum");
//   calculate_data_statistics_element->LinkEndChild(variables_maximum_element);

//   TiXmlText* variables_maximum_text = new TiXmlText("variables_maximum");
//   variables_maximum_element->LinkEndChild(variables_maximum_text);

   // Variables mean

//   TiXmlElement* variables_mean_element = new TiXmlElement("VariablesMean");
//   calculate_data_statistics_element->LinkEndChild(variables_mean_element);

//   TiXmlText* variables_mean_text = new TiXmlText("variables_mean");
//   variables_mean_element->LinkEndChild(variables_mean_text);

   // Variables standard deviation

//   TiXmlElement* variables_standard_deviation_element = new TiXmlElement("VariablesStandardDeviation");
//   calculate_data_statistics_element->LinkEndChild(variables_standard_deviation_element);

//   TiXmlText* variables_standard_deviation_text = new TiXmlText("variables_standard_deviation");
//   variables_standard_deviation_element->LinkEndChild(variables_standard_deviation_text);

//   return(calculate_data_statistics_element);    
//}


// TiXmlElement* calculate_data_histogram_XML(void) const method

/// This method returns a HTML formated string with the histogram of the data matrix. 

//TiXmlElement* DataSet::calculate_data_histogram_XML(void) const
//{
//   const Vector< Vector< Vector<double> > > data_histogram = calculate_data_histogram();

//   const Vector<std::string> default_variables_name = variables_information.get_default_names();

//   TiXmlElement* calculate_data_histogram_element = new TiXmlElement("CalculateDataHistogram");

   // Variables name

//   TiXmlElement* variables_name_element = new TiXmlElement("VariablesName");
//   calculate_data_histogram_element->LinkEndChild(variables_name_element);

//   TiXmlText* variables_name_text = new TiXmlText("data_string");
//   variables_name_element->LinkEndChild(variables_name_text);

//   return(calculate_data_histogram_element);
//}


// TiXmlElement* split_instances_XML(void) const method

//TiXmlElement* DataSet::split_instances_XML(void) const
//{
//   return(NULL);
//}


// TiXmlElement* calculate_target_class_distribution_XML(void) const method

/// This method returns a HTML string with a table about the ditribution of the target variables classes. 

//TiXmlElement* DataSet::calculate_target_class_distribution_XML(void) const
//{
//   const Vector<unsigned int> target_class_distribution = calculate_target_class_distribution();

//   const Vector<std::string> default_variables_name = variables_information.get_default_names();

//   TiXmlElement* calculate_target_class_distribution_element = new TiXmlElement("CalculateTargetClassDistribution");

   // Variables name

//   TiXmlElement* variables_name_element = new TiXmlElement("VariablesName");
//   calculate_target_class_distribution_element->LinkEndChild(variables_name_element);

//   TiXmlText* variables_name_text = new TiXmlText("data_string");
//   variables_name_element->LinkEndChild(variables_name_text);

//   return(calculate_target_class_distribution_element);
//}

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
