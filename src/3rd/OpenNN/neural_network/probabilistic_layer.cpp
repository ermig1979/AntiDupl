/****************************************************************************************************************/
/*                                                                                                              */
/*   OpenNN: Open Neural Networks Library                                                                       */
/*   www.opennn.cimne.com                                                                                       */
/*                                                                                                              */
/*   M U L T I L A Y E R   P E R C E P T R O N   C L A S S                                                      */
/*                                                                                                              */
/*   Roberto Lopez                                                                                              */
/*   International Center for Numerical Methods in Engineering (CIMNE)                                          */
/*   Technical University of Catalonia (UPC)                                                                    */
/*   Barcelona, Spain                                                                                           */
/*   E-mail: rlopez@cimne.upc.edu                                                                               */
/*                                                                                                              */
/****************************************************************************************************************/

// System includes

#include <cmath>   
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>

// OpenNN includes

#include "probabilistic_layer.h"

// TinyXml includes

#include "../../tinyxml/tinyxml.h"

namespace OpenNN
{

// DEFAULT CONSTRUCTOR

/// Default constructor. 
/// It creates a probabilistic layer object with zero probabilistic neurons.

ProbabilisticLayer::ProbabilisticLayer(void)
{
   set();
}


// PROBABILISTIC NEURONS NUMBER CONSTRUCTOR

/// Probabilistic neurons number constructor. 
/// It creates a probabilistic layer with a given size.
/// @param new_probabilistic_neurons_number Number of neurons in the layer. 

ProbabilisticLayer::ProbabilisticLayer(const unsigned int& new_probabilistic_neurons_number)
{
   set(new_probabilistic_neurons_number);
}


// COPY CONSTRUCTOR

/// Copy constructor. 
/// It creates a copy of an existing probabilistic layer object. 
/// @param other_probabilistic_layer Probabilistic layer to be copied.

ProbabilisticLayer::ProbabilisticLayer(const ProbabilisticLayer& other_probabilistic_layer)
{
   set(other_probabilistic_layer);
}


// DESTRUCTOR

/// Destructor.
/// This destructor does not delete any pointer. 

ProbabilisticLayer::~ProbabilisticLayer(void)
{
}


// ASSIGNMENT OPERATOR

/// Assignment operator. 
/// It assigns to this object the members of an existing probabilistic layer object.
/// @param other_probabilistic_layer Probabilistic layer object to be assigned.

ProbabilisticLayer& ProbabilisticLayer::operator = (const ProbabilisticLayer& other_probabilistic_layer)
{
   if(this != &other_probabilistic_layer) 
   {
      probabilistic_neurons_number = other_probabilistic_layer.probabilistic_neurons_number;

      probabilistic_method = other_probabilistic_layer.probabilistic_method;

      display = other_probabilistic_layer.display;
   }

   return(*this);
}


// EQUAL TO OPERATOR

// bool operator == (const ProbabilisticLayer&) const method

/// Equal to operator. 
/// It compares this object with another object of the same class. 
/// It returns true if the members of the two objects have the same values, and false otherwise.
/// @ param other_probabilistic_layer Probabilistic layer to be compared with.

bool ProbabilisticLayer::operator == (const ProbabilisticLayer& other_probabilistic_layer) const
{
   if(probabilistic_neurons_number == other_probabilistic_layer.probabilistic_neurons_number
   && probabilistic_method == other_probabilistic_layer.probabilistic_method
   && display == other_probabilistic_layer.display)
   {
      return(true);   
   }   
   else
   {
      return(false);  
   }
}


// METHODS

// const unsigned int& get_outputs_number(void) const method

/// This method returns the number of probabilistic neurons in the layer. 

const unsigned int& ProbabilisticLayer::get_probabilistic_neurons_number(void) const
{
   return(probabilistic_neurons_number);
}


// const ProbabilisticMethod& get_probabilistic_method(void) const method

/// This method returns the method to be used for interpreting the outputs as probabilistic values. 
/// The two methods available for that are Competitive and Softmax. 

const ProbabilisticLayer::ProbabilisticMethod& ProbabilisticLayer::get_probabilistic_method(void) const
{
   return(probabilistic_method);
}


// std::string write_probabilistic_method(void) const method

/// This method returns a string with the probabilistic method for the outputs ("Competitive" or "Softmax").

std::string ProbabilisticLayer::write_probabilistic_method(void) const
{
   if(probabilistic_method == Competitive)
   {
      return("Competitive");
   }
   else if(probabilistic_method == Softmax)
   {
      return("Softmax");
   }
   else
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: ProbabilisticLayer class.\n"
             << "std::string write_probabilistic_method(void) const method.\n"
             << "Unknown probabilistic method.\n";
 
	  throw std::logic_error(buffer.str());
   }
}


