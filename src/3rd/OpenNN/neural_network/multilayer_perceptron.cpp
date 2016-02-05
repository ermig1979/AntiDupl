/****************************************************************************************************************/
/*                                                                                                              */
/*   OpenNN: Open Neural MultilayerPerceptrons Library                                                          */
/*   www.opennn.cimne.com                                                                                       */
/*                                                                                                              */
/*   M U L T I L A Y E R   P E R C E P T R O N   C L A S S                                                      */
/*                                                                                                              */
/*   Roberto Lopez                                                                                              */
/*   International Center for Numerical Methods in Engineering (CIMNE)                                          */
/*   Technical University of Catalonia (UPC)                                                                    */
/*   Barcelona, Spain                                                                                           */
/*   E-mail: rlopez@cimne.upc.edu                                                                               */
/*                                                                                                              */
/****************************************************************************************************************/

// System includes

#include <cmath>   
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>

// OpenNN includes

#include "multilayer_perceptron.h"


namespace OpenNN
{

// DEFAULT CONSTRUCTOR

/// Default constructor. 
///	It creates a multilayer perceptron object witout any layer.
/// This constructor also initializes the rest of class members to their default values.

MultilayerPerceptron::MultilayerPerceptron(void)
{
   set();
}


// LAYERS CONSTRUCTOR

/// Layers constructor. 
/// It creates a multilayer perceptron object with given layers of perceptrons. 
/// This constructor also initializes the rest of class members to their default values.

MultilayerPerceptron::MultilayerPerceptron(const Vector<PerceptronLayer>& new_layers)
{
   set(new_layers);
}


// NETWORK ARCHITECTURE CONSTRUCTOR

/// Architecture constructor. 
/// It creates a multilayer perceptron object with an arbitrary architecture. 
/// The architecture is represented by a vector of unsigned integers. 
/// The first element is the number of inputs.
/// The rest of elements are the number of perceptrons in the subsequent layers. 
/// The multilayer perceptron parameters are initialized at random. 
/// @param new_architecture Vector of unsigned integers representing the architecture of the multilayer perceptron. 

MultilayerPerceptron::MultilayerPerceptron(const Vector<unsigned int>& new_architecture)
{
   set(new_architecture);
}


// ONE LAYER CONSTRUCTOR

/// One layer constructor. 
/// It creates a one-layer perceptron object. 
/// The multilayer perceptron parameters are initialized at random. 
/// This constructor also initializes the rest of class members to their default values:
/// <ul>
/// <li> PerceptronLayer activation function: Linear.
/// </ul> 
/// @param new_inputs_number Number of inputs in the layer.
/// @param new_neurons_number Number of neurons in the layer.

MultilayerPerceptron::MultilayerPerceptron(const unsigned int& new_inputs_number, const unsigned int& new_neurons_number)
{
   set(new_inputs_number, new_neurons_number);
}


// TWO LAYERS CONSTRUCTOR

/// Two layers constructor. 
/// It creates a multilayer perceptron object with a hidden layer of
/// perceptrons and an outputs layer of perceptrons. 
/// The multilayer perceptron parameters are initialized at random. 
/// This constructor also initializes the rest of class members to their default values.
/// @param new_inputs_number Number of inputs in the multilayer perceptron
/// @param new_hidden_neurons_number Number of neurons in the hidden layer of the multilayer perceptron
/// @param new_outputs_number Number of outputs neurons.

MultilayerPerceptron::MultilayerPerceptron(const unsigned int& new_inputs_number, const unsigned int& new_hidden_neurons_number, const unsigned int& new_outputs_number)
{
   set(new_inputs_number, new_hidden_neurons_number, new_outputs_number);

   set_default();
}


// COPY CONSTRUCTOR

/// Copy constructor. 
/// It creates a copy of an existing multilayer perceptron object. 
/// @param other_multilayer_perceptron Multilayer perceptron object to be copied.

MultilayerPerceptron::MultilayerPerceptron(const MultilayerPerceptron& other_multilayer_perceptron)
{
   set(other_multilayer_perceptron);
}


// DESTRUCTOR

/// Destructor.
/// This destructor does not delete any pointer. 

MultilayerPerceptron::~MultilayerPerceptron(void)
{
}


// ASSIGNMENT OPERATOR

/// Assignment operator. 
/// It assigns to this object the members of an existing multilayer perceptron object.
/// @param other_multilayer_perceptron Multilayer perceptron object to be assigned.

MultilayerPerceptron& MultilayerPerceptron::operator = (const MultilayerPerceptron& other_multilayer_perceptron)
{
   if(this != &other_multilayer_perceptron) 
   {
      layers = other_multilayer_perceptron.layers;
   
      display = other_multilayer_perceptron.display;
   }

   return(*this);
}


// EQUAL TO OPERATOR

// bool operator == (const MultilayerPerceptron&) const method

/// Equal to operator. 
/// It compares this object with another object of the same class. 
/// It returns true if the members of the two objects have the same values, and false otherwise.
/// @ param other_multilayer_perceptron Multilayer perceptron to be compared with.

bool MultilayerPerceptron::operator == (const MultilayerPerceptron& other_multilayer_perceptron) const
{
   if(layers == other_multilayer_perceptron.layers   
   && display == other_multilayer_perceptron.display)
   {
      return(true);   
   }
   else
   {
      return(false);
   }
}


// METHODS

// const Vector<PerceptronLayer>& get_layers(void) const method

/// This method returns the layers of the multilayer perceptron 
/// The format is a reference to the vector of vectors of perceptrons.
/// Note that each layer might have a different size.

const Vector<PerceptronLayer>& MultilayerPerceptron::get_layers(void) const 
{
   return(layers);
}


// const PerceptronLayer& get_layer(const unsigned int&) const method

/// This method returns a reference to the vector of perceptrons in a single layer.
/// @param i Index of layer.

const PerceptronLayer& MultilayerPerceptron::get_layer(const unsigned int& i) const 
{
   // Control sentence (if debug)

   #ifdef _DEBUG 

   const unsigned int layers_number = count_layers_number();

   if(i >= layers_number)
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: MultilayerPerceptron class.\n"
             << "const PerceptronLayer get_layer(const unsigned int&) const method.\n"
             << "Index of layer must be less than number of layers.\n";

	  throw std::logic_error(buffer.str());
   }

   #endif
 
   return(layers[i]);
}


// PerceptronLayer* get_layer_pointer(const unsigned int&) const method

/// This method returns a pointer to a given layer of perceptrons. 
/// @param i Index of perceptron layer. 

PerceptronLayer* MultilayerPerceptron::get_layer_pointer(const unsigned int& i) 
{
   // Control sentence (if debug)

   #ifdef _DEBUG 

   const unsigned int layers_number = count_layers_number();

   if(i >= layers_number)
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: MultilayerPerceptron class.\n"
             << "PerceptronLayer* get_layer_pointer(const unsigned int&) const method.\n"
             << "Index of layer must be less than number of layers.\n";

	  throw std::logic_error(buffer.str());
   }

   #endif
 
   return(&layers[i]);
}


// unsigned int count_perceptrons_number(void) const method

/// This method returns the total number of perceptrons in the multilayer perceptron.
/// This is equal to the sum of the perceptrons of all layers. 

unsigned int MultilayerPerceptron::count_perceptrons_number(void) const
{
   const Vector<unsigned int> layers_perceptrons_number = arrange_layers_perceptrons_numbers();

   return(layers_perceptrons_number.calculate_sum());
}


// Vector<unsigned int> count_cumulative_perceptrons_number(void) const method

/// This method returns a vector of size the number of layers, where each element is equal to the total number of neurons in the current and all the previous layers. 

Vector<unsigned int> MultilayerPerceptron::count_cumulative_perceptrons_number(void) const
{
   const unsigned int layers_number = count_layers_number();

   Vector<unsigned int> cumulative_neurons_number(layers_number);

   if(layers_number != 0)
   {
      Vector<unsigned int> layers_size = arrange_layers_perceptrons_numbers();

      cumulative_neurons_number[0] = layers_size[0];

      for(unsigned int i = 1; i < layers_number; i++)
      {
         cumulative_neurons_number[i] = cumulative_neurons_number[i-1] + layers_size[i];   
      }
   }  

   return(cumulative_neurons_number);
}


// Vector<unsigned int> arrange_layers_parameters_number(void) const method

/// This method returns a vector of integers with size the number of layers, 
/// where each element contains the number of parameters in the corresponding layer.

Vector<unsigned int> MultilayerPerceptron::arrange_layers_parameters_number(void) const
{
   const unsigned int layers_number = count_layers_number();

   Vector<unsigned int> layers_parameters_number(layers_number);

   for(unsigned int i = 0; i < layers_number; i++)
   {
      layers_parameters_number[i] = layers[i].count_parameters_number();
   }

   return(layers_parameters_number);
}


// Vector<unsigned int> arrange_layers_cumulative_parameters_number(void) const method

/// This method returns a vector of integers with size the number of layers, 
/// where each element contains the total number of parameters in the corresponding and the previous layers.

Vector<unsigned int> MultilayerPerceptron::arrange_layers_cumulative_parameters_number(void) const
{
   const unsigned int layers_number = count_layers_number();

   Vector<unsigned int> layers_cumulative_parameters_number(layers_number);

   layers_cumulative_parameters_number[0] = layers[0].count_parameters_number();

   for(unsigned int i = 1; i < layers_number; i++)
   {
      layers_cumulative_parameters_number[i] = layers_cumulative_parameters_number[i-1] + layers[i].count_parameters_number();   
   }

   return(layers_cumulative_parameters_number);
}


// Vector< Vector<double> > arrange_layers_biases(void) const method

/// This method returns the bias values from the neurons in all the layers. 
/// The format is a vector of vectors of real values. 
/// The size of this vector is the number of layers.
/// The size of each subvector is the number of neurons in the corresponding layer. 

Vector< Vector<double> > MultilayerPerceptron::arrange_layers_biases(void) const
{
   const unsigned int layers_number = count_layers_number();

   Vector< Vector<double> > layers_biases(layers_number);

   for(unsigned int i = 0; i < layers_number; i++)
   {
      layers_biases[i] = layers[i].arrange_biases();
   }

   return(layers_biases);
}


// Vector< Matrix<double> > arrange_layers_synaptic_weights(void) const method

/// This method returns the synaptic weight values from the neurons in all the layers. 
/// The format is a vector of matrices of real values. 
/// The size of this vector is the number of layers.
/// The number of rows of each sub_matrix is the number of neurons in the corresponding layer. 
/// The number of columns of each sub_matrix is the number of inputs to the corresponding layer. 

Vector< Matrix<double> > MultilayerPerceptron::arrange_layers_synaptic_weights(void) const
{
   const unsigned int layers_number = count_layers_number();

   Vector< Matrix<double> > layers_synaptic_weights(layers_number);

   for(unsigned int i = 0; i < layers_number; i++)
   {
      layers_synaptic_weights[i] = layers[i].arrange_synaptic_weights();   
   }

   return(layers_synaptic_weights);
}


// Vector< Matrix<double> > get_layers_parameters(void) const method

/// This method returns the neural parameter values (biases and synaptic weights) from the neurons in all 
/// the layers. 
/// The format is a vector of matrices of real values. 
/// The size of this vector is the number of layers.
/// The number of rows of each sub_matrix is the number of neurons in the corresponding layer. 
/// The number of columns of each sub_matrix is the number of parameters (inputs + 1) to the corresponding layer. 

Vector< Vector<double> > MultilayerPerceptron::get_layers_parameters(void) const
{
   const unsigned int layers_number = count_layers_number();

   Vector< Vector<double> > layers_parameters(layers_number);

   for(unsigned int i = 0; i < layers_number; i++)
   {
      layers_parameters[i] = layers[i].arrange_parameters();   
   }

   return(layers_parameters);
}


// unsigned int count_parameters_number(void) const method

/// This method returns the number of parameters (biases and synaptic weights) in the multilayer perceptron. 

unsigned int MultilayerPerceptron::count_parameters_number(void) const
{
   const unsigned int layers_number = count_layers_number();

   unsigned int parameters_number = 0;

   for(unsigned int i = 0; i < layers_number; i++)
   {
      parameters_number += layers[i].count_parameters_number();
   }

   return(parameters_number);
}


// Vector<double> arrange_parameters(void) const method

/// This method returns the values of all the biases and synaptic weights in the multilayer perceptron as a single vector.

Vector<double> MultilayerPerceptron::arrange_parameters(void) const
{
   const unsigned int layers_number = count_layers_number();

   const unsigned int parameters_number = count_parameters_number();

   Vector<double> parameters(parameters_number);

   unsigned int position = 0;

   for(unsigned int i = 0; i < layers_number; i++)
   {
      const Vector<double> layer_parameters = layers[i].arrange_parameters();
      const unsigned int layer_parameters_number = layer_parameters.size();

      parameters.tuck_in(position, layer_parameters);
      position += layer_parameters_number; 
   }

   return(parameters);
}


// unsigned int get_layer_index(const unsigned int&) const method

/// This method returns the index of the layer at which a perceptron belongs to. 
/// @param neuron_index Index of the neuron. 

