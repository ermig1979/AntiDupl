/****************************************************************************************************************/
/*                                                                                                              */
/*   OpenNN: Open Neural Networks Library                                                                       */
/*   www.opennn.cimne.com                                                                                       */
/*                                                                                                              */
/*   E V O L U T I O N A R Y   A L G O R I T H M   C L A S S                                                    */
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
#include <time.h>

// OpenNN includes

#include "evolutionary_algorithm.h"

namespace OpenNN
{

// DEFAULT CONSTRUCTOR

/// Default constructor. 
/// It creates a evolutionary training algorithm not associated to any performance functional.
/// It also initializes the class members to their default values.

EvolutionaryAlgorithm::EvolutionaryAlgorithm(void) : TrainingAlgorithm()
{
   set_default();
}


// PERFORMANCE FUNCTIONAL CONSTRUCTOR 

/// Performance functional constructor. 
/// It creates a evolutionary training algorithm associated to a performance functional.
/// It also initializes the class members to their default values.
/// @param new_performance_functional_pointer Pointer to a performance functional object.

EvolutionaryAlgorithm::EvolutionaryAlgorithm(PerformanceFunctional* new_performance_functional_pointer)
: TrainingAlgorithm(new_performance_functional_pointer)
{
   set_default();
}


// XML CONSTRUCTOR

/// XML constructor. 
/// It creates a evolutionary training algorithm not associated to any performance functional.
/// It also loads the class members from a XML element.
/// @param evolutionary_algorithm_element Tiny XML element with the evolutionary algorithm object members. 

EvolutionaryAlgorithm::EvolutionaryAlgorithm(TiXmlElement* evolutionary_algorithm_element) 
 : TrainingAlgorithm(evolutionary_algorithm_element)
{
   set_default();
}


// DESTRUCTOR

/// Destructor.

EvolutionaryAlgorithm::~EvolutionaryAlgorithm(void)
{

}


// METHODS

// const FitnessAssignmentMethod& get_fitness_assignment_method(void) const method

/// This method returns the fitness assignment method used for training.
 
const EvolutionaryAlgorithm::FitnessAssignmentMethod& EvolutionaryAlgorithm::get_fitness_assignment_method(void) const
{
   return(fitness_assignment_method);
}


// std::string write_fitness_assignment_method(void) const method

/// This method returns a string with the name of the method used for fitness assignment.  

std::string EvolutionaryAlgorithm::write_fitness_assignment_method(void) const
{
   switch(fitness_assignment_method)
   {
      case LinearRanking:
      {
         return("LinearRanking");
	  }
      break;

	  default:
      {
         std::ostringstream buffer;

         buffer << "OpenNN Exception: EvolutionaryAlgorithm class.\n"
                << "std::string write_fitness_assignment_method(void) const method.\n"
                << "Unknown fitness assignment method.\n";
 
         throw std::logic_error(buffer.str().c_str());	     
	  }
      break;
   }
}


// const SelectionMethod& get_selection_method(void) const method

/// This method returns the selection method used for training.

const EvolutionaryAlgorithm::SelectionMethod& EvolutionaryAlgorithm::get_selection_method(void) const
{
   return(selection_method);
}


// std::string write_selection_method(void) const method

/// This method returns a string with the name of the method used for selection.  

std::string EvolutionaryAlgorithm::write_selection_method(void) const
{
   switch(selection_method)
   {
      case RouletteWheel:
      {
         return("RouletteWheel");
	  }
      break;

      case StochasticUniversalSampling:
      {
         return("StochasticUniversalSampling");
	  }
      break;

	  default:
      {
         std::ostringstream buffer;

         buffer << "OpenNN Exception: EvolutionaryAlgorithm class.\n"
                << "std::string write_selection_method(void) const method.\n"
                << "Unknown selection method.\n";
 
         throw std::logic_error(buffer.str().c_str());	     
	  }
      break;
   }
}


// const RecombinationMethod& get_recombination_method(void) const method

/// This method returns the recombination method used for training.

const EvolutionaryAlgorithm::RecombinationMethod& EvolutionaryAlgorithm::get_recombination_method(void) const
{
   return(recombination_method);
}


// std::string write_recombination_method(void) const method

/// This method returns a string with the name of the method used for recombination.  

std::string EvolutionaryAlgorithm::write_recombination_method(void) const
{
   switch(recombination_method)
   {
      case Line:
      {
         return("Line");
	  }
      break;

      case Intermediate:
      {
         return("Intermediate");
	  }
      break;

	  default:
      {
         std::ostringstream buffer;

         buffer << "OpenNN Exception: EvolutionaryAlgorithm class.\n"
                << "std::string write_recombination_method(void) const method.\n"
                << "Unknown recombination method.\n";
 
         throw std::logic_error(buffer.str().c_str());	     
	  }
      break;
   }
}


// const MutationMethod get_mutation_method(void) const method

/// This method returns the mutation method used for training.

const EvolutionaryAlgorithm::MutationMethod& EvolutionaryAlgorithm::get_mutation_method(void) const
{
   return(mutation_method);
}


// std::string write_mutation_method(void) const method

/// This method returns a string with the name of the method used for mutation.  

std::string EvolutionaryAlgorithm::write_mutation_method(void) const
{
   switch(mutation_method)
   {
      case Normal:
      {
         return("Normal");
	  }
      break;

      case Uniform:
      {
         return("Uniform");
	  }
      break;

	  default:
      {
         std::ostringstream buffer;
 
         buffer << "OpenNN Exception: EvolutionaryAlgorithm class.\n"
                << "std::string get_mutation_method_name(void) const method.\n"
                << "Unknown mutation method.\n";
 
         throw std::logic_error(buffer.str().c_str());	     
	  }
      break;
   }
}


// unsigned int get_population_size(void) const method

/// This method returns the number of individuals in the population.

unsigned int EvolutionaryAlgorithm::get_population_size(void) const
{
   return(population.get_rows_number());
}


// const Matrix<double>& get_population(void) const method

/// This method returns the population matrix.

const Matrix<double>& EvolutionaryAlgorithm::get_population(void) const
{
   return(population);
}


// const Vector<double>& get_performance(void) const method

/// This method returns the actual performance value of all individuals in the population.

const Vector<double>& EvolutionaryAlgorithm::get_performance(void) const
{
   return(performance);
}


// const Vector<double>& get_fitness(void) const method

/// This method returns the actual fitness value of all individuals in the population.

const Vector<double>& EvolutionaryAlgorithm::get_fitness(void) const
{
   return(fitness);
}


// const Vector<bool>& get_selection(void) const method

/// This method returns the actual selection value of all individuals in the population.

const Vector<bool>& EvolutionaryAlgorithm::get_selection(void) const
{
   return(selection);
}


// const double& get_warning_parameters_norm(void) const method

/// This method returns the minimum value for the norm of the parameters vector at wich a warning message is 
/// written to the screen. 

const double& EvolutionaryAlgorithm::get_warning_parameters_norm(void) const
{
   return(warning_parameters_norm);       
}


// const double& get_error_parameters_norm(void) const method

/// This method returns the value for the norm of the parameters vector at wich an error message is 
/// written to the screen and the program exits. 

const double& EvolutionaryAlgorithm::get_error_parameters_norm(void) const
{
   return(error_parameters_norm);
}


// const double& get_minimum_parameters_increment_norm(void) const method

/// This method returns the minimum norm of the parameter increment vector used as a stopping criteria when training. 

const double& EvolutionaryAlgorithm::get_minimum_parameters_increment_norm(void) const
{
   return(minimum_parameters_increment_norm);
}


// const double& get_minimum_performance_increase(void) const method

/// This method returns the minimum performance improvement during training.  

const double& EvolutionaryAlgorithm::get_minimum_performance_increase(void) const
{
   return(minimum_performance_increase);
}


// const double& get_performance_goal(void) const method

/// This method returns the goal value for the performance. 
/// This is used as a stopping criterium when training a multilayer perceptron

const double& EvolutionaryAlgorithm::get_performance_goal(void) const
{
   return(performance_goal);
}


// const unsigned int& get_maximum_generalization_evaluation_decreases(void) const method

/// This method returns the maximum number of generalization failures during the training process. 

const unsigned int& EvolutionaryAlgorithm::get_maximum_generalization_evaluation_decreases(void) const
{
   return(maximum_generalization_evaluation_decreases);
}


// const double& get_maximum_time(void) const method

/// This method returns the maximum training time.  

const double& EvolutionaryAlgorithm::get_maximum_time(void) const
{
   return(maximum_time);
}


// const bool& get_reserve_parameters_history(void) const method

/// This method returns true if the parameters history matrix is to be reserved, and false otherwise.

const bool& EvolutionaryAlgorithm::get_reserve_parameters_history(void) const
{
   return(reserve_parameters_history);     
}


// const bool& get_reserve_parameters_norm_history(void) const method 

/// This method returns true if the parameters norm history vector is to be reserved, and false otherwise.

const bool& EvolutionaryAlgorithm::get_reserve_parameters_norm_history(void) const
{
   return(reserve_parameters_norm_history);     
}


// const bool& get_reserve_evaluation_history(void) const method

/// This method returns true if the evaluation history vector is to be reserved, and false otherwise.

const bool& EvolutionaryAlgorithm::get_reserve_evaluation_history(void) const
{
   return(reserve_evaluation_history);     
}


// const bool& get_reserve_elapsed_time_history(void) const method

/// This method returns true if the elapsed time history vector is to be reserved, and false otherwise.

const bool& EvolutionaryAlgorithm::get_reserve_elapsed_time_history(void) const
{
   return(reserve_elapsed_time_history);     
}


// const bool& get_reserve_generalization_evaluation_history(void) const method

/// This method returns true if the Generalization evaluation history vector is to be reserved, and false otherwise.

const bool& EvolutionaryAlgorithm::get_reserve_generalization_evaluation_history(void) const
{
   return(reserve_generalization_evaluation_history);
}


// const unsigned int& get_display_period(void) const method

/// This method returns the number of epochs between the training showing progress. 

const unsigned int& EvolutionaryAlgorithm::get_display_period(void) const
{
   return(display_period);
}



// const bool& get_reserve_population_history(void) const method

/// This method returns true if the population history vector of matrices is to be reserved, and false otherwise.

const bool& EvolutionaryAlgorithm::get_reserve_population_history(void) const
{
   return(reserve_population_history);
}


// const bool& get_reserve_best_individual_history(void) const method

/// This method returns true if the best individual history vector of vectors is to be reserved, and false otherwise.

const bool& EvolutionaryAlgorithm::get_reserve_best_individual_history(void) const
{
   return(reserve_best_individual_history);
}


// const bool& get_reserve_mean_norm_history(void) const method

/// This method returns true if the mean population norm history vector is to be reserved, and false otherwise.

const bool& EvolutionaryAlgorithm::get_reserve_mean_norm_history(void) const
{
   return(reserve_mean_norm_history);
}


// const bool& get_reserve_standard_deviation_norm_history(void) const method

/// This method returns true if the standard deviation of the population norm history vector is to be reserved,
/// and false otherwise.

const bool& EvolutionaryAlgorithm::get_reserve_standard_deviation_norm_history(void) const
{
   return(reserve_standard_deviation_norm_history);
}


// const bool& get_reserve_best_norm_history(void) const method

/// This method returns true if the norm of the best individual in the population history vector is to be 
/// reserved, and false otherwise.

const bool& EvolutionaryAlgorithm::get_reserve_best_norm_history(void) const
{
   return(reserve_best_norm_history);
}


// const bool& get_reserve_mean_evaluation_history(void) const method

/// This method returns true if the mean evaluation history vector is to be reserved, and false otherwise.

const bool& EvolutionaryAlgorithm::get_reserve_mean_evaluation_history(void) const
{
   return(reserve_mean_evaluation_history);
}


// const bool& get_reserve_standard_deviation_evaluation_history(void) const method

/// This method returns true if the standard deviation of the evaluation history vector is to be reserved,
/// and false otherwise.

const bool& EvolutionaryAlgorithm::get_reserve_standard_deviation_evaluation_history(void) const
{
   return(reserve_standard_deviation_evaluation_history);
}


// const bool& get_reserve_best_evaluation_history(void) const method

/// This method returns true if the best evaluation history vector is to be reserved, and false otherwise.

const bool& EvolutionaryAlgorithm::get_reserve_best_evaluation_history(void) const
{
   return(reserve_best_evaluation_history);
}


// void set(void) method

/// This method sets the performance functional pointer of this object to NULL. 
/// It also sets the rest of members to their default values. 

void EvolutionaryAlgorithm::set(void)
{
   performance_functional_pointer = NULL;

   set_default();
}


// void set(PerformanceFunctional*) method

/// This method sets a new performance functional pointer to the evolutionary algorithm object. 
/// It also sets the rest of members to their default values. 

void EvolutionaryAlgorithm::set(PerformanceFunctional* new_performance_functional_pointer)
{
   performance_functional_pointer = new_performance_functional_pointer;

   set_default();
}


// void set_default(void) method

/// This method sets the members of the evolutionary algorithm object to their default values.
/// Training operators:
/// <ul>
/// <li> Fitness assignment method: Linear ranking.
/// <li> Selection method: Stochastic universal sampling.
/// <li> Recombination method: Intermediate.
/// <li> Mutation method: Normal.
/// </ul>
/// Training parameters:
/// <ul>
/// <li> Population size: 10*parameters_number or 0.
/// <li> Perform elitism: false.
/// <li> Selective pressure: 1.5.
/// <li> Recombination size: 0.25.
/// <li> Mutation rate: = 1/parameters_number or 0.
/// <li> Mutation range: = 0.1
/// </ul>
/// Stopping criteria:
/// <ul> 
/// <li> Performance goal: -1.0e99.
/// <li> Mean performance goal: -1.0e99.
/// <li> Standard deviation of performance goal: -1.0e99.
/// <li> Maximum training time: 1.0e6.
/// <li> Maximum number of generations: 100. 
/// </ul> 
/// Training history:
/// <ul> 
/// <li> Population = false.
/// <li> Mean norm = false.
/// <li> Standard deviation norm = false.
/// <li> Best norm = false.
/// <li> Mean performance = false.
/// <li> Standard deviation performance = false.
/// <li> Best performance = false.
/// </ul> 
/// User stuff: 
/// <ul>
/// <li> Display: true. 
/// <li> Display period: 1. 
/// </ul>

void EvolutionaryAlgorithm::set_default(void)
{
   unsigned int parameters_number = 0;

   if(performance_functional_pointer)
   {
	  NeuralNetwork* neural_network_pointer = performance_functional_pointer->get_neural_network_pointer();

     if(neural_network_pointer)
     {
        parameters_number = neural_network_pointer->count_parameters_number();	  
     }
  }

   // Fitness assignment method

   fitness_assignment_method = LinearRanking;

   // Selection method

   selection_method = StochasticUniversalSampling;

   // Recombination method

   recombination_method = Intermediate;

   // Mutation method

   mutation_method = Normal;

   // Training parameters

   unsigned int population_size = 10*parameters_number;   

   elitism = false;
   selective_pressure = 1.5;

   recombination_size = 0.25;

   if(parameters_number != 0)
   {
      mutation_rate = 1.0/(double)parameters_number;
   }
   else
   {
      mutation_rate = 0.0;
   }

   mutation_range = 0.1;

   // Stopping criteria

   performance_goal = -1.0e99;
   mean_performance_goal = -1.0e99;
   standard_deviation_performance_goal = 0.0;

   maximum_time = 1.0e6;

   maximum_generations_number = 1000;

   // Population matrix

   population.set(population_size, parameters_number);

   initialize_population_normal();

   // Performance vector

   performance.set(population_size);

   // Fitness vector
  
   fitness.set(population_size);

   // Selection vector

   selection.set(population_size);

   // Training history

   reserve_population_history = false;

   reserve_best_individual_history = false;

   reserve_mean_norm_history = false;
   reserve_standard_deviation_norm_history = false;
   reserve_best_norm_history = false;

   reserve_mean_evaluation_history = false;
   reserve_standard_deviation_evaluation_history = false;
   reserve_best_evaluation_history = false;

   reserve_elapsed_time_history = false;

   // User stuff

   display_period = 100;

}


// void set_population_size(unsigned int) method

/// This method sets a new population with a new number of individuals.  
/// The new population size must be an even number equal or greater than four. 
///
/// @param new_population_size Number of individuals in the population. This must be an even number equal or 
/// greater than four. 

void EvolutionaryAlgorithm::set_population_size(const unsigned int& new_population_size)
{
   // Control sentence (if debug)

   #ifdef _DEBUG 

   check();

   #endif

   const NeuralNetwork* neural_network_pointer = performance_functional_pointer->get_neural_network_pointer();

   // Control sentence (if debug)

   #ifdef _DEBUG 

   if(!neural_network_pointer)
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: EvolutionaryAlgorithm class.\n"
             << "void set_population_size(unsigned int) method.\n"
             << "Multilayer perceptron pointer is NULL.\n";

      throw std::logic_error(buffer.str().c_str());	  
   }

