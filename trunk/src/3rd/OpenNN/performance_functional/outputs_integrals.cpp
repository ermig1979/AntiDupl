/****************************************************************************************************************/
/*                                                                                                              */
/*   OpenNN: Open Neural Networks Library                                                                       */
/*   www.opennn.cimne.com                                                                                       */
/*                                                                                                              */
/*   O U T P U T S   I N T E G R A L S   C L A S S                                                              */
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

#include "outputs_integrals.h"

// TinyXml includes

#include "../../tinyxml/tinyxml.h"


namespace OpenNN
{

// DEFAULT CONSTRUCTOR

/// Default constructor. 
/// It creates an outputs integrals performance term not associated to any neural network.
/// It also initializes all the rest of class members to their default values.

OutputsIntegrals::OutputsIntegrals(void) 
 : PerformanceTerm()
{
   set_default();
}


// NEURAL NETWORK CONSTRUCTOR

/// Neural network constructor. 
/// It creates an outputs integrals performance term associated to a neural network.
/// It also initializes all the rest of class members to their default values.
/// @param new_neural_network_pointer Pointer to a neural network object.

OutputsIntegrals::OutputsIntegrals(NeuralNetwork* new_neural_network_pointer) 
: PerformanceTerm(new_neural_network_pointer)
{
   set_default();
}


// XML CONSTRUCTOR

/// XML constructor. 
/// It creates an outputs integrals performance term not associated to any neural network.
/// The class members are loaded from a XML element. 
/// @param outputs_integrals_element Pointer to a Tiny XML element with the object data. 

OutputsIntegrals::OutputsIntegrals(TiXmlElement* outputs_integrals_element) 
 : PerformanceTerm(outputs_integrals_element)
{
   set_default();

   from_XML(outputs_integrals_element);
}


// DESTRUCTOR

/// Destructor.

OutputsIntegrals::~OutputsIntegrals(void) 
{
}


// const NumericalIntegration& get_numerical_integration(void) const method

/// This method returns a constant reference to the numerical integration object inside the outputs integrals object. 

const NumericalIntegration& OutputsIntegrals::get_numerical_integration(void) const
{
   return(numerical_integration);
}


// NumericalIntegration* get_numerical_integration_pointer(void) method

/// This method returns a pointer to the numerical integration object inside the outputs integrals object. 

NumericalIntegration* OutputsIntegrals::get_numerical_integration_pointer(void) 
{
   return(&numerical_integration);
}


// const Vector<double>& get_outputs_integrals_weights(void) const method

/// This method returns the weights for each integral of the neural network outputs.

const Vector<double>& OutputsIntegrals::get_outputs_integrals_weights(void) const
{
   return(outputs_integrals_weights);
}


// const double& get_output_integral_weight(const unsigned int&) const method

/// This method returns the weight of a single integral of some neural network output.
/// @param i Index of neural network output. 

const double& OutputsIntegrals::get_output_integral_weight(const unsigned int& i) const
{
   return(outputs_integrals_weights[i]);
}


// void set_numerical_integration(const NumericalIntegration&) method

/// This method sets a new numerical integration object inside the outputs integral object. 
/// @param new_numerical_integration Numerical integration object. 

void OutputsIntegrals::set_numerical_integration(const NumericalIntegration& new_numerical_integration)
{
   numerical_integration = new_numerical_integration;
}


// void set_outputs_integrals_weights(const Vector<double>&) method

/// This method sets new weights for each integral of the neural network outputs.
/// @param new_outputs_integrals_weights Weight values. 

void OutputsIntegrals::set_outputs_integrals_weights(const Vector<double>& new_outputs_integrals_weights)
{
   outputs_integrals_weights = new_outputs_integrals_weights;
}


// void set_output_integral_weight(const unsigned int&, const double&) method

/// This method sets a new weight for a single integral of the neural network outputs.
/// @param i Index of output. 
/// @param new_output_integral_weight Weight value. 

void OutputsIntegrals::set_output_integral_weight(const unsigned int& i, const double& new_output_integral_weight)
{
   outputs_integrals_weights[i] = new_output_integral_weight;
}


// void set_default(void) method

/// This method sets the default values for the outputs integrals object: 
/// <ul>
/// <li> Outputs integrals weights: 1 for each neural network output. 
/// <li> Display: true.
/// </ul>

void OutputsIntegrals::set_default(void)
{
   unsigned int outputs_number = 0;

   if(neural_network_pointer)
   {
      const MultilayerPerceptron* multilayer_perceptron_pointer = neural_network_pointer->get_multilayer_perceptron_pointer();

      if(multilayer_perceptron_pointer)
	  {
         outputs_number = multilayer_perceptron_pointer->count_inputs_number();
	  }
   }

   outputs_integrals_weights.set(outputs_number, 1.0);
  
   display = true;
}


// METHODS

// void check(void) const method

/// This method checks that there are a neural network and a mathematical model associated to the outputs integrals performance term,
/// and that there is a multilayer perceptron in the neural network. 
/// It also checks that the number of inputs in the neural network is 1. 
/// If some of the above conditions is not hold, the method throws an exception. 

void OutputsIntegrals::check(void) const
{
   std::ostringstream buffer;

   // Neural network stuff

   if(!neural_network_pointer)
   {
      buffer << "OpenNN Exception: OutputsIntegrals class.\n"
             << "void check(void) const method.\n"
             << "Pointer to neural network is NULL.\n";

      throw std::logic_error(buffer.str().c_str());	  
   }

   const MultilayerPerceptron* multilayer_perceptron_pointer = neural_network_pointer->get_multilayer_perceptron_pointer();

   if(!multilayer_perceptron_pointer)
   {
      buffer << "OpenNN Exception: OutputsIntegrals class.\n"
             << "void check(void) const method.\n"
             << "Pointer to multilayer perceptron is NULL.\n";

      throw std::logic_error(buffer.str().c_str());	  
   }

   const unsigned int inputs_number = multilayer_perceptron_pointer->count_inputs_number();
   const unsigned int outputs_number = multilayer_perceptron_pointer->count_outputs_number();

   if(inputs_number != 1)
   {
      buffer << "OpenNN Exception: OutputsIntegrals class.\n"
             << "void check(void) const method.\n"
             << "Number of inputs in multilayer perceptron is not one.\n";

      throw std::logic_error(buffer.str().c_str());	  
   }

   if(outputs_number == 0)
   {
      buffer << "OpenNN Exception: OutputsIntegrals class.\n"
             << "void check(void) const method.\n"
             << "Number of outputs in multilayer perceptron object is zero.\n";

      throw std::logic_error(buffer.str().c_str());	  
   }
}


// double calculate_evaluation(void) const method

/// This method returns the objective value of a multilayer perceptron according to the sum squared error on a data set.
/// @todo

double OutputsIntegrals::calculate_evaluation(void) const
{
   std::ostringstream buffer;

   buffer << "OpenNN Exception: OutputsIntegrals class.\n"
          << "double calculate_evaluation(void) const method.\n"
          << "This method is under development.\n";

   throw std::logic_error(buffer.str().c_str());	  

/*
   // Control sentence

   #ifdef _DEBUG 

   check();

   #endif

   // Neural network stuff 

   const MultilayerPerceptron* multilayer_perceptron_pointer = neural_network_pointer->get_multilayer_perceptron_pointer();

   const unsigned int outputs_number = multilayer_perceptron_pointer->count_outputs_number();

   // Outputs integrals

   double evaluation = 0;

   for(unsigned int i = 0; i < outputs_number; i++)
   {
      evaluation += 0.0;
   }

   return(evaluation);
*/
}


// double calculate_evaluation(const Vector<double>&) const method

/// This method returns which would be the evaluation of a neural network for an hypothetical vector of parameters. 
/// It does not set that vector of parameters to the neural network. 
// @param parameters Vector of a potential parameters for the neural network associated to the performance functional.

double OutputsIntegrals::calculate_evaluation(const Vector<double>&) const
{
   std::ostringstream buffer;

   buffer << "OpenNN Exception: OutputsIntegrals class.\n"
          << "double calculate_evaluation(const Vector<double>&) const method.\n"
          << "This method is under development.\n";

   throw std::logic_error(buffer.str().c_str());	  
/*  
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

      buffer << "OpenNN Exception: OutputsIntegrals class." << std::endl
             << "double calculate_evaluation(const Vector<double>&) const method." << std::endl
             << "Size (" << size << ") must be equal to number of parameters (" << parameters_number << ")." << std::endl;

      throw std::logic_error(buffer.str().c_str());
   }

   #endif

   NeuralNetwork neural_network_copy(*neural_network_pointer);

   neural_network_copy.set_parameters(parameters);

   OutputsIntegrals sum_squared_error_copy(*this);

   sum_squared_error_copy.set_neural_network_pointer(&neural_network_copy);

   return(sum_squared_error_copy.calculate_evaluation());
*/
}


// Vector<double> calculate_gradient(void) const method

/// This method calculates the objective gradient by means of the back-propagation algorithm, 
/// and returns it in a single vector of size the number of multilayer_perceptron_pointer parameters. 
/// @todo

Vector<double> OutputsIntegrals::calculate_gradient(void) const
{
   // Neural network stuff

   #ifdef _DEBUG 

   check();

   #endif

   Vector<double> gradient;

   return(gradient);
}


// Matrix<double> calculate_Hessian(void) const method

/// This method calculates the objective Hessian by means of the back-propagation algorithm, 
/// and returns it in a single symmetric matrix of size the number of multilayer_perceptron_pointer parameters. 
/// @todo

Matrix<double> OutputsIntegrals::calculate_Hessian(void) const
{
   // Neural network stuff

   #ifdef _DEBUG 

   check();

   #endif
    
   Matrix<double> Hessian;

   return(Hessian);   
}


// std::string write_performance_term_type(void) const method

/// This method returns a string with the name of the outputs integrals performance type, "OUTPUTS_INTEGRALS".

std::string OutputsIntegrals::write_performance_term_type(void) const
{
   return("OUTPUTS_INTEGRALS");
}


// TiXmlElement* to_XML(void) method method 

/// This method returns a representation of the sum squared error object, in XML format. 

TiXmlElement* OutputsIntegrals::to_XML(void) const
{
   std::ostringstream buffer;

   // Nueral network outputs integrals

   TiXmlElement* outputs_integrals_element = new TiXmlElement("OutputsIntegrals");
   outputs_integrals_element->SetAttribute("Version", 4);

   // Numerical differentiation
   
   if(numerical_differentiation_pointer)
   {
      TiXmlElement* element = numerical_differentiation_pointer->to_XML();
      outputs_integrals_element->LinkEndChild(element);
   }

   // Outputs integrals weights
   {
      TiXmlElement* element = new TiXmlElement("OutputsIntegralsWeights");
      outputs_integrals_element->LinkEndChild(element);

      buffer.str("");
      buffer << outputs_integrals_weights;

      TiXmlText* text = new TiXmlText(buffer.str().c_str());
      element->LinkEndChild(text);
   }

   // Display
   {
      TiXmlElement* element = new TiXmlElement("Display");
      outputs_integrals_element->LinkEndChild(element);

      buffer.str("");
      buffer << display;

      TiXmlText* text = new TiXmlText(buffer.str().c_str());
      element->LinkEndChild(text);
   }

   return(outputs_integrals_element);
}


// void from_XML(TiXmlElement*) method

/// This method loads a sum squared error object from a XML element. 
// @param sum_squared_error_element Tiny XML element containing the members of a sum squared error element. 

void OutputsIntegrals::from_XML(TiXmlElement* outputs_integrals_element)
{
   if(outputs_integrals_element)
   { 
      // Display
      {
         TiXmlElement* display_element = outputs_integrals_element->FirstChildElement("Display");

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
