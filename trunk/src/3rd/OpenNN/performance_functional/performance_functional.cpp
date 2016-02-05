/****************************************************************************************************************/
/*                                                                                                              */
/*   OpenNN: Open Neural Networks Library                                                                       */
/*   www.opennn.cimne.com                                                                                       */
/*                                                                                                              */
/*   P E R F O R M A N C E   F U N C T I O N A L   C L A S S                                                    */
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
#include <cmath>

// OpenNN includes

#include "../utilities/vector.h"
#include "../utilities/matrix.h"

#include "../utilities/numerical_differentiation.h"

#include "performance_functional.h"
#include "sum_squared_error.h"
#include "mean_squared_error.h"
#include "root_mean_squared_error.h"
#include "normalized_squared_error.h"
#include "minkowski_error.h"
#include "cross_entropy_error.h"

#include "inverse_sum_squared_error.h"

#include "neural_parameters_norm.h"
#include "outputs_integrals.h"
#include "independent_parameters_error.h"

#include "final_solutions_error.h"
#include "solutions_error.h"

// TinyXml includes

#include "../../tinyxml/tinyxml.h"


namespace OpenNN
{

// DEFAULT CONSTRUCTOR

/// Default constructor.  
/// It creates a performance functional object with all pointers initialized to NULL. 
/// It also initializes all the rest of class members to their default values.

PerformanceFunctional::PerformanceFunctional(void)
 : neural_network_pointer(NULL)
 , objective_term_pointer(NULL)
 , regularization_term_pointer(NULL)
 , constraints_term_pointer(NULL)
{
   set_default();

   construct_objective_term(objective_term_type);
}


// NEURAL NETWORK CONSTRUCTOR

/// Neural network constructor. 
/// It creates a performance functional object associated to a neural network object. 
/// The rest of pointers are initialized to NULL.
/// It also initializes all the rest of class members to their default values.
/// @param new_neural_network_pointer Pointer to a neural network object.

PerformanceFunctional::PerformanceFunctional(NeuralNetwork* new_neural_network_pointer)
 : neural_network_pointer(new_neural_network_pointer)
 , objective_term_pointer(NULL)
 , regularization_term_pointer(NULL)
 , constraints_term_pointer(NULL)
{
   set_default();

   construct_objective_term(objective_term_type);
}


// NEURAL NETWORK AND DATA SET CONSTRUCTOR

/// Neural network and data set constructor. 
/// It creates a performance functional object associated to a neural network and a data set objects. 
/// The rest of pointers are initialized to NULL.
/// It also initializes all the rest of class members to their default values.
/// @param new_neural_network_pointer Pointer to a neural network object.
/// @param new_data_set_pointer Pointer to a data set object.

PerformanceFunctional::PerformanceFunctional(NeuralNetwork* new_neural_network_pointer, DataSet* new_data_set_pointer)
 : neural_network_pointer(new_neural_network_pointer)
 , objective_term_pointer(NULL)
 , regularization_term_pointer(NULL)
 , constraints_term_pointer(NULL)
{
   set_default();

   construct_objective_term(objective_term_type);
   objective_term_pointer->set_data_set_pointer(new_data_set_pointer);
}


// NEURAL NETWORK AND MATHEMATICAL MODEL CONSTRUCTOR

/// Neural network and mathematical model constructor. 
/// It creates a performance functional object associated to a neural network and a mathematical model objects. 
/// The rest of pointers are initialized to NULL.
/// It also initializes all the rest of class members to their default values.
/// @param new_neural_network_pointer Pointer to a neural network object.
/// @param new_mathematical_model_pointer Pointer to a mathematical model object.

PerformanceFunctional::PerformanceFunctional(NeuralNetwork* new_neural_network_pointer, MathematicalModel* new_mathematical_model_pointer)
 : neural_network_pointer(new_neural_network_pointer)
 , objective_term_pointer(NULL)
 , regularization_term_pointer(NULL)
 , constraints_term_pointer(NULL)
{
   set_default();

   construct_objective_term(objective_term_type);
   objective_term_pointer->set_mathematical_model_pointer(new_mathematical_model_pointer);
}


// NEURAL NETWORK, MATHEMATICAL MODEL AND DATA SET CONSTRUCTOR

/// Neural network, mathematical model and data set constructor. 
/// It creates a performance functional object associated to a neural network, a mathematical model and a data set objects. 
/// The rest of pointers are initialized to NULL.
/// It also initializes all the rest of class members to their default values.
/// @param new_neural_network_pointer Pointer to a neural network object.
/// @param new_mathematical_model_pointer Pointer to a mathematical model object.
/// @param new_data_set_pointer Pointer to a data set object.

PerformanceFunctional::PerformanceFunctional(NeuralNetwork* new_neural_network_pointer, MathematicalModel* new_mathematical_model_pointer, DataSet* new_data_set_pointer)
 : neural_network_pointer(new_neural_network_pointer)
 , objective_term_pointer(NULL)
 , regularization_term_pointer(NULL)
 , constraints_term_pointer(NULL)
{
   set_default();

   construct_objective_term(objective_term_type, new_mathematical_model_pointer, new_data_set_pointer);
}


// OBJECTIVE TERM CONSTRUCTOR 

/// Objective term constructor. 
/// It creates a performance functional object with a given objective functional.
/// The rest of pointers are initialized to NULL. 
/// The other members are set to their default values, but the objective term type, which is set to USER_PERFORMANCE_TERM. 

PerformanceFunctional::PerformanceFunctional(PerformanceTerm* new_objective_term_pointer)
 : neural_network_pointer(NULL)
 , objective_term_pointer(new_objective_term_pointer)
 , regularization_term_pointer(NULL)
 , constraints_term_pointer(NULL)
{
   set_default();

   objective_term_type = PerformanceFunctional::USER_PERFORMANCE_TERM;
}


// FILE CONSTRUCTOR

/// File constructor. 
/// It creates a performance functional object by loading its members from an XML-type file.
/// Please be careful with the format of that file, which is specified in the OpenNN manual.
/// @param filename Name of performance functional file.

PerformanceFunctional::PerformanceFunctional(const std::string& filename)
 : neural_network_pointer(NULL)
 , objective_term_pointer(NULL)
 , regularization_term_pointer(NULL)
 , constraints_term_pointer(NULL)
{
   set_default();

   load(filename);
}


// COPY CONSTRUCTOR

/// Copy constructor. 
/// It creates a copy of an existing performance functional object. 
/// @param other_performance_functional Performance functional object to be copied.
/// @todo

PerformanceFunctional::PerformanceFunctional(const PerformanceFunctional& other_performance_functional)
 : neural_network_pointer(NULL)
 , objective_term_pointer(NULL)
 , regularization_term_pointer(NULL)
 , constraints_term_pointer(NULL)
{
   neural_network_pointer = other_performance_functional.neural_network_pointer;

   objective_term_type = other_performance_functional.objective_term_type;
   regularization_term_type = other_performance_functional.regularization_term_type;
   constraints_term_type = other_performance_functional.constraints_term_type;

   if(other_performance_functional.objective_term_pointer)
   {
	   construct_objective_term(objective_term_type);

	   objective_term_pointer->set(*other_performance_functional.objective_term_pointer);
   }

   if(other_performance_functional.regularization_term_pointer)
   {
	   construct_regularization_term(regularization_term_type);

	   regularization_term_pointer->set(*other_performance_functional.regularization_term_pointer);
   }

   if(other_performance_functional.constraints_term_pointer)
   {
	   construct_constraints_term(constraints_term_type);

	   constraints_term_pointer->set(*other_performance_functional.constraints_term_pointer);
   }

   objective_term_flag = other_performance_functional.objective_term_flag;

   regularization_term_flag = other_performance_functional.regularization_term_flag;

   constraints_term_flag = other_performance_functional.constraints_term_flag;

   display = other_performance_functional.display;  
}


// DESTRUCTOR

/// Destructor.
/// It deletes the objective, regularization and constraints terms. 

PerformanceFunctional::~PerformanceFunctional(void)
{
   delete objective_term_pointer;
   delete regularization_term_pointer;
   delete constraints_term_pointer;
}


// METHODS

// const PerformanceTermType& get_objective_term_type(void) const method

/// This method returns the type of objective term used in the performance functional expression. 

const PerformanceFunctional::PerformanceTermType& PerformanceFunctional::get_objective_term_type(void) const
{
   return(objective_term_type);
}


// const PerformanceTermType& get_regularization_term_type(void) const method

/// This method returns the type of regularization term used in the performance functional expression. 

const PerformanceFunctional::PerformanceTermType& PerformanceFunctional::get_regularization_term_type(void) const
{
   return(regularization_term_type);
}


// const PerformanceTermType& get_constraints_term_type(void) const method

/// This method returns the type of constraints term used in the performance functional expression. 

const PerformanceFunctional::PerformanceTermType& PerformanceFunctional::get_constraints_term_type(void) const
{
   return(constraints_term_type);
}


// std::string write_objective_term_type(void) const

/// This method returns a string with the type of objective term used in the performance functional expression. 

std::string PerformanceFunctional::write_objective_term_type(void) const
{
   if(objective_term_type == NONE)
   {
      return("NONE");
   }
   else if(objective_term_type == SUM_SQUARED_ERROR)
   {
      return("SUM_SQUARED_ERROR");
   }
   else if(objective_term_type == MEAN_SQUARED_ERROR)
   {
      return("MEAN_SQUARED_ERROR");
   }
   else if(objective_term_type == ROOT_MEAN_SQUARED_ERROR)
   {
      return("ROOT_MEAN_SQUARED_ERROR");
   }
   else if(objective_term_type == NORMALIZED_SQUARED_ERROR)
   {
      return("NORMALIZED_SQUARED_ERROR");
   }
   else if(objective_term_type == MINKOWSKI_ERROR)
   {
      return("MINKOWSKI_ERROR");
   }
   else if(objective_term_type == CROSS_ENTROPY_ERROR)
   {
      return("CROSS_ENTROPY_ERROR");
   }
   else if(objective_term_type == NEURAL_PARAMETERS_NORM)
   {
      return("NEURAL_PARAMETERS_NORM");
   }
   else if(objective_term_type == OUTPUTS_INTEGRALS)
   {
      return("OUTPUTS_INTEGRALS");
   }
   else if(objective_term_type == SOLUTION_ERROR)
   {
      return("SOLUTION_ERROR");
   }
   else if(objective_term_type == FINAL_SOLUTIONS_ERROR)
   {
      return("FINAL_SOLUTIONS_ERROR");
   }
   else if(objective_term_type == INDEPENDENT_PARAMETERS_ERROR)
   {
      return("INDEPENDENT_PARAMETERS_ERROR");
   }
   else if(objective_term_type == INVERSE_SUM_SQUARED_ERROR)
   {
      return("INVERSE_SUM_SQUARED_ERROR");
   }
   else if(objective_term_type == USER_PERFORMANCE_TERM)
   {
      return("USER_PERFORMANCE_TERM");
   }
   else
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: PerformanceFunctional class.\n"
             << "std::string write_objective_term_type(void) const method.\n"
             << "Unknown performance term type.\n";
 
	  throw std::logic_error(buffer.str());
   }
}


