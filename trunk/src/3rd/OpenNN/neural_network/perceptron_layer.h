/****************************************************************************************************************/
/*                                                                                                              */
/*   OpenNN: Open Neural Networks Library                                                                       */
/*   www.opennn.cimne.com                                                                                       */
/*                                                                                                              */
/*   P E R C E P T R O N   L A Y E R   C L A S S   H E A D E R                                                  */
/*                                                                                                              */
/*   Roberto Lopez                                                                                              */
/*   International Center for Numerical Methods in Engineering (CIMNE)                                          */
/*   Technical University of Catalonia (UPC)                                                                    */
/*   Barcelona, Spain                                                                                           */
/*   E-mail: rlopez@cimne.upc.edu                                                                               */
/*                                                                                                              */
/****************************************************************************************************************/

#ifndef __PERCEPTRONLAYER_H__
#define __PERCEPTRONLAYER_H__

// System includes

#include <string>

// OpenNN includes

#include "perceptron.h"

#include "../utilities/vector.h"
#include "../utilities/matrix.h"

namespace OpenNN
{

/// This class represents a layer of perceptrons.
/// Layers of perceptrons will be used to construct multilayer perceptrons. 

class PerceptronLayer
{

public:

   // DEFAULT CONSTRUCTOR

   explicit PerceptronLayer(void);

   // ARCHITECTURE CONSTRUCTOR 

   explicit PerceptronLayer(const unsigned int&, const unsigned int&);

   // COPY CONSTRUCTOR

   PerceptronLayer(const PerceptronLayer&);

   // DESTRUCTOR
   
   virtual ~PerceptronLayer(void);

   // ASSIGNMENT OPERATOR

   PerceptronLayer& operator = (const PerceptronLayer&);

   // EQUAL TO OPERATOR

   bool operator == (const PerceptronLayer&) const;

   // GET METHODS

   bool is_empty(void) const;

   const Vector<Perceptron>& get_perceptrons(void) const;
   const Perceptron& get_perceptron(const unsigned int&) const;

   unsigned int count_inputs_number(void) const;
   unsigned int count_perceptrons_number(void) const;

   // PerceptronLayer parameters

   Vector<double> arrange_biases(void) const;
   Matrix<double> arrange_synaptic_weights(void) const;

   unsigned int count_parameters_number(void) const;
   Vector<double> arrange_parameters(void) const;

   Vector<unsigned int> count_cumulative_parameters_number(void) const;

   // Activation functions

   const Perceptron::ActivationFunction& get_activation_function(void) const;

   std::string write_activation_function_name(void) const;

   // Display messages

   const bool& get_display(void) const;

   // SET METHODS

   void set(void);
   void set(const Vector<Perceptron>&);
   void set(const unsigned int&, const unsigned int&);
   void set(const PerceptronLayer&);

   void set_default(void);

   // Architecture

   void set_inputs_number(const unsigned int&);
   void set_perceptrons_number(const unsigned int&);

   void set_perceptrons(const Vector<Perceptron>&);
   void set_perceptron(const unsigned int&, const Perceptron&);

   // PerceptronLayer parameters

   void set_biases(const Vector<double>&);
   void set_synaptic_weights(const Matrix<double>&);

   void set_parameters(const Vector<double>&);

   // Activation functions

   void set_activation_function(const Perceptron::ActivationFunction&);
   void set_activation_function(const std::string&);

   // Display messages

   void set_display(const bool&);

   // Growing and pruning

   void grow_input(void);
   void grow_perceptron(void);

   void prune_input(const unsigned int&);
   void prune_perceptron(const unsigned int&);

   // PerceptronLayer initialization methods

   void initialize_random(void);

   // Parameters initialization methods

   void initialize_biases(const double&); 
   void initialize_synaptic_weights(const double&);

   void initialize_parameters(const double&);

   void initialize_parameters_uniform(void);
   void initialize_parameters_uniform(const double&, const double&);
   void initialize_parameters_uniform(const Vector<double>&, const Vector<double>&);
   void initialize_parameters_uniform(const Vector< Vector<double> >&);

   void initialize_parameters_normal(void);
   void initialize_parameters_normal(const double&, const double&);
   void initialize_parameters_normal(const Vector<double>&, const Vector<double>&);
   void initialize_parameters_normal(const Vector< Vector<double> >&);

   // Parameters norm 

   double calculate_parameters_norm(void) const;

   // PerceptronLayer combination

   Vector<double> calculate_combination(const Vector<double>&) const; 
   Matrix<double> calculate_combination_Jacobian(const Vector<double>&) const;
   Vector< Matrix<double> > calculate_combination_Hessian_form(const Vector<double>&) const;

   Vector<double> calculate_combination_parameters(const Vector<double>&, const Vector<double>&) const; 
   Matrix<double> calculate_combination_parameters_Jacobian(const Vector<double>&, const Vector<double>&) const; 
   Vector< Matrix<double> > calculate_combination_parameters_Hessian_form(const Vector<double>&, const Vector<double>&) const; 

   // PerceptronLayer activation 

   Vector<double> calculate_activation(const Vector<double>&) const;
   Vector<double> calculate_activation_derivative(const Vector<double>&) const;
   Vector<double> calculate_activation_second_derivative(const Vector<double>&) const;

   Matrix<double> arrange_activation_Jacobian(const Vector<double>&) const;
   Vector< Matrix<double> > arrange_activation_Hessian_form(const Vector<double>&) const;

   // PerceptronLayer outputs 

   Vector<double> calculate_outputs(const Vector<double>&) const;
   Matrix<double> calculate_Jacobian(const Vector<double>&) const;
   Vector< Matrix<double> > calculate_Hessian_form(const Vector<double>&) const;

   Vector<double> calculate_parameters_output(const Vector<double>&, const Vector<double>&) const;
   Matrix<double> calculate_parameters_Jacobian(const Vector<double>&, const Vector<double>&) const;
   Vector< Matrix<double> > calculate_parameters_Hessian_form(const Vector<double>&, const Vector<double>&) const;

   // Expression methods

   std::string write_expression(const Vector<std::string>&, const Vector<std::string>&) const;

   // Hinton diagram methods

   // Serialization methods


protected:

   // MEMBERS

   /// Vectors of perceptrons which defines the layer.
   /// The size of the vector is equal to the number of perceptrons in the layer.

   Vector<Perceptron> perceptrons;

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

