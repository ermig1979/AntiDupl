/****************************************************************************************************************/
/*                                                                                                              */
/*   OpenNN: Open Neural Networks Library                                                                       */
/*   www.opennn.cimne.com                                                                                       */
/*                                                                                                              */
/*   M I N K O W S K I   E R R O R   C L A S S   H E A D E R                                                    */
/*                                                                                                              */
/*   Roberto Lopez                                                                                              */
/*   International Center for Numerical Methods in Engineering (CIMNE)                                          */
/*   Technical University of Catalonia (UPC)                                                                    */
/*   Barcelona, Spain                                                                                           */
/*   E-mail: rlopez@cimne.upc.edu                                                                               */
/*                                                                                                              */
/****************************************************************************************************************/

#ifndef __MINKOWSKIERROR_H__
#define __MINKOWSKIERROR_H__

// OpenNN includes

#include "performance_functional.h"
#include "../data_set/data_set.h"

namespace OpenNN
{

/// This class represents the Minkowski error performance term. 
/// The Minkowski error measures the difference between the outputs of a neural network and the targets in a data set. 
/// This performance term is used in data modeling problems.
/// It can be more useful when the data set presents outliers. 

class MinkowskiError : public PerformanceTerm
{

public:

   // DEFAULT CONSTRUCTOR

   explicit MinkowskiError(void);

   // NEURAL NETWORK CONSTRUCTOR

   explicit MinkowskiError(NeuralNetwork*);

   // DATA SET CONSTRUCTOR

   explicit MinkowskiError(DataSet*);

   // NEURAL NETWORK AND DATA SET CONSTRUCTOR

   explicit MinkowskiError(NeuralNetwork*, DataSet*);

   // XML CONSTRUCTOR

   explicit MinkowskiError(TiXmlElement*);

   // DESTRUCTOR

   virtual ~MinkowskiError(void);


   // METHODS

   // Get methods

   double get_Minkowski_parameter(void) const;

   // Set methods

   void set_default(void);

   void set_Minkowski_parameter(const double&);

   // Checking methods

   void check(void) const;

   // Evaluation methods

   double calculate_evaluation(void) const;
   double calculate_evaluation(const Vector<double>&) const;   
   double calculate_generalization_evaluation(void) const;   

   Vector<double> calculate_gradient(void) const;
   Matrix<double> calculate_Hessian(void) const;

   std::string write_performance_term_type(void) const;

   // Serialization methods

   TiXmlElement* to_XML(void) const;   
   void from_XML(TiXmlElement*);   

   int calculate_sign(const double&) const;

private:

   // MEMBERS

   /// Minkowski exponent value.

   double Minkowski_parameter;

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