// std::string write_regularization_term_type(void) const method

/// This method returns a string with the type of regularization term used in the performance functional expression. 

std::string PerformanceFunctional::write_regularization_term_type(void) const
{
   if(regularization_term_type == NONE)
   {
      return("NONE");
   }
   else if(regularization_term_type == SUM_SQUARED_ERROR)
   {
      return("SUM_SQUARED_ERROR");
   }
   else if(regularization_term_type == MEAN_SQUARED_ERROR)
   {
      return("MEAN_SQUARED_ERROR");
   }
   else if(regularization_term_type == ROOT_MEAN_SQUARED_ERROR)
   {
      return("ROOT_MEAN_SQUARED_ERROR");
   }
   else if(regularization_term_type == NORMALIZED_SQUARED_ERROR)
   {
      return("NORMALIZED_SQUARED_ERROR");
   }
   else if(regularization_term_type == MINKOWSKI_ERROR)
   {
      return("MINKOWSKI_ERROR");
   }
   else if(regularization_term_type == CROSS_ENTROPY_ERROR)
   {
      return("CROSS_ENTROPY_ERROR");
   }
   else if(regularization_term_type == NEURAL_PARAMETERS_NORM)
   {
      return("NEURAL_PARAMETERS_NORM");
   }
   else if(regularization_term_type == OUTPUTS_INTEGRALS)
   {
      return("OUTPUTS_INTEGRALS");
   }
   else if(regularization_term_type == SOLUTION_ERROR)
   {
      return("SOLUTION_ERROR");
   }
   else if(regularization_term_type == FINAL_SOLUTIONS_ERROR)
   {
      return("FINAL_SOLUTIONS_ERROR");
   }
   else if(regularization_term_type == INDEPENDENT_PARAMETERS_ERROR)
   {
      return("INDEPENDENT_PARAMETERS_ERROR");
   }
   else if(regularization_term_type == INVERSE_SUM_SQUARED_ERROR)
   {
      return("INVERSE_SUM_SQUARED_ERROR");
   }
   else if(regularization_term_type == USER_PERFORMANCE_TERM)
   {
      return("USER_PERFORMANCE_TERM");
   }
   else
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: PerformanceFunctional class.\n"
             << "std::string write_regularization_term_type(void) const method.\n"
             << "Unknown performance term type.\n";
 
	  throw std::logic_error(buffer.str());
   }
}


// std::string write_constraints_term_type(void) const method

/// This method returns a string with the type of constraints term used in the performance functional expression. 

std::string PerformanceFunctional::write_constraints_term_type(void) const
{
   if(constraints_term_type == NONE)
   {
      return("NONE");
   }
   else if(constraints_term_type == SUM_SQUARED_ERROR)
   {
      return("SUM_SQUARED_ERROR");
   }
   else if(constraints_term_type == MEAN_SQUARED_ERROR)
   {
      return("MEAN_SQUARED_ERROR");
   }
   else if(constraints_term_type == ROOT_MEAN_SQUARED_ERROR)
   {
      return("ROOT_MEAN_SQUARED_ERROR");
   }
   else if(constraints_term_type == NORMALIZED_SQUARED_ERROR)
   {
      return("NORMALIZED_SQUARED_ERROR");
   }
   else if(constraints_term_type == MINKOWSKI_ERROR)
   {
      return("MINKOWSKI_ERROR");
   }
   else if(constraints_term_type == CROSS_ENTROPY_ERROR)
   {
      return("CROSS_ENTROPY_ERROR");
   }
   else if(constraints_term_type == NEURAL_PARAMETERS_NORM)
   {
      return("NEURAL_PARAMETERS_NORM");
   }
   else if(constraints_term_type == OUTPUTS_INTEGRALS)
   {
      return("OUTPUTS_INTEGRALS");
   }
   else if(constraints_term_type == SOLUTION_ERROR)
   {
      return("SOLUTION_ERROR");
   }
   else if(constraints_term_type == FINAL_SOLUTIONS_ERROR)
   {
      return("FINAL_SOLUTIONS_ERROR");
   }
   else if(constraints_term_type == INDEPENDENT_PARAMETERS_ERROR)
   {
      return("INDEPENDENT_PARAMETERS_ERROR");
   }
   else if(constraints_term_type == INVERSE_SUM_SQUARED_ERROR)
   {
      return("INVERSE_SUM_SQUARED_ERROR");
   }
   else if(constraints_term_type == USER_PERFORMANCE_TERM)
   {
      return("USER_PERFORMANCE_TERM");
   }
   else
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: PerformanceFunctional class.\n"
             << "std::string write_constraints_term_type(void) const method.\n"
             << "Unknown performance term type.\n";
 
	  throw std::logic_error(buffer.str());
   }
}


// const bool& get_objective_term_flag(void) const method

/// This method returns the flag value for including or not the objective term in the performance functional expression. 

const bool& PerformanceFunctional::get_objective_term_flag(void) const
{
   return(objective_term_flag);
}


// const bool& get_regularization_term_flag(void) const method

/// This method returns the flag value for using regularization or not. 

const bool& PerformanceFunctional::get_regularization_term_flag(void) const
{
   return(regularization_term_flag);
}


// const bool& get_constraints_term_flag(void) const method

/// This method returns true if constraints are to be included in the performance functional expression, 
/// and false otherwise. 

const bool& PerformanceFunctional::get_constraints_term_flag(void) const
{
   return(constraints_term_flag);
}


// const bool& get_display(void) const method

/// This method returns true if messages from this class can be displayed on the screen, or false if messages 
/// from this class can't be displayed on the screen.

const bool& PerformanceFunctional::get_display(void) const
{
   return(display);
}


// void set_neural_network_pointer(NeuralNetwork*) method

/// This method sets a pointer to a multilayer perceptron object which is to be associated to the performance functional.
/// @param new_neural_network_pointer Pointer to a neural network object to be associated to the performance functional.

void PerformanceFunctional::set_neural_network_pointer(NeuralNetwork* new_neural_network_pointer)
{
   neural_network_pointer = new_neural_network_pointer;
}


// void set_objective_term_pointer(PerformanceTerm*) method

/// This method sets a new objective term into the performance functional.
/// @param new_objective_term_pointer Pointer to a performance term representing the objective functional. 

void PerformanceFunctional::set_objective_term_pointer(PerformanceTerm* new_objective_term_pointer)
{
   destruct_objective_term();

   objective_term_pointer = new_objective_term_pointer;

   objective_term_flag = true;

   std::string objective_term_type = new_objective_term_pointer->write_performance_term_type();

   set_objective_term_type(objective_term_type);
}


// void set_regularization_term_pointer(PerformanceTerm*) method

/// This method sets a new regularization term into the performance functional.
/// @param new_regularization_term_pointer Pointer to a performance term representing the regularization functional. 

void PerformanceFunctional::set_regularization_term_pointer(PerformanceTerm* new_regularization_term_pointer)
{
   destruct_regularization_term();

   regularization_term_pointer = new_regularization_term_pointer;

   regularization_term_flag = true;

   std::string regularization_term_type = new_regularization_term_pointer->write_performance_term_type();

   set_regularization_term_type(regularization_term_type);
}


// void set_constraints_term_pointer(PerformanceTerm*) method

/// This method sets a new constraints term into the performance functional.
/// @param new_constraints_term_pointer Pointer to a performance term representing the constraints functional. 

void PerformanceFunctional::set_constraints_term_pointer(PerformanceTerm* new_constraints_term_pointer)
{
   destruct_constraints_term();

   constraints_term_pointer = new_constraints_term_pointer;

   constraints_term_flag = true;

   std::string constraints_term_type = new_constraints_term_pointer->write_performance_term_type();

   set_constraints_term_type(constraints_term_type);
}


// void set_default(void) method

/// This method sets the members of the performance functional object to their default values.

void PerformanceFunctional::set_default(void)
{
   set_objective_term_type(NORMALIZED_SQUARED_ERROR);
   set_regularization_term_type(NONE);
   set_constraints_term_type(NONE);

   objective_term_flag = true;
   regularization_term_flag = false;
   constraints_term_flag = false;

   display = true;
}


// void set_objective_term_type(const PerformanceTermType&) method

/// This method sets a new type for the objective term. 
/// @param new_objective_term_type Type of objective term. 

void PerformanceFunctional::set_objective_term_type(const PerformanceTermType& new_objective_term_type)
{
   objective_term_type = new_objective_term_type;
}


// void set_regularization_term_type(const PerformanceTermType&) method

/// This method sets a new type for the regularization term. 
/// @param new_regularization_term_type Type of regularization term. 

void PerformanceFunctional::set_regularization_term_type(const PerformanceTermType& new_regularization_term_type)
{
   regularization_term_type = new_regularization_term_type;
}


// void set_constraints_term_type(const PerformanceTermType&) method

/// This method sets a new type for the constraints term. 
/// @param new_constraints_term_type Type of constraints term. 

void PerformanceFunctional::set_constraints_term_type(const PerformanceTermType& new_constraints_term_type)
{
   constraints_term_type = new_constraints_term_type;
}


// void set_objective_term_type(const std::string&) method

