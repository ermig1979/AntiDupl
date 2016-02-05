/****************************************************************************************************************/
/*                                                                                                              */
/*   OpenNN: Open Neural Networks Library                                                                       */
/*   www.opennn.cimne.com                                                                                       */
/*                                                                                                              */
/*   N E U R A L   N E T W O R K   C L A S S                                                                    */
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

#include "neural_network.h"

// TinyXml includes

#include "../../tinyxml/tinyxml.h"

namespace OpenNN
{

// DEFAULT CONSTRUCTOR

/// Default constructor. 
/// It creates an empty neural network object.
/// All pointers in the object are initialized to NULL. 
/// The rest of members are initialized to their default values.

NeuralNetwork::NeuralNetwork(void)
 : multilayer_perceptron_pointer(NULL)
 , scaling_layer_pointer(NULL)
 , unscaling_layer_pointer(NULL)
 , bounding_layer_pointer(NULL)
 , probabilistic_layer_pointer(NULL)
 , conditions_layer_pointer(NULL)
 , inputs_outputs_information_pointer(NULL)
 , independent_parameters_pointer(NULL)
{
   set_default();
}


// MULTILAYER PERCEPTRON CONSTRUCTOR

/// Multilayer Perceptron constructor. 
/// It creates a neural network object from a given multilayer perceptron. 
/// The rest of pointers are initialized to NULL. 
/// This constructor also initializes the rest of class members to their default values.

NeuralNetwork::NeuralNetwork(const MultilayerPerceptron& new_multilayer_perceptron)
 : multilayer_perceptron_pointer(NULL)
 , scaling_layer_pointer(NULL)
 , unscaling_layer_pointer(NULL)
 , bounding_layer_pointer(NULL)
 , probabilistic_layer_pointer(NULL)
 , conditions_layer_pointer(NULL)
 , inputs_outputs_information_pointer(NULL)
 , independent_parameters_pointer(NULL)
{
   multilayer_perceptron_pointer = new MultilayerPerceptron(new_multilayer_perceptron);

   set_default();
}


// MULTILAYER PERCEPTRON ARCHITECTURE CONSTRUCTOR

/// Multilayer perceptron architecture constructor. 
/// It creates a neural network object with a multilayer perceptron given by its architecture. 
/// The rest of pointers are initialized to NULL.  
/// This constructor also initializes the rest of class members to their default values.
/// @param new_multilayer_perceptron_architecture Vector with the number of inputs and the numbers of perceptrons in each layer. 
/// The size of this vector must be equal to one plus the number of layers.

NeuralNetwork::NeuralNetwork(const Vector<unsigned int>& new_multilayer_perceptron_architecture)
 : multilayer_perceptron_pointer(NULL)
 , scaling_layer_pointer(NULL)
 , unscaling_layer_pointer(NULL)
 , bounding_layer_pointer(NULL)
 , probabilistic_layer_pointer(NULL)
 , conditions_layer_pointer(NULL)
 , inputs_outputs_information_pointer(NULL)
 , independent_parameters_pointer(NULL)
{
   multilayer_perceptron_pointer = new MultilayerPerceptron(new_multilayer_perceptron_architecture);

   set(new_multilayer_perceptron_architecture);
}


// ONE LAYER CONSTRUCTOR

/// One layer constructor. 
/// It creates a one-layer perceptron object. 
/// The number of independent parameters is set to zero.  
/// The multilayer perceptron parameters are initialized at random. 
/// @param new_inputs_number Number of inputs in the layer.
/// @param new_perceptrons_number Number of perceptrons in the layer.

NeuralNetwork::NeuralNetwork(const unsigned int& new_inputs_number, const unsigned int& new_perceptrons_number)
 : multilayer_perceptron_pointer(NULL)
 , scaling_layer_pointer(NULL)
 , unscaling_layer_pointer(NULL)
 , bounding_layer_pointer(NULL)
 , probabilistic_layer_pointer(NULL)
 , conditions_layer_pointer(NULL)
 , inputs_outputs_information_pointer(NULL)
 , independent_parameters_pointer(NULL)
{
   multilayer_perceptron_pointer = new MultilayerPerceptron(new_inputs_number, new_perceptrons_number);

   set_default();
}


// TWO LAYERS CONSTRUCTOR

/// Two layers constructor. 
/// It creates a neural network object with a two layers perceptron. 
/// The rest of pointers of this object are initialized to NULL. 
/// The other members are initialized to their default values. 
/// @param new_inputs_number Number of inputs in the multilayer perceptron
/// @param new_hidden_perceptrons_number Number of neurons in the hidden layer of the multilayer perceptron
/// @param new_output_perceptrons_number Number of outputs neurons.

NeuralNetwork::NeuralNetwork(const unsigned int& new_inputs_number, const unsigned int& new_hidden_perceptrons_number, const unsigned int& new_output_perceptrons_number)
 : multilayer_perceptron_pointer(NULL)
 , scaling_layer_pointer(NULL)
 , unscaling_layer_pointer(NULL)
 , bounding_layer_pointer(NULL)
 , probabilistic_layer_pointer(NULL)
 , conditions_layer_pointer(NULL)
 , inputs_outputs_information_pointer(NULL)
 , independent_parameters_pointer(NULL)
{
   multilayer_perceptron_pointer = new MultilayerPerceptron(new_inputs_number, new_hidden_perceptrons_number, new_output_perceptrons_number);

   set_default();
}


// INDEPENDENT PARAMETERS CONSTRUCTOR

/// Independent parameters constructor. 
/// It creates a neural network with only independent parameters.
/// The independent parameters are initialized at random. 
/// @param new_independent_parameters_number Number of independent parameters associated to the multilayer perceptron

NeuralNetwork::NeuralNetwork(const unsigned int& new_independent_parameters_number)
 : multilayer_perceptron_pointer(NULL)
 , scaling_layer_pointer(NULL)
 , unscaling_layer_pointer(NULL)
 , bounding_layer_pointer(NULL)
 , probabilistic_layer_pointer(NULL)
 , conditions_layer_pointer(NULL)
 , inputs_outputs_information_pointer(NULL)
 , independent_parameters_pointer(NULL)
{
   independent_parameters_pointer = new IndependentParameters(new_independent_parameters_number);

   set_default();
}


// FILE CONSTRUCTOR

/// File constructor. 
/// It creates a neural network object by loading its members from an XML-type file.
/// Please be careful with the format of that file, which is specified in the OpenNN manual.
/// @param filename Name of neural network file.

NeuralNetwork::NeuralNetwork(const std::string& filename)
 : multilayer_perceptron_pointer(NULL)
 , scaling_layer_pointer(NULL)
 , unscaling_layer_pointer(NULL)
 , bounding_layer_pointer(NULL)
 , probabilistic_layer_pointer(NULL)
 , conditions_layer_pointer(NULL)
 , inputs_outputs_information_pointer(NULL)
 , independent_parameters_pointer(NULL)
{
   load(filename);
}


// COPY CONSTRUCTOR

/// Copy constructor. 
/// It creates a copy of an existing neural network object. 
/// @param other_neural_network Neural network object to be copied.

NeuralNetwork::NeuralNetwork(const NeuralNetwork& other_neural_network)
 : multilayer_perceptron_pointer(NULL)
 , scaling_layer_pointer(NULL)
 , unscaling_layer_pointer(NULL)
 , bounding_layer_pointer(NULL)
 , probabilistic_layer_pointer(NULL)
 , conditions_layer_pointer(NULL)
 , inputs_outputs_information_pointer(NULL)
 , independent_parameters_pointer(NULL)
{
   set(other_neural_network);
}


// DESTRUCTOR

/// Destructor.

NeuralNetwork::~NeuralNetwork(void)
{
   delete multilayer_perceptron_pointer;
   delete scaling_layer_pointer;
   delete unscaling_layer_pointer;
   delete bounding_layer_pointer;
   delete probabilistic_layer_pointer;
   delete conditions_layer_pointer;
   delete inputs_outputs_information_pointer;
   delete independent_parameters_pointer;
}


// ASSIGNMENT OPERATOR

/// Assignment operator. 
/// It assigns to this object the members of an existing neural network object.
/// @param other_neural_network Neural network object to be assigned.

NeuralNetwork& NeuralNetwork::operator = (const NeuralNetwork& other_neural_network)
{
   if(this != &other_neural_network) 
   {
      delete_pointers();

      multilayer_perceptron_pointer = new MultilayerPerceptron(*other_neural_network.multilayer_perceptron_pointer);

      scaling_layer_pointer = new ScalingLayer(*other_neural_network.scaling_layer_pointer);

      unscaling_layer_pointer = new UnscalingLayer(*other_neural_network.unscaling_layer_pointer);

      bounding_layer_pointer = new BoundingLayer(*other_neural_network.bounding_layer_pointer);

      probabilistic_layer_pointer = new ProbabilisticLayer(*other_neural_network.probabilistic_layer_pointer);

      conditions_layer_pointer = new ConditionsLayer(*other_neural_network.conditions_layer_pointer);

      independent_parameters_pointer = new IndependentParameters(*other_neural_network.independent_parameters_pointer);

      multilayer_perceptron_flag = other_neural_network.multilayer_perceptron_flag;

      scaling_layer_flag = other_neural_network.scaling_layer_flag;

      unscaling_layer_flag = other_neural_network.unscaling_layer_flag;

      bounding_layer_flag = other_neural_network.bounding_layer_flag;

      probabilistic_layer_flag = other_neural_network.probabilistic_layer_flag;

      conditions_layer_flag = other_neural_network.conditions_layer_flag;

      display = other_neural_network.display;
   }

   return(*this);
}


// EQUAL TO OPERATOR

/// Equal to operator. 
/// @param other_neural_network Neural network object to be assigned.

bool NeuralNetwork::operator == (const NeuralNetwork& other_neural_network) const
{
   if(*multilayer_perceptron_pointer == *other_neural_network.multilayer_perceptron_pointer
   && *scaling_layer_pointer == *other_neural_network.scaling_layer_pointer
   && *unscaling_layer_pointer == *other_neural_network.unscaling_layer_pointer
   && *bounding_layer_pointer == *other_neural_network.bounding_layer_pointer
   && *probabilistic_layer_pointer == *other_neural_network.probabilistic_layer_pointer
   && *conditions_layer_pointer == *other_neural_network.conditions_layer_pointer
   && *independent_parameters_pointer == *other_neural_network.independent_parameters_pointer
   && multilayer_perceptron_flag == other_neural_network.multilayer_perceptron_flag
   && scaling_layer_flag == other_neural_network.scaling_layer_flag
   && unscaling_layer_flag == other_neural_network.unscaling_layer_flag
   && bounding_layer_flag == other_neural_network.bounding_layer_flag
   && probabilistic_layer_flag == other_neural_network.probabilistic_layer_flag
   && conditions_layer_flag == other_neural_network.conditions_layer_flag
   &&  display == other_neural_network.display)
   {
      return(true);
   }
   else
   {
      return(false);
   }
}


// METHODS

// MultilayerPerceptron* get_multilayer_perceptron_pointer(void) const method

/// This method returns a pointer to the multilayer perceptron composing this neural network. 

MultilayerPerceptron* NeuralNetwork::get_multilayer_perceptron_pointer(void) const 
{
   return(multilayer_perceptron_pointer);
}


// ScalingLayer get_scaling_layer_pointer(void) const method

/// This method returns a pointer to the scaling layer composing this neural network. 

ScalingLayer* NeuralNetwork::get_scaling_layer_pointer(void) const
{
   return(scaling_layer_pointer);
}


// UnscalingLayer* get_unscaling_layer_pointer(void) const method

/// This method returns a pointer to the unscaling layer composing this neural network. 

UnscalingLayer* NeuralNetwork::get_unscaling_layer_pointer(void) const
{
   return(unscaling_layer_pointer);
}


// BoundingLayer* get_bounding_layer_pointer(void) const method

/// This method returns a pointer to the bounding layer composing this neural network. 

BoundingLayer* NeuralNetwork::get_bounding_layer_pointer(void) const
{
   return(bounding_layer_pointer);
}


// ProbabilisticLayer* get_probabilistic_layer_pointer(void) const method

/// This method returns a pointer to the probabilistic layer composing this neural network. 

ProbabilisticLayer* NeuralNetwork::get_probabilistic_layer_pointer(void) const
{
   return(probabilistic_layer_pointer);
}


// ConditionsLayer* get_conditions_layer(void) const method

/// This method returns a pointer to the conditions layer composing this neural network. 

ConditionsLayer* NeuralNetwork::get_conditions_layer_pointer(void) const
{
   return(conditions_layer_pointer);
}


// InputsOutputsInformation& get_inputs_outputs_information_pointer(void) const method

/// This method returns a pointer to the inputs outputs information object composing this neural network. 

InputsOutputsInformation* NeuralNetwork::get_inputs_outputs_information_pointer(void) const
{
   return(inputs_outputs_information_pointer);
}


// IndependentParameters* get_independent_parameters_pointer(void) const method

/// This method returns a pointer to the independent parameters object composing this neural network. 

IndependentParameters* NeuralNetwork::get_independent_parameters_pointer(void) const
{
   return(independent_parameters_pointer);
}


// const bool& get_multilayer_perceptron_flag(void) const method

/// This method returns the flag used by the neural network to apply the multilayer perceptron or not.  

const bool& NeuralNetwork::get_multilayer_perceptron_flag(void) const 
{
   return(multilayer_perceptron_flag);
}


// const bool& get_scaling_layer_flag(void) const method

/// This method returns the flag used by the neural network to apply the scaling layer or not.  

const bool& NeuralNetwork::get_scaling_layer_flag(void) const
{
   return(scaling_layer_flag);
}


// const bool& get_unscaling_layer_flag(void) const method

/// This method returns the flag used by the neural network to apply the unscaling layer or not.  

const bool& NeuralNetwork::get_unscaling_layer_flag(void) const
{
   return(unscaling_layer_flag);
}


// const bool& get_bounding_layer_flag(void) const method

/// This method returns the flag used by the neural network to apply the bounding layer or not.  

const bool& NeuralNetwork::get_bounding_layer_flag(void) const
{
   return(bounding_layer_flag);
}


// const bool& get_probabilistic_layer_flag(void) const method

/// This method returns the flag used by the neural network to apply the probabilistic layer or not.  

const bool& NeuralNetwork::get_probabilistic_layer_flag(void) const
{
   return(probabilistic_layer_flag);
}


// const bool& get_conditions_layer_flag(void) const method

/// This method returns the flag used by the neural network to apply the conditions layer or not.  

const bool& NeuralNetwork::get_conditions_layer_flag(void) const
{
   return(conditions_layer_flag);
}


// const bool& get_display(void) const method

/// This method returns true if messages from this class are to be displayed on the screen, or false if messages 
/// from this class are not to be displayed on the screen.

const bool& NeuralNetwork::get_display(void) const
{
   return(display);
}


// void set(void) method

/// This method deletes all the pointers in the neural network.
/// It also sets the rest of members to their default values. 

void NeuralNetwork::set(void)
{
   delete_pointers();

   set_default();
}


// void set(const MultilayerPerceptron&) method

/// This method deletes all the pointers in the neural network and then constructs a copy of an exisiting multilayer perceptron.
/// It also sets the rest of members to their default values. 
/// @param new_multilayer_perceptron Multilayer perceptron object to be copied. 

void NeuralNetwork::set(const MultilayerPerceptron& new_multilayer_perceptron)
{
   delete_pointers();

   multilayer_perceptron_pointer = new MultilayerPerceptron(new_multilayer_perceptron);

   set_default();
}


// void set(const Vector<unsigned int>&) method

/// This method sets a new neural network with a given multilayer perceptron architecture.
/// It also sets the rest of members to their default values. 
/// @param new_multilayer_perceptron_architecture Architecture of the multilayer perceptron. 

void NeuralNetwork::set(const Vector<unsigned int>& new_multilayer_perceptron_architecture)
{
   delete_pointers();

   multilayer_perceptron_pointer = new MultilayerPerceptron(new_multilayer_perceptron_architecture);

   set_default();
}


// void set(const unsigned int&, const unsigned int&) method

/// This method sets a new multilayer_perceptron_pointer architecture with one layer and zero independent parameters.
/// It also sets the rest of members to their default values. 
/// @param new_inputs_number Number of inputs.
/// @param new_outputs_number Number of outputs.

void NeuralNetwork::set(const unsigned int& new_inputs_number, const unsigned int& new_outputs_number)
{
   delete_pointers();

   multilayer_perceptron_pointer = new MultilayerPerceptron(new_inputs_number, new_outputs_number);

   set_default();
}


// void set(const unsigned int&, const unsigned int&, const unsigned int&) method

/// This method sets a new multilayer_perceptron_pointer architecture with one hidden layer and zero independent parameters.
/// It also sets the rest of members to their default values. 
/// @param new_inputs_number Number of inputs.
/// @param new_hidden_neurons_number Number of neurons in the hidden layer. 
/// @param new_outputs_number Number of outputs.

void NeuralNetwork::set(const unsigned int& new_inputs_number, const unsigned int& new_hidden_neurons_number, const unsigned int& new_outputs_number)
{
   delete_pointers();

   multilayer_perceptron_pointer = new MultilayerPerceptron(new_inputs_number, new_hidden_neurons_number, new_outputs_number);

   set_default();
}


// void set(const unsigned int&) method

/// This method sets a null new multilayer_perceptron_pointer architecture a given number of independent parameters.
/// It also sets the rest of members to their default values. 
/// @param new_independent_parameters_number Number of independent_parameters_pointer.

void NeuralNetwork::set(const unsigned int& new_independent_parameters_number)
{
   delete_pointers();

   independent_parameters_pointer = new IndependentParameters(new_independent_parameters_number);

   set_default();
}


// void set(const std::string&) method

/// This method sets the neural network members by loading them from a XML file. 
/// @param filename Neural network XML filename.

void NeuralNetwork::set(const std::string& filename)
{
   delete_pointers();

   load(filename);
}


// void set(const NeuralNetwork&) method

/// This method sets the members of this neural network object with those from other neural network object. 
/// @param other_neural_network Neural network object to be copied. 

void NeuralNetwork::set(const NeuralNetwork& other_neural_network)
{
   // Pointers 

   delete_pointers();

   if(other_neural_network.multilayer_perceptron_pointer)
   {
      multilayer_perceptron_pointer = new MultilayerPerceptron(*other_neural_network.multilayer_perceptron_pointer);
   }

   if(other_neural_network.scaling_layer_pointer)
   {
      scaling_layer_pointer = new ScalingLayer(*other_neural_network.scaling_layer_pointer);
   }

   if(other_neural_network.unscaling_layer_pointer)
   {
      unscaling_layer_pointer = new UnscalingLayer(*other_neural_network.unscaling_layer_pointer);
   }

   if(other_neural_network.bounding_layer_pointer)
   {
      bounding_layer_pointer = new BoundingLayer(*other_neural_network.bounding_layer_pointer);
   }

   if(other_neural_network.probabilistic_layer_pointer)
   {
      probabilistic_layer_pointer = new ProbabilisticLayer(*other_neural_network.probabilistic_layer_pointer);
   }

   if(other_neural_network.conditions_layer_pointer)
   {
      conditions_layer_pointer = new ConditionsLayer(*other_neural_network.conditions_layer_pointer);
   }

   if(other_neural_network.inputs_outputs_information_pointer)
   {
      inputs_outputs_information_pointer = new InputsOutputsInformation(*other_neural_network.inputs_outputs_information_pointer);
   }

   if(other_neural_network.independent_parameters_pointer)
   {
      independent_parameters_pointer = new IndependentParameters(*other_neural_network.independent_parameters_pointer);
   }

   // Flags

   multilayer_perceptron_flag = other_neural_network.multilayer_perceptron_flag;

   scaling_layer_flag = other_neural_network.scaling_layer_flag;

   unscaling_layer_flag = other_neural_network.unscaling_layer_flag;

   bounding_layer_flag = other_neural_network.bounding_layer_flag;

   probabilistic_layer_flag = other_neural_network.probabilistic_layer_flag;

   conditions_layer_flag = other_neural_network.conditions_layer_flag;

   // Other

   display = other_neural_network.display;
}


// void set_default(void) method

/// This method sets those members which are not pointer to their default values.

void NeuralNetwork::set_default(void)
{
   multilayer_perceptron_flag = true;
   scaling_layer_flag = false;
   unscaling_layer_flag = false;
   bounding_layer_flag = false;
   probabilistic_layer_flag = false;
   conditions_layer_flag = false;

   display = true;
}


// void set_multilayer_perceptron_pointer(MultilayerPerceptron*) method

/// This method sets a new multilayer perceptron within the neural network. 
/// @param new_multilayer_perceptron_pointer Pointer to a multilayer perceptron object. 
/// Note that the neural network destructror will delete this pointer. 

void NeuralNetwork::set_multilayer_perceptron_pointer(MultilayerPerceptron* new_multilayer_perceptron_pointer)
{
   if(new_multilayer_perceptron_pointer != multilayer_perceptron_pointer)
   {
      delete multilayer_perceptron_pointer;

      multilayer_perceptron_pointer = new_multilayer_perceptron_pointer;
   }
}


// void set_scaling_layer_pointer(ScalingLayer*) method

/// This method sets a new scaling layer within the neural network. 
/// @param new_scaling_layer_pointer Pointer to a scaling layer object. 
/// Note that the neural network destructror will delete this pointer. 

void NeuralNetwork::set_scaling_layer_pointer(ScalingLayer* new_scaling_layer_pointer)
{
   if(new_scaling_layer_pointer != scaling_layer_pointer)
   {
      delete scaling_layer_pointer;

      scaling_layer_pointer = new_scaling_layer_pointer;
   }
}


// void set_unscaling_layer_pointer(UnscalingLayer*) method

/// This method sets a new unscaling layer within the neural network. 
/// @param new_unscaling_layer_pointer Pointer to an unscaling layer object. 
/// Note that the neural network destructror will delete this pointer. 

void NeuralNetwork::set_unscaling_layer_pointer(UnscalingLayer* new_unscaling_layer_pointer)
{
   if(new_unscaling_layer_pointer != unscaling_layer_pointer)
   {
      delete unscaling_layer_pointer;

      unscaling_layer_pointer = new_unscaling_layer_pointer;
   }
}


// void set_bounding_layer_pointer(BoundingLayer*) method

/// This method sets a new bounding layer within the neural network.
/// @param new_bounding_layer_pointer Pointer to a bounding layer object. 
/// Note that the neural network destructror will delete this pointer. 

void NeuralNetwork::set_bounding_layer_pointer(BoundingLayer* new_bounding_layer_pointer)
{
   if(new_bounding_layer_pointer != bounding_layer_pointer)
   {
      delete bounding_layer_pointer;

      bounding_layer_pointer = new_bounding_layer_pointer;
   }
}


// void set_probabilistic_layer_pointer(ProbabilisticLayer*) method

/// This method sets a new probabilistic layer within the neural network. 
/// @param new_probabilistic_layer_pointer Pointer to a probabilistic layer object. 
/// Note that the neural network destructror will delete this pointer. 

void NeuralNetwork::set_probabilistic_layer_pointer(ProbabilisticLayer* new_probabilistic_layer_pointer)
{
   if(new_probabilistic_layer_pointer != probabilistic_layer_pointer)
   {
      delete probabilistic_layer_pointer;

      probabilistic_layer_pointer = new_probabilistic_layer_pointer;
   }
}


// void set_conditions_layer_pointer(ConditionsLayer*) method

/// This method sets a new conditions layer within the neural network. 
/// @param new_conditions_layer_pointer Pointer to a conditions layer object. 
/// Note that the neural network destructror will delete this pointer. 

void NeuralNetwork::set_conditions_layer_pointer(ConditionsLayer* new_conditions_layer_pointer)
{
   if(new_conditions_layer_pointer != conditions_layer_pointer)
   {
      delete conditions_layer_pointer;

      conditions_layer_pointer = new_conditions_layer_pointer;
   }
}


// void set_inputs_outputs_information_pointer(InputsOutputsInformation*) method

/// This method sets a new inputs-outputs information object within the neural network. 
/// @param new_inputs_outputs_information_pointer Pointer to an inputs-outputs information object. 
/// Note that the neural network destructror will delete this pointer. 

void NeuralNetwork::set_inputs_outputs_information_pointer(InputsOutputsInformation* new_inputs_outputs_information_pointer)
{
   if(new_inputs_outputs_information_pointer != inputs_outputs_information_pointer)
   {
      delete inputs_outputs_information_pointer;

      inputs_outputs_information_pointer = new_inputs_outputs_information_pointer;
   }
}


// void set_independent_parameters_pointer(IndependentParameters*) method

/// This method sets new independent parameters within the neural network. 
/// @param new_independent_parameters_pointer Pointer to an independent parameters object. 
/// Note that the neural network destructror will delete this pointer. 

void NeuralNetwork::set_independent_parameters_pointer(IndependentParameters* new_independent_parameters_pointer)
{
   if(new_independent_parameters_pointer != independent_parameters_pointer)
   {
      delete independent_parameters_pointer;

      independent_parameters_pointer = new_independent_parameters_pointer;
   }
}


// void set_multilayer_perceptron_flag(const bool&) method

/// This method sets the flag for using a multilayer perceptron within the neural network or not. 
/// @param new_multilayer_perceptron_flag True if the multilayer perceptron is to be used, and false otherwise. 

void NeuralNetwork::set_multilayer_perceptron_flag(const bool& new_multilayer_perceptron_flag)
{
   multilayer_perceptron_flag = new_multilayer_perceptron_flag;
}


// void set_scaling_layer_flag(const bool&) method

/// This method sets the flag for scaling the inputs to the multilayer perceptron or not. 
/// @param new_scaling_layer_flag True if the inputs are to be scaled, and false otherwise. 

void NeuralNetwork::set_scaling_layer_flag(const bool& new_scaling_layer_flag)
{
   unsigned int inputs_number = 0;

   if(multilayer_perceptron_pointer)
   {
      inputs_number = multilayer_perceptron_pointer->count_inputs_number();   
   }

   if(!scaling_layer_pointer)
   {
      scaling_layer_pointer = new ScalingLayer(inputs_number);   
   }

   scaling_layer_flag = new_scaling_layer_flag;
}


// void set_unscaling_layer_flag(const bool&) method

/// This method sets the flag for unscaling the outputs from the multilayer perceptron or not. 
/// @param new_unscaling_layer_flag True if the outputs are to be unscaled, and false otherwise. 

void NeuralNetwork::set_unscaling_layer_flag(const bool& new_unscaling_layer_flag)
{
   unsigned int outputs_number = 0;

   if(multilayer_perceptron_pointer)
   {
      outputs_number = multilayer_perceptron_pointer->count_outputs_number();   
   }

   if(!unscaling_layer_flag)
   {
      unscaling_layer_pointer = new UnscalingLayer(outputs_number);   
   }

   unscaling_layer_flag = new_unscaling_layer_flag;
}


// void set_scaling_unscaling_layers_flag(const bool&) method

/// This method sets a unique flag for scaling the inputs and unscaling the outputs. 
/// @param new_scaling_unscaling_layers_flag True if the inputs are to be scaled and the outputs unscaled, false otherwise. 

void NeuralNetwork::set_scaling_unscaling_layers_flag(const bool& new_scaling_unscaling_layers_flag)
{
   set_scaling_layer_flag(new_scaling_unscaling_layers_flag);
   set_unscaling_layer_flag(new_scaling_unscaling_layers_flag);
}


// void set_bounding_layer_flag(const bool&) method

/// This method sets the flag for using a bounding layer for the outputs or not. 
/// @param new_bounding_layer_flag True if the outputs are to be bounded, and false otherwise. 

void NeuralNetwork::set_bounding_layer_flag(const bool& new_bounding_layer_flag)
{
   unsigned int outputs_number = 0;

   if(multilayer_perceptron_pointer)
   {
      outputs_number = multilayer_perceptron_pointer->count_outputs_number();   
   }

   if(!bounding_layer_pointer)
   {
      bounding_layer_pointer = new BoundingLayer(outputs_number);   
   }

   bounding_layer_flag = new_bounding_layer_flag;
}


// void set_probabilistic_layer_flag(const bool&) method

/// This method sets the flag for using a probabilistic layer in the neural network.
/// A probabilistic layer is used if the outputs are to be interpreted as probabilities, and false otherwise. 
/// @param new_probabilistic_layer_flag Flag for using a probabilistic layer not. 

void NeuralNetwork::set_probabilistic_layer_flag(const bool& new_probabilistic_layer_flag)
{
   unsigned int outputs_number = 0;

   if(multilayer_perceptron_pointer)
   {
      outputs_number = multilayer_perceptron_pointer->count_outputs_number();   
   }

   if(!probabilistic_layer_pointer)
   {
      probabilistic_layer_pointer = new ProbabilisticLayer(outputs_number);   
   }

   probabilistic_layer_flag = new_probabilistic_layer_flag;
}


// void set_conditions_layer_flag(const bool&) method

/// This method sets the flag for applying some boundary conditions defined by a particular and an homogeneous solutions. 
/// @param new_conditions_layer_flag True if the boundary conditions are to be applied, and false otherwise. 

void NeuralNetwork::set_conditions_layer_flag(const bool& new_conditions_layer_flag)
{
   if(!conditions_layer_pointer)
   {
      unsigned int inputs_number = 0;
      unsigned int outputs_number = 0;

      if(multilayer_perceptron_pointer)
      {
         inputs_number = multilayer_perceptron_pointer->count_inputs_number();   
         outputs_number = multilayer_perceptron_pointer->count_outputs_number();   
      }

      conditions_layer_pointer = new ConditionsLayer(inputs_number, outputs_number);   
   }

   conditions_layer_flag = new_conditions_layer_flag;
}


// unsigned int count_parameters_number(void) const method

/// This method returns the number of parameters in the multilayer perceptron 
/// The number of parameters is the sum of all the multilayer_perceptron_pointer parameters (biases and synaptic weights) and independent parameters.

unsigned int NeuralNetwork::count_parameters_number(void) const
{
   unsigned int parameters_number = 0;

   if(multilayer_perceptron_pointer)
   {
      parameters_number += multilayer_perceptron_pointer->count_parameters_number();
   }

   if(independent_parameters_pointer)
   {
      parameters_number += independent_parameters_pointer->count_parameters_number();
   }
 
   return(parameters_number);
}
 

// Vector<double> arrange_parameters(void) const method

/// This method returns the values of the parameters in the multilayer perceptron as a single vector.
/// This contains all the multilayer_perceptron_pointer parameters (biases and synaptic weights) and preprocessed independent parameters. 

Vector<double> NeuralNetwork::arrange_parameters(void) const 
{
   // Only network parameters 

   if(multilayer_perceptron_pointer && !independent_parameters_pointer)
   {
      return(multilayer_perceptron_pointer->arrange_parameters());
   }

   // Only independent parameters

   else if(!multilayer_perceptron_pointer && independent_parameters_pointer)
   {
      const bool& independent_parameters_scaling_flag = independent_parameters_pointer->get_scaling_flag();

      if(!independent_parameters_scaling_flag)
	  {
         const Vector<double> parameters = independent_parameters_pointer->get_parameters();

         return(parameters);   
	  }
	  else
	  {
         return(independent_parameters_pointer->calculate_scaled_parameters());
	  }
   }

   // Both neural and independent parameters

   else if(multilayer_perceptron_pointer && independent_parameters_pointer)
   {
      const Vector<double> network_parameters = multilayer_perceptron_pointer->arrange_parameters();

      const bool& independent_parameters_scaling_flag = independent_parameters_pointer->get_scaling_flag();

	  if(!independent_parameters_scaling_flag)
	  {  
         const Vector<double> independent_parameters = independent_parameters_pointer->get_parameters();

         return(network_parameters.get_assembly(independent_parameters));
	  }
	  else
	  {
         const Vector<double> scaled_independent_parameters = independent_parameters_pointer->calculate_scaled_parameters();

         return(network_parameters.get_assembly(scaled_independent_parameters));
	  }
   }

   // None neural neither independent parameters

   else 
   {
      const Vector<double> parameters;

      return(parameters);
   }
}


// void set_parameters(const Vector<double>&) method

/// This method sets all the parameters (multilayer_perceptron_pointer parameters and independent parameters) from a single vector.
/// @param new_parameters New set of parameter values. 

void NeuralNetwork::set_parameters(const Vector<double>& new_parameters)
{
   // Control sentence (if debug)

   #ifdef _DEBUG 

   const unsigned int size = new_parameters.size();

   const unsigned int parameters_number = count_parameters_number();

   if(size != parameters_number)
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: NeuralNetwork class.\n"
             << "void set_parameters(const Vector<double>&) method.\n"
             << "Size must be equal to number of parameters.\n";

	  throw std::logic_error(buffer.str());
   }

