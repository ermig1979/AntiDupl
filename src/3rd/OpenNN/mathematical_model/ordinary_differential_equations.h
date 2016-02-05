/****************************************************************************************************************/
/*                                                                                                              */
/*   OpenNN: Open Neural Networks Library                                                                       */
/*   www.opennn.cimne.com                                                                                       */
/*                                                                                                              */
/*   O R D I N A R Y   D I F F E R E N T I A L   E Q U A T I O N S   C L A S S   H E A D E R                    */
/*                                                                                                              */
/*   Roberto Lopez                                                                                              */ 
/*   International Center for Numerical Methods in Engineering (CIMNE)                                          */
/*   Technical University of Catalonia (UPC)                                                                    */
/*   Barcelona, Spain                                                                                           */
/*   E-mail: rlopez@cimne.upc.edu                                                                               */ 
/*                                                                                                              */
/****************************************************************************************************************/


#ifndef __ORDINARYDIFFERENTIALEQUATIONS_H__
#define __ORDINARYDIFFERENTIALEQUATIONS_H__

// OpenNN includes

#include "../utilities/vector.h"
#include "../utilities/matrix.h"

#include "mathematical_model.h"

#include "../neural_network/neural_network.h"

namespace OpenNN
{

/// 
/// This class represents the concept of a mathematical model defined by ordinary differential equations.
/// A mathematical model is the base for learning in some types of problems, such as optimal control and inverse problems. 
/// 


class OrdinaryDifferentialEquations : public MathematicalModel
{

public:

   // DEFAULT CONSTRUCTOR

   explicit OrdinaryDifferentialEquations(void);

   // XML CONSTRUCTOR

   explicit OrdinaryDifferentialEquations(TiXmlElement*);

   // FILE CONSTRUCTOR

   explicit OrdinaryDifferentialEquations(const std::string&);

   // COPY CONSTRUCTOR

   OrdinaryDifferentialEquations(const OrdinaryDifferentialEquations&);

   // DESTRUCTOR

   virtual ~OrdinaryDifferentialEquations(void);

   // ASSIGNMENT OPERATOR

   OrdinaryDifferentialEquations& operator = (const OrdinaryDifferentialEquations&);

   // EQUAL TO OPERATOR

   bool operator == (const OrdinaryDifferentialEquations&) const;

   // Enumerations

   /// Enumeration of available methods for numerical integration of ordinary differential equations. 

   enum SolutionMethod{RungeKutta, RungeKuttaFehlberg};


   // METHODS

   // Get methods

   const double& get_initial_independent_variable(void) const;
   const double& get_final_independent_variable(void) const;

   const Vector<double>& get_initial_dependent_variables(void) const;
   const double& get_initial_dependent_variable(const unsigned int&) const;

   const SolutionMethod& get_solution_method(void) const;
   std::string write_solution_method(void) const;

   const unsigned int& get_points_number(void) const;

   const double& get_tolerance(void) const;

   const unsigned int& get_initial_size(void) const;
   const unsigned int& get_warning_size(void) const;
   const unsigned int& get_error_size(void) const;

   // Set methods

   void set(const OrdinaryDifferentialEquations&);

   void set_initial_independent_variable(const double&);
   void set_final_independent_variable(const double&);

   void set_initial_dependent_variables(const Vector<double>&);
   void set_initial_dependent_variable(const unsigned int&, const double&);

   void set_solution_method(const SolutionMethod&);
   void set_solution_method(const std::string&);

   void set_points_number(const unsigned int&);

   void set_tolerance(const double& );

   void set_initial_size(const unsigned int&);
   void set_warning_size(const unsigned int&);
   void set_error_size(const unsigned int&);

   virtual void set_default(void);

   // Ordinary differential equations methods

   /// This pure virtual method calculates the derivatives of the dependent variables with respect to the independent variable. 

   virtual Vector<double> calculate_dependent_variables_dots(const NeuralNetwork&, const Vector<double>&) const = 0;

   // Numerical solution methods

   Matrix<double> calculate_Runge_Kutta_solution(const NeuralNetwork&) const;
   Vector<double> calculate_Runge_Kutta_final_solution(const NeuralNetwork&) const;

   Matrix<double> calculate_Runge_Kutta_Fehlberg_solution(const NeuralNetwork&) const;
   Vector<double> calculate_Runge_Kutta_Fehlberg_final_solution(const NeuralNetwork&) const;

   virtual Matrix<double> calculate_solutions(const NeuralNetwork&) const;
   virtual Vector<double> calculate_final_solutions(const NeuralNetwork&) const;

   // Serialization methods

   std::string to_string(void) const;
   
   virtual TiXmlElement* to_XML(void) const;   
   virtual void from_XML(TiXmlElement*);   

   virtual void save_data(const NeuralNetwork&, const std::string&) const;

protected: 

   /// Initial value for the only independent variable. 

   double initial_independent_variable;

   /// Final value for the only independent variable. 

   double final_independent_variable;

   /// Initial values for the dependent variables. 

   Vector<double> initial_dependent_variables;

   /// Numerical integration method (Runge-Kutta or Runge-Kutta-Fehlberg).

   SolutionMethod solution_method;

   /// Number of points in the Runge-Kutta method. 

   unsigned int points_number;

   /// Tolerance in the Runge-Kutta-Fehlberg method. 

   double tolerance;

   /// Initial number of points in the Runge-Kutta-Fehlberg method. 

   unsigned int initial_size;

   /// Number of points at which the the Runge-Kutta-Fehlberg method displays a warning message. 

   unsigned int warning_size;

   /// Number of points at which the the Runge-Kutta-Fehlberg method throws an exception. 

   unsigned int error_size;
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
