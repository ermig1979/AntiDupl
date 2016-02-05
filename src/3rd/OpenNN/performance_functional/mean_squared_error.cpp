/****************************************************************************************************************/
/*                                                                                                              */
/*   OpenNN: Open Neural Networks Library                                                                       */
/*   www.opennn.cimne.com                                                                                       */
/*                                                                                                              */
/*   M E A N   S Q U A R E D   E R R O R   C L A S S                                                            */
/*                                                                                                              */
/*   Roberto Lopez                                                                                              */
/*   International Center for Numerical Methods in Engineering (CIMNE)                                          */
/*   Technical University of Catalonia (UPC)                                                                    */
/*   Barcelona, Spain                                                                                           */
/*   E-mail: rlopez@cimne.upc.edu                                                                               */
/*                                                                                                              */
/****************************************************************************************************************/

// System includes

#include <string>
#include <sstream>
#include <iostream>
#include <fstream>
#include <limits>
#include <math.h>

// OpenNN includes

#include "mean_squared_error.h"


// TinyXml includes

#include "../../tinyxml/tinyxml.h"


namespace OpenNN
{
// DEFAULT CONSTRUCTOR

/// Default constructor. 
/// It creates a mean squared error performance term not associated to any 
/// neural network and not measured on any data set.
/// It also initializes all the rest of class members to their default values.

MeanSquaredError::MeanSquaredError(void) : PerformanceTerm()
{
}


// NEURAL NETWORK CONSTRUCTOR

/// Neural network constructor. 
/// It creates a mean squared error performance term object associated to a 
/// neural network object but not measured on any data set object.
/// It also initializes all the rest of class members to their default values.
/// @param new_neural_network_pointer Pointer to a neural network object.

MeanSquaredError::MeanSquaredError(NeuralNetwork* new_neural_network_pointer)
: PerformanceTerm(new_neural_network_pointer)
{
}


// DATA SET CONSTRUCTOR

/// Data set constructor. 
/// It creates a mean squared error performance term not associated to any 
/// neural network but to be measured on a given data set object.
/// It also initializes all the rest of class members to their default values.
/// @param new_data_set_pointer Pointer to a data set object.

MeanSquaredError::MeanSquaredError(DataSet* new_data_set_pointer)
: PerformanceTerm(new_data_set_pointer)
{
}


// NEURAL NETWORK AND DATA SET CONSTRUCTOR

/// Neural network and data set constructor. 
/// It creates a mean squared error performance term object associated to a 
/// neural network and measured on a data set.
/// It also initializes all the rest of class members to their default values.
/// @param new_neural_network_pointer Pointer to a neural network object.
/// @param new_data_set_pointer Pointer to a data set object.

MeanSquaredError::MeanSquaredError(NeuralNetwork* new_neural_network_pointer, DataSet* new_data_set_pointer)
: PerformanceTerm(new_neural_network_pointer, new_data_set_pointer)
{
}


// XML CONSTRUCTOR

/// XML constructor. 
/// It creates a mean squared error object with all pointers set to NULL. 
/// The object members are loaded by means of a XML element.
/// Please be careful with the format of that file, which is specified in the OpenNN manual.
/// @param mean_squared_error_element Tiny XML element with the mean squared error elements.

MeanSquaredError::MeanSquaredError(TiXmlElement* mean_squared_error_element)
 : PerformanceTerm(mean_squared_error_element)
{
}


// COPY CONSTRUCTOR

/// Copy constructor. 
/// It creates a copy of an existing mean squared error object. 
/// @param other_mean_squared_error Mean squared error object to be copied.

MeanSquaredError::MeanSquaredError(const MeanSquaredError& other_mean_squared_error)
: PerformanceTerm(other_mean_squared_error)
{
}


// DESTRUCTOR

/// Destructor.

MeanSquaredError::~MeanSquaredError(void)
{
}


// METHODS

// void check(void) const method

/// This method checks that there are a neural network and a data set associated to the mean squared error, 
/// and that the numbers of inputs and outputs in the neural network are equal to the numbers of inputs and targets in the data set. 
/// If some of the above conditions is not hold, the method throws an exception. 

void MeanSquaredError::check(void) const
{
   std::ostringstream buffer;

   // Neural network stuff

   if(!neural_network_pointer)
   {
      buffer << "OpenNN Exception: MeanSquaredError class.\n"
             << "void check(void) const method.\n"
             << "Pointer to neural network is NULL.\n";

      throw std::logic_error(buffer.str().c_str());	  
   }

   const MultilayerPerceptron* multilayer_perceptron_pointer = neural_network_pointer->get_multilayer_perceptron_pointer();

   if(!multilayer_perceptron_pointer)
   {
      buffer << "OpenNN Exception: MeanSquaredError class.\n"
             << "void check(void) const method.\n"
             << "Pointer to multilayer perceptron is NULL.\n";

      throw std::logic_error(buffer.str().c_str());	  
   }

   const unsigned int inputs_number = multilayer_perceptron_pointer->count_inputs_number();
   const unsigned int outputs_number = multilayer_perceptron_pointer->count_outputs_number();

   if(inputs_number == 0)
   {
      buffer << "OpenNN Exception: MeanSquaredError class.\n"
             << "void check(void) const method.\n"
             << "Number of inputs in multilayer perceptron object is zero.\n";

      throw std::logic_error(buffer.str().c_str());	  
   }

   if(outputs_number == 0)
   {
      buffer << "OpenNN Exception: MeanSquaredError class.\n"
             << "void check(void) const method.\n"
             << "Number of outputs in multilayer perceptron object is zero.\n";

      throw std::logic_error(buffer.str().c_str());	  
   }

   // Data set stuff

   if(!data_set_pointer)
   {
      buffer << "OpenNN Exception: MeanSquaredError class.\n"
             << "void check(void) const method.\n"
             << "Pointer to data set is NULL.\n";

      throw std::logic_error(buffer.str().c_str());	  
   }

   // Sum squared error stuff

   const VariablesInformation& variables_information = data_set_pointer->get_variables_information();

   const unsigned int targets_number = variables_information.count_targets_number();

   if(inputs_number != inputs_number)
   {
      buffer << "OpenNN Exception: MeanSquaredError class.\n"
             << "void check(void) const method.\n"
             << "Number of inputs in multilayer perceptron must be equal to number of inputs in data set.\n";

      throw std::logic_error(buffer.str().c_str());	  
   }

   if(outputs_number != targets_number)
   {
      buffer << "OpenNN Exception: MeanSquaredError class.\n"
             << "void check(void) const method.\n"
             << "Number of outputs in multilayer perceptron must be equal to number of targets in data set.\n";

      throw std::logic_error(buffer.str().c_str());
   }
}


// double calculate_evaluation(void) const method

/// This method returns the objective value of a multilayer perceptron according to the mean squared error on 
/// a data set.

double MeanSquaredError::calculate_evaluation(void) const
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