unsigned int MultilayerPerceptron::get_layer_index(const unsigned int& neuron_index) const
{
   // Control sentence (if debug)

   #ifdef _DEBUG 

   const unsigned int neurons_number = count_perceptrons_number();

   if(neuron_index >= neurons_number)
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: MultilayerPerceptron class.\n"
             << "int get_layer_index(const unsigned int&) const method.\n"
             << "Index of neuron must be less than number of neurons.\n";

	  throw std::logic_error(buffer.str());
   }

   #endif

   const Vector<unsigned int> cumulative_neurons_number = count_cumulative_perceptrons_number();

   const unsigned int layer_index = cumulative_neurons_number.calculate_cumulative_index(neuron_index);

   return(layer_index);
}


// unsigned int get_perceptron_index(const unsigned int&, const unsigned int&) const method

/// This method returns the index of a neuron, given the layer it belongs and its position in that layer. 
/// @param layer_index Index of layer. 
/// @param perceptron_position Position on the perceptron in that layer. 

unsigned int MultilayerPerceptron::get_perceptron_index(const unsigned int& layer_index, const unsigned int& perceptron_position) const
{
   if(layer_index == 0)
   {
      return(perceptron_position);
   }
   else
   {
      const Vector<unsigned int> cumulative_neurons_number = count_cumulative_perceptrons_number();

      return(cumulative_neurons_number[layer_index-1] + perceptron_position);
   }
}


// unsigned int get_layer_bias_index(const unsigned int&, const unsigned int&) const method

/// This method returns the index in the vector of parameters of a bias. 
/// @param layer_index Index of layer.
/// @param perceptron_index Index of perceptron within that layer.

unsigned int MultilayerPerceptron::get_layer_bias_index(const unsigned int& layer_index, const unsigned int& perceptron_index) const
{  
   // Control sentence (if debug)

   #ifdef _DEBUG 

   const unsigned int layers_number = count_layers_number();

   if(layer_index >= layers_number)
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: MultilayerPerceptron class.\n"
             << "unsigned int get_layer_bias_index(const unsigned int&, const unsigned int&) const method.\n"
             << "Index of layer must be less than number of layers.\n";

	  throw std::logic_error(buffer.str());
   }

   const unsigned int layer_perceptrons_number = layers[layer_index].count_perceptrons_number(); 

   if(perceptron_index >= layer_perceptrons_number)
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: MultilayerPerceptron class.\n"
             << "unsigned int get_layer_bias_index(const unsigned int&, const unsigned int&) const method.\n"
             << "Index of perceptron must be less than number of perceptrons in that layer.\n";

	  throw std::logic_error(buffer.str());
   }

   #endif

   unsigned int layer_bias_index = 0;

   // Previous layers

   for(unsigned int i = 0; i < layer_index; i++)
   {
      layer_bias_index += layers[i].count_parameters_number();
   }

   // Previous layer neurons

   for(unsigned int j = 0; j < perceptron_index; j++)
   {
      layer_bias_index += layers[layer_index].get_perceptron(j).count_parameters_number();
   }

   return(layer_bias_index);
}


// unsigned int get_layer_synaptic_weight_index(const unsigned int&, const unsigned int&, const unsigned int&) const method

/// This method returns the index in the vector of parameters of a synaptic weight. 
/// @param layer_index Index of layer.
/// @param perceptron_index Index of perceptron within that layer.
/// @param input_index Index of inputs within that perceptron.

unsigned int MultilayerPerceptron::get_layer_synaptic_weight_index
(const unsigned int& layer_index, const unsigned int& perceptron_index, const unsigned int& input_index) const
{
   // Control sentence (if debug)

   #ifdef _DEBUG 

   const unsigned int layers_number = count_layers_number();

   if(layer_index >= layers_number)
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: MultilayerPerceptron class.\n"
             << "unsigned int get_layer_synaptic_weight_index(const unsigned int&, const unsigned int&, const unsigned int&) method.\n"
             << "Index of layer must be less than number of layers.\n";

	  throw std::logic_error(buffer.str());
   }

   const unsigned int layer_perceptrons_number = layers[layer_index].count_perceptrons_number();

   if(perceptron_index >= layer_perceptrons_number)
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: MultilayerPerceptron class.\n"
             << "unsigned int get_layer_synaptic_weight_index(const unsigned int&, const unsigned int&, const unsigned int&) method.\n"
             << "Index of perceptron must be less than number of perceptrons in layer.\n";

	  throw std::logic_error(buffer.str());
   }

   const unsigned int layer_inputs_number = layers[layer_index].count_inputs_number();

   if(input_index >= layer_inputs_number)
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: MultilayerPerceptron class.\n"
             << "unsigned int get_layer_synaptic_weight_index(const unsigned int&, const unsigned int&, const unsigned int&) method.\n"
             << "Index of inputs must be less than number of inputs in perceptron.\n";

	  throw std::logic_error(buffer.str());
   }

   #endif

   unsigned int layer_synaptic_weight_index = 0;

   // Previous layers

   if(layer_index > 0)
   {
      for(unsigned int i = 0; i < layer_index-1; i++)
      {
         layer_synaptic_weight_index += layers[layer_index].count_parameters_number();
      }
   }

   // Previous layer neurons

   if(perceptron_index > 0)
   {
      for(unsigned int i = 0; i < perceptron_index-1; i++)
      {
         layer_synaptic_weight_index += layers[layer_index].get_perceptron(i).count_parameters_number();
      }
   }

   // Hidden neuron bias

   layer_synaptic_weight_index += 1;

   // Synaptic weight index

   layer_synaptic_weight_index += input_index;

   return(layer_synaptic_weight_index);
}


// Vector<unsigned int> arrange_parameter_indices(const unsigned int&) const method

/// This method returns the layer, neuron and parameter indices of a neural parameter.
/// @param parameter_index Index of parameter within the parameters vector. 

Vector<unsigned int> MultilayerPerceptron::arrange_parameter_indices(const unsigned int& parameter_index) const
{
   // Control sentence (if debug)

   #ifdef _DEBUG 

   const unsigned int parameters_number = count_parameters_number();

   if(parameter_index >= parameters_number)
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: MultilayerPerceptron class.\n"
             << "Vector<int> arrange_parameter_indices(const unsigned int&) const method.\n"
             << "Index of neural parameter must be less than number of multilayer_perceptron_pointer parameters.\n";

	  throw std::logic_error(buffer.str());
   }

   #endif

   return(arrange_parameters_indices().arrange_row(parameter_index));
}


// Matrix<unsigned int> arrange_parameters_indices(void) const method

/// This method returns a matrix with the indices of the multilayer_perceptron_pointer parameters. 
/// That indices include the layer index, the neuron index and the parameter index. 
/// The number of rows is the number of multilayer_perceptron_pointer parameters. 
/// The number of columns is 3. 

Matrix<unsigned int> MultilayerPerceptron::arrange_parameters_indices(void) const
{
   unsigned int perceptron_parameters_number;

   const unsigned int layers_number = count_layers_number();
   const Vector<unsigned int> layers_size = arrange_layers_perceptrons_numbers();

   unsigned int parameters_number = count_parameters_number();

   Matrix<unsigned int> parameters_indices(parameters_number, 3);
   
   unsigned int parameter_index = 0;

   for(unsigned int i = 0; i < layers_number; i++)
   {
      for(unsigned int j = 0; j < layers_size[i]; j++)
	  {
         perceptron_parameters_number = layers[i].get_perceptron(j).count_parameters_number();

	     for(unsigned int k = 0; k < perceptron_parameters_number; k++)
		 {
		    parameters_indices[parameter_index][0] = i;
		    parameters_indices[parameter_index][1] = j;
		    parameters_indices[parameter_index][2] = k;
            parameter_index++;
		 }
	  }
   }

   return(parameters_indices);
}


// Vector<Perceptron::ActivationFunction> get_layers_activation_function(void) const method

/// This method returns the activation function of every layer in a single vector. 

Vector<Perceptron::ActivationFunction> MultilayerPerceptron::get_layers_activation_function(void) const
{
   const unsigned int layers_number = count_layers_number();

   Vector<Perceptron::ActivationFunction> layers_activation_function(layers_number);

   for(unsigned int i = 0; i < layers_number; i++)
   {
	  layers_activation_function[i] = layers[i].get_activation_function();
   }

   return(layers_activation_function);
}
   

// Vector<std::string> write_layers_activation_function(void) const method

/// This method returns a vector of strings with the name of the activation functions for the layers. 
/// The size of this vector is the number of layers. 

Vector<std::string> MultilayerPerceptron::write_layers_activation_function(void) const
{
   const unsigned int layers_number = count_layers_number();

   Vector<std::string> layers_activation_function_name(layers_number);

   for(unsigned int i = 0; i < layers_number; i++)
   {
      layers_activation_function_name[i] = layers[i].write_activation_function_name();
   }

   return(layers_activation_function_name);
}


// const bool& get_display(void) const method

/// This method returns true if messages from this class are to be displayed on the screen, or false if messages 
/// from this class are not to be displayed on the screen.

const bool& MultilayerPerceptron::get_display(void) const
{
   return(display);
}


// void set_default(void) method

/// This method sets those members not related to the multilayer perceptron architecture to their default values:
/// <ul>
/// <li> First perceptron layers activation function: Hyperbolic tangent.
/// <li> Last perceptron layer activation function: Linear.
/// <li> Display: True.
/// </ul>

void MultilayerPerceptron::set_default(void)
{
   // Multilayer perceptron architecture

   const unsigned int layers_number = count_layers_number();

   if(layers_number > 0)
   { 
      for(unsigned int i = 0; i < layers_number-1; i++)
      {
         layers[i].set_activation_function(Perceptron::HyperbolicTangent);         
      }

      layers[layers_number-1].set_activation_function(Perceptron::Linear);
   }
 
   // Display messages
   
   set_display(true);
}


// void set(void) method

/// This method sets an empty multilayer_perceptron_pointer architecture. 

void MultilayerPerceptron::set(void)
{
   layers.set();	
}


// void set(const Vector<PerceptronLayer>&) method

/// This method sets a multilayer_perceptron_pointer architecture with given layers of perceptrons. 
/// @param new_layers Vector of vectors of perceptrons, which represent the multilayer_perceptron_pointer architecture. 

void MultilayerPerceptron::set(const Vector<PerceptronLayer>& new_layers)
{
   layers = new_layers;   
}


// void set(const Vector<unsigned int>&) method

/// This method sets the architecture of the multilayer perceptron. 
/// The architecture is represented as a vector of unsigned integers.
/// The number of layers is the size of that vector minus one. 
/// The first element in the vector represents the number of inputs. 
/// The rest of elements represent the corresponding number of perceptrons in each layer. 
/// All the parameters of the multilayer perceptron are initialized at random.
/// @param new_architecture Architecture of the multilayer perceptron.

void MultilayerPerceptron::set(const Vector<unsigned int>& new_architecture)
{
   std::ostringstream buffer;

   const unsigned int new_architecture_size = new_architecture.size();

   if(new_architecture_size == 0)
   {
      set();
   }
   else if(new_architecture_size == 1)
   {
      buffer << "OpenNN Exception: MultilayerPerceptron class.\n" 
             << "void set_architecture(const Vector<unsigned int>&) method.\n"
             << "Size of architecture cannot be one.\n";

   	  throw std::logic_error(buffer.str());   
   }
   else
   {
      for(unsigned int i = 0; i < new_architecture_size; i++)
      {
         if(new_architecture[i] == 0)
         {
            buffer << "OpenNN Exception: MultilayerPerceptron class.\n" 
                   << "void set_architecture(const Vector<unsigned int>&) method.\n"
                   << "Size " << i << " must be greater than zero.\n";

   	        throw std::logic_error(buffer.str());
         }
      }

      const unsigned int new_layers_number = new_architecture_size-1;
      layers.set(new_layers_number);
      
     // First layer

      for(unsigned int i = 0; i < new_layers_number; i++)
      {
	     layers[i].set_perceptrons_number(new_architecture[i+1]);
         layers[i].set_inputs_number(new_architecture[i]);
      }
   
      // Activation

      for(unsigned int i = 0; i < new_layers_number-1; i++)
      {
         layers[i].set_activation_function(Perceptron::HyperbolicTangent);
      }

      layers[new_layers_number-1].set_activation_function(Perceptron::Linear);
   }
}   


// void set(const unsigned int&, const unsigned int&) method

/// This method sets a new architecture with just one layer. 
/// @param new_inputs_number Number of inputs to the multilayer perceptron.
/// @param new_perceptrons_number Number of perceptrons in the unique layer. This is also the number of outputs.

void MultilayerPerceptron::set(const unsigned int& new_inputs_number, const unsigned int& new_perceptrons_number)
{
   // Control sentence (if debug)

   #ifdef _DEBUG 

   std::ostringstream buffer;

   if(new_inputs_number == 0)
   {  
      buffer << "OpenNN Exception: MultilayerPerceptron class.\n" 
             << "void set(const unsigned int&, const unsigned int&) method.\n"
             << "Number of inputs cannot be zero.\n";

	  throw std::logic_error(buffer.str());
   }   
   else if(new_perceptrons_number == 0)
   {  
      buffer << "OpenNN Exception: MultilayerPerceptron class.\n" 
             << "void set_architecture(const unsigned int&, const unsigned int&) method.\n"
             << "Number of perceptrons cannot be zero.\n";

	  throw std::logic_error(buffer.str());
   }

   #endif

   layers.set(1);

   layers[0].set(new_inputs_number, new_perceptrons_number);
}


// void set(const unsigned int&, const unsigned int&, const unsigned int&) method

