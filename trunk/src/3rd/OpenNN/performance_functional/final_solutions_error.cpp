/****************************************************************************************************************/
/*                                                                                                              */
/*   OpenNN: Open Neural Networks Library                                                                       */
/*   www.opennn.cimne.com                                                                                       */
/*                                                                                                              */
/*   F I N A L   S O L U T I O N S   E R R O R   C L A S S                                                      */
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

#include "final_solutions_error.h"

// TinyXml includes

#include "../../tinyxml/tinyxml.h"


namespace OpenNN
{

// DEFAULT CONSTRUCTOR

/// Default constructor. 
/// It creates a final solutions error performance term not associated to any neural network and not measured on any mathematical model.
/// It also initializes all the rest of class members to their default values.

FinalSolutionsError::FinalSolutionsError(void) 
 : PerformanceTerm()
{
   construct_numerical_differentiation();

   set_default();
}


// NEURAL NETWORK CONSTRUCTOR

/// Neural network constructor. 
/// It creates a final solutions error performance term associated to a neural network but not measured on any mathematical model.
/// It also initializes all the rest of class members to their default values.
/// @param new_neural_network_pointer Pointer to a neural network object.

FinalSolutionsError::FinalSolutionsError(NeuralNetwork* new_neural_network_pointer)
: PerformanceTerm(new_neural_network_pointer)
{
   construct_numerical_differentiation();

   set_default();
}


// MATHEMATICAL MODEL CONSTRUCTOR

/// Mathematical model constructor. 
/// It creates a final solutions error performance term not associated to any neural network but to be measured on a given mathematical model.
/// It also initializes all the rest of class members to their default values.
/// @param new_mathematical_model_pointer Pointer to a mathematical model object.

FinalSolutionsError::FinalSolutionsError(MathematicalModel* new_mathematical_model_pointer)
: PerformanceTerm(new_mathematical_model_pointer)
{
   construct_numerical_differentiation();

   set_default();
}


// NEURAL NETWORK AND MATHEMATICAL MODEL CONSTRUCTOR

/// Neural network and mathematical model constructor. 
/// It creates a final solutions error performance term object associated to a given neural network and to be measured on a given mathematical model.
/// It also initializes all the rest of class members to their default values.
/// @param new_neural_network_pointer Pointer to a neural network object.
/// @param new_mathematical_model_pointer Pointer to a mathematical model object.

FinalSolutionsError::FinalSolutionsError(NeuralNetwork* new_neural_network_pointer, MathematicalModel* new_mathematical_model_pointer)
: PerformanceTerm(new_neural_network_pointer, new_mathematical_model_pointer)
{
   construct_numerical_differentiation();

   set_default();
}


// XML CONSTRUCTOR

/// XML constructor. 
/// It creates a final solutions error performance term neither associated to a neural network nor to a mathematical model.
/// It also sets the rest of members with values from a XML element.
/// @param final_state_error_element Pointer to a TinyXML with the member data. 

FinalSolutionsError::FinalSolutionsError(TiXmlElement* final_state_error_element)
 : PerformanceTerm(final_state_error_element)
{
   construct_numerical_differentiation();

   set_default();

   from_XML(final_state_error_element);
}


// COPY CONSTRUCTOR

/// Copy constructor. 
/// It creates a copy of an existing final solutions error object. 
/// @param other_final_solutions_error Final solutions error object to be copied.

FinalSolutionsError::FinalSolutionsError(const FinalSolutionsError& other_final_solutions_error)
 : PerformanceTerm()
{
   set(other_final_solutions_error);
}


// DESTRUCTOR

/// Destructor.

FinalSolutionsError::~FinalSolutionsError(void)
{
}


// ASSIGNMENT OPERATOR

// FinalSolutionsError& operator = (const FinalSolutionsError&) method

/// Assignment operator.  

FinalSolutionsError& FinalSolutionsError::operator = (const FinalSolutionsError& other_final_solutions_error)
{
   if(this != &other_final_solutions_error) 
   {
      *neural_network_pointer = *other_final_solutions_error.neural_network_pointer;
      *data_set_pointer = *other_final_solutions_error.data_set_pointer;
      *mathematical_model_pointer = *other_final_solutions_error.mathematical_model_pointer;
      *numerical_differentiation_pointer = *other_final_solutions_error.numerical_differentiation_pointer;
      display = other_final_solutions_error.display;

      final_solutions_errors_weights = other_final_solutions_error.final_solutions_errors_weights;
      target_final_solutions = other_final_solutions_error.target_final_solutions;
   }

   return(*this);
}


// EQUAL TO OPERATOR

// bool operator == (const FinalSolutionsError&) const method

/// Equal to operator. 
/// @todo

bool FinalSolutionsError::operator == (const FinalSolutionsError& other_final_solutions_error) const
{
   if(*neural_network_pointer == *other_final_solutions_error.neural_network_pointer
   && *mathematical_model_pointer == *other_final_solutions_error.mathematical_model_pointer
   && *numerical_differentiation_pointer == *other_final_solutions_error.numerical_differentiation_pointer
   && display == other_final_solutions_error.display    
   && final_solutions_errors_weights == other_final_solutions_error.final_solutions_errors_weights
   && target_final_solutions == other_final_solutions_error.target_final_solutions)
   {
      return(true);
   }
   else
   {
      return(false);  
   }
}


// METHODS

// const Vector<double>& get_final_solutions_errors_weights(void) const method

/// This method returns the weight values for each error in the final solutions. 

const Vector<double>& FinalSolutionsError::get_final_solutions_errors_weights(void) const
{
   return(final_solutions_errors_weights);
}


// const Vector<double>& get_target_final_solutions(void) const method

/// This method returns the desired final state of each dependent variable.

const Vector<double>& FinalSolutionsError::get_target_final_solutions(void) const
{
   return(target_final_solutions);
}


// void set(void) method

/// This method sets the neural network and mathematical model pointers to NULL.
/// It also deletes the numerical differentiation pointer inside this class. 
/// Finally it sets the rest of members to their default values. 

void FinalSolutionsError::set(void)
{
   neural_network_pointer = NULL;
   data_set_pointer = NULL;
   mathematical_model_pointer = NULL;

   delete numerical_differentiation_pointer;

   numerical_differentiation_pointer = NULL;

   set_default();   
}


// void set(NeuralNetwork*) method

/// @todo

void FinalSolutionsError::set(NeuralNetwork*)
{
}


// void set(MathematicalModel*) method

/// @todo

void FinalSolutionsError::set(MathematicalModel*)
{
}


// void set(NeuralNetwork*, MathematicalModel*) method

/// @todo

void FinalSolutionsError::set(NeuralNetwork*, MathematicalModel*)
{
}


// void set(const FinalSolutionsError&) method

/// This method sets this final solutions error object with the members from another object of the same class. 
/// @param other_final_solutions_error Final solutions error object to be copied. 

void FinalSolutionsError::set(const FinalSolutionsError& other_final_solutions_error)
{
   neural_network_pointer = other_final_solutions_error.neural_network_pointer;

   data_set_pointer = other_final_solutions_error.data_set_pointer;

   mathematical_model_pointer = other_final_solutions_error.mathematical_model_pointer;

   if(other_final_solutions_error.numerical_differentiation_pointer)
   {
      numerical_differentiation_pointer = new NumericalDifferentiation(*other_final_solutions_error.numerical_differentiation_pointer);
   }

   display = other_final_solutions_error.display;  
   
   final_solutions_errors_weights = other_final_solutions_error.final_solutions_errors_weights;

   target_final_solutions = other_final_solutions_error.target_final_solutions;
}


// void set_mathematical_model_pointer(MathematicalModel*) method

/// This method associates this final solutions error object with a given mathematical model object. 
/// @param new_mathematical_model_pointer Pointer to a mathematical model object. 

void FinalSolutionsError::set_mathematical_model_pointer(MathematicalModel* new_mathematical_model_pointer)
{
	mathematical_model_pointer = new_mathematical_model_pointer;

	if(mathematical_model_pointer)
	{
	   const unsigned int dependent_variables_number = mathematical_model_pointer->get_dependent_variables_number();

       target_final_solutions.set(dependent_variables_number, 0.0);

       final_solutions_errors_weights.set(dependent_variables_number, 1.0);
	}
}


// void set_final_solutions_errors_weights(const Vector<double>&) method

/// This method sets new weight values for each error in the final solutions. 
/// @param new_final_solutions_errors_weights Weight values. The size of this vector must be equal to the nuber of dependent variables in the mathematical model. 

void FinalSolutionsError::set_final_solutions_errors_weights(const Vector<double>& new_final_solutions_errors_weights)
{
   final_solutions_errors_weights = new_final_solutions_errors_weights;
}


// void set_final_solution_error_weight(const unsigned int&, const double&) method

/// This method sets a new weight value for a single error in the final solutions. 
/// @param i Index of dependent variable. 
/// @param new_final_solution_error_weight Weight value for the error of that dependent variable.

void FinalSolutionsError::set_final_solution_error_weight(const unsigned int& i, const double& new_final_solution_error_weight)
{
   final_solutions_errors_weights[i] = new_final_solution_error_weight;
}


// void set_target_final_solutions(const Vector<double>&) method

/// This method sets the desired final states for each dependent variable.
/// @param new_target_final_solutions Desired final values. The size of this vector must be equal to the number of dependent variables in the mathematical model. 

void FinalSolutionsError::set_target_final_solutions(const Vector<double>& new_target_final_solutions)
{
   target_final_solutions = new_target_final_solutions;
}


// void set_target_final_solution(const unsigned int& const double&) method

/// This method sets the desired final state of a single dependent variable.
/// @param i Index of dependent variable. 
/// @param new_target_final_solution Desired final value of that dependent variable.

void FinalSolutionsError::set_target_final_solution(const unsigned int& i, const double& new_target_final_solution)
{
   target_final_solutions[i] = new_target_final_solution;
}


// void set_default(void)

/// This method sets the default variables of this object. 
/// <ul>
/// <li> Target final solutions: 0 for all dependent variables. 
/// <li> Final solutions errors weights: 1 for all dependent variables. 
/// <li> Display: True.
/// </ul>

void FinalSolutionsError::set_default(void)
{
	if(mathematical_model_pointer)
	{
	   const unsigned int dependent_variables_number = mathematical_model_pointer->get_dependent_variables_number();

       target_final_solutions.set(dependent_variables_number, 0.0);

       final_solutions_errors_weights.set(dependent_variables_number, 1.0);
	}
    else
    {
       target_final_solutions.set();

       final_solutions_errors_weights.set();   
    }

	display = true;
}


// void check(void) const method

/// This method checks that there are a neural network and a mathematical model associated to the sum squared error, 
/// and that the number of dependent variables in the mathematical model is equal to the number of target final solutions in the performance term. 
/// If some of the above conditions is not hold, the method throws an exception. 

void FinalSolutionsError::check(void) const
{
   std::ostringstream buffer;

   // Neural network stuff

   if(!neural_network_pointer)
   {
      buffer << "OpenNN Exception: FinalSolutionsError class.\n"
             << "void check(void) const method.\n"
             << "Pointer to neural network is NULL.\n";

      throw std::logic_error(buffer.str().c_str());	  
   }

   const MultilayerPerceptron* multilayer_perceptron_pointer = neural_network_pointer->get_multilayer_perceptron_pointer();

   if(!multilayer_perceptron_pointer)
   {
      buffer << "OpenNN Exception: FinalSolutionsError class.\n"
             << "void check(void) const method.\n"
             << "Pointer to multilayer perceptron is NULL.\n";

      throw std::logic_error(buffer.str().c_str());	  
   }

   const unsigned int inputs_number = multilayer_perceptron_pointer->count_inputs_number();
   const unsigned int outputs_number = multilayer_perceptron_pointer->count_outputs_number();

   if(inputs_number == 0)
   {
      buffer << "OpenNN Exception: FinalSolutionsError class.\n"
             << "void check(void) const method.\n"
             << "Number of inputs in multilayer perceptron object is zero.\n";

      throw std::logic_error(buffer.str().c_str());	  
   }

   if(outputs_number == 0)
   {
      buffer << "OpenNN Exception: FinalSolutionsError class.\n"
             << "void check(void) const method.\n"
             << "Number of outputs in multilayer perceptron object is zero.\n";

      throw std::logic_error(buffer.str().c_str());	  
   }

   // Mathematical model stuff

   if(!mathematical_model_pointer)
   {
      buffer << "OpenNN Exception: FinalSolutionsError class.\n"
             << "void check(void) const method.\n"
             << "Pointer to mathematical model is NULL.\n";

      throw std::logic_error(buffer.str().c_str());	  
   }

   // Final solutions error stuff

   const unsigned int dependent_variables_number = mathematical_model_pointer->get_dependent_variables_number();

   const unsigned int target_final_solutions_size = target_final_solutions.size();

   if(target_final_solutions_size != dependent_variables_number)
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: FinalSolutionsError class." << std::endl
             << "double calculate_evaluation(void) const method." << std::endl
             << "Size of target final solutions must be equal to number of dependent variables." << std::endl;

      throw std::logic_error(buffer.str().c_str());	  
   }

}


