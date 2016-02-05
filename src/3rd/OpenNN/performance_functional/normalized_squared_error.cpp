/****************************************************************************************************************/
/*                                                                                                              */
/*   OpenNN: Open Neural Networks Library                                                                       */
/*   www.opennn.cimne.com                                                                                       */
/*                                                                                                              */
/*   N O R M A L I Z E D   S Q U A R E D   E R R O R   C L A S S                                                */
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
#include <cmath>

// OpenNN includes

#include "normalized_squared_error.h"

// TinyXml includes

#include "../../tinyxml/tinyxml.h"


namespace OpenNN
{

// DEFAULT CONSTRUCTOR

/// Default constructor. 
/// It creates a normalized squared error performance term object not associated to any 
/// neural network and not measured on any data set.
/// It also initializes all the rest of class members to their default values.

NormalizedSquaredError::NormalizedSquaredError(void) : PerformanceTerm()
{
}


// NEURAL NETWORK CONSTRUCTOR

/// Neural network constructor. 
/// It creates a normalized squared error performance term associated to a neural network object but not measured on any data set.
/// It also initializes all the rest of class members to their default values.
/// @param new_neural_network_pointer Pointer to a neural network object.

NormalizedSquaredError::NormalizedSquaredError(NeuralNetwork* new_neural_network_pointer)
: PerformanceTerm(new_neural_network_pointer)
{
}


// DATA SET CONSTRUCTOR

/// Data set constructor. 
/// It creates a normalized squared error performance term not associated to any 
/// neural network but to be measured on a data set object.
/// It also initializes all the rest of class members to their default values.
/// @param new_data_set_pointer Pointer to a data set object.

NormalizedSquaredError::NormalizedSquaredError(DataSet* new_data_set_pointer) 
: PerformanceTerm(new_data_set_pointer)
{
}


// NEURAL NETWORK AND DATA SET CONSTRUCTOR

/// Neural network and data set constructor. 
/// It creates a normalized squared error performance term associated to a neural network and measured on a data set.
/// It also initializes all the rest of class members to their default values.
/// @param new_neural_network_pointer Pointer to a neural network object.
/// @param new_data_set_pointer Pointer to a data set object.

NormalizedSquaredError::NormalizedSquaredError(NeuralNetwork* new_neural_network_pointer, DataSet* new_data_set_pointer)
: PerformanceTerm(new_neural_network_pointer, new_data_set_pointer)
{
}


// XML CONSTRUCTOR

/// XML constructor. 
/// It creates a normalized squared error not associated to any neural network and not measured on any data set.
/// It also sets all the rest of class members from a TinyXML element.
/// @param normalized_squared_error_element XML element with the class members. 

NormalizedSquaredError::NormalizedSquaredError(TiXmlElement* normalized_squared_error_element) 
 : PerformanceTerm(normalized_squared_error_element)
{
}


// DESTRUCTOR

/// Destructor.

NormalizedSquaredError::~NormalizedSquaredError(void)
{
}


// METHODS

// double calculate_training_normalization_coefficient(const Matrix<double>&, const Vector<double>&) const method

/// This method returns the normalization coefficient to be used for the evaluation of the error. 
/// This is measured on the training instances of the data set. 

double NormalizedSquaredError::calculate_normalization_coefficient(const Matrix<double>& target_data, const Vector<double>& target_data_mean) const
{
   return(target_data.calculate_sum_squared_error(target_data_mean));
}


// void check(void) const method

/// This method checks that there are a neural network and a data set associated to the normalized squared error, 
/// and that the numbers of inputs and outputs in the neural network are equal to the numbers of inputs and targets in the data set. 
/// If some of the above conditions is not hold, the method throws an exception. 

void NormalizedSquaredError::check(void) const
{
   std::ostringstream buffer;

   // Neural network stuff

   if(!neural_network_pointer)
   {
      buffer << "OpenNN Exception: NormalizedquaredError class.\n"
             << "void check(void) const method.\n"
             << "Pointer to neural network is NULL.\n";

      throw std::logic_error(buffer.str().c_str());	  
   }

   const MultilayerPerceptron* multilayer_perceptron_pointer = neural_network_pointer->get_multilayer_perceptron_pointer();

   if(!multilayer_perceptron_pointer)
   {
      buffer << "OpenNN Exception: NormalizedquaredError class.\n"
             << "void check(void) const method.\n"
             << "Pointer to multilayer perceptron is NULL.\n";

      throw std::logic_error(buffer.str().c_str());	  
   }

   const unsigned int multilayer_perceptron_inputs_number = multilayer_perceptron_pointer->count_inputs_number();
   const unsigned int multilayer_perceptron_outputs_number = multilayer_perceptron_pointer->count_outputs_number();

   if(multilayer_perceptron_inputs_number == 0)
   {
      buffer << "OpenNN Exception: NormalizedquaredError class.\n"
             << "void check(void) const method.\n"
             << "Number of inputs in multilayer perceptron object is zero.\n";

      throw std::logic_error(buffer.str().c_str());	  
   }

   if(multilayer_perceptron_outputs_number == 0)
   {
      buffer << "OpenNN Exception: NormalizedquaredError class.\n"
             << "void check(void) const method.\n"
             << "Number of outputs in multilayer perceptron object is zero.\n";

      throw std::logic_error(buffer.str().c_str());	  
   }

   // Data set stuff

   if(!data_set_pointer)
   {
      buffer << "OpenNN Exception: NormalizedquaredError class.\n"
             << "void check(void) const method.\n"
             << "Pointer to data set is NULL.\n";

      throw std::logic_error(buffer.str().c_str());	  
   }

   // Sum squared error stuff

   const VariablesInformation& variables_information = data_set_pointer->get_variables_information();

   const unsigned int data_set_inputs_number = variables_information.count_inputs_number();
   const unsigned int data_set_targets_number = variables_information.count_targets_number();

   if(multilayer_perceptron_inputs_number != data_set_inputs_number)
   {
      buffer << "OpenNN Exception: NormalizedquaredError class.\n"
             << "void check(void) const method.\n"
             << "Number of inputs in multilayer perceptron (" << multilayer_perceptron_inputs_number << ") must be equal to number of inputs in data set (" << data_set_inputs_number << ").\n";

      throw std::logic_error(buffer.str().c_str());	  
   }

   if(multilayer_perceptron_outputs_number != data_set_targets_number)
   {
      buffer << "OpenNN Exception: NormalizedquaredError class.\n"
             << "void check(void) const method.\n"
             << "Number of outputs in multilayer perceptron (" << multilayer_perceptron_outputs_number << ") must be equal to number of targets in data set (" << data_set_targets_number << ").\n";

      throw std::logic_error(buffer.str().c_str());
   }
}


// double calculate_evaluation(void) const method

/// This method returns the evaluation value of a multilayer perceptron according to the normalized squared error 
/// on a data set.

double NormalizedSquaredError::calculate_evaluation(void) const
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

