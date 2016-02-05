/****************************************************************************************************************/
/*                                                                                                              */
/*   OpenNN: Open Neural Networks Library                                                                       */
/*   www.opennn.cimne.com                                                                                       */
/*                                                                                                              */
/*   S U M   S Q U A R E D   E R R O R   C L A S S                                                              */
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
#include <cmath>
#include <sstream>
#include <string>
#include <limits>

// OpenNN includes

#include "sum_squared_error.h"

// TinyXml includes

#include "../../tinyxml/tinyxml.h"


namespace OpenNN
{

// DEFAULT CONSTRUCTOR

/// Default constructor. 
/// It creates a sum squared error performance term not associated to any neural network and not measured on any data set.
/// It also initializes all the rest of class members to their default values.

SumSquaredError::SumSquaredError(void) : PerformanceTerm()
{
}


// NEURAL NETWORK CONSTRUCTOR

/// Neural network constructor. 
/// It creates a sum squared error performance term associated to a neural network but not measured on any data set.
/// It also initializes all the rest of class members to their default values.
/// @param new_neural_network_pointer Pointer to a neural network object.

SumSquaredError::SumSquaredError(NeuralNetwork* new_neural_network_pointer) 
: PerformanceTerm(new_neural_network_pointer)
{
}


// DATA SET CONSTRUCTOR

/// Data set constructor. 
/// It creates a sum squared error not associated to any neural network but to be measured on a data set object.
/// It also initializes all the rest of class members to their default values.
/// @param new_data_set_pointer Pointer to a data set object.

SumSquaredError::SumSquaredError(DataSet* new_data_set_pointer)
: PerformanceTerm(new_data_set_pointer)
{
}


// NEURAL NETWORK AND DATA SET CONSTRUCTOR

/// Neural network and data set constructor. 
/// It creates a sum squared error associated to a neural network and measured on a data set.
/// It also initializes all the rest of class members to their default values.
/// @param new_neural_network_pointer Pointer to a neural network object.
/// @param new_data_set_pointer Pointer to a data set object.

SumSquaredError::SumSquaredError(NeuralNetwork* new_neural_network_pointer, DataSet* new_data_set_pointer)
 : PerformanceTerm(new_neural_network_pointer, new_data_set_pointer)
{
}


// XML CONSTRUCTOR

/// XML constructor. 
/// It creates a sum squared error not associated to any neural network and not measured on any data set.
/// It also sets all the rest of class members from a TinyXML element.
/// @param sum_squared_error_element XML element with the class members. 

SumSquaredError::SumSquaredError(TiXmlElement* sum_squared_error_element) 
 : PerformanceTerm(sum_squared_error_element)
{
}



// COPY CONSTRUCTOR

/// Copy constructor. 
/// It creates a sum squared error not associated to any neural network and not measured on any data set.
/// It also sets all the rest of class members from another sum squared error object.
/// @param new_sum_squared_error Object to be copied. 

SumSquaredError::SumSquaredError(const SumSquaredError& new_sum_squared_error)
 : PerformanceTerm(new_sum_squared_error)
{

}


// DESTRUCTOR

/// Destructor.

SumSquaredError::~SumSquaredError(void) 
{
}


// METHODS

// void check(void) const method

/// This method checks that there are a neural network and a data set associated to the sum squared error, 
/// and that the numbers of inputs and outputs in the neural network are equal to the numbers of inputs and targets in the data set. 
/// If some of the above conditions is not hold, the method throws an exception. 

void SumSquaredError::check(void) const
{
   std::ostringstream buffer;

   // Neural network stuff

   if(!neural_network_pointer)
   {
      buffer << "OpenNN Exception: SumSquaredError class.\n"
             << "void check(void) const method.\n"
             << "Pointer to neural network is NULL.\n";

      throw std::logic_error(buffer.str().c_str());	  
   }

   const MultilayerPerceptron* multilayer_perceptron_pointer = neural_network_pointer->get_multilayer_perceptron_pointer();

   if(!multilayer_perceptron_pointer)
   {
      buffer << "OpenNN Exception: SumSquaredError class.\n"
             << "void check(void) const method.\n"
             << "Pointer to multilayer perceptron is NULL.\n";

      throw std::logic_error(buffer.str().c_str());	  
   }

   const unsigned int inputs_number = multilayer_perceptron_pointer->count_inputs_number();
   const unsigned int outputs_number = multilayer_perceptron_pointer->count_outputs_number();

   // Data set stuff

   if(!data_set_pointer)
   {
      buffer << "OpenNN Exception: SumSquaredError class.\n"
             << "void check(void) const method.\n"
             << "Pointer to data set is NULL.\n";

      throw std::logic_error(buffer.str().c_str());	  
   }

   // Sum squared error stuff

   const VariablesInformation& variables_information = data_set_pointer->get_variables_information();

   const unsigned int targets_number = variables_information.count_targets_number();

   if(inputs_number != inputs_number)
   {
      buffer << "OpenNN Exception: SumSquaredError class.\n"
             << "void check(void) const method.\n"
             << "Number of inputs in multilayer perceptron must be equal to number of inputs in data set.\n";

      throw std::logic_error(buffer.str().c_str());	  
   }

   if(outputs_number != targets_number)
   {
      buffer << "OpenNN Exception: SumSquaredError class.\n"
             << "void check(void) const method.\n"
             << "Number of outputs in multilayer perceptron must be equal to number of targets in data set.\n";

      throw std::logic_error(buffer.str().c_str());
   }
}


// double calculate_evaluation(void) const method

/// This method returns the evaluation value of a neural network according to the sum squared error on a data set.

double SumSquaredError::calculate_evaluation(void) const
{
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

   // Sum squared error stuff

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

   return(sum_squared_error);
}


// double calculate_evaluation(const Vector<double>&) const method

/// This method returns which would be the sum squard error evaluation of a neural network for an hypothetical vector of parameters. 
/// It does not set that vector of parameters to the neural network. 
/// @param parameters Vector of a potential parameters for the neural network associated to the performance term.
/// @todo

double SumSquaredError::calculate_evaluation(const Vector<double>& parameters) const
{
   // Neural network stuff

   #ifdef _DEBUG 

   check();

   #endif

   // Sum squared error stuff

   #ifdef _DEBUG 

   std::ostringstream buffer;

   const unsigned int size = parameters.size();

   const unsigned int parameters_number = neural_network_pointer->count_parameters_number();

   if(size != parameters_number)
   {
      buffer << "OpenNN Exception: SumSquaredError class." << std::endl
             << "double calculate_evaluation(const Vector<double>&) const method." << std::endl
             << "Size (" << size << ") must be equal to number of parameters (" << parameters_number << ")." << std::endl;

      throw std::logic_error(buffer.str().c_str());
   }

   #endif

   NeuralNetwork neural_network_copy(*neural_network_pointer);

   neural_network_copy.set_parameters(parameters);

   SumSquaredError sum_squared_error_copy(*this);

   sum_squared_error_copy.set_neural_network_pointer(&neural_network_copy);

   return(sum_squared_error_copy.calculate_evaluation());
}


// double calculate_generalization_evaluation(void) const method

/// This method returns the sum squared error of the neural network measured on the generalization instances of the data set.

double SumSquaredError::calculate_generalization_evaluation(void) const
{
   #ifdef _DEBUG 

   check();

   #endif

   // Neural network stuff

   const MultilayerPerceptron* multilayer_perceptron_pointer = neural_network_pointer->get_multilayer_perceptron_pointer();

   const unsigned int inputs_number = multilayer_perceptron_pointer->count_inputs_number();
   const unsigned int outputs_number = multilayer_perceptron_pointer->count_outputs_number();

   // Data set stuff

   const InstancesInformation& instances_information = data_set_pointer->get_instances_information();
   const unsigned int generalization_instances_number = instances_information.count_generalization_instances_number();

   // Sum squared error stuff

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

   return(generalization_objective);
}


// Vector<double> calculate_gradient(void) const method

/// This method calculates the performance term gradient by means of the back-propagation algorithm, 
/// and returns it in a single vector of size the number of neural network parameters. 

Vector<double> SumSquaredError::calculate_gradient(void) const
{
   #ifdef _DEBUG 

   check();

   #endif

   // Neural network stuff

   const MultilayerPerceptron* multilayer_perceptron_pointer = neural_network_pointer->get_multilayer_perceptron_pointer();

   // Neural network stuff

   const ConditionsLayer* conditions_layer_pointer = neural_network_pointer->get_conditions_layer_pointer();

   const unsigned int inputs_number = multilayer_perceptron_pointer->count_inputs_number();
   const unsigned int outputs_number = multilayer_perceptron_pointer->count_outputs_number();

   const unsigned int layers_number = neural_network_pointer->get_multilayer_perceptron_pointer()->count_layers_number();

   const unsigned int network_parameters_number = multilayer_perceptron_pointer->count_parameters_number();

   Vector< Vector< Vector<double> > > first_order_forward_propagation(2); 

   const bool& conditions_layer_flag = neural_network_pointer->get_conditions_layer_flag();

   Vector<double> particular_solution;
   Vector<double> homogeneous_solution;

   // Data set stuff

   const InstancesInformation& instances_information = data_set_pointer->get_instances_information();

   unsigned int training_instances_number = instances_information.count_training_instances_number();

   Vector<double> inputs(inputs_number);
   Vector<double> targets(outputs_number);

   // Sum squared error stuff

   Vector<double> output_objective_gradient(outputs_number);

   Vector< Matrix<double> > layers_combination_parameters_Jacobian; 

   Vector< Vector<double> > layers_inputs(layers_number); 
   Vector< Vector<double> > layers_delta; 

   Vector<double> point_gradient(network_parameters_number, 0.0);

   Vector<double> objective_gradient(network_parameters_number, 0.0);

   for(unsigned int i = 0; i < training_instances_number; i++)
   {
      inputs = data_set_pointer->get_training_input_instance(i);

      targets = data_set_pointer->get_training_target_instance(i);

      first_order_forward_propagation = neural_network_pointer->get_multilayer_perceptron_pointer()->calculate_first_order_forward_propagation(inputs);

      const Vector< Vector<double> >& layers_activation = first_order_forward_propagation[0];
      const Vector< Vector<double> >& layers_activation_derivative = first_order_forward_propagation[1];

	  layers_inputs[0] = inputs;

	  for(unsigned int j = 1; j < layers_number; j++)
	  {
	     layers_inputs[j] = layers_activation[j-1];
	  }

	  layers_combination_parameters_Jacobian = neural_network_pointer->get_multilayer_perceptron_pointer()->calculate_layers_combination_parameters_Jacobian(layers_inputs);

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

      point_gradient = calculate_point_gradient(layers_combination_parameters_Jacobian, layers_delta);

      objective_gradient += point_gradient;
   }


   return(objective_gradient);
}


// Matrix<double> calculate_Hessian(void) const method

/// This method calculates the objective Hessian by means of the back-propagation algorithm, 
/// and returns it in a single symmetric matrix of size the number of neural network parameters. 

Matrix<double> SumSquaredError::calculate_Hessian(void) const
{
   #ifdef _DEBUG 

   check();

   #endif

   // Neural network stuff

   const MultilayerPerceptron* multilayer_perceptron_pointer = neural_network_pointer->get_multilayer_perceptron_pointer();

   const ConditionsLayer* conditions_layer_pointer = neural_network_pointer->get_conditions_layer_pointer();

   const unsigned int inputs_number = multilayer_perceptron_pointer->count_inputs_number();
   const unsigned int outputs_number = multilayer_perceptron_pointer->count_outputs_number();

   const unsigned int layers_number = multilayer_perceptron_pointer->count_layers_number();

   const unsigned int parameters_number = multilayer_perceptron_pointer->count_parameters_number();

   const Vector<unsigned int> layers_perceptrons_number = multilayer_perceptron_pointer->arrange_layers_perceptrons_numbers();

   const unsigned int conditions_layer_flag = neural_network_pointer->get_conditions_layer_flag();

   Vector< Vector< Vector<double> > > second_order_forward_propagation(3); 

   Vector < Vector< Vector<double> > > perceptrons_combination_parameters_gradient(layers_number);
   Matrix < Matrix<double> > interlayers_combination_combination_Jacobian;

   Vector<double> particular_solution;
   Vector<double> homogeneous_solution;

   // Data set stuff

   const InstancesInformation& instances_information = data_set_pointer->get_instances_information();

   const unsigned int training_instances_number = instances_information.count_training_instances_number();

   Vector<double> inputs(inputs_number);
   Vector<double> targets(outputs_number);

   // Sum squared error stuff

   Vector< Vector<double> > layers_delta(layers_number);
   Matrix< Matrix<double> > interlayers_Delta(layers_number, layers_number);

   Vector<double> output_objective_gradient(outputs_number);
   Matrix<double> output_objective_Hessian(outputs_number, outputs_number);

   Matrix<double> objective_Hessian(parameters_number, parameters_number, 0.0);

   for(unsigned int i = 0; i < training_instances_number; i++)
   {
      inputs = data_set_pointer->get_training_input_instance(i);

      targets = data_set_pointer->get_training_target_instance(i);

      second_order_forward_propagation = multilayer_perceptron_pointer->calculate_second_order_forward_propagation(inputs);
	  
	  Vector< Vector<double> >& layers_activation = second_order_forward_propagation[0];
	  Vector< Vector<double> >& layers_activation_derivative = second_order_forward_propagation[1];
	  Vector< Vector<double> >& layers_activation_second_derivative = second_order_forward_propagation[2];

	  Vector< Vector<double> > layers_inputs(layers_number);

	  layers_inputs[0] = inputs;

	  for(unsigned int j = 1; j < layers_number; j++)
	  {
	     layers_inputs[j] = layers_activation[j-1];
	  }

	  perceptrons_combination_parameters_gradient = multilayer_perceptron_pointer->calculate_perceptrons_combination_parameters_gradient(layers_inputs);

      interlayers_combination_combination_Jacobian = multilayer_perceptron_pointer->calculate_interlayers_combination_combination_Jacobian(inputs);

      if(!conditions_layer_flag)
      {
         output_objective_gradient = (layers_activation[layers_number-1] - targets)*2.0;
		 output_objective_Hessian.initialize_diagonal(2.0);

		 layers_delta = calculate_layers_delta(layers_activation_derivative, output_objective_gradient);
         interlayers_Delta = calculate_interlayers_Delta(layers_activation_derivative, layers_activation_second_derivative, interlayers_combination_combination_Jacobian, output_objective_gradient, output_objective_Hessian, layers_delta);
      }
      else
      {
         particular_solution = conditions_layer_pointer->calculate_particular_solution(inputs);
         homogeneous_solution = conditions_layer_pointer->calculate_homogeneous_solution(inputs);

         output_objective_gradient = (particular_solution+homogeneous_solution*layers_activation[layers_number-1] - targets)*2.0;              

		 layers_delta = calculate_layers_delta(layers_activation_derivative, homogeneous_solution, output_objective_gradient);
      }

	  objective_Hessian += calculate_point_Hessian(layers_activation_derivative, perceptrons_combination_parameters_gradient, interlayers_combination_combination_Jacobian, layers_delta, interlayers_Delta);
   }

   return(objective_Hessian);   
}


// Vector<double> calculate_evaluation_terms(void) const method

/// This method calculates the squared error terms for each instance, and returns it in a vector of size the number training instances. 

Vector<double> SumSquaredError::calculate_evaluation_terms(void) const
{
   // Control sentence (if debug)

   #ifdef _DEBUG 

   check();

   #endif

   const MultilayerPerceptron* multilayer_perceptron_pointer = neural_network_pointer->get_multilayer_perceptron_pointer();

   const unsigned int inputs_number = multilayer_perceptron_pointer->count_inputs_number();
   const unsigned int outputs_number = multilayer_perceptron_pointer->count_outputs_number();

   const InstancesInformation& instances_information = data_set_pointer->get_instances_information();

   const unsigned int training_instances_number = instances_information.count_training_instances_number();

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

   return(evaluation_terms);
}


// Vector<double> calculate_evaluation_terms(const Vector<double>&) const method

/// This method returns the evaluation terms vector for a hypotetical vector of parameters. 
/// @param parameters Neural network parameters for which the evaluation terms vector is to be computed. 

Vector<double> SumSquaredError::calculate_evaluation_terms(const Vector<double>& parameters) const
{
   // Control sentence (if debug)

   #ifdef _DEBUG 
   
   check();

   #endif


   #ifdef _DEBUG 

   const unsigned int size = parameters.size();

   const unsigned int parameters_number = neural_network_pointer->count_parameters_number();

   if(size != parameters_number)
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: SumSquaredError class." << std::endl
             << "double calculate_evaluation_terms(const Vector<double>&) const method." << std::endl
             << "Size (" << size << ") must be equal to number of neural network parameters (" << parameters_number << ")." << std::endl;

      throw std::logic_error(buffer.str().c_str());	  
   }

