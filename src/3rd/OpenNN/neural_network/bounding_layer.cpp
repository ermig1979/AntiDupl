/****************************************************************************************************************/
/*                                                                                                              */
/*   OpenNN: Open Neural Networks Library                                                                       */
/*   www.opennn.cimne.com                                                                                       */
/*                                                                                                              */
/*   B O U N D I N G   L A Y E R   C L A S S                                                                    */
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

#include "bounding_layer.h"

// TinyXml includes

#include "../../tinyxml/tinyxml.h"

namespace OpenNN
{

// DEFAULT CONSTRUCTOR

/// Default constructor. 
/// It creates a bounding layer object with zero bounding neurons.

BoundingLayer::BoundingLayer(void)
{
   set();
}


// BOUNDING NEURONS NUMBER CONSTRUCTOR

/// Bounding neurons number constructor. 
/// It creates a bounding layer with a given size. 
/// @param bounding_neurons_number Number of bounding neurons in the layer. 

BoundingLayer::BoundingLayer(const unsigned int& bounding_neurons_number)
{
   set(bounding_neurons_number);

   set_default();
}


// XML CONSTRUCTOR

/// XML constructor. 
/// It creates a bounding layer and loads its members from a XML element. 
/// @param bounding_layer_element TinyXML element with the member data. 

BoundingLayer::BoundingLayer(TiXmlElement* bounding_layer_element)
{
   set(bounding_layer_element);
}


// COPY CONSTRUCTOR

/// Copy constructor. 
/// It creates a copy of an existing bounding layer object. 
/// @param other_bounding_layer Bounding layer to be copied.

BoundingLayer::BoundingLayer(const BoundingLayer& other_bounding_layer)
{
   set(other_bounding_layer);
}


// DESTRUCTOR

/// Destructor.
/// This destructor does not delete any pointer. 

BoundingLayer::~BoundingLayer(void)
{
}


// ASSIGNMENT OPERATOR

/// Assignment operator. 
/// It assigns to this object the members of an existing bounding layer object.
/// @param other_bounding_layer Bounding layer object to be assigned.

BoundingLayer& BoundingLayer::operator = (const BoundingLayer& other_bounding_layer)
{
   if(this != &other_bounding_layer) 
   {
      lower_bounds = other_bounding_layer.lower_bounds;
      upper_bounds = other_bounding_layer.upper_bounds;
      display = other_bounding_layer.display;
   }

   return(*this);
}


// EQUAL TO OPERATOR

// bool operator == (const BoundingLayer&) const method

/// Equal to operator. 
/// It compares this object with another object of the same class. 
/// It returns true if the members of the two objects have the same values, and false otherwise.
/// @ param other_bounding_layer Bounding layer to be compared with.

bool BoundingLayer::operator == (const BoundingLayer& other_bounding_layer) const
{
    if(lower_bounds == other_bounding_layer.lower_bounds
    && upper_bounds == other_bounding_layer.upper_bounds
    && display == other_bounding_layer.display)
    {
       return(true);
    }
    else
    {
       return(false);
    }
}


// bool is_empty(void) const method

/// This method returns true if the size of the layer is zero, and false otherwise. 

bool BoundingLayer::is_empty(void) const
{
   if(count_bounding_neurons_number() == 0)
   {
      return(true);
   }
   else
   {
      return(false);
   }
}


// unsigned int count_bounding_neurons_number(void) const method

/// This method returns the number of bounding neurons in the layer. 

unsigned int BoundingLayer::count_bounding_neurons_number(void) const
{
   return(lower_bounds.size());
}


// const Vector<double>& get_lower_bounds(void) const method

/// This method returns the lower bounds values of all the bounding neurons in the layer. 

const Vector<double>& BoundingLayer::get_lower_bounds(void) const
{
   return(lower_bounds);               
}


// double get_lower_bound(const unsigned int&) const const method

/// This method returns the lower bound value of a single bounding neuron.
/// @param i Index of bounding neuron. 

double BoundingLayer::get_lower_bound(const unsigned int& i) const
{
   // Control sentence (if debug)

   #ifdef _DEBUG 

   const unsigned int bounding_neurons_number = count_bounding_neurons_number();

   if(i >= bounding_neurons_number)
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: BoundingLayer class.\n" 
             << "double get_lower_bound(const unsigned int&) const method.\n"
             << "Index must be less than number of bounding neurons.\n";

	  throw std::logic_error(buffer.str());
   }

   #endif

   return(lower_bounds[i]);
}


// const Vector<double>& get_upper_bound(void) const method

/// This method returns the upper bounds values of all the bounding neurons in the layer.

