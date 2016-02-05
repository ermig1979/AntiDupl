/****************************************************************************************************************/
/*                                                                                                              */
/*   OpenNN: Open Neural Networks Library                                                                       */
/*   www.opennn.cimne.com                                                                                       */
/*                                                                                                              */ 
/*   Q U A S I - N E W T O N   M E T H O D    C L A S S                                                         */
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
#include <cmath>
#include <ctime>

// OpenNN includes

#include "quasi_newton_method.h"

namespace OpenNN
{

// DEFAULT CONSTRUCTOR

/// Default constructor. 
/// It creates a quasi-Newton method training algorithm not associated to any performance functional. 
/// It also initializes the class members to their default values.

QuasiNewtonMethod::QuasiNewtonMethod(void) 
 : TrainingAlgorithm()
{
   set_default();
}


// PERFORMANCE FUNCTIONAL CONSTRUCTOR

/// Performance functional constructor. 
/// It creates a quasi-Newton method training algorithm associated to a performance functional. 
/// It also initializes the class members to their default values.
/// @param new_performance_functional_pointer Pointer to a performance functional object.

QuasiNewtonMethod::QuasiNewtonMethod(PerformanceFunctional* new_performance_functional_pointer)
: TrainingAlgorithm(new_performance_functional_pointer)
{
   training_rate_algorithm.set_performance_functional_pointer(new_performance_functional_pointer);

   set_default();
}


// XML CONSTRUCTOR

/// XML constructor. 
/// It creates a quasi-Newton method training algorithm not associated to any performance functional. 
/// It also initializes the class members to their default values.

QuasiNewtonMethod::QuasiNewtonMethod(TiXmlElement* quasi_Newton_method_element)
 : TrainingAlgorithm(quasi_Newton_method_element)
{
   set_default();
}


// DESTRUCTOR

/// Destructor.
/// It does not delete any object. 

QuasiNewtonMethod::~QuasiNewtonMethod(void)
{
}


// METHODS



// const TrainingRateAlgorithm& get_training_rate_algorithm(void) const method

/// This method returns a constant reference to the training rate algorithm object inside the quasi-Newton method object. 

const TrainingRateAlgorithm& QuasiNewtonMethod::get_training_rate_algorithm(void) const
{
   return(training_rate_algorithm);
}


// TrainingRateAlgorithm* get_training_rate_algorithm_pointer(void) method

/// This method returns a pointer to the training rate algorithm object inside the quasi-Newton method object. 

TrainingRateAlgorithm* QuasiNewtonMethod::get_training_rate_algorithm_pointer(void)
{
   return(&training_rate_algorithm);
}


// const InverseHessianApproximationMethod& get_inverse_Hessian_approximation_method(void) const method

/// This method returns the method for approximating the inverse Hessian matrix to be used when training. 

const QuasiNewtonMethod::InverseHessianApproximationMethod& QuasiNewtonMethod::get_inverse_Hessian_approximation_method(void) const
{
   return(inverse_Hessian_approximation_method);
}


// std::string write_inverse_Hessian_approximation_method(void) const method

/// This method returns the name of the method for the approximation of the inverse Hessian. 

std::string QuasiNewtonMethod::write_inverse_Hessian_approximation_method(void) const
{
   switch(inverse_Hessian_approximation_method)
   {
      case DFP:
      {
         return("DFP");
	  }
      break;

      case BFGS:
      {
         return("BFGS");
	  }
      break;

	  default:
      {
         std::ostringstream buffer;

         buffer << "OpenNN Exception: QuasiNewtonMethod class.\n"
                << "std::string write_inverse_Hessian_approximation_method(void) const method.\n"
                << "Unknown inverse Hessian approximation method.\n";
 
         throw std::logic_error(buffer.str().c_str());	     
	  }
      break;
   }
}


// const double& get_warning_parameters_norm(void) const method

/// This method returns the minimum value for the norm of the parameters vector at wich a warning message is written to the screen. 

const double& QuasiNewtonMethod::get_warning_parameters_norm(void) const
{
   return(warning_parameters_norm);       
}


// const double& get_warning_gradient_norm(void) const method

/// This method returns the minimum value for the norm of the gradient vector at wich a warning message is written to the screen. 

const double& QuasiNewtonMethod::get_warning_gradient_norm(void) const
{
   return(warning_gradient_norm);       
}


// const double& get_warning_training_rate(void) const method

/// This method returns the training rate value at wich a warning message is written to the screen during line minimization.

const double& QuasiNewtonMethod::get_warning_training_rate(void) const
{
   return(warning_training_rate);
}


// const double& get_error_parameters_norm(void) const method

/// This method returns the value for the norm of the parameters vector at wich an error message is written to the screen and the program exits. 

const double& QuasiNewtonMethod::get_error_parameters_norm(void) const
{
   return(error_parameters_norm);
}


// const double& get_error_gradient_norm(void) const method

/// This method returns the value for the norm of the gradient vector at wich an error message is written
/// to the screen and the program exits. 

const double& QuasiNewtonMethod::get_error_gradient_norm(void) const
{
   return(error_gradient_norm);
}


// const double& get_error_training_rate(void) const method

/// This method returns the training rate value at wich the line minimization algorithm is assumed to fail when 
/// bracketing a minimum.

const double& QuasiNewtonMethod::get_error_training_rate(void) const
{
   return(error_training_rate);
}


// const double& get_minimum_parameters_increment_norm(void) const method

/// This method returns the minimum norm of the parameter increment vector used as a stopping criteria when training. 

const double& QuasiNewtonMethod::get_minimum_parameters_increment_norm(void) const
{
   return(minimum_parameters_increment_norm);
}


// const double& get_minimum_performance_increase(void) const method

/// This method returns the minimum performance improvement during training.  

const double& QuasiNewtonMethod::get_minimum_performance_increase(void) const
{
   return(minimum_performance_increase);
}


// const double& get_performance_goal(void) const method

/// This method returns the goal value for the performance. 
/// This is used as a stopping criterium when training a multilayer perceptron

const double& QuasiNewtonMethod::get_performance_goal(void) const
{
   return(performance_goal);
}


// const double& get_gradient_norm_goal(void) const method

/// This method returns the goal value for the norm of the objective function gradient.
/// This is used as a stopping criterium when training a multilayer perceptron

const double& QuasiNewtonMethod::get_gradient_norm_goal(void) const
{
   return(gradient_norm_goal);
}


// const unsigned int& get_maximum_generalization_evaluation_decreases(void) const method

/// This method returns the maximum number of generalization failures during the training process. 

const unsigned int& QuasiNewtonMethod::get_maximum_generalization_evaluation_decreases(void) const
{
   return(maximum_generalization_evaluation_decreases);
}


// const unsigned int& get_maximum_epochs_number(void) const method

/// This method returns the maximum number of epochs for training.

const unsigned int& QuasiNewtonMethod::get_maximum_epochs_number(void) const
{
   return(maximum_epochs_number);
}


// const double& get_maximum_time(void) const method

/// This method returns the maximum training time.  

const double& QuasiNewtonMethod::get_maximum_time(void) const
{
   return(maximum_time);
}



// const bool& get_reserve_parameters_history(void) const method

/// This method returns true if the parameters history matrix is to be reserved, and false otherwise.

const bool& QuasiNewtonMethod::get_reserve_parameters_history(void) const
{
   return(reserve_parameters_history);     
}


// const bool& get_reserve_parameters_norm_history(void) const method 

/// This method returns true if the parameters norm history vector is to be reserved, and false otherwise.

const bool& QuasiNewtonMethod::get_reserve_parameters_norm_history(void) const
{
   return(reserve_parameters_norm_history);     
}


// const bool& get_reserve_evaluation_history(void) const method

/// This method returns true if the evaluation history vector is to be reserved, and false otherwise.

const bool& QuasiNewtonMethod::get_reserve_evaluation_history(void) const
{
   return(reserve_evaluation_history);     
}


// const bool& get_reserve_gradient_history(void) const method

/// This method returns true if the gradient history vector of vectors is to be reserved, and false otherwise.

const bool& QuasiNewtonMethod::get_reserve_gradient_history(void) const
{
   return(reserve_gradient_history);     
}


// const bool& get_reserve_gradient_norm_history(void) const method

/// This method returns true if the gradient norm history vector is to be reserved, and false otherwise.

const bool& QuasiNewtonMethod::get_reserve_gradient_norm_history(void) const
{
   return(reserve_gradient_norm_history);     
}



// const bool& get_reserve_training_direction_history(void) const method

/// This method returns true if the training direction history matrix is to be reserved, and false otherwise.

const bool& QuasiNewtonMethod::get_reserve_training_direction_history(void) const
{
   return(reserve_training_direction_history);     
}


// const bool& get_reserve_training_rate_history(void) const method

/// This method returns true if the training rate history vector is to be reserved, and false otherwise.

const bool& QuasiNewtonMethod::get_reserve_training_rate_history(void) const
{
   return(reserve_training_rate_history);     
}


// const bool& get_reserve_elapsed_time_history(void) const method

/// This method returns true if the elapsed time history vector is to be reserved, and false otherwise.

const bool& QuasiNewtonMethod::get_reserve_elapsed_time_history(void) const
{
   return(reserve_elapsed_time_history);     
}


// const bool& get_reserve_inverse_Hessian_history(void) const method

/// This method returns true if the inverse Hessian history is to be reserved, and false otherwise.

const bool& QuasiNewtonMethod::get_reserve_inverse_Hessian_history(void) const
{
   return(reserve_inverse_Hessian_history);     
}


// const bool& get_reserve_generalization_evaluation_history(void) const method

/// This method returns true if the Generalization evaluation history vector is to be reserved, and false otherwise.

const bool& QuasiNewtonMethod::get_reserve_generalization_evaluation_history(void) const
{
   return(reserve_generalization_evaluation_history);
}


// const unsigned int& get_display_period(void) const method

/// This method returns the number of epochs between the training showing progress. 

const unsigned int& QuasiNewtonMethod::get_display_period(void) const
{
   return(display_period);
}


// void set_inverse_Hessian_approximation_method(const InverseHessianApproximationMethod&) method

/// This method sets a new inverse Hessian approximatation method value. 
/// @param new_inverse_Hessian_approximation_method Inverse Hessian approximation method value.

void QuasiNewtonMethod::set_inverse_Hessian_approximation_method(const QuasiNewtonMethod::InverseHessianApproximationMethod& 
new_inverse_Hessian_approximation_method)
{
   inverse_Hessian_approximation_method = new_inverse_Hessian_approximation_method;
}


// void set_inverse_Hessian_approximation_method(const std::string&) method

/// This method sets a new method for approximating the inverse of the Hessian matrix from a string containing the name. 
/// Possible values are:
/// <ul>
/// <li> "DFP"
/// <li> "BFGS"
/// </ul>
/// @param new_inverse_Hessian_approximation_method_name Name of inverse Hessian approximation method.  

void QuasiNewtonMethod::set_inverse_Hessian_approximation_method(const std::string& new_inverse_Hessian_approximation_method_name)
{
   if(new_inverse_Hessian_approximation_method_name == "DFP")
   {
      inverse_Hessian_approximation_method = DFP;
   }
   else if(new_inverse_Hessian_approximation_method_name == "BFGS")
   {
      inverse_Hessian_approximation_method = BFGS;
   }
   else
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: QuasiNewtonMethod class.\n"
             << "void set_inverse_Hessian_approximation_method(const std::string&) method.\n"
			 << "Unknown inverse Hessian approximation method: " << new_inverse_Hessian_approximation_method_name << ".\n";
   
      throw std::logic_error(buffer.str().c_str());	  
   }
}


