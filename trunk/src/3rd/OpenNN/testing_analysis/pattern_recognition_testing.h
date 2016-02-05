/****************************************************************************************************************/
/*                                                                                                              */
/*   OpenNN: Open Neural Networks Library                                                                       */
/*   www.opennn.cimne.com                                                                                       */
/*                                                                                                              */
/*   P A T T E R N   R E C O G N I T I O N   T E S T I N G   C L A S S   H E A D E R                            */
/*                                                                                                              */
/*   Roberto Lopez                                                                                              */ 
/*   International Center for Numerical Methods in Engineering (CIMNE)                                          */
/*   Technical University of Catalonia (UPC)                                                                    */
/*   Barcelona, Spain                                                                                           */
/*   E-mail: rlopez@cimne.upc.edu                                                                               */ 
/*                                                                                                              */
/****************************************************************************************************************/

#ifndef __PATTERNRECOGNITIONTESTING_H__
#define __PATTERNRECOGNITIONTESTING_H__

// System includes

#include "../utilities/vector.h"
#include "../utilities/matrix.h"
#include "../data_set/data_set.h"

#include "../neural_network/neural_network.h"

namespace OpenNN
{

/// This class contains some useful methods for pattern recognition (or classification) problem testing.
/// That includes binary classification tests or confusion. 

class PatternRecognitionTesting 
{

public:  

   // DEFAULT CONSTRUCTOR

   explicit PatternRecognitionTesting(void);

   // NEURAL NETWORK CONSTRUCTOR

   explicit PatternRecognitionTesting(NeuralNetwork*);

   // DATA SET CONSTRUCTOR

   explicit PatternRecognitionTesting(DataSet*);

   // GENERAL CONSTRUCTOR

   explicit PatternRecognitionTesting(NeuralNetwork*, DataSet*);

   // XML CONSTRUCTOR

   explicit PatternRecognitionTesting(TiXmlElement*);

   // DESTRUCTOR

   virtual ~PatternRecognitionTesting(void);

   /// Structure with the results from a linear regression analysis. 

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

   // Output data methods

   Matrix<bool> calculate_binary_output_data(void) const;

   Matrix<bool> calculate_binary_training_output_data(void) const;
   Matrix<bool> calculate_binary_generalization_output_data(void) const;
   Matrix<bool> calculate_binary_testing_output_data(void) const;

   // Target data methods

   Matrix<bool> calculate_binary_target_data(void) const;

   Matrix<bool> calculate_binary_training_target_data(void) const;
   Matrix<bool> calculate_binary_generalization_target_data(void) const;
   Matrix<bool> calculate_binary_testing_target_data(void) const;
  
   // Test methods
   
   Vector<double> calculate_binary_classification_test(void) const;

   Matrix<unsigned int> calculate_confusion(void) const;

   void save_binary_classification_test(const std::string&) const;
   void save_confusion(const std::string&) const;   

   // Serialization methods 

   std::string to_string(void) const;

   virtual TiXmlElement* to_XML(void) const;
   virtual void from_XML(TiXmlElement*);


private: 

   /// Pointer to the neural network object to be tested. 

	NeuralNetwork* neural_network_pointer;

    /// Pointer to the data set object on which the neural network is tested. 

	DataSet* data_set_pointer;

    /// Display.

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

