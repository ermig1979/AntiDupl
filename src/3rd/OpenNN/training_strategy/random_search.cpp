/****************************************************************************************************************/
/*                                                                                                              */
/*   OpenNN: Open Neural Networks Library                                                                       */
/*   www.opennn.cimne.com                                                                                       */
/*                                                                                                              */
/*   R A N D O M   S E A R C H   C L A S S                                                                      */
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
#include <cmath>
#include <ctime>

// OpenNN includes

#include "random_search.h"

namespace OpenNN
{

// DEFAULT CONSTRUCTOR

/// Default constructor. 
/// It creates a random search training algorithm not associated to any performance functional object. 
/// It also initializes the class members to their default values.

RandomSearch::RandomSearch(void) 
 : TrainingAlgorithm()
{
   set_default();
}


// PERFORMANCE FUNCTIONAL CONSTRUCTOR 

/// Performance functional constructor. 
/// It creates a random search training algorithm associated to a performance functional object. 
/// It also initializes the class members to their default values.
/// @param new_performance_functional_pointer Pointer to a performance functional object.

RandomSearch::RandomSearch(PerformanceFunctional* new_performance_functional_pointer)
: TrainingAlgorithm(new_performance_functional_pointer)
{   
   set_default();
}


// XML CONSTRUCTOR 

/// XML constructor. 
/// It creates a random search training algorithm not associated to any performance functional object. 
/// It also loads the rest of class members from a XML element.
/// @param random_search_element TinyXML element containing the members of a random search object. 

RandomSearch::RandomSearch(TiXmlElement* random_search_element) : TrainingAlgorithm(random_search_element)
{
   from_XML(random_search_element);
}


// DESTRUCTOR

/// Destructor.
/// It does not delete any object. 

RandomSearch::~RandomSearch(void)
{

}


// const double& get_warning_parameters_norm(void) const method

/// This method returns the minimum value for the norm of the parameters vector at wich a warning message is written to the screen. 

const double& RandomSearch::get_warning_parameters_norm(void) const
{
   return(warning_parameters_norm);       
}


// const double& get_warning_training_rate(void) const method

/// This method returns the training rate value at wich a warning message is written to the screen during line minimization.

const double& RandomSearch::get_warning_training_rate(void) const
{
   return(warning_training_rate);
}


// const double& get_error_parameters_norm(void) const method

/// This method returns the value for the norm of the parameters vector at wich an error message is 
/// written to the screen and the program exits. 

const double& RandomSearch::get_error_parameters_norm(void) const
{
   return(error_parameters_norm);
}


// const double& get_error_training_rate(void) const method

/// This method returns the training rate value at wich the line minimization algorithm is assumed to fail when 
/// bracketing a minimum.

const double& RandomSearch::get_error_training_rate(void) const
{
   return(error_training_rate);
}


// const double& get_minimum_parameters_increment_norm(void) const method

/// This method returns the minimum norm of the parameter increment vector used as a stopping criteria when training. 

const double& RandomSearch::get_minimum_parameters_increment_norm(void) const
{
   return(minimum_parameters_increment_norm);
}


// const double& get_minimum_performance_increase(void) const method

/// This method returns the minimum performance improvement during training.  

const double& RandomSearch::get_minimum_performance_increase(void) const
{
   return(minimum_performance_increase);
}


// const double& get_performance_goal(void) const method

/// This method returns the goal value for the performance. 
/// This is used as a stopping criterium when training a multilayer perceptron

const double& RandomSearch::get_performance_goal(void) const
{
   return(performance_goal);
}


// const unsigned int& get_maximum_generalization_evaluation_decreases(void) const method

/// This method returns the maximum number of generalization failures during the training process. 

const unsigned int& RandomSearch::get_maximum_generalization_evaluation_decreases(void) const
{
   return(maximum_generalization_evaluation_decreases);
}


// const unsigned int& get_maximum_epochs_number(void) const method

/// This method returns the maximum number of epochs for training.

const unsigned int& RandomSearch::get_maximum_epochs_number(void) const
{
   return(maximum_epochs_number);
}


// const double& get_maximum_time(void) const method

/// This method returns the maximum training time.  

const double& RandomSearch::get_maximum_time(void) const
{
   return(maximum_time);
}


// const bool& get_reserve_parameters_history(void) const method

/// This method returns true if the parameters history matrix is to be reserved, and false otherwise.

const bool& RandomSearch::get_reserve_parameters_history(void) const
{
   return(reserve_parameters_history);     
}


// const bool& get_reserve_parameters_norm_history(void) const method 

/// This method returns true if the parameters norm history vector is to be reserved, and false otherwise.

const bool& RandomSearch::get_reserve_parameters_norm_history(void) const
{
   return(reserve_parameters_norm_history);     
}


// const bool& get_reserve_evaluation_history(void) const method

/// This method returns true if the evaluation history vector is to be reserved, and false otherwise.

const bool& RandomSearch::get_reserve_evaluation_history(void) const
{
   return(reserve_evaluation_history);     
}


// const bool& get_reserve_training_direction_history(void) const method

/// This method returns true if the training direction history matrix is to be reserved, and false otherwise.

const bool& RandomSearch::get_reserve_training_direction_history(void) const
{
   return(reserve_training_direction_history);     
}


// const bool& get_reserve_training_rate_history(void) const method

/// This method returns true if the training rate history vector is to be reserved, and false otherwise.

const bool& RandomSearch::get_reserve_training_rate_history(void) const
{
   return(reserve_training_rate_history);     
}


// const bool& get_reserve_elapsed_time_history(void) const method

/// This method returns true if the elapsed time history vector is to be reserved, and false otherwise.

const bool& RandomSearch::get_reserve_elapsed_time_history(void) const
{
   return(reserve_elapsed_time_history);     
}


// const bool& get_reserve_generalization_evaluation_history(void) const method

/// This method returns true if the Generalization evaluation history vector is to be reserved, and false otherwise.

const bool& RandomSearch::get_reserve_generalization_evaluation_history(void) const
{
   return(reserve_generalization_evaluation_history);
}


// const unsigned int& get_display_period(void) const method

/// This method returns the number of epochs between the training showing progress. 

const unsigned int& RandomSearch::get_display_period(void) const
{
   return(display_period);
}


// const double& get_training_rate_reduction_factor(void) const method

/// This method returns the reducing factor for the training rate. 

const double& RandomSearch::get_training_rate_reduction_factor(void) const
{
   return(training_rate_reduction_factor);
}


// const unsigned int& get_training_rate_reduction_period(void) const method

/// This method returns the reducing period for the training rate. 

const unsigned int& RandomSearch::get_training_rate_reduction_period(void) const
{
   return(training_rate_reduction_period);
}


// void set_default(void) method

/// This method sets all the random search object members to their default values:
/// <ul>
/// <li> Training rate reduction factor: 0.9
/// <li> Training rate reduction period: 10
/// <li> Warning parameters norm: 1.0e6
/// <li> Error parameters norm: 1.0e9
/// <li> Performance goal: -1.0e99
/// <li> Maximum time: 1.0e6
/// <li> Maximum epochs number: 100
/// <li> Reserve potential parameters history: False
/// <li> Reserve potential parameters norm history: False
/// <li> Reserve evaluation history: False.
/// <li> Display: True
/// <li> Display period: 10
/// </ul>

void RandomSearch::set_default(void)
{   
   // TRAINING PARAMETERS

   first_training_rate = 0.01;

   training_rate_reduction_factor = 0.9;
   training_rate_reduction_period = 10;

   // STOPPING CRITERIA

   minimum_performance_increase = 0.0;
   performance_goal = -1.0e99;

   maximum_epochs_number = 100;
   maximum_time = 1000.0;

   // TRAINING HISTORY

   reserve_parameters_history = false;
   reserve_parameters_norm_history = false;

   reserve_evaluation_history = true;

   reserve_training_direction_history = false;
   reserve_training_rate_history = false;
   reserve_elapsed_time_history = false;

   // UTILITIES

   display = true;
   display_period = 10;
}


// void set_training_rate_reduction_factor(const double&) method

/// This method sets a new value for the reduction factor of the training rate. 
/// @param new_training_rate_reduction_factor Reduction factor value.

void RandomSearch::set_training_rate_reduction_factor(const double& new_training_rate_reduction_factor)
{
   training_rate_reduction_factor = new_training_rate_reduction_factor;
}


// void set_training_rate_reduction_period(unsigned int) method

/// This method sets a new period value for the reduction of the training rate. This is measured in training epochs. 
/// @param new_training_rate_reduction_period Reduction period for the training rate.

void RandomSearch::set_training_rate_reduction_period(const unsigned int& new_training_rate_reduction_period)
{
   training_rate_reduction_period = new_training_rate_reduction_period;
}


// void set_reserve_parameters_history(bool) method

/// This method makes the potential parameters history vector of vectors to be reseved or not in memory.
/// @param new_reserve_parameters_history True if the potential parameters history is to be reserved, false otherwise.

void RandomSearch::set_reserve_parameters_history(const bool& new_reserve_parameters_history)
{
   reserve_parameters_history = new_reserve_parameters_history;
}


// void set_reserve_parameters_norm_history(bool) method

/// This method makes the potential parameters norm history vector to be reseved or not in memory.
/// @param new_reserve_parameters_norm_history True if the potential parameters norm history is to be reserved, 
/// false otherwise.

void RandomSearch::set_reserve_parameters_norm_history(const bool& new_reserve_parameters_norm_history)
{
   reserve_parameters_norm_history = new_reserve_parameters_norm_history;   
}


// void set_reserve_evaluation_history(bool) method

/// This method makes the potential evaluation history vector to be reseved or not in memory.
/// @param new_reserve_evaluation_history True if the potential evaluation history is to be reserved, 
/// false otherwise.

void RandomSearch::set_reserve_evaluation_history(const bool& new_reserve_evaluation_history)
{
   reserve_evaluation_history = new_reserve_evaluation_history;
}


// void set_reserve_all_training_history(bool) method

/// This method makes the training history of all variables to reseved or not in memory.
/// @param new_reserve_all_training_history True if the training history of all variables is to be reserved, 
/// false otherwise.

void RandomSearch::set_reserve_all_training_history(const bool& new_reserve_all_training_history)
{
   // Neural network

   reserve_parameters_history = new_reserve_all_training_history;
   reserve_parameters_norm_history = new_reserve_all_training_history;
   
   // Performance functional

   reserve_evaluation_history = new_reserve_all_training_history;
   reserve_evaluation_history = new_reserve_all_training_history;
  
   // Training algorithm

   reserve_elapsed_time_history = new_reserve_all_training_history;
}


// void set_warning_parameters_norm(const double&) method

/// This method sets a new value for the parameters vector norm at which a warning message is written to the 
/// screen. 
/// @param new_warning_parameters_norm Warning norm of parameters vector value. 

void RandomSearch::set_warning_parameters_norm(const double& new_warning_parameters_norm)
{
   // Control sentence (if debug)

   #ifdef _DEBUG 

   if(new_warning_parameters_norm < 0.0)
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: RandomSearch class.\n"
             << "void set_warning_parameters_norm(const double&) method.\n"
             << "Warning parameters norm must be equal or greater than 0.\n";

      throw std::logic_error(buffer.str().c_str());	  
   }