const Vector<double>& BoundingLayer::get_upper_bounds(void) const
{
   return(upper_bounds);               
}


// double get_upper_bound(const unsigned int&) const method

/// This method returns the upper bound value of a single bounding neuron. 
/// @param i Index of bounding neuron. 

double BoundingLayer::get_upper_bound(const unsigned int& i) const
{
   // Control sentence (if debug)

   #ifdef _DEBUG 

   const unsigned int bounding_neurons_number = count_bounding_neurons_number();

   if(bounding_neurons_number == 0)
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: BoundingLayer class.\n" 
             << "double get_upper_bound(const unsigned int&) const method.\n"
             << "Number of bounding neurons is zero.\n";

	  throw std::logic_error(buffer.str());
   }
   else if(i >= bounding_neurons_number)
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: BoundingLayer class.\n" 
             << "double get_upper_bound(const unsigned int&) const method.\n"
             << "Index must be less than number of bounding neurons.\n";

	  throw std::logic_error(buffer.str());
   }

   #endif

   return(upper_bounds[i]);
}


// Vector< Vector<double>* > get_bounds(void) method

/// This method returns the lower bounds and the upper bounds of all the bounding neurons. 
/// The format is a vector of pointers to vectors of size two. 
/// The first element contains the lower bound values.
/// The second element contains the upper bound values.

Vector< Vector<double>* > BoundingLayer::get_bounds(void)
{
   Vector< Vector<double>* > bounds(2);

   bounds[0] = &lower_bounds;
   bounds[1] = &upper_bounds;

   return(bounds);
}


// void set(void)

/// This method sets the number of bounding neurons to be zero. 
/// It also sets the rest of memebers to their default values. 

void BoundingLayer::set(void)
{
   lower_bounds.set();
   upper_bounds.set();

   set_default();
}


// void set(const unsigned int&)

/// This method resizes the bounding layer. 
/// It also sets the rest of memebers to their default values. 
/// @param new_bounding_neurons_number Size of the bounding layer. 

void BoundingLayer::set(const unsigned int& new_bounding_neurons_number)
{
   lower_bounds.set(new_bounding_neurons_number);
   upper_bounds.set(new_bounding_neurons_number);

   set_default();
}


// void set(TiXmlElement*)

/// This method sets the bounding layer members from a XML element. 
/// @param bounding_layer_element Pointer to a Tiny XML element containing the member data. 

void BoundingLayer::set(TiXmlElement* bounding_layer_element)
{
   from_XML(bounding_layer_element);
}


// void set(const BoundingLayer&)

/// This method sets the members of this object to be the members of another object of the same class. 
/// @param other_bounding_layer Object to be copied. 

void BoundingLayer::set(const BoundingLayer& other_bounding_layer)
{
   lower_bounds = other_bounding_layer.lower_bounds;

   upper_bounds = other_bounding_layer.upper_bounds;

   display = other_bounding_layer.display;
}


// void set_lower_bound(const Vector<double>&) method

/// This method sets new lower bounds for all the neurons in the layer. 
/// @param new_lower_bounds New set of lower bounds for the bounding neurons. 

void BoundingLayer::set_lower_bounds(const Vector<double>& new_lower_bounds)
{
   // Control sentence (if debug)

   #ifdef _DEBUG 

   unsigned int bounding_neurons_number = count_bounding_neurons_number();

   if(new_lower_bounds.size() != bounding_neurons_number)
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: BoundingLayer class.\n"
             << "void set_lower_bounds(const Vector<double>&) method.\n"
             << "Size must be equal to number of bounding neurons number.\n";

	  throw std::logic_error(buffer.str());
   }

   #endif

   // Set lower bound of bounding neurons

   lower_bounds = new_lower_bounds;
}


// void set_lower_bound(const unsigned int&, const double&) method

/// This method sets a new lower bound for a single neuron.
/// This value is used for unscaling that variable so that it is not less than the lower bound. 
/// @param index Index of bounding neuron.
/// @param new_lower_bound New lower bound for the neuron with index i.

void BoundingLayer::set_lower_bound(const unsigned int& index, const double& new_lower_bound)
{
   const unsigned int bounding_neurons_number = count_bounding_neurons_number();

   // Control sentence (if debug)

   #ifdef _DEBUG 

   if(index >= bounding_neurons_number)
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: BoundingLayer class.\n"
             << "void set_lower_bound(const unsigned int&, const double&) method.\n"
             << "Index of bounding neurons must be less than number of bounding neurons.\n";

	  throw std::logic_error(buffer.str());
   }

   #endif

   if(lower_bounds.size() != bounding_neurons_number)
   {
      lower_bounds.set(bounding_neurons_number, -1.0e99);
   }

   // Set lower bound of single neuron

   lower_bounds[index] = new_lower_bound;
}


