/****************************************************************************************************************/
/*                                                                                                              */
/*   OpenNN: Open Neural Networks Library                                                                       */
/*   www.opennn.cimne.com                                                                                       */
/*                                                                                                              */
/*   T R A I N I N G   S T R A T E G Y   C L A S S                                                              */
/*                                                                                                              */
/*   Roberto Lopez                                                                                              */
/*   International Center for Numerical Methods in Engineering (CIMNE)                                          */
/*   Technical University of Catalonia (UPC)                                                                    */
/*   Barcelona, Spain                                                                                           */
/*   E-mail: rlopez@cimne.upc.edu                                                                               */
/*                                                                                                              */
/****************************************************************************************************************/

// System includes

#include <iostream>
#include <fstream>
#include <algorithm>
#include <functional>
#include <limits>
#include <cmath>
#include <ctime>

// OpenNN includes

#include "training_strategy.h"

#include "random_search.h"
#include "evolutionary_algorithm.h"

#include "gradient_descent.h"
#include "conjugate_gradient.h"
#include "quasi_newton_method.h"
#include "levenberg_marquardt_algorithm.h"

#include "newton_method.h"

// TinyXml includes

#include "../../tinyxml/tinyxml.h"

namespace OpenNN
{

// DEFAULT CONSTRUCTOR

/// Default constructor. 
/// It creates a training strategy object not associated to any performance functional object.  
/// It also constructs the main training algorithm object. 

TrainingStrategy::TrainingStrategy(void)
 : performance_functional_pointer(NULL),
   initialization_training_algorithm_pointer(NULL),
   main_training_algorithm_pointer(NULL),
   refinement_training_algorithm_pointer(NULL)
{
   set_default();

   construct_main_training_algorithm(main_training_algorithm_type);
}


// PERFORMANCE FUNCTIONAL CONSTRUCTOR

/// Performance functional constructor. 
/// It creates a training strategy object associated to a performance functional object.
/// It also constructs the main training algorithm object. 
/// @param new_performance_functional_pointer Pointer to a performance functional object.

TrainingStrategy::TrainingStrategy(PerformanceFunctional* new_performance_functional_pointer)
 : performance_functional_pointer(new_performance_functional_pointer),
   initialization_training_algorithm_pointer(NULL),
   main_training_algorithm_pointer(NULL),
   refinement_training_algorithm_pointer(NULL)
{
   set_default();

   construct_main_training_algorithm(main_training_algorithm_type);
}


// XML CONSTRUCTOR

/// XML constructor. 
/// It creates a training strategy object not associated to any performance functional object.
/// It also loads the members of this object from a XML element. 
/// @param training_strategy_element Pointer to an element of the Tiny XML library.

TrainingStrategy::TrainingStrategy(TiXmlElement* training_strategy_element)
 : performance_functional_pointer(NULL),
   initialization_training_algorithm_pointer(NULL),
   main_training_algorithm_pointer(NULL),
   refinement_training_algorithm_pointer(NULL)
{
   set_default();

   from_XML(training_strategy_element);
}



// FILE CONSTRUCTOR

/// File constructor. 
/// It creates a training strategy object associated to a performance functional object.
/// It also loads the members of this object from a XML file. 
/// @param filename Name of training strategy XML file.

TrainingStrategy::TrainingStrategy(const std::string& filename)
 : performance_functional_pointer(NULL),
   initialization_training_algorithm_pointer(NULL),
   main_training_algorithm_pointer(NULL),
   refinement_training_algorithm_pointer(NULL)
{
   set_default();

   load(filename);
}


// DESTRUCTOR 

/// Destructor.
/// This destructor deletes the initialization, main and refinement training algorithm objects.

TrainingStrategy::~TrainingStrategy(void)
{ 
   delete initialization_training_algorithm_pointer;
   delete main_training_algorithm_pointer;
   delete refinement_training_algorithm_pointer;
}


// METHODS

// PerformanceFunctional* get_performance_functional_pointer(void) const method

/// This method returns a pointer to the performance functional object to which the training strategy is associated.

PerformanceFunctional* TrainingStrategy::get_performance_functional_pointer(void) const
{
   return(performance_functional_pointer);
}


// TrainingAlgorithm* get_initialization_training_algorithm_pointer(void) const method

/// This method returns a pointer to the initialization training algorithm object composing this training strategy object.

TrainingAlgorithm* TrainingStrategy::get_initialization_training_algorithm_pointer(void) const
{
   return(initialization_training_algorithm_pointer);
}


// TrainingAlgorithm* get_main_training_algorithm_pointer(void) const method

/// This method returns a pointer to the main training algorithm object composing this training strategy object.

TrainingAlgorithm* TrainingStrategy::get_main_training_algorithm_pointer(void) const
{
   return(main_training_algorithm_pointer);
}


// TrainingAlgorithm* get_refinement_training_algorithm_pointer(void) const method

/// This method returns a pointer to the refinement training algorithm object composing this training strategy object.

TrainingAlgorithm* TrainingStrategy::get_refinement_training_algorithm_pointer(void) const
{
   return(refinement_training_algorithm_pointer);
}


// const TrainingAlgorithmType& get_initialization_training_algorithm_type(void) const method

/// This method returns the type of the initialization training algorithm composing this training strategy object. 

const TrainingStrategy::TrainingAlgorithmType& TrainingStrategy::get_initialization_training_algorithm_type(void) const
{
   return(initialization_training_algorithm_type);
}


// const TrainingAlgorithmType& get_main_training_algorithm_type(void) const method

/// This method returns the type of the main training algorithm composing this training strategy object. 

const TrainingStrategy::TrainingAlgorithmType& TrainingStrategy::get_main_training_algorithm_type(void) const
{
   return(main_training_algorithm_type);
}


// const TrainingAlgorithmType& get_refinement_training_algorithm_type(void) const method

/// This method returns the type of the refinement training algorithm composing this training strategy object. 

const TrainingStrategy::TrainingAlgorithmType& TrainingStrategy::get_refinement_training_algorithm_type(void) const
{
   return(refinement_training_algorithm_type);
}


// std::string TrainingStrategy::write_initialization_training_algorithm_type(void) const

/// This method returns a string with the type of the initialization training algorithm composing this training strategy object. 

std::string TrainingStrategy::write_initialization_training_algorithm_type(void) const
{
   if(initialization_training_algorithm_type == NONE)
   {
      return("NONE");
   }
   else if(initialization_training_algorithm_type == RANDOM_SEARCH)
   {
      return("RANDOM_SEARCH");
   }
   else if(initialization_training_algorithm_type == EVOLUTIONARY_ALGORITHM)
   {
      return("EVOLUTIONARY_ALGORITHM");
   }
   else if(initialization_training_algorithm_type == GRADIENT_DESCENT)
   {
      return("GRADIENT_DESCENT");
   }
   else if(initialization_training_algorithm_type == CONJUGATE_GRADIENT)
   {
      return("CONJUGATE_GRADIENT");
   }
   else if(initialization_training_algorithm_type == QUASI_NEWTON_METHOD)
   {
      return("QUASI_NEWTON_METHOD");
   }
   else if(initialization_training_algorithm_type == LEVENBERG_MARQUARDT_ALGORITHM)
   {
      return("LEVENBERG_MARQUARDT_ALGORITHM");
   }
   else if(initialization_training_algorithm_type == NEWTON_METHOD)
   {
      return("NEWTON_METHOD");
   }
   else if(initialization_training_algorithm_type == USER_TRAINING_ALGORITHM)
   {
      return("USER_TRAINING_ALGORITHM");
   }
   else
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: TrainingStrategy class.\n"
             << "std::string write_initialization_training_algorithm_type(void) const method.\n"
             << "Unknown training algorithm type.\n";
 
	  throw std::logic_error(buffer.str());
   }
}