   #endif

   unsigned int parameters_number = neural_network_pointer->count_parameters_number();

   if(new_population_size < 4)
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: EvolutionaryAlgorithm class.\n"
             << "void set_population_size(unsigned int) method.\n"
             << "New population size must be equal or greater than 4.\n";

      throw std::logic_error(buffer.str().c_str());	  
   }
   else if(new_population_size%2 != 0)
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: EvolutionaryAlgorithm class.\n"
             << "void set_population_size(unsigned int) method.\n"
             << "New population size is not divisible by 2.\n";

      throw std::logic_error(buffer.str().c_str());	  
   }
   else
   {
      // Set population matrix

      population.resize(new_population_size, parameters_number);

      initialize_population_normal();

      // Set performance vector

      performance.resize(new_population_size);

      // Set fitness vector

      fitness.resize(new_population_size);

      // Set selection vector

      selection.resize(new_population_size);
   }
}


// void set_fitness_assignment_method(const std::string&) method

/// This method sets a new method for fitness assignment from a string containing the name.
/// Possible values are:
/// <ul>
/// <li> "LinearRanking"
/// </ul>
/// @param new_fitness_assignment_method_name String with name of method for fitness assignment.   

void EvolutionaryAlgorithm::set_fitness_assignment_method(const std::string& new_fitness_assignment_method_name)
{
   if(new_fitness_assignment_method_name == "LinearRanking")
   {
      fitness_assignment_method = LinearRanking;
   }
   else
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: EvolutionaryAlgorithm class.\n"
             << "void set_fitness_assignment_method(const std::string&) method.\n"
			 << "Unknown fitness assignment method: " << new_fitness_assignment_method_name << ".\n";
   
      throw std::logic_error(buffer.str().c_str());	  
   }
}


// void set_selection_method(const std::string&) method

/// This method sets a new method for selection from a string containing the name.
/// Possible values are:
/// <ul>
/// <li> "LinearRanking"
/// <li> "StochasticUniversalSampling"
/// </ul>
/// @param new_selection_method_name String with name of method for selection.   

void EvolutionaryAlgorithm::set_selection_method(const std::string& new_selection_method_name)
{
   if(new_selection_method_name == "RouletteWheel")
   {
      selection_method = RouletteWheel;
   }
   else if(new_selection_method_name == "StochasticUniversalSampling")
   {
      selection_method = StochasticUniversalSampling;
   }
   else
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: EvolutionaryAlgorithm class.\n"
             << "void set_selection_method(const std::string&) method.\n"
			 << "Unknown selection method: " << new_selection_method_name << ".\n";
   
      throw std::logic_error(buffer.str().c_str());	  
   }
}


// void set_recombination_method(const std::string&) method

/// This method sets a new method for recombination from a string containing the name.
/// Possible values are:
/// <ul>
/// <li> "Line"
/// <li> "Intermediate"
/// </ul>
/// @param new_recombination_method_name String with name of method for recombination.   

void EvolutionaryAlgorithm::set_recombination_method(const std::string& new_recombination_method_name)
{
   if(new_recombination_method_name == "Line")
   {
      recombination_method = Line;
   }
   else if(new_recombination_method_name == "Intermediate")
   {
      recombination_method = Intermediate;
   }
   else
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: EvolutionaryAlgorithm class.\n"
             << "void set_recombination_method(const std::string&) method.\n"
			 << "Unknown recombination method: " << new_recombination_method_name << ".\n";
   
      throw std::logic_error(buffer.str().c_str());	  
   }
}


// void set_mutation_method(const std::string&) method

/// This method sets a new method for mutation from a string containing the name.
/// Possible values are:
/// <ul>
/// <li> "Normal"
/// <li> "Uniform"
/// </ul>
/// @param new_mutation_method_name String with name of method for mutation.   

void EvolutionaryAlgorithm::set_mutation_method(const std::string& new_mutation_method_name)
{
   if(new_mutation_method_name == "Normal")
   {
      mutation_method = Normal;
   }
   else if(new_mutation_method_name == "Uniform")
   {
      mutation_method = Uniform;
   }
   else
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: EvolutionaryAlgorithm class.\n"
             << "void set_mutation_method(const std::string&) method.\n"
			 << "Unknown mutationg method: " << new_mutation_method_name << ".\n";
   
      throw std::logic_error(buffer.str().c_str());	  
   }
}


// void set_population(const Matrix<double>&) method

/// This method sets a new population.
///
/// @param new_population Population Matrix.

void EvolutionaryAlgorithm::set_population(const Matrix<double>& new_population)
{
   // Control sentence (if debug)

   #ifdef _DEBUG 

   if(performance_functional_pointer == NULL)
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: EvolutionaryAlgorithm class.\n"
             << "void set_population(const Matrix<double>&) method.\n"
             << "Objective functional pointer is NULL.\n";

      throw std::logic_error(buffer.str().c_str());	  
   }

   const NeuralNetwork* neural_network_pointer = performance_functional_pointer->get_neural_network_pointer();

   if(!neural_network_pointer)
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: EvolutionaryAlgorithm class.\n"
             << "void set_population(const Matrix<double>&) method.\n"
             << "Multilayer perceptron pointer is NULL.\n";

      throw std::logic_error(buffer.str().c_str());	  
   }

   const unsigned int population_size = get_population_size();
   const unsigned int parameters_number = neural_network_pointer->count_parameters_number();   

   if(new_population.get_rows_number() != population_size)
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: EvolutionaryAlgorithm class.\n"
             << "void set_population(const Matrix<double>&) method.\n"
             << "New population size is not equal to population size.\n";

      throw std::logic_error(buffer.str().c_str());	  
   }
   else if(new_population.get_columns_number() != parameters_number)
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: EvolutionaryAlgorithm class.\n"
             << "void set_population(const Matrix<double>&) method.\n"
             << "New number of parameters is not equal to number of parameters.\n";

      throw std::logic_error(buffer.str().c_str());	  
   }

   #endif

   // Set population

   population = new_population;
}


// void set_performance(const Vector<double>&) method

/// This method sets a new population performance vector.
///
/// @param new_performance Population performance values.

void EvolutionaryAlgorithm::set_performance(const Vector<double>& new_performance)
{
   // Control sentence (if debug)

   #ifdef _DEBUG 

   unsigned int population_size = get_population_size();

   if(new_performance.size() != population_size)
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: EvolutionaryAlgorithm class.\n"
             << "void set_performance(const Vector<double>&) method.\n"
             << "Size is not equal to population size.\n";

      throw std::logic_error(buffer.str().c_str());	  
   }

   #endif

   // Set performance

   performance = new_performance;
}


// void set_fitness(const Vector<double>&) method

/// This method sets a new population fitness vector.
///
/// @param new_fitness Population fitness values.

void EvolutionaryAlgorithm::set_fitness(const Vector<double>& new_fitness)
{
   // Control sentence (if debug)

   #ifdef _DEBUG 

   unsigned int population_size = get_population_size();

   if(new_fitness.size() != population_size)
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: EvolutionaryAlgorithm class.\n"
             << "void set_fitness(Vector<double>) method.\n"
             << "Size is not equal to population size.\n";

      throw std::logic_error(buffer.str().c_str());	  
   }

   #endif

   // Set fitness

   fitness = new_fitness;
}


