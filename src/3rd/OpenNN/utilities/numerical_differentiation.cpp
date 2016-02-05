/****************************************************************************************************************/
/*                                                                                                              */
/*   OpenNN: Open Neural Networks Library                                                                       */
/*   www.opennn.cimne.com                                                                                       */
/*                                                                                                              */
/*   N U M E R I C A L   D I F F E R E N T I A T I O N   C L A S S                                              */
/*                                                                                                              */
/*   Roberto Lopez                                                                                              */ 
/*   International Center for Numerical Methods in Engineering (CIMNE)                                          */
/*   Technical University of Catalonia (UPC)                                                                    */
/*   Barcelona, Spain                                                                                           */
/*   E-mail: rlopez@cimne.upc.edu                                                                               */ 
/*                                                                                                              */
/****************************************************************************************************************/

// System includes

#include<iostream>

// OpenNN includes

#include "numerical_differentiation.h"

// TinyXml includes

#include "../../tinyxml/tinyxml.h"


namespace OpenNN
{

// DEFAULT CONSTRUCTOR

/// Default constructor. 
/// It creates a numerical differentiation object with the default members. 

NumericalDifferentiation::NumericalDifferentiation(void)
{ 
   set_default();
}


// COPY CONSTRUCTOR

/// Copy constructor. 

NumericalDifferentiation::NumericalDifferentiation(const NumericalDifferentiation& other_numerical_differentiation)
{ 
   set(other_numerical_differentiation);
}


// DESTRUCTOR

/// Destructor.

NumericalDifferentiation::~NumericalDifferentiation(void)
{

}


// ASSIGNMENT OPERATOR

// NumericalDifferentiation& operator = (const NumericalDifferentiation&) method

/// Assignment operator.
/// This operator sets the members of this object with the members of another object. 
/// @param other_numerical_differentiation Numerical differentiation object to be copied.

NumericalDifferentiation& NumericalDifferentiation::operator = (const NumericalDifferentiation& other_numerical_differentiation)
{
   if(this != &other_numerical_differentiation) 
   {
      numerical_differentiation_method = other_numerical_differentiation.numerical_differentiation_method;

      precision_digits = other_numerical_differentiation.precision_digits;

      display = other_numerical_differentiation.display;
   }

   return(*this);
}


// EQUAL TO OPERATOR

// bool operator == (const NumericalDifferentiation&) const method

/// Equal to operator. 
/// This operator compares another object to this object. 
/// It returns true if both objects are equal and false otherwise. 
/// @param other_numerical_differentiation Numerical differentiation object to be compared.

bool NumericalDifferentiation::operator == (const NumericalDifferentiation& other_numerical_differentiation) const
{
   if(numerical_differentiation_method == other_numerical_differentiation.numerical_differentiation_method
   && precision_digits == other_numerical_differentiation.precision_digits
   && display == other_numerical_differentiation.display)
   {
      return(true);
   }
   else
   {
      return(false);
   }
}


// METHODS

// const NumericalDifferentiationMethod& get_numerical_differentiation_method(void) const method

/// This method returns the method used for numerical differentiation (forward differences or central differences).

const NumericalDifferentiation::NumericalDifferentiationMethod& NumericalDifferentiation::get_numerical_differentiation_method(void) const
{
   return(numerical_differentiation_method);                           
}


// std::string write_numerical_differentiation_method(void) const method

/// This method returns a string with the name of the method to be used for numerical differentiation. 

std::string NumericalDifferentiation::write_numerical_differentiation_method(void) const
{
   switch(numerical_differentiation_method)
   {
      case ForwardDifferences:
      {
         return("ForwardDifferences");
	  }
	  break;

      case CentralDifferences:
      {
         return("CentralDifferences");
 	  }
	  break;

      default:
      {
         std::ostringstream buffer;

         buffer << "OpenNN Exception: NumericalDifferentiation class.\n"
                << "std::string write_numerical_differentiation_method(void) const method.\n"
                << "Unknown numerical differentiation method.\n";
 
         throw std::logic_error(buffer.str());
	  }
	  break;
   }
}


// const unsigned int& get_precision_digits(void) const method

/// This method returns the number of precision digits required for the derivatives. 

const unsigned int& NumericalDifferentiation::get_precision_digits(void) const
{
   return(precision_digits);
}


// const bool& get_display(void) const method

/// This method returns the flag used by this class for displaying or not displaying warnings.

const bool& NumericalDifferentiation::get_display(void) const
{
   return(display);
}


// void set(const NumericalDifferentiation&) method

/// This method sets the members of this object to be equal to those of another object. 
/// @param other_numerical_differentiation Numerical differentiation object to be copied. 

void NumericalDifferentiation::set(const NumericalDifferentiation& other_numerical_differentiation)
{
   numerical_differentiation_method = other_numerical_differentiation.numerical_differentiation_method;

   precision_digits = other_numerical_differentiation.precision_digits;

   display = other_numerical_differentiation.display;

}


// void set_numerical_differentiation_method(const NumericalDifferentiationMethod&)

/// This method sets the method to be used for numerical differentiation (forward differences or central differences).
/// @param new_numerical_differentiation_method New numerical differentiation method.

void NumericalDifferentiation::set_numerical_differentiation_method
(const NumericalDifferentiation::NumericalDifferentiationMethod& new_numerical_differentiation_method)
{
   numerical_differentiation_method = new_numerical_differentiation_method;
}


// void set_numerical_differentiation_method(const std::string&) method

/// This method sets the method to be used for the numerical differentiation.
/// The argument is a string with the name of the numerical differentiation method. 
/// @param new_numerical_differentiation_method Numerical differentiation method name string.

void NumericalDifferentiation::set_numerical_differentiation_method(const std::string& new_numerical_differentiation_method)
{
   if(new_numerical_differentiation_method == "ForwardDifferences")
   {
      numerical_differentiation_method = ForwardDifferences;
   }
   else if(new_numerical_differentiation_method == "CentralDifferences")
   {
      numerical_differentiation_method = CentralDifferences;
   }
   else
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: NumericalDifferentiation class.\n"
             << "void set_numerical_differentiation_method(const std::string&) method.\n"
			 << "Unknown numerical differentiation method: " << new_numerical_differentiation_method << ".\n";

      throw std::logic_error(buffer.str());
   }	
}