// std::string TrainingStrategy::write_main_training_algorithm_type(void) const

/// This method returns a string with the type of the main training algorithm composing this training strategy object. 

std::string TrainingStrategy::write_main_training_algorithm_type(void) const
{
   if(main_training_algorithm_type == NONE)
   {
      return("NONE");
   }
   else if(main_training_algorithm_type == RANDOM_SEARCH)
   {
      return("RANDOM_SEARCH");
   }
   else if(main_training_algorithm_type == EVOLUTIONARY_ALGORITHM)
   {
      return("EVOLUTIONARY_ALGORITHM");
   }
   else if(main_training_algorithm_type == GRADIENT_DESCENT)
   {
      return("GRADIENT_DESCENT");
   }
   else if(main_training_algorithm_type == CONJUGATE_GRADIENT)
   {
      return("CONJUGATE_GRADIENT");
   }
   else if(main_training_algorithm_type == QUASI_NEWTON_METHOD)
   {
      return("QUASI_NEWTON_METHOD");
   }
   else if(main_training_algorithm_type == LEVENBERG_MARQUARDT_ALGORITHM)
   {
      return("LEVENBERG_MARQUARDT_ALGORITHM");
   }
   else if(main_training_algorithm_type == NEWTON_METHOD)
   {
      return("NEWTON_METHOD");
   }
   else if(main_training_algorithm_type == USER_TRAINING_ALGORITHM)
   {
      return("USER_TRAINING_ALGORITHM");
   }
   else
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: TrainingStrategy class.\n"
             << "std::string write_main_training_algorithm_type(void) const method.\n"
             << "Unknown training algorithm type.\n";
 
	  throw std::logic_error(buffer.str());
   }
}


// std::string TrainingStrategy::write_refinement_training_algorithm_type(void) const

/// This method returns a string with the type of the refinement training algorithm composing this training strategy object. 

std::string TrainingStrategy::write_refinement_training_algorithm_type(void) const
{
   if(refinement_training_algorithm_type == NONE)
   {
      return("NONE");
   }
   else if(refinement_training_algorithm_type == RANDOM_SEARCH)
   {
      return("RANDOM_SEARCH");
   }
   else if(refinement_training_algorithm_type == EVOLUTIONARY_ALGORITHM)
   {
      return("EVOLUTIONARY_ALGORITHM");
   }
   else if(refinement_training_algorithm_type == GRADIENT_DESCENT)
   {
      return("GRADIENT_DESCENT");
   }
   else if(refinement_training_algorithm_type == CONJUGATE_GRADIENT)
   {
      return("CONJUGATE_GRADIENT");
   }
   else if(refinement_training_algorithm_type == QUASI_NEWTON_METHOD)
   {
      return("QUASI_NEWTON_METHOD");
   }
   else if(refinement_training_algorithm_type == LEVENBERG_MARQUARDT_ALGORITHM)
   {
      return("LEVENBERG_MARQUARDT_ALGORITHM");
   }
   else if(refinement_training_algorithm_type == NEWTON_METHOD)
   {
      return("NEWTON_METHOD");
   }
   else if(refinement_training_algorithm_type == USER_TRAINING_ALGORITHM)
   {
      return("USER_TRAINING_ALGORITHM");
   }
   else
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: TrainingStrategy class.\n"
             << "std::string write_refinement_training_algorithm_type(void) const method.\n"
             << "Unknown training algorithm type.\n";
 
	  throw std::logic_error(buffer.str());
   }
}


// const bool& get_initialization_training_algorithm_flag(void) method

/// This method returns the flag value for applying or not the initialization training algorithm within the training strategy. 

const bool& TrainingStrategy::get_initialization_training_algorithm_flag(void)
{
   return(initialization_training_algorithm_flag);
}


// const bool& get_main_training_algorithm_flag(void) method

/// This method returns the flag value for applying or not the main training algorithm within the training strategy. 

const bool& TrainingStrategy::get_main_training_algorithm_flag(void)
{
   return(main_training_algorithm_flag);
}


// const bool& get_refinement_training_algorithm_flag(void) method

/// This method returns the flag value for applying or not the refinement training algorithm within the training strategy. 

const bool& TrainingStrategy::get_refinement_training_algorithm_flag(void)
{
   return(refinement_training_algorithm_flag);
}


// const bool& get_display(void) const method

/// This method returns true if messages from this class can be displayed on the screen, or false if messages from
/// this class can't be displayed on the screen.

