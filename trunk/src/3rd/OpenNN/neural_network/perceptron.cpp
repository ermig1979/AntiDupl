/****************************************************************************************************************/
/*                                                                                                              */
/*   OpenNN: Open Neural Networks Library                                                                       */
/*   www.opennn.cimne.com                                                                                       */
/*                                                                                                              */
/*   P E R C E P T R O N   C L A S S                                                                            */
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
#include <sstream>
#include <cstdlib>
#include <ctime>

// OpenNN includes

#include "perceptron.h"
#include "../utilities/vector.h"
#include "../utilities/matrix.h"

namespace OpenNN
{

/// Default constructor. 
/// It creates a perceptron object with zero inputs.
/// The neuron's bias is initialized to zero. 
/// This constructor also initializes the rest of class members to their default values.
	
Perceptron::Perceptron(void)
{
   set();
}


/// Inputs number constructor. 
/// It creates a perceptron object with a given number of inputs. 
/// The neuron's free paramameters (bias and synaptic weights) are initialized at random with a normal 
/// distribution of mean 0 and standard deviation 1.
/// This constructor also initializes the rest of class members to their default values.
/// @param new_inputs_number Number of inputs in the neuron.

Perceptron::Perceptron(const unsigned int& new_inputs_number)
{
   set(new_inputs_number);
}


/// Inputs number and initialization constructor. 
/// This creates a perceptron with a given number of inputs.
/// It also initializes the bias and synaptic weights with a given value. 
/// @param new_inputs_number Number of inputs in the neuron.
/// @param new_parameters_value Bias and synaptic weights initialization value. 

Perceptron::Perceptron(const unsigned int& new_inputs_number, const double& new_parameters_value)
{
   set(new_inputs_number, new_parameters_value);
}


/// Copy constructor. 
/// It creates a percdeptron object by copying its members with those for other perceptron object. 
/// @param other_perceptron Perceptron object to be copied.

Perceptron::Perceptron(const Perceptron& other_perceptron)
{
   set(other_perceptron);
}


/// Destructor.
/// This destructor does not delete any pointer.

Perceptron::~Perceptron(void)
{
}


// ASSIGNMENT OPERATOR

/// Assignment operator. 
/// It assigns to the current perceptron the members of an existing perceptron.
/// @param other_perceptron Assigning perceptron object .

Perceptron& Perceptron::operator=(const Perceptron& other_perceptron)
{
   if(this != &other_perceptron) 
   {
      bias = other_perceptron.bias;
   
      synaptic_weights = other_perceptron.synaptic_weights;

      activation_function = other_perceptron.activation_function;

      display = other_perceptron.display;
   }

   return(*this);
}


// EQUAL TO OPERATOR

// bool operator == (const Perceptron&) const method

/// Equal to operator. 
/// It compares this object with another object of the same class. 
/// It returns true if the members of the two objects have the same values, and false otherwise.
/// @ param other_perceptron Perceptron to be compared with.

bool Perceptron::operator == (const Perceptron& other_perceptron) const
{
   if(bias == other_perceptron.bias
   && synaptic_weights == other_perceptron.synaptic_weights
   && activation_function == other_perceptron.activation_function
   && display == other_perceptron.display)
   {
      return(true);
   }
   else
   {
      return(false);
   }
}


// METHODS

// const ActivationFunction& get_activation_function(void) const method

/// This method returns the activation function of the neuron. 

const Perceptron::ActivationFunction& Perceptron::get_activation_function(void) const 
{
   return(activation_function);                           
}


// std::string write_activation_function(void) const method

/// This method returns a string with the name of the activation function of the neuron. 

std::string Perceptron::write_activation_function(void) const
{
   switch(activation_function)
   {
      case Perceptron::Logistic:   
      {
         return("Logistic");
      }
      break;

      case Perceptron::HyperbolicTangent:   
      {
         return("HyperbolicTangent");
      }
      break;

      case Perceptron::Threshold:   
      {
         return("Threshold");
      }
      break;

      case Perceptron::SymmetricThreshold:   
      {
         return("SymmetricThreshold");
      }
      break;

      case Perceptron::Linear:   
      {
         return("Linear");
      }
      break;

      default:
      {
         std::ostringstream buffer;

         buffer << "OpenNN Exception: Perceptron class.\n" 
                << "std::string get_activation_function(void) const method.\n"
                << "Unknown activation function.\n";

         throw std::logic_error(buffer.str());
      }
      break;
   }
}


// unsigned int count_inputs_number(void) const method

/// This method returns the number of inputs to the neuron. 

unsigned int Perceptron::count_inputs_number(void) const
{
   return(synaptic_weights.size());
}    


// double get_bias(void) const method

/// This method returns the bias value of the neuron.

const double& Perceptron::get_bias(void) const
{
   return(bias);
}


// Vector<double>& arrange_synaptic_weights(void)

/// This method returns the synaptic weight values of the neuron.

const Vector<double>& Perceptron::arrange_synaptic_weights(void) const
{
   return(synaptic_weights);
}


// double get_synaptic_weight(const unsigned int&) const method

/// This method returns the synaptic weight value with index i of the neuron.
/// @param synaptic_weight_index Synaptic weight index.
 
const double& Perceptron::get_synaptic_weight(const unsigned int& synaptic_weight_index) const
{
   // Control sentence (if debug)

   #ifdef _DEBUG 
      
   const unsigned int inputs_number = count_inputs_number();

   if(synaptic_weight_index >= inputs_number)
   {
      std::ostringstream buffer;

	  buffer << "OpenNN Exception: Perceptron class.\n"
                << "double get_synaptic_weight(const unsigned int&) const method.\n"
                << "Index of synaptic weight must be less than number of inputs.\n";

      throw std::logic_error(buffer.str());
   }

   #endif

   // Get single synaptic weights

   return(synaptic_weights[synaptic_weight_index]);   
}


// const bool& get_display(void) const method

/// This method returns true if messages from this class are to be displayed on the screen, or false if messages 
/// from this class are not to be displayed on the screen.

const bool& Perceptron::get_display(void) const
{
   return(display);
}


//  void set(void) method

/// This method sets the number of inputs to zero and the rest of members to their default values. 

void Perceptron::set(void)
{
   initialize_bias_normal(0.0, 0.2);

   synaptic_weights.set();

   activation_function = HyperbolicTangent;

   display = true;
}


// void set(const unsigned int&) method

/// This method sets a new number of inputs.
/// It also sets the other members of this object to their defaul values. 
/// @param new_inputs_number Number of inputs in the neuron. 

void Perceptron::set(const unsigned int& new_inputs_number)
{
   // Set synaptic weights

   activation_function = HyperbolicTangent;

   initialize_bias_normal(0.0, 0.2);

   synaptic_weights.set(new_inputs_number);   
   initialize_synaptic_weights_normal(0.0, 0.2);
   
   display = true;
}


// void set(const unsigned int&, const double&) method

/// This method sets the number of inputs to a given value and initializes the bias and the synaptic weights with a given value. 
/// @param new_inputs_number Number of inputs in the neuron. 
/// @param new_parameters_value Parameters initialization value.  

void Perceptron::set(const unsigned int& new_inputs_number, const double& new_parameters_value)
{
   bias = new_parameters_value;
   synaptic_weights.set(new_inputs_number, new_parameters_value);

   activation_function = HyperbolicTangent;

   display = true;
}


// void set(const Perceptron&)

/// This method sets the members of this perceptron object with those from other perceptron object. 
/// @param other_perceptron Setting perceptron object. 

void Perceptron::set(const Perceptron& other_perceptron)
{
   bias = other_perceptron.bias;
   
   synaptic_weights = other_perceptron.synaptic_weights;

   activation_function = other_perceptron.activation_function;

   display = other_perceptron.display;
}


// void set_activation_function(const ActivationFunction&) method

/// This method sets a new activation function in the neuron. 
/// @param new_activation_function Activation function.

void Perceptron::set_activation_function(const Perceptron::ActivationFunction& new_activation_function)
{
   activation_function = new_activation_function;
}


// void set_activation_function(const std::string&) method

/// This method sets a new activation function in the perceptron with a string containing 
/// the name of the activation function.
/// @param new_activation_function_name String with name of activation function. 

void Perceptron::set_activation_function(const std::string& new_activation_function_name)
{
   if(new_activation_function_name == "Logistic")
   {
      activation_function = Logistic;
   }
   else if(new_activation_function_name == "HyperbolicTangent")
   {
      activation_function = HyperbolicTangent;
   }
   else if(new_activation_function_name == "Threshold")
   {
      activation_function = Threshold;
   }
   else if(new_activation_function_name == "SymmetricThreshold")
   {
      activation_function = SymmetricThreshold;
   }
   else if(new_activation_function_name == "Linear")
   {
      activation_function = Linear;
   }
   else
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: Perceptron class.\n"
             << "void set_activation_function(const std::string&) method.\n"
   	         << "Unknown activation function: " << new_activation_function_name << ".\n";

      throw std::logic_error(buffer.str());
   }
}


