/****************************************************************************************************************/
/*                                                                                                              */
/*   OpenNN: Open Neural Networks Library                                                                       */
/*   www.opennn.cimne.com                                                                                       */
/*                                                                                                              */
/*   S O L U T I O N S   E R R O R   C L A S S   H E A D E R                                                    */
/*                                                                                                              */
/*   Roberto Lopez                                                                                              */
/*   International Center for Numerical Methods in Engineering (CIMNE)                                          */
/*   Technical University of Catalonia (UPC)                                                                    */
/*   Barcelona, Spain                                                                                           */
/*   E-mail: rlopez@cimne.upc.edu                                                                               */
/*                                                                                                              */
/****************************************************************************************************************/

#ifndef __SOLUTIONSERROR_H__
#define __SOLUTIONSERROR_H__

// OpenNN includes

#include "../utilities/numerical_integration.h"

#include "performance_functional.h"

namespace OpenNN
{

/// This class implements a performance term called solutions error. 
/// This performance term is measured on a mathematical model. 
/// It computes the difference between the solution from a mathematical model and a given target solution. 
/// This type of functional is very used in optimal control problems, in which the neural network represents the control variables. 
/// The first and second derivatives of this type of performance term are calculated with numerical differentiation. 

class SolutionsError : public PerformanceTerm
{

public:

   // DEFAULT CONSTRUCTOR

   explicit SolutionsError(void);

   // NEURAL NETWORK CONSTRUCTOR

   explicit SolutionsError(NeuralNetwork*);

   // DATA SET CONSTRUCTOR

   explicit SolutionsError(MathematicalModel*);

   // GENERAL CONSTRUCTOR

   explicit SolutionsError(NeuralNetwork*, MathematicalModel*);

   // XML CONSTRUCTOR

   explicit SolutionsError(TiXmlElement*);

   // COPY CONSTRUCTOR

   SolutionsError(const SolutionsError&);

   // ASSIGNMENT OPERATOR

   SolutionsError& operator = (const SolutionsError&);

   // EQUAL TO OPERATOR

   bool operator == (const SolutionsError&) const;

   // DESTRUCTOR

   virtual ~SolutionsError(void);

   // ENUMERATIONS

   /// Enumeration of the different methods to compute the errors between the mathematical model solutions and the target solutions. 

   enum SolutionsErrorMethod{SolutionsErrorSum, SolutionsErrorIntegral};

   // METHODS

   // Get methods

   const SolutionsErrorMethod& get_solutions_error_method(void) const;
   std::string write_solutions_error_method(void) const;

   const Vector<double>& get_solutions_errors_weights(void) const;
   const double& get_solution_error_weight(const unsigned int&) const;

   // Set methods

   void set(const SolutionsError&);

   void set_solutions_error_method(const SolutionsErrorMethod&);
   void set_solutions_error_method(const std::string&);

   void set_solutions_errors_weights(const Vector<double>&);
   void set_solution_error_weight(const unsigned int&, const double&);

   void set_default(void);

   // Target trajectories methods 

   virtual Matrix<double> calculate_target_dependent_variables(const Matrix<double>&) const;

   // Checking methods

   void check(void) const;

   // Constraints methods

   double calculate_solutions_error_sum(void) const;   
   double calculate_solutions_error_integral(void) const;   

   virtual double calculate_evaluation(void) const;   
   virtual double calculate_evaluation(const Vector<double>&) const;   

   std::string write_performance_term_type(void) const;

   std::string write_information(void) const;

   // Serialization methods

   virtual void print(void) const;

   TiXmlElement* to_XML(void) const;   
   void from_XML(TiXmlElement*);

protected:

   /// Numerical integration object. 

   NumericalIntegration numerical_integration;

   /// Method used to compute the error between the mathematical model solutions and the target solutions. 

   SolutionsErrorMethod solutions_error_method;

   /// Weights for the different dependent variables errors. 

   Vector<double> solutions_errors_weights;

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
