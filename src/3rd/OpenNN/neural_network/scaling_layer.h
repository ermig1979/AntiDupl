/****************************************************************************************************************/
/*                                                                                                              */
/*   OpenNN: Open Neural Networks Library                                                                       */
/*   www.opennn.cimne.com                                                                                       */
/*                                                                                                              */
/*   S C A L I N G   L A Y E R   C L A S S   H E A D E R                                                        */
/*                                                                                                              */
/*   Roberto Lopez                                                                                              */
/*   International Center for Numerical Methods in Engineering (CIMNE)                                          */
/*   Technical University of Catalonia (UPC)                                                                    */
/*   Barcelona, Spain                                                                                           */
/*   E-mail: rlopez@cimne.upc.edu                                                                               */
/*                                                                                                              */
/****************************************************************************************************************/

#ifndef __SCALINGLAYER_H__
#define __SCALINGLAYER_H__

// System includes

#include <string>

// OpenNN includes

#include "../utilities/vector.h"
#include "../utilities/matrix.h"

// TinyXml includes

#include "../../tinyxml/tinyxml.h"

namespace OpenNN
{

/// This class represents a layer of scaling neurons.
/// Scaling layers are included in the definition of a neural network. 
/// They are used to normalize variables so they are in an appropriate range for computer processing.  

class ScalingLayer
{

public:

   // DEFAULT CONSTRUCTOR

   explicit ScalingLayer(void);

   // INPUTS NUMBER CONSTRUCTOR

   explicit ScalingLayer(const unsigned int&);

   // STATISTICS CONSTRUCTOR

   explicit ScalingLayer(const Vector< Vector<double> >&);

   // COPY CONSTRUCTOR

   ScalingLayer(const ScalingLayer&);

   // DESTRUCTOR

   virtual ~ScalingLayer(void);

   // ASSIGNMENT OPERATOR

   ScalingLayer& operator = (const ScalingLayer&);

   // EQUAL TO OPERATOR

   bool operator == (const ScalingLayer&) const;

   // ENUMERATIONS

   /// Enumeration of available methods for scaling the input variables.  
   
   enum ScalingMethod{MinimumMaximum, MeanStandardDeviation};

   // GET METHODS

   unsigned int count_scaling_neurons_number(void) const;

   // Inputs statistics

   const Vector<double>& get_minimums(void) const;
   double get_minimum(const unsigned int&) const;

   const Vector<double>& get_maximums(void) const;
   double get_maximum(const unsigned int&) const;

   const Vector<double>& get_means(void) const;
   double get_mean(const unsigned int&) const;

   const Vector<double>& get_standard_deviations(void) const;
   double get_standard_deviation(const unsigned int&) const;

   Vector< Vector<double>* > get_minimums_maximums(void);
   Vector< Vector<double>* > get_means_standard_deviations(void);
   Vector< Vector<double>* > get_statistics(void);

   // Variables scaling and unscaling

   const ScalingMethod& get_scaling_method(void) const;

   std::string write_scaling_method(void) const;

   // Display messages

   const bool& get_display(void) const;

   // SET METHODS

   void set(void);
   void set(const unsigned int&);
   void set(const Vector< Vector<double> >&);
   void set(TiXmlElement*);
   void set(const ScalingLayer&);

   virtual void set_default(void);

   // Input variables statistics

   void set_minimums(const Vector<double>&);
   void set_minimum(const unsigned int&, const double&);

   void set_maximums(const Vector<double>&);
   void set_maximum(const unsigned int&, const double&);

   void set_means(const Vector<double>&);
   void set_mean(const unsigned int&, const double&);

   void set_standard_deviations(const Vector<double>&);
   void set_standard_deviation(const unsigned int&, const double&);

   // Inputs statistics

   void set_minimums_maximums(const Vector< Vector<double> >&);
   void set_means_standard_deviations(const Vector< Vector<double> >&);

   void set_statistics(const Vector< Vector<double> >&);

   // Scaling method

   void set_scaling_method(const ScalingMethod&);
   void set_scaling_method(const std::string&);

   // Display messages

   void set_display(const bool&);

   // Check methods

   bool is_empty(void) const;

   // Inputs scaling function

   void initialize_random(void);

   void check_range(const Vector<double>&) const;

   Vector<double> calculate_outputs(const Vector<double>&) const;
   Vector<double> calculate_derivative(const Vector<double>&) const;
   Vector<double> calculate_second_derivative(const Vector<double>&) const;

   Vector<double> calculate_minimum_maximum_output(const Vector<double>&) const;
   Vector<double> calculate_minimum_maximum_derivative(const Vector<double>&) const;
   Vector<double> calculate_minimum_maximum_second_derivative(const Vector<double>&) const;

   Vector<double> calculate_mean_standard_deviation_output(const Vector<double>&) const;
   Vector<double> calculate_mean_standard_deviation_derivative(const Vector<double>&) const;
   Vector<double> calculate_mean_standard_deviation_second_derivative(const Vector<double>&) const;

   Matrix<double> arrange_Jacobian(const Vector<double>&) const;
   Vector< Matrix<double> > arrange_Hessian_form(const Vector<double>&) const;

   // Expression methods

   std::string write_minimum_maximum_expression(const Vector<std::string>&, const Vector<std::string>&) const;
   
   std::string write_mean_standard_deviation_expression(const Vector<std::string>&, const Vector<std::string>&) const;

   std::string write_expression(const Vector<std::string>&, const Vector<std::string>&) const;

   // Serialization methods

   std::string to_string(void) const;

   TiXmlElement* to_XML(void) const;
   virtual void from_XML(TiXmlElement*);

protected:

   // MEMBERS

   /// Minimum of input variables.

   Vector<double> minimums;

   /// Maximum of input variables.

   Vector<double> maximums;

   /// Mean of input variables.

   Vector<double> means;

   /// Standard deviation of input variables.

   Vector<double> standard_deviations;

   /// Method for scaling the input variables.

   ScalingMethod scaling_method;

   /// Display warning messages to screen. 

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