/// This method sets a new multilayer_perceptron_pointer architecture with two layers, a hidden layer and an outputs layer. 
/// @param new_inputs_number Number of inputs to the multilayer perceptron.
/// @param new_hidden_neurons_number Number of neurons in the hidden layer. 
/// @param new_outputs_number Number of outputs from the multilayer perceptron.

void MultilayerPerceptron::set(const unsigned int& new_inputs_number, const unsigned int& new_hidden_neurons_number, const unsigned int& new_outputs_number)
{
  // Control sentence (if debug)

   #ifdef _DEBUG 

   std::ostringstream buffer;

   if(new_inputs_number == 0)
   {
      buffer << "OpenNN Exception: MultilayerPerceptron class.\n" 
             << "void set(const unsigned int&, const unsigned int&, const unsigned int&) method.\n"
             << "Number of inputs must be greater than zero.\n";

	  throw std::logic_error(buffer.str());
   }
   else if(new_hidden_neurons_number == 0)
   {
      buffer << "OpenNN Exception: MultilayerPerceptron class.\n" 
             << "void set(const unsigned int&, const unsigned int&, const unsigned int&) method.\n"
             << "Number of hidden neurons must be greater than zero.\n";

	  throw std::logic_error(buffer.str());
   }
   else if(new_outputs_number == 0)
   {
      buffer << "OpenNN Exception: MultilayerPerceptron class.\n" 
             << "void set(const unsigned int&, const unsigned int&, const unsigned int&) method.\n"
             << "Number of outputs must be greater than zero.\n";
   
	  throw std::logic_error(buffer.str());
   }
   
   #endif

   layers.set(2);

   layers[0].set(new_inputs_number, new_hidden_neurons_number);
   layers[0].set_activation_function(Perceptron::HyperbolicTangent);

   layers[1].set(new_hidden_neurons_number, new_outputs_number);
   layers[1].set_activation_function(Perceptron::Linear);
}


// void set(const MultilayerPerceptron&) method

/// This method sets the members of this object to be the members of another object of the same class. 
/// @param other_multilayer_perceptron Object to be copied. 

void MultilayerPerceptron::set(const MultilayerPerceptron& other_multilayer_perceptron)
{
   layers = other_multilayer_perceptron.layers;

   display = other_multilayer_perceptron.display;
}


// void set_inputs_number(const unsigned int) method

/// This method set a new number of inputs in the multilayer perceptron. 
/// @param new_inputs_number Number of inputs. 

void MultilayerPerceptron::set_inputs_number(const unsigned int new_inputs_number)
{
   if(is_empty())
   {
      std::ostringstream buffer;
   
      buffer << "OpenNN Exception: MultilayerPerceptron class.\n" 
             << "void set_inputs_number(const unsigned int&) method.\n"
             << "Multilayer perceptron is empty.\n";

	  throw std::logic_error(buffer.str());
   } 

   layers[0].set_inputs_number(new_inputs_number);
}


// void set_layers_perceptrons_number(const Vector<unsigned int>&) method

/// This method sets the size of the layers of the multilayer perceptron.
/// It neither modifies the number of inputs nor the number of outputs. 
/// @param new_layers_size New numbers of neurons for the layers of the multilayer perceptron
/// The number of elements of this vector is the number of layers. 

void MultilayerPerceptron::set_layers_perceptrons_number(const Vector<unsigned int>& new_layers_size)
{
   const Vector<unsigned int> inputs_number(1, count_inputs_number());

   set(inputs_number.get_assembly(new_layers_size));
}


// void set_layer_perceptrons_number(const unsigned int&, const unsigned int&) method

/// This method sets the size of the layer of the multilayer perceptron when this is unique. 
/// All the parameters of the multilayer perceptron are initialized at random.
/// @param layer_index Index of layer.
/// @param new_layer_perceptrons_number New numbers of neurons for that layer of the multilayer perceptron

void MultilayerPerceptron::set_layer_perceptrons_number(const unsigned int& layer_index, const unsigned int& new_layer_perceptrons_number)
{
   const unsigned int layer_inputs_number = layers[layer_index].count_inputs_number();

   layers[layer_index].set_perceptrons_number(new_layer_perceptrons_number);
   
   layers[layer_index].set_inputs_number(layer_inputs_number);

   const unsigned int layers_number = count_layers_number();

   if(layer_index < layers_number-1)
   {
      layers[layer_index+1].set_inputs_number(new_layer_perceptrons_number);   
   }
}


// void set_layers(const Vector<PerceptronLayer>&) method

/// This method sets a new multilayer_perceptron_pointer architecture by means of a pack of layers of perceptrons. 
/// @param new_layers Vector o vectors of perceptron neurons representing a multilayer_perceptron_pointer architecture arranged in layers. 

void MultilayerPerceptron::set_layers(const Vector<PerceptronLayer>& new_layers)
{
   layers = new_layers;
}


// void set_layers_biases(const Vector< Vector<double> >&) method

/// This method sets all the biases of the layers in the multilayer perceptron
/// The format is a vector of vectors of real numbers. 
/// The size of this vector is the number of layers.
/// The size of each subvector is the number of neurons in the corresponding layer. 
/// @param new_layers_biases New set of biases in the layers. 

void MultilayerPerceptron::set_layers_biases(const Vector< Vector<double> >& new_layers_biases)
{
   const unsigned int layers_number = count_layers_number();

   // Control sentence (if debug)

   #ifdef _DEBUG 

   const unsigned int size = new_layers_biases.size();

   if(size != layers_number)
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: MultilayerPerceptron class.\n"
             << "void set_layers_biases(const Vector< Vector<double> >&) method.\n"
             << "Size (" << size << ") must be equal to number of layers (" << layers_number << ").\n";

	  throw std::logic_error(buffer.str());
   }

   #endif

   const Vector<unsigned int> layers_size = arrange_layers_perceptrons_numbers();

   for(unsigned int i = 0; i < layers_number; i++)
   {
      layers[i].set_biases(new_layers_biases[i]); 
   }
}


// void set_layers_synaptic_weights(const Vector< Matrix<double> >&) method

/// This method sets all the synaptic weights of the layers in the multilayer perceptron
/// The format is a vector of matrices of real numbers. 
/// The size of this vector is the number of layers.
/// The number of rows of each sub_matrix is the number of neurons in the corresponding layer. 
/// The number of columns of each sub_matrix is the number of inputs to the corresponding layer. 
/// @param new_layers_synaptic_weights New set of synaptic weights in the layers. 

void MultilayerPerceptron::set_layers_synaptic_weights(const Vector< Matrix<double> >& new_layers_synaptic_weights)
{
   const unsigned int layers_number = count_layers_number();

   // Control sentence (if debug)

   #ifdef _DEBUG 

   const unsigned int size = new_layers_synaptic_weights.size();

   if(size != layers_number)
   {
      std::ostringstream buffer;

	  buffer << "OpenNN Exception: MultilayerPerceptron class.\n"
             << "void set_layers_synaptic_weights(const Vector< Matrix<double> >&) method.\n"
             << "Size must be equal to number of layers.\n";

	  throw std::logic_error(buffer.str());
   }

   #endif

   for(unsigned int i = 0; i < layers_number; i++)
   {
      layers[i].set_synaptic_weights(new_layers_synaptic_weights[i]);
   }

}


// void set_layers_parameters(const Vector< Vector<double> >&) method

/// This method sets the multilayer_perceptron_pointer parameters of all layers in the multilayer perceptron
/// The argument is a vector of vectors of real numbers. 
/// The number of elements is the number of layers. 
/// Each element contains the vector of parameters of a single layer
/// @param new_layers_parameters New vector of layers parameters. 

void MultilayerPerceptron::set_layers_parameters(const Vector< Vector<double> >& new_layers_parameters)
{
   const unsigned int layers_number = count_layers_number();

   // Control sentence (if debug)

   #ifdef _DEBUG 

   const unsigned int new_layers_parameters_size = new_layers_parameters.size();

   if(new_layers_parameters_size != layers_number)
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: MultilayerPerceptron class.\n"
             << "void set_layers_parameters(const Vector< Vector<double> >&) method.\n"
             << "Size of layer parameters must be equal to number of layers.\n";

	  throw std::logic_error(buffer.str());
   }

   #endif

   for(unsigned int i = 0; i < layers_number; i++)
   {
      layers[i].set_parameters(new_layers_parameters[i]);
   }
}


// void set_parameters(const Vector<double>&) method

/// This method sets all the biases and synaptic weights in the multilayer perceptron from a single vector.
/// @param new_parameters New set of biases and synaptic weights values. 

void MultilayerPerceptron::set_parameters(const Vector<double>& new_parameters)
{
   // Control sentence (if debug)

   #ifdef _DEBUG 

   const unsigned int size = new_parameters.size();

   const unsigned int parameters_number = count_parameters_number();

   if(size != parameters_number)
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: MultilayerPerceptron class.\n"
             << "void set_parameters(const Vector<double>&) method.\n"
             << "Size must be equal to number of biases and synaptic weights.\n";

	  throw std::logic_error(buffer.str());
   }

   #endif

   const unsigned int layers_number = count_layers_number();

   unsigned int layer_parameters_number;
   Vector<double> layer_parameters;

   unsigned int position = 0;

   for(unsigned int i = 0; i < layers_number; i++)
   {
      layer_parameters_number = layers[i].count_parameters_number();
      layer_parameters = layers[i].arrange_parameters();

      layer_parameters = new_parameters.take_out(position, layer_parameters_number);
      layers[i].set_parameters(layer_parameters);
      position += layer_parameters_number;
   }
}


// void set_layers_activation_function(const Vector<Perceptron::ActivationFunction>&) method

/// This class sets a new activation (or transfer) function in all the layers of the multilayer perceptron 
/// @param new_layers_activation_function Activation function for the layers.
/// The size of this Vector must be equal to the number of layers, and each element corresponds
/// to the activation function of one layer. 

void MultilayerPerceptron::set_layers_activation_function
(const Vector<Perceptron::ActivationFunction>& new_layers_activation_function)
{ 
   const unsigned int layers_number = count_layers_number();

   // Control sentence (if debug)

   #ifdef _DEBUG 

   const unsigned int new_layers_activation_function_size = new_layers_activation_function.size();

   if(new_layers_activation_function_size != layers_number)
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: MultilayerPerceptron class.\n" 
             << "void set_layers_activation_function(const Vector<Perceptron::ActivationFunction>&) method.\n"
             << "Size of activation function of layers must be equal to number of layers.\n";

	  throw std::logic_error(buffer.str());
   }

   #endif

   for(unsigned int i = 0; i < layers_number; i++)
   {
      layers[i].set_activation_function(new_layers_activation_function[i]);
   }
}


// void set_layers_activation_function(const Vector<std::string>&) method

/// This method sets a new activation (or transfer) function in all the layers. 
/// The argument is a string containing the name of the function ("Logistic", "HyperbolicTangent", "Threshold", etc).
/// @param new_layers_activation_function Activation function for the layers. 

void MultilayerPerceptron::set_layers_activation_function(const Vector<std::string>& new_layers_activation_function)
{
   const unsigned int layers_number = count_layers_number();

   for(unsigned int i = 0; i < layers_number; i++)
   {
      layers[i].set_activation_function(new_layers_activation_function[i]);
   }
}


// void set_display(const bool&) method

/// This method sets a new display value. 
/// If it is set to true messages from this class are to be displayed on the screen;
/// if it is set to false messages from this class are not to be displayed on the screen.
/// @param new_display Display value.

void MultilayerPerceptron::set_display(const bool& new_display)
{
   display = new_display;
}


// bool is_empty(void) const method

/// This method returns true if the number of layers in the multilayer perceptron is zero, and false otherwise. 

bool MultilayerPerceptron::is_empty(void) const
{
   if(layers.empty())
   {
      return(true);
   }
   else
   {
      return(false);
   }
}


// void grow_input(const unsigned int&) method

/// @todo

void MultilayerPerceptron::grow_input(const unsigned int&)
{
}


// void grow_layer(int, int) method

/// @todo

void MultilayerPerceptron::grow_layer(const unsigned int&, const unsigned int&)
{
}


// void prune_input(int) method

/// @todo

void MultilayerPerceptron::prune_input(const unsigned int&)
{
}


// void prune_layer(int, int) method

/// @todo

void MultilayerPerceptron::prune_layer(const unsigned int&, const unsigned int&)
{
}


// void initialize_random(void) method

/// This method sets a random architecture in the multilayer perceptron.
/// It also sets random activation functions for each layer.
/// This method is useful for testing purposes. 

void MultilayerPerceptron::initialize_random(void)
{
   const unsigned int architecture_size = rand()%10 + 2;

   Vector<unsigned int> architecture(architecture_size);

   for(unsigned int i = 0; i < architecture_size; i++)
   {
      architecture[i]  = rand()%10 + 1;
   }

   set(architecture);

   const unsigned int layers_number = count_layers_number();

   // Layers activation function

   for(unsigned int i = 0; i < layers_number; i++)
   {
      switch(rand()%5)
      {
         case 0:
         {
            layers[i].set_activation_function(Perceptron::Logistic);
         }
         break;

         case 1:
         {
            layers[i].set_activation_function(Perceptron::HyperbolicTangent);
         }
         break;

         case 2:
         {
            layers[i].set_activation_function(Perceptron::Threshold);
         }
         break;

         case 3:
         {
            layers[i].set_activation_function(Perceptron::SymmetricThreshold);
         }
         break;

         case 4:
         {
            layers[i].set_activation_function(Perceptron::Linear);
         }
         break;

         default:
         {
            std::ostringstream buffer;

            buffer << "OpenNN Exception: MultilayerPerceptron class.\n"
                   << "void initialize_random(void) method.\n"
                   << "Unknown layer activation function.\n";
 
	        throw std::logic_error(buffer.str());
         }
         break;
      }        
   }

   // Display messages
   
   set_display(true);
}


