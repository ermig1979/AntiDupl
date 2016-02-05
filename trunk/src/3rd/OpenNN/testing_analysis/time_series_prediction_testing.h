/****************************************************************************************************************/
/*                                                                                                              */
/*   OpenNN: Open Neural Networks Library                                                                       */
/*   www.opennn.cimne.com                                                                                       */
/*                                                                                                              */
/*   T I M E   S E R I E S   P R E D I C T I O N   T E S T I N G   C L A S S   H E A D E R                      */
/*                                                                                                              */
/*   Roberto Lopez                                                                                              */
/*   International Center for Numerical Methods in Engineering (CIMNE)                                          */
/*   Technical University of Catalonia (UPC)                                                                    */
/*   Barcelona, Spain                                                                                           */
/*   E-mail: rlopez@cimne.upc.edu                                                                               */
/*                                                                                                              */
/****************************************************************************************************************/

#ifndef __TIMESERIESPREDICTIONTESTING_H__
#define __TIMESERIESPREDICTIONTESTING_H__

// OpenNN includes

#include "../utilities/vector.h"
#include "../utilities/matrix.h"
#include "../data_set/data_set.h"
#include "../neural_network/neural_network.h"

namespace OpenNN
{

///
/// This class contains methods for testing time series prediction problems.
///

class TimeSeriesPredictionTesting 
{

public:  

   // DEFAULT CONSTRUCTOR

   explicit TimeSeriesPredictionTesting(void);

   // NEURAL NETWORK CONSTRUCTOR

   explicit TimeSeriesPredictionTesting(NeuralNetwork*);

   // DATA SET CONSTRUCTOR

   explicit TimeSeriesPredictionTesting(DataSet*);

   // GENERAL CONSTRUCTOR

   explicit TimeSeriesPredictionTesting(NeuralNetwork*, DataSet*);

   // XML CONSTRUCTOR

   explicit TimeSeriesPredictionTesting(TiXmlElement*);

   // DESTRUCTOR

   virtual ~TimeSeriesPredictionTesting(void);

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

   // Serialization methods 

   std::string to_string(void) const;

   virtual TiXmlElement* to_XML(void) const;
   virtual void from_XML(TiXmlElement*);


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