const bool& TrainingStrategy::get_display(void) const
{
   return(display);
}


// void set(void) method

/// This method sets the performance functional pointer to NULL.
/// It also destructs the initialization, main and refinement training algorithms. 
/// Finally, it sets the rest of members to their default values. 

void TrainingStrategy::set(void)
{
   performance_functional_pointer = NULL;

   destruct_initialization_training_algorithm();
   destruct_main_training_algorithm();
   destruct_refinement_training_algorithm();

   set_default();
}


// void set(PerformanceFunctional*) method

/// This method sets a new performance functional pointer.
/// It also destructs the initialization, main and refinement training algorithms. 
/// Finally, it sets the rest of members to their default values. 
/// @param new_performance_functional_pointer Pointer to a performance functional object. 

void TrainingStrategy::set(PerformanceFunctional* new_performance_functional_pointer)
{
   performance_functional_pointer = new_performance_functional_pointer;

   set_default();

   destruct_initialization_training_algorithm();
   construct_main_training_algorithm(main_training_algorithm_type);
   destruct_refinement_training_algorithm();
}


// void set_initialization_training_algorithm_flag(const bool&) method

/// This method sets a new flag for using the initialization training algorithm within the training strategy. 
/// @param new_initialization_training_algorithm_flag Flag for using or not this algorithm within the strategy. 

void TrainingStrategy::set_initialization_training_algorithm_flag(const bool& new_initialization_training_algorithm_flag)
{
   initialization_training_algorithm_flag = new_initialization_training_algorithm_flag;
}


// void set_main_training_algorithm_flag(const bool&) method

/// This method sets a new flag for using the main training algorithm within the training strategy. 
/// @param new_main_training_algorithm_flag Flag for using or not this algorithm within the strategy. 

void TrainingStrategy::set_main_training_algorithm_flag(const bool& new_main_training_algorithm_flag)
{
   main_training_algorithm_flag = new_main_training_algorithm_flag;
}


// void set_refinement_training_algorithm_flag(const bool&) method

/// This method sets a new flag for using the refinement training algorithm within the training strategy. 
/// @param new_refinement_training_algorithm_flag Flag for using or not this algorithm within the strategy. 

void TrainingStrategy::set_refinement_training_algorithm_flag(const bool& new_refinement_training_algorithm_flag)
{
   refinement_training_algorithm_flag = new_refinement_training_algorithm_flag;
}


// void set_initialization_training_algorithm_type(const TrainingAlgorithmType&) method

/// This method sets a new type of initialization training algorithm. 
/// Note that it neither sets the training algorithm object nor the flag. 
/// @param new_initialization_training_algorithm_type Type of initialization training algorithm.

void TrainingStrategy::set_initialization_training_algorithm_type(const TrainingAlgorithmType& new_initialization_training_algorithm_type)
{
   initialization_training_algorithm_type = new_initialization_training_algorithm_type;
}


// void set_main_training_algorithm_type(const TrainingAlgorithmType&) method

/// This method sets a new type of main training algorithm. 
/// Note that it neither sets the training algorithm object nor the flag. 
/// @param new_main_training_algorithm_type Type of main training algorithm.

void TrainingStrategy::set_main_training_algorithm_type(const TrainingAlgorithmType& new_main_training_algorithm_type)
{
   main_training_algorithm_type = new_main_training_algorithm_type;
}


// void set_refinement_training_algorithm_type(const TrainingAlgorithmType&) method

/// This method sets a new type of refinement training algorithm. 
/// Note that it neither sets the training algorithm object nor the flag. 
/// @param new_refinement_training_algorithm_type Type of refinement training algorithm.

void TrainingStrategy::set_refinement_training_algorithm_type(const TrainingAlgorithmType& new_refinement_training_algorithm_type)
{
   refinement_training_algorithm_type = new_refinement_training_algorithm_type;
}


// void set_initialization_training_algorithm_type(const std::string&) method

/// This method sets the initialization training algorithm type from a string. 
/// Note that it neither sets the training algorithm object nor the flag. 
/// @param new_training_algorithm_type String with the initialization training algorithm type.

void TrainingStrategy::set_initialization_training_algorithm_type(const std::string& new_training_algorithm_type)
{
   if(new_training_algorithm_type == "NONE")
   {
      set_initialization_training_algorithm_type(NONE);
   }
   else if(new_training_algorithm_type == "RANDOM_SEARCH")
   {
      set_initialization_training_algorithm_type(RANDOM_SEARCH);
   }
   else if(new_training_algorithm_type == "EVOLUTIONARY_ALGORITHM")
   {
      set_initialization_training_algorithm_type(EVOLUTIONARY_ALGORITHM);
   }
   else if(new_training_algorithm_type == "GRADIENT_DESCENT")
   {
      set_initialization_training_algorithm_type(GRADIENT_DESCENT);
   }
   else if(new_training_algorithm_type == "CONJUGATE_GRADIENT")
   {
      set_initialization_training_algorithm_type(CONJUGATE_GRADIENT);
   }
   else if(new_training_algorithm_type == "QUASI_NEWTON_METHOD")
   {
      set_initialization_training_algorithm_type(QUASI_NEWTON_METHOD);
   }
   else if(new_training_algorithm_type == "LEVENBERG_MARQUARDT_ALGORITHM")
   {
      set_initialization_training_algorithm_type(LEVENBERG_MARQUARDT_ALGORITHM);
   }
   else if(new_training_algorithm_type == "NEWTON_METHOD")
   {
      set_initialization_training_algorithm_type(NEWTON_METHOD);
   }
   else if(new_training_algorithm_type == "USER_TRAINING_ALGORITHM")
   {
      set_initialization_training_algorithm_type(USER_TRAINING_ALGORITHM);
   }
   else
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: TrainingStrategy class.\n"
             << "void set_initialization_training_algorithm_type(const std::string&) method.\n"
   	         << "Unknown training algorithm type: " << new_training_algorithm_type << ".\n";

      throw std::logic_error(buffer.str());
   }   
}


// void set_main_training_algorithm_type(const std::string&) method