// void set_selection(const Vector<bool>&) method

/// This method sets a new population selection vector.
///
/// @param new_selection Population selection values.

void EvolutionaryAlgorithm::set_selection(const Vector<bool>& new_selection)
{
   // Control sentence (if debug)

   #ifdef _DEBUG 

   unsigned int population_size = get_population_size();

   if(new_selection.size() != population_size)
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: EvolutionaryAlgorithm class.\n"
             << "void set_selection(Vector<double>) method.\n"
             << "Size is not equal to population size.\n";

      throw std::logic_error(buffer.str().c_str());	  
   }

   #endif

   // Set selection

   selection = new_selection;
}


// void set_reserve_population_history(bool) method

/// This method makes the population history vector of matrices to be reseved or not in memory.
/// @param new_reserve_population_history True if the population history vector of matrices is to be reserved, false 
/// otherwise.

void EvolutionaryAlgorithm::set_reserve_population_history(const bool& new_reserve_population_history)
{
   reserve_population_history = new_reserve_population_history;
}


// void set_reserve_best_individual_history(bool) method

/// This method makes the best individual history vector of vectors to be reseved or not in memory.
/// @param new_reserve_best_individual_history True if the best individual history vector of vectors is to be reserved, 
/// false otherwise.

void EvolutionaryAlgorithm::set_reserve_best_individual_history(const bool& new_reserve_best_individual_history)
{
   reserve_best_individual_history = new_reserve_best_individual_history;
}


// void set_reserve_mean_norm_history(bool) method

/// This method makes the mean norm history vector to be reseved or not in memory.
///
/// @param new_reserve_mean_norm_history True if the mean norm history vector is to be reserved, false otherwise.

void EvolutionaryAlgorithm::set_reserve_mean_norm_history(const bool& new_reserve_mean_norm_history)
{
   reserve_mean_norm_history = new_reserve_mean_norm_history;
}


// void set_reserve_standard_deviation_norm_history(bool) method

/// This method makes the standard deviation norm history vector to be reseved or not in memory.
///
/// @param new_reserve_standard_deviation_norm_history True if the standard deviation norm history vector is to be 
/// reserved, false otherwise.

void EvolutionaryAlgorithm::
set_reserve_standard_deviation_norm_history(const bool& new_reserve_standard_deviation_norm_history)
{
   reserve_standard_deviation_norm_history = new_reserve_standard_deviation_norm_history;
}


// void set_reserve_best_norm_history(bool) method

/// This method makes the best norm history vector to be reseved or not in memory.
///
/// @param new_reserve_best_norm_history True if the best norm history vector is to be reserved, false otherwise.

void EvolutionaryAlgorithm::set_reserve_best_norm_history(const bool& new_reserve_best_norm_history)
{
   reserve_best_norm_history = new_reserve_best_norm_history;
}


// void set_reserve_mean_evaluation_history(bool) method

/// This method makes the mean evaluation history vector to be reseved or not in memory.
///
/// @param new_reserve_mean_evaluation_history True if the mean evaluation history vector is to be reserved, false 
/// otherwise.

void EvolutionaryAlgorithm::set_reserve_mean_evaluation_history(const bool& new_reserve_mean_evaluation_history) 
{
   reserve_mean_evaluation_history = new_reserve_mean_evaluation_history;
}


// void set_reserve_standard_deviation_evaluation_history(bool) method

/// This method makes the standard deviation evaluation history vector to be reseved or not in memory.
///
/// @param new_reserve_standard_deviation_evaluation_history True if the standard deviation evaluation history vector 
/// is to be reserved, false otherwise.

void EvolutionaryAlgorithm
::set_reserve_standard_deviation_evaluation_history(const bool& new_reserve_standard_deviation_evaluation_history)
{
   reserve_standard_deviation_evaluation_history = new_reserve_standard_deviation_evaluation_history;
}


// void set_reserve_best_evaluation_history(bool) method

/// This method makes the best evaluation history vector to be reseved or not in memory.
///
/// @param new_reserve_best_evaluation_history True if the best evaluation history vector is to be reserved, 
/// false otherwise.

void EvolutionaryAlgorithm::set_reserve_best_evaluation_history(const bool& new_reserve_best_evaluation_history)
{
   reserve_best_evaluation_history = new_reserve_best_evaluation_history;
}


// void set_reserve_all_training_history(bool) method

/// This method makes the training history of all variables to reseved or not in memory.
///
/// @param new_reserve_all_training_history True if the training history of all variables is to be reserved, 
/// false otherwise.

void EvolutionaryAlgorithm::set_reserve_all_training_history(const bool& new_reserve_all_training_history)
{
   // Multilayer perceptron

   reserve_population_history = new_reserve_all_training_history;

   reserve_best_individual_history = new_reserve_all_training_history;

   reserve_mean_norm_history = new_reserve_all_training_history;
   reserve_standard_deviation_norm_history = new_reserve_all_training_history;
   reserve_best_norm_history = new_reserve_all_training_history;

   // Objective functional

   reserve_mean_evaluation_history = new_reserve_all_training_history;
   reserve_standard_deviation_evaluation_history = new_reserve_all_training_history;
   reserve_best_evaluation_history = new_reserve_all_training_history;

   // Training algorithm 

   reserve_elapsed_time_history = new_reserve_all_training_history;
}


// Vector<double> get_individual(const unsigned int&) const method

/// This method returns the Vector of parameters corresponding to the individual i in the population.
///
/// @param i Index of individual in the population.

Vector<double> EvolutionaryAlgorithm::get_individual(const unsigned int& i) const
{
   // Control sentence (if debug)

   #ifdef _DEBUG 

   unsigned int population_size = get_population_size();

   if(i >= population_size)
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: EvolutionaryAlgorithm class.\n"
             << "Vector<double> get_individual(const unsigned int&) const method.\n"
             << "Index must be less than population size.\n";

      throw std::logic_error(buffer.str().c_str());	  
   }
  
   #endif

   // Get individual

   const Vector<double> individual = population.arrange_row(i);

   return(individual);
}



// set_individual(const unsigned int&, Vector<double>) method

/// This method sets a new Vector of parameters to the individual i in the population. 
///
/// @param i Index of individual in the population.
/// @param individual Vector of parameters to be assigned to individual i.

void EvolutionaryAlgorithm::set_individual(const unsigned int& i, const Vector<double>& individual)
{
   // Control sentence (if debug)

   #ifdef _DEBUG 
 
   const unsigned int size = individual.size();

   const NeuralNetwork* neural_network_pointer = performance_functional_pointer->get_neural_network_pointer();

   const unsigned int parameters_number = neural_network_pointer->count_parameters_number();

   const unsigned int population_size = get_population_size();

   if(i >= population_size)
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: EvolutionaryAlgorithm class.\n"
             << "set_individual(const unsigned int&, Vector<double>) method.\n"
             << "Index must be less than population size.\n";

      throw std::logic_error(buffer.str().c_str());	  
   }
   else if(size != parameters_number)
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: EvolutionaryAlgorithm class.\n"
             << "set_individual(const unsigned int&, Vector<double>) method.\n"
             << "Size must be equal to number of parameters.\n";

      throw std::logic_error(buffer.str().c_str());	  
   }
  
   #endif

   // Get individual

   population.set_row(i, individual);
}


// void set_warning_parameters_norm(const double&) method

/// This method sets a new value for the parameters vector norm at which a warning message is written to the 
/// screen. 
/// @param new_warning_parameters_norm Warning norm of parameters vector value. 

void EvolutionaryAlgorithm::set_warning_parameters_norm(const double& new_warning_parameters_norm)
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


// void set_error_parameters_norm(const double&) method

/// This method sets a new value for the parameters vector norm at which an error message is written to the 
/// screen and the program exits. 
/// @param new_error_parameters_norm Error norm of parameters vector value. 

void EvolutionaryAlgorithm::set_error_parameters_norm(const double& new_error_parameters_norm)
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


// void set_minimum_parameters_increment_norm(const double&) method

/// This method sets a new value for the minimum parameters increment norm stopping criterium. 
/// @param new_minimum_parameters_increment_norm Value of norm of parameters increment norm used to stop training. 

void EvolutionaryAlgorithm::set_minimum_parameters_increment_norm(const double& new_minimum_parameters_increment_norm)
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

void EvolutionaryAlgorithm::set_minimum_performance_increase(const double& new_minimum_performance_increase)
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

void EvolutionaryAlgorithm::set_performance_goal(const double& new_performance_goal)
{
   performance_goal = new_performance_goal;
}


// void set_maximum_generalization_evaluation_decreases(const unsigned int&) method

/// This method sets a new maximum number of generalization failures. 
/// @param new_maximum_generalization_evaluation_decreases Maximum number of epochs in which the generalization evalutation decreases. 

void EvolutionaryAlgorithm::set_maximum_generalization_evaluation_decreases(const unsigned int& new_maximum_generalization_evaluation_decreases)
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


// void set_maximum_time(const double&) method

/// This method sets a new maximum training time.  
/// @param new_maximum_time Maximum training time.

void EvolutionaryAlgorithm::set_maximum_time(const double& new_maximum_time)
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

void EvolutionaryAlgorithm::set_reserve_parameters_history(const bool& new_reserve_parameters_history)
{
   reserve_parameters_history = new_reserve_parameters_history;     
}


// void set_reserve_parameters_norm_history(bool) method

/// This method makes the parameters norm history vector to be reseved or not in memory.
/// @param new_reserve_parameters_norm_history True if the parameters norm history vector is to be reserved, false otherwise.

void EvolutionaryAlgorithm::set_reserve_parameters_norm_history(const bool& new_reserve_parameters_norm_history)
{
   reserve_parameters_norm_history = new_reserve_parameters_norm_history;     
}


// void set_reserve_evaluation_history(bool) method

/// This method makes the evaluation history vector to be reseved or not in memory.
/// @param new_reserve_evaluation_history True if the evaluation history vector is to be reserved, false otherwise.

void EvolutionaryAlgorithm::set_reserve_evaluation_history(const bool& new_reserve_evaluation_history)
{
   reserve_evaluation_history = new_reserve_evaluation_history;     
}


// void set_reserve_elapsed_time_history(bool) method

/// This method makes the elapsed time over the epochs to be reseved or not in memory. This is a vector.
/// @param new_reserve_elapsed_time_history True if the elapsed time history vector is to be reserved, false 
/// otherwise.

void EvolutionaryAlgorithm::set_reserve_elapsed_time_history(const bool& new_reserve_elapsed_time_history)
{
   reserve_elapsed_time_history = new_reserve_elapsed_time_history;     
}


// void set_reserve_generalization_evaluation_history(bool) method

/// This method makes the Generalization evaluation history to be reserved or not in memory. 
/// This is a vector. 
/// @param new_reserve_generalization_evaluation_history True if the Generalization evaluation history is to be reserved, false otherwise. 

void EvolutionaryAlgorithm::set_reserve_generalization_evaluation_history(const bool& new_reserve_generalization_evaluation_history)  
{
   reserve_generalization_evaluation_history = new_reserve_generalization_evaluation_history;
}


// void set_display_period(unsigned int) method

/// This method sets a new number of epochs between the training showing progress. 
/// @param new_display_period
/// Number of epochs between the training showing progress. 

void EvolutionaryAlgorithm::set_display_period(const unsigned int& new_display_period)
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



// Vector<double> calculate_population_norm(void) const method

/// This method returns a vector containing the norm of each individual in the population.

Vector<double> EvolutionaryAlgorithm::calculate_population_norm(void) const
{
   unsigned int population_size = get_population_size();

   Vector<double> population_norm(population_size);
      
   for(unsigned int i = 0; i < population_size; i++)
   {
      Vector<double> individual = get_individual(i); 
           
      population_norm[i] = individual.calculate_norm();     
   }               
   
   return(population_norm);            
}