// void initialize_biases(const double&) method

/// This method initializes the biases of all the perceptrons in the multilayer perceptron with a given value. 
/// @param value Biases initialization value. 

void MultilayerPerceptron::initialize_biases(const double& value)
{
   const unsigned int layers_number = count_layers_number();   

   for(unsigned int i = 0; i < layers_number; i++)
   {
      layers[i].initialize_biases(value);
   }
}


// void initialize_synaptic_weights(const double&) const method

/// This method initializes the synaptic weights of all the perceptrons in the multilayer perceptron with a given value. 
/// @param value Synaptic weights initialization value. 

void MultilayerPerceptron::initialize_synaptic_weights(const double& value) 
{
   const unsigned int layers_number = count_layers_number();   
   const Vector<unsigned int> layers_size = arrange_layers_perceptrons_numbers();

   for(unsigned int i = 0; i < layers_number; i++)
   {
      layers[i].initialize_synaptic_weights(value);
   }
}


// void initialize_parameters(const double&) method

/// This method initializes all the biases and synaptic weights in the neural newtork with a given value.
/// @param value Multilayer perceptron parameters initialization value. 

void MultilayerPerceptron::initialize_parameters(const double& value)
{
   const unsigned int parameters_number = count_parameters_number();

   const Vector<double> parameters(parameters_number, value);

   set_parameters(parameters);
}


// void initialize_parameters_uniform(void) method

/// This method initializes all the biases and synaptic weights in the neural newtork at random with values comprised 
/// between -1 and +1.

void MultilayerPerceptron::initialize_parameters_uniform(void)
{
   const unsigned int parameters_number = count_parameters_number();

   Vector<double> parameters(parameters_number);

   parameters.initialize_uniform();

   set_parameters(parameters);
}


// void initialize_parameters_uniform(const double&, const double&) method

/// This method initializes all the biases and synaptic weights in the multilayer perceptron at random with values 
/// comprised between a minimum and a maximum values.
/// @param minimum Minimum initialization value.
/// @param maximum Maximum initialization value.

void MultilayerPerceptron::initialize_parameters_uniform(const double& minimum, const double& maximum)
{
   const unsigned int parameters_number = count_parameters_number();

   Vector<double> parameters(parameters_number);

   parameters.initialize_uniform(minimum, maximum);

   set_parameters(parameters);
}


// void initialize_parameters_uniform(const Vector<double>&, const Vector<double>&) method

/// This method initializes all the biases and synaptic weights in the multilayer perceptron at random, with values 
/// comprised between different minimum and maximum numbers for each parameter.
/// @param minimum Vector of minimum initialization values.
/// @param maximum Vector of maximum initialization values.

void MultilayerPerceptron::initialize_parameters_uniform(const Vector<double>& minimum, const Vector<double>& maximum)
{
   const unsigned int parameters_number = count_parameters_number();

   Vector<double> parameters(parameters_number);

   parameters.initialize_uniform(minimum, maximum);

   set_parameters(parameters);
}


// void initializeparameters_uniform(const Vector< Vector<double> >&) method

/// This method initializes all the biases and synaptic weights in the multilayer perceptron at random, with values 
/// comprised between a different minimum and maximum numbers for each parameter.
/// All minimum are maximum initialization values must be given from a vector of two real vectors.
/// The first element must contain the minimum inizizalization value for each parameter.
/// The second element must contain the maximum inizizalization value for each parameter.
/// @param minimum_maximum Vector of minimum and maximum initialization values.

void MultilayerPerceptron::initialize_parameters_uniform(const Vector< Vector<double> >& minimum_maximum)
{
   const unsigned int parameters_number = count_parameters_number();

   Vector<double> parameters(parameters_number);

   parameters.initialize_uniform(minimum_maximum[0], minimum_maximum[1]);

   set_parameters(parameters);
}


// void initialize_parameters_normal(void) method

/// This method initializes all the biases and synaptic weights in the newtork with random values chosen from a 
/// normal distribution with mean 0 and standard deviation 1.

void MultilayerPerceptron::initialize_parameters_normal(void)
{
   const unsigned int parameters_number = count_parameters_number();

   Vector<double> parameters(parameters_number);

   parameters.initialize_normal();

   set_parameters(parameters);
}


// void initialize_parameters_normal(const double&, const double&) method

/// This method initializes all the biases and synaptic weights in the multilayer perceptron with random random values 
/// chosen from a normal distribution with a given mean and a given standard deviation.
/// @param mean Mean of normal distribution.
/// @param standard_deviation Standard deviation of normal distribution.

void MultilayerPerceptron::initialize_parameters_normal(const double& mean, const double& standard_deviation)
{
   const unsigned int parameters_number = count_parameters_number();

   Vector<double> parameters(parameters_number);

   parameters.initialize_normal(mean, standard_deviation);

   set_parameters(parameters);
}


// void initialize_parameters_normal(const Vector<double>&, const Vector<double>&) method

/// This method initializes all the biases an synaptic weights in the multilayer perceptron with random values chosen 
/// from normal distributions with different mean and standard deviation for each parameter.
/// @param mean Vector of mean values.
/// @param standard_deviation Vector of standard deviation values.

void MultilayerPerceptron::initialize_parameters_normal(const Vector<double>& mean, const Vector<double>& standard_deviation)
{
   const unsigned int parameters_number = count_parameters_number();

   Vector<double> parameters(parameters_number);

   parameters.initialize_normal(mean, standard_deviation);

   set_parameters(parameters);
}


// void initialize_parameters_normal(const Vector< Vector<double> >&) method

/// This method initializes all the biases and synaptic weights in the multilayer perceptron with random values chosen 
/// from normal distributions with different mean and standard deviation for each parameter.
/// All mean and standard deviation values are given from a vector of two real vectors.
/// The first element must contain the mean value for each parameter.
/// The second element must contain the standard deviation value for each parameter.
/// @param mean_standard_deviation Vector of mean and standard deviation values.

void MultilayerPerceptron::initialize_parameters_normal(const Vector< Vector<double> >& mean_standard_deviation)
{
   const unsigned int parameters_number = count_parameters_number();

   Vector<double> parameters(parameters_number);

   parameters.initialize_normal(mean_standard_deviation[0], mean_standard_deviation[1]);

   set_parameters(parameters);
}


// void initialize_parameters(void) method

/// This method initializes the parameters at random with values chosen from a normal distribution with mean 0 and standard deviation 1.

void MultilayerPerceptron::initialize_parameters(void)
{
   initialize_parameters_normal();
}


// double calculate_parameters_norm(void) const const method

/// This method returns the norm of the vector of multilayer_perceptron_pointer parameters. 

double MultilayerPerceptron::calculate_parameters_norm(void) const 
{
   const Vector<double> parameters = arrange_parameters();

   const double parameters_norm = parameters.calculate_norm();

   return(parameters_norm);
}


// Vector<double> calculate_outputs(const Vector<double>&) const method

/// This method returns the outputs from the multilayer_perceptron_pointer architecture for an inputs to the multilayer_perceptron_pointer architecture. 
/// That is, it computes the inputs-outputs relationship of the raw multilayer perceptron 
/// @param inputs Vector of inputs to the first layer of the multilayer_perceptron_pointer architecture. 

Vector<double> MultilayerPerceptron::calculate_outputs(const Vector<double>& inputs) const
{
   // Control sentence (if debug)

   #ifdef _DEBUG 

   const unsigned int size = inputs.size();

   const unsigned int inputs_number = count_inputs_number();

   if(size != inputs_number) 
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: MultilayerPerceptron class.\n"
             << "Vector<double> calculate_outputs(const Vector<double>&) const method.\n"
             << "Size of inputs (" << size <<") must be equal to number of inputs (" << inputs_number << ").\n";

	  throw std::logic_error(buffer.str());
   }   
   
   #endif

   const unsigned int layers_number = count_layers_number();

   Vector<double> outputs;

   if(layers_number == 0)
   {
      return(outputs);   
   }
   else
   {
      outputs = layers[0].calculate_outputs(inputs);

      for(unsigned int i = 1; i < layers_number; i++)
      {
         outputs = layers[i].calculate_outputs(outputs);
      }
   }

   return(outputs);
}


// Matrix<double> calculate_Jacobian(const Vector<double>&) const method

/// This method returns the partial derivatives of the outputs from the last layer with respect to the inputs to the first layer in the multilayer perceptron 
/// That is, it computes the inputs-outputs partial derivatives of the raw multilayer perceptron 
/// @param inputs Vector of inputs to the first layer of the multilayer_perceptron_pointer architecture. 

Matrix<double> MultilayerPerceptron::calculate_Jacobian(const Vector<double>& inputs) const
{
   #ifdef _DEBUG 

   const unsigned int size = inputs.size();

   const unsigned int inputs_number = count_inputs_number();

   if(size != inputs_number)
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: MultilayerPerceptron class.\n"
             << "Matrix<double> calculate_Jacobian(const Vector<double>&) const method.\n"
             << "Size must be equal to number of inputs.\n";

	  throw std::logic_error(buffer.str());
   }

   #endif

   const unsigned int layers_number = count_layers_number();

   if(layers_number == 0)
   {
      Matrix<double> Jacobian;

      return(Jacobian);
   }
   else
   {   
      const Vector< Matrix<double> > layers_Jacobian = calculate_layers_Jacobian(inputs);

      Matrix<double> Jacobian = layers_Jacobian[layers_number-1];

      for(int i = layers_number-2; i > -1; i--)
      {
         Jacobian = Jacobian.dot(layers_Jacobian[i]);   
      }

      return(Jacobian);
   }
}


// Vector< Matrix<double> > calculate_Hessian_form(const Vector<double>&) const

/// This method returns the second partial derivatives of the outputs from the last layer with respect to the inputs to the first layer. 
/// That is, the Hessian matrix of the multilayer_perceptron_pointer architectue inputs-outputs function. 
/// @todo

Vector< Matrix<double> > MultilayerPerceptron::calculate_Hessian_form(const Vector<double>& inputs) const
{   
   #ifdef _DEBUG 

   const unsigned int size = inputs.size();

   const unsigned int inputs_number = count_inputs_number();

   if(size != inputs_number)
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: MultilayerPerceptron class.\n"
             << "Vector< Matrix<double> > calculate_Hessian_form(const Vector<double>&) const method.\n"
             << "Size must be equal to number of inputs.\n";

	  throw std::logic_error(buffer.str());
   }

   #endif

   const unsigned int layers_number = count_layers_number();
   const Vector<unsigned int> layers_size = arrange_layers_perceptrons_numbers();
   const unsigned int outputs_number = count_outputs_number();

   // Calculate forward propagation of outputs, Jacobian and Hessian form

   const Vector< Matrix<double> > layers_Jacobian = calculate_layers_Jacobian(inputs);

   const Vector< Vector< Matrix<double> > > layers_Hessian_form = calculate_layers_Hessian_form(inputs);

   // Calculate multilayer_perceptron_pointer Hessian form

   Vector< Matrix<double> > Hessian_form = layers_Hessian_form[layers_number-1];
   Matrix<double> Jacobian = layers_Jacobian[layers_number-1];

   for(unsigned int layer_index = layers_number-2; layer_index != 0; layer_index--)
   {
      for(unsigned int output_index = 0; output_index < outputs_number; output_index++)
      {
         Hessian_form[output_index] = (layers_Jacobian[layer_index].calculate_transpose()).dot(Hessian_form[output_index]).dot(layers_Jacobian[layer_index]);

         for(unsigned int neuron_index = 0; neuron_index < layers_size[layer_index]; neuron_index++)
	     {
            Hessian_form[output_index] += layers_Hessian_form[layer_index][neuron_index]*Jacobian[output_index][neuron_index]; 
	     }
	  }

     Jacobian = Jacobian.dot(layers_Jacobian[layer_index]);   
   }

//   Matrix<double> hidden_layer_Jacobian = layers_Jacobian[0];
//   Matrix<double> output_layer_Jacobian = layers_Jacobian[1];

//   Vector< Matrix<double> > hidden_layer_Hessian_form = layers_Hessian_form[0];
//   Vector< Matrix<double> > output_layer_Hessian_form = layers_Hessian_form[1];

//   Vector< Matrix<double> > Hessian_form(outputs_number);

//   for(unsigned int output_index = 0; output_index < outputs_number; output_index++)
//   {
//      Hessian_form[output_index] = (hidden_layer_Jacobian.calculate_transpose()).dot(output_layer_Hessian_form[output_index]).dot(hidden_layer_Jacobian);

//      for(unsigned int neuron_index = 0; neuron_index < layers_size[0]; neuron_index++)
//      {
//         Hessian_form[output_index] += hidden_layer_Hessian_form[neuron_index]*output_layer_Jacobian[output_index][neuron_index]; 
//	  }
//   }

   return(Hessian_form);
}


// Vector<double> calculate_layer_combination_combination(const unsigned int&, const Vector<double>&) const method

/// This method returns the combination vector of a given layer as a function of the combination vector of the previous layer. 
/// @param layer_index Index of layer.
/// @param previous_layer_combination Combination vector of previous layer. 