/// This method sets the main training algorithm type from a string. 
/// Note that it neither sets the training algorithm object nor the flag. 
/// @param new_training_algorithm_type String with the main training algorithm type.

void TrainingStrategy::set_main_training_algorithm_type(const std::string& new_training_algorithm_type)
{
   if(new_training_algorithm_type == "NONE")
   {
      set_main_training_algorithm_type(NONE);
   }
   else if(new_training_algorithm_type == "RANDOM_SEARCH")
   {
      set_main_training_algorithm_type(RANDOM_SEARCH);
   }
   else if(new_training_algorithm_type == "EVOLUTIONARY_ALGORITHM")
   {
      set_main_training_algorithm_type(EVOLUTIONARY_ALGORITHM);
   }
   else if(new_training_algorithm_type == "GRADIENT_DESCENT")
   {
      set_main_training_algorithm_type(GRADIENT_DESCENT);
   }
   else if(new_training_algorithm_type == "CONJUGATE_GRADIENT")
   {
      set_main_training_algorithm_type(CONJUGATE_GRADIENT);
   }
   else if(new_training_algorithm_type == "QUASI_NEWTON_METHOD")
   {
      set_main_training_algorithm_type(QUASI_NEWTON_METHOD);
   }
   else if(new_training_algorithm_type == "LEVENBERG_MARQUARDT_ALGORITHM")
   {
      set_main_training_algorithm_type(LEVENBERG_MARQUARDT_ALGORITHM);
   }
   else if(new_training_algorithm_type == "NEWTON_METHOD")
   {
      set_main_training_algorithm_type(NEWTON_METHOD);
   }
   else if(new_training_algorithm_type == "USER_TRAINING_ALGORITHM")
   {
      set_main_training_algorithm_type(USER_TRAINING_ALGORITHM);
   }
   else
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: TrainingStrategy class.\n"
             << "void set_main_training_algorithm_type(const std::string&) method.\n"
   	         << "Unknown training algorithm type: " << new_training_algorithm_type << ".\n";

      throw std::logic_error(buffer.str());
   }   
}


// void set_refinement_training_algorithm_type(const std::string&) method

/// This method sets the refinement training algorithm type from a string. 
/// Note that it neither sets the training algorithm object nor the flag. 
/// @param new_training_algorithm_type String with the refinement training algorithm type.

void TrainingStrategy::set_refinement_training_algorithm_type(const std::string& new_training_algorithm_type)
{
   if(new_training_algorithm_type == "NONE")
   {
      set_refinement_training_algorithm_type(NONE);
   }
   else if(new_training_algorithm_type == "RANDOM_SEARCH")
   {
      set_refinement_training_algorithm_type(RANDOM_SEARCH);
   }
   else if(new_training_algorithm_type == "EVOLUTIONARY_ALGORITHM")
   {
      set_refinement_training_algorithm_type(EVOLUTIONARY_ALGORITHM);
   }
   else if(new_training_algorithm_type == "GRADIENT_DESCENT")
   {
      set_refinement_training_algorithm_type(GRADIENT_DESCENT);
   }
   else if(new_training_algorithm_type == "CONJUGATE_GRADIENT")
   {
      set_refinement_training_algorithm_type(CONJUGATE_GRADIENT);
   }
   else if(new_training_algorithm_type == "QUASI_NEWTON_METHOD")
   {
      set_refinement_training_algorithm_type(QUASI_NEWTON_METHOD);
   }
   else if(new_training_algorithm_type == "LEVENBERG_MARQUARDT_ALGORITHM")
   {
      set_refinement_training_algorithm_type(LEVENBERG_MARQUARDT_ALGORITHM);
   }
   else if(new_training_algorithm_type == "NEWTON_METHOD")
   {
      set_refinement_training_algorithm_type(NEWTON_METHOD);
   }
   else if(new_training_algorithm_type == "USER_TRAINING_ALGORITHM")
   {
      set_refinement_training_algorithm_type(USER_TRAINING_ALGORITHM);
   }
   else
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: TrainingStrategy class.\n"
             << "void set_refinement_training_algorithm_type(const std::string&) method.\n"
   	         << "Unknown training algorithm type: " << new_training_algorithm_type << ".\n";

      throw std::logic_error(buffer.str());
   }   
}


// void set_performance_functional_pointer(PerformanceFunctional*) method

/// This method sets a pointer to a performance functional object to be associated to the training strategy.
/// @param new_performance_functional_pointer Pointer to a performance functional object.

void TrainingStrategy::set_performance_functional_pointer(PerformanceFunctional* new_performance_functional_pointer)
{
   performance_functional_pointer = new_performance_functional_pointer;
}


// void set_initialization_training_algorithm_pointer(TrainingAlgorithm*) method

/// This method sets a new initialization training algorithm to be part of the training strategy. 
/// Note that the training algorithms are deleted by the training strategy destructor.

void TrainingStrategy::set_initialization_training_algorithm_pointer(TrainingAlgorithm* new_initialization_training_algorithm_pointer)
{
   initialization_training_algorithm_pointer = new_initialization_training_algorithm_pointer;
}


// void set_main_training_algorithm_pointer(TrainingAlgorithm*) method

/// This method sets a new main training algorithm to be part of the training strategy. 
/// Note that the training algorithms are deleted by the training strategy destructor.

void TrainingStrategy::set_main_training_algorithm_pointer(TrainingAlgorithm* new_main_training_algorithm_pointer)
{
   main_training_algorithm_pointer = new_main_training_algorithm_pointer;
}


// void set_refinement_training_algorithm_pointer(TrainingAlgorithm*) method

/// This method sets a new refinement training algorithm to be part of the training strategy. 
/// Note that the training algorithms are deleted by the training strategy destructor.

void TrainingStrategy::set_refinement_training_algorithm_pointer(TrainingAlgorithm* new_refinement_training_algorithm_pointer)
{
   refinement_training_algorithm_pointer = new_refinement_training_algorithm_pointer;
}


// void set_display(const bool&) method

/// This method sets a new display value. 
/// If it is set to true messages from this class are to be displayed on the screen;
/// if it is set to false messages from this class are not to be displayed on the screen.
/// @param new_display Display value.