/// This method sets a new type for the objective term from a string. 
/// @param new_objective_term_type String with the type of objective term. 

void PerformanceFunctional::set_objective_term_type(const std::string& new_objective_term_type)
{
   if(new_objective_term_type == "NONE")
   {
      set_objective_term_type(NONE);
   }
   else if(new_objective_term_type == "SUM_SQUARED_ERROR")
   {
      set_objective_term_type(SUM_SQUARED_ERROR);
   }
   else if(new_objective_term_type == "MEAN_SQUARED_ERROR")
   {
      set_objective_term_type(MEAN_SQUARED_ERROR);
   }
   else if(new_objective_term_type == "ROOT_MEAN_SQUARED_ERROR")
   {
      set_objective_term_type(ROOT_MEAN_SQUARED_ERROR);
   }
   else if(new_objective_term_type == "NORMALIZED_SQUARED_ERROR")
   {
      set_objective_term_type(NORMALIZED_SQUARED_ERROR);
   }
   else if(new_objective_term_type == "MINKOWSKI_ERROR")
   {
      set_objective_term_type(MINKOWSKI_ERROR);
   }
   else if(new_objective_term_type == "MINKOWSKI_ERROR")
   {
      set_objective_term_type(MINKOWSKI_ERROR);
   }
   else if(new_objective_term_type == "NEURAL_PARAMETERS_NORM")
   {
      set_objective_term_type(NEURAL_PARAMETERS_NORM);
   }
   else if(new_objective_term_type == "OUTPUTS_INTEGRALS")
   {
      set_objective_term_type(OUTPUTS_INTEGRALS);
   }
   else if(new_objective_term_type == "SOLUTION_ERROR")
   {
      set_objective_term_type(SOLUTION_ERROR);
   }
   else if(new_objective_term_type == "FINAL_SOLUTIONS_ERROR")
   {
      set_objective_term_type(FINAL_SOLUTIONS_ERROR);
   }
   else if(new_objective_term_type == "INDEPENDENT_PARAMETERS_ERROR")
   {
      set_objective_term_type(INDEPENDENT_PARAMETERS_ERROR);
   }
   else if(new_objective_term_type == "INVERSE_SUM_SQUARED_ERROR")
   {
      set_objective_term_type(INVERSE_SUM_SQUARED_ERROR);
   }
   else if(new_objective_term_type == "USER_PERFORMANCE_TERM")
   {
      set_objective_term_type(USER_PERFORMANCE_TERM);
   }
   else
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: PerformanceFunctional class.\n"
             << "void set_objective_term_type(const std::string&) method.\n"
   	         << "Unknown performance term type: " << new_objective_term_type << ".\n";

      throw std::logic_error(buffer.str());
   }   
}


// void set_regularization_term_type(const std::string&) method

/// This method sets a new type for the regularization term from a string. 
/// @param new_regularization_term_type String with the type of regularization term. 

void PerformanceFunctional::set_regularization_term_type(const std::string& new_regularization_term_type)
{
   if(new_regularization_term_type == "NONE")
   {
      set_regularization_term_type(NONE);
   }
   else if(new_regularization_term_type == "SUM_SQUARED_ERROR")
   {
      set_regularization_term_type(SUM_SQUARED_ERROR);
   }
   else if(new_regularization_term_type == "MEAN_SQUARED_ERROR")
   {
      set_regularization_term_type(MEAN_SQUARED_ERROR);
   }
   else if(new_regularization_term_type == "ROOT_MEAN_SQUARED_ERROR")
   {
      set_regularization_term_type(ROOT_MEAN_SQUARED_ERROR);
   }
   else if(new_regularization_term_type == "NORMALIZED_SQUARED_ERROR")
   {
      set_regularization_term_type(NORMALIZED_SQUARED_ERROR);
   }
   else if(new_regularization_term_type == "MINKOWSKI_ERROR")
   {
      set_regularization_term_type(MINKOWSKI_ERROR);
   }
   else if(new_regularization_term_type == "MINKOWSKI_ERROR")
   {
      set_regularization_term_type(MINKOWSKI_ERROR);
   }
   else if(new_regularization_term_type == "NEURAL_PARAMETERS_NORM")
   {
      set_regularization_term_type(NEURAL_PARAMETERS_NORM);
   }
   else if(new_regularization_term_type == "OUTPUTS_INTEGRALS")
   {
      set_regularization_term_type(OUTPUTS_INTEGRALS);
   }
   else if(new_regularization_term_type == "SOLUTION_ERROR")
   {
      set_regularization_term_type(SOLUTION_ERROR);
   }
   else if(new_regularization_term_type == "FINAL_SOLUTIONS_ERROR")
   {
      set_regularization_term_type(FINAL_SOLUTIONS_ERROR);
   }
   else if(new_regularization_term_type == "INDEPENDENT_PARAMETERS_ERROR")
   {
      set_regularization_term_type(INDEPENDENT_PARAMETERS_ERROR);
   }
   else if(new_regularization_term_type == "INVERSE_SUM_SQUARED_ERROR")
   {
      set_regularization_term_type(INVERSE_SUM_SQUARED_ERROR);
   }
   else if(new_regularization_term_type == "USER_PERFORMANCE_TERM")
   {
      set_regularization_term_type(USER_PERFORMANCE_TERM);
   }
   else
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: PerformanceFunctional class.\n"
             << "void set_regularization_term_type(const std::string&) method.\n"
   	         << "Unknown performance functional type: " << new_regularization_term_type << ".\n";

      throw std::logic_error(buffer.str());
   }   
}


// void set_constraints_term_type(const std::string&) method

/// This method sets a new type for the constraints term from a string. 
/// @param new_constraints_term_type String with the type of constraints term. 

void PerformanceFunctional::set_constraints_term_type(const std::string& new_constraints_term_type)
{
   if(new_constraints_term_type == "NONE")
   {
      set_constraints_term_type(NONE);
   }
   else if(new_constraints_term_type == "SUM_SQUARED_ERROR")
   {
      set_constraints_term_type(SUM_SQUARED_ERROR);
   }
   else if(new_constraints_term_type == "MEAN_SQUARED_ERROR")
   {
      set_constraints_term_type(MEAN_SQUARED_ERROR);
   }
   else if(new_constraints_term_type == "ROOT_MEAN_SQUARED_ERROR")
   {
      set_constraints_term_type(ROOT_MEAN_SQUARED_ERROR);
   }
   else if(new_constraints_term_type == "NORMALIZED_SQUARED_ERROR")
   {
      set_constraints_term_type(NORMALIZED_SQUARED_ERROR);
   }
   else if(new_constraints_term_type == "MINKOWSKI_ERROR")
   {
      set_constraints_term_type(MINKOWSKI_ERROR);
   }
   else if(new_constraints_term_type == "MINKOWSKI_ERROR")
   {
      set_constraints_term_type(MINKOWSKI_ERROR);
   }
   else if(new_constraints_term_type == "NEURAL_PARAMETERS_NORM")
   {
      set_constraints_term_type(NEURAL_PARAMETERS_NORM);
   }
   else if(new_constraints_term_type == "OUTPUTS_INTEGRALS")
   {
      set_constraints_term_type(OUTPUTS_INTEGRALS);
   }
   else if(new_constraints_term_type == "SOLUTION_ERROR")
   {
      set_constraints_term_type(SOLUTION_ERROR);
   }
   else if(new_constraints_term_type == "FINAL_SOLUTIONS_ERROR")
   {
      set_constraints_term_type(FINAL_SOLUTIONS_ERROR);
   }
   else if(new_constraints_term_type == "INDEPENDENT_PARAMETERS_ERROR")
   {
      set_constraints_term_type(INDEPENDENT_PARAMETERS_ERROR);
   }
   else if(new_constraints_term_type == "INVERSE_SUM_SQUARED_ERROR")
   {
      set_constraints_term_type(INVERSE_SUM_SQUARED_ERROR);
   }
   else if(new_constraints_term_type == "USER_PERFORMANCE_TERM")
   {
      set_constraints_term_type(USER_PERFORMANCE_TERM);
   }
   else
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: PerformanceFunctional class.\n"
             << "void set_constraints_term_type(const std::string&) method.\n"
   	         << "Unknown performance term type: " << new_constraints_term_type << ".\n";

      throw std::logic_error(buffer.str());
   }   
}


// void set_objective_term_flag(const bool&) method

/// This method sets a new flag value for using the objective term or not. 
/// @param new_objective_term_flag Flag value for the use of objective term. 

void PerformanceFunctional::set_objective_term_flag(const bool& new_objective_term_flag)
{
   objective_term_flag = new_objective_term_flag;
}


// void set_regularization_term_flag(const bool&) method

/// This method sets a new flag value for using regularization or not. 
/// @param new_regularization_term_flag Flag value for the use of regularization. 

void PerformanceFunctional::set_regularization_term_flag(const bool& new_regularization_term_flag)
{
   regularization_term_flag = new_regularization_term_flag;
}


// void set_constraints_term_flag(const bool&) method

/// This method sets a new flag value for using constraints or not in the performance functional expression. 
/// @param new_constraints_term_flag Flag value for the use of constraints. 

void PerformanceFunctional::set_constraints_term_flag(const bool& new_constraints_term_flag)
{
   constraints_term_flag = new_constraints_term_flag;
}


// void set_display(const bool&) method

/// This method sets a new display value. 
/// If it is set to true messages from this class are to be displayed on the screen;
/// if it is set to false messages from this class are not to be displayed on the screen.
/// @param new_display Display value.

void PerformanceFunctional::set_display(const bool& new_display)
{
   display = new_display;
}


// void construct_objective_term(const PerformanceTermType&) method

/// This method creates a new objective term inside the performance functional of a given performance term type. 
/// @param new_objective_term_type Type of objective term to be created. 

