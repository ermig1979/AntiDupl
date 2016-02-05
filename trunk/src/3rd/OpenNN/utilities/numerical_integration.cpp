/****************************************************************************************************************/
/*                                                                                                              */
/*   OpenNN: Open Neural Networks Library                                                                       */
/*   www.opennn.cimne.com                                                                                       */
/*                                                                                                              */
/*   N U M E R I C A L   I N T E G R A T I O N   C L A S S                                                      */
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

#include "numerical_integration.h"

// TinyXml includes

#include "../../tinyxml/tinyxml.h"


namespace OpenNN
{

// DEFAULT CONSTRUCTOR

/// Default constructor.

NumericalIntegration::NumericalIntegration(void)	
{   

}


// DESTRUCTOR

/// Destructor.

NumericalIntegration::~NumericalIntegration(void)
{

}


// METHODS

// const NumericalIntegrationMethod& get_numerical_integration_method(void) const method

/// This method returns the method used for numerical integration (trapezoid method or Simpson's method).

const NumericalIntegration::NumericalIntegrationMethod& NumericalIntegration::get_numerical_integration_method(void) const
{
   return(numerical_integration_method);                           
}


// std::string write_numerical_integration_method(void) const method

/// This method returns a string with the name of the method to be used for numerical integration. 

std::string NumericalIntegration::write_numerical_integration_method(void) const
{
   switch(numerical_integration_method)
   {
      case TrapezoidMethod:
      {
         return("TrapezoidMethod");
	  }
	  break;

      case SimpsonMethod:
      {
         return("SimpsonMethod");
 	  }
	  break;

      default:
      {
         std::ostringstream buffer;

         buffer << "OpenNN Exception: NumericalIntegration class.\n"
                << "std::string write_numerical_integration_method(void) const method.\n"
                << "Unknown numerical integration method.\n";
 
         throw std::logic_error(buffer.str());
	  }
	  break;
   }
}


// const bool& get_display(void) const method

/// This method returns the flag used by this class for displaying or not displaying warnings.

const bool& NumericalIntegration::get_display(void) const
{
   return(display);
}


// void set(const NumericalIntegration&) method

/// This method sets the members of this object to be equal to those of another object. 
/// @param other_numerical_integration Numerical integration object to be copied. 

void NumericalIntegration::set(const NumericalIntegration& other_numerical_integration)
{
   numerical_integration_method = other_numerical_integration.numerical_integration_method;

   display = other_numerical_integration.display;
}


// void set_numerical_integration_method(const NumericalIntegrationMethod&)

/// This method sets the method to be used for numerical integration (trapezoid method or Simpson's method).
/// @param new_numerical_integration_method New numerical integration method.

void NumericalIntegration::set_numerical_integration_method
(const NumericalIntegration::NumericalIntegrationMethod& new_numerical_integration_method)
{
   numerical_integration_method = new_numerical_integration_method;
}


// void set_numerical_integration_method(const std::string&) method

/// This method sets the method to be used for the numerical integration.
/// The argument is a string with the name of the numerical integration method. 
/// @param new_numerical_integration_method Numerical integration method name string.

void NumericalIntegration::set_numerical_integration_method(const std::string& new_numerical_integration_method)
{
   if(new_numerical_integration_method == "TrapezoidMethod")
   {
      numerical_integration_method = TrapezoidMethod;
   }
   else if(new_numerical_integration_method == "SimpsonMethod")
   {
      numerical_integration_method = SimpsonMethod;
   }
   else
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: NumericalIntegration class.\n"
             << "void set_numerical_integration_method(const std::string&) method.\n"
			 << "Unknown numerical integration method: " << new_numerical_integration_method << ".\n";

      throw std::logic_error(buffer.str());
   }	
}


// void set_display(const bool&) method

/// This method sets a new flag for displaying warnings from this class or not. 
/// @param new_display Display flag. 

void NumericalIntegration::set_display(const bool& new_display)
{
   display = new_display;
}


// void set_default(void) method

/// This method sets the default values to the numerical integration object:
/// <ul>
/// <li> Numerical integration method: Simpson method.
/// <li> Display: true
/// </ul>

void NumericalIntegration::set_default(void)
{
   numerical_integration_method = SimpsonMethod;

   display = true;
}


// double calculate_trapezoid_integral(const Vector<double>&, const Vector<double>&) const method

/// This method evaluates the integral of a function given as a set of n pairs of data (x,y) using the composite 
/// trapezoid rule. 
/// @param x Vector of x data.
/// @param y Vector of y data.

double NumericalIntegration::calculate_trapezoid_integral(const Vector<double>& x, const Vector<double>& y) const
{
   // Number of integration points

   unsigned int n = x.size();

   // Calculate integral

   double trapezoid_integral = 0;

   for(unsigned int i = 0; i < n-1; i++)
   {
      trapezoid_integral += 0.5*(x[i+1]-x[i])*(y[i+1]+y[i]);
   }

   // Return integral value

   return(trapezoid_integral);
}


// double calculate_Simpson_integral(const Vector<double>&, const Vector<double>&) const method

/// This method evaluates the integral of a function given as a set of n pairs of data (x,y) using the composite 
/// Simpson's rule.
/// @param x Vector of x data.
/// @param y Vector of y data.