void TrainingStrategy::set_display(const bool& new_display)
{
   display = new_display;
}


// void set_default(void) method 

/// This method sets the members of the training strategy object to their default values:
/// <ul>
/// <li> Initialization training algorithm type: None.
/// <li> Main training algorithm type: Quasi-Newton method.
/// <li> Refinement training algorithm type: None.
/// <li> Initialization training algorithm flag: false.
/// <li> Main training algorithm flag: true.
/// <li> Refinement training algorithm flag: false.
/// <li> Display: true.
/// </ul> 

void TrainingStrategy::set_default(void)
{
   initialization_training_algorithm_type = TrainingStrategy::NONE;
   main_training_algorithm_type = TrainingStrategy::QUASI_NEWTON_METHOD;
   refinement_training_algorithm_type = NONE;

   initialization_training_algorithm_flag = false;
   main_training_algorithm_flag = true;
   refinement_training_algorithm_flag = false;
   
   display = true;
}


// void construct_initialization_training_algorithm(const TrainingAlgorithmType&) const

/// This method constructs the initialization training algorithm object, given its type. 
/// It also sets the type of that training algorithm.
/// Finally, it sets the initialization training algorithm flag to true. 
/// @param new_training_algorithm_type Type of initialization training algorithm. 

void TrainingStrategy::construct_initialization_training_algorithm(const TrainingAlgorithmType& new_training_algorithm_type)
{
   if(initialization_training_algorithm_pointer)
   {
      delete initialization_training_algorithm_pointer;
   }

   initialization_training_algorithm_type = new_training_algorithm_type;
   initialization_training_algorithm_flag = true;

   switch(initialization_training_algorithm_type)
   {
      case RANDOM_SEARCH:
      {
         initialization_training_algorithm_pointer = new RandomSearch(performance_functional_pointer);
      }
      break;

      case EVOLUTIONARY_ALGORITHM:
      {
         initialization_training_algorithm_pointer = new EvolutionaryAlgorithm(performance_functional_pointer);
      }
      break;

      case GRADIENT_DESCENT:
      {
         initialization_training_algorithm_pointer = new GradientDescent(performance_functional_pointer);
      }
      break;

      case CONJUGATE_GRADIENT:
      {
         initialization_training_algorithm_pointer = new ConjugateGradient(performance_functional_pointer);
      }
      break;

      case QUASI_NEWTON_METHOD:
      {
         initialization_training_algorithm_pointer = new QuasiNewtonMethod(performance_functional_pointer);
      }
      break;

      case LEVENBERG_MARQUARDT_ALGORITHM:
      {
         initialization_training_algorithm_pointer = new LevenbergMarquardtAlgorithm(performance_functional_pointer);
      }
      break;

      case NEWTON_METHOD:
      {
         initialization_training_algorithm_pointer = new NewtonMethod(performance_functional_pointer);
      }
      break;

      case USER_TRAINING_ALGORITHM:
      {
         initialization_training_algorithm_pointer = NULL;
      }
      break;

      default:
      {
         std::ostringstream buffer;

         buffer << "OpenNN Exception: TrainingStrategy class.\n"
                << "void construct_initialization_training_algorithm(const TrainingAlgorithmType&) method.\n"
                << "Unknown training algorithm type.\n";
 
         throw std::logic_error(buffer.str().c_str());	     
      }
      break;
   }
}


// void construct_main_training_algorithm(const TrainingAlgorithmType&) const

/// This method constructs the main training algorithm object, given its type. 
/// It also sets the type of that training algorithm.
/// Finally, it sets the main training algorithm flag to true. 
/// @param new_training_algorithm_type Type of main training algorithm. 

void TrainingStrategy::construct_main_training_algorithm(const TrainingAlgorithmType& new_training_algorithm_type)
{
   if(main_training_algorithm_pointer)
   {
      delete main_training_algorithm_pointer;
   }

   main_training_algorithm_type = new_training_algorithm_type;
   main_training_algorithm_flag = true;

   switch(main_training_algorithm_type)
   {
      case RANDOM_SEARCH:
      {
         main_training_algorithm_pointer = new RandomSearch(performance_functional_pointer);
      }
      break;

      case EVOLUTIONARY_ALGORITHM:
      {
         main_training_algorithm_pointer = new EvolutionaryAlgorithm(performance_functional_pointer);
      }
      break;

      case GRADIENT_DESCENT:
      {
         main_training_algorithm_pointer = new GradientDescent(performance_functional_pointer);
      }
      break;

      case CONJUGATE_GRADIENT:
      {
         main_training_algorithm_pointer = new ConjugateGradient(performance_functional_pointer);
      }
      break;

      case QUASI_NEWTON_METHOD:
      {
         main_training_algorithm_pointer = new QuasiNewtonMethod(performance_functional_pointer);
      }
      break;

      case LEVENBERG_MARQUARDT_ALGORITHM:
      {
         main_training_algorithm_pointer = new LevenbergMarquardtAlgorithm(performance_functional_pointer);
      }
      break;

      case NEWTON_METHOD:
      {
         main_training_algorithm_pointer = new NewtonMethod(performance_functional_pointer);
      }
      break;

      case USER_TRAINING_ALGORITHM:
      {
         main_training_algorithm_pointer = NULL;
      }
      break;

      default:
      {
         std::ostringstream buffer;

         buffer << "OpenNN Exception: TrainingStrategy class.\n"
                << "void construct_main_training_algorithm(const TrainingAlgorithmType&) method.\n"
                << "Unknown training algorithm type.\n";
 
         throw std::logic_error(buffer.str().c_str());	     
      }
      break;
   }
}


// void construct_refinement_training_algorithm(const TrainingAlgorithmType&) const

/// This method constructs the refinement training algorithm object, given its type. 
/// It also sets the type of that training algorithm.
/// Finally, it sets the refinement training algorithm flag to true. 
/// @param new_training_algorithm_type Type of refinement training algorithm. 

