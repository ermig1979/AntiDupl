/****************************************************************************************************************/
/*                                                                                                              */
/*   OpenNN: Open Neural Networks Library                                                                       */
/*   www.opennn.cimne.com                                                                                       */
/*                                                                                                              */
/*   S O L U T I O N S   E R R O R   C L A S S                                                                  */
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

#include "solutions_error.h"

// TinyXml includes

#include "../../tinyxml/tinyxml.h"


namespace OpenNN
{


// DEFAULT CONSTRUCTOR

/// Default constructor. 
/// It creates a solutions error performance term not associated to any neural network and not measured on any mathematical model.
/// It also initializes all the rest of class members to their default values.

SolutionsError::SolutionsError(void) : PerformanceTerm()
{
   construct_numerical_differentiation();

   set_default();
}


// NEURAL NETWORK CONSTRUCTOR

/// Neural network constructor. 
/// It creates a solutions error associated to a neural network, but not to a .
/// It also initializes all the rest of class members to their default values.
/// @param new_neural_network_pointer Pointer to a neural network object.

SolutionsError::SolutionsError(NeuralNetwork* new_neural_network_pointer)
: PerformanceTerm(new_neural_network_pointer)
{
   construct_numerical_differentiation();

   set_default();
}


// MATHEMATICAL MODEL CONSTRUCTOR

/// Mathematical model constructor. 
/// It creates solutions error object not associated to any neural network but to be measured on a given mathematical model object.
/// It also initializes all the rest of class members to their default values.
/// @param new_mathematical_model_pointer Pointer to a mathematical model object.

SolutionsError::SolutionsError(MathematicalModel* new_mathematical_model_pointer)
: PerformanceTerm(new_mathematical_model_pointer)
{
   construct_numerical_differentiation();

   set_default();
}


// NEURAL NETWORK AND MATHEMATICAL MODEL  CONSTRUCTOR

/// Neural network and mathematical model constructor. 
/// It creates a solutions error functional associated to a neural network and measured on a mathematical model.
/// It also initializes all the rest of class members to their default values.
/// @param new_neural_network_pointer Pointer to a neural network object.
/// @param new_mathematical_model_pointer Pointer to a mathematical model object.

SolutionsError::SolutionsError(NeuralNetwork* new_neural_network_pointer, MathematicalModel* new_mathematical_model_pointer)
 : PerformanceTerm(new_neural_network_pointer, new_mathematical_model_pointer)
{
   construct_numerical_differentiation();

   set_default();
}


// XML CONSTRUCTOR

/// xml constructor. 
/// It creates a solutions error performance term not associated to any neural network and not measured on any mathematical model.
/// It also initializes all the rest of class members with values taken from a Tiny XML element.
/// @param solutions_error_element XML element containing the class members. 

SolutionsError::SolutionsError(TiXmlElement* solutions_error_element) 
 : PerformanceTerm(solutions_error_element)
{
   set_default();

   from_XML(solutions_error_element);
}


// COPY CONSTRUCTOR

/// Copy constructor. 
/// It creates a copy of an existing solutions error object. 
/// @param other_solutions_error Solutions error object to be copied.

SolutionsError::SolutionsError(const SolutionsError& other_solutions_error)
 : PerformanceTerm()
{
   set_default();

   set(other_solutions_error);
}


// DESTRUCTOR

/// Destructor.
/// This destructor does not delete any object. 
/// The numerical differentiation object is deleted by the performance term destructor. 

SolutionsError::~SolutionsError(void)
{
}


// ASSIGNMENT OPERATOR

// FinalSolutionsError& operator = (const FinalSolutionsError&) method

/// Assignment operator. 
/// It assigns to this solutions error object the members from another solutions error object. 
/// @param other_solutions_error Solutions error object to be copied. 

SolutionsError& SolutionsError::operator = (const SolutionsError& other_solutions_error)
{
   if(this != &other_solutions_error) 
   {
      *neural_network_pointer = *other_solutions_error.neural_network_pointer;
      *data_set_pointer = *other_solutions_error.data_set_pointer;
      *mathematical_model_pointer = *other_solutions_error.mathematical_model_pointer;
      *numerical_differentiation_pointer = *other_solutions_error.numerical_differentiation_pointer;
      display = other_solutions_error.display;

	  solutions_error_method = other_solutions_error.solutions_error_method;
      solutions_errors_weights = other_solutions_error.solutions_errors_weights;
   }

   return(*this);
}


// EQUAL TO OPERATOR

// bool operator == (const FinalSolutionsError&) const method

/// Equal to operator. 
/// It compares this object to another object, and returns true if both have the same member data, and false otherwise. 

bool SolutionsError::operator == (const SolutionsError& other_solutions_error) const
{
   if(*neural_network_pointer == *other_solutions_error.neural_network_pointer
   && *mathematical_model_pointer == *other_solutions_error.mathematical_model_pointer
   && *numerical_differentiation_pointer == *other_solutions_error.numerical_differentiation_pointer
   && display == other_solutions_error.display    
   && solutions_errors_weights == other_solutions_error.solutions_errors_weights)
   {
      return(true);
   }
   else
   {
      return(false);  
   }
}


// METHODS

// const SolutionsErrorMethod& get_solutions_error_method(void) const method

/// This method returns the method for computing the solutions error.

const SolutionsError::SolutionsErrorMethod& SolutionsError::get_solutions_error_method(void) const
{
   return(solutions_error_method);
}


// string:std: write_solutions_error_method(void) const method

/// This method returns a string with the name of the method for computing the solutions error.

std::string SolutionsError::write_solutions_error_method(void) const
{
   if(solutions_error_method == SolutionsErrorSum)
   {
      return("SolutionsErrorSum");
   }
   else if(solutions_error_method == SolutionsErrorIntegral)
   {
      return("SolutionsErrorIntegral");
   }
   else
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: SolutionsError class.\n"
             << "std::string write_solutions_error_method(void) const method.\n"
             << "Unknown solutions error method.\n";
 
	  throw std::logic_error(buffer.str());
   }
}