double NumericalIntegration::calculate_Simpson_integral(const Vector<double>& x, const Vector<double>& y) const
{
   double Simpson_integral = 0.0;

   // Number of integration points

   unsigned int n = x.size();
  
   unsigned int m = 0;

   double a = 0.0;
   double fa = 0.0;
   double b = 0.0;
   double fb = 0.0;
   double c = 0.0;
   double fc = 0.0;
   double wa = 0.0;
   double wb = 0.0;
   double wc = 0.0;
   double h = 0.0;

   double sum = 0.0;// evenSum = 0.0;
  
   if(n%2 != 0)
   { 
      m=(n-1)/2;

      for(unsigned int i = 0 ; i < m ; i++ )
      {
         a = x[2*i]; 
         b = x[2*i+1];  
         c = x[2*i+2];

         fa = y[2*i]; 
         fb = y[2*i+1]; 
         fc = y[2*i+2];
                   
         wa = (c-a)/((a-b)*(a-c))*(1.0/3.0*(a*a+c*c+a*c)-0.5*(a+c)*(b+c)+b*c);
         wb = (c-a)/((b-a)*(b-c))*(1.0/3.0*(a*a+c*c+a*c)-0.5*(a+c)*(a+c)+a*c);
         wc = (c-a)/((c-a)*(c-b))*(1.0/3.0*(a*a+c*c+a*c)-0.5*(a+c)*(a+b)+a*b);
          
         sum += wa*fa+wb*fb+wc*fc;
      }
   }
   else
   {
      m=(n-2)/2;

      for(unsigned int i = 0; i < m; i++ )
      {
         a = x[2*i]; 
         b = x[2*i+1];  
         c = x[2*i+2];

         fa = y[2*i]; 
         fb = y[2*i+1]; 
         fc = y[2*i+2];
             
         wa = (c-a)/((a-b)*(a-c))*(1.0/3.0*(a*a+c*c+a*c)-0.5*(a+c)*(b+c)+b*c);
         wb = (c-a)/((b-a)*(b-c))*(1.0/3.0*(a*a+c*c+a*c)-0.5*(a+c)*(a+c)+a*c);
         wc = (c-a)/((c-a)*(c-b))*(1.0/3.0*(a*a+c*c+a*c)-0.5*(a+c)*(a+b)+a*b);
         
	 sum += wa*fa+wb*fb+wc*fc;
      }

      // Trapezoid
       
      h = x[n-1]-x[n-2];

      sum += h*(y[n-1]+y[n-2])/2.0;          
   }
     
   Simpson_integral = sum ;

   return(Simpson_integral);
}


// double calculate_integral(const Vector<double>&, const Vector<double>&) const method

/// This method evaluates the integral of a function given as a set of n pairs of data (x,y). 
/// @param x Vector of x data.
/// @param y Vector of y data.

double NumericalIntegration::calculate_integral(const Vector<double>& x, const Vector<double>& y) const
{
   switch(numerical_integration_method)
   {
      case TrapezoidMethod:
      {
         return(calculate_trapezoid_integral(x, y));
	  }
	  break;

      case SimpsonMethod:
      {
         return(calculate_Simpson_integral(x, y));
 	  }
	  break;

      default:
      {
         std::ostringstream buffer;

         buffer << "OpenNN Exception: NumericalIntegration class.\n"
                << "double calculate_integral(const Vector<double>&, const Vector<double>&) const method.\n"
                << "Unknown numerical integration method.\n";
 
         throw std::logic_error(buffer.str());
	  }
	  break;
   }
 
}

// TiXmlElement* to_XML(void) const method

/// This method serializes this numerical integration object into a XML element. 

TiXmlElement* NumericalIntegration::to_XML(void) const  
{
   std::ostringstream buffer;

   // Numerical integration

   TiXmlElement* numerical_integration_element = new TiXmlElement("NumericalIntegration");
   numerical_integration_element->SetAttribute("Version", 4);

   // Numerical integration method
   {
      TiXmlElement* element = new TiXmlElement("NumericalIntegrationMethod");
      numerical_integration_element->LinkEndChild(element);

      TiXmlText* text = new TiXmlText(write_numerical_integration_method().c_str());
      element->LinkEndChild(text);
   }

   // Display
   {
      TiXmlElement* element = new TiXmlElement("Display");
      numerical_integration_element->LinkEndChild(element);

      buffer.str("");
      buffer << display;

      TiXmlText* text = new TiXmlText(buffer.str().c_str());
      element->LinkEndChild(text);
   }

   return(numerical_integration_element);
}


// void from_XML(TiXmlElement*) method

/// This method deserializes the object from a XML element.
/// @param numerical_integration_element TinyXML element with the member data. 

void NumericalIntegration::from_XML(TiXmlElement* numerical_integration_element)   
{
   if(numerical_integration_element)
   {
      // Numerical integration method
      {
         TiXmlElement* element = numerical_integration_element->FirstChildElement("NumericalIntegrationMethod");

         if(element)
         {
            std::string new_numerical_integration_method = element->GetText(); 

            try
            {
               set_numerical_integration_method(new_numerical_integration_method);
            }
            catch(std::exception& e)
            {
               std::cout << e.what() << std::endl;		 
            }
         }
      }

      // Display
      {
         TiXmlElement* element = numerical_integration_element->FirstChildElement("Display");

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
