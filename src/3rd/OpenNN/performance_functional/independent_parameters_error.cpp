/****************************************************************************************************************/
/*                                                                                                              */
/*   OpenNN: Open Neural Networks Library                                                                       */
/*   www.opennn.cimne.com                                                                                       */
/*                                                                                                              */
/*   I N D E P E N D E N T   P A R A M E T E R S   E R R O R   C L A S S                                        */
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

#include "independent_parameters_error.h"

// TinyXml includes

#include "../../tinyxml/tinyxml.h"


namespace OpenNN
{

// DEFAULT CONSTRUCTOR

/// Default constructor. 
/// It creates a independent parameters error performance term with all pointers initialized to NULL.
/// It also initializes all the rest of class members to their default values.

IndependentParametersError::IndependentParametersError(void) 
 : PerformanceTerm()
{
   construct_numerical_differentiation();

   set_default();
}


// NEURAL NETWORK CONSTRUCTOR

/// Neural network constructor. 
/// It creates a independent parameters error performance term associated to a neural network.
/// It also initializes all the rest of class members to their default values.
/// @param new_neural_network_pointer Pointer to a neural network object.

IndependentParametersError::IndependentParametersError(NeuralNetwork* new_neural_network_pointer)
: PerformanceTerm(new_neural_network_pointer)
{
   construct_numerical_differentiation();

   set_default();
}


// XML CONSTRUCTOR

/// XML constructor. 
/// It creates a independent parameters error performance term with all pointers initialized to NULL.
/// It also loads the rest of class members from a XML element.
/// @param independent_parameters_error_element Tiny XML element of a independent parameters values object.

IndependentParametersError::IndependentParametersError(TiXmlElement* independent_parameters_error_element) 
 : PerformanceTerm(independent_parameters_error_element)
{
   construct_numerical_differentiation();

   set_default();
}


// DESTRUCTOR

/// Destructor.

IndependentParametersError::~IndependentParametersError(void)
{
}


// ASSIGNMENT OPERATOR

// FinalSolutionsError& operator = (const FinalSolutionsError&) method

/// Assignment operator.  

IndependentParametersError& IndependentParametersError::operator = (const IndependentParametersError& other_independent_parameters_error)
{
   if(this != &other_independent_parameters_error) 
   {
      *neural_network_pointer = *other_independent_parameters_error.neural_network_pointer;
      *data_set_pointer = *other_independent_parameters_error.data_set_pointer;
      *mathematical_model_pointer = *other_independent_parameters_error.mathematical_model_pointer;
      *numerical_differentiation_pointer = *other_independent_parameters_error.numerical_differentiation_pointer;
      display = other_independent_parameters_error.display;

      target_independent_parameters = other_independent_parameters_error.target_independent_parameters;
      independent_parameters_errors_weights = other_independent_parameters_error.independent_parameters_errors_weights;
   }

   return(*this);
}


// EQUAL TO OPERATOR

// bool operator == (const FinalSolutionsError&) const method

/// Equal to operator. 

bool IndependentParametersError::operator == (const IndependentParametersError& other_independent_parameters_error) const
{
   if(*neural_network_pointer == *other_independent_parameters_error.neural_network_pointer
   && *mathematical_model_pointer == *other_independent_parameters_error.mathematical_model_pointer
   && *numerical_differentiation_pointer == *other_independent_parameters_error.numerical_differentiation_pointer
   && display == other_independent_parameters_error.display    
   && target_independent_parameters == other_independent_parameters_error.target_independent_parameters
   && independent_parameters_errors_weights == other_independent_parameters_error.independent_parameters_errors_weights)
   {
      return(true);
   }
   else
   {
      return(false);  
   }
}


// METHODS


// const Vector<double>& get_target_independent_parameters(void) const method

/// This method returns the desired values for the independent parameter. 

const Vector<double>& IndependentParametersError::get_target_independent_parameters(void) const
{
   return(target_independent_parameters);
}


// const double& get_target_independent_parameter(const unsigned int&) const method

/// This method returns the desired value of a single independent parameter. 
/// @param i Index of independent parameter. 

const double& IndependentParametersError::get_target_independent_parameter(const unsigned int& i) const
{
   return(target_independent_parameters[i]);
}


// const Vector<double>& get_independent_parameters_errors_weights(void) const method

/// This method returns the weight for each error between the actual independent parameters and their target values. 

const Vector<double>& IndependentParametersError::get_independent_parameters_errors_weights(void) const
{
   return(independent_parameters_errors_weights);
}


// const double& get_independent_parameter_error_weight(const unsigned int&) const method

/// This method returns the weight for a singel error between an independent parameters and its target value. 
/// @param i Index of independent parameter parameter. 

const double& IndependentParametersError::get_independent_parameter_error_weight(const unsigned int& i) const
{
   return(independent_parameters_errors_weights[i]);   
}


// void set_target_independent_parameters(const Vector<double>&) method

/// This method sets new desired values for the independent parameters. 
/// @param new_target_independent_parameters Vector of desired values for the independent parameters. 

void IndependentParametersError::set_target_independent_parameters(const Vector<double>& new_target_independent_parameters)
{
   target_independent_parameters = new_target_independent_parameters;
}


// void set_target_independent_parameter(const unsigned int&, const double&) method

/// This method sets the desired value of a single independent parameter. 
/// @param i Index of independent parameter. 
/// @param new_target_independent_parameter Desired value for that parameter. 

void IndependentParametersError::set_target_independent_parameter(const unsigned int& i, const double& new_target_independent_parameter)
{
   target_independent_parameters[i] = new_target_independent_parameter;
}


// void set_independent_parameters_errors_weights(const Vector<double>&) method

/// This method sets new weights for each error between the actual independent parameters and their target values. 
/// @param new_independent_parameters_errors_weights Vector of weights, with size the number of independent parameters.

void IndependentParametersError::set_independent_parameters_errors_weights(const Vector<double>& new_independent_parameters_errors_weights) 
{
   independent_parameters_errors_weights = new_independent_parameters_errors_weights;
}


// void set_independent_parameter_error_weight(const unsigned int&, const double&) method

/// This method sets a new weight for the error between a single independent parameter and its target value. 
/// @param i Index of independent parameter. 
/// @param new_independent_parameter_error_weight Weight value.

void IndependentParametersError::set_independent_parameter_error_weight(const unsigned int& i, const double& new_independent_parameter_error_weight)
{
   independent_parameters_errors_weights[i] = new_independent_parameter_error_weight;
}


// void set_default(void) method

/// This method sets the default values for this object:
/// <ul>
/// <li> Target independent parameters: 0 for all parameters. 
/// <li> Errors weights: 1 for all errors. 
/// <li> Display: True. 
/// </ul>

void IndependentParametersError::set_default(void)
{
   if(neural_network_pointer)
   {
      const IndependentParameters* independent_parameters_pointer = neural_network_pointer->get_independent_parameters_pointer();

	  if(independent_parameters_pointer)
	  {
         const unsigned int independent_parameters_number  = independent_parameters_pointer->count_parameters_number();	  

         target_independent_parameters.set(independent_parameters_number, 0.0);

		 independent_parameters_errors_weights.set(independent_parameters_number, 1.0);
	  }
   }
   else
   {
       target_independent_parameters.set();
       independent_parameters_errors_weights.set();
   }

   display = true;
}


// void check(void) const method

/// This method checks that there are a neural network and a data set associated to the sum squared error, 
/// and that the number of independent parameters in the neural network is equal to the number of size of the target independent parameters in the performance term. 
/// If some of the above conditions is not hold, the method throws an exception. 

void IndependentParametersError::check(void) const
{
   std::ostringstream buffer;

   // Neural network stuff

   if(!neural_network_pointer)
   {
      buffer << "OpenNN Exception: IndependentParametersError class.\n"
             << "void check(void) const method.\n"
             << "Pointer to neural network is NULL.\n";

      throw std::logic_error(buffer.str().c_str());	  
   }

   const IndependentParameters* independent_parameters_pointer = neural_network_pointer->get_independent_parameters_pointer();

   if(!independent_parameters_pointer)
   {
      buffer << "OpenNN Exception: IndependentParametersError class.\n"
             << "void check(void) const method.\n"
             << "Pointer to independent parameters is NULL.\n";

      throw std::logic_error(buffer.str().c_str());	  
   }

   const unsigned int independent_parameters_number = independent_parameters_pointer->count_parameters_number();

   if(independent_parameters_number == 0)
   {
      buffer << "OpenNN Exception: IndependentParametersError class.\n"
             << "void check(void) const method.\n"
             << "Number of independent parameters is zero.\n";

      throw std::logic_error(buffer.str().c_str());	  
   }

   // Mathematical model stuff

   if(!mathematical_model_pointer)
   {
      buffer << "OpenNN Exception: IndependentParametersError class.\n"
             << "void check(void) const method.\n"
             << "Pointer to mathematical model is NULL.\n";

      throw std::logic_error(buffer.str().c_str());	  
   }

   // Independent parameters error stuff

   const unsigned int target_independent_parameters_size = target_independent_parameters.size();

   if(target_independent_parameters_size != independent_parameters_number)
   {
      buffer << "OpenNN Exception: IndependentParametersError class." << std::endl
             << "double calculate_evaluation(void) const method." << std::endl
             << "Size of target independent parameters must be equal to number of independent parameters." << std::endl;

      throw std::logic_error(buffer.str().c_str());	  
   }

   const unsigned int independent_parameters_errors_weights_size = independent_parameters_errors_weights.size();

   if(independent_parameters_errors_weights_size != independent_parameters_number)
   {
      buffer << "OpenNN Exception: IndependentParametersError class." << std::endl
             << "double calculate_evaluation(void) const method." << std::endl
             << "Size of independent parameters errors weights must be equal to number of independent parameters." << std::endl;

      throw std::logic_error(buffer.str().c_str());	  
   }

}


// double calculate_evaluation(void) const method

/// This method returns the dot product between the independent parameters vector and its targets vector.

double IndependentParametersError::calculate_evaluation(void) const
{
   // Neural network stuff

   #ifdef _DEBUG 

   check();

   #endif

   // Neural network stuff

   const IndependentParameters* independent_parameters_pointer = neural_network_pointer->get_independent_parameters_pointer();

   const Vector<double> independent_parameters = independent_parameters_pointer->get_parameters();

   const Vector<double> independent_parameters_error = independent_parameters - target_independent_parameters;

   return((independent_parameters_errors_weights*independent_parameters_error*independent_parameters_error).calculate_sum());
}


// double calculate_evaluation(const Vector<double>&) const method

/// @todo

double IndependentParametersError::calculate_evaluation(const Vector<double>& parameters) const  
{
   // Neural network stuff

   #ifdef _DEBUG 

   check();

   #endif


#ifdef _DEBUG 

   const unsigned int size = parameters.size();

   const unsigned int parameters_number = neural_network_pointer->count_parameters_number();

   if(size != parameters_number)
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: IndependentParametersError class." << std::endl
             << "double calculate_evaluation(const Vector<double>&) const method." << std::endl
             << "Size of parameters (" << size << ") must be equal to number of parameters (" << parameters_number << ")." << std::endl;

      throw std::logic_error(buffer.str().c_str());
   }