   #endif

   if(multilayer_perceptron_pointer && !independent_parameters_pointer)
   {// Only multilayer_perceptron_pointer parameters 

      multilayer_perceptron_pointer->set_parameters(new_parameters);
   }
   else if(!multilayer_perceptron_pointer && independent_parameters_pointer)
   {// Only independent parameters
      if(!independent_parameters_pointer->get_scaling_flag())
	  {
	     independent_parameters_pointer->set_parameters(new_parameters);   
	  }
	  else
	  {
         independent_parameters_pointer->unscale_parameters(new_parameters);
	  }
   }
   else if(multilayer_perceptron_pointer && independent_parameters_pointer)
   {// Both neural and independent parameters
      // MultilayerPerceptron parameters

      const unsigned int network_parameters_number = multilayer_perceptron_pointer->count_parameters_number();
      const unsigned int independent_parameters_number = independent_parameters_pointer->count_parameters_number();

      const Vector<double> network_parameters = new_parameters.take_out(0, network_parameters_number);

      multilayer_perceptron_pointer->set_parameters(network_parameters);

      // Independent parameters

      if(!independent_parameters_pointer->get_scaling_flag())
	  {
         const Vector<double> independent_parameters = new_parameters.take_out(network_parameters_number, independent_parameters_number);

	     independent_parameters_pointer->set_parameters(independent_parameters);   
	  }
	  else
	  {
         const Vector<double> scaled_independent_parameters = new_parameters.take_out(network_parameters_number, independent_parameters_number);

		 independent_parameters_pointer->unscale_parameters(scaled_independent_parameters);
	  }
   }
   else 
   {// None neural neither independent parameters
      return;
   }
}