// double calculate_mean_performance(void) const method

/// This method returns the mean value of the individuals performance. 

double EvolutionaryAlgorithm::calculate_mean_performance(void) const
{
   return(performance.calculate_mean());
}


// double calculate_standard_deviation_performance(void) const method

/// This method returns the standard deviation value of the individuals performance. 

double EvolutionaryAlgorithm::calculate_standard_deviation_performance(void) const
{
   return(performance.calculate_standard_deviation());
}


// Training parameters

// const bool& get_elitism(void) const method

/// This method returns true if elitism is to be performed, and false otherwise. 

const bool& EvolutionaryAlgorithm::get_elitism(void) const
{
   return(elitism);
}


// const double& get_selective_pressure(void) const method 

/// This method returns the selective pressure value.

const double& EvolutionaryAlgorithm::get_selective_pressure(void) const
{
   return(selective_pressure);
}


// const double& get_recombination_size(void) const method

/// This method returns the recombination size value.

const double& EvolutionaryAlgorithm::get_recombination_size(void) const
{
   return(recombination_size);
}


// const double& get_mutation_rate(void) const method

/// This method returns the mutation rate value.

const double& EvolutionaryAlgorithm::get_mutation_rate(void) const
{
   return(mutation_rate);
}


// const double& get_mutation_range(void) const method

/// This method returns the mutation range value.

const double& EvolutionaryAlgorithm::get_mutation_range(void) const
{
   return(mutation_range);
}


// const double& get_mean_performance_goal(void) const method

/// This method returns the mean performance value of the population at which training will stop.

const double& EvolutionaryAlgorithm::get_mean_performance_goal(void) const
{
   return(mean_performance_goal);
}


// const double& get_standard_deviation_performance_goal(void) const method

/// This method returns the standard deviation of the performance at which training will stop.

const double& EvolutionaryAlgorithm::get_standard_deviation_performance_goal(void) const
{
   return(standard_deviation_performance_goal);
}


// const unsigned int& get_maximum_generations_number(void) const method

/// This method returns the maximum number of generations to train. 

const unsigned int& EvolutionaryAlgorithm::get_maximum_generations_number(void) const
{
   return(maximum_generations_number);
}


// void set_elitism(const bool&) method

/// This method sets a new elitism flag to the evolutionary algorithm. 
/// If elitism is set to true the best individual of each generation will be always selected for recombination. 
/// @param new_elitism Elitism flag.

void EvolutionaryAlgorithm::set_elitism(const bool& new_elitism)
{
   elitism = new_elitism;
}


// void set_selective_pressure(const double&) method

/// This method sets a new value for the selective pressure parameter.
/// Linear ranking allows values for the selective pressure between 1 and 2.
///
/// @param new_selective_pressure Selective pressure value. This must be between 1 and 2 for linear ranking fitness
/// assignment. 

void EvolutionaryAlgorithm::set_selective_pressure(const double& new_selective_pressure)
{
   switch(fitness_assignment_method)
   {
      case LinearRanking:
	  {
         if(new_selective_pressure < 1.0 || new_selective_pressure > 2.0)
         {
            std::ostringstream buffer;

            buffer << "OpenNN Exception: EvolutionaryAlgorithm class.\n"
                   << "void set_selective_pressure(const double&) method. "
                   << "Case linear ranking.\n"
                   << "Selective pressure must be a value between 1 and 2.\n";

            throw std::logic_error(buffer.str().c_str());	        
         }
         
	     // Set selective pressure

	     selective_pressure = new_selective_pressure;
	  }
      break;
   }
}


// void set_recombination_size(const double&) method

/// This method sets a new value for the recombination size parameter.
/// The recombination size value must be equal or greater than 0.
///
/// @param new_recombination_size Recombination size value. This must be equal or greater than 0.

void EvolutionaryAlgorithm::set_recombination_size(const double& new_recombination_size)
{
   if(new_recombination_size < 0.0)
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: EvolutionaryAlgorithm class.\n"
             << "void set_recombination_size(const double&) method.\n"
             << "Recombination size must be equal or greater than 0.\n";

      throw std::logic_error(buffer.str().c_str());	  
   }

   // Set recombination size

   recombination_size = new_recombination_size;
}


// void set_mutation_rate(const double&) method

/// This method sets a new value for the mutation rate parameter.
/// The mutation rate value must be between 0 and 1.
///
/// @param new_mutation_rate Mutation rate value. This value must lie in the interval [0,1]. 

void EvolutionaryAlgorithm::set_mutation_rate(const double& new_mutation_rate)
{
   // Control sentence

   if(new_mutation_rate < 0.0 || new_mutation_rate > 1.0)
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: EvolutionaryAlgorithm class.\n"
             << "void set_mutation_rate(const double&) method.\n"
             << "Mutation rate must be a value between 0 and 1.\n";

      throw std::logic_error(buffer.str().c_str());	  
   }

   // Set mutation rate

   mutation_rate = new_mutation_rate;
}


// void set_mutation_range(const double&) method

/// This method sets a new value for the mutation range parameter.
/// The mutation range value must be 0 or a positive number. 
///
/// @param new_mutation_range Mutation range value. This must be equal or greater than 0.

void EvolutionaryAlgorithm::set_mutation_range(const double& new_mutation_range)
{
   // Control sentence

   if(new_mutation_range < 0.0)
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: EvolutionaryAlgorithm class.\n"
             << "void set_mutation_range(const double&) method.\n"
             << "Mutation range must be equal or greater than 0.\n";

      throw std::logic_error(buffer.str().c_str());	  
   }

   // Set mutation range

   mutation_range = new_mutation_range;
}


// void set_maximum_generations_number(unsigned int) method

/// This method sets a new value for the maximum number of generations to perform_training.
/// The maximum number of generations value must be a positive number. 
/// @param new_maximum_generations_number Maximum number of generations value.

void EvolutionaryAlgorithm::set_maximum_generations_number(const unsigned int& new_maximum_generations_number)
{
   // Control sentence (if debug)

   #ifdef _DEBUG 

   if(new_maximum_generations_number == 0)
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: EvolutionaryAlgorithm class.\n"
             << "void set_maximum_generations_number(unsigned int) method.\n"
             << "Maximum number of generations must be greater than 0.\n";

      throw std::logic_error(buffer.str().c_str());	  
   }

   #endif

   // Set maximum number of generations

   maximum_generations_number = new_maximum_generations_number;
}



// void set_mean_performance_goal(const double&) method

/// This method sets a new value for the mean performance goal stopping criterium.
/// @param new_mean_performance_goal Goal value for the mean performance of the population. 

void EvolutionaryAlgorithm::set_mean_performance_goal(const double& new_mean_performance_goal)
{
   mean_performance_goal = new_mean_performance_goal;
}


// void set_standard_deviation_performance_goal(const double&) method

/// This method sets a new value for the standard deviation performance goal stopping criterium.
/// @param new_standard_deviation_performance_goal Goal for the standard deviation performance of the population. 

void EvolutionaryAlgorithm::set_standard_deviation_performance_goal(const double& new_standard_deviation_performance_goal)
{
   // Control sentence (if debug)

   #ifdef _DEBUG 

   if(new_standard_deviation_performance_goal < 0.0)
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: EvolutionaryAlgorithm class.\n"
             << "void set_standard_deviation_performance_goal(const double&) method.\n"
             << "Standard deviation of performance goal must be equal or greater than 0.\n";

      throw std::logic_error(buffer.str().c_str());	  
   }

   #endif

   // Set standard deviation of performance goal

   standard_deviation_performance_goal = new_standard_deviation_performance_goal;

}

// void set_fitness_assignment_method(FitnessAssignmentMethod) method

/// This method sets a new fitness assignment method to be used for training.
/// @param new_fitness_assignment_method Fitness assignment method chosen for training.

void EvolutionaryAlgorithm::set_fitness_assignment_method
(const EvolutionaryAlgorithm::FitnessAssignmentMethod& new_fitness_assignment_method)
{
   fitness_assignment_method = new_fitness_assignment_method;
}


// void set_selection_method(SelectionMethod) method

/// This method sets a new selection method to be used for training.
///
/// @param new_selection_method Selection method chosen for training.

void EvolutionaryAlgorithm::
set_selection_method(const EvolutionaryAlgorithm::SelectionMethod& new_selection_method)
{
   selection_method = new_selection_method;
}


// void set_recombination_method(RecombinationMethod) method

/// This method sets a new recombination method to be used for training.
///
/// @param new_recombination_method Recombination method chosen for training. 

void EvolutionaryAlgorithm
::set_recombination_method(const EvolutionaryAlgorithm::RecombinationMethod& new_recombination_method)
{
   recombination_method = new_recombination_method;
}


// void set_mutation_method(MutationMethod) method

/// This method sets a new mutation method to be used for training.
///
/// @param new_mutation_method Mutation method chosen for training. 

void EvolutionaryAlgorithm::set_mutation_method(const EvolutionaryAlgorithm::MutationMethod& new_mutation_method)
{
   mutation_method = new_mutation_method;
}	


// void initialize_population(const double&) method

/// This method initializes the population matrix with a given value.
/// @param new_value Initialization value. 

void EvolutionaryAlgorithm::initialize_population(const double& new_value)
{
   population.initialize(new_value);
}



// void initialize_population_uniform(void) method

/// This method initializes the parameters of all the individuals in the population at random, with values 
/// comprised between -1 and 1.

void EvolutionaryAlgorithm::initialize_population_uniform(void)
{
   population.initialize_uniform();
}


// void initialize_population_uniform(const double&, const double&) method

/// This method initializes the parameters of all the individuals in the population at random, with values 
/// comprised between a minimum and a maximum value.
///
/// @param minimum Minimum initialization value.
/// @param maximum Maximum initialization value.

void EvolutionaryAlgorithm::initialize_population_uniform(const double& minimum, const double& maximum)
{
   population.initialize_uniform(minimum, maximum);
}


// void initialize_population_uniform(Vector<double>, Vector<double>) method

/// This method initializes the parameters of all the individuals in the population at random, with values 
/// comprised between different minimum and maximum values for each variable.
///
/// @param minimum Vector of minimum initialization values.
/// @param maximum Vector of maximum initialization values.

void EvolutionaryAlgorithm::initialize_population_uniform(const Vector<double>& minimum, const Vector<double>& maximum)
{
   const NeuralNetwork* neural_network_pointer = performance_functional_pointer->get_neural_network_pointer();

   const unsigned int parameters_number = neural_network_pointer->count_parameters_number();   

   // Control sentence (if debug)

   #ifdef _DEBUG 

   const unsigned int minimum_size = minimum.size();
   const unsigned int maximum_size = maximum.size();

   if(minimum_size != parameters_number || maximum_size != parameters_number)   
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: EvolutionaryAlgorithm class.\n"
             << "void initialize_population_uniform(Vector<double>, Vector<double>).\n"
             << "Minimum value and maximum value sizes must be equal to number of parameters.\n";
 
      throw std::logic_error(buffer.str().c_str());	  
   }

   #endif

   Vector<double> individual(parameters_number);

   const unsigned int population_size = get_population_size();

   for(unsigned int i = 0; i < population_size; i++)
   {
      individual.initialize_uniform(minimum, maximum);

      set_individual(i, individual);
   }
}


// void initialize_population_normal(void) method

/// This method initializes the parameters of all the individuals in the population with random values chosen
/// from a normal distribution with mean 0 and standard deviation 1.

void EvolutionaryAlgorithm::initialize_population_normal(void)
{
   population.initialize_normal();
}


// void initialize_population_normal(const double&, const double&) method

/// This method initializes the parameters of all the individuals in the population with random values chosen
/// from a normal distribution with a given mean and a given standard deviation.
///
/// @param mean Mean of normal distribution.
/// @param standard_deviation Standard deviation of normal distribution.

void EvolutionaryAlgorithm::initialize_population_normal(const double& mean, const double& standard_deviation)
{
   population.initialize_normal(mean, standard_deviation);
}


// void initialize_population_normal(Vector<double>, Vector<double>) method