   #endif

   NeuralNetwork neural_network_copy(*neural_network_pointer);

   neural_network_copy.set_parameters(parameters);

   SumSquaredError sum_squared_error_copy(*this);

   sum_squared_error_copy.set_neural_network_pointer(&neural_network_copy);

   return(sum_squared_error_copy.calculate_evaluation_terms());
}


// Matrix<double> calculate_Jacobian_terms(void) const method

/// This method returns the Jacobian_terms matrix of the sum squared error function, whose elements are given by the 
/// derivatives of the squared errors data set with respect to the multilayer_perceptron_pointer parameters.
/// The Jacobian_terms matrix here is computed using a back-propagation algorithm.

Matrix<double> SumSquaredError::calculate_Jacobian_terms(void) const
{
   #ifdef _DEBUG 

   check();

   #endif 

   const MultilayerPerceptron* multilayer_perceptron_pointer = neural_network_pointer->get_multilayer_perceptron_pointer();

   const unsigned int inputs_number = multilayer_perceptron_pointer->count_inputs_number();
   const unsigned int outputs_number = multilayer_perceptron_pointer->count_outputs_number();
   const unsigned int layers_number = multilayer_perceptron_pointer->count_layers_number();

   const unsigned int network_parameters_number = multilayer_perceptron_pointer->count_parameters_number();

   Vector< Vector< Vector<double> > > first_order_forward_propagation(2);

   Vector< Vector<double> > layers_inputs(layers_number);
   Vector< Matrix<double> > layers_combination_parameters_Jacobian(layers_number);

   Vector<double> particular_solution;
   Vector<double> homogeneous_solution;

   const bool conditions_layer_flag = neural_network_pointer->get_conditions_layer_flag();

   // Data set

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

	  layers_inputs[0] = inputs;

	  for(unsigned int j = 1; j < layers_number; j++)
	  {
	     layers_inputs[j] = layers_activation[j-1];
	  }

	  layers_combination_parameters_Jacobian = multilayer_perceptron_pointer->calculate_layers_combination_parameters_Jacobian(layers_inputs);

      if(!conditions_layer_flag)
      {
         const Vector<double>& outputs = first_order_forward_propagation[0][layers_number-1]; 

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
      else
      {
         ConditionsLayer* conditions_layer_pointer = neural_network_pointer->get_conditions_layer_pointer();

         particular_solution = conditions_layer_pointer->calculate_particular_solution(inputs);
         homogeneous_solution = conditions_layer_pointer->calculate_homogeneous_solution(inputs);

         const Vector<double>& output_layer_activation = first_order_forward_propagation[0][layers_number-1]; 

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

      point_gradient = calculate_point_gradient(layers_combination_parameters_Jacobian, layers_delta);

      Jacobian_terms.set_row(i, point_gradient);
  }

   return(Jacobian_terms);
}


// FirstOrderEvaluationTerms calculate_first_order_evaluation_terms(void) const method

/// This method returns the first order evaluation of the terms objective function.
/// This is a structure containing the objective terms vector and the objective terms Jacobian. 

PerformanceTerm::FirstOrderEvaluationTerms SumSquaredError::calculate_first_order_evaluation_terms(void) const
{
   FirstOrderEvaluationTerms first_order_evaluation_terms;

   first_order_evaluation_terms.evaluation_terms = calculate_evaluation_terms();
   first_order_evaluation_terms.Jacobian_terms = calculate_Jacobian_terms();

   return(first_order_evaluation_terms);
}


// Vector<double> calculate_squared_errors(void) const method

/// This method returns the squared errors of the training instances. 

Vector<double> SumSquaredError::calculate_squared_errors(void) const
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

/// This method returns a string with the name of the sum squared error performance type, "SUM_SQUARED_ERROR".

std::string SumSquaredError::write_performance_term_type(void) const
{
   return("SUM_SQUARED_ERROR");
}


// TiXmlElement* to_XML(void) method method 

/// This method returns a representation of the sum squared error object, in XML format. 

TiXmlElement* SumSquaredError::to_XML(void) const
{
   std::ostringstream buffer;

   // Sum squared error

   TiXmlElement* sum_squared_error_element = new TiXmlElement("SumSquaredError");
   sum_squared_error_element->SetAttribute("Version", 4);

   // Display

   {
      TiXmlElement* display_element = new TiXmlElement("Display");
      sum_squared_error_element->LinkEndChild(display_element);

      buffer.str("");
      buffer << display;

      TiXmlText* display_text = new TiXmlText(buffer.str().c_str());
      display_element->LinkEndChild(display_text);
   }

   return(sum_squared_error_element);
}


// void load(TiXmlElement*) method

/// This method loads a sum squared error object from a XML element. 
// @param sum_squared_error_element Tiny XML element containing the members of a sum squared error element. 

void SumSquaredError::from_XML(TiXmlElement* sum_squared_error_element)
{
   if(sum_squared_error_element)
   { 
      // Display
      {
         TiXmlElement* display_element = sum_squared_error_element->FirstChildElement("Display");

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