   const Vector<double> training_target_data_mean = data_set_pointer->calculate_training_target_data_mean();

   // Normalized squared error stuff 

   Vector<double> inputs(inputs_number);
   Vector<double> outputs(outputs_number);
   Vector<double> targets(outputs_number);

   double sum_squared_error = 0.0;
   double normalization_coefficient = 0.0;

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

	  // Normalization coefficient

	  normalization_coefficient += targets.calculate_sum_squared_error(training_target_data_mean);
   }

   if(normalization_coefficient < 1.0e-99)
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: NormalizedSquaredError class.\n"
		     << "double calculate_evaluation(void) const method.\n"
             << "Normalization coefficient is zero.\n";

      throw std::logic_error(buffer.str().c_str());
   }

   return(sum_squared_error/normalization_coefficient);
}


// double calculate_evaluation(const Vector<double>&) const method

/// This method returns which would be the objective of a multilayer perceptron for an hypothetical vector of parameters. 
/// It does not set that vector of parameters to the multilayer perceptron. 
/// @param parameters Vector of a potential parameters for the multilayer perceptron associated to the performance functional.

double NormalizedSquaredError::calculate_evaluation(const Vector<double>& parameters) const 
{
   // Control sentence (if debug)

   #ifdef _DEBUG 

   check();

   #endif

   #ifdef _DEBUG 

   std::ostringstream buffer;

   const unsigned int size = parameters.size();

   const unsigned int parameters_number = neural_network_pointer->count_parameters_number();

   if(size != parameters_number)
   {
      buffer << "OpenNN Exception: NormalizedSquaredError class.\n"
             << "double calculate_evaluation(const Vector<double>&) method.\n"
             << "Size (" << size << ") must be equal to number of parameters (" << parameters_number << ").\n";

      throw std::logic_error(buffer.str().c_str());
   }

   #endif

   NeuralNetwork neural_network_copy(*neural_network_pointer);

   neural_network_copy.set_parameters(parameters);

   NormalizedSquaredError normalized_squared_error_copy(*this);

   normalized_squared_error_copy.set_neural_network_pointer(&neural_network_copy);

   return(normalized_squared_error_copy.calculate_evaluation());
}