   // Mean squared error stuff

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

      // Sum squared error

	  sum_squared_error += outputs.calculate_sum_squared_error(targets);
   }

   return(sum_squared_error/(double)training_instances_number);
}


// double calculate_evaluation(const Vector<double>&) const method

/// This method returns which would be the objective of a multilayer perceptron for an hypothetical 
/// vector of parameters. It does not set that vector of parameters to the neural network. 
/// @param parameters Vector of a potential parameters for the neural network associated to the performance term.

double MeanSquaredError::calculate_evaluation(const Vector<double>& parameters) const
{
   // Control sentence (if debug)

   #ifdef _DEBUG 

   check();

   #endif


   #ifdef _DEBUG 

   unsigned int size = parameters.size();

   unsigned int parameters_number = neural_network_pointer->count_parameters_number();

   if(size != parameters_number)
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: MeanSquaredError class.\n"
             << "double calculate_evaluation(const Vector<double>&) const method.\n"
             << "Size (" << size << ") must be equal to number of parameters (" << parameters_number << ").\n";

      throw std::logic_error(buffer.str().c_str());
   }

   #endif

   NeuralNetwork neural_network_copy(*neural_network_pointer);

   neural_network_copy.set_parameters(parameters);

   MeanSquaredError mean_squared_error_copy(*this);

   mean_squared_error_copy.set_neural_network_pointer(&neural_network_copy);

   return(mean_squared_error_copy.calculate_evaluation());
}


// double calculate_generalization_evaluation(void) const method

/// This method returns the mean squared error of the multilayer perceptron measured on the generalization instances of the 
/// data set.

double MeanSquaredError::calculate_generalization_evaluation(void) const
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
   else
   {
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

      return(generalization_objective/(double)generalization_instances_number);
   }
}


// Vector<double> calculate_gradient(void) const method

/// This method calculates the objective gradient by means of the back-propagation algorithm, 
/// and returns it in a single vector of size the number of multilayer_perceptron_pointer parameters. 