// void set_upper_bounds(const Vector<double>&) method

/// This method sets new upper bounds for all the bounding neurons.
/// These values are used for unscaling variables so that they are not greater than the upper bounds. 
/// @param new_upper_bounds New set of upper bounds for the layer.

void BoundingLayer::set_upper_bounds(const Vector<double>& new_upper_bounds)
{
   // Control sentence (if debug)

   #ifdef _DEBUG 

   const unsigned int bounding_neurons_number = count_bounding_neurons_number();

   if(new_upper_bounds.size() != bounding_neurons_number)
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: BoundingLayer class.\n"
             << "void set_upper_bound(const Vector<double>&) method.\n"
             << "Size must be equal to number of bounding neurons.\n";

	  throw std::logic_error(buffer.str());
   }

   #endif

   // Set upper bound of neurons

   upper_bounds = new_upper_bounds;
}


// void set_upper_bound(const unsigned int&, const double&) method

/// This method sets a new upper bound for a single neuron.
/// This value is used for unscaling that variable so that it is not greater than the upper bound. 
/// @param index Index of bounding neuron.
/// @param new_upper_bound New upper bound for the bounding neuron with that index.

void BoundingLayer::set_upper_bound(const unsigned int& index, const double& new_upper_bound)
{
   const unsigned int bounding_neurons_number = count_bounding_neurons_number();

   // Control sentence (if debug)

   #ifdef _DEBUG 

   if(index >= bounding_neurons_number)
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: BoundingLayer class.\n"
             << "void set_upper_bound(const unsigned int&, const double&) method.\n"
             << "Index of bounding neuron must be less than number of bounding neurons.\n";

	  throw std::logic_error(buffer.str());
   }

   #endif

   if(upper_bounds.size() != bounding_neurons_number)
   {
      upper_bounds.set(bounding_neurons_number, 1.0e99);
   }

   // Set upper bound of single bounding neuron

   upper_bounds[index] = new_upper_bound;
}


// void set_bounds(const Vector< Vector<double> >&) method

/// This method sets both the lower bounds and the upper bounds of all the neurons in the layer.
/// The format is a vector of two real vectors.
/// The first element must contain the lower bound values for the bounding neurons.
/// The second element must contain the upper bound values for the bounding neurons.
/// These values are used for unscaling variables so that they are neither less than the lower bounds nor greater than the upper bounds. 
/// @param new_bounds New set of lower and upper bounds.

void BoundingLayer::set_bounds(const Vector< Vector<double> >& new_bounds)
{
   // Control sentence (if debug)

   #ifdef _DEBUG 

   const unsigned int size = new_bounds.size();

   const unsigned int bounding_neurons_number = count_bounding_neurons_number();

   if(size != 2)
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: BoundingLayer class.\n"
             << "void set_bounds(const Vector< Vector<double> >&) method.\n"
             << "Number of rows must be 2.\n";

	  throw std::logic_error(buffer.str());
   }
   else if(new_bounds[0].size() != bounding_neurons_number
        && new_bounds[1].size() != bounding_neurons_number)
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: BoundingLayer class.\n"
             << "void set_bounds(const Vector< Vector<double> >&) method.\n"
             << "Number of columns must be equal to number of bounding neurons.\n";

	  throw std::logic_error(buffer.str());
   }

   #endif

   // Set lower and upper bounds of bounding neurons

   set_lower_bounds(new_bounds[0]);
   set_upper_bounds(new_bounds[1]);
}


// void set_display(const bool&) method

/// This method sets a new display value. 
/// If it is set to true messages from this class are to be displayed on the screen;
/// if it is set to false messages from this class are not to be displayed on the screen.
/// @param new_display Display value.

void BoundingLayer::set_display(const bool& new_display)
{
   display = new_display;
}


// void set_default(void) method

/// This method sets the members to their default values:
/// <ul>
/// <li> Display: True. 
/// </ul>

void BoundingLayer::set_default(void)
{
   display = true;        
}


// void initialize_random(void) method

/// @todo

void BoundingLayer::initialize_random(void)
{

}


// Vector<double> calculate_outputs(const Vector<double>&) const method

/// This method calculates the outputs from the bounding layer for a set of inputs to that layer. 
/// @param inputs Set of inputs to the bounding layer.

