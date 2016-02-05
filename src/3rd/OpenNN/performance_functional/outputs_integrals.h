/****************************************************************************************************************/
/*                                                                                                              */
/*   OpenNN: Open Neural Networks Library                                                                       */
/*   www.opennn.cimne.com                                                                                       */
/*                                                                                                              */
/*   O U T P U T S   I N T E G R A L S   C L A S S   H E A D E R                                                */
/*                                                                                                              */
/*   Roberto Lopez                                                                                              */
/*   International Center for Numerical Methods in Engineering (CIMNE)                                          */
/*   Technical University of Catalonia (UPC)                                                                    */
/*   Barcelona, Spain                                                                                           */
/*   E-mail: rlopez@cimne.upc.edu                                                                               */
/*                                                                                                              */
/****************************************************************************************************************/

#ifndef __OUTPUTSINTEGRALS_H__
#define __OUTPUTSINTEGRALS_H__

// OpenNN includes

#include "performance_functional.h"
#include "../utilities/numerical_integration.h"

namespace OpenNN
{

/// This class represents the outputs integrals performance term. 
/// It is defined as the weighted sum of the integrals of the neural network outputs.
/// The neural network here must have only one input. 
/// This performance term might be used in optimal control as an objective or a regularization terms. 

class OutputsIntegrals : public PerformanceTerm
{

public:

   // DEFAULT CONSTRUCTOR

   explicit OutputsIntegrals(void);

   // NEURAL NETWORK CONSTRUCTOR

   explicit OutputsIntegrals(NeuralNetwork*);

   // XML CONSTRUCTOR

   explicit OutputsIntegrals(TiXmlElement*);

   // DESTRUCTOR

   virtual ~OutputsIntegrals(void);    

   // METHODS

   // Get methods

   const NumericalIntegration& get_numerical_integration(void) const;
   NumericalIntegration* get_numerical_integration_pointer(void);

   const Vector<double>& get_outputs_integrals_weights(void) const;
   const double& get_output_integral_weight(const unsigned int&) const;

   // Set methods

   void set_numerical_integration(const NumericalIntegration&);

   void set_outputs_integrals_weights(const Vector<double>&);
   void set_output_integral_weight(const unsigned int&, const double&);

   void set_default(void);

   // Checking methods

   void check(void) const;

   // Regularization methods

   double calculate_evaluation(void) const;   
   double calculate_evaluation(const Vector<double>&) const;

   Vector<double> calculate_gradient(void) const;

   Matrix<double> calculate_Hessian(void) const;

   std::string write_performance_term_type(void) const;

   // Serialization methods

   TiXmlElement* to_XML(void) const;   

   void from_XML(TiXmlElement*);

private:

   /// Object for numerical integration of functions. 

   NumericalIntegration numerical_integration;

   /// Weigth for each output integral. 

   Vector<double> outputs_integrals_weights;

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