void TrainingStrategy::construct_refinement_training_algorithm(const TrainingAlgorithmType& new_training_algorithm_type)
{
   if(refinement_training_algorithm_pointer)
   {
      delete refinement_training_algorithm_pointer;
   }

   refinement_training_algorithm_type = new_training_algorithm_type;
   refinement_training_algorithm_flag = true;

   switch(main_training_algorithm_type)
   {
      case RANDOM_SEARCH:
      {
         refinement_training_algorithm_pointer = new RandomSearch(performance_functional_pointer);
      }
      break;

      case EVOLUTIONARY_ALGORITHM:
      {
         refinement_training_algorithm_pointer = new EvolutionaryAlgorithm(performance_functional_pointer);
      }
      break;

      case GRADIENT_DESCENT:
      {
         refinement_training_algorithm_pointer = new GradientDescent(performance_functional_pointer);
      }
      break;

      case CONJUGATE_GRADIENT:
      {
         refinement_training_algorithm_pointer = new ConjugateGradient(performance_functional_pointer);
      }
      break;

      case QUASI_NEWTON_METHOD:
      {
         refinement_training_algorithm_pointer = new QuasiNewtonMethod(performance_functional_pointer);
      }
      break;

      case LEVENBERG_MARQUARDT_ALGORITHM:
      {
         refinement_training_algorithm_pointer = new LevenbergMarquardtAlgorithm(performance_functional_pointer);
      }
      break;

      case NEWTON_METHOD:
      {
         refinement_training_algorithm_pointer = new NewtonMethod(performance_functional_pointer);
      }
      break;

      case USER_TRAINING_ALGORITHM:
      {
         refinement_training_algorithm_pointer = NULL;
      }
      break;

      default:
      {
         std::ostringstream buffer;

         buffer << "OpenNN Exception: TrainingStrategy class.\n"
                << "void construct_refinement_training_algorithm(const TrainingAlgorithmType&) method.\n"
                << "Unknown training algorithm type.\n";
 
         throw std::logic_error(buffer.str().c_str());	     
      }
      break;
   }
}


// void destruct_initialization_training_algorithm(void) method

/// This method deletes the initialization training algorithm object which composes this training strategy object. 

void TrainingStrategy::destruct_initialization_training_algorithm(void)
{
   delete initialization_training_algorithm_pointer;

   initialization_training_algorithm_pointer = NULL;

   initialization_training_algorithm_type = NONE;

   initialization_training_algorithm_flag = false;
}


// void destruct_main_training_algorithm(void) method

/// This method deletes the main training algorithm object which composes this training strategy object. 

void TrainingStrategy::destruct_main_training_algorithm(void)
{
   delete main_training_algorithm_pointer;

   main_training_algorithm_pointer = NULL;

   main_training_algorithm_type = NONE;

   main_training_algorithm_flag = false;
}


// void destruct_refinement_training_algorithm(void) method

/// This method deletes the refinement training algorithm object which composes this training strategy object. 

void TrainingStrategy::destruct_refinement_training_algorithm(void)
{
   delete refinement_training_algorithm_pointer;

   refinement_training_algorithm_pointer = NULL;

   refinement_training_algorithm_type = NONE;

   refinement_training_algorithm_flag = false;
}


// TrainingStrategyResults perform_training(void) method

/// This is the most important method of this class. 
/// It optimizes the performance functional of a neural network.
/// The most general training strategy consists of three steps: initialization, main and refinement training processes. 
/// This method also returns a structure with the results from training. 

TrainingStrategy::Results TrainingStrategy::perform_training(void)
{
   #ifdef _DEBUG 

   std::ostringstream buffer;

   #endif

   Results training_strategy_results;

   training_strategy_results.initialization_training_algorithm_results_pointer = NULL;
   training_strategy_results.main_training_algorithm_results_pointer = NULL;
   training_strategy_results.refinement_training_algorithm_results_pointer = NULL;

   if(initialization_training_algorithm_flag)
   {    
      #ifdef _DEBUG 

      if(!initialization_training_algorithm_pointer)
      {
         buffer << "OpenNN Exception: TrainingStrategy class.\n"
                << "void perform_training(void) method.\n"
                << "Pointer to initialization training algorithm is NULL.\n";

         throw std::logic_error(buffer.str().c_str());	  
      }

      #endif

      training_strategy_results.initialization_training_algorithm_results_pointer 
	  = initialization_training_algorithm_pointer->perform_training();
   }

   if(main_training_algorithm_flag)
   {    
      #ifdef _DEBUG 

      if(!main_training_algorithm_pointer)
      {
         buffer << "OpenNN Exception: TrainingStrategy class.\n"
                << "void perform_training(void) method.\n"
                << "Pointer to main training algorithm is NULL.\n";

         throw std::logic_error(buffer.str().c_str());	  
      }

      #endif

      training_strategy_results.initialization_training_algorithm_results_pointer = main_training_algorithm_pointer->perform_training();
   }

   if(refinement_training_algorithm_flag)
   {    
      #ifdef _DEBUG 

      if(!initialization_training_algorithm_pointer)
      {
         buffer << "OpenNN Exception: TrainingStrategy class.\n"
                << "void perform_training(void) method.\n"
                << "Pointer to refinement training algorithm is NULL.\n";

         throw std::logic_error(buffer.str().c_str());	  
      }

      #endif

      training_strategy_results.initialization_training_algorithm_results_pointer = refinement_training_algorithm_pointer->perform_training();
   }

   return(training_strategy_results);
}


// std::string to_string(void) const method

/// This method returns a string representation of the training strategy. 

std::string TrainingStrategy::to_string(void) const
{
   std::ostringstream buffer;

   buffer << "Training strategy\n"
          << "Initialization training algorithm type: " << write_initialization_training_algorithm_type() << "\n"
          << "Main training algorithm type: " << write_main_training_algorithm_type() << "\n"
          << "Refinement training algorithm type: " << write_refinement_training_algorithm_type() << "\n"
          << "Initialization training algorithm flag: " << initialization_training_algorithm_flag << "\n"
          << "Main training algorithm flag: " << main_training_algorithm_flag << "\n"
          << "Refinement training algorithm flag: " << refinement_training_algorithm_flag << "\n";

   if(initialization_training_algorithm_pointer)
   {
      buffer << "Initialization training algorithm:\n" 
             << initialization_training_algorithm_pointer->to_string();   
   }

   if(main_training_algorithm_pointer)
   {
      buffer << "Main training algorithm:\n" 
             << main_training_algorithm_pointer->to_string();   
   }

   if(refinement_training_algorithm_pointer)
   {
      buffer << "Refinement training algorithm:\n" 
             << refinement_training_algorithm_pointer->to_string();   
   }

   return(buffer.str());
}


