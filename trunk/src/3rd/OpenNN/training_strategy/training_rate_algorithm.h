/****************************************************************************************************************/
/*                                                                                                              */
/*   OpenNN: Open Neural Networks Library                                                                       */
/*   www.opennn.cimne.com                                                                                       */
/*                                                                                                              */
/*   T R A I N I N G   R A T E   A L G O R I T H M   C L A S S   H E A D E R                                    */
/*                                                                                                              */
/*   Roberto Lopez                                                                                              */
/*   International Center for Numerical Methods in Engineering (CIMNE)                                          */
/*   Technical University of Catalonia (UPC)                                                                    */
/*   Barcelona, Spain                                                                                           */
/*   E-mail: rlopez@cimne.upc.edu                                                                               */
/*                                                                                                              */
/****************************************************************************************************************/

#ifndef __TRAININGRATEALGORITHM_H__
#define __TRAININGRATEALGORITHM_H__

// OpenNN includes

#include "../neural_network/neural_network.h"
#include "../performance_functional/performance_functional.h"

// TinyXml includes

#include "../../tinyxml/tinyxml.h"

namespace OpenNN
{

/// This class is used by many different training algorithms to calculate the training rate given a training direction. 
/// It implements the golden section method and the Brent's methods. 

class TrainingRateAlgorithm
{

public:

   // ENUMERATIONS

   /// Available training operators for obtaining the perform_training rate.

   enum TrainingRateMethod{Fixed, GoldenSection, BrentMethod};

   // DEFAULT CONSTRUCTOR

   explicit TrainingRateAlgorithm(void);

   // GENERAL CONSTRUCTOR

   explicit TrainingRateAlgorithm(PerformanceFunctional*);

   // XML CONSTRUCTOR

   explicit TrainingRateAlgorithm(TiXmlElement*);

   // DESTRUCTOR

   virtual ~TrainingRateAlgorithm(void);


   // METHODS

   // Get methods

   PerformanceFunctional* get_performance_functional_pointer(void);

   // Training operators

   const TrainingRateMethod& get_training_rate_method(void) const;
   std::string write_training_rate_method(void) const;

   // Training parameters

   const double& get_first_training_rate(void) const;
   const double& get_bracketing_factor(void) const;   
   const double& get_training_rate_tolerance(void) const;

   const double& get_warning_training_rate(void) const;

   const double& get_error_training_rate(void) const;
  
   // Utilities
   
   const bool& get_display(void) const;
  
   // Set methods

   void set(void);
   void set(PerformanceFunctional*);

   void set_performance_functional_pointer(PerformanceFunctional*);

   // Training operators

   void set_training_rate_method(const TrainingRateMethod&);
   void set_training_rate_method(const std::string&);

   // Training parameters

   void set_first_training_rate(const double&);
   void set_bracketing_factor(const double&);   
   void set_training_rate_tolerance(const double&);

   void set_warning_training_rate(const double&);

   void set_error_training_rate(const double&);

   // Utilities

   void set_display(const bool&);

   virtual void set_default(void);

   // Training rate method

   double calculate_golden_section_training_rate(const Vector<double>&, const Vector<double>&, const Vector<double>&) const;
   double calculate_Brent_method_training_rate(const Vector<double>&, const Vector<double>&, const Vector<double>&) const;

   Vector< Vector<double> > calculate_bracketing_training_rate(const double&, const Vector<double>&, const double&) const;

   Vector<double> calculate_fixed_directional_point(const double&, const Vector<double>&, const double&) const;
   Vector<double> calculate_golden_section_directional_point(const double&, const Vector<double>&, const double&) const;
   Vector<double> calculate_Brent_method_directional_point(const double&, const Vector<double>&, const double&) const;

   Vector<double> calculate_directional_point(const double&, const Vector<double>&, const double&) const;

   // Serialization methods

   virtual TiXmlElement* to_XML(void) const;   
   virtual void from_XML(TiXmlElement*);   


protected:

   // FIELDS

   /// Pointer to an external performance functional object.

   PerformanceFunctional* performance_functional_pointer;


   // TRAINING OPERATORS

   /// Variable containing the actual method used to obtain a suitable perform_training rate. 

   TrainingRateMethod training_rate_method;

   /// Increase factor when bracketing a minimum.

   double bracketing_factor;

   /// Initial training rate in line minimization.

   double first_training_rate;

   /// Maximum interval length for the training rate.

   double training_rate_tolerance;

   /// Big training rate value at which the algorithm displays a warning. 

   double warning_training_rate;

   /// Big training rate value at which the algorithm throws an exception. 

   double error_training_rate;

   // UTILITIES

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

