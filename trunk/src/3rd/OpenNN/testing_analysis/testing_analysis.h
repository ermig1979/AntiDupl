/****************************************************************************************************************/
/*                                                                                                              */
/*   OpenNN: Open Neural Networks Library                                                                       */
/*   www.opennn.cimne.com                                                                                       */
/*                                                                                                              */
/*   T E S T I N G   A N A L Y S I S   C L A S S   H E A D E R                                                  */
/*                                                                                                              */
/*   Roberto Lopez                                                                                              */
/*   International Center for Numerical Methods in Engineering (CIMNE)                                          */
/*   Technical University of Catalonia (UPC)                                                                    */
/*   Barcelona, Spain                                                                                           */
/*   E-mail: rlopez@cimne.upc.edu                                                                               */
/*                                                                                                              */
/****************************************************************************************************************/

#ifndef __TESTINGANALYSIS_H__
#define __TESTINGANALYSIS_H__

// OpenNN includes

#include "../utilities/vector.h"
#include "../utilities/matrix.h"

#include "../data_set/data_set.h"
#include "../mathematical_model/mathematical_model.h"

#include "../neural_network/neural_network.h"

#include "function_regression_testing.h"
#include "pattern_recognition_testing.h"
#include "time_series_prediction_testing.h"
#include "inverse_problem_testing.h"


namespace OpenNN
{

/// This class contains tools for testing neural networks in different learning tasks. 
/// In particular, it is composed by four different objects:
/// <ul>
/// <li> Function regression testing.
/// <li> Pattern recognition testing. 
/// <li> Time series prediction testing. 
/// <li> Inverse problem testing. 
/// </ul>

class TestingAnalysis
{

public:  

   // DEFAULT CONSTRUCTOR

   explicit TestingAnalysis(void);

   // NEURAL NETWORK CONSTRUCTOR

   explicit TestingAnalysis(NeuralNetwork*);

   // MATHEMATICAL MODEL CONSTRUCTOR

   explicit TestingAnalysis(MathematicalModel*);

   // DATA SET CONSTRUCTOR

   explicit TestingAnalysis(DataSet*);

   // NEURAL NETWORK AND DATA SET CONSTRUCTOR

   explicit TestingAnalysis(NeuralNetwork*, DataSet*);

   // NEURAL NETWORK AND MATHEMATICAL MODEL CONSTRUCTOR

   explicit TestingAnalysis(NeuralNetwork*, MathematicalModel*);

   // NEURAL NETWORK, DATA SET AND MATHEMATICAL MODEL CONSTRUCTOR

   explicit TestingAnalysis(NeuralNetwork*, DataSet*, MathematicalModel*);

   // XML CONSTRUCTOR

   explicit TestingAnalysis(TiXmlElement*);

   // FILE CONSTRUCTOR

   explicit TestingAnalysis(const std::string&);


   // DESTRUCTOR

   virtual ~TestingAnalysis(void);

   // METHODS

   // Get methods

   NeuralNetwork* get_neural_network_pointer(void) const;
   DataSet* get_data_set_pointer(void) const;
   MathematicalModel* get_mathematical_model_pointer(void) const;

   FunctionRegressionTesting* get_function_regression_testing_pointer(void) const;   
   PatternRecognitionTesting* get_pattern_recognition_testing_pointer(void) const;
   TimeSeriesPredictionTesting* get_time_series_prediction_testing_pointer(void) const;
   InverseProblemTesting* get_inverse_problem_testing_pointer(void) const;

   const bool& get_display(void) const;

   // Set methods

   void set_neural_network_pointer(NeuralNetwork*);
   void set_data_set_pointer(DataSet*);
   void set_mathematical_model_pointer(MathematicalModel*);

   void set_function_regression_testing_pointer(FunctionRegressionTesting*);
   void set_pattern_recognition_testing_pointer(PatternRecognitionTesting*);
   void set_time_series_prediction_testing_pointer(TimeSeriesPredictionTesting*);
   void set_inverse_problem_testing_pointer(InverseProblemTesting*);

   void set_display(const bool&);

   void set_default(void);

   // Pointer methods

   void construct_function_regression_testing(void);
   void construct_pattern_recognition_testing(void);
   void construct_time_series_prediction_testing(void);
   void construct_inverse_problem_testing(void);

   void destruct_function_regression_testing(void);
   void destruct_pattern_recognition_testing(void);
   void destruct_time_series_prediction_testing(void);
   void destruct_inverse_problem_testing(void);

   // Serialization methods 

   std::string to_string(void) const;

   void print(void) const;

   virtual TiXmlElement* to_XML(void) const;
   virtual void from_XML(TiXmlElement*);

   void save(const std::string&) const;
   void load(const std::string&);


private: 

   // MEMBERS

   /// Pointer to the neural network object to be tested. 

   NeuralNetwork* neural_network_pointer;

   /// Pointer to a data set object.

   DataSet* data_set_pointer;

   /// Pointer to a mathematical model object.

   MathematicalModel* mathematical_model_pointer;

   /// Pointer to the function regression testing object composing the testing analysis object.  

   FunctionRegressionTesting* function_regression_testing_pointer;

   /// Pointer to the pattern recognition testing object composing the testing analysis object.  

   PatternRecognitionTesting* pattern_recognition_testing_pointer;

   /// Pointer to the time series prediction testing object composing the testing analysis object.  

   TimeSeriesPredictionTesting* time_series_prediction_testing_pointer;

   /// Pointer to the inverse problem testing object composing the testing analysis object.  

   InverseProblemTesting* inverse_problem_testing_pointer;

   /// Display messages to screen.
   
   bool display;
};

}

#endif

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
