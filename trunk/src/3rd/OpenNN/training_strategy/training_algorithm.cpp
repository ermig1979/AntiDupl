/****************************************************************************************************************/
/*                                                                                                              */
/*   OpenNN: Open Neural Networks Library                                                                       */
/*   www.opennn.cimne.com                                                                                       */
/*                                                                                                              */
/*   T R A I N I N G   A L G O R I T H M   C L A S S                                                            */
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
#include <algorithm>
#include <functional>
#include <limits>
#include <cmath>
#include <ctime>

// OpenNN includes

#include "training_algorithm.h"

#include "gradient_descent.h"
#include "conjugate_gradient.h"
#include "quasi_newton_method.h"

// TinyXml includes

#include "../../tinyxml/tinyxml.h"

namespace OpenNN
{


// DEFAULT CONSTRUCTOR

/// Default constructor. 
/// It creates a training algorithm object not associated to any performance functional object.  

TrainingAlgorithm::TrainingAlgorithm(void)
 : performance_functional_pointer(NULL)
{ 
   set_default();
}


// GENERAL CONSTRUCTOR

/// General constructor. 
/// It creates a training algorithm object associated to a performance functional object.
/// @param new_performance_functional_pointer Pointer to a performance functional object.

TrainingAlgorithm::TrainingAlgorithm(PerformanceFunctional* new_performance_functional_pointer)
 : performance_functional_pointer(new_performance_functional_pointer)
{
   set_default();
}


// XML CONSTRUCTOR

/// XML constructor. 
/// It creates a training algorithm object not associated to any performance functional object. 
/// It also loads the other members from a XML element.

TrainingAlgorithm::TrainingAlgorithm(TiXmlElement* training_algorithm_element)
 : performance_functional_pointer(NULL)
{ 
   from_XML(training_algorithm_element);
}


// DESTRUCTOR 

/// Destructor

TrainingAlgorithm::~TrainingAlgorithm(void)
{ 
}


// METHODS

// PerformanceFunctional* get_performance_functional_pointer(void) const method

/// This method returns a pointer to the performance functional object to which the training algorithm is 
/// associated.

PerformanceFunctional* TrainingAlgorithm::get_performance_functional_pointer(void) const
{
   return(performance_functional_pointer);
}


// const bool& get_display(void) const method

/// This method returns true if messages from this class can be displayed on the screen, or false if messages from
/// this class can't be displayed on the screen.

const bool& TrainingAlgorithm::get_display(void) const
{
   return(display);
}


// void set(void) method

/// This method sets the performance functional pointer to NULL.
/// It also sets the rest of members to their default values. 

void TrainingAlgorithm::set(void)
{
   performance_functional_pointer = NULL;

   set_default();
}


// void set(PerformanceFunctional*) method

/// This method sets a new performance functional pointer.
/// It also sets the rest of members to their default values. 
/// @param new_performance_functional_pointer Pointer to a performance functional object. 

void TrainingAlgorithm::set(PerformanceFunctional* new_performance_functional_pointer)
{
   performance_functional_pointer = new_performance_functional_pointer;

   set_default();
}


// void set_performance_functional_pointer(PerformanceFunctional*) method

/// This method sets a pointer to a performance functional object to be associated to the training algorithm.
/// @param new_performance_functional_pointer Pointer to a performance functional object.

void TrainingAlgorithm::set_performance_functional_pointer(PerformanceFunctional* new_performance_functional_pointer)
{
   performance_functional_pointer = new_performance_functional_pointer;
}


// void set_display(const bool&) method

/// This method sets a new display value. 
/// If it is set to true messages from this class are to be displayed on the screen;
/// if it is set to false messages from this class are not to be displayed on the screen.
/// @param new_display Display value.

void TrainingAlgorithm::set_display(const bool& new_display)
{
   display = new_display;
}


// void set_default(void) method 

/// This method sets the members of the training algorithm object to their default values.

void TrainingAlgorithm::set_default(void)
{
   display = true;
}


// std::string write_training_algorithm_type(void) const method

/// This method writes a string with the type of training algoritm.

std::string TrainingAlgorithm::write_training_algorithm_type(void) const
{
   return("USER_TRAINING_ALGORITHM");
}


// void check(void) const method

/// This method performs a default checking for training algorithms. 
/// In particular, it checks that the performance functional pointer associated to the training algorithm is not NULL,
/// and that the neural network associated to that performance functional is neither NULL.
/// If that checkings are not hold, an exception is thrown. 

void TrainingAlgorithm::check(void) const
{
   std::ostringstream buffer;

   if(!performance_functional_pointer)
   {
      buffer << "OpenNN Exception: TrainingAlgorithm class.\n"
             << "void check(void) const method.\n"
             << "Pointer to performance functional is NULL.\n";

      throw std::logic_error(buffer.str().c_str());	  
   }

   const NeuralNetwork* neural_network_pointer = performance_functional_pointer->get_neural_network_pointer();

   if(neural_network_pointer == NULL)
   {
      buffer << "OpenNN Exception: TrainingAlgorithm class.\n"
             << "void check(void) const method.\n"
             << "Pointer to neural network is NULL.\n";

      throw std::logic_error(buffer.str().c_str());
   }
}


// std::string to_string(void) const method

/// This method returns a default string representation of a training algorithm. 

std::string TrainingAlgorithm::to_string(void) const
{
   std::ostringstream buffer;

   buffer << "Training strategy\n" 
          << "Display: " << display << "\n";

   return(buffer.str());
}


// TiXmlElement* to_XML(void) const method

/// This method returns a default string representation in XML-type format of the training algorithm object.
/// This containts the training operators, the training parameters, stopping criteria and other stuff.

TiXmlElement* TrainingAlgorithm::to_XML(void) const
{
   std::ostringstream buffer;

   // Training algorithm

   TiXmlElement* training_algorithm_element = new TiXmlElement("TrainingAlgorithm");
   training_algorithm_element->SetAttribute("Version", 4); 

   // Display
   {
      TiXmlElement* display_element = new TiXmlElement("Display");
      training_algorithm_element->LinkEndChild(display_element);

      buffer.str("");
      buffer << display;

      TiXmlText* display_text = new TiXmlText(buffer.str().c_str());
      display_element->LinkEndChild(display_text);
   }

   return(training_algorithm_element);
}


// void from_XML(TiXmlElement*) method

/// This method loads a training algorithm object from a XML element.
/// @param training_algorithm_element Pointer to a Tiny XML element containing the training algorithm members. 

void TrainingAlgorithm::from_XML(TiXmlElement* training_algorithm_element)
{
   // Display

   TiXmlElement* display_element = training_algorithm_element->FirstChildElement("Display");

   if(display_element)
   {
      std::string new_display = display_element->GetText(); 

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


// void print(void) const method

/// This method prints to the screen the XML-type representation of the training algorithm object. 

void TrainingAlgorithm::print(void) const
{
   std::cout << to_string();
}


// void save(const std::string&) const method

/// This method saves to a XML-type file the members of the training algorithm object. 
/// @param filename Name of training algorithm XML-type file. 

void TrainingAlgorithm::save(const std::string& filename) const
{
   std::ostringstream buffer;

   TiXmlDocument document;

   // Declaration

   TiXmlDeclaration* declaration = new TiXmlDeclaration("1.0", "", "");
   document.LinkEndChild(declaration);

   // Performance functional

   TiXmlElement* training_algorithm_element = to_XML();
   document.LinkEndChild(training_algorithm_element);

   document.SaveFile(filename.c_str());
}


// void load(const std::string&) method

/// This method loads a gradient descent object from a XML-type file. 
/// Please mind about the file format, wich is specified in the User's Guide. 
/// @param filename Name of training algorithm XML-type file. 

void TrainingAlgorithm::load(const std::string& filename)
{
   set_default();

   std::ostringstream buffer;

   TiXmlDocument document(filename.c_str());

   if (!document.LoadFile())
   {
      buffer << "OpenNN Exception: TrainingAlgorithm class.\n"
             << "void load(const std::string&) method.\n"
             << "Cannot load XML file " << filename << ".\n";

      throw std::logic_error(buffer.str());
   }

   // Training algorithm element
  
   TiXmlElement* training_algorithm_element = document.FirstChildElement("TrainingAlgorithm");

   if(!training_algorithm_element)
   {
      buffer << "OpenNN Exception: TrainingAlgorithm class.\n"
             << "void load(const std::string&) method.\n"
             << "File " << filename << " is not a valid training algorithm file.\n";

      throw std::logic_error(buffer.str());
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
