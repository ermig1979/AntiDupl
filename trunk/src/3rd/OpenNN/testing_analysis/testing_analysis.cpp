/****************************************************************************************************************/
/*                                                                                                              */
/*   OpenNN: Open Neural Networks Library                                                                       */
/*   www.opennn.cimne.com                                                                                       */
/*                                                                                                              */
/*   T E S T I N G   A N A L Y S I S   C L A S S                                                                */
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

#include "testing_analysis.h"

namespace OpenNN
{

// DEFAULT CONSTRUCTOR

/// Default constructor. 
/// It creates a testing analysis object neither associated to a neural network nor to a mathematical model or a data set.
/// By default, it constructs the function regression testing object. 

TestingAnalysis::TestingAnalysis(void)
 : neural_network_pointer(NULL),
   data_set_pointer(NULL),
   mathematical_model_pointer(NULL),
   function_regression_testing_pointer(NULL),
   pattern_recognition_testing_pointer(NULL),
   time_series_prediction_testing_pointer(NULL),
   inverse_problem_testing_pointer(NULL)
{
   construct_function_regression_testing();

   set_default();

}


// NEURAL NETWORK CONSTRUCTOR

/// Neural network constructor. 
/// It creates a testing analysis object associated to a neural network but not to a mathematical model or a data set.
/// By default, it constructs the function regression testing object. 
/// @param new_neural_network_pointer Pointer to a neural network object.

TestingAnalysis::TestingAnalysis(NeuralNetwork* new_neural_network_pointer)
: neural_network_pointer(new_neural_network_pointer),
   data_set_pointer(NULL),
   mathematical_model_pointer(NULL),
   function_regression_testing_pointer(NULL),
   pattern_recognition_testing_pointer(NULL),
   time_series_prediction_testing_pointer(NULL),
   inverse_problem_testing_pointer(NULL)
{
   construct_function_regression_testing();

   set_default();
}


// MATHEMATICAL MODEL CONSTRUCTOR

/// Mathematical mmodel constructor. 
/// It creates a testing analysis object not associated to a neural network, not associated to a data set, and associated to a mathematical model. 
/// By default, it constructs the inverse problem testing object. 
/// @param new_mathematical_model_pointer Pointer to a mathematical model object.

TestingAnalysis::TestingAnalysis(MathematicalModel* new_mathematical_model_pointer)
: neural_network_pointer(NULL),
   data_set_pointer(NULL),
   mathematical_model_pointer(new_mathematical_model_pointer),
   function_regression_testing_pointer(NULL),
   pattern_recognition_testing_pointer(NULL),
   time_series_prediction_testing_pointer(NULL),
   inverse_problem_testing_pointer(NULL)
{
   construct_inverse_problem_testing();

   set_default();
}


// DATA SET CONSTRUCTOR

/// Data set constructor. 
/// It creates a testing analysis object not associated to a neural network, associated to a data set and not associated to a mathematical model. 
/// By default, it constructs the function regression testing object. 
/// @param new_data_set_pointer Pointer to a data set object.

TestingAnalysis::TestingAnalysis(DataSet* new_data_set_pointer)
: neural_network_pointer(NULL),
   data_set_pointer(new_data_set_pointer),
   mathematical_model_pointer(NULL),
   function_regression_testing_pointer(NULL),
   pattern_recognition_testing_pointer(NULL),
   time_series_prediction_testing_pointer(NULL),
   inverse_problem_testing_pointer(NULL)
{
   construct_function_regression_testing();

   set_default();
}


// NEURAL NETWORK AND MATHEMATICAL MODEL CONSTRUCTOR

/// Neural network and mathematical model constructor. 
/// It creates a testing analysis object associated to a neural network and to a mathematical model, but not to a data set.
/// By default, it constructs the inverse problem testing object. 
/// @param new_neural_network_pointer Pointer to a neural network object.
/// @param new_mathematical_model_pointer Pointer to a mathematical model object.

TestingAnalysis::TestingAnalysis(NeuralNetwork* new_neural_network_pointer, MathematicalModel* new_mathematical_model_pointer)
 : neural_network_pointer(new_neural_network_pointer),
   data_set_pointer(NULL),
   mathematical_model_pointer(new_mathematical_model_pointer),
   function_regression_testing_pointer(NULL),
   pattern_recognition_testing_pointer(NULL),
   time_series_prediction_testing_pointer(NULL),
   inverse_problem_testing_pointer(NULL)
{
   construct_inverse_problem_testing();

   set_default();
}


// NEURAL NETWORK AND DATA SET CONSTRUCTOR

/// Neural network and data set constructor. 
/// It creates a testing analysis object associated to a neural network and to a data set.
/// By default, it constructs the function regression testing object. 
/// @param new_neural_network_pointer Pointer to a neural network object.
/// @param new_data_set_pointer Pointer to a data set object.

TestingAnalysis::TestingAnalysis(NeuralNetwork* new_neural_network_pointer, DataSet* new_data_set_pointer)
 : neural_network_pointer(new_neural_network_pointer),
   data_set_pointer(new_data_set_pointer),
   mathematical_model_pointer(NULL),
   function_regression_testing_pointer(NULL),
   pattern_recognition_testing_pointer(NULL),
   time_series_prediction_testing_pointer(NULL),
   inverse_problem_testing_pointer(NULL)
{
   construct_function_regression_testing();

   set_default();
}


// NEURAL NETWORK, MATHEMATICAL MODEL AND DATA SET CONSTRUCTOR

/// Neural network, mathematical model and data set constructor. 
/// It creates a testing analysis object associated to a neural network, a mathematical model and a data set.
/// By default, it constructs the inverse problem testing object. 
/// @param new_neural_network_pointer Pointer to a neural network object.
/// @param new_mathematical_model_pointer Pointer to a mathematical model object.
/// @param new_data_set_pointer Pointer to a data set object.

TestingAnalysis::TestingAnalysis(NeuralNetwork* new_neural_network_pointer, DataSet* new_data_set_pointer, MathematicalModel* new_mathematical_model_pointer)
 : neural_network_pointer(new_neural_network_pointer),
   data_set_pointer(new_data_set_pointer),
   mathematical_model_pointer(new_mathematical_model_pointer),
   function_regression_testing_pointer(NULL),
   pattern_recognition_testing_pointer(NULL),
   time_series_prediction_testing_pointer(NULL),
   inverse_problem_testing_pointer(NULL)
{
   construct_inverse_problem_testing();

   set_default();
}


// XML CONSTRUCTOR

/// XML constructor. 
/// It creates a testing analysis object neither associated to a neural network nor to a mathematical model or a data set. 
/// It also loads the members of this object from a TinyXML element. 
/// @param testing_analysis_element Pointer to a XML element containing the member data. 

TestingAnalysis::TestingAnalysis(TiXmlElement* testing_analysis_element)
 : neural_network_pointer(NULL),
   data_set_pointer(NULL),
   mathematical_model_pointer(NULL),
   function_regression_testing_pointer(NULL),
   pattern_recognition_testing_pointer(NULL),
   time_series_prediction_testing_pointer(NULL),
   inverse_problem_testing_pointer(NULL)
{
   set_default();

   from_XML(testing_analysis_element);
}


// FILE CONSTRUCTOR

/// File constructor. 
/// It creates a testing analysis object neither associated to a neural network nor to a mathematical model or a data set. 
/// It also loads the members of this object from XML file. 
/// @param filename Name of testing analysis XML file.  

TestingAnalysis::TestingAnalysis(const std::string& filename)
 : neural_network_pointer(NULL),
   data_set_pointer(NULL),
   mathematical_model_pointer(NULL),
   function_regression_testing_pointer(NULL),
   pattern_recognition_testing_pointer(NULL),
   time_series_prediction_testing_pointer(NULL),
   inverse_problem_testing_pointer(NULL)
{
   set_default();

   load(filename);
}

// DESTRUCTOR

/// Destructor. 
/// It deletes the function regression testing, pattern recognition testing, time series prediction testing and inverse problem testing objects. 

TestingAnalysis::~TestingAnalysis()
{
   delete function_regression_testing_pointer;
   delete pattern_recognition_testing_pointer;
   delete time_series_prediction_testing_pointer;
   delete inverse_problem_testing_pointer;
}


// METHODS

// NeuralNetwork* get_neural_network_pointer(void) const method

/// This method returns a pointer to the neural network object which is to be tested.

NeuralNetwork* TestingAnalysis::get_neural_network_pointer(void) const
{
   return(neural_network_pointer);   
}


// DataSet* get_data_set_pointer(void) const method

/// This method returns a pointer to the data set object on which the neural network is tested. 

DataSet* TestingAnalysis::get_data_set_pointer(void) const
{
   return(data_set_pointer);
}


// MathematicalModel* get_mathematical_model_pointer(void) const method

/// This method returns a pointer to the mathematical model object on which the neural network is tested. 

MathematicalModel* TestingAnalysis::get_mathematical_model_pointer(void) const
{
   return(mathematical_model_pointer);
}


// FunctionRegressionTesting* get_function_regression_testing_pointer(void) const method

/// This method returns a pointer to the function regression testing object composing this testing analysis object. 

FunctionRegressionTesting* TestingAnalysis::get_function_regression_testing_pointer(void) const
{
   return(function_regression_testing_pointer);    
}


// PatternRecognitionTesting* get_pattern_recognition_testing_pointer(void) const method

/// This method returns a pointer to the pattern recognition testing object composing this testing analysis object. 

PatternRecognitionTesting* TestingAnalysis::get_pattern_recognition_testing_pointer(void) const
{
   return(pattern_recognition_testing_pointer);    
}


// TimeSeriesPredictionTesting* get_time_series_prediction_testing_pointer(void) const method

/// This method returns a pointer to the time series prediction testing object composing this testing analysis object. 

TimeSeriesPredictionTesting* TestingAnalysis::get_time_series_prediction_testing_pointer(void) const
{
   return(time_series_prediction_testing_pointer);    
}


// InverseProblemTesting* get_inverse_problem_testing_pointer(void) const method

/// This method returns a pointer to the inverse problem testing object composing this testing analysis object. 

InverseProblemTesting* TestingAnalysis::get_inverse_problem_testing_pointer(void) const
{
   return(inverse_problem_testing_pointer);    
}


// const bool& get_display(void) const method

/// This method returns true if messages from this class can be displayed on the screen,
/// or false if messages from this class can't be displayed on the screen.

const bool& TestingAnalysis::get_display(void) const
{
   return(display);     
}


// void set_default(void) method

/// This method sets some default values to the testing analysis object:
/// <ul>
/// <li> Display: True.
/// </ul>

void TestingAnalysis::set_default(void)
{
   display = true;
}


// void set_neural_network_pointer(NeuralNetwork*) method

/// This method sets a new neural network object to be tested.
/// @param new_neural_network_pointer Pointer to a neural network object.

void TestingAnalysis::set_neural_network_pointer(NeuralNetwork* new_neural_network_pointer)
{
   neural_network_pointer = new_neural_network_pointer;   
}


// void set_mathematical_model_pointer(MathematicalModel*) method

/// This method sets a mathematical model to be used for validating the quality of a trained neural network.
/// @param new_mathematical_model_pointer Pointer to a mathematical model object.

void TestingAnalysis::set_mathematical_model_pointer(MathematicalModel* new_mathematical_model_pointer)
{
   mathematical_model_pointer = new_mathematical_model_pointer;   
}


// void set_data_set_pointer(DataSet*) method

/// This method sets a new data set to be used for validating the quality of a trained neural network.
/// @param new_data_set_pointer Pointer to a data set object.

void TestingAnalysis::set_data_set_pointer(DataSet* new_data_set_pointer)
{
   data_set_pointer = new_data_set_pointer;   
}


// void set_function_regression_testing_pointer(FunctionRegressionTesting*) method

/// This method sets a new function regression testing into the testing analysis. 
/// Note that this pointer will be deleted by the destructor. 
/// @param new_function_regression_testing_pointer Pointer to a function regression testing object. 

void TestingAnalysis::set_function_regression_testing_pointer(FunctionRegressionTesting* new_function_regression_testing_pointer)
{
   function_regression_testing_pointer = new_function_regression_testing_pointer;
}


// void set_pattern_recognition_testing_pointer(PatternRecognitionTesting*) method

/// This method sets a new pattern recognition testing into the testing analysis. 
/// Note that this pointer will be deleted by the destructor. 
/// @param new_pattern_recognition_testing_pointer Pointer to a pattern recognition testing object. 

void TestingAnalysis::set_pattern_recognition_testing_pointer(PatternRecognitionTesting* new_pattern_recognition_testing_pointer)
{
   pattern_recognition_testing_pointer = new_pattern_recognition_testing_pointer;
}


// void set_time_series_prediction_testing_pointer(TimeSeriesPredictionTesting*) method

/// This method sets a new time series prediction testing into the testing analysis. 
/// Note that this pointer will be deleted by the destructor. 
/// @param new_time_series_prediction_testing_pointer Pointer to a time series prediction testing object. 

void TestingAnalysis::set_time_series_prediction_testing_pointer(TimeSeriesPredictionTesting* new_time_series_prediction_testing_pointer)
{
   time_series_prediction_testing_pointer = new_time_series_prediction_testing_pointer;
}


// void set_inverse_problem_testing_pointer(InverseProblemTesting*) method

/// This method sets a new inverse problem testing into the testing analysis. 
/// Note that this pointer will be deleted by the destructor. 
/// @param new_inverse_problem_testing_pointer Pointer to an inverse problem testing object. 

void TestingAnalysis::set_inverse_problem_testing_pointer(InverseProblemTesting* new_inverse_problem_testing_pointer)
{
   inverse_problem_testing_pointer = new_inverse_problem_testing_pointer;
}


// void set_display(const bool&) method

/// This method sets a new display value. 
/// If it is set to true messages from this class are to be displayed on the screen;
/// if it is set to false messages from this class are not to be displayed on the screen.
/// @param new_display Display value.

void TestingAnalysis::set_display(const bool& new_display)
{
   display = new_display;
}


// void construct_function_regression_testing(void) method

/// This method constructs the function regression testing object inside the testing analysis object.
/// It sets the actual neural network and data set to the new object. 

void TestingAnalysis::construct_function_regression_testing(void)
{
   if(!function_regression_testing_pointer)
   {
      function_regression_testing_pointer = new FunctionRegressionTesting(neural_network_pointer, data_set_pointer);
   }
}


// void construct_pattern_recognition_testing(void) method

/// This method constructs the pattern recognition testing object inside the testing analysis object.
/// It sets the actual neural network and data set to the new object. 

void TestingAnalysis::construct_pattern_recognition_testing(void)
{
   if(!pattern_recognition_testing_pointer)
   {
      pattern_recognition_testing_pointer = new PatternRecognitionTesting(neural_network_pointer, data_set_pointer);
   }
}


// void construct_time_series_prediction_testing(void) method

/// This method constructs the time series prediction testing object inside the testing analysis object.
/// It sets the actual neural network and data set to the new object. 

void TestingAnalysis::construct_time_series_prediction_testing(void)
{
   if(!time_series_prediction_testing_pointer)
   {
      time_series_prediction_testing_pointer = new TimeSeriesPredictionTesting(neural_network_pointer, data_set_pointer);
   }
}


// void construct_inverse_problem_testing(void) method

/// This method constructs the function regression testing object inside the testing analysis object.
/// It sets the actual neural network, data set and mathematical model to the new object. 

void TestingAnalysis::construct_inverse_problem_testing(void)
{
   if(!inverse_problem_testing_pointer)
   {
      inverse_problem_testing_pointer = new InverseProblemTesting(neural_network_pointer, data_set_pointer, mathematical_model_pointer);
   }
}


// void destruct_function_regression_testing(void) method

/// This method deletes the function regression testing object composing the testing analysis object. 

void TestingAnalysis::destruct_function_regression_testing(void)
{
   delete function_regression_testing_pointer;

   function_regression_testing_pointer = NULL;
}


// void destruct_pattern_recognition_testing(void) method

/// This method deletes the pattern recognition testing object composing the testing analysis object. 

void TestingAnalysis::destruct_pattern_recognition_testing(void)
{
   delete pattern_recognition_testing_pointer;

   pattern_recognition_testing_pointer = NULL;
}


// void destruct_time_series_prediction_testing(void) method

/// This method deletes the time series prediction testing object composing the testing analysis object. 

void TestingAnalysis::destruct_time_series_prediction_testing(void)
{
   delete time_series_prediction_testing_pointer;

   time_series_prediction_testing_pointer = NULL;
}


// void destruct_inverse_problem_testing(void) method

/// This method deletes the inverse problem testing object composing the testing analysis object. 

void TestingAnalysis::destruct_inverse_problem_testing(void)
{
   delete inverse_problem_testing_pointer;

   inverse_problem_testing_pointer = NULL;
}


// std::string to_string(void) const method

/// This method returns a string representation of the testing analysis object. 

std::string TestingAnalysis::to_string(void) const
{
   std::ostringstream buffer;

   buffer << "Testing analysis\n"
          << "Display: " << display << "\n";

   if(function_regression_testing_pointer)
   {
      buffer << function_regression_testing_pointer->to_string();
   }

   if(pattern_recognition_testing_pointer)
   {
      buffer << pattern_recognition_testing_pointer->to_string();
   }

   if(time_series_prediction_testing_pointer)
   {
      buffer << time_series_prediction_testing_pointer->to_string();
   }

   if(inverse_problem_testing_pointer)
   {
      buffer << inverse_problem_testing_pointer->to_string();
   }

   return(buffer.str());
}


// void print(void) const method

/// This method prints to the standard output the string representation of this testing analysis object. 

void TestingAnalysis::print(void) const
{
   std::cout << to_string();
}


// TiXmlElement* to_XML(void) const method

/// This method serializes the testing analysis object into a XML element of the TinyXML library. 
/// See the OpenNN manual for more information about the format of this element. 
/// @todo 

TiXmlElement* TestingAnalysis::to_XML(void) const
{
   return(NULL);
}


// void from_XML(TiXmlElement*) method

/// @todo 

void TestingAnalysis::from_XML(TiXmlElement*)
{
}


// void save(const std::string&) const method
/// @todo 

void TestingAnalysis::save(const std::string&) const
{

}


// void load(const std::string&) method
/// @todo 

void TestingAnalysis::load(const std::string&)
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