// const bool& get_display(void) const method

/// This method returns true if messages from this class are to be displayed on the screen, or false if messages 
/// from this class are not to be displayed on the screen.

const bool& ProbabilisticLayer::get_display(void) const
{
   return(display);
}


// void set(void) method

/// This method sets a probabilistic layer with zero probabilistic neurons.
/// It also sets the rest of members to their default values. 

void ProbabilisticLayer::set(void)
{
   probabilistic_neurons_number = 0;

   set_default();
}


// void set(const unsigned int&) method

/// This method resizes the size of the probabilistic layer. 
/// It also sets the rest of class members to their default values.
/// @param new_probabilistic_neurons_number New size for the probabilistic layer. 

void ProbabilisticLayer::set(const unsigned int& new_probabilistic_neurons_number)
{
   probabilistic_neurons_number = new_probabilistic_neurons_number;

   set_default();
}


// void set(const ProbabilisticLayer&) method

/// This method sets this object to be equal to another object of the same class.
/// @param other_probabilistic_layer Probabilistic layer object to be copied. 

void ProbabilisticLayer::set(const ProbabilisticLayer& other_probabilistic_layer)
{
   probabilistic_neurons_number = other_probabilistic_layer.probabilistic_neurons_number;

   probabilistic_method = other_probabilistic_layer.probabilistic_method;

   display = other_probabilistic_layer.display;
}


// void set_probabilistic_neurons_number(const unsigned int&) method

/// This method resizes the size of the probabilistic layer. 
/// @param new_probabilistic_neurons_number New size for the probabilistic layer. 

void ProbabilisticLayer::set_probabilistic_neurons_number(const unsigned int& new_probabilistic_neurons_number) 
{
   probabilistic_neurons_number = new_probabilistic_neurons_number;
}


// void set_default(void) method

/// This method sets the members to their default values:
/// <ul>
/// <li> Probabilistic method: Softmax.
/// <li> Display: True. 
/// </ul>

void ProbabilisticLayer::set_default(void)
{
   probabilistic_method = Softmax;
   
   display = true;
}


// void set_probablistic_method(const ProbabilisticMethod&) method

/// This method sets the chosen method for probabilistic postprocessing. 
/// Current probabilistic methods include Competitive and Softmax. 
/// @param new_probabilistic_method Method for interpreting the outputs as probabilistic values. 

void ProbabilisticLayer::set_probabilistic_method(const ProbabilisticMethod& new_probabilistic_method)
{
   probabilistic_method = new_probabilistic_method;
}


// void set_probabilistic_method(const std::string&) method

/// This method sets a new method for probabilistic processing from a string with the name. 
/// Current probabilistic methods include Competitive and Softmax. 
/// @param new_probabilistic_method Method for interpreting the outputs as probabilistic values. 

void ProbabilisticLayer::set_probabilistic_method(const std::string& new_probabilistic_method)
{
   if(new_probabilistic_method == "Competitive")
   {
      set_probabilistic_method(Competitive);
   }
   else if(new_probabilistic_method == "Softmax")
   {
      set_probabilistic_method(Softmax);
   }
   else
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: ProbabilisticLayer class.\n"
             << "void set_probabilistic_method(const std::string&) method.\n"
			 << "Unknown probabilistic method: " << new_probabilistic_method << ".\n";

	  throw std::logic_error(buffer.str());
   }
}


// void set_display(const bool&) method

/// This method sets a new display value. 
/// If it is set to true messages from this class are to be displayed on the screen;
/// if it is set to false messages from this class are not to be displayed on the screen.
/// @param new_display Display value.

void ProbabilisticLayer::set_display(const bool& new_display)
{
   display = new_display;
}


// void initialize_random(void) method

/// @todo

void ProbabilisticLayer::initialize_random(void)
{

}


// Vector<double> calculate_outputs(const Vector<double>&) const method

/// This method processes the input to the probabilistic layer in order to obtain a set of outputs which can be interpreted as probabilities. 
/// This posprocessing is performed according to the probabilistic method to be used. 
/// @param inputs Set of inputs to the probabilistic layer.