// void delete_pointers(void) method

/// This method deletes all the pointers composing the neural network:
/// <ul>
/// <li> Multilayer perceptron.
/// <li> Scaling layer.
/// <li> Unscaling layer.
/// <li> Bounding layer.
/// <li> Probabilistic layer. 
/// <li> Conditions layer. 
/// <li> Inputs-outputs information. 
/// <li> Independent parameters. 
/// </ul>

void NeuralNetwork::delete_pointers(void)
{
   delete multilayer_perceptron_pointer;
   delete scaling_layer_pointer;
   delete unscaling_layer_pointer;
   delete bounding_layer_pointer;
   delete probabilistic_layer_pointer;
   delete conditions_layer_pointer;
   delete inputs_outputs_information_pointer;
   delete independent_parameters_pointer;

   multilayer_perceptron_pointer = NULL;
   scaling_layer_pointer = NULL;
   unscaling_layer_pointer = NULL;
   bounding_layer_pointer = NULL;
   probabilistic_layer_pointer = NULL;
   conditions_layer_pointer = NULL;
   inputs_outputs_information_pointer = NULL;
   independent_parameters_pointer = NULL;
}


// void construct_multilayer_perceptron(void) method

/// This method constructs an empty multilayer perceptron within the neural network. 
/// It also sets the corresponding flag to true. 

void NeuralNetwork::construct_multilayer_perceptron(void)
{
   if(!multilayer_perceptron_pointer)
   {
      multilayer_perceptron_pointer = new MultilayerPerceptron();
   }

   multilayer_perceptron_flag = true;
}


// void construct_scaling_layer(void) method

/// This method constructs a scaling layer within the neural network. 
/// The size of the scaling layer is the number of inputs in the multilayer perceptron. 
/// It also sets the corresponding flag to true. 

void NeuralNetwork::construct_scaling_layer(void)
{
   if(!scaling_layer_pointer)
   {
      unsigned int inputs_number = 0;

      if(multilayer_perceptron_pointer)
      {
         inputs_number = multilayer_perceptron_pointer->count_inputs_number();
      }

      scaling_layer_pointer = new ScalingLayer(inputs_number);
   }

   scaling_layer_flag = true;
}


// void construct_unscaling_layer(void) method

/// This method constructs an unscaling layer within the neural network. 
/// The size of the unscaling layer is the number of outputs in the multilayer perceptron. 
/// It also sets the corresponding flag to true. 

void NeuralNetwork::construct_unscaling_layer(void)
{
   if(!unscaling_layer_pointer)
   {
      unsigned int outputs_number = 0;

      if(multilayer_perceptron_pointer)
      {
         outputs_number = multilayer_perceptron_pointer->count_outputs_number();
      }

      unscaling_layer_pointer = new UnscalingLayer(outputs_number);
   }

   unscaling_layer_flag = true;
}


// void construct_bounding_layer(void) method

/// This method constructs a bounding layer within the neural network. 
/// The size of the bounding layer is the number of outputs in the multilayer perceptron. 
/// It also sets the corresponding flag to true. 

void NeuralNetwork::construct_bounding_layer(void)
{
   if(!bounding_layer_pointer)
   {
      unsigned int outputs_number = 0;

      if(multilayer_perceptron_pointer)
      {
         outputs_number = multilayer_perceptron_pointer->count_outputs_number();
      }

      bounding_layer_pointer = new BoundingLayer(outputs_number);
   }

   bounding_layer_flag = true;
}


// void construct_probabilistic_layer(void) method

/// This method constructs a probabilistic layer within the neural network. 
/// The size of the probabilistic layer is the number of outputs in the multilayer perceptron. 
/// It also sets the corresponding flag to true. 

void NeuralNetwork::construct_probabilistic_layer(void)
{
   if(!probabilistic_layer_pointer)
   {
      unsigned int outputs_number = 0;

      if(multilayer_perceptron_pointer)
      {
         outputs_number = multilayer_perceptron_pointer->count_outputs_number();
      }

      probabilistic_layer_pointer = new ProbabilisticLayer(outputs_number);
   }

   probabilistic_layer_flag = true;
}


// void construct_conditions_layer(void) method

/// This method constructs a conditions layer within the neural network. 
/// The number of external inputs in the conditions layer is the number of inputs in the multilayer perceptron. 
/// The size fo the conditions layer is the number of outputs in the multilayer perceptron. 
/// It also sets the corresponding flag to true. 

void NeuralNetwork::construct_conditions_layer(void)
{
   if(!conditions_layer_pointer)
   {
      unsigned int inputs_number = 0;
      unsigned int outputs_number = 0;

      if(multilayer_perceptron_pointer)
      {
         inputs_number = multilayer_perceptron_pointer->count_inputs_number();
         outputs_number = multilayer_perceptron_pointer->count_outputs_number();
      }

      conditions_layer_pointer = new ConditionsLayer(inputs_number, outputs_number);
   }

   conditions_layer_flag = true;
}


// void construct_inputs_outputs_information(void) method

/// This method constructs an inputs outputs information object within the neural network. 
/// The number of inputs is the number of inputs in the multilayer perceptron. 
/// The number of outputs is the number of outputs in the multilayer perceptron. 

void NeuralNetwork::construct_inputs_outputs_information(void)
{
   if(!inputs_outputs_information_pointer)
   {
      unsigned int inputs_number = 0;
      unsigned int outputs_number = 0;

      if(multilayer_perceptron_pointer)
      {
         inputs_number = multilayer_perceptron_pointer->count_inputs_number();
         outputs_number = multilayer_perceptron_pointer->count_outputs_number();
      }

      inputs_outputs_information_pointer = new InputsOutputsInformation(inputs_number, outputs_number);
   }
}


// void construct_independent_parameters(void) method

/// This method constructs an independent parameters object within the neural network. 
/// It sets the number of parameters to zero. 

void NeuralNetwork::construct_independent_parameters(void)
{
   if(!independent_parameters_pointer)
   {
      independent_parameters_pointer = new IndependentParameters();
   }
}


// void destruct_multilayer_perceptron(void) method

/// This method deletes the multilayer perceptron within the neural network. 
/// It also sets the corresponding flag to false. 

void NeuralNetwork::destruct_multilayer_perceptron(void)
{
   delete multilayer_perceptron_pointer;

   multilayer_perceptron_pointer = NULL;

   multilayer_perceptron_flag = false;
}


// void destruct_scaling_layer(void) method

/// This method deletes the scaling layer within the neural network. 
/// It also sets the corresponding flag to false. 

void NeuralNetwork::destruct_scaling_layer(void)
{
   delete scaling_layer_pointer;

   scaling_layer_pointer = NULL;

   scaling_layer_flag = false;
}


// void destruct_unscaling_layer(void) method

/// This method deletes the unscaling layer within the neural network. 
/// It also sets the corresponding flag to false. 

void NeuralNetwork::destruct_unscaling_layer(void)
{
   delete unscaling_layer_pointer;

   unscaling_layer_pointer = NULL;

   unscaling_layer_flag = false;
}


// void destruct_bounding_layer(void) method

/// This method deletes the bounding layer within the neural network. 
/// It also sets the corresponding flag to false. 

void NeuralNetwork::destruct_bounding_layer(void)
{
   delete bounding_layer_pointer;

   bounding_layer_pointer = NULL;

   bounding_layer_flag = false;
}


// void destruct_probabilistic_layer(void) method

/// This method deletes the probabilistic layer within the neural network. 
/// It also sets the corresponding flag to false. 

void NeuralNetwork::destruct_probabilistic_layer(void)
{
   delete probabilistic_layer_pointer;

   probabilistic_layer_pointer = NULL;

   probabilistic_layer_flag = false;
}


// void destruct_conditions_layer(void) method

/// This method deletes the conditions layer within the neural network. 
/// It also sets the corresponding flag to false. 

void NeuralNetwork::destruct_conditions_layer(void)
{
   delete conditions_layer_pointer;

   conditions_layer_pointer = NULL;

   conditions_layer_flag = false;
}


// void destruct_inputs_outputs_information(void) method

/// This method deletes the inputs-outputs information object within the neural network. 

void NeuralNetwork::destruct_inputs_outputs_information(void)
{
   delete inputs_outputs_information_pointer;

   inputs_outputs_information_pointer = NULL;
}


// void destruct_independent_parameters(void) method

/// This method deletes the independent parameters object within the neural network. 

void NeuralNetwork::destruct_independent_parameters(void)
{
   delete independent_parameters_pointer;

   independent_parameters_pointer = NULL;
}


// void initialize_random(void) method

/// This method initializes the neural network at random. 
/// This is useful for testing purposes. 

void NeuralNetwork::initialize_random(void)
{
   unsigned int inputs_number;
   unsigned int outputs_number;

   // Multilayer perceptron

   if(rand()%5)
   {
      if(!multilayer_perceptron_pointer)
      {
         multilayer_perceptron_pointer = new MultilayerPerceptron();
      }

      multilayer_perceptron_pointer->initialize_random();

      inputs_number = multilayer_perceptron_pointer->count_inputs_number();
      outputs_number = multilayer_perceptron_pointer->count_outputs_number();
   }
   else
   {
      inputs_number =  rand()%10 + 1;
      outputs_number =  rand()%10 + 1;   
   }

   // Scaling layer

   if(rand()%5)
   {
      if(scaling_layer_pointer)
      {
         scaling_layer_pointer = new ScalingLayer(inputs_number);
      }

      scaling_layer_pointer->initialize_random();
   }

   // Unscaling layer

   if(rand()%5)
   {
      if(unscaling_layer_pointer)
      {
         unscaling_layer_pointer = new UnscalingLayer(outputs_number);
      }

      unscaling_layer_pointer->initialize_random();
   }

   // Bounding layer 

   if(rand()%5)
   {
      if(bounding_layer_pointer)
      {
         bounding_layer_pointer = new BoundingLayer(outputs_number);
      }

      bounding_layer_pointer->initialize_random();
   }

   // Probabilistic layer

   if(rand()%5)
   {
      if(probabilistic_layer_pointer)
      {
         probabilistic_layer_pointer = new ProbabilisticLayer(outputs_number);
      }

      probabilistic_layer_pointer->initialize_random();
   }

   // Conditions layer

   if(rand()%5)
   {
      if(conditions_layer_pointer)
      {
         conditions_layer_pointer = new ConditionsLayer(inputs_number, outputs_number);
      }

      conditions_layer_pointer->initialize_random();
   }

   // Inputs outputs information

   if(rand()%5)
   {
      if(inputs_outputs_information_pointer)
      {
         inputs_outputs_information_pointer = new InputsOutputsInformation(inputs_number, outputs_number);
      }

      inputs_outputs_information_pointer->initialize_random();
   }

   // Independent parameters 

   if(rand()%5)
   {
      if(independent_parameters_pointer)
      {
         independent_parameters_pointer = new IndependentParameters();
      }

      inputs_outputs_information_pointer->initialize_random();
   }
}