/// This method initializes the parameters of all the individuals in the population with random values chosen
/// from normal distributions with different mean and standard deviation for each free parameter.
///
/// @param mean Vector of mean values.
/// @param standard_deviation Vector of standard deviation values.

void EvolutionaryAlgorithm::
initialize_population_normal(const Vector<double>& mean, const Vector<double>& standard_deviation)
{
   const NeuralNetwork* neural_network_pointer = performance_functional_pointer->get_neural_network_pointer();

   const unsigned int parameters_number = neural_network_pointer->count_parameters_number();   

   // Control sentence (if debug)

   #ifdef _DEBUG 

   const unsigned int mean_size = mean.size();
   const unsigned int standard_deviation_size = standard_deviation.size();

   if(mean_size != parameters_number || standard_deviation_size != parameters_number)   
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: EvolutionaryAlgorithm class.\n"
             << "void initialize_population_normal(Vector<double>, Vector<double>).\n"
             << "Mean and standard deviation sizes must be equal to number of parameters.\n";
 
      throw std::logic_error(buffer.str().c_str());	  
   }

   #endif

   Vector<double> individual(parameters_number);

   const unsigned int population_size = get_population_size();

   for(unsigned int i = 0; i < population_size; i++)
   {
      individual.initialize_normal(mean, standard_deviation);

      set_individual(i, individual);
   }
}


// void perform_fitness_assignment(void) method

/// This method assigns a fitness value to all the individuals in the population according to the finess assignment operator.

void EvolutionaryAlgorithm::perform_fitness_assignment(void)
{
   switch(fitness_assignment_method)
   {
      case LinearRanking:
      { 
         perform_linear_ranking_fitness_assignment();
      }

      break;

	  default:
	  {
         std::ostringstream buffer;

         buffer << "OpenNN Exception: EvolutionaryAlgorithm class.\n"
                << "void perform_fitness_assignment(void).\n"
                << "Unknown fitness assignment method.\n";
 
         throw std::logic_error(buffer.str().c_str());	     
	  }
	  break;
   }
}


// void perform_selection(void) method

/// This method selects for recombination some individuals from the population according to the selection operator. 

void EvolutionaryAlgorithm::perform_selection(void)
{
   switch(selection_method)
   {
      case RouletteWheel:
      {
         perform_roulette_wheel_selection();
      }
      break;

      case StochasticUniversalSampling:
      {
         perform_stochastic_universal_sampling_selection();
      }
      break;

	  default:
	  {
         std::ostringstream buffer;

         buffer << "OpenNN Exception: EvolutionaryAlgorithm class.\n"
                << "void perform_selection(void).\n"
                << "Unknown selection method.\n";
 
         throw std::logic_error(buffer.str().c_str());	     
	  }
	  break;
   }
}


// void perform_recombination(void) method

/// This method recombinates the selected individuals according to the recombination operator. 

void EvolutionaryAlgorithm::perform_recombination(void)
{
   switch(recombination_method)
   {
      case Intermediate:
      {
         perform_intermediate_recombination();
      }
      break;

      case Line:
      {
         perform_line_recombination();
      } 
      break;

	  default:
	  {
         std::ostringstream buffer;
 
         buffer << "OpenNN Exception: EvolutionaryAlgorithm class.\n"
                << "void perform_recombination(void).\n"
                << "Unknown recombination method.\n";
 
         throw std::logic_error(buffer.str().c_str());	     
	  }
	  break;
   }
}


// void perform_mutation(void) method

/// This method mutates the population matrix according to the mutation operator. 

void EvolutionaryAlgorithm::perform_mutation(void)
{
   switch(mutation_method)
   {
      case Normal:
      {
         perform_normal_mutation();
      }
      break;

      case Uniform:
      {
         perform_uniform_mutation();
      }
      break;

	  default:
	  {
         std::ostringstream buffer;

         buffer << "OpenNN Exception: EvolutionaryAlgorithm class.\n"
                << "void perform_mutationg(void).\n"
                << "Unknown mutation method.\n";
 
         throw std::logic_error(buffer.str().c_str());	     
	  }
	  break;
   }
}


// void evolve_population(void) method

/// This method generates a new population matrix by applying fitness assignment, selection, recombination and mutation. 

void EvolutionaryAlgorithm::evolve_population(void)
{
   // Fitness assignment
  
   perform_fitness_assignment();

   // Selection

   perform_selection();

   // Recombination

   perform_recombination();

   // Mutation

   perform_mutation();
}


// void evaluate_population(void) method

/// This method evaluates the performance functional of all individuals in the population. 
/// Results are stored in the performance vector.

void EvolutionaryAlgorithm::evaluate_population(void)
{
   // Control sentence (if debug)

   #ifdef _DEBUG 

   if(performance_functional_pointer == NULL)   
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: EvolutionaryAlgorithm class.\n"
             << "void evaluate_population(void).\n"
             << "Objective functional pointer is NULL.\n";
 
      throw std::logic_error(buffer.str().c_str());	  
   }

   #endif

   // Multilayer perceptron

   const NeuralNetwork* neural_network_pointer = performance_functional_pointer->get_neural_network_pointer();

   // Control sentence (if debug)

   #ifdef _DEBUG 

   if(!neural_network_pointer)   
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: EvolutionaryAlgorithm class.\n"
             << "void evaluate_population(void).\n"
             << "Multilayer perceptron pointer is NULL.\n";
 
      throw std::logic_error(buffer.str().c_str());	  
   }

   #endif

   const unsigned int parameters_number = neural_network_pointer->count_parameters_number();

   Vector<double> individual(parameters_number);

   // Evaluate performance functional for all individuals

   unsigned int population_size = get_population_size();

   for(unsigned int i = 0; i < population_size; i++)
   {
      individual = get_individual(i);

      performance[i] = performance_functional_pointer->calculate_evaluation(individual);
      
      if(!(performance[i] > -1.0e99 && performance[i] < 1.0e99))
      {
         std::ostringstream buffer;

         buffer << "OpenNN Exception: EvolutionaryAlgorithm class.\n"
                << "void evaluate_population(void) method.\n"
                << "Performance of individual " << i << " is not a real number.\n";

         throw std::logic_error(buffer.str().c_str());	     
      }                
   }
}


// void perform_linear_ranking_fitness_assignment(void) method
//
/// This method ranks all individuals in the population by their objective performance, so that the least fit 
/// individual has rank 1 and the fittest individual has rank [population size].
/// It then assigns them a fitness value linearly proportional to their rank. Results are stored in the fitness 
/// vector.

void EvolutionaryAlgorithm::perform_linear_ranking_fitness_assignment(void)
{
   // Sorted performance vector

   unsigned int population_size = get_population_size();

   Vector<double> sorted_performance(population_size);

   sorted_performance = performance;

   std::sort(sorted_performance.begin(), sorted_performance.end(), std::less<double>());

   // Rank vector

   Vector<int> rank(population_size);

   for(unsigned int i = 0; i < population_size; i++)
   {
      for(unsigned int j = 0; j < population_size; j++)
      {
         if(performance[j] == sorted_performance[i])
         {
            rank[j] = population_size - i;
         }
      }
   }

   // Perform linear ranking fitness assignment

   for(unsigned int i = 0; i < population_size; i++)
   {
      fitness[i] = 2.0 - selective_pressure
      + 2.0*(selective_pressure - 1.0)*(rank[i] - 1.0)/(population_size - 1.0);
      
      if(!(fitness[i] > -1.0e99 && fitness[i] < 1.0e99))
      {
         std::ostringstream buffer;

         buffer << "Flooe Error: EvolutionaryAlgorithm class.\n"
                << "void perform_linear_ranking_fitness_assignment(void) method.\n"
                << "Fitness of individual " << i << " is not a real number.\n";

         throw std::logic_error(buffer.str().c_str());	     
      }          
   }
}


// void perform_roulette_wheel_selection(void) method

/// This metod performs selection with roulette wheel selection. It selects half of the individuals from the 
/// population. 
/// Results are stored in the selection vector. 

void EvolutionaryAlgorithm::perform_roulette_wheel_selection(void)
{
   // Set selection vector to false 

   selection.initialize(false);

   unsigned int population_size = get_population_size();

   unsigned int selected_individuals_number = population_size/2;

   // Cumulative fitness vector

   Vector<double> cumulative_fitness(population_size);

   cumulative_fitness[0] = fitness[0]; 

   for(unsigned int i = 1; i < population_size; i++)
   {
      cumulative_fitness[i] = cumulative_fitness[i-1] + fitness[i];
   }

   // Select individuals until the desired number of selections is obtained

   unsigned int selected_individuals_count = 0;

   if(elitism)
   {
      unsigned int best_individual_index = performance.calculate_minimal_index();

      selection[best_individual_index] = true; 

      selected_individuals_count++;
   }

   do
   {
      // Random number between 0 and total cumulative fitness

      double random = (double)rand()/(RAND_MAX+1.0);

      double pointer = cumulative_fitness[population_size-1]*random;

      // Perform selection

      if(pointer < cumulative_fitness[0])
      {
         if(selection[0] == false)
         {
            selection[0] = true;
            selected_individuals_count++;
         }
      }
      
      for(unsigned int i = 1; i < population_size; i++)
      {
         if(pointer < cumulative_fitness[i] && pointer >= cumulative_fitness[i-1])
         {
            if(selection[i] == false)
            {
               selection[i] = true;
               selected_individuals_count++;
            }
         }
      }
   }while(selected_individuals_count != selected_individuals_number);

   // Control sentence

   if(selected_individuals_count != selected_individuals_number)
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: EvolutionaryAlgorithm class.\n"
             << "void perform_roulette_wheel_selection(void) method.\n"
             << "Count number of selected individuals is not equal to number of selected individuals.\n";

      throw std::logic_error(buffer.str().c_str());
   }
}


// void perform_stochastic_universal_sampling_selection(void) method
//
/// This metod performs selection with stochastic universal sampling. It selects half of the individuals from the
/// population. 
/// Results are stored in the selection vector. 

void EvolutionaryAlgorithm::perform_stochastic_universal_sampling_selection(void)
{
   // Set selection vector to false

   selection.initialize(false);

   unsigned int population_size = get_population_size();
 
   unsigned int selected_individuals_number = population_size/2;

   Vector<double> cumulative_fitness(population_size);

   Vector<double> pointer(selected_individuals_number);

   // Cumulative fitness vector

   cumulative_fitness[0] = fitness[0];

   for(unsigned int i = 1; i < population_size; i++)
   {  
      cumulative_fitness[i] = cumulative_fitness[i-1] + fitness[i];
   }


   // Pointer vector

   // Random number between 0 and totalCumulativeFitnees/(double)selected_individuals_number 

   double random = (double)rand()/(RAND_MAX+1.0);

   pointer[0] = random
   *cumulative_fitness[population_size-1]/(double)selected_individuals_number;

   for(unsigned int i = 1; i < selected_individuals_number; i++)
   {
      pointer[i] = pointer[i-1] 
      + cumulative_fitness[population_size-1]/(double)selected_individuals_number;
   }

   // Selection vector

   unsigned int selected_individuals_count = 0;

//   if(elitism)
//   {
//      unsigned int best_individual_index = performance.calculate_minimal_index();
//
//      selection[best_individual_index] = true; 
//
//      selected_individuals_count++;
//   }

   if(pointer[0] <= cumulative_fitness[0])
   {
      selection[0] = true;
      selected_individuals_count++;
   }

   for(unsigned int i = 0; i < selected_individuals_number; i++)
   {
      for(unsigned int j = 1; j < population_size; j++)
      {
         if(pointer[i] <= cumulative_fitness[j] && pointer[i] > cumulative_fitness[j-1])
         {
            selection[j] = true;
            selected_individuals_count++;
         }
      }
   }

   // Number of selected individuals control sentence 

   if(selected_individuals_count != selected_individuals_number)
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: EvolutionaryAlgorithm class.\n"
             << "void perform_stochastic_universal_sampling_selection(void) method.\n"
             << "Count number of selected individuals is not equal to number of selected individuals.\n";

      throw std::logic_error(buffer.str().c_str());	  
   }
}


