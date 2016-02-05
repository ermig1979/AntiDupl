/****************************************************************************************************************/
/*                                                                                                              */
/*   OpenNN: Open Neural Networks Library                                                                       */
/*   www.opennn.cimne.com                                                                                       */
/*                                                                                                              */
/*   E V O L U T I O N A R Y   A L G O R I T H M   C L A S S   H E A D E R                                      */
/*                                                                                                              */
/*   Roberto Lopez                                                                                              */
/*   International Center for Numerical Methods in Engineering (CIMNE)                                          */
/*   Technical University of Catalonia (UPC)                                                                    */
/*   Barcelona, Spain                                                                                           */
/*   E-mail: rlopez@cimne.upc.edu                                                                               */
/*                                                                                                              */
/****************************************************************************************************************/


#ifndef __EVOLUTIONARYALGORITHM_H__
#define __EVOLUTIONARYALGORITHM_H__


#include "training_algorithm.h"
#include "../performance_functional/performance_functional.h"

namespace OpenNN
{

///
/// This concrete class represents an evolutionary training algorithm for a performance functional of a neural network.
///

class EvolutionaryAlgorithm : public TrainingAlgorithm
{

public:

   // ENUMERATIONS

   /// Enumeration of the available training operators for fitness assignment.

   enum FitnessAssignmentMethod{LinearRanking};

   /// Enumeration of the available training operators for selection. 

   enum SelectionMethod{RouletteWheel, StochasticUniversalSampling};

   /// Enumeration of the available training operators for recombination.

   enum RecombinationMethod{Line, Intermediate};

   /// Enumeration of the available training operators for mutation.

   enum MutationMethod{Normal, Uniform};

   // DEFAULT CONSTRUCTOR

   explicit EvolutionaryAlgorithm(void);

    // GENERAL CONSTRUCTOR

   explicit EvolutionaryAlgorithm(PerformanceFunctional*);

   // XML CONSTRUCTOR

   explicit EvolutionaryAlgorithm(TiXmlElement*);


   // DESTRUCTOR

   virtual ~EvolutionaryAlgorithm(void);

      // STRUCTURES

   ///
   /// This structure contains the training results for the evolutionary algorithm. 
   ///

   struct EvolutionaryAlgorithmResults : public TrainingAlgorithm::Results
   {  
      // Training history

      /// History of the population matrix over the generations. 

      Vector< Matrix<double> > population_history;

      /// History of the best individual parameters over the generations. 

      Vector< Vector<double> > best_individual_history;

      /// History of the mean norm of the individuals over the generations. 

      Vector<double> mean_norm_history;

      /// History of the standard deviation of the individuals norm over the generations. 

      Vector<double> standard_deviation_norm_history;

      /// History of the norm of the best individual over the generations. 

      Vector<double> best_norm_history;

      /// History of the population evaluation over the generations. 

      Vector< Vector<double> > evaluation_history;

      /// History of the mean evaluation of the individuals over the generations. 

      Vector<double> mean_evaluation_history;

      /// History of the standard deviation of the population evaluation over the generations. 

      Vector<double> standard_deviation_evaluation_history;

      /// History of the evaluation of the best individual over each generations. 

      Vector<double> best_evaluation_history;

      /// History of the generalization evaluation of the best individual over each generations. 

      Vector<double> generalization_evaluation_history;

      /// History of the mean generalization evaluation over the generations. 

      Vector<double> mean_generalization_evaluation_history;

      /// History of the standard deviation of the population generalization evaluation. 

      Vector<double> standard_deviation_generalization_evaluation_history;

      /// History of the best generalization evaluation over the generations. 

      Vector<double> best_generalization_evaluation_history;

      /// History of the elapsed time over the generations. 

      Vector<double> elapsed_time_history;

      // Final values

      /// Final mean norm of the population. 

      double final_mean_norm;

      /// Final standard deviation of the population norm. 

      double final_standard_deviation_norm;

      /// Final norm of the best individual ever. 

      double final_best_norm;

      /// Final mean population performance. 

      double final_mean_performance;

      /// Final standard deviation of the population performance. 

      double final_standard_deviation_performance;

      /// Performance of the best individual ever. 

      double final_best_performance;

      /// Total elapsed time in the training process. 

      double elapsed_time;

      void resize_training_history(const unsigned int&);
      std::string to_string(void) const;
   };


   // METHODS

   // Get methods

   // Training parameters

   const double& get_warning_parameters_norm(void) const;

   const double& get_error_parameters_norm(void) const;

   // Stopping criteria

   const double& get_minimum_parameters_increment_norm(void) const;

   const double& get_minimum_performance_increase(void) const;
   const double& get_performance_goal(void) const;
   const unsigned int& get_maximum_generalization_evaluation_decreases(void) const;

   const unsigned int& get_maximum_generations_number(void) const;
   const double& get_maximum_time(void) const;

   // Reserve training history

   const bool& get_reserve_parameters_history(void) const;
   const bool& get_reserve_parameters_norm_history(void) const;