   #endif

   // Set warning parameters norm

   warning_parameters_norm = new_warning_parameters_norm;     
}



// void set_warning_training_rate(const double&) method

/// This method sets a new training rate value at wich a warning message is written to the screen during line 
/// minimization.
/// @param new_warning_training_rate Warning training rate value.

void RandomSearch::set_warning_training_rate(const double& new_warning_training_rate)
{
   // Control sentence (if debug)

   #ifdef _DEBUG 

   if(new_warning_training_rate < 0.0)
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: RandomSearch class.\n" 
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

void RandomSearch::set_error_parameters_norm(const double& new_error_parameters_norm)
{
   // Control sentence (if debug)

   #ifdef _DEBUG 

   if(new_error_parameters_norm < 0.0)
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: RandomSearch class.\n"
             << "void set_error_parameters_norm(const double&) method.\n"
             << "Error parameters norm must be equal or greater than 0.\n";

      throw std::logic_error(buffer.str().c_str());	  
   }

   #endif

   // Set error parameters norm

   error_parameters_norm = new_error_parameters_norm;
}


// void set_error_training_rate(const double&) method

/// This method sets a new training rate value at wich a the line minimization algorithm is assumed to fail when 
/// bracketing a minimum.
/// @param new_error_training_rate Error training rate value.

