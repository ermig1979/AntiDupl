/****************************************************************************************************************/
/*                                                                                                              */
/*   OpenNN: Open Neural Networks Library                                                                       */
/*   www.opennn.cimne.com                                                                                       */
/*                                                                                                              */
/*   N E U R A L   P A R A M E T E R S   N O R M   C L A S S                                                    */
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

#include "neural_parameters_norm.h"

// TinyXml includes

#include "../../tinyxml/tinyxml.h"


namespace OpenNN
{

// DEFAULT CONSTRUCTOR

/// Default constructor. 
/// It creates a neural parameters norm functional not associated to any neural network.
/// It also initializes all the rest of class members to their default values.

NeuralParametersNorm::NeuralParametersNorm(void) 
 : PerformanceTerm()
{
   set_default();
}


// NEURAL NETWORK CONSTRUCTOR

/// Neural network constructor. 
/// It creates a neural parameters norm functional associated to a neural network.
/// It also initializes all the rest of class members to their default values.
/// @param new_neural_network_pointer Pointer to a neural network object.

NeuralParametersNorm::NeuralParametersNorm(NeuralNetwork* new_neural_network_pointer) 
: PerformanceTerm(new_neural_network_pointer)
{
   set_default();
}


// XML CONSTRUCTOR

/// XML constructor. 
/// It creates a neural parameters norm object not associated to any neural network.
/// The object members are loaded by means of a XML element.
/// Please be careful with the format of that file, which is specified in the OpenNN manual.
/// @param neural_parameters_norm_element Tiny XML element with the neural parameters norm elements.

NeuralParametersNorm::NeuralParametersNorm(TiXmlElement* neural_parameters_norm_element) 
 : PerformanceTerm()
{
   set_default();

   from_XML(neural_parameters_norm_element);
}


// DESTRUCTOR

/// Destructor.
/// This destructor does not delete any pointer. 

NeuralParametersNorm::~NeuralParametersNorm(void) 
{
}


// METHODS

// const double& get_neural_parameters_norm_weight(void) const method

/// This method returns the weight value for the neural parameters norm in the performance term expression. 

const double& NeuralParametersNorm::get_neural_parameters_norm_weight(void) const
{
   return(neural_parameters_norm_weight);
}


// void set_neural_parameters_norm_weight(const double&) method

/// This method sets a new weight value for the neural parameters norm in the performance term expression. 

void NeuralParametersNorm::set_neural_parameters_norm_weight(const double& new_neural_parameters_norm_weight)
{
   neural_parameters_norm_weight = new_neural_parameters_norm_weight;
}


// void set_default(void) method

/// This method sets the default values for the neural parameters norm object:
/// <ul>
/// <li> Neural parameters norm weight: 0.1.
/// <li> Display: true.
/// </ul>

void NeuralParametersNorm::set_default(void)
{
   neural_parameters_norm_weight = 0.01;

   display = true;
}


// void check(void) const method

/// This method checks that there is a neural network associated to this performance term,
/// and that there is a multilayer perceptron in the neural network. 
/// If some of the above conditions is not hold, the method throws an exception. 

void NeuralParametersNorm::check(void) const
{
   std::ostringstream buffer;

   // Neural network stuff

   if(!neural_network_pointer)
   {
      buffer << "OpenNN Exception: NeuralParametersNorm class.\n"
             << "void check(void) const method.\n"
             << "Pointer to neural network is NULL.\n";

      throw std::logic_error(buffer.str().c_str());	  
   }

   const MultilayerPerceptron* multilayer_perceptron_pointer = neural_network_pointer->get_multilayer_perceptron_pointer();

   if(!multilayer_perceptron_pointer)
   {
      buffer << "OpenNN Exception: NeuralParametersNorm class.\n"
             << "void check(void) const method.\n"
             << "Pointer to multilayer perceptron is NULL.\n";

      throw std::logic_error(buffer.str().c_str());	  
   }

   const unsigned int inputs_number = multilayer_perceptron_pointer->count_inputs_number();
   const unsigned int outputs_number = multilayer_perceptron_pointer->count_outputs_number();

   if(inputs_number == 0)
   {
      buffer << "OpenNN Exception: NeuralParametersNorm class.\n"
             << "void check(void) const method.\n"
             << "Number of inputs in multilayer perceptron object is zero.\n";

      throw std::logic_error(buffer.str().c_str());	  
   }

   if(outputs_number == 0)
   {
      buffer << "OpenNN Exception: NeuralParametersNorm class.\n"
             << "void check(void) const method.\n"
             << "Number of outputs in multilayer perceptron object is zero.\n";

      throw std::logic_error(buffer.str().c_str());	  
   }

}


// double calculate_evaluation(void) const method

/// This method returns the evaluation of this peformance term. 
/// It is equal to the weighted norm of the parameters from the associated neural network.

double NeuralParametersNorm::calculate_evaluation(void) const
{
   #ifdef _DEBUG 

   check();

   #endif

   const MultilayerPerceptron* multilayer_perceptron_pointer = neural_network_pointer->get_multilayer_perceptron_pointer();

   const Vector<double> multilayer_perceptron_parameters = multilayer_perceptron_pointer->arrange_parameters();

   return(multilayer_perceptron_parameters.calculate_norm());
}


// double calculate_evaluation(const Vector<double>&) const method

/// This method returns the neural parameters norm value of a neural network for a vector of parameters. 
/// It does not set that vector of parameters to the neural network. 
/// @param parameters Vector of parameters for the neural network associated to the performance term.

double NeuralParametersNorm::calculate_evaluation(const Vector<double>& parameters) const
{
   // Control sentence (if debug)

   #ifdef _DEBUG 

   check();

   #endif

   return(parameters.calculate_norm());
}


// Vector<double> calculate_gradient(void) const method

/// This method calculates the objective gradient by means of the back-propagation algorithm, 
/// and returns it in a single vector of size the number of multilayer_perceptron_pointer parameters. 

Vector<double> NeuralParametersNorm::calculate_gradient(void) const
{
   // Control sentence (if debug)

   #ifdef _DEBUG 

   check();

   #endif

   const Vector<double> parameters = neural_network_pointer->arrange_parameters();

   const double parameters_norm = parameters.calculate_norm();

   return(parameters/parameters_norm);
}


// Matrix<double> calculate_Hessian(void) const method

/// This method calculates the objective Hessian by means of the back-propagation algorithm, 
/// and returns it in a single symmetric matrix of size the number of multilayer_perceptron_pointer parameters. 

Matrix<double> NeuralParametersNorm::calculate_Hessian(void) const
{
   // Neural network stuff

   #ifdef _DEBUG 

   check();

   #endif

   const Vector<double> parameters = neural_network_pointer->arrange_parameters();

//   const unsigned int parameters_number = parameters.size();

   const double parameters_norm = parameters.calculate_norm();

   const Matrix<double> Hessian = parameters.direct(parameters)/pow(parameters_norm, 3);

   return(Hessian);   
}


// std::string write_performance_term_type(void) const method

/// This method returns a string with the name of the neural parameters norm performance type, "NEURAL_PARAMETERS_NORM".

std::string NeuralParametersNorm::write_performance_term_type(void) const
{
   return("NEURAL_PARAMETERS_NORM");
}


// std::string write_information(void) const method

std::string NeuralParametersNorm::write_information(void) const
{
   std::ostringstream buffer;
   
   buffer << "Neural parameters norm: " << calculate_evaluation() << "\n";

   return(buffer.str());
}


// TiXmlElement* to_XML(void) method method 

/// This method returns a representation of the sum squared error object, in XML format. 

TiXmlElement* NeuralParametersNorm::to_XML(void) const
{
   std::ostringstream buffer;

   // Neural parameters norm

   TiXmlElement* neural_network_parameters_norm_element = new TiXmlElement("NeuralParametersNorm");
   neural_network_parameters_norm_element->SetAttribute("Version", 4);

   // Neural parameters norm weight
   {
      TiXmlElement* weight_element = new TiXmlElement("NeuralParametersNormWeight");
      neural_network_parameters_norm_element->LinkEndChild(weight_element);

      buffer.str("");
      buffer << neural_parameters_norm_weight;

      TiXmlText* weight_text = new TiXmlText(buffer.str().c_str());
      weight_element->LinkEndChild(weight_text);
   }

   // Display

   {
      TiXmlElement* display_element = new TiXmlElement("Display");
      neural_network_parameters_norm_element->LinkEndChild(display_element);

      buffer.str("");
      buffer << display;

      TiXmlText* display_text = new TiXmlText(buffer.str().c_str());
      display_element->LinkEndChild(display_text);
   }

   return(neural_network_parameters_norm_element);
}


// void from_XML(TiXmlElement*) method

/// This method loads a sum squared error object from a XML element. 
// @param sum_squared_error_element Tiny XML element containing the members of a sum squared error element. 

void NeuralParametersNorm::from_XML(TiXmlElement* neural_parameters_norm_element)
{
   if(neural_parameters_norm_element)
   { 
      // Neural parameters norm weight
      {
         TiXmlElement* element = neural_parameters_norm_element->FirstChildElement("NeuralParametersNormWeight");

         if(element)
         {
            try
            {
               const double new_neural_parameters_norm_weight = atof(element->GetText());     

               set_neural_parameters_norm_weight(new_neural_parameters_norm_weight);
            }
            catch(std::exception& e)
            {
               std::cout << e.what() << std::endl;		 
            }
         }
      }

      // Display
      {
         TiXmlElement* element = neural_parameters_norm_element->FirstChildElement("Display");

         if(element)
         {
            try
            {
               std::string new_display_string = element->GetText();     

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
