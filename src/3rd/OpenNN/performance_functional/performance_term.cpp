/****************************************************************************************************************/
/*                                                                                                              */
/*   OpenNN: Open Neural Networks Library                                                                       */
/*   www.opennn.cimne.com                                                                                       */
/*                                                                                                              */
/*   P E R F O R M A N C E   T E R M   C L A S S                                                                */
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

#include "performance_term.h"

// TinyXml includes

#include "../../tinyxml/tinyxml.h"


namespace OpenNN
{

// DEFAULT CONSTRUCTOR

/// Default constructor. 
/// It creates a default performance term object, with all pointers initialized to NULL.
/// It also initializes all the rest of class members to their default values.

PerformanceTerm::PerformanceTerm(void)
 : neural_network_pointer(NULL), 
   data_set_pointer(NULL),
   mathematical_model_pointer(NULL),
   numerical_differentiation_pointer(NULL)
{
   set_default();
}


// NEURAL NETWORK CONSTRUCTOR

/// Neural network constructor. 
/// It creates a performance term object associated to a neural network object. 
/// The rest of pointers are initialized to NULL.
/// It also initializes all the rest of class members to their default values.
/// @param new_neural_network_pointer Pointer to a neural network object.

PerformanceTerm::PerformanceTerm(NeuralNetwork* new_neural_network_pointer)
 : neural_network_pointer(new_neural_network_pointer), 
   data_set_pointer(NULL),
   mathematical_model_pointer(NULL),
   numerical_differentiation_pointer(NULL)
{
   set_default();
}


// DATA SET CONSTRUCTOR

/// Data set constructor. 
/// It creates a performance term object associated to a given data set object. 
/// The rest of pointers are initialized to NULL.
/// It also initializes all the rest of class members to their default values.
/// @param new_data_set_pointer Pointer to a data set object.

PerformanceTerm::PerformanceTerm(DataSet* new_data_set_pointer)
 : neural_network_pointer(NULL), 
   data_set_pointer(new_data_set_pointer),
   mathematical_model_pointer(NULL),
   numerical_differentiation_pointer(NULL)
{
   set_default();
}


// MATHEMATICAL MODEL CONSTRUCTOR

/// Mathematical model constructor. 
/// It creates a performance term object associated to a given mathematical model object. 
/// The rest of pointers are initialized to NULL.
/// It also initializes all the rest of class members to their default values.
/// @param new_mathematical_model_pointer Pointer to a mathematical model object.

PerformanceTerm::PerformanceTerm(MathematicalModel* new_mathematical_model_pointer)
 : neural_network_pointer(NULL), 
   data_set_pointer(NULL),
   mathematical_model_pointer(new_mathematical_model_pointer),
   numerical_differentiation_pointer(NULL)
{
   set_default();
}


// NEURAL NETWORK AND DATA SET CONSTRUCTOR

/// Neural network and data set constructor. 
/// It creates a performance term object associated to a neural network and to be measured on a data set. 
/// The rest of pointers are initialized to NULL.
/// It also initializes all the rest of class members to their default values.
/// @param new_neural_network_pointer Pointer to a neural network object.
/// @param new_data_set_pointer Pointer to a data set object.

PerformanceTerm::PerformanceTerm(NeuralNetwork* new_neural_network_pointer, DataSet* new_data_set_pointer)
 : neural_network_pointer(new_neural_network_pointer), 
   data_set_pointer(new_data_set_pointer),
   mathematical_model_pointer(NULL),
   numerical_differentiation_pointer(NULL)
{
   set_default();
}


// NEURAL NETWORK AND MATHEMATICAL MODEL CONSTRUCTOR

/// Neural network and mathematical model constructor. 
/// It creates a performance term object associated to a neural network and to be measured on a mathematical model. 
/// The rest of pointers are initialized to NULL.
/// It also initializes all the rest of class members to their default values.
/// @param new_neural_network_pointer Pointer to a neural network object.
/// @param new_mathematical_model_pointer Pointer to a mathematical model object.

PerformanceTerm::PerformanceTerm(NeuralNetwork* new_neural_network_pointer, MathematicalModel* new_mathematical_model_pointer)
 : neural_network_pointer(new_neural_network_pointer), 
   data_set_pointer(NULL),
   mathematical_model_pointer(new_mathematical_model_pointer),
   numerical_differentiation_pointer(NULL)
{
   set_default();
}


// NEURAL NETWORK, MATHEMATICAL MODEL AND DATA SET CONSTRUCTOR

/// Neural network, mathematical model and data set constructor. 
/// It creates a performance term object associated to a neural network and to be measured on both a mathematical model and a data set. 
/// The rest of pointers are initialized to NULL.
/// It also initializes all the rest of class members to their default values.
/// @param new_neural_network_pointer Pointer to a neural network object.
/// @param new_mathematical_model_pointer Pointer to a mathematical model object.
/// @param new_data_set_pointer Pointer to a data set object.

PerformanceTerm::PerformanceTerm(NeuralNetwork* new_neural_network_pointer, MathematicalModel* new_mathematical_model_pointer, DataSet* new_data_set_pointer)
 : neural_network_pointer(new_neural_network_pointer), 
   data_set_pointer(new_data_set_pointer),
   mathematical_model_pointer(new_mathematical_model_pointer),
   numerical_differentiation_pointer(NULL)
{
   set_default();
}


// XML CONSTRUCTOR

/// XML constructor. 
/// It creates a default performance term object, with all pointers initialized to NULL.
/// It also loads all the rest of class members from a XML element.
/// @param performance_term_element Pointer to a TinyXML element with the object data.

PerformanceTerm::PerformanceTerm(TiXmlElement* performance_term_element)
 : neural_network_pointer(NULL), 
   data_set_pointer(NULL),
   mathematical_model_pointer(NULL),
   numerical_differentiation_pointer(NULL)
{
   set_default();

   from_XML(performance_term_element);
}


// COPY CONSTRUCTOR

/// Copy constructor. 
/// It creates a copy of an existing performance term object. 
/// @param other_performance_term Performance term object to be copied.

PerformanceTerm::PerformanceTerm(const PerformanceTerm& other_performance_term)
 : neural_network_pointer(NULL), 
   data_set_pointer(NULL),
   mathematical_model_pointer(NULL),
   numerical_differentiation_pointer(NULL)
{
   neural_network_pointer = other_performance_term.neural_network_pointer;

   data_set_pointer = other_performance_term.data_set_pointer;
   mathematical_model_pointer = other_performance_term.mathematical_model_pointer;

   if(other_performance_term.numerical_differentiation_pointer)
   {
      numerical_differentiation_pointer = new NumericalDifferentiation(*other_performance_term.numerical_differentiation_pointer);
   }

   display = other_performance_term.display;  
}


// DESTRUCTOR

/// Destructor.
/// It deletes the numerical differentiation object composing this performance term object. 

PerformanceTerm::~PerformanceTerm(void)
{
   delete numerical_differentiation_pointer;
}


// ASSIGNMENT OPERATOR

// PerformanceTerm& operator = (const PerformanceTerm&) method

/// Assignment operator. 
/// It assigns to this performance term object the members from another performance term object. 
/// @param other_performance_term Performance term object to be copied. 

PerformanceTerm& PerformanceTerm::operator = (const PerformanceTerm& other_performance_term)
{
   if(this != &other_performance_term) 
   {
      neural_network_pointer = other_performance_term.neural_network_pointer;
      data_set_pointer = other_performance_term.data_set_pointer;
      mathematical_model_pointer = other_performance_term.mathematical_model_pointer;
      numerical_differentiation_pointer = other_performance_term.numerical_differentiation_pointer;
      display = other_performance_term.display;
   }

   return(*this);
}


// EQUAL TO OPERATOR

// bool operator == (const PerformanceTerm&) const method

/// Equal to operator. 
/// It compares this object to another object. 
/// The return is true if both objects have the same member data, and false otherwise. 

bool PerformanceTerm::operator == (const PerformanceTerm& other_performance_term) const
{
   if(*neural_network_pointer == *other_performance_term.neural_network_pointer
   && *data_set_pointer == *other_performance_term.data_set_pointer
   && *mathematical_model_pointer == *other_performance_term.mathematical_model_pointer
//   && *numerical_differentiation_pointer == *other_performance_term.numerical_differentiation_pointer
   && display == other_performance_term.display)
   {
      return(true);
   }
   else
   {
      return(false);
   }
}


// METHODS

// const bool& get_display(void) const method

/// This method returns true if messages from this class can be displayed on the screen, or false if messages 
/// from this class can't be displayed on the screen.

const bool& PerformanceTerm::get_display(void) const
{
   return(display);
}


// void set(void) method

/// This method sets all the member pointers to NULL (neural network, data set, mathematical model and numerical differentiation).   
/// It also initializes all the rest of class members to their default values.

void PerformanceTerm::set(void)
{
   neural_network_pointer = NULL;
   data_set_pointer = NULL;
   mathematical_model_pointer = NULL;
   numerical_differentiation_pointer = NULL;

   set_default();
}


// void set(NeuralNetwork*) method

/// This method sets all the member pointers to NULL, but the neural network, which set to a given pointer. 
/// It also initializes all the rest of class members to their default values.
/// @param new_neural_network_pointer Pointer to a neural network object.

void PerformanceTerm::set(NeuralNetwork* new_neural_network_pointer)
{
   neural_network_pointer = new_neural_network_pointer;
   data_set_pointer = NULL;
   mathematical_model_pointer = NULL;
   numerical_differentiation_pointer = NULL;

   set_default();
}


// void set(DataSet*) method

/// This method sets all the member pointers to NULL, but the data set, which set to a given pointer. 
/// It also initializes all the rest of class members to their default values.
/// @param new_data_set_pointer Pointer to a data set object.

void PerformanceTerm::set(DataSet* new_data_set_pointer)
{
   neural_network_pointer = NULL;
   data_set_pointer = new_data_set_pointer;
   mathematical_model_pointer = NULL;
   numerical_differentiation_pointer = NULL;

   set_default();
}


// void set(MathematicalModel*) method

/// This method sets all the member pointers to NULL, but the mathematical model, which set to a given pointer. 
/// It also initializes all the rest of class members to their default values.
/// @param new_mathematical_model_pointer Pointer to a mathematical model object.

void PerformanceTerm::set(MathematicalModel* new_mathematical_model_pointer)
{
   neural_network_pointer = NULL;
   data_set_pointer = NULL;
   mathematical_model_pointer = new_mathematical_model_pointer;
   numerical_differentiation_pointer = NULL;

   set_default();
}


// void set(NeuralNetwork*, DataSet*) method

/// This method sets new neural network and data set pointers. 
/// It also sets the mathematical model and numerical differentiation pointers to NULL.
/// Finally, it initializes all the rest of class members to their default values.
/// @param new_neural_network_pointer Pointer to a neural network object.
/// @param new_data_set_pointer Pointer to a data set object.

void PerformanceTerm::set(NeuralNetwork* new_neural_network_pointer, DataSet* new_data_set_pointer)
{
   neural_network_pointer = new_neural_network_pointer;
   data_set_pointer = new_data_set_pointer;
   mathematical_model_pointer = NULL;
   numerical_differentiation_pointer = NULL;

   set_default();
}


// void set(NeuralNetwork*, MathematicalModel*) method

/// This method sets new neural network and mathematical model pointers. 
/// It also sets the data set and numerical differentiation pointers to NULL.
/// Finally, it initializes all the rest of class members to their default values.
/// @param new_neural_network_pointer Pointer to a neural network object.
/// @param new_mathematical_model_pointer Pointer to a data set object.

void PerformanceTerm::set(NeuralNetwork* new_neural_network_pointer, MathematicalModel* new_mathematical_model_pointer)
{
   neural_network_pointer = new_neural_network_pointer;
   data_set_pointer = NULL;
   mathematical_model_pointer = new_mathematical_model_pointer;
   numerical_differentiation_pointer = NULL;

   set_default();
}


// void set(NeuralNetwork*, MathematicalModel*, DataSet*) method

/// This method sets new neural network, mathematical model and data set pointers. 
/// The numerical differentiation pointer is set to NULL.
/// It also initializes all the rest of class members to their default values.
/// @param new_neural_network_pointer Pointer to a neural network object.
/// @param new_mathematical_model_pointer Pointer to mathematical model object.
/// @param new_data_set_pointer Pointer to a data set object.

void PerformanceTerm::set(NeuralNetwork* new_neural_network_pointer, MathematicalModel* new_mathematical_model_pointer, DataSet* new_data_set_pointer)
{
   neural_network_pointer = new_neural_network_pointer;
   data_set_pointer = new_data_set_pointer;
   mathematical_model_pointer = new_mathematical_model_pointer;
   numerical_differentiation_pointer = NULL;

   set_default();
}


// void set(const PerformanceTerm&) method

/// This method sets to this performance term object the members of another performance term object. 
/// @param other_performance_term Performance term to be copied. 

void PerformanceTerm::set(const PerformanceTerm& other_performance_term)
{
   neural_network_pointer = other_performance_term.neural_network_pointer;

   data_set_pointer = other_performance_term.data_set_pointer;

   mathematical_model_pointer = other_performance_term.mathematical_model_pointer;

   if(other_performance_term.numerical_differentiation_pointer)
   {
      numerical_differentiation_pointer = new NumericalDifferentiation(*other_performance_term.numerical_differentiation_pointer);
   }

   display = other_performance_term.display;  
}


// void set_neural_network_pointer(NeuralNetwork*) method

/// This method sets a pointer to a neural network object which is to be associated to the performance term.
/// @param new_neural_network_pointer Pointer to a neural network object to be associated to the performance term.

void PerformanceTerm::set_neural_network_pointer(NeuralNetwork* new_neural_network_pointer)
{
   neural_network_pointer = new_neural_network_pointer;
}


// void set_mathematical_model_pointer(MathematicalModel*) method

/// This method sets a new mathematical model on which the performance term is to be measured. 

void PerformanceTerm::set_mathematical_model_pointer(MathematicalModel* new_mathematical_model_pointer)
{
   mathematical_model_pointer = new_mathematical_model_pointer;
}


// void set_data_set_pointer(DataSet*) method

/// This method sets a new data set on which the performance term is to be measured. 

void PerformanceTerm::set_data_set_pointer(DataSet* new_data_set_pointer)
{
   data_set_pointer = new_data_set_pointer;
}


// void set_numerical_differentiation_pointer(NumericalDifferentiation*) method

/// This method sets a new numerical differentiation pointer in this performance term object. 
/// @param new_numerical_differentiation_pointer Pointer to a numerical differentiation object. 

void PerformanceTerm::set_numerical_differentiation_pointer(NumericalDifferentiation* new_numerical_differentiation_pointer)
{
   numerical_differentiation_pointer = new_numerical_differentiation_pointer;
}


// void set_default(void) method

/// This method sets the members of the performance term to their default values:
/// <ul>
/// <li> Display: true.
/// </ul>

void PerformanceTerm::set_default(void)
{
   display = true;
}


// void set_display(const bool&) method

/// This method sets a new display value. 
/// If it is set to true messages from this class are to be displayed on the screen;
/// if it is set to false messages from this class are not to be displayed on the screen.
/// @param new_display Display value.

void PerformanceTerm::set_display(const bool& new_display)
{
   display = new_display;
}


// void construct_numerical_differentiation(void) method

/// This method constructs the numerical differentiation object which composes the performance term class. 

void PerformanceTerm::construct_numerical_differentiation(void)
{
   if(!numerical_differentiation_pointer)
   {
      numerical_differentiation_pointer = new NumericalDifferentiation();
   }
}


// void delete_numerical_differentiation_pointer(void) method

/// This method deletes the numerical differentiation object which composes the performance term class. 

void PerformanceTerm::delete_numerical_differentiation_pointer(void)
{
   delete numerical_differentiation_pointer;

   numerical_differentiation_pointer = NULL;
}


// void check(void) const method

/// This method checks that there is a neural network associated to the performance term.
/// If some of the above conditions is not hold, the method throws an exception. 

void PerformanceTerm::check(void) const
{
   std::ostringstream buffer;

   if(!neural_network_pointer)
   {
      buffer << "OpenNN Exception: PerformanceTerm class.\n"
             << "void check(void) const.\n"
             << "Pointer to neural network is NULL.\n";

      throw std::logic_error(buffer.str().c_str());	  
   }
}


// Vector< Vector<double> > calculate_layers_delta(const Vector< Vector<double> >&, const Vector<double>&) method

/// This method returns the delta vector for all the layers in the multilayer perceptron 
/// The format of this quantity is a vector of vectors. 
/// @param layers_activation_derivative Forward propagation activation derivative. 
/// @param output_objective_gradient Gradient of the outputs objective function. 

Vector< Vector<double> > PerformanceTerm::calculate_layers_delta
(const Vector< Vector<double> >& layers_activation_derivative, 
 const Vector<double>& output_objective_gradient) const
{
   // Neural network stuff

   #ifdef _DEBUG 
   
   check();

   #endif

   const MultilayerPerceptron* multilayer_perceptron_pointer = neural_network_pointer->get_multilayer_perceptron_pointer();

   #ifdef _DEBUG 

   std::ostringstream buffer;

   if(!multilayer_perceptron_pointer)
   {
      buffer << "OpenNN Exception: PerformanceTerm class.\n"
             << "Vector< Vector<double> > calculate_layers_delta(const Vector< Vector<double> >&, const Vector<double>&) const method.\n"
             << "Pointer to multilayer perceptron in neural network is NULL.\n";

      throw std::logic_error(buffer.str().c_str());	  
   }

   #endif

   const unsigned int layers_number = multilayer_perceptron_pointer->count_layers_number();
   const Vector<unsigned int> layers_perceptrons_number = multilayer_perceptron_pointer->arrange_layers_perceptrons_numbers();

   // Control sentence (if debug)

   #ifdef _DEBUG 

   // Forward propagation activation derivative size

   const unsigned int layers_activation_derivative_size = layers_activation_derivative.size();

   if(layers_activation_derivative_size != layers_number)
   {
      buffer << "OpenNN Exception: PerformanceTerm class.\n"
             << "Vector< Vector<double> > calculate_layers_delta(const Vector< Vector<double> >&, const Vector<double>&) method.\n"
             << "Size of forward propagation activation derivative vector must be equal to number of layers.\n";

      throw std::logic_error(buffer.str().c_str());	  
   }

   if(layers_number > 0)
   {
      const unsigned int output_objective_gradient_size = output_objective_gradient.size();

      if(output_objective_gradient_size != layers_perceptrons_number[layers_number-1])
      {
         buffer << "OpenNN Exception: PerformanceTerm class.\n"
                << "Vector<double> calculate_layers_delta(const Vector< Vector<double> >&, const Vector<double>&) method.\n"
                << "Size of outputs objective gradient (" << output_objective_gradient_size << ") must be equal to "               
                << "number of outputs (" << layers_perceptrons_number[layers_number-1] << ").\n";

         throw std::logic_error(buffer.str().c_str());	     
      }
   }

   #endif

   // Neural network stuff

   Matrix<double> layer_synaptic_weights;

   // Performance functional stuff

   Vector< Vector<double> > layers_delta(layers_number);

   // Output layer

   if(layers_number > 0)
   {
      layers_delta[layers_number-1] = layers_activation_derivative[layers_number-1]*output_objective_gradient;

      // Rest of hidden layers

      for(int i = layers_number-2; i >= 0; i--) 
      {   
		 layer_synaptic_weights = neural_network_pointer->get_multilayer_perceptron_pointer()->get_layer(i+1).arrange_synaptic_weights();

		 layers_delta[i] = layers_activation_derivative[i]*(layers_delta[i+1].dot(layer_synaptic_weights));
	  }
   }

   return(layers_delta);
}


// Vector< Vector<double> > calculate_layers_delta(const Vector< Vector<double> >&, const Vector<double>&, const Vector<double>&) method

/// This method returns the delta vector for all the layers in the multilayer perceptron, when boundary conditions are imposed. 
/// The format of this quantity is a vector of vectors. 
/// @param layers_activation_derivative Forward propagation activation derivative. 
/// @param homogeneous_solution Homogeneous solution for calculating the conditioned outputs. 
/// @param output_objective_gradient Gradient of the outputs objective function. 

Vector< Vector<double> > PerformanceTerm::calculate_layers_delta
(const Vector< Vector<double> >& layers_activation_derivative, 
 const Vector<double>& homogeneous_solution,
 const Vector<double>& output_objective_gradient) const
{
   const MultilayerPerceptron* multilayer_perceptron_pointer = neural_network_pointer->get_multilayer_perceptron_pointer();

   unsigned int layers_number = multilayer_perceptron_pointer->count_layers_number();
   const Vector<unsigned int> layers_perceptrons_number = multilayer_perceptron_pointer->arrange_layers_perceptrons_numbers();

   // Control sentence (if debug)

   #ifdef _DEBUG 

   // Forward propagation activation derivative size

   const unsigned int layers_activation_derivative_size = layers_activation_derivative.size();

   if(layers_activation_derivative_size != layers_number)
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: PerformanceTerm class.\n"
             << "Vector< Vector<double> > calculate_layers_delta(const Vector< Vector<double> >&, const Vector<double>&) const method.\n"
             << "Size of forward propagation activation derivative vector must be equal to number of layers.\n";

      throw std::logic_error(buffer.str().c_str());	  
   }
      