// double calculate_generalization_evaluation(void) const method

double NormalizedSquaredError::calculate_generalization_evaluation(void) const
{
   // Control sentence

   #ifdef _DEBUG 

   check();

   #endif

   // Neural networks stuff

   const MultilayerPerceptron* multilayer_perceptron_pointer = neural_network_pointer->get_multilayer_perceptron_pointer();

   const unsigned int inputs_number = multilayer_perceptron_pointer->count_inputs_number();
   const unsigned int outputs_number = multilayer_perceptron_pointer->count_outputs_number();

   const InstancesInformation& instances_information = data_set_pointer->get_instances_information();

   const unsigned int generalization_instances_number = instances_information.count_generalization_instances_number();

   if(generalization_instances_number < 2)
   {
      return(0.0);
   }
   
   const Vector<double> generalization_target_data_mean = data_set_pointer->calculate_generalization_target_data_mean();

   Vector<double> inputs(inputs_number);
   Vector<double> outputs(outputs_number);
   Vector<double> targets(outputs_number);

   double sum_squared_error = 0.0;
   double normalization_coefficient = 0.0;

   for(unsigned int i = 0; i < generalization_instances_number; i++)
   {
      // Input vector

	  inputs = data_set_pointer->get_generalization_input_instance(i);

      // Output vector

      outputs = multilayer_perceptron_pointer->calculate_outputs(inputs);

      // Target vector

      targets = data_set_pointer->get_generalization_target_instance(i);

      // Sum squared error

	  sum_squared_error += outputs.calculate_sum_squared_error(targets);

	  // Normalization coefficient

	  normalization_coefficient += targets.calculate_sum_squared_error(generalization_target_data_mean);
   }

   if(normalization_coefficient < 1.0e-99)
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: NormalizedSquaredError class.\n"
		     << "double calculate_generalization_evaluation(void) const method.\n"
             << "Normalization coefficient is zero.\n";

      throw std::logic_error(buffer.str().c_str());
   }

   return(sum_squared_error/normalization_coefficient);
}


// Vector<double> calculate_gradient(void) const method

/// This method returns the normalized squared error function gradient of a multilayer perceptron on a data set. 
/// It uses the error back-propagation method.

