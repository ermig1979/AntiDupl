/****************************************************************************************************************/
/*                                                                                                              */
/*   OpenNN: Open Neural Networks Library                                                                       */
/*   www.opennn.cimne.com                                                                                       */
/*                                                                                                              */
/*   R A N D O M   S E A R C H   C L A S S   H E A D E R                                                        */
/*                                                                                                              */ 
/*   Roberto Lopez                                                                                              */ 
/*   International Center for Numerical Methods in Engineering (CIMNE)                                          */
/*   Technical University of Catalonia (UPC)                                                                    */
/*   Barcelona, Spain                                                                                           */
/*   E-mail: rlopez@cimne.upc.edu                                                                               */ 
/*                                                                                                              */
/****************************************************************************************************************/

#ifndef __RANDOMSEARCH_H__
#define __RANDOMSEARCH_H__

// OpenNN includes

#include "../performance_functional/performance_functional.h"

#include "training_algorithm.h"
 
// TinyXml includes

#include "../../tinyxml/tinyxml.h"

namespace OpenNN
{

///
/// This concrete class represents a random search training algorithm for a performance functional of a neural network.
///

class RandomSearch : public TrainingAlgorithm
{

public:

   // DEFAULT CONSTRUCTOR

   explicit RandomSearch(void); 


   // PERFORMANCE FUNCTIONAL CONSTRUCTOR

   explicit RandomSearch(PerformanceFunctional*); 


   // XML CONSTRUCTOR

   explicit RandomSearch(TiXmlElement*); 


   // DESTRUCTOR

   virtual ~RandomSearch(void);

   // STRUCTURES

   ///
   /// This structure contains the training results for the random search. 
   ///

   struct RandomSearchResults : public TrainingAlgorithm::Results
   {  
      // TRAINING HISTORY

      /// History of the neural network parameters over the training epochs. 

      Vector< Vector<double> > parameters_history;

      /// History of the parameters norm over the training epochs. 

      Vector<double> parameters_norm_history;

      /// History of the performance function evaluation over the training epochs. 

      Vector<double> evaluation_history;

      /// History of the generalization evaluation over the training epochs. 

      Vector<double> generalization_evaluation_history;

      /// History of the random search training direction over the training epochs. 

      Vector< Vector<double> > training_direction_history;

      /// History of the random search training rate over the training epochs. 

      Vector<double> training_rate_history;

      /// History of the elapsed time over the training epochs. 

      Vector<double> elapsed_time_history;

      // FINAL VALUES

      /// Final neural network parameters vector. 

      Vector<double> final_parameters;

      /// Final neural network parameters norm. 

      double final_parameters_norm;

      /// Final performance function evaluation. 

      double final_evaluation;

      /// Final generalization evaluation. 

      double final_generalization_evaluation;

      /// Final random search training direction. 

      Vector<double> final_training_direction;

      /// Final random search training rate. 

      double final_training_rate;

      /// Elapsed time of the training process. 

      double elapsed_time;

      // METHODS

      void resize_training_history(const unsigned int&);

      std::string to_string(void) const;
  };


   // METHODS

   // Get methods

   const double& get_training_rate_reduction_factor(void) const;
   const unsigned int& get_training_rate_reduction_period(void) const;

   const bool& get_reserve_parameters_history(void) const;
   const bool& get_reserve_parameters_norm_history(void) const;

   const bool& get_reserve_evaluation_history(void) const;

   // Training parameters

   const double& get_warning_parameters_norm(void) const;
   const double& get_warning_training_rate(void) const;

   const double& get_error_parameters_norm(void) const;
   const double& get_error_training_rate(void) const;

   // Stopping criteria

   const double& get_minimum_parameters_increment_norm(void) const;

   const double& get_minimum_performance_increase(void) const;
   const double& get_performance_goal(void) const;
   const unsigned int& get_maximum_generalization_evaluation_decreases(void) const;