void RandomSearch::set_error_training_rate(const double& new_error_training_rate)
{
   // Control sentence (if debug)

   #ifdef _DEBUG 

   if(new_error_training_rate < 0.0)
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: RandomSearch class.\n"
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

void RandomSearch::set_minimum_parameters_increment_norm(const double& new_minimum_parameters_increment_norm)
{
   // Control sentence (if debug)

   #ifdef _DEBUG 

   if(new_minimum_parameters_increment_norm < 0.0)
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: RandomSearch class.\n"
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

void RandomSearch::set_minimum_performance_increase(const double& new_minimum_performance_increase)
{
   // Control sentence (if debug)

   #ifdef _DEBUG 

   if(new_minimum_performance_increase < 0.0)
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: RandomSearch class.\n"
             << "void set_minimum_performance_increase(const double&) method.\n"
             << "Minimum performance improvement must be equal or greater than 0.\n";

      throw std::logic_error(buffer.str().c_str());	  
   }

   #endif

   // Set minimum performance improvement

   minimum_performance_increase = new_minimum_performance_increase;
}


// void set_performance_goal(const doubleT) method

/// This method sets a new goal value for the performance. 
/// This is used as a stopping criterium when training a multilayer perceptron
/// @param new_performance_goal Goal value for the performance.

void RandomSearch::set_performance_goal(const double& new_performance_goal)
{
   performance_goal = new_performance_goal;
}


// void set_maximum_generalization_evaluation_decreases(const unsigned int&) method

/// This method sets a new maximum number of generalization failures. 
/// @param new_maximum_generalization_evaluation_decreases Maximum number of epochs in which the generalization evalutation decreases. 

void RandomSearch::set_maximum_generalization_evaluation_decreases(const unsigned int& new_maximum_generalization_evaluation_decreases)
{
   // Control sentence (if debug)

   #ifdef _DEBUG 

   if(new_maximum_generalization_evaluation_decreases < 0)
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: RandomSearch class.\n"
             << "void set_maximum_generalization_evaluation_decreases(const unsigned int&) method.\n"
             << "Number of generalization performance decreases must be equal or greater than 0.\n";

      throw std::logic_error(buffer.str().c_str());	  
   }

   #endif

   // Set maximum generalization performace decrases

   maximum_generalization_evaluation_decreases = new_maximum_generalization_evaluation_decreases;
}


