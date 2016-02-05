/****************************************************************************************************************/
/*                                                                                                              */
/*   OpenNN: Open Neural Networks Library                                                                       */
/*   www.opennn.cimne.com                                                                                       */
/*                                                                                                              */
/*   G R A D I E N T   D E S C E N T   C L A S S                                                                */
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

// Open NN includes

#include "gradient_descent.h"

namespace OpenNN
{

// DEFAULT CONSTRUCTOR

/// Default constructor. 
/// It creates a gradient descent training algorithm not associated to any performance functional object.
/// It also initializes the class members to their default values.

GradientDescent::GradientDescent(void) 
 : TrainingAlgorithm()
{
   set_default();
}


// PERFORMANCE FUNCTIONAL CONSTRUCTOR 

/// Performance functional constructor. 
/// It creates a gradient descent training algorithm associated to a performance functional.
/// It also initializes the class members to their default values.
/// @param new_performance_functional_pointer Pointer to a performance functional object.

GradientDescent::GradientDescent(PerformanceFunctional* new_performance_functional_pointer)
: TrainingAlgorithm(new_performance_functional_pointer)
{
   training_rate_algorithm.set_performance_functional_pointer(new_performance_functional_pointer);

   set_default();
}


// XML CONSTRUCTOR

/// XML constructor. 
/// It creates a gradient descent training algorithm not associated to any performance functional object.
/// It also loads the class members from a XML element.
/// @param gradient_descent_element Tiny XML element with the members of a gradient descent object. 

GradientDescent::GradientDescent(TiXmlElement* gradient_descent_element) : TrainingAlgorithm(gradient_descent_element)
{
   set_default();

   from_XML(gradient_descent_element);
}


// DESTRUCTOR

/// Destructor.

GradientDescent::~GradientDescent(void)
{
}


// METHODS

// const TrainingRateAlgorithm& get_training_rate_algorithm(void) const method

/// This method returns a constant reference to the training rate algorithm object inside the gradient descent object. 

const TrainingRateAlgorithm& GradientDescent::get_training_rate_algorithm(void) const
{
   return(training_rate_algorithm);
}


// TrainingRateAlgorithm* get_training_rate_algorithm_pointer(void) method

/// This method returns a pointer to the training rate algorithm object inside the gradient descent object. 

TrainingRateAlgorithm* GradientDescent::get_training_rate_algorithm_pointer(void)
{
   return(&training_rate_algorithm);
}


// const double& get_warning_parameters_norm(void) const method

/// This method returns the minimum value for the norm of the parameters vector at wich a warning message is 
/// written to the screen. 

const double& GradientDescent::get_warning_parameters_norm(void) const
{
   return(warning_parameters_norm);       
}


// const double& get_warning_gradient_norm(void) const method

/// This method returns the minimum value for the norm of the gradient vector at wich a warning message is written
/// to the screen. 

const double& GradientDescent::get_warning_gradient_norm(void) const
{
   return(warning_gradient_norm);       
}


// const double& get_warning_training_rate(void) const method

/// This method returns the training rate value at wich a warning message is written to the screen during line 
/// minimization.

const double& GradientDescent::get_warning_training_rate(void) const
{
   return(warning_training_rate);
}


// const double& get_error_parameters_norm(void) const method

/// This method returns the value for the norm of the parameters vector at wich an error message is 
/// written to the screen and the program exits. 

const double& GradientDescent::get_error_parameters_norm(void) const
{
   return(error_parameters_norm);
}


// const double& get_error_gradient_norm(void) const method

/// This method returns the value for the norm of the gradient vector at wich an error message is written
/// to the screen and the program exits. 

const double& GradientDescent::get_error_gradient_norm(void) const
{
   return(error_gradient_norm);
}


// const double& get_error_training_rate(void) const method

/// This method returns the training rate value at wich the line minimization algorithm is assumed to fail when 
/// bracketing a minimum.

const double& GradientDescent::get_error_training_rate(void) const
{
   return(error_training_rate);
}


// const double& get_minimum_parameters_increment_norm(void) const method

/// This method returns the minimum norm of the parameter increment vector used as a stopping criteria when training. 

const double& GradientDescent::get_minimum_parameters_increment_norm(void) const
{
   return(minimum_parameters_increment_norm);
}


// const double& get_minimum_performance_increase(void) const method

/// This method returns the minimum performance improvement during training.  

const double& GradientDescent::get_minimum_performance_increase(void) const
{
   return(minimum_performance_increase);
}


// const double& get_performance_goal(void) const method

/// This method returns the goal value for the performance. 
/// This is used as a stopping criterium when training a multilayer perceptron

const double& GradientDescent::get_performance_goal(void) const
{
   return(performance_goal);
}


// const double& get_gradient_norm_goal(void) const method

/// This method returns the goal value for the norm of the objective function gradient.
/// This is used as a stopping criterium when training a multilayer perceptron

const double& GradientDescent::get_gradient_norm_goal(void) const
{
   return(gradient_norm_goal);
}


// const unsigned int& get_maximum_generalization_evaluation_decreases(void) const method

/// This method returns the maximum number of generalization failures during the training process. 

const unsigned int& GradientDescent::get_maximum_generalization_evaluation_decreases(void) const
{
   return(maximum_generalization_evaluation_decreases);
}


// const unsigned int& get_maximum_epochs_number(void) const method

/// This method returns the maximum number of epochs for training.

const unsigned int& GradientDescent::get_maximum_epochs_number(void) const
{
   return(maximum_epochs_number);
}


// const double& get_maximum_time(void) const method

/// This method returns the maximum training time.  

const double& GradientDescent::get_maximum_time(void) const
{
   return(maximum_time);
}


// const bool& get_reserve_parameters_history(void) const method

/// This method returns true if the parameters history matrix is to be reserved, and false otherwise.

const bool& GradientDescent::get_reserve_parameters_history(void) const
{
   return(reserve_parameters_history);     
}


// const bool& get_reserve_parameters_norm_history(void) const method 

/// This method returns true if the parameters norm history vector is to be reserved, and false otherwise.

const bool& GradientDescent::get_reserve_parameters_norm_history(void) const
{
   return(reserve_parameters_norm_history);     
}


// const bool& get_reserve_evaluation_history(void) const method

/// This method returns true if the evaluation history vector is to be reserved, and false otherwise.

const bool& GradientDescent::get_reserve_evaluation_history(void) const
{
   return(reserve_evaluation_history);     
}


// const bool& get_reserve_gradient_history(void) const method

/// This method returns true if the gradient history vector of vectors is to be reserved, and false otherwise.

const bool& GradientDescent::get_reserve_gradient_history(void) const
{
   return(reserve_gradient_history);     
}


// const bool& get_reserve_gradient_norm_history(void) const method

/// This method returns true if the gradient norm history vector is to be reserved, and false otherwise.

const bool& GradientDescent::get_reserve_gradient_norm_history(void) const
{
   return(reserve_gradient_norm_history);     
}


// const bool& get_reserve_training_direction_history(void) const method

/// This method returns true if the training direction history matrix is to be reserved, and false otherwise.

const bool& GradientDescent::get_reserve_training_direction_history(void) const
{
   return(reserve_training_direction_history);     
}


// const bool& get_reserve_training_rate_history(void) const method

/// This method returns true if the training rate history vector is to be reserved, and false otherwise.

const bool& GradientDescent::get_reserve_training_rate_history(void) const
{
   return(reserve_training_rate_history);     
}


// const bool& get_reserve_elapsed_time_history(void) const method

/// This method returns true if the elapsed time history vector is to be reserved, and false otherwise.

const bool& GradientDescent::get_reserve_elapsed_time_history(void) const
{
   return(reserve_elapsed_time_history);     
}


// const bool& get_reserve_generalization_evaluation_history(void) const method

/// This method returns true if the Generalization evaluation history vector is to be reserved, and false otherwise.

const bool& GradientDescent::get_reserve_generalization_evaluation_history(void) const
{
   return(reserve_generalization_evaluation_history);
}


// const unsigned int& get_display_period(void) const method

/// This method returns the number of epochs between the training showing progress. 

const unsigned int& GradientDescent::get_display_period(void) const
{
   return(display_period);
}


// void set_training_rate_algorithm(const TrainingRateAlgorithm&) method

/// This method sets a new training rate algorithm object into the gradient descent object.
/// @param new_training_rate_algorithm Object of the class TrainingRateAlgorithm

void GradientDescent::set_training_rate_algorithm(const TrainingRateAlgorithm& new_training_rate_algorithm)
{
   training_rate_algorithm = new_training_rate_algorithm;
}


// void set_default(void) method

void GradientDescent::set_default(void)
{
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

   reserve_training_direction_history = false;
   reserve_training_rate_history = false;
   reserve_elapsed_time_history = false;

   // UTILITIES

   display = true;
   display_period = 100;
}


// void set_reserve_all_training_history(bool) method

/// This method makes the training history of all variables to reseved or not in memory:
/// <ul>
/// <li> Parameters.
/// <li> Parameters norm.
/// <li> Performance.
/// <li> Gradient. 
/// <li> Gradient norm. 
/// <li> Generalization performance. 
/// <li> Training direction.
/// <li> Training direction norm. 
/// <li> Training rate.
/// </ul>
/// @param new_reserve_all_training_history True if the training history of all variables is to be reserved, false otherwise.

void GradientDescent::set_reserve_all_training_history(const bool& new_reserve_all_training_history)
{
   // Multilayer perceptron

   reserve_parameters_history = new_reserve_all_training_history;
   reserve_parameters_norm_history = new_reserve_all_training_history;
   
   // Performance functional

   reserve_evaluation_history = new_reserve_all_training_history;
   reserve_gradient_history = new_reserve_all_training_history;
   reserve_gradient_norm_history = new_reserve_all_training_history;

   reserve_generalization_evaluation_history = new_reserve_all_training_history;

   // Training algorithm

   reserve_training_direction_history = new_reserve_all_training_history;
   reserve_training_rate_history = new_reserve_all_training_history;

   reserve_elapsed_time_history = new_reserve_all_training_history;
}


// void set_warning_parameters_norm(const double&) method

/// This method sets a new value for the parameters vector norm at which a warning message is written to the 
/// screen. 
/// @param new_warning_parameters_norm Warning norm of parameters vector value. 

void GradientDescent::set_warning_parameters_norm(const double& new_warning_parameters_norm)
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

void GradientDescent::set_warning_gradient_norm(const double& new_warning_gradient_norm)
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


// void set_warning_training_rate(const double&) method

/// This method sets a new training rate value at wich a warning message is written to the screen during line 
/// minimization.
/// @param new_warning_training_rate Warning training rate value.

void GradientDescent::set_warning_training_rate(const double& new_warning_training_rate)
{
   // Control sentence (if debug)

   #ifdef _DEBUG 

   if(new_warning_training_rate < 0.0)
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: TrainingAlgorithm class.\n" 
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

void GradientDescent::set_error_parameters_norm(const double& new_error_parameters_norm)
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

void GradientDescent::set_error_gradient_norm(const double& new_error_gradient_norm)
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


// void set_error_training_rate(const double&) method

/// This method sets a new training rate value at wich a the line minimization algorithm is assumed to fail when 
/// bracketing a minimum.
/// @param new_error_training_rate Error training rate value.

void GradientDescent::set_error_training_rate(const double& new_error_training_rate)
{
   // Control sentence (if debug)

   #ifdef _DEBUG 

   if(new_error_training_rate < 0.0)
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: TrainingAlgorithm class.\n"
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

void GradientDescent::set_minimum_parameters_increment_norm(const double& new_minimum_parameters_increment_norm)
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

void GradientDescent::set_minimum_performance_increase(const double& new_minimum_performance_increase)
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

void GradientDescent::set_performance_goal(const double& new_performance_goal)
{
   performance_goal = new_performance_goal;
}


// void set_gradient_norm_goal(const double&) method

/// This method sets a new the goal value for the norm of the objective function gradient. 
/// This is used as a stopping criterium when training a multilayer perceptron
/// @param new_gradient_norm_goal Goal value for the norm of the objective function gradient.

void GradientDescent::set_gradient_norm_goal(const double& new_gradient_norm_goal)
{
   // Control sentence (if debug)

   #ifdef _DEBUG 

   if(new_gradient_norm_goal < 0.0)
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: GradientDescent class.\n"
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

void GradientDescent::set_maximum_generalization_evaluation_decreases(const unsigned int& new_maximum_generalization_evaluation_decreases)
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

void GradientDescent::set_maximum_epochs_number(const unsigned int& new_maximum_epochs_number)
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

void GradientDescent::set_maximum_time(const double& new_maximum_time)
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

void GradientDescent::set_reserve_parameters_history(const bool& new_reserve_parameters_history)
{
   reserve_parameters_history = new_reserve_parameters_history;     
}


// void set_reserve_parameters_norm_history(bool) method

/// This method makes the parameters norm history vector to be reseved or not in memory.
/// @param new_reserve_parameters_norm_history True if the parameters norm history vector is to be reserved, false otherwise.

void GradientDescent::set_reserve_parameters_norm_history(const bool& new_reserve_parameters_norm_history)
{
   reserve_parameters_norm_history = new_reserve_parameters_norm_history;     
}


// void set_reserve_evaluation_history(bool) method

/// This method makes the evaluation history vector to be reseved or not in memory.
/// @param new_reserve_evaluation_history True if the evaluation history vector is to be reserved, false otherwise.

void GradientDescent::set_reserve_evaluation_history(const bool& new_reserve_evaluation_history)
{
   reserve_evaluation_history = new_reserve_evaluation_history;     
}


// void set_reserve_gradient_history(bool) method

/// This method makes the gradient history vector of vectors to be reseved or not in memory.
/// @param new_reserve_gradient_history True if the gradient history matrix is to be reserved, false otherwise.

void GradientDescent::set_reserve_gradient_history(const bool& new_reserve_gradient_history)
{
   reserve_gradient_history = new_reserve_gradient_history;    
}


// void set_reserve_gradient_norm_history(bool) method

/// This method makes the gradient norm history vector to be reseved or not in memory.
/// @param new_reserve_gradient_norm_history True if the gradient norm history matrix is to be reserved, false 
/// otherwise.

void GradientDescent::set_reserve_gradient_norm_history(const bool& new_reserve_gradient_norm_history)
{
   reserve_gradient_norm_history = new_reserve_gradient_norm_history;     
}


// void set_reserve_training_direction_history(bool) method

/// This method makes the training direction history vector of vectors to be reseved or not in memory.
/// @param new_reserve_training_direction_history True if the training direction history matrix is to be reserved, 
/// false otherwise.

void GradientDescent::set_reserve_training_direction_history(const bool& new_reserve_training_direction_history)
{
   reserve_training_direction_history = new_reserve_training_direction_history;          
}


// void set_reserve_training_rate_history(bool) method

/// This method makes the training rate history vector to be reseved or not in memory.
/// @param new_reserve_training_rate_history True if the training rate history vector is to be reserved, false 
/// otherwise.

void GradientDescent::set_reserve_training_rate_history(const bool& new_reserve_training_rate_history)
{
   reserve_training_rate_history = new_reserve_training_rate_history;          
}


// void set_reserve_elapsed_time_history(bool) method

/// This method makes the elapsed time over the epochs to be reseved or not in memory. This is a vector.
/// @param new_reserve_elapsed_time_history True if the elapsed time history vector is to be reserved, false 
/// otherwise.

void GradientDescent::set_reserve_elapsed_time_history(const bool& new_reserve_elapsed_time_history)
{
   reserve_elapsed_time_history = new_reserve_elapsed_time_history;     
}


// void set_reserve_generalization_evaluation_history(bool) method

/// This method makes the Generalization evaluation history to be reserved or not in memory. 
/// This is a vector. 
/// @param new_reserve_generalization_evaluation_history True if the Generalization evaluation history is to be reserved, false otherwise. 

void GradientDescent::set_reserve_generalization_evaluation_history(const bool& new_reserve_generalization_evaluation_history)  
{
   reserve_generalization_evaluation_history = new_reserve_generalization_evaluation_history;
}


// void set_display_period(unsigned int) method

/// This method sets a new number of epochs between the training showing progress. 
/// @param new_display_period
/// Number of epochs between the training showing progress. 

void GradientDescent::set_display_period(const unsigned int& new_display_period)
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



// Vector<double> calculate_training_direction(const Vector<double>&) const method

/// This method returns the gradient descent training direction, which is the negative of the normalized gradient. 
//@todo

Vector<double> GradientDescent::calculate_training_direction(const Vector<double>& gradient) const
{
   const double gradient_norm = gradient.calculate_norm();

   return(gradient*(-1.0)/gradient_norm);
}


// std::string GradientDescentResults::to_string(void) const method

std::string GradientDescent::GradientDescentResults::to_string(void) const
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


// void GradientDescentResults::resize_training_history(const unsigned int& new_size) method

/// This method resizes all the training history variables. 
/// @param new_size Size of training history variables. 

void GradientDescent::GradientDescentResults::resize_training_history(const unsigned int& new_size)
{
   parameters_history.resize(new_size);
   parameters_norm_history.resize(new_size);

   evaluation_history.resize(new_size);
   generalization_evaluation_history.resize(new_size);
   gradient_history.resize(new_size);
   gradient_norm_history.resize(new_size);
   
   training_direction_history.resize(new_size);
   training_rate_history.resize(new_size);
   elapsed_time_history.resize(new_size);
}


// GradientDescentResults* perform_training(void) method

/// @todo

GradientDescent::GradientDescentResults* GradientDescent::perform_training(void)
{
   GradientDescentResults* training_results_pointer = new GradientDescentResults; 

   // Control sentence (if debug)

   #ifdef _DEBUG 

   check();

   #endif

   // Start training 

   if(display)
   {
      std::cout << "Training with gradient descent...\n";
   }

   // Neural network stuff

   NeuralNetwork* neural_network_pointer = performance_functional_pointer->get_neural_network_pointer();

   unsigned int parameters_number = neural_network_pointer->count_parameters_number();

   Vector<double> parameters(parameters_number);
   double parameters_norm;

   Vector<double> parameters_increment(parameters_number);
   double parameters_increment_norm;

   // Performance functional stuff

   double generalization_evaluation = 0.0; 
   double old_generalization_evaluation = 0.0;
   double generalization_evaluation_increment = 0.0;
      
   double performance = 0.0;
   double old_performance = 0.0;
   double performance_increase = 0.0;

   Vector<double> gradient(parameters_number);
   double gradient_norm;

   PerformanceFunctional::FirstOrderEvaluation first_order_evaluation;

   // Training algorithm stuff 

   unsigned int generalization_evaluation_decreases_count = 0;

   Vector<double> training_direction(parameters_number);

   double initial_training_rate = 0.0;
   double training_rate = 0.0;
   double old_training_rate = 0.0;

   Vector<double> directional_point(2);
   directional_point[0] = 0.0;
   directional_point[1] = 0.0;

   bool stop_training = false;

   time_t beginning_time, current_time;
   time(&beginning_time);
   double elapsed_time;

   training_results_pointer->resize_training_history(maximum_epochs_number+1);    
   
   // Main loop

   for(unsigned int epoch = 0; epoch <= maximum_epochs_number; epoch++)
   {
      // Neural network stuff

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

      generalization_evaluation = performance_functional_pointer->calculate_generalization_evaluation();

	  if(epoch != 0 && generalization_evaluation > old_generalization_evaluation)
	  {
	     generalization_evaluation_decreases_count++;	  
	  }

      // Training algorithm 

      training_direction = calculate_training_direction(gradient);
                        
      if(epoch == 0)
      {
         initial_training_rate = training_rate_algorithm.get_first_training_rate();
      }
      else
      {
         initial_training_rate = old_training_rate;
      }    
      
	  directional_point = training_rate_algorithm.calculate_directional_point(performance, training_direction, initial_training_rate);

      training_rate = directional_point[0];

      parameters_increment = training_direction*training_rate;
      parameters_increment_norm = parameters_increment.calculate_norm();
      
      // Elapsed time

      time(&current_time);
      elapsed_time = difftime(current_time, beginning_time);

      // Training history multilayer perceptron 

      if(reserve_parameters_history)
      {
         training_results_pointer->parameters_history[epoch] = parameters;
      }

      if(reserve_parameters_norm_history)
      {
         training_results_pointer->parameters_norm_history[epoch] = parameters_norm; 
      }

      // Training history performance functional

      if(reserve_evaluation_history)
      {
         training_results_pointer->evaluation_history[epoch] = performance;
      }

      if(reserve_gradient_history)
      {
         training_results_pointer->gradient_history[epoch] = gradient;                                
      }

      if(reserve_gradient_norm_history)
      {
         training_results_pointer->gradient_norm_history[epoch] = gradient_norm;
      }

      if(reserve_generalization_evaluation_history)
      {
         training_results_pointer->generalization_evaluation_history[epoch] = generalization_evaluation;
      }

      // Training history training algorithm

      if(reserve_training_direction_history)
      {
         training_results_pointer->training_direction_history[epoch] = training_direction;                                
      }

      if(reserve_training_rate_history)
      {
         training_results_pointer->training_rate_history[epoch] = training_rate;
      }

      if(reserve_elapsed_time_history)
      {
         training_results_pointer->elapsed_time_history[epoch] = elapsed_time;
      }

      // Stopping Criteria

      if(parameters_increment_norm <= minimum_parameters_increment_norm)
      {
         if(display)
         {
            std::cout << "Epoch " << epoch << ": Minimum parameters increment norm reached.\n";
            std::cout << "Parameters increment norm: " << parameters_increment_norm << std::endl;
         }

         stop_training = true;
      }

      else if(epoch != 0 && performance_increase <= minimum_performance_increase)
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

      else if(epoch != 0 && generalization_evaluation_increment > 0.0)
      {
         if(display)
         {
            std::cout << "Epoch " << epoch << ": Validation performance stopped improving.\n";
            std::cout << "Validation performance increment: " << generalization_evaluation_increment << std::endl;
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
         if(display)
		 {
            std::cout << "Parameters norm: " << parameters_norm << "\n"
                      << "Performance: " << performance << "\n"
                      << "Gradient norm: " << gradient_norm << "\n"
			          << performance_functional_pointer->write_information() 
                      << "Training rate: " << training_rate << "\n"
                      << "Elapsed time: " << elapsed_time << std::endl; 

            if(generalization_evaluation != 0)
            {
               std::cout << "Validation performance: " << generalization_evaluation << std::endl;
            }

		 }
    
         training_results_pointer->resize_training_history(1+epoch);

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
            std::cout << "Validation performance: " << generalization_evaluation << std::endl;
         }

      }

      // Set new parameters

      parameters += parameters_increment;

      neural_network_pointer->set_parameters(parameters);

      // Update stuff

      old_performance = performance;
      old_generalization_evaluation = generalization_evaluation;
   
      old_training_rate = training_rate;
   } 

   return(training_results_pointer);
}


// std::string write_training_algorithm_type(void) const method

std::string GradientDescent::write_training_algorithm_type(void) const
{
   return("GRADIENT_DESCENT");
}


// TiXmlElement* to_XML(void) const method

/// This method prints to the screen the training parameters, the stopping criteria and other user stuff 
/// concerning the gradient descent object.

TiXmlElement* GradientDescent::to_XML(void) const
{
   std::ostringstream buffer;

   // Training algorithm

   TiXmlElement* gradient_descent_element = new TiXmlElement("GradientDescent");
   gradient_descent_element->SetAttribute("Version", 4); 


   // Warning parameters norm

   TiXmlElement* warning_parameters_norm_element = new TiXmlElement("WarningParametersNorm");
   gradient_descent_element->LinkEndChild(warning_parameters_norm_element);

   buffer.str("");
   buffer << warning_parameters_norm;

   TiXmlText* warning_parameters_norm_text = new TiXmlText(buffer.str().c_str());
   warning_parameters_norm_element->LinkEndChild(warning_parameters_norm_text);

   // Warning gradient norm 

   TiXmlElement* warning_gradient_norm_element = new TiXmlElement("WarningGradientNorm");
   gradient_descent_element->LinkEndChild(warning_gradient_norm_element);

   buffer.str("");
   buffer << warning_gradient_norm;

   TiXmlText* warning_gradient_norm_text = new TiXmlText(buffer.str().c_str());
   warning_gradient_norm_element->LinkEndChild(warning_gradient_norm_text);

   // Warning training rate 

   TiXmlElement* warning_training_rate_element = new TiXmlElement("WarningTrainingRate");
   gradient_descent_element->LinkEndChild(warning_training_rate_element);

   buffer.str("");
   buffer << warning_training_rate;

   TiXmlText* warning_training_rate_text = new TiXmlText(buffer.str().c_str());
   warning_training_rate_element->LinkEndChild(warning_training_rate_text);

   // Error parameters norm

   TiXmlElement* error_parameters_norm_element = new TiXmlElement("ErrorParametersNorm");
   gradient_descent_element->LinkEndChild(error_parameters_norm_element);

   buffer.str("");
   buffer << error_parameters_norm;

   TiXmlText* error_parameters_norm_text = new TiXmlText(buffer.str().c_str());
   error_parameters_norm_element->LinkEndChild(error_parameters_norm_text);

   // Error gradient norm 

   TiXmlElement* error_gradient_norm_element = new TiXmlElement("ErrorGradientNorm");
   gradient_descent_element->LinkEndChild(error_gradient_norm_element);

   buffer.str("");
   buffer << error_gradient_norm;

   TiXmlText* error_gradient_norm_text = new TiXmlText(buffer.str().c_str());
   error_gradient_norm_element->LinkEndChild(error_gradient_norm_text);

   // Error training rate

   TiXmlElement* error_training_rate_element = new TiXmlElement("ErrorTrainingRate");
   gradient_descent_element->LinkEndChild(error_training_rate_element);

   buffer.str("");
   buffer << error_training_rate;

   TiXmlText* error_training_rate_text = new TiXmlText(buffer.str().c_str());
   error_training_rate_element->LinkEndChild(error_training_rate_text);

   // Minimum parameters increment norm

   TiXmlElement* minimum_parameters_increment_norm_element = new TiXmlElement("MinimumParametersIncrement");
   gradient_descent_element->LinkEndChild(minimum_parameters_increment_norm_element);

   buffer.str("");
   buffer << minimum_parameters_increment_norm;

   TiXmlText* minimum_parameters_increment_norm_text = new TiXmlText(buffer.str().c_str());
   minimum_parameters_increment_norm_element->LinkEndChild(minimum_parameters_increment_norm_text);

   // Minimum performance increase 

   TiXmlElement* minimum_performance_increase_element = new TiXmlElement("MinimumPerformanceIncrease");
   gradient_descent_element->LinkEndChild(minimum_performance_increase_element);

   buffer.str("");
   buffer << minimum_performance_increase;

   TiXmlText* minimum_performance_increase_text = new TiXmlText(buffer.str().c_str());
   minimum_performance_increase_element->LinkEndChild(minimum_performance_increase_text);

   // Performance goal 

   TiXmlElement* performance_goal_element = new TiXmlElement("PerformanceGoal");
   gradient_descent_element->LinkEndChild(performance_goal_element);

   buffer.str("");
   buffer << performance_goal;

   TiXmlText* performance_goal_text = new TiXmlText(buffer.str().c_str());
   performance_goal_element->LinkEndChild(performance_goal_text);

   // Gradient norm goal 

   TiXmlElement* gradient_norm_goal_element = new TiXmlElement("GradientNormGoal");
   gradient_descent_element->LinkEndChild(gradient_norm_goal_element);

   buffer.str("");
   buffer << gradient_norm_goal;

   TiXmlText* gradient_norm_goal_text = new TiXmlText(buffer.str().c_str());
   gradient_norm_goal_element->LinkEndChild(gradient_norm_goal_text);

   // Maximum generalization performance decreases

   TiXmlElement* maximum_generalization_evaluation_decreases_element = new TiXmlElement("MaximumGeneralizationEvaluationDecreases");
   gradient_descent_element->LinkEndChild(maximum_generalization_evaluation_decreases_element);

   buffer.str("");
   buffer << maximum_generalization_evaluation_decreases;

   TiXmlText* maximum_generalization_evaluation_decreases_text = new TiXmlText(buffer.str().c_str());
   maximum_generalization_evaluation_decreases_element->LinkEndChild(maximum_generalization_evaluation_decreases_text);

   // Maximum epochs number 

   TiXmlElement* maximum_epochs_number_element = new TiXmlElement("MaximumEpochsNumber");
   gradient_descent_element->LinkEndChild(maximum_epochs_number_element);

   buffer.str("");
   buffer << maximum_epochs_number;

   TiXmlText* maximum_epochs_number_text = new TiXmlText(buffer.str().c_str());
   maximum_epochs_number_element->LinkEndChild(maximum_epochs_number_text);

   // Maximum time 

   TiXmlElement* maximum_time_element = new TiXmlElement("MaximumTime");
   gradient_descent_element->LinkEndChild(maximum_time_element);

   buffer.str("");
   buffer << maximum_time;

   TiXmlText* maximum_time_text = new TiXmlText(buffer.str().c_str());
   maximum_time_element->LinkEndChild(maximum_time_text);

   // Reserve parameters history 

   TiXmlElement* reserve_parameters_history_element = new TiXmlElement("ReserveParametersHistory");
   gradient_descent_element->LinkEndChild(reserve_parameters_history_element);

   buffer.str("");
   buffer << reserve_parameters_history;

   TiXmlText* reserve_parameters_history_text = new TiXmlText(buffer.str().c_str());
   reserve_parameters_history_element->LinkEndChild(reserve_parameters_history_text);

   // Reserve parameters norm history 

   TiXmlElement* reserve_parameters_norm_history_element = new TiXmlElement("ReserveParametersNormHistory");
   gradient_descent_element->LinkEndChild(reserve_parameters_norm_history_element);

   buffer.str("");
   buffer << reserve_parameters_norm_history;

   TiXmlText* reserve_parameters_norm_history_text = new TiXmlText(buffer.str().c_str());
   reserve_parameters_norm_history_element->LinkEndChild(reserve_parameters_norm_history_text);

   // Reserve evaluation history 

   TiXmlElement* reserve_evaluation_history_element = new TiXmlElement("ReservePerformanceHistory");
   gradient_descent_element->LinkEndChild(reserve_evaluation_history_element);

   buffer.str("");
   buffer << reserve_evaluation_history;

   TiXmlText* reserve_evaluation_history_text = new TiXmlText(buffer.str().c_str());
   reserve_evaluation_history_element->LinkEndChild(reserve_evaluation_history_text);

   // Reserve gradient history 

   TiXmlElement* reserve_gradient_history_element = new TiXmlElement("ReserveGradientHistory");
   gradient_descent_element->LinkEndChild(reserve_gradient_history_element);

   buffer.str("");
   buffer << reserve_gradient_history;

   TiXmlText* reserve_gradient_history_text = new TiXmlText(buffer.str().c_str());
   reserve_gradient_history_element->LinkEndChild(reserve_gradient_history_text);

   // Reserve gradient norm history 

   TiXmlElement* reserve_gradient_norm_history_element = new TiXmlElement("ReserveGradientNormHistory");
   gradient_descent_element->LinkEndChild(reserve_gradient_norm_history_element);

   buffer.str("");
   buffer << reserve_gradient_norm_history;

   TiXmlText* reserve_gradient_norm_history_text = new TiXmlText(buffer.str().c_str());
   reserve_gradient_norm_history_element->LinkEndChild(reserve_gradient_norm_history_text);

   // Reserve training direction history 

   TiXmlElement* reserve_training_direction_history_element = new TiXmlElement("ReserveTrainingDirectionHistory");
   gradient_descent_element->LinkEndChild(reserve_training_direction_history_element);

   buffer.str("");
   buffer << reserve_training_direction_history;

   TiXmlText* reserve_training_direction_history_text = new TiXmlText(buffer.str().c_str());
   reserve_training_direction_history_element->LinkEndChild(reserve_training_direction_history_text);

   // Reserve training rate history 

   TiXmlElement* reserve_training_rate_history_element = new TiXmlElement("ReserveTrainingRateHistory");
   gradient_descent_element->LinkEndChild(reserve_training_rate_history_element);

   buffer.str("");
   buffer << reserve_training_rate_history;

   TiXmlText* reserve_training_rate_history_text = new TiXmlText(buffer.str().c_str());
   reserve_training_rate_history_element->LinkEndChild(reserve_training_rate_history_text);

   // Reserve elapsed time history 

   TiXmlElement* reserve_elapsed_time_history_element = new TiXmlElement("ReserveElapsedTimeHistory");
   gradient_descent_element->LinkEndChild(reserve_elapsed_time_history_element);

   buffer.str("");
   buffer << reserve_elapsed_time_history;

   TiXmlText* reserve_elapsed_time_history_text = new TiXmlText(buffer.str().c_str());
   reserve_elapsed_time_history_element->LinkEndChild(reserve_elapsed_time_history_text);

   // Reserve generalization evaluation history 

   TiXmlElement* reserve_generalization_evaluation_history_element = new TiXmlElement("ReserveGeneralizationPerformanceHistory");
   gradient_descent_element->LinkEndChild(reserve_generalization_evaluation_history_element);

   buffer.str("");
   buffer << reserve_generalization_evaluation_history;

   TiXmlText* reserve_generalization_evaluation_history_text = new TiXmlText(buffer.str().c_str());
   reserve_generalization_evaluation_history_element->LinkEndChild(reserve_generalization_evaluation_history_text);

   // Display period

   TiXmlElement* display_period_element = new TiXmlElement("DisplayPeriod");
   gradient_descent_element->LinkEndChild(display_period_element);

   buffer.str("");
   buffer << display_period;

   TiXmlText* display_period_text = new TiXmlText(buffer.str().c_str());
   display_period_element->LinkEndChild(display_period_text);

   // Display warnings 

   TiXmlElement* display_element = new TiXmlElement("Display");
   gradient_descent_element->LinkEndChild(display_element);

   buffer.str("");
   buffer << display;

   TiXmlText* display_text = new TiXmlText(buffer.str().c_str());
   display_element->LinkEndChild(display_text);

   return(gradient_descent_element);
}


// void from_XML(TiXmlElement*) method

void GradientDescent::from_XML(TiXmlElement* gradient_descent_element)
{
   // Training rate algorithm

   TiXmlElement* training_rate_algorithm_element = gradient_descent_element->FirstChildElement("TrainingRateAlgorithm");
   
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

   TiXmlElement* warning_parameters_norm_element = gradient_descent_element->FirstChildElement("WarningParametersNorm");

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

   TiXmlElement* warning_gradient_norm_element = gradient_descent_element->FirstChildElement("WarningGradientNorm");

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

   TiXmlElement* warning_training_rate_element = gradient_descent_element->FirstChildElement("WarningTrainingRate");

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

   TiXmlElement* error_parameters_norm_element = gradient_descent_element->FirstChildElement("ErrorParametersNorm");

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

   TiXmlElement* error_gradient_norm_element = gradient_descent_element->FirstChildElement("ErrorGradientNorm");

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

   TiXmlElement* error_training_rate_element = gradient_descent_element->FirstChildElement("ErrorTrainingRate");

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

   TiXmlElement* minimum_parameters_increment_norm_element = gradient_descent_element->FirstChildElement("MinimumParametersIncrementNorm");

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

   TiXmlElement* minimum_performance_increase_element = gradient_descent_element->FirstChildElement("MinimumPerformanceIncrease");

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

   TiXmlElement* performance_goal_element = gradient_descent_element->FirstChildElement("PerformanceGoal");

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

   TiXmlElement* gradient_norm_goal_element = gradient_descent_element->FirstChildElement("GradientNormGoal");

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

   TiXmlElement* maximum_generalization_evaluation_decreases_element = gradient_descent_element->FirstChildElement("MaximumGeneralizationEvaluationDecreases");

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

   TiXmlElement* maximum_epochs_number_element = gradient_descent_element->FirstChildElement("MaximumEpochsNumber");

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

   TiXmlElement* maximum_time_element = gradient_descent_element->FirstChildElement("MaximumTime");

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

   TiXmlElement* reserve_parameters_history_element = gradient_descent_element->FirstChildElement("ReserveParametersHistory");

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

   TiXmlElement* reserve_parameters_norm_history_element = gradient_descent_element->FirstChildElement("ReserveParametersNormHistory");

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

   TiXmlElement* reserve_evaluation_history_element = gradient_descent_element->FirstChildElement("ReservePerformanceHistory");

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

   TiXmlElement* reserve_gradient_history_element = gradient_descent_element->FirstChildElement("ReserveGradientHistory");

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

   TiXmlElement* reserve_gradient_norm_history_element = gradient_descent_element->FirstChildElement("ReserveGradientNormHistory");

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

   // Reserve training direction history 

   TiXmlElement* reserve_training_direction_history_element = gradient_descent_element->FirstChildElement("ReserveTrainingDirectionHistory");

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

   TiXmlElement* reserve_training_rate_history_element = gradient_descent_element->FirstChildElement("ReserveTrainingRateHistory");

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

   TiXmlElement* reserve_elapsed_time_history_element = gradient_descent_element->FirstChildElement("ReserveElapsedTimeHistory");

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

   TiXmlElement* reserve_generalization_evaluation_history_element = gradient_descent_element->FirstChildElement("ReserveGeneralizationPerformanceHistory");

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

   TiXmlElement* display_period_element = gradient_descent_element->FirstChildElement("DisplayPeriod");

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

   TiXmlElement* display_element = gradient_descent_element->FirstChildElement("Display");

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