// void set_reserve_all_training_history(bool) method

/// This method makes the training history of all variables to reseved or not in memory.
/// @param new_reserve_all_training_history True if the training history of all variables is to be reserved, 
/// false otherwise.

void QuasiNewtonMethod::set_reserve_all_training_history(const bool& new_reserve_all_training_history)
{
   reserve_elapsed_time_history = new_reserve_all_training_history;
   reserve_parameters_history = new_reserve_all_training_history;
   reserve_parameters_norm_history = new_reserve_all_training_history;
   reserve_evaluation_history = new_reserve_all_training_history;
   reserve_gradient_history = new_reserve_all_training_history;
   reserve_gradient_norm_history = new_reserve_all_training_history;
   reserve_training_direction_history = new_reserve_all_training_history;
   reserve_training_rate_history = new_reserve_all_training_history;
}


// void set_default(void) method

void QuasiNewtonMethod::set_default(void)
{
   inverse_Hessian_approximation_method = BFGS;

   training_rate_algorithm.set_default();

   // TRAINING PARAMETERS

   warning_parameters_norm = 1.0e6;
   warning_gradient_norm = 1.0e6;   
   warning_training_rate = 1.0e6;

   error_parameters_norm = 1.0e9;
   error_gradient_norm = 1.0e9;
   error_training_rate = 1.0e9;

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
   reserve_inverse_Hessian_history = false;

   reserve_training_direction_history = false;
   reserve_training_rate_history = false;
   reserve_elapsed_time_history = false;

   // UTILITIES

   display = true;
   display_period = 100;
}


// void set_warning_parameters_norm(const double&) method

/// This method sets a new value for the parameters vector norm at which a warning message is written to the 
/// screen. 
/// @param new_warning_parameters_norm Warning norm of parameters vector value. 

void QuasiNewtonMethod::set_warning_parameters_norm(const double& new_warning_parameters_norm)
{
   // Control sentence (if debug)

   #ifdef _DEBUG 

   if(new_warning_parameters_norm < 0.0)
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: QuasiNewtonMethod class.\n"
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

void QuasiNewtonMethod::set_warning_gradient_norm(const double& new_warning_gradient_norm)
{
   // Control sentence (if debug)

   #ifdef _DEBUG 

   if(new_warning_gradient_norm < 0.0)
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: QuasiNewtonMethod class.\n"
             << "void set_warning_gradient_norm(const double&) method.\n"
             << "Warning gradient norm must be equal or greater than 0.\n";

      throw std::logic_error(buffer.str().c_str());	  
   }

   #endif

   // Set warning gradient norm

   warning_gradient_norm = new_warning_gradient_norm;     
}


// void set_warning_training_rate(const double&) method

/// This method sets a new training rate value at wich a warning message is written to the screen during line 
/// minimization.
/// @param new_warning_training_rate Warning training rate value.

void QuasiNewtonMethod::set_warning_training_rate(const double& new_warning_training_rate)
{
   // Control sentence (if debug)

   #ifdef _DEBUG 

   if(new_warning_training_rate < 0.0)
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: QuasiNewtonMethod class.\n" 
             << "void set_warning_training_rate(const double&) method.\n"
             << "Warning training rate must be equal or greater than 0.\n";

      throw std::logic_error(buffer.str().c_str());	  
   }

   #endif

   warning_training_rate = new_warning_training_rate;
}


// void set_error_parameters_norm(const double&) method

/// This method sets a new value for the parameters vector norm at which an error message is written to the 
/// screen and the program exits. 
/// @param new_error_parameters_norm Error norm of parameters vector value. 

void QuasiNewtonMethod::set_error_parameters_norm(const double& new_error_parameters_norm)
{
   // Control sentence (if debug)

   #ifdef _DEBUG 

   if(new_error_parameters_norm < 0.0)
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: QuasiNewtonMethod class.\n"
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

void QuasiNewtonMethod::set_error_gradient_norm(const double& new_error_gradient_norm)
{
   // Control sentence (if debug)

   #ifdef _DEBUG 

   if(new_error_gradient_norm < 0.0)
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: QuasiNewtonMethod class.\n"
             << "void set_error_gradient_norm(const double&) method.\n"
             << "Error gradient norm must be equal or greater than 0.\n";

      throw std::logic_error(buffer.str().c_str());	  
   }

   #endif

   // Set error gradient norm

   error_gradient_norm = new_error_gradient_norm;
}


// void set_error_training_rate(const double&) method

/// This method sets a new training rate value at wich a the line minimization algorithm is assumed to fail when 
/// bracketing a minimum.
/// @param new_error_training_rate Error training rate value.

void QuasiNewtonMethod::set_error_training_rate(const double& new_error_training_rate)
{
   // Control sentence (if debug)

   #ifdef _DEBUG 

   if(new_error_training_rate < 0.0)
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: QuasiNewtonMethod class.\n"
             << "void set_error_training_rate(const double&) method.\n"
             << "Error training rate must be equal or greater than 0.\n";

      throw std::logic_error(buffer.str().c_str());	  
   }

   #endif

   // Set error training rate

   error_training_rate = new_error_training_rate;
}


// void set_minimum_parameters_increment_norm(const double&) method

/// This method sets a new value for the minimum parameters increment norm stopping criterium. 
/// @param new_minimum_parameters_increment_norm Value of norm of parameters increment norm used to stop training. 