void PerformanceFunctional::construct_objective_term(const PerformanceTermType& new_objective_term_type)
{
   if(objective_term_pointer)
   {
      delete objective_term_pointer;
   }

   objective_term_type = new_objective_term_type;
   objective_term_flag = true;

   switch(new_objective_term_type)
   {
      case NONE:
      {
         objective_term_pointer = NULL;
      }
      break;

      case SUM_SQUARED_ERROR:
      {
         objective_term_pointer = new SumSquaredError(neural_network_pointer);
      }
      break;

      case MEAN_SQUARED_ERROR:
      {
         objective_term_pointer = new MeanSquaredError(neural_network_pointer);
      }
      break;

      case ROOT_MEAN_SQUARED_ERROR:
      {
         objective_term_pointer = new RootMeanSquaredError(neural_network_pointer);
      }
      break;

      case NORMALIZED_SQUARED_ERROR:
      {
         objective_term_pointer = new NormalizedSquaredError(neural_network_pointer);
      }
      break;

      case MINKOWSKI_ERROR:
      {
         objective_term_pointer = new MinkowskiError(neural_network_pointer);
      }
      break;

      case CROSS_ENTROPY_ERROR:
      {
         objective_term_pointer = new CrossEntropyError(neural_network_pointer);
      }
      break;

      case NEURAL_PARAMETERS_NORM:
      {
         objective_term_pointer = new NeuralParametersNorm(neural_network_pointer);
      }
      break;

      case OUTPUTS_INTEGRALS:
      {
         objective_term_pointer = new OutputsIntegrals(neural_network_pointer);
      }
      break;

      case SOLUTION_ERROR:
      {
         objective_term_pointer = new SolutionsError(neural_network_pointer);
      }
      break;

      case FINAL_SOLUTIONS_ERROR:
      {
         objective_term_pointer = new FinalSolutionsError(neural_network_pointer);
      }
      break;

      case INDEPENDENT_PARAMETERS_ERROR:
      {
         objective_term_pointer = new IndependentParametersError(neural_network_pointer);
      }
      break;

      case INVERSE_SUM_SQUARED_ERROR:
      {
         objective_term_pointer = new InverseSumSquaredError(neural_network_pointer);
      }
      break;

      case USER_PERFORMANCE_TERM:
      {
         objective_term_pointer = NULL;
      }
      break;

      default:
      {
         std::ostringstream buffer;

         buffer << "OpenNN Exception: PerformanceFunctional class.\n"
                << "void construct_objective_term(const PerformanceTermType&) method.\n"
                << "Unknown performance term type.\n";
 
         throw std::logic_error(buffer.str().c_str());	     
      }
      break;
   }
}


// void construct_regularization_term(const PerformanceTermType&) method

/// This method creates a new regularization term inside the performance functional of a given performance term type. 
/// @param new_regularization_term_type Type of regularization term to be created. 

void PerformanceFunctional::construct_regularization_term(const PerformanceTermType& new_regularization_term_type)
{
   if(regularization_term_pointer)
   {
      delete regularization_term_pointer;
   }

   regularization_term_type = new_regularization_term_type;
   regularization_term_flag = true;

   switch(regularization_term_type)
   {
      case NONE:
      {
         regularization_term_pointer = NULL;
      }
      break;

      case SUM_SQUARED_ERROR:
      {
         regularization_term_pointer = new SumSquaredError(neural_network_pointer);
      }
      break;

      case MEAN_SQUARED_ERROR:
      {
         regularization_term_pointer = new MeanSquaredError(neural_network_pointer);
      }
      break;

      case ROOT_MEAN_SQUARED_ERROR:
      {
         regularization_term_pointer = new RootMeanSquaredError(neural_network_pointer);
      }
      break;

      case NORMALIZED_SQUARED_ERROR:
      {
         regularization_term_pointer = new NormalizedSquaredError(neural_network_pointer);
      }
      break;

      case MINKOWSKI_ERROR:
      {
         regularization_term_pointer = new MinkowskiError(neural_network_pointer);
      }
      break;

      case CROSS_ENTROPY_ERROR:
      {
         regularization_term_pointer = new CrossEntropyError(neural_network_pointer);
      }
      break;

      case NEURAL_PARAMETERS_NORM:
      {
         regularization_term_pointer = new NeuralParametersNorm(neural_network_pointer);
      }
      break;

      case OUTPUTS_INTEGRALS:
      {
         regularization_term_pointer = new OutputsIntegrals(neural_network_pointer);
      }
      break;

      case SOLUTION_ERROR:
      {
         regularization_term_pointer = new SolutionsError(neural_network_pointer);
      }
      break;

      case FINAL_SOLUTIONS_ERROR:
      {
         regularization_term_pointer = new FinalSolutionsError(neural_network_pointer);
      }
      break;

      case INDEPENDENT_PARAMETERS_ERROR:
      {
         regularization_term_pointer = new IndependentParametersError(neural_network_pointer);
      }
      break;

      case INVERSE_SUM_SQUARED_ERROR:
      {
         regularization_term_pointer = new InverseSumSquaredError(neural_network_pointer);
      }
      break;

      case USER_PERFORMANCE_TERM:
      {
         regularization_term_pointer = NULL;
      }
      break;

      default:
      {
         std::ostringstream buffer;

         buffer << "OpenNN Exception: PerformanceFunctional class.\n"
                << "void set_regularization_term_type(const PerformanceTermType&) method.\n"
                << "Unknown performance term type.\n";
 
         throw std::logic_error(buffer.str().c_str());	     
      }
      break;
   }
}


// void construct_constraints_term_type(const PerformanceTermType&) method

/// This method creates a new constraints term inside the performance functional of a given performance term type. 
/// @param new_constraints_term_type Type of constraints term to be created. 

void PerformanceFunctional::construct_constraints_term(const PerformanceTermType& new_constraints_term_type)
{
   if(constraints_term_pointer)
   {
      delete constraints_term_pointer;
   }

   constraints_term_type = new_constraints_term_type;
   constraints_term_flag = true;

   switch(constraints_term_type)
   {
      case NONE:
      {
         constraints_term_pointer = NULL;
      }
      break;

      case SUM_SQUARED_ERROR:
      {
         constraints_term_pointer = new SumSquaredError(neural_network_pointer);
      }
      break;

      case MEAN_SQUARED_ERROR:
      {
         constraints_term_pointer = new MeanSquaredError(neural_network_pointer);
      }
      break;

      case ROOT_MEAN_SQUARED_ERROR:
      {
         constraints_term_pointer = new RootMeanSquaredError(neural_network_pointer);
      }
      break;

      case NORMALIZED_SQUARED_ERROR:
      {
         constraints_term_pointer = new NormalizedSquaredError(neural_network_pointer);
      }
      break;

      case MINKOWSKI_ERROR:
      {
         constraints_term_pointer = new MinkowskiError(neural_network_pointer);
      }
      break;

      case CROSS_ENTROPY_ERROR:
      {
         constraints_term_pointer = new CrossEntropyError(neural_network_pointer);
      }
      break;

      case NEURAL_PARAMETERS_NORM:
      {
         constraints_term_pointer = new NeuralParametersNorm(neural_network_pointer);
      }
      break;

      case OUTPUTS_INTEGRALS:
      {
         constraints_term_pointer = new OutputsIntegrals(neural_network_pointer);
      }
      break;

      case SOLUTION_ERROR:
      {
         constraints_term_pointer = new SolutionsError(neural_network_pointer);
      }
      break;

      case FINAL_SOLUTIONS_ERROR:
      {
         constraints_term_pointer = new FinalSolutionsError(neural_network_pointer);
      }
      break;

      case INDEPENDENT_PARAMETERS_ERROR:
      {
         constraints_term_pointer = new IndependentParametersError(neural_network_pointer);
      }
      break;

      case INVERSE_SUM_SQUARED_ERROR:
      {
         constraints_term_pointer = new InverseSumSquaredError(neural_network_pointer);
      }
      break;

      case USER_PERFORMANCE_TERM:
      {
         constraints_term_pointer = NULL;
      }
      break;

      default:
      {
         std::ostringstream buffer;

         buffer << "OpenNN Exception: PerformanceFunctional class.\n"
                << "void set_constraints_term_type(const PerformanceTermType&) method.\n"
                << "Unknown performance term type.\n";
 
         throw std::logic_error(buffer.str().c_str());	     
      }
      break;
   }
}


// void construct_objective_term(const PerformanceTermType&, MathematicalModel*) method

/// This method creates a new objective term inside the performance functional. 
/// @param new_performance_term_type Type of objective term. 
/// @param new_mathematical_model_pointer Pointer to a mathematical model object. 

void PerformanceFunctional::construct_objective_term(const PerformanceTermType& new_performance_term_type, MathematicalModel* new_mathematical_model_pointer)
{
   construct_objective_term(new_performance_term_type);

   objective_term_pointer->set_mathematical_model_pointer(new_mathematical_model_pointer);
}


// void construct_regularization_term(const PerformanceTermType&, MathematicalModel*)

/// This method creates a new regularization term inside the performance functional. 
/// @param new_performance_term_type Type of objective term. 
/// @param new_mathematical_model_pointer Pointer to a mathematical model object. 

void PerformanceFunctional::construct_regularization_term(const PerformanceTermType& new_performance_term_type, MathematicalModel* new_mathematical_model_pointer)
{
   construct_regularization_term(new_performance_term_type);

   regularization_term_pointer->set_mathematical_model_pointer(new_mathematical_model_pointer);
}


// void construct_constraints_term(const PerformanceTermType&, MathematicalModel*)

/// This method creates a new constraints term inside the performance functional. 
/// @param new_performance_term_type Type of objective term. 
/// @param new_mathematical_model_pointer Pointer to a mathematical model object. 

void PerformanceFunctional::construct_constraints_term(const PerformanceTermType& new_performance_term_type, MathematicalModel* new_mathematical_model_pointer)
{
   construct_constraints_term(new_performance_term_type);

   constraints_term_pointer->set_mathematical_model_pointer(new_mathematical_model_pointer);
}


// void construct_objective_term(const PerformanceTermType&, DataSet*)

/// This method creates a new objective term inside the performance functional. 
/// @param new_performance_term_type Type of objective term. 
/// @param new_data_set_pointer Pointer to a data set object. 

void PerformanceFunctional::construct_objective_term(const PerformanceTermType& new_performance_term_type, DataSet* new_data_set_pointer)
{
   construct_objective_term(new_performance_term_type);

   objective_term_pointer->set_data_set_pointer(new_data_set_pointer);
}