// Vector<double> calculate_evaluation(void) const method

double FinalSolutionsError::calculate_evaluation(void) const   
{
   // Control sentence

   #ifdef _DEBUG 

   check();

   #endif

   // Final state error stuff

   const unsigned int independent_variables_number = mathematical_model_pointer->get_independent_variables_number();
   const unsigned int dependent_variables_number = mathematical_model_pointer->get_dependent_variables_number();

   const Vector<double> final_solutions = mathematical_model_pointer->calculate_final_solutions(*neural_network_pointer);

   const Vector<double> dependent_variables_final_solutions = final_solutions.take_out(independent_variables_number, dependent_variables_number);

   const Vector<double> final_solutions_errors = dependent_variables_final_solutions - target_final_solutions;

   return((final_solutions_errors_weights*final_solutions_errors*final_solutions_errors).calculate_sum());
}


// double calculate_evaluation(const Vector<double>&) const method

double FinalSolutionsError::calculate_evaluation(const Vector<double>& parameters) const   
{
   // Control sentence

   #ifdef _DEBUG 

   check();

   #endif

   // Performance term stuff

   #ifdef _DEBUG 

   const unsigned int size = parameters.size();

   const unsigned int parameters_number = neural_network_pointer->count_parameters_number();

   if(size != parameters_number)
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: FinalSolutionsError class." << std::endl
             << "double calculate_evaluation(const Vector<double>&) const method." << std::endl
             << "Size of parameters (" << size << ") must be equal to number of parameters (" << parameters_number << ")." << std::endl;

      throw std::logic_error(buffer.str().c_str());
   }

   #endif

   NeuralNetwork neural_network_copy(*neural_network_pointer);
   neural_network_copy.set_parameters(parameters);

   FinalSolutionsError final_solutions_error_copy(*this);
   final_solutions_error_copy.set_neural_network_pointer(&neural_network_copy);
   final_solutions_error_copy.set_mathematical_model_pointer(mathematical_model_pointer);

   return(final_solutions_error_copy.calculate_evaluation());
}