   const unsigned int objective_function_output_gradient_size = output_objective_gradient.size();

   if(objective_function_output_gradient_size != layers_perceptrons_number[layers_number-1])
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: PerformanceTerm class.\n"
             << "Vector<double> calculate_layers_delta(const Vector< Vector<double> >&, const Vector<double>&) const method.\n"
             << "Size of objective function outputs derivative (" << objective_function_output_gradient_size << ")must be equal to "               
             << "number of outputs (" << layers_perceptrons_number[layers_number-1] << ").\n";

      throw std::logic_error(buffer.str().c_str());	  
   }

   #endif

   const Vector<PerceptronLayer>& layers = multilayer_perceptron_pointer->get_layers();

   Matrix<double> synaptic_weights;

   double sum;

   // Set layers delta vector of vectors

   Vector< Vector<double> > layers_delta(layers_number);

   for(unsigned int i = 0; i < layers_number; i++)
   {
      layers_delta[i].set(layers_perceptrons_number[i]);
   }

   // Output layer

   layers_delta[layers_number-1] = layers_activation_derivative[layers_number-1]*homogeneous_solution*output_objective_gradient;

   // Rest of hidden layers

   for(int h = layers_number-2; h >= 0; h--) 
   {   
      for(unsigned int i = 0; i < layers_perceptrons_number[h]; i++)
      {
         sum = 0.0;         

         for(unsigned int j = 0; j < layers_perceptrons_number[h+1]; j++)
         {
            synaptic_weights = layers[h+1].arrange_synaptic_weights();

            sum += (synaptic_weights[i][j])*layers_delta[h+1][j];
         }		 

         layers_delta[h][i] = layers_activation_derivative[h][i]*sum;
      }
   }

   return(layers_delta);
}


