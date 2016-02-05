/****************************************************************************************************************/
/*                                                                                                              */
/*   OpenNN: Open Neural Networks Library                                                                       */
/*   www.opennn.cimne.com                                                                                       */
/*                                                                                                              */
/*   L E V E N B E R G - M A R Q U A R D T   A L G O R I T H M   C L A S S   H E A D E R                        */
/*                                                                                                              */
/*   Roberto Lopez                                                                                              */
/*   International Center for Numerical Methods in Engineering (CIMNE)                                          */
/*   Technical University of Catalonia (UPC)                                                                    */
/*   Barcelona, Spain                                                                                           */
/*   E-mail: rlopez@cimne.upc.edu                                                                               */
/*                                                                                                              */
/****************************************************************************************************************/

#ifndef __LEVENBERGMARQUARDTALGORITHM_H__
#define __LEVENBERGMARQUARDTALGORITHM_H__

// OpenNN includes

#include "../performance_functional/sum_squared_error.h"
#include "../performance_functional/mean_squared_error.h"
#include "../performance_functional/normalized_squared_error.h"

#include "training_algorithm.h"

#include "../utilities/linear_algebraic_equations.h"

// TinyXml includes

#include "../../tinyxml/tinyxml.h"

namespace OpenNN
{

/// This concrete class represents a Levenberg-Marquardt Algorithm training
/// algorithm for the sum squared error performance functional for a multilayer perceptron.

class LevenbergMarquardtAlgorithm : public TrainingAlgorithm
{

public:

   // DEFAULT CONSTRUCTOR

   explicit LevenbergMarquardtAlgorithm(void);

   // PERFORMANCE FUNCTIONAL CONSTRUCTOR

   explicit LevenbergMarquardtAlgorithm(PerformanceFunctional*);

   // XML CONSTRUCTOR

   explicit LevenbergMarquardtAlgorithm(TiXmlElement*);

   // DESTRUCTOR

   virtual ~LevenbergMarquardtAlgorithm(void);

   // STRUCTURES

   ///
   /// This structure contains the training results for the Levenberg-Marquardt algorithm. 
   ///

   struct LevenbergMarquardtAlgorithmResults : public TrainingAlgorithm::Results
   {                                      
      // Training history

      /// History of the neural network parameters over the training epochs. 

      Vector< Vector<double> > parameters_history;

      /// History of the parameters norm over the training epochs. 

      Vector<double> parameters_norm_history;

      /// History of the performance function evaluation over the training epochs. 

      Vector<double> evaluation_history;

      /// History of the generalization evaluation over the training epochs. 

      Vector<double> generalization_evaluation_history;

      /// History of the performance function gradient over the training epochs. 

      Vector< Vector<double> > gradient_history;

      /// History of the gradient norm over the training epochs. 

      Vector<double> gradient_norm_history;

      /// History of the Hessian approximation over the training epochs. 

      Vector< Matrix<double> > Hessian_approximation_history; 

      /// History of the damping parameter over the training epochs. 

      Vector<double> damping_parameter_history;

      /// History of the elapsed time over the training epochs. 

      Vector<double> elapsed_time_history;

      // Final values

      /// Final neural network parameters vector. 

      Vector<double> final_parameters;

      /// Final neural network parameters norm. 

      double final_parameters_norm;

      /// Final performance function evaluation. 

      double final_evaluation;

      /// Final generalization evaluation. 

      double final_generalization_evaluation;

      /// Final performance function gradient. 

      Vector<double> final_gradient;

      /// Final gradient norm.

      double final_gradient_norm;

      /// Elapsed time of the training process. 

      double elapsed_time;

      void resize_training_history(const unsigned int&);
      std::string to_string(void) const;
   };

   // METHODS

   // Get methods

   // Training parameters

   const double& get_warning_parameters_norm(void) const;
   const double& get_warning_gradient_norm(void) const;

   const double& get_error_parameters_norm(void) const;
   const double& get_error_gradient_norm(void) const;

   // Stopping criteria

   const double& get_minimum_parameters_increment_norm(void) const;

   const double& get_minimum_performance_increase(void) const;
   const double& get_performance_goal(void) const;
   const double& get_gradient_norm_goal(void) const;
   const unsigned int& get_maximum_generalization_evaluation_decreases(void) const;

   const unsigned int& get_maximum_epochs_number(void) const;
   const double& get_maximum_time(void) const;

   // Reserve training history

   const bool& get_reserve_parameters_history(void) const;
   const bool& get_reserve_parameters_norm_history(void) const;

   const bool& get_reserve_evaluation_history(void) const;
   const bool& get_reserve_gradient_history(void) const;
   const bool& get_reserve_gradient_norm_history(void) const;
   const bool& get_reserve_inverse_Hessian_history(void) const;
   const bool& get_reserve_generalization_evaluation_history(void) const;

   const bool& get_reserve_elapsed_time_history(void) const;

   // Utilities

   const unsigned int& get_display_period(void) const;

   const double& get_damping_parameter(void) const;

   const double& get_damping_parameter_factor(void) const;