// const Vector<double>& get_solutions_errors_weights(void) const method

/// This method returns the weights for every single solution error, corresponding to a dependent variable. 

const Vector<double>& SolutionsError::get_solutions_errors_weights(void) const
{
   return(solutions_errors_weights);
}


// const double& get_solution_error_weight(const unsigned int&) const method

/// This method returns the weight for a single solution error, corresponding to a dependent variable. 

const double& SolutionsError::get_solution_error_weight(const unsigned int& i) const
{
   return(solutions_errors_weights[i]);
}


// void set(const SolutionsError&) method

/// This method sets to this object the data from another object of the same class.
/// @param other_solutions_error Solutions error object to be copied. 

void SolutionsError::set(const SolutionsError& other_solutions_error)
{
   neural_network_pointer = other_solutions_error.neural_network_pointer;

   data_set_pointer = other_solutions_error.data_set_pointer;

   mathematical_model_pointer = other_solutions_error.mathematical_model_pointer;

   if(other_solutions_error.numerical_differentiation_pointer)
   {
      numerical_differentiation_pointer = new NumericalDifferentiation(*other_solutions_error.numerical_differentiation_pointer);
   }

   display = other_solutions_error.display;  
   
   solutions_errors_weights = other_solutions_error.solutions_errors_weights;
}


// void set_solutions_error_method(const SolutionsErrorMethod&) method

/// This method sets a new method for calculating the error between the solution of a mathematical model and a target solution. 
/// @param new_solutions_error_method Method for evaluating this performance term. 

void SolutionsError::set_solutions_error_method(const SolutionsErrorMethod& new_solutions_error_method) 
{
   solutions_error_method = new_solutions_error_method;
}


// void set_solutions_error_method(const std::string&) method

/// This method sets a new solutions error method from a string with that method's name.
/// @param new_solutions_error_method String with the name of the method. 