Vector<double> NormalizedSquaredError::calculate_gradient(void) const
{
   // Control sentence (if debug)

   #ifdef _DEBUG 

   check();

   #endif

   // Neural network stuff

   const unsigned int parameters_number = neural_network_pointer->count_parameters_number();

   const MultilayerPerceptron* multilayer_perceptron_pointer = neural_network_pointer->get_multilayer_perceptron_pointer();

   const unsigned int inputs_number = multilayer_perceptron_pointer->count_inputs_number();
   const unsigned int outputs_number = multilayer_perceptron_pointer->count_outputs_number();

   const unsigned int layers_number = multilayer_perceptron_pointer->count_layers_number();

   Vector< Vector< Vector<double> > > first_order_forward_propagation(2); 

   Vector< Vector<double> > layers_inputs(layers_number); 

   Vector< Matrix<double> > layers_combination_parameters_Jacobian; 

   const ConditionsLayer* conditions_layer_pointer = neural_network_pointer->get_conditions_layer_pointer();

   const bool& conditions_layer_flag = neural_network_pointer->get_conditions_layer_flag();

   Vector<double> particular_solution;
   Vector<double> homogeneous_solution;

   // Data set stuff

   const InstancesInformation& instances_information = data_set_pointer->get_instances_information();

   const unsigned int training_instances_number = instances_information.count_training_instances_number();

   const Vector<double> training_target_data_mean = data_set_pointer->calculate_training_target_data_mean();

   Vector<double> inputs(inputs_number);
   Vector<double> targets(outputs_number);

   // Normalized squared error stuff

   Vector<double> output_objective_gradient(outputs_number);

   Vector< Vector<double> > layers_delta; 

   Vector<double> gradient(parameters_number, 0.0);

   double normalization_coefficient = 0.0;

   // Main loop

   for(unsigned int i = 0; i < training_instances_number; i++)
   {
      // Data set

      inputs = data_set_pointer->get_training_input_instance(i);

      targets = data_set_pointer->get_training_target_instance(i);

	  // Multilayer perceptron

      first_order_forward_propagation = multilayer_perceptron_pointer->calculate_first_order_forward_propagation(inputs);
      const Vector< Vector<double> >& layers_activation = first_order_forward_propagation[0];
      const Vector< Vector<double> >& layers_activation_derivative = first_order_forward_propagation[1];

      layers_inputs = multilayer_perceptron_pointer->arrange_layers_input(inputs, layers_activation);   

	  layers_combination_parameters_Jacobian = multilayer_perceptron_pointer->calculate_layers_combination_parameters_Jacobian(layers_inputs);

	  // Performance functional

      if(!conditions_layer_flag)
      {
         output_objective_gradient = (layers_activation[layers_number-1]-targets)*2.0;

         layers_delta = calculate_layers_delta(layers_activation_derivative, output_objective_gradient);
      }
      else
      {
         particular_solution = conditions_layer_pointer->calculate_particular_solution(inputs);
         homogeneous_solution = conditions_layer_pointer->calculate_homogeneous_solution(inputs);

         output_objective_gradient = (particular_solution+homogeneous_solution*layers_activation[layers_number-1] - targets)*2.0;              

		 layers_delta = calculate_layers_delta(layers_activation_derivative, homogeneous_solution, output_objective_gradient);
      }

	  normalization_coefficient += targets.calculate_sum_squared_error(training_target_data_mean);

      gradient += calculate_point_gradient(layers_combination_parameters_Jacobian, layers_delta);
   }

   return(gradient/normalization_coefficient);
}
	

// Matrix<double> calculate_Hessian(void) const method

/// This method returns the normalized squared error function Hessian of a multilayer perceptron on a data set. 
/// It uses the error back-propagation method.
/// @todo

Matrix<double> NormalizedSquaredError::calculate_Hessian(void) const
{
   Matrix<double> objective_Hessian;

   return(objective_Hessian);
}


// Vector<double> calculate_evaluation_terms(void) const method

/// This method returns evaluation vector of the objective terms function for the normalized squared error. 
/// It uses the error back-propagation method.

Vector<double> NormalizedSquaredError::calculate_evaluation_terms(void) const
{
   // Control sentence (if debug)

   #ifdef _DEBUG 

   check();

   #endif

   // Neural network stuff

   const MultilayerPerceptron* multilayer_perceptron_pointer = neural_network_pointer->get_multilayer_perceptron_pointer();

   unsigned int inputs_number = multilayer_perceptron_pointer->count_inputs_number();
   unsigned int outputs_number = multilayer_perceptron_pointer->count_outputs_number();

   // Data set stuff

   const InstancesInformation& instances_information = data_set_pointer->get_instances_information();

   const unsigned int training_instances_number = instances_information.count_training_instances_number();

   const Vector<double> training_target_data_mean = data_set_pointer->calculate_training_target_data_mean();

   // Calculate

   Vector<double> evaluation_terms(training_instances_number);

   Vector<double> inputs(inputs_number);
   Vector<double> outputs(outputs_number);
   Vector<double> targets(outputs_number);

   double normalization_coefficient = 0.0;

   for(unsigned int i = 0; i < training_instances_number; i++)
   {
      // Input vector

	  inputs = data_set_pointer->get_training_input_instance(i);

      // Output vector

      outputs = multilayer_perceptron_pointer->calculate_outputs(inputs);

      // Target vector

      targets = data_set_pointer->get_training_target_instance(i);

      // Sum squared error

	  evaluation_terms[i] = outputs.calculate_distance(targets);

	  // Normalization coefficient

	  normalization_coefficient += targets.calculate_sum_squared_error(training_target_data_mean);
   }

   if(normalization_coefficient < 1.0e-99)
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: NormalizedSquaredError class.\n"
		     << "double calculate_evaluation_terms(void) const method.\n"
             << "Normalization coefficient is zero.\n";

      throw std::logic_error(buffer.str().c_str());
   }

   return(evaluation_terms/sqrt(normalization_coefficient));
}


