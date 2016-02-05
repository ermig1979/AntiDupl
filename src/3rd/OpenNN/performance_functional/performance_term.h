/****************************************************************************************************************/
/*                                                                                                              */
/*   OpenNN: Open Neural Networks Library                                                                       */
/*   www.opennn.cimne.com                                                                                       */
/*                                                                                                              */
/*   P E R F O R M A N C E   T E R M   C L A S S   H E A D E R                                                  */
/*                                                                                                              */
/*   Roberto Lopez                                                                                              */
/*   International Center for Numerical Methods in Engineering (CIMNE)                                          */
/*   Technical University of Catalonia (UPC)                                                                    */
/*   Barcelona, Spain                                                                                           */
/*   E-mail: rlopez@cimne.upc.edu                                                                               */
/*                                                                                                              */
/****************************************************************************************************************/

#ifndef __PERFORMANCETERM_H__
#define __PERFORMANCETERM_H__

// System includes

#include <string>

// OpenNN includes

#include "../utilities/vector.h"
#include "../utilities/matrix.h"
#include "../utilities/numerical_differentiation.h"

#include "../data_set/data_set.h"
#include "../mathematical_model/mathematical_model.h"

#include "../neural_network/neural_network.h"

// TinyXml includes

#include "../../tinyxml/tinyxml.h"

namespace OpenNN
{

/// This class represents the concept of performance term. 
/// A performance term is a summand in the performance functional expression. 
/// Any derived class must implement the calculate_evaluation(void) method.

class PerformanceTerm
{

public:

   // DEFAULT CONSTRUCTOR

   explicit PerformanceTerm(void);

   // NEURAL NETWORK CONSTRUCTOR

   explicit PerformanceTerm(NeuralNetwork*);

   // DATA SET CONSTRUCTOR

   explicit PerformanceTerm(DataSet*);

   // MATHEMATICAL MODEL CONSTRUCTOR

   explicit PerformanceTerm(MathematicalModel*);

   // NEURAL NETWORK AND DATA SET CONSTRUCTOR

   explicit PerformanceTerm(NeuralNetwork*, DataSet*);

   // NEURAL NETWORK AND MATHEMATICAL MODEL CONSTRUCTOR

   explicit PerformanceTerm(NeuralNetwork*, MathematicalModel*);

   // NEURAL NETWORK, MATHEMATICAL MODEL AND DATA SET CONSTRUCTOR

   explicit PerformanceTerm(NeuralNetwork*, MathematicalModel*, DataSet*);

   // XML CONSTRUCTOR

   explicit PerformanceTerm(TiXmlElement*);

   // COPY CONSTRUCTOR

   PerformanceTerm(const PerformanceTerm&);

   // DESTRUCTOR

   virtual ~PerformanceTerm(void);

   // ASSIGNMENT OPERATOR

   virtual PerformanceTerm& operator = (const PerformanceTerm&);

   // EQUAL TO OPERATOR

   virtual bool operator == (const PerformanceTerm&) const;

   // STRUCTURES

   /// This structure contains the zero order evaluation quantities of a performance term. 
   /// This only includes the evaluation itself.

   struct ZeroOrderEvaluation
   {
      /// Performance term evaluation. 

      double evaluation;
   };

   /// This structure contains the first order evaluation quantities of a performance term. 
   /// This includes the evaluation itself and the gradient vector.

   struct FirstOrderEvaluation
   {
      /// Performance term evaluation. 

      double evaluation;

      /// Performance term gradient vector. 

      Vector<double> gradient;
   };


   /// This structure contains the second order evaluation quantities of a performance term. 
   /// This includes the evaluation itself, the gradient vector and the Hessian matrix.

   struct SecondOrderEvaluation
   {
      /// Peformance term evaluation. 

      double evaluation;

      /// Performance term gradient vector. 

      Vector<double> gradient;

	  /// Performance term Hessian matrix. 

      Matrix<double> Hessian;
   };


   /// Set of subterms vector and subterms Jacobian matrix of the performance term. 
   /// A method returning this structure might be more efficient than calculating the objective terms and the terms Jacobian separately.

   struct FirstOrderEvaluationTerms
   {
      /// Subterms evaluation vector. 

      Vector<double> evaluation_terms;

      /// Subterms Jacobian matrix. 

      Matrix<double> Jacobian_terms;
   };


   // METHODS

   // Get methods

   /// This method returns a pointer to the neural network object associated to the performance term.

   inline NeuralNetwork* get_neural_network_pointer(void) const 
   {
      return(neural_network_pointer);
   }


