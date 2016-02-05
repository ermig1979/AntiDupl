/****************************************************************************************************************/
/*                                                                                                              */
/*   OpenNN: Open Neural Networks Library                                                                       */
/*   www.opennn.cimne.com                                                                                       */
/*                                                                                                              */
/*   T R A I N I N G   R A T E   A L G O R I T H M   C L A S S                                                  */
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

#include "training_rate_algorithm.h"

// TinyXml includes

#include "../../tinyxml/tinyxml.h"

namespace OpenNN
{

// DEFAULT CONSTRUCTOR

/// Default constructor. 
/// It creates a training rate algorithm object not associated to any performance functional object.  
/// It also initializes the class members to their default values. 

TrainingRateAlgorithm::TrainingRateAlgorithm(void)
 : performance_functional_pointer(NULL)
{ 
   set_default();
}


// GENERAL CONSTRUCTOR

/// General constructor. 
/// It creates a training rate algorithm associated to a performance functional.
/// It also initializes the class members to their default values. 
/// @param new_performance_functional_pointer Pointer to a performance functional object.

TrainingRateAlgorithm::TrainingRateAlgorithm(PerformanceFunctional* new_performance_functional_pointer)
 : performance_functional_pointer(new_performance_functional_pointer)
{
   set_default();
}


// XML CONSTRUCTOR

/// XML constructor. 
/// It creates a training rate algorithm object not associated to any performance functional object.  
/// It also loads the class members from a XML element. 
/// 

TrainingRateAlgorithm::TrainingRateAlgorithm(TiXmlElement* training_rate_algorithm_element)
 : performance_functional_pointer(NULL)
{ 
   from_XML(training_rate_algorithm_element);
}


// DESTRUCTOR 

/// Destructor

TrainingRateAlgorithm::~TrainingRateAlgorithm(void)
{ 
}


// METHODS

// PerformanceFunctional* get_performance_functional_pointer(void) method

/// This method returns a pointer to the performance functional object to which the training algorithm is 
/// associated.

PerformanceFunctional* TrainingRateAlgorithm::get_performance_functional_pointer(void)
{
   return(performance_functional_pointer);
}


// const TrainingRateMethod& get_training_rate_method(void) const method

/// This method returns the training rate method used for training.

const TrainingRateAlgorithm::TrainingRateMethod& TrainingRateAlgorithm::get_training_rate_method(void) const
{
   return(training_rate_method);
}


// std::string write_training_rate_method(void) const method

/// This method returns a string with the name of the training rate method to be used. 

std::string TrainingRateAlgorithm::write_training_rate_method(void) const
{
   switch(training_rate_method)
   {
      case Fixed:
      {
         return("Fixed");
	   }
      break;

      case GoldenSection:
      {
         return("GoldenSection");
	   }
      break;

      case BrentMethod:
      {
         return("BrentMethod");
	   }
      break;

      default:
      {
         std::ostringstream buffer;

         buffer << "OpenNN Exception: TrainingRateAlgorithm class.\n"
                << "std::string get_training_rate_method(void) const method.\n"
                << "Unknown training rate method.\n";
 
         throw std::logic_error(buffer.str().c_str());	      
	   }
      break;
   }
}


// const double& get_first_training_rate(void) const method

/// This method returns the initial training rate value in line minimization.

const double& TrainingRateAlgorithm::get_first_training_rate(void) const
{
   return(first_training_rate);
}


// const double& get_bracketing_factor(void) const method

/// This method returns the increase factor when bracketing a minimum in line minimization. 

const double& TrainingRateAlgorithm::get_bracketing_factor(void) const
{
   return(bracketing_factor);       
}


// const double& get_training_rate_tolerance(void) const method

/// This method returns the tolerance value in line minimization.

const double& TrainingRateAlgorithm::get_training_rate_tolerance(void) const
{
   return(training_rate_tolerance);
}


// const double& get_warning_training_rate(void) const method

/// This method returns the training rate value at wich a warning message is written to the screen during line 
/// minimization.

const double& TrainingRateAlgorithm::get_warning_training_rate(void) const
{
   return(warning_training_rate);
}


// const double& get_error_training_rate(void) const method

/// This method returns the training rate value at wich the line minimization algorithm is assumed to fail when 
/// bracketing a minimum.

const double& TrainingRateAlgorithm::get_error_training_rate(void) const
{
   return(error_training_rate);
}


// const bool& get_display(void) const method

/// This method returns true if messages from this class can be displayed on the screen, or false if messages from
/// this class can't be displayed on the screen.

const bool& TrainingRateAlgorithm::get_display(void) const
{
   return(display);
}


// void set(void) method

/// This method sets the performance functional pointer to NULL.
/// It also sets the rest of members to their default values. 

void TrainingRateAlgorithm::set(void)
{
   performance_functional_pointer = NULL;
   set_default();
}


// void set(PerformanceFunctional*) method

/// This method sets a new performance functional pointer.
/// It also sets the rest of members to their default values. 
/// @param new_performance_functional_pointer Pointer to a performance functional object. 

void TrainingRateAlgorithm::set(PerformanceFunctional* new_performance_functional_pointer)
{
   performance_functional_pointer = new_performance_functional_pointer;
   set_default();
}


// void set_default(void) method 

/// This method sets the members of the training rate algorithm to their default values.

void TrainingRateAlgorithm::set_default(void)
{
   // TRAINING OPERATORS

   training_rate_method = BrentMethod;

   // TRAINING PARAMETERS

   bracketing_factor = 1.5;

   first_training_rate = 1.0e-2;
   training_rate_tolerance = 1.0e-6;

   warning_training_rate = 1.0e6;

   error_training_rate = 1.0e9;

   // UTILITIES

   display = true;
}


// void set_performance_functional_pointer(PerformanceFunctional*) method

/// This method sets a pointer to a performance functional object to be associated to the training algorithm.
/// @param new_performance_functional_pointer Pointer to a performance functional object.

void TrainingRateAlgorithm::set_performance_functional_pointer(PerformanceFunctional* new_performance_functional_pointer)
{
   performance_functional_pointer = new_performance_functional_pointer;
}


// void set_training_rate_method(const TrainingRateMethod&) method

/// This method sets a new training rate method to be used for training.
/// @param new_training_rate_method Training rate method.

void TrainingRateAlgorithm::set_training_rate_method(const TrainingRateAlgorithm::TrainingRateMethod& new_training_rate_method)
{
   training_rate_method = new_training_rate_method;
}


// void set_training_rate_method(const std::string&) method

/// This method sets the method for obtaining the training rate from a string with the name of the method. 
/// @param new_training_rate_method Name of training rate method ("Fixed", "GoldenSection", "BrentMethod" or "UserTrainingRate"). 

void TrainingRateAlgorithm::set_training_rate_method(const std::string& new_training_rate_method)
{
   if(new_training_rate_method == "Fixed")
   {
      training_rate_method = Fixed;
   }
   else if(new_training_rate_method == "GoldenSection")
   {
      training_rate_method = GoldenSection;
   }
   else if(new_training_rate_method == "BrentMethod")
   {
      training_rate_method = BrentMethod;
   }
   else
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: TrainingRateAlgorithm class.\n"
             << "void set_method(const std::string&) method.\n"
			 << "Unknown training rate method: " << new_training_rate_method << ".\n";
   
      throw std::logic_error(buffer.str().c_str());	  
   }
}