Vector<double> MultilayerPerceptron::calculate_layer_combination_combination(const unsigned int& layer_index, const Vector<double>& previous_layer_combination) const
{
   // Control sentence (if debug)

   #ifdef _DEBUG 

   const unsigned int layers_number = count_layers_number();

   if(layer_index == 0)
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: MultilayerPerceptron class.\n"
             << "Matrix<double> calculate_layer_combination_combination(const unsigned int&, const Vector<double>&) const.\n"
             << "Index of layer must be greater than zero.\n";

	  throw std::logic_error(buffer.str());
   }
   else if(layer_index >= layers_number)
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: MultilayerPerceptron class.\n"
             << "Matrix<double> calculate_layer_combination_combination(const unsigned int&, const Vector<double>&) const.\n"
             << "Index of layer must be less than numnber of layers.\n";

	  throw std::logic_error(buffer.str());
   }

   const unsigned int size = previous_layer_combination.size();
   const unsigned int previous_layer_perceptrons_number = layers[layer_index-1].count_perceptrons_number();

   if(size != previous_layer_perceptrons_number)
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: MultilayerPerceptron class.\n"
             << "Matrix<double> calculate_layer_combination_combination(const unsigned int&, const Vector<double>&) const.\n"
             << "Size must be equal to size of previous layer.\n";

	  throw std::logic_error(buffer.str());
   }

   #endif

   // Calculate combination to layer

   const Vector<double> previous_layer_activation = layers[layer_index-1].calculate_activation(previous_layer_combination);

   const Vector<double> layer_combination_combination = layers[layer_index].calculate_combination(previous_layer_activation);

   return(layer_combination_combination);
}


// Matrix<double> calculate_layer_combination_combination_Jacobian(const unsigned int&, const Vector<double>&) const method

/// This method returns the partial derivatives of the combinations of one layer with respect to the combinations in the previous layer. 
/// Note that, for efficiency issues, the argument here is the activation derivative of the previous layer, and not the combination of the previous layer. 
/// This quantity is the Jacobian matrix of the layer combination-combination function. 
/// @param layer_index Index of layer. 
/// @param previous_layer_activation_derivative Vector of activation derivatives of the previous layer. 

Matrix<double> MultilayerPerceptron::calculate_layer_combination_combination_Jacobian(const unsigned int& layer_index, const Vector<double>& previous_layer_activation_derivative) const   
{
   const unsigned int previous_layer_perceptrons_number = layers[layer_index-1].count_perceptrons_number();

   // Control sentence (if debug)

   #ifdef _DEBUG 

   const unsigned int layers_number = count_layers_number();

   if(layer_index == 0)
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: MultilayerPerceptron class.\n"
             << "Matrix<double> calculate_layer_combination_combination_Jacobian(const unsigned int&, const Vector<double>&) const.\n"
             << "Index of layer must be greater than zero.\n";

	  throw std::logic_error(buffer.str());
   }
   else if(layer_index >= layers_number)
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: MultilayerPerceptron class.\n"
             << "Matrix<double> calculate_layer_combination_combination_Jacobian(const unsigned int&, const Vector<double>&) const.\n"
             << "Index of layer must be less than numnber of layers.\n";

	  throw std::logic_error(buffer.str());
   }

   const unsigned int size = previous_layer_activation_derivative.size();

   if(size != previous_layer_perceptrons_number)
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: MultilayerPerceptron class.\n"
             << "Matrix<double> calculate_layer_combination_combination_Jacobian(const unsigned int&, const Vector<double>&).\n"
             << "Size of activation derivative must be equal to size of previous layer.\n";

	  throw std::logic_error(buffer.str());
   }

   #endif

   const Matrix<double> layer_synaptic_weights = layers[layer_index].arrange_synaptic_weights();

   Matrix<double> previous_layer_activation_Jacobian(previous_layer_perceptrons_number, previous_layer_perceptrons_number, 0.0);
   previous_layer_activation_Jacobian.set_diagonal(previous_layer_activation_derivative);

   return(layer_synaptic_weights.dot(previous_layer_activation_Jacobian));
}


// Vector<double> calculate_interlayer_combination_combination(const unsigned int&, const unsigned int&, const Vector<double>&) const method

/// This method returns the combination vector of a layer (image) with respect to the combination vector of another layer (domain). 
/// @param domain_layer_index Index of domain layer. 
/// @param image_layer_index Index of image layer.
/// @param domain_layer_combination Combination vector of domain layer. 

Vector<double> MultilayerPerceptron::calculate_interlayer_combination_combination(const unsigned int& domain_layer_index, const unsigned int& image_layer_index, const Vector<double>& domain_layer_combination) const
{
   if(domain_layer_index > image_layer_index)
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: MultilayerPerceptron class.\n"
             << "Vector<double> calculate_interlayer_combination_combination(const unsigned int&, const unsigned int&, const Vector<double>&) const method.\n"
             << "Index of domain layer must be less or equal than index of image layer.\n";

	  throw std::logic_error(buffer.str());
   }

   if(domain_layer_index == image_layer_index)
   {
      return(domain_layer_combination);
   }
   else
   {
      Vector<double> interlayer_combination_combination(domain_layer_combination);     

      for(unsigned int i = domain_layer_index+1; i <= image_layer_index; i++)
      {
         interlayer_combination_combination = calculate_layer_combination_combination(i, interlayer_combination_combination);
      }

      return(interlayer_combination_combination); 
   }
}


// Matrix<double> calculate_interlayer_combination_combination_Jacobian(const unsigned int&, const unsigned int&, const Vector<double>&) const method

/// This method returns the partial derivatives of the combinations of a layer (image) with respect to the combinations of another layer (domain). 
/// This quantity is the Jacobian matrix of the interlayers combination combination function. 
/// @param image_layer_index Index of image layer. 
/// @param domain_layer_index Index of domain layer. 
/// @param domain_layer_combination Vector of combination values of the domain layer. 

Matrix<double> MultilayerPerceptron::calculate_interlayer_combination_combination_Jacobian(const unsigned int& domain_layer_index, const unsigned int& image_layer_index, const Vector<double>& domain_layer_combination) const
{
   Matrix<double> interlayer_combination_combination_Jacobian;

   if(domain_layer_index < image_layer_index)
   {
      const unsigned int size = image_layer_index-domain_layer_index;

      Vector< Vector<double> > layers_combination_combination(size);
      Vector< Matrix<double> > layers_combination_combination_Jacobian(size);

      layers_combination_combination[0] = calculate_layer_combination_combination(domain_layer_index+1, domain_layer_combination);      
      layers_combination_combination_Jacobian[0] = calculate_layer_combination_combination_Jacobian(domain_layer_index+1, layers[domain_layer_index].calculate_activation_derivative(domain_layer_combination));

      for(unsigned int i = 1; i < size; i++)
      {
         layers_combination_combination[i] = calculate_layer_combination_combination(domain_layer_index+i+1, layers_combination_combination[i-1]);

         layers_combination_combination_Jacobian[i] = calculate_layer_combination_combination_Jacobian(domain_layer_index+i+1, layers_combination_combination[i-1]); 
      }

      interlayer_combination_combination_Jacobian = layers_combination_combination_Jacobian[size-1];

      for(int i = size-2; i > -1; i--)
      {
         interlayer_combination_combination_Jacobian = interlayer_combination_combination_Jacobian.dot(layers_combination_combination_Jacobian[i]);   
      }
   }
   else if(domain_layer_index == image_layer_index)
   {
      const unsigned int image_layer_perceptrons_number = layers[image_layer_index].count_perceptrons_number();

      interlayer_combination_combination_Jacobian.set_identity(image_layer_perceptrons_number);
   }
   else
   {
      const unsigned int image_layer_perceptrons_number = layers[image_layer_index].count_perceptrons_number();
      const unsigned int domain_layer_perceptrons_number = layers[domain_layer_index].count_perceptrons_number();

      interlayer_combination_combination_Jacobian.set(image_layer_perceptrons_number, domain_layer_perceptrons_number, 0.0);      
   }

   return(interlayer_combination_combination_Jacobian); 
}


// Vector<double> calculate_output_layer_combination(const unsigned int&, const Vector<double>&) const method

/// This method calculates the output values from the neural network from the combination values of a given layer. 
/// @param layer_index Index of a layer.
/// @param layer_combinations Combination values of the layer with the previous index. 

Vector<double> MultilayerPerceptron::calculate_output_layer_combination(const unsigned int& layer_index, const Vector<double>& layer_combinations) const
{
   // Control sentence (if debug)

   #ifdef _DEBUG 

   const unsigned int size = layer_combinations.size();

   const unsigned int layer_perceptrons_number = layers[layer_index].count_perceptrons_number();

   if(size != layer_perceptrons_number) 
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: MultilayerPerceptron class.\n"
             << "Vector<double> calculate_output_layer_combination(const unsigned int&, const Vector<double>&) const method.\n"
             << "Size must be equal to layer size.\n";

	  throw std::logic_error(buffer.str());
   }   
   
   #endif

   const unsigned int layers_number = count_layers_number();

   Vector<double> outputs = layers[layer_index].calculate_activation(layer_combinations);

   if(layer_index < layers_number)
   {
      for(unsigned int i = layer_index+1; i < layers_number; i++)
      {
         outputs = layers[i].calculate_outputs(outputs);
      }
   }

   return(outputs);
}


// Vector< Matrix<double> > calculate_output_layers_delta(const Vector< Vector<double> >&, const Vector<double>&) method

/// This method calculates the output delta matrix for each layer in the multilayer perceptron.
/// The format is a vector of matrices. 
/// @param layers_activation_derivative Activation derivative values for each layer. 

Vector< Matrix<double> > MultilayerPerceptron::calculate_output_layers_delta(const Vector< Vector<double> >& layers_activation_derivative) const
{
   // Control sentence (if debug)

   const unsigned int layers_number = count_layers_number();
   const Vector<unsigned int> layers_size = arrange_layers_perceptrons_numbers();

   // Control sentence (if debug)

   #ifdef _DEBUG 

   // Forward propagation activation derivative size

   const unsigned int layers_activation_derivative_size = layers_activation_derivative.size();

   if(layers_activation_derivative_size != layers_number)
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: MultilayerPerceptron class.\n"
             << "Vector< Vector<double> > calculate_output_layers_delta(const Vector< Vector<double> >&) method.\n"
             << "Size of forward propagation activation derivative vector must be equal to number of layers.\n";

      throw std::logic_error(buffer.str().c_str());
   }

   #endif

   Vector< Matrix<double> > output_layers_delta(layers_number);

   if(layers_number > 0)
   {
      // Output layer

      output_layers_delta[layers_number-1] = layers[layers_number-1].arrange_activation_Jacobian(layers_activation_derivative[layers_number-1]);

      // Rest of layers

      for(int i = layers_number-2; i >= 0; i--) 
      {   
		 output_layers_delta[i] = output_layers_delta[i+1].dot(layers[i+1].arrange_synaptic_weights()).dot(layers[i].arrange_activation_Jacobian(layers_activation_derivative[i]));
	  }
   }

   return(output_layers_delta);
}


// Matrix< Vector< Matrix<double> > > calculate_output_interlayers_Delta(void) method 

/// @todo
/// @param second_order_forward_propagation
/// @param interlayers_combination_combination_Jacobian
/// @param output_layers_delta