// void print(void) const method

/// This method prints to the screen the string representation of the training strategy object.

void TrainingStrategy::print(void) const
{
   std::cout << to_string();
}


// TiXmlElement* to_XML(void) const method

/// This method returns a default string representation in XML-type format of the training algorithm object.
/// This containts the training operators, the training parameters, stopping criteria and other stuff.

TiXmlElement* TrainingStrategy::to_XML(void) const
{
   std::ostringstream buffer;

   // Training strategy

   TiXmlElement* training_strategy_element = new TiXmlElement("TrainingStrategy");
   training_strategy_element->SetAttribute("Version", 4); 

   // Initialization training algorithm type
   {
      TiXmlElement* initialization_training_algorithm_type_element = new TiXmlElement("InitializationTrainingAlgorithmType");
      training_strategy_element->LinkEndChild(initialization_training_algorithm_type_element);
        
      const std::string new_initialization_training_algorithm_type = write_initialization_training_algorithm_type();

      TiXmlText* initialization_training_algorithm_type_text = new TiXmlText(new_initialization_training_algorithm_type.c_str());
      initialization_training_algorithm_type_element->LinkEndChild(initialization_training_algorithm_type_text);
   }

   // Main training algorithm type
   {   
      TiXmlElement* main_training_algorithm_type_element = new TiXmlElement("MainTrainingAlgorithmType");
      training_strategy_element->LinkEndChild(main_training_algorithm_type_element);

      const std::string new_main_training_algorithm_type = write_main_training_algorithm_type();

      TiXmlText* main_training_algorithm_type_text = new TiXmlText(new_main_training_algorithm_type.c_str());
      main_training_algorithm_type_element->LinkEndChild(main_training_algorithm_type_text);
   }

   // Refinement training algorithm type
   {
      TiXmlElement* refinement_training_algorithm_type_element = new TiXmlElement("RefinementTrainingAlgorithmType");
      training_strategy_element->LinkEndChild(refinement_training_algorithm_type_element);

      const std::string new_refinement_training_algorithm_type = write_refinement_training_algorithm_type();

      TiXmlText* refinement_training_algorithm_type_text = new TiXmlText(new_refinement_training_algorithm_type.c_str());
      refinement_training_algorithm_type_element->LinkEndChild(refinement_training_algorithm_type_text);
   }

   // Initialization training algorithm flag
   {
      TiXmlElement* initialization_training_algorithm_flag_element = new TiXmlElement("InitializationTrainingAlgorithmFlag");
      training_strategy_element->LinkEndChild(initialization_training_algorithm_flag_element);

      buffer.str("");
      buffer << initialization_training_algorithm_flag;

      TiXmlText* initialization_training_algorithm_flag_text = new TiXmlText(buffer.str().c_str());
      initialization_training_algorithm_flag_element->LinkEndChild(initialization_training_algorithm_flag_text);
   }

   // Main training algorithm flag
   {
      TiXmlElement* main_training_algorithm_flag_element = new TiXmlElement("MainTrainingAlgorithmFlag");
      training_strategy_element->LinkEndChild(main_training_algorithm_flag_element);

      buffer.str("");
      buffer << main_training_algorithm_flag;

      TiXmlText* main_training_algorithm_flag_text = new TiXmlText(buffer.str().c_str());
      main_training_algorithm_flag_element->LinkEndChild(main_training_algorithm_flag_text);
   }

   // Refinement training algorithm flag
   {
      TiXmlElement* refinement_training_algorithm_flag_element = new TiXmlElement("RefinementTrainingAlgorithmFlag");
      training_strategy_element->LinkEndChild(refinement_training_algorithm_flag_element);

      buffer.str("");
      buffer << refinement_training_algorithm_flag;

      TiXmlText* refinement_training_algorithm_flag_text = new TiXmlText(buffer.str().c_str());
      refinement_training_algorithm_flag_element->LinkEndChild(refinement_training_algorithm_flag_text);
   }

   // Initialization training algorithm 

   if(initialization_training_algorithm_pointer)
   {
	  TiXmlElement* initialization_training_algorithm_element = initialization_training_algorithm_pointer->to_XML();

      training_strategy_element->LinkEndChild(initialization_training_algorithm_element);
   }

   // Main training algorithm

   if(main_training_algorithm_pointer)
   {
	  TiXmlElement* main_training_algorithm_element = main_training_algorithm_pointer->to_XML();

      training_strategy_element->LinkEndChild(main_training_algorithm_element);
   }

   // Refinement training algorithm

   if(refinement_training_algorithm_pointer)
   {
	  TiXmlElement* refinement_training_algorithm_element = refinement_training_algorithm_pointer->to_XML();

      training_strategy_element->LinkEndChild(refinement_training_algorithm_element);
   }

   // Display
   {
      TiXmlElement* display_element = new TiXmlElement("Display");
      training_strategy_element->LinkEndChild(display_element);

      buffer.str("");
      buffer << display;

      TiXmlText* display_text = new TiXmlText(buffer.str().c_str());
      display_element->LinkEndChild(display_text);
   }

   return(training_strategy_element);
}


// void from_XML(TiXmlElement*) method

/// This method loads the members of this training strategy object from a XML element. 
/// @param training_strategy_element Pointer to a XML element of the TinyXML library. 

