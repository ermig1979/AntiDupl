/****************************************************************************************************************/
/*                                                                                                              */
/*   OpenNN: Open Neural Networks Library                                                                       */
/*   www.opennn.cimne.com                                                                                       */
/*                                                                                                              */
/*   L E V E N B E R G - M A R Q U A R D T   A L G O R I T H M   C L A S S                                      */
/*                                                                                                              */
/*   Roberto Lopez                                                                                              */
/*   International Center for Numerical Methods in Engineering (CIMNE)                                          */
/*   Technical University of Catalonia (UPC)                                                                    */
/*   Barcelona, Spain                                                                                           */
/*   E-mail: rlopez@cimne.upc.edu                                                                               */
/*                                                                                                              */
/****************************************************************************************************************/

// System includes

#include <string>
#include <sstream>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <functional>
#include <limits>
#include <math.h>
#include <time.h>

// OpenNN includes

#include "levenberg_marquardt_algorithm.h"

#include "../data_set/data_set.h"
//#include "../utilities/linear_algebraic_equations.h"

namespace OpenNN
{

// DEFAULT CONSTRUCTOR

/// Default constructor. 
/// It creates a Levenberg-Marquardt training algorithm object not associated to any performance functional object. 
/// It also initializes the class members to their default values.

LevenbergMarquardtAlgorithm::LevenbergMarquardtAlgorithm(void)
 : TrainingAlgorithm()
{
   set_default();
}


// PERFORMANCE FUNCTIONAL CONSTRUCTOR

/// Performance functional constructor. 
/// It creates a Levenberg-Marquardt training algorithm object associated associated with a given performance functional object. 
/// It also initializes the class members to their default values.
/// @param new_performance_functional_pointer Pointer to an external performance functional object. 

LevenbergMarquardtAlgorithm::LevenbergMarquardtAlgorithm(PerformanceFunctional* new_performance_functional_pointer)
 : TrainingAlgorithm(new_performance_functional_pointer)
{
   set_default();
}


// XML CONSTRUCTOR

/// XML Constructor.
/// This method creates a Levenberg-Marquardt algorithm object, and loads its members from a XML element. 
/// @param Levenberg_Marquardt_algorithm_element Pointer to a TinyXML element containing the Levenberg-Marquardt algorithm data. 

LevenbergMarquardtAlgorithm::LevenbergMarquardtAlgorithm(TiXmlElement* Levenberg_Marquardt_algorithm_element) 
 : TrainingAlgorithm(Levenberg_Marquardt_algorithm_element)
{
   set_default();

   from_XML(Levenberg_Marquardt_algorithm_element);
}


// DESTRUCTOR

/// Destructor.
/// This destructor does not delete any object. 

LevenbergMarquardtAlgorithm::~LevenbergMarquardtAlgorithm(void)
{
}


// const double& get_warning_parameters_norm(void) const method

/// This method returns the minimum value for the norm of the parameters vector at wich a warning message is 
/// written to the screen. 

const double& LevenbergMarquardtAlgorithm::get_warning_parameters_norm(void) const
{
   return(warning_parameters_norm);       
}


// const double& get_warning_gradient_norm(void) const method

/// This method returns the minimum value for the norm of the gradient vector at wich a warning message is written
/// to the screen. 

const double& LevenbergMarquardtAlgorithm::get_warning_gradient_norm(void) const
{
   return(warning_gradient_norm);       
}


// const double& get_error_parameters_norm(void) const method

/// This method returns the value for the norm of the parameters vector at wich an error message is 
/// written to the screen and the program exits. 

const double& LevenbergMarquardtAlgorithm::get_error_parameters_norm(void) const
{
   return(error_parameters_norm);
}


// const double& get_error_gradient_norm(void) const method

/// This method returns the value for the norm of the gradient vector at wich an error message is written
/// to the screen and the program exits. 

const double& LevenbergMarquardtAlgorithm::get_error_gradient_norm(void) const
{
   return(error_gradient_norm);
}


// const double& get_minimum_parameters_increment_norm(void) const method

/// This method returns the minimum norm of the parameter increment vector used as a stopping criteria when training. 

const double& LevenbergMarquardtAlgorithm::get_minimum_parameters_increment_norm(void) const
{
   return(minimum_parameters_increment_norm);
}


// const double& get_minimum_performance_increase(void) const method

/// This method returns the minimum performance improvement during training.  

const double& LevenbergMarquardtAlgorithm::get_minimum_performance_increase(void) const
{
   return(minimum_performance_increase);
}


// const double& get_performance_goal(void) const method

/// This method returns the goal value for the performance. 
/// This is used as a stopping criterium when training a multilayer perceptron

const double& LevenbergMarquardtAlgorithm::get_performance_goal(void) const
{
   return(performance_goal);
}


// const double& get_gradient_norm_goal(void) const method

/// This method returns the goal value for the norm of the objective function gradient.
/// This is used as a stopping criterium when training a multilayer perceptron

const double& LevenbergMarquardtAlgorithm::get_gradient_norm_goal(void) const
{
   return(gradient_norm_goal);
}


// const unsigned int& get_maximum_generalization_evaluation_decreases(void) const method

/// This method returns the maximum number of generalization failures during the training process. 

const unsigned int& LevenbergMarquardtAlgorithm::get_maximum_generalization_evaluation_decreases(void) const
{
   return(maximum_generalization_evaluation_decreases);
}


// const unsigned int& get_maximum_epochs_number(void) const method

/// This method returns the maximum number of epochs for training.

const unsigned int& LevenbergMarquardtAlgorithm::get_maximum_epochs_number(void) const
{
   return(maximum_epochs_number);
}


// const double& get_maximum_time(void) const method

/// This method returns the maximum training time.  

const double& LevenbergMarquardtAlgorithm::get_maximum_time(void) const
{
   return(maximum_time);
}


// const bool& get_reserve_parameters_history(void) const method

/// This method returns true if the parameters history matrix is to be reserved, and false otherwise.

const bool& LevenbergMarquardtAlgorithm::get_reserve_parameters_history(void) const
{
   return(reserve_parameters_history);     
}


// const bool& get_reserve_parameters_norm_history(void) const method 

/// This method returns true if the parameters norm history vector is to be reserved, and false otherwise.

const bool& LevenbergMarquardtAlgorithm::get_reserve_parameters_norm_history(void) const
{
   return(reserve_parameters_norm_history);     
}


// const bool& get_reserve_evaluation_history(void) const method

/// This method returns true if the evaluation history vector is to be reserved, and false otherwise.

const bool& LevenbergMarquardtAlgorithm::get_reserve_evaluation_history(void) const
{
   return(reserve_evaluation_history);     
}


// const bool& get_reserve_gradient_history(void) const method

/// This method returns true if the gradient history vector of vectors is to be reserved, and false otherwise.

const bool& LevenbergMarquardtAlgorithm::get_reserve_gradient_history(void) const
{
   return(reserve_gradient_history);     
}


// const bool& get_reserve_gradient_norm_history(void) const method

/// This method returns true if the gradient norm history vector is to be reserved, and false otherwise.

const bool& LevenbergMarquardtAlgorithm::get_reserve_gradient_norm_history(void) const
{
   return(reserve_gradient_norm_history);     
}


// const bool& get_reserve_inverse_Hessian_history(void) const method

/// This method returns true if the inverse Hessian history vector of matrices is to be reserved, and false otherwise.

const bool& LevenbergMarquardtAlgorithm::get_reserve_inverse_Hessian_history(void) const
{
   return(reserve_inverse_Hessian_history);
}


// const bool& get_reserve_elapsed_time_history(void) const method

/// This method returns true if the elapsed time history vector is to be reserved, and false otherwise.

const bool& LevenbergMarquardtAlgorithm::get_reserve_elapsed_time_history(void) const
{
   return(reserve_elapsed_time_history);     
}


// const bool& get_reserve_generalization_evaluation_history(void) const method

/// This method returns true if the Generalization evaluation history vector is to be reserved, and false otherwise.

const bool& LevenbergMarquardtAlgorithm::get_reserve_generalization_evaluation_history(void) const
{
   return(reserve_generalization_evaluation_history);
}


// const unsigned int& get_display_period(void) const method

/// This method returns the number of epochs between the training showing progress. 

const unsigned int& LevenbergMarquardtAlgorithm::get_display_period(void) const
{
   return(display_period);
}


// const double& get_damping_parameter(void) const method

/// This method returns the damping parameter for the Hessian approximation. 

const double& LevenbergMarquardtAlgorithm::get_damping_parameter(void) const
{
   return(damping_parameter);
}


// const double& get_damping_parameter_factor(void) const method

/// This method returns the damping parameter factor (beta in the User's Guide) for the Hessian approximation. 

const double& LevenbergMarquardtAlgorithm::get_damping_parameter_factor(void) const
{
   return(damping_parameter_factor);
}


// const double& get_minimum_damping_parameter(void) const method

/// This method returns the minimum damping parameter allowed in the algorithm. 

const double& LevenbergMarquardtAlgorithm::get_minimum_damping_parameter(void) const
{
   return(minimum_damping_parameter);
}


// const double& get_maximum_damping_parameter(void) const method

/// This method returns the maximum damping parameter allowed in the algorithm. 

const double& LevenbergMarquardtAlgorithm::get_maximum_damping_parameter(void) const
{
   return(maximum_damping_parameter);
}


// const bool& get_reserve_damping_parameter_history(void) const method

/// This method returns true if the damping parameter history vector is to be reserved, and false otherwise.

const bool& LevenbergMarquardtAlgorithm::get_reserve_damping_parameter_history(void) const
{
   return(reserve_damping_parameter_history);
}


// const Vector<double> const get_damping_parameter_history(void) const method

/// This method returns a vector containing the damping parameter history over the training epochs.

const Vector<double>& LevenbergMarquardtAlgorithm::get_damping_parameter_history(void) const
{
   return(damping_parameter_history);
}


// void set_default(void) method

/// This method sets the following default values for the Levenberg-Marquardt algorithm:
/// Training parameters:
/// <ul>
/// <li> Levenberg-Marquardt parameter: 0.001.
/// </ul>
/// Stopping criteria:
/// <ul> 
/// <li> Performance goal: 0.0.
/// <li> Gradient approximation norm goal: 0.0.
/// <li> Maximum training time: 1.0e6.
/// <li> Maximum number of epochs: 100. 
/// </ul> 
/// User stuff: 
/// <ul>
/// <li> Epochs between showing progress: 10. 
/// </ul>

void LevenbergMarquardtAlgorithm::set_default(void)
{
   // TRAINING PARAMETERS

   warning_parameters_norm = 1.0e6;
   warning_gradient_norm = 1.0e6;   

   error_parameters_norm = 1.0e9;
   error_gradient_norm = 1.0e9;

   // STOPPING CRITERIA

   minimum_parameters_increment_norm = 0.0;

   minimum_performance_increase = 0.0;
   performance_goal = -1.0e99;
   gradient_norm_goal = 0.0;
   maximum_generalization_evaluation_decreases = 1000000;

   maximum_epochs_number = 1000;
   maximum_time = 1000.0;

   // TRAINING HISTORY

   reserve_parameters_history = false;
   reserve_parameters_norm_history = false;

   reserve_evaluation_history = true;
   reserve_gradient_history = false;
   reserve_gradient_norm_history = false;
   reserve_generalization_evaluation_history = false;

   reserve_elapsed_time_history = false;

   // UTILITIES

   display = true;
   display_period = 100;

   // Training parameters

   damping_parameter = 1.0e-3;

   damping_parameter_factor = 10.0;

   minimum_damping_parameter = 1.0e-6;
   maximum_damping_parameter = 1.0e6;

   reserve_damping_parameter_history = false;
}


// void set_damping_parameter(const double&) method

/// This method sets a new damping parameter (lambda in the User's Guide) for the Hessian approximation. 
/// @param new_damping_parameter Damping parameter value. 

void LevenbergMarquardtAlgorithm::set_damping_parameter(const double& new_damping_parameter)
{
   if(new_damping_parameter <= minimum_damping_parameter)
   {
      damping_parameter = minimum_damping_parameter;
   }
   else if(new_damping_parameter >= maximum_damping_parameter)
   {
      damping_parameter = maximum_damping_parameter;
   }
   else
   {
      damping_parameter = new_damping_parameter;
   }
}


// void set_damping_parameter_factor(const double&) method

/// This method sets a new damping parameter factor (beta in the User's Guide) for the Hessian approximation. 
/// @param new_damping_parameter_factor Damping parameter factor value. 

void LevenbergMarquardtAlgorithm::set_damping_parameter_factor(const double& new_damping_parameter_factor)
{
   #ifdef _DEBUG 

   if(new_damping_parameter_factor <= 0.0)
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: LevenbergMarquardtAlgorithm class." << std::endl
             << "void set_damping_parameter_factor(const double&) method." << std::endl
             << "Damping parameter factor must be greater than zero." << std::endl;

      throw std::logic_error(buffer.str().c_str());	  
   }

