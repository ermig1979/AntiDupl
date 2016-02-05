/****************************************************************************************************************/
/*                                                                                                              */
/*   OpenNN: Open Neural Networks Library                                                                       */
/*   www.opennn.cimne.com                                                                                       */
/*                                                                                                              */
/*   C R O S S   E N T R O P Y   E R R O R   C L A S S                                                          */
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
#include <math.h>

// OpenNN includes

#include "cross_entropy_error.h"

// TinyXml includes

#include "../../tinyxml/tinyxml.h"


namespace OpenNN
{

// DEFAULT CONSTRUCTOR

/// Default constructor. 
/// It creates a default cross entropy error performance term object, 
/// which is not associated to any neural network and not measured on any data set.
/// It also initializes all the rest of class members to their default values.

CrossEntropyError::CrossEntropyError(void) : PerformanceTerm()
{
}


// NEURAL NETWORK CONSTRUCTOR

/// Neural network constructor. 
/// It creates a cross entropy error performance term associated to a neural network but not measured on any data set.
/// It also initializes all the rest of class members to their default values.
/// @param new_neural_network_pointer Pointer to a neural network object.

CrossEntropyError::CrossEntropyError(NeuralNetwork* new_neural_network_pointer)
 : PerformanceTerm(new_neural_network_pointer)
{
}


// DATA SET CONSTRUCTOR

/// Data set constructor. 
/// It creates a cross entropy error not associated to any neural network but to be measured on a data set object.
/// It also initializes all the rest of class members to their default values.
/// @param new_data_set_pointer Pointer to a data set object.

CrossEntropyError::CrossEntropyError(DataSet* new_data_set_pointer) 
: PerformanceTerm(new_data_set_pointer)
{
}


// NEURAL NETWORK AND DATA SET CONSTRUCTOR

/// Neural network and data set constructor. 
/// It creates a cross entropy error performance term object associated to a neural network and measured on a data set.
/// It also initializes all the rest of class members to their default values:
/// @param new_neural_network_pointer: Pointer to a neural network object.
/// @param new_data_set_pointer: Pointer to a data set object.

CrossEntropyError::CrossEntropyError(NeuralNetwork* new_neural_network_pointer, DataSet* new_data_set_pointer)
: PerformanceTerm(new_neural_network_pointer, new_data_set_pointer)
{
}


// XML CONSTRUCTOR

/// XML constructor. 
/// It creates a cross entropy error not associated to any neural network and not measured on any data set.
/// It also sets all the rest of class members from a TinyXML element.
/// @param sum_squared_error_element XML element with the class members. 

CrossEntropyError::CrossEntropyError(TiXmlElement* sum_squared_error_element) 
 : PerformanceTerm(sum_squared_error_element)
{
}



// COPY CONSTRUCTOR

/// Copy constructor. 
/// It creates a cross entropy error not associated to any neural network and not measured on any data set.
/// It also sets all the rest of class members from another sum squared error object.
/// @param new_cross_entropy_error Object to be copied. 

CrossEntropyError::CrossEntropyError(const CrossEntropyError& new_cross_entropy_error)
 : PerformanceTerm(new_cross_entropy_error)
{

}


// DESTRUCTOR

/// Destructor.

CrossEntropyError::~CrossEntropyError(void) 
{
}


// ASSIGNMENT OPERATOR

/// Assignment operator. 
/// @param other_cross_entropy_error Object to be copied. 

CrossEntropyError& CrossEntropyError::operator = (const CrossEntropyError& other_cross_entropy_error)
{
   if(this != &other_cross_entropy_error) 
   {
      *neural_network_pointer = *other_cross_entropy_error.neural_network_pointer;
      *data_set_pointer = *other_cross_entropy_error.data_set_pointer;
      display = other_cross_entropy_error.display;
   }

   return(*this);

}

// EQUAL TO OPERATOR

/// Equal to operator. 
/// If compares this object with another object of the same class, and returns true if they are equal, and false otherwise. 
/// @param other_cross_entropy_error Object to be compared with. 

bool CrossEntropyError::operator == (const CrossEntropyError& other_cross_entropy_error) const
{
   if(*neural_network_pointer == *other_cross_entropy_error.neural_network_pointer
   && *mathematical_model_pointer == *other_cross_entropy_error.mathematical_model_pointer
   && display == other_cross_entropy_error.display)    
   {
      return(true);
   }
   else
   {
      return(false);  
   }

}


// METHODS


// void check(void) const method

/// This method checks that there are a neural network and a data set associated to the cross entropy error, 
/// and that the numbers of inputs and outputs in the neural network are equal to the numbers of inputs and targets in the data set. 

void CrossEntropyError::check(void) const
{
   std::ostringstream buffer;

   // Neural network stuff

   if(!neural_network_pointer)
   {
      buffer << "OpenNN Exception: CrossEntropyError class.\n"
             << "void check(void) const method.\n"
             << "Pointer to neural network is NULL.\n";

      throw std::logic_error(buffer.str().c_str());	  
   }

   const MultilayerPerceptron* multilayer_perceptron_pointer = neural_network_pointer->get_multilayer_perceptron_pointer();

   if(!multilayer_perceptron_pointer)
   {
      buffer << "OpenNN Exception: CrossEntropyError class.\n"
             << "void check(void) const method.\n"
             << "Pointer to multilayer perceptron is NULL.\n";

      throw std::logic_error(buffer.str().c_str());	  
   }

   const unsigned int inputs_number = multilayer_perceptron_pointer->count_inputs_number();
   const unsigned int outputs_number = multilayer_perceptron_pointer->count_outputs_number();

   if(inputs_number == 0)
   {
      buffer << "OpenNN Exception: CrossEntropyError class.\n"
             << "void check(void) const method.\n"
             << "Number of inputs in multilayer perceptron object is zero.\n";

      throw std::logic_error(buffer.str().c_str());	  
   }

   if(outputs_number == 0)
   {
      buffer << "OpenNN Exception: CrossEntropyError class.\n"
             << "void check(void) const method.\n"
             << "Number of outputs in multilayer perceptron object is zero.\n";

      throw std::logic_error(buffer.str().c_str());	  
   }

   const bool probabilistic_layer_flag = neural_network_pointer->get_probabilistic_layer_flag();

   if(probabilistic_layer_flag == false)
   {
      buffer << "OpenNN Exception: CrossEntropyError class.\n"
             << "double calculate_evaluation(void) const method.\n"
             << "Probabilistc prostprocessing flag must be true.\n";

      throw std::logic_error(buffer.str().c_str());
   }

   const ProbabilisticLayer* probabilistic_layer_pointer = neural_network_pointer->get_probabilistic_layer_pointer();

   if(!probabilistic_layer_pointer)
   {
      buffer << "OpenNN Exception: CrossEntropyError class.\n"
             << "void check(void) const method.\n"
             << "Pointer to probabilistic layer is NULL.\n";

      throw std::logic_error(buffer.str().c_str());	  
   }

   const ProbabilisticLayer::ProbabilisticMethod& outputs_probabilizing_method = probabilistic_layer_pointer->get_probabilistic_method();

   if(outputs_probabilizing_method != ProbabilisticLayer::Softmax)
   {
      buffer << "OpenNN Exception: CrossEntropyError class.\n"
             << "void check(void) const method.\n"
             << "Probabilistic method is not Softmax.\n";

      throw std::logic_error(buffer.str().c_str());
   }

   // Data set stuff

   if(!data_set_pointer)
   {
      buffer << "OpenNN Exception: CrossEntropyError class.\n"
             << "void check(void) const method.\n"
             << "Pointer to data set is NULL.\n";

      throw std::logic_error(buffer.str().c_str());	  
   }

   // Sum squared error stuff

   const VariablesInformation& variables_information = data_set_pointer->get_variables_information();

   const unsigned int targets_number = variables_information.count_targets_number();

   if(inputs_number != inputs_number)
   {
      buffer << "OpenNN Exception: CrossEntropyError class.\n"
             << "void check(void) const method.\n"
             << "Number of inputs in multilayer perceptron must be equal to number of inputs in data set.\n";

      throw std::logic_error(buffer.str().c_str());	  
   }

   if(outputs_number != targets_number)
   {
      buffer << "OpenNN Exception: CrossEntropyError class.\n"
             << "void check(void) const method.\n"
             << "Number of outputs in multilayer perceptron must be equal to number of targets in data set.\n";

      throw std::logic_error(buffer.str().c_str());
   }
}


// double calculate_evaluation(void) method

/// @todo

double CrossEntropyError::calculate_evaluation(void) const
{
   std::ostringstream buffer;

   buffer << "OpenNN Exception: CrossEntropyError class.\n"
          << "double calculate_evaluation(void) method.\n"
          << "This method is under development.\n";

   throw std::logic_error(buffer.str().c_str());

/*
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

   // Cross entropy error

   Vector<double> inputs(inputs_number);
   Vector<double> outputs(outputs_number);
   Vector<double> targets(outputs_number);

   double objective = 0.0;

   for(unsigned int i = 0; i < training_instances_number; i++)
   {
      // Input vector

	  inputs = data_set_pointer->get_training_input_instance(i);

      // Output vector

      outputs = multilayer_perceptron_pointer->calculate_outputs(inputs);

      // Target vector

      targets = data_set_pointer->get_training_target_instance(i);

      // Cross entropy error

      for(unsigned int j = 0; j < outputs_number; j++)
      {
         objective -= targets[j]*log(outputs[j]) + (1.0 - targets[j])*log(1.0 - outputs[j]);
      }
   }

   return(objective);
*/
}


// double calculate_evaluation(const Vector<double>&) const method

/// This method returns which would be the objective of a neural for a given vector of parameters. 
/// It does not set that vector of parameters to the neural network. 
// @param parameters Vector of a potential parameters for the neural network associated to this object.

double CrossEntropyError::calculate_evaluation(const Vector<double>&) const
{
   std::ostringstream buffer;

   buffer << "OpenNN Exception: CrossEntropyError class.\n"
          << "double calculate_evaluation() method.\n"
          << "This method is under development.\n";

   throw std::logic_error(buffer.str().c_str());
/*
   #ifdef _DEBUG 

   check();

   #endif

   #ifdef _DEBUG 

   std::ostringstream buffer;

   const unsigned int size = parameters.size();

   const unsigned int parameters_number = neural_network_pointer->count_parameters_number();

   if(size != parameters_number)
   {
      buffer << "OpenNN Exception: CrossEntropyError class.\n"
             << "double calculate_evaluation(const Vector<double>&) const method.\n"
             << "Size (" << size << ") must be equal to number of parameters (" << parameters_number << ").\n";

      throw std::logic_error(buffer.str().c_str());
   }

   #endif

   NeuralNetwork neural_network_copy(*neural_network_pointer);

   neural_network_copy.set_parameters(parameters);

   CrossEntropyError cross_entropy_error_copy(*this);

   cross_entropy_error_copy.set_neural_network_pointer(&neural_network_copy);
   cross_entropy_error_copy.set_data_set_pointer(data_set_pointer);

   return(cross_entropy_error_copy.calculate_evaluation());
*/
}


// double calculate_minimum_objective(void) method

/// This method returns the minimum achieveable cross entropy for the training data. 
/// @todo 

double CrossEntropyError::calculate_minimum_objective(void)
{
   return(0.0);
}


// double calculate_generalization_evaluation(void) const method

/// This method returns the cross entropy error of the neural network measured on the generalization instances of the data set.
/// @todo

double CrossEntropyError::calculate_generalization_evaluation(void) const
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

