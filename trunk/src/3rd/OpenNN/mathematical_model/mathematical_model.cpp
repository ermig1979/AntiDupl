/****************************************************************************************************************/
/*                                                                                                              */
/*   OpenNN: Open Neural Networks Library                                                                       */
/*   www.opennn.cimne.com                                                                                       */
/*                                                                                                              */
/*   M A T H E M A T I C A L   M O D E L   C L A S S                                                            */
/*                                                                                                              */
/*   Roberto Lopez                                                                                              */ 
/*   International Center for Numerical Methods in Engineering (CIMNE)                                          */
/*   Technical University of Catalonia (UPC)                                                                    */
/*   Barcelona, Spain                                                                                           */
/*   E-mail: rlopez@cimne.upc.edu                                                                               */ 
/*                                                                                                              */
/****************************************************************************************************************/

// System includes

#include<fstream>
#include<iostream>
#include<string>
#include<sstream>
#include <time.h>

// OpenNN includes

#include "../utilities/vector.h"
#include "../utilities/matrix.h"
#include "mathematical_model.h"

namespace OpenNN
{

// DEFAULT CONSTRUCTOR

/// Default constructor.
/// It constructs a default mathematical model object, with zero independent and dependent variables. 

MathematicalModel::MathematicalModel(void)
{                                            
   set_default();
}


// XML CONSTRUCTOR

/// XML constructor.
/// It creates a mathematical model and loads its members from a Tiny XML element. 
/// @param mathematical_model_element Pointer to a XML element containing the mathematical model members. 

MathematicalModel::MathematicalModel(TiXmlElement* mathematical_model_element)
{                
   set_default();

   from_XML(mathematical_model_element);   
}


// FILE CONSTRUCTOR

/// File constructor.
/// It creates a mathematical model and loads its members from a XML file. 
/// @param filename Name of mathematical model XML file. 

MathematicalModel::MathematicalModel(const std::string& filename)
{                
   set_default();

   load(filename);   
}


// COPY CONSTRUCTOR

/// Copy constructor. 
/// It creates a mathematical model object and copies its members from another object. 
/// @param other_mathematical_model Mathematical model object to be copied. 

MathematicalModel::MathematicalModel(const MathematicalModel& other_mathematical_model)
{                
   set(other_mathematical_model);   
}


// DESTRUCTOR
 
/// Destructor. 
/// It does not delete any object.  

MathematicalModel::~MathematicalModel(void)
{ 
}


// ASSIGNMENT OPERATOR

/// Assignment operator. 
/// It assigns to this object the members of an existing mathematical model object.
/// @param other_mathematical_model Mathematical model object to be assigned.

MathematicalModel& MathematicalModel::operator = (const MathematicalModel& other_mathematical_model)
{
   if(this != &other_mathematical_model) 
   {
      independent_variables_number = other_mathematical_model.independent_variables_number;
      dependent_variables_number = other_mathematical_model.dependent_variables_number;

      display = other_mathematical_model.display;
   }

   return(*this);
}


// EQUAL TO OPERATOR

/// Equal to operator. 
/// It compares this object with another object of the same class. 
/// It returns true if the members of the two objects have the same values, and false otherwise.
/// @ param other_mathematical_model Mathematical model to be compared with.

bool MathematicalModel::operator == (const MathematicalModel& other_mathematical_model) const
{
   if(independent_variables_number == other_mathematical_model.independent_variables_number
   && dependent_variables_number == other_mathematical_model.dependent_variables_number
   && display == other_mathematical_model.display)
   {
      return(true);
   }
   else
   {
      return(false);
   }
}


// METHODS

// const unsigned int& get_independent_variables_number(void) const method

/// This method returns the number of independent variables in the mathematical model. 

const unsigned int& MathematicalModel::get_independent_variables_number(void) const
{
   return(independent_variables_number);
}


// unsigned int get_dependent_variables_number(void) const method

/// This method returns the number of dependent variables in the mathematical model. 

const unsigned int& MathematicalModel::get_dependent_variables_number(void) const
{
   return(dependent_variables_number);
}


// unsigned int count_variables_number(void) const method

/// This method returns the total number variablesin the mathematical model.
/// This is the sum of the numbers of independent and dependent variables. 

unsigned int MathematicalModel::count_variables_number(void) const
{
   return(independent_variables_number + dependent_variables_number);
}


// const bool get_display(void) method

/// This method returns true if messages from this class can be displayed on the screen,
/// or false if messages from this class can't be displayed on the screen.

const bool& MathematicalModel::get_display(void) const
{
   return(display);     
}  


// void set(const MathematicalModel&) method

/// This method sets the members of this mathematical model object with those from other mathematical model object. 
/// @param other_mathematical_model Mathematical model object to be copied. 

void MathematicalModel::set(const MathematicalModel& other_mathematical_model)
{
   independent_variables_number = other_mathematical_model.independent_variables_number;
   dependent_variables_number = other_mathematical_model.dependent_variables_number;

   display = other_mathematical_model.display;
}


// void set_independent_variables_number(const unsigned int&) method

/// This method sets the number of independent variables in the mathematical model. 
/// @param new_independent_variables_number Number of independent variables.

void MathematicalModel::set_independent_variables_number(const unsigned int& new_independent_variables_number)
{
   independent_variables_number = new_independent_variables_number;
}


// void set_dependent_variables_number(const unsigned int&) method

/// This method sets the number of dependent variables in the mathematical model. 
/// @param new_dependent_variables_number Number of dependent variables.

void MathematicalModel::set_dependent_variables_number(const unsigned int& new_dependent_variables_number)
{
   dependent_variables_number = new_dependent_variables_number;
}


// void set_default(void) method

/// This method sets the following default values in the mathematical model:
/// <ul>
/// <li> Number of dependent variables: 0.
/// <li> Number of independent variables: 0.
/// <li> Display: True.
/// </ul>

void MathematicalModel::set_default(void)
{
   dependent_variables_number = 0;
   independent_variables_number = 0;

   display = true;
}


// void set_display(const bool&) const method

/// This method sets a new display value. 
/// If it is set to true messages from this class are to be displayed on the screen;
/// if it is set to false messages from this class are not to be displayed on the screen.
/// @param new_display Display value.

void MathematicalModel::set_display(const bool& new_display) 
{
   display = new_display;     
}


// Matrix<double> calculate_solutions(const NeuralNetwork&) const method

/// This virtual method returns the solutions to the mathematical model, 
/// which are given by the independent and the dependent variables. 
/// This method needs to be derived, otherwise an exception is thrown. 

Matrix<double> MathematicalModel::calculate_solutions(const NeuralNetwork&) const
{
   std::ostringstream buffer;

   buffer << "OpenNN Exception: MathematicalModel class.\n"
          << "Matrix<double> calculate_solutions(const NeuralNetwork&) const method.\n"
          << "This method has not been derived.\n";
 
   throw std::logic_error(buffer.str());
}


// Vector<double> calculate_final_solutions(const NeuralNetwork&) const method

/// This virtual method returns the final solutions of the mathematical model, 
/// which are given by the final independent and dependent variables. 
/// This method needs to be derived, otherwise an exception is thrown. 

Vector<double> MathematicalModel::calculate_final_solutions(const NeuralNetwork&) const
{
   std::ostringstream buffer;

   buffer << "OpenNN Exception: MathematicalModel class.\n"
          << "Vector<double> calculate_final_solutions(const NeuralNetwork&) const method.\n"
          << "This method has not been derived.\n";
 
   throw std::logic_error(buffer.str());
}


// Matrix<double> calculate_dependent_variables(const NeuralNetwork&, const Matrix<double>&) const method

/// This virtual method returns the dependent variables solutions to the mathematical model, 
/// This method needs to be derived, otherwise an exception is thrown. 

Matrix<double> MathematicalModel::calculate_dependent_variables(const NeuralNetwork&, const Matrix<double>&) const
{
   std::ostringstream buffer;

   buffer << "OpenNN Exception: MathematicalModel class.\n"
          << "Matrix<double> calculate_dependent_variables(const NeuralNetwork&, const Matrix<double>&) const method.\n"
          << "This method has not been derived.\n";
 
   throw std::logic_error(buffer.str());
}


// std::string to_string(void) const method

/// This method returns a string representation of the current mathematical model object. 

std::string MathematicalModel::to_string(void) const
{
   std::ostringstream buffer; 

   buffer << "Mathematical model\n"
          << "Independent variables number: " << independent_variables_number << "\n" 
          << "Dependent variables number: " << dependent_variables_number << "\n"
          << "Display: " << display << "\n";

   return(buffer.str());
}


// void print(void) const method

/// This method outputs to the console the string representation of the mathematical model. 

void MathematicalModel::print(void) const
{
   std::cout << to_string();
}



// TiXmlElement* to_XML(void) const method

/// This method serializes the mathematical model object into a XML element of the TinyXML library. 
/// See the OpenNN manual for more information about the format of this element. 

TiXmlElement* MathematicalModel::to_XML(void) const   
{
   std::ostringstream buffer;

   TiXmlElement* mathematical_model_element = new TiXmlElement("MathematicalModel");
   mathematical_model_element->SetAttribute("Version", 4);

   // Independent variables number 
   {
      TiXmlElement* independent_variables_number_element = new TiXmlElement("IndependentVariablesNumber");
      mathematical_model_element->LinkEndChild(independent_variables_number_element);

      buffer.str("");
      buffer << independent_variables_number;

      TiXmlText* independent_variables_number_text = new TiXmlText(buffer.str().c_str());
      independent_variables_number_element->LinkEndChild(independent_variables_number_text);
   }

   // Dependent variables number 
   {
      TiXmlElement* dependent_variables_number_element = new TiXmlElement("DependentVariablesNumber");
      mathematical_model_element->LinkEndChild(dependent_variables_number_element);

      buffer.str("");
      buffer << dependent_variables_number;

      TiXmlText* dependent_variables_number_text = new TiXmlText(buffer.str().c_str());
      dependent_variables_number_element->LinkEndChild(dependent_variables_number_text);
   }

   // Display 
   {
      TiXmlElement* display_element = new TiXmlElement("Display");
      mathematical_model_element->LinkEndChild(display_element);

      buffer.str("");
      buffer << display;

      TiXmlText* display_text = new TiXmlText(buffer.str().c_str());
      display_element->LinkEndChild(display_text);
   }

   return(mathematical_model_element);   
}


// void from_XML(TiXmlElement*) method

/// This method deserializes a TinyXML element into this mathematical model object. 
/// @param mathematical_model_element Pointer to a XML element containing the member data. 

void MathematicalModel::from_XML(TiXmlElement* mathematical_model_element)  
{
   std::ostringstream buffer;

   if(mathematical_model_element != NULL)
   {
      // Control sentence 
      {
         const char* text = mathematical_model_element->GetText();

         if(text)
         { 
            const std::string string(text);

            if(string != "MathematicalModel")
            {
               buffer << "OpenNN Exception: MathematicalModel class.\n" 
                      << "void from_XML(TiXmlElement*) method.\n"
                      << "Unkown root element: " << string << ".\n";

	           throw std::logic_error(buffer.str());
            }
         }
         else
         {
            buffer << "OpenNN Exception: MathematicalModel class.\n" 
                   << "void from_XML(TiXmlElement*) method.\n"
                   << "Unkown root element.\n";       

            throw std::logic_error(buffer.str());
         }
      }

      // Independent variables number
      {
         TiXmlElement* element = mathematical_model_element->FirstChildElement("IndependentVariablesNumber");

         if(element)
         {
            const char* text = element->GetText();     

            if(text)
            {
               try
               {
                  set_independent_variables_number(atoi(text));
               }
               catch(std::exception& e)
               {
                  std::cout << e.what() << std::endl;		 
               }
            }
         }
      }

      // Dependent variables number
      {
         TiXmlElement* element = mathematical_model_element->FirstChildElement("DependentVariablesNumber");

         if(element)
         {
            const char* text = element->GetText();     

            if(text)
            {
               try
               {
                  set_dependent_variables_number(atoi(text));
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
         TiXmlElement* element = mathematical_model_element->FirstChildElement("Display");

         if(element)
         {
            const char* text = element->GetText();     

            if(text)
            {
               try
               {
                  const std::string string(text);

                  set_display(string != "0");
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


// void save(const std::string&) const method

/// This method saves to a file the XML representation of the mathematical object. 
/// @param filename Name of mathematical model XML file. 

void MathematicalModel::save(const std::string& filename) const
{
   TiXmlDocument document;

   // Declaration

   TiXmlDeclaration* declaration = new TiXmlDeclaration("1.0", "", "");
   document.LinkEndChild(declaration);

   // Mathematical model plug-in

   TiXmlElement* mathematical_model_plug_in_element = to_XML();
   document.LinkEndChild(mathematical_model_plug_in_element);

   // Save

   document.SaveFile(filename.c_str());

}


// void load(const std::string&) method

/// This method loads the members of the mathematical model from a XML file. 
/// @param filename Name of mathematical model XML file. 

void MathematicalModel::load(const std::string& filename)
{
   std::ostringstream buffer;

   TiXmlDocument document(filename.c_str());

   if(!document.LoadFile())
   {
      buffer << "OpenNN Exception: MathematicalModel class.\n"
             << "void load(const std::string&) method.\n"
             << "Cannot load XML file " << filename << ".\n";

      throw std::logic_error(buffer.str());
   }

   // Mathematical model element
  
   TiXmlElement* mathematical_model_element = document.FirstChildElement();

   if(!mathematical_model_element)
   {
      buffer << "OpenNN Exception: MathematicalModel class.\n"
             << "void load(const std::string&) method.\n"
             << "File " << filename << " is not a valid mathematical model file.\n";

      throw std::logic_error(buffer.str());
   }

   from_XML(mathematical_model_element);
}


// void save_data(const NeuralNetwork&, const std::string&) const method

/// @todo

void MathematicalModel::save_data(const NeuralNetwork&, const std::string&) const
{
   std::ostringstream buffer;

   buffer << "OpenNN Exception: MathematicalModel class.\n"
          << "void save_data(const NeuralNetwork&, const std::string&) const method.\n"
          << "This method has not been derived.\n";
 
   throw std::logic_error(buffer.str());
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