// Vector<double> calculate_evaluation_terms(const Vector<double>&) const method

/// This method returns which would be the objective terms evaluation vector of a multilayer perceptron for an hypothetical vector of multilayer_perceptron_pointer parameters. 
/// It does not set that vector of parameters to the multilayer perceptron. 
/// @param network_parameters Vector of a potential multilayer_perceptron_pointer parameters for the multilayer perceptron associated to the performance functional.

Vector<double> NormalizedSquaredError::calculate_evaluation_terms(const Vector<double>& network_parameters) const
{
   // Control sentence (if debug)

   #ifdef _DEBUG 

   check();

   #endif


   #ifdef _DEBUG 

   const unsigned int size = network_parameters.size();

   const MultilayerPerceptron* multilayer_perceptron_pointer = neural_network_pointer->get_multilayer_perceptron_pointer();

   const unsigned int network_parameters_number = multilayer_perceptron_pointer->count_parameters_number();

   if(size != network_parameters_number)
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: NormalizedSquaredError class.\n"
             << "double calculate_evaluation_terms(const Vector<double>&) const method.\n"
             << "Size (" << size << ") must be equal to number of multilayer_perceptron_pointer parameters (" << network_parameters_number << ").\n";

      throw std::logic_error(buffer.str().c_str());
   }

   #endif

   NeuralNetwork neural_network_copy(*neural_network_pointer);

   neural_network_copy.set_parameters(network_parameters);

   NormalizedSquaredError normalized_squared_error_copy(*this);

   normalized_squared_error_copy.set_neural_network_pointer(&neural_network_copy);

   return(normalized_squared_error_copy.calculate_evaluation_terms());
}


// Matrix<double> calculate_Jacobian_terms(void) const method

/// This method returns the Jacobian_terms matrix of the sum squared error function, whose elements are given by the 
/// derivatives of the squared errors data set with respect to the multilayer_perceptron_pointer parameters.
/// The Jacobian_terms matrix here is computed using a back-propagation algorithm.