// void set_bias(const double&) method

/// This method sets a new bias value for the perceptron.
/// @param new_bias Bias value.
 
void Perceptron::set_bias(const double& new_bias)
{
   bias = new_bias;   
}


// void set_synaptic_weights(const Vector<double>&) method

/// This method a new set of synaptic weights for the perceptron.
/// @param new_synaptic_weights Synaptic weight values.
 
void Perceptron::set_synaptic_weights(const Vector<double>& new_synaptic_weights)
{
   // Control sentence (if debug)

   #ifdef _DEBUG 

   const unsigned int inputs_number = count_inputs_number();

   if(new_synaptic_weights.size() != inputs_number)
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: Perceptron class.\n"
             << "void set_synaptic_weights(const Vector<double>&) method.\n"
             << "Size of synaptic weights vector must be equal to number of inputs.\n";

      throw std::logic_error(buffer.str());
   }

   #endif

   // Set synaptic weights
   
   synaptic_weights = new_synaptic_weights;
}


// void set_synaptic_weight(const unsigned int&, const double&) method

/// This method sets the synaptic weight value with index i for the neuron.
/// @param synaptic_weight_index Synaptic weight index.
/// @param new_synaptic_weight Synaptic weight value.

void Perceptron::set_synaptic_weight(const unsigned int& synaptic_weight_index, const double& new_synaptic_weight)
{
   // Control sentence (if debug)

   #ifdef _DEBUG 

   const unsigned int inputs_number = count_inputs_number();

   if(synaptic_weight_index >= inputs_number)
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: Perceptron class.\n"
             << "void set_synaptic_weight(const unsigned int&, const double&) method.\n"
             << "Index of synaptic weight must be less than number of inputs.\n";

      throw std::logic_error(buffer.str());
   }

   #endif

   // Set single synaptic weight

   synaptic_weights[synaptic_weight_index] = new_synaptic_weight;
}