   const double& get_minimum_damping_parameter(void) const;
   const double& get_maximum_damping_parameter(void) const;

   const bool& get_reserve_damping_parameter_history(void) const;

   const Vector<double>& get_damping_parameter_history(void) const;

   // Set methods

   void set_default(void);

   void set_damping_parameter(const double&);

   void set_damping_parameter_factor(const double&);

   void set_minimum_damping_parameter(const double&);
   void set_maximum_damping_parameter(const double&);

   void set_reserve_damping_parameter_history(const bool&);

   // Training parameters

   void set_warning_parameters_norm(const double&);
   void set_warning_gradient_norm(const double&);

   void set_error_parameters_norm(const double&);
   void set_error_gradient_norm(const double&);

   // Stopping criteria

   void set_minimum_parameters_increment_norm(const double&);

   void set_minimum_performance_increase(const double&);
   void set_performance_goal(const double&);
   void set_gradient_norm_goal(const double&);
   void set_maximum_generalization_evaluation_decreases(const unsigned int&);

   void set_maximum_epochs_number(const unsigned int&);
   void set_maximum_time(const double&);

   // Reserve training history

   void set_reserve_parameters_history(const bool&);
   void set_reserve_parameters_norm_history(const bool&);

   void set_reserve_evaluation_history(const bool&);
   void set_reserve_gradient_history(const bool&);
   void set_reserve_gradient_norm_history(const bool&);
   void set_reserve_inverse_Hessian_history(const bool&);
   void set_reserve_generalization_evaluation_history(const bool&);

   void set_reserve_elapsed_time_history(const bool&);

   /// This method makes the training history of all variables to be reseved or not in memory.

   virtual void set_reserve_all_training_history(const bool&);

   // Utilities

   void set_display_period(const unsigned int&);

   // Training methods

   void check(void) const;

   Vector<double> calculate_gradient(const Vector<double>&, const Matrix<double>&) const;
   Matrix<double> calculate_Hessian_approximation(const Matrix<double>&) const;

   LevenbergMarquardtAlgorithmResults* perform_training(void);

   std::string write_training_algorithm_type(void) const;

   // Serialization methods

   TiXmlElement* to_XML(void) const;

   void from_XML(TiXmlElement*);

private:

   // MEMBERS

   /// Linear algebraic equations object.

   LinearAlgebraicEquations linear_algebraic_equations;

   /// Initial Levenberg-Marquardt parameter.

   double damping_parameter;

   /// Minimum Levenberg-Marquardt parameter.

   double minimum_damping_parameter;

   /// Maximum Levenberg-Marquardt parameter.

   double maximum_damping_parameter;

   /// Damping paramtere increase/decrease factor.

   double damping_parameter_factor;

   /// True if the damping parameter history vector is to be reserved, false otherwise.  

   bool reserve_damping_parameter_history;

   /// Vector containing the damping parameter history over the training epochs.

   Vector<double> damping_parameter_history;


   /// Value for the parameters norm at which a warning message is written to the screen. 

   double warning_parameters_norm;

   /// Value for the gradient norm at which a warning message is written to the screen. 

   double warning_gradient_norm;   

   /// Value for the parameters norm at which the training process is assumed to fail. 
   
   double error_parameters_norm;

   /// Value for the gradient norm at which the training process is assumed to fail. 

   double error_gradient_norm;


   // STOPPING CRITERIA

   /// Norm of the parameters increment vector at which training stops.

   double minimum_parameters_increment_norm;

   /// Minimum performance improvement between two successive epochs. It is used as a perform_training stopping criterion.

   double minimum_performance_increase;

   /// Goal value for the performance. It is used as a perform_training stopping criterion.

   double performance_goal;

   /// Goal value for the norm of the objective function gradient. It is used as a stopping criterion.

   double gradient_norm_goal;

   unsigned int maximum_generalization_evaluation_decreases;

   /// Maximum number of epochs to perform_training. It is used as a perform_training stopping criterion.

   unsigned int maximum_epochs_number;

   /// Maximum training time. It is used as a perform_training stopping criterion.

   double maximum_time;

   // TRAINING HISTORY

   /// True if the parameters history matrix is to be reserved, false otherwise.

   bool reserve_parameters_history;

   /// True if the parameters norm history vector is to be reserved, false otherwise.

   bool reserve_parameters_norm_history;

   /// True if the evaluation history vector is to be reserved, false otherwise.

   bool reserve_evaluation_history;

   /// True if the gradient history matrix is to be reserved, false otherwise.

   bool reserve_gradient_history;

   /// True if the gradient norm history vector is to be reserved, false otherwise.

   bool reserve_gradient_norm_history;

   /// True if the inverse Hessian history vector of matrices is to be reserved, false otherwise.

   bool reserve_inverse_Hessian_history;

   /// True if the elapsed time history vector is to be reserved, false otherwise.

   bool reserve_elapsed_time_history;

   /// True if the Generalization evaluation history vector is to be reserved, false otherwise. 

   bool reserve_generalization_evaluation_history;

   /// Number of epochs between the training showing progress.

   unsigned int display_period;
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