   #endif

   damping_parameter_factor = new_damping_parameter_factor;
}


// void set_minimum_damping_parameter(const double&) method

/// This method sets a new minimum damping parameter allowed in the algorithm. 
/// @param new_minimum_damping_parameter Minimum damping parameter value. 

void LevenbergMarquardtAlgorithm::set_minimum_damping_parameter(const double& new_minimum_damping_parameter)
{
   #ifdef _DEBUG 

   if(new_minimum_damping_parameter <= 0.0)
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: LevenbergMarquardtAlgorithm class." << std::endl
             << "void set_minimum_damping_parameter(const double&) method." << std::endl
             << "Minimum damping parameter must be greater than zero." << std::endl;

      throw std::logic_error(buffer.str().c_str());	  
   }

   #endif

   minimum_damping_parameter = new_minimum_damping_parameter;
}


// void set_maximum_damping_parameter(const double&) method

/// This method sets a new maximum damping parameter allowed in the algorithm. 
/// @param new_maximum_damping_parameter Maximum damping parameter value. 

void LevenbergMarquardtAlgorithm::set_maximum_damping_parameter(const double& new_maximum_damping_parameter)
{
   #ifdef _DEBUG 

   if(new_maximum_damping_parameter <= 0.0)
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: LevenbergMarquardtAlgorithm class." << std::endl
             << "void set_maximum_damping_parameter(const double&) method." << std::endl
             << "Maximum damping parameter must be greater than zero." << std::endl;

      throw std::logic_error(buffer.str().c_str());	  
   }

   #endif

   maximum_damping_parameter = new_maximum_damping_parameter;
}


// void set_reserve_damping_parameter_history(bool) method

/// This method makes the damping parameter history vector to be reseved or not in memory.
/// @param new_reserve_damping_parameter_history True if the damping parameter history vector is to be reserved, false otherwise.

void LevenbergMarquardtAlgorithm::set_reserve_damping_parameter_history(const bool& new_reserve_damping_parameter_history)
{
   reserve_damping_parameter_history = new_reserve_damping_parameter_history;
}


// void set_warning_parameters_norm(const double&) method

/// This method sets a new value for the parameters vector norm at which a warning message is written to the 
/// screen. 
/// @param new_warning_parameters_norm Warning norm of parameters vector value. 

void LevenbergMarquardtAlgorithm::set_warning_parameters_norm(const double& new_warning_parameters_norm)
{
   // Control sentence (if debug)

   #ifdef _DEBUG 

   if(new_warning_parameters_norm < 0.0)
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: TrainingAlgorithm class.\n"
             << "void set_warning_parameters_norm(const double&) method.\n"
             << "Warning parameters norm must be equal or greater than 0.\n";

      throw std::logic_error(buffer.str().c_str());	  
   }

   #endif

   // Set warning parameters norm

   warning_parameters_norm = new_warning_parameters_norm;     
}


// void set_warning_gradient_norm(const double&) method

/// This method sets a new value for the gradient vector norm at which 
/// a warning message is written to the screen. 
/// @param new_warning_gradient_norm Warning norm of gradient vector value. 

void LevenbergMarquardtAlgorithm::set_warning_gradient_norm(const double& new_warning_gradient_norm)
{
   // Control sentence (if debug)

   #ifdef _DEBUG 

   if(new_warning_gradient_norm < 0.0)
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: TrainingAlgorithm class.\n"
             << "void set_warning_gradient_norm(const double&) method.\n"
             << "Warning gradient norm must be equal or greater than 0.\n";

      throw std::logic_error(buffer.str().c_str());	  
   }

   #endif

   // Set warning gradient norm

   warning_gradient_norm = new_warning_gradient_norm;     
}


// void set_error_parameters_norm(const double&) method

/// This method sets a new value for the parameters vector norm at which an error message is written to the 
/// screen and the program exits. 
/// @param new_error_parameters_norm Error norm of parameters vector value. 

void LevenbergMarquardtAlgorithm::set_error_parameters_norm(const double& new_error_parameters_norm)
{
   // Control sentence (if debug)

   #ifdef _DEBUG 

   if(new_error_parameters_norm < 0.0)
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: TrainingAlgorithm class.\n"
             << "void set_error_parameters_norm(const double&) method.\n"
             << "Error parameters norm must be equal or greater than 0.\n";

      throw std::logic_error(buffer.str().c_str());	  
   }

   #endif

   // Set error parameters norm

   error_parameters_norm = new_error_parameters_norm;
}


// void set_error_gradient_norm(const double&) method

