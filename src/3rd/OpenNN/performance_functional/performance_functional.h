/****************************************************************************************************************/
/*                                                                                                              */
/*   OpenNN: Open Neural Networks Library                                                                       */
/*   www.opennn.cimne.com                                                                                       */
/*                                                                                                              */
/*   P E R F O R M A N C E   F U N C T I O N A L   C L A S S   H E A D E R                                      */
/*                                                                                                              */
/*   Roberto Lopez                                                                                              */
/*   International Center for Numerical Methods in Engineering (CIMNE)                                          */
/*   Technical University of Catalonia (UPC)                                                                    */
/*   Barcelona, Spain                                                                                           */
/*   E-mail: rlopez@cimne.upc.edu                                                                               */
/*                                                                                                              */
/****************************************************************************************************************/

#ifndef __PERFORMANCEFUNCTIONAL_H__
#define __PERFORMANCEFUNCTIONAL_H__

// System includes

#include <string>

// OpenNN includes

#include "../utilities/vector.h"
#include "../utilities/matrix.h"
#include "../utilities/numerical_differentiation.h"

#include "../data_set/data_set.h"
#include "../mathematical_model/mathematical_model.h"

#include "../neural_network/neural_network.h"
#include "performance_term.h"

// TinyXml includes

#include "../../tinyxml/tinyxml.h"

namespace OpenNN
{

/// This abstract class represents the concept of performance functional for a neural network. 
/// A performance functional is composed of three terms: An performance term, a regularization functional 
/// and a constraints functional. 
/// Any derived class must implement the calculate_evaluation(void) method.

class PerformanceFunctional
{

public:

   // DEFAULT CONSTRUCTOR

   explicit PerformanceFunctional(void);

   // OBJECTIVE FUNCTIONAL CONSTRUCTOR

   explicit PerformanceFunctional(PerformanceTerm*);

   // NEURAL NETWORK CONSTRUCTOR

   explicit PerformanceFunctional(NeuralNetwork*);

   // NEURAL NETWORK AND DATA SET CONSTRUCTOR

   explicit PerformanceFunctional(NeuralNetwork*, DataSet*);

   // NEURAL NETWORK AND MATHEMATICAL MODEL CONSTRUCTOR

   explicit PerformanceFunctional(NeuralNetwork*, MathematicalModel*);

   // NEURAL NETWORK, MATHEMATICAL MODEL AND DATA SET CONSTRUCTOR

   explicit PerformanceFunctional(NeuralNetwork*, MathematicalModel*, DataSet*);

   // FILE CONSTRUCTOR

   explicit PerformanceFunctional(const std::string&);

   // COPY CONSTRUCTOR

   PerformanceFunctional(const PerformanceFunctional&);

   // DESTRUCTOR

   virtual ~PerformanceFunctional(void);

   // STRUCTURES 

   /// Evaluation value of the peformance function. 
   /// This is a very simple structure with just one value. 

   struct ZeroOrderEvaluation
   {
      /// Performance function evaluation. 

      double performance;
   };

   /// Set of evaluation value and gradient vector of the peformance function. 
   /// A method returning this structure might be implemented more efficiently than the performance and gradient methods separately.

   struct FirstOrderEvaluation
   {
      /// Performance function evaluation. 

      double performance;

      /// Performance function gradient vector. 

      Vector<double> gradient;
   };

   /// Set of evaluation value, gradient vector and Hessian matrix of the peformance function. 
   /// A method returning this structure might be implemented more efficiently than the performance, gradient and Hessian methods separately.

   struct SecondOrderEvaluation
   {
      /// Performance function evaluation. 

      double performance;

      /// Performance function gradient vector. 

	  Vector<double> gradient;

      /// Performance function Hessian matrix. 

	  Matrix<double> Hessian;
   };


   // ENUMERATIONS

   /// Enumeration of available performance terms in OpenNN.  
   
   enum PerformanceTermType
   {
      NONE,
      SUM_SQUARED_ERROR,
      MEAN_SQUARED_ERROR,
      ROOT_MEAN_SQUARED_ERROR,
      NORMALIZED_SQUARED_ERROR,
      MINKOWSKI_ERROR,
      CROSS_ENTROPY_ERROR,
      NEURAL_PARAMETERS_NORM,
      OUTPUTS_INTEGRALS,
      SOLUTION_ERROR,
      FINAL_SOLUTIONS_ERROR,
      INDEPENDENT_PARAMETERS_ERROR,
      INVERSE_SUM_SQUARED_ERROR,
      USER_PERFORMANCE_TERM
   };


   // METHODS

   // Get methods

   /// This method returns a pointer to the neural network associated to the performance functional.

   inline NeuralNetwork* get_neural_network_pointer(void) const 
   {
      return(neural_network_pointer);
   }

   /// This method returns a pointer to the mathematical model associated to the performance functional.

//   inline MathematicalModel* get_mathematical_model_pointer(void) const 
//   {
//      return(mathematical_model_pointer);
//   }

   /// This method returns a pointer to the data set associated to the performance functional.

//   inline DataSet* get_data_set_pointer(void) const 
//   {
//      return(data_set_pointer);
//   }

   /// This method returns a pointer to the performance term composing the performance functional.

   inline PerformanceTerm* get_objective_term_pointer(void) const
   {
      return(objective_term_pointer);   
   }

   /// This method returns a pointer to the regularization functional composing the performance functional.

   inline PerformanceTerm* get_regularization_term_pointer(void) const
   {
      return(regularization_term_pointer);
   }