// void set_display(const bool&) method

/// This method sets a new display value. 
/// If it is set to true messages from this class are to be displayed on the screen;
/// if it is set to false messages from this class are not to be displayed on the screen.
/// @param new_display Display value.
 
void Perceptron::set_display(const bool& new_display)
{
   display = new_display;   
}


// void set_inputs_number(unsigned int) method

/// This method sets a new number of inputs in the neuron.
/// The new synaptic weights are initialized at random with a normal distribution of 
/// mean 0 and standard deviation 1.
/// @param new_inputs_number Number of inputs in the neuton.
 
void Perceptron::set_inputs_number(const unsigned int& new_inputs_number)
{
   initialize_bias_normal(0.0,1.0);

   synaptic_weights.set(new_inputs_number);
   initialize_synaptic_weights_normal(0.0,1.0);
}


// unsigned int count_parameters_number(void) const method

/// This method returns the number of parameters (bias and synaptic weights) in the perceptron.

unsigned int Perceptron::count_parameters_number(void) const
{
   const unsigned int inputs_number = count_inputs_number();
   
   return(1 + inputs_number);
}


// Vector<double> arrange_parameters(void) const method

/// This method returns the parameters (bias and synaptic weights) of the perceptron.

Vector<double> Perceptron::arrange_parameters(void) const
{
   const unsigned int parameters_number = count_parameters_number();

   Vector<double> parameters(parameters_number);

   parameters[0] = bias;

   const unsigned int inputs_number = count_inputs_number();

   for(unsigned int i = 0; i < inputs_number; i++)
   {
      parameters[(unsigned int)1+i] = synaptic_weights[i];
   }

   return(parameters); 
}


// void set_parameters(const Vector<double>&) method

/// This method set a new set of parameters (bias and synaptic weights) to the perceptron.
/// @param new_parameters New set of parameters

void Perceptron::set_parameters(const Vector<double>& new_parameters)
{
   const unsigned int inputs_number = count_inputs_number();

   // Control sentence (if debug)

   #ifdef _DEBUG 

   const unsigned int size = new_parameters.size();

   if(size != 1+inputs_number)
   {
	  std::ostringstream buffer;

      buffer << "OpenNN Exception: Perceptron class.\n"
             << "void set_parameters(const Vector<double>&) method.\n"
             << "Size must be equal to one plus number of inputs.\n";

      throw std::logic_error(buffer.str());
   }

   #endif

   bias = new_parameters[0];

   for(unsigned int i = 0; i < inputs_number; i++)
   {
      synaptic_weights[i] = new_parameters[i+1];
   }
}


// void initialize_bias(const double&) method

/// This method initializes the bias with a given value. 
/// @param value Initialization value. 