Matrix<double> NormalizedSquaredError::calculate_Jacobian_terms(void) const
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

   Vector< Vector< Vector<double> > > first_order_forward_propagation(2);

   Vector< Matrix<double> > layers_combination_parameters_Jacobian; 

   Vector< Vector<double> > layers_inputs(layers_number); 

   Vector<double> particular_solution;
   Vector<double> homogeneous_solution;

   const bool conditions_layer_flag = neural_network_pointer->get_conditions_layer_flag();

   const ConditionsLayer* conditions_layer_pointer = neural_network_pointer->get_conditions_layer_pointer();

   // Data set stuff

   // Normalized squared error

   const InstancesInformation& instances_information = data_set_pointer->get_instances_information();

   const unsigned int training_instances_number = instances_information.count_training_instances_number();

   const Vector<double> training_target_data_mean = data_set_pointer->calculate_training_target_data_mean();

   Vector<double> inputs(inputs_number);
   Vector<double> targets(outputs_number);

   // Normalized squared error

   Vector<double> term(outputs_number);
   double term_norm;

   Vector<double> output_objective_gradient(outputs_number);

   Vector< Vector<double> > layers_delta(layers_number);
   Vector<double> point_gradient(parameters_number);

   Matrix<double> Jacobian_terms(training_instances_number, parameters_number);

   double normalization_coefficient = 0.0;

   // Main loop

   for(unsigned int i = 0; i < training_instances_number; i++)
   {
      // Data set

      inputs = data_set_pointer->get_training_input_instance(i);

	  targets = data_set_pointer->get_training_target_instance(i);

	  // Neural network

      first_order_forward_propagation = multilayer_perceptron_pointer->calculate_first_order_forward_propagation(inputs);

      const Vector< Vector<double> >& layers_activation = first_order_forward_propagation[0];
      const Vector< Vector<double> >& layers_activation_derivative = first_order_forward_propagation[1];

      layers_inputs = multilayer_perceptron_pointer->arrange_layers_input(inputs, layers_activation);

	  layers_combination_parameters_Jacobian = multilayer_perceptron_pointer->calculate_layers_combination_parameters_Jacobian(layers_inputs);
	  
	  // Performance functional

      if(!conditions_layer_flag) // No boundary conditions
      {
         const Vector<double>& outputs = layers_activation[layers_number-1]; 

         term = outputs-targets;
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
      else // Conditions
      {        

         particular_solution = conditions_layer_pointer->calculate_particular_solution(inputs);
         homogeneous_solution = conditions_layer_pointer->calculate_homogeneous_solution(inputs);

         const Vector<double>& output_layer_activation = layers_activation[layers_number-1]; 

         term = (particular_solution+homogeneous_solution*output_layer_activation - targets);              
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

	  normalization_coefficient += targets.calculate_sum_squared_error(training_target_data_mean);

      point_gradient = calculate_point_gradient(layers_combination_parameters_Jacobian, layers_delta);

      Jacobian_terms.set_row(i, point_gradient);
  }

   return(Jacobian_terms/sqrt(normalization_coefficient));
}


// FirstOrderEvaluationTerms calculate_first_order_evaluation_terms(void) method

/// This method returns a first order objective terms evaluation structure, which contains the values and the Jacobian of the objective terms function. 
/// @todo

NormalizedSquaredError::FirstOrderEvaluationTerms NormalizedSquaredError::calculate_first_order_evaluation_terms(void)
{
   FirstOrderEvaluationTerms first_order_evaluation_terms;

   first_order_evaluation_terms.evaluation_terms = calculate_evaluation_terms();

   first_order_evaluation_terms.Jacobian_terms = calculate_Jacobian_terms();

   return(first_order_evaluation_terms);
}


// Vector<double> calculate_squared_errors(void) const method

/// This method returns the squared errors of the training instances. 

Vector<double> NormalizedSquaredError::calculate_squared_errors(void) const
{
   // Control sentence (if debug)

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

   // Calculate

   Vector<double> squared_errors(training_instances_number);

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

      squared_errors[i] = outputs.calculate_sum_squared_error(targets);
   }

   return(squared_errors);
}


// std::string write_performance_term_type(void) const method

/// This method returns a string with the name of the normalized squared error performance type, "NORMALIZED_SQUARED_ERROR".

std::string NormalizedSquaredError::write_performance_term_type(void) const
{
   return("NORMALIZED_SQUARED_ERROR");
}


// TiXmlElement* to_XML(void) const method 

/// This method serializes the normalized squared error object into a XML element of the TinyXML library. 
/// See the OpenNN manual for more information about the format of this element. 

TiXmlElement* NormalizedSquaredError::to_XML(void) const
{
   std::ostringstream buffer;

   // Normalized squared error

   TiXmlElement* normalized_squared_error_element = new TiXmlElement("NormalizedSquaredError");
   normalized_squared_error_element->SetAttribute("Version", 4);

   // Display

   {
      TiXmlElement* display_element = new TiXmlElement("Display");
      normalized_squared_error_element->LinkEndChild(display_element);

      buffer.str("");
      buffer << display;

      TiXmlText* display_text = new TiXmlText(buffer.str().c_str());
      display_element->LinkEndChild(display_text);
   }

   return(normalized_squared_error_element);
}


// void from_XML(TiXmlElement*) method

/// This method loads a root mean squared error object from a XML element. 
/// @param normalized_squared_error_element Pointer to a TinyXML element with the object data. 

void NormalizedSquaredError::from_XML(TiXmlElement* normalized_squared_error_element)
{
   if(normalized_squared_error_element)
   { 
      // Display
      {
         TiXmlElement* display_element = normalized_squared_error_element->FirstChildElement("Display");

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