   #endif

   NeuralNetwork neural_network_copy(*neural_network_pointer);

   neural_network_copy.set_parameters(parameters);

   IndependentParametersError independent_parameters_error_copy(*this);

   independent_parameters_error_copy.set_neural_network_pointer(&neural_network_copy);

   return(independent_parameters_error_copy.calculate_evaluation());
}


// Vector<double> calculate_gradient(void) const method

Vector<double> IndependentParametersError::calculate_gradient(void) const
{
   // Neural network stuff

   #ifdef _DEBUG 

   check();

   #endif

   const unsigned int parameters_number = neural_network_pointer->count_parameters_number();

   const IndependentParameters* independent_parameters_pointer = neural_network_pointer->get_independent_parameters_pointer();

   const unsigned int independent_parameters_number = independent_parameters_pointer->count_parameters_number();

   const unsigned int multilayer_perceptron_parameters_number = parameters_number - independent_parameters_number;

   Vector<double> multilayer_perceptron_gradient(multilayer_perceptron_parameters_number, 0.0);

   const Vector<double> independent_parameters = independent_parameters_pointer->get_parameters();

   const Vector<double> independent_parameters_gradient = (independent_parameters - target_independent_parameters)*2.0;

   return(multilayer_perceptron_gradient.get_assembly(independent_parameters_gradient));
}