void QuasiNewtonMethod::set_minimum_parameters_increment_norm(const double& new_minimum_parameters_increment_norm)
{
   // Control sentence (if debug)

   #ifdef _DEBUG 

   if(new_minimum_parameters_increment_norm < 0.0)
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: QuasiNewtonMethod class.\n"
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

void QuasiNewtonMethod::set_minimum_performance_increase(const double& new_minimum_performance_increase)
{
   // Control sentence (if debug)

   #ifdef _DEBUG 

   if(new_minimum_performance_increase < 0.0)
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: QuasiNewtonMethod class.\n"
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

void QuasiNewtonMethod::set_performance_goal(const double& new_performance_goal)
{
   performance_goal = new_performance_goal;
}


// void set_gradient_norm_goal(const double&) method

/// This method sets a new the goal value for the norm of the objective function gradient. 
/// This is used as a stopping criterium when training a multilayer perceptron
/// @param new_gradient_norm_goal Goal value for the norm of the objective function gradient.

void QuasiNewtonMethod::set_gradient_norm_goal(const double& new_gradient_norm_goal)
{
   // Control sentence (if debug)

   #ifdef _DEBUG 

   if(new_gradient_norm_goal < 0.0)
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: QuasiNewtonMethod class.\n"
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

void QuasiNewtonMethod::set_maximum_generalization_evaluation_decreases(const unsigned int& new_maximum_generalization_evaluation_decreases)
{
   // Control sentence (if debug)

   #ifdef _DEBUG 

   if(new_maximum_generalization_evaluation_decreases < 0)
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: QuasiNewtonMethod class.\n"
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

void QuasiNewtonMethod::set_maximum_epochs_number(const unsigned int& new_maximum_epochs_number)
{
   // Control sentence (if debug)

   #ifdef _DEBUG 

   if(new_maximum_epochs_number < 0)
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: QuasiNewtonMethod class.\n"
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

void QuasiNewtonMethod::set_maximum_time(const double& new_maximum_time)
{
   // Control sentence (if debug)

   #ifdef _DEBUG 

   if(new_maximum_time < 0.0)
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: QuasiNewtonMethod class.\n"
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

void QuasiNewtonMethod::set_reserve_parameters_history(const bool& new_reserve_parameters_history)
{
   reserve_parameters_history = new_reserve_parameters_history;     
}


// void set_reserve_parameters_norm_history(bool) method

/// This method makes the parameters norm history vector to be reseved or not in memory.
/// @param new_reserve_parameters_norm_history True if the parameters norm history vector is to be reserved, false otherwise.

void QuasiNewtonMethod::set_reserve_parameters_norm_history(const bool& new_reserve_parameters_norm_history)
{
   reserve_parameters_norm_history = new_reserve_parameters_norm_history;     
}


// void set_reserve_evaluation_history(bool) method

/// This method makes the evaluation history vector to be reseved or not in memory.
/// @param new_reserve_evaluation_history True if the evaluation history vector is to be reserved, false otherwise.

void QuasiNewtonMethod::set_reserve_evaluation_history(const bool& new_reserve_evaluation_history)
{
   reserve_evaluation_history = new_reserve_evaluation_history;     
}


// void set_reserve_gradient_history(bool) method

/// This method makes the gradient history vector of vectors to be reseved or not in memory.
/// @param new_reserve_gradient_history True if the gradient history matrix is to be reserved, false otherwise.

void QuasiNewtonMethod::set_reserve_gradient_history(const bool& new_reserve_gradient_history)
{
   reserve_gradient_history = new_reserve_gradient_history;    
}


// void set_reserve_gradient_norm_history(bool) method

/// This method makes the gradient norm history vector to be reseved or not in memory.
/// @param new_reserve_gradient_norm_history True if the gradient norm history matrix is to be reserved, false 
/// otherwise.

void QuasiNewtonMethod::set_reserve_gradient_norm_history(const bool& new_reserve_gradient_norm_history)
{
   reserve_gradient_norm_history = new_reserve_gradient_norm_history;     
}


// void set_reserve_inverse_Hessian_history(bool) method

/// This method sets the history of the inverse of the Hessian matrix to be reserved or not in memory.
/// This is a vector of matrices. 
/// @param new_reserve_inverse_Hessian_history True if the inverse Hessian history is to be reserved, false otherwise. 

void QuasiNewtonMethod::set_reserve_inverse_Hessian_history(const bool& new_reserve_inverse_Hessian_history)
{
   reserve_inverse_Hessian_history = new_reserve_inverse_Hessian_history;
}


// void set_reserve_training_direction_history(bool) method

/// This method makes the training direction history vector of vectors to be reseved or not in memory.
/// @param new_reserve_training_direction_history True if the training direction history matrix is to be reserved, 
/// false otherwise.

void QuasiNewtonMethod::set_reserve_training_direction_history(const bool& new_reserve_training_direction_history)
{
   reserve_training_direction_history = new_reserve_training_direction_history;          
}


// void set_reserve_training_rate_history(bool) method

/// This method makes the training rate history vector to be reseved or not in memory.
/// @param new_reserve_training_rate_history True if the training rate history vector is to be reserved, false 
/// otherwise.

void QuasiNewtonMethod::set_reserve_training_rate_history(const bool& new_reserve_training_rate_history)
{
   reserve_training_rate_history = new_reserve_training_rate_history;          
}


// void set_reserve_elapsed_time_history(bool) method

/// This method makes the elapsed time over the epochs to be reseved or not in memory. This is a vector.
/// @param new_reserve_elapsed_time_history True if the elapsed time history vector is to be reserved, false 
/// otherwise.

void QuasiNewtonMethod::set_reserve_elapsed_time_history(const bool& new_reserve_elapsed_time_history)
{
   reserve_elapsed_time_history = new_reserve_elapsed_time_history;     
}


// void set_reserve_generalization_evaluation_history(bool) method

/// This method makes the Generalization evaluation history to be reserved or not in memory. 
/// This is a vector. 
/// @param new_reserve_generalization_evaluation_history True if the Generalization evaluation history is to be reserved, false otherwise. 

void QuasiNewtonMethod::set_reserve_generalization_evaluation_history(const bool& new_reserve_generalization_evaluation_history)  
{
   reserve_generalization_evaluation_history = new_reserve_generalization_evaluation_history;
}


// void set_display_period(unsigned int) method

/// This method sets a new number of epochs between the training showing progress. 
/// @param new_display_period
/// Number of epochs between the training showing progress. 

void QuasiNewtonMethod::set_display_period(const unsigned int& new_display_period)
{
   // Control sentence (if debug)

   #ifdef _DEBUG 
     
   if(new_display_period <= 0)
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: QuasiNewtonMethod class.\n"
             << "void set_display_period(const double&) method.\n"
             << "First training rate must be greater than 0.\n";

      throw std::logic_error(buffer.str().c_str());	  
   }

   #endif

   display_period = new_display_period;
}


// Vector<double> calculate_inverse_Hessian_approximation(
// const Vector<double>&, const Vector<double>&, 
// const Vector<double>&, const Vector<double>&, 
// const Matrix<double>&) method

/// This method calculates an approximation of the inverse Hessian, accoring to the method used. 
/// @param old_parameters Another point of the objective function.
/// @param parameters Current point of the objective function
/// @param old_gradient Gradient at the other point. 
/// @param gradient Gradient at the current point. 
/// @param old_inverse_Hessian Inverse Hessian at the other point of the objective function.

Matrix<double> QuasiNewtonMethod::calculate_inverse_Hessian_approximation(
const Vector<double>& old_parameters, const Vector<double>& parameters, 
const Vector<double>& old_gradient, const Vector<double>& gradient,
const Matrix<double>& old_inverse_Hessian) const
{
   switch(inverse_Hessian_approximation_method)
   {
      case DFP:
      {
         return(calculate_DFP_inverse_Hessian(old_parameters, parameters, old_gradient, gradient, old_inverse_Hessian));
      }
      break;

      case BFGS:
      {
         return(calculate_BFGS_inverse_Hessian(old_parameters, parameters, old_gradient, gradient, old_inverse_Hessian));
      }
      break;

	  default:
	  {
         std::ostringstream buffer;

         buffer << "OpenNN Exception: QuasiNewtonMethod class.\n"
                << "Vector<double> calculate_inverse_Hessian_approximation(const Vector<double>&, const Vector<double>&, const Vector<double>&, const Vector<double>&, const Matrix<double>&) method.\n"
                << "Unknown inverse Hessian approximation method.\n";

         throw std::logic_error(buffer.str().c_str());	     
	  }
	  break;
   }
}


// Vector<double> calculate_training_direction(const Vector<double>&, const Matrix<double>&) const method

/// This method returns the quasi-Newton method training direction, which has been previously normalized.
/// @param gradient Gradient vector. 
/// @param inverse_Hessian_approximation Inverse Hessian approximation matrix. 

Vector<double> QuasiNewtonMethod::calculate_training_direction(const Vector<double>& gradient, const Matrix<double>& inverse_Hessian_approximation) const
{
   Vector<double> training_direction = inverse_Hessian_approximation.dot(gradient)*(-1.0);
 
   double training_direction_norm = training_direction.calculate_norm();

   return(training_direction/training_direction_norm);
}


// Vector<double> calculate_gradient_descent_training_direction(const Vector<double>&) const method

/// This method returns the gradient descent training direction, which is the negative of the normalized gradient. 
/// @param gradient Gradient vector.

Vector<double> QuasiNewtonMethod::calculate_gradient_descent_training_direction(const Vector<double>& gradient) const
{
   double gradient_norm = gradient.calculate_norm(); 

   return(gradient/(-1.0*gradient_norm));   
}


// Matrix<double> calculate_DFP_inverse_Hessian(const Vector<double>&, const Vector<double>&, const Vector<double>&, const Vector<double>&, const Matrix<double>&) method

/// This method returns an approximation of the inverse Hessian matrix according to the Davidon-Fletcher-Powel 
/// (DFP) algorithm. 
/// @param old_parameters A previous set of parameters.
/// @param old_gradient The gradient of the objective function for that previous set of parameters.
/// @param old_inverse_Hessian The Hessian of the objective function for that previous set of parameters.
/// @param parameters Actual set of parameters.
/// @param gradient The gradient of the objective function for the actual set of parameters.

Matrix<double> QuasiNewtonMethod::calculate_DFP_inverse_Hessian(
const Vector<double>& old_parameters, const Vector<double>& parameters, const Vector<double>& old_gradient, const Vector<double>& gradient, const Matrix<double>& old_inverse_Hessian) const
{
   // Control sentence (if debug)

   #ifdef _DEBUG 

   NeuralNetwork* neural_network_pointer = performance_functional_pointer->get_neural_network_pointer();

   unsigned int parameters_number = neural_network_pointer->count_parameters_number();            

   unsigned int old_parameters_size = old_parameters.size();
   unsigned int parameters_size = parameters.size();

   if(old_parameters_size != parameters_number)
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: QuasiNewtonMethod class.\n"
             << "Matrix<double> calculate_DFP_inverse_Hessian(const Vector<double>&, const Vector<double>&, const Vector<double>&, const Vector<double>&, const Matrix<double>&) method.\n"
             << "Size of old parameters vector must be equal to number of parameters.\n";

      throw std::logic_error(buffer.str().c_str());	  
   }
   else if(parameters_size != parameters_number)
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: QuasiNewtonMethod class.\n"
             << "Matrix<double> calculate_DFP_inverse_Hessian(const Vector<double>&, const Vector<double>&, const Vector<double>&, const Vector<double>&, const Matrix<double>&) method.\n"
             << "Size of parameters vector must be equal to number of parameters.\n";

      throw std::logic_error(buffer.str().c_str());	  
   }

   unsigned int old_gradient_size = old_gradient.size();
   unsigned int gradient_size = gradient.size();
    
   if(old_gradient_size != parameters_number)
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: QuasiNewtonMethod class.\n"
             << "Matrix<double> calculate_DFP_inverse_Hessian(const Vector<double>&, const Vector<double>&, const Vector<double>&, const Vector<double>&, const Matrix<double>&) method.\n"
             << "Size of old gradient vector must be equal to number of parameters.\n";

      throw std::logic_error(buffer.str().c_str());	  
   }
   else if(gradient_size != parameters_number)
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: QuasiNewtonMethod class.\n"
             << "Matrix<double> calculate_DFP_inverse_Hessian(const Vector<double>&, const Vector<double>&, const Vector<double>&, const Vector<double>&, const Matrix<double>&) method.\n"
             << "Size of gradient vector must be equal to number of parameters.\n";

      throw std::logic_error(buffer.str().c_str());	  
   }

   unsigned int rows_number = old_inverse_Hessian.get_rows_number();
   unsigned int columns_number = old_inverse_Hessian.get_columns_number();

   if(rows_number != parameters_number)
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: QuasiNewtonMethod class.\n"
             << "Matrix<double> calculate_DFP_inverse_Hessian(const Vector<double>&, const Vector<double>&, const Vector<double>&, const Vector<double>&, const Matrix<double>&) method.\n"
             << "Number of rows in old inverse Hessian must be equal to number of parameters.\n";

      throw std::logic_error(buffer.str().c_str());	  
   }
   else if(columns_number != parameters_number)
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: QuasiNewtonMethod class.\n"
             << "Matrix<double> calculate_DFP_inverse_Hessian(const Vector<double>&, const Vector<double>&, const Vector<double>&, const Vector<double>&, const Matrix<double>&) method.\n"
             << "Number of columns in old inverse Hessian must be equal to number of parameters.\n";

      throw std::logic_error(buffer.str().c_str());	  
   }
 
   #endif

   // Parameters difference Vector
   
   Vector<double> parameters_difference = parameters - old_parameters;
   
   // Control sentence (if debug)

   #ifdef _DEBUG 

   if(parameters_difference > -1.0e-50 && parameters_difference < 1.0e-50)
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: QuasiNewtonMethod class.\n"
             << "Matrix<double> calculate_DFP_inverse_Hessian(const Vector<double>&, const Vector<double>&, const Vector<double>&, const Vector<double>&, const Matrix<double>&) method.\n"
             << "Parameters difference vector is zero.\n";

      throw std::logic_error(buffer.str().c_str());	  
   }

   #endif

   // Gradient difference Vector
   
   Vector<double> gradient_difference = gradient - old_gradient;

   // Control sentence (if debug)

   #ifdef _DEBUG 

   if(gradient_difference > -1.0e-50 && gradient_difference < 1.0e-50)
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: QuasiNewtonMethod class.\n"
             << "Matrix<double> calculate_DFP_inverse_Hessian(const Vector<double>&, const Vector<double>&, const Vector<double>&, const Vector<double>&, const Matrix<double>&) method.\n"
             << "Gradient difference vector is zero.\n";

      throw std::logic_error(buffer.str().c_str());	  
   }

   #endif

   // Control sentence (if debug)

   #ifdef _DEBUG 

   if(old_inverse_Hessian > -1.0e-50 && old_inverse_Hessian < 1.0e-50)
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: QuasiNewtonMethod class.\n"
             << "Matrix<double> calculate_DFP_inverse_Hessian(const Vector<double>&, const Vector<double>&, const Vector<double>&, const Vector<double>&, const Matrix<double>&) method.\n"
             << "Old inverse Hessian matrix is zero.\n";

      throw std::logic_error(buffer.str().c_str());	  
   }

   if(fabs(parameters_difference.dot(gradient_difference)) < 1.0e-50)
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: QuasiNewtonMethod class.\n"
             << "Matrix<double> calculate_DFP_inverse_Hessian(const Vector<double>&, const Vector<double>&, const Vector<double>&, const Vector<double>&, const Matrix<double>&) method.\n"
             << "Denominator of first term is zero.\n";

      throw std::logic_error(buffer.str().c_str());	  
   }
   else if(fabs(gradient_difference.dot(old_inverse_Hessian).dot(gradient_difference)) < 1.0e-50)
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: QuasiNewtonMethod class.\n"
             << "Matrix<double> calculate_DFP_inverse_Hessian(const Vector<double>&, const Vector<double>&, const Vector<double>&, const Vector<double>&, const Matrix<double>&) method.\n"
             << "Denominator of second term is zero.\n";

      throw std::logic_error(buffer.str().c_str());	  
   }

   #endif

   Matrix<double> DFP_inverse_Hessian = old_inverse_Hessian
   + parameters_difference.direct(parameters_difference)
   /parameters_difference.dot(gradient_difference)
   - (old_inverse_Hessian.dot(gradient_difference)).direct(old_inverse_Hessian.dot(gradient_difference))
   /gradient_difference.dot(old_inverse_Hessian).dot(gradient_difference);

   return(DFP_inverse_Hessian);               

}