// void set_first_training_rate(const double&) method

/// This method sets a new value to be used as an initial training rate in line  minimization.
/// @param new_first_training_rate Initial training rate value.

void TrainingRateAlgorithm::set_first_training_rate(const double& new_first_training_rate)
{
   // Control sentence (if debug)

   #ifdef _DEBUG 
     
   if(new_first_training_rate < 0.0)
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: TrainingRateAlgorithm class.\n"
             << "void set_first_training_rate(const double&) method.\n"
             << "First training rate must be equal or greater than 0.\n";

      throw std::logic_error(buffer.str().c_str());	  
   }

   #endif

   // Set first training rate

   first_training_rate = new_first_training_rate;
}


// void set_bracketing_factor(const double&) method

/// This method sets a new increase factor value to be used for line minimization when bracketing a minimum.
/// @param new_bracketing_factor Bracketing factor value.

void TrainingRateAlgorithm::set_bracketing_factor(const double& new_bracketing_factor)
{ 
   // Control sentence (if debug)

   #ifdef _DEBUG 

   if(new_bracketing_factor < 0.0) 
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: TrainingRateAlgorithm class.\n"
             << "void set_bracketing_factor(const double&) method.\n"
             << "Bracketing factor must be equal or greater than 0.\n";

      throw std::logic_error(buffer.str().c_str());	  
   }

   #endif

   bracketing_factor = new_bracketing_factor;
}