// std::string write_performance_term_type(void) const method

/// This method returns a string with the name of the final solutions error performance type, "FINAL_SOLUTIONS_ERROR".

std::string FinalSolutionsError::write_performance_term_type(void) const
{
   return("FINAL_SOLUTIONS_ERROR");
}


// std::string write_information(void) const method

std::string FinalSolutionsError::write_information(void) const
{
   std::ostringstream buffer;

   const unsigned int independent_variables_number = mathematical_model_pointer->get_independent_variables_number();
   const unsigned int dependent_variables_number = mathematical_model_pointer->get_dependent_variables_number();

   const Vector<double> final_solutions = mathematical_model_pointer->calculate_final_solutions(*neural_network_pointer);

   const Vector<double> dependent_variables_final_solutions = final_solutions.take_out(independent_variables_number, dependent_variables_number);

   const Vector<double> final_solutions_errors = dependent_variables_final_solutions - target_final_solutions;

   const double evaluation = (final_solutions_errors_weights*final_solutions_errors*final_solutions_errors).calculate_sum();

   buffer << "Final solutions error\n"
	      << "Target final solutions: " << target_final_solutions << "\n"
		  << "Final solutions: " << dependent_variables_final_solutions << "\n"
		  << "Evaluation: " << evaluation << "\n";

   return(buffer.str());
}


