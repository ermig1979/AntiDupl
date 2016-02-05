/****************************************************************************************************************/
/*                                                                                                              */
/*   OpenNN: Open Neural Networks Library                                                                       */
/*   www.opennn.cimne.com                                                                                       */
/*                                                                                                              */
/*   M I N K O W S K I   E R R O R   C L A S S                                                                  */
/*                                                                                                              */
/*   Roberto Lopez                                                                                              */
/*   International Center for Numerical Methods in Engineering (CIMNE)                                          */
/*   Technical University of Catalonia (UPC)                                                                    */
/*   Barcelona, Spain                                                                                           */
/*   E-mail: rlopez@cimne.upc.edu                                                                               */
/*                                                                                                              */
/****************************************************************************************************************/

// OpenNN includes

#include "minkowski_error.h"

// System includes

#include <string>
#include <sstream>
#include <iostream>
#include <fstream>
#include <cmath>

// TinyXml includes

#include "../../tinyxml/tinyxml.h"


namespace OpenNN
{

// DEFAULT CONSTRUCTOR

/// Default constructor. 
/// It creates Minkowski error performance term not associated to any neural network and not measured on any data set.
/// It also initializes all the rest of class members to their default values.

MinkowskiError::MinkowskiError(void) : PerformanceTerm()
{
   set_default();
}


// NEURAL NETWORK CONSTRUCTOR

/// Neural network constructor. 
/// It creates a Minkowski error performance term associated to a neural network but not measured on any data set.
/// It also initializes all the rest of class members to their default values.
/// @param new_neural_network_pointer Pointer to a neural network object.

MinkowskiError::MinkowskiError(NeuralNetwork* new_neural_network_pointer)
: PerformanceTerm(new_neural_network_pointer)
{
   set_default();
}


// DATA SET CONSTRUCTOR

/// Data set constructor. 
/// It creates a Minkowski error performance term not associated to any neural network but to be measured on a data set.
/// It also initializes all the rest of class members to their default values.
/// @param new_data_set_pointer Pointer to a data set object.

MinkowskiError::MinkowskiError(DataSet* new_data_set_pointer)
: PerformanceTerm(new_data_set_pointer)
{
   set_default();
}


// NEURAL NETWORK AND DATA SET CONSTRUCTOR

/// Neural network and data set constructor. 
/// It creates a Minkowski error performance term object associated to a neural network and measured on a data set.
/// It also initializes all the rest of class members to their default values.
/// @param new_neural_network_pointer Pointer to a neural network object.
/// @param new_data_set_pointer Pointer to a data set object.

MinkowskiError::MinkowskiError(NeuralNetwork* new_neural_network_pointer, DataSet* new_data_set_pointer)
 : PerformanceTerm(new_neural_network_pointer, new_data_set_pointer)
{
   set_default();
}


// XML CONSTRUCTOR

/// XML constructor. 
/// It creates a Minkowski error object neither associated to a neural network nor to a data set. 
/// The object members are loaded by means of a XML element.
/// @param mean_squared_error_element Tiny XML element with the Minkowski error elements.

MinkowskiError::MinkowskiError(TiXmlElement* mean_squared_error_element)
 : PerformanceTerm(mean_squared_error_element)
{
   set_default();

   from_XML(mean_squared_error_element);
}


// DESTRUCTOR

/// Destructor.
/// It does not delete any pointer. 

MinkowskiError::~MinkowskiError(void) 
{
}


// METHODS

// double get_Minkowski_parameter(void) const method

/// This method returns the Minkowski exponent value used to calculate the error. 

double MinkowskiError::get_Minkowski_parameter(void) const
{
   return(Minkowski_parameter);
}


// void set_default(void) method

/// This method sets the default values to a Minkowski error object:
/// <ul>
/// <li> Minkowski parameter: 1.5.
/// <li> Display: true.
/// </ul>

void MinkowskiError::set_default(void)
{
   Minkowski_parameter = 1.5;

   display = true;
}


// void set_Minkowski_parameter(const double&) method

/// This method sets a new Minkowski exponent value to be used in order to calculate the error. 
/// The Minkowski R-value must be comprised between 1 and 2. 
/// @param new_Minkowski_parameter Minkowski exponent value. 

void MinkowskiError::set_Minkowski_parameter(const double& new_Minkowski_parameter)
{
   // Control sentence

   if(new_Minkowski_parameter < 1.0 || new_Minkowski_parameter > 2.0)
   {
      std::ostringstream buffer;

      buffer << "OpenNN Error. MinkowskiError class.\n"
             << "void set_Minkowski_parameter(const double&) method.\n"
             << "The Minkowski parameter must be comprised between 1 and 2\n";
    
      throw std::logic_error(buffer.str().c_str());
   }

   // Set Minkowski parameter
  
   Minkowski_parameter = new_Minkowski_parameter;
}


// void check(void) const method

/// This method checks that there are a neural network and a data set associated to the Minkowski error, 
/// and that the numbers of inputs and outputs in the neural network are equal to the numbers of inputs and targets in the data set. 
/// If some of the above conditions is not hold, the method throws an exception. 

void MinkowskiError::check(void) const
{
   std::ostringstream buffer;

   // Neural network stuff

   if(!neural_network_pointer)
   {
      buffer << "OpenNN Exception: MinkowskiError class.\n"
             << "void check(void) const method.\n"
             << "Pointer to neural network is NULL.\n";

      throw std::logic_error(buffer.str().c_str());	  
   }

   const MultilayerPerceptron* multilayer_perceptron_pointer = neural_network_pointer->get_multilayer_perceptron_pointer();

   if(!multilayer_perceptron_pointer)
   {
      buffer << "OpenNN Exception: MinkowskiError class.\n"
             << "void check(void) const method.\n"
             << "Pointer to multilayer perceptron is NULL.\n";

      throw std::logic_error(buffer.str().c_str());	  
   }

   const unsigned int inputs_number = multilayer_perceptron_pointer->count_inputs_number();
   const unsigned int outputs_number = multilayer_perceptron_pointer->count_outputs_number();

   if(inputs_number == 0)
   {
      buffer << "OpenNN Exception: MinkowskiError class.\n"
             << "void check(void) const method.\n"
             << "Number of inputs in multilayer perceptron object is zero.\n";

      throw std::logic_error(buffer.str().c_str());	  
   }

   if(outputs_number == 0)
   {
      buffer << "OpenNN Exception: MinkowskiError class.\n"
             << "void check(void) const method.\n"
             << "Number of outputs in multilayer perceptron object is zero.\n";

      throw std::logic_error(buffer.str().c_str());	  
   }

   // Data set stuff

   if(!data_set_pointer)
   {
      buffer << "OpenNN Exception: MinkowskiError class.\n"
             << "void check(void) const method.\n"
             << "Pointer to data set is NULL.\n";

      throw std::logic_error(buffer.str().c_str());	  
   }

   // Sum squared error stuff

   const VariablesInformation& variables_information = data_set_pointer->get_variables_information();

   const unsigned int targets_number = variables_information.count_targets_number();

   if(inputs_number != inputs_number)
   {
      buffer << "OpenNN Exception: MinkowskiError class.\n"
             << "void check(void) const method.\n"
             << "Number of inputs in multilayer perceptron must be equal to number of inputs in data set.\n";

      throw std::logic_error(buffer.str().c_str());	  
   }

   if(outputs_number != targets_number)
   {
      buffer << "OpenNN Exception: MinkowskiError class.\n"
             << "void check(void) const method.\n"
             << "Number of outputs in multilayer perceptron must be equal to number of targets in data set.\n";

      throw std::logic_error(buffer.str().c_str());
   }
}


// double calculate_evaluation(const Vector<double>&) const method

/// This method returns which would be the Minkowski error of for an hypothetical vector of parameters. 
/// It does not set that vector of parameters to the neural network. 
/// @param parameters Vector of a potential parameters for the neural network associated to the Minkowski error.

double MinkowskiError::calculate_evaluation(const Vector<double>& parameters) const
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