Vector<double> MeanSquaredError::calculate_gradient(void) const
{
   // Control sentence (if debug)

   #ifdef _DEBUG 

   check();

   #endif

   // Multilayer percepron stuff

   const MultilayerPerceptron* multilayer_perceptron_pointer = neural_network_pointer->get_multilayer_perceptron_pointer();

   const unsigned int inputs_number = multilayer_perceptron_pointer->count_inputs_number();
   const unsigned int outputs_number = multilayer_perceptron_pointer->count_outputs_number();

   const unsigned int layers_number = multilayer_perceptron_pointer->count_layers_number();

   const unsigned int parameters_number = multilayer_perceptron_pointer->count_parameters_number();

   Vector< Vector< Vector<double> > > first_order_forward_propagation(2); 

   const ConditionsLayer* conditions_layer_pointer = neural_network_pointer->get_conditions_layer_pointer();

   const bool& conditions_layer_flag = neural_network_pointer->get_conditions_layer_flag();

   Vector<double> particular_solution;
   Vector<double> homogeneous_solution;

   // Data set stuff

   const InstancesInformation& instances_information = data_set_pointer->get_instances_information();

   const unsigned int training_instances_number = instances_information.count_training_instances_number();

   Vector<double> inputs(inputs_number);
   Vector<double> targets(outputs_number);

   // Mean squared error stuff

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
         output_objective_gradient = (layers_activation[layers_number-1]-targets)*(2.0/(double)training_instances_number);

         layers_delta = calculate_layers_delta(layers_activation_derivative, output_objective_gradient);
      }
      else
      {
         particular_solution = conditions_layer_pointer->calculate_particular_solution(inputs);
         homogeneous_solution = conditions_layer_pointer->calculate_homogeneous_solution(inputs);

         output_objective_gradient = (particular_solution+homogeneous_solution*layers_activation[layers_number-1] - targets)*(2.0/(double)training_instances_number);              

		 layers_delta = calculate_layers_delta(layers_activation_derivative, homogeneous_solution, output_objective_gradient);
      }

      point_gradient = calculate_point_gradient(inputs, layers_activation, layers_delta);

      objective_gradient += point_gradient;
   }

   return(objective_gradient);
}


// Matrix<double> calculate_Hessian(void) const method

/// @todo

Matrix<double> MeanSquaredError::calculate_Hessian(void) const
{
   Matrix<double> H;

   return(H);
}


// FirstOrderEvaluation calculate_first_order_evaluation(void) const method

/// @todo

PerformanceTerm::FirstOrderEvaluation MeanSquaredError::calculate_first_order_evaluation(void) const
{
   FirstOrderEvaluation first_order_evaluation;

   first_order_evaluation.evaluation = calculate_evaluation();
   first_order_evaluation.gradient = calculate_gradient();

   return(first_order_evaluation);
}


// SecondOrderEvaluation calculate_second_order_evaluation(void) const method

/// @todo

PerformanceTerm::SecondOrderEvaluation MeanSquaredError::calculate_second_order_evaluation(void) const
{
   SecondOrderEvaluation second_order_evaluation;

   second_order_evaluation.evaluation = calculate_evaluation();
   second_order_evaluation.gradient = calculate_gradient();
   second_order_evaluation.Hessian = calculate_Hessian();

   return(second_order_evaluation);
}


// Vector<double> calculate_evaluation_terms(void) const method

/// This method returns evaluation vector of the objective terms function for the mean squared error. 
/// It uses the error back-propagation method.

Vector<double> MeanSquaredError::calculate_evaluation_terms(void) const
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

   // Mean squared error stuff

   Vector<double> evaluation_terms(training_instances_number);

   Vector<double> inputs(inputs_number);
   Vector<double> outputs(outputs_number);
   Vector<double> targets(outputs_number);

   for(unsigned int i = 0; i < training_instances_number; i++)
   {
      // Input vector

	  inputs = data_set_pointer->get_training_input_instance(i);

      // Output vector

      outputs = multilayer_perceptron_pointer->calculate_outputs(inputs);

      // Target vector

      targets = data_set_pointer->get_training_target_instance(i);

      // Error

      evaluation_terms[i] = outputs.calculate_distance(targets);
   }

   return(evaluation_terms/sqrt((double)training_instances_number));
}


// Vector<double> calculate_evaluation_terms(const Vector<double>&) const method

/// This method returns which would be the objective terms evaluation vector of a multilayer perceptron for an hypothetical vector of multilayer_perceptron_pointer parameters. 
/// It does not set that vector of parameters to the multilayer perceptron. 
/// @param network_parameters Vector of a potential multilayer_perceptron_pointer parameters for the multilayer perceptron associated to the performance functional.

Vector<double> MeanSquaredError::calculate_evaluation_terms(const Vector<double>& network_parameters) const
{
   // Control sentence (if debug)

   #ifdef _DEBUG 

   check();

   #endif

   #ifdef _DEBUG 

   std::ostringstream buffer;

   const unsigned int& size = network_parameters.size();

   const unsigned int parameters_number = neural_network_pointer->count_parameters_number();

   if(size != parameters_number)
   {
      buffer << "OpenNN Exception: MeanSquaredError class.\n"
             << "double calculate_evaluation_terms(const Vector<double>&) const method.\n"
             << "Size (" << size << ") must be equal to number of multilayer_perceptron_pointer parameters (" << parameters_number << ").\n";

      throw std::logic_error(buffer.str().c_str());
   }

   #endif

   NeuralNetwork neural_network_copy(*neural_network_pointer);

   neural_network_copy.set_parameters(network_parameters);

   MeanSquaredError mean_squared_error_copy(*this);

   mean_squared_error_copy.set_neural_network_pointer(&neural_network_copy);

   return(mean_squared_error_copy.calculate_evaluation_terms());
}


