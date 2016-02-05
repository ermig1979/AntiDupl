/****************************************************************************************************************/
/*                                                                                                              */
/*   OpenNN: Open Neural Networks Library                                                                       */
/*   www.opennn.cimne.com                                                                                       */
/*                                                                                                              */
/*   G R A D I E N T   D E S C E N T   C L A S S   H E A D E R                                                  */
/*                                                                                                              */
/*   Roberto Lopez                                                                                              */
/*   International Center for Numerical Methods in Engineering (CIMNE)                                          */
/*   Technical University of Catalonia (UPC)                                                                    */
/*   Barcelona, Spain                                                                                           */
/*   E-mail: rlopez@cimne.upc.edu                                                                               */
/*                                                                                                              */
/****************************************************************************************************************/

#ifndef __GRADIENTDESCENT_H__
#define __GRADIENTDESCENT_H__

// OpenNN includes

#include "../performance_functional/performance_functional.h"

#include "training_algorithm.h"
#include "training_rate_algorithm.h"


namespace OpenNN
{

/// This concrete class represents the gradient descent training algorithm for a performance functional of a 
/// multilayer perceptron.

class GradientDescent : public TrainingAlgorithm
{

public:

   // DEFAULT CONSTRUCTOR

   explicit GradientDescent(void); 

   // PERFORMANCE FUNCTIONAL CONSTRUCTOR

   explicit GradientDescent(PerformanceFunctional*);

   // XML CONSTRUCTOR

   explicit GradientDescent(TiXmlElement*); 


   // DESTRUCTOR

   virtual ~GradientDescent(void);

   // STRUCTURES

   ///
   /// This structure contains the training results for the gradient descent. 
   ///

   struct GradientDescentResults : public TrainingAlgorithm::Results
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

      Vector< Vector<double> >  gradient_history;

      /// History of the gradient norm over the training epochs. 

      Vector<double> gradient_norm_history;

      /// History of the random search training direction over the training epochs. 

      Vector< Vector<double> >  training_direction_history;

      /// History of the random search training rate over the training epochs. 

      Vector<double> training_rate_history;

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

      /// Final gradient descent training direction. 

      Vector<double> final_training_direction;

      /// Final gradient descent training rate. 

      double final_training_rate;

      /// Elapsed time of the training process. 

      double elapsed_time;

      void resize_training_history(const unsigned int&);
      std::string to_string(void) const;
   };

   // METHODS

   const TrainingRateAlgorithm& get_training_rate_algorithm(void) const;
   TrainingRateAlgorithm* get_training_rate_algorithm_pointer(void);

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
   const bool& get_reserve_generalization_evaluation_history(void) const;

   const bool& get_reserve_training_direction_history(void) const;
   const bool& get_reserve_training_rate_history(void) const;
   const bool& get_reserve_elapsed_time_history(void) const;

   // Utilities

   const unsigned int& get_display_period(void) const;

   // Set methods

   void set_training_rate_algorithm(const TrainingRateAlgorithm&);


   void set_default(void);

   void set_reserve_all_training_history(const bool&);


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
   void set_reserve_generalization_evaluation_history(const bool&);

   void set_reserve_training_direction_history(const bool&);
   void set_reserve_training_rate_history(const bool&);
   void set_reserve_elapsed_time_history(const bool&);

   // Utilities

   void set_display_period(const unsigned int&);

   // Training methods

   Vector<double> calculate_training_direction(const Vector<double>&) const;

   GradientDescentResults* perform_training(void);

   std::string write_training_algorithm_type(void) const;

   // Serialization methods

   TiXmlElement* to_XML(void) const;
   void from_XML(TiXmlElement*);

private:

   // TRAINING OPERATORS

   /// Training rate algorithm object for one-dimensional minimization. 

   TrainingRateAlgorithm training_rate_algorithm;

   // TRAINING PARAMETERS

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
