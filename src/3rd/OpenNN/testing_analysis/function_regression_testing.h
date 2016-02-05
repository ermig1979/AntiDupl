/****************************************************************************************************************/
/*                                                                                                              */
/*   OpenNN: Open Neural Networks Library                                                                       */
/*   www.opennn.cimne.com                                                                                       */
/*                                                                                                              */
/*   F U N C T I O N   R E G R E S S I O N   T E S T I N G   C L A S S   H E A D E R                            */
/*                                                                                                              */
/*   Roberto Lopez                                                                                              */
/*   International Center for Numerical Methods in Engineering (CIMNE)                                          */
/*   Technical University of Catalonia (UPC)                                                                    */
/*   Barcelona, Spain                                                                                           */
/*   E-mail: rlopez@cimne.upc.edu                                                                               */
/*                                                                                                              */
/****************************************************************************************************************/

#ifndef __FUNCTIONREGRESSIONTESTING_H__
#define __FUNCTIONREGRESSIONTESTING_H__

// OpenNN includes

#include "../utilities/vector.h"
#include "../utilities/matrix.h"

#include "../data_set/data_set.h"

#include "../neural_network/neural_network.h"

namespace OpenNN
{

/// This class contains methods for testing function regression problems.
/// It includes linear regression analysis or error data analysis. 

class FunctionRegressionTesting 
{

public:  

   // DEFAULT CONSTRUCTOR

   explicit FunctionRegressionTesting(void);

   // NEURAL NETWORK CONSTRUCTOR

   explicit FunctionRegressionTesting(NeuralNetwork*);

   // DATA SET CONSTRUCTOR

   explicit FunctionRegressionTesting(DataSet*);

   // GENERAL CONSTRUCTOR

   explicit FunctionRegressionTesting(NeuralNetwork*, DataSet*);

   // XML CONSTRUCTOR

   explicit FunctionRegressionTesting(TiXmlElement*);

   // DESTRUCTOR

   virtual ~FunctionRegressionTesting(void);

   // STRUCTURES

   ///
   /// Structure with the results from a linear regression analysis. 
   ///

   struct LinearRegressionAnalysisResults
   {
      /// Intercept, slope and correlation coefficient for each output variable. 

      Vector< Vector<double> > linear_regression_parameters;

      /// Scaled target data from data set and output data from neural network. 

      Vector< Matrix<double> > scaled_target_output_data;

      void save(const std::string&) const;
   };

   // METHODS

   // Get methods

   NeuralNetwork* get_neural_network_pointer(void) const;
   DataSet* get_data_set_pointer(void) const;

   const bool& get_display(void) const;

   // Set methods

   void set_neural_network_pointer(NeuralNetwork*);
   void set_data_set_pointer(DataSet*);

   void set_display(const bool&);
    
   void set_default(void);

   void check(void) const;

   // Testing targets outputs data methods

   Vector< Matrix<double> > calculate_scaled_target_output_data(void) const;

   // Linear regression parameters methods

   Vector< Vector<double> > calculate_linear_regression_parameters(void) const;

   // Linear regression analysis methods

   LinearRegressionAnalysisResults perform_linear_regression_analysis(void) const;

   // Error data methods

   Vector< Matrix<double> > calculate_error_data(void) const;

   Vector< Vector< Vector<double> > > calculate_error_data_statistics(void) const;

   Vector< Vector< Vector<double> > > calculate_error_data_histogram(const unsigned int&) const;
   Vector< Vector< Vector<double> > > calculate_error_data_histogram(void) const;

   // Serialization methods 

   std::string to_string(void) const;

   virtual TiXmlElement* to_XML(void) const;
   virtual void from_XML(TiXmlElement*);

   // HTML methods

//   TiXmlElement* perform_linear_regression_analysis_XML(void) const;

//   TiXmlElement* calculate_error_data_XML(void) const;
//   TiXmlElement* calculate_error_data_statistics_XML(void) const;
//   TiXmlElement* calculate_error_data_histogram_XML(void) const;

private: 

   /// Pointer to the neural network object to be tested. 

   NeuralNetwork* neural_network_pointer;

   /// Pointer to the data set object on which the neural network is tested. 

   DataSet* data_set_pointer;

   /// Display messages from this object. 

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