// void perform_intermediate_recombination(void) method

/// This method performs inediate recombination between pairs of selected individuals to generate a new 
/// population. 
/// Each selected individual is to be recombined with two other selected individuals chosen at random. 
/// Results are stored in the population matrix.

void EvolutionaryAlgorithm::perform_intermediate_recombination(void)
{
   unsigned int population_size = get_population_size();

   const NeuralNetwork* neural_network_pointer = performance_functional_pointer->get_neural_network_pointer();
     
   const unsigned int parameters_number = neural_network_pointer->count_parameters_number();

   Matrix<double> new_population(population_size, parameters_number);

   unsigned int count = 0;
   for(unsigned int i = 0; i < population_size; i++)
   {
      if(selection[i] == true)
      count ++;        
   }

   Vector<double> parent_1(parameters_number);
   Vector<double> parent_2(parameters_number);

   Vector<double> offspring(parameters_number);

   Matrix<int> recombination(population_size, 2);

   // Start recombination   

   unsigned int new_population_size_count = 0;

   for(unsigned int i = 0; i < population_size; i++)
   {
      if(selection[i] == true)
      {
         // Set parent 1

         parent_1 = get_individual(i);

         // Generate 2 offspring with parent 1

         for(unsigned int j = 0; j < 2; j++)
         {
            // Choose parent 2 at random among selected individuals   

            bool parent_2_candidate = false;

            do{
               // Integer random number beteen 0 and population size

               double random = (double)rand()/(RAND_MAX+1.0);

               unsigned int parent_2_candidate_index = (unsigned int)(population_size*random);

               // Check if candidate for parent 2 is ok

               if(selection[parent_2_candidate_index] == true && parent_2_candidate_index != i)
               {
                  parent_2_candidate = true;

                  recombination[new_population_size_count][0] = i;

                  recombination[new_population_size_count][1] = parent_2_candidate_index;

                  parent_2 = get_individual(parent_2_candidate_index);

                  // Perform inediate recombination between parent 1 and parent 2

                  for(unsigned int j = 0; j < parameters_number; j++)
                  {
                     // Choose the scaling factor to be a random number between
                     // -recombination_size and 1+recombination_size for each
                     // variable anew.

                     double random = (double)rand()/(RAND_MAX+1.0);

                     double scaling_factor = -1.0*recombination_size + (1.0 + recombination_size)*random;

                     offspring[j] = scaling_factor*parent_1[j] + (1.0 - scaling_factor)*parent_2[j];
                  }

                  // Add offspring to new_population matrix

                  new_population.set_row(new_population_size_count, offspring);   
                  
                  new_population_size_count++;
               }
            }while(parent_2_candidate != true);
         }
      }
   }

   // Count number of new individuals control sentence

   if(new_population_size_count != population_size)
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: EvolutionaryAlgorithm class.\n"
             << "void perform_intermediate_recombination(void) method.\n"
             << "Count new population size is not equal to population size.\n";

      throw std::logic_error(buffer.str().c_str());	  
   }

   // Set new population

   population = new_population;
}


// void perform_line_recombination(void) method

/// This method performs line recombination between pairs of selected individuals to generate a new population. 
/// Each selected individual is to be recombined with two other selected individuals chosen at random. 
/// Results are stored in the population matrix.

void EvolutionaryAlgorithm::perform_line_recombination(void)
{
   const unsigned int population_size = get_population_size();

   const NeuralNetwork* neural_network_pointer = performance_functional_pointer->get_neural_network_pointer();
     
   const unsigned int parameters_number = neural_network_pointer->count_parameters_number();

   Matrix<double> new_population(population_size, parameters_number);

   Vector<double> parent_1(parameters_number);
   Vector<double> parent_2(parameters_number);

   Vector<double> offspring(parameters_number);
   Vector<double> parent_1_term(parameters_number);
   Vector<double> parent_2_term(parameters_number);

   Matrix<int> recombination(population_size, 2);

   // Start recombination   

   unsigned int new_population_size_count = 0;

   for(unsigned int i = 0; i < population_size; i++)
   {
      if(selection[i] == true)
      {
         // Set parent 1

         parent_1 = get_individual(i);

         // Generate 2 offspring with parent 1

         for(unsigned int j = 0; j < 2; j++)
         {
            // Choose parent 2 at random among selected individuals   

            bool parent_2_candidate = false;

            do
            {
               // Integer random number beteen 0 and population size

               double random = (double)rand()/(RAND_MAX + 1.0);

               unsigned int parent_2_candidate_index = (unsigned int)(population_size*random);

               // Check if candidate for parent 2 is ok

               if(selection[parent_2_candidate_index] == true && parent_2_candidate_index != i)
               {
                  parent_2_candidate = true;

                  recombination[new_population_size_count][0] = i;
                  recombination[new_population_size_count][1] = parent_2_candidate_index;

                  parent_2 = get_individual(parent_2_candidate_index);

                  // Perform inediate recombination between parent 1
                  // and parent 2

                  // Choose the scaling factor to be a random number between
                  // -recombination_size and 1+recombination_size for all
                  // variables.

                  double random = (double)rand()/(RAND_MAX+1.0);

                  double scaling_factor = -1.0*recombination_size 
                  + (1.0 + recombination_size)*random;

                  parent_1_term = parent_1*scaling_factor;
                  parent_2_term = parent_2*(1.0 - scaling_factor); 

                  offspring = parent_1_term + parent_2_term;

                  // Add offspring to new_population matrix

                  new_population.set_row(new_population_size_count, offspring);   

                  new_population_size_count++;
               }
            }while(parent_2_candidate == false);
         }
      }
   }

   // Count new population size control sentence

   if(new_population_size_count != population_size)
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: EvolutionaryAlgorithm class.\n"
             << "void perform_line_recombination(void) method.\n"
             << "Count new population size is not equal to population size.\n";

      throw std::logic_error(buffer.str().c_str());	  
   }

   // Set new population

   population = new_population;
}


// void perform_normal_mutation(void) method

/// This method performs normal mutation to all individuals in order to generate a new population. 
/// Results are stored in the population matrix.

void EvolutionaryAlgorithm::perform_normal_mutation(void)
{
   unsigned int population_size = get_population_size();

   const NeuralNetwork* neural_network_pointer = performance_functional_pointer->get_neural_network_pointer();
     
   const unsigned int parameters_number = neural_network_pointer->count_parameters_number();

   Vector<double> individual(parameters_number);

   for(unsigned int i = 0; i < population_size; i++)
   {
      individual = get_individual(i);

      for(unsigned int j = 0; j < parameters_number; j++)
      {
         // Random number between 0 and 1

         double pointer = calculate_random_uniform(0.0, 1.0);

         if(pointer < mutation_rate)
         {
            individual[j] += calculate_random_normal(0.0, mutation_range);
         }
      }

      set_individual(i, individual);
   }
}  


// void perform_uniform_mutation(void) method

/// This method performs uniform mutation to all individuals in order to generate a new population. 
/// Results are stored in the population matrix.

void EvolutionaryAlgorithm::perform_uniform_mutation(void)
{
   const unsigned int population_size = get_population_size();

   const NeuralNetwork* neural_network_pointer = performance_functional_pointer->get_neural_network_pointer();
     
   const unsigned int parameters_number = neural_network_pointer->count_parameters_number();
   
   Vector<double> individual(parameters_number, 0.0);

   for(unsigned int i = 0; i < population_size; i++)
   {
      individual = get_individual(i);

      for(unsigned int j = 0; j < parameters_number; j++)
      {
         // random number between 0 and 1

         double pointer = (double)rand()/(RAND_MAX+1.0);

         if(pointer < mutation_rate)
         {
            // random number between 0 and 1

            double random = (double)rand()/(RAND_MAX+1.0);

            double uniformlyDistributedRandomNumber
            = (-1.0 + 2.0*random)*mutation_range;

            individual[j] += uniformlyDistributedRandomNumber;
         }
      }

      set_individual(i, individual);
   }
}


// std::string to_string(void) const method

/// This method returns a string representation of the current evolutionary algorithm resutls structure. 

std::string EvolutionaryAlgorithm::EvolutionaryAlgorithmResults::to_string(void) const
{
   std::ostringstream buffer;

   // Population history

   if(!population_history.empty())
   {
	   if(!population_history[0].empty())
	   {
          buffer << "% Population history:\n"
                 << population_history << "\n"; 
	   }
   }

   // Best individual history

   if(!best_individual_history.empty())
   {
      if(!population_history[0].empty())
	  {
          buffer << "% Best individual history:\n"
                 << best_individual_history << "\n"; 
	  }
   }

   // Mean norm history   

   if(!mean_norm_history.empty())
   {
       buffer << "% Mean norm history:\n"
              << mean_norm_history << "\n"; 
   }

   // Standard deviation norm history

   if(!standard_deviation_norm_history.empty())
   {
       buffer << "% Standard deviation norm history:\n"
              << standard_deviation_norm_history << "\n"; 
   }

   // Best norm history 

   if(!best_norm_history.empty())
   {
       buffer << "% Best norm history:\n"
              << best_norm_history << "\n"; 
   }

   // Evaluation history

   if(!evaluation_history.empty())
   {
       buffer << "% Evaluation history:\n"
              << evaluation_history << "\n"; 
   }

   // Mean evaluation history

   if(!mean_evaluation_history.empty())
   {
       buffer << "% Mean evaluation history:\n"
              << mean_evaluation_history << "\n"; 
   }

   // Standard deviation evaluation history

   if(!standard_deviation_evaluation_history.empty())
   {
       buffer << "% Standard deviation evaluation history:\n"
              << standard_deviation_evaluation_history << "\n"; 
   }

   // Best evaluation history

   if(!best_evaluation_history.empty())
   {
       buffer << "% Best evaluation history:\n"
              << best_evaluation_history << "\n"; 
   }

   // Generalization evaluation history

   if(!generalization_evaluation_history.empty())
   {
       buffer << "% Generalization evaluation history:\n"
              << generalization_evaluation_history << "\n"; 
   }

   // Mean generalization evaluation history

   if(!mean_generalization_evaluation_history.empty())
   {
       buffer << "% Mean generalization evaluation history:\n"
              << mean_generalization_evaluation_history << "\n"; 
   }

   // Standard deviation generalization evaluation history

   if(!standard_deviation_generalization_evaluation_history.empty())
   {
       buffer << "% Standard deviation generalization evaluation history:\n"
              << standard_deviation_generalization_evaluation_history << "\n"; 
   }

   // Best generalization evaluation history

   if(!best_generalization_evaluation_history.empty())
   {
       buffer << "% Best generalization evaluation history:\n"
              << best_generalization_evaluation_history << "\n"; 
   }

   // Elapsed time history   

   if(!elapsed_time_history.empty())
   {
       buffer << "% Elapsed time history:\n"
              << elapsed_time_history << "\n"; 
   }

   return(buffer.str());
}


// void resize_training_history(const unsigned int&) method

/// This method resizes all the training history variables. 
/// @param new_size Size of training history variables. 

void EvolutionaryAlgorithm::EvolutionaryAlgorithmResults::resize_training_history(const unsigned int& new_size) 
{
   population_history.resize(new_size);
   best_individual_history.resize(new_size);
   mean_norm_history.resize(new_size);
   standard_deviation_norm_history.resize(new_size);
   best_norm_history.resize(new_size);
   
   evaluation_history.resize(new_size);
   mean_evaluation_history.resize(new_size);
   standard_deviation_evaluation_history.resize(new_size);
   best_evaluation_history.resize(new_size);
   generalization_evaluation_history.resize(new_size);
   mean_generalization_evaluation_history.resize(new_size);
   standard_deviation_generalization_evaluation_history.resize(new_size);
   best_generalization_evaluation_history.resize(new_size);

   elapsed_time_history.resize(new_size);
}


// EvolutionaryAlgorithmResults* perform_training(void) method

/// This method trains a neural network with an associated performance function according to the evolutionary algorithm.
/// Training occurs according to the training operators and their related parameters.
/// @todo