// void set_maximum_epochs_number(const unsigned int&) method

/// This method sets a maximum number of epochs for training.
/// @param new_maximum_epochs_number Maximum number of epochs for training.

void RandomSearch::set_maximum_epochs_number(const unsigned int& new_maximum_epochs_number)
{
   // Control sentence (if debug)

   #ifdef _DEBUG 

   if(new_maximum_epochs_number < 0)
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: RandomSearch class.\n"
             << "void set_maximum_epochs_number(const unsigned int&) method.\n"
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

void RandomSearch::set_maximum_time(const double& new_maximum_time)
{
   // Control sentence (if debug)

   #ifdef _DEBUG 

   if(new_maximum_time < 0.0)
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: RandomSearch class.\n"
             << "void set_maximum_time(const double&) method.\n"
             << "Maximum time must be equal or greater than 0.\n";

      throw std::logic_error(buffer.str().c_str());	  
   }
   
   #endif

   // Set maximum time

   maximum_time = new_maximum_time;
}


// void set_reserve_training_direction_history(bool) method

/// This method makes the training direction history vector of vectors to be reseved or not in memory.
/// @param new_reserve_training_direction_history True if the training direction history matrix is to be reserved, 
/// false otherwise.

void RandomSearch::set_reserve_training_direction_history(const bool& new_reserve_training_direction_history)
{
   reserve_training_direction_history = new_reserve_training_direction_history;          
}


// void set_reserve_training_rate_history(bool) method

/// This method makes the training rate history vector to be reseved or not in memory.
/// @param new_reserve_training_rate_history True if the training rate history vector is to be reserved, false 
/// otherwise.

void RandomSearch::set_reserve_training_rate_history(const bool& new_reserve_training_rate_history)
{
   reserve_training_rate_history = new_reserve_training_rate_history;          
}


// void set_reserve_elapsed_time_history(bool) method

/// This method makes the elapsed time over the epochs to be reseved or not in memory. This is a vector.
/// @param new_reserve_elapsed_time_history True if the elapsed time history vector is to be reserved, false 
/// otherwise.

void RandomSearch::set_reserve_elapsed_time_history(const bool& new_reserve_elapsed_time_history)
{
   reserve_elapsed_time_history = new_reserve_elapsed_time_history;     
}


// void set_reserve_generalization_evaluation_history(bool) method