void SolutionsError::set_solutions_error_method(const std::string& new_solutions_error_method)
{
   if(new_solutions_error_method == "SolutionsErrorSum")
   {
      set_solutions_error_method(SolutionsErrorSum);
   }
   else if(new_solutions_error_method == "SolutionsErrorIntegral")
   {
      set_solutions_error_method(SolutionsErrorIntegral);
   }
   else
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: SolutionsError class.\n"
             << "void set_solutions_error_method(const std::string&) method.\n"
			 << "Unknown solutions error method: " << new_solutions_error_method << ".\n";

	  throw std::logic_error(buffer.str());
   }  
}


// void set_solutions_errors_weights(const Vector<double>&) method

/// This method sets new weights for the solution error of every dependent variable. 
/// @param new_solutions_errors_weights Weights for the errors between the dependent variable solutions and the corresponding target solutions. 

void SolutionsError::set_solutions_errors_weights(const Vector<double>& new_solutions_errors_weights)
{
   solutions_errors_weights = new_solutions_errors_weights;
}


// void set_solution_error_weight(const unsigned int&, const double&) method

/// This method sets a new weight for the solution error of a single dependent variable. 
/// @param i Index of dependent variable. 
/// @param new_solution_error_weight Weight for the error between the above dependent variable solution and the corresponding target solution. 

void SolutionsError::set_solution_error_weight(const unsigned int& i, const double& new_solution_error_weight)
{
   solutions_errors_weights[i] = new_solution_error_weight;
}


// void set_default(void) method

/// This method sets the default values for an object of the solutions error class:
/// <ul> 
/// <li> Solutions error method: Solutions error sum. 
/// <li> Solutions error weights: 1 for all dependent variables. 
/// <li> Display: True.
/// </ul>

void SolutionsError::set_default(void)
{
   solutions_error_method = SolutionsErrorSum;

   if(mathematical_model_pointer)
   {
      const unsigned int dependent_variables_number = mathematical_model_pointer->get_dependent_variables_number();

      solutions_errors_weights.set(dependent_variables_number, 1.0);   
   }

   if(numerical_differentiation_pointer)
   {
	  numerical_differentiation_pointer->set_default();
   }

   numerical_integration.set_default();

   display = true;
}


// Matrix<double> calculate_target_dependent_variables(const Matrix<double>&) const method

/// This method returns the default target solution matrix. 

Matrix<double> SolutionsError::calculate_target_dependent_variables(const Matrix<double>& independent_variables) const
{
   #ifdef _DEBUG 

   check();

   #endif

   const unsigned int& rows_number = independent_variables.get_rows_number();
   const unsigned int& dependent_variables_number = mathematical_model_pointer->get_dependent_variables_number();

   const Matrix<double> target_dependent_variables(rows_number, dependent_variables_number, 0.0);

   return(target_dependent_variables);
}


// void check(void) const method

/// This method checks that:
/// <ul> 
/// <li> The pointer to the neural network is not NULL.
/// <li> The pointer to the multilayer perceptron inside the neural network is not NULL.
/// <li> The pointer to the mathematical method is not NULL.
/// <li> The size of the solutions errors weights is equal to the number of dependent variables. 
/// </ul> 
/// If some of the conditions above is not hold, an exception is thrown. 