EvolutionaryAlgorithm::EvolutionaryAlgorithmResults* EvolutionaryAlgorithm::perform_training(void)
{
   if(display)
   {
      std::cout << "Training with the evolutionary algorithm...\n";
   }

   EvolutionaryAlgorithmResults* evolutionary_algorithm_training_results_pointer = new EvolutionaryAlgorithmResults;

   evolutionary_algorithm_training_results_pointer->resize_training_history(1+maximum_generations_number);

   // Performance functional

   #ifdef _DEBUG 

   if(performance_functional_pointer == NULL)
   {
      std::ostringstream buffer;

      std::cout << "OpenNN Exception: EvolutionaryAlgorithm class.\n"
                << "EvolutionaryAlgorithmResults* perform_training(void) method.\n"
                << "Pointer to performance functional is NULL.\n";

      throw std::logic_error(buffer.str().c_str());
   }

   #endif

   // Neural network

   NeuralNetwork* neural_network_pointer = performance_functional_pointer->get_neural_network_pointer();

   #ifdef _DEBUG 

   if(neural_network_pointer == NULL)
   {
      std::ostringstream buffer;

      std::cout << "OpenNN Exception: EvolutionaryAlgorithm class.\n"
                << "EvolutionaryAlgorithmResults* perform_training(void) method.\n"
                << "Pointer to neural network is NULL.\n";

      throw std::logic_error(buffer.str().c_str());
   }

   #endif


   #ifdef _DEBUG 

   if(population.get_rows_number() == 0)
   {
      std::ostringstream buffer;

      std::cout << "OpenNN Exception: EvolutionaryAlgorithm class.\n"
                << "EvolutionaryAlgorithmResults* perform_training(void) method.\n"
                << "population matrix is empty.\n";

      throw std::logic_error(buffer.str().c_str());
   }

   #endif


   Vector<double> population_norm;

   double mean_norm;
   double standard_deviation_norm;

   Vector<double> best_individual;
   double best_norm = 0.0;

   // Performance functional stuff

   double best_performance = 1.0e99;

   double generalization_evaluation = 0.0; 
//   double old_generalization_evaluation = 0.0;

   // Training algorithm stuff 

   unsigned int generalization_evaluation_decreases_count = 0;

   time_t beginning_time, current_time;
   time(&beginning_time);
   double elapsed_time;

   bool stop_training = false;
   
   // Main loop

   for(unsigned int generation = 0; generation <= maximum_generations_number; generation++)
   {
      // Population stuff

      if(reserve_population_history)
      {
         evolutionary_algorithm_training_results_pointer->population_history[generation] = population; 
      }

      population_norm = calculate_population_norm();

      // Mean norm 

      mean_norm = population_norm.calculate_mean();

      if(reserve_mean_norm_history)
      {
         evolutionary_algorithm_training_results_pointer->mean_norm_history[generation] = mean_norm;
      }

      // Standard deviation of norm

      standard_deviation_norm = population_norm.calculate_standard_deviation();

      if(reserve_standard_deviation_norm_history)
      {
         evolutionary_algorithm_training_results_pointer->standard_deviation_norm_history[generation] = standard_deviation_norm;                                
      }
    
      evaluate_population();

      const unsigned int population_size = get_population_size();
 
      for(unsigned int i = 0; i < population_size; i++)
      {
         if(performance[i] < best_performance)
         {
            best_individual = get_individual(i);

            neural_network_pointer->set_parameters(best_individual);

            best_norm = best_individual.calculate_norm();

            best_performance = performance[i];

            //old_generalization_evaluation = generalization_evaluation;

            generalization_evaluation = performance_functional_pointer->calculate_generalization_evaluation();
         }
      }

      // Best individual 

      if(reserve_best_individual_history)
      {
         evolutionary_algorithm_training_results_pointer->best_individual_history[generation] = best_individual;
      }

      // Best individual norm

      if(reserve_best_norm_history)
      {
         evolutionary_algorithm_training_results_pointer->best_norm_history[generation] = best_norm;
      }

      // Mean performance

      double mean_performance = performance.calculate_mean();

      if(reserve_mean_evaluation_history)
      {
         evolutionary_algorithm_training_results_pointer->mean_evaluation_history[generation] = mean_performance;
      }

      // Standard deviation of performance

      double standard_deviation_performance = performance.calculate_standard_deviation();

      if(reserve_standard_deviation_evaluation_history)
      {
         evolutionary_algorithm_training_results_pointer->standard_deviation_evaluation_history[generation] = standard_deviation_performance;
      }

      // Best performance

      if(reserve_best_evaluation_history)
      {
         evolutionary_algorithm_training_results_pointer->best_evaluation_history[generation] = best_performance;
      }

      // Generalization performance

      if(reserve_generalization_evaluation_history)
      {
         evolutionary_algorithm_training_results_pointer->generalization_evaluation_history[generation] = generalization_evaluation;
      }

      // Elapsed time

      time(&current_time);
      elapsed_time = difftime(current_time, beginning_time);

      if(reserve_elapsed_time_history)
      {
         evolutionary_algorithm_training_results_pointer->elapsed_time_history[generation] = elapsed_time;
      }

      // Training history multilayer perceptron 

      if(reserve_population_history)
      {
         evolutionary_algorithm_training_results_pointer->population_history[generation] = population;                                
      }

      if(reserve_best_individual_history)
      {
         evolutionary_algorithm_training_results_pointer->best_individual_history[generation] = best_individual;                                
      }

      if(reserve_mean_norm_history)
      {
         evolutionary_algorithm_training_results_pointer->mean_norm_history[generation] = mean_norm;                                
      }

      if(reserve_standard_deviation_norm_history)
      {
         evolutionary_algorithm_training_results_pointer->standard_deviation_norm_history[generation] = standard_deviation_norm;                                
      }

      if(reserve_best_norm_history)
      {
         evolutionary_algorithm_training_results_pointer->best_norm_history[generation] = best_norm;                                
      }

      // Training history training algorithm

      if(reserve_mean_evaluation_history)
      {
         evolutionary_algorithm_training_results_pointer->mean_evaluation_history[generation] = mean_performance;                                
      }

      if(reserve_standard_deviation_evaluation_history)
      {
         evolutionary_algorithm_training_results_pointer->standard_deviation_evaluation_history[generation] = standard_deviation_performance;                                
      }

      if(reserve_best_evaluation_history)
      {
         evolutionary_algorithm_training_results_pointer->best_evaluation_history[generation] = best_performance;                                
      }

      if(reserve_elapsed_time_history)
      {
         evolutionary_algorithm_training_results_pointer->elapsed_time_history[generation] = elapsed_time;                                
      }

      // Stopping criteria

      if(best_performance <= performance_goal)
      {
         if(display)
         {
            std::cout << "Generation " << generation << ": Performance goal reached.\n"
                      << performance_functional_pointer->write_information();
         }

		 stop_training = true;
      }

      if(mean_performance <= mean_performance_goal)
      {
         if(display)
         {
            std::cout << "Generation " << generation << ": Mean performance goal reached.\n";
         }
         
		 stop_training = true;
      }

      if(standard_deviation_performance <= standard_deviation_performance_goal)
      {
         if(display)
         {
            std::cout << "Generation " << generation << ": Standard deviation of performance goal reached.\n";
         }
         
		 stop_training = true;
      }

      else if(generalization_evaluation_decreases_count > maximum_generalization_evaluation_decreases)
      {
         if(display)
         {
            std::cout << "Generation " << generation << ": Maximum generalization performance decreases reached.\n";
            std::cout << "Generalization performance decreases: "<< generalization_evaluation_decreases_count << std::endl;
         }

         stop_training = true;
      }

      else if(elapsed_time >= maximum_time)
      {
         if(display)
         {
            std::cout << "Generation " << generation << ": Maximum training time reached.\n";
         }

		 stop_training = true;
      }

      else if(generation == maximum_generations_number)
      {
         if(display)
         {
            std::cout << "Generation " << generation << ": Maximum number of generations reached.\n";
         }

         break;
      }

      if(stop_training)
      {
         evolutionary_algorithm_training_results_pointer->final_mean_norm = mean_norm;
         evolutionary_algorithm_training_results_pointer->final_standard_deviation_norm = standard_deviation_norm;
         evolutionary_algorithm_training_results_pointer->final_best_norm = best_norm;
         evolutionary_algorithm_training_results_pointer->final_mean_performance = mean_performance;
         evolutionary_algorithm_training_results_pointer->final_standard_deviation_performance = standard_deviation_performance;
         evolutionary_algorithm_training_results_pointer->final_best_performance = best_performance;
         evolutionary_algorithm_training_results_pointer->elapsed_time = elapsed_time;

         if(display)
		 {
            std::cout << "Mean norm: " << mean_norm << "\n"
                      << "Standard deviation of norm: " << standard_deviation_norm << "\n"
                      << "Best norm: " << best_norm << "\n"
                      << "Mean performance: " << mean_performance << "\n"
                      << "Standard deviation of performance: " << standard_deviation_performance << "\n"
                      << "Best performance: " << best_performance << "\n"                  
                      << performance_functional_pointer->write_information()
                      << "Elapsed time: " << elapsed_time << ";\n";
		 }

         break;
	  }
      else if(display && generation % display_period == 0)
      {
         std::cout << "Generation " << generation << ";\n"
                   << "Mean norm: " << mean_norm << "\n" 
                   << "Standard deviation of norm: " << standard_deviation_norm << "\n"
                   << "Best norm: " << best_norm << "\n"
                   << "Mean performance: " << mean_performance << "\n"
                   << "Standard deviation of performance: " << standard_deviation_performance << "\n"
                   << "Best performance: " << best_performance << "\n"
                   << performance_functional_pointer->write_information()
                   << "Elapsed time: " << elapsed_time << ";\n";
      }

      // Update stuff

 //     old_generalization_evaluation = generalization_evaluation;

      selection.initialize(false);

      evolve_population();

   }

   return(evolutionary_algorithm_training_results_pointer);
}


// std::string write_training_algorithm_type(void) const method

std::string EvolutionaryAlgorithm::write_training_algorithm_type(void) const
{
   return("EVOLUTIONARY_ALGORITHM");
}


// TiXmlElement* to_XML(void) const method

/// This method prints to the screen the members of the evolutionary algorithm object.
///
/// Training operators:
/// <ul>
/// <li> Fitness assignment method.
/// <li> Selection method.
/// <li> Recombination method.
/// <li> Mutation method.
/// </ul>
///
/// Training parameters:
/// <ul>
/// <li> Population size.
/// <li> Selective pressure.
/// <li> Recombination size.
/// <li> Mutation rate.
/// <li> Mutation range.
/// </ul>
///
/// Stopping criteria:
/// <ul> 
/// <li> Performance goal.
/// <li> Mean performance goal.
/// <li> Standard deviation of performance goal.
/// <li> Maximum time.
/// <li> Maximum number of generations. 
/// </ul> 
///  
/// User stuff: 
/// <ul>
/// <li> Display. 
/// <li> Display period. 
/// <li> Reserve elapsed time.
/// <li> Reserve mean norm history.
/// <li> Reserve standard deviation of norm history.
/// <li> Reserve best norm history.
/// <li> Reserve mean evaluation history.
/// <li> Reserve standard deviation of evaluation history.
/// <li> Reserve best evaluation history.
/// </ul>
///
/// Population matrix. 