/// This method makes the Generalization evaluation history to be reserved or not in memory. 
/// This is a vector. 
/// @param new_reserve_generalization_evaluation_history True if the Generalization evaluation history is to be reserved, false otherwise. 

void RandomSearch::set_reserve_generalization_evaluation_history(const bool& new_reserve_generalization_evaluation_history)  
{
   reserve_generalization_evaluation_history = new_reserve_generalization_evaluation_history;
}


// void set_display_period(unsigned int) method

/// This method sets a new number of epochs between the training showing progress. 
/// @param new_display_period
/// Number of epochs between the training showing progress. 

void RandomSearch::set_display_period(const unsigned int& new_display_period)
{
   // Control sentence (if debug)

   #ifdef _DEBUG 
     
   if(new_display_period <= 0)
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: RandomSearch class.\n"
             << "void set_display_period(const double&) method.\n"
             << "First training rate must be greater than 0.\n";

      throw std::logic_error(buffer.str().c_str());	  
   }

   #endif

   display_period = new_display_period;
}



// Vector<double> calculate_training_direction(void) method

/// This method calculates a random vector to be used as training direction.

Vector<double> RandomSearch::calculate_training_direction(void)
{   
   const NeuralNetwork* neural_network_pointer = performance_functional_pointer->get_neural_network_pointer();

   unsigned int parameters_number = neural_network_pointer->count_parameters_number();

   Vector<double> random(parameters_number);
   double random_norm;

   do
   { 
      random.initialize_uniform();   
      random_norm = random.calculate_norm();             
   }while(random_norm > 1.0);

   return(random/random_norm);
}


// void resize_training_history(const unsigned int&) method

/// This method resizes all the training history variables. 
/// @param new_size Size of training history variables. 

void RandomSearch::RandomSearchResults::resize_training_history(const unsigned int& new_size)
{
   parameters_history.resize(new_size);
   parameters_norm_history.resize(new_size);

   evaluation_history.resize(new_size);
   generalization_evaluation_history.resize(new_size);

   training_direction_history.resize(new_size);
   training_rate_history.resize(new_size);
   elapsed_time_history.resize(new_size);
}


// std::string to_string(void) const method

/// This method returns a string representation of the current random search results structure. 