Matrix< Vector< Matrix<double> > > MultilayerPerceptron::calculate_output_interlayers_Delta
(const Vector< Vector< Vector<double> > >& second_order_forward_propagation, 
 const Matrix< Matrix<double> >& interlayers_combination_combination_Jacobian,
 const Vector< Matrix<double> >& output_layers_delta) const
{
   const unsigned int layers_number = count_layers_number();
   const Vector<unsigned int> layers_size = arrange_layers_perceptrons_numbers();
   const unsigned int outputs_number = count_outputs_number();

   // Control sentence (if debug)

   #ifdef _DEBUG 

   std::ostringstream buffer;

   // Second order forward propagation

   const unsigned int second_order_forward_propagation_size = second_order_forward_propagation.size();

   if(second_order_forward_propagation_size != 3)
   {
      buffer << "OpenNN Exception: MultilayerPerceptron class.\n"
             << "Matrix< Vector< Matrix<double> > > calculate_output_interlayers_delta(@todo) method.\n"
             << "Size of second order forward propagation must be three.\n";

      throw std::logic_error(buffer.str().c_str());
   }

   // Interlayers combination-combination Jacobian

   const unsigned int interlayers_combination_combination_Jacobian_rows_number = interlayers_combination_combination_Jacobian.get_rows_number();

   if(interlayers_combination_combination_Jacobian_rows_number != layers_number)
   {
      buffer << "OpenNN Exception: MultilayerPerceptron class.\n"
             << "Matrix< Vector< Matrix<double> > > calculate_output_interlayers_delta(@todo) method.\n"
             << "Number of rows of interlayers combination-combination Jacobian must be equal to number of layers.\n";

      throw std::logic_error(buffer.str().c_str());
   }

   // MultilayerPerceptron outputs layers delta

   const unsigned int output_layers_delta_size = output_layers_delta.size();

   if(output_layers_delta_size != layers_number)
   {
      buffer << "OpenNN Exception: MultilayerPerceptron class.\n"
             << "Matrix< Vector< Matrix<double> > > calculate_output_interlayers_delta(@todo) method.\n"
             << "Size of multilayer_perceptron_pointer outputs layers delta must be equal to number of layers.\n";

      throw std::logic_error(buffer.str().c_str());
   }

   #endif

   const Vector< Matrix<double> > layers_synaptic_weights = arrange_layers_synaptic_weights();

   const Vector< Vector<double> >& layers_activation_derivative = second_order_forward_propagation[1];
   const Vector< Vector<double> >& layers_activation_second_derivative = second_order_forward_propagation[2];

   // The Delta form consists of a square matrix of size the number of layers

   Matrix< Vector< Matrix<double> > > output_interlayers_Delta(layers_number, layers_number);

   // Each Delta element is a vector of size the number of outputs

   for(unsigned int i = 0; i < layers_number; i++)
   {
      for(unsigned int j = 0; j < layers_number; j++)
      {
	     output_interlayers_Delta[i][j].set(outputs_number);
      }
   }

   // The subelements are matrices with rows and columns numbers the sizes of the first and the second layers

   for(unsigned int i = 0; i < layers_number; i++)
   {
      for(unsigned int j = 0; j < layers_number; j++)
      {
	     for(unsigned int k = 0; k < outputs_number; k++)
		 {
		    output_interlayers_Delta[i][j][k].set(layers_size[i], layers_size[j]);
		 }
      }
   }

   if(layers_number > 0)
   {
      // Output-outputs layer (OK)

      output_interlayers_Delta[layers_number-1][layers_number-1] = layers[layers_number-1].arrange_activation_Hessian_form(layers_activation_second_derivative[layers_number-1]);      

      // Rest of hidden layers

      double sum_1;
      double sum_2;

      for(unsigned int L = layers_number-1; L != 0; L--) 
      {   
         for(unsigned int M = layers_number-1; M >= L; M--) 
         {   
            if(!(L == layers_number-1 && M == layers_number-1))
            {
              for(unsigned int i = 0; i < outputs_number; i++)
	           {
                  for(unsigned int j = 0; j < layers_size[L]; j++)
		          {	 
                     sum_1 = 0.0;
         
                     for(unsigned int l = 0; l < layers_size[L+1]; l++)
			         {
			            sum_1 += output_layers_delta[L+1][i][l]*layers_synaptic_weights[L+1][l][j];
			         }

                     for(unsigned int k = 0; k < layers_size[M]; k++)
		             {
   			            sum_2 = 0.0;

                        for(unsigned int l = 0; l < layers_size[L+1]; l++)
			            {
			               sum_2 += output_interlayers_Delta[L+1][M][i][l][k]*layers_synaptic_weights[L+1][l][j];		   
			            }

                        output_interlayers_Delta[L][M][i][j][k] 
			            = layers_activation_second_derivative[L][j]*interlayers_combination_combination_Jacobian[L][M][j][k]*sum_1
			            + layers_activation_derivative[L][j]*sum_2;

                        output_interlayers_Delta[M][L][i][k][j] = output_interlayers_Delta[L][M][i][j][k];    
		             }		 
		          }
	           }
			}
		 }		 
	  }
   }

   return(output_interlayers_Delta);
}


// Matrix<double> calculate_parameters_Jacobian(const Vector<double>&) const method

/// This method calculates the parameters matrix of the multilayer perceptron for an inputs vector. 
/// The elements of that matrix are the partial derivatives of the outputs with respect to the multilayer_perceptron_pointer parameters. 

Matrix<double> MultilayerPerceptron::calculate_parameters_Jacobian(const Vector<double>& inputs, const Vector<double>&) const
{   
   #ifdef _DEBUG 

   const unsigned int inputs_number = count_inputs_number();
   const unsigned int size = inputs.size();

   if(size != inputs_number)
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: MultilayerPerceptron class.\n"
             << "void calculate_parameters_Jacobian(Vector<double>&) const method.\n"
             << "Size must be equal to number of inputs.\n";

	  throw std::logic_error(buffer.str());
   }

   #endif

   // Neural network stuff

   const unsigned int layers_number = count_layers_number();
   const Vector<unsigned int> layers_size = arrange_layers_perceptrons_numbers();

   // Calculate required quantities

   const Vector< Vector< Vector<double> > > first_order_forward_propagation = calculate_first_order_forward_propagation(inputs);
   const Vector< Vector<double> >& layers_activation = first_order_forward_propagation[0];
   const Vector< Vector<double> >& layers_activation_derivative = first_order_forward_propagation[1];

   const Vector< Vector<double> > layers_inputs = arrange_layers_input(inputs, layers_activation);

   const Vector< Matrix<double> > layers_combination_parameters_Jacobian = calculate_layers_combination_parameters_Jacobian(layers_inputs);

   const Vector< Matrix<double> > output_layers_delta = calculate_output_layers_delta(layers_activation_derivative);

   Matrix<double> parameters_Jacobian = output_layers_delta[0].dot(layers_combination_parameters_Jacobian[0]);

   for(unsigned int i = 1; i < layers_number; i++)
   {
      parameters_Jacobian = parameters_Jacobian.get_assembly_columns(output_layers_delta[i].dot(layers_combination_parameters_Jacobian[i]));
   }

   return(parameters_Jacobian);
}


// Vector< Matrix<double> > calculate_parameters_Hessian_form(const Vector<double>&) const

/// This method returns the second partial derivatives of the outputs from the multilayer_perceptron_pointer with respect to the multilayer_perceptron_pointer parameters. 
/// @todo

Vector< Matrix<double> > MultilayerPerceptron::calculate_parameters_Hessian_form(const Vector<double>& inputs, const Vector<double>&) const
{
   // Neural network stuff

   const unsigned int layers_number = count_layers_number();
   const Vector<unsigned int> layers_size = arrange_layers_perceptrons_numbers();
   const unsigned int outputs_number = count_outputs_number();

   const unsigned int parameters_number = count_parameters_number();

   const Vector< Matrix<double> > layers_synaptic_weights = arrange_layers_synaptic_weights();

   // Calculate required quantities

   const Vector< Vector< Vector<double> > > second_order_forward_propagation = calculate_second_order_forward_propagation(inputs);

   const Vector< Vector<double> >& layers_activation = second_order_forward_propagation[0];
   const Vector< Vector<double> >& layers_activation_derivative = second_order_forward_propagation[1];
//   const Vector< Vector<double> >& layers_activation_second_derivative = second_order_forward_propagation[2];

   const Vector< Vector<double> > layers_inputs = arrange_layers_input(inputs, layers_activation);

   const Vector< Vector< Vector<double> > > perceptrons_combination_parameters_gradient = calculate_perceptrons_combination_parameters_gradient(layers_inputs);

   const Matrix< Matrix<double> > interlayers_combination_combination_Jacobian = calculate_interlayers_combination_combination_Jacobian(inputs);

   const Vector< Matrix<double> > output_layers_delta = calculate_output_layers_delta(layers_activation_derivative);

   const Matrix< Vector< Matrix<double> > > output_interlayers_Delta
   = calculate_output_interlayers_Delta(second_order_forward_propagation, interlayers_combination_combination_Jacobian, output_layers_delta);

   // Size multilayer_perceptron_pointer parameters Hessian form 

   Vector< Matrix<double> > parameters_Hessian_form(outputs_number);

   for(unsigned int i = 0; i < outputs_number; i++)
   {
      parameters_Hessian_form[i].set(parameters_number, parameters_number);
   }

   // Calculate Hessian form elements

   if(layers_number > 0)
   {
      Matrix<unsigned int> parameters_indices = arrange_parameters_indices();

      unsigned int layer_j;
      unsigned int neuron_j;
      unsigned int parameter_j;

      unsigned int layer_k;
      unsigned int neuron_k;
      unsigned int parameter_k;

      Vector<double> neuron_j_combination_parameters_gradient;
      Vector<double> neuron_k_combination_parameters_gradient;

      for(unsigned int i = 0; i < outputs_number; i++)
      {
         for(unsigned int j = 0; j < parameters_number; j++)
         {
            layer_j = parameters_indices[j][0];
            neuron_j = parameters_indices[j][1];
            parameter_j = parameters_indices[j][2];

	        for(unsigned int k = j; k < parameters_number; k++)
		    {
               layer_k = parameters_indices[k][0];
               neuron_k = parameters_indices[k][1];
               parameter_k = parameters_indices[k][2];
			   		   
			   parameters_Hessian_form[i][j][k] =
			   output_interlayers_Delta[layer_j][layer_k][i][neuron_j][neuron_k]
			   *perceptrons_combination_parameters_gradient[layer_j][neuron_j][parameter_j]
			   *perceptrons_combination_parameters_gradient[layer_k][neuron_k][parameter_k]
			   ;

			   if(layer_j != 0 && parameter_j != 0) // Neither the first layer nor parameter is a bias
			   {
   			      //parameters_Hessian_form[i][j][k] +=
			      //output_layers_delta[layer_j][i][neuron_j]
				  //*layers_activation_derivative[layer_j-1][parameter_j-1]
				  //*interlayers_combination_combination_Jacobian[layer_j-1][layer_k][parameter_j-1][neuron_k]
   			      //*perceptrons_combination_parameters_gradient[layer_k][neuron_k][parameter_k]
    			  ;				  
			   }

			   if(layer_k != 0 && parameter_k != 0)
			   {
			     parameters_Hessian_form[i][j][k] +=
			     output_layers_delta[layer_k][i][neuron_k]
			     *layers_activation_derivative[layer_k-1][parameter_k-1]
			     *interlayers_combination_combination_Jacobian[layer_k-1][layer_j][parameter_k-1][neuron_j]
   			     *perceptrons_combination_parameters_gradient[layer_j][neuron_j][parameter_j]
			     ;		   
			   }  

			   parameters_Hessian_form[i][k][j] = parameters_Hessian_form[i][j][k];
            }
	     }
      }
   }

   return(parameters_Hessian_form);
}


// Vector< Vector<double> > arrange_layers_input(const Vector<double>&, const Vector< Vector<double> >&) const method

/// This method returns the layers inputs from the multilayer perceptron inputs and the layers outputs. 
/// The format is a vector of subvectors. 
/// The size of the vector is the number of layers.
/// The size of each subvector is the number of inputs to the corresponding layer.
/// @param inputs Input values to the multilayer perceptron.
/// @param layers_activation Output values from each layer. 

Vector< Vector<double> > MultilayerPerceptron::arrange_layers_input(const Vector<double>& inputs, const Vector< Vector<double> >& layers_activation) const
{
   // Neural network stuff

   const unsigned int layers_number = count_layers_number();

   #ifdef _DEBUG 

   const unsigned int inputs_number = count_inputs_number();
   const unsigned int inputs_size = inputs.size();

   if(inputs_size != inputs_number)
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: MultilayerPerceptron class.\n"
             << "Vector< Vector<double> > arrange_layers_input(const Vector<double>&, const Vector< Vector<double> >&) const method.\n"
             << "Size must be equal to number of inputs.\n";

	  throw std::logic_error(buffer.str());
   }

   const unsigned int layers_activation_size = layers_activation.size();

   if(layers_activation_size != layers_number)
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: MultilayerPerceptron class.\n"
             << "Vector< Vector<double> > arrange_layers_input(const Vector<double>&, const Vector< Vector<double> >&) const method.\n"
             << "Size must be equal to number of inputs.\n";

	  throw std::logic_error(buffer.str());
   }

   #endif

   // Arrange layers inputs

   Vector< Vector<double> > layers_inputs(layers_number);

   if(layers_number != 0)
   {
      layers_inputs[0] = inputs;

      for(unsigned int j = 1; j < layers_number; j++)
      {
         layers_inputs[j] = layers_activation[j-1];
      }
   }

   return(layers_inputs);
}

// Vector< Vector<double> > calculate_layers_input(const Vector<double>&) const method

/// This method returns a vector of vectors, where each element contains the input values of a layer in response 
/// to an inputs to the multilayer perceptron 
/// @param inputs Input values to the multilayer perceptron

Vector< Vector<double> > MultilayerPerceptron::calculate_layers_input(const Vector<double>& inputs) const
{
   const unsigned int layers_number = count_layers_number();

   Vector< Vector<double> > layers_inputs(layers_number);

   layers_inputs[0] = inputs;

   for(unsigned int i = 1; i < layers_number; i++)
   {
      layers_inputs[i] = layers[i-1].calculate_outputs(layers_inputs[i-1]);
   }

   return(layers_inputs);
}


// Vector< Vector<double> > calculate_layers_combination(const Vector<double>&) const method

/// This method returns a vector of vectors, where each element contains the combination values of a layer in response 
/// to an inputs to the multilayer perceptron 
/// @param inputs Input values to the multilayer perceptron

Vector< Vector<double> > MultilayerPerceptron::calculate_layers_combination(const Vector<double>& inputs) const
{
   // Control sentence (if debug)

   #ifdef _DEBUG 

   const unsigned int inputs_size = inputs.size();

   const unsigned int inputs_number = count_inputs_number();

   if(inputs_size != inputs_number)
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: MultilayerPerceptron class.\n"
             << "Vector< Vector<double> > calculate_layers_combination(const Vector<double>&) const method.\n"
             << "Size must be equal to number of inputs.\n";

	  throw std::logic_error(buffer.str());
   }

   #endif

   const unsigned int layers_number = count_layers_number();

   Vector< Vector<double> > layers_combination(layers_number);

   if(layers_number > 0)
   {
      Vector< Vector<double> > layers_activation(layers_number);

      layers_combination[0] = layers[0].calculate_combination(inputs);

      layers_activation[0] = layers[0].calculate_activation(layers_combination[0]);

      for(unsigned int i = 1; i < layers_number; i++)
      {
         layers_combination[i] = layers[i].calculate_combination(layers_activation[i-1]);

         layers_activation[i] = layers[i].calculate_activation(layers_combination[i]);
      }
   }

   return(layers_combination);
}