// Vector<double> calculate_point_gradient(const Vector<double>&, const Vector< Vector<double> >&, const Vector<double>&) const method

/// This method returns the gradient of the performance term function at some input point. 
/// @param inputs Input vector. 
/// @param layers_activation Activations of all layers in the multilayer perceptron 
/// @param layers_delta Vector of vectors containing the partial derivatives of the outputs objective function with respect to all the combinations of all layers. 

Vector<double> PerformanceTerm::calculate_point_gradient
(const Vector<double>& inputs, 
 const Vector< Vector<double> >& layers_activation, 
 const Vector< Vector<double> >& layers_delta) const
{
   // Control sentence (if debug)

   #ifdef _DEBUG 

   check();

   #endif

   const MultilayerPerceptron* multilayer_perceptron_pointer = neural_network_pointer->get_multilayer_perceptron_pointer();

   #ifdef _DEBUG 

   std::ostringstream buffer;

   if(!multilayer_perceptron_pointer)
   {
      buffer << "OpenNN Exception: PerformanceTerm class.\n"
             << "Vector<double> calculate_point_gradient(const Vector<double>&, const Vector< Vector<double> >&, const Vector<double>&) const method.\n"
             << "Pointer to multilayer perceptron is NULL.\n";

      throw std::logic_error(buffer.str().c_str());	  
   }

   #endif

   const unsigned int inputs_number = multilayer_perceptron_pointer->count_inputs_number();
   const unsigned int layers_number = multilayer_perceptron_pointer->count_layers_number();
   const Vector<unsigned int> layers_perceptrons_number = multilayer_perceptron_pointer->arrange_layers_perceptrons_numbers();

   // Control sentence (if debug)

   #ifdef _DEBUG 
 
   // Input size

   const unsigned int inputs_size = inputs.size();

   if(inputs_size != inputs_number)
   {
      buffer << "OpenNN Exception: PerformanceTerm class.\n"
             << "Vector< Vector<double> > calculate_layers_objective_gradient(const Vector< Vector<double> >&, const Vector<double>&, const Vector<double>&) method.\n"
             << "Size of inputs (" << inputs_size << ") must be equal to inputs number (" << inputs_number << ").\n";

      throw std::logic_error(buffer.str().c_str());  
   }

   // Forward propagation activation size

   const unsigned int layers_activation_size = layers_activation.size();

   if(layers_activation_size != layers_number)
   {
      buffer << "OpenNN Exception: PerformanceTerm class.\n"
             << "Vector< Vector<double> > calculate_layers_objective_gradient(const Vector< Vector<double> >&, const Vector<double>&, const Vector<double>&) method.\n"
             << "Size of forward propagation activation (" << layers_activation_size << ") must be equal to number of layers (" << layers_number << ").\n";

      throw std::logic_error(buffer.str().c_str());	  
   }

   // Hidden errors size

   const unsigned int layers_delta_size = layers_delta.size();
      
   if(layers_delta_size != layers_number)
   {
      buffer << "OpenNN Exception: PerformanceTerm class.\n"
             << "Vector< Vector<double> > calculate_layers_objective_gradient(const Vector< Vector<double> >&, const Vector<double>&) method.\n"
             << "Size of layers delta ("<< layers_delta_size << ") must be equal to number of layers (" << layers_number << ").\n";

      throw std::logic_error(buffer.str().c_str());	  
   }

   #endif

   const unsigned int parameters_number = neural_network_pointer->count_parameters_number();

   Vector<double> point_gradient(parameters_number);

   unsigned int index = 0;

   const Vector< Vector<double> > layers_inputs = multilayer_perceptron_pointer->arrange_layers_input(inputs, layers_activation);

   const Vector< Matrix<double> > layers_combination_parameters_Jacobian = multilayer_perceptron_pointer->calculate_layers_combination_parameters_Jacobian(layers_inputs);

   for(unsigned int i = 0; i < layers_number; i++)
   {
      point_gradient.tuck_in(index, layers_delta[i].dot(layers_combination_parameters_Jacobian[i]));

      index += multilayer_perceptron_pointer->get_layer(i).count_parameters_number();
   }

   if(layers_number != 0)
   {
      Vector<double> synaptic_weights;

      unsigned int index = 0;

      // First layer

      for(unsigned int i = 0; i < layers_perceptrons_number[0]; i++)
      {
         // Bias

         point_gradient[index] = layers_delta[0][i];
         index++;

         // Synaptic weights

         synaptic_weights = multilayer_perceptron_pointer->get_layer(0).get_perceptron(i).arrange_synaptic_weights();

         for(unsigned int j = 0; j < inputs_number; j++)
         {
            point_gradient[index] = layers_delta[0][i]*inputs[j];
            index++;   
         }
      }

      // Rest of layers	
    
      for(unsigned int h = 1; h < layers_number; h++)
      {      
         for(unsigned int i = 0; i < layers_perceptrons_number[h]; i++)
         {
            // Bias

            point_gradient[index] = layers_delta[h][i];
            index++;

            // Synaptic weights

            synaptic_weights = multilayer_perceptron_pointer->get_layer(h).get_perceptron(i).arrange_synaptic_weights();

            for(unsigned int j = 0; j < layers_perceptrons_number[h-1]; j++)
            {
               point_gradient[index] = layers_delta[h][i]*layers_activation[h-1][j];
               index++;   
            }
         }
      }

   }

   return(point_gradient);
}