// void set_display(const bool&) method

/// This method sets a new flag for displaying warnings from this class or not. 
/// @param new_display Display flag. 

void NumericalDifferentiation::set_display(const bool& new_display)
{
   display = new_display;
}


// void set_precision_digits(const unsigned int&) method

/// This method sets a new number of precision digits required for the derivatives. 
/// @param new_precision_digits Number of precision digits. 

void NumericalDifferentiation::set_precision_digits(const unsigned int& new_precision_digits)
{
   precision_digits = new_precision_digits;
}


// void set_default(void) method

/// This method sets default values for the members of this object:
/// <ul> 
/// <li> Numerical differentiation method: Central differences.
/// <li> Precision digits: 6.
/// <li> Display: true.
/// </ul>

void NumericalDifferentiation::set_default(void)
{
   numerical_differentiation_method = CentralDifferences;

   precision_digits = 6;

   display = true;
}


// double calculate_h(const double&) const  method

/// This method calculates a proper step size for computing the derivatives, as a function of the inputs point value. 
/// @param x Input value. 

double NumericalDifferentiation::calculate_h(const double& x) const
{
   const double eta = pow(10.0, (int)(-1*precision_digits));

   return(sqrt(eta)*(1.0 + fabs(x)));
}


// Vector<double> calculate_h(const Vector<double>&) const method

/// This method calculates a vector of step sizes for computing the derivatives, as a function of a vector of inputs. 
/// @param x Input vector. 

Vector<double> NumericalDifferentiation::calculate_h(const Vector<double>& x) const
{
   const double eta = pow(10.0, (int)(-1*precision_digits));

   const unsigned int n = x.size();

   Vector<double> h(n);

   for(unsigned int i = 0; i < n; i++)
   {
      h[i] = sqrt(eta)*(1.0 + fabs(x[i]));
   }
 
   return(h);
}


// TiXmlElement* to_XML(void) const method

/// This method serializes this numerical differentiation object into a XML element. 

TiXmlElement* NumericalDifferentiation::to_XML(void) const  
{
   std::ostringstream buffer;

   // Numerical differentiation

   TiXmlElement* numerical_differentiation_element = new TiXmlElement("NumericalDifferentiation");
   numerical_differentiation_element->SetAttribute("Version", 4);

   // Numerical differentiation method
   {
      TiXmlElement* element = new TiXmlElement("NumericalDifferentiationMethod");
      numerical_differentiation_element->LinkEndChild(element);

      TiXmlText* text = new TiXmlText(write_numerical_differentiation_method().c_str());
      element->LinkEndChild(text);
   }

   // Precision digits
   {
      TiXmlElement* element = new TiXmlElement("PrecisionDigits");
      numerical_differentiation_element->LinkEndChild(element);

      buffer.str("");
      buffer << precision_digits;

      TiXmlText* text = new TiXmlText(buffer.str().c_str());
      element->LinkEndChild(text);
   }

   // Display
   {
      TiXmlElement* element = new TiXmlElement("Display");
      numerical_differentiation_element->LinkEndChild(element);

      buffer.str("");
      buffer << display;

      TiXmlText* text = new TiXmlText(buffer.str().c_str());
      element->LinkEndChild(text);
   }

   return(numerical_differentiation_element);
}


// void from_XML(TiXmlElement*) method

/// This method deserializes the object from a XML element.
/// @param numerical_differentiation_element TinyXML element with the member data. 

void NumericalDifferentiation::from_XML(TiXmlElement* numerical_differentiation_element)   
{
   if(numerical_differentiation_element)
   {
      // Numerical differentiation method
      {
         TiXmlElement* element = numerical_differentiation_element->FirstChildElement("NumericalDifferentiationMethod");

         if(element)
         {
            std::string new_numerical_differentiation_method = element->GetText(); 

            try
            {
               set_numerical_differentiation_method(new_numerical_differentiation_method);
            }
            catch(std::exception& e)
            {
               std::cout << e.what() << std::endl;		 
            }
         }
      }

      // Precision digits
      {
         TiXmlElement* element = numerical_differentiation_element->FirstChildElement("PrecisionDigits");

         if(element)
         {
            const unsigned int new_precision_digits = atoi(element->GetText()); 

            try
            {
               set_precision_digits(new_precision_digits);
            }
            catch(std::exception& e)
            {
               std::cout << e.what() << std::endl;		 
            }
         }
      }

      // Display
      {
         TiXmlElement* element = numerical_differentiation_element->FirstChildElement("Display");

         if(element)
         {
            std::string new_display = element->GetText(); 

            try
            {
               set_display(new_display != "0");
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
