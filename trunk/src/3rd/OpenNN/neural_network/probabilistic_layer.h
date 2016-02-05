/****************************************************************************************************************/
/*                                                                                                              */
/*   OpenNN: Open Neural Networks Library                                                                       */
/*   www.opennn.cimne.com                                                                                       */
/*                                                                                                              */
/*   P R O B A B I L I S T I C   L A Y E R   C L A S S   H E A D E R                                            */
/*                                                                                                              */
/*   Roberto Lopez                                                                                              */
/*   International Center for Numerical Methods in Engineering (CIMNE)                                          */
/*   Technical University of Catalonia (UPC)                                                                    */
/*   Barcelona, Spain                                                                                           */
/*   E-mail: rlopez@cimne.upc.edu                                                                               */
/*                                                                                                              */
/****************************************************************************************************************/

#ifndef __PROBABILISTICLAYER_H__
#define __PROBABILISTICLAYER_H__

// System includes

#include <string>

// OpenNN includes

#include "../utilities/vector.h"
#include "../utilities/matrix.h"

// TinyXml includes

#include "../../tinyxml/tinyxml.h"

namespace OpenNN
{

/// This class represents a layer of probabilistic neurons.
/// The neural network defined in OpenNN includes a probabilistic layer for those problems 
/// when the outptus are to be interpreted as probabilities. 

class ProbabilisticLayer
{

public:

   // DEFAULT CONSTRUCTOR

   explicit ProbabilisticLayer(void);

   // PROBABILISTIC NEURONS NUMBER CONSTRUCTOR

   explicit ProbabilisticLayer(const unsigned int&);

   // COPY CONSTRUCTOR

   ProbabilisticLayer(const ProbabilisticLayer&);

   // DESTRUCTOR

   virtual ~ProbabilisticLayer(void);

   // ASSIGNMENT OPERATOR

   ProbabilisticLayer& operator = (const ProbabilisticLayer&);

   // EQUAL TO OPERATOR

   bool operator == (const ProbabilisticLayer&) const;

   // ENUMERATIONS

   /// Enumeration of available methods for interpreting variables as probabilities.  
   
   enum ProbabilisticMethod{Competitive, Softmax};

   // GET METHODS

   const unsigned int& get_probabilistic_neurons_number(void) const;

   // Probabilistic processing

   const ProbabilisticMethod& get_probabilistic_method(void) const;
   std::string write_probabilistic_method(void) const;

   // Display messages

   const bool& get_display(void) const;

   // SET METHODS

   void set(void);
   void set(const unsigned int&);
   void set(const ProbabilisticLayer&);

   void set_probabilistic_neurons_number(const unsigned int&);

   void set_probabilistic_method(const ProbabilisticMethod&);
   void set_probabilistic_method(const std::string&);

   virtual void set_default(void);

   // Display messages

   void set_display(const bool&);


   void initialize_random(void);

   // Probabilistic post-processing

   Vector<double> calculate_outputs(const Vector<double>&) const;
   Matrix<double> calculate_Jacobian(const Vector<double>&) const;
   Vector< Matrix<double> > calculate_Hessian_form(const Vector<double>&) const;

   Vector<double> calculate_competitive_output(const Vector<double>&) const;
   Matrix<double> calculate_competitive_Jacobian(const Vector<double>&) const;
   Vector< Matrix<double> > calculate_competitive_Hessian_form(const Vector<double>&) const;

   Vector<double> calculate_softmax_output(const Vector<double>&) const;
   Matrix<double> calculate_softmax_Jacobian(const Vector<double>&) const;
   Vector< Matrix<double> > calculate_softmax_Hessian_form(const Vector<double>&) const;

   // Expression methods

   std::string write_competitive_expression(const Vector<std::string>&, const Vector<std::string>&) const;
   std::string write_softmax_expression(const Vector<std::string>&, const Vector<std::string>&) const;

   std::string write_expression(const Vector<std::string>&, const Vector<std::string>&) const;

   // Serialization methods

   std::string to_string(void) const;

   virtual TiXmlElement* to_XML(void) const;
   virtual void from_XML(TiXmlElement*);

protected:

   // MEMBERS

   /// Number of probabilistic neurons in the layer. 

   unsigned int probabilistic_neurons_number;

   /// Probabilistic processing method.

   ProbabilisticMethod probabilistic_method;

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

