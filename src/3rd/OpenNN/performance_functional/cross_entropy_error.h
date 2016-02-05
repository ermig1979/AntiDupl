/****************************************************************************************************************/
/*                                                                                                              */
/*   OpenNN: Open Neural Networks Library                                                                       */
/*   www.opennn.cimne.com                                                                                       */
/*                                                                                                              */
/*   C R O S S   E N T R O P Y   E R R O R   C L A S S   H E A D E R                                            */
/*                                                                                                              */
/*   Roberto Lopez                                                                                              */
/*   International Center for Numerical Methods in Engineering (CIMNE)                                          */
/*   Technical University of Catalonia (UPC)                                                                    */
/*   Barcelona, Spain                                                                                           */
/*   E-mail: rlopez@cimne.upc.edu                                                                               */
/*                                                                                                              */
/****************************************************************************************************************/

#ifndef __CROSSENTROPYERROR_H__
#define __CROSSENTROPYERROR_H__

// OpenNN includes

#include "performance_term.h"
#include "../data_set/data_set.h"

namespace OpenNN
{

/// This class represents the cross entropy performance term. 
/// This functional is used in pattern recognition problems.

class CrossEntropyError : public PerformanceTerm
{

public:

   // DEFAULT CONSTRUCTOR

   explicit CrossEntropyError(void);

   // NEURAL NETWORK CONSTRUCTOR

   explicit CrossEntropyError(NeuralNetwork*);

   // DATA SET CONSTRUCTOR

   explicit CrossEntropyError(DataSet*);

   // GENERAL CONSTRUCTOR

   explicit CrossEntropyError(NeuralNetwork*, DataSet*);

   // XML CONSTRUCTOR

   explicit CrossEntropyError(TiXmlElement*);

   // COPY CONSTRUCTOR

   CrossEntropyError(const CrossEntropyError&);

   // DESTRUCTOR

   virtual ~CrossEntropyError(void);

   // ASSIGNMENT OPERATOR

   CrossEntropyError& operator = (const CrossEntropyError&);

   // EQUAL TO OPERATOR

   bool operator == (const CrossEntropyError&) const;


   // METHODS

   // Checking methods

   void check(void) const;

   // Evaluation methods

   double calculate_evaluation(void) const;   

   double calculate_evaluation(const Vector<double>&) const;
   
   double calculate_minimum_objective(void);

   double calculate_generalization_evaluation(void) const;
   double calculate_minimum_generalization_objective(void);

   Vector<double> calculate_gradient(void) const;
   Matrix<double> calculate_Hessian(void) const;

   std::string write_performance_term_type(void) const;

   // Serialization methods

   TiXmlElement* to_XML(void) const;   
   void from_XML(TiXmlElement*);   

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