// void set_display(const bool&) method

/// This method sets a new display value. 
/// If it is set to true messages from this class are to be displayed on the screen;
/// if it is set to false messages from this class are not to be displayed on the screen.
/// @param new_display Display value.

void NeuralNetwork::set_display(const bool& new_display)
{
   display = new_display;
}


// unsigned int count_layers_number(void) method

/// This method returns the number of layers in the neural network. 
/// That includes perceptron, scaling, unscaling, bounding, probabilistic or conditions layers. 

unsigned int NeuralNetwork::count_layers_number(void)
{
   unsigned int layers_number = 0; 

   if(multilayer_perceptron_pointer)
   {
      layers_number += multilayer_perceptron_pointer->count_layers_number();
   }

   if(scaling_layer_pointer)
   {
      layers_number += 1;   
   }

   if(unscaling_layer_pointer)
   {
      layers_number += 1;   
   }

   if(bounding_layer_pointer)
   {
      layers_number += 1;   
   }

   if(probabilistic_layer_pointer)
   {
      layers_number += 1;   
   }

   if(conditions_layer_pointer)
   {
      layers_number += 1;   
   }

   return(layers_number);
}


// void initialize_parameters(const double&) method

/// This method initializes all the neural and the independent parameters with a given value. 

void NeuralNetwork::initialize_parameters(const double& value)
{
   if(multilayer_perceptron_pointer)
   {
      multilayer_perceptron_pointer->initialize_parameters(value);
   }

   if(independent_parameters_pointer)
   {
      independent_parameters_pointer->initialize_parameters(value);
   }
}


// void initialize_parameters_uniform(void) method

/// This method initializes all the parameters in the newtork (biases and synaptic weiths + independent parameters) 
/// at random with values comprised between -1 and +1.

void NeuralNetwork::initialize_parameters_uniform(void)
{
   if(multilayer_perceptron_pointer)
   {
      multilayer_perceptron_pointer->initialize_parameters_uniform();  
   }

   if(independent_parameters_pointer)
   {
      independent_parameters_pointer->initialize_parameters_uniform();  
   }
}


// void initialize_parameters_uniform(const double&, const double&) method

/// This method initializes all the parameters in the newtork (biases and synaptic weiths + independent 
/// parameters) at random with values comprised between a given minimum and a given maximum values.
/// @param minimum Minimum initialization value.
/// @param maximum Maximum initialization value.

void NeuralNetwork::initialize_parameters_uniform(const double& minimum, const double& maximum)
{
   if(multilayer_perceptron_pointer)
   {
      multilayer_perceptron_pointer->initialize_parameters_uniform(minimum, maximum);  
   }

   if(independent_parameters_pointer)
   {
      independent_parameters_pointer->initialize_parameters_uniform(minimum, maximum);  
   }
}


// void initialize_parameters_uniform(const Vector<double>&, const Vector<double>&) method

/// This method initializes all the parameters in the newtork (biases and synaptic weiths + independent 
/// parameters) at random with values comprised between a different minimum and maximum numbers for each free 
/// parameter.
/// @param minimum Vector of minimum initialization values.
/// @param maximum Vector of maximum initialization values.

void NeuralNetwork::initialize_parameters_uniform(const Vector<double>& minimum, const Vector<double>& maximum)
{
   if(multilayer_perceptron_pointer)
   {
      multilayer_perceptron_pointer->initialize_parameters_uniform(minimum, maximum);  
   }

   if(independent_parameters_pointer)
   {
      independent_parameters_pointer->initialize_parameters_uniform(minimum, maximum);  
   }
}


// void initialize_parameters_uniform(const Vector< Vector<double> >&) method

/// This method initializes all the parameters in the newtork (biases and synaptic weiths + independent 
/// parameters) values comprised between a different minimum and maximum numbers for each parameter.
/// Minimum and maximum initialization values are given from a vector of two real vectors.
/// The first element must contain the minimum initialization value for each parameter.
/// The second element must contain the maximum initialization value for each parameter.
/// @param minimum_maximum Vector of minimum and maximum initialization vectors.

void NeuralNetwork::initialize_parameters_uniform(const Vector< Vector<double> >& minimum_maximum)
{
   if(multilayer_perceptron_pointer)
   {
      multilayer_perceptron_pointer->initialize_parameters_uniform(minimum_maximum);  
   }

   if(independent_parameters_pointer)
   {
      independent_parameters_pointer->initialize_parameters_uniform(minimum_maximum); 
   }
}


// void initialize_parameters_normal(void) method

/// This method initializes all the parameters in the neural newtork (biases and synaptic weiths + independent 
/// parameters) at random with values chosen from a normal distribution with mean 0 and standard deviation 1.

void NeuralNetwork::initialize_parameters_normal(void)
{
   if(multilayer_perceptron_pointer)
   {
      multilayer_perceptron_pointer->initialize_parameters_normal();  
   }

   if(independent_parameters_pointer)
   {
      independent_parameters_pointer->initialize_parameters_normal(); 
   }
}


// void initialize_parameters_normal(const double&, const double&) method

/// This method initializes all the parameters in the newtork (biases and synaptic weiths + independent 
/// parameters) at random with values chosen from a normal distribution with a given mean and a given standard 
/// deviation.
/// @param mean Mean of normal distribution.
/// @param standard_deviation Standard deviation of normal distribution.

void NeuralNetwork::initialize_parameters_normal(const double& mean, const double& standard_deviation)
{
   if(multilayer_perceptron_pointer)
   {
      multilayer_perceptron_pointer->initialize_parameters_normal(mean, standard_deviation);  
   }

   if(independent_parameters_pointer)
   {
      independent_parameters_pointer->initialize_parameters_normal(mean, standard_deviation); 
   }
}


// void initialize_parameters_normal(const Vector<double>&, const Vector<double>&) method

/// This method initializes all the parameters in the neural newtork (biases and synaptic weiths + 
/// independent parameters) at random with values chosen from normal distributions with a given mean and a given 
/// standard deviation for each parameter.
/// @param mean Vector of minimum initialization values.
/// @param standard_deviation Vector of maximum initialization values.

void NeuralNetwork::initialize_parameters_normal(const Vector<double>& mean, const Vector<double>& standard_deviation)
{
   if(multilayer_perceptron_pointer)
   {
      multilayer_perceptron_pointer->initialize_parameters_normal(mean, standard_deviation);  
   }

   if(independent_parameters_pointer)
   {
      independent_parameters_pointer->initialize_parameters_normal(mean, standard_deviation);  
   }
}


// void initialize_parameters_normal(const Vector< Vector<double> >&) method

/// This method initializes all the parameters in the newtork (biases and synaptic weiths + independent 
/// parameters) at random with values chosen from normal distributions with a given mean and a given standard 
/// deviation for each parameter.
/// All mean and standard deviation values are given from a vector of two real vectors.
/// The first element must contain the mean value for each parameter.
/// The second element must contain the standard deviation value for each parameter.
/// @param mean_standard_deviation Mean and standard deviation vectors.

void NeuralNetwork::initialize_parameters_normal(const Vector< Vector<double> >& mean_standard_deviation)
{
   if(multilayer_perceptron_pointer)
   {
      multilayer_perceptron_pointer->initialize_parameters_normal(mean_standard_deviation);  
   }

   if(independent_parameters_pointer)
   {
      independent_parameters_pointer->initialize_parameters_normal(mean_standard_deviation);  
   }
}


// double calculate_parameters_norm(void) const method

/// This method returns the norm of the vector of parameters.

double NeuralNetwork::calculate_parameters_norm(void) const
{
   const Vector<double> parameters = arrange_parameters();

   const double parameters_norm = parameters.calculate_norm();

   return(parameters_norm);
}


// Vector<double> calculate_outputs(const Vector<double>&) method method

/// This method calculates the outputs vector from the multilayer perceptron in response to an inputs vector.
/// The activity for that is the following:
/// <ul>
/// <li> Check inputs range.
/// <li> Calculate scaled inputs.
/// <li> Calculate forward propagation.
/// <li> Calculate unscaled outputs.
/// <li> Apply boundary condtions.
/// <li> Calculate bounded outputs. 
/// </ul>
/// @param inputs Set of inputs to the multilayer perceptron

Vector<double> NeuralNetwork::calculate_outputs(const Vector<double>& inputs) const
{
   // Control sentence (if debug)

   #ifdef _DEBUG 

   if(multilayer_perceptron_pointer)
   {
      const unsigned int inputs_size = inputs.size();

      const unsigned int inputs_number = multilayer_perceptron_pointer->count_inputs_number();

      if(inputs_size != inputs_number) 
      {
         std::ostringstream buffer;

         buffer << "OpenNN Exception: NeuralNetwork class.\n"
                << "Vector<double> calculate_outputs(const Vector<double>&) const method.\n"
                << "Size of inputs must be equal to number of inputs.\n";

	   throw std::logic_error(buffer.str());
      }
   }
   
   #endif

   Vector<double> outputs(inputs);

   // Scaling layer

   if(scaling_layer_pointer && scaling_layer_flag)
   {
      outputs = scaling_layer_pointer->calculate_outputs(inputs);
   }

   // Multilayer perceptron

   if(multilayer_perceptron_pointer && multilayer_perceptron_flag)
   {
      outputs = multilayer_perceptron_pointer->calculate_outputs(outputs);
   }

   // Conditions

   if(conditions_layer_pointer && conditions_layer_flag)
   {
      outputs = conditions_layer_pointer->calculate_outputs(inputs, outputs);
   }

   // Unscaling layer

   if(unscaling_layer_pointer && unscaling_layer_flag)
   {
      outputs = unscaling_layer_pointer->calculate_outputs(outputs);
   }

   // Probabilistic layer

   if(probabilistic_layer_pointer && probabilistic_layer_flag)
   {
      outputs = probabilistic_layer_pointer->calculate_outputs(outputs);   
   }

   // Bounding layer

   if(bounding_layer_pointer && bounding_layer_flag)
   {
      outputs = bounding_layer_pointer->calculate_outputs(outputs);
   }

   return(outputs);
}


// Vector<double> calculate_parameters_output(const Vector<double>&, const Vector<double>&) const method

/// This method returns which would be the outputs for a given inputs and a set of parameters. 
/// @param inputs Vector of inputs to the multilayer perceptron 
/// @param parameters Vector of potential parameters of the neural network.

Vector<double> NeuralNetwork::calculate_parameters_output(const Vector<double>& inputs, const Vector<double>& parameters) const
{
   // Control sentence (if debug)

   #ifdef _DEBUG 

   const unsigned int inputs_size = inputs.size();

   const unsigned int inputs_number = multilayer_perceptron_pointer->count_inputs_number();

   if(inputs_size != inputs_number)
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: NeuralNetwork class.\n"
             << "Vector<double> calculate_parameters_output(const Vector<double>&, const Vector<double>&) const method.\n"
             << "Size of inputs (" << inputs_size << ") must be equal to number of inputs (" << inputs_number << ").\n";

	  throw std::logic_error(buffer.str());
   }

   const unsigned int parameters_size = parameters.size();

   const unsigned int parameters_number = count_parameters_number();

   if(parameters_size != parameters_number)
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: NeuralNetwork class.\n"
             << "Vector<double> calculate_parameters_output(const Vector<double>&, const Vector<double>&) const method.\n"
             << "Size of potential parameters (" << parameters_size << ") must be equal to number of parameters (" << parameters_number << ").\n";

	  throw std::logic_error(buffer.str());
   }

   #endif

   NeuralNetwork copy(*this);

   copy.set_parameters(parameters);

   return(copy.calculate_outputs(inputs));
}


// Matrix<double> calculate_output_data(const Matrix<double>&) const method

/// This method calculates a set of outputs from the multilayer perceptron in response to a set of inputs.
/// The format is a matrix, where each row is a single inputs vector. 
/// @param input_data Matrix of inputs to the neural network. 

Matrix<double> NeuralNetwork::calculate_output_data(const Matrix<double>& input_data) const
{
   const unsigned int inputs_number = multilayer_perceptron_pointer->count_inputs_number();
   const unsigned int outputs_number = multilayer_perceptron_pointer->count_outputs_number();

   // Control sentence (if debug)

   #ifdef _DEBUG 

   const unsigned int columns_number = input_data.get_columns_number();

   if(columns_number != inputs_number) 
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: NeuralNetwork class.\n"
             << "Matrix<double> calculate_output_data(const Matrix<double>&) const method.\n"
             << "Number of columns must be equal to number of inputs.\n";

	  throw std::logic_error(buffer.str());
   }   

   #endif
  
   const unsigned int input_vectors_number = input_data.get_rows_number();

   Matrix<double> output_data(input_vectors_number, outputs_number);

   Vector<double> inputs(inputs_number);
   Vector<double> outputs(outputs_number);

   for(unsigned int i = 0; i < input_vectors_number; i++)
   {
      inputs = input_data.arrange_row(i);
      outputs = calculate_outputs(inputs);
      output_data.set_row(i, outputs);
   }

   return(output_data);
}


// Matrix<double> calculate_Jacobian(const Vector<double>&) const method

/// This method returns the Jacobian Matrix of the multilayer perceptron for a set of inputs, corresponding to the 
/// point in inputs space at which the Jacobian Matrix is to be found. It uses a forward-propagation method.
/// @param inputs Set of inputs to the multilayer perceptron
/// @todo