TiXmlElement* EvolutionaryAlgorithm::to_XML(void) const
{
   std::ostringstream buffer;

   // Training algorithm

   TiXmlElement* evolutionary_algorithm_element = new TiXmlElement("EvolutionaryAlgorithm");
   evolutionary_algorithm_element->SetAttribute("Version", 4); 

   // Fitness assignment method
   
   TiXmlElement* fitness_assignment_method_element = new TiXmlElement("FitnessAssignmentMethod");
   evolutionary_algorithm_element->LinkEndChild(fitness_assignment_method_element);

   buffer.str("");
   buffer << write_fitness_assignment_method();

   TiXmlText* fitness_assignment_method_text = new TiXmlText(buffer.str().c_str());
   fitness_assignment_method_element->LinkEndChild(fitness_assignment_method_text);

   // Selection method

   TiXmlElement* selection_method_element = new TiXmlElement("SelectionMethod");
   evolutionary_algorithm_element->LinkEndChild(selection_method_element);

   buffer.str("");
   buffer << write_selection_method();

   TiXmlText* selection_method_text = new TiXmlText(buffer.str().c_str());
   selection_method_element->LinkEndChild(selection_method_text);

   // Recombination method

   TiXmlElement* recombination_method_element = new TiXmlElement("RecombinationMethod");
   evolutionary_algorithm_element->LinkEndChild(recombination_method_element);

   buffer.str("");
   buffer << write_recombination_method();

   TiXmlText* recombination_method_text = new TiXmlText(buffer.str().c_str());
   recombination_method_element->LinkEndChild(recombination_method_text);

   // Mutation method

   TiXmlElement* mutation_method_element = new TiXmlElement("MutationMethod");
   evolutionary_algorithm_element->LinkEndChild(mutation_method_element);

   buffer.str("");
   buffer << write_mutation_method();

   TiXmlText* mutation_method_text = new TiXmlText(buffer.str().c_str());
   mutation_method_element->LinkEndChild(mutation_method_text);

   // Elitism

   TiXmlElement* elitism_element = new TiXmlElement("Elitism");
   evolutionary_algorithm_element->LinkEndChild(elitism_element);

   buffer.str("");
   buffer << elitism;

   TiXmlText* elitism_text = new TiXmlText(buffer.str().c_str());
   elitism_element->LinkEndChild(elitism_text);

   // Selective pressure. 

   TiXmlElement* selective_pressure_element = new TiXmlElement("SelectivePressure");
   evolutionary_algorithm_element->LinkEndChild(selective_pressure_element);

   buffer.str("");
   buffer << selective_pressure;

   TiXmlText* selective_pressure_text = new TiXmlText(buffer.str().c_str());
   selective_pressure_element->LinkEndChild(selective_pressure_text);

   // Recombination size. 

   TiXmlElement* recombination_size_element = new TiXmlElement("RecombinationSize");
   evolutionary_algorithm_element->LinkEndChild(recombination_size_element);

   buffer.str("");
   buffer << recombination_size;

   TiXmlText* recombination_size_text = new TiXmlText(buffer.str().c_str());
   recombination_size_element->LinkEndChild(recombination_size_text);

   // Mutation rate.

   TiXmlElement* mutation_rate_element = new TiXmlElement("MutationRate");
   evolutionary_algorithm_element->LinkEndChild(mutation_rate_element);

   buffer.str("");
   buffer << mutation_rate;

   TiXmlText* mutation_rate_text = new TiXmlText(buffer.str().c_str());
   mutation_rate_element->LinkEndChild(mutation_rate_text);

   // Mutation range

   TiXmlElement* mutation_range_element = new TiXmlElement("MutationRange");
   evolutionary_algorithm_element->LinkEndChild(mutation_range_element);

   buffer.str("");
   buffer << mutation_range;

   TiXmlText* mutation_range_text = new TiXmlText(buffer.str().c_str());
   mutation_range_element->LinkEndChild(mutation_range_text);
    
   // Mean performance goal

   TiXmlElement* mean_performance_goal_element = new TiXmlElement("MeanPerformanceGoal");
   evolutionary_algorithm_element->LinkEndChild(mean_performance_goal_element);

   buffer.str("");
   buffer << mean_performance_goal;

   TiXmlText* mean_performance_goal_text = new TiXmlText(buffer.str().c_str());
   mean_performance_goal_element->LinkEndChild(mean_performance_goal_text);

   // Standard deviation performance goal

   TiXmlElement* standard_deviation_performance_goal_element = new TiXmlElement("StandardDeviationPerformanceGoal");
   evolutionary_algorithm_element->LinkEndChild(standard_deviation_performance_goal_element);

   buffer.str("");
   buffer << standard_deviation_performance_goal;

   TiXmlText* standard_deviation_performance_goal_text = new TiXmlText(buffer.str().c_str());
   standard_deviation_performance_goal_element->LinkEndChild(standard_deviation_performance_goal_text);

   // Maximum generations number

   TiXmlElement* maximum_generations_number_element = new TiXmlElement("MaximumGenerationsNumber");
   evolutionary_algorithm_element->LinkEndChild(maximum_generations_number_element);

   buffer.str("");
   buffer << maximum_generations_number;

   TiXmlText* maximum_generations_number_text = new TiXmlText(buffer.str().c_str());
   maximum_generations_number_element->LinkEndChild(maximum_generations_number_text);

   // Maximum generalization performance decreases

   TiXmlElement* maximum_generalization_evaluation_decreases_element = new TiXmlElement("MaximumGeneralizationPerformanceDeceases");
   evolutionary_algorithm_element->LinkEndChild(maximum_generalization_evaluation_decreases_element);

   buffer.str("");
   buffer << maximum_generalization_evaluation_decreases;

   TiXmlText* maximum_generalization_evaluation_decreases_text = new TiXmlText(buffer.str().c_str());
   maximum_generalization_evaluation_decreases_element->LinkEndChild(maximum_generalization_evaluation_decreases_text);

   return(evolutionary_algorithm_element);
}


// void from_XML(TiXmlElement*) method

/// This method loads a evolutionary algorithm object from a XML element. 
/// Please mind about the file format, wich is specified in the User's Guide. 
/// @param evolutionary_algorithm_element Tiny XML element with the evolutionary algorithm object members.

void EvolutionaryAlgorithm::from_XML(TiXmlElement* evolutionary_algorithm_element)
{
   // Fitness assignment method
   
   TiXmlElement* fitness_assignment_method_element = evolutionary_algorithm_element->FirstChildElement("FitnessAssignmentMethod");

   if(fitness_assignment_method_element)
   {
      std::string new_fitness_assignment_method = fitness_assignment_method_element->GetText();

      try
      {
         set_fitness_assignment_method(new_fitness_assignment_method);
      }
      catch(std::exception& e)
      {
         std::cout << e.what() << std::endl;		 
      }
   }

   // Selection method

   TiXmlElement* selection_method_element = evolutionary_algorithm_element->FirstChildElement("SelectionMethod");

   if(selection_method_element)
   {
      std::string new_selection_method = selection_method_element->GetText();

      try
      {
         set_selection_method(new_selection_method);
      }
      catch(std::exception& e)
      {
         std::cout << e.what() << std::endl;		 
      }
   }

   // Recombination method

   TiXmlElement* recombination_method_element = evolutionary_algorithm_element->FirstChildElement("RecombinationMethod");

   if(recombination_method_element)
   {
      std::string new_recombination_method = recombination_method_element->GetText();

      try
      {
         set_recombination_method(new_recombination_method);
      }
      catch(std::exception& e)
      {
         std::cout << e.what() << std::endl;		 
      }
   }

   // Mutation method

   TiXmlElement* mutation_method_element = evolutionary_algorithm_element->FirstChildElement("MutationMethod");

   if(mutation_method_element)
   {
      std::string new_mutation_method = mutation_method_element->GetText();

      try
      {
         set_mutation_method(new_mutation_method);
      }
      catch(std::exception& e)
      {
         std::cout << e.what() << std::endl;		 
      }
   }

   // Elitism

   TiXmlElement* elitism_element = evolutionary_algorithm_element->FirstChildElement("Elitism");

   if(elitism_element)
   {
      std::string new_elitism = elitism_element->GetText();

      try
      {
         set_elitism(new_elitism != "0");
      }
      catch(std::exception& e)
      {
         std::cout << e.what() << std::endl;		 
      }
   }

   // Selective pressure. 

   TiXmlElement* selective_pressure_element = evolutionary_algorithm_element->FirstChildElement("SelectivePressure");

   if(selective_pressure_element)
   {
      double new_selective_pressure = atof(selective_pressure_element->GetText()); 

      try
      {
         set_selective_pressure(new_selective_pressure);
      }
      catch(std::exception& e)
      {
         std::cout << e.what() << std::endl;		 
      }
   }

   // Recombination size. 

   TiXmlElement* recombination_size_element = evolutionary_algorithm_element->FirstChildElement("RecombinationSize");

   if(recombination_size_element)
   {
      double new_recombination_size = atof(recombination_size_element->GetText()); 

      try
      {
         set_recombination_size(new_recombination_size);
      }
      catch(std::exception& e)
      {
         std::cout << e.what() << std::endl;		 
      }
   }

   // Mutation rate.

   TiXmlElement* mutation_rate_element = evolutionary_algorithm_element->FirstChildElement("MutationRate");

   if(mutation_rate_element)
   {
      double new_mutation_rate = atof(mutation_rate_element->GetText()); 

      try
      {
         set_mutation_rate(new_mutation_rate);
      }
      catch(std::exception& e)
      {
         std::cout << e.what() << std::endl;		 
      }
   }

   // Mutation range

   TiXmlElement* mutation_range_element = evolutionary_algorithm_element->FirstChildElement("MutationRange");

   if(mutation_rate_element)
   {
      double new_mutation_range = atof(mutation_range_element->GetText()); 

      try
      {
         set_mutation_range(new_mutation_range);
      }
      catch(std::exception& e)
      {
         std::cout << e.what() << std::endl;		 
      }
   }

   // Mean performance goal

   TiXmlElement* mean_performance_goal_element = evolutionary_algorithm_element->FirstChildElement("MeanPerformanceGoal");

   if(mean_performance_goal_element)
   {
      double new_mean_performance_goal = atof(mean_performance_goal_element->GetText()); 

      try
      {
         set_mean_performance_goal(new_mean_performance_goal);
      }
      catch(std::exception& e)
      {
         std::cout << e.what() << std::endl;		 
      }
   }

   // Standard deviation performance goal

   TiXmlElement* standard_deviation_performance_goal_element = evolutionary_algorithm_element->FirstChildElement("StandardDeviationPerformanceGoal");

   if(standard_deviation_performance_goal_element)
   {
      double new_standard_deviation_performance_goal = atof(standard_deviation_performance_goal_element->GetText()); 

      try
      {
         set_standard_deviation_performance_goal(new_standard_deviation_performance_goal);
      }
      catch(std::exception& e)
      {
         std::cout << e.what() << std::endl;		 
      }
   }

   // Maximum generations number

   TiXmlElement* maximum_generations_number_element = evolutionary_algorithm_element->FirstChildElement("MaximumGenerationsNumber");

   if(maximum_generations_number_element)
   {
      unsigned int new_maximum_generations_number = atoi(maximum_generations_number_element->GetText()); 

      try
      {
         set_maximum_generations_number(new_maximum_generations_number);
      }
      catch(std::exception& e)
      {
         std::cout << e.what() << std::endl;		 
      }
   }

   // Maximum generalization performance decreases

   TiXmlElement* maximum_generalization_evaluation_decreases_element = evolutionary_algorithm_element->FirstChildElement("MaximumGeneralizationEvaluationDecreases");

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
}


// double calculate_random_uniform(const double&, const double&) const method

double EvolutionaryAlgorithm::calculate_random_uniform(const double& minimum, const double& maximum) const
{
   double random = (double)rand()/(RAND_MAX+1.0);

   double random_uniform = minimum + (maximum-minimum)*random;

   return(random_uniform);
}


// double calculate_random_normal(const double&, const double&) const method

double EvolutionaryAlgorithm::calculate_random_normal(const double& mean, const double& standard_deviation) const
{
   double random_normal = 0.0;

   const double pi = 4.0*atan(1.0);

   double random_uniform_1;
   double random_uniform_2;

   do
   {
      random_uniform_1 = (double)rand()/(RAND_MAX+1.0);

   }while(random_uniform_1 == 0.0);

   random_uniform_2 = (double)rand()/(RAND_MAX+1.0);

   // Box-Muller transformation

   random_normal = mean + sqrt(-2.0*log(random_uniform_1))*sin(2.0*pi*random_uniform_2)*standard_deviation;

   return(random_normal);
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