//  Vector<double> calculate_point_gradient(const Vector< Matrix<double> >&, const Vector< Vector<double> >&) const method

/// This method returns the gradient of the performance term function at some input point. 
/// @param layers_combination_parameters_Jacobian
/// @param layers_delta

Vector<double> PerformanceTerm::calculate_point_gradient
(const Vector< Matrix<double> >& layers_combination_parameters_Jacobian, 
 const Vector< Vector<double> >& layers_delta) const
{
   // Control sentence (if debug)

   #ifdef _DEBUG 

   check();

   #endif

   const MultilayerPerceptron* multilayer_perceptron_pointer = neural_network_pointer->get_multilayer_perceptron_pointer();

   // Control sentence (if debug)

   #ifdef _DEBUG 

   std::ostringstream buffer;

   if(!multilayer_perceptron_pointer)
   {
      buffer << "OpenNN Exception: PerformanceTerm class.\n"
             << "Vector<double> calculate_point_gradient(const Vector<double>&, const Vector< Vector<double> >&, const Vector<double>&) const method.\n"
             << "Pointer to network is NULL.\n";

      throw std::logic_error(buffer.str().c_str());	  
   }

   #endif

   const unsigned int layers_number = multilayer_perceptron_pointer->count_layers_number();
   const Vector<unsigned int> layers_size = multilayer_perceptron_pointer->arrange_layers_perceptrons_numbers();

   // Control sentence (if debug)

   #ifdef _DEBUG 
 
   // Input size

   const unsigned int layers_combination_parameters_Jacobian_size = layers_combination_parameters_Jacobian.size();

   if(layers_combination_parameters_Jacobian_size != layers_number)
   {
      buffer << "OpenNN Exception: PerformanceTerm class.\n"
             << "Vector< Vector<double> > calculate_layers_objective_gradient(const Vector< Vector<double> >&, const Vector<double>&, const Vector<double>&) method.\n"
             << "Size of forward propagation activation (" << layers_combination_parameters_Jacobian_size << ") must be equal to number of layers (" << layers_number << ").\n";

      throw std::logic_error(buffer.str().c_str());	  
   }

   // Hidden errors size

   const unsigned int layers_delta_size = layers_delta.size();
      
   if(layers_delta_size != layers_number)
   {
      buffer << "OpenNN Exception: PerformanceTerm class.\n"
             << "Vector< Vector<double> > calculate_layers_objective_gradient(const Vector< Vector<double> >&, const Vector<double>&) method.\n"
             << "Size of layers delta ("<< layers_delta_size << ") must be equal to number of layers (" << layers_number << ").\n";

      throw std::logic_error(buffer.str().c_str());	  
   }

   #endif

   const unsigned int parameters_number = multilayer_perceptron_pointer->count_parameters_number();

   Vector<double> point_gradient(parameters_number);

   unsigned int index = 0;

   for(unsigned int i = 0; i < layers_number; i++)
   {
      point_gradient.tuck_in(index, layers_delta[i].dot(layers_combination_parameters_Jacobian[i]));

      index += neural_network_pointer->get_multilayer_perceptron_pointer()->get_layer(i).count_parameters_number();
   }

   return(point_gradient);
}