/// This method sets a new value for the gradient vector norm at which an error message is written to the screen 
/// and the program exits. 
/// @param new_error_gradient_norm Error norm of gradient vector value. 

void LevenbergMarquardtAlgorithm::set_error_gradient_norm(const double& new_error_gradient_norm)
{
   // Control sentence (if debug)

   #ifdef _DEBUG 

   if(new_error_gradient_norm < 0.0)
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: TrainingAlgorithm class.\n"
             << "void set_error_gradient_norm(const double&) method.\n"
             << "Error gradient norm must be equal or greater than 0.\n";

      throw std::logic_error(buffer.str().c_str());	  
   }

   #endif

   // Set error gradient norm

   error_gradient_norm = new_error_gradient_norm;
}


// void set_minimum_parameters_increment_norm(const double&) method

/// This method sets a new value for the minimum parameters increment norm stopping criterium. 
/// @param new_minimum_parameters_increment_norm Value of norm of parameters increment norm used to stop training. 

void LevenbergMarquardtAlgorithm::set_minimum_parameters_increment_norm(const double& new_minimum_parameters_increment_norm)
{
   // Control sentence (if debug)

   #ifdef _DEBUG 

   if(new_minimum_parameters_increment_norm < 0.0)
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: TrainingAlgorithm class.\n"
             << "void new_minimum_parameters_increment_norm(const double&) method.\n"
             << "Minimum parameters increment norm must be equal or greater than 0.\n";

      throw std::logic_error(buffer.str().c_str());	  
   }

   #endif

   // Set error training rate

   minimum_parameters_increment_norm = new_minimum_parameters_increment_norm;
}


// void set_minimum_performance_increase(const double&) method

/// This method sets a new minimum performance improvement during training.  
/// @param new_minimum_performance_increase Minimum improvement in the performance between two epochs.

void LevenbergMarquardtAlgorithm::set_minimum_performance_increase(const double& new_minimum_performance_increase)
{
   // Control sentence (if debug)

   #ifdef _DEBUG 

   if(new_minimum_performance_increase < 0.0)
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: TrainingAlgorithm class.\n"
             << "void set_minimum_performance_increase(const double&) method.\n"
             << "Minimum performance improvement must be equal or greater than 0.\n";

      throw std::logic_error(buffer.str().c_str());	  
   }

   #endif

   // Set minimum performance improvement

   minimum_performance_increase = new_minimum_performance_increase;
}


// void set_performance_goal(const double&) method

/// This method sets a new goal value for the performance. 
/// This is used as a stopping criterium when training a multilayer perceptron
/// @param new_performance_goal Goal value for the performance.

void LevenbergMarquardtAlgorithm::set_performance_goal(const double& new_performance_goal)
{
   performance_goal = new_performance_goal;
}


// void set_gradient_norm_goal(const double&) method

/// This method sets a new the goal value for the norm of the objective function gradient. 
/// This is used as a stopping criterium when training a multilayer perceptron
/// @param new_gradient_norm_goal Goal value for the norm of the objective function gradient.

void LevenbergMarquardtAlgorithm::set_gradient_norm_goal(const double& new_gradient_norm_goal)
{
   // Control sentence (if debug)

   #ifdef _DEBUG 

   if(new_gradient_norm_goal < 0.0)
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: TrainingAlgorithm class.\n"
             << "void set_gradient_norm_goal(const double&) method.\n"
             << "Gradient norm goal must be equal or greater than 0.\n";

      throw std::logic_error(buffer.str().c_str());	  
   }

   #endif

   // Set gradient norm goal

   gradient_norm_goal = new_gradient_norm_goal;
}


// void set_maximum_generalization_evaluation_decreases(const unsigned int&) method

/// This method sets a new maximum number of generalization failures. 
/// @param new_maximum_generalization_evaluation_decreases Maximum number of epochs in which the generalization evalutation decreases. 

void LevenbergMarquardtAlgorithm::set_maximum_generalization_evaluation_decreases(const unsigned int& new_maximum_generalization_evaluation_decreases)
{
   // Control sentence (if debug)

   #ifdef _DEBUG 

   if(new_maximum_generalization_evaluation_decreases < 0)
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: TrainingAlgorithm class.\n"
             << "void set_maximum_generalization_evaluation_decreases(const unsigned int&) method.\n"
             << "Number of generalization performance decreases must be equal or greater than 0.\n";

      throw std::logic_error(buffer.str().c_str());	  
   }

   #endif

   // Set maximum generalization performace decrases

   maximum_generalization_evaluation_decreases = new_maximum_generalization_evaluation_decreases;
}


// void set_maximum_epochs_number(unsigned int) method

/// This method sets a maximum number of epochs for training.
/// @param new_maximum_epochs_number Maximum number of epochs for training.

void LevenbergMarquardtAlgorithm::set_maximum_epochs_number(const unsigned int& new_maximum_epochs_number)
{
   // Control sentence (if debug)

   #ifdef _DEBUG 

   if(new_maximum_epochs_number < 0)
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: TrainingAlgorithm class.\n"
             << "void set_maximum_epochs_number(unsigned int) method.\n"
             << "Number of epochs must be equal or greater than 0.\n";

      throw std::logic_error(buffer.str().c_str());	  
   }

   #endif

   // Set maximum epochs number

   maximum_epochs_number = new_maximum_epochs_number;
}


// void set_maximum_time(const double&) method

/// This method sets a new maximum training time.  
/// @param new_maximum_time Maximum training time.

void LevenbergMarquardtAlgorithm::set_maximum_time(const double& new_maximum_time)
{
   // Control sentence (if debug)

   #ifdef _DEBUG 

   if(new_maximum_time < 0.0)
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: TrainingAlgorithm class.\n"
             << "void set_maximum_time(const double&) method.\n"
             << "Maximum time must be equal or greater than 0.\n";

      throw std::logic_error(buffer.str().c_str());	  
   }
   
   #endif

   // Set maximum time

   maximum_time = new_maximum_time;
}


// void set_reserve_parameters_history(bool) method

/// This method makes the parameters history vector of vectors to be reseved or not in memory.
/// @param new_reserve_parameters_history True if the parameters history vector of vectors is to be reserved, false otherwise.

void LevenbergMarquardtAlgorithm::set_reserve_parameters_history(const bool& new_reserve_parameters_history)
{
   reserve_parameters_history = new_reserve_parameters_history;     
}


// void set_reserve_parameters_norm_history(bool) method

/// This method makes the parameters norm history vector to be reseved or not in memory.
/// @param new_reserve_parameters_norm_history True if the parameters norm history vector is to be reserved, false otherwise.

void LevenbergMarquardtAlgorithm::set_reserve_parameters_norm_history(const bool& new_reserve_parameters_norm_history)
{
   reserve_parameters_norm_history = new_reserve_parameters_norm_history;     
}


// void set_reserve_evaluation_history(bool) method

/// This method makes the evaluation history vector to be reseved or not in memory.
/// @param new_reserve_evaluation_history True if the evaluation history vector is to be reserved, false otherwise.

void LevenbergMarquardtAlgorithm::set_reserve_evaluation_history(const bool& new_reserve_evaluation_history)
{
   reserve_evaluation_history = new_reserve_evaluation_history;     
}


// void set_reserve_gradient_history(bool) method

/// This method makes the gradient history vector of vectors to be reseved or not in memory.
/// @param new_reserve_gradient_history True if the gradient history matrix is to be reserved, false otherwise.

void LevenbergMarquardtAlgorithm::set_reserve_gradient_history(const bool& new_reserve_gradient_history)
{
   reserve_gradient_history = new_reserve_gradient_history;    
}


// void set_reserve_gradient_norm_history(bool) method

/// This method makes the gradient norm history vector to be reseved or not in memory.
/// @param new_reserve_gradient_norm_history True if the gradient norm history matrix is to be reserved, false 
/// otherwise.

void LevenbergMarquardtAlgorithm::set_reserve_gradient_norm_history(const bool& new_reserve_gradient_norm_history)
{
   reserve_gradient_norm_history = new_reserve_gradient_norm_history;     
}


// void set_reserve_inverse_Hessian_history(bool) method

/// This method sets the history of the inverse of the Hessian matrix to be reserved or not in memory.
/// This is a vector of matrices. 
/// @param new_reserve_inverse_Hessian_history True if the inverse Hessian history is to be reserved, false otherwise. 

void LevenbergMarquardtAlgorithm::set_reserve_inverse_Hessian_history(const bool& new_reserve_inverse_Hessian_history)
{
   reserve_inverse_Hessian_history = new_reserve_inverse_Hessian_history;
}


// void set_reserve_elapsed_time_history(bool) method

/// This method makes the elapsed time over the epochs to be reseved or not in memory. This is a vector.
/// @param new_reserve_elapsed_time_history True if the elapsed time history vector is to be reserved, false 
/// otherwise.