// Matrix<double> calculate_BFGS_inverse_Hessian(const Vector<double>&, const Vector<double>&, const Matrix<double>&, const Vector<double>&, const Vector<double>&) method

/// This method returns an approximation of the inverse Hessian matrix according to the 
/// Broyden-Fletcher-Goldfarb-Shanno (BGFS) algorithm. 
/// @param old_parameters A previous set of parameters.
/// @param old_gradient The gradient of the objective function for that previous set of parameters.
/// @param old_inverse_Hessian The Hessian of the objective function for that previous set of parameters.
/// @param parameters Actual set of parameters.
/// @param gradient The gradient of the objective function for the actual set of parameters.

Matrix<double> QuasiNewtonMethod::calculate_BFGS_inverse_Hessian(
const Vector<double>& old_parameters, const Vector<double>& parameters, const Vector<double>& old_gradient, const Vector<double>& gradient, const Matrix<double>& old_inverse_Hessian) const
{
   // Control sentence (if debug)

   #ifdef _DEBUG 

	NeuralNetwork* neural_network_pointer = performance_functional_pointer->get_neural_network_pointer();

   unsigned int parameters_number = neural_network_pointer->count_parameters_number();            

   unsigned int old_parameters_size = old_parameters.size();
   unsigned int parameters_size = parameters.size();
    
   if(old_parameters_size != parameters_number)
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: QuasiNewtonMethod class.\n"
             << "Matrix<double> calculate_BFGS_inverse_Hessian(const Vector<double>&, const Vector<double>&, const Vector<double>&, const Vector<double>&, const Matrix<double>&) method.\n"
             << "Size of old parameters vector must be equal to number of parameters.\n";

      throw std::logic_error(buffer.str().c_str());	  
   }
   else if(parameters_size != parameters_number)
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: QuasiNewtonMethod class.\n"
             << "Matrix<double> calculate_BFGS_inverse_Hessian(const Vector<double>&, const Vector<double>&, const Vector<double>&, const Vector<double>&, const Matrix<double>&) method.\n"
             << "Size of parameters vector must be equal to number of parameters.\n";

      throw std::logic_error(buffer.str().c_str());	  
   }

   unsigned int old_gradient_size = old_gradient.size();
   unsigned int gradient_size = gradient.size();

   if(old_gradient_size != parameters_number)
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: QuasiNewtonMethod class.\n"
             << "Matrix<double> calculate_BFGS_inverse_Hessian(const Vector<double>&, const Vector<double>&, const Vector<double>&, const Vector<double>&, const Matrix<double>&) method." 
             << std::endl
             << "Size of old gradient vector must be equal to number of parameters.\n";

      throw std::logic_error(buffer.str().c_str());	  
   }
   else if(gradient_size != parameters_number)
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: QuasiNewtonMethod class.\n"
             << "Matrix<double> calculate_BFGS_inverse_Hessian(const Vector<double>&, const Vector<double>&, const Vector<double>&, const Vector<double>&, const Matrix<double>&) method." 
             << std::endl
             << "Size of gradient vector must be equal to number of parameters.\n";

      throw std::logic_error(buffer.str().c_str());	  
   }

   unsigned int rows_number = old_inverse_Hessian.get_rows_number();
   unsigned int columns_number = old_inverse_Hessian.get_columns_number();

   if(rows_number != parameters_number)
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: QuasiNewtonMethod class.\n"
             << "Matrix<double> calculate_BFGS_inverse_Hessian(const Vector<double>&, const Vector<double>&, const Vector<double>&, const Vector<double>&, const Matrix<double>&) method.\n"
             << "Number of rows in old inverse Hessian must be equal to number of parameters.\n";

      throw std::logic_error(buffer.str().c_str());	  
   }
   else if(columns_number != parameters_number)
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: QuasiNewtonMethod class.\n"
             << "Matrix<double> calculate_BFGS_inverse_Hessian(const Vector<double>&, const Vector<double>&, const Vector<double>&, const Vector<double>&, const Matrix<double>&) method.\n"
             << "Number of columns in old inverse Hessian must be equal to number of parameters.\n";

      throw std::logic_error(buffer.str().c_str());	  
   }

   #endif

   // Parameters difference Vector
   
   Vector<double> parameters_difference = parameters - old_parameters;

   // Control sentence (if debug)

   #ifdef _DEBUG 

   if(parameters_difference > -1.0e-50 && parameters_difference < 1.0e-50)
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: QuasiNewtonMethod class.\n"
             << "Matrix<double> calculate_BFGS_inverse_Hessian(const Vector<double>&, const Vector<double>&, const Vector<double>&, const Vector<double>&, const Matrix<double>&) method.\n"
             << "Parameters difference vector is zero.\n";

      throw std::logic_error(buffer.str().c_str());	  
   }

   #endif

   // Gradient difference Vector
   
   Vector<double> gradient_difference = gradient - old_gradient;

   // Control sentence (if debug)

   #ifdef _DEBUG 

   if(gradient_difference > -1.0e-50 && gradient_difference < 1.0e-50)
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: QuasiNewtonMethod class.\n"
             << "Matrix<double> calculate_DFP_inverse_Hessian(const Vector<double>&, const Vector<double>&, const Vector<double>&, const Vector<double>&, const Matrix<double>&) method.\n"
             << "Gradient difference vector is zero.\n";

      throw std::logic_error(buffer.str().c_str());	  
   }

   #endif

   // Control sentence (if debug)

   #ifdef _DEBUG 

   if(old_inverse_Hessian > -1.0e-50 && old_inverse_Hessian < 1.0e-50)
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: QuasiNewtonMethod class.\n"
             << "Matrix<double> calculate_DFP_inverse_Hessian(const Vector<double>&, const Vector<double>&, const Vector<double>&, const Vector<double>&, const Matrix<double>&) method.\n"
             << "Old inverse Hessian matrix is zero.\n";
   
	  throw std::logic_error(buffer.str());
   }

   #endif

   // BGFS Vector

   Vector<double> BFGS = parameters_difference/parameters_difference.dot(gradient_difference) 
   - old_inverse_Hessian.dot(gradient_difference)
   /gradient_difference.dot(old_inverse_Hessian).dot(gradient_difference);

   // Calculate inverse Hessian approximation

   Matrix<double> BFGS_inverse_Hessian = old_inverse_Hessian
   + parameters_difference.direct(parameters_difference)
   /parameters_difference.dot(gradient_difference) 
   - (old_inverse_Hessian.dot(gradient_difference)).direct(gradient_difference.dot(old_inverse_Hessian))
   /gradient_difference.dot(old_inverse_Hessian).dot(gradient_difference)
   + (BFGS.direct(BFGS))*(gradient_difference.dot(old_inverse_Hessian).dot(gradient_difference));   
   
   return(BFGS_inverse_Hessian);               
}