Vector<double> ProbabilisticLayer::calculate_outputs(const Vector<double>& inputs) const
{
   // Control sentence (if debug)

   #ifdef _DEBUG 

   const unsigned int size = inputs.size();

   if(size != probabilistic_neurons_number) 
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: ProbabilisticLayer class.\n"
             << "Vector<double> calculate_outputs(const Vector<double>&) const method.\n"
             << "Size must be equal to number of probabilistic neurons.\n";

	  throw std::logic_error(buffer.str());
   }   

   #endif

   switch(probabilistic_method)
   {
      case Competitive:
      {
         return(calculate_competitive_output(inputs));
      }
      break;

      case Softmax:
      {
         return(calculate_softmax_output(inputs));
      }
      break;       

      default:
      {
         std::ostringstream buffer;

         buffer << "OpenNN Exception: ProbabilisticLayer class.\n"
                << "Vector<double> calculate_outputs(const Vector<double>&) const method.\n"               
                << "Unknown probabilistic method.\n";
 
	     throw std::logic_error(buffer.str());
      }// end default
      break;

   }// end switch
}  


// Matrix<double> calculate_Jacobian(const Vector<double>&) const method

/// This method returns the partial derivatives of the outputs from the probabilistic layer with respect to its inputs,
/// depending on the probabilistic method to be used.
/// This quantity is the Jacobian matrix of the probabilistic function. 
/// @param inputs Inputs to the probabilistic layer.

Matrix<double> ProbabilisticLayer::calculate_Jacobian(const Vector<double>& inputs) const
{
   // Control sentence (if debug)

   #ifdef _DEBUG 

   const unsigned int size = inputs.size();

   if(size != probabilistic_neurons_number) 
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: ProbabilisticLayer class.\n"
             << "Matrix<double> calculate_Jacobian(const Vector<double>&) const method.\n"               
             << "Size must be equal to number of probabilistic neurons.\n";

	  throw std::logic_error(buffer.str());
   }   

   #endif

   switch(probabilistic_method)
   {
      case Competitive:
      {
         return(calculate_competitive_Jacobian(inputs));
      }
      break;

      case Softmax:
      {
          return(calculate_softmax_Jacobian(inputs));
      }
      break;       

      default:
      {
         std::ostringstream buffer;

         buffer << "OpenNN Exception: ProbabilisticLayer class.\n"
                << "Matrix<double> calculate_Jacobian(const Vector<double>&) const method.\n"               
                << "Unknown probabilistic method.\n";
 
	     throw std::logic_error(buffer.str());
      }// end default
      break;

   }// end switch
}


// Vector< Matrix<double> > calculate_Hessian_form(const Vector<double>&) const method

/// This method calculates the Hessian form of the probabilistic layer. 
/// This is a vector of matrices. 
/// The elements contain second partial derivatives of the outputs from the layer with resptect to the inputs to it.

Vector< Matrix<double> > ProbabilisticLayer::calculate_Hessian_form(const Vector<double>& inputs) const
{
   // Control sentence (if debug)

   #ifdef _DEBUG 

   const unsigned int size = inputs.size();

   if(size != probabilistic_neurons_number) 
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: ProbabilisticLayer class.\n"
             << "Matrix<double> calculate_Hessian_form(const Vector<double>&) const method.\n"               
             << "Size must be equal to number of probabilistic neurons.\n";

	  throw std::logic_error(buffer.str());
   }   

   #endif

   switch(probabilistic_method)
   {
      case Competitive:
      {
         return(calculate_competitive_Hessian_form(inputs));
      }
      break;

      case Softmax:
      {
          return(calculate_softmax_Hessian_form(inputs));
      }
      break;       

      default:
      {
         std::ostringstream buffer;

         buffer << "OpenNN Exception: ProbabilisticLayer class.\n"
                << "Matrix<double> calculate_Hessian_form(const Vector<double>&) const method.\n"               
                << "Unknown probabilistic method.\n";
 
	     throw std::logic_error(buffer.str());
      }// end default
      break;

   }// end switch
}


// Vector<double> calculate_competitive_output(const Vector<double>&) const method

/// This method returns the outputs from the layer for given inputs when the probabilistic method is the competitive. 
/// @param inputs Vector of input values to the probabilistic layer. 

Vector<double> ProbabilisticLayer::calculate_competitive_output(const Vector<double>& inputs) const
{
   return(inputs.calculate_competitive());
}