      buffer << "OpenNN Exception: MeanSquaredError class.\n"
             << "double calculate_evaluation(const Vector<double>&) const method.\n"
             << "Size (" << size << ") must be equal to number of parameters (" << parameters_number << ").\n";

      throw std::logic_error(buffer.str().c_str());
   }

   #endif

   NeuralNetwork neural_network_copy(*neural_network_pointer);

   neural_network_copy.set_parameters(parameters);

   MinkowskiError Minkowski_error_copy(*this);

   Minkowski_error_copy.set_neural_network_pointer(&neural_network_copy);

   return(Minkowski_error_copy.calculate_evaluation());
}


// double calculate_evaluation(void) const method

/// This method returns the Minkowski error evaluation.

double MinkowskiError::calculate_evaluation(void) const
{
   // Control sentence

   #ifdef _DEBUG 

   check();

   #endif

   // Neural network stuff

   const MultilayerPerceptron* multilayer_perceptron_pointer = neural_network_pointer->get_multilayer_perceptron_pointer();

   const unsigned int inputs_number = multilayer_perceptron_pointer->count_inputs_number();
   const unsigned int outputs_number = multilayer_perceptron_pointer->count_outputs_number();

   // Data set

   const InstancesInformation& instances_information = data_set_pointer->get_instances_information();
   const unsigned int training_instances_number = instances_information.count_training_instances_number();

   Vector<double> inputs(inputs_number);
   Vector<double> outputs(outputs_number);
   Vector<double> targets(outputs_number);

   double Minkowski_error = 0.0;

   for(unsigned int i = 0; i < training_instances_number; i++)
   {
      // Input vector

	  inputs = data_set_pointer->get_training_input_instance(i);

      // Output vector

      outputs = multilayer_perceptron_pointer->calculate_outputs(inputs);

      // Target vector

      targets = data_set_pointer->get_training_target_instance(i);

      // Minkowski error

	  Minkowski_error += outputs.calculate_Minkowski_error(targets, Minkowski_parameter);
   }

   return(Minkowski_error);
}


