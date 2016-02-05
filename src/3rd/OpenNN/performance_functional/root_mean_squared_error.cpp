/****************************************************************************************************************/
/*                                                                                                              */
/*   OpenNN: Open Neural Networks Library                                                                       */
/*   www.opennn.cimne.com                                                                                       */
/*                                                                                                              */
/*   R O O T   M E A N   S Q U A R E D   E R R O R   C L A S S                                                  */
/*                                                                                                              */
/*   Roberto Lopez                                                                                              */
/*   International Center for Numerical Methods in Engineering (CIMNE)                                          */
/*   Technical University of Catalonia (UPC)                                                                    */
/*   Barcelona, Spain                                                                                           */
/*   E-mail: rlopez@cimne.upc.edu                                                                               */
/*                                                                                                              */
/****************************************************************************************************************/

// OpenNN includes

#include "root_mean_squared_error.h"

// System includes

#include <iostream>
#include <cmath>
#include <fstream>
#include <limits>
#include <sstream>
#include <string>

// TinyXml includes

#include "../../tinyxml/tinyxml.h"


namespace OpenNN
{

// DEFAULT CONSTRUCTOR

/// Default constructor. 
/// It creates a root mean squared error performance term object not associated to any 
/// neural network and not measured on any data set.
/// It also initializes all the rest of class members to their default values.

RootMeanSquaredError::RootMeanSquaredError(void) : PerformanceTerm()
{
}


// NEURAL NETWORK CONSTRUCTOR

/// Neural network constructor. 
/// It creates a root mean squared error associated to a neural network object but not to a data set object. 
/// It also initializes all the rest of class members to their default values.
/// @param new_neural_network_pointer Pointer to a neural network object.

RootMeanSquaredError::RootMeanSquaredError(NeuralNetwork* new_neural_network_pointer)
: PerformanceTerm(new_neural_network_pointer)
{
}


// DATA SET CONSTRUCTOR

/// Data set constructor. 
/// It creates a root mean squared error associated to a data set object but not to a neural network object. 
/// It also initializes all the rest of class members to their default values.
/// @param new_data_set_pointer Pointer to a data set object.

RootMeanSquaredError::RootMeanSquaredError(DataSet* new_data_set_pointer)
: PerformanceTerm(new_data_set_pointer)
{
}


// NEURAL NETWORK AND DATA SET CONSTRUCTOR

/// Neural network and data set constructor. 
/// It creates a root mean squared error performance term object associated to a 
/// neural network and measured on a data set.
/// It also initializes all the rest of class members to their default values.
/// @param new_neural_network_pointer Pointer to a neural network object.
/// @param new_data_set_pointer Pointer to a data set object.

RootMeanSquaredError::RootMeanSquaredError(NeuralNetwork* new_neural_network_pointer, DataSet* new_data_set_pointer)
: PerformanceTerm(new_neural_network_pointer, new_data_set_pointer)
{
}


// XML CONSTRUCTOR

/// XML constructor. 
/// This constructor creates a root mean squared object neither associated to a neural network nor a data set. 
/// It also loads the member data from a XML element.
/// @param root_mean_squared_error_element TinyXML element with the object members. 

RootMeanSquaredError::RootMeanSquaredError(TiXmlElement* root_mean_squared_error_element)
: PerformanceTerm(root_mean_squared_error_element)
{
}


// DESTRUCTOR

/// Destructor.

RootMeanSquaredError::~RootMeanSquaredError(void)
{
}


// METHODS

// void check(void) const method

/// This method checks that there are a neural network and a data set associated to the root mean squared error, 
/// and that the numbers of inputs and outputs in the neural network are equal to the numbers of inputs and targets in the data set. 
/// If some of the above conditions is not hold, the method throws an exception. 

void RootMeanSquaredError::check(void) const
{
   std::ostringstream buffer;

   // Neural network stuff

   if(!neural_network_pointer)
   {
      buffer << "OpenNN Exception: RootMeanSquaredError class.\n"
             << "void check(void) const method.\n"
             << "Pointer to neural network is NULL.\n";

      throw std::logic_error(buffer.str().c_str());	  
   }

   const MultilayerPerceptron* multilayer_perceptron_pointer = neural_network_pointer->get_multilayer_perceptron_pointer();

   if(!multilayer_perceptron_pointer)
   {
      buffer << "OpenNN Exception: RootMeanSquaredError class.\n"
             << "void check(void) const method.\n"
             << "Pointer to multilayer perceptron is NULL.\n";

      throw std::logic_error(buffer.str().c_str());	  
   }

   const unsigned int inputs_number = multilayer_perceptron_pointer->count_inputs_number();
   const unsigned int outputs_number = multilayer_perceptron_pointer->count_outputs_number();

   if(inputs_number == 0)
   {
      buffer << "OpenNN Exception: RootMeanSquaredError class.\n"
             << "void check(void) const method.\n"
             << "Number of inputs in multilayer perceptron object is zero.\n";

      throw std::logic_error(buffer.str().c_str());	  
   }

   if(outputs_number == 0)
   {
      buffer << "OpenNN Exception: RootMeanSquaredError class.\n"
             << "void check(void) const method.\n"
             << "Number of outputs in multilayer perceptron object is zero.\n";

      throw std::logic_error(buffer.str().c_str());	  
   }

   // Data set stuff

   if(!data_set_pointer)
   {
      buffer << "OpenNN Exception: RootMeanSquaredError class.\n"
             << "void check(void) const method.\n"
             << "Pointer to data set is NULL.\n";

      throw std::logic_error(buffer.str().c_str());	  
   }

   // Sum squared error stuff

   const VariablesInformation& variables_information = data_set_pointer->get_variables_information();

   const unsigned int targets_number = variables_information.count_targets_number();

   if(inputs_number != inputs_number)
   {
      buffer << "OpenNN Exception: RootMeanSquaredError class.\n"
             << "void check(void) const method.\n"
             << "Number of inputs in multilayer perceptron must be equal to number of inputs in data set.\n";

      throw std::logic_error(buffer.str().c_str());	  
   }

   if(outputs_number != targets_number)
   {
      buffer << "OpenNN Exception: RootMeanSquaredError class.\n"
             << "void check(void) const method.\n"
             << "Number of outputs in multilayer perceptron must be equal to number of targets in data set.\n";

      throw std::logic_error(buffer.str().c_str());
   }
}


// double calculate_evaluation(void) const method

/// This method returns the evaluation value of a neural network according to the root mean squared error 
/// on the training instances of a data set.

double RootMeanSquaredError::calculate_evaluation(void) const
{
   // Control sentence

   #ifdef _DEBUG 

   check();

   #endif

   // Neural network stuff

   const MultilayerPerceptron* multilayer_perceptron_pointer = neural_network_pointer->get_multilayer_perceptron_pointer();

   const unsigned int inputs_number = multilayer_perceptron_pointer->count_inputs_number();
   const unsigned int outputs_number = multilayer_perceptron_pointer->count_outputs_number();

   // Data set stuff

   const InstancesInformation& instances_information = data_set_pointer->get_instances_information();

   const unsigned int training_instances_number = instances_information.count_training_instances_number();

   // Root mean squared error

   Vector<double> inputs(inputs_number);
   Vector<double> outputs(outputs_number);
   Vector<double> targets(outputs_number);

   double sum_squared_error = 0.0;

   for(unsigned int i = 0; i < training_instances_number; i++)
   {
      // Input vector

	  inputs = data_set_pointer->get_training_input_instance(i);

      // Output vector

      outputs = multilayer_perceptron_pointer->calculate_outputs(inputs);

      // Target vector

      targets = data_set_pointer->get_training_target_instance(i);

      // Sum squaresd error

	  sum_squared_error += outputs.calculate_sum_squared_error(targets);           
   }

   return(sqrt(sum_squared_error/(double)training_instances_number));
}


// double calculate_evaluation(const Vector<double>&) const method

/// This method returns which would be the objective of a multilayer perceptron for an hypothetical 
/// vector of parameters. It does not set that vector of parameters to the multilayer perceptron. 
/// @param parameters Vector of a potential parameters for the multilayer perceptron associated 
/// to the performance term.

double RootMeanSquaredError::calculate_evaluation(const Vector<double>& parameters) const
{
   // Control sentence (if debug)

   #ifdef _DEBUG 
   
   check();

   #endif

   #ifdef _DEBUG 

   std::ostringstream buffer;

   unsigned int size = parameters.size();

   unsigned int parameters_number = neural_network_pointer->count_parameters_number();

   if(size != parameters_number)
   {
      buffer << "OpenNN Exception: RootMeanSquaredError class.\n"
             << "double calculate_evaluation(const Vector<double>&) const method.\n"
             << "Size (" << size << ") must be equal to number of parameters (" << parameters_number << ").\n";

      throw std::logic_error(buffer.str().c_str());	  
   }

   #endif

   NeuralNetwork neural_network_copy(*neural_network_pointer);

   neural_network_copy.set_parameters(parameters);

   RootMeanSquaredError root_mean_squared_error_copy(*this);

   root_mean_squared_error_copy.set_neural_network_pointer(&neural_network_copy);

   return(root_mean_squared_error_copy.calculate_evaluation());
}


// Vector<double> calculate_gradient(void) const method

/// This method calculates the gradient the root mean squared error funcion by means of the back-propagation algorithm.

Vector<double> RootMeanSquaredError::calculate_gradient(void) const
{
   // Control sentence

   #ifdef _DEBUG 

   check();

   #endif

   // Neural network stuff

   const MultilayerPerceptron* multilayer_perceptron_pointer = neural_network_pointer->get_multilayer_perceptron_pointer();

   const unsigned int inputs_number = multilayer_perceptron_pointer->count_inputs_number();
   const unsigned int outputs_number = multilayer_perceptron_pointer->count_outputs_number();

   const unsigned int layers_number = multilayer_perceptron_pointer->count_layers_number();

   const unsigned int parameters_number = multilayer_perceptron_pointer->count_parameters_number();

   // Data set stuff

   Vector< Vector< Vector<double> > > first_order_forward_propagation(2); 

   const bool& conditions_layer_flag = neural_network_pointer->get_conditions_layer_flag();

   const ConditionsLayer* conditions_layer_pointer = neural_network_pointer->get_conditions_layer_pointer();

   Vector<double> particular_solution;
   Vector<double> homogeneous_solution;

   // Data set stuff

   const InstancesInformation& instances_information = data_set_pointer->get_instances_information();

   const unsigned int training_instances_number = instances_information.count_training_instances_number();

   Vector<double> inputs(inputs_number);
   Vector<double> targets(outputs_number);

   // Performance functional stuff

   double objective = calculate_evaluation();

   Vector< Vector<double> > layers_delta; 

   Vector<double> output_objective_gradient(outputs_number);

   Vector<double> point_gradient(parameters_number, 0.0);

   Vector<double> objective_gradient(parameters_number, 0.0);

   // Main loop

   for(unsigned int i = 0; i < training_instances_number; i++)
   {
      inputs = data_set_pointer->get_training_input_instance(i);

      targets = data_set_pointer->get_training_target_instance(i);

      first_order_forward_propagation = multilayer_perceptron_pointer->calculate_first_order_forward_propagation(inputs);

	  const Vector< Vector<double> >& layers_activation = first_order_forward_propagation[0];
      const Vector< Vector<double> >& layers_activation_derivative = first_order_forward_propagation[1];

      if(!conditions_layer_flag)
      {
         output_objective_gradient = (layers_activation[layers_number-1]-targets)/(training_instances_number*objective);

         layers_delta = calculate_layers_delta(layers_activation_derivative, output_objective_gradient);
      }
      else
      {
         particular_solution = conditions_layer_pointer->calculate_particular_solution(inputs);
         homogeneous_solution = conditions_layer_pointer->calculate_homogeneous_solution(inputs);

         output_objective_gradient = (particular_solution+homogeneous_solution*layers_activation[layers_number-1] - targets)/(training_instances_number*objective);              

		 layers_delta = calculate_layers_delta(layers_activation_derivative, homogeneous_solution, output_objective_gradient);
      }

      point_gradient = calculate_point_gradient(inputs, layers_activation, layers_delta);
      objective_gradient += point_gradient;
   }

   return(objective_gradient);
}


// double calculate_generalization_evaluation(void) const method

/// This method returns the root mean squared error of the multilayer perceptron measured on the generalization instances of the data set.

double RootMeanSquaredError::calculate_generalization_evaluation(void) const
{
   // Control sentence (if debug)

   #ifdef _DEBUG 

   check();

   #endif

   const MultilayerPerceptron* multilayer_perceptron_pointer = neural_network_pointer->get_multilayer_perceptron_pointer();

   const unsigned int inputs_number = multilayer_perceptron_pointer->count_inputs_number();
   const unsigned int outputs_number = multilayer_perceptron_pointer->count_outputs_number();

   const InstancesInformation& instances_information = data_set_pointer->get_instances_information();
   const unsigned int generalization_instances_number = instances_information.count_generalization_instances_number();

   if(generalization_instances_number == 0)
   {
      return(0.0);
   }

   Vector<double> inputs(inputs_number);
   Vector<double> outputs(outputs_number);
   Vector<double> targets(outputs_number);

   double generalization_objective = 0.0;

   for(unsigned int i = 0; i < generalization_instances_number; i++)
   {
      // Input vector

	  inputs = data_set_pointer->get_generalization_input_instance(i);

      // Output vector

      outputs = multilayer_perceptron_pointer->calculate_outputs(inputs);

      // Target vector

      targets = data_set_pointer->get_generalization_target_instance(i);

      // Sum of squares error

      generalization_objective += outputs.calculate_sum_squared_error(targets);           
   }

   return(sqrt(generalization_objective/(double)generalization_instances_number));
}


// Matrix<double> calculate_Hessian(void) const method

/// @todo

Matrix<double> RootMeanSquaredError::calculate_Hessian(void) const
{
   Matrix<double> m;

   return(m);
}


// std::string write_performance_term_type(void) const method

/// This method returns a string with the name of the root mean squared error performance type, "ROOT_MEAN_SQUARED_ERROR".

std::string RootMeanSquaredError::write_performance_term_type(void) const
{
   return("ROOT_MEAN_SQUARED_ERROR");
}


// TiXmlElement* to_XML(void) const method 

/// This method serializes the root mean squared error object into a XML element of the TinyXML library. 
/// See the OpenNN manual for more information about the format of this element. 

TiXmlElement* RootMeanSquaredError::to_XML(void) const
{
   std::ostringstream buffer;

   // Root mean squared error

   TiXmlElement* root_mean_squared_error_element = new TiXmlElement("RootMeanSquaredError");
   root_mean_squared_error_element->SetAttribute("Version", 4);

   // Display
   {
      TiXmlElement* display_element = new TiXmlElement("Display");
      root_mean_squared_error_element->LinkEndChild(display_element);

      buffer.str("");
      buffer << display;

      TiXmlText* display_text = new TiXmlText(buffer.str().c_str());
      display_element->LinkEndChild(display_text);
   }

   return(root_mean_squared_error_element);
}


// void from_XML(TiXmlElement*) method

/// @todo

void RootMeanSquaredError::from_XML(TiXmlElement*)   
{
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