Matrix<double> NeuralNetwork::calculate_Jacobian(const Vector<double>& inputs) const
{
   #ifdef _DEBUG 

   const unsigned int size = inputs.size();

   const unsigned int inputs_number = multilayer_perceptron_pointer->count_inputs_number();

   if(size != inputs_number)
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: NeuralNetwork class.\n"
             << "void calculate_Jacobian(const Vector<double>&) const method.\n"
             << "Size must be equal to number of inputs.\n";

	  throw std::logic_error(buffer.str());
   }

   #endif

   Vector<double> outputs(inputs);

   Matrix<double> scaling_layer_Jacobian;
   Matrix<double> unscaling_layer_Jacobian;
   Matrix<double> multilayer_perceptron_Jacobian;
   Matrix<double> bounding_layer_Jacobian;
   Matrix<double> conditions_layer_Jacobian;
   Matrix<double> probabilistic_layer_Jacobian;

   // Scaling layer

   if(scaling_layer_flag)
   {
      const Vector<double> scaling_layer_derivative = scaling_layer_pointer->calculate_derivative(outputs);
      scaling_layer_Jacobian = scaling_layer_pointer->arrange_Jacobian(scaling_layer_derivative);

      outputs = scaling_layer_pointer->calculate_outputs(outputs);
   }

   // Multilayer perceptron

   if(multilayer_perceptron_flag)
   {
      multilayer_perceptron_Jacobian = multilayer_perceptron_pointer->calculate_Jacobian(outputs);

      outputs = multilayer_perceptron_pointer->calculate_outputs(outputs);
   }

   // Unscaling layer
   
   if(unscaling_layer_flag)
   {
      const Vector<double> unscaling_layer_derivative = unscaling_layer_pointer->calculate_derivative(outputs);
      unscaling_layer_Jacobian = scaling_layer_pointer->arrange_Jacobian(unscaling_layer_derivative);

      outputs = unscaling_layer_pointer->calculate_outputs(outputs);
   }

   // Probabilistic layer

   if(probabilistic_layer_flag)
   {
      probabilistic_layer_Jacobian = probabilistic_layer_pointer->calculate_Jacobian(outputs);

      outputs = probabilistic_layer_pointer->calculate_outputs(outputs);
   }

   // Bounding layer

   if(bounding_layer_flag)
   {
      const Vector<double>& derivatives = bounding_layer_pointer->calculate_derivative(outputs);

      bounding_layer_Jacobian = bounding_layer_pointer->arrange_Jacobian(derivatives);

      outputs = bounding_layer_pointer->calculate_outputs(outputs);
   }

   const unsigned int outputs_number = multilayer_perceptron_pointer->count_outputs_number();

   Matrix<double> Jacobian(outputs_number, outputs_number, 0.0);
   Jacobian.set_diagonal(1.0);

   // Bounding layer

   if(bounding_layer_flag)
   {
      Jacobian = Jacobian.dot(bounding_layer_Jacobian);
   }

   // Probabilistic outputs

   if(probabilistic_layer_flag)
   {
      Jacobian = Jacobian.dot(probabilistic_layer_Jacobian);
   }

   // Unscaling layer

   if(unscaling_layer_flag)
   {
      Jacobian = Jacobian.dot(unscaling_layer_Jacobian);
   }

   // Multilayer perceptron

   if(multilayer_perceptron_flag)
   {
      Jacobian = Jacobian.dot(multilayer_perceptron_Jacobian);
   }

   // Scaling layer

   if(scaling_layer_flag)
   {
      Jacobian = Jacobian.dot(scaling_layer_Jacobian);
   }

   // Conditions 

   if(conditions_layer_flag)
   {
      conditions_layer_Jacobian = conditions_layer_pointer->calculate_Jacobian(inputs, outputs, Jacobian);

      outputs = conditions_layer_pointer->calculate_outputs(inputs, outputs);
   }

   if(conditions_layer_flag)
   {
      Jacobian = Jacobian.dot(conditions_layer_Jacobian);
   }


   return(Jacobian);
}


// Matrix<double> calculate_parameters_Jacobian(const Vector<double>&) const method

/// This method returns the partial derivatives of the outputs with respect to the multilayer_perceptron_pointer parameters.
/// @todo

Matrix<double> NeuralNetwork::calculate_parameters_Jacobian(const Vector<double>& inputs, const Vector<double>& parameters) const
{
   return(multilayer_perceptron_pointer->calculate_parameters_Jacobian(inputs, parameters));
}


// Vector< Matrix<double> > calculate_Hessian_form(const Vector<double>&) const method

/// This method returns the second partial derivatives of the outputs with respect to the inputs.
/// @todo

Vector< Matrix<double> > NeuralNetwork::calculate_Hessian_form(const Vector<double>& inputs) const
{
   return(multilayer_perceptron_pointer->calculate_Hessian_form(inputs));
}


// Vector< Matrix<double> > calculate_parameters_Hessian_form(const Vector<double>&) const method

/// This method returns the second partial derivatives of the outputs with respect to the multilayer_perceptron_pointer parameters.
/// @todo

Vector< Matrix<double> > NeuralNetwork::calculate_parameters_Hessian_form(const Vector<double>& inputs, const Vector<double>& parameters) const
{
   return(multilayer_perceptron_pointer->calculate_parameters_Hessian_form(inputs, parameters));
}


// void set_information(const Vector< Vector<std::string> >&) method

/// This method sets all the possible information about the inputs and output variables.
/// The format is a vector of vectors of size eight: 
/// <ul>
/// <li> Name of input variables.
/// <li> Units of input variables.
/// <li> Description of input variables.
/// <li> Name of output variables.
/// <li> Units of output variables.
/// <li> Description of output variables.
/// </ul>
/// @param new_inputs_outputs_information Input and output variables information. 

void NeuralNetwork::set_inputs_outputs_information(const Vector< Vector<std::string> >& new_inputs_outputs_information)
{
   if(!multilayer_perceptron_pointer)
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: NeuralNetwork class.\n"
             << "void set_inputs_outputs_information(const Vector< Vector<std::string> >&) method.\n" 
             << "Pointer to multilayer perceptron is NULL.\n";

	  throw std::logic_error(buffer.str());
   }

   const unsigned int inputs_number = multilayer_perceptron_pointer->count_inputs_number();
   const unsigned int outputs_number = multilayer_perceptron_pointer->count_outputs_number();

   #ifdef _DEBUG 

   const unsigned int size = new_inputs_outputs_information.size();

   if(size != 6)
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: NeuralNetwork class.\n"
             << "void set_inputs_outputs_information(const Vector< Vector<std::string> >&) method.\n" 
             << "Size of vector must be 6.\n";

	  throw std::logic_error(buffer.str());
   }

   for(unsigned int i = 0; i < 3; i++)
   {
      if(new_inputs_outputs_information[i].size() != inputs_number)
      {    
         std::ostringstream buffer;

         buffer << "OpenNN Exception: NeuralNetwork class.\n"
                << "void set_inputs_outputs_information(const Vector< Vector<std::string> >&) method.\n" 
                << "Size of subvector " << i << "must be equal to number of inputs.\n";

         throw std::logic_error(buffer.str());
      }   
   }

   for(unsigned int i = 3; i < 6; i++)
   {
      if(new_inputs_outputs_information[i].size() != outputs_number)
      {    
         std::ostringstream buffer;

         buffer << "OpenNN Exception: NeuralNetwork class.\n"
                << "void set_inputs_outputs_information(const Vector< Vector<std::string> >&) method.\n" 
                << "Size of subvector " << i << "must be equal to number of outputs.\n";

	     throw std::logic_error(buffer.str());
      }   
   }

   #endif

   if(!inputs_outputs_information_pointer)
   {     
      inputs_outputs_information_pointer = new InputsOutputsInformation(inputs_number, outputs_number);
   }

   inputs_outputs_information_pointer->set_inputs_name(new_inputs_outputs_information[0]);
   inputs_outputs_information_pointer->set_inputs_units(new_inputs_outputs_information[1]);
   inputs_outputs_information_pointer->set_inputs_description(new_inputs_outputs_information[2]);

   inputs_outputs_information_pointer->set_outputs_name(new_inputs_outputs_information[3]);
   inputs_outputs_information_pointer->set_outputs_units(new_inputs_outputs_information[4]);
   inputs_outputs_information_pointer->set_outputs_description(new_inputs_outputs_information[5]);
}


// Vector< Vector<std::string> > arrange_inputs_outputs_information(void) method

/// This method returns all the available information about the inputs and output variables. 
/// The format is a vector of pointers to vectors of size six: 
/// <ul>
/// <li> Name of input variables.
/// <li> Name of output variables.
/// <li> Units of input variables.
/// <li> Units of output variables.
/// <li> Description of input variables.
/// <li> Description of output variables.
/// </ul>

Vector< Vector<std::string> > NeuralNetwork::arrange_inputs_outputs_information(void)
{
   if(!inputs_outputs_information_pointer)
   {
      if(!multilayer_perceptron_pointer)
      {
         std::ostringstream buffer;

         buffer << "OpenNN Exception: NeuralNetwork class.\n"
                << "Vector< Vector<std::string> > arrange_inputs_outputs_information(void) method.\n" 
                << "Pointer to multilayer perceptron is NULL.\n";

	     throw std::logic_error(buffer.str());      
      }

      const unsigned int inputs_number = multilayer_perceptron_pointer->count_inputs_number();
      const unsigned int outputs_number = multilayer_perceptron_pointer->count_outputs_number();

      inputs_outputs_information_pointer = new InputsOutputsInformation(inputs_number, outputs_number);
   }

   Vector< Vector<std::string> > inputs_outputs_information(6);
 
   inputs_outputs_information[0] = inputs_outputs_information_pointer->get_inputs_name();
   inputs_outputs_information[1] = inputs_outputs_information_pointer->get_inputs_units();
   inputs_outputs_information[2] = inputs_outputs_information_pointer->get_inputs_description();

   inputs_outputs_information[3] = inputs_outputs_information_pointer->get_outputs_name();
   inputs_outputs_information[4] = inputs_outputs_information_pointer->get_outputs_units();
   inputs_outputs_information[5] = inputs_outputs_information_pointer->get_outputs_description();

   return(inputs_outputs_information);
}


// Vector< Vector<double> > arrange_inputs_outputs_statistics(void) method

/// This method returns all the available statistics of the inputs and output variables. 
/// The format is a vector of pointers to vectors of size ten: 
/// <ul>
/// <li> Mean of input variables.
/// <li> Mean of output variables.
/// <li> Standard deviation of input variables.
/// <li> Standard deviation of output variables.
/// <li> Minimum of input variables.
/// <li> Minimum of output variables.
/// <li> Maximum of input variables.
/// <li> Maximum of output variables.
/// </ul>

Vector< Vector<double> > NeuralNetwork::arrange_inputs_outputs_statistics(void)
{
   if(!scaling_layer_pointer)
   {
      if(!multilayer_perceptron_pointer)
      {
         std::ostringstream buffer;

         buffer << "OpenNN Exception: NeuralNetwork class.\n"
                << "Vector< Vector<double> > arrange_inputs_outputs_statistics(void) method.\n" 
                << "Pointer to multilayer perceptron is NULL.\n";

	     throw std::logic_error(buffer.str());      
      }

      const unsigned int inputs_number = multilayer_perceptron_pointer->count_inputs_number();

      scaling_layer_pointer = new ScalingLayer(inputs_number);
   }

   if(!unscaling_layer_pointer)
   {
      if(!multilayer_perceptron_pointer)
      {
         std::ostringstream buffer;

         buffer << "OpenNN Exception: NeuralNetwork class.\n"
                << "Vector< Vector<double> > arrange_inputs_outputs_statistics(void) method.\n" 
                << "Pointer to multilayer perceptron is NULL.\n";

	     throw std::logic_error(buffer.str());      
      }

      const unsigned int outputs_number = multilayer_perceptron_pointer->count_outputs_number();

      unscaling_layer_pointer = new UnscalingLayer(outputs_number);
   }

   Vector< Vector<double> > statistics(8);

   statistics[0] = scaling_layer_pointer->get_minimums();
   statistics[1] = scaling_layer_pointer->get_maximums();

   statistics[2] = scaling_layer_pointer->get_means();
   statistics[3] = scaling_layer_pointer->get_standard_deviations();

   statistics[4] = unscaling_layer_pointer->get_minimums();
   statistics[5] = unscaling_layer_pointer->get_maximums();

   statistics[6] = unscaling_layer_pointer->get_means();
   statistics[7] = unscaling_layer_pointer->get_standard_deviations();

   return(statistics);
}


// void set_inputs_outputs_minimums_maximums(const Vector< Vector<double> >&) method

/// This method sets the scaling and unscaling layers minimums and maximums from a single vector.
/// The format is:
/// <ul>
/// <li> Inputs minimums. 
/// <li> Inputs maximums. 
/// <li> Outputs minimums. 
/// <li> Outputs maximums. 
/// </ul>
/// @param new_inputs_outputs_minimum_maximum Vector with the minimums and maximums of the input and output variables. 

void NeuralNetwork::set_inputs_outputs_minimums_maximums(const Vector< Vector<double> >& new_inputs_outputs_minimum_maximum)
{
   if(!multilayer_perceptron_pointer)
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: NeuralNetwork class.\n"
             << "void set_inputs_outputs_minimum_maximum(const Vector< Vector<double> >&) method.\n"
             << "Multilayer perceptron pointer is NULL.\n";

	  throw std::logic_error(buffer.str());
   }

   const unsigned int outputs_number = multilayer_perceptron_pointer->count_outputs_number();
   const unsigned int inputs_number = multilayer_perceptron_pointer->count_inputs_number();

   // Control sentence (if debug)

   #ifdef _DEBUG 

   if(new_inputs_outputs_minimum_maximum.size() != 4)
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: NeuralNetwork class.\n"
             << "void set_variables_minimum_maximum(const Vector< Vector<double> >&) method.\n"
             << "Size of inputs-outputs minimums and maximums vector must be 4.\n";

	  throw std::logic_error(buffer.str());

	  return;
   }

   // Inputs

   for(unsigned int i = 0; i < 2; i++)
   {
	  if(new_inputs_outputs_minimum_maximum[i].size() != inputs_number)
	  {
         std::ostringstream buffer;

         buffer << "OpenNN Exception: NeuralNetwork class.\n"
                << "void set_variables_minimum_maximum(const Vector< Vector<double> >&) method.\n"
                << "Size of element " << i << " must be equal to number of inputs.\n";

	     throw std::logic_error(buffer.str());

		 return;
	  }
   }

   for(unsigned int i = 2; i < 4; i++)
   {
	  if(new_inputs_outputs_minimum_maximum[i].size() != outputs_number)
 	  {
         std::ostringstream buffer;

         buffer << "OpenNN Exception: NeuralNetwork class.\n"
                << "void set_variables_minimum_maximum(const Vector< Vector<double> >&) method.\n"
                << "Size of element " << i << " must be equal to number of outputs.\n";

	     throw std::logic_error(buffer.str());
	  }
   }

   #endif

   // Scaling layer

   if(!scaling_layer_pointer)
   {
      scaling_layer_pointer = new ScalingLayer(inputs_number);
   }

   scaling_layer_pointer->set_minimums(new_inputs_outputs_minimum_maximum[0]);
   scaling_layer_pointer->set_maximums(new_inputs_outputs_minimum_maximum[1]);

   // Unscaling layer

   if(!unscaling_layer_pointer)
   {
      unscaling_layer_pointer = new UnscalingLayer(outputs_number);
   }

   unscaling_layer_pointer->set_minimums(new_inputs_outputs_minimum_maximum[2]);
   unscaling_layer_pointer->set_maximums(new_inputs_outputs_minimum_maximum[3]);
}