// void construct_regularization_term(const PerformanceTermType&, DataSet*)

/// This method creates a new regularization term inside the performance functional. 
/// @param new_performance_term_type Type of regularization term. 
/// @param new_data_set_pointer Pointer to a data set object. 

void PerformanceFunctional::construct_regularization_term(const PerformanceTermType& new_performance_term_type, DataSet* new_data_set_pointer)
{
   construct_regularization_term(new_performance_term_type);

   regularization_term_pointer->set_data_set_pointer(new_data_set_pointer);
}


// void construct_constraints_term(const PerformanceTermType&, DataSet*)

/// This method creates a new constraints term inside the performance functional. 
/// @param new_performance_term_type Type of constraints term. 
/// @param new_data_set_pointer Pointer to a data set object. 

void PerformanceFunctional::construct_constraints_term(const PerformanceTermType& new_performance_term_type, DataSet* new_data_set_pointer)
{
   construct_constraints_term(new_performance_term_type);

   constraints_term_pointer->set_data_set_pointer(new_data_set_pointer);
}


// void construct_objective_term(const PerformanceTermType&, MathematicalModel*, DataSet*)

/// This method creates a new objective term inside the performance functional. 
/// @param new_performance_term_type Type of objective term. 
/// @param new_mathematical_model_pointer Pointer to a mathematical model object. 
/// @param new_data_set_pointer Pointer to a data set object. 

void PerformanceFunctional::construct_objective_term(const PerformanceTermType& new_performance_term_type, MathematicalModel* new_mathematical_model_pointer, DataSet* new_data_set_pointer)
{
   construct_objective_term(new_performance_term_type);

   objective_term_pointer->set_mathematical_model_pointer(new_mathematical_model_pointer);
   objective_term_pointer->set_data_set_pointer(new_data_set_pointer);
}


// void construct_regularization_term(const PerformanceTermType&, MathematicalModel*, DataSet*)

/// This method creates a new regularization term inside the performance functional. 
/// @param new_performance_term_type Type of regularization term. 
/// @param new_mathematical_model_pointer Pointer to a mathematical model object. 
/// @param new_data_set_pointer Pointer to a data set object. 

void PerformanceFunctional::construct_regularization_term(const PerformanceTermType& new_performance_term_type, MathematicalModel* new_mathematical_model_pointer, DataSet* new_data_set_pointer)
{
   construct_regularization_term(new_performance_term_type);

   regularization_term_pointer->set_mathematical_model_pointer(new_mathematical_model_pointer);
   regularization_term_pointer->set_data_set_pointer(new_data_set_pointer);
}


// void construct_constraints_term(const PerformanceTermType&, MathematicalModel*, DataSet*)

/// This method creates a new constraints term inside the performance functional. 
/// @param new_performance_term_type Type of constraints term. 
/// @param new_mathematical_model_pointer Pointer to a mathematical model object. 
/// @param new_data_set_pointer Pointer to a data set object. 

void PerformanceFunctional::construct_constraints_term(const PerformanceTermType& new_performance_term_type, MathematicalModel* new_mathematical_model_pointer, DataSet* new_data_set_pointer)
{
   construct_constraints_term(new_performance_term_type);

   constraints_term_pointer->set_mathematical_model_pointer(new_mathematical_model_pointer);
   constraints_term_pointer->set_data_set_pointer(new_data_set_pointer);
}


// void construct_objective_term_type(const std::string&) method

/// This method creates a new objective term inside the performance functional of a given performance term type. 
/// @param new_objective_term_type String with the type of objective term to be created. 

void PerformanceFunctional::construct_objective_term(const std::string& new_objective_term_type)
{
   if(new_objective_term_type == "NONE")
   {
      construct_objective_term(NONE);
   }
   else if(new_objective_term_type == "SUM_SQUARED_ERROR")
   {
      construct_objective_term(SUM_SQUARED_ERROR);
   }
   else if(new_objective_term_type == "MEAN_SQUARED_ERROR")
   {
      construct_objective_term(MEAN_SQUARED_ERROR);
   }
   else if(new_objective_term_type == "ROOT_MEAN_SQUARED_ERROR")
   {
      construct_objective_term(ROOT_MEAN_SQUARED_ERROR);
   }
   else if(new_objective_term_type == "NORMALIZED_SQUARED_ERROR")
   {
      construct_objective_term(NORMALIZED_SQUARED_ERROR);
   }
   else if(new_objective_term_type == "MINKOWSKI_ERROR")
   {
      construct_objective_term(MINKOWSKI_ERROR);
   }
   else if(new_objective_term_type == "MINKOWSKI_ERROR")
   {
      construct_objective_term(MINKOWSKI_ERROR);
   }
   else if(new_objective_term_type == "NEURAL_PARAMETERS_NORM")
   {
      construct_objective_term(NEURAL_PARAMETERS_NORM);
   }
   else if(new_objective_term_type == "OUTPUTS_INTEGRALS")
   {
      construct_objective_term(OUTPUTS_INTEGRALS);
   }
   if(new_objective_term_type == "SOLUTION_ERROR")
   {
      construct_objective_term(SOLUTION_ERROR);
   }
   if(new_objective_term_type == "FINAL_SOLUTIONS_ERROR")
   {
      construct_objective_term(FINAL_SOLUTIONS_ERROR);
   }
   else if(new_objective_term_type == "INDEPENDENT_PARAMETERS_ERROR")
   {
      construct_objective_term(INDEPENDENT_PARAMETERS_ERROR);
   }
   else if(new_objective_term_type == "INVERSE_SUM_SQUARED_ERROR")
   {
      construct_objective_term(INVERSE_SUM_SQUARED_ERROR);
   }
   else if(new_objective_term_type == "USER_PERFORMANCE_TERM")
   {
      construct_objective_term(USER_PERFORMANCE_TERM);
   }
   else
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: PerformanceFunctional class.\n"
             << "void construct_objective_term(const std::string&) method.\n"
   	         << "Unknown objective term type: " << new_objective_term_type << ".\n";

      throw std::logic_error(buffer.str());
   }   
}


// void construct_regularization_term(const std::string&) method

/// This method creates a new regularization term inside the performance functional of a given performance term type. 
/// @param new_regularization_term_type String with the type of regularization term to be created. 

void PerformanceFunctional::construct_regularization_term(const std::string& new_regularization_term_type)
{
   if(new_regularization_term_type == "NONE")
   {
      construct_regularization_term(NONE);
   }
   else if(new_regularization_term_type == "SUM_SQUARED_ERROR")
   {
      construct_regularization_term(SUM_SQUARED_ERROR);
   }
   else if(new_regularization_term_type == "MEAN_SQUARED_ERROR")
   {
      construct_regularization_term(MEAN_SQUARED_ERROR);
   }
   else if(new_regularization_term_type == "ROOT_MEAN_SQUARED_ERROR")
   {
      construct_regularization_term(ROOT_MEAN_SQUARED_ERROR);
   }
   else if(new_regularization_term_type == "NORMALIZED_SQUARED_ERROR")
   {
      construct_regularization_term(NORMALIZED_SQUARED_ERROR);
   }
   else if(new_regularization_term_type == "MINKOWSKI_ERROR")
   {
      construct_regularization_term(MINKOWSKI_ERROR);
   }
   else if(new_regularization_term_type == "MINKOWSKI_ERROR")
   {
      construct_regularization_term(MINKOWSKI_ERROR);
   }
   else if(new_regularization_term_type == "NEURAL_PARAMETERS_NORM")
   {
      construct_regularization_term(NEURAL_PARAMETERS_NORM);
   }
   else if(new_regularization_term_type == "OUTPUTS_INTEGRALS")
   {
      construct_regularization_term(OUTPUTS_INTEGRALS);
   }
   if(new_regularization_term_type == "SOLUTION_ERROR")
   {
      construct_regularization_term(SOLUTION_ERROR);
   }
   if(new_regularization_term_type == "FINAL_SOLUTIONS_ERROR")
   {
      construct_regularization_term(FINAL_SOLUTIONS_ERROR);
   }
   else if(new_regularization_term_type == "INDEPENDENT_PARAMETERS_ERROR")
   {
      construct_regularization_term(INDEPENDENT_PARAMETERS_ERROR);
   }
   else if(new_regularization_term_type == "INVERSE_SUM_SQUARED_ERROR")
   {
      construct_regularization_term(INVERSE_SUM_SQUARED_ERROR);
   }
   else if(new_regularization_term_type == "USER_PERFORMANCE_TERM")
   {
      construct_regularization_term(USER_PERFORMANCE_TERM);
   }
   else
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: PerformanceFunctional class.\n"
             << "void construct_regularization_term(const std::string&) method.\n"
   	         << "Unknown regularization term type: " << new_regularization_term_type << ".\n";

      throw std::logic_error(buffer.str());
   }   
}


// void set_constraints_term_type(const std::string&) method

/// This method creates a new constraints term inside the performance functional of a given performance term type. 
/// @param new_constraints_term_type String with the type of constraints term to be created. 