// Vector< Matrix<double> > calculate_layers_combination_Jacobian(const Vector<double>&) const method

/// This method returns the Jacobians of all the layer combination functions. 
/// The format of this quantity is a vector of matrices. 

Vector< Matrix<double> > MultilayerPerceptron::calculate_layers_combination_Jacobian(const Vector<double>& inputs) const
{
   const unsigned int layers_number = count_layers_number();

   Vector< Matrix<double> > layers_combination_Jacobian(layers_number-2);

   Vector< Vector<double> > layers_output(layers_number);

   Vector< Matrix<double> > layers_Jacobian(layers_number);

   // Hidden and outputs layers Jacobian

   layers_output[0] = layers[0].calculate_outputs(inputs);   

   layers_combination_Jacobian[0] = layers[0].calculate_combination_Jacobian(inputs);

   for(unsigned int i = 1; i < layers_number; i++)
   {
      layers_output[i] = layers[i].calculate_outputs(layers_output[i-1]);         

      layers_Jacobian[i] = layers[i].calculate_Jacobian(layers_output[i-1]);
   }

   Matrix<double> output_layer_Jacobian = layers[layers_number-1].calculate_Jacobian(layers_output[layers_number-1]);

   // Calculate forward propagation Jacobian

   layers_Jacobian[layers_number] = output_layer_Jacobian;

   for(int i = layers_number-1; i > -1; i--)
   {
      layers_Jacobian[layers_number] = layers_Jacobian[layers_number].dot(layers_Jacobian[i]);   
   }

   for(int i = layers_number-1; i > -1; i--)
   {
      layers_Jacobian[i] = layers_Jacobian[i];

	  for(int j = i-1; j > -1; j--)
	  {
         layers_Jacobian[i] = layers_Jacobian[i].dot(layers_Jacobian[j]);     
	  }      
   }

   return(layers_combination_Jacobian);
}


// Vector< Matrix<double> > calculate_layers_combination_parameters_Jacobian(const Vector< Vector<double> >&) const method

/// This method returns the Jacobian matix of the combination function for each layer. 
/// The format is a vector of matrices. 
/// The size of the vector is the number of layers.
/// @param layers_inputs Input values for each layer.  

Vector< Matrix<double> > MultilayerPerceptron::calculate_layers_combination_parameters_Jacobian(const Vector< Vector<double> >& layers_inputs) const
{
   const unsigned int layers_number = count_layers_number();

   Vector< Matrix<double> > layers_combination_parameters_Jacobian(layers_number);

   const Vector<double> dummy;

   for(unsigned int i = 0; i < layers_number; i++)
   {
      layers_combination_parameters_Jacobian[i] = layers[i].calculate_combination_parameters_Jacobian(layers_inputs[i], dummy);         
   }

   return(layers_combination_parameters_Jacobian);
}


// Vector< Vector< Vector<double> > > calculate_perceptrons_combination_parameters_gradient(const Vector< Vector<double> >&) const method

/// This method returns the combination parameters gradient of all neurons in the network architecture. 
/// The format is a vector of subvectors of subsubvectors.
/// The size of the vector is the number of layers.
/// The size of each subvector is the number of perceptrons in the layer. 
/// The size of each subsubvector is the number of inputs to the perceptron. 
/// That quantities will be useful for calculating some multilayer perceptron derivatives. 
/// @param layers_inputs Vector of subvectors with the inputs to each layer. 

Vector< Vector< Vector<double> > > MultilayerPerceptron::calculate_perceptrons_combination_parameters_gradient(const Vector< Vector<double> >& layers_inputs) const
{
   const unsigned int layers_number = count_layers_number();

   // Control sentence (if debug)

   #ifdef _DEBUG 

   const unsigned int layers_input_size = layers_inputs.size();

   if(layers_input_size != layers_number)
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: MultilayerPerceptron class.\n"
             << "Vector< Vector< Vector<double> > > calculate_perceptrons_combination_parameters_gradient(const Vector< Vector<double> >&) const method.\n"
             << "Size must be equal to number of layers.\n";

	  throw std::logic_error(buffer.str());
   }

   #endif

   const Vector<unsigned int> layers_inputs_number = get_layers_inputs_number();

   #ifdef _DEBUG 

   for(unsigned int i = 0; i < layers_number; i++)
   {
      if(layers_inputs[i].size() != layers_inputs_number[i])
      {
         std::ostringstream buffer;

         buffer << "OpenNN Exception: MultilayerPerceptron class.\n"
                << "Vector< Vector< Vector<double> > > calculate_perceptrons_combination_parameters_gradient(const Vector< Vector<double> >&) const method.\n"
                << "Size of inputs to layer " << i << " must be equal to size of that layer.\n";

   	     throw std::logic_error(buffer.str());
      }
   }

   #endif

   const Vector<unsigned int> layers_size = arrange_layers_perceptrons_numbers();

   Vector< Vector< Vector<double> > > perceptrons_combination_parameters_gradient(layers_number);

   for(unsigned int i = 0; i < layers_number; i++)
   {
      perceptrons_combination_parameters_gradient[i].set(layers_size[i]);   

      for(unsigned int j = 0; j < layers_size[i]; j++)
      {
		  const Perceptron& perceptron = layers[i].get_perceptron(j);

          perceptrons_combination_parameters_gradient[i][j] = perceptron.calculate_combination_parameters_gradient(layers_inputs[i]);   
      }
   }

   return(perceptrons_combination_parameters_gradient);
}


// Vector< Vector<double> > calculate_layers_activation(const Vector<double>&) const method

/// This method returns a vector of vectors, where each element contains the activation values of a layer in response 
/// to an inputs to the multilayer perceptron 
/// @param inputs Input values to the multilayer perceptron

Vector< Vector<double> > MultilayerPerceptron::calculate_layers_activation(const Vector<double>& inputs) const
{
   // Control sentence (if debug)

   #ifdef _DEBUG 

   const unsigned int inputs_size = inputs.size();

   const unsigned int inputs_number = count_inputs_number();

   if(inputs_size != inputs_number)
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: MultilayerPerceptron class.\n"
             << "Vector< Vector<double> > calculate_layers_activation(const Vector<double>&) const method.\n"
             << "Size must be equal to number of inputs.\n";

	  throw std::logic_error(buffer.str());
   }

   #endif

   const unsigned int layers_number = count_layers_number();

   Vector< Vector<double> > layers_combination(layers_number);
   Vector< Vector<double> > layers_activation(layers_number);

   layers_combination[0] = layers[0].calculate_combination(inputs);
   layers_activation[0] = layers[0].calculate_activation(layers_combination[0]);

   for(unsigned int i = 1; i < layers_number; i++)
   {
      layers_combination[i] = layers[i].calculate_combination(layers_activation[i-1]);
      layers_activation[i] = layers[i].calculate_activation(layers_combination[i]);
   }

   return(layers_activation);
}


// Vector< Vector<double> > calculate_layers_activation_derivative(const Vector<double>&) const method

/// This method returns a vector of vectors, where each element contains the activation derivatives of a layer in response 
/// to an inputs to the multilayer perceptron 
/// @param inputs Input values to the multilayer perceptron

Vector< Vector<double> > MultilayerPerceptron::calculate_layers_activation_derivative(const Vector<double>& inputs) const
{
   // Control sentence (if debug)

   #ifdef _DEBUG 

   const unsigned int inputs_size = inputs.size();
   const unsigned int inputs_number = count_inputs_number();

   if(inputs_size != inputs_number)
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: MultilayerPerceptron class.\n"
             << "Vector< Vector<double> > calculate_layers_activation_derivative(const Vector<double>&) const method.\n"
             << "Size must be equal to number of inputs.\n";

	  throw std::logic_error(buffer.str());
   }

   #endif

   const unsigned int layers_number = count_layers_number();

   Vector< Vector<double> > layers_combination(layers_number);
   Vector< Vector<double> > layers_activation(layers_number);
   Vector< Vector<double> > layers_activation_derivative(layers_number);

   if(layers_number != 0)
   {   
      layers_combination[0] = layers[0].calculate_combination(inputs);
      layers_activation[0] = layers[0].calculate_activation(layers_combination[0]);
      layers_activation_derivative[0] = layers[0].calculate_activation_derivative(layers_combination[0]);
  
      for(unsigned int i = 1; i < layers_number; i++)
      {
         layers_combination[i] = layers[i].calculate_combination(layers_activation[i-1]);   
         layers_activation[i] = layers[i].calculate_activation(layers_combination[i]);
         layers_activation_derivative[i] = layers[i].calculate_activation_derivative(layers_combination[i]);         
      }
   }

   return(layers_activation_derivative);
}


// Vector< Vector<double> > calculate_layers_activation_second_derivative(const Vector<double>&) const method

/// This method returns a vector of vectors with the forward propagation values, their derivatives and their second derivatives. 
/// The size of the vector is equal to the number of layers.
/// The elements are the following:
/// <ul>
/// <li> Activation second derivative from layers.
/// <li> Activation second derivative from outputs layer.
/// </ul>
/// @param inputs Set of inputs to the multilayer perceptron

Vector< Vector<double> > MultilayerPerceptron::calculate_layers_activation_second_derivative(const Vector<double>& inputs) const
{
   // Control sentence (if debug)

   #ifdef _DEBUG 

   const unsigned int inputs_size = inputs.size();

   const unsigned int inputs_number = count_inputs_number();

   if(inputs_size != inputs_number)
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: MultilayerPerceptron class.\n"
             << "Vector< Vector<double> > calculate_layers_activation_second_derivative(const Vector<double>&) const method.\n"
             << "Size must be equal to number of inputs.\n";

	  throw std::logic_error(buffer.str());
   }

   #endif

   const unsigned int layers_number = count_layers_number();

   Vector< Vector<double> > layers_combination(layers_number);
   Vector< Vector<double> > layers_activation(layers_number);
   Vector< Vector<double> > layers_activation_second_derivative(layers_number);

   layers_combination[0] = layers[0].calculate_combination(inputs);
   layers_activation[0] = layers[0].calculate_activation(layers_combination[0]);
   layers_activation_second_derivative[0] = layers[0].calculate_activation_second_derivative(layers_combination[0]);
  
   for(unsigned int i = 1; i < layers_number; i++)
   {
      layers_combination[i] = layers[i].calculate_combination(layers_activation[i-1]);
   
      layers_activation[i] = layers[i].calculate_activation(layers_combination[i]);

      layers_activation_second_derivative[i] = layers[i].calculate_activation_second_derivative(layers_combination[i]);         
   }

   return(layers_activation_second_derivative);
}


// Vector< Matrix<double> > calculate_layers_Jacobian(const Vector<double>&) const method

/// This method returns the partial derivatives of the outputs from each layer with respect to the inputs to the corresponding layer, 
/// for a vector of inputs to the neural netwok. 
/// The format of this quantity is a vector of matrices. 
/// @param inputs Vector of inputs to the multilayer perceptron 

Vector< Matrix<double> > MultilayerPerceptron::calculate_layers_Jacobian(const Vector<double>& inputs) const
{
   const unsigned int layers_number = count_layers_number();

   Vector<Vector<double> > layers_output(layers_number);
   Vector< Matrix<double> > layers_Jacobian(layers_number);

   layers_output[0] = layers[0].calculate_outputs(inputs);   
   layers_Jacobian[0] = layers[0].calculate_Jacobian(inputs);

   for(unsigned int i = 1; i < layers_number; i++)
   {
      layers_output[i] = layers[i].calculate_outputs(layers_output[i-1]);         

      layers_Jacobian[i] = layers[i].calculate_Jacobian(layers_output[i-1]);
   }

   return(layers_Jacobian);
}


// Vector< Matrix<double> > calculate_layers_Jacobian(const Vector<double>&) const method

/// This method returns the second partial derivatives of the outputs from each layer with respect to the inputs to the corresponding layer, 
/// for a vector of inputs to the neural netwok. 
/// The format of this quantity is a vector of vectors of matrices. 
/// @param inputs Vector of inputs to the multilayer perceptron 

Vector< Vector< Matrix<double> > > MultilayerPerceptron::calculate_layers_Hessian_form(const Vector<double>& inputs) const
{
   const unsigned int layers_number = count_layers_number();

   Vector<Vector<double> > layers_output(layers_number);
   Vector< Vector< Matrix<double> > > layers_Hessian_form(layers_number);

   layers_output[0] = layers[0].calculate_outputs(inputs);   
   layers_Hessian_form[0] = layers[0].calculate_Hessian_form(inputs);

   for(unsigned int i = 1; i < layers_number; i++)
   {
      layers_output[i] = layers[i].calculate_outputs(layers_output[i-1]);         

      layers_Hessian_form[i] = layers[i].calculate_Hessian_form(layers_output[i-1]);
   }

   return(layers_Hessian_form);
}


// Matrix< Matrix<double> > calculate_interlayers_combination_combination_Jacobian(const Vector<double>&) const method

/// This method returns the partial derivatives of the combination of one neuron with respect to the combination of another neuron for all layers in the multilayer perceptron
/// This quantity is a Jacobian form, and it is represented as a matrix of matrices. 
/// @param inputs Vector of inputs to the multilayer perceptron 