// void set_training_rate_tolerance(const double&) method

/// This method sets a new tolerance value to be used in line minimization.
/// @param new_training_rate_tolerance Tolerance value in line minimization.

void TrainingRateAlgorithm::set_training_rate_tolerance(const double& new_training_rate_tolerance)
{
   // Control sentence (if debug)

   #ifdef _DEBUG 
                                      
   if(new_training_rate_tolerance < 0.0)
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: TrainingRateAlgorithm class.\n"
             << "void set_training_rate_tolerance(const double&) method.\n"
             << "Tolerance must be equal or greater than 0.\n";

      throw std::logic_error(buffer.str().c_str());	  
   }

   #endif

   // Set training rate tolerance

   training_rate_tolerance = new_training_rate_tolerance;
}


// void set_warning_training_rate(const double&) method

/// This method sets a new training rate value at wich a warning message is written to the screen during line 
/// minimization.
/// @param new_warning_training_rate Warning training rate value.

void TrainingRateAlgorithm::set_warning_training_rate(const double& new_warning_training_rate)
{
   // Control sentence (if debug)

   #ifdef _DEBUG 

   if(new_warning_training_rate < 0.0)
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: TrainingRateAlgorithm class.\n" 
             << "void set_warning_training_rate(const double&) method.\n"
             << "Warning training rate must be equal or greater than 0.\n";

      throw std::logic_error(buffer.str().c_str());	  
   }

   #endif

   warning_training_rate = new_warning_training_rate;
}


// void set_error_training_rate(const double&) method

/// This method sets a new training rate value at wich a the line minimization algorithm is assumed to fail when 
/// bracketing a minimum.
/// @param new_error_training_rate Error training rate value.

void TrainingRateAlgorithm::set_error_training_rate(const double& new_error_training_rate)
{
   // Control sentence (if debug)

   #ifdef _DEBUG 

   if(new_error_training_rate < 0.0)
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: TrainingRateAlgorithm class.\n"
             << "void set_error_training_rate(const double&) method.\n"
             << "Error training rate must be equal or greater than 0.\n";

      throw std::logic_error(buffer.str().c_str());	  
   }

   #endif

   // Set error training rate

   error_training_rate = new_error_training_rate;
}


// void set_display(const bool&) method

/// This method sets a new display value. 
/// If it is set to true messages from this class are to be displayed on the screen;
/// if it is set to false messages from this class are not to be displayed on the screen.
/// @param new_display Display value.

void TrainingRateAlgorithm::set_display(const bool& new_display)
{
   display = new_display;
}


// Vector<double> calculate_directional_point(const double&, const Vector<double>&, const double&) const method

/// This method returns a vector with two elements, the training rate calculated by means of the training rate
/// object member, and the performance for that training rate. 
/// @param performance Initial objective function performance.
/// @param training_direction Initial training direction.
/// @param initial_training_rate Initial training rate to start the algorithm. 

Vector<double> TrainingRateAlgorithm::calculate_directional_point(const double& performance, const Vector<double>& training_direction, const double& initial_training_rate) const 
{
   #ifdef _DEBUG 

   if(performance_functional_pointer == NULL)
   {
      std::ostringstream buffer;

      buffer << "OpenNN Error: TrainingRateAlgorithm class.\n"
             << "Vector<double> calculate_directional_point(const double&, const Vector<double>&, const double&) const method.\n"
             << "Pointer to performance functional is NULL.\n";

      throw std::logic_error(buffer.str().c_str());
   }

   #endif

   #ifdef _DEBUG 

   NeuralNetwork* neural_network_pointer = performance_functional_pointer->get_neural_network_pointer();

   if(neural_network_pointer == NULL)
   {
      std::ostringstream buffer;

      buffer << "OpenNN Error: TrainingRateAlgorithm class.\n"
             << "Vector<double> calculate_directional_point(const double&, const Vector<double>&, const double&) const method.\n"
             << "Pointer to neural network is NULL.\n";

      throw std::logic_error(buffer.str().c_str());
   }

   #endif
   

   switch(training_rate_method)
   {
      case TrainingRateAlgorithm::Fixed:
      {
         return(calculate_fixed_directional_point(performance, training_direction, initial_training_rate));
      }
      break;

      case TrainingRateAlgorithm::GoldenSection:
      {
         return(calculate_golden_section_directional_point(performance, training_direction, initial_training_rate));
      }
      break;

      case TrainingRateAlgorithm::BrentMethod:
      {
         return(calculate_Brent_method_directional_point(performance, training_direction, initial_training_rate));
      }
      break;

	  default:
	  {
         std::ostringstream buffer;

         buffer << "OpenNN Exception: TrainingRateAlgorithm class\n"
                << "Vector<double> calculate_directional_point(double, const Vector<double>&, double) const method.\n" 
                << "Unknown training rate method.\n";

         throw std::logic_error(buffer.str().c_str());	     
	   }
   }
}