// void set_inputs_outputs_means_standard_deviations(const Vector< Vector<double> >&) method

/// This method sets the scaling and unscaling layers means and standard deviations from a single vector.
/// The format is:
/// <ul>
/// <li> Inputs means. 
/// <li> Inputs standard deviations. 
/// <li> Outputs means. 
/// <li> Outputs standard deviations. 
/// </ul>
/// @param new_inputs_outputs_mean_standard_deviation Vector with the means and standard deviations of the input and output variables. 

void NeuralNetwork::set_inputs_outputs_means_standard_deviations(const Vector< Vector<double> >& new_inputs_outputs_mean_standard_deviation)
{
   if(!multilayer_perceptron_pointer)
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: NeuralNetwork class.\n"
             << "void set_inputs_outputs_means_standard_deviations(const Vector< Vector<double> >&) method.\n"
             << "Multilayer perceptron pointer is NULL.\n";

	  throw std::logic_error(buffer.str());
   }

   const unsigned int outputs_number = multilayer_perceptron_pointer->count_outputs_number();
   const unsigned int inputs_number = multilayer_perceptron_pointer->count_inputs_number();

   // Control sentence (if debug)

   #ifdef _DEBUG 

   const unsigned int new_variables_mean_standard_deviation_size = new_inputs_outputs_mean_standard_deviation.size();

   if(new_variables_mean_standard_deviation_size != 4)
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: NeuralNetwork class.\n"
             << "void set_inputs_outputs_mean_standard_deviation(const Vector< Vector<double> >&) method.\n"
             << "Size must be 4.\n";

	  throw std::logic_error(buffer.str());

	  return;
   }

   unsigned int size;

   // Inputs

   for(unsigned int i = 0; i < 2; i++)
   {
      size = new_inputs_outputs_mean_standard_deviation[i].size();

	  if(size != inputs_number)
	  {
         std::ostringstream buffer;

         buffer << "OpenNN Exception: NeuralNetwork class.\n"
                << "void set_inputs_outputs_mean_standard_deviation(const Vector< Vector<double> >&) method.\n"
                << "Size of element " << i << " must be equal to number of inputs.\n";

         throw std::logic_error(buffer.str());

		 return;
	  }
   }

   // Outputs

   for(unsigned int i = 2; i < 4; i++)
   {
      size = new_inputs_outputs_mean_standard_deviation[i].size();

	  if(size != outputs_number)
 	  {
         std::ostringstream buffer;

         buffer << "OpenNN Exception: NeuralNetwork class.\n"
                << "void set_inputs_outputs_mean_standard_deviation(const Vector< Vector<double> >&) method.\n"
                << "Size of element " << i << " must be equal to number of outputs.\n";

	     throw std::logic_error(buffer.str());
	  }
   }

   #endif

   // Scaling layer

   if(!scaling_layer_pointer)
   {
      scaling_layer_pointer = new ScalingLayer(inputs_number);   
   }

   scaling_layer_pointer->set_means(new_inputs_outputs_mean_standard_deviation[0]);
   scaling_layer_pointer->set_standard_deviations(new_inputs_outputs_mean_standard_deviation[1]);

   // Unscaling layer

   if(!unscaling_layer_pointer)
   {
      unscaling_layer_pointer = new UnscalingLayer(outputs_number);   
   }

   unscaling_layer_pointer->set_means(new_inputs_outputs_mean_standard_deviation[2]);
   unscaling_layer_pointer->set_standard_deviations(new_inputs_outputs_mean_standard_deviation[3]);
}


// void set_statistics(const Vector< Vector<double> >&) method

/// This method sets all the available statistics about the inputs and output variables.
/// The format is a vector of ten real vectors: 
/// <ul>
/// <li> Minimum of input variables.
/// <li> Maximum of input variables.
/// <li> Mean of input variables.
/// <li> Standard deviation of input variables.
/// <li> Minimum of output variables.
/// <li> Maximum of output variables.
/// <li> Mean of output variables.
/// <li> Standard deviation of output variables.
/// </ul>
/// @param new_inputs_outputs_statistics Input and output variables statistics. 

void NeuralNetwork::set_inputs_outputs_statistics(const Vector< Vector<double> >& new_inputs_outputs_statistics)
{
   if(!multilayer_perceptron_pointer)
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: NeuralNetwork class.\n"
             << "void set_inputs_outputs_statistics(const Vector< Vector<double> >&) method.\n"
             << "Multilayer perceptron pointer is NULL.\n";

	  throw std::logic_error(buffer.str());
   }

   const unsigned int inputs_number = multilayer_perceptron_pointer->count_inputs_number();
   const unsigned int outputs_number = multilayer_perceptron_pointer->count_outputs_number();

   // Control sentence (if debug)

   #ifdef _DEBUG 

   std::ostringstream buffer;

   const unsigned int new_statistics_size = new_inputs_outputs_statistics.size();

   if(new_statistics_size != 8)
   {
      buffer << "OpenNN Exception: NeuralNetwork class.\n"
             << "void set_inputs_outputs_statistics(const Vector< Vector<double> >&) method.\n"
             << "Size must be 8.\n";

	  throw std::logic_error(buffer.str());
   }

   unsigned int size;

   for(unsigned int i = 0; i < 4; i++)
   {
      size = new_inputs_outputs_statistics[i].size();

	  if(size != inputs_number)
	  {
         buffer << "OpenNN Exception: NeuralNetwork class.\n"
                << "void set_inputs_outputs_statistics(const Vector< Vector<double> >&) method.\n"
                << "Size of element " << i << " must be equal to number of inputs.\n";

	     throw std::logic_error(buffer.str());
	  }
   }

   for(unsigned int i = 4; i < 8; i++)
   {
      size = new_inputs_outputs_statistics[i].size();

	  if(size != outputs_number)
 	  {
         buffer << "OpenNN Exception: NeuralNetwork class.\n"
                << "void set_variables_statistics(const Vector< Vector<double> >&) method.\n"
                << "Size of element " << i << " must be equal to number of outputs.\n";

	     throw std::logic_error(buffer.str());
	  }
   }

   #endif

   // Scaling layer

   if(!scaling_layer_pointer)
   {
      scaling_layer_pointer = new ScalingLayer(inputs_number);
   }

   scaling_layer_pointer->set_minimums(new_inputs_outputs_statistics[0]);
   scaling_layer_pointer->set_maximums(new_inputs_outputs_statistics[1]);
   scaling_layer_pointer->set_means(new_inputs_outputs_statistics[2]);
   scaling_layer_pointer->set_standard_deviations(new_inputs_outputs_statistics[3]);

   if(!unscaling_layer_pointer)
   {
      unscaling_layer_pointer = new UnscalingLayer(outputs_number);
   }

   unscaling_layer_pointer->set_minimums(new_inputs_outputs_statistics[4]);
   unscaling_layer_pointer->set_maximums(new_inputs_outputs_statistics[5]);
   unscaling_layer_pointer->set_means(new_inputs_outputs_statistics[6]);
   unscaling_layer_pointer->set_standard_deviations(new_inputs_outputs_statistics[7]);
}


// void set_inputs_scaling_outputs_unscaling_methods(const std::string&) method

/// This method sets the scaling method of the scaling layer and the unscaling method of the unscaling layer. 
/// @param new_inputs_scaling_outputs_unscaling_methods Method for scaling inputs and uscaling outputs.
/// It can be "MinimumMaximum" and "MeanStandardDeviation".

void NeuralNetwork::set_inputs_scaling_outputs_unscaling_methods(const std::string& new_inputs_scaling_outputs_unscaling_methods)
{
   if(new_inputs_scaling_outputs_unscaling_methods == "MinimumMaximum")
   {
      scaling_layer_pointer->set_scaling_method(ScalingLayer::MinimumMaximum);
      unscaling_layer_pointer->set_unscaling_method(UnscalingLayer::MinimumMaximum);
   }
   else if(new_inputs_scaling_outputs_unscaling_methods == "MeanStandardDeviation")
   {
      scaling_layer_pointer->set_scaling_method(ScalingLayer::MeanStandardDeviation);
      unscaling_layer_pointer->set_unscaling_method(UnscalingLayer::MeanStandardDeviation);
   }
   else
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: NeuralNetwork class.\n"
             << "void new_inputs_scaling_outputs_unscaling_methods(const std::string&) method.\n"
			 << "Unknown scaling/unscaling method: " << new_inputs_scaling_outputs_unscaling_methods << ".\n";

	  throw std::logic_error(buffer.str());
   }
}


// std::string to_string(void) const method

/// This method returns a string representation of the current neural network object. 

std::string NeuralNetwork::to_string(void) const
{
   std::ostringstream buffer;

   buffer << "NeuralNetwork\n";

   // Multilayer perceptron 

   if(multilayer_perceptron_pointer)
   {
      buffer << multilayer_perceptron_pointer->to_string();
   }
 
   // Scaling layer

   if(scaling_layer_pointer)
   {
      buffer << scaling_layer_pointer->to_string();
   }

   // Unscaling layer

   if(unscaling_layer_pointer)
   {
      buffer << unscaling_layer_pointer->to_string();
   }

   // Bounding layer

   if(bounding_layer_pointer)
   {
      buffer << bounding_layer_pointer->to_string();
   }

   // Probabilistic layer

   if(probabilistic_layer_pointer)
   {
      buffer << probabilistic_layer_pointer->to_string();
   }

   // Conditions layer

   if(conditions_layer_pointer)
   {
      buffer << conditions_layer_pointer->to_string();
   }

   // Inputs outputs information 

   if(inputs_outputs_information_pointer)
   {
      buffer << inputs_outputs_information_pointer->to_string();
   }

   // Independent parameters

   if(independent_parameters_pointer)
   {
      buffer << independent_parameters_pointer->to_string();
   }

   buffer << "Multilayer perceptron flag: " << multilayer_perceptron_flag << "\n"
          << "Scaling layer flag: " <<  scaling_layer_flag << "\n"
          << "Unscaling layer flag: " <<  unscaling_layer_flag << "\n"
          << "Bounding layer flag: " <<  bounding_layer_flag << "\n"
          << "Probabilistic layer flag: " <<  probabilistic_layer_flag << "\n"
          << "Conditions layer flag: " <<  conditions_layer_flag << "\n"
          << "Display: " <<  display << "\n";

   return(buffer.str());
}


// TiXmlElement* to_XML(void) const method

/// This method serializes the neural network object into a XML element of the TinyXML library. 
/// See the OpenNN manual for more information about the format of this element. 

TiXmlElement* NeuralNetwork::to_XML(void) const
{
   std::ostringstream buffer;

   TiXmlElement* neural_network_element = new TiXmlElement("NeuralNetwork");
   neural_network_element->SetAttribute("Version", 4);

   // Inputs outputs information 

   if(inputs_outputs_information_pointer)
   {
      TiXmlElement* element = inputs_outputs_information_pointer->to_XML();
      neural_network_element->LinkEndChild(element);    
   }

   // Multilayer perceptron 

   if(multilayer_perceptron_pointer)
   {
      TiXmlElement* element = multilayer_perceptron_pointer->to_XML();
      neural_network_element->LinkEndChild(element);
   }
 
   // Scaling layer

   if(scaling_layer_pointer)
   {
      TiXmlElement* element = scaling_layer_pointer->to_XML();
      neural_network_element->LinkEndChild(element);    
   }

   // Unscaling layer

   if(unscaling_layer_pointer)
   {
      TiXmlElement* element = unscaling_layer_pointer->to_XML();
      neural_network_element->LinkEndChild(element);    
   }

   // Probabilistic layer

   if(probabilistic_layer_pointer)
   {
      TiXmlElement* element = probabilistic_layer_pointer->to_XML();
      neural_network_element->LinkEndChild(element);    
   }

   // Bounding layer

   if(bounding_layer_pointer)
   {
      TiXmlElement* element = bounding_layer_pointer->to_XML();
      neural_network_element->LinkEndChild(element);    
   }

   // Conditions layer

   if(conditions_layer_pointer)
   {
      TiXmlElement* element = conditions_layer_pointer->to_XML();
      neural_network_element->LinkEndChild(element);    
   }

   // Independent parameters

   if(independent_parameters_pointer)
   {
      TiXmlElement* element = independent_parameters_pointer->to_XML();
      neural_network_element->LinkEndChild(element);       
   }

   // Scaling layer flag
   {
      TiXmlElement* scaling_layer_flag_element = new TiXmlElement("ScalingLayerFlag");
      neural_network_element->LinkEndChild(scaling_layer_flag_element);

      buffer.str("");
      buffer << scaling_layer_flag;

      TiXmlText* scaling_layer_flag_text = new TiXmlText(buffer.str().c_str());
      scaling_layer_flag_element->LinkEndChild(scaling_layer_flag_text);
   }

   // Unscaling layer flag
   {
      TiXmlElement* element = new TiXmlElement("UnscalingLayerFlag");
      neural_network_element->LinkEndChild(element);

      buffer.str("");
      buffer << unscaling_layer_flag;

      TiXmlText* text = new TiXmlText(buffer.str().c_str());
      element->LinkEndChild(text);
   }

   // Probabilistic layer flag
   {
      TiXmlElement* element = new TiXmlElement("ProbabilisticLayerFlag");
      neural_network_element->LinkEndChild(element);

      buffer.str("");
      buffer << probabilistic_layer_flag;

      TiXmlText* text = new TiXmlText(buffer.str().c_str());
      element->LinkEndChild(text);
   }

   // Bounding layer flag
   {
      TiXmlElement* bounding_layer_flag_element = new TiXmlElement("BoundingLayerFlag");
      neural_network_element->LinkEndChild(bounding_layer_flag_element);

      buffer.str("");
      buffer << bounding_layer_flag;

      TiXmlText* bounding_layer_flag_text = new TiXmlText(buffer.str().c_str());
      bounding_layer_flag_element->LinkEndChild(bounding_layer_flag_text);
   }

   // Conditions layer flag
   {
      TiXmlElement* conditions_layer_flag_element = new TiXmlElement("ConditionsLayerFlag");
      neural_network_element->LinkEndChild(conditions_layer_flag_element);

      buffer.str("");
      buffer << conditions_layer_flag;

      TiXmlText* conditions_layer_flag_text = new TiXmlText(buffer.str().c_str());
      conditions_layer_flag_element->LinkEndChild(conditions_layer_flag_text);
   }

   // Display warnings
   {
      TiXmlElement* display_element = new TiXmlElement("Display");
      neural_network_element->LinkEndChild(display_element);

      buffer.str("");
      buffer << display;

      TiXmlText* display_text = new TiXmlText(buffer.str().c_str());
      display_element->LinkEndChild(display_text);
   }

   return(neural_network_element);
}


// void from_XML(TiXmlElement*) method

/// This method deserializes a TinyXML element into this neural network object. 
/// @param neural_network_element Pointer to a XML element containing the member data. 

