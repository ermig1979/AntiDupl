/****************************************************************************************************************/
/*                                                                                                              */
/*   OpenNN: Open Neural Networks Library                                                                       */
/*   www.opennn.cimne.com                                                                                       */
/*                                                                                                              */
/*   I N D E P E N D E N T    P A R A M E T E R S   C L A S S   H E A D E R                                     */
/*                                                                                                              */
/*   Roberto Lopez                                                                                              */
/*   International Center for Numerical Methods in Engineering (CIMNE)                                          */
/*   Technical University of Catalonia (UPC)                                                                    */
/*   Barcelona, Spain                                                                                           */
/*   E-mail: rlopez@cimne.upc.edu                                                                               */
/*                                                                                                              */
/****************************************************************************************************************/

#ifndef __INDEPENDENTPARAMETERS_H__
#define __INDEPENDENTPARAMETERS_H__

// System includes

#include <string>

// OpenNN includes

#include "../utilities/vector.h"
#include "../utilities/matrix.h"

// TinyXml includes

#include "../../tinyxml/tinyxml.h"


namespace OpenNN
{

/// This class represents the concept of independent parameters. 
/// Independent parameters are set of free parameters which do not belong to any class of neuron. 
/// They can be used for many different purposes, including a turn arround for function optimization problems. 

class IndependentParameters
{

public:

   // DEFAULT CONSTRUCTOR

   explicit IndependentParameters(void);

   // INDEPENDENT PARAMETERS NUMBER CONSTRUCTOR

   explicit IndependentParameters(const unsigned int&);

   // INDEPENDENT PARAMETERS CONSTRUCTOR

   //explicit IndependentParameters(const Vector<double>&);

   // COPY CONSTRUCTOR

   IndependentParameters(const IndependentParameters&);

   // DESTRUCTOR

   virtual ~IndependentParameters(void);

   // ASSIGNMENT OPERATOR

   IndependentParameters& operator = (const IndependentParameters&);

   // EQUAL TO OPERATOR

   bool operator == (const IndependentParameters&) const;

   // ENUMERATIONS

   /// Enumeration of available methods for scaling and unscaling the independent parameters.  
   
   enum ScalingMethod{MeanStandardDeviation, MinimumMaximum};

   // GET METHODS

   // Independent parameters

   /// This method returns the number of parameters independent of the multilayer perceptron
   /// Independent parameters can be used in the context of neural netwotks for many purposes.

   inline int count_parameters_number(void) const
   {
      return(parameters.size());
   }

   const Vector<double>& get_parameters(void) const;   
   double get_parameter(const unsigned int&) const;   

   // Independent parameters information

   const Vector<std::string>& get_names(void) const;
   const std::string& get_name(const unsigned int&) const;

   const Vector<std::string>& get_units(void) const;
   const std::string& get_unit(const unsigned int&) const;

   const Vector<std::string>& get_descriptions(void) const;
   const std::string& get_description(const unsigned int&) const;

   // Independent parameters statistics

   const Vector<double>& get_minimums(void) const;
   double get_minimum(const unsigned int&) const;

   const Vector<double>& get_maximums(void) const;
   double get_maximum(const unsigned int&) const;

   const Vector<double>& get_means(void) const;
   double get_mean(const unsigned int&) const;

   const Vector<double>& get_standard_deviations(void) const;
   double get_standard_deviation(const unsigned int&) const;

   // Independent parameters scaling and unscaling

   const ScalingMethod& get_scaling_method(void) const;
   std::string write_scaling_method(void) const;

   const bool& get_scaling_flag(void) const;

   // Independent parameters bounds

   const Vector<double>& get_lower_bounds(void) const;
   double get_lower_bound(const unsigned int&) const;

   const Vector<double>& get_upper_bounds(void) const;
   double get_upper_bound(const unsigned int&) const;

   Vector< Vector<double>* > get_bounds(void);

   bool get_bounding_flag(void) const;

   // Display messages

   const bool& get_display(void) const;