Vector<double> BoundingLayer::calculate_outputs(const Vector<double>& inputs) const
{
   // Control sentence (if debug)

   #ifdef _DEBUG 

   const unsigned int inputs_size = inputs.size();

   const unsigned int bounding_neurons_number = count_bounding_neurons_number();

   if(inputs_size != bounding_neurons_number) 
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: BoundingLayer class.\n"
             << "Vector<double> calculate_outputs(const Vector<double>&) const method.\n"
             << "Size of inputs must be equal to number of bounding neurons.\n";

	  throw std::logic_error(buffer.str());
   }   

   #endif

   return(inputs.calculate_lower_upper_bounded(lower_bounds, upper_bounds));
}  


// Vector<double> calculate_derivative(const Vector<double>&) const method

/// This method returns the derivatives of the outputs with respect to the inputs. 
/// @param inputs Set of input values to the bounding layer. 

Vector<double> BoundingLayer::calculate_derivative(const Vector<double>& inputs) const
{
   const unsigned int bounding_neurons_number = count_bounding_neurons_number();

   const Vector<double> outputs = calculate_outputs(inputs);

   Vector<double> derivatives(bounding_neurons_number);

   for(unsigned int i = 0; i < bounding_neurons_number; i++)
   {
      if(outputs[i] <= lower_bounds[i] || outputs[i] >= upper_bounds[i])
      {           
         derivatives[i] = 0.0;
	  }
      else
      {
         derivatives[i] = 1.0;
      }
   }

   return(derivatives);
}


// Vector<double> calculate_second_derivative(const Vector<double>&) const method

/// This method returns the second derivatives of the outputs with respect to the inputs. 
/// @param inputs Set of input values to the bounding layer. 

Vector<double> BoundingLayer::calculate_second_derivative(const Vector<double>& inputs) const
{
   std::ostringstream buffer;

   const unsigned int bounding_neurons_number = count_bounding_neurons_number();

   const Vector<double> outputs = calculate_outputs(inputs);

   for(unsigned int i = 0; i < bounding_neurons_number; i++)
   {
      if(outputs[i] == lower_bounds[i])
      {
         buffer << "OpenNN Exception: BoundingLayer class.\n"
                << "Vector<double> calculate_outputs(const Vector<double>&) const method.\n"
                << "Output is equal to lower bound. The bounding function is not differentiable at this point.\n";

	     throw std::logic_error(buffer.str());
      }
      else if(outputs[i] == upper_bounds[i])
      {
         buffer << "OpenNN Exception: BoundingLayer class.\n"
                << "Vector<double> calculate_outputs(const Vector<double>&) const method.\n"
                << "Output is equal to upper bound. The bounding function is not differentiable at this point.\n";

	     throw std::logic_error(buffer.str());
      }
   }

   Vector<double> second_derivative(bounding_neurons_number, 0.0);

   return(second_derivative);
}


// Matrix<double> arrange_Jacobian(const Vector<double>&) const method

/// This method arranges a "Jacobian matrix" from a vector of derivatives. 
/// The Jacobian matrix is composed of the partial derivatives of the layer outputs with respect to the layer inputs. 
/// @param derivatives Vector of outputs-inputs derivatives of each bounding neuron. 

Matrix<double> BoundingLayer::arrange_Jacobian(const Vector<double>& derivatives) const
{   
   const unsigned int bounding_neurons_number = count_bounding_neurons_number();

   // Control sentence (if debug)

   #ifdef _DEBUG 

   const unsigned int derivatives_size = derivatives.size();

   if(derivatives_size != bounding_neurons_number) 
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: BoundingLayer class.\n"
             << "Matrix<double> arrange_Jacobian(const Vector<double>&) method.\n"
             << "Size of derivatives must be equal to number of bounding neurons.\n";

	  throw std::logic_error(buffer.str());
   }   

   #endif

   Matrix<double> Jacobian(bounding_neurons_number, bounding_neurons_number, 0.0);
   Jacobian.set_diagonal(derivatives);

   return(Jacobian);
}  


// Vector< Matrix<double> > arrange_Hessian_form(const Vector<double>&) const method

/// This method arranges a "Hessian form" vector of matrices from a vector of derivatives. 
/// The Hessian form is composed of the second partial derivatives of the layer outputs with respect to the layer inputs. 

Vector< Matrix<double> > BoundingLayer::arrange_Hessian_form(const Vector<double>&) const
{
   const unsigned int bounding_neurons_number = count_bounding_neurons_number();

   Vector< Matrix<double> > bounded_Hessian_form(bounding_neurons_number);

   for(unsigned int i = 0; i < bounding_neurons_number; i++)
   {
      bounded_Hessian_form[i].set(bounding_neurons_number, bounding_neurons_number, 0.0);
   }

   return(bounded_Hessian_form);
}