void NeuralNetwork::from_XML(TiXmlElement* neural_network_element)
{
   if(neural_network_element)
   {

      // Multilayer perceptron element
      {
         TiXmlElement* multilayer_perceptron_element = neural_network_element->FirstChildElement("MultilayerPerceptron");

         if(multilayer_perceptron_element)
         {
            if(!multilayer_perceptron_pointer)
            {
               multilayer_perceptron_pointer = new MultilayerPerceptron();
            }

            multilayer_perceptron_pointer->from_XML(multilayer_perceptron_element);
         }
      }

      // Scaling layer
      {
         TiXmlElement* scaling_layer_element = neural_network_element->FirstChildElement("ScalingLayer");

         if(scaling_layer_element)
         {
            if(!scaling_layer_pointer)
            {
               scaling_layer_pointer = new ScalingLayer();
            }
      
            scaling_layer_pointer->from_XML(scaling_layer_element);
         }
      }

      // Unscaling layer
      {
         TiXmlElement* unscaling_layer_element = neural_network_element->FirstChildElement("UnscalingLayer");

         if(unscaling_layer_element)
         {
            if(!unscaling_layer_pointer)
            {
               unscaling_layer_pointer = new UnscalingLayer();
            }
      
            unscaling_layer_pointer->from_XML(unscaling_layer_element);
         }
      }

      // Bounding layer
      {
         TiXmlElement* bounding_layer_element = neural_network_element->FirstChildElement("BoundingLayer");

         if(bounding_layer_element)
         {
            if(!bounding_layer_pointer)
            {
               bounding_layer_pointer = new BoundingLayer();
            }
      
            bounding_layer_pointer->from_XML(bounding_layer_element);
         }
      }

      // Probabilistic layer 
      {
         TiXmlElement* probabilistic_layer_element = neural_network_element->FirstChildElement("ProbabilisticLayer");

         if(probabilistic_layer_element)
         {
            if(!probabilistic_layer_pointer)
            {
               probabilistic_layer_pointer = new ProbabilisticLayer();
            }
      
            probabilistic_layer_pointer->from_XML(probabilistic_layer_element);
         }
      }

      // Conditions layer 
      {
         TiXmlElement* conditions_layer_element = neural_network_element->FirstChildElement("ConditionsLayer");

         if(conditions_layer_element)
         {
            if(!conditions_layer_pointer)
            {
               conditions_layer_pointer = new ConditionsLayer();
            }
      
            conditions_layer_pointer->from_XML(conditions_layer_element);
         }
      }

      // Inputs outputs information
      {
         TiXmlElement* inputs_outputs_information_element = neural_network_element->FirstChildElement("InputsOutputsInformation");

         if(inputs_outputs_information_element)
         {
            if(!inputs_outputs_information_pointer)
            {
               inputs_outputs_information_pointer = new InputsOutputsInformation();
            }
      
            inputs_outputs_information_pointer->from_XML(inputs_outputs_information_element);
         }
      }

      // Indpependent parameters
      {
         TiXmlElement* independent_parameters_element = neural_network_element->FirstChildElement("IndependentParameters");

         if(independent_parameters_element)
         {
            if(!independent_parameters_pointer)
            {
               independent_parameters_pointer = new IndependentParameters();
            }
      
            independent_parameters_pointer->from_XML(independent_parameters_element);
         }
      }

      // Multilayer perceptron flag
      {
         TiXmlElement* multilayer_perceptron_flag_element = neural_network_element->FirstChildElement("MultilayerPerceptronFlag");

         if(multilayer_perceptron_flag_element)
         {
            std::string new_multilayer_perceptron_flag_string = multilayer_perceptron_flag_element->GetText();           

            try
            {
               set_multilayer_perceptron_flag(new_multilayer_perceptron_flag_string != "0");
            }
            catch(std::exception& e)
            {
               std::cout << e.what() << std::endl;		 
            }
         }
      }

      // Scaling layer flag
      {
         TiXmlElement* scaling_layer_flag_element = neural_network_element->FirstChildElement("ScalingLayerFlag");

         if(scaling_layer_flag_element)
         {
            std::string new_scaling_layer_flag_string = scaling_layer_flag_element->GetText();           

            try
            {
               set_scaling_layer_flag(new_scaling_layer_flag_string != "0");
            }
            catch(std::exception& e)
            {
               std::cout << e.what() << std::endl;		 
            }
         }
      }

      // Unscaling layer flag
      {
         TiXmlElement* unscaling_layer_flag_element = neural_network_element->FirstChildElement("UnscalingLayerFlag");

         if(unscaling_layer_flag_element)
         {
            std::string new_unscaling_layer_flag_string = unscaling_layer_flag_element->GetText();           

            try
            {
               set_unscaling_layer_flag(new_unscaling_layer_flag_string != "0");
            }
            catch(std::exception& e)
            {
               std::cout << e.what() << std::endl;		 
            }
         }
      }

      // Bounding layer flag
      {
         TiXmlElement* bounding_layer_flag_element = neural_network_element->FirstChildElement("BoundingLayerFlag");

         if(bounding_layer_flag_element)
         {
            std::string new_bounding_layer_flag_string = bounding_layer_flag_element->GetText();           

            try
            {
               set_bounding_layer_flag(new_bounding_layer_flag_string != "0");
            }
            catch(std::exception& e)
            {
               std::cout << e.what() << std::endl;		 
            }
         }
      }

      // Probabilistic layer flag
      {
         TiXmlElement* probabilistic_layer_flag_element = neural_network_element->FirstChildElement("ProbabilisticLayerFlag");

         if(probabilistic_layer_flag_element)
         {
            std::string new_probabilistic_layer_flag_string = probabilistic_layer_flag_element->GetText();           

            try
            {
               set_probabilistic_layer_flag(new_probabilistic_layer_flag_string != "0");
            }
            catch(std::exception& e)
            {
               std::cout << e.what() << std::endl;		 
            }
         }
      }

      // Conditions layer flag
      {
         TiXmlElement* conditions_layer_flag_element = neural_network_element->FirstChildElement("ConditionsLayerFlag");

         if(conditions_layer_flag_element)
         {
            std::string new_conditions_layer_flag_string = conditions_layer_flag_element->GetText();           

            try
            {
               set_conditions_layer_flag(new_conditions_layer_flag_string != "0");
            }
            catch(std::exception& e)
            {
               std::cout << e.what() << std::endl;		 
            }
         }
      }

      // Display
      {
         TiXmlElement* display_element = neural_network_element->FirstChildElement("Display");

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
   }
}


// void print(void) const method   

/// This method prints to the screen the members of a neural network object in a XML-type format.

void NeuralNetwork::print(void) const
{
   if(display)
   {
      std::cout << to_string();
   }
}


// void save(const std::string&) const method 

/// This method saves to a XML-type format file the members of a neural network object.
/// @param filename Name of multilayer perceptron XML-type file.

void NeuralNetwork::save(const std::string& filename) const
{
   TiXmlDocument document;

   // Declaration

   TiXmlDeclaration* declaration = new TiXmlDeclaration("1.0", "", "");
   document.LinkEndChild(declaration);

   // Multilayer perceptron element

   TiXmlElement* multilayer_perceptron_element = to_XML();
   document.LinkEndChild(multilayer_perceptron_element);

   // Save

   document.SaveFile(filename.c_str());
 }


// void save_parameters(const std::string&) const method 

/// This method saves to a data file the parameters of a neural network object.
/// @param filename Name of parameters data file.

void NeuralNetwork::save_parameters(const std::string& filename) const
{
   std::ofstream file(filename.c_str());

   if(!file.is_open())
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: NeuralNetwork class.\n"
             << "void save_parameters(const std::string&) const method.\n"
             << "Cannot open parameters data file.\n";

	  throw std::logic_error(buffer.str());
   }

   const Vector<double> parameters = arrange_parameters();

   file << parameters << std::endl;

   // Close file

   file.close();
}


// void load(const std::string&) method

/// This method loads from a XML-type file the members of a neural network object.
/// Please mind about the file format, which is specified in the User's Guide. 
/// @param filename Name of multilayer perceptron XML-type file.

void NeuralNetwork::load(const std::string& filename)
{
   std::ostringstream buffer;

   TiXmlDocument document(filename.c_str());

   if(!document.LoadFile())
   {
      buffer << "OpenNN Exception: NeuralNetwork class.\n"
             << "void load(const std::string&) method.\n"
             << "Cannot load XML file " << filename << ".\n";

      throw std::logic_error(buffer.str());
   }

   // Neural network element
  
   TiXmlElement* neural_network_element = document.FirstChildElement();

   if(!neural_network_element)
   {
      buffer << "OpenNN Exception: NeuralNetwork class.\n"
             << "void load(const std::string&) method.\n"
             << "File " << filename << " is not a valid neural network file.\n";

      throw std::logic_error(buffer.str());
   }

   from_XML(neural_network_element);
}


// void load_parameters(const std::string&) method

/// This method loads the multilayer perceptron parameters from a data file. 
/// The format of this file is just a sequence of numbers. 
/// @param filename Name of parameters data file. 

void NeuralNetwork::load_parameters(const std::string& filename)
{
   std::ifstream file(filename.c_str());

   if(!file.is_open())
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: NeuralNetwork class.\n"
             << "void load_parameters(const std::string&) method.\n"
             << "Cannot open parameters data file.\n";

	  throw std::logic_error(buffer.str());
   }

   const unsigned int parameters_number = count_parameters_number();

   Vector<double> new_parameters(parameters_number);

   new_parameters.load(filename);

   set_parameters(new_parameters);

   file.close();
}


// std::string write_expression(void) const method

/// @todo

std::string NeuralNetwork::write_expression(void) const
{
   std::ostringstream buffer;

   #ifdef _DEBUG 

   if(!inputs_outputs_information_pointer)
   {
      buffer.str("");

      buffer << "OpenNN Exception: NeuralNetwork class.\n"
             << "std::string write_expression(void) const method.\n"
             << "Pointer to inputs-outputs information is NULL.\n";

	  throw std::logic_error(buffer.str());
   }

   #endif


   #ifdef _DEBUG 

   if(!multilayer_perceptron_pointer)
   {
      buffer.str("");

      buffer << "OpenNN Exception: NeuralNetwork class.\n"
             << "std::string write_expression(void) const method.\n"
             << "Pointer to multilayer perceptron is NULL.\n";

	  throw std::logic_error(buffer.str());
   }

   #endif

   const unsigned int inputs_number = multilayer_perceptron_pointer->count_inputs_number();
   const unsigned int outputs_number = multilayer_perceptron_pointer->count_outputs_number();

   const Vector<std::string> inputs_name = inputs_outputs_information_pointer->write_default_inputs_name();
   const Vector<std::string> outputs_name = inputs_outputs_information_pointer->write_default_outputs_name();

   // Scaled inputs

   Vector<std::string> scaled_inputs_name(inputs_number);

   for(unsigned int i = 0; i < inputs_number; i++)
   {
      buffer.str("");

      buffer << "scaled_" << inputs_name[i];
       
      scaled_inputs_name[i] = buffer.str();
   }      

   // Scaled outputs

   Vector<std::string> scaled_outputs_name(outputs_number);

   for(unsigned int i = 0; i < outputs_number; i++)
   {
      buffer.str("");

      buffer << "scaled_" << outputs_name[i];
       
      scaled_outputs_name[i] = buffer.str();
   }      

   // Probabilistic outputs

   Vector<std::string> probabilistic_outputs_name(outputs_number);

   for(unsigned int i = 0; i < outputs_number; i++)
   {
      buffer.str("");

      buffer << "probabilistic_" << outputs_name[i];
       
      probabilistic_outputs_name[i] = buffer.str();
   }      

   buffer.str("");

   // Scaling layer

   if(scaling_layer_flag)
   {
      buffer << scaling_layer_pointer->write_expression(inputs_name, scaled_inputs_name);
   }

   // Multilayer perceptron 

   if(multilayer_perceptron_flag)
   {
      if(scaling_layer_flag && unscaling_layer_flag)
      {
         buffer << multilayer_perceptron_pointer->write_expression(scaled_inputs_name, scaled_outputs_name);      
      }
      else if(scaling_layer_flag)
      {
         buffer << multilayer_perceptron_pointer->write_expression(scaled_inputs_name, outputs_name);      
      }
      else if(unscaling_layer_flag)
      {
         buffer << multilayer_perceptron_pointer->write_expression(inputs_name, scaled_outputs_name);      
      }
      else
      {
         buffer << multilayer_perceptron_pointer->write_expression(inputs_name, outputs_name);      
      }
   }

   // Outputs unscaling

   if(unscaling_layer_flag)
   {
      buffer << unscaling_layer_pointer->write_expression(scaled_outputs_name, outputs_name);
   }

   // Probabilistic layer

   if(probabilistic_layer_flag)
   {
      buffer << probabilistic_layer_pointer->write_expression(outputs_name, probabilistic_outputs_name);
   }

   // Conditions layer

   if(conditions_layer_flag)
   {
//      buffer << conditions_layer_pointer->write_expression(inputs_name, outputs_name);   
   }

   return(buffer.str());
}


// void save_expression(const std::string&) method

/// This method saves the mathematical expression represented by the neural network to a text file. 
/// @param filename Name of expression text file. 

void NeuralNetwork::save_expression(const std::string& filename)
{
   std::ofstream file(filename.c_str());

   if(!file.is_open())
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: NeuralNetwork class.\n"
             << "void  save_expression(const std::string&) method.\n"
             << "Cannot open expression text file.\n";

	  throw std::logic_error(buffer.str());
   }

   file << write_expression();

   file.close();
}


// void save_data(const std::string&) const method

/// This method saves a set of input-output values from the neural network to a data file. 
/// @param filename Name of data file. 

void NeuralNetwork::save_data(const std::string& filename) const
{
   #ifdef _DEBUG 

   std::ostringstream buffer;

   if(!multilayer_perceptron_pointer)
   {
      buffer << "OpenNN Exception: NeuralNetwork class.\n"
             << "void save_data(const std::string&) const method.\n"
             << "Pointer to multilayer perceptron is NULL.\n";

	  throw std::logic_error(buffer.str());
   }

   #endif

   const unsigned int inputs_number = multilayer_perceptron_pointer->count_inputs_number();

   #ifdef _DEBUG 

   if(inputs_number != 1)
   {
      buffer << "OpenNN Exception: NeuralNetwork class.\n"
             << "void save_data(const std::string&) const method.\n"
             << "Number of inputs is not 1.\n";

	  throw std::logic_error(buffer.str());
   }

   #endif

   #ifdef _DEBUG 

   if(!scaling_layer_pointer)
   {
      buffer << "OpenNN Exception: NeuralNetwork class.\n"
             << "void save_data(const std::string&) const method.\n"
             << "Pointer to scaling layer is NULL.\n";

	  throw std::logic_error(buffer.str());
   }

   #endif

   const unsigned int outputs_number = multilayer_perceptron_pointer->count_outputs_number();

   const unsigned int variables_number = inputs_number + outputs_number;

   const Vector<double> inputs_minimums = scaling_layer_pointer->get_minimums();
   const Vector<double> inputs_maximums = scaling_layer_pointer->get_maximums();
 
   const unsigned int points_number = 101;

   Matrix<double> data(points_number, variables_number);

   Vector<double> inputs(inputs_minimums);
   Vector<double> outputs(outputs_number);
   Vector<double> row(variables_number);

   const Vector<double> increment = (inputs_maximums - inputs_minimums)/(double)(points_number-1.0);

   for(unsigned int i = 0; i < points_number; i++)
   {      
      outputs = calculate_outputs(inputs);

      row = inputs.get_assembly(outputs);

      data.set_row(i, row);

      inputs += increment;
   }

   data.save(filename);
}