void LevenbergMarquardtAlgorithm::set_reserve_elapsed_time_history(const bool& new_reserve_elapsed_time_history)
{
   reserve_elapsed_time_history = new_reserve_elapsed_time_history;     
}


// void set_reserve_generalization_evaluation_history(bool) method

/// This method makes the Generalization evaluation history to be reserved or not in memory. 
/// This is a vector. 
/// @param new_reserve_generalization_evaluation_history True if the Generalization evaluation history is to be reserved, false otherwise. 

void LevenbergMarquardtAlgorithm::set_reserve_generalization_evaluation_history(const bool& new_reserve_generalization_evaluation_history)  
{
   reserve_generalization_evaluation_history = new_reserve_generalization_evaluation_history;
}


// void set_display_period(unsigned int) method

/// This method sets a new number of epochs between the training showing progress. 
/// @param new_display_period
/// Number of epochs between the training showing progress. 

void LevenbergMarquardtAlgorithm::set_display_period(const unsigned int& new_display_period)
{
   // Control sentence (if debug)

   #ifdef _DEBUG 
     
   if(new_display_period <= 0)
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: TrainingAlgorithm class.\n"
             << "void set_display_period(const double&) method.\n"
             << "First training rate must be greater than 0.\n";

      throw std::logic_error(buffer.str().c_str());	  
   }

   #endif

   display_period = new_display_period;
}


// void check(void) const method

/// This method checks that the Levenberg-Marquard object is ok for training.  
/// In particular, it checks that:
/// <ul>
/// <li> The performance functional pointer associated to the training algorithm is not NULL,
/// <li> The neural network associated to that performance functional is neither NULL.
/// <li> The data set associated to that performance functional is neither NULL.
/// </ul>
/// If that checkings are not hold, an exception is thrown. 

void LevenbergMarquardtAlgorithm::check(void) const
{
   std::ostringstream buffer;

   if(!performance_functional_pointer)
   {
      buffer << "OpenNN Exception: LevenbergMarquardtAlgorithm class.\n"
             << "void check(void) const method.\n"
             << "Pointer to performance functional is NULL.\n";

      throw std::logic_error(buffer.str().c_str());	  
   }

   const PerformanceTerm* objective_term_pointer = performance_functional_pointer->get_objective_term_pointer();

   if(!objective_term_pointer)
   {
      buffer << "OpenNN Exception: LevenbergMarquardtAlgorithm class.\n"
             << "void check(void) const method.\n"
             << "Pointer to objective term in performance functional is NULL.\n";

      throw std::logic_error(buffer.str().c_str());	  
   }

   const DataSet* data_set_pointer = objective_term_pointer->get_data_set_pointer();

   if(!data_set_pointer)
   {
      buffer << "OpenNN Exception: LevenbergMarquardtAlgorithm class." << std::endl
             << "void check(void) const method.\n"
             << "Pointer to data set in objective term is NULL." << std::endl;

      throw std::logic_error(buffer.str().c_str());	  
   }

   const NeuralNetwork* neural_network_pointer = performance_functional_pointer->get_neural_network_pointer();

   if(!neural_network_pointer)
   {
      buffer << "OpenNN Exception: LevenbergMarquardtAlgorithm class." << std::endl
             << "void check(void) const method.\n"
             << "Pointer to neural network is NULL." << std::endl;

      throw std::logic_error(buffer.str().c_str());	  
   }
}

// Matrix<double> calculate_Hessian_approximation(const Matrix<double>&) const method

/// This method returns an approximation of the Hessian matrix of the objective function as a function of the objective terms Jacobian. 
/// @param Jacobian_terms Jacobian matrix of the objective terms function. 

Matrix<double> LevenbergMarquardtAlgorithm::calculate_Hessian_approximation(const Matrix<double>& Jacobian_terms) const
{
   // Control sentence (if debug)

   #ifdef _DEBUG 
   
   check();

   std::ostringstream buffer;

   const DataSet* data_set_pointer = performance_functional_pointer->get_objective_term_pointer()->get_data_set_pointer();

   const InstancesInformation& instances_information = data_set_pointer->get_instances_information();

   const unsigned int training_instances_number = instances_information.count_training_instances_number();

   const unsigned int rows_number = Jacobian_terms.get_rows_number();

   if(rows_number != training_instances_number)
   {
      buffer << "OpenNN Exception: LevenbergMarquardtAlgorithm class." << std::endl
             << "Matrix<double> calculate_Hessian_approximation(const Matrix<double>&) const method." << std::endl
             << "Number of rows of errors derivatives must be equal to number of training instances." << std::endl;

      throw std::logic_error(buffer.str().c_str());	  
   }

   #endif

   const NeuralNetwork* neural_network_pointer = performance_functional_pointer->get_neural_network_pointer();

   const MultilayerPerceptron* multilayer_perceptron_pointer = neural_network_pointer->get_multilayer_perceptron_pointer();

   #ifdef _DEBUG 

   if(!multilayer_perceptron_pointer)
   {
      buffer << "OpenNN Exception: LevenbergMarquardtAlgorithm class." << std::endl
             << "Matrix<double> calculate_Hessian_approximation(const Matrix<double>&) const method." << std::endl
             << "Pointer to multilayer perceptron in neural network is NULL." << std::endl;

      throw std::logic_error(buffer.str().c_str());	  
   }

   #endif

   const unsigned int network_parameters_number = multilayer_perceptron_pointer->count_parameters_number();

   #ifdef _DEBUG 

   const unsigned int columns_number = Jacobian_terms.get_columns_number();

   if(columns_number != network_parameters_number)
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: LevenbergMarquardtAlgorithm class." << std::endl
             << "Matrix<double> calculate_Hessian_approximation(const Matrix<double>&) const method." << std::endl
             << "Number of columns of errors derivatives must be equal to number of network parameters." << std::endl;

      throw std::logic_error(buffer.str().c_str());	  
   }

   #endif

   Matrix<double> identity(network_parameters_number, network_parameters_number);
   identity.initialize_identity();

   return(Jacobian_terms.calculate_transpose().dot(Jacobian_terms) +  identity*damping_parameter);
}


// Vector<double> calculate_gradient(const Vector<double>&, const Matrix<double>&) const method

/// This method returns the exact gradient vector of the objective function as a function of the objective terms vector and the objective terms Jacobian matrix. 
/// @param evaluation_terms Vector with the objective terms values. 
/// @param Jacobian_terms Jacobian matrix of the objective terms function. 

Vector<double> LevenbergMarquardtAlgorithm
::calculate_gradient(const Vector<double>& evaluation_terms, const Matrix<double>& Jacobian_terms) const
{
   // Control sentence (if debug)

   #ifdef _DEBUG 

   DataSet* data_set_pointer = performance_functional_pointer->get_objective_term_pointer()->get_data_set_pointer();

   const InstancesInformation& instances_information = data_set_pointer->get_instances_information();

   const unsigned int training_instances_number = instances_information.count_training_instances_number();

   const unsigned int size = evaluation_terms.size();

   if(size != training_instances_number)
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: LevenbergMarquardtAlgorithm class." << std::endl
             << "Vector<double> calculate_gradient(const Vector<double>&, const Matrix<double>&) const method." << std::endl
             << "Size of errors must be equal to number of training instances." << std::endl;

      throw std::logic_error(buffer.str().c_str());	  
   }

   const unsigned int rows_number = Jacobian_terms.get_rows_number();

   if(rows_number != training_instances_number)
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: LevenbergMarquardtAlgorithm class." << std::endl
             << "Vector<double> calculate_gradient(const Vector<double>&, const Matrix<double>&) const method." << std::endl
             << "Number of rows of errors derivatives must be equal to number of training instances." << std::endl;

      throw std::logic_error(buffer.str().c_str());	  
   }

   const NeuralNetwork* neural_network_pointer = performance_functional_pointer->get_neural_network_pointer();

   const MultilayerPerceptron* multilayer_perceptron_pointer = neural_network_pointer->get_multilayer_perceptron_pointer();

   const unsigned int network_parameters_number = multilayer_perceptron_pointer->count_parameters_number();

   const unsigned int columns_number = Jacobian_terms.get_columns_number();

   if(columns_number != network_parameters_number)
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: LevenbergMarquardtAlgorithm class." << std::endl
             << "Vector<double> calculate_gradient(const Vector<double>&, const Matrix<double>&) const method." << std::endl
             << "Number of columns of errors derivatives must be equal to number of network parameters." << std::endl;

      throw std::logic_error(buffer.str().c_str());	  
   }

   #endif

   return(Jacobian_terms.calculate_transpose().dot(evaluation_terms)*2.0);

}


// void resize_training_history(const unsigned int&) method

/// This method resizes all the training history variables. 
/// @param new_size Size of training history variables. 

