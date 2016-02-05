/****************************************************************************************************************/
/*                                                                                                              */
/*   OpenNN: Open Neural Networks Library                                                                       */
/*   www.opennn.cimne.com                                                                                       */
/*                                                                                                              */
/*   U N S C A L I N G   L A Y E R   C L A S S   H E A D E R                                                    */
/*                                                                                                              */
/*   Roberto Lopez                                                                                              */
/*   International Center for Numerical Methods in Engineering (CIMNE)                                          */
/*   Technical University of Catalonia (UPC)                                                                    */
/*   Barcelona, Spain                                                                                           */
/*   E-mail: rlopez@cimne.upc.edu                                                                               */
/*                                                                                                              */
/****************************************************************************************************************/

#ifndef __UNSCALINGLAYER_H__
#define __UNSCALINGLAYER_H__

// System includes

#include <string>

// OpenNN includes

#include "../utilities/vector.h"
#include "../utilities/matrix.h"

// TinyXml includes

#include "../../tinyxml/tinyxml.h"


namespace OpenNN
{

/// This class represents a layer of unscaling neurons.
/// Unscaling layers are included in the definition of a neural network. 
/// They are used to unnormalize variables so they are in the original range after computer processing.  

class UnscalingLayer
{

public:

   // DEFAULT CONSTRUCTOR

   explicit UnscalingLayer(void);

   // UNSCALING NEURONS NUMBER CONSTRUCTOR

   explicit UnscalingLayer(const unsigned int&);

   // STATISTICS CONSTRUCTOR

   explicit UnscalingLayer(const Vector< Vector<double> >&);

   // XML CONSTRUCTOR

   explicit UnscalingLayer(TiXmlElement*);

   // COPY CONSTRUCTOR

   UnscalingLayer(const UnscalingLayer&);

   // DESTRUCTOR

   virtual ~UnscalingLayer(void);

   // ASSIGNMENT OPERATOR

   UnscalingLayer& operator = (const UnscalingLayer&);

   // EQUAL TO OPERATOR

   bool operator == (const UnscalingLayer&) const;

   // ENUMERATIONS

   /// Enumeration of available methods for input variables, output variables and independent parameters scaling.  
   
   enum UnscalingMethod{MinimumMaximum, MeanStandardDeviation};

   // GET METHODS

   // Outputs number

   unsigned int count_unscaling_neurons_number(void) const;

   // Output variables statistics

   const Vector<double>& get_minimums(void) const;
   const double& get_minimum(const unsigned int&) const;

   const Vector<double>& get_maximums(void) const;
   const double& get_maximum(const unsigned int&) const;

   const Vector<double>& get_means(void) const;
   const double& get_mean(const unsigned int&) const;

   const Vector<double>& get_standard_deviations(void) const;
   const double& get_standard_deviation(const unsigned int&) const;

   Vector< Vector<double>* > get_minimums_maximums(void);
   Vector< Vector<double>* > get_means_standard_deviations(void);

   Vector< Vector<double>* > get_statistics(void);

   // Outputs unscaling method

   const UnscalingMethod& get_unscaling_method(void) const;

   std::string write_unscaling_method(void) const; 

   // Display messages

   const bool& get_display(void) const;

   // SET METHODS

   void set(void);
   void set(const unsigned int&);
   void set(const Vector< Vector<double> >&);
   void set(TiXmlElement*);
   void set(const UnscalingLayer&);

   virtual void set_default(void);

   // Output variables statistics

   void set_minimums(const Vector<double>&);
   void set_minimum(const unsigned int&, const double&);

   void set_maximums(const Vector<double>&);
   void set_maximum(const unsigned int&, const double&);

   void set_means(const Vector<double>&);
   void set_mean(const unsigned int&, const double&);

   void set_standard_deviations(const Vector<double>&);
   void set_standard_deviation(const unsigned int&, const double&);

   void set_minimums_maximums(const Vector< Vector<double> >&);
   void set_means_standard_deviations(const Vector< Vector<double> >&);

   void set_statistics(const Vector< Vector<double> >&);

   // Outputs unscaling method

   void set_unscaling_method(const UnscalingMethod&);
   void set_unscaling_method(const std::string&);

   // Display messages

   void set_display(const bool&);

   bool is_empty(void) const;
  
   // UnscalingLayer and unscaling

   void initialize_random(void);

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

   void check_range(const Vector<double>&) const;

   // Serialization methods

   std::string to_string(void) const;

   virtual TiXmlElement* to_XML(void) const;
   virtual void from_XML(TiXmlElement*);

   // Expression methods

   std::string write_minimum_maximum_expression(const Vector<std::string>&, const Vector<std::string>&) const;
   std::string write_mean_stadard_deviation_expression(const Vector<std::string>&, const Vector<std::string>&) const;

   std::string write_expression(const Vector<std::string>&, const Vector<std::string>&) const;

protected:

   // MEMBERS

   /// Minimum of output variables.

   Vector<double> minimums;

   /// Maximum of output variables.

   Vector<double> maximums;

   /// Mean of output variables.

   Vector<double> means;

   /// Standard deviation of output variables.

   Vector<double> standard_deviations;

   /// Unscaling method for the output variables.

   UnscalingMethod unscaling_method;

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

