/****************************************************************************************************************/
/*                                                                                                              */
/*   OpenNN: Open Neural Networks Library                                                                       */
/*   www.opennn.cimne.com                                                                                       */
/*                                                                                                              */
/*   M E A N   S Q U A R E D   E R R O R    C L A S S   H E A D E R                                             */
/*                                                                                                              */
/*   Roberto Lopez                                                                                              */
/*   International Center for Numerical Methods in Engineering (CIMNE)                                          */
/*   Technical University of Catalonia (UPC)                                                                    */
/*   Barcelona, Spain                                                                                           */
/*   E-mail: rlopez@cimne.upc.edu                                                                               */
/*                                                                                                              */
/****************************************************************************************************************/


#ifndef __MEANSQUAREDERROR_H__
#define __MEANSQUAREDERROR_H__

// OpenNN includes

#include "performance_term.h"
#include "../data_set/data_set.h"

namespace OpenNN
{

/// This class represents the mean squared error performance term. 
/// The mean squared error measures the difference between the outputs from a neural network and the targets in a data set. 
/// This functional is used in data modeling problems, such as function regression, 
/// pattern recognition and time series prediction.

class MeanSquaredError : public PerformanceTerm
{

public:

   // DEFAULT CONSTRUCTOR

   explicit MeanSquaredError(void);

   // NEURAL NETWORK CONSTRUCTOR

   explicit MeanSquaredError(NeuralNetwork*);

   // DATA SET CONSTRUCTOR

   explicit MeanSquaredError(DataSet*);

   // GENERAL CONSTRUCTOR

   explicit MeanSquaredError(NeuralNetwork*, DataSet*);

   // XML CONSTRUCTOR

   explicit MeanSquaredError(TiXmlElement*);

   // COPY CONSTRUCTOR

   MeanSquaredError(const MeanSquaredError&);

   // DESTRUCTOR

   virtual ~MeanSquaredError(void);

   // STRUCTURES


   // METHODS

   // Get methods

   // Set methods

   // Checking methods

   void check(void) const;

   // Objective methods

   double calculate_evaluation(void) const;
   double calculate_evaluation(const Vector<double>&) const;   
   double calculate_generalization_evaluation(void) const;

   Vector<double> calculate_gradient(void) const;

   Matrix<double> calculate_Hessian(void) const;

   FirstOrderEvaluation calculate_first_order_evaluation(void) const;
   SecondOrderEvaluation calculate_second_order_evaluation(void) const;

   // Objective terms methods

   Vector<double> calculate_evaluation_terms(void) const;
   Vector<double> calculate_evaluation_terms(const Vector<double>&) const;

   Matrix<double> calculate_Jacobian_terms(void) const;

   FirstOrderEvaluationTerms calculate_first_order_evaluation_terms(void);

   std::string write_performance_term_type(void) const;

   // Serialization methods

   TiXmlElement* to_XML(void) const;   


private:


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