// Vector<double> calculate_bracketing_training_rate(const double&, const Vector<double>&, const double&) const method

/// This method returns a vector with two elements, a bracketing training rate for the optimum, 
/// and the performance for that training rate. 
/// This algorithm is used by line minimization algorithms. 
/// @param performance Initial performance function performance.
/// @param training_direction Initial training direction.
/// @param initial_training_rate Initial training rate to start the algorithm. 

Vector< Vector<double> > TrainingRateAlgorithm::calculate_bracketing_training_rate(const double& performance, const Vector<double>& training_direction, const double& initial_training_rate) const
{
   // Interior point

   Vector<double> A(2);
   A[0] = 0;
   A[1] = performance;

   Vector<double> U(2);
   U[0] = initial_training_rate;
   U[1] = performance_functional_pointer->calculate_directional_performance(training_direction, U[0]);      

   Vector<double> B = U;

   while(A[1] <= U[1])
   {
      B = U;

      U[0] /= bracketing_factor;
      U[1] = performance_functional_pointer->calculate_directional_performance(training_direction, U[0]);      

	  if(U[0] < training_rate_tolerance)
	  {
         //std::ostringstream buffer;

         //buffer << "OpenNN Warning: TrainingRateAlgorithm class.\n"
         //       << "Vector<double> calculate_bracketing_training_rate(double, const Vector<double>&, double) const method\n." 
         //       << "Interior point is " << U[0] << std::endl;	   

         //throw std::runtime_error(buffer.str());   

         Vector< Vector<double> > bracketing_training_rate(3, A);

		 return(bracketing_training_rate);
	  }
   }

   while(U[1] >= B[1])
   {
      B[0] *= bracketing_factor;
      B[1] = performance_functional_pointer->calculate_directional_performance(training_direction, B[0]);      


	  if(B[0] > error_training_rate)
	  {
         std::ostringstream buffer;

         buffer << "OpenNN Warning: TrainingRateAlgorithm class.\n"
                << "Vector<double> calculate_bracketing_training_rate(double, const Vector<double>&, double) const method\n." 
                << "Right point is " << B[0] << "\n";	   
 
         throw std::logic_error(buffer.str().c_str());	     
	  }
   }

   if((A[0] >= U[0] || U[0] >= B[0]) || (A[1] <= U[1] || U[1] >= B[1]))
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: TrainingRateAlgorithm class.\n"
             << "Vector<double> calculate_bracketing_training_rate(double, const Vector<double>&, double) const method\n." 
             << "Uncorrect triplet:\n"
             << "A = (" << A[0] << "," << A[1] << ")\n"
             << "U = (" << U[0] << "," << U[1] << ")\n"
             << "B = (" << B[0] << "," << B[1] << ")\n";
            
      throw std::logic_error(buffer.str());      
   }

   // Return bracket

   Vector< Vector<double> > bracketing_training_rate(3);
   bracketing_training_rate[0] = A;
   bracketing_training_rate[1] = U;
   bracketing_training_rate[2] = B;
   
   return(bracketing_training_rate);
}


// Vector<double> calculate_fixed_directional_point(const double&, const Vector<double>&, const double&) const method

/// This method returns a vector with two elements, a fixed training rate equal to the first training rate object member, 
/// and the performance for that training rate. 
/// @param training_direction Initial training direction.

Vector<double> TrainingRateAlgorithm::calculate_fixed_directional_point(const double&, const Vector<double>& training_direction, const double&) const 
{
   Vector<double> directional_point(2);

   directional_point[0] = first_training_rate;
   directional_point[1] = performance_functional_pointer->calculate_directional_performance(training_direction, first_training_rate);

   return(directional_point);
}