void PerformanceFunctional::construct_constraints_term(const std::string& new_constraints_term_type)
{
   if(new_constraints_term_type == "NONE")
   {
      construct_constraints_term(NONE);
   }
   else if(new_constraints_term_type == "SUM_SQUARED_ERROR")
   {
      construct_constraints_term(SUM_SQUARED_ERROR);
   }
   else if(new_constraints_term_type == "MEAN_SQUARED_ERROR")
   {
      construct_constraints_term(MEAN_SQUARED_ERROR);
   }
   else if(new_constraints_term_type == "ROOT_MEAN_SQUARED_ERROR")
   {
      construct_constraints_term(ROOT_MEAN_SQUARED_ERROR);
   }
   else if(new_constraints_term_type == "NORMALIZED_SQUARED_ERROR")
   {
      construct_constraints_term(NORMALIZED_SQUARED_ERROR);
   }
   else if(new_constraints_term_type == "MINKOWSKI_ERROR")
   {
      construct_constraints_term(MINKOWSKI_ERROR);
   }
   else if(new_constraints_term_type == "MINKOWSKI_ERROR")
   {
      construct_constraints_term(MINKOWSKI_ERROR);
   }
   else if(new_constraints_term_type == "NEURAL_PARAMETERS_NORM")
   {
      construct_constraints_term(NEURAL_PARAMETERS_NORM);
   }
   else if(new_constraints_term_type == "OUTPUTS_INTEGRALS")
   {
      construct_constraints_term(OUTPUTS_INTEGRALS);
   }
   if(new_constraints_term_type == "SOLUTION_ERROR")
   {
      construct_constraints_term(SOLUTION_ERROR);
   }
   if(new_constraints_term_type == "FINAL_SOLUTIONS_ERROR")
   {
      construct_constraints_term(FINAL_SOLUTIONS_ERROR);
   }
   else if(new_constraints_term_type == "INDEPENDENT_PARAMETERS_ERROR")
   {
      construct_constraints_term(INDEPENDENT_PARAMETERS_ERROR);
   }
   else if(new_constraints_term_type == "INVERSE_SUM_SQUARED_ERROR")
   {
      construct_constraints_term(INVERSE_SUM_SQUARED_ERROR);
   }
   else if(new_constraints_term_type == "USER_PERFORMANCE_TERM")
   {
      construct_constraints_term(USER_PERFORMANCE_TERM);
   }
   else
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: PerformanceFunctional class.\n"
             << "void construct_constraints_term(const std::string&) method.\n"
   	         << "Unknown constraints term type: " << new_constraints_term_type << ".\n";

      throw std::logic_error(buffer.str());
   }   
}


// void destruct_objective_term(void) method

/// This method deletes the objective term object. 
/// It also sets the objective term type to NONE and the corresponding flag to false. 

void PerformanceFunctional::destruct_objective_term(void)
{
   delete objective_term_pointer;

   objective_term_type = NONE;

   objective_term_pointer = NULL;

   objective_term_flag = false;
}


// void destruct_regularization_term(void) method

/// This method deletes the regularization term object. 
/// It also sets the regularization term type to NONE and the corresponding flag to false. 

void PerformanceFunctional::destruct_regularization_term(void)
{
   delete regularization_term_pointer;

   regularization_term_type = NONE;

   regularization_term_pointer = NULL;

   regularization_term_flag = false;
}


// void destruct_constraints_term(void) method

/// This method deletes the constraints term object. 
/// It also sets the constraints term type to NONE and the corresponding flag to false. 

void PerformanceFunctional::destruct_constraints_term(void)
{
   delete constraints_term_pointer;

   constraints_term_type = NONE;

   constraints_term_pointer = NULL;

   constraints_term_flag = false;
}


// void destruct_all_terms(void) method

/// This method destructs the objective, regularization and constraints terms. 

void PerformanceFunctional::destruct_all_terms(void)
{
   destruct_objective_term();
   destruct_regularization_term();
   destruct_constraints_term();
}


// double calculate_evaluation(void) const method

/// This method calculates the performance value of the performance functional, 
/// as the sum of the objective and the regularization terms. 

double PerformanceFunctional::calculate_evaluation(void) const 
{
   // Control sentence (if debug)

   #ifdef _DEBUG 

   std::ostringstream buffer;

   if(!neural_network_pointer)
   {

      buffer << "OpenNN Exception: PerformanceFunctional class.\n"
             << "double calculate_evaluation(void) const method.\n"
             << "Pointer to neural network is NULL.\n";

      throw std::logic_error(buffer.str().c_str());	  
   }

   #endif

   double performance = 0.0;

   if(objective_term_flag)
   {
      #ifdef _DEBUG 

      if(!objective_term_pointer)
      {
         buffer << "OpenNN Exception: PerformanceFunctional class.\n"
                << "double calculate_evaluation(void) const method.\n"
                << "Pointer to performance term is NULL.\n";

         throw std::logic_error(buffer.str().c_str());	  
      }

       #endif

      performance += objective_term_pointer->calculate_evaluation();
   }

   if(regularization_term_flag)
   {
      #ifdef _DEBUG 

      if(!regularization_term_pointer)
      {
         buffer << "OpenNN Exception: PerformanceFunctional class.\n"
                << "double calculate_evaluation(void) const method.\n"
                << "Pointer to regularization functional is NULL.\n";

         throw std::logic_error(buffer.str().c_str());	  
      }

       #endif

      performance += regularization_term_pointer->calculate_evaluation();
   }

   if(constraints_term_flag)
   {
      #ifdef _DEBUG 

      if(!constraints_term_pointer)
      {
         buffer << "OpenNN Exception: PerformanceFunctional class.\n"
                << "double calculate_evaluation(void) const method.\n"
                << "Pointer to constraints functional is NULL.\n";

         throw std::logic_error(buffer.str().c_str());	  
      }

       #endif

      performance += constraints_term_pointer->calculate_evaluation();
   }

   return(performance);
}


// double calculate_evaluation(const Vector<double>&) const method

/// This method returns the performance of a neural network for a given vector of parameters. 
/// It does not set that vector of parameters to the neural network. 
/// @param parameters Vector of parameters for the neural network associated to the performance functional.

double PerformanceFunctional::calculate_evaluation(const Vector<double>& parameters) 
{
   // Control sentence (if debug)

   #ifdef _DEBUG 

   const unsigned int size = parameters.size();

   const unsigned int parameters_number = neural_network_pointer->count_parameters_number();

   if(size != parameters_number)
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: PerformanceFunctional class.\n"
             << "double calculate_evaluation(const Vector<double>&) method.\n"
             << "Size (" << size << ") must be equal to number of parameters (" << parameters_number << ").\n";

      throw std::logic_error(buffer.str().c_str());	  
   }

   #endif

   const Vector<double> original_parameters = neural_network_pointer->arrange_parameters();

   neural_network_pointer->set_parameters(parameters);
   
   const double performance = calculate_evaluation();

   neural_network_pointer->set_parameters(original_parameters);

   return(performance);
}


// double calculate_generalization_evaluation(void) const method

/// This method calculates the performance value of the performance functional, 
/// as the sum of the objective and the regularization terms. 

double PerformanceFunctional::calculate_generalization_evaluation(void) const 
{
   // Control sentence (if debug)

   #ifdef _DEBUG 

   if(!neural_network_pointer)
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: PerformanceFunctional class.\n"
             << "double calculate_generalization_evaluation(void) const method.\n"
             << "Pointer to neural network is NULL.\n";

      throw std::logic_error(buffer.str().c_str());	  
   }

   #endif

   double generalization_peformance = 0.0;

   if(objective_term_flag)
   {
      generalization_peformance += objective_term_pointer->calculate_generalization_evaluation();
   }

   if(regularization_term_flag)
   {
      generalization_peformance += regularization_term_pointer->calculate_generalization_evaluation();
   }

   if(constraints_term_flag)
   {
      generalization_peformance += constraints_term_pointer->calculate_generalization_evaluation();
   }

   return(generalization_peformance);
}


// Vector<double> calculate_gradient(void) const method

/// This method returns the objective function gradient, as the sum of the objective and the regularization gradient vectors. 

Vector<double> PerformanceFunctional::calculate_gradient(void) const
{
   // Control sentence (if debug)

   #ifdef _DEBUG 

   std::ostringstream buffer;

   if(!neural_network_pointer)
   {
      buffer << "OpenNN Exception: PerformanceFunctional class.\n"
             << "Vector<double> calculate_gradient(void) const method.\n"
             << "Pointer to neural network is NULL.\n";

      throw std::logic_error(buffer.str().c_str());	  
   }

   #endif

   const unsigned int& parameters_number = neural_network_pointer->count_parameters_number();

   Vector<double> gradient(parameters_number, 0.0);

   if(objective_term_flag)
   {
      #ifdef _DEBUG 

      if(!objective_term_pointer)
      {
         buffer << "OpenNN Exception: PerformanceFunctional class.\n"
                << "Vector<double> calculate_gradient(void) const method.\n"
                << "Pointer to objective term is NULL.\n";

         throw std::logic_error(buffer.str().c_str());	  
      }

      #endif

      gradient += objective_term_pointer->calculate_gradient();
   }

   if(regularization_term_flag)
   {
      #ifdef _DEBUG 

      if(!regularization_term_pointer)
      {
         buffer << "OpenNN Exception: PerformanceFunctional class.\n"
                << "Vector<double> calculate_gradient(void) const method.\n"
                << "Pointer to regularization term is NULL.\n";

         throw std::logic_error(buffer.str().c_str());	  
      }

      #endif

      gradient += regularization_term_pointer->calculate_gradient();
   }

   if(constraints_term_flag)
   {
      #ifdef _DEBUG 

      if(!constraints_term_pointer)
      {
         buffer << "OpenNN Exception: PerformanceFunctional class.\n"
                << "Vector<double> calculate_gradient(void) const method.\n"
                << "Pointer to constraints term is NULL.\n";

         throw std::logic_error(buffer.str().c_str());	  
      }

      #endif

      gradient += constraints_term_pointer->calculate_gradient();
   }

   return(gradient);
}


// Vector<double> calculate_gradient(const Vector<double>&) method

/// This method returns the performance gradient for a given vector of parameters.
/// It does not set that vector of parameters to the neural network.
/// @param parameters Vector of parameters for the neural network associated to the performance functional.

Vector<double> PerformanceFunctional::calculate_gradient(const Vector<double>& parameters)
{
   #ifdef _DEBUG 

   if(!neural_network_pointer)
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: PerformanceFunctional class.\n"
             << "Vector<double> calculate_gradient(const Vector<double>&) method.\n"
             << "Pointer to neural network is NULL.\n";

      throw std::logic_error(buffer.str().c_str());	  
   }

   #endif

   #ifdef _DEBUG 

   const unsigned int parameters_number = neural_network_pointer->count_parameters_number();

   const unsigned int size = parameters.size();

   if(size != parameters_number)
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: PerformanceFunctional class.\n"
             << "Vector<double> calculate_gradient(const Vector<double>&) method.\n"
             << "Size (" << size << ") must be equal to number of parameters (" << parameters_number << ").\n";

      throw std::logic_error(buffer.str().c_str());	  
   }
   
   #endif
    
   // Calculate potential gradient

   const Vector<double> original_parameters = neural_network_pointer->arrange_parameters();

   // Set potential parameters

   neural_network_pointer->set_parameters(parameters);

   // Calcualate gradient

   const Vector<double> gradient = calculate_gradient();

   // Restart original parameters

   neural_network_pointer->set_parameters(original_parameters);

   return(gradient);
}