void SolutionsError::check(void) const
{
   std::ostringstream buffer;

   // Neural network stuff

   if(!neural_network_pointer)
   {
      buffer << "OpenNN Exception: SolutionsError class.\n"
             << "void check(void) const method.\n"
             << "Pointer to neural network is NULL.\n";

      throw std::logic_error(buffer.str().c_str());	  
   }

   const MultilayerPerceptron* multilayer_perceptron_pointer = neural_network_pointer->get_multilayer_perceptron_pointer();

   if(!multilayer_perceptron_pointer)
   {
      buffer << "OpenNN Exception: SolutionsError class.\n"
             << "void check(void) const method.\n"
             << "Pointer to multilayer perceptron is NULL.\n";

      throw std::logic_error(buffer.str().c_str());	  
   }

   const unsigned int inputs_number = multilayer_perceptron_pointer->count_inputs_number();
   const unsigned int outputs_number = multilayer_perceptron_pointer->count_outputs_number();

   if(inputs_number == 0)
   {
      buffer << "OpenNN Exception: SolutionsError class.\n"
             << "void check(void) const method.\n"
             << "Number of inputs in multilayer perceptron object is zero.\n";

      throw std::logic_error(buffer.str().c_str());	  
   }

   if(outputs_number == 0)
   {
      buffer << "OpenNN Exception: SolutionsError class.\n"
             << "void check(void) const method.\n"
             << "Number of outputs in multilayer perceptron object is zero.\n";

      throw std::logic_error(buffer.str().c_str());	  
   }

   // Mathematical model stuff

   if(!mathematical_model_pointer)
   {
      buffer << "OpenNN Exception: SolutionsError class.\n"
             << "void check(void) const method.\n"
             << "Pointer to mathematical model is NULL.\n";

      throw std::logic_error(buffer.str().c_str());	  
   }

   const unsigned int& dependent_variables_number = mathematical_model_pointer->get_dependent_variables_number();

   // Solutions error stuff

   const unsigned int solutions_errors_weights_size = solutions_errors_weights.size();

   if(solutions_errors_weights_size != dependent_variables_number)
   {
      buffer << "OpenNN Exception: SolutionsError class.\n"
             << "void check(void) const method.\n"
             << "Size of solutions errors weights (" << solutions_errors_weights_size << ") is not equal to number of dependent variables (" << dependent_variables_number << ").\n";

      throw std::logic_error(buffer.str().c_str());	     
   }
}


// double calculate_solutions_error_sum(void) const method

/// This method calculates the solutions error evaluation as the weighted mean squared error between 
/// the solutions to the mathematical model and the target solutions. 

double SolutionsError::calculate_solutions_error_sum(void) const   
{
   // Control sentence

   #ifdef _DEBUG 

   check();

   #endif

   const unsigned int independent_variables_number = mathematical_model_pointer->get_independent_variables_number();
   const unsigned int dependent_variables_number = mathematical_model_pointer->get_dependent_variables_number();

   const unsigned int variables_number = mathematical_model_pointer->count_variables_number();

   const Matrix<double> solution = mathematical_model_pointer->calculate_solutions(*neural_network_pointer);    

   const Vector<unsigned int> independent_variables_indices(0, 1, independent_variables_number-1);
   const Vector<unsigned int> dependent_variables_indices(independent_variables_number, 1, variables_number-1);

   const Matrix<double> independent_variables = solution.arrange_submatrix_columns(independent_variables_indices);
   const Matrix<double> dependent_variables = solution.arrange_submatrix_columns(dependent_variables_indices);

   const Matrix<double> target_dependent_variables = calculate_target_dependent_variables(independent_variables);

   double evaluation = 0.0;

   for(unsigned int i = 0; i < dependent_variables_number; i++)
   {
      if(solutions_errors_weights[i] != 0.0)
      {
		  const Vector<double> dependent_variable = dependent_variables.arrange_column(i);
		  const Vector<double> target_dependent_variable = target_dependent_variables.arrange_column(i);

		  evaluation += solutions_errors_weights[i]*(dependent_variable-target_dependent_variable).calculate_norm();
      }   
   }

   const unsigned int rows_number = independent_variables.get_rows_number();

   return(evaluation/(double)rows_number);
}


// double calculate_solutions_error_integral(void) const method

/// @todo

double SolutionsError::calculate_solutions_error_integral(void) const   
{
   return(0.0);
}


// double calculate_evaluation(void) const method

/// This method returns the objective value of a neural network according to the solutions error on a mathematical model.

double SolutionsError::calculate_evaluation(void) const
{
   // Control sentence

   #ifdef _DEBUG 

   check();

   #endif

   switch(solutions_error_method)
   {
      case SolutionsErrorSum:
      {
         return(calculate_solutions_error_sum());
      }            
      break;

      case SolutionsErrorIntegral:
      {
         return(calculate_solutions_error_integral());
      }
      break;

      default:
      {
         std::ostringstream buffer;

         buffer << "OpenNN Exception: SolutionsError class\n"
                << "double calculate_evaluation(void) const method.\n"               
                << "Unknown solutions error method.\n";

	     throw std::logic_error(buffer.str());
      }
      break;
   }
}