// TiXmlElement* to_XML(void) method method 

/// This method returns a representation of the sum squared error object, in XML format. 

TiXmlElement* FinalSolutionsError::to_XML(void) const
{
   std::ostringstream buffer;

   // Final solutions error

   TiXmlElement* final_solution_error_element = new TiXmlElement("FinalSolutionsError");
   final_solution_error_element->SetAttribute("Version", 4);

   // Numerical differentiation
   {
      if(numerical_differentiation_pointer)
      {
         TiXmlElement* element = numerical_differentiation_pointer->to_XML();

         if(element)
         {
            final_solution_error_element->LinkEndChild(element);
         }
      }
   }

   // Final solutions errors weights
   {
      TiXmlElement* element = new TiXmlElement("FinalSolutionsErrorsWeights");
      final_solution_error_element->LinkEndChild(element);

      buffer.str("");
      buffer << final_solutions_errors_weights;

      TiXmlText* text = new TiXmlText(buffer.str().c_str());
      element->LinkEndChild(text);
   }

   // Target final solution
   {
      TiXmlElement* element = new TiXmlElement("TargetFinalSolution");
      final_solution_error_element->LinkEndChild(element);

      buffer.str("");
      buffer << target_final_solutions;

      TiXmlText* text = new TiXmlText(buffer.str().c_str());
      element->LinkEndChild(text);
   }

   // Display
   {
      TiXmlElement* display_element = new TiXmlElement("Display");
      final_solution_error_element->LinkEndChild(display_element);

      buffer.str("");
      buffer << display;

      TiXmlText* display_text = new TiXmlText(buffer.str().c_str());
      display_element->LinkEndChild(display_text);
   }

   return(final_solution_error_element);
}


// void from_XML(TiXmlElement*) method

/// This method loads a final solutions error object from a XML element. 
/// @todo

void FinalSolutionsError::from_XML(TiXmlElement* final_solutions_error_element)
{
   if(final_solutions_error_element)
   { 
      // Display
      {
         TiXmlElement* display_element = final_solutions_error_element->FirstChildElement("Display");

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