// void resize_training_history(const unsigned int&) method

/// This method resizes all the training history variables. 
/// @param new_size Size of training history variables. 

void QuasiNewtonMethod::QuasiNetwonMethodResults::resize_training_history(const unsigned int& new_size)
{
   if(!parameters_history.empty())
   {
      parameters_history.resize(new_size);
   }


   if(!parameters_norm_history.empty())
   {
      parameters_norm_history.resize(new_size);
   }


   if(!evaluation_history.empty())
   {
      evaluation_history.resize(new_size);
   }

   if(!generalization_evaluation_history.empty())
   {
      generalization_evaluation_history.resize(new_size);
   }

   if(!gradient_history.empty())
   {
      gradient_history.resize(new_size);
   }

   if(!gradient_norm_history.empty())
   {
      gradient_norm_history.resize(new_size);
   }

   if(!inverse_Hessian_history.empty())
   {
      inverse_Hessian_history.resize(new_size);
   }
   
   if(!training_direction_history.empty())
   {
      training_direction_history.resize(new_size);
   }

   if(!training_rate_history.empty())
   {
      training_rate_history.resize(new_size);
   }

   if(!elapsed_time_history.empty())
   {
      elapsed_time_history.resize(new_size);
   }
}


// std::string to_string(void) const method

/// This method returns a string representation of the current quasi-Newton method results structure. 

std::string QuasiNewtonMethod::QuasiNetwonMethodResults::to_string(void) const
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

   // Inverse Hessian history

   if(!inverse_Hessian_history.empty())
   {
	   if(!inverse_Hessian_history[0].empty())
	   {
          buffer << "% Inverse Hessian history:\n"
                 << inverse_Hessian_history << "\n"; 
	   }
   }

   // Training direction history

   if(!training_direction_history.empty())
   {
	   if(!training_direction_history[0].empty())
	   {
          buffer << "% Training direction history:\n"
                 << training_direction_history << "\n"; 
	   }
   }

   // Training rate history

   if(!training_rate_history.empty())
   {
       buffer << "% Training rate history:\n"
              << training_rate_history << "\n"; 
   }

   // Elapsed time history

   if(!elapsed_time_history.empty())
   {
       buffer << "% Elapsed time history:\n"
              << elapsed_time_history << "\n"; 
   }

   return(buffer.str());
}


// QuasiNewtonMethodResults* perform_training(void) method

/// This method trains a neural network with an associated performance functional according to the quasi-Newton method.
/// Training occurs according to the training operators, training parameters and stopping criteria.