// Matrix<double> calculate_competitive_Jacobian(const Vector<double>&) const method

/// This method throws an exception, since the competitive function is not derivable. 

Matrix<double> ProbabilisticLayer::calculate_competitive_Jacobian(const Vector<double>&) const
{
   std::ostringstream buffer;

   buffer << "OpenNN Exception: ProbabilisticLayer class.\n"
          << "Matrix<double> calculate_competitive_Jacobian(const Vector<double>&) const method.\n"               
          << "The competitive function is not derivable.\n";
 
   throw std::logic_error(buffer.str());
}


// Vector< Matrix<double> > calculate_competitive_Hessian_form(const Vector<double>&) const method

/// This method throws an exception, since the competitive function is not derivable. 

Vector< Matrix<double> > ProbabilisticLayer::calculate_competitive_Hessian_form(const Vector<double>&) const
{
   std::ostringstream buffer;

   buffer << "OpenNN Exception: ProbabilisticLayer class.\n"
          << "Vector< Matrix<double> > calculate_competitive_Hessian_form(const Vector<double>&) const method.\n"               
          << "The competitive function is not derivable.\n";
 
   throw std::logic_error(buffer.str());
}


// Vector<double> calculate_softmax_output(const Vector<double>&) const method

/// This method returns the outputs of the softmax function for given inputs. 
/// @param inputs Input values to the probabilistic layer. 

Vector<double> ProbabilisticLayer::calculate_softmax_output(const Vector<double>& inputs) const
{
   return(inputs.calculate_softmax());
}


// Matrix<double> calculate_softmax_Jacobian(const Vector<double>&) const method

/// This method returns the partial derivatives of the softmax outputs with respect to the inputs. 
/// @param inputs Input values to the probabilistic layer. 

Matrix<double> ProbabilisticLayer::calculate_softmax_Jacobian(const Vector<double>& inputs) const
{
   Matrix<double> probabilistic_Jacobian(probabilistic_neurons_number, probabilistic_neurons_number);

   const Vector<double> outputs = inputs.calculate_softmax();

   for(unsigned int i = 0; i < probabilistic_neurons_number; i++)
   {
      for(unsigned int j = 0; j < probabilistic_neurons_number; j++)
      {
         if(i == j)
         {
            probabilistic_Jacobian[i][i] = outputs[i]*(1.0 - outputs[i]);
         }
         else
         {
            probabilistic_Jacobian[i][j] = -outputs[i]*outputs[j];
         }
      }         
   }         

   return(probabilistic_Jacobian);
}


// Vector< Matrix<double> > calculate_softmax_Hessian_form(const Vector<double>&) const method

/// This method returns the second partial derivatives of the softmax outputs with respect to the inputs,
/// in the so called Hessian form. 
/// @todo

Vector< Matrix<double> > ProbabilisticLayer::calculate_softmax_Hessian_form(const Vector<double>&) const
{
   Vector< Matrix<double> > Hessian_form;

   return(Hessian_form);
}


// std::string to_string(void) const method

/// This method returns a string representation of the current probabilistic layer object. 

std::string ProbabilisticLayer::to_string(void) const
{
   std::ostringstream buffer;

   buffer << "Probabilistic layer\n"
          << "Probabilistic neurons number: " << probabilistic_neurons_number << "\n"
          << "Probabilistic method: " << write_probabilistic_method() << "\n"
          << "Display: "<< display << "\n";

   return(buffer.str());
}


// TiXmlElement* to_XML(void) const method

/// This method serializes the probabilistic layer object into a XML element of the TinyXML library. 
/// See the OpenNN manual for more information about the format of this element. 

TiXmlElement* ProbabilisticLayer::to_XML(void) const
{
   std::ostringstream buffer;

   TiXmlElement* probabilistic_layer_element = new TiXmlElement("ProbabilisticLayer");
   probabilistic_layer_element->SetAttribute("Version", 4);

   // Probabilistic neurons number 
   {
      TiXmlElement* element = new TiXmlElement("ProbabilisticNeuronsNumber");
      probabilistic_layer_element->LinkEndChild(element);

      buffer.str("");
      buffer << probabilistic_neurons_number;

      TiXmlText* text = new TiXmlText(buffer.str().c_str());
      element->LinkEndChild(text);
   }

   // Probabilistic method

   {
      TiXmlElement* element = new TiXmlElement("ProbabilisticMethod");
      probabilistic_layer_element->LinkEndChild(element);

      TiXmlText* text = new TiXmlText(write_probabilistic_method().c_str());
      element->LinkEndChild(text);
   }

   // Display 
   {
      TiXmlElement* element = new TiXmlElement("Display");
      probabilistic_layer_element->LinkEndChild(element);

      buffer.str("");
      buffer << display;

      TiXmlText* text = new TiXmlText(buffer.str().c_str());
      element->LinkEndChild(text);
   }

   return(probabilistic_layer_element);
}