   const unsigned int generalization_instances_number = instances_information.count_generalization_instances_number();

   Vector<double> inputs(inputs_number);
   Vector<double> outputs(outputs_number);
   Vector<double> targets(outputs_number);

   double generalization_evaluation = 0.0;

   for(unsigned int i = 0; i < generalization_instances_number; i++)
   {
      // Input vector

	  inputs = data_set_pointer->get_generalization_input_instance(i);

      // Output vector

      outputs = multilayer_perceptron_pointer->calculate_outputs(inputs);

      // Target vector

      targets = data_set_pointer->get_generalization_target_instance(i);

      // Cross entropy error

      for(unsigned int j = 0; j < outputs_number; j++)
      {
         generalization_evaluation -= targets[j]*log(outputs[j]) + (1.0 - targets[j])*log(1.0 - outputs[j]);
      }
   }

   return(generalization_evaluation);
}


// double calculate_minimum_generalization_objective(void) method

/// This method returns the minimum achieveable cross entropy for the generalization data. 
/// @todo

double CrossEntropyError::calculate_minimum_generalization_objective(void)
{
   return(0.0);
}


// Vector<double> calculate_gradient(void) const

/// This method returns the cross entropy error gradient of a neural network on a data set. 
/// It uses the error back-propagation method.
/// @todo