void Perceptron::initialize_bias(const double& value)
{
   bias = value;
}


// void initialize_bias_uniform(const double&, const double&) method

/// This method initializes the neuron's bias with a random value chosen from a uniform distribution.
/// @param minimum Minimum initialization value.
/// @param maximum Maximum initialization value.

void Perceptron::initialize_bias_uniform(const double& minimum, const double& maximum)
{
   // Control sentence (if debug)

   #ifdef _DEBUG 

   if(minimum > maximum)
   {
	 std::ostringstream buffer;

      buffer << "OpenNN Exception: Perceptron class.\n" 
             << "initialize_bias_uniform(const double&, const double&) method.\n"
             << "Minimum value must be less than maximum value.\n";

      throw std::logic_error(buffer.str());
   }

   #endif

   bias = calculate_random_uniform(minimum, maximum);
}


// void initialize_synaptic_weights(const double&) method

/// This method initializes all the synaptic weights of the neuron with a given value. 
/// @param value Initialization value. 

void Perceptron::initialize_synaptic_weights(const double& value)
{
   synaptic_weights.initialize(value);
}


// void initialize_synaptic_weights_uniform(const double&, const double&) method

/// This method initializes the neuron's synaptic weights with random values chosen from an uniform distribution.
/// @param minimum Minimum initialization value.
/// @param maximum Maximum initialization value.

void Perceptron::initialize_synaptic_weights_uniform(const double& minimum, const double& maximum)
{
   synaptic_weights.initialize_uniform(minimum, maximum);
}


// void initialize_bias_normal(const double&, const double&) method

/// This method initializes the neuron's bias with random values chosen from a normal distribution.
/// @param mean Mean of normal distribution.
/// @param standard_deviation Standard deviation of normal distribution.

void Perceptron::initialize_bias_normal(const double& mean, const double& standard_deviation)
{
   // Control sentence (if debug)

   #ifdef _DEBUG 

   if(standard_deviation < 0.0)
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: Perceptron class.\n" 
             << "initialize_bias_normal(const double&, const double&) method.\n"
             << "Standard deviation must be equal or greater than zero.\n";

      throw std::logic_error(buffer.str());
   }

   #endif

   bias = calculate_random_normal(mean, standard_deviation);
}


// void initialize_synaptic_weights_normal(const double&, const double&) method

/// This method initializes the neuron's synaptic weights with random values chosen from a normal distribution.
/// @param mean Mean of normal distribution.
/// @param standard_deviation Standard deviation of normal distribution.

void Perceptron::initialize_synaptic_weights_normal(const double& mean, const double& standard_deviation)
{
   synaptic_weights.initialize_normal(mean, standard_deviation);
}


// void initialize_parameters(const double&) method

/// This method initializes the bias and the synaptic weights with a given value. 
/// @param value Parameters initialization value. 

void Perceptron::initialize_parameters(const double& value)
{
   bias = value;
   synaptic_weights.initialize(value);
}


// double calculate_combination(const Vector<double>&) method

/// This method returns the combination to the neuron for a set of inputs signals, using the dot product 
/// combination function.
/// @param inputs Set of inputs signals to the neuron.

double Perceptron::calculate_combination(const Vector<double>& inputs) const
{
   // Control sentence (if debug)

   #ifdef _DEBUG 

   const unsigned int inputs_number = count_inputs_number();

   if(inputs_number == 0)
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: Perceptron class.\n" 
             << "calculate_combination(const Vector<double>&) method.\n"
             << "Number of inputs must be greater than zero.\n";

      throw std::logic_error(buffer.str());
   }

   const unsigned int inputs_size = inputs.size();

   if(inputs_size != inputs_number)
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: Perceptron class.\n"
             << "double calculate_combination(const Vector<double>&) method.\n"
             << "Size of inputs (" << inputs_size << ") must be equal to number of inputs (" << inputs_number << ").\n";

      throw std::logic_error(buffer.str());
   }

   #endif

   // Calculate combination
        
   return(bias + synaptic_weights.dot(inputs));   
}


// double calculate_combination_parameters(const Vector<double>&, const Vector<double>&) const method

/// This method returns which would be the combination for some inputs and for a potential set of parameters. 
/// @param inputs Vector of inputs to the perceptron.
/// @param parameters Potential bias and synaptic weights in the perceptron. 