// void from_XML(TiXmlElement*) method

/// This method deserializes a TinyXML element into this probabilistic layer object. 
/// @param probabilistic_layer_element Pointer to a XML element containing the member data. 

void ProbabilisticLayer::from_XML(TiXmlElement* probabilistic_layer_element)
{
   if(probabilistic_layer_element)
   {
      // Probabilistic neurons number
      {
         TiXmlElement* element = probabilistic_layer_element->FirstChildElement("ProbabilisticNeuronsNumber");

         if(element)
         {
            const char* text = element->GetText();     

            if(text)
            {
               try
               {
                  set_probabilistic_neurons_number(atoi(text));
               }
               catch(std::exception& e)
               {
                  std::cout << e.what() << std::endl;		 
               }
            }
         }
      }

      // Probabilistic method
      {
         TiXmlElement* element = probabilistic_layer_element->FirstChildElement("ProbabilisticMethod");

         if(element)
         {
            const char* text = element->GetText();     

            if(text)
            {
               try
               {
                  std::string new_probabilistic_method(text);

                  set_probabilistic_method(new_probabilistic_method);
               }
               catch(std::exception& e)
               {
                  std::cout << e.what() << std::endl;		 
               }
            }
         }      
      }

      // Display
      {
         TiXmlElement* display_element = probabilistic_layer_element->FirstChildElement("Display");

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


// std::string write_competitive_expression(const Vector<std::string>&, const Vector<std::string>&) const method

/// This method returns a string with the expression of the competitive probabilistic outputs function. 
/// @param inputs_names Names of inputs to the probabilistic layer. 
/// @param outputs_names Names of outputs to the probabilistic layer. 

std::string ProbabilisticLayer::write_competitive_expression(const Vector<std::string>& inputs_names, const Vector<std::string>& outputs_names) const
{
	std::ostringstream buffer;

	for(unsigned int i = 0; i < probabilistic_neurons_number; i++)
	{
        buffer << outputs_names[i] << "= Competitive(" << inputs_names.to_string() << ");\n";
	}

	return(buffer.str());
}


// std::string write_softmax_expression(const Vector<std::string>&, const Vector<std::string>&) const method

/// This method returns a string with the expression of the softmax probabilistic outputs function. 
/// @param inputs_names Names of inputs to the probabilistic layer. 
/// @param outputs_names Names of outputs to the probabilistic layer. 

std::string ProbabilisticLayer::write_softmax_expression(const Vector<std::string>& inputs_names, const Vector<std::string>& outputs_names) const
{
	std::ostringstream buffer;

	for(unsigned int i = 0; i < probabilistic_neurons_number; i++)
	{
        buffer << outputs_names[i] << "= Softmax(" << inputs_names.to_string() << ");\n";
	}

	return(buffer.str());
}


// std::string write_expression(const Vector<std::string>&, const Vector<std::string>&) const method

/// This method returns a string with the expression of the probabilistic outputs function, 
/// depending on the probabilistic method to be used. 
/// @param inputs_names Names of inputs to the probabilistic layer. 
/// @param outputs_names Names of outputs to the probabilistic layer. 

std::string ProbabilisticLayer::write_expression(const Vector<std::string>& inputs_names, const Vector<std::string>& outputs_names) const
{	
   switch(probabilistic_method)
   {
      case Competitive:
      {   
		  return(write_competitive_expression(inputs_names, outputs_names));
      }
      break;

      case Softmax:
      {
		  return(write_softmax_expression(inputs_names, outputs_names));
      }
      break;       

      default:
      {
         std::ostringstream buffer;

         buffer << "OpenNN Exception: ProbabilisticLayer class.\n"
                << "std::string write_expression(const Vector<std::string>&, const Vector<std::string>&) const method.\n"               
                << "Unknown probabilistic method.\n";
 
	  throw std::logic_error(buffer.str());
      }// end default
      break;
   }// end switch
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
