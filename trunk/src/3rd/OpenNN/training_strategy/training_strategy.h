/****************************************************************************************************************/
/*                                                                                                              */
/*   OpenNN: Open Neural Networks Library                                                                       */
/*   www.opennn.cimne.com                                                                                       */
/*                                                                                                              */
/*   T R A I N I N G   S T R A T E G Y   C L A S S   H E A D E R                                                */
/*                                                                                                              */
/*   Roberto Lopez                                                                                              */
/*   International Center for Numerical Methods in Engineering (CIMNE)                                          */
/*   Technical University of Catalonia (UPC)                                                                    */
/*   Barcelona, Spain                                                                                           */
/*   E-mail: rlopez@cimne.upc.edu                                                                               */
/*                                                                                                              */
/****************************************************************************************************************/

#ifndef __TRAININGSTRATEGY_H__
#define __TRAININGSTRATEGY_H__

// OpenNN includes

#include "../performance_functional/performance_functional.h"

#include "training_algorithm.h"

// TinyXml includes

#include "../../tinyxml/tinyxml.h"

namespace OpenNN
{

/// This class represents the concept of training strategy for a neural network. 
/// A training strategy is composed of three training algorithms:
/// <ul>
/// <li> Initialization training algorithm.
/// <li> Main training algorithm.
/// <li> Refinement trainining algorithm.
/// </ul> 
   
class TrainingStrategy
{

public:

   // DEFAULT CONSTRUCTOR

   explicit TrainingStrategy(void);

   // GENERAL CONSTRUCTOR

   explicit TrainingStrategy(PerformanceFunctional*);

   // XML CONSTRUCTOR

   explicit TrainingStrategy(TiXmlElement*);

   // FILE CONSTRUCTOR

   explicit TrainingStrategy(const std::string&);

   // DESTRUCTOR

   virtual ~TrainingStrategy(void);

   // ENUMERATIONS

   /// Enumeration of all the available types of training algorithms. 

   enum TrainingAlgorithmType
   {
      NONE,
      RANDOM_SEARCH,
      EVOLUTIONARY_ALGORITHM,
      GRADIENT_DESCENT,
      CONJUGATE_GRADIENT,
      QUASI_NEWTON_METHOD,
      LEVENBERG_MARQUARDT_ALGORITHM,
      NEWTON_METHOD,
      USER_TRAINING_ALGORITHM
   };

   // STRUCTURES 

   /// This structure stores the results from the training strategy.
   /// They are composed of the initialization, refinement and training algorithms results. 

   struct Results
   {
      /// Pointer to a structure with results from the initialization training algorithm.

	  TrainingAlgorithm::Results* initialization_training_algorithm_results_pointer;

      /// Pointer to a structure with results from the main training algorithm.

	  TrainingAlgorithm::Results* main_training_algorithm_results_pointer;

      /// Pointer to a structure with results from the refinement training algorithm.

	  TrainingAlgorithm::Results* refinement_training_algorithm_results_pointer;	   


      void save(const std::string&) const;
  };

   // METHODS

   // Get methods

   PerformanceFunctional* get_performance_functional_pointer(void) const;

   TrainingAlgorithm* get_initialization_training_algorithm_pointer(void) const;
   TrainingAlgorithm* get_main_training_algorithm_pointer(void) const;
   TrainingAlgorithm* get_refinement_training_algorithm_pointer(void) const;

   const TrainingAlgorithmType& get_initialization_training_algorithm_type(void) const;
   const TrainingAlgorithmType& get_main_training_algorithm_type(void) const;
   const TrainingAlgorithmType& get_refinement_training_algorithm_type(void) const;

   std::string write_initialization_training_algorithm_type(void) const;
   std::string write_main_training_algorithm_type(void) const;
   std::string write_refinement_training_algorithm_type(void) const;

   const bool& get_initialization_training_algorithm_flag(void);
   const bool& get_main_training_algorithm_flag(void);
   const bool& get_refinement_training_algorithm_flag(void);

   const bool& get_display(void) const;
  
   // Set methods

   void set(void);
   void set(PerformanceFunctional*);
   virtual void set_default(void);

   void set_performance_functional_pointer(PerformanceFunctional*);

   void set_initialization_training_algorithm_pointer(TrainingAlgorithm*);
   void set_main_training_algorithm_pointer(TrainingAlgorithm*);
   void set_refinement_training_algorithm_pointer(TrainingAlgorithm*);

   void set_initialization_training_algorithm_type(const TrainingAlgorithmType&);
   void set_main_training_algorithm_type(const TrainingAlgorithmType&);
   void set_refinement_training_algorithm_type(const TrainingAlgorithmType&);

   void set_initialization_training_algorithm_type(const std::string&);
   void set_main_training_algorithm_type(const std::string&);
   void set_refinement_training_algorithm_type(const std::string&);

   void set_initialization_training_algorithm_flag(const bool&);
   void set_main_training_algorithm_flag(const bool&);
   void set_refinement_training_algorithm_flag(const bool&);

   void set_display(const bool&);

   // Pointer methods

   void construct_initialization_training_algorithm(const TrainingAlgorithmType&);
   void construct_main_training_algorithm(const TrainingAlgorithmType&);
   void construct_refinement_training_algorithm(const TrainingAlgorithmType&);

   void destruct_initialization_training_algorithm(void);
   void destruct_main_training_algorithm(void);
   void destruct_refinement_training_algorithm(void);

   // Training methods

   // This method trains a neural network which has a performance functional associated. 

   Results perform_training(void);

   // Serialization methods

   std::string to_string(void) const;

   void print(void) const;

   virtual TiXmlElement* to_XML(void) const;   
   virtual void from_XML(TiXmlElement*);   

   void save(const std::string&) const;
   void load(const std::string&);

protected:

   /// Pointer to an external performance functional object.

   PerformanceFunctional* performance_functional_pointer;

   /// Pointer to an internal training algorithm object to be used for initialization purposes. 

   TrainingAlgorithm* initialization_training_algorithm_pointer;

   /// Pointer to an internal training algorithm object to be used for main training. 

   TrainingAlgorithm* main_training_algorithm_pointer;

   /// Pointer to an internal training algorithm object to be used for refinement purposes. 

   TrainingAlgorithm* refinement_training_algorithm_pointer;


   /// Type of initialization training algorithm. 

   TrainingAlgorithmType initialization_training_algorithm_type;

   /// Type of main training algorithm. 

   TrainingAlgorithmType main_training_algorithm_type;

   /// Type of refinement training algorithm. 

   TrainingAlgorithmType refinement_training_algorithm_type;


   /// Flag for using or not the initialization training algorithm within the training strategy. 

   bool initialization_training_algorithm_flag;

   /// Flag for using or not the main training algorithm within the training strategy. 

   bool main_training_algorithm_flag;

   /// Flag for using or not the refinement training algorithm within the training strategy. 

   bool refinement_training_algorithm_flag;


   /// Display messages to screen.

   bool display;

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