double Perceptron::calculate_combination_parameters(const Vector<double>& inputs, const Vector<double>& parameters) const
{
   // Control sentence (if debug)

   #ifdef _DEBUG 

   std::ostringstream buffer;

   const unsigned int inputs_size = inputs.size();
   const unsigned int inputs_number = count_inputs_number();

   if(inputs_size != inputs_number)
   {
      buffer << "OpenNN Exception: Perceptron class.\n" 
             << "double calculate_combination_parameters(const Vector<double>&, const Vector<double>&) const method.\n"
             << "Size of inputs must be equal to number of inputs.\n";

      throw std::logic_error(buffer.str());
   }

   const unsigned int parameters_size = parameters.size();

   const unsigned int parameters_number = count_parameters_number();

   if(parameters_size != parameters_number)
   {
      buffer << "OpenNN Exception: Perceptron class.\n" 
             << "double calculate_combination_parameters(const Vector<double>&, const Vector<double>&) const method.\n"
             << "Size of potential parameters (" << parameters_size << ") must be equal to number of parameters (" << parameters_number << ").\n";

      throw std::logic_error(buffer.str());
   }

   #endif

   // Construct a copy 

   Perceptron copy(*this);

   // Set potential parameters

   copy.set_parameters(parameters);

   // Calculate combination

   return(copy.calculate_combination(inputs));  
}


// double calculate_activation(const double&) const  method

/// This method returns the activation from the neuron for a combination.
/// The outputs depends on the activation function used.
/// @param combination Combination of the neuron.

double Perceptron::calculate_activation(const double& combination) const
{
   switch(activation_function)   
   {
      case Perceptron::Logistic:
      {
         return(1.0/(1.0 + exp(-combination)));
      }
      break;
                                     
      case Perceptron::HyperbolicTangent:
      {
         return(1.0-2.0/(exp(2.0*combination)+1.0));   
      }
      break;

      case Perceptron::Threshold:
      {
         if(combination < 0)
         {
            return(0.0);
         }
         else
         {
            return(1.0);
         }
      }
      break;

      case Perceptron::SymmetricThreshold:
      {
         if(combination < 0)
         {
            return(-1.0);
         }
         else
         {
            return(1.0);
         }
      }
      break;

      case Perceptron::Linear:
      {
         return(combination);        
      }
      break;
      
      default:
      {
         std::ostringstream buffer;

         buffer << "OpenNN Exception: Perceptron class.\n" 
                << "double calculate_activation(const double&) const  method.\n"
                << "Unknown activation function.\n";

         throw std::logic_error(buffer.str());
      }
      break;
   }
}


// double calculate_activation_derivative(const double&) const  method

/// This method returns the activation derivative of the neuron for a combination.
/// The activation derivative depends on the activation function used.
/// @param combination Combination of the neuron.

double Perceptron::calculate_activation_derivative(const double& combination) const
{
   switch(activation_function)   
   {
      case Perceptron::Logistic:
      {
         const double exponent = exp(-combination);

         return(exponent/((1.0+exponent)*(1.0+exponent)));
      }
      break;
                                     
      case Perceptron::HyperbolicTangent:
      {
         return(1.0 - pow(tanh(combination), 2));
      }
      break;

      case Perceptron::Threshold:
      {
         if(combination != 0.0)
         {
            return(0.0); 
         }
         else
         {
            std::ostringstream buffer;

            buffer << "OpenNN Exception: Perceptron class.\n" 
                   << "double calculate_activation_derivative(const double&) const  method.\n"
                   << "Threshold activation function is not derivable.\n";

            throw std::logic_error(buffer.str());
         }
      }
      break;

      case Perceptron::SymmetricThreshold:
      {
         if(combination != 0.0)
         {
            return(0.0); 
         }
         else
         {
            std::ostringstream buffer;

            buffer << "OpenNN Exception: Perceptron class.\n" 
                      << "double calculate_activation_derivative(const double&) const  method.\n"
                      << "Symmetric threshold activation function is not derivable.\n";

            throw std::logic_error(buffer.str());
         }
      }
      break;

      case Perceptron::Linear:
      {
         return(1.0);
      }
      break;

      default:
      {
         std::ostringstream buffer;

         buffer << "OpenNN Exception: Perceptron class.\n" 
                << "double calculate_activation_derivative(const double&) const  method.\n"
                << "Unknown activation function.\n";

         throw std::logic_error(buffer.str());
      }
      break;
   }
}


// double calculate_activation_second_derivative(const double&) const  method

/// This method returns the activation second derivative of the neuron for a combination.
/// The second derivative of the outputs depends on the activation function used.
/// @param combination Combination of the neuron.