// double calculate_generalization_evaluation(void) const method

/// This method returns the Minkowski error of the multilayer perceptron measured on the generalization instances of the 
/// data set.

double MinkowskiError::calculate_generalization_evaluation(void) const
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

      // Minkowski error

	  generalization_evaluation = outputs.calculate_Minkowski_error(targets, Minkowski_parameter);
   }

   return(generalization_evaluation);
}


// Vector<double> calculate_gradient(void) const method

/// This method returns the Minkowski error function gradient of a multilayer perceptron on an inputs-targets 
/// data set. It uses the error back-propagation method.
/// @todo

Vector<double> MinkowskiError::calculate_gradient(void) const
{
   // Control sentence (if debug)

   #ifdef _DEBUG 

   check();

   #endif

   // Neural network stuff

   const MultilayerPerceptron* multilayer_perceptron_pointer = neural_network_pointer->get_multilayer_perceptron_pointer();

   const unsigned int inputs_number = multilayer_perceptron_pointer->count_inputs_number();
   const unsigned int outputs_number = multilayer_perceptron_pointer->count_outputs_number();

   const unsigned int parameters_number = multilayer_perceptron_pointer->count_parameters_number();

   // Data set stuff

   const InstancesInformation& instances_information = data_set_pointer->get_instances_information();

   const unsigned int training_instances_number = instances_information.count_training_instances_number();

   Vector<double> inputs(inputs_number);
   Vector<double> targets(outputs_number);

   Vector<double> objective_gradient(parameters_number, 0.0);

   for(unsigned int i = 0; i < training_instances_number; i++)
   {
      // Input vector

	  inputs = data_set_pointer->get_training_input_instance(i);

      // Target vector

      targets = data_set_pointer->get_training_target_instance(i);

      // Minkowski error gradient

//      for(unsigned int j = 0; j < outputs_number; j++)
//      {
//         output_objective_gradient[i] = Minkowski_parameter*pow(fabs(outputs[i] - target[i]), Minkowski_parameter-1.0);		 
//      }
//      objective_gradient += calculate_point_gradient(inputs, targets);           
   }

   return(objective_gradient);
}


// Matrix<double> calculate_Hessian(void) const method

/// @todo

Matrix<double> MinkowskiError::calculate_Hessian(void) const
{
   Matrix<double> objective_Hessian;

   return(objective_Hessian);
}


// std::string write_performance_term_type(void) const method

/// This method returns a string with the name of the Minkowski error performance type, "MINKOWSKI_ERROR".

std::string MinkowskiError::write_performance_term_type(void) const
{
   return("MINKOWSKI_ERROR");
}


// TiXmlElement* to_XML(void) const method 

/// This method serializes the Minkowski error object into a XML element of the TinyXML library. 
/// See the OpenNN manual for more information about the format of this element. 

TiXmlElement* MinkowskiError::to_XML(void) const
{
   std::ostringstream buffer;

   // Minkowski error

   TiXmlElement* Minkowski_error_element = new TiXmlElement("MinkowskiError");
   Minkowski_error_element->SetAttribute("Version", 4);

   // Minkowski parameter
   {
      TiXmlElement* Minkowski_parameter_element = new TiXmlElement("MinkowskiParameter");
      Minkowski_error_element->LinkEndChild(Minkowski_parameter_element);

      buffer.str("");
      buffer << Minkowski_parameter;

      TiXmlText* Minkowski_parameter_text = new TiXmlText(buffer.str().c_str());
      Minkowski_parameter_element->LinkEndChild(Minkowski_parameter_text);
   }
   // Display

   {
      TiXmlElement* display_element = new TiXmlElement("Display");
      Minkowski_error_element->LinkEndChild(display_element);

      buffer.str("");
      buffer << display;

      TiXmlText* display_text = new TiXmlText(buffer.str().c_str());
      display_element->LinkEndChild(display_text);
   }

   return(Minkowski_error_element);
}


// void from_XML(TiXmlElement*) method

/// @todo

void MinkowskiError::from_XML(TiXmlElement* Minkoski_error_element)  
{
   if(Minkoski_error_element)
   { 
      // Minkowski parameter
      {
         TiXmlElement* element = Minkoski_error_element->FirstChildElement("MinkowskiParameter");

         if(element)
         {
            const double new_Minkowski_parameter = atof(element->GetText()); 

            try
            {
               set_Minkowski_parameter(new_Minkowski_parameter);
            }
            catch(std::exception& e)
            {
               std::cout << e.what() << std::endl;		 
            }
         }
      
      }

      // Display
      {
         TiXmlElement* display_element = Minkoski_error_element->FirstChildElement("Display");

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


// int calculate_sign(const double&) const  method

/// This method returns -1 if some real value is negative and 1 if it is positive. 
/// @param value Real value. 

int MinkowskiError::calculate_sign(const double& value) const
{
   if(value < 0.0)
   {
      return(-1);
   }
   else
   {
      return(1);
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