   /// This method returns a pointer to the constraints functional composing the performance functional.

   inline PerformanceTerm* get_constraints_term_pointer(void)
   {
      return(constraints_term_pointer);
   }


   // Functional type methods

   const PerformanceTermType& get_objective_term_type(void) const;
   const PerformanceTermType& get_regularization_term_type(void) const;
   const PerformanceTermType& get_constraints_term_type(void) const;

   std::string write_objective_term_type(void) const;
   std::string write_regularization_term_type(void) const;
   std::string write_constraints_term_type(void) const;

   const bool& get_objective_term_flag(void) const;
   const bool& get_regularization_term_flag(void) const;
   const bool& get_constraints_term_flag(void) const;

   // Serialization methods

   const bool& get_display(void) const;

   // Set methods

   void set_neural_network_pointer(NeuralNetwork*);

   void set_objective_term_pointer(PerformanceTerm*);
   void set_regularization_term_pointer(PerformanceTerm*);
   void set_constraints_term_pointer(PerformanceTerm*);

   void set_default(void);

   // Functionals methods 

   void set_objective_term_type(const PerformanceTermType&);
   void set_objective_term_type(const std::string&);

   void set_regularization_term_type(const PerformanceTermType&);
   void set_regularization_term_type(const std::string&);

   void set_constraints_term_type(const PerformanceTermType&);
   void set_constraints_term_type(const std::string&);

   void set_objective_term_flag(const bool&);
   void set_regularization_term_flag(const bool&);
   void set_constraints_term_flag(const bool&);

   // Serialization methods

   void set_display(const bool&);
   
   // Pointer methods

   void construct_objective_term(const PerformanceTermType&);
   void construct_regularization_term(const PerformanceTermType&);
   void construct_constraints_term(const PerformanceTermType&);

   void construct_objective_term(const PerformanceTermType&, MathematicalModel*);
   void construct_regularization_term(const PerformanceTermType&, MathematicalModel*);
   void construct_constraints_term(const PerformanceTermType&, MathematicalModel*);

   void construct_objective_term(const PerformanceTermType&, DataSet*);
   void construct_regularization_term(const PerformanceTermType&, DataSet*);
   void construct_constraints_term(const PerformanceTermType&, DataSet*);

   void construct_objective_term(const PerformanceTermType&, MathematicalModel*, DataSet*);
   void construct_regularization_term(const PerformanceTermType&, MathematicalModel*, DataSet*);
   void construct_constraints_term(const PerformanceTermType&, MathematicalModel*, DataSet*);

   void construct_objective_term(const std::string&);
   void construct_regularization_term(const std::string&);
   void construct_constraints_term(const std::string&);

   void destruct_objective_term(void);
   void destruct_regularization_term(void);
   void destruct_constraints_term(void);

   void destruct_all_terms(void);

   // Performance functional methods

   double calculate_evaluation(void) const;
   Vector<double> calculate_gradient(void) const;
   Matrix<double> calculate_Hessian(void) const;

   double calculate_evaluation(const Vector<double>&);
   Vector<double> calculate_gradient(const Vector<double>&);
   Matrix<double> calculate_Hessian(const Vector<double>&);

   virtual Matrix<double> calculate_inverse_Hessian(void) const;

   virtual Vector<double> calculate_vector_dot_Hessian(const Vector<double>&) const;

   virtual ZeroOrderEvaluation calculate_zero_order_evaluation(void) const;
   virtual FirstOrderEvaluation calculate_first_order_evaluation(void) const;
   virtual SecondOrderEvaluation calculate_second_order_evaluation(void) const;

   virtual double calculate_generalization_evaluation(void) const;

   // Taylor approximation methods

   double calculate_zero_order_Taylor_approximation(const Vector<double>&) const;
   double calculate_first_order_Taylor_approximation(const Vector<double>&) const;
   double calculate_second_order_Taylor_approximation(const Vector<double>&) const;

   // Directional performance

   double calculate_directional_performance(const Vector<double>&, double);
   double calculate_directional_performance_derivative(const Vector<double>&, double);
   double calculate_directional_performance_second_derivative(const Vector<double>&, double);

   // Serialization methods

   virtual TiXmlElement* to_XML(void) const;   
   virtual void from_XML(TiXmlElement*);   
   
   virtual void print(void) const;   
   virtual void save(const std::string&) const;   
   virtual void load(const std::string&);   

//   virtual TiXmlElement* calculate_performance_XML(void) const;    
//   virtual TiXmlElement* calculate_generalization_evaluation_XML(void) const;    
//   virtual TiXmlElement* calculate_gradient_XML(void) const;    

   virtual std::string write_information(void);   


private:

   /// Pointer to a neural network object.

   NeuralNetwork* neural_network_pointer;

   /// Weight of the objective term in the performance functional expression. 

   PerformanceTerm* objective_term_pointer;

   /// Weight of the regularization term in the performance functional expression. 

   PerformanceTerm* regularization_term_pointer;

   /// Pointer to the constraints functional composing the performance functional. 

   PerformanceTerm* constraints_term_pointer;

   /// Type of performance term. 

   PerformanceTermType objective_term_type;

   /// Type of regularization functional. 

   PerformanceTermType regularization_term_type;

   /// Type of performance term. 

   PerformanceTermType constraints_term_type;

   /// Flag for using the objective term.

   bool objective_term_flag;

   /// Flag for using the regularization term.

   bool regularization_term_flag;

   /// Flag for using constraints. 

   bool constraints_term_flag;

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