std::string RandomSearch::RandomSearchResults::to_string(void) const
{
   std::ostringstream buffer;

   // Parameters history

   if(!parameters_history.empty())
   {
      if(!parameters_history.empty())
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
   
   // Evaluation history   

   if(!evaluation_history.empty())
   {
       buffer << "% Evaluation history:\n"
              << evaluation_history << "\n"; 
   }

   // Generalization evaluation history

   if(!generalization_evaluation_history.empty())
   {
       buffer << "% Generalization evaluation history:\n"
              << generalization_evaluation_history << "\n"; 
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


// RandomSearchResults* perform_training(void) method

/// This method trains a neural network with an associated performance functional according to the random search training algorithm.
/// Training occurs according to the training parameters. 

RandomSearch::RandomSearchResults* RandomSearch::perform_training(void)
{
   // Control sentence (if debug)

   #ifdef _DEBUG 

   check();

   #endif

   RandomSearchResults* random_search_training_results_pointer = new RandomSearchResults;

   // Start training 

   if(display)
   {
      std::cout << "Training with random search...\n";
   }
   
   random_search_training_results_pointer->resize_training_history(1+maximum_epochs_number);

   // Elapsed time

   time_t beginning_time, current_time;
   time(&beginning_time);
   double elapsed_time;

   // Neural network stuff

   NeuralNetwork* neural_network_pointer = performance_functional_pointer->get_neural_network_pointer();

   #ifdef _DEBUG 

   if(neural_network_pointer == NULL)
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: RandomSearch class.\n"
             << "RandomSearchResults* perform_training(void) method.\n"
             << "Pointer to neural network in performance functional is NULL.\n";

      throw std::logic_error(buffer.str().c_str());	  
   }

   #endif

   const unsigned int parameters_number = neural_network_pointer->count_parameters_number();

   Vector<double> parameters(parameters_number);

   double parameters_norm = 0.0;

   // Performance functional stuff
    
   double performance = 0.0;

   // Training algorithm stuff 

   Vector<double> training_direction(parameters_number);
   double training_rate = 0.0;

   Vector<double> parameters_increment(parameters_number);

   bool stop_training = false;

   // Main loop 

   for(unsigned int epoch = 0; epoch <= maximum_epochs_number; epoch++)
   {
      if(epoch == 0)
      {
         parameters = neural_network_pointer->arrange_parameters();	  

         parameters_norm = parameters.calculate_norm();
      }      

      // Performance functional stuff

      if(epoch == 0)
	  {
         performance = performance_functional_pointer->calculate_evaluation();
	  }
	  else
	  {
         performance = performance_functional_pointer->calculate_evaluation(parameters);
	  }

      if(performance < performance)
      {      
         parameters = parameters;

         neural_network_pointer->set_parameters(parameters);

         performance = performance;
      }

      // Training algorithm stuff

      training_direction = calculate_training_direction();
  
      if(epoch == 0)
      {
         training_rate = 1.0;      
      }
      else if(epoch%training_rate_reduction_period == 0)
      {
         training_rate *= training_rate_reduction_factor; 
      }       

      parameters_increment = training_direction*training_rate;

      // Elapsed time

      time(&current_time);
      elapsed_time = difftime(current_time, beginning_time);

      // Training history multilayer perceptron 

      if(reserve_parameters_history)
      {
         random_search_training_results_pointer->parameters_history[epoch] = parameters;                                
      }

      if(reserve_parameters_history)
      {
         random_search_training_results_pointer->parameters_history[epoch] = parameters;                                
      }

      if(reserve_parameters_norm_history)
      {
         random_search_training_results_pointer->parameters_norm_history[epoch] = parameters_norm; 
      }       
	  
	  if(reserve_parameters_norm_history)
      {
         random_search_training_results_pointer->parameters_norm_history[epoch] = parameters_norm; 
      }

      // Training history performance functional

      if(reserve_evaluation_history)
      {
         random_search_training_results_pointer->evaluation_history[epoch] = performance;
      }

      if(reserve_evaluation_history)
      {
         random_search_training_results_pointer->evaluation_history[epoch] = performance;
      }

      // Training history training algorithm

      if(reserve_training_direction_history)
      {
         random_search_training_results_pointer->training_direction_history[epoch] = training_direction;                                
      }

      if(reserve_training_rate_history)
      {
         random_search_training_results_pointer->training_rate_history[epoch] = training_rate;
      }

      if(reserve_elapsed_time_history)
      {
         random_search_training_results_pointer->elapsed_time_history[epoch] = elapsed_time;
      }

      // Stopping Criteria

      if(performance <= performance_goal)
      {
         if(display)
         {
            std::cout << "Epoch " << epoch << ": Performance goal reached.\n";
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
         random_search_training_results_pointer->final_parameters = parameters;
         random_search_training_results_pointer->final_parameters_norm = parameters_norm;

         random_search_training_results_pointer->final_evaluation = performance;
         //random_search_training_results_pointer->final_generalization_evaluation = generalization_evaluation;
  
         random_search_training_results_pointer->final_training_direction = training_direction;
         random_search_training_results_pointer->final_training_rate = training_rate;
         random_search_training_results_pointer->elapsed_time = elapsed_time;

         if(display)
		 {
            std::cout << "Potential parameters norm: " << parameters_norm << "\n"
                      << "Parameters norm: " << parameters_norm << "\n"
                      << "Potential performance: " << performance << "\n"
                      << "Performance: " << performance << "\n"
                      << performance_functional_pointer->write_information()
                      << "Training rate: " << training_rate << "\n"
                      << "Elapsed time: " << elapsed_time << std::endl; 
		 }   
 
         break;
      }

      else if(display && epoch % display_period == 0)
      {
         std::cout << "Epoch " << epoch << ";\n"
                   << "Potential parameters norm: " << parameters_norm << "\n"
                   << "Parameters norm: " << parameters_norm << "\n" 
                   << "Potential performance: " << performance << "\n"
                   << "Performance: " << performance << "\n"
                   << performance_functional_pointer->write_information()
                   << "Training rate: " << training_rate << "\n"
                   << "Elapsed time: " << elapsed_time << std::endl; 
      }

      // Set new parameters

      parameters = parameters + parameters_increment;
      parameters_norm = parameters.calculate_norm();
   }

   return(random_search_training_results_pointer);
}


// std::string write_training_algorithm_type(void) const method

std::string RandomSearch::write_training_algorithm_type(void) const
{
   return("RANDOM_SEARCH");
}


// TiXmlElement* to_XML(void) const method

/// This method prints to the screen the training parameters, the stopping criteria
/// and other user stuff concerning the random search object.

TiXmlElement* RandomSearch::to_XML(void) const 
{
   std::ostringstream buffer;

   // Training algorithm

   TiXmlElement* random_search_element = new TiXmlElement("RandomSearch");
   random_search_element->SetAttribute("Version", 4); 

   // Training rate reduction factor

   TiXmlElement* training_rate_reduction_factor_element = new TiXmlElement("TrainingRateReductionFactor");
   random_search_element->LinkEndChild(training_rate_reduction_factor_element);

   buffer.str("");
   buffer << training_rate_reduction_factor;

   TiXmlText* training_rate_reduction_factor_text = new TiXmlText(buffer.str().c_str());
   training_rate_reduction_factor_element->LinkEndChild(training_rate_reduction_factor_text);

   // Training rate reduction period

   TiXmlElement* training_rate_reduction_period_element = new TiXmlElement("TrainingRateReductionPeriod");
   random_search_element->LinkEndChild(training_rate_reduction_period_element);

   buffer.str("");
   buffer << training_rate_reduction_period;

   TiXmlText* training_rate_reduction_period_text = new TiXmlText(buffer.str().c_str());
   training_rate_reduction_period_element->LinkEndChild(training_rate_reduction_period_text);

   // First training rate

   TiXmlElement* first_training_rate_element = new TiXmlElement("FirstTrainingRate");
   random_search_element->LinkEndChild(first_training_rate_element);

   buffer.str("");
   buffer << first_training_rate;

   TiXmlText* first_training_rate_text = new TiXmlText(buffer.str().c_str());
   first_training_rate_element->LinkEndChild(first_training_rate_text);

   // Warning parameters norm

   TiXmlElement* warning_parameters_norm_element = new TiXmlElement("WarningParametersNorm");
   random_search_element->LinkEndChild(warning_parameters_norm_element);

   buffer.str("");
   buffer << warning_parameters_norm;

   TiXmlText* warning_parameters_norm_text = new TiXmlText(buffer.str().c_str());
   warning_parameters_norm_element->LinkEndChild(warning_parameters_norm_text);

   // Warning training rate 

   TiXmlElement* warning_training_rate_element = new TiXmlElement("WarningTrainingRate");
   random_search_element->LinkEndChild(warning_training_rate_element);

   buffer.str("");
   buffer << warning_training_rate;

   TiXmlText* warning_training_rate_text = new TiXmlText(buffer.str().c_str());
   warning_training_rate_element->LinkEndChild(warning_training_rate_text);

   // Error parameters norm

   TiXmlElement* error_parameters_norm_element = new TiXmlElement("ErrorParametersNorm");
   random_search_element->LinkEndChild(error_parameters_norm_element);

   buffer.str("");
   buffer << error_parameters_norm;

   TiXmlText* error_parameters_norm_text = new TiXmlText(buffer.str().c_str());
   error_parameters_norm_element->LinkEndChild(error_parameters_norm_text);

   // Error training rate

   TiXmlElement* error_training_rate_element = new TiXmlElement("ErrorTrainingRate");
   random_search_element->LinkEndChild(error_training_rate_element);

   buffer.str("");
   buffer << error_training_rate;

   TiXmlText* error_training_rate_text = new TiXmlText(buffer.str().c_str());
   error_training_rate_element->LinkEndChild(error_training_rate_text);

   // Performance goal 

   TiXmlElement* performance_goal_element = new TiXmlElement("PerformanceGoal");
   random_search_element->LinkEndChild(performance_goal_element);

   buffer.str("");
   buffer << performance_goal;

   TiXmlText* performance_goal_text = new TiXmlText(buffer.str().c_str());
   performance_goal_element->LinkEndChild(performance_goal_text);

   // Maximum epochs number 

   TiXmlElement* maximum_epochs_number_element = new TiXmlElement("MaximumEpochsNumber");
   random_search_element->LinkEndChild(maximum_epochs_number_element);

   buffer.str("");
   buffer << maximum_epochs_number;

   TiXmlText* maximum_epochs_number_text = new TiXmlText(buffer.str().c_str());
   maximum_epochs_number_element->LinkEndChild(maximum_epochs_number_text);

   // Maximum time 

   TiXmlElement* maximum_time_element = new TiXmlElement("MaximumTime");
   random_search_element->LinkEndChild(maximum_time_element);

   buffer.str("");
   buffer << maximum_time;

   TiXmlText* maximum_time_text = new TiXmlText(buffer.str().c_str());
   maximum_time_element->LinkEndChild(maximum_time_text);

   // Reserve parameters history 

   TiXmlElement* reserve_parameters_history_element = new TiXmlElement("ReserveParametersHistory");
   random_search_element->LinkEndChild(reserve_parameters_history_element);

   buffer.str("");
   buffer << reserve_parameters_history;

   TiXmlText* reserve_parameters_history_text = new TiXmlText(buffer.str().c_str());
   reserve_parameters_history_element->LinkEndChild(reserve_parameters_history_text);

   // Reserve parameters norm history 

   TiXmlElement* reserve_parameters_norm_history_element = new TiXmlElement("ReserveParametersNormHistory");
   random_search_element->LinkEndChild(reserve_parameters_norm_history_element);

   buffer.str("");
   buffer << reserve_parameters_norm_history;

   TiXmlText* reserve_parameters_norm_history_text = new TiXmlText(buffer.str().c_str());
   reserve_parameters_norm_history_element->LinkEndChild(reserve_parameters_norm_history_text);

   // Reserve evaluation history 

   TiXmlElement* reserve_evaluation_history_element = new TiXmlElement("ReservePerformanceHistory");
   random_search_element->LinkEndChild(reserve_evaluation_history_element);

   buffer.str("");
   buffer << reserve_evaluation_history;

   TiXmlText* reserve_evaluation_history_text = new TiXmlText(buffer.str().c_str());
   reserve_evaluation_history_element->LinkEndChild(reserve_evaluation_history_text);

   // Reserve training direction history 

   TiXmlElement* reserve_training_direction_history_element = new TiXmlElement("ReserveTrainingDirectionHistory");
   random_search_element->LinkEndChild(reserve_training_direction_history_element);

   buffer.str("");
   buffer << reserve_training_direction_history;

   TiXmlText* reserve_training_direction_history_text = new TiXmlText(buffer.str().c_str());
   reserve_training_direction_history_element->LinkEndChild(reserve_training_direction_history_text);

   // Reserve training rate history 

   TiXmlElement* reserve_training_rate_history_element = new TiXmlElement("ReserveTrainingRateHistory");
   random_search_element->LinkEndChild(reserve_training_rate_history_element);

   buffer.str("");
   buffer << reserve_training_rate_history;

   TiXmlText* reserve_training_rate_history_text = new TiXmlText(buffer.str().c_str());
   reserve_training_rate_history_element->LinkEndChild(reserve_training_rate_history_text);

   // Reserve elapsed time history 

   TiXmlElement* reserve_elapsed_time_history_element = new TiXmlElement("ReserveElapsedTimeHistory");
   random_search_element->LinkEndChild(reserve_elapsed_time_history_element);

   buffer.str("");
   buffer << reserve_elapsed_time_history;

   TiXmlText* reserve_elapsed_time_history_text = new TiXmlText(buffer.str().c_str());
   reserve_elapsed_time_history_element->LinkEndChild(reserve_elapsed_time_history_text);

   // Reserve generalization evaluation history 

   TiXmlElement* reserve_generalization_evaluation_history_element = new TiXmlElement("ReserveGeneralizationPerformanceHistory");
   random_search_element->LinkEndChild(reserve_generalization_evaluation_history_element);

   buffer.str("");
   buffer << reserve_generalization_evaluation_history;

   TiXmlText* reserve_generalization_evaluation_history_text = new TiXmlText(buffer.str().c_str());
   reserve_generalization_evaluation_history_element->LinkEndChild(reserve_generalization_evaluation_history_text);

   // Display period

   TiXmlElement* display_period_element = new TiXmlElement("DisplayPeriod");
   random_search_element->LinkEndChild(display_period_element);

   buffer.str("");
   buffer << display_period;

   TiXmlText* display_period_text = new TiXmlText(buffer.str().c_str());
   display_period_element->LinkEndChild(display_period_text);

   // Display

   TiXmlElement* display_element = new TiXmlElement("Display");
   random_search_element->LinkEndChild(display_element);

   buffer.str("");
   buffer << display;

   TiXmlText* display_text = new TiXmlText(buffer.str().c_str());
   display_element->LinkEndChild(display_text);

   return(random_search_element);
}


// void from_XML(TiXmlElement*) method

void RandomSearch::from_XML(TiXmlElement*) 
{

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