   const bool& get_reserve_evaluation_history(void) const;
   const bool& get_reserve_generalization_evaluation_history(void) const;

   const bool& get_reserve_elapsed_time_history(void) const;

   // Utilities

   const unsigned int& get_display_period(void) const;

   // Population methods

   unsigned int get_population_size(void) const;

   const Matrix<double>& get_population(void) const;

   // Training operators

   const FitnessAssignmentMethod& get_fitness_assignment_method(void) const;
   std::string write_fitness_assignment_method(void) const;

   const SelectionMethod& get_selection_method(void) const;
   std::string write_selection_method(void) const;

   const RecombinationMethod& get_recombination_method(void) const;
   std::string write_recombination_method(void) const;

   const MutationMethod& get_mutation_method(void) const;
   std::string write_mutation_method(void) const;

   // Population values

   const Vector<double>& get_performance(void) const;
   const Vector<double>& get_fitness(void) const;
   const Vector<bool>& get_selection(void) const;

   const bool& get_elitism(void) const;
   const double& get_selective_pressure(void) const;

   const double& get_recombination_size(void) const;
   const double& get_mutation_rate(void) const;
   const double& get_mutation_range(void) const;    
   const double& get_mean_performance_goal(void) const;
   const double& get_standard_deviation_performance_goal(void) const;

   const bool& get_reserve_population_history(void) const;
   const bool& get_reserve_best_individual_history(void) const;
   const bool& get_reserve_mean_norm_history(void) const;
   const bool& get_reserve_standard_deviation_norm_history(void) const;
   const bool& get_reserve_best_norm_history(void) const;

   const bool& get_reserve_mean_evaluation_history(void) const;
   const bool& get_reserve_standard_deviation_evaluation_history(void) const;
   const bool& get_reserve_best_evaluation_history(void) const;

   // Set methods

   void set(void);
   void set(PerformanceFunctional*);

   void set_default(void);

   void set_fitness_assignment_method(const FitnessAssignmentMethod&);
   void set_fitness_assignment_method(const std::string&);

   void set_selection_method(const SelectionMethod&);
   void set_selection_method(const std::string&);

   void set_recombination_method(const RecombinationMethod&);
   void set_recombination_method(const std::string&);

   void set_mutation_method(const MutationMethod&);
   void set_mutation_method(const std::string&);

   void set_population_size(const unsigned int&);

   void set_population(const Matrix<double>&);

   void set_performance(const Vector<double>&);
   void set_fitness(const Vector<double>&);
   void set_selection(const Vector<bool>&);

   void set_elitism(const bool&);
   void set_selective_pressure(const double&);
   void set_recombination_size(const double&);

   void set_mutation_rate(const double&);
   void set_mutation_range(const double&);

   void set_maximum_generations_number(const unsigned int&);
   void set_mean_performance_goal(const double&);
   void set_standard_deviation_performance_goal(const double&);

   void set_reserve_population_history(const bool&);

   void set_reserve_best_individual_history(const bool&);

   void set_reserve_mean_norm_history(const bool&);
   void set_reserve_standard_deviation_norm_history(const bool&);
   void set_reserve_best_norm_history(const bool&);

   void set_reserve_mean_evaluation_history(const bool&);
   void set_reserve_standard_deviation_evaluation_history(const bool&);
   void set_reserve_best_evaluation_history(const bool&);

   void set_reserve_all_training_history(const bool&);

   // Training parameters

   void set_warning_parameters_norm(const double&);

   void set_error_parameters_norm(const double&);

   // Stopping criteria

   void set_minimum_parameters_increment_norm(const double&);

   void set_minimum_performance_increase(const double&);
   void set_performance_goal(const double&);
   void set_maximum_generalization_evaluation_decreases(const unsigned int&);

   void set_maximum_time(const double&);

   // Reserve training history

   void set_reserve_parameters_history(const bool&);
   void set_reserve_parameters_norm_history(const bool&);

   void set_reserve_evaluation_history(const bool&);
   void set_reserve_generalization_evaluation_history(const bool&);

   void set_reserve_elapsed_time_history(const bool&);

   // Utilities

   void set_display_period(const unsigned int&);

   // Population methods

   Vector<double> get_individual(const unsigned int&) const;
   void set_individual(const unsigned int&, const Vector<double>&);

   double calculate_mean_performance(void) const;
   double calculate_standard_deviation_performance(void) const;

   // Initialization methods

   void initialize_population(const double&);

   void initialize_population_uniform(void);
   void initialize_population_uniform(const double&, const double&);
   void initialize_population_uniform(const Vector<double>&, const Vector<double>&);

   void initialize_population_normal(void);
   void initialize_population_normal(const double&, const double&);
   void initialize_population_normal(const Vector<double>&, const Vector<double>&);
    
   // Population norm methods

   Vector<double> calculate_population_norm(void) const;

   // Population performance methods

   void perform_fitness_assignment(void);
   void perform_selection(void);
   void perform_recombination(void);
   void perform_mutation(void);