// Matrix<double> calculate_Hessian(void) const method

Matrix<double> IndependentParametersError::calculate_Hessian(void) const   
{
   // Neural network stuff

   #ifdef _DEBUG 

   check();

   #endif

   const unsigned int parameters_number = neural_network_pointer->count_parameters_number();

   const IndependentParameters* independent_parameters_pointer = neural_network_pointer->get_independent_parameters_pointer();

   const unsigned int independent_parameters_number = independent_parameters_pointer->count_parameters_number();

   const unsigned int multilayer_perceptron_parameters_number = parameters_number - independent_parameters_number;

   Matrix<double> Hessian(parameters_number, parameters_number, 0.0);

   for(unsigned int i = multilayer_perceptron_parameters_number; i < parameters_number; i++)
   {
      for(unsigned int j = multilayer_perceptron_parameters_number; j < parameters_number; j++)
      {
         Hessian[i][j] = 2.0;
      }     
   }

   return(Hessian);
}


// std::string write_performance_term_type(void) const method

/// This method returns a string with the name of the independent parameters error performance type, "INDEPENDENT_PARAMETERS_ERROR".

std::string IndependentParametersError::write_performance_term_type(void) const
{
   return("INDEPENDENT_PARAMETERS_ERROR");
}


// std::string write_information(void) const method