// std::string write_expression(const Vector<std::string>&, const Vector<std::string>&) const method

/// This method returns a string with the expression of the lower and upper bounds functions. 

std::string BoundingLayer::write_expression(const Vector<std::string>& inputs_name, const Vector<std::string>& outputs_name) const
{
   std::stringstream expression;
   
   const unsigned int bounding_neurons_number = count_bounding_neurons_number();

   for(unsigned int i = 0; i < bounding_neurons_number; i++)
   {
      expression << outputs_name[i] << " < " << lower_bounds[i] << " ? " << lower_bounds[i] << " : " << inputs_name[i] << "\n";      
      expression << outputs_name[i] << " > " << upper_bounds[i] << " ? " << upper_bounds[i] << " : " << inputs_name[i] << "\n";      
   }

   return(expression.str());
}


// std::string to_string(void) const method

/// This method returns a string representation of the current bonding layer object. 

std::string BoundingLayer::to_string(void) const
{
   std::ostringstream buffer;

   buffer << "Bounding layer\n"  
          << "Lower bounds: " << lower_bounds << "\n"
          << "Upper bounds: " << upper_bounds << "\n"
          << "Display: " << display << "\n";

   return(buffer.str());
}


// TiXmlElement* to_XML(void) const method

/// This method serializes the bounding layer object into a XML element of the TinyXML library. 
/// See the OpenNN manual for more information about the format of this element. 

TiXmlElement* BoundingLayer::to_XML(void) const
{
   std::ostringstream buffer;

   TiXmlElement* bounding_layer_element = new TiXmlElement("BoundingLayer");
   bounding_layer_element->SetAttribute("Version", 4);

   // Lower bounds

   {
      TiXmlElement* element = new TiXmlElement("LowerBounds");
      bounding_layer_element->LinkEndChild(element);

      buffer.str("");
      buffer << lower_bounds;

      TiXmlText* text = new TiXmlText(buffer.str().c_str());
      element->LinkEndChild(text);
   }

   // Upper bounds

   {
      TiXmlElement* element = new TiXmlElement("UpperBounds");
      bounding_layer_element->LinkEndChild(element);

      buffer.str("");
      buffer << upper_bounds;

      TiXmlText* text = new TiXmlText(buffer.str().c_str());
      element->LinkEndChild(text);
   }

   // Display

   {
      TiXmlElement* display_element = new TiXmlElement("Display");
      bounding_layer_element->LinkEndChild(display_element);

      buffer.str("");
      buffer << display;

      TiXmlText* display_text = new TiXmlText(buffer.str().c_str());
      display_element->LinkEndChild(display_text);
   }

   return(bounding_layer_element);
}


// void from_XML(TiXmlElement*) method

/// This method deserializes a TinyXML element into this bounding layer object. 
/// @param bounding_layer_element Pointer to a XML element containing the member data. 

void BoundingLayer::from_XML(TiXmlElement* bounding_layer_element)
{
   if(bounding_layer_element)
   {

      // Control sentence 
//      {
//         const char* text = bounding_layer_element->GetText();     

//         const std::string string(text);

//         if(string != "BoundingLayer")
//         {
//            std::ostringstream buffer;

//            buffer << "OpenNN Exception: BoundingLayer class.\n" 
//                   << "void from_XML(TiXmlElement*) method.\n"
//                   << "Unkown root element: " << text << ".\n";

//   	        throw std::logic_error(buffer.str());
//         }
//      }

      // Lower bounds
      {
         TiXmlElement* lower_bounds_element = bounding_layer_element->FirstChildElement("LowerBounds");

         if(lower_bounds_element)
         {
            const char* lower_bounds_text = lower_bounds_element->GetText();

            if(lower_bounds_text)
            {
               Vector<double> new_lower_bounds;                  
               new_lower_bounds.parse(lower_bounds_text);

               try
               {
                  set_lower_bounds(new_lower_bounds);
               }
               catch(std::exception& e)
               {
                  std::cout << e.what() << std::endl;		 
               }
            }
         }
      }

      // Upper bounds
      {
         TiXmlElement* upper_bounds_element = bounding_layer_element->FirstChildElement("UpperBounds");

         if(upper_bounds_element)
         {
            const char* upper_bounds_text = upper_bounds_element->GetText();

            if(upper_bounds_text)
            {
               Vector<double> new_upper_bounds;                  
               new_upper_bounds.parse(upper_bounds_text);

               try
               {
                  set_upper_bounds(new_upper_bounds);
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
         TiXmlElement* display_element = bounding_layer_element->FirstChildElement("Display");

         if(display)
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