// Matrix<double> calculate_Hessian(void) const method

/// This method returns the default objective function Hessian matrix, which is computed using numerical 
/// differentiation.

Matrix<double> PerformanceFunctional::calculate_Hessian(void) const
{
   const unsigned int parameters_number = neural_network_pointer->count_parameters_number();

   Matrix<double> Hessian(parameters_number, parameters_number, 0.0);
   
   if(objective_term_flag)
   {
      Hessian += objective_term_pointer->calculate_Hessian();   
   }

   if(regularization_term_flag)
   {
      Hessian += regularization_term_pointer->calculate_Hessian();
   }

   if(constraints_term_flag)
   {
      Hessian += constraints_term_pointer->calculate_Hessian();
   }

   return(Hessian);
}


// Vector<double> calculate_Hessian(const Vector<double>&) method

/// This method returns which would be the objective function Hessian of a multilayer perceptron for an 
/// hypothetical vector of parameters.
/// It does not set that vector of parameters to the multilayer perceptron.
/// @param parameters Vector of a potential parameters for the multilayer perceptron associated 
/// to the performance functional.

Matrix<double> PerformanceFunctional::calculate_Hessian(const Vector<double>& parameters)
{
   // Control sentence (if debug)

   #ifdef _DEBUG 

   const unsigned int size = parameters.size();
   const unsigned int parameters_number = neural_network_pointer->count_parameters_number();

   if(size != parameters_number)
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: PerformanceFunctional class.\n"
             << "double calculate_Hessian(const Vector<double>&) method.\n"
             << "Size must be equal to number of parameters.\n";

      throw std::logic_error(buffer.str().c_str());	  
   }

   #endif

   // Calculate potential Hessian

   const Vector<double> original_parameters = neural_network_pointer->arrange_parameters();

   // Set potential parameters

   neural_network_pointer->set_parameters(parameters);

   // Get objective function Hessian

   const Matrix<double> Hessian = calculate_Hessian();

   // Restart original parameters

   neural_network_pointer->set_parameters(original_parameters);

   return(Hessian);
}



// Matrix<double> calculate_inverse_Hessian(void) const method

/// This method returns inverse matrix of the Hessian.
/// It first computes the Hessian matrix and then computes its inverse. 
/// @todo

Matrix<double> PerformanceFunctional::calculate_inverse_Hessian(void) const
{  
   std::ostringstream buffer;

   buffer << "OpenNN Exception: PerformanceFunctional class.\n"
          << "Matrix<double> calculate_inverse_Hessian(void) const method.\n"
          << "This method is not yet implemented.\n";

   throw std::logic_error(buffer.str().c_str());	  

//   const Matrix<double> Hessian = calculate_Hessian();
         
//   return(Hessian.calculate_inverse());               
}


// Vector<double> calculate_vector_dot_Hessian(Vector<double>) const method

/// This method returns the default product of some vector with the objective function Hessian matrix, which is 
/// computed using numerical differentiation.
/// @param vector Vector in the dot product. 

Vector<double> PerformanceFunctional::calculate_vector_dot_Hessian(const Vector<double>& vector) const
{
   // Control sentence

   const unsigned int size = vector.size();

   const unsigned int parameters_number = neural_network_pointer->count_parameters_number();

   if(size != parameters_number)
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: PerformanceFunctional class.\n"
             << "Vector<double> calculate_vector_dot_Hessian(Vector<double>) method.\n"
             << "Size of vector must be equal to number of parameters.\n";

      throw std::logic_error(buffer.str().c_str());	  
   }

   // Calculate vector Hessian product

   Vector<double> vector_Hessian_product(parameters_number);

   return(vector_Hessian_product);
}


// ZeroOrderEvaluation calculate_zero_order_evaluation(void) const method

/// This method returns a zero order evaluation structure, which just contains the evaluation value of the performance function. 

PerformanceFunctional::ZeroOrderEvaluation PerformanceFunctional::calculate_zero_order_evaluation(void) const
{
   ZeroOrderEvaluation zero_order_evaluation;

   zero_order_evaluation.performance = calculate_evaluation();

   return(zero_order_evaluation);
}


// FirstOrderEvaluation calculate_first_order_evaluation(void) const method

/// This method returns a first order evaluation structure, which contains the value and the gradient of the performance function. 

PerformanceFunctional::FirstOrderEvaluation PerformanceFunctional::calculate_first_order_evaluation(void) const
{
   FirstOrderEvaluation first_order_evaluation;

   first_order_evaluation.performance = calculate_evaluation();
   first_order_evaluation.gradient = calculate_gradient();

   return(first_order_evaluation);
}


// SecondOrderEvaluation calculate_second_order_evaluation(void) const method

/// This method returns a second order evaluation structure, which contains the value, the gradient and the Hessian of the performance function. 

PerformanceFunctional::SecondOrderEvaluation PerformanceFunctional::calculate_second_order_evaluation(void) const
{
   SecondOrderEvaluation second_order_evaluation;

   second_order_evaluation.performance = calculate_evaluation();
   second_order_evaluation.gradient = calculate_gradient();
   second_order_evaluation.Hessian = calculate_Hessian();

   return(second_order_evaluation);
}


// double calculate_zero_order_Taylor_approximation(const Vector<double>&) const method

/// This method returns the Taylor approximation of the performance function at some point near the parameters. 
/// The order of the approximation here is zero, i.e., only the performance value is used. 

double PerformanceFunctional::calculate_zero_order_Taylor_approximation(const Vector<double>&) const 
{
   return(calculate_evaluation());
}


// double calculate_first_order_Taylor_approximation(const Vector<double>&) const method

/// This method returns the Taylor approximation of the performance function at some point near the parameters. 
/// The order of the approximation here is one, i.e., both the performance value and the performance gradient are used. 
/// @param parameters Approximation point. 

double PerformanceFunctional::calculate_first_order_Taylor_approximation(const Vector<double>& parameters) const
{
   // Control sentence (if debug)

   #ifdef _DEBUG 

   const unsigned int parameters_size = parameters.size();            
   const unsigned int parameters_number = neural_network_pointer->count_parameters_number();            

   if(parameters_size != parameters_number)
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: PerformanceFunctional class.\n"
             << "double calculate_first_order_Taylor_approximation(const Vector<double>&) const method.\n"
             << "Size of potential parameters must be equal to number of parameters.\n";

      throw std::logic_error(buffer.str().c_str());	  
   }

   #endif

   const Vector<double> original_parameters = neural_network_pointer->arrange_parameters();

   const double performance = calculate_evaluation();
   const Vector<double> gradient = calculate_gradient();

   const double first_order_Taylor_approximation = performance + gradient.dot(parameters-parameters);

   return(first_order_Taylor_approximation);
}


// double calculate_second_order_Taylor_approximation(const Vector<double>&) const method

/// This method returns the Taylor approximation of the performance function at some point near the parameters. 
/// The order of the approximation here is two, i.e., the performance value, the performance gradient and the performance Hessian are used. 
/// @param parameters Approximation point. 

double PerformanceFunctional::calculate_second_order_Taylor_approximation(const Vector<double>& parameters) const
{
   // Control sentence (if debug)

   #ifdef _DEBUG 

   const unsigned int parameters_size = parameters.size();            
   const unsigned int parameters_number = neural_network_pointer->count_parameters_number();            

   if(parameters_size != parameters_number)
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: PerformanceFunctional class.\n"
             << "double calculate_second_order_Taylor_approximation(const Vector<double>&) const method.\n"
             << "Size of potential parameters must be equal to number of parameters.\n";

      throw std::logic_error(buffer.str().c_str());	  
   }

   #endif

   // Neural network stuff 

   const Vector<double> original_parameters = neural_network_pointer->arrange_parameters();
   const Vector<double> parameters_difference = parameters - parameters;

   // Performance functioal stuff

   const double performance = calculate_evaluation();
   const Vector<double> gradient = calculate_gradient();
   const Matrix<double> Hessian = calculate_Hessian();
   
   const double second_order_Taylor_approximation = performance 
   + gradient.dot(parameters_difference) 
   + parameters_difference.dot(Hessian).dot(parameters_difference)/2.0;

   return(second_order_Taylor_approximation);
}


// double calculate_directional_performance(const Vector<double>&, double) const method

/// This method returns the value of the performance function at some step along some direction. 
/// @param direction Direction vector.
/// @param rate Step value. 

double PerformanceFunctional::calculate_directional_performance(const Vector<double>& direction, double rate) 
{
   return(calculate_evaluation(neural_network_pointer->arrange_parameters() + direction*rate));
}


// double calculate_directional_performance_derivative(const Vector<double>&, double) const method

/// This method returns the derivative of the performance function at some step along some direction. 
/// @param direction Direction vector.
/// @param rate Step value. 

double PerformanceFunctional::calculate_directional_performance_derivative(const Vector<double>& direction, double rate) 
{
   const Vector<double> gradient = calculate_gradient(neural_network_pointer->arrange_parameters() + direction*rate);

   const Vector<double> normalized_direction = direction/direction.calculate_norm();

   return(gradient.dot(normalized_direction));
}


// double calculate_directional_performance_second_derivative(const Vector<double>&, double) const method

/// This method returns the second derivative of the performance function at some step along some direction. 
/// @param direction Direction vector.
/// @param rate Step value. 

double PerformanceFunctional::calculate_directional_performance_second_derivative(const Vector<double>& direction, double rate) 
{
   const Matrix<double> Hessian = calculate_Hessian(neural_network_pointer->arrange_parameters() + direction*rate);

   const Vector<double> normalized_direction = direction/direction.calculate_norm();

   return(normalized_direction.dot(Hessian).dot(normalized_direction));
}


// TiXmlElement* to_XML(void) const method 

/// This method serializes a default performance functional object into a XML element of the TinyXML library. 
/// See the OpenNN manual for more information about the format of this element. 