// Matrix<double> calculate_Jacobian_terms(void) const method

/// This method returns the Jacobian matrix of the mean squared error function, whose elements are given by the 
/// derivatives of the squared errors data set with respect to the multilayer_perceptron_pointer parameters.

Matrix<double> MeanSquaredError::calculate_Jacobian_terms(void) const
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

   const unsigned int network_parameters_number = multilayer_perceptron_pointer->count_parameters_number();

   Vector< Vector< Vector<double> > > first_order_forward_propagation(2);

   Vector<double> particular_solution;
   Vector<double> homogeneous_solution;

   const ConditionsLayer* conditions_layer_pointer = neural_network_pointer->get_conditions_layer_pointer();

   const bool& conditions_layer_flag = neural_network_pointer->get_conditions_layer_flag();

   // Data set stuff

   const InstancesInformation& instances_information = data_set_pointer->get_instances_information();

   const unsigned int training_instances_number = instances_information.count_training_instances_number();

   Vector<double> inputs(inputs_number);
   Vector<double> targets(outputs_number);

   // Objective functional

   Vector<double> term(outputs_number);
   double term_norm;

   Vector<double> output_objective_gradient(outputs_number);

   Vector< Vector<double> > layers_delta(layers_number);
   Vector<double> point_gradient(network_parameters_number);

   Matrix<double> Jacobian_terms(training_instances_number, network_parameters_number);

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
         const Vector<double>& outputs = first_order_forward_propagation[0][layers_number-1]; 

         term = (outputs-targets);
         term_norm = term.calculate_norm();

         if(term_norm == 0.0)
   	     {
	        output_objective_gradient.initialize(0.0);
	     }
         else
	     {
            output_objective_gradient = term/term_norm;	      
	     }

         layers_delta = calculate_layers_delta(layers_activation_derivative, output_objective_gradient);
      }
      else
      {
         particular_solution = conditions_layer_pointer->calculate_particular_solution(inputs);
         homogeneous_solution = conditions_layer_pointer->calculate_homogeneous_solution(inputs);

         term = (particular_solution+homogeneous_solution*layers_activation[layers_number-1] - targets)/sqrt((double)training_instances_number);              
         term_norm = term.calculate_norm();

         if(term_norm == 0.0)
   	     {
	        output_objective_gradient.initialize(0.0);
	     }
	     else
	     {
            output_objective_gradient = term/term_norm;	      
	     }

		 layers_delta = calculate_layers_delta(layers_activation_derivative, homogeneous_solution, output_objective_gradient);
	  }

      point_gradient = calculate_point_gradient(inputs, layers_activation, layers_delta);

      Jacobian_terms.set_row(i, point_gradient);
  }

   return(Jacobian_terms/sqrt((double)training_instances_number));
}


// FirstOrderEvaluationTerms calculate_first_order_evaluation_terms(void) method

/// This method returns a first order objective terms evaluation structure, which contains the values and the Jacobian of the objective terms function. 

/// @todo

MeanSquaredError::FirstOrderEvaluationTerms MeanSquaredError::calculate_first_order_evaluation_terms(void)
{
   FirstOrderEvaluationTerms first_order_evaluation_terms;

   first_order_evaluation_terms.evaluation_terms = calculate_evaluation_terms();

   first_order_evaluation_terms.Jacobian_terms = calculate_Jacobian_terms();

   return(first_order_evaluation_terms);
}


// std::string write_performance_term_type(void) const method

/// This method returns a string with the name of the mean squared error performance type, "MEAN_SQUARED_ERROR".

std::string MeanSquaredError::write_performance_term_type(void) const
{
   return("MEAN_SQUARED_ERROR");
}


// TiXmlElement* to_XML(void) const method 

/// This method serializes the mean squared error object into a XML element of the TinyXML library. 
/// See the OpenNN manual for more information about the format of this element. 

TiXmlElement* MeanSquaredError::to_XML(void) const
{
   std::ostringstream buffer;

   // Mean squared error

   TiXmlElement* mean_squared_error_element = new TiXmlElement("MeanSquaredError");
   mean_squared_error_element->SetAttribute("Version", 4);

   // Display

   {
      TiXmlElement* element = new TiXmlElement("Display");
      mean_squared_error_element->LinkEndChild(element);

      buffer.str("");
      buffer << display;

      TiXmlText* text = new TiXmlText(buffer.str().c_str());
      element->LinkEndChild(text);
   }

   return(mean_squared_error_element);
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