double Perceptron::calculate_activation_second_derivative(const double& combination) const
{
   switch(activation_function)   
   {
      case Perceptron::Logistic:
      {
         const double exponent = exp(combination);

         return(-exponent*(exponent-1.0)/((exponent+1.0)*(exponent+1.0)*(exponent+1.0)));
      }
      break;
                                     
      case Perceptron::HyperbolicTangent:
      {
         return(-2.0*tanh(combination)*(1.0 - pow(tanh(combination),2)));
      }
      break;

      case Perceptron::Threshold:
      {
         if(combination != 0.0)
         {
            return(0.0); 
         }
         else
         {
            std::ostringstream buffer;

            buffer << "OpenNN Exception: Perceptron class.\n" 
                   << "double calculate_activation_second_derivative(const double&) const  method.\n"
                   << "Threshold activation function is not derivable.\n";

            throw std::logic_error(buffer.str());
         }
      }
      break;

      case Perceptron::SymmetricThreshold:
      {
         if(combination != 0.0)
         {
            return(0.0); 
         }
         else
         {
            std::ostringstream buffer;

            buffer << "OpenNN Exception: Perceptron class.\n" 
                   << "double calculate_activation_second_derivative(const double&) const  method.\n"
                   << "Symmetric threshold activation function is not derivable.\n";

            throw std::logic_error(buffer.str());
         }
      }
      break;

      case Perceptron::Linear:
      {
         return(0.0);        
      }
      break;

      default:
      {
         std::ostringstream buffer;

         buffer << "OpenNN Exception: Perceptron class.\n" 
                << "double calculate_activation_second_derivative(const double&) const  method.\n"
                << "Unknown activation function.\n";

         throw std::logic_error(buffer.str());
      }
      break;
   }
}


// double calculate_outputs(const Vector<double>&) const method

/// This method returns the outputs from the neuron for a set of inputs signals.
/// The outputs depends on the activation function used.
/// @param inputs Set of inputs signals to the neuron.

double Perceptron::calculate_outputs(const Vector<double>& inputs) const
{
   // Control sentence (if debug)

   #ifdef _DEBUG 

   const unsigned int size = inputs.size();
   const unsigned int inputs_number = count_inputs_number();

   if(size != inputs_number)
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: Perceptron class.\n" 
             << "double calculate_outputs(const Vector<double>&) const method.\n"
             << "Size must be equal to number of inputs.\n";

      throw std::logic_error(buffer.str());
   }

   #endif

   // Calculate outputs 

   return(calculate_activation(calculate_combination(inputs)));  
}


// double calculate_parameters_output(const Vector<double>&, const Vector<double>&) const method

/// This method returns the outputs from the neuron for a set of inputs.
/// The outputs depends on the activation function used.
/// @param inputs Set of inputs signals to the neuron.
/// @param parameters Set of potential bias and synaptic weights in the perceptron. 

double Perceptron::calculate_parameters_output(const Vector<double>& inputs, const Vector<double>& parameters) const
{
   // Control sentence (if debug)

   #ifdef _DEBUG 

   const unsigned int inputs_size = inputs.size();
   const unsigned int inputs_number = count_inputs_number();

   if(inputs_size != inputs_number)
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: Perceptron class.\n" 
             << "double calculate_parameters_output(const Vector<double>&, const Vector<double>&) const method.\n"
             << "Size of inputs must be equal to number of inputs.\n";

      throw std::logic_error(buffer.str());
   }

   const unsigned int parameters_size = parameters.size();

   const unsigned int parameters_number = count_parameters_number();

   if(parameters_size != parameters_number)
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: Perceptron class.\n" 
             << "double calculate_parameters_output(const Vector<double>&, const Vector<double>&) const method.\n"
             << "Size of potential parameters (" << parameters_size << ") must be equal to number of parameters (" << parameters_number << ").\n";

      throw std::logic_error(buffer.str());
   }

   #endif

   Perceptron copy(*this);

   copy.set_parameters(parameters);

   return(copy.calculate_outputs(inputs));  
}


// Vector<double> calculate_gradient(const Vector<double>&) const method

/// This method returns the partial derivatives of the outputs with respect to the inputs. 
/// @param inputs Vector of inputs to the perceptron. 

Vector<double> Perceptron::calculate_gradient(const Vector<double>& inputs) const
{
   // Control sentence (if debug)

   #ifdef _DEBUG 

   const unsigned int size = inputs.size();
   const unsigned int inputs_number = count_inputs_number();

   if(size != inputs_number)
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: Perceptron class.\n" 
             << "Vector<double> calculate_gradient(const Vector<double>&) const method.\n"
             << "Size must be equal to number of inputs.\n";

      throw std::logic_error(buffer.str());
   }

   #endif

   // Calculate gradient

   const double combination = calculate_combination(inputs);

   const double activation_derivative = calculate_activation_derivative(combination);

   return(synaptic_weights*activation_derivative);
}