void LevenbergMarquardtAlgorithm::LevenbergMarquardtAlgorithmResults::resize_training_history(const unsigned int& new_size)
{
   parameters_history.resize(new_size);
   parameters_norm_history.resize(new_size);

   evaluation_history.resize(new_size);
   generalization_evaluation_history.resize(new_size);
   gradient_history.resize(new_size);
   gradient_norm_history.resize(new_size);
   Hessian_approximation_history.resize(new_size); 

   damping_parameter_history.resize(new_size);
   elapsed_time_history.resize(new_size);   
}


// std::string to_string(void) const method

/// This method returns a string representation of the current Levenberg-Marquardt algorithm results structure. 

std::string LevenbergMarquardtAlgorithm::LevenbergMarquardtAlgorithmResults::to_string(void) const
{
   std::ostringstream buffer;

   // Parameters history

   if(!parameters_history.empty())
   {
      if(!parameters_history[0].empty())
      {
          buffer << "% Parameters history:\n"
                 << parameters_history << "\n"; 
	  }
   }

   // Parameters norm history

   if(!parameters_norm_history.empty())
   {
       buffer << "% Parameters norm history:\n"
              << parameters_norm_history << "\n"; 
   }
   
   // Performance history   

   if(!evaluation_history.empty())
   {
       buffer << "% Performance history:\n"
              << evaluation_history << "\n"; 
   }

   // Generalization evaluation history

   if(!generalization_evaluation_history.empty())
   {
       buffer << "% Generalization evaluation history:\n"
              << generalization_evaluation_history << "\n"; 
   }

   // Gradient history 

   if(!gradient_history.empty())
   {
      if(!gradient_history[0].empty())
      {
          buffer << "% Gradient history:\n"
                 << gradient_history << "\n"; 
	  }
   }

   // Gradient norm history

   if(!gradient_norm_history.empty())
   {
       buffer << "% Gradient norm history:\n"
              << gradient_norm_history << "\n"; 
   }

   // Hessian approximation history

   if(!Hessian_approximation_history.empty())
   {
      if(!Hessian_approximation_history[0].empty())
      {
          buffer << "% Hessian approximation history:\n"
                 << Hessian_approximation_history << "\n"; 
	  }
   }

   // Damping parameter history

   if(!damping_parameter_history.empty())
   {
       buffer << "% Damping parameter history:\n"
              << damping_parameter_history << "\n"; 
   }

   // Elapsed time history

   if(!elapsed_time_history.empty())
   {
       buffer << "% Elapsed time history:\n"
              << elapsed_time_history << "\n"; 
   }

   return(buffer.str());
}


// LevenbergMarquardtAlgorithmResults* perform_training(void) method

/// This method trains a neural network with an associated performance functional according to the Levenberg-Marquardt algorithm.
/// Training occurs according to the training parameters.

LevenbergMarquardtAlgorithm::LevenbergMarquardtAlgorithmResults* LevenbergMarquardtAlgorithm::perform_training(void)
{
   std::ostringstream buffer;

   buffer << "OpenNN Exception: LevenbergMarquardtAlgorithm class.\n"
          << "LevenbergMarquardtAlgorithmResults* perform_training(void) method.\n"
          << "This method is under development.\n";

   throw std::logic_error(buffer.str().c_str());	  

/*
   // Control sentence

   #ifdef _DEBUG 
  
   check();

   #endif

   // Start training

   if(display)
   {
      std::cout << "Training with Levenberg-Marquardt algorithm..." << std::endl;
   }

   LevenbergMarquardtAlgorithmResults* Levenberg_Marquardt_algorithm_training_results_pointer = new LevenbergMarquardtAlgorithmResults;

   Levenberg_Marquardt_algorithm_training_results_pointer->resize_training_history(1+maximum_epochs_number);

   // Neural network stuff

   NeuralNetwork* neural_network_pointer = performance_functional_pointer->get_neural_network_pointer();

   const unsigned int parameters_number = neural_network_pointer->count_parameters_number();

   Vector<double> parameters(parameters_number);

   double parameters_norm;

   // Data set stuff

   const DataSet* data_set_pointer = performance_functional_pointer->get_objective_term_pointer()->get_data_set_pointer();

   const InstancesInformation& instances_information = data_set_pointer->get_instances_information();

   const unsigned int training_instances_number = instances_information.count_training_instances_number();

   // Performance functional stuff

   const PerformanceTerm* objective_term_pointer = performance_functional_pointer->get_objective_term_pointer();

   double performance = 0.0;
   double old_performance = 0.0;
   double performance_increment = 0.0;

   double generalization_evaluation = 0.0; 
   double old_generalization_evaluation = 0.0;
   
   unsigned int generalization_evaluation_decreases_count = 0;

   Vector<double> evaluation_terms(training_instances_number);
   Matrix<double> Jacobian_terms(training_instances_number, parameters_number);

   Vector<double> gradient(parameters_number);

   double gradient_norm;

   Matrix<double> JacobianT_dot_Jacobian(parameters_number, parameters_number);

   Matrix<double> Hessian_approximation(parameters_number, parameters_number);

   // Training strategy stuff

   Vector<double> parameters_increment(parameters_number);
   double parameters_increment_norm;

   Matrix<double> identity(parameters_number, parameters_number);
   identity.initialize_identity();

   bool stop_training = false;

   time_t beginning_time, current_time;
   time(&beginning_time);
   double elapsed_time;

   // Main loop

   for(unsigned int epoch = 0; epoch <= maximum_epochs_number; epoch++)
   {
      // Neural network

      parameters = neural_network_pointer->arrange_parameters();

      parameters_norm = parameters.calculate_norm();

      if(display && parameters_norm >= warning_parameters_norm)
      {
         std::cout << "OpenNN Warning: Parameters norm is " << parameters_norm << "." << std::endl;          
      }

      // Performance functional 

	  evaluation_terms = objective_term_pointer->calculate_evaluation_terms();

      performance = (evaluation_terms*evaluation_terms).calculate_sum()/2.0; 

      if(epoch == 0)
      {      
         performance_increment = 0.0; 
      }
      else
      {
         performance_increment = performance - old_performance; 
      }

      Jacobian_terms = objective_term_pointer->calculate_Jacobian_terms();

	  gradient = Jacobian_terms.calculate_transpose().dot(evaluation_terms);    

      gradient_norm = gradient.calculate_norm();

      if(display && gradient_norm >= warning_gradient_norm)
      {
         std::cout << "OpenNN Warning: Gradient norm is " << gradient_norm << "." << std::endl;          
      }

      generalization_evaluation = performance_functional_pointer->calculate_generalization_evaluation();

      if(epoch != 0 && generalization_evaluation > old_generalization_evaluation)
      {
         generalization_evaluation_decreases_count++;	  
      }

      // Training algorithm  

      JacobianT_dot_Jacobian = Jacobian_terms.calculate_transpose().dot(Jacobian_terms);    

	  for(unsigned int i = 0; i < 3; i++)
      {
         Hessian_approximation = (JacobianT_dot_Jacobian + identity*damping_parameter);

         parameters_increment = gradient*(-1.0);         
         linear_algebraic_equations.perform_Gauss_Jordan_elimination(Hessian_approximation, parameters_increment);

         parameters += parameters_increment;
		 
         double new_performance = objective_term_pointer->calculate_evaluation(parameters);

         if(new_performance <= performance) // succesfull step
         {
//			 std::cout << "ok" << std::endl;
             set_damping_parameter(damping_parameter/damping_parameter_factor);

            break;
         }
         else
         {
             set_damping_parameter(damping_parameter*damping_parameter_factor);
//			 std::cout << "ko" << std::endl;

//            continue;
         }
      }

      parameters_increment_norm = parameters_increment.calculate_norm();
      
      // Elapsed time

      time(&current_time);
      elapsed_time = difftime(current_time, beginning_time);

      // Training history neural network

      if(reserve_parameters_history)
      {
         Levenberg_Marquardt_algorithm_training_results_pointer->parameters_history[epoch] = parameters;                                
      }

      if(reserve_parameters_norm_history)
      {
         Levenberg_Marquardt_algorithm_training_results_pointer->parameters_norm_history[epoch] = parameters_norm; 
      }

      // Training history performance functional

      if(reserve_evaluation_history)
      {
         Levenberg_Marquardt_algorithm_training_results_pointer->evaluation_history[epoch] = performance;
      }

      if(reserve_generalization_evaluation_history)
      {
         Levenberg_Marquardt_algorithm_training_results_pointer->generalization_evaluation_history[epoch] = generalization_evaluation;
      }

      if(reserve_gradient_history)
      {
         Levenberg_Marquardt_algorithm_training_results_pointer->gradient_history[epoch] = gradient;                                
      }

      if(reserve_gradient_norm_history)
      {
         Levenberg_Marquardt_algorithm_training_results_pointer->gradient_norm_history[epoch] = gradient_norm;
      }

      if(reserve_inverse_Hessian_history)
      {
         Levenberg_Marquardt_algorithm_training_results_pointer->Hessian_approximation_history[epoch] = Hessian_approximation; // as computed by linear algebraic equations object
      }

      // Training history training algorithm

      if(reserve_damping_parameter_history)
      {
         Levenberg_Marquardt_algorithm_training_results_pointer->damping_parameter_history[epoch] = damping_parameter;
      }

      if(reserve_elapsed_time_history)
      {
         Levenberg_Marquardt_algorithm_training_results_pointer->elapsed_time_history[epoch] = elapsed_time;
      }

	  // Stopping Criteria

      if(parameters_increment_norm <= minimum_parameters_increment_norm)
      {
         if(display)
         {
            std::cout << "Epoch " << epoch << ": Minimum parameters increment norm reached.\n"
			          << "Parameters increment norm: " << parameters_increment_norm << std::endl;
         }

         stop_training = true;
      }

      else if(performance <= performance_goal)
      {
         if(display)
         {
            std::cout << "Epoch " << epoch << ": Performance goal reached.\n";
         }

         stop_training = true;
      }

      else if(epoch != 0 && performance_increment <= minimum_performance_increase)
      {
         if(display)
         {
            std::cout << "Epoch " << epoch << ": Minimum performance increase reached.\n"
                      << "Performance increase: " << performance_increment << std::endl;
         }

         //stop_training = true;
      }

      else if(gradient_norm <= gradient_norm_goal)
      {
         if(display)
         {
            std::cout << "Epoch " << epoch << ": Gradient norm goal reached." << std::endl;  
         }

         stop_training = true;
      }

      else if(generalization_evaluation_decreases_count >= maximum_generalization_evaluation_decreases)
      {
         if(display)
         {
            std::cout << "Epoch " << epoch << ": Maximum generalization performance decreases reached.\n"
                      << "Generalization performance decreases: "<< generalization_evaluation_decreases_count << std::endl;
         }

         stop_training = true;
      }

      else if(epoch == maximum_epochs_number)
      {
         if(display)
         {
            std::cout << "Epoch " << epoch << ": Maximum number of epochs reached." << std::endl;
         }

         stop_training = true;
      }

      else if(elapsed_time >= maximum_time)
      {
         if(display)
         {
            std::cout << "Epoch " << epoch << ": Maximum training time reached." << std::endl;
         }

         stop_training = true;
      }

	  if(stop_training)
      {
         Levenberg_Marquardt_algorithm_training_results_pointer->final_parameters = parameters;
         Levenberg_Marquardt_algorithm_training_results_pointer->final_parameters_norm = parameters_norm;

         Levenberg_Marquardt_algorithm_training_results_pointer->final_evaluation = performance;
         Levenberg_Marquardt_algorithm_training_results_pointer->final_generalization_evaluation = generalization_evaluation;

         Levenberg_Marquardt_algorithm_training_results_pointer->final_gradient = gradient;
         Levenberg_Marquardt_algorithm_training_results_pointer->final_gradient_norm = gradient_norm;
   
         Levenberg_Marquardt_algorithm_training_results_pointer->elapsed_time = elapsed_time;

         if(display)
		 {
            std::cout << "Parameters norm: " << parameters_norm << "\n"
                      << "Performance: " << performance << "\n"
                      << "Gradient norm: " << gradient_norm << "\n"
                      << performance_functional_pointer->write_information()
			          << "Damping parameter: " << damping_parameter << "\n"
                      << "Elapsed time: " << elapsed_time << std::endl; 

            if(generalization_evaluation != 0)
            {
               std::cout << "Generalization performance: " << generalization_evaluation << std::endl;
            }
         }   
         break;
      }
      else if(display && epoch % display_period == 0)
      {
         std::cout << "Epoch " << epoch << ";\n" 
                   << "Parameters norm: " << parameters_norm << "\n"
                   << "Performance: " << performance << "\n"
                   << "Gradient norm: " << gradient_norm << "\n"
                   << performance_functional_pointer->write_information()
                   << "Damping parameter: " << damping_parameter << "\n"
                   << "Elapsed time: " << elapsed_time << std::endl; 

         if(generalization_evaluation != 0)
         {
            std::cout << "Generalization performance: " << generalization_evaluation << std::endl;
         }

      }

      // Update stuff

      old_performance = performance;
      old_generalization_evaluation = generalization_evaluation;

      // Set new parameters

      neural_network_pointer->set_parameters(parameters);
   } 

   return(Levenberg_Marquardt_algorithm_training_results_pointer);
*/
}