Matrix< Matrix<double> > MultilayerPerceptron::calculate_interlayers_combination_combination_Jacobian(const Vector<double>& inputs) const  
{
   #ifdef _DEBUG 

   const unsigned int size = inputs.size();

   const unsigned int inputs_number = count_inputs_number();

   if(size != inputs_number)
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: MultilayerPerceptron class.\n"
             << "Matrix< Matrix<double> > calculate_interlayers_combination_combination_Jacobian(const Vector<double>&) const method.\n"
             << "Size of inpouts must be equal to number of inputs.\n";

	  throw std::logic_error(buffer.str());
   }

   #endif

   const Vector< Vector<double> > layers_combination = calculate_layers_combination(inputs);

   return(calculate_interlayers_combination_combination_Jacobian(layers_combination));
}


// Matrix <Matrix<double> > calculate_interlayers_combination_combination_Jacobian(const Vector< Vector<double> >&) const method

/// This method returns the partial derivatives of the combination of one neuron with respect to the combination of another neuron for all layers in the multilayer perceptron
/// This quantity is a Jacobian form, and it is represented as a matrix of matrices. 
/// @param layers_combination Vector vectors representing the combinations of all layers. 

Matrix < Matrix<double> > MultilayerPerceptron::calculate_interlayers_combination_combination_Jacobian(const Vector< Vector<double> >& layers_combination) const   
{
   const unsigned int layers_number = count_layers_number();

   #ifdef _DEBUG 

   unsigned int size;

   const Vector<unsigned int> layers_size = arrange_layers_perceptrons_numbers();

   for(unsigned int i = 0; i < layers_number; i++)
   {
      size = layers_combination[i].size();

      if(size != layers_size[i])
      {
         std::ostringstream buffer;

         buffer << "OpenNN Exception: MultilayerPerceptron class.\n"
                << "Matrix< Matrix<double> > calculate_interlayers_combination_combination_Jacobian(const Vector< Vector<double> >&) const method.\n"
                << "Size must be equal to size of layer.\n";

   	     throw std::logic_error(buffer.str());
      }
   }

   #endif

   // Calculate interlayers combination combination Jacobian

   Matrix< Matrix<double> > interlayers_combination_combination_Jacobian(layers_number, layers_number);

   for(unsigned int image_index = 0; image_index < layers_number; image_index++)
   {
      for(unsigned int domain_index = 0; domain_index < layers_number; domain_index++)
      {
         interlayers_combination_combination_Jacobian[image_index][domain_index] = calculate_interlayer_combination_combination_Jacobian(domain_index, image_index, layers_combination[domain_index]); 
      }
   }

   return(interlayers_combination_combination_Jacobian);
}


// Vector< Vector< Vector<double> > > calculate_first_order_forward_propagation(const Vector<double>&) const method

/// This method returns the first order forward propagation quantities from the multilayer perceptron for a given inputs. 
/// That quantites include the activation and the activation derivative of all layers. 
/// The format is a vector of vectors of vectors. 
/// The first index refers to the quantity (0 for the activation and 1 for the activation derivative).
/// The second index is the index of the layer. 
/// The third index is the index of the neuron. 
/// @param inputs Vector of inputs to the multilayer perceptron 

Vector< Vector< Vector<double> > > MultilayerPerceptron::calculate_first_order_forward_propagation(const Vector<double>& inputs) const
{
   // Control sentence (if debug)

   #ifdef _DEBUG 

   const unsigned int inputs_size = inputs.size();

   const unsigned int inputs_number = count_inputs_number();

   if(inputs_size != inputs_number)
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: MultilayerPerceptron class.\n"
             << "Vector< Vector< Vector<double> > > calculate_first_order_forward_propagation(const Vector<double>&) const method.\n"
             << "Size must be equal to number of inputs.\n";

	  throw std::logic_error(buffer.str());
   }

   #endif

   const unsigned int layers_number = count_layers_number();

   Vector< Vector<double> > layers_combination(layers_number);

   Vector< Vector< Vector<double> > > first_order_forward_propagation(2);

   first_order_forward_propagation[0].set(layers_number);
   first_order_forward_propagation[1].set(layers_number);

   layers_combination[0] = layers[0].calculate_combination(inputs);

   first_order_forward_propagation[0][0] = layers[0].calculate_activation(layers_combination[0]);

   first_order_forward_propagation[1][0] = layers[0].calculate_activation_derivative(layers_combination[0]);

   for(unsigned int i = 1; i < layers_number; i++)
   {
      layers_combination[i] = layers[i].calculate_combination(first_order_forward_propagation[0][i-1]);

      first_order_forward_propagation[0][i] = layers[i].calculate_activation(layers_combination[i]);

      first_order_forward_propagation[1][i] = layers[i].calculate_activation_derivative(layers_combination[i]);
   }

   return(first_order_forward_propagation);
}


// Vector< Vector< Vector<double> > > calculate_second_order_forward_propagation(const Vector<double>&) const method

/// This method returns the second order forward propagation quantities from the multilayer perceptron for a given inputs. 
/// That quantites include the activation, the activation derivative and the activation second derivative of all layers. 
/// The format is a vector of vectors of vectors. 
/// The first index refers to the activation derivative order (0 for the activation, 1 for the activation derivative and 2 for the activation second derivative).
/// The second index is the index of the layer. 
/// The third index is the index of the neuron. 
/// @param inputs Vector of inputs to the multilayer perceptron 

Vector< Vector< Vector<double> > > MultilayerPerceptron::calculate_second_order_forward_propagation(const Vector<double>& inputs) const
{
   // Control sentence (if debug)

   #ifdef _DEBUG 

   const unsigned int inputs_size = inputs.size();

   const unsigned int inputs_number = count_inputs_number();

   if(inputs_size != inputs_number)
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: MultilayerPerceptron class.\n"
             << "Vector< Vector< Vector<double> > > calculate_second_order_forward_propagation(const Vector<double>&) const method.\n"
             << "Size of multilayer_perceptron_pointer inputs must be equal to number of inputs.\n";

	  throw std::logic_error(buffer.str());
   }

   #endif

   const unsigned int layers_number = count_layers_number();

   Vector< Vector<double> > layers_combination(layers_number);

   Vector< Vector< Vector<double> > > second_order_forward_propagation(3);

   second_order_forward_propagation[0].set(layers_number);
   second_order_forward_propagation[1].set(layers_number);
   second_order_forward_propagation[2].set(layers_number);

   layers_combination[0] = layers[0].calculate_combination(inputs);

   second_order_forward_propagation[0][0] = layers[0].calculate_activation(layers_combination[0]);
  
   second_order_forward_propagation[1][0] = layers[0].calculate_activation_derivative(layers_combination[0]);

   second_order_forward_propagation[2][0] = layers[0].calculate_activation_second_derivative(layers_combination[0]);

   for(unsigned int i = 1; i < layers_number; i++)
   {
      layers_combination[i] = layers[i].calculate_combination(second_order_forward_propagation[0][i-1]);

      second_order_forward_propagation[0][i] = layers[i].calculate_activation(layers_combination[i]);

      second_order_forward_propagation[1][i] = layers[i].calculate_activation_derivative(layers_combination[i]);

      second_order_forward_propagation[2][i] = layers[i].calculate_activation_second_derivative(layers_combination[i]);
   }

   return(second_order_forward_propagation);
}


// std::string to_string(void) const method

/// This method returns a string representation of the current multilayer perceptron object. 

std::string MultilayerPerceptron::to_string(void) const
{
   std::ostringstream buffer;

   buffer << "MultilayerPerceptron\n"
          << "Architecture: " << arrange_architecture() << "\n"
          << "Layers activation function: " << write_layers_activation_function() << "\n"
          << "Parameters: " << arrange_parameters() << "\n"
          << "Display: " << display << "\n";

   return(buffer.str());
}


// TiXmlElement* to_XML(void) const method

/// This method serializes the multilayer perceptron object into a XML element of the TinyXML library. 
/// See the OpenNN manual for more information about the format of this element. 

TiXmlElement* MultilayerPerceptron::to_XML(void) const
{
   std::ostringstream buffer;

   TiXmlElement* multilayer_perceptron_element = new TiXmlElement("MultilayerPerceptron");
   multilayer_perceptron_element->SetAttribute("Version", 4);

   // Architecture
   {
      TiXmlElement* architecture_element = new TiXmlElement("Architecture");
      multilayer_perceptron_element->LinkEndChild(architecture_element);

      std::string architecture_string = arrange_architecture().to_string();

      TiXmlText* architecture_text = new TiXmlText(architecture_string.c_str());
      architecture_element->LinkEndChild(architecture_text);
   }

   // Layers activation function
   {
      TiXmlElement* layers_activation_function_element = new TiXmlElement("LayersActivationFunction");
      multilayer_perceptron_element->LinkEndChild(layers_activation_function_element);

      std::string layers_activation_function_string = write_layers_activation_function().to_string();

      TiXmlText* layers_activation_function_text = new TiXmlText(layers_activation_function_string.c_str());
      layers_activation_function_element->LinkEndChild(layers_activation_function_text);
   }

   // Parameters 
   {
      TiXmlElement* parameters_element = new TiXmlElement("Parameters");
      multilayer_perceptron_element->LinkEndChild(parameters_element);

      std::string parameters_string = arrange_parameters().to_string();

      TiXmlText* parameters_text = new TiXmlText(parameters_string.c_str());
      parameters_element->LinkEndChild(parameters_text);
   }

   // Display
   {
      TiXmlElement* display_element = new TiXmlElement("Display");
      multilayer_perceptron_element->LinkEndChild(display_element);

      buffer.str("");
      buffer << display;

      TiXmlText* display_text = new TiXmlText(buffer.str().c_str());
      display_element->LinkEndChild(display_text);
   }

   return(multilayer_perceptron_element);
}


// void from_XML(TiXmlElement*) method

/// This method deserializes a TinyXML element into this multilayer perceptron object. 
/// @param multilayer_perceptron_element Pointer to a XML element containing the member data. 

void MultilayerPerceptron::from_XML(TiXmlElement* multilayer_perceptron_element)
{
   if(multilayer_perceptron_element)
   {
      // Architecture
      {
         TiXmlElement* architecture_element = multilayer_perceptron_element->FirstChildElement("Architecture");

         if(architecture_element)
         {
            const char* architecture_text = architecture_element->GetText();

            if(architecture_text)
            {
               Vector<unsigned int> new_architecture;                  
               new_architecture.parse(architecture_text);

               try
               {
                  set(new_architecture);
               }
               catch(std::exception& e)
               {
                  std::cout << e.what() << std::endl;		 
               }
            }
         }
      }

      // Layers activation function
      {
         TiXmlElement* layers_activation_function_element = multilayer_perceptron_element->FirstChildElement("LayersActivationFunction");

         if(layers_activation_function_element)
         {      
            const char* layers_activation_function_text = layers_activation_function_element->GetText();
   
            if(layers_activation_function_text)
            {
               Vector<std::string> new_layers_activation_function;                  
               new_layers_activation_function.parse(layers_activation_function_text);

               try
               {
                  set_layers_activation_function(new_layers_activation_function);
               }
               catch(std::exception& e)
               {
                  std::cout << e.what() << std::endl;		 
               }
            }
         }
      }

      // Parameters 
      {
         TiXmlElement* parameters_element = multilayer_perceptron_element->FirstChildElement("Parameters");

         if(parameters_element)
         {
            const char* parameters_text = parameters_element->GetText();

            if(parameters_text)
            {
               Vector<double> new_parameters;                  
               new_parameters.parse(parameters_text);

               try
               {
                  set_parameters(new_parameters);
               }
               catch(std::exception& e)
               {
                  std::cout << e.what() << std::endl;		 
               }
            }
         }
      }

      // Display 
      {         
         TiXmlElement* display_element = multilayer_perceptron_element->FirstChildElement("Display");

         if(display)
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
   }
}


// std::string write_expression(const Vector<std::string>&, const Vector<std::string>&) const method

/// This method returns a string with the expression of the forward propagation process in a multilayer perceptron.
/// @todo

std::string MultilayerPerceptron::write_expression(const Vector<std::string>& inputs_name, const Vector<std::string>& outputs_name) const
{
   const unsigned int layers_number = count_layers_number();
   const Vector<unsigned int> layers_perceptrons_number = arrange_layers_perceptrons_numbers();

   std::ostringstream buffer;

   if(layers_number == 0)
   {
   }
   else if(layers_number == 1)
   {
      buffer << layers[0].write_expression(inputs_name, outputs_name) << "\n";        
   }
   else
   {
      Vector< Vector<std::string> > layers_outputs_name(layers_number);

      for(unsigned int i = 0; i < layers_number; i++)
      {
         layers_outputs_name[i].set(layers_perceptrons_number[i]);

         for(unsigned int j = 0; j < layers_perceptrons_number[i]; j++)
         {
            std::ostringstream new_buffer;
            new_buffer << "y_" << i+1 << "_" << j+1;
            layers_outputs_name[i][j] = new_buffer.str();
         }
      }

      buffer << layers[0].write_expression(inputs_name, layers_outputs_name[0]);        
   
      for(unsigned int i = 1; i < layers_number-1; i++)
      {
         buffer << layers[i].write_expression(layers_outputs_name[i-1], layers_outputs_name[i]);      
      }

      buffer << layers[layers_number-1].write_expression(layers_outputs_name[layers_number-2], outputs_name); 
   }

   return(buffer.str());
}

}

// OpenNN: Open Neural MultilayerPerceptrons Library.
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