Vector<double> CrossEntropyError::calculate_gradient(void) const
{
   #ifdef _DEBUG 

   check();

   #endif

   // Neural network stuff

   const MultilayerPerceptron* multilayer_perceptron_pointer = neural_network_pointer->get_multilayer_perceptron_pointer();

   const unsigned int inputs_number = multilayer_perceptron_pointer->count_inputs_number();
   const unsigned int outputs_number = multilayer_perceptron_pointer->count_outputs_number();

   const unsigned int layers_number = multilayer_perceptron_pointer->count_layers_number();

   const unsigned int network_parameters_number = multilayer_perceptron_pointer->count_parameters_number();

   const ConditionsLayer* conditions_layer_pointer = neural_network_pointer->get_conditions_layer_pointer();

   const bool& conditions_layer_flag = neural_network_pointer->get_conditions_layer_flag();

   #ifdef _DEBUG 

   std::ostringstream buffer;

   const Matrix<double> target_data = data_set_pointer->arrange_target_data();

   if(target_data < 0.0)
   {
      buffer << "OpenNN Exception: CrossEntropyError class.\n"
             << "Vector<double> calculate_gradient(void) const method.\n"
             << "Target data must be equal or greater than zero.\n";

      throw std::logic_error(buffer.str().c_str());
   }

   if(target_data > 1.0)
   {
      buffer << "OpenNN Exception: CrossEntropyError class.\n"
             << "Vector<double> calculate_gradient(void) const method.\n"
             << "Target data must be less or equal or than one.\n";

      throw std::logic_error(buffer.str().c_str());
   }

   #endif

   // Neural network stuff    

   Vector< Vector< Vector<double> > > first_order_forward_propagation(2); 

   Vector<double> particular_solution;
   Vector<double> homogeneous_solution;

   // Data set stuff

   const InstancesInformation& instances_information = data_set_pointer->get_instances_information();

   const unsigned int training_instances_number = instances_information.count_training_instances_number();

   Vector<double> inputs(inputs_number);
   Vector<double> targets(outputs_number);

   // Sum squared error stuff

   Vector<double> output_objective_gradient(outputs_number);

   Vector< Vector<double> > layers_delta; 

   Vector<double> point_gradient(network_parameters_number, 0.0);

   Vector<double> gradient(network_parameters_number, 0.0);


   for(unsigned int i = 0; i < training_instances_number; i++)
   {
      inputs = data_set_pointer->get_training_input_instance(i);

      targets = data_set_pointer->get_training_target_instance(i);

      first_order_forward_propagation = multilayer_perceptron_pointer->calculate_first_order_forward_propagation(inputs);

      const Vector< Vector<double> >& layers_activation = first_order_forward_propagation[0];
      const Vector< Vector<double> >& layers_activation_derivative = first_order_forward_propagation[1];

      if(!conditions_layer_flag)
      {
         const Vector<double>& outputs = layers_activation[layers_number-1];

         for(unsigned int j = 0; j < outputs_number; j++)
         {
            output_objective_gradient[j] = -targets[j]/outputs[j] + (1.0 - targets[j])*(1.0 - outputs[j]);
         }

         layers_delta = calculate_layers_delta(layers_activation_derivative, output_objective_gradient);
      }
      else
      {
         particular_solution = conditions_layer_pointer->calculate_particular_solution(inputs);
         homogeneous_solution = conditions_layer_pointer->calculate_homogeneous_solution(inputs);

         Vector<double> outputs = particular_solution + homogeneous_solution*layers_activation[layers_number-1];

         for(unsigned int j = 0; j < outputs_number; j++)
         {
            output_objective_gradient[j] = -targets[j]/outputs[j] + (1.0 - targets[j])*(1.0 - outputs[j]);
         }

		 layers_delta = calculate_layers_delta(layers_activation_derivative, homogeneous_solution, output_objective_gradient);
      }

      point_gradient = calculate_point_gradient(inputs, layers_activation, layers_delta);

      gradient += point_gradient;
   }

   return(gradient);
}


