/****************************************************************************************************************/
/*                                                                                                              */
/*   OpenNN: Open Neural Networks Library                                                                       */
/*   www.opennn.cimne.com                                                                                       */
/*                                                                                                              */
/*   C O N D I T I O N S   L A Y E R   C L A S S   H E A D E R                                                  */
/*                                                                                                              */
/*   Roberto Lopez                                                                                              */
/*   International Center for Numerical Methods in Engineering (CIMNE)                                          */
/*   Technical University of Catalonia (UPC)                                                                    */
/*   Barcelona, Spain                                                                                           */
/*   E-mail: rlopez@cimne.upc.edu                                                                               */
/*                                                                                                              */
/****************************************************************************************************************/

#ifndef __CONDITIONSLAYER_H__
#define __CONDITIONSLAYER_H__

// System includes

#include <string>

// OpenNN includes

#include "../utilities/vector.h"
#include "../utilities/matrix.h"

// TinyXml includes

#include "../../tinyxml/tinyxml.h"

namespace OpenNN
{

/// This class represents a layer of conditions. 
/// Conditions are used to ensure that given outputs are provided for given inputs. 
/// Note that this is quite a difficult topic in the field of neural networks. 

class ConditionsLayer
{

public:

   // DEFAULT CONSTRUCTOR

   explicit ConditionsLayer(void);

   // INPUTS-CONDITIONS NUMBER CONSTRUCTOR

   explicit ConditionsLayer(const unsigned int&, const unsigned int&);

   // XML CONSTRUCTOR

   explicit ConditionsLayer(TiXmlElement*);

   // COPY CONSTRUCTOR

   ConditionsLayer(const ConditionsLayer&);

   // DESTRUCTOR

   virtual ~ConditionsLayer(void);

   // ASSIGNMENT OPERATOR

   ConditionsLayer& operator = (const ConditionsLayer&);

   // EQUAL TO OPERATOR

   bool operator == (const ConditionsLayer&) const;

   // ENUMERATIONS

   /// Enumeration of available methods for applying conditions.
   
   enum ConditionsMethod{OneCondition, TwoConditions, UserConditionsMethod};

   // GET METHODS

   const unsigned int& get_external_inputs_number(void) const;
   const unsigned int& get_conditions_neurons_number(void) const;

   const ConditionsMethod& get_conditions_method(void) const;
   std::string write_conditions_method(void) const;

   const Vector<double>& get_external_input_values(void) const;
   const double& get_external_input_value(const unsigned int&) const;
   
   const Matrix<double>& get_output_values(void) const;
   const double& get_output_value(const unsigned int&, const unsigned int&) const;

   const bool& get_display(void) const;

   // SET METHODS

   void set(void);
   void set(const unsigned int&, const unsigned int&);
   void set(const ConditionsLayer&);

   void set_external_inputs_number(const unsigned int&);
   void set_conditions_neurons_number(const unsigned int&);

   void set_conditions_method(const ConditionsMethod&);
   void set_conditions_method(const std::string&);

   void set_external_input_values(const Vector<double>&);
   void set_external_input_value(const unsigned int&, const double&);
   
   void set_output_values(const Matrix<double>&);
   void set_output_value(const unsigned int&, const unsigned int&, const double&);

   void set_display(const bool&);

   void set_default(void);

   // Conditions 

   void initialize_random(void);

   void check(void) const;

   // Particular solution

   /// This method returns the particular solution for applying boundary conditions.
   virtual Vector<double> calculate_particular_solution(const Vector<double>&) const;

   /// This method returns the particular solution Jacobian for applying boundary conditions.
   virtual Matrix<double> calculate_particular_solution_Jacobian(const Vector<double>&) const;

   virtual Vector< Matrix<double> > calculate_particular_solution_Hessian_form(const Vector<double>&) const;

   // Homogeneous solution

   /// This method returns the homogeneous solution for applying boundary conditions.
   virtual Vector<double> calculate_homogeneous_solution(const Vector<double>&) const;

   /// This method returns the homogeneous solution Jacobian for applying boundary conditions.
   virtual Matrix<double> calculate_homogeneous_solution_Jacobian(const Vector<double>&) const;

   virtual Vector< Matrix<double> > calculate_homogeneous_solution_Hessian_form(const Vector<double>&) const;

   Vector<double> calculate_outputs(const Vector<double>&, const Vector<double>&) const;

   Matrix<double> calculate_Jacobian(const Vector<double>&, const Vector<double>&, const Matrix<double>&) const;

   Vector< Matrix<double> > calculate_Hessian_form(const Vector<double>&, const Vector<double>&) const;

   // One condition methods

   Vector<double> calculate_one_condition_particular_solution(const Vector<double>&) const;
   Matrix<double> calculate_one_condition_particular_solution_Jacobian(const Vector<double>&) const;
   Vector< Matrix<double> > calculate_one_condition_particular_solution_Hessian_form(const Vector<double>&) const;

   Vector<double> calculate_one_condition_homogeneous_solution(const Vector<double>&) const;
   Matrix<double> calculate_one_condition_homogeneous_solution_Jacobian(const Vector<double>&) const;
   Vector< Matrix<double> > calculate_one_condition_homogeneous_solution_Hessian_form(const Vector<double>&) const;

   // Two conditions methods

   Vector<double> calculate_two_conditions_particular_solution(const Vector<double>&) const;
   Matrix<double> calculate_two_conditions_particular_solution_Jacobian(const Vector<double>&) const;
   Vector< Matrix<double> > calculate_two_conditions_particular_solution_Hessian_form(const Vector<double>&) const;

   Vector<double> calculate_two_conditions_homogeneous_solution(const Vector<double>&) const;
   Matrix<double> calculate_two_conditions_homogeneous_solution_Jacobian(const Vector<double>&) const;
   Vector< Matrix<double> > calculate_two_conditions_homogeneous_solution_Hessian_form(const Vector<double>&) const;

   // Expression methods

   virtual std::string write_particular_solution_expression(const Vector<std::string>&, const Vector<std::string>&) const;
   virtual std::string write_homogeneous_solution_expression(const Vector<std::string>&, const Vector<std::string>&) const;

   std::string write_one_condition_particular_solution_expression(const Vector<std::string>&, const Vector<std::string>&) const;
   std::string write_one_condition_homogeneous_solution_expression(const Vector<std::string>&, const Vector<std::string>&) const;
   
   std::string write_two_conditions_particular_solution_expression(const Vector<std::string>&, const Vector<std::string>&) const;
   std::string write_two_conditions_homogeneous_solution_expression(const Vector<std::string>&, const Vector<std::string>&) const;

   std::string write_output_expression(const Vector<std::string>&, const Vector<std::string>&, const Vector<std::string>&, const Vector<std::string>&) const;

   std::string write_expression(const Vector<std::string>&, const Vector<std::string>&, const Vector<std::string>&) const;

   // Serialization methods

   std::string to_string(void) const;

   TiXmlElement* to_XML(void) const;
   void from_XML(TiXmlElement*);

protected:

   /// Number of inputs to the conditions layer. 
   /// This will be the number of inputs to the neural network. 

   unsigned int external_inputs_number;

   /// Number of conditions neurons.
   /// This will be the number of outputs from the neural network. 

   unsigned int conditions_neurons_number;

   /// Conditions method (one or two conditions).

   ConditionsMethod conditions_method;

   /// Independent values of the conditions.

   Vector<double> external_input_values;

   /// Dependent values of the conditions.

   Matrix<double> output_values;

   /// Display messages to screen. 

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