// void set_reserve_all_training_history(const bool&) method

void LevenbergMarquardtAlgorithm::set_reserve_all_training_history(const bool&)
{
   reserve_parameters_history = true;
   reserve_parameters_norm_history = true;

   reserve_evaluation_history = true;
   reserve_generalization_evaluation_history = true;

   reserve_gradient_history = true;
   reserve_gradient_norm_history = true;
   reserve_inverse_Hessian_history = true;

   reserve_damping_parameter_history = true;
   reserve_elapsed_time_history = true;
}


// std::string write_training_algorithm_type(void) const method

std::string LevenbergMarquardtAlgorithm::write_training_algorithm_type(void) const
{
   return("LEVENBERG_MARQUARDT_ALGORITHM");
}


// TiXmlElement* to_XML(void) const method

TiXmlElement* LevenbergMarquardtAlgorithm::to_XML(void) const
{
   std::ostringstream buffer;

   // Training algorithm

   TiXmlElement* Levenberg_Marquardt_algorithm_element = new TiXmlElement("LevenbergMarquardtAlgorithm");
   Levenberg_Marquardt_algorithm_element->SetAttribute("Version", 4); 


   // Warning parameters norm

   TiXmlElement* warning_parameters_norm_element = new TiXmlElement("WarningParametersNorm");
   Levenberg_Marquardt_algorithm_element->LinkEndChild(warning_parameters_norm_element);

   buffer.str("");
   buffer << warning_parameters_norm;

   TiXmlText* warning_parameters_norm_text = new TiXmlText(buffer.str().c_str());
   warning_parameters_norm_element->LinkEndChild(warning_parameters_norm_text);

   // Warning gradient norm 

   TiXmlElement* warning_gradient_norm_element = new TiXmlElement("WarningGradientNorm");
   Levenberg_Marquardt_algorithm_element->LinkEndChild(warning_gradient_norm_element);

   buffer.str("");
   buffer << warning_gradient_norm;

   TiXmlText* warning_gradient_norm_text = new TiXmlText(buffer.str().c_str());
   warning_gradient_norm_element->LinkEndChild(warning_gradient_norm_text);

   // Error parameters norm

   TiXmlElement* error_parameters_norm_element = new TiXmlElement("ErrorParametersNorm");
   Levenberg_Marquardt_algorithm_element->LinkEndChild(error_parameters_norm_element);

   buffer.str("");
   buffer << error_parameters_norm;

   TiXmlText* error_parameters_norm_text = new TiXmlText(buffer.str().c_str());
   error_parameters_norm_element->LinkEndChild(error_parameters_norm_text);

   // Error gradient norm 

   TiXmlElement* error_gradient_norm_element = new TiXmlElement("ErrorGradientNorm");
   Levenberg_Marquardt_algorithm_element->LinkEndChild(error_gradient_norm_element);

   buffer.str("");
   buffer << error_gradient_norm;

   TiXmlText* error_gradient_norm_text = new TiXmlText(buffer.str().c_str());
   error_gradient_norm_element->LinkEndChild(error_gradient_norm_text);

   // Minimum parameters increment norm

   TiXmlElement* minimum_parameters_increment_norm_element = new TiXmlElement("MinimumParametersIncrement");
   Levenberg_Marquardt_algorithm_element->LinkEndChild(minimum_parameters_increment_norm_element);

   buffer.str("");
   buffer << minimum_parameters_increment_norm;

   TiXmlText* minimum_parameters_increment_norm_text = new TiXmlText(buffer.str().c_str());
   minimum_parameters_increment_norm_element->LinkEndChild(minimum_parameters_increment_norm_text);

   // Minimum performance increase 

   TiXmlElement* minimum_performance_increase_element = new TiXmlElement("MinimumPerformanceIncrease");
   Levenberg_Marquardt_algorithm_element->LinkEndChild(minimum_performance_increase_element);

   buffer.str("");
   buffer << minimum_performance_increase;

   TiXmlText* minimum_performance_increase_text = new TiXmlText(buffer.str().c_str());
   minimum_performance_increase_element->LinkEndChild(minimum_performance_increase_text);

   // Performance goal 

   TiXmlElement* performance_goal_element = new TiXmlElement("PerformanceGoal");
   Levenberg_Marquardt_algorithm_element->LinkEndChild(performance_goal_element);

   buffer.str("");
   buffer << performance_goal;

   TiXmlText* performance_goal_text = new TiXmlText(buffer.str().c_str());
   performance_goal_element->LinkEndChild(performance_goal_text);

   // Gradient norm goal 

   TiXmlElement* gradient_norm_goal_element = new TiXmlElement("GradientNormGoal");
   Levenberg_Marquardt_algorithm_element->LinkEndChild(gradient_norm_goal_element);

   buffer.str("");
   buffer << gradient_norm_goal;

   TiXmlText* gradient_norm_goal_text = new TiXmlText(buffer.str().c_str());
   gradient_norm_goal_element->LinkEndChild(gradient_norm_goal_text);

   // Maximum generalization performance decreases

   TiXmlElement* maximum_generalization_evaluation_decreases_element = new TiXmlElement("MaximumGeneralizationEvaluationDecreases");
   Levenberg_Marquardt_algorithm_element->LinkEndChild(maximum_generalization_evaluation_decreases_element);

   buffer.str("");
   buffer << maximum_generalization_evaluation_decreases;

   TiXmlText* maximum_generalization_evaluation_decreases_text = new TiXmlText(buffer.str().c_str());
   maximum_generalization_evaluation_decreases_element->LinkEndChild(maximum_generalization_evaluation_decreases_text);

   // Maximum epochs number 

   TiXmlElement* maximum_epochs_number_element = new TiXmlElement("MaximumEpochsNumber");
   Levenberg_Marquardt_algorithm_element->LinkEndChild(maximum_epochs_number_element);

   buffer.str("");
   buffer << maximum_epochs_number;

   TiXmlText* maximum_epochs_number_text = new TiXmlText(buffer.str().c_str());
   maximum_epochs_number_element->LinkEndChild(maximum_epochs_number_text);

   // Maximum time 

   TiXmlElement* maximum_time_element = new TiXmlElement("MaximumTime");
   Levenberg_Marquardt_algorithm_element->LinkEndChild(maximum_time_element);

   buffer.str("");
   buffer << maximum_time;

   TiXmlText* maximum_time_text = new TiXmlText(buffer.str().c_str());
   maximum_time_element->LinkEndChild(maximum_time_text);

   // Reserve parameters history 

   TiXmlElement* reserve_parameters_history_element = new TiXmlElement("ReserveParametersHistory");
   Levenberg_Marquardt_algorithm_element->LinkEndChild(reserve_parameters_history_element);

   buffer.str("");
   buffer << reserve_parameters_history;

   TiXmlText* reserve_parameters_history_text = new TiXmlText(buffer.str().c_str());
   reserve_parameters_history_element->LinkEndChild(reserve_parameters_history_text);

   // Reserve parameters norm history 

   TiXmlElement* reserve_parameters_norm_history_element = new TiXmlElement("ReserveParametersNormHistory");
   Levenberg_Marquardt_algorithm_element->LinkEndChild(reserve_parameters_norm_history_element);

   buffer.str("");
   buffer << reserve_parameters_norm_history;

   TiXmlText* reserve_parameters_norm_history_text = new TiXmlText(buffer.str().c_str());
   reserve_parameters_norm_history_element->LinkEndChild(reserve_parameters_norm_history_text);

   // Reserve evaluation history 

   TiXmlElement* reserve_evaluation_history_element = new TiXmlElement("ReservePerformanceHistory");
   Levenberg_Marquardt_algorithm_element->LinkEndChild(reserve_evaluation_history_element);

   buffer.str("");
   buffer << reserve_evaluation_history;

   TiXmlText* reserve_evaluation_history_text = new TiXmlText(buffer.str().c_str());
   reserve_evaluation_history_element->LinkEndChild(reserve_evaluation_history_text);

   // Reserve gradient history 

   TiXmlElement* reserve_gradient_history_element = new TiXmlElement("ReserveGradientHistory");
   Levenberg_Marquardt_algorithm_element->LinkEndChild(reserve_gradient_history_element);

   buffer.str("");
   buffer << reserve_gradient_history;

   TiXmlText* reserve_gradient_history_text = new TiXmlText(buffer.str().c_str());
   reserve_gradient_history_element->LinkEndChild(reserve_gradient_history_text);

   // Reserve gradient norm history 

   TiXmlElement* reserve_gradient_norm_history_element = new TiXmlElement("ReserveGradientNormHistory");
   Levenberg_Marquardt_algorithm_element->LinkEndChild(reserve_gradient_norm_history_element);

   buffer.str("");
   buffer << reserve_gradient_norm_history;

   TiXmlText* reserve_gradient_norm_history_text = new TiXmlText(buffer.str().c_str());
   reserve_gradient_norm_history_element->LinkEndChild(reserve_gradient_norm_history_text);

   // Reserve inverse Hessian history 

   TiXmlElement* reserve_inverse_Hessian_history_element = new TiXmlElement("ReserveInverseHessianHistory");
   Levenberg_Marquardt_algorithm_element->LinkEndChild(reserve_inverse_Hessian_history_element);

   buffer.str("");
   buffer << reserve_inverse_Hessian_history;

   TiXmlText* reserve_inverse_Hessian_history_text = new TiXmlText(buffer.str().c_str());
   reserve_inverse_Hessian_history_element->LinkEndChild(reserve_inverse_Hessian_history_text);

   // Reserve elapsed time history 

   TiXmlElement* reserve_elapsed_time_history_element = new TiXmlElement("ReserveElapsedTimeHistory");
   Levenberg_Marquardt_algorithm_element->LinkEndChild(reserve_elapsed_time_history_element);

   buffer.str("");
   buffer << reserve_elapsed_time_history;

   TiXmlText* reserve_elapsed_time_history_text = new TiXmlText(buffer.str().c_str());
   reserve_elapsed_time_history_element->LinkEndChild(reserve_elapsed_time_history_text);

   // Reserve generalization evaluation history 

   TiXmlElement* reserve_generalization_evaluation_history_element = new TiXmlElement("ReserveGeneralizationPerformanceHistory");
   Levenberg_Marquardt_algorithm_element->LinkEndChild(reserve_generalization_evaluation_history_element);

   buffer.str("");
   buffer << reserve_generalization_evaluation_history;

   TiXmlText* reserve_generalization_evaluation_history_text = new TiXmlText(buffer.str().c_str());
   reserve_generalization_evaluation_history_element->LinkEndChild(reserve_generalization_evaluation_history_text);

   // Display period

   TiXmlElement* display_period_element = new TiXmlElement("DisplayPeriod");
   Levenberg_Marquardt_algorithm_element->LinkEndChild(display_period_element);

   buffer.str("");
   buffer << display_period;

   TiXmlText* display_period_text = new TiXmlText(buffer.str().c_str());
   display_period_element->LinkEndChild(display_period_text);

   // Display

   TiXmlElement* display_element = new TiXmlElement("Display");
   Levenberg_Marquardt_algorithm_element->LinkEndChild(display_element);

   buffer.str("");
   buffer << display;

   TiXmlText* display_text = new TiXmlText(buffer.str().c_str());
   display_element->LinkEndChild(display_text);

   return(Levenberg_Marquardt_algorithm_element);
}


