/*                                                                                                              */
/*   OpenNN: Open Neural Networks Library                                                                       */
/*   www.opennn.cimne.com                                                                                       */
/*                                                                                                              */
/*   I N V E R S E   P R O B L E M   T E S T I N G   C L A S S                                                  */
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
#include <string>
#include <sstream>
#include <cmath>

// OpenNN includes

#include "inverse_problem_testing.h"

namespace OpenNN
{

// DEFAULT CONSTRUCTOR

/// Default constructor. 
///	It creates a inverse problem testing object not associated to a neural network, a data set or a mathematical model.

InverseProblemTesting::InverseProblemTesting(void) 
 : neural_network_pointer(NULL),
   data_set_pointer(NULL),
   mathematical_model_pointer(NULL)
{
   set_default();
}


// NEURAL NETWORK CONSTRUCTOR

/// Neural network constructor. 
/// It creates a inverse problem testing object associated to a neural network but not to a a data set or a mathematical model.
/// @param new_neural_network_pointer Pointer to a neural network object.

InverseProblemTesting::InverseProblemTesting(NeuralNetwork* new_neural_network_pointer)
 : neural_network_pointer(new_neural_network_pointer),
   data_set_pointer(NULL),
   mathematical_model_pointer(NULL)
{
   set_default();
}


// DATA SET CONSTRUCTOR

/// Data set constructor. 
/// It creates a inverse problem testing object associated to a data set, but not to a neural network or a mathematical model.
/// @param new_data_set_pointer Pointer to a data set object.

InverseProblemTesting::InverseProblemTesting(DataSet* new_data_set_pointer)
 : neural_network_pointer(NULL),
   data_set_pointer(new_data_set_pointer),
   mathematical_model_pointer(NULL)
{
   set_default();
}


// NEURAL NETWORK, MATHEMATICAL MODEL AND DATA SET CONSTRUCTOR

/// Neural network, mathematical model and data set constructor. 
/// It creates a inverse problem testing object associated to a neural network, a mathematical model and a data set.
/// @param new_neural_network_pointer Pointer to a neural network object.
/// @param new_mathematical_model_pointer Pointer to a mathematical model object.
/// @param new_data_set_pointer Pointer to a data set object.

InverseProblemTesting::InverseProblemTesting
(NeuralNetwork* new_neural_network_pointer, DataSet* new_data_set_pointer, MathematicalModel* new_mathematical_model_pointer)
 : neural_network_pointer(new_neural_network_pointer),
   data_set_pointer(new_data_set_pointer),
   mathematical_model_pointer(new_mathematical_model_pointer)
{
   set_default();
}
 

// XML CONSTRUCTOR

/// XML constructor. 
/// It creates an inverse problem testing neither associated to a neural network, nor to a mathematical model, nor to a data set.
/// It also loads the members of this object from a TinyXML element. 
/// @param inverse_problem_testing_element XML element which contains the member data.

InverseProblemTesting::InverseProblemTesting(TiXmlElement* inverse_problem_testing_element)
 : neural_network_pointer(NULL),
   data_set_pointer(NULL),
   mathematical_model_pointer(NULL)
{
   set_default();

   from_XML(inverse_problem_testing_element);
}


// DESTRUCTOR

/// Destructor. 
/// This destructor does not delete any pointer. 

InverseProblemTesting::~InverseProblemTesting()
{
}


// METHODS

// NeuralNetwork* get_neural_network_pointer(void) const method

/// This method returns a pointer to the neural network which is to be tested. 

NeuralNetwork* InverseProblemTesting::get_neural_network_pointer(void) const
{
   return(neural_network_pointer);
}


// DataSet* get_mathematical_model_pointer(void) const method

/// This method returns a pointer to the mathematical model object on which the neural network is tested. 

MathematicalModel* InverseProblemTesting::get_mathematical_model_pointer(void) const
{
   return(mathematical_model_pointer);
}


// DataSet* get_data_set_pointer(void) const method

/// This method returns a pointer to the data set object on which the neural network is tested. 

DataSet* InverseProblemTesting::get_data_set_pointer(void) const
{
   return(data_set_pointer);
}


// const bool& get_display(void) const method

/// This method returns true if messages from this class can be displayed on the screen,
/// or false if messages from this class can't be displayed on the screen.

const bool& InverseProblemTesting::get_display(void) const
{
   return(display);
}


// void set_neural_network_pointer(NeuralNetwork*) method

/// This method sets a new neural network to be tested. 
/// @param new_neural_network_pointer Pointer to the neural network object to be tested. 

void InverseProblemTesting::set_neural_network_pointer(NeuralNetwork* new_neural_network_pointer)
{
   neural_network_pointer = new_neural_network_pointer;
}


// void set_data_set_pointer(DataSet*) method

/// This method sets a new data set on which the neural network is to be tested. 
/// @param new_data_set_pointer Pointer to the data set object used for testing. 

void InverseProblemTesting::set_data_set_pointer(DataSet* new_data_set_pointer)
{
   data_set_pointer = new_data_set_pointer;
}


// void set_mathematical_model_pointer(DataSet*) method

/// This method sets a new mathematical model on which the neural network is to be tested. 
/// @param new_mathematical_model_pointer Pointer to the mathematical model object used for testing. 

void InverseProblemTesting::set_mathematical_model_pointer(MathematicalModel* new_mathematical_model_pointer)
{
   mathematical_model_pointer = new_mathematical_model_pointer;
}


// void set_display(const bool&) method

/// This method sets a new display flag. 
/// @param new_display Flag for displaying messages from this class or not. 

void InverseProblemTesting::set_display(const bool& new_display)
{
   display = new_display;
}    


// void set_default(void) method

/// This method sets the default values from the inverse problem testing class:
/// <ul>
/// <li> Display: True.
/// </ul>

void InverseProblemTesting::set_default(void)
{
   display = true;
}


// std::string to_string(void) const method

/// This method returns a string representation of the current inverse problem testing object. 

std::string InverseProblemTesting::to_string(void) const
{
   std::ostringstream buffer;

   buffer << "Inverse problem testing\n"
          << "Display: " << display << "\n";

   return(buffer.str());
}


// TiXmlElement* to_XML(void) const method

/// This method serializes the inverse problem testing object into a XML element of the TinyXML library. 
/// See the OpenNN manual for more information about the format of this element. 
/// @todo

TiXmlElement* InverseProblemTesting::to_XML(void) const
{
   return(NULL);
}


// void from_XML(TiXmlElement*) method

/// @todo

void InverseProblemTesting::from_XML(TiXmlElement*)
{
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