// Vector<double> calculate_parameters_gradient(const Vector<double>&) const method

/// This method returns the partial derivatives of the outputs with respect to the parameters at some inputs point. 
/// @param inputs Vector of inputs to the perceptron. 

Vector<double> Perceptron::calculate_parameters_gradient(const Vector<double>& inputs) const
{
   const unsigned int inputs_number = count_inputs_number();

   // Control sentence (if debug)

   #ifdef _DEBUG 

   const unsigned int size = inputs.size();

   if(size != inputs_number)
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: Perceptron class.\n" 
             << "double calculate_parameters_gradient(const Vector<double>&) const method.\n"
             << "Size must be equal to number of inputs.\n";

      throw std::logic_error(buffer.str());
   }

   #endif

   // Calculate parameters gradient

   const double combination = calculate_combination(inputs);

   const double activation_derivative = calculate_activation_derivative(combination);

   Vector<double> parameters_gradient(1+inputs_number);

   // Bias

   parameters_gradient[0] = activation_derivative;

   // Synaptic weights

   for(unsigned int i = 1; i < 1+inputs_number; i++)
   {
      parameters_gradient[i] = inputs[i-1]*activation_derivative;
   }

   return(parameters_gradient);
}


// Vector<double> calculate_combination_gradient(const Vector<double>&) const method

/// This method returns the partial derivatives of the combination with respect to the inputs.  

Vector<double> Perceptron::calculate_combination_gradient(const Vector<double>&) const
{
   return(synaptic_weights);
}


// Vector<double> calculate_combination_parameters_gradient(const Vector<double>&) const method

/// This method returns the partial derivatives of the combination with respect to the parameters at some inputs point. 
/// @param inputs Vector of inputs to the perceptron. 

Vector<double> Perceptron::calculate_combination_parameters_gradient(const Vector<double>& inputs) const
{   
   const unsigned int inputs_number = count_inputs_number();

   // Control sentence (if debug)

   #ifdef _DEBUG 

   const unsigned int size = inputs.size();

   if(size != inputs_number)
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: Perceptron class.\n" 
             << "double calculate_combination_parameters_gradient(const Vector<double>&) const method.\n"
             << "Size must be equal to number of inputs.\n";

      throw std::logic_error(buffer.str());
   }

   #endif

   // Calculate combination gradient parameters

   Vector<double> combination_parameters_gradient(1+inputs_number);

   // Bias

   combination_parameters_gradient[0] = 1.0;

   // Synaptic weights

   for(unsigned int i = 1; i < 1+inputs_number; i++)
   {
      combination_parameters_gradient[i] = inputs[i-1];
   }

   return(combination_parameters_gradient);
}


// Matrix<double> calculate_Hessian(const Vector<double>&) const method

/// This method returns the second partial derivatives of the outputs with respect to the inputs. 
/// @param inputs Vector of inputs to the perceptron. 

Matrix<double> Perceptron::calculate_Hessian(const Vector<double>& inputs) const
{   
   // Control sentence (if debug)

   #ifdef _DEBUG 

   const unsigned int inputs_number = count_inputs_number();
   const unsigned int inputs_size = inputs.size();

   if(inputs_size != inputs_number)
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: Perceptron class.\n" 
             << "Matrix<double> calculate_Hessian(const Vector<double>&, const Vector<double>&) const method.\n"
             << "Size of inputs must be equal to number of inputs.\n";

      throw std::logic_error(buffer.str());
   }

   #endif

   const double combination = calculate_combination(inputs);
   const double activation_second_derivative = calculate_activation_second_derivative(combination);

   return(synaptic_weights.direct(synaptic_weights)*activation_second_derivative);
}


// Matrix<double> calculate_parameters_Hessian(const Vector<double>&) const method

/// This method retuns the second derivatives of the outputs with respect to the parameters at some inputs point. 
/// @param inputs Input vector. 