   /// This method returns a pointer to the mathematical model object associated to the performance term.

   inline MathematicalModel* get_mathemtaical_model_pointer(void) const 
   {
      return(mathematical_model_pointer);
   }


   /// This method returns a pointer to the data set object associated to the performance term.

   inline DataSet* get_data_set_pointer(void) const 
   {
      return(data_set_pointer);
   }


   /// This method returns a pointer to the numerical differentiation object used in this performance term object. 

   inline NumericalDifferentiation* get_numerical_differentiation_pointer(void) const
   {
      return(numerical_differentiation_pointer);
   }

   const bool& get_display(void) const;

   // Set methods

   virtual void set(void);
   virtual void set(NeuralNetwork*);
   virtual void set(DataSet*);
   virtual void set(MathematicalModel*);
   virtual void set(NeuralNetwork*, DataSet*);
   virtual void set(NeuralNetwork*, MathematicalModel*);
   virtual void set(NeuralNetwork*, MathematicalModel*, DataSet*);
   virtual void set(const PerformanceTerm&);

   virtual void set_neural_network_pointer(NeuralNetwork*);

   virtual void set_mathematical_model_pointer(MathematicalModel*);
   virtual void set_data_set_pointer(DataSet*);

   void set_numerical_differentiation_pointer(NumericalDifferentiation*);

   virtual void set_default(void);

   void set_display(const bool&);

   // Pointer methods

   void construct_numerical_differentiation(void);
   void delete_numerical_differentiation_pointer(void);

   // Checking methods

   virtual void check(void) const;

   // Layers delta methods
   
   Vector< Vector<double> > calculate_layers_delta(const Vector< Vector<double> >&, const Vector<double>&) const;
   Vector< Vector<double> > calculate_layers_delta(const Vector< Vector<double> >&, const Vector<double>&, const Vector<double>&) const;

   // Interlayers Delta methods

   Matrix< Matrix <double> > calculate_interlayers_Delta(const Vector< Vector<double> >&, const Vector< Vector<double> >&, const Matrix< Matrix<double> >&, const Vector<double>&, const Matrix<double>&, const Vector< Vector<double> >&) const;

   // Point objective function methods

   Vector<double> calculate_point_gradient(const Vector<double>&, const Vector< Vector<double> >&, const Vector< Vector<double> >&) const;
   Vector<double> calculate_point_gradient(const Vector< Matrix<double> >&, const Vector< Vector<double> >&) const;

   Matrix<double> calculate_point_Hessian(const Vector< Vector<double> >&, const Vector< Vector< Vector<double> > >&, const Matrix< Matrix<double> >&, const Vector< Vector<double> >&, const Matrix< Matrix<double> >&) const;

   // Objective methods

   /// This method returns the evaluation value of the performance term.

   virtual double calculate_evaluation(void) const = 0;

   /// This method returns the default evaluation of a performance term for a given set of neural network parameters. 

   virtual double calculate_evaluation(const Vector<double>&) const
   {
      return(0.0);
   };

   /// This method returns an evaluation of the performance term for generalization purposes.  

   virtual double calculate_generalization_evaluation(void) const
   {
      return(0.0);
   }

   /// This method returns the objective function gradient vector for a neural network. 

   virtual Vector<double> calculate_gradient(void) const; 

   /// This method returns the objective function Hessian matrix for a neural network.

   virtual Matrix<double> calculate_Hessian(void) const; 

   virtual Vector<double> calculate_evaluation_terms(void) const;
   virtual Vector<double> calculate_evaluation_terms(const Vector<double>&) const;

   virtual Matrix<double> calculate_Jacobian_terms(void) const;

   virtual PerformanceTerm::FirstOrderEvaluationTerms calculate_first_order_evaluation_terms(void) const;

   virtual std::string write_performance_term_type(void) const;

   virtual std::string write_information(void) const;

   // Serialization methods

   virtual std::string to_string(void) const;

   virtual TiXmlElement* to_XML(void) const;   
   virtual void from_XML(TiXmlElement*);   

   unsigned int calculate_Kronecker_delta(const unsigned int&, const unsigned int&) const;

protected:

   /// Pointer to a multilayer perceptron object.

   NeuralNetwork* neural_network_pointer;

   /// Pointer to a data set object.

   DataSet* data_set_pointer;

   /// Pointer to a mathematical model object.

   MathematicalModel* mathematical_model_pointer;

   /// Numerical differentiation object.

   NumericalDifferentiation* numerical_differentiation_pointer;

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