//// TiXmlElement* report_parameters_XML(void) const method
//
//TiXmlElement* NeuralNetwork::report_parameters_XML(void) const
//{
//   TiXmlElement* report_parameters_element = new TiXmlElement("ReportParameters");
//
//   TiXmlElement* parameters_element = new TiXmlElement("Parameters");
//   report_parameters_element->LinkEndChild(parameters_element);
//
//   const Vector<double> parameters = arrange_parameters();
//
//   std::string parameters_string = parameters.to_string();
//
//   TiXmlText* parameters_text = new TiXmlText(parameters_string.c_str());
//   parameters_element->LinkEndChild(parameters_text);
//
//   return(report_parameters_element);
//}
//
//
//// TiXmlElement* report_parameters_number_XML(void) const method
//
//TiXmlElement* NeuralNetwork::report_parameters_number_XML(void) const
//{
//   TiXmlElement* report_parameters_number_element = new TiXmlElement("ReportParametersNumber");
//
//   TiXmlElement* parameters_number_element = new TiXmlElement("ParametersNumber");
//   report_parameters_number_element->LinkEndChild(parameters_number_element);
//
//   const unsigned int parameters_number = count_parameters_number();
//
//   std::ostringstream buffer;
//   buffer << parameters_number;
//
//   std::string parameters_string = buffer.str();
//
//   TiXmlText* parameters_text = new TiXmlText(parameters_string.c_str());
//   parameters_number_element->LinkEndChild(parameters_text);
//
//   return(report_parameters_number_element);
//}
//
//
//// TiXmlElement* calculate_parameters_norm_XML(void) const method
//
//TiXmlElement* NeuralNetwork::calculate_parameters_norm_XML(void) const
//{
//   TiXmlElement* calculate_parameters_norm_element = new TiXmlElement("CalculateParametersNorm");
//
//   TiXmlElement* parameters_norm_element = new TiXmlElement("ParametersNorm");
//   calculate_parameters_norm_element->LinkEndChild(parameters_norm_element);
//
//   const double parameters_norm = calculate_parameters_norm();
//
//   std::ostringstream buffer;
//   buffer << parameters_norm;
//
//   std::string parameters_norm_string = buffer.str();
//
//   TiXmlText* parameters_norm_text = new TiXmlText(parameters_norm_string.c_str());
//   parameters_norm_element->LinkEndChild(parameters_norm_text);
//
//   return(calculate_parameters_norm_element);
//}
//
//
//// TiXmlElement* calculate_output_XML(const Vector<double>&) const method
//
//TiXmlElement* NeuralNetwork::calculate_output_XML(const Vector<double>& inputs) const
//{
//   TiXmlElement* calculate_output_element = new TiXmlElement("CalculateOutput");
//
//   TiXmlElement* input_element = new TiXmlElement("Input");
//   calculate_output_element->LinkEndChild(input_element);
//
//   std::ostringstream buffer;
//   buffer << inputs;
//
//   std::string input_string = buffer.str();
//
//   TiXmlText* input_text = new TiXmlText(input_string.c_str());
//   input_element->LinkEndChild(input_text);
//
//   const Vector<double> outputs = calculate_outputs(inputs);
//
//   TiXmlElement* output_element = new TiXmlElement("Output");
//   calculate_output_element->LinkEndChild(output_element);
//
//   std::string output_string = outputs.to_string();
//
//   TiXmlText* output_text = new TiXmlText(output_string.c_str());
//   output_element->LinkEndChild(output_text);
//
//   return(calculate_output_element);
//}
//
//
//// TiXmlElement* calculate_output_data_XML(const Matrix<double>&) const method
//
//TiXmlElement* NeuralNetwork::calculate_output_data_XML(const Matrix<double>& input_data) const
//{
//   TiXmlElement* calculate_output_data_element = new TiXmlElement("CalculateOutputData");
//
//   TiXmlElement* input_data_element = new TiXmlElement("InputData");
//   calculate_output_data_element->LinkEndChild(input_data_element);
//
//   std::ostringstream buffer;
//   buffer << input_data;
//
//   std::string input_data_string = buffer.str();
//
//   TiXmlText* input_data_text = new TiXmlText(input_data_string.c_str());
//   input_data_element->LinkEndChild(input_data_text);
//
//   const Matrix<double> output_data = calculate_output_data(input_data);
//
//   TiXmlElement* output_data_element = new TiXmlElement("OutputData");
//   calculate_output_data_element->LinkEndChild(output_data_element);
//
//   std::string output_data_string = output_data.to_string();
//
//   TiXmlText* output_data_text = new TiXmlText(output_data_string.c_str());
//   output_data_element->LinkEndChild(output_data_text);
//
//   return(calculate_output_data_element);
//}
//
//
//// TiXmlElement* calculate_Jacobian_XML(const Vector<double>&) const method
//
//TiXmlElement* NeuralNetwork::calculate_Jacobian_XML(const Vector<double>& inputs) const
//{
//   TiXmlElement* calculate_Jacobian_element = new TiXmlElement("CalculateJacobian");
//
//   TiXmlElement* input_element = new TiXmlElement("Input");
//   calculate_Jacobian_element->LinkEndChild(input_element);
//
//   std::ostringstream buffer;
//   buffer << inputs;
//
//   std::string input_string = buffer.str();
//
//   TiXmlText* input_text = new TiXmlText(input_string.c_str());
//   input_element->LinkEndChild(input_text);
//
//   const Matrix<double> Jacobian = calculate_Jacobian(inputs);
//
//   TiXmlElement* Jacobian_element = new TiXmlElement("Jacobian");
//   calculate_Jacobian_element->LinkEndChild(Jacobian_element);
//
//   std::string Jacobian_string = Jacobian.to_string();
//
//   TiXmlText* Jacobian_text = new TiXmlText(Jacobian_string.c_str());
//   Jacobian_element->LinkEndChild(Jacobian_text);
//
//   return(calculate_Jacobian_element);
//}
//
//
//// TiXmlElement* calculate_parameters_Jacobian_XML(const Vector<double>&) const method
//
//TiXmlElement* NeuralNetwork::calculate_parameters_Jacobian_XML(const Vector<double>& inputs, const Vector<double>& parameters) const
//{
//   TiXmlElement* calculate_parameters_Jacobian_element = new TiXmlElement("CalculateParametersJacobian");
//
//   TiXmlElement* input_element = new TiXmlElement("Input");
//   calculate_parameters_Jacobian_element->LinkEndChild(input_element);
//
//   std::ostringstream buffer;
//   buffer << inputs;
//
//   const std::string input_string = buffer.str();
//
//   TiXmlText* input_text = new TiXmlText(input_string.c_str());
//   input_element->LinkEndChild(input_text);
//
//   const Matrix<double> parameters_Jacobian = calculate_parameters_Jacobian(inputs, parameters);
//
//   TiXmlElement* parameters_Jacobian_element = new TiXmlElement("ParametersJacobian");
//   calculate_parameters_Jacobian_element->LinkEndChild(parameters_Jacobian_element);
//
//   const std::string parameters_Jacobian_string = parameters_Jacobian.to_string();
//
//   TiXmlText* parameters_Jacobian_text = new TiXmlText(parameters_Jacobian_string.c_str());
//   parameters_Jacobian_element->LinkEndChild(parameters_Jacobian_text);
//
//   return(calculate_parameters_Jacobian_element);
//}
//
//
//// TiXmlElement* calculate_parameters_Jacobian_data_XML(const Matrix<double>&) const method
//
////TiXmlElement* NeuralNetwork::calculate_parameters_Jacobian_data_XML(const Matrix<double>& input_data, const Vector<double>& parameters) const
////{
////   TiXmlElement* calculate_parameters_Jacobian_data_element = new TiXmlElement("CalculateParametersJacobianData");
//
////   TiXmlElement* input_data_element = new TiXmlElement("InputData");
////   calculate_parameters_Jacobian_data_element->LinkEndChild(input_data_element);
//
////   std::ostringstream buffer;
////   buffer << input_data;
//
////   const std::string input_data_string = buffer.str();
//
////   TiXmlText* input_data_text = new TiXmlText(input_data_string);
////   input_data_element->LinkEndChild(input_data_text);
//
////   Matrix<double> parameters_Jacobian_data = calculate_parameters_Jacobian_data(input_data, parameters);
//
////   TiXmlElement* parameters_Jacobian_element = new TiXmlElement("ParametersJacobian");
////   calculate_parameters_Jacobian_element->LinkEndChild(parameters_Jacobian_element);
//
////   std::string parameters_Jacobian_string = parameters_Jacobian.to_string();
//
////   TiXmlText* parameters_Jacobian_text = new TiXmlText(parameters_Jacobian_string);
////   parameters_Jacobian_element->LinkEndChild(parameters_Jacobian_text);
//
////   return(calculate_parameters_Jacobian_data_element);
////}
//
//
//// TiXmlElement* calculate_Hessian_form_XML(const Vector<double>&) const method
//
//TiXmlElement* NeuralNetwork::calculate_Hessian_form_XML(const Vector<double>& inputs) const
//{
//   TiXmlElement* calculate_Hessian_form_element = new TiXmlElement("CalculateHessianForm");
//
//   TiXmlElement* input_element = new TiXmlElement("Input");
//   calculate_Hessian_form_element->LinkEndChild(input_element);
//
//   std::ostringstream buffer;
//   buffer << inputs;
//
//   const std::string input_string = buffer.str();
//
//   TiXmlText* input_text = new TiXmlText(input_string.c_str());
//   input_element->LinkEndChild(input_text);
//
//   const Vector< Matrix<double> > Hessian_form = calculate_Hessian_form(inputs);
//
//   TiXmlElement* Hessian_form_element = new TiXmlElement("HessianForm");
//   calculate_Hessian_form_element->LinkEndChild(Hessian_form_element);
//
//   std::string Hessian_form_string = Hessian_form.to_string();
//
//   TiXmlText* Hessian_form_text = new TiXmlText(Hessian_form_string.c_str());
//   Hessian_form_element->LinkEndChild(Hessian_form_text);
//
//   return(calculate_Hessian_form_element);
//}
//
//
//// TiXmlElement* calculate_Hessian_form_data_XML(const Matrix<double>&) const method
//
//TiXmlElement* NeuralNetwork::calculate_Hessian_form_data_XML(const Matrix<double>& input_data) const
//{
//   TiXmlElement* calculate_Hessian_form_data_element = new TiXmlElement("CalculateHessianFormData");
//
//   TiXmlElement* input_data_element = new TiXmlElement("InputData");
//   calculate_Hessian_form_data_element->LinkEndChild(input_data_element);
//
//   std::ostringstream buffer;
//   buffer << input_data;
//
//   std::string input_data_string = buffer.str();
//
//   TiXmlText* input_text = new TiXmlText(input_data_string.c_str());
//   input_data_element->LinkEndChild(input_text);
//
////   Vector< Matrix<double> > Hessian_form_data = calculate_Hessian_form(input_data);
//
////   TiXmlElement* Hessian_form_data_element = new TiXmlElement("HessianForm");
////   calculate_Hessian_form_element->LinkEndChild(Hessian_form_element);
//
////   std::string Hessian_form_string = Hessian_form.to_string();
//
////   TiXmlText* Hessian_form_text = new TiXmlText(Hessian_form_string);
////   Hessian_form_element->LinkEndChild(Hessian_form_text);
//
//   return(calculate_Hessian_form_data_element);
//}
//
//
//// TiXmlElement* calculate_parameters_Hessian_form_XML(const Vector<double>&, const Vector<double>&) const method
//
//TiXmlElement* NeuralNetwork::calculate_parameters_Hessian_form_XML(const Vector<double>& inputs, const Vector<double>& parameters) const
//{
//   TiXmlElement* calculate_parameters_Hessian_form_element = new TiXmlElement("CalculateParametersHessianForm");
//
//   TiXmlElement* input_element = new TiXmlElement("Input");
//   calculate_parameters_Hessian_form_element->LinkEndChild(input_element);
//
//   std::ostringstream buffer;
//   buffer << inputs;
//
//   std::string input_string = buffer.str();
//
//   TiXmlText* input_text = new TiXmlText(input_string.c_str());
//   input_element->LinkEndChild(input_text);
//
//   Vector< Matrix<double> > parameters_Hessian_form = calculate_parameters_Hessian_form(inputs, parameters);
//
//   TiXmlElement* parameters_Hessian_form_element = new TiXmlElement("ParametersHessianForm");
//   calculate_parameters_Hessian_form_element->LinkEndChild(parameters_Hessian_form_element);
//
//   std::string parameters_Hessian_form_string = parameters_Hessian_form.to_string();
//
//   TiXmlText* Hessian_form_text = new TiXmlText(parameters_Hessian_form_string.c_str());
//   parameters_Hessian_form_element->LinkEndChild(Hessian_form_text);
//
//   return(calculate_parameters_Hessian_form_element);
//}
//
//
//// TiXmlElement* calculate_parameters_Hessian_form_data_XML(const Matrix<double>& input_data, const Vector<double>&) const method
//
////TiXmlElement* NeuralNetwork::calculate_parameters_Hessian_form_data_XML(const Matrix<double>& input_data, const Vector<double>& parameters) const
////{
////   TiXmlElement* calculate_parameters_Hessian_form_data_element = new TiXmlElement("CalculateParametersHessianFormData");
//
////   TiXmlElement* input_data_element = new TiXmlElement("InputData");
////   calculate_parameters_Hessian_form_data_element->LinkEndChild(input_data_element);
//
////   std::ostringstream buffer;
////   buffer << input_data;
//
////   std::string input_data_string = buffer.str();
//
////   TiXmlText* input_text = new TiXmlText(input_data_string);
////   input_data_element->LinkEndChild(input_text);
//
////   return(calculate_parameters_Hessian_form_data_element);
////}
//
//
//// TiXmlElement* report_expression_XML(void) const method
//
//TiXmlElement* NeuralNetwork::report_expression_XML(void) const
//{
//   TiXmlElement* report_expression_element = new TiXmlElement("ReportExpression");
//
//   return(report_expression_element);
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