   const unsigned int& get_maximum_epochs_number(void) const;
   const double& get_maximum_time(void) const;

   // Reserve training history

   const bool& get_reserve_generalization_evaluation_history(void) const;

   const bool& get_reserve_training_direction_history(void) const;
   const bool& get_reserve_training_rate_history(void) const;
   const bool& get_reserve_elapsed_time_history(void) const;

   // Utilities

   const unsigned int& get_display_period(void) const;

   // Set methods

   void set_default(void);

   void set_training_rate_reduction_factor(const double&);
   void set_training_rate_reduction_period(const unsigned int&);

   void set_reserve_parameters_history(const bool&);
   void set_reserve_parameters_norm_history(const bool&);

   void set_reserve_evaluation_history(const bool&);

   // Training parameters

   void set_warning_parameters_norm(const double&);
   void set_warning_training_rate(const double&);

   void set_error_parameters_norm(const double&);
   void set_error_training_rate(const double&);

   // Stopping criteria

   void set_minimum_parameters_increment_norm(const double&);

   void set_minimum_performance_increase(const double&);
   void set_performance_goal(const double&);
   void set_maximum_generalization_evaluation_decreases(const unsigned int&);

   void set_maximum_epochs_number(const unsigned int&);
   void set_maximum_time(const double&);

   // Reserve training history

   void set_reserve_generalization_evaluation_history(const bool&);

   void set_reserve_training_direction_history(const bool&);
   void set_reserve_training_rate_history(const bool&);
   void set_reserve_elapsed_time_history(const bool&);

   /// This method makes the training history of all variables to be reseved or not in memory.

   virtual void set_reserve_all_training_history(const bool&);

   // Utilities

   void set_display_period(const unsigned int&);

   // Training methods

   Vector<double> calculate_training_direction(void);

   RandomSearchResults* perform_training(void);

   std::string write_training_algorithm_type(void) const;

   // Serialization methods

   TiXmlElement* to_XML(void) const;
   void from_XML(TiXmlElement*);

private: 

   // MEMBERS

   // TRAINING PARAMETERS

   double first_training_rate;

   double training_rate_reduction_factor;

   unsigned int training_rate_reduction_period;

   /// Value for the parameters norm at which a warning message is written to the screen. 

   double warning_parameters_norm;

   /// Training rate value at wich a warning message is written to the screen.

   double warning_training_rate;

   /// Value for the parameters norm at which the training process is assumed to fail. 
   
   double error_parameters_norm;

   /// Training rate at wich the line minimization algorithm is assumed to be unable to bracket a minimum.

   double error_training_rate;


   // STOPPING CRITERIA

   /// Norm of the parameters increment vector at which training stops.

   double minimum_parameters_increment_norm;

   /// Minimum performance improvement between two successive epochs. It is used as a perform_training stopping criterion.

   double minimum_performance_increase;

   /// Goal value for the performance. It is used as a perform_training stopping criterion.

   double performance_goal;


   unsigned int maximum_generalization_evaluation_decreases;

   /// Maximum number of epochs to perform_training. It is used as a perform_training stopping criterion.

   unsigned int maximum_epochs_number;

   /// Maximum training time. It is used as a perform_training stopping criterion.

   double maximum_time;


   // TRAINING HISTORY


   bool reserve_parameters_history;
   bool reserve_parameters_norm_history;

   bool reserve_evaluation_history; 

   /// True if the Generalization evaluation history vector is to be reserved, false otherwise. 

   bool reserve_generalization_evaluation_history;


   /// True if the training direction history matrix is to be reserved, false otherwise.
   
   bool reserve_training_direction_history;

   bool reserve_training_direction_norm_history;

   /// True if the training rate history vector is to be reserved, false otherwise.

   bool reserve_training_rate_history;

   /// True if the elapsed time history vector is to be reserved, false otherwise.

   bool reserve_elapsed_time_history;


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