std::string IndependentParametersError::write_information(void) const
{
   std::ostringstream buffer;

   buffer << "Independent parameters error: " << calculate_evaluation() << "\n";

   return(buffer.str());
}


// TiXmlElement* to_XML(void) method method 

/// This method returns a representation of the sum squared error object, in XML format. 

TiXmlElement* IndependentParametersError::to_XML(void) const
{
   std::ostringstream buffer;

   // Independent parameters error

   TiXmlElement* independent_parameters_error_element = new TiXmlElement("IndependentParametersError");
   independent_parameters_error_element->SetAttribute("Version", 4);

   // Numerical differentiation

   if(numerical_differentiation_pointer)
   {
      TiXmlElement* element = numerical_differentiation_pointer->to_XML();
      independent_parameters_error_element->LinkEndChild(element);
   }

   // Target independent parameters 

   {
   TiXmlElement* element = new TiXmlElement("TargetIndependentParamters");
   independent_parameters_error_element->LinkEndChild(element);

   buffer.str("");
   buffer << target_independent_parameters;

   TiXmlText* text = new TiXmlText(buffer.str().c_str());
   element->LinkEndChild(text);
   }

   // Independent parameters errors weights

   {
   TiXmlElement* element = new TiXmlElement("IndependentParametersErrorsWeights");
   independent_parameters_error_element->LinkEndChild(element);

   buffer.str("");
   buffer << independent_parameters_errors_weights;

   TiXmlText* text = new TiXmlText(buffer.str().c_str());
   element->LinkEndChild(text);
   }

   // Display

   {
   TiXmlElement* display_element = new TiXmlElement("Display");
   independent_parameters_error_element->LinkEndChild(display_element);

   buffer.str("");
   buffer << display;

   TiXmlText* display_text = new TiXmlText(buffer.str().c_str());
   display_element->LinkEndChild(display_text);
   }


   return(independent_parameters_error_element);
}


// void from_XML(TiXmlElement*) method

// This method loads a sum squared error object from a XML file. 
// @param element Name of XML sum squared error file. 

void IndependentParametersError::from_XML(TiXmlElement* independent_parameters_error_element)
{
   if(independent_parameters_error_element)
   { 
      // Display
      {
         TiXmlElement* display_element = independent_parameters_error_element->FirstChildElement("Display");

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
