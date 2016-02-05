/****************************************************************************************************************/
/*                                                                                                              */
/*   OpenNN: Open Neural Networks Library                                                                       */
/*   www.opennn.cimne.com                                                                                       */
/*                                                                                                              */
/*   T I M E   S E R I E S   P R E D I C T I O N   T E S T I N G   C L A S S                                    */
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

#include "time_series_prediction_testing.h"

namespace OpenNN
{

// DEFAULT CONSTRUCTOR

/// Default constructor. 
/// It creates a time series prediction testing object neither associated to a neural network nor to a data set.

TimeSeriesPredictionTesting::TimeSeriesPredictionTesting(void) 
 : neural_network_pointer(NULL),
   data_set_pointer(NULL)
{
   set_default();
}


// NEURAL NETWORK CONSTRUCTOR

/// Neural network constructor. 
/// It creates a time series prediction testing object associated to a neural network but not to a data set.
/// @param new_neural_network_pointer Pointer to a neural network object.

TimeSeriesPredictionTesting::TimeSeriesPredictionTesting(NeuralNetwork* new_neural_network_pointer)
 : neural_network_pointer(new_neural_network_pointer),
   data_set_pointer(NULL)
{
   set_default();
}


// DATA SET CONSTRUCTOR

/// Data set constructor. 
/// It creates a time series prediction testing object associated to a data set but not to a neural network.
/// @param new_data_set_pointer Pointer to a data set object.

TimeSeriesPredictionTesting::TimeSeriesPredictionTesting(DataSet* new_data_set_pointer)
 : neural_network_pointer(NULL),
   data_set_pointer(new_data_set_pointer)
{
   set_default();
}


// GENERAL CONSTRUCTOR

/// General constructor. 
/// It creates a time series prediction testing object associated to a neural network and a data set objects.
/// @param new_neural_network_pointer Pointer to a neural network object.
/// @param new_data_set_pointer Pointer to a data set object.

TimeSeriesPredictionTesting::TimeSeriesPredictionTesting(NeuralNetwork* new_neural_network_pointer, DataSet* new_data_set_pointer)
 : neural_network_pointer(new_neural_network_pointer),
   data_set_pointer(new_data_set_pointer)
{  
   set_default();
}


// XML CONSTRUCTOR

/// XML constructor. 
/// It creates a time series prediction testing object neither associated to a neural network nor to a data set objects.
/// It also loads the members of this object from a TinyXML element. 
/// @param time_series_prediction_testing_element XML element which contains the member data.

TimeSeriesPredictionTesting::TimeSeriesPredictionTesting(TiXmlElement* time_series_prediction_testing_element)
 : neural_network_pointer(NULL),
   data_set_pointer(NULL)
{
   set_default();

   from_XML(time_series_prediction_testing_element);
}



// DESTRUCTOR

/// Destructor. 
/// It does not delete any pointer. 

TimeSeriesPredictionTesting::~TimeSeriesPredictionTesting()
{
}


// METHODS

// NeuralNetwork* get_neural_network_pointer(void) const method

/// This method returns a pointer to the neural network which is to be tested. 

NeuralNetwork* TimeSeriesPredictionTesting::get_neural_network_pointer(void) const
{
   return(neural_network_pointer);
}


// DataSet* get_data_set_pointer(void) const method

/// This method returns a pointer to the data set object on which the neural network is tested. 

DataSet* TimeSeriesPredictionTesting::get_data_set_pointer(void) const
{
   return(data_set_pointer);
}


// const bool& get_display(void) const method

/// This method returns true if messages from this class can be displayed on the screen,
/// or false if messages from this class can't be displayed on the screen.

const bool& TimeSeriesPredictionTesting::get_display(void) const
{
   return(display);
}


// void set_neural_network_pointer(NeuralNetwork*) method

/// This method sets a new neural network to be tested. 
/// @param new_neural_network_pointer Pointer to the neural network object to be tested. 

void TimeSeriesPredictionTesting::set_neural_network_pointer(NeuralNetwork* new_neural_network_pointer)
{
   neural_network_pointer = new_neural_network_pointer;
}


// void set_data_set_pointer(DataSet*) method

/// This method sets a new data set on which the neural network is to be tested. 
/// @param new_data_set_pointer Pointer to the data set object used for testing. 

void TimeSeriesPredictionTesting::set_data_set_pointer(DataSet* new_data_set_pointer)
{
   data_set_pointer = new_data_set_pointer;
}


// void set_display(const bool&) method

/// This method sets a new display flag. 
/// @param new_display Flag for displaying messages from this class or not. 

void TimeSeriesPredictionTesting::set_display(const bool& new_display)
{
   display = new_display;
}    


// void set_default(void) method

/// This method sets the default values from the function regression testing class:
/// <ul>
/// <li> Display: True.
/// </ul>

void TimeSeriesPredictionTesting::set_default(void)
{
   display = true;
}


// std::string to_string(void) const method

/// This method returns a string representation of this time series prediction testing object. 

std::string TimeSeriesPredictionTesting::to_string(void) const
{
   std::ostringstream buffer;

   buffer << "Time series prediction testing\n"
          << "Display: " << display << "\n";

   return(buffer.str());
}


// TiXmlElement* to_XML(void) const method

/// This method serializes the time series prediction testing object into a XML element of the TinyXML library. 

TiXmlElement* TimeSeriesPredictionTesting::to_XML(void) const
{
   return(NULL);
}


// void from_XML(TiXmlElement*) method

/// This method deserializes a TinyXML element into this time series prediction testing object. 
// @param time_series_prediction_element Pointer to a XML element containing the member data. 
/// @todo 

void TimeSeriesPredictionTesting::from_XML(TiXmlElement*)
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