TiXmlElement* PerformanceFunctional::to_XML(void) const
{
   std::ostringstream buffer;

   // Performance functional

   TiXmlElement* performance_functional_element = new TiXmlElement("PerformanceFunctional");
   performance_functional_element->SetAttribute("Version", 4);

   // Objective term type

   TiXmlElement* objective_term_type_element = new TiXmlElement("ObjectiveTermType");
   performance_functional_element->LinkEndChild(objective_term_type_element);

   buffer.str("");
   buffer << write_objective_term_type();

   TiXmlText* objective_term_type_text = new TiXmlText(buffer.str().c_str());
   objective_term_type_element->LinkEndChild(objective_term_type_text);

   // Regularization term type

   TiXmlElement* regularization_term_type_element = new TiXmlElement("RegularizationTermType");
   performance_functional_element->LinkEndChild(regularization_term_type_element);

   buffer.str("");
   buffer << write_regularization_term_type();

   TiXmlText* regularization_term_type_text = new TiXmlText(buffer.str().c_str());
   regularization_term_type_element->LinkEndChild(regularization_term_type_text);

   // Constraints term type

   TiXmlElement* constraints_term_type_element = new TiXmlElement("ConstraintsTermType");
   performance_functional_element->LinkEndChild(constraints_term_type_element);

   buffer.str("");
   buffer << write_constraints_term_type();

   TiXmlText* constraints_term_type_text = new TiXmlText(buffer.str().c_str());
   constraints_term_type_element->LinkEndChild(constraints_term_type_text);

   // Objective flag

   TiXmlElement* objective_term_flag_element = new TiXmlElement("ObjectiveFlag");
   performance_functional_element->LinkEndChild(objective_term_flag_element);

   buffer.str("");
   buffer << objective_term_flag;

   TiXmlText* objective_term_flag_text = new TiXmlText(buffer.str().c_str());
   objective_term_flag_element->LinkEndChild(objective_term_flag_text);

   // Regularization flag

   TiXmlElement* regularization_term_flag_element = new TiXmlElement("RegularizationFlag");
   performance_functional_element->LinkEndChild(regularization_term_flag_element);

   buffer.str("");
   buffer << regularization_term_flag;

   TiXmlText* regularization_term_flag_text = new TiXmlText(buffer.str().c_str());
   regularization_term_flag_element->LinkEndChild(regularization_term_flag_text);

   // Constraints flag

   TiXmlElement* constraints_term_flag_element = new TiXmlElement("ConstraintsFlag");
   performance_functional_element->LinkEndChild(constraints_term_flag_element);

   buffer.str("");
   buffer << constraints_term_flag;

   TiXmlText* constraints_term_flag_text = new TiXmlText(buffer.str().c_str());
   constraints_term_flag_element->LinkEndChild(constraints_term_flag_text);

   // Objective term

   if(objective_term_pointer)
   {
      TiXmlElement* objective_term_element = objective_term_pointer->to_XML();

      performance_functional_element->LinkEndChild(objective_term_element);
   }

   // Regularization term

   if(regularization_term_pointer)
   {
      TiXmlElement* regularization_term_element = regularization_term_pointer->to_XML();

      performance_functional_element->LinkEndChild(regularization_term_element);
   }

   // Constraints term

   if(constraints_term_pointer)
   {
      TiXmlElement* constraints_term_element = constraints_term_pointer->to_XML();

      performance_functional_element->LinkEndChild(constraints_term_element);
   }

   // Display

   TiXmlElement* display_element = new TiXmlElement("Display");
   performance_functional_element->LinkEndChild(display_element);

   buffer.str("");
   buffer << display;

   TiXmlText* display_text = new TiXmlText(buffer.str().c_str());
   display_element->LinkEndChild(display_text);

   return(performance_functional_element);
}


// void from_XML(TiXmlElement*) method

/// This method sets the performance functional member data from an XML element. 
/// @param performance_functional_element Pointer to a TinyXML element with the performance functional data. 

void PerformanceFunctional::from_XML(TiXmlElement* performance_functional_element)
{
   if(!performance_functional_element)
   {
      return;
   }

   // Objective term type

   TiXmlElement* objective_term_type_element = performance_functional_element->FirstChildElement("ObjectiveTermType");

   if(objective_term_type_element)
   {
      const std::string new_objective_term_type = objective_term_type_element->GetText();           

      try
      {
         set_objective_term_type(new_objective_term_type);
      }
      catch(std::exception& e)
      {
         std::cout << e.what() << std::endl;		 
      }
   }
    
   // Regularization term type

   TiXmlElement* regularization_term_type_element = performance_functional_element->FirstChildElement("RegularizationTermType");

   if(regularization_term_type_element)
   {
      std::string new_regularization_term_type = regularization_term_type_element->GetText();           

      try
      {
         set_regularization_term_type(new_regularization_term_type);
      }
      catch(std::exception& e)
      {
         std::cout << e.what() << std::endl;		 
      }
   }

   // Constraints term type

   TiXmlElement* constraints_term_type_element = performance_functional_element->FirstChildElement("ConstraintsTermType");

   if(constraints_term_type_element)
   {
      std::string new_constraints_term_type = constraints_term_type_element->GetText();           

      try
      {
         set_constraints_term_type(new_constraints_term_type);
      }
      catch(std::exception& e)
      {
         std::cout << e.what() << std::endl;		 
      }
   }

   // Objective flag

   TiXmlElement* objective_term_flag_element = performance_functional_element->FirstChildElement("ObjectiveFlag");

   if(objective_term_flag_element)
   {
      std::string new_objective_term_flag_string = objective_term_flag_element->GetText();           

      try
      {
         set_objective_term_flag(new_objective_term_flag_string != "0");
      }
      catch(std::exception& e)
      {
         std::cout << e.what() << std::endl;		 
      }
   }

   // Regularization flag

   TiXmlElement* regularization_term_flag_element = performance_functional_element->FirstChildElement("RegularizationFlag");

   if(regularization_term_flag_element)
   {
      std::string new_regularization_term_flag_string = regularization_term_flag_element->GetText();           

      try
      {
         set_regularization_term_flag(new_regularization_term_flag_string != "0");
      }
      catch(std::exception& e)
      {
         std::cout << e.what() << std::endl;		 
      }
   }

   // Constraints flag

   TiXmlElement* constraints_term_flag_element = performance_functional_element->FirstChildElement("ConstraintsFlag");

   if(constraints_term_flag_element)
   {
      std::string new_constraints_term_flag_string = constraints_term_flag_element->GetText();           

      try
      {
         set_constraints_term_flag(new_constraints_term_flag_string != "0");
      }
      catch(std::exception& e)
      {
         std::cout << e.what() << std::endl;		 
      }
   }

   // Objective term

   // Regularization term

   // Constraints term

   // Display warnings

   TiXmlElement* display_element = performance_functional_element->FirstChildElement("Display");

   if(display_element)
   {
      std::string new_display_string = display_element->GetText();           

      try
      {
         set_display(new_display_string != "0");
      }
      catch(std::exception& e)
      {
         std::cout << e.what() << std::endl;		 
      }
   }
}


// void print(void) method

/// This method prints the members of the performance functional object to the screen in XML-type format. 

void PerformanceFunctional::print(void) const
{
}


// void save(const std::string&) const method

/// This method saves to a XML-type file a string representation of the performance functional object. 
/// @param filename Name of XML-type performance functional file. 

void PerformanceFunctional::save(const std::string& filename) const
{
   TiXmlDocument document;

   // Declaration

   TiXmlDeclaration* declaration = new TiXmlDeclaration("1.0", "", "");
   document.LinkEndChild(declaration);

   // Performance functional

   TiXmlElement* performance_functional_element = to_XML();
   document.LinkEndChild(performance_functional_element);

   document.SaveFile(filename.c_str());
}


// void load(const std::string&) method

/// This method loads a default performance functional XML-type file. 
/// @param filename Name of default XML-type performance functional file. 

void PerformanceFunctional::load(const std::string& filename)
{
   std::ostringstream buffer;

   TiXmlDocument document(filename.c_str());

   if(!document.LoadFile())
   {
      buffer << "OpenNN Exception: PerformanceFunctional class.\n"
             << "void load(const std::string&) method.\n"
             << "Cannot load XML file " << filename << ".\n";

      throw std::logic_error(buffer.str());
   }

   // Performance functional element
  
   TiXmlElement* performance_functional_element = document.FirstChildElement();

   if(!performance_functional_element)
   {
      buffer << "OpenNN Exception: PerformanceFunctional class.\n"
             << "void load(const std::string&) method.\n"
             << "File " << filename << " is not a valid performance functional file.\n";

      throw std::logic_error(buffer.str());
   }

   from_XML(performance_functional_element);
}


// std::string write_information(void) method

/// This method returns any useful information about the objective function during training. 
/// By default it is an empty string.

std::string PerformanceFunctional::write_information(void)  
{
   std::ostringstream buffer;
   
   if(objective_term_pointer)
   {
      buffer << objective_term_pointer->write_information();
   }

   if(regularization_term_pointer)
   {
      buffer << regularization_term_pointer->write_information();
   }

   if(constraints_term_pointer)
   {
      buffer << constraints_term_pointer->write_information();
   }

   return(buffer.str());
}



//// TiXmlElement* calculate_performance_XML(void) const  method
//
//TiXmlElement* PerformanceFunctional::calculate_performance_XML(void) const
//{
//   std::ostringstream buffer;
//
//   TiXmlElement* performance_element = new TiXmlElement("Performance");
//
//   return(performance_element);   
//}
//
//
//// TiXmlElement* calculate_generalization_evaluation_XML(void) const  method
//
//TiXmlElement* PerformanceFunctional::calculate_generalization_evaluation_XML(void) const
//{
//   std::ostringstream buffer;
//
//   TiXmlElement* generalization_evaluation_element = new TiXmlElement("GeneralizationPerformance");
//
//   return(generalization_evaluation_element);   
//}
//
//
//// TiXmlElement* calculate_gradient_XML(void) method
//
//TiXmlElement* PerformanceFunctional::calculate_gradient_XML(void) const     
//{
//   TiXmlElement* gradient_element = new TiXmlElement("Gradient");
//
//   return(gradient_element);   
//}

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