Matrix<double> Perceptron::calculate_parameters_Hessian(const Vector<double>& inputs) const
{
   // Control sentence (if debug)

   #ifdef _DEBUG 

   const unsigned int inputs_size = inputs.size();
   const unsigned int inputs_number = count_inputs_number();

   if(inputs_size != inputs_number)
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: Perceptron class.\n" 
             << "Matrix<double> calculate_parameters_Hessian(const Vector<double>&) const method.\n"
             << "Size of inputs must be equal to number of inputs.\n";

      throw std::logic_error(buffer.str());
   }

   #endif

   const double combination = calculate_combination(inputs);
   const double activation_second_derivative = calculate_activation_second_derivative(combination);

   const unsigned int parameters_number = count_parameters_number();

   Matrix<double> parameters_Hessian(parameters_number, parameters_number);

   // Bias - bias derivative

   parameters_Hessian[0][0] = activation_second_derivative;

   // Bias - synaptic weight derivative

   for(unsigned int i = 1; i < parameters_number; i++)
   {
      parameters_Hessian[0][i] = activation_second_derivative*inputs[i-1];
   }

   // Synaptic weight -synaptic weight derivative

   for(unsigned int i = 1; i < parameters_number; i++)
   {
      for(unsigned int j = 1; j < parameters_number; j++)
      {
         parameters_Hessian[i][j] = activation_second_derivative*inputs[i-1]*inputs[j-1];
      }
   }

   // Hessian symmetry

   for(unsigned int i = 0; i < parameters_number; i++)
   {
      for(unsigned int j = 0; j < i; j++)
      {
         parameters_Hessian[i][j] = parameters_Hessian[j][i];
      }
   }
   
   return(parameters_Hessian);   
}


// Matrix<double> calculate_combination_Hessian(const Vector<double>&) const method

/// This method retuns the second derivatives of the combination with respect to the inputs. 

Matrix<double> Perceptron::calculate_combination_Hessian(const Vector<double>&) const
{
   const unsigned int inputs_number = count_inputs_number();

   const Matrix<double> combination_Hessian(inputs_number, inputs_number, 0.0); 

   return(combination_Hessian);   
}


// Matrix<double> calculate_combination_parameters_Hessian(const Vector<double>&) const method

/// This method retuns the second derivatives of the combination with respect to the parameters. 

Matrix<double> Perceptron::calculate_combination_parameters_Hessian(const Vector<double>&) const
{
   const unsigned int parameters_number = count_parameters_number();

   const Matrix<double> Hessian(parameters_number, parameters_number, 0.0);

   return(Hessian);   
}


// void grow_input(void) method

/// This method makes the perceptron to have one more input. 
/// The corresponding synaptic weight is initialized to zero. 

void Perceptron::grow_input(void)
{
   synaptic_weights.push_back(0.0);
}


// void prune_input(const unsigned int&) method

/// This method removes a given input from the neuron.
/// @param index Index of input to be removed. 

void Perceptron::prune_input(const unsigned int& index)
{
   synaptic_weights.erase(synaptic_weights.begin()+index-1);
}


// std::string write_expression(const Vector<std::string>&, const std::string&) const method

/// This method returns a string with the mathematical expression represented by the perceptron.
/// @param inputs_name Name of input variables to the neuron. 
/// @param output_name Name of output variable from the neuron. 

std::string Perceptron::write_expression(const Vector<std::string>& inputs_name, const std::string& output_name) const
{
   const unsigned int inputs_number = count_inputs_number();

   std::string activation_function_name = write_activation_function();

   std::ostringstream buffer;

   buffer << output_name << "=" << activation_function_name << "("
          << bias;

   for(unsigned int i = 0; i < inputs_number; i++)
   {
      if(synaptic_weights[i] >= 0)
      {
         buffer << "+";	     
      }

      buffer << synaptic_weights[i] << "*" << inputs_name[i];
   }

   buffer << ");\n";

   return(buffer.str());
}


// double calculate_random_uniform(const double&, const double&)const  method

/// This method returns a random number chosen from a uniform distribution. 
/// @param minimum Minimum value.
/// @param maximum Maximum value.

double Perceptron::calculate_random_uniform(const double& minimum, const double& maximum) const 
{
   const double random = (double)rand()/(RAND_MAX+1.0);

   const double random_uniform = minimum + (maximum-minimum)*random;

   return(random_uniform);
}


// double calculate_random_normal(const double&, const double&) const method

/// This method returns a random number chosen from a normal distribution. 
/// @param mean Mean value of normal distribution.
/// @param standard_deviation Standard deviation value of normal distribution. 

double Perceptron::calculate_random_normal(const double& mean, const double& standard_deviation) const 
{
   double random_uniform_1;

   do
   {
      random_uniform_1 = (double)rand()/(RAND_MAX+1.0);

   }while(random_uniform_1 == 0.0);

   const double random_uniform_2 = (double)rand()/(RAND_MAX+1.0);

   // Box-Muller transformation

   const double pi = 4.0*atan(1.0);

   const double random_normal = mean + sqrt(-2.0*log(random_uniform_1))*sin(2.0*pi*random_uniform_2)*standard_deviation;

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