// Matrix< Matrix<double> > calculate_interlayers_Delta(const Vector< Vector<double> >&, const Vector< Vector<double> >&, const Vector<double>&, const Matrix<double>&, const Vector< Vector<double> >&) method

/// This method returns the second partial derivatives of the outputs objective function with respect to the combinations of two layers.
/// That quantity is called interlayers Delta, and it is represented as a matrix of matrices. 
/// @param layers_activation_derivative Activation derivatives of all layers in the multilayer perceptron
/// @param layers_activation_second_derivative Activation second derivatives of all layers in the multilayer perceptron
/// @param interlayers_combination_combination_Jacobian_form Matrix of matrices containing the partial derivatives of all layers combinations with respect to all layers combinations. 
/// @param output_objective_gradient Gradient vector of the outputs objective function. 
/// @param output_objective_Hessian Hessian matrix of the outputs objective function. 
/// @param layers_delta Vector of vectors containing the partial derivatives of the outputs objective function with respect to the combinations of all layers. 
/// @todo

Matrix< Matrix<double> > PerformanceTerm::calculate_interlayers_Delta
(const Vector< Vector<double> >& layers_activation_derivative,
 const Vector< Vector<double> >& layers_activation_second_derivative,
 const Matrix< Matrix<double> >& interlayers_combination_combination_Jacobian_form, 
 const Vector<double>& output_objective_gradient,
 const Matrix<double>& output_objective_Hessian,
 const Vector< Vector<double> >& layers_delta) const
{
   // Neural network stuff

   #ifdef _DEBUG 

   check();

   #endif

   const MultilayerPerceptron* multilayer_perceptron_pointer = neural_network_pointer->get_multilayer_perceptron_pointer();

   #ifdef _DEBUG 

   std::ostringstream buffer;

   if(!multilayer_perceptron_pointer)
   {
      buffer << "OpenNN Exception: PerformanceTerm class.\n"
             << "Matrix< Matrix<double> > calculate_interlayers_Delta(const Vector< Vector<double> >&, const Vector< Vector<double> >&, const Vector<double>&, const Matrix<double>&) method.\n"
             << "Multilayer perceptron pointer is NULL.\n";

      throw std::logic_error(buffer.str().c_str());	  
   }

   #endif

   const unsigned int layers_number = multilayer_perceptron_pointer->count_layers_number();
   const Vector<unsigned int> layers_size = multilayer_perceptron_pointer->arrange_layers_perceptrons_numbers();

   // Control sentence (if debug)

   #ifdef _DEBUG 

   if(layers_number != 0)
   {
      const unsigned int output_objective_gradient_size = output_objective_gradient.size();

      if(output_objective_gradient_size != layers_size[layers_number-1])
      {
         buffer << "OpenNN Exception: PerformanceTerm class.\n"
      //       << "Vector<double> calculate_interlayers_Delta(const Vector<double>&) method.\n"
	  //       << "Size (" << size << ") must be equal to number of parameters (" << parameters_number << ")." 
  	         << std::endl;

         throw std::logic_error(buffer.str().c_str());
      }

      const unsigned int output_objective_Hessian_rows_number = output_objective_Hessian.get_rows_number();
      const unsigned int output_objective_Hessian_columns_number = output_objective_Hessian.get_columns_number();

      if(output_objective_Hessian_rows_number != layers_size[layers_number-1])
      {
         buffer << "OpenNN Exception: PerformanceTerm class.\n"
      //       << "Vector<double> calculate_interlayers_Delta(const Vector<double>&) method.\n"
      //       << "Size (" << size << ") must be equal to number of parameters (" << parameters_number << ")." 
  	         << std::endl;

         throw std::logic_error(buffer.str().c_str());
      }

      if(output_objective_Hessian_columns_number != layers_size[layers_number-1])
      {
         buffer << "OpenNN Exception: PerformanceTerm class.\n"
      //       << "Vector<double> calculate_interlayers_Delta(const Vector<double>&) method.\n"
      //       << "Size (" << size << ") must be equal to number of parameters (" << parameters_number << ")." 
  	         << std::endl;

         throw std::logic_error(buffer.str().c_str());
      }
   }

   #endif

   const Vector< Matrix<double> > layers_synaptic_weights = multilayer_perceptron_pointer->arrange_layers_synaptic_weights();

   // Objective functional stuff

   Matrix< Matrix<double> > interlayers_Delta(layers_number, layers_number);

   for(unsigned int i = 0; i < layers_number; i++)
   {
      for(unsigned int j = 0; j < layers_number; j++)
      {
         interlayers_Delta[i][j].set(layers_size[i], layers_size[j]);
      }
   }

   if(layers_number > 0)
   {
      // Output-outputs layer 

      interlayers_Delta[layers_number-1][layers_number-1] = (output_objective_Hessian*(layers_activation_derivative[layers_number-1]*layers_activation_derivative[layers_number-1])).sum_diagonal(output_objective_gradient*layers_activation_second_derivative[layers_number-1]);

      // Rest of hidden layers

      for(int i = layers_number-1; i >= 0; i--) 
      {   
         for(int j = layers_number-1; j >= 0; j--) 
         {   
            if((unsigned int)i != layers_number-1 && (unsigned int)j != layers_number-1)
            {
			   interlayers_Delta[i][j] = layers_activation_second_derivative[j]*interlayers_combination_combination_Jacobian_form[i][j]*(layers_delta[j+1].dot(layers_synaptic_weights[j+1]))
			   + layers_activation_second_derivative[j]*(interlayers_Delta[i][j+1].dot(layers_synaptic_weights[j+1]));
            }
		 }
	  }
   }

   return(interlayers_Delta);
}