// Vector<double> calculate_golden_section_directional_point(double, Vector<double>, double) const method

/// This method returns the training rate by searching in a given direction to locate the minimum of the objective 
/// function in that direction. It uses the golden section method.
/// @param performance Neural multilayer_perceptron_pointer's performance value.
/// @param training_direction Training direction vector.
/// @param initial_training_rate Initial training rate in line minimization.

Vector<double> TrainingRateAlgorithm::calculate_golden_section_directional_point
(const double& performance, const Vector<double>& training_direction, const double& initial_training_rate) const
{
   std::ostringstream buffer;

   // Bracket minimum

   try
   {
      Vector< Vector<double> > bracketing_training_rate = calculate_bracketing_training_rate(performance, training_direction, initial_training_rate);

      Vector<double> A = bracketing_training_rate[0];
      Vector<double> U = bracketing_training_rate[1];
      Vector<double> B = bracketing_training_rate[2];

	  if(A == B)
	  {
	     return(A);
	  }

      Vector<double> V(2);

      // Reduce the interval

      do
      {
         V[0] = calculate_golden_section_training_rate(A, U, B);	  
         V[1] = performance_functional_pointer->calculate_directional_performance(training_direction, V[0]);

         // Update points
 
         if(V[0] < U[0] && V[1] >= U[1])
	      {
            A = V;
            //B = B;
            //U = U;
	      }
         else if(V[0] < U[0] && V[1] <= U[1])
         {
            //A = A;
            B = U;
            U = V;
	      }
	      else if(V[0] > U[0] && V[1] >= U[1])
	      {
            //A = A;
            B = V;
            //U = U;
         }
         else if(V[0] > U[0] && V[1] <= U[1])
         {
            A = U;
            //B = B;
            U = V;
         }
		 else if(V[0] == U[0])
		 {
            buffer << "OpenNN Exception: TrainingRateAlgorithm class.\n"
                   << "Vector<double> calculate_golden_section_directional_point(double, const Vector<double>, double) const method.\n"
                   << "Both interior points have the same ordinate.\n";

            std::cout << buffer.str() << std::endl;

	        break;
		 }
	     else
	     {
            buffer << "OpenNN Exception: TrainingRateAlgorithm class.\n" 
                   << "Vector<double> calculate_golden_section_directional_point(double, const Vector<double>, double) const method.\n" 
                   << "Unknown set:\n" 
		             << "A = (" << A[0] << "," << A[1] << ")\n"
		             << "B = (" << B[0] << "," << B[1] << ")\n"
		             << "U = (" << U[0] << "," << U[1] << ")\n"
		             << "V = (" << V[0] << "," << V[1] << ")\n";
         
	        throw std::logic_error(buffer.str());
	     }

		 // Check triplet

	     if(A[1] < U[1] || U[1] > B[1])
	     {
            buffer << "OpenNN Exception: TrainingRateAlgorithm class.\n"
                   << "Vector<double> calculate_golden_section_directional_point(double, const Vector<double>, double) const method.\n"
                   << "Triplet does not satisfy minimum condition:\n"
		           << "A = (" << A[0] << "," << A[1] << ")\n"
		           << "B = (" << B[0] << "," << B[1] << ")\n"
		           << "U = (" << U[0] << "," << U[1] << ")\n";

	        throw std::logic_error(buffer.str());
	     }

      }while(B[0] - A[0] > training_rate_tolerance);

	  return(U);
   }
   catch(std::range_error& e) // Interval is of length 0
   {  
      std::cerr << e.what() << std::endl;

	  Vector<double> A(2);
	  A[0] = 0.0;
      A[1] = performance;

      return(A);
   }
   catch(std::logic_error& e) 
   {  
      std::cerr << e.what() << std::endl;

	  Vector<double> X(2);
	  X[0] = first_training_rate;
      X[1] = performance_functional_pointer->calculate_directional_performance(training_direction, X[0]);

	   if(X[1] > performance)
	   {
	      X[0] = 0.0;
	      X[1] = 0.0;
	   }

      return(X);
   }
}


// Vector<double> calculate_Brent_method_directional_point(const double&, const Vector<double>, const double&) const method

/// This method returns the training rate by searching in a given direction to locate the minimum of the performance 
/// function in that direction. It uses the Brent's method.
/// @param performance Neural multilayer_perceptron_pointer's performance value.
/// @param training_direction Training direction vector.
/// @param initial_training_rate Initial training rate in line minimization.

