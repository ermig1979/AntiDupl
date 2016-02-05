/****************************************************************************************************************/
/*                                                                                                              */
/*   OpenNN: Open Neural Networks Library                                                                       */
/*   www.opennn.cimne.com                                                                                       */
/*                                                                                                              */
/*   F U N C T I O N   R E G R E S S I O N   T E S T I N G   C L A S S                                          */
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

// OpenNN includes

#include "function_regression_testing.h"

namespace OpenNN
{

// DEFAULT CONSTRUCTOR

/// Default constructor. 
/// It creates a function regression testing object neither associated to a neural network nor to a data set.

FunctionRegressionTesting::FunctionRegressionTesting(void)
 : neural_network_pointer(NULL),
   data_set_pointer(NULL)
{
   set_default();
}


// NEURAL NETWORK CONSTRUCTOR

/// Neural network constructor. 
/// It creates a function regression testing object associated to a neural network but not to a a data set.
/// @param new_neural_network_pointer Pointer to a neural network object.

FunctionRegressionTesting::FunctionRegressionTesting(NeuralNetwork* new_neural_network_pointer)
 : neural_network_pointer(new_neural_network_pointer),
   data_set_pointer(NULL)
{
   set_default();
}


// DATA SET CONSTRUCTOR

/// Data set constructor. 
/// It creates a function regression testing object associated to a data set but not to a neural network.
/// @param new_data_set_pointer Pointer to a data set object.

FunctionRegressionTesting::FunctionRegressionTesting(DataSet* new_data_set_pointer)
 : neural_network_pointer(NULL),
   data_set_pointer(new_data_set_pointer)
{
   set_default();
}


// GENERAL CONSTRUCTOR

/// General constructor. 
/// It creates a function regression testing object associated to a neural network and a data set objects.
/// @param new_neural_network_pointer Pointer to a neural network object.
/// @param new_data_set_pointer Pointer to a data set object.

FunctionRegressionTesting::FunctionRegressionTesting(NeuralNetwork* new_neural_network_pointer, DataSet* new_data_set_pointer)
 : neural_network_pointer(new_neural_network_pointer),
   data_set_pointer(new_data_set_pointer)
{  
   set_default();
}


// XML CONSTRUCTOR

/// XML constructor. 
/// It creates a function regression testing object neither associated to a neural network nor to a data set objects.
/// It also loads the members of this object from a TinyXML element. 
/// @param function_regression_testing_element XML element which contains the member data.

FunctionRegressionTesting::FunctionRegressionTesting(TiXmlElement* function_regression_testing_element)
 : neural_network_pointer(NULL),
   data_set_pointer(NULL)
{
   set_default();

   from_XML(function_regression_testing_element);
}


// DESTRUCTOR

/// Destructor. 
/// It does not delete any pointer. 

FunctionRegressionTesting::~FunctionRegressionTesting()
{
}


// NeuralNetwork* get_neural_network_pointer(void) const method

/// This method returns a pointer to the neural network which is to be tested. 

NeuralNetwork* FunctionRegressionTesting::get_neural_network_pointer(void) const
{
   return(neural_network_pointer);
}


// DataSet* get_data_set_pointer(void) const method

/// This method returns a pointer to the data set object on which the neural network is tested. 

DataSet* FunctionRegressionTesting::get_data_set_pointer(void) const
{
   return(data_set_pointer);
}


// const bool& get_display(void) const method

/// This method returns true if messages from this class can be displayed on the screen,
/// or false if messages from this class can't be displayed on the screen.

const bool& FunctionRegressionTesting::get_display(void) const
{
   return(display);
}


// void set_neural_network_pointer(NeuralNetwork*) method

/// This method sets a new neural network to be tested. 
/// @param new_neural_network_pointer Pointer to the neural network object to be tested. 

void FunctionRegressionTesting::set_neural_network_pointer(NeuralNetwork* new_neural_network_pointer)
{
   neural_network_pointer = new_neural_network_pointer;
}


// void set_data_set_pointer(DataSet*) method

/// This method sets a new data set on which the neural network is to be tested. 
/// @param new_data_set_pointer Pointer to the data set object used for testing. 

void FunctionRegressionTesting::set_data_set_pointer(DataSet* new_data_set_pointer)
{
   data_set_pointer = new_data_set_pointer;
}


// void set_display(const bool&) method

/// This method sets a new display flag. 
/// @param new_display Flag for displaying messages from this class or not. 

void FunctionRegressionTesting::set_display(const bool& new_display)
{
   display = new_display;
}    


// void set_default(void) method

/// This method sets the default values from the function regression testing class:
/// <ul>
/// <li> Display: True.
/// </ul>

void FunctionRegressionTesting::set_default(void)
{
   display = true;
}


// Vector< Matrix<double> > calculate_scaled_target_output_data(void) const method

/// This method returns a vector of matrices with number of rows equal to number of testing instances and 
/// number of columns equal to two (the targets value and the outputs value). 

Vector< Matrix<double> > FunctionRegressionTesting::calculate_scaled_target_output_data(void) const
{
   // Control sentence (if debug)

   #ifdef _DEBUG 

   check();

   #endif

   // Data set stuff

   const InstancesInformation& instances_information = data_set_pointer->get_instances_information();

   const unsigned int testing_instances_number = instances_information.count_testing_instances_number(); 

   const Matrix<double> testing_input_data = data_set_pointer->arrange_testing_input_data();

   const Matrix<double> testing_target_data = data_set_pointer->arrange_testing_target_data();

   // Neural network stuff

   const MultilayerPerceptron* multilayer_perceptron_pointer = neural_network_pointer->get_multilayer_perceptron_pointer();

   const unsigned int outputs_number = multilayer_perceptron_pointer->count_outputs_number();

   bool scaling_layer_flag = neural_network_pointer->get_scaling_layer_flag();   
   bool unscaling_layer_flag = neural_network_pointer->get_unscaling_layer_flag();   

   neural_network_pointer->set_scaling_layer_flag(false);   
   neural_network_pointer->set_unscaling_layer_flag(false);   

   const Matrix<double> testing_output_data = neural_network_pointer->calculate_output_data(testing_input_data);
   
   neural_network_pointer->set_scaling_layer_flag(scaling_layer_flag);   
   neural_network_pointer->set_unscaling_layer_flag(unscaling_layer_flag);   

   // Function regression testing stuff

   Vector< Matrix<double> > scaled_target_output_data(outputs_number);  

   for(unsigned int i = 0; i < outputs_number; i++)
   {
      scaled_target_output_data[i].set(testing_instances_number, 2);

	  for(unsigned int j = 0; j < testing_instances_number; j++)
	  {
         scaled_target_output_data[i][j][0] = testing_target_data[j][i];
         scaled_target_output_data[i][j][1] = testing_output_data[j][i];
	  }
   }

   return(scaled_target_output_data);
}


// void check(void) const method

/// This method checks that:
/// <ul>
/// <li> The multilayer perceptron pointer is not NULL. 
/// <li> The data set pointer is not NULL. 
/// </ul>

void FunctionRegressionTesting::check(void) const
{
   std::ostringstream buffer;

   if(!neural_network_pointer)
   {
      buffer << "OpenNN Exception: FunctionRegressionTesting class.\n" 
             << "void check(void) const method.\n"
             << "Neural network is NULL.\n";

      throw std::logic_error(buffer.str().c_str());	  	  
   }

   if(!data_set_pointer)
   {
      buffer << "OpenNN Exception: FunctionRegressionTesting class.\n" 
             << "void check(void) const method.\n"
             << "Data set is NULL.\n";

      throw std::logic_error(buffer.str().c_str());	  
   }
}


// Vector< Vector<double> > calculate_linear_regression_parameters(void) const method

/// This method performs a linear regression analysis between the testing instances in the data set and 
/// the corresponding neural network outputs. 
/// It returns all the provided parameters in a vector of vectors. 
/// The number of elements in the vector is equal to the number of output variables. 
/// The size of each element is equal to the number of regression parameters (2). 
/// In this way, each subvector contains the regression parameters intercept and slope of an output variable.

Vector< Vector<double> > FunctionRegressionTesting::calculate_linear_regression_parameters(void) const
{
   // Control sentence (if debug)

   #ifdef _DEBUG 

   check();

   #endif

   // Data set stuff

   const InstancesInformation& instances_information = data_set_pointer->get_instances_information();

   const unsigned int testing_instances_number = instances_information.count_testing_instances_number();

   // Neural network stuff

   const MultilayerPerceptron* multilayer_perceptron_pointer = neural_network_pointer->get_multilayer_perceptron_pointer();

   const unsigned int inputs_number = multilayer_perceptron_pointer->count_outputs_number();
   const unsigned int outputs_number = multilayer_perceptron_pointer->count_outputs_number();

   #ifdef _DEBUG 

   std::ostringstream buffer;

   if(testing_instances_number == 0)
   {
      buffer << "OpenNN Exception: FunctionRegressionTesting class.\n" 
             << "Vector< Vector<double> > calculate_linear_regression_parameters(void) const method.\n"
             << "Number of testing instances is zero.\n";

      throw std::logic_error(buffer.str().c_str());	  
   }

   #endif

   // Get regression parameters

   Vector<double> inputs(inputs_number);
   Vector<double> targets(outputs_number);
   Vector<double> outputs(outputs_number);

   Vector<double> targets_sum(outputs_number, 0.0);
   Vector<double> squared_targets_sum(outputs_number, 0.0);
   Vector<double> outputs_sum(outputs_number, 0.0);
   Vector<double> targets_outputs_sum(outputs_number, 0.0);

   for(unsigned int i = 0; i < testing_instances_number; i++)
   {
      targets = data_set_pointer->get_testing_target_instance(i);   

      inputs = data_set_pointer->get_testing_input_instance(i);   
	  outputs = multilayer_perceptron_pointer->calculate_outputs(inputs);   

	  targets_sum += targets;
	  squared_targets_sum += targets*targets;
	  outputs_sum += outputs;
      targets_outputs_sum += targets*outputs;
   }

   const Vector<double> intercept = (outputs_sum*squared_targets_sum-targets_sum*targets_outputs_sum)
   /(squared_targets_sum*testing_instances_number - targets_sum*targets_sum);
	  
   const Vector<double> slope = (targets_outputs_sum*testing_instances_number - outputs_sum*targets_sum)
   /(squared_targets_sum*testing_instances_number - targets_sum*targets_sum);

   Vector< Vector<double> > linear_regression_parameters(outputs_number);   

   for(unsigned int i = 0; i < outputs_number; i++)
   {
      linear_regression_parameters[i].set(2);

	  linear_regression_parameters[i][0] = intercept[i];
      linear_regression_parameters[i][1] = slope[i];
   }
 
   return(linear_regression_parameters);          
}


// FunctionRegressionTesting::LinearRegressionAnalysisResults FunctionRegressionTesting::perform_linear_regression_analysis(void) const

/// This method performs a linear regression analysis of a neural network on the testing indices of a data set. 
/// It returns a linear regression analysis results structure, which consists on:
/// <ul>
/// <li> Linear regression parameters.
/// <li> Scaled target and output data. 
/// </ul>

FunctionRegressionTesting::LinearRegressionAnalysisResults FunctionRegressionTesting::perform_linear_regression_analysis(void) const
{
   LinearRegressionAnalysisResults linear_regression_analysis_results;

   linear_regression_analysis_results.linear_regression_parameters = calculate_linear_regression_parameters();
   linear_regression_analysis_results.scaled_target_output_data = calculate_scaled_target_output_data();

   return(linear_regression_analysis_results);
}


// void LinearRegressionAnalysisResults::save(const std::string&) const method

/// This method saves a linear regression analysis results structure to a data file. 
/// @param filename Name of results data file. 

void FunctionRegressionTesting::LinearRegressionAnalysisResults::save(const std::string& filename) const
{
   std::ofstream file(filename.c_str());

   file << "Linear regression parameters:\n"
        << linear_regression_parameters
        << "Scaled target-output data:\n"
        << scaled_target_output_data;

   file.close();
}


// Matrix<double> calculate_error_data(void) const method

/// This method calculates the errors between the outputs from a neural network and the testing instances in a data set. 
/// It returns a vector of tree matrices:
/// <ul> 
/// <li> Absolute error.  
/// <li> Relative error.
/// <li> Percentage error. 
/// </ul> 
/// The number of rows in each matrix is the number of testing instances in the data set. 
/// The number of columns is the number of outputs in the neural network. 

Vector< Matrix<double> > FunctionRegressionTesting::calculate_error_data(void) const
{
   // Data set stuff

   #ifdef _DEBUG 

   check();

   #endif

   const Matrix<double> testing_input_data = data_set_pointer->arrange_testing_input_data();

   const Matrix<double> testing_target_data = data_set_pointer->arrange_testing_target_data();

   const Matrix<double> testing_output_data = neural_network_pointer->calculate_output_data(testing_input_data);

   const UnscalingLayer* unscaling_layer_pointer = neural_network_pointer->get_unscaling_layer_pointer();

   #ifdef _DEBUG 

   std::ostringstream buffer;

   if(!unscaling_layer_pointer)
   {
      buffer << "OpenNN Exception: FunctionRegressionTesting class.\n" 
             << "Vector< Matrix<double> > calculate_error_data(void) const.\n"
             << "Unscaling layer is NULL.\n";

      throw std::logic_error(buffer.str().c_str());	  	  
   }

   #endif

   const Vector<double>& outputs_minimum = unscaling_layer_pointer->get_minimums();
   const Vector<double>& outputs_maximum = unscaling_layer_pointer->get_maximums();

   // Error data

   Vector< Matrix<double> > error_data(3);

   // Absolute error

   error_data[0] = testing_target_data - testing_output_data;

   // Relative error

   error_data[1] = error_data[0]/(outputs_maximum-outputs_minimum);

   // Percentage error

   error_data[2] = error_data[1]*100.0;

   return(error_data);
}


// Vector< Vector< Vector<double> > > calculate_error_data_statistics(void) const method

/// This method calculates the basic statistics on the error data (to do).
/// <ul>
/// <li> Minimum. 
/// <li> Maximum. 
/// <li> Mean.
/// <li> Standard deviation
/// </ul>

Vector< Vector< Vector<double> > > FunctionRegressionTesting::calculate_error_data_statistics(void) const
{
   const Vector< Vector< Vector<double> > > statistics;

   return(statistics);
}


// Vector< Vector< Vector<double> > > calculate_error_data_histogram(const unsigned int&) const method

/// This method calculates histograms for the relative errors of all the output variables. 
/// The number of bins is set by the user. 
/// @param bins_number Number of bins in the histograms. 

Vector< Vector< Vector<double> > > FunctionRegressionTesting::calculate_error_data_histogram(const unsigned int& bins_number) const
{
   const Vector< Matrix<double> > error_data = calculate_error_data();

   return(error_data[2].calculate_histogram(bins_number));
}


// Vector< Vector< Vector<double> > > calculate_error_data_histogram(void) const method

/// This method calculates histograms for the relative errors of all the output variables. 
/// The number of bins is the default (10). 

Vector< Vector< Vector<double> > > FunctionRegressionTesting::calculate_error_data_histogram(void) const
{
   Vector< Matrix<double> > error_data = calculate_error_data();

   return(error_data[0].calculate_histogram());
}


// std::string to_string(void) const method

/// This method returns a string representation of this function regression testing object. 

std::string FunctionRegressionTesting::to_string(void) const
{
   std::ostringstream buffer;

   buffer << "Function regression testing\n"
          << "Display: " << display << "\n";

   return(buffer.str());
}


// TiXmlElement* to_XML(void) const method

/// This method serializes this function regression testing object into a TinyXML element. 
/// Please read the OpenNN manual for more information about this. 
/// @todo 

TiXmlElement* FunctionRegressionTesting::to_XML(void) const
{
   return(NULL);
}


// void from_XML(TiXmlElement*) method

/// This method deserializes a TinyXML element into this function regression testing object. 
// @param function_regression_testing_element Pointer to a XML element containing the member data. 
/// @todo 

void FunctionRegressionTesting::from_XML(TiXmlElement*)
{
}


//// TiXmlElement* calculate_error_data_XML(void) const method
//
//TiXmlElement* FunctionRegressionTesting::calculate_error_data_XML(void) const
//{
//
//   // Neural network stuff
//
//   const MultilayerPerceptron* multilayer_perceptron_pointer = neural_network_pointer->get_multilayer_perceptron_pointer();
//   const VariablesInformation& variables_information = multilayer_perceptron_pointer->get_variables_information();
//
//   unsigned int outputs_number = multilayer_perceptron_pointer->count_outputs_number();
//
//   Vector<std::string> default_outputs_name = variables_information.write_default_outputs_name();
//
//   // Data set stuff
//
//   unsigned int testing_instances_number = data_set_pointer->get_testing_instances_number();
//
//   Vector<unsigned int> testing_instances_indices = data_set_pointer->get_testing_instances_indices();
//
//   Matrix<double> testing_input_data = data_set_pointer->arrange_testing_input_data();
//
//   Matrix<double> testing_target_data = data_set_pointer->arrange_testing_target_data();
//
//   Matrix<double> testing_output_data = multilayer_perceptron_pointer->calculate_output_data(testing_input_data);
//
//   unsigned int width = (unsigned int)(100.0/(1.0 + outputs_number*5.0));
//
//   return(NULL);
//}
//
//
//// TiXmlElement* calculate_error_data_statistics_XML(void) const method
//
//TiXmlElement* FunctionRegressionTesting::calculate_error_data_statistics_XML(void) const
//{
//   // Neural network stuff
//
//   const MultilayerPerceptron* multilayer_perceptron_pointer = neural_network_pointer->get_multilayer_perceptron_pointer();
//   const VariablesInformation& variables_information = multilayer_perceptron_pointer->get_variables_information();
//
//   unsigned int outputs_number = multilayer_perceptron_pointer->count_outputs_number();
//   Vector<std::string> default_outputs_name = variables_information.write_default_outputs_name();
//
//   return(NULL);
//}
//
//
//// TiXmlElement* get_error_data_histogram_XML(void) const method
//
//TiXmlElement* FunctionRegressionTesting::calculate_error_data_histogram_XML(void) const
//{
//   // Neural network stuff
//
////   const MultilayerPerceptron* multilayer_perceptron_pointer = neural_network_pointer->get_multilayer_perceptron_pointer();
////   const VariablesInformation& variables_information = multilayer_perceptron_pointer->get_variables_information();
//
////   int outputs_number = multilayer_perceptron_pointer->count_outputs_number();
////   Vector<std::string> default_outputs_name = variables_information.write_default_outputs_name();
//
////   Vector< Vector< Vector<double> > > error_data_histogram = calculate_error_data_histogram();
//
////   const unsigned int bins_number = 10;
//
//   return(NULL);
//}
//
//
//// TiXmlElement* perform_linear_regression_analysis_XML(void) const method
//
//TiXmlElement* FunctionRegressionTesting::perform_linear_regression_analysis_XML(void) const
//{
//   return(NULL);
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