// Matrix<double> calculate_point_Hessian(const Vector<double>&, const Matrix< Matrix<double> >&, const Vector< Vector<double> >&, const Matrix< Matrix<double> >&) const method

/// This method returns the Hessian of the performance term at some input. 
/// @param layers_activation_derivative
/// @param perceptrons_combination_parameters_gradient
/// @param interlayers_combination_combination_Jacobian 
/// @param layers_delta
/// @param interlayers_Delta
/// @todo

Matrix<double> PerformanceTerm::calculate_point_Hessian
(const Vector< Vector<double> >& layers_activation_derivative,
 const Vector< Vector< Vector<double> > >& perceptrons_combination_parameters_gradient, 
 const Matrix< Matrix<double> >& interlayers_combination_combination_Jacobian, 
 const Vector< Vector<double> >& layers_delta,
 const Matrix< Matrix<double> >& interlayers_Delta) const
{
   // Neural network stuff

   #ifdef _DEBUG 

   check();

   #endif

   const MultilayerPerceptron* multilayer_perceptron_pointer = neural_network_pointer->get_multilayer_perceptron_pointer();

   #ifdef _DEBUG 

   std::ostringstream buffer;

   if(!multilayer_perceptron_pointer)
   {
      buffer << "OpenNN Exception: PerformanceTerm class.\n"
             << "Matrix<double> calculate_point_Hessian(const Vector<double>&, const Matrix< Matrix<double> >&, const Vector< Vector<double> >&, const Matrix< Matrix<double> >&) const method.\n"
             << "Multilayer perceptron pointer is NULL.\n";

      throw std::logic_error(buffer.str().c_str());	  
   }

   #endif

   const unsigned int layers_number = multilayer_perceptron_pointer->count_layers_number();

   const unsigned int parameters_number = multilayer_perceptron_pointer->count_parameters_number();

   #ifdef _DEBUG 

   const unsigned int layers_activation_derivative_size = layers_activation_derivative.size();

   if(layers_activation_derivative_size != layers_number)
   {
      buffer << "OpenNN Exception: PerformanceTerm class.\n"
             << "Matrix<double> calculate_point_Hessian(const Vector<double>&, const Matrix< Matrix<double> >&, const Vector< Vector<double> >&, const Matrix< Matrix<double> >&) const method.\n"
             << "Size of layers activation derivative must be equal to number of layers in multilayer perceptron.\n";

      throw std::logic_error(buffer.str().c_str());	  
   }

//   const unsigned int perceptrons_combination_parameters_gradient_size = perceptrons_combination_parameters_gradient.size();

//   const unsigned int interlayers_combination_combination_Jacobian_rows_number = interlayers_combination_combination_Jacobian.get_rows_number();
//   const unsigned int interlayers_combination_combination_Jacobian_columns_number = interlayers_combination_combination_Jacobian.get_columns_number();

//   const unsigned int layers_delta_size = layers_delta.size();

//   const unsigned int interlayers_Delta_rows_number = interlayers_Delta.get_rows_number();
//   const unsigned int interlayers_Delta_columns_number = interlayers_Delta.get_columns_number();

   #endif

   // Objective functional

   Matrix<double> point_Hessian(parameters_number, parameters_number, 0.0);

   Vector<unsigned int> parameter_indices(3);

   unsigned int layer_index_i;
   unsigned int neuron_index_i;
   unsigned int parameter_index_i;

   unsigned int layer_index_j;
   unsigned int neuron_index_j;
   unsigned int parameter_index_j;

   if(layers_number > 0)
   {
      for(unsigned int i = 0; i < parameters_number; i++)
	  {
         parameter_indices = multilayer_perceptron_pointer->arrange_parameter_indices(i);
         layer_index_i = parameter_indices[0];
         neuron_index_i = parameter_indices[1];
         parameter_index_i = parameter_indices[2];

	     for(unsigned int j = 0; j < parameters_number; j++)
		 {
            parameter_indices = multilayer_perceptron_pointer->arrange_parameter_indices(j);
            layer_index_j = parameter_indices[0];
            neuron_index_j = parameter_indices[1];
            parameter_index_j = parameter_indices[2];
           
            point_Hessian[i][j] 
		    = perceptrons_combination_parameters_gradient[layer_index_i][neuron_index_i][parameter_index_i]*perceptrons_combination_parameters_gradient[layer_index_j][neuron_index_j][parameter_index_j]*interlayers_Delta[layer_index_j][layer_index_i][neuron_index_j][neuron_index_i]
			+ perceptrons_combination_parameters_gradient[layer_index_i][neuron_index_i][parameter_index_i]*layers_delta[layer_index_j][neuron_index_j]*layers_activation_derivative[layer_index_j][neuron_index_j]*interlayers_combination_combination_Jacobian[layer_index_j][layer_index_i][neuron_index_j][neuron_index_i];
         }
	  }

      for(unsigned int i = 0; i < parameters_number; i++)
      {
         for(unsigned int j = 0; j < i; j++)
         {
            point_Hessian[i][j] = point_Hessian[j][i];
         }
      }
   } 
   
   return(point_Hessian);
}