Vector<double> TrainingRateAlgorithm::calculate_Brent_method_directional_point
(const double& performance, const Vector<double>& training_direction, const double& initial_training_rate) const 
{        
   std::ostringstream buffer;

   // Bracket minimum

   try
   {
      Vector< Vector<double> > bracketing_training_rate = calculate_bracketing_training_rate(performance, training_direction, initial_training_rate);

      Vector<double> A = bracketing_training_rate[0];
      Vector<double> U = bracketing_training_rate[1];
      Vector<double> B = bracketing_training_rate[2];

	  if(A == B)
	  {
	     return(A);
	  }

      Vector<double> V(2);

      // Reduce the interval

      while(B[0] - A[0] > training_rate_tolerance)
      {
          try
	      {
            V[0] = calculate_Brent_method_training_rate(A, U, B);	  
	      }
         catch(std::logic_error&)
	      {
            V[0] = calculate_golden_section_training_rate(A, U, B);	  
	      }

         // Calculate performance for V

         V[1] = performance_functional_pointer->calculate_directional_performance(training_direction, V[0]);

         // Update points
 
         if(V[0] < U[0] && V[1] >= U[1])
	      {
            A = V;
            //B = B;
            //U = U;
	      }
         else if(V[0] < U[0] && V[1] <= U[1])
         {
            //A = A;
            B = U;
            U = V;
	     }
	     else if(V[0] > U[0] && V[1] >= U[1])
	     {
            //A = A;
            B = V;
            //U = U;
         }
         else if(V[0] > U[0] && V[1] <= U[1])
         {
            A = U;
            //B = B;
            U = V;
         }
		   else if(V[0] == U[0])
		   {
            buffer << "OpenNN Exception: TrainingRateAlgorithm class.\n"
                   << "Vector<double> calculate_Brent_method_directional_point(double, const Vector<double>, double) const method.\n"
                   << "Both interior points have the same ordinate.\n";

	        break;
		  }
	     else
	     {
            buffer << "OpenNN Exception: TrainingRateAlgorithm class.\n" 
                   << "Vector<double> calculate_Brent_method_directional_point(double, const Vector<double>, double) const method.\n" 
                   << "Unknown set:\n" 
		             << "A = (" << A[0] << "," << A[1] << ")\n"
		             << "B = (" << B[0] << "," << B[1] << ")\n"
		             << "U = (" << U[0] << "," << U[1] << ")\n"
		             << "V = (" << V[0] << "," << V[1] << ")\n";
         
	        throw std::logic_error(buffer.str());
	     }

		 // Check triplet

	     if(A[1] < U[1] || U[1] > B[1])
	     {
            std::ostringstream buffer;

            buffer << "OpenNN Exception: TrainingRateAlgorithm class.\n"
                   << "Vector<double> calculate_Brent_method_directional_point(double, const Vector<double>, double) const method.\n"
                   << "Triplet does not satisfy minimum condition:\n"
		           << "A = (" << A[0] << "," << A[1] << ")\n"
		           << "B = (" << B[0] << "," << B[1] << ")\n"
		           << "U = (" << U[0] << "," << U[1] << ")\n";

	        throw std::logic_error(buffer.str());
	     }
      }

	  return(U);
   }
   catch(std::range_error& e) // Interval is of length 0
   {  
      std::cerr << e.what() << std::endl;

      Vector<double> A(2);
	   A[0] = 0.0;
      A[1] = performance;

      return(A);
   }
   catch(std::logic_error& e)
   {  
      std::cerr << e.what() << std::endl;

	  Vector<double> X(2);
	  X[0] = first_training_rate;
      X[1] = performance_functional_pointer->calculate_directional_performance(training_direction, X[0]);      

      if(X[1] > performance)
	  {
	     X[0] = 0.0;
	     X[1] = 0.0;
	  }

      return(X);
   }
}


// double calculate_golden_section_training_rate(const Vector<double>&, const Vector<double>&, const Vector<double>&) const method

/// This method calculates the golden section point within a minimum interval defined by three points. 
/// @param A Left point of the interval. 
/// @param U Interior point of the interval. 
/// @param B Right point of the interval. 

