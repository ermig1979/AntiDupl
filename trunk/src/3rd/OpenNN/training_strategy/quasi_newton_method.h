/****************************************************************************************************************/
/*                                                                                                              */
/*   OpenNN: Open Neural Networks Library                                                                       */
/*   www.opennn.cimne.com                                                                                       */
/*                                                                                                              */
/*   Q U A S I - N E W T O N   M E T H O D    C L A S S   H E A D E R                                           */
/*                                                                                                              */
/*   Roberto Lopez                                                                                              */
/*   International Center for Numerical Methods in Engineering (CIMNE)                                          */
/*   Technical University of Catalonia (UPC)                                                                    */
/*   Barcelona, Spain                                                                                           */
/*   E-mail: rlopez@cimne.upc.edu                                                                               */
/*                                                                                                              */
/****************************************************************************************************************/

#ifndef __QUASINEWTONMETHOD_H__
#define __QUASINEWTONMETHOD_H__

// OpenNN includes

#include "../performance_functional/performance_functional.h"

#include "training_algorithm.h"
#include "training_rate_algorithm.h"

// TinyXml includes

#include "../../tinyxml/tinyxml.h"

namespace OpenNN
{

///
/// This concrete class represents a quasi-Newton training algorithm for a performance functional of a neural network.
///

class QuasiNewtonMethod : public TrainingAlgorithm
{

public:

   // ENUMERATIONS

   /// Enumeration of the available training operators for obtaining the approximation to the inverse Hessian.

   enum InverseHessianApproximationMethod{DFP, BFGS};


   // DEFAULT CONSTRUCTOR

   explicit QuasiNewtonMethod(void);

   // PERFORMANCE FUNCTIONAL CONSTRUCTOR

   explicit QuasiNewtonMethod(PerformanceFunctional*);

   // XML CONSTRUCTOR

   explicit QuasiNewtonMethod(TiXmlElement*);


   // DESTRUCTOR

   virtual ~QuasiNewtonMethod(void);

   // STRUCTURES

   ///
   /// This structure contains the training results for the quasi-Newton method. 
   ///

   struct QuasiNetwonMethodResults : public TrainingAlgorithm::Results
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

      /// History of the inverse Hessian approximation over the training epochs. 

      Vector< Matrix<double> > inverse_Hessian_history;

      /// History of the random search training direction over the training epochs. 

      Vector< Vector<double> > training_direction_history;

      /// History of the random search training rate over the training epochs. 

      Vector<double> training_rate_history;

      /// History of the elapsed time over the training epochs. 

      Vector<double> elapsed_time_history;

      void resize_training_history(const unsigned int&);

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

      /// Final approximation of the inverse Hessian. 

      Matrix<double> final_inverse_Hessian_approximation;

      /// Final quasi-Newton method training direction. 

      Vector<double> final_training_direction;

      /// Final quasi-Newton method training rate. 

      double final_training_rate;

      /// Elapsed time of the training process. 

      double elapsed_time;

      std::string to_string(void) const;
   };

   // METHODS

   // Get methods

   const TrainingRateAlgorithm& get_training_rate_algorithm(void) const;
   TrainingRateAlgorithm* get_training_rate_algorithm_pointer(void);

   const InverseHessianApproximationMethod& get_inverse_Hessian_approximation_method(void) const;
   std::string write_inverse_Hessian_approximation_method(void) const;

   // Training parameters

   const double& get_warning_parameters_norm(void) const;
   const double& get_warning_gradient_norm(void) const;
   const double& get_warning_training_rate(void) const;

   const double& get_error_parameters_norm(void) const;
   const double& get_error_gradient_norm(void) const;
   const double& get_error_training_rate(void) const;

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

   const bool& get_reserve_training_direction_history(void) const;
   const bool& get_reserve_training_rate_history(void) const;
   const bool& get_reserve_elapsed_time_history(void) const;

   // Utilities

   const unsigned int& get_display_period(void) const;

   // Set methods

   void set_inverse_Hessian_approximation_method(const InverseHessianApproximationMethod&);
   void set_inverse_Hessian_approximation_method(const std::string&);

   void set_default(void);

   // Training parameters

   void set_warning_parameters_norm(const double&);
   void set_warning_gradient_norm(const double&);
   void set_warning_training_rate(const double&);

   void set_error_parameters_norm(const double&);
   void set_error_gradient_norm(const double&);
   void set_error_training_rate(const double&);

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

   void set_reserve_training_direction_history(const bool&);
   void set_reserve_training_rate_history(const bool&);
   void set_reserve_elapsed_time_history(const bool&);

   // Utilities

   void set_display_period(const unsigned int&);

   // Training methods

   Vector<double> calculate_training_direction(const Vector<double>&, const Matrix<double>&) const;

   Matrix<double> calculate_inverse_Hessian_approximation(const Vector<double>&, const Vector<double>&, const Vector<double>&, const Vector<double>&, const Matrix<double>&) const;

   Vector<double> calculate_gradient_descent_training_direction(const Vector<double>&) const;

   Matrix<double> calculate_DFP_inverse_Hessian
   (const Vector<double>&, const Vector<double>&, const Vector<double>&, const Vector<double>&, const Matrix<double>&) const;

   Matrix<double> calculate_BFGS_inverse_Hessian
   (const Vector<double>&, const Vector<double>&, const Vector<double>&, const Vector<double>&, const Matrix<double>&) const;

   QuasiNetwonMethodResults* perform_training(void);

   // Training history methods

   void set_reserve_all_training_history(const bool&);

   std::string write_training_algorithm_type(void) const;

   // Serialization methods

   TiXmlElement* to_XML(void) const;
   void from_XML(TiXmlElement*);

  
private: 

   /// Training rate algorithm object. 

   TrainingRateAlgorithm training_rate_algorithm;

   /// Variable containing the actual method used to obtain a suitable training rate. 

   InverseHessianApproximationMethod inverse_Hessian_approximation_method;


   /// Value for the parameters norm at which a warning message is written to the screen. 

   double warning_parameters_norm;

   /// Value for the gradient norm at which a warning message is written to the screen. 

   double warning_gradient_norm;   

   /// Training rate value at wich a warning message is written to the screen.

   double warning_training_rate;

   /// Value for the parameters norm at which the training process is assumed to fail. 
   
   double error_parameters_norm;

   /// Value for the gradient norm at which the training process is assumed to fail. 

   double error_gradient_norm;

   /// Training rate at wich the line minimization algorithm is assumed to be unable to bracket a minimum.

   double error_training_rate;


   // STOPPING CRITERIA

   /// Norm of the parameters increment vector at which training stops.

   double minimum_parameters_increment_norm;

   /// Minimum performance improvement between two successive epochs. It is used as a perform_training stopping criterion.

   double minimum_performance_increase;

   /// Goal value for the performance. It is used as a perform_training stopping criterion.

   double performance_goal;

   /// Goal value for the norm of the objective function gradient. It is used as a stopping criterion.

   double gradient_norm_goal;

   /// Maximum number of epochs in which the generalization evaluation can decrease.  It is used as a stopping criterion.

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

   /// True if the training direction history matrix is to be reserved, false otherwise.
   
   bool reserve_training_direction_history;

   /// True if the training rate history vector is to be reserved, false otherwise.

   bool reserve_training_rate_history;

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