// Vector<double> calculate_gradient(void) const method

/// This method returns the default gradient vector of the objective term in the performance function definition. 
/// @todo

Vector<double> PerformanceTerm::calculate_gradient(void) const
{
   // Neural network stuff

   #ifdef _DEBUG 

   check();

   #endif

   // Performance functional stuff

   #ifdef _DEBUG 

   std::ostringstream buffer;

   if(!numerical_differentiation_pointer)
   {
      buffer << "OpenNN Exception: PerformanceTerm class.\n"
             << "Vector<double> calculate_gradient(void) const method.\n"
             << "Numerical differentiation pointer is NULL.\n";

      throw std::logic_error(buffer.str().c_str());	  
   }

   #endif

   const Vector<double> parameters = neural_network_pointer->arrange_parameters();

   return(numerical_differentiation_pointer->calculate_gradient(*this, &PerformanceTerm::calculate_evaluation, parameters));
}


// Matrix<double> calculate_Hessian(void) const method

/// @todo

Matrix<double> PerformanceTerm::calculate_Hessian(void) const
{
   // Neural network stuff

   #ifdef _DEBUG 

   check();

   #endif

   const Vector<double> parameters = neural_network_pointer->arrange_parameters();

   return(numerical_differentiation_pointer->calculate_Hessian(*this, &PerformanceTerm::calculate_evaluation, parameters));
}


