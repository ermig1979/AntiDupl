/****************************************************************************************************************/
/*                                                                                                              */
/*   OpenNN: Open Neural Networks Library                                                                       */
/*   www.opennn.cimne.com                                                                                       */
/*                                                                                                              */
/*   N O R M A L I Z E D   S Q U A R E D   E R R O R   C L A S S   H E A D E R                                  */
/*                                                                                                              */
/*   Roberto Lopez                                                                                              */ 
/*   International Center for Numerical Methods in Engineering (CIMNE)                                          */
/*   Technical University of Catalonia (UPC)                                                                    */
/*   Barcelona, Spain                                                                                           */
/*   E-mail: rlopez@cimne.upc.edu                                                                               */
/*                                                                                                              */
/****************************************************************************************************************/

#ifndef __NORMALIZEDSQUAREDERROR_H__
#define __NORMALIZEDSQUAREDERROR_H__

// OpenNN includes

#include "performance_term.h"
#include "../data_set/data_set.h"

namespace OpenNN
{

/// This class represents the normalized squared error performance term. 
/// This performance term is used in data modeling problems.
/// If it has a value of unity then the neural network is predicting the data "in the mean",
/// A value of zero means perfect prediction of data.

class NormalizedSquaredError : public PerformanceTerm
{

public:

   // GENERAL CONSTRUCTOR

   explicit NormalizedSquaredError(NeuralNetwork*, DataSet*);

   // NEURAL NETWORK CONSTRUCTOR

   explicit NormalizedSquaredError(NeuralNetwork*);

   // DATA SET CONSTRUCTOR

   explicit NormalizedSquaredError(DataSet*);

   // DEFAULT CONSTRUCTOR

   explicit NormalizedSquaredError(void);

   // XML CONSTRUCTOR

   explicit NormalizedSquaredError(TiXmlElement*);

   // DESTRUCTOR

   virtual ~NormalizedSquaredError(void);

   // METHODS

   // Get methods

   // Set methods

   // Normalization coefficients 

   double calculate_normalization_coefficient(const Matrix<double>&, const Vector<double>&) const;

   // Checking methods

   void check(void) const;

   // Evaluation methods

   double calculate_evaluation(void) const;
   Vector<double> calculate_gradient(void) const;
   Matrix<double> calculate_Hessian(void) const;

   double calculate_evaluation(const Vector<double>&) const;

   double calculate_generalization_evaluation(void) const;   

   // Objective terms methods

   Vector<double> calculate_evaluation_terms(void) const;
   Vector<double> calculate_evaluation_terms(const Vector<double>&) const;

   Matrix<double> calculate_Jacobian_terms(void) const;

   PerformanceTerm::FirstOrderEvaluationTerms calculate_first_order_evaluation_terms(void);

   // Squared errors methods

   Vector<double> calculate_squared_errors(void) const;

   std::string write_performance_term_type(void) const;

   // Serialization methods

   TiXmlElement* to_XML(void) const;   

   void from_XML(TiXmlElement*);


private:

   // MEMBERS

   /// Mean values of all the target variables. 

   Vector<double> training_target_mean;
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