double TrainingRateAlgorithm::calculate_golden_section_training_rate(const Vector<double>& A, const Vector<double>& U, const Vector<double>& B) const
{
//   const double tau = 0.382; // (3.0-sqrt(5.0))/2.0   

   if(U[0] < A[0] + 0.5*(B[0] - A[0]))
   {
      return(A[0] + 0.618*(B[0] - A[0]));   
   }
   else
   {
      return(A[0] + 0.382*(B[0] - A[0]));   
   }
}


// double calculate_Brent_method_training_rate(const Vector<double>&, const Vector<double>&, const Vector<double>&) const method

/// This method returns the minimimal training rate of a parabola defined by three directional points. 
/// @param A Left point of the interval. 
/// @param B Interior point of the interval. 
/// @param C Right point of the interval. 

double TrainingRateAlgorithm::calculate_Brent_method_training_rate(const Vector<double>& A, const Vector<double>& B, const Vector<double>& C) const
{
   std::ostringstream buffer;

   const double c = -(A[1]*(B[0]-C[0]) + B[1]*(C[0]-A[0]) + C[1]*(A[0]-B[0]))/((A[0]-B[0])*(B[0]-C[0])*(C[0]-A[0]));

   if(c == 0) 
   {
      buffer << "OpenNN Exception: TrainingRateAlgorithm class.\n"
             << "double calculate_Brent_method_training_rate(Vector<double>&, Vector<double>&, Vector<double>&) const method.\n"
             << "Parabola cannot be constructed.\n";

      throw std::logic_error(buffer.str());
   }
   else if(c < 0) 
   {
      buffer << "OpenNN Exception: TrainingRateAlgorithm class.\n"
             << "double calculate_Brent_method_training_rate(Vector<double>&, Vector<double>&, Vector<double>&) const method.\n"
             << "Parabola does not have a minimum but a maximum.\n";

      throw std::logic_error(buffer.str());
   }

   const double b = (A[1]*(B[0]*B[0]-C[0]*C[0]) + B[1]*(C[0]*C[0]-A[0]*A[0]) + C[1]*(A[0]*A[0]-B[0]*B[0]))/((A[0]-B[0])*(B[0]-C[0])*(C[0]-A[0]));

   const double Brent_method_training_rate = -b/(2.0*c);

   if(Brent_method_training_rate <= A[0] || Brent_method_training_rate >= C[0])
   {
      buffer << "OpenNN Exception: TrainingRateAlgorithm class.\n"
             << "double calculate_parabola_minimal_training_rate(Vector<double>&, Vector<double>&, Vector<double>&) const method.\n"
             << "Brent method training rate is not inside interval.\n"
	         << "Interval: (" << A[0] << "," << B[0] << ")\n"
	         << "Brent method training rate: " << Brent_method_training_rate << std::endl;

      throw std::logic_error(buffer.str());
   }

   return(Brent_method_training_rate);
}


// TiXmlElement* to_XML(void) const method

/// This method returns a default string representation in XML-type format of the training algorithm object.
/// This containts the training operators, the training parameters, stopping criteria and other stuff.