// double calculate_evaluation(const Vector<double>&) const method

/// This method calculates the solutions error value of the neural network for a given set of parameters. 
/// @param parameters Vector of potential neural network parameters. 

double SolutionsError::calculate_evaluation(const Vector<double>& parameters) const   
{
   // Control sentence

   #ifdef _DEBUG 

   check();

   #endif

   NeuralNetwork neural_network_copy(*neural_network_pointer);

   neural_network_copy.set_parameters(parameters);

   SolutionsError solutions_error_copy(*this);

   solutions_error_copy.set_neural_network_pointer(&neural_network_copy);
   solutions_error_copy.set_mathematical_model_pointer(mathematical_model_pointer);

   return(solutions_error_copy.calculate_evaluation());
}


// std::string write_performance_term_type(void) const method

/// This method returns a string with the name of the solutions error performance type, "SOLUTIONS_ERROR".

std::string SolutionsError::write_performance_term_type(void) const
{
   return("SOLUTION_ERROR");
}


// std::string write_information(void) const method

std::string SolutionsError::write_information(void) const
{
   std::ostringstream buffer;
   
   buffer << "Solutions error: " << calculate_evaluation() << "\n";

   return(buffer.str());
}


// void print(void) const method

/// This method prints to the screen the string representation of this solutions error object. 

void SolutionsError::print(void) const
{
   if(display)
   {
      std::cout << "Solutions error:\n"
	            << "Solutions error method: " << write_solutions_error_method() << "\n"
                << "Solutions errors weights: " << solutions_errors_weights << std::endl;
   }
}


// TiXmlElement* to_XML(void) method method 

/// This method returns a representation of the solutions error object, in XML format. 

TiXmlElement* SolutionsError::to_XML(void) const
{
   std::ostringstream buffer;

   // Solutions error

   TiXmlElement* solutions_error_element = new TiXmlElement("SolutionsError");
   solutions_error_element->SetAttribute("Version", 4);

   // Numerical differentiation
   
   if(numerical_differentiation_pointer)
   {
      TiXmlElement* element = numerical_differentiation_pointer->to_XML();
      solutions_error_element->LinkEndChild(element);
   }

   // Numerical integration
   {
      TiXmlElement* element = numerical_integration.to_XML();
      solutions_error_element->LinkEndChild(element);
   }

   // Solutions error method
   {
      TiXmlElement* element = new TiXmlElement("SolutionsErrorMethod");
      solutions_error_element->LinkEndChild(element);

      TiXmlText* text = new TiXmlText(write_solutions_error_method().c_str());
      element->LinkEndChild(text);
   }


   // Solutions error weights
   {
      TiXmlElement* constraints_weights_element = new TiXmlElement("SolutionsErrorWeights");
      solutions_error_element->LinkEndChild(constraints_weights_element);

      buffer.str("");
      buffer << solutions_errors_weights;

      TiXmlText* constraints_weights_text = new TiXmlText(buffer.str().c_str());
      constraints_weights_element->LinkEndChild(constraints_weights_text);
   }

   // Display
   {
      TiXmlElement* display_element = new TiXmlElement("Display");
      solutions_error_element->LinkEndChild(display_element);

      buffer.str("");
      buffer << display;

      TiXmlText* display_text = new TiXmlText(buffer.str().c_str());
      display_element->LinkEndChild(display_text);
   }

   return(solutions_error_element);
}


// void from_XML(TiXmlElement) method

/// This method loads a solutions error object from a XML element. 
// @param trajectory_error_element Tiny XML element with the members of the solutions error object. 

void SolutionsError::from_XML(TiXmlElement* solutions_error_element)
{
   if(solutions_error_element)
   { 
      // Display
      {
         TiXmlElement* display_element = solutions_error_element->FirstChildElement("Display");

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