// Vector<double> calculate_evaluation_terms(void) const method 

/// This method returns the evaluation of all the subterms composing the performance term. 

Vector<double> PerformanceTerm::calculate_evaluation_terms(void) const
{
   Vector<double> v;

   return(v);
}


// Vector<double> calculate_evaluation_terms(const Vector<double>&) const method 

/// This method returns the evaluation of all the subterms composing the performance term. 

Vector<double> PerformanceTerm::calculate_evaluation_terms(const Vector<double>&) const
{
   Vector<double> v;

   return(v);
}


// Matrix<double> calculate_Jacobian_terms(void) const method 

/// This method returns the Jacobian matrix of the subterms composing the performance term. 

Matrix<double> PerformanceTerm::calculate_Jacobian_terms(void) const
{
   Matrix<double> m;

   return(m);
}


// PerformanceTerm::FirstOrderEvaluationTerms calculate_first_order_evaluation_terms(void) const

/// This method returns the evaluation of all the subterms composing the performance term. 

PerformanceTerm::FirstOrderEvaluationTerms PerformanceTerm::calculate_first_order_evaluation_terms(void) const
{
	FirstOrderEvaluationTerms a;

	return(a);
}


// std::string write_performance_term_type(void) const method

/// This method returns a string with the default type of performance term, "USER_PERFORMANCE_TERM".

std::string PerformanceTerm::write_performance_term_type(void) const
{
   return("USER_PERFORMANCE_TERM");
}


// std::string write_information(void) const method

/// This method returns a string with the default information of the performance term.
/// It will be used by the training strategy to monitor the training process. 
/// By default this information is empty. 

std::string PerformanceTerm::write_information(void) const
{
   return("");
}


// std::string to_string(void) const method

/// This method returns the default string representation of a performance term.

std::string PerformanceTerm::to_string(void) const
{
   std::ostringstream buffer;

   buffer << "Performance term\n"
          << "Display: " << display << "\n";

   return(buffer.str());
}


// TiXmlElement* to_XML(void) const method 

/// This method serializes a default performance term object into a XML element of the TinyXML library. 
/// See the OpenNN manual for more information about the format of this element. 

TiXmlElement* PerformanceTerm::to_XML(void) const
{
   std::ostringstream buffer;

   // Performance term

   TiXmlElement* objective_term_element = new TiXmlElement("PerformanceTerm");
   objective_term_element->SetAttribute("Version", 4);

   return(objective_term_element);
}


// void from_XML(TiXmlElement*) method   

/// This method loads a default performance term from a XML element. 
/// @param objective_term_element Tiny XML element containing the performance term members.

void PerformanceTerm::from_XML(TiXmlElement* objective_term_element)
{
   // Root

   if(!objective_term_element)
   {
      return;
   }

   // Display warnings

   TiXmlElement* display_element = objective_term_element->FirstChildElement("Display");

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


// unsigned int calculate_Kronecker_delta(const unsigned int&, const unsigned int&) const method

/// This method returns the Knronecker delta of two integers a and b, which equals 1 if they are equal and 0 otherwise.
/// @param a First integer.
/// @param b Second integer.

unsigned int PerformanceTerm::calculate_Kronecker_delta(const unsigned int& a, const unsigned int& b) const
{
   if(a == b)
   {
      return(1);
   }
   else
   {
      return(0);
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