void TrainingStrategy::from_XML(TiXmlElement* training_strategy_element)
{
   if(!training_strategy_element)
   {
      return;
   }

   // Initialization training algorithm type
   {
      TiXmlElement* initialization_training_algorithm_type_element = training_strategy_element->FirstChildElement("InitializationTrainingAlgorithmType");

      if(initialization_training_algorithm_type_element)
      {
         const std::string new_initialization_training_algorithm_type = initialization_training_algorithm_type_element->GetText();           

         try
         {
            set_initialization_training_algorithm_type(new_initialization_training_algorithm_type);
         }
         catch(std::exception& e)
         {
            std::cout << e.what() << std::endl;		 
         }
      }
   }

   // Main training algorithm type
   {
      TiXmlElement* main_training_algorithm_type_element = training_strategy_element->FirstChildElement("MainTrainingAlgorithmType");

      if(main_training_algorithm_type_element)
      {
         const std::string new_main_training_algorithm_type = main_training_algorithm_type_element->GetText();           

         try
         {
            set_main_training_algorithm_type(new_main_training_algorithm_type);
         }
         catch(std::exception& e)
         {
            std::cout << e.what() << std::endl;		 
         }
      }
   }

   // Refinement training algorithm type
   {
      TiXmlElement* refinement_training_algorithm_type_element = training_strategy_element->FirstChildElement("RefinementTrainingAlgorithmType");

      if(refinement_training_algorithm_type_element)
      {
         const std::string new_refinement_training_algorithm_type = refinement_training_algorithm_type_element->GetText();           

         try
         {
            set_refinement_training_algorithm_type(new_refinement_training_algorithm_type);
         }
         catch(std::exception& e)
         {
            std::cout << e.what() << std::endl;		 
         }
      }
   }

   // Initialization training algorithm flag
   {
      TiXmlElement* initialization_training_algorithm_flag_element = training_strategy_element->FirstChildElement("InitializationTrainingAlgorithmFlag");

      if(initialization_training_algorithm_flag_element)
      {
         const std::string new_initialization_training_algorithm_flag = initialization_training_algorithm_flag_element->GetText();           

         try
         {
            set_initialization_training_algorithm_flag(new_initialization_training_algorithm_flag != "0");
         }
         catch(std::exception& e)
         {
            std::cout << e.what() << std::endl;		 
         }
      }
   }

   // Main training algorithm flag
   {
      TiXmlElement* main_training_algorithm_flag_element = training_strategy_element->FirstChildElement("MainTrainingAlgorithmFlag");

      if(main_training_algorithm_flag_element)
      {
         const std::string new_main_training_algorithm_flag = main_training_algorithm_flag_element->GetText();           

         try
         {
            set_main_training_algorithm_flag(new_main_training_algorithm_flag != "0");
         }
         catch(std::exception& e)
         {
            std::cout << e.what() << std::endl;		 
         }
      }
   }

   // Refinement training algorithm flag
   {
      TiXmlElement* refinement_training_algorithm_flag_element = training_strategy_element->FirstChildElement("RefinementTrainingAlgorithmFlag");

      if(refinement_training_algorithm_flag_element)
      {
         const std::string new_refinement_training_algorithm_flag = refinement_training_algorithm_flag_element->GetText();           

         try
         {
            set_refinement_training_algorithm_flag(new_refinement_training_algorithm_flag != "0");
         }
         catch(std::exception& e)
         {
            std::cout << e.what() << std::endl;		 
         }
      }
   }

   // Initialization training algorithm 

   if(initialization_training_algorithm_pointer)
   {
      TiXmlElement* element = initialization_training_algorithm_pointer->to_XML();

      training_strategy_element->LinkEndChild(element);
   }

   // Main training algorithm

   if(main_training_algorithm_pointer)
   {
      TiXmlElement* element = main_training_algorithm_pointer->to_XML();

      training_strategy_element->LinkEndChild(element);
   }

   // Refinement training algorithm 

   if(refinement_training_algorithm_pointer)
   {
      TiXmlElement* element = refinement_training_algorithm_pointer->to_XML();

      training_strategy_element->LinkEndChild(element);
   }

   // Display 
   {
      TiXmlElement* display_element = training_strategy_element->FirstChildElement("Display");

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


// void save(const std::string&) const method

/// This method saves to a XML-type file the members of the training algorithm object. 
/// @param filename Name of training algorithm XML-type file. 

void TrainingStrategy::save(const std::string& filename) const
{
   std::ostringstream buffer;

   TiXmlDocument document;

   // Declaration

   TiXmlDeclaration* declaration = new TiXmlDeclaration("1.0", "", "");
   document.LinkEndChild(declaration);

   // Training strategy

   TiXmlElement* training_strategy_element = to_XML();
   document.LinkEndChild(training_strategy_element);

   document.SaveFile(filename.c_str());
}


// void load(const std::string&) method

/// This method loads a gradient descent object from a XML-type file. 
/// Please mind about the file format, wich is specified in the User's Guide. 
/// @param filename Name of training algorithm XML-type file. 

void TrainingStrategy::load(const std::string& filename)
{
   set_default();

   std::ostringstream buffer;

   TiXmlDocument document(filename.c_str());

   if (!document.LoadFile())
   {
      buffer << "OpenNN Exception: TrainingStrategy class.\n"
             << "void load(const std::string&) method.\n"
             << "Cannot load XML file " << filename << ".\n";

      throw std::logic_error(buffer.str());
   }

   // Training algorithm element
  
   TiXmlElement* training_algorithm_element = document.FirstChildElement("TrainingStrategy");

   if(!training_algorithm_element)
   {
      buffer << "OpenNN Exception: TrainingStrategy class.\n"
             << "void load(const std::string&) method.\n"
             << "File " << filename << " is not a valid training algorithm file.\n";

      throw std::logic_error(buffer.str());
   }
}


// void Results::save(const std::string&) const method

/// This method saves the results structure to a data file. 
/// @param filename Name of training strategy results data file. 

void TrainingStrategy::Results::save(const std::string& filename) const
{
   std::ofstream file(filename.c_str());

   if(initialization_training_algorithm_results_pointer)
   {
      file << initialization_training_algorithm_results_pointer->to_string();
   }

   if(main_training_algorithm_results_pointer)
   {
      file << main_training_algorithm_results_pointer->to_string();  
   }

   if(refinement_training_algorithm_results_pointer)
   {
      file << refinement_training_algorithm_results_pointer->to_string();  
   }

   file.close();
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