TiXmlElement* TrainingRateAlgorithm::to_XML(void) const
{
   std::ostringstream buffer;

   // Training algorithm

   TiXmlElement* training_rate_algorithm_element = new TiXmlElement("TrainingRateAlgorithm");
   training_rate_algorithm_element->SetAttribute("Version", 4); 

   // Training rate method

   TiXmlElement* training_rate_method_element = new TiXmlElement("TrainingRateMethod");
   training_rate_algorithm_element->LinkEndChild(training_rate_method_element);

   TiXmlText* training_rate_method_text = new TiXmlText(write_training_rate_method().c_str());
   training_rate_method_element->LinkEndChild(training_rate_method_text);

   // Bracketing factor

   TiXmlElement* bracketing_factor_element = new TiXmlElement("BracketingFactor");
   training_rate_algorithm_element->LinkEndChild(bracketing_factor_element);

   buffer.str("");
   buffer << bracketing_factor;

   TiXmlText* bracketing_factor_text = new TiXmlText(buffer.str().c_str());
   bracketing_factor_element->LinkEndChild(bracketing_factor_text);

   // First training rate

   TiXmlElement* first_training_rate_element = new TiXmlElement("FirstTrainingRate");
   training_rate_algorithm_element->LinkEndChild(first_training_rate_element);

   buffer.str("");
   buffer << first_training_rate;

   TiXmlText* first_training_rate_text = new TiXmlText(buffer.str().c_str());
   first_training_rate_element->LinkEndChild(first_training_rate_text);

   // Training rate tolerance 

   TiXmlElement* training_rate_tolerance_element = new TiXmlElement("TrainingRateTolerance");
   training_rate_algorithm_element->LinkEndChild(training_rate_tolerance_element);

   buffer.str("");
   buffer << training_rate_tolerance;

   TiXmlText* training_rate_tolerance_text = new TiXmlText(buffer.str().c_str());
   training_rate_tolerance_element->LinkEndChild(training_rate_tolerance_text);

   // Warning training rate 

   TiXmlElement* warning_training_rate_element = new TiXmlElement("WarningTrainingRate");
   training_rate_algorithm_element->LinkEndChild(warning_training_rate_element);

   buffer.str("");
   buffer << warning_training_rate;

   TiXmlText* warning_training_rate_text = new TiXmlText(buffer.str().c_str());
   warning_training_rate_element->LinkEndChild(warning_training_rate_text);

   // Error training rate

   TiXmlElement* error_training_rate_element = new TiXmlElement("ErrorTrainingRate");
   training_rate_algorithm_element->LinkEndChild(error_training_rate_element);

   buffer.str("");
   buffer << error_training_rate;

   TiXmlText* error_training_rate_text = new TiXmlText(buffer.str().c_str());
   error_training_rate_element->LinkEndChild(error_training_rate_text);

   // Display warnings

   TiXmlElement* display_element = new TiXmlElement("Display");
   training_rate_algorithm_element->LinkEndChild(display_element);

   buffer.str("");
   buffer << display;

   TiXmlText* display_text = new TiXmlText(buffer.str().c_str());
   display_element->LinkEndChild(display_text);

   return(training_rate_algorithm_element);
}


// virtual void from_XML(TiXmlElement*) method

/// This method loads a gradient descent object from a XML-type file. 
/// Please mind about the file format, wich is specified in the User's Guide. 
/// @param training_rate_algorithm_element Tiny XML element with the training rate algorithm members. 

void TrainingRateAlgorithm::from_XML(TiXmlElement* training_rate_algorithm_element)
{
   // Training rate method

   TiXmlElement* training_rate_method_element = training_rate_algorithm_element->FirstChildElement("TrainingRateMethod");

   if(training_rate_method_element)
   {
      std::string new_training_rate_method = training_rate_method_element->GetText(); 

      try
      {
         set_training_rate_method(new_training_rate_method);
      }
      catch(std::exception& e)
      {
         std::cout << e.what() << std::endl;		 
      }
   }

   // Bracketing factor

   TiXmlElement* bracketing_factor_element = training_rate_algorithm_element->FirstChildElement("BracketingFactor");

   if(bracketing_factor_element)
   {
      double new_bracketing_factor = atof(bracketing_factor_element->GetText()); 

      try
      {
         set_bracketing_factor(new_bracketing_factor);
      }
      catch(std::exception& e)
      {
         std::cout << e.what() << std::endl;		 
      }
   }

   // First training rate

   TiXmlElement* first_training_rate_element = training_rate_algorithm_element->FirstChildElement("FirstTrainingRate");

   if(first_training_rate_element)
   {
      double new_first_training_rate = atof(first_training_rate_element->GetText()); 

      try
      {
         set_first_training_rate(new_first_training_rate);
      }
      catch(std::exception& e)
      {
         std::cout << e.what() << std::endl;		 
      }
   }

   // Training rate tolerance 

   TiXmlElement* training_rate_tolerance_element = training_rate_algorithm_element->FirstChildElement("TrainingRateTolerance");

   if(training_rate_tolerance_element)
   {
      double new_training_rate_tolerance = atof(training_rate_tolerance_element->GetText()); 

      try
      {
         set_training_rate_tolerance(new_training_rate_tolerance);
      }
      catch(std::exception& e)
      {
         std::cout << e.what() << std::endl;		 
      }
   }

   // Warning training rate 

   TiXmlElement* warning_training_rate_element = training_rate_algorithm_element->FirstChildElement("WarningTrainingRate");

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

   // Error training rate

   TiXmlElement* error_training_rate_element = training_rate_algorithm_element->FirstChildElement("ErrorTrainingRate");

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

   // Display warnings

   TiXmlElement* display_element = training_rate_algorithm_element->FirstChildElement("Display");

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