// void load(TiXmlElement*) method

/// This method loads a Levenberg-Marquardt method object from a XML element.
/// Please mind about the format, wich is specified in the OpenNN manual. 

void LevenbergMarquardtAlgorithm::from_XML(TiXmlElement* Levenberg_Marquardt_algorithm_element)
{
   // Warning parameters norm

   TiXmlElement* warning_parameters_norm_element = Levenberg_Marquardt_algorithm_element->FirstChildElement("WarningParametersNorm");

   if(warning_parameters_norm_element)
   {
      double new_warning_parameters_norm = atof(warning_parameters_norm_element->GetText()); 

      try
      {
         set_warning_parameters_norm(new_warning_parameters_norm);
      }
      catch(std::exception& e)
      {
         std::cout << e.what() << std::endl;		 
      }
   }

   // Warning gradient norm 

   TiXmlElement* warning_gradient_norm_element = Levenberg_Marquardt_algorithm_element->FirstChildElement("WarningGradientNorm");

   if(warning_gradient_norm_element)
   {
      double new_warning_gradient_norm = atof(warning_gradient_norm_element->GetText()); 

      try
      {
         set_warning_gradient_norm(new_warning_gradient_norm);
      }
      catch(std::exception& e)
      {
         std::cout << e.what() << std::endl;		 
      }
   }

   // Error parameters norm

   TiXmlElement* error_parameters_norm_element = Levenberg_Marquardt_algorithm_element->FirstChildElement("ErrorParametersNorm");

   if(error_parameters_norm_element)
   {
      double new_error_parameters_norm = atof(error_parameters_norm_element->GetText()); 

      try
      {
          set_error_parameters_norm(new_error_parameters_norm);
      }
      catch(std::exception& e)
      {
         std::cout << e.what() << std::endl;		 
      }
   }

   // Error gradient norm 

   TiXmlElement* error_gradient_norm_element = Levenberg_Marquardt_algorithm_element->FirstChildElement("ErrorGradientNorm");

   if(error_gradient_norm_element)
   {
      double new_error_gradient_norm = atof(error_gradient_norm_element->GetText()); 

      try
      {
         set_error_gradient_norm(new_error_gradient_norm);
      }
      catch(std::exception& e)
      {
         std::cout << e.what() << std::endl;		 
      }
   }

   // Minimum parameters increment norm

   TiXmlElement* minimum_parameters_increment_norm_element = Levenberg_Marquardt_algorithm_element->FirstChildElement("MinimumParametersIncrementNorm");

   if(minimum_parameters_increment_norm_element)
   {
      double new_minimum_parameters_increment_norm = atof(minimum_parameters_increment_norm_element->GetText()); 

      try
      {
         set_minimum_parameters_increment_norm(new_minimum_parameters_increment_norm);
      }
      catch(std::exception& e)
      {
         std::cout << e.what() << std::endl;		 
      }
   }

   // Minimum performance increase 

   TiXmlElement* minimum_performance_increase_element = Levenberg_Marquardt_algorithm_element->FirstChildElement("MinimumPerformanceIncrease");

   if(minimum_performance_increase_element)
   {
      double new_minimum_performance_increase = atof(minimum_performance_increase_element->GetText()); 

      try
      {
         set_minimum_performance_increase(new_minimum_performance_increase);
      }
      catch(std::exception& e)
      {
         std::cout << e.what() << std::endl;		 
      }
   }

   // Performance goal 

   TiXmlElement* performance_goal_element = Levenberg_Marquardt_algorithm_element->FirstChildElement("PerformanceGoal");

   if(performance_goal_element)
   {
      double new_performance_goal = atof(performance_goal_element->GetText()); 

      try
      {
         set_performance_goal(new_performance_goal);
      }
      catch(std::exception& e)
      {
         std::cout << e.what() << std::endl;		 
      }
   }

   // Gradient norm goal 

   TiXmlElement* gradient_norm_goal_element = Levenberg_Marquardt_algorithm_element->FirstChildElement("GradientNormGoal");

   if(gradient_norm_goal_element)
   {
      double new_gradient_norm_goal = atof(gradient_norm_goal_element->GetText()); 

      try
      {
         set_gradient_norm_goal(new_gradient_norm_goal);
      }
      catch(std::exception& e)
      {
         std::cout << e.what() << std::endl;		 
      }
   }

   // Maximum generalization performance decreases

   TiXmlElement* maximum_generalization_evaluation_decreases_element = Levenberg_Marquardt_algorithm_element->FirstChildElement("MaximumGeneralizationEvaluationDecreases");

   if(maximum_generalization_evaluation_decreases_element)
   {
      unsigned int new_maximum_generalization_evaluation_decreases = atoi(maximum_generalization_evaluation_decreases_element->GetText()); 

      try
      {
         set_maximum_generalization_evaluation_decreases(new_maximum_generalization_evaluation_decreases);
      }
      catch(std::exception& e)
      {
         std::cout << e.what() << std::endl;		 
      }
   }

   // Maximum epochs number 

   TiXmlElement* maximum_epochs_number_element = Levenberg_Marquardt_algorithm_element->FirstChildElement("MaximumEpochsNumber");

   if(maximum_epochs_number_element)
   {
       unsigned int new_maximum_epochs_number = atoi(maximum_epochs_number_element->GetText()); 

      try
      {
         set_maximum_epochs_number(new_maximum_epochs_number);
      }
      catch(std::exception& e)
      {
         std::cout << e.what() << std::endl;		 
      }
   }

   // Maximum time 

   TiXmlElement* maximum_time_element = Levenberg_Marquardt_algorithm_element->FirstChildElement("MaximumTime");

   if(maximum_time_element)
   {
      double new_maximum_time = atof(maximum_time_element->GetText()); 

      try
      {
         set_maximum_time(new_maximum_time);
      }
      catch(std::exception& e)
      {
         std::cout << e.what() << std::endl;		 
      }
   }

   // Reserve parameters history 

   TiXmlElement* reserve_parameters_history_element = Levenberg_Marquardt_algorithm_element->FirstChildElement("ReserveParametersHistory");

   if(reserve_parameters_history_element)
   {
      std::string new_reserve_parameters_history = reserve_parameters_history_element->GetText(); 

      try
      {
         set_reserve_parameters_history(new_reserve_parameters_history != "0");
      }
      catch(std::exception& e)
      {
         std::cout << e.what() << std::endl;		 
      }
   }

   // Reserve parameters norm history 

   TiXmlElement* reserve_parameters_norm_history_element = Levenberg_Marquardt_algorithm_element->FirstChildElement("ReserveParametersNormHistory");

   if(reserve_parameters_norm_history_element)
   {
      std::string new_reserve_parameters_norm_history = reserve_parameters_norm_history_element->GetText(); 

      try
      {
         set_reserve_parameters_norm_history(new_reserve_parameters_norm_history != "0");
      }
      catch(std::exception& e)
      {
         std::cout << e.what() << std::endl;		 
      }
   }

   // Reserve evaluation history 

   TiXmlElement* reserve_evaluation_history_element = Levenberg_Marquardt_algorithm_element->FirstChildElement("ReservePerformanceHistory");

   if(reserve_evaluation_history_element)
   {
      std::string new_reserve_evaluation_history = reserve_evaluation_history_element->GetText(); 

      try
      {
         set_reserve_evaluation_history(new_reserve_evaluation_history != "0");
      }
      catch(std::exception& e)
      {
         std::cout << e.what() << std::endl;		 
      }
   }

   // Reserve gradient history 

   TiXmlElement* reserve_gradient_history_element = Levenberg_Marquardt_algorithm_element->FirstChildElement("ReserveGradientHistory");

   if(reserve_gradient_history_element)
   {
      std::string new_reserve_gradient_history = reserve_gradient_history_element->GetText(); 

      try
      {
         set_reserve_gradient_history(new_reserve_gradient_history != "0");
      }
      catch(std::exception& e)
      {
         std::cout << e.what() << std::endl;		 
      }
   }

   // Reserve gradient norm history 

   TiXmlElement* reserve_gradient_norm_history_element = Levenberg_Marquardt_algorithm_element->FirstChildElement("ReserveGradientNormHistory");

   if(reserve_gradient_norm_history_element)
   {
      std::string new_reserve_gradient_norm_history = reserve_gradient_norm_history_element->GetText(); 

      try
      {
         set_reserve_gradient_norm_history(new_reserve_gradient_norm_history != "0");
      }
      catch(std::exception& e)
      {
         std::cout << e.what() << std::endl;		 
      }
   }

   // Reserve elapsed time history 

   TiXmlElement* reserve_elapsed_time_history_element = Levenberg_Marquardt_algorithm_element->FirstChildElement("ReserveElapsedTimeHistory");

   if(reserve_elapsed_time_history_element)
   {
      std::string new_reserve_elapsed_time_history = reserve_elapsed_time_history_element->GetText(); 

      try
      {
         set_reserve_elapsed_time_history(new_reserve_elapsed_time_history != "0");
      }
      catch(std::exception& e)
      {
         std::cout << e.what() << std::endl;		 
      }
   }

   // Reserve generalization evaluation history 

   TiXmlElement* reserve_generalization_evaluation_history_element = Levenberg_Marquardt_algorithm_element->FirstChildElement("ReserveGeneralizationPerformanceHistory");

   if(reserve_generalization_evaluation_history_element)
   {
      std::string new_reserve_generalization_evaluation_history = reserve_generalization_evaluation_history_element->GetText(); 

      try
      {
         set_reserve_generalization_evaluation_history(new_reserve_generalization_evaluation_history != "0");
      }
      catch(std::exception& e)
      {
         std::cout << e.what() << std::endl;		 
      }
   }

   // Display period

   TiXmlElement* display_period_element = Levenberg_Marquardt_algorithm_element->FirstChildElement("DisplayPeriod");

   if(display_period_element)
   {
      unsigned int new_display_period = atoi(display_period_element->GetText()); 

      try
      {
         set_display_period(new_display_period);
      }
      catch(std::exception& e)
      {
         std::cout << e.what() << std::endl;		 
      }
   }

   // Display

   TiXmlElement* display_element = Levenberg_Marquardt_algorithm_element->FirstChildElement("Display");

   if(display_element)
   {
      std::string new_display = display_element->GetText(); 

      try
      {
         set_display(new_display != "0");
      }
      catch(std::exception& e)
      {
         std::cout << e.what() << std::endl;		 
      }
   }
}

}


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