   void evolve_population(void);

   void evaluate_population(void);

   // Fitness assignment methods

   void perform_linear_ranking_fitness_assignment(void);

   // Selection methods

   void perform_roulette_wheel_selection(void);
   void perform_stochastic_universal_sampling_selection(void);

   // Recombination methods

   void perform_intermediate_recombination(void);
   void perform_line_recombination(void);

   // Mutation methods

   void perform_normal_mutation(void);
   void perform_uniform_mutation(void);

   // Training methods

   EvolutionaryAlgorithmResults* perform_training(void);

   std::string write_training_algorithm_type(void) const;

   // Serialization methods

   TiXmlElement* to_XML(void) const;
   void from_XML(TiXmlElement*);


private:

   // MEMBERS

   /// Fitness assignment training operators enumeration.

   FitnessAssignmentMethod fitness_assignment_method;

   /// Selection training operators enumeration.

   SelectionMethod selection_method;

   /// Recombination training operators enumeration.

   RecombinationMethod recombination_method;

   /// Mutation training operators enumeration.

   MutationMethod mutation_method;

   // Population stuff

   /// Population matrix.

   Matrix<double> population;

   /// Performance of population.

   Vector<double> performance;

   /// Fitness of population.

   Vector<double> fitness;

   /// Selected individuals in population.

   Vector<bool> selection;
   
   // Training parameters

   bool elitism;

   /// Selective pressure. 
   /// Linear ranking allows values for the selective pressure between 1 and 2.

   double selective_pressure;

   /// Recombination size. 
   /// The recombination size value must be equal or greater than 0.

   double recombination_size;

   /// Mutation rate.
   /// The mutation rate value must be between 0 and 1.

   double mutation_rate;

   /// Mutation range.
   /// The mutation range value must be 0 or a positive number. 

   double mutation_range;

   double mean_performance_goal;

   double standard_deviation_performance_goal;

   /// Maximum number of generations to perform_training.
   
   unsigned int maximum_generations_number;

   /// True if the population history, which is a vector of matrices, is to be reserved, false otherwise.
   /// Reserving the population history can be compuationally expensive if the number of parameters, 
   /// the population size and the number of generations are big numbers. 

   unsigned int maximum_generalization_evaluation_decreases;


   bool reserve_population_history;

   /// True if the history of the best individual ever is to be reserved, and false otherwise. 
   /// The best individual history is a vector of vectors. 

   bool reserve_best_individual_history;

   /// True if the mean norm history vector is to be reserved, false otherwise.

   bool reserve_mean_norm_history;

   /// True if the standard deviation of norm history vector is to be reserved, false otherwise.

   bool reserve_standard_deviation_norm_history;

   /// True if the best norm history vector is to be reserved, false otherwise.

   bool reserve_best_norm_history;
   
   /// True if the mean evaluation history vector is to be reserved, false otherwise.

   bool reserve_mean_evaluation_history;

   /// True if the standard deviation of evaluation history vector is to be reserved, false otherwise.

   bool reserve_standard_deviation_evaluation_history;

   /// True if the best evaluation history vector is to be reserved, false otherwise.

   bool reserve_best_evaluation_history;

   /// True if the parameters history matrix is to be reserved, false otherwise.

   bool reserve_parameters_history;

   /// True if the parameters norm history vector is to be reserved, false otherwise.

   bool reserve_parameters_norm_history;

   /// True if the evaluation history vector is to be reserved, false otherwise.

   bool reserve_evaluation_history;


   /// True if the elapsed time history vector is to be reserved, false otherwise.

   bool reserve_elapsed_time_history;

   /// True if the Generalization evaluation history vector is to be reserved, false otherwise. 

   bool reserve_generalization_evaluation_history;


   // METHODS

   // Serialization methods

   double calculate_random_uniform(const double&, const double&) const;
   double calculate_random_normal(const double&, const double&) const;


   /// Value for the parameters norm at which a warning message is written to the screen. 

   double warning_parameters_norm;

   /// Value for the parameters norm at which the training process is assumed to fail. 
   
   double error_parameters_norm;


   // STOPPING CRITERIA

   /// Norm of the parameters increment vector at which training stops.

   double minimum_parameters_increment_norm;

   /// Minimum performance improvement between two successive generations. It is used as a perform_training stopping criterion.

   double minimum_performance_increase;

   /// Goal value for the performance. It is used as a perform_training stopping criterion.

   double performance_goal;

   /// Maximum training time. It is used as a perform_training stopping criterion.

   double maximum_time;

   /// Number of generations between the training showing progress.

   unsigned int display_period;

};

}

#endif


// OpenNN: An Open Source Neural Networks C++ OpenNN.
// Copyright (C) 2005-2012 Roberto Lopez 
//
// This OpenNN is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 2.1 of the License, or any later version.
//
// This OpenNN is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.

// You should have received a copy of the GNU Lesser General Public
// License along with this OpenNN; if not, write to the Free Software
// Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