   // SET METHODS

   void set(void);
   void set(const unsigned int&);
   void set(const Vector<double>&);
   void set(const IndependentParameters&);

   virtual void set_default(void);

   // Independent parameters

   void set_parameters_number(const unsigned int&);

   void set_parameters(const Vector<double>&);
   void set_parameter(const unsigned int&, const double&);

   // Independent parameters information

   void set_names(const Vector<std::string>&);
   void set_name(const unsigned int&, const std::string&);

   void set_units(const Vector<std::string>&);
   void set_unit(const unsigned int&, const std::string&);

   void set_descriptions(const Vector<std::string>&);
   void set_description(const unsigned int&, const std::string&);

   // Independent parameters statistics

   void set_minimums(const Vector<double>&);
   void set_minimum(const unsigned int&, const double&);

   void set_maximums(const Vector<double>&);
   void set_maximum(const unsigned int&, const double&);

   void set_means(const Vector<double>&);
   void set_mean(const unsigned int&, const double&);

   void set_standard_deviations(const Vector<double>&);
   void set_standard_deviation(const unsigned int&, const double&);
   
   // Independent parameters scaling and unscaling

   void set_scaling_method(const ScalingMethod&);
   void set_scaling_method(const std::string&);

   void set_scaling_flag(const bool&);

   // Independent parameters bounds

   void set_lower_bounds(void);
   void set_lower_bounds(const Vector<double>&);
   void set_lower_bound(const unsigned int&, const double&);

   void set_upper_bounds(void);
   void set_upper_bounds(const Vector<double>&);
   void set_upper_bound(const unsigned int&, const double&);

   void set_bounds(void);
   void set_bounds(const Vector< Vector<double> >&);

   void set_bounding_flag(const bool&);

   // Display messages

   void set_display(const bool&);

   // Check methods

   bool is_empty(void) const;   

   // Independent parameters initialization methods

   void initialize_random(void);

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

   //double calculate_parameters_norm(void) const;

   // Independent parameters

   Vector<double> calculate_scaled_parameters(void) const;
   void unscale_parameters(const Vector<double>&);

   void bound_parameters(void);
   void bound_parameter(const unsigned int&);

   // Information 

   Vector< Vector<std::string> > arrange_information(void);     

   //void set_information(const Vector< Vector<std::string> >&);     

   // Statistics 

   Vector< Vector<double> > arrange_statistics(void);

   Vector< Vector<double> > arrange_minimums_maximums(void);
   Vector< Vector<double> > arrange_means_standard_deviations(void);

   void set_statistics(const Vector< Vector<double> >&);
   void set_minimums_maximums(const Vector< Vector<double> >&);
   void set_means_standard_deviations(const Vector< Vector<double> >&);

   // Serialization methods

   std::string to_string(void) const;

   TiXmlElement* to_XML(void) const;
   void from_XML(TiXmlElement*);

protected:

   // MEMBERS

   /// Independent parameters.

   Vector<double> parameters;

   /// Name of independent parameters.

   Vector<std::string> names;

   /// Units of independent parameters.

   Vector<std::string> units;

   /// Description of independent parameters.

   Vector<std::string> descriptions;

   /// Minimum of independent parameters.

   Vector<double> minimums;

   /// Maximum of independent parameters.

   Vector<double> maximums;

   /// Mean of independent parameters.

   Vector<double> means;

   /// Standard deviation of independent parameters.

   Vector<double> standard_deviations;

   /// Lower bound of independent parameters.

   Vector<double> lower_bounds;

   /// Upper bound of independent parameters.

   Vector<double> upper_bounds;

   /// Independent parameters scaling and unscaling method.

   ScalingMethod scaling_method;

   /// Flag for scaling and unscaling the independent parameters

   bool scaling_flag;

   /// True if the independent parameters are to be bounded.

   bool bounding_flag;

   /// Display warnings when the the independent parameters fall outside their minimum-maximum range. 

   bool display_range_warning;

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