// Matrix<double> calculate_Hessian(void) const

/// @todo

Matrix<double> CrossEntropyError::calculate_Hessian(void) const
{
   Matrix<double> objective_Hessian;

   return(objective_Hessian);
}


// std::string write_performance_term_type(void) const method

/// This method returns a string with the name of the cross entropy error performance type, "CROSS_ENTROPY_ERROR".

std::string CrossEntropyError::write_performance_term_type(void) const
{
   return("CROSS_ENTROPY_ERROR");
}


// TiXmlElement* to_XML(void) const method 

/// This method serializes the cross entropy error object into a XML element of the TinyXML library. 
/// See the OpenNN manual for more information about the format of this element. 

TiXmlElement* CrossEntropyError::to_XML(void) const
{
   std::ostringstream buffer;

   // Cross entropy error 

   TiXmlElement* cross_entropy_error_element = new TiXmlElement("CrossEntropyError");
   cross_entropy_error_element->SetAttribute("Version", 4);

   // Display

   {
      TiXmlElement* display_element = new TiXmlElement("Display");
      cross_entropy_error_element->LinkEndChild(display_element);

      buffer.str("");
      buffer << display;

      TiXmlText* display_text = new TiXmlText(buffer.str().c_str());
      display_element->LinkEndChild(display_text);
   }

   return(cross_entropy_error_element);
}


// void from_XML(TiXmlElement*) method

/// This method deserializes a TinyXML element into this cross entropy object. 
/// @param cross_entropy_error_element Pointer to a XML element containing the member data. 

void CrossEntropyError::from_XML(TiXmlElement* cross_entropy_error_element)  
{
   if(cross_entropy_error_element)
   { 
      // Display
      {
         TiXmlElement* display_element = cross_entropy_error_element->FirstChildElement("Display");

         if(display_element)
         {
            std::string new_display_string = display_element->GetText();     

            try
            {
               set_display(new_display_string != "0");
            }
            catch(std::logic_error& e)
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