QuasiNewtonMethod::QuasiNetwonMethodResults* QuasiNewtonMethod::perform_training(void)
{
   // Control sentence (if debug)

   #ifdef _DEBUG 

   check();

   #endif

   // Start training 

   if(display)
   {
      std::cout << "Training with quasi-Newton method...\n";
   }

   QuasiNetwonMethodResults* results_pointer = new QuasiNetwonMethodResults;

   if(reserve_parameters_history)
   {
      results_pointer->parameters_history.resize(1 + maximum_epochs_number);
   }
   if(reserve_parameters_norm_history)
   {
      results_pointer->parameters_norm_history.resize(1 + maximum_epochs_number);
   }
   if(reserve_evaluation_history)
   {
      results_pointer->evaluation_history.resize(1 + maximum_epochs_number);
   }
   if(reserve_generalization_evaluation_history)
   {
      results_pointer->generalization_evaluation_history.resize(1 + maximum_epochs_number);
   }
   if(reserve_gradient_history)
   {
      results_pointer->gradient_history.resize(1 + maximum_epochs_number);
   }
   if(reserve_gradient_norm_history)
   {
      results_pointer->gradient_norm_history.resize(1 + maximum_epochs_number);
   }
   if(reserve_inverse_Hessian_history)
   {
      results_pointer->inverse_Hessian_history.resize(1 + maximum_epochs_number);
   }
   if(reserve_training_direction_history)
   {
      results_pointer->training_direction_history.resize(1 + maximum_epochs_number);
   }
   if(reserve_training_rate_history)
   {
      results_pointer->training_rate_history.resize(1 + maximum_epochs_number);
   }
   if(reserve_elapsed_time_history)
   {
      results_pointer->elapsed_time_history.resize(1 + maximum_epochs_number);
   }

   // Neural network stuff

   NeuralNetwork* neural_network_pointer = performance_functional_pointer->get_neural_network_pointer();

   const unsigned int parameters_number = neural_network_pointer->count_parameters_number();

   Vector<double> parameters(parameters_number);
   Vector<double> old_parameters(parameters_number);
   double parameters_norm;

   Vector<double> parameters_increment(parameters_number);
   double parameters_increment_norm;

   // Performance functional stuff
     
   double performance = 0.0;
   double old_performance = 0.0;
   double performance_increase = 0.0;

   Vector<double> gradient(parameters_number);
   Vector<double> old_gradient(parameters_number);
   double gradient_norm;

   Matrix<double> inverse_Hessian(parameters_number, parameters_number);
   Matrix<double> old_inverse_Hessian(parameters_number, parameters_number);

   double generalization_evaluation = 0.0; 
   double old_generalization_evaluation = 0.0;

   // Training algorithm stuff 

   Vector<double> training_direction(parameters_number);

   double training_slope;

   const double& first_training_rate = training_rate_algorithm.get_first_training_rate();

   double initial_training_rate = 0.0;
   double training_rate = 0.0;
   double old_training_rate = 0.0;

   Vector<double> directional_point(2);
   directional_point[0] = 0.0;
   directional_point[1] = 0.0;

   bool stop_training = false;

   unsigned int generalization_evaluation_decreases_count = 0;

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
         std::cout << "OpenNN Warning: Parameters norm is " << parameters_norm << ".\n";          
      }

      // Performance functional stuff

      if(epoch == 0)
      {      
         performance = performance_functional_pointer->calculate_evaluation();
         performance_increase = 0.0; 
      }
      else
      {
         performance = directional_point[1];
         performance_increase = old_performance - performance; 
      }

      gradient = performance_functional_pointer->calculate_gradient();

      gradient_norm = gradient.calculate_norm();

      if(display && gradient_norm >= warning_gradient_norm)
      {
         std::cout << "OpenNN Warning: Gradient norm is " << gradient_norm << ".\n";          
      }

      if(epoch == 0)
      {
         inverse_Hessian.initialize_identity();
      }
      else
      {
         inverse_Hessian = calculate_inverse_Hessian_approximation(old_parameters, parameters, old_gradient, gradient, old_inverse_Hessian);	
      }

      generalization_evaluation = performance_functional_pointer->calculate_generalization_evaluation();

      if(epoch != 0 && generalization_evaluation > old_generalization_evaluation)
      {
         generalization_evaluation_decreases_count++;	  
      }

      // Training algorithm 

      training_direction = calculate_training_direction(gradient, inverse_Hessian);          

      // Calculate performance training slope

      training_slope = (gradient/gradient_norm).dot(training_direction);

      // Check for a descent direction 

      if(training_slope >= 0.0)
      {
         // Reset training direction

         training_direction = calculate_gradient_descent_training_direction(gradient);
      }

      // Get initial training rate

      if(epoch == 0)
      {
         initial_training_rate = first_training_rate;
      }
      else
      {
         initial_training_rate = old_training_rate;
      }

      directional_point = training_rate_algorithm.calculate_directional_point(performance, training_direction, initial_training_rate);

      training_rate = directional_point[0];      

      if(epoch != 0 && training_rate < 1.0e-99)
      {
         // Reset training direction

         training_direction = calculate_gradient_descent_training_direction(gradient);         

         directional_point = training_rate_algorithm.calculate_directional_point(performance, training_direction, first_training_rate);

         training_rate = directional_point[0];
      }

      parameters_increment = training_direction*training_rate;
      parameters_increment_norm = parameters_increment.calculate_norm();
      
      // Elapsed time

      time(&current_time);
      elapsed_time = difftime(current_time, beginning_time);

      // Training history multilayer perceptron 

      if(reserve_parameters_history)
      {
         results_pointer->parameters_history[epoch] = parameters;                                
      }

      if(reserve_parameters_norm_history)
      {
         results_pointer->parameters_norm_history[epoch] = parameters_norm; 
      }

      if(reserve_evaluation_history)
      {
         results_pointer->evaluation_history[epoch] = performance;
      }

      if(reserve_generalization_evaluation_history)
      {
         results_pointer->generalization_evaluation_history[epoch] = generalization_evaluation;
      }

      if(reserve_gradient_history)
      {
         results_pointer->gradient_history[epoch] = gradient;                                
      }

      if(reserve_gradient_norm_history)
      {
         results_pointer->gradient_norm_history[epoch] = gradient_norm;
      }

      if(reserve_inverse_Hessian_history)
      {
         results_pointer->inverse_Hessian_history[epoch] = inverse_Hessian;
      }

      // Training history training algorithm

      if(reserve_training_direction_history)
      {
         results_pointer->training_direction_history[epoch] = training_direction;                                
      }

      if(reserve_training_rate_history)
      {
         results_pointer->training_rate_history[epoch] = training_rate;
      }

      if(reserve_elapsed_time_history)
      {
         results_pointer->elapsed_time_history[epoch] = elapsed_time;
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

      if(epoch != 0 && performance_increase <= minimum_performance_increase)
      {
         if(display)
         {
            std::cout << "Epoch " << epoch << ": Minimum performance increase reached.\n"
                      << "Performance increase: " << performance_increase << std::endl;
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

      else if(gradient_norm <= gradient_norm_goal)
      {
         if(display)
         {
            std::cout << "Epoch " << epoch << ": Gradient norm goal reached.\n";  
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
            std::cout << "Epoch " << epoch << ": Maximum number of epochs reached.\n";
         }

         stop_training = true;
      }

      else if(elapsed_time >= maximum_time)
      {
         if(display)
         {
            std::cout << "Epoch " << epoch << ": Maximum training time reached.\n";
         }

         stop_training = true;
      }

      if(stop_training)
      {
         results_pointer->final_parameters = parameters;
         results_pointer->final_parameters_norm = parameters_norm;

         results_pointer->final_evaluation = performance;
         results_pointer->final_generalization_evaluation = generalization_evaluation;

         results_pointer->final_gradient = gradient;
         results_pointer->final_gradient_norm = gradient_norm;
   
         results_pointer->final_training_direction = training_direction;
         results_pointer->final_training_rate = training_rate;
         results_pointer->elapsed_time = elapsed_time;

		 results_pointer->resize_training_history(epoch+1);

         if(display)
		 {
            std::cout << "Parameters norm: " << parameters_norm << "\n"
                      << "Performance: " << performance <<  "\n"
                      << "Gradient norm: " << gradient_norm <<  "\n"
                      << performance_functional_pointer->write_information() 
                      << "Training rate: " << training_rate <<  "\n"
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
                   << "Training rate: " << training_rate << "\n"
                   << "Elapsed time: " << elapsed_time << std::endl; 

         if(generalization_evaluation != 0)
         {
            std::cout << "Generalization performance: " << generalization_evaluation << std::endl;
         }
      }

      // Update stuff

      old_parameters = parameters;

      old_performance = performance;

	  old_gradient = gradient;

	  old_inverse_Hessian = inverse_Hessian;
 
	  old_generalization_evaluation = generalization_evaluation;

      old_training_rate = training_rate;

      old_generalization_evaluation = generalization_evaluation;

      // Set new parameters

      parameters += parameters_increment;

      neural_network_pointer->set_parameters(parameters);
   }

   return(results_pointer);
}


// std::string write_training_algorithm_type(void) const method

std::string QuasiNewtonMethod::write_training_algorithm_type(void) const
{
   return("QUASI_NEWTON_METHOD");
}


// TiXmlElement* to_XML(void) const method

/// This method returns a XML-type string representation of this quasi-Newton method object. 
/// It contains the training methods and parameters chosen, as well as 
/// the stopping criteria and other user stuff concerning the quasi-Newton method object.

TiXmlElement* QuasiNewtonMethod::to_XML(void) const
{
   std::ostringstream buffer;

   // Quasi-Newton method

   TiXmlElement* quasi_Newton_method_element = new TiXmlElement("QuasiNewtonMethod");
   quasi_Newton_method_element->SetAttribute("Version", 4); 

   // Training rate algorithm

   TiXmlElement* training_rate_algorithm_element = training_rate_algorithm.to_XML();
   quasi_Newton_method_element->LinkEndChild(training_rate_algorithm_element);

   // Warning parameters norm

   TiXmlElement* warning_parameters_norm_element = new TiXmlElement("WarningParametersNorm");
   quasi_Newton_method_element->LinkEndChild(warning_parameters_norm_element);

   buffer.str("");
   buffer << warning_parameters_norm;

   TiXmlText* warning_parameters_norm_text = new TiXmlText(buffer.str().c_str());
   warning_parameters_norm_element->LinkEndChild(warning_parameters_norm_text);

   // Warning gradient norm 

   TiXmlElement* warning_gradient_norm_element = new TiXmlElement("WarningGradientNorm");
   quasi_Newton_method_element->LinkEndChild(warning_gradient_norm_element);

   buffer.str("");
   buffer << warning_gradient_norm;

   TiXmlText* warning_gradient_norm_text = new TiXmlText(buffer.str().c_str());
   warning_gradient_norm_element->LinkEndChild(warning_gradient_norm_text);

   // Warning training rate 

   TiXmlElement* warning_training_rate_element = new TiXmlElement("WarningTrainingRate");
   quasi_Newton_method_element->LinkEndChild(warning_training_rate_element);

   buffer.str("");
   buffer << warning_training_rate;

   TiXmlText* warning_training_rate_text = new TiXmlText(buffer.str().c_str());
   warning_training_rate_element->LinkEndChild(warning_training_rate_text);

   // Error parameters norm

   TiXmlElement* error_parameters_norm_element = new TiXmlElement("ErrorParametersNorm");
   quasi_Newton_method_element->LinkEndChild(error_parameters_norm_element);

   buffer.str("");
   buffer << error_parameters_norm;

   TiXmlText* error_parameters_norm_text = new TiXmlText(buffer.str().c_str());
   error_parameters_norm_element->LinkEndChild(error_parameters_norm_text);

   // Error gradient norm 

   TiXmlElement* error_gradient_norm_element = new TiXmlElement("ErrorGradientNorm");
   quasi_Newton_method_element->LinkEndChild(error_gradient_norm_element);

   buffer.str("");
   buffer << error_gradient_norm;

   TiXmlText* error_gradient_norm_text = new TiXmlText(buffer.str().c_str());
   error_gradient_norm_element->LinkEndChild(error_gradient_norm_text);

   // Error training rate

   TiXmlElement* error_training_rate_element = new TiXmlElement("ErrorTrainingRate");
   quasi_Newton_method_element->LinkEndChild(error_training_rate_element);

   buffer.str("");
   buffer << error_training_rate;

   TiXmlText* error_training_rate_text = new TiXmlText(buffer.str().c_str());
   error_training_rate_element->LinkEndChild(error_training_rate_text);

   // Minimum parameters increment norm

   TiXmlElement* minimum_parameters_increment_norm_element = new TiXmlElement("MinimumParametersIncrement");
   quasi_Newton_method_element->LinkEndChild(minimum_parameters_increment_norm_element);

   buffer.str("");
   buffer << minimum_parameters_increment_norm;

   TiXmlText* minimum_parameters_increment_norm_text = new TiXmlText(buffer.str().c_str());
   minimum_parameters_increment_norm_element->LinkEndChild(minimum_parameters_increment_norm_text);

   // Minimum performance increase 

   TiXmlElement* minimum_performance_increase_element = new TiXmlElement("MinimumPerformanceIncrease");
   quasi_Newton_method_element->LinkEndChild(minimum_performance_increase_element);

   buffer.str("");
   buffer << minimum_performance_increase;

   TiXmlText* minimum_performance_increase_text = new TiXmlText(buffer.str().c_str());
   minimum_performance_increase_element->LinkEndChild(minimum_performance_increase_text);

   // Performance goal 

   TiXmlElement* performance_goal_element = new TiXmlElement("PerformanceGoal");
   quasi_Newton_method_element->LinkEndChild(performance_goal_element);

   buffer.str("");
   buffer << performance_goal;

   TiXmlText* performance_goal_text = new TiXmlText(buffer.str().c_str());
   performance_goal_element->LinkEndChild(performance_goal_text);

   // Gradient norm goal 

   TiXmlElement* gradient_norm_goal_element = new TiXmlElement("GradientNormGoal");
   quasi_Newton_method_element->LinkEndChild(gradient_norm_goal_element);

   buffer.str("");
   buffer << gradient_norm_goal;

   TiXmlText* gradient_norm_goal_text = new TiXmlText(buffer.str().c_str());
   gradient_norm_goal_element->LinkEndChild(gradient_norm_goal_text);

   // Maximum generalization performance decreases

   TiXmlElement* maximum_generalization_evaluation_decreases_element = new TiXmlElement("MaximumGeneralizationEvaluationDecreases");
   quasi_Newton_method_element->LinkEndChild(maximum_generalization_evaluation_decreases_element);

   buffer.str("");
   buffer << maximum_generalization_evaluation_decreases;

   TiXmlText* maximum_generalization_evaluation_decreases_text = new TiXmlText(buffer.str().c_str());
   maximum_generalization_evaluation_decreases_element->LinkEndChild(maximum_generalization_evaluation_decreases_text);

   // Maximum epochs number 

   TiXmlElement* maximum_epochs_number_element = new TiXmlElement("MaximumEpochsNumber");
   quasi_Newton_method_element->LinkEndChild(maximum_epochs_number_element);

   buffer.str("");
   buffer << maximum_epochs_number;

   TiXmlText* maximum_epochs_number_text = new TiXmlText(buffer.str().c_str());
   maximum_epochs_number_element->LinkEndChild(maximum_epochs_number_text);

   // Maximum time 

   TiXmlElement* maximum_time_element = new TiXmlElement("MaximumTime");
   quasi_Newton_method_element->LinkEndChild(maximum_time_element);

   buffer.str("");
   buffer << maximum_time;

   TiXmlText* maximum_time_text = new TiXmlText(buffer.str().c_str());
   maximum_time_element->LinkEndChild(maximum_time_text);

   // Reserve parameters history 

   TiXmlElement* reserve_parameters_history_element = new TiXmlElement("ReserveParametersHistory");
   quasi_Newton_method_element->LinkEndChild(reserve_parameters_history_element);

   buffer.str("");
   buffer << reserve_parameters_history;

   TiXmlText* reserve_parameters_history_text = new TiXmlText(buffer.str().c_str());
   reserve_parameters_history_element->LinkEndChild(reserve_parameters_history_text);

   // Reserve parameters norm history 

   TiXmlElement* reserve_parameters_norm_history_element = new TiXmlElement("ReserveParametersNormHistory");
   quasi_Newton_method_element->LinkEndChild(reserve_parameters_norm_history_element);

   buffer.str("");
   buffer << reserve_parameters_norm_history;

   TiXmlText* reserve_parameters_norm_history_text = new TiXmlText(buffer.str().c_str());
   reserve_parameters_norm_history_element->LinkEndChild(reserve_parameters_norm_history_text);

   // Reserve evaluation history 

   TiXmlElement* reserve_evaluation_history_element = new TiXmlElement("ReservePerformanceHistory");
   quasi_Newton_method_element->LinkEndChild(reserve_evaluation_history_element);

   buffer.str("");
   buffer << reserve_evaluation_history;

   TiXmlText* reserve_evaluation_history_text = new TiXmlText(buffer.str().c_str());
   reserve_evaluation_history_element->LinkEndChild(reserve_evaluation_history_text);

   // Reserve gradient history 

   TiXmlElement* reserve_gradient_history_element = new TiXmlElement("ReserveGradientHistory");
   quasi_Newton_method_element->LinkEndChild(reserve_gradient_history_element);

   buffer.str("");
   buffer << reserve_gradient_history;

   TiXmlText* reserve_gradient_history_text = new TiXmlText(buffer.str().c_str());
   reserve_gradient_history_element->LinkEndChild(reserve_gradient_history_text);

   // Reserve gradient norm history 

   TiXmlElement* reserve_gradient_norm_history_element = new TiXmlElement("ReserveGradientNormHistory");
   quasi_Newton_method_element->LinkEndChild(reserve_gradient_norm_history_element);

   buffer.str("");
   buffer << reserve_gradient_norm_history;

   TiXmlText* reserve_gradient_norm_history_text = new TiXmlText(buffer.str().c_str());
   reserve_gradient_norm_history_element->LinkEndChild(reserve_gradient_norm_history_text);

   // Reserve inverse Hessian history 

   TiXmlElement* reserve_inverse_Hessian_history_element = new TiXmlElement("ReserveInverseHessianHistory");
   quasi_Newton_method_element->LinkEndChild(reserve_inverse_Hessian_history_element);

   buffer.str("");
   buffer << reserve_inverse_Hessian_history;

   TiXmlText* reserve_inverse_Hessian_history_text = new TiXmlText(buffer.str().c_str());
   reserve_inverse_Hessian_history_element->LinkEndChild(reserve_inverse_Hessian_history_text);

   // Reserve training direction history 

   TiXmlElement* reserve_training_direction_history_element = new TiXmlElement("ReserveTrainingDirectionHistory");
   quasi_Newton_method_element->LinkEndChild(reserve_training_direction_history_element);

   buffer.str("");
   buffer << reserve_training_direction_history;

   TiXmlText* reserve_training_direction_history_text = new TiXmlText(buffer.str().c_str());
   reserve_training_direction_history_element->LinkEndChild(reserve_training_direction_history_text);

   // Reserve training rate history 

   TiXmlElement* reserve_training_rate_history_element = new TiXmlElement("ReserveTrainingRateHistory");
   quasi_Newton_method_element->LinkEndChild(reserve_training_rate_history_element);

   buffer.str("");
   buffer << reserve_training_rate_history;

   TiXmlText* reserve_training_rate_history_text = new TiXmlText(buffer.str().c_str());
   reserve_training_rate_history_element->LinkEndChild(reserve_training_rate_history_text);

   // Reserve elapsed time history 

   TiXmlElement* reserve_elapsed_time_history_element = new TiXmlElement("ReserveElapsedTimeHistory");
   quasi_Newton_method_element->LinkEndChild(reserve_elapsed_time_history_element);

   buffer.str("");
   buffer << reserve_elapsed_time_history;

   TiXmlText* reserve_elapsed_time_history_text = new TiXmlText(buffer.str().c_str());
   reserve_elapsed_time_history_element->LinkEndChild(reserve_elapsed_time_history_text);

   // Reserve generalization evaluation history 

   TiXmlElement* reserve_generalization_evaluation_history_element = new TiXmlElement("ReserveGeneralizationPerformanceHistory");
   quasi_Newton_method_element->LinkEndChild(reserve_generalization_evaluation_history_element);

   buffer.str("");
   buffer << reserve_generalization_evaluation_history;

   TiXmlText* reserve_generalization_evaluation_history_text = new TiXmlText(buffer.str().c_str());
   reserve_generalization_evaluation_history_element->LinkEndChild(reserve_generalization_evaluation_history_text);

   // Display period

   TiXmlElement* display_period_element = new TiXmlElement("DisplayPeriod");
   quasi_Newton_method_element->LinkEndChild(display_period_element);

   buffer.str("");
   buffer << display_period;

   TiXmlText* display_period_text = new TiXmlText(buffer.str().c_str());
   display_period_element->LinkEndChild(display_period_text);

   // Display

   TiXmlElement* display_element = new TiXmlElement("Display");
   quasi_Newton_method_element->LinkEndChild(display_element);

   buffer.str("");
   buffer << display;

   TiXmlText* display_text = new TiXmlText(buffer.str().c_str());
   display_element->LinkEndChild(display_text);

   return(quasi_Newton_method_element);
}


// void from_XML(TiXmlElement*) const method

void QuasiNewtonMethod::from_XML(TiXmlElement* quasi_Newton_method_element) 
{
   // Inverse Hessian approximation method

   TiXmlElement* inverse_Hessian_approximation_method_element = quasi_Newton_method_element->FirstChildElement("InverseHessianApproximationMethod");

   if(inverse_Hessian_approximation_method_element)
   {
      std::string new_inverse_Hessian_approximation_method = inverse_Hessian_approximation_method_element->GetText(); 

      try
      {
         set_inverse_Hessian_approximation_method(new_inverse_Hessian_approximation_method);
      }
      catch(std::exception& e)
      {
         std::cout << e.what() << std::endl;		 
      }
   }

   // Training rate algorithm

   TiXmlElement* training_rate_algorithm_element = quasi_Newton_method_element->FirstChildElement("TrainingRateAlgorithm");

   if(training_rate_algorithm_element)
   {
      try
      {
         training_rate_algorithm.from_XML(training_rate_algorithm_element);
      }
      catch(std::exception& e)
      {
         std::cout << e.what() << std::endl;		 
      }
   }

   // Warning parameters norm

   TiXmlElement* warning_parameters_norm_element = quasi_Newton_method_element->FirstChildElement("WarningParametersNorm");

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

   TiXmlElement* warning_gradient_norm_element = quasi_Newton_method_element->FirstChildElement("WarningGradientNorm");

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

   // Warning training rate 

   TiXmlElement* warning_training_rate_element = quasi_Newton_method_element->FirstChildElement("WarningTrainingRate");

   if(warning_training_rate_element)
   {
      double new_warning_training_rate = atof(warning_training_rate_element->GetText()); 

      try
      {
         set_warning_training_rate(new_warning_training_rate);
      }
      catch(std::exception& e)
      {
         std::cout << e.what() << std::endl;		 
      }
   }

   // Error parameters norm

   TiXmlElement* error_parameters_norm_element = quasi_Newton_method_element->FirstChildElement("ErrorParametersNorm");

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

   TiXmlElement* error_gradient_norm_element = quasi_Newton_method_element->FirstChildElement("ErrorGradientNorm");

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

   // Error training rate

   TiXmlElement* error_training_rate_element = quasi_Newton_method_element->FirstChildElement("ErrorTrainingRate");

   if(error_training_rate_element)
   {
      double new_error_training_rate = atof(error_training_rate_element->GetText()); 

      try
      {
         set_error_training_rate(new_error_training_rate);
      }
      catch(std::exception& e)
      {
         std::cout << e.what() << std::endl;		 
      }
   }

   // Minimum parameters increment norm

   TiXmlElement* minimum_parameters_increment_norm_element = quasi_Newton_method_element->FirstChildElement("MinimumParametersIncrementNorm");

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

   TiXmlElement* minimum_performance_increase_element = quasi_Newton_method_element->FirstChildElement("MinimumPerformanceIncrease");

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

   TiXmlElement* performance_goal_element = quasi_Newton_method_element->FirstChildElement("PerformanceGoal");

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

   TiXmlElement* gradient_norm_goal_element = quasi_Newton_method_element->FirstChildElement("GradientNormGoal");

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

   TiXmlElement* maximum_generalization_evaluation_decreases_element = quasi_Newton_method_element->FirstChildElement("MaximumGeneralizationEvaluationDecreases");

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

   TiXmlElement* maximum_epochs_number_element = quasi_Newton_method_element->FirstChildElement("MaximumEpochsNumber");

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

   TiXmlElement* maximum_time_element = quasi_Newton_method_element->FirstChildElement("MaximumTime");

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

   TiXmlElement* reserve_parameters_history_element = quasi_Newton_method_element->FirstChildElement("ReserveParametersHistory");

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

   TiXmlElement* reserve_parameters_norm_history_element = quasi_Newton_method_element->FirstChildElement("ReserveParametersNormHistory");

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

   TiXmlElement* reserve_evaluation_history_element = quasi_Newton_method_element->FirstChildElement("ReservePerformanceHistory");

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

   TiXmlElement* reserve_gradient_history_element = quasi_Newton_method_element->FirstChildElement("ReserveGradientHistory");

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

   TiXmlElement* reserve_gradient_norm_history_element = quasi_Newton_method_element->FirstChildElement("ReserveGradientNormHistory");

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

   // Reserve inverse Hessian history 

   TiXmlElement* reserve_inverse_Hessian_history_element = quasi_Newton_method_element->FirstChildElement("ReserveInverseHessianHistory");

   if(reserve_inverse_Hessian_history_element)
   {
      std::string new_reserve_inverse_Hessian_history = reserve_inverse_Hessian_history_element->GetText(); 

      try
      {
         set_reserve_inverse_Hessian_history(new_reserve_inverse_Hessian_history != "0");
      }
      catch(std::exception& e)
      {
         std::cout << e.what() << std::endl;		 
      }
   }

   // Reserve training direction history 

   TiXmlElement* reserve_training_direction_history_element = quasi_Newton_method_element->FirstChildElement("ReserveTrainingDirectionHistory");

   if(reserve_training_direction_history_element)
   {
      std::string new_reserve_training_direction_history = reserve_training_direction_history_element->GetText(); 

      try
      {
         set_reserve_training_direction_history(new_reserve_training_direction_history != "0");
      }
      catch(std::exception& e)
      {
         std::cout << e.what() << std::endl;		 
      }
   }

   // Reserve training rate history 

   TiXmlElement* reserve_training_rate_history_element = quasi_Newton_method_element->FirstChildElement("ReserveTrainingRateHistory");

   if(reserve_training_rate_history_element)
   {
      std::string new_reserve_training_rate_history = reserve_training_rate_history_element->GetText(); 

      try
      {
         set_reserve_training_rate_history(new_reserve_training_rate_history != "0");
      }
      catch(std::exception& e)
      {
         std::cout << e.what() << std::endl;		 
      }
   }

   // Reserve elapsed time history 

   TiXmlElement* reserve_elapsed_time_history_element = quasi_Newton_method_element->FirstChildElement("ReserveElapsedTimeHistory");

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

   TiXmlElement* reserve_generalization_evaluation_history_element = quasi_Newton_method_element->FirstChildElement("ReserveGeneralizationPerformanceHistory");

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

   TiXmlElement* display_period_element = quasi_Newton_method_element->FirstChildElement("DisplayPeriod");

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

   // Display warnings

   TiXmlElement* display_element = quasi_Newton_method_element->FirstChildElement("Display");

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

