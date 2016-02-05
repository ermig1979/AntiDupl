/****************************************************************************************************************/
/*                                                                                                              */
/*   OpenNN: Open Neural Networks Library                                                                       */
/*   www.opennn.cimne.com                                                                                       */
/*                                                                                                              */
/*   P A T T E R N   R E C O G N I T I O N   T E S T I N G   C L A S S                                          */
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
#include <string>
#include <sstream>
#include <cmath>

// OpenNN includes

#include "pattern_recognition_testing.h"

namespace OpenNN
{

// DEFAULT CONSTRUCTOR

/// Default constructor. It creates a pattern recognition utilites object 
/// not associated to any neural network neither any data set objects.

PatternRecognitionTesting::PatternRecognitionTesting(void)
 : neural_network_pointer(NULL),
   data_set_pointer(NULL)
{
}


// NEURAL NETWORK CONSTRUCTOR

/// Neural network constructor. 
/// It creates a pattern recognition utilites object associated to a neural network but not to a data set.
/// @param new_neural_network_pointer Pointer to a neural network object.

PatternRecognitionTesting::PatternRecognitionTesting(NeuralNetwork* new_neural_network_pointer)
 : neural_network_pointer(new_neural_network_pointer),
   data_set_pointer(NULL)
{
}


// DATA SET CONSTRUCTOR

/// Data set constructor. 
/// It creates a pattern recognition utilites object associated to a data set but not to a neural network.
/// @param new_data_set_pointer Pointer to a data set object.

PatternRecognitionTesting::PatternRecognitionTesting(DataSet* new_data_set_pointer)
 : neural_network_pointer(NULL),
   data_set_pointer(new_data_set_pointer)
{
}


// GENERAL CONSTRUCTOR

/// General constructor. 
/// It creates a pattern recognition testing object associated to a neural network and a data set.
/// @param new_neural_network_pointer Pointer to a neural network object.
/// @param new_data_set_pointer Pointer to a data set object.

PatternRecognitionTesting::PatternRecognitionTesting(NeuralNetwork* new_neural_network_pointer, DataSet* new_data_set_pointer)
 : neural_network_pointer(new_neural_network_pointer),
   data_set_pointer(new_data_set_pointer)
{
}


// XML CONSTRUCTOR

/// XML constructor. 
/// It creates a pattern recognition testing object neither associated to a neural network nor to a data set objects.
/// It also loads the members of this object from a TinyXML element. 
/// @param pattern_recognition_testing_element XML element which contains the member data.

PatternRecognitionTesting::PatternRecognitionTesting(TiXmlElement* pattern_recognition_testing_element)
 : neural_network_pointer(NULL),
   data_set_pointer(NULL)
{
   set_default();

   from_XML(pattern_recognition_testing_element);
}


// DESTRUCTOR

/// Destructor. 

PatternRecognitionTesting::~PatternRecognitionTesting(void)
{
}


// METHODS

// NeuralNetwork* get_neural_network_pointer(void) const method

/// This method returns a pointer to the neural network which is to be tested. 

NeuralNetwork* PatternRecognitionTesting::get_neural_network_pointer(void) const
{
   return(neural_network_pointer);
}


// DataSet* get_data_set_pointer(void) const method

/// This method returns a pointer to the data set object on which the neural network is tested. 

DataSet* PatternRecognitionTesting::get_data_set_pointer(void) const
{
   return(data_set_pointer);
}


// const bool& get_display(void) const method

/// This method returns true if messages from this class can be displayed on the screen,
/// or false if messages from this class can't be displayed on the screen.

const bool& PatternRecognitionTesting::get_display(void) const
{
   return(display);
}


// void set_neural_network_pointer(NeuralNetwork*) method

/// This method sets a new neural network to be tested. 
/// @param new_neural_network_pointer Pointer to the neural network object to be tested. 

void PatternRecognitionTesting::set_neural_network_pointer(NeuralNetwork* new_neural_network_pointer)
{
   neural_network_pointer = new_neural_network_pointer;
}


// void set_data_set_pointer(DataSet*) method

/// This method sets a new data set on which the neural network is to be tested. 
/// @param new_data_set_pointer Pointer to the data set object used for testing. 

void PatternRecognitionTesting::set_data_set_pointer(DataSet* new_data_set_pointer)
{
   data_set_pointer = new_data_set_pointer;
}


// void set_display(const bool&) method

/// This method sets a new display flag. 
/// @param new_display Flag for displaying messages from this class or not. 

void PatternRecognitionTesting::set_display(const bool& new_display)
{
   display = new_display;
}    


// void set_default(void) method

/// This method sets the default values from the pattern recognition testing class:
/// <ul>
/// <li> Display: True.
/// </ul>


void PatternRecognitionTesting::set_default(void)
{
   display = true;
}


// Matrix<bool> calculate_binary_target_data(void) const method

/// This method returns a matrix with the binary values of all the instances of the target variables in 
/// the data set.

Matrix<bool> PatternRecognitionTesting::calculate_binary_target_data(void) const
{
   // Control sentence (if debug)

   #ifdef _DEBUG 

   if(!data_set_pointer)
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: PatternRecognitionTesting class.\n"
             << "Matrix<bool> get_binary_target_data(void) const method.\n"
             << "Data set is NULL.\n";

      throw std::logic_error(buffer.str().c_str());	  
   }

   #endif

   // Data set stuff

   const unsigned int instances_number = data_set_pointer->get_instances_number();

   const VariablesInformation& variables_information = data_set_pointer->get_variables_information();

   const unsigned int targets_number = variables_information.count_targets_number();

   #ifdef _DEBUG 

   if(instances_number == 0)
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: PatternRecognitionTesting class.\n"
             << "Matrix<bool> get_binary_target_data(void) method."
             << "Number of instances must be greater than zero.";

      throw std::logic_error(buffer.str().c_str());	  
   }

   if(targets_number == 0)
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: PatternRecognitionTesting class.\n"
             << "Matrix<bool> get_binary_target_data(void) method.\n"
             << "Number of target variables must be greater than zero.\n";

      throw std::logic_error(buffer.str().c_str());	  
   }

   #endif


   // Calculate targets data 

   Matrix<bool> target_data(instances_number, targets_number, false);

   Vector<double> targets(targets_number);

   if(targets_number == 1)
   {
      for(unsigned int i = 0; i < instances_number; i++)
      {
         targets = data_set_pointer->get_target_instance(i);
     
         if(targets[0] > 0.5)
		 {
            target_data[i][0] = true;
		 }
      }                     
   }
   else if(targets_number > 1)
   {
      int winner_index;

      for(unsigned int i = 0; i < instances_number; i++)
      {
         targets = data_set_pointer->get_target_instance(i);
     
         winner_index = targets.calculate_maximal_index();

         target_data[i][winner_index] = true;
      }                  
   }
   
   return(target_data);
}


// Matrix<bool> calculate_binary_training_target_data(void) const method

/// This method returns a matrix with the binary values of the training instances of the target variables in 
/// the data set.

Matrix<bool> PatternRecognitionTesting::calculate_binary_training_target_data(void) const
{
   // Control sentence (if debug)

   #ifdef _DEBUG 

   if(!data_set_pointer)
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: PatternRecognitionTesting class.\n"
             << "Matrix<bool> get_binary_training_target_data(void) const method.\n"
             << "Data set is NULL.\n";

      throw std::logic_error(buffer.str().c_str());	  
   }

   #endif

   // Data set stuff

   const VariablesInformation& variables_information = data_set_pointer->get_variables_information();
   const InstancesInformation& instances_information = data_set_pointer->get_instances_information();

   const unsigned int training_instances_number = instances_information.count_training_instances_number();

   const unsigned int targets_number = variables_information.count_targets_number();

   #ifdef _DEBUG 

   if(training_instances_number == 0)
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: PatternRecognitionTesting class.\n"
             << "Matrix<bool> get_binary_training_target_data(void) method.\n"
             << "Number of training instances must be greater than zero.\n";

      throw std::logic_error(buffer.str().c_str());	  
   }

   if(targets_number == 0)
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: PatternRecognitionTesting class.\n"
             << "Matrix<bool> get_binary_training_target_data(void) method.\n"
             << "Number of target variables must be greater than zero.\n";

      throw std::logic_error(buffer.str().c_str());	  
   }

   #endif

   // Calculate training targets data 

   Matrix<bool> training_target_data(training_instances_number, targets_number, false);

   Vector<double> targets(targets_number);

   if(targets_number == 1)
   {
      for(unsigned int i = 0; i < training_instances_number; i++)
      {
         targets = data_set_pointer->get_training_target_instance(i);
     
         if(targets[0] > 0.5)
		 {
            training_target_data[i][0] = true;
		 }
      }                     
   }
   else if(targets_number > 1)
   {
      int winner_index;

      for(unsigned int i = 0; i < training_instances_number; i++)
      {
         targets = data_set_pointer->get_training_target_instance(i);
     
         winner_index = targets.calculate_maximal_index();

         training_target_data[i][winner_index] = true;
      }                  
   }
   
   return(training_target_data);
}


// Matrix<bool> calculate_binary_generalization_target_data(void) const method

/// This method returns a matrix with the binary values of the generalization instances of the target variables in the data set.

Matrix<bool> PatternRecognitionTesting::calculate_binary_generalization_target_data(void) const
{
   // Control sentence (if debug)

   #ifdef _DEBUG 

   if(!data_set_pointer)
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: PatternRecognitionTesting class.\n"
             << "Matrix<bool> calculate_binary_generalization_target_data(void) const method.\n"
             << "Data set is NULL.\n";

      throw std::logic_error(buffer.str().c_str());	  
   }

   #endif

   // Get generalization targets data

   const VariablesInformation variables_information = data_set_pointer->get_variables_information();
   const InstancesInformation instances_information = data_set_pointer->get_instances_information();

   const unsigned int generalization_instances_number = instances_information.count_generalization_instances_number();
             
   const unsigned int targets_number = variables_information.count_targets_number();

   #ifdef _DEBUG 

   if(generalization_instances_number == 0)
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: PatternRecognitionTesting class.\n"
             << "Matrix<bool> calculate_binary_generalization_target_data(void) const method.\n"
             << "Number of generalization instances must be greater than zero.\n";

      throw std::logic_error(buffer.str().c_str());	  
   }

   if(targets_number == 0)
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: PatternRecognitionTesting class.\n"
             << "Matrix<bool> calculate_binary_generalization_target_data(void) const method.\n"
             << "Number of target variables must be greater than zero.\n";

      throw std::logic_error(buffer.str().c_str());	  
   }

   #endif

   Matrix<bool> generalization_target_data(generalization_instances_number, targets_number, false);

   Vector<double> targets(targets_number);

   if(targets_number == 1)
   {
      for(unsigned int i = 0; i < generalization_instances_number; i++)
      {
         targets = data_set_pointer->get_generalization_target_instance(i);
     
         if(targets[0] > 0.5)
		 {
            generalization_target_data[i][0] = true;
		 }
      }                     
   }
   else if(targets_number > 1)
   {
      int winner_index;

      for(unsigned int i = 0; i < generalization_instances_number; i++)
      {
         targets = data_set_pointer->get_generalization_target_instance(i);
     
         winner_index = targets.calculate_maximal_index();

         generalization_target_data[i][winner_index] = true;
      }                  
   }
   
   return(generalization_target_data);
}


// Matrix<bool> calculate_binary_testing_target_data(void) const method

/// This method returns a matrix with the binary values of the testing instances of the target variables in the data set.

Matrix<bool> PatternRecognitionTesting::calculate_binary_testing_target_data(void) const
{
   // Control sentence (if debug)

   #ifdef _DEBUG 

   std::ostringstream buffer;

   if(!data_set_pointer)
   {
      buffer << "OpenNN Exception: PatternRecognitionTesting class.\n"
             << "Matrix<bool> calculate_binary_testing_target_data(void) const method.\n"
             << "Data set is NULL.\n";

      throw std::logic_error(buffer.str().c_str());	  
   }

   #endif

   // Get testing targets data

   const VariablesInformation& variables_information = data_set_pointer->get_variables_information();
   const InstancesInformation& instances_information = data_set_pointer->get_instances_information();

   const unsigned int testing_instances_number = instances_information.count_testing_instances_number();
             
   const unsigned int targets_number = variables_information.count_targets_number();

   // Control sentence (if debug)

   #ifdef _DEBUG 

   if(testing_instances_number == 0)
   {
      buffer << "OpenNN Exception: PatternRecognitionTesting class.\n"
             << "Matrix<bool> calculate_binary_testing_target_data(void) const method.\n"
             << "Number of testing instances must be greater than zero.\n";

      throw std::logic_error(buffer.str().c_str());	  
   }

   if(targets_number == 0)
   {
      buffer << "OpenNN Exception: PatternRecognitionTesting class.\n"
             << "Matrix<bool> calculate_binary_testing_target_data(void) const method.\n"
             << "Number of target variables must be greater than zero.\n";

      throw std::logic_error(buffer.str().c_str());	  
   }

   #endif

   Matrix<bool> testing_target_data(testing_instances_number, targets_number, false);

   Vector<double> targets(targets_number);

   if(targets_number == 1)
   {
      for(unsigned int i = 0; i < testing_instances_number; i++)
      {
         targets = data_set_pointer->get_testing_target_instance(i);
     
         if(targets[0] > 0.5)
         {
            testing_target_data[i][0] = true;
         }  
      }               
   }
   else if(targets_number > 1)
   {
      int winner_index;

      for(unsigned int i = 0; i < testing_instances_number; i++)
      {
         targets = data_set_pointer->get_testing_target_instance(i);
     
         winner_index = targets.calculate_maximal_index();

         testing_target_data[i][winner_index] = true;
      }               
   }
   
   return(testing_target_data);
}


// Matrix<bool> calculate_binary_output_data(void) const method

/// This method retuns a Matrix containing the binary neural network outputs for all the instances in the data set.

Matrix<bool> PatternRecognitionTesting::calculate_binary_output_data(void) const
{
   // Control sentence (if debug)

   #ifdef _DEBUG 

   std::ostringstream buffer;

   if(!data_set_pointer)
   {
      buffer << "OpenNN Exception: PatternRecognitionTesting class.\n"
             << "Matrix<bool> calculate_binary_output_data(void) const method.\n"
             << "Data set is NULL.\n";

      throw std::logic_error(buffer.str().c_str());	  
   }

   #endif

   // Neural network stuff 

   const MultilayerPerceptron* multilayer_perceptron_pointer = neural_network_pointer->get_multilayer_perceptron_pointer();

   const unsigned int multilayer_perceptron_inputs_number = multilayer_perceptron_pointer->count_inputs_number();
   const unsigned int outputs_number = multilayer_perceptron_pointer->count_outputs_number();

   #ifdef _DEBUG 

   const VariablesInformation& variables_information = data_set_pointer->get_variables_information();

   const unsigned int data_set_inputs_number = variables_information.count_inputs_number();
   const unsigned int targets_number = variables_information.count_targets_number();

   // Control sentence
   
   if(multilayer_perceptron_inputs_number != data_set_inputs_number)
   {
      buffer << "OpenNN Exception: PatternRecognitionTesting class.\n"
             << "Matrix<bool> calculate_binary_output_data(void) const method.\n"
             << "Number of inputs in neural network is not equal to number of inputs in data set.\n";

      throw std::logic_error(buffer.str().c_str());	  
   }
   else if(outputs_number != targets_number)
   {
      buffer << "OpenNN Exception: PatternRecognitionTesting class.\n"
             << "Matrix<bool> calculate_binary_output_data(void) const method.\n"
             << "Number of outputs in neural network is not equal to number of targets in data set.\n";

      throw std::logic_error(buffer.str().c_str());	  
   }

   #endif

   // Calculate outputs data

   const unsigned int instances_number = data_set_pointer->get_instances_number();

   Matrix<bool> output_data(instances_number, outputs_number, false);

   Vector<double> inputs(multilayer_perceptron_inputs_number);
   Vector<double> outputs(outputs_number);

   if(outputs_number == 1)
   {
      for(unsigned int i = 0; i < instances_number; i++)
      {
         inputs = data_set_pointer->get_input_instance(i);
     
         outputs = multilayer_perceptron_pointer->calculate_outputs(inputs);

         if(outputs[0] > 0.5)
		 {
            output_data[i][0] = true;
		 }
      }               
   }
   else
   {
      int winner_index;

      for(unsigned int i = 0; i < instances_number; i++)
      {
         inputs = data_set_pointer->get_input_instance(i);
     
         outputs = multilayer_perceptron_pointer->calculate_outputs(inputs);

         winner_index = outputs.calculate_maximal_index();

         output_data[i][winner_index] = true;
      }               
   }

   return(output_data);
}


// Matrix<bool> calculate_binary_training_output_data(void) const method

/// This method retuns a Matrix containing the binary neural network outputs for the training instances in the data set.

Matrix<bool> PatternRecognitionTesting::calculate_binary_training_output_data(void) const
{
   // Control sentence (if debug)

   #ifdef _DEBUG 

   if(!data_set_pointer)
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: PatternRecognitionTesting class.\n"
             << "Matrix<bool> calculate_binary_training_output_data(void) const method.\n"
             << "Data set is NULL.\n";

      throw std::logic_error(buffer.str().c_str());	  
   }

   #endif

   // Neural network stuff

   const MultilayerPerceptron* multilayer_perceptron_pointer = neural_network_pointer->get_multilayer_perceptron_pointer();

   const unsigned int multilayer_perceptron_inputs_number = multilayer_perceptron_pointer->count_inputs_number();
   const unsigned int outputs_number = multilayer_perceptron_pointer->count_outputs_number();


   #ifdef _DEBUG 

   const VariablesInformation& variables_information = data_set_pointer->get_variables_information();

   const unsigned int data_set_inputs_number = variables_information.count_inputs_number();
   const unsigned int targets_number = variables_information.count_targets_number();

   // Control sentence
   
   if(multilayer_perceptron_inputs_number != data_set_inputs_number)
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: PatternRecognitionTesting class.\n"
             << "Matrix<bool> calculate_binary_training_output_data(void) const method.\n"
             << "Number of inputs in neural network is not be equal to number of inputs in data set.\n";

      throw std::logic_error(buffer.str().c_str());	  
   }
   else if(outputs_number != targets_number)
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: PatternRecognitionTesting class.\n"
             << "Matrix<bool> calculate_binary_output_data(void) const method.\n"
             << "Number of outputs in neural netwoek is not equal to number of targets in data set.\n";

      throw std::logic_error(buffer.str().c_str());	  
   }

   #endif

   const InstancesInformation& instances_information = data_set_pointer->get_instances_information();

   const unsigned int training_instances_number = instances_information.count_training_instances_number();

   // Calculate training outputs data

   Matrix<bool> training_output_data(training_instances_number, outputs_number, false);

   Vector<double> inputs(multilayer_perceptron_inputs_number);
   Vector<double> outputs(outputs_number);

   if(outputs_number == 1)
   {
      for(unsigned int i = 0; i < training_instances_number; i++)
      {
         inputs = data_set_pointer->get_training_input_instance(i);
     
         outputs = multilayer_perceptron_pointer->calculate_outputs(inputs);

         if(outputs[0] > 0.5)
		 {
            training_output_data[i][0] = true;
		 }
      }               
   }
   else
   {
      unsigned int winner_index;

      for(unsigned int i = 0; i < training_instances_number; i++)
      {
         inputs = data_set_pointer->get_training_input_instance(i);
     
         outputs = multilayer_perceptron_pointer->calculate_outputs(inputs);

         winner_index = outputs.calculate_maximal_index();

         training_output_data[i][winner_index] = true;
      }               
   }

   return(training_output_data);
}



// Matrix<bool> calculate_binary_generalization_output_data(void) const method

/// This method retuns a Matrix containing the binary neural network outputs for the training instances in the data set.

Matrix<bool> PatternRecognitionTesting::calculate_binary_generalization_output_data(void) const
{
   // Control sentence (if debug)

   #ifdef _DEBUG 

   std::ostringstream buffer;

   if(!data_set_pointer)
   {
      buffer << "OpenNN Exception: PatternRecognitionTesting class.\n"
             << "Matrix<bool> calculate_binary_generalization_output_data(void) const method.\n"
             << "Data set is NULL.\n";

      throw std::logic_error(buffer.str().c_str());	  
   }

   #endif
   
   const MultilayerPerceptron* multilayer_perceptron_pointer = neural_network_pointer->get_multilayer_perceptron_pointer();

   const unsigned int multilayer_perceptron_inputs_number = multilayer_perceptron_pointer->count_inputs_number();
   const unsigned int outputs_number = multilayer_perceptron_pointer->count_outputs_number();

   #ifdef _DEBUG 

   const VariablesInformation& variables_information = data_set_pointer->get_variables_information();

   const unsigned int data_set_inputs_number = variables_information.count_inputs_number();
   const unsigned int targets_number = variables_information.count_targets_number();

   // Control sentence
   
   if(multilayer_perceptron_inputs_number != data_set_inputs_number)
   {
      buffer << "OpenNN Exception: PatternRecognitionTesting class." << std::endl 
             << "Matrix<bool> calculate_binary_generalization_output_data(void) const method." << std::endl
             << "Number of inputs in neural network is not equal to number of inputs in data set." << std::endl;

      throw std::logic_error(buffer.str().c_str());	  
   }
   else if(outputs_number != targets_number)
   {
      buffer << "OpenNN Exception: PatternRecognitionTesting class." << std::endl
             << "Matrix<bool> calculate_binary_generalization_output_data(void) const method." << std::endl
             << "Number of outputs in neural network is not equal to number of targets in data set." << std::endl;

      throw std::logic_error(buffer.str().c_str());	  
   }

   #endif

   // Calculate generalization outputs data

   const InstancesInformation& instances_information = data_set_pointer->get_instances_information();

   const unsigned int generalization_instances_number = instances_information.count_generalization_instances_number();

   Matrix<bool> generalization_output_data(generalization_instances_number, outputs_number, false);

   Vector<double> inputs(multilayer_perceptron_inputs_number);
   Vector<double> outputs(outputs_number);

   if(outputs_number == 1)
   {
      for(unsigned int i = 0; i < generalization_instances_number; i++)
      {
         inputs = data_set_pointer->get_generalization_input_instance(i);
     
         outputs = multilayer_perceptron_pointer->calculate_outputs(inputs);

         if(outputs[0] > 0.5)
		 {
            generalization_output_data[i][0] = true;
		 }
      }               
   }
   else
   {
      int winner_index;

      for(unsigned int i = 0; i < generalization_instances_number; i++)
      {
         inputs = data_set_pointer->get_generalization_input_instance(i);
     
         outputs = multilayer_perceptron_pointer->calculate_outputs(inputs);

         winner_index = outputs.calculate_maximal_index();

         generalization_output_data[i][winner_index] = true;
      }               
   }
   
   return(generalization_output_data);
}


// Matrix<bool> calculate_binary_testing_output_data(void) const method

/// This method retuns a Matrix containing the binary neural network outputs for the training instances in 
/// the data set.

Matrix<bool> PatternRecognitionTesting::calculate_binary_testing_output_data(void) const
{
   // Control sentence (if debug)

   #ifdef _DEBUG 

   std::ostringstream buffer;

   if(!data_set_pointer)
   {
      buffer << "OpenNN Exception: PatternRecognitionTesting class." << std::endl 
             << "Matrix<bool> calculate_binary_testing_output_data(void) const method." << std::endl
             << "Data set is NULL." << std::endl;

      throw std::logic_error(buffer.str().c_str());	  
   }

   #endif
            
   const MultilayerPerceptron* multilayer_perceptron_pointer = neural_network_pointer->get_multilayer_perceptron_pointer();

   unsigned int inputs_number = multilayer_perceptron_pointer->count_inputs_number();
   unsigned int outputs_number = multilayer_perceptron_pointer->count_outputs_number();


   #ifdef _DEBUG 

   const VariablesInformation variables_information = data_set_pointer->get_variables_information();

   // Control sentence
   
   if(inputs_number != variables_information.count_inputs_number())
   {
      buffer << "OpenNN Exception: PatternRecognitionTesting class." << std::endl 
             << "Matrix<bool> calculate_binary_testing_output_data(void) const method." << std::endl
             << "Number of inputs in neural network is not equal to number of inputs in testing data set." << std::endl;

      throw std::logic_error(buffer.str().c_str());	  
   }
   
   if(outputs_number != variables_information.count_targets_number())
   {
      buffer << "OpenNN Exception: PatternRecognitionTesting class." << std::endl
             << "Matrix<bool> calculate_binary_testing_output_data(void) const method." << std::endl
             << "Number of outputs in neural network is not equal to number of targets in data set." << std::endl;

      throw std::logic_error(buffer.str().c_str());	  
   }

   #endif

   const InstancesInformation instances_information = data_set_pointer->get_instances_information();

   const unsigned int testing_instances_number = instances_information.count_testing_instances_number();

   // Calculate testing outputs data

   Matrix<bool> testing_output_data(testing_instances_number, outputs_number, false);

   Vector<double> inputs(inputs_number);
   Vector<double> outputs(outputs_number);

   if(outputs_number == 1)
   {
      for(unsigned int i = 0; i < testing_instances_number; i++)
      {
         inputs = data_set_pointer->get_testing_input_instance(i);
     
         outputs = multilayer_perceptron_pointer->calculate_outputs(inputs);

         if(outputs[0] > 0.5)
		 {
            testing_output_data[i][0] = true;
		 }
      }               
   }
   else
   {
      int winner_index;

      for(unsigned int i = 0; i < testing_instances_number; i++)
      {
         inputs = data_set_pointer->get_testing_input_instance(i);
     
         outputs = multilayer_perceptron_pointer->calculate_outputs(inputs);

         winner_index = outputs.calculate_maximal_index();

         testing_output_data[i][winner_index] = true;
      }               
   }
   
   return(testing_output_data);
}



// Matrix<unsigned int> calculate_confusion(void) const method

/// This method returns the confusion matrix of a neural network on the testing instances of a data set. 

Matrix<unsigned int> PatternRecognitionTesting::calculate_confusion(void) const
{
   #ifdef _DEBUG 

   std::ostringstream buffer;

   if(!neural_network_pointer)
   {
      buffer << "OpenNN Exception: PatternRecognitionTesting class.\n"
             << "Matrix<unsigned int> calculate_confusion(void) const method.\n"
             << "Pointer to neural network is NULL.\n";

      throw std::logic_error(buffer.str().c_str());	  
   }

   #endif
             
   const MultilayerPerceptron* multilayer_perceptron_pointer = neural_network_pointer->get_multilayer_perceptron_pointer();

   #ifdef _DEBUG 

   if(!multilayer_perceptron_pointer)
   {
      buffer << "OpenNN Exception: PatternRecognitionTesting class.\n"
             << "Matrix<unsigned int> calculate_confusion(void) const method.\n"
             << "Pointer to multilayer perceptron in neural network is NULL.\n";

      throw std::logic_error(buffer.str().c_str());	  
   }

   #endif

   const unsigned int outputs_number = multilayer_perceptron_pointer->count_outputs_number();

   #ifdef _DEBUG 

   if(!data_set_pointer)
   {
      buffer << "OpenNN Exception: PatternRecognitionTesting class." << std::endl 
             << "Matrix<unsigned int> calculate_confusion(void) const method." << std::endl
             << "Pointer to data set is NULL." << std::endl;

      throw std::logic_error(buffer.str().c_str());	  
   }

   #endif

   #ifdef _DEBUG 

   const unsigned int inputs_number = multilayer_perceptron_pointer->count_inputs_number();

   // Control sentence
   
   const VariablesInformation& variables_information = data_set_pointer->get_variables_information();


   if(inputs_number != variables_information.count_inputs_number())
   {
	   buffer << "OpenNN Exception: PatternRecognitionTesting class." << std::endl 
             << "Matrix<unsigned int> calculate_confusion(void) method." << std::endl
             << "Number of inputs in neural network must be equal to number of inputs in data set." << std::endl;

      throw std::logic_error(buffer.str().c_str());	  
   }

   if(outputs_number != variables_information.count_targets_number())
   {
      buffer << "OpenNN Exception: PatternRecognitionTesting class." << std::endl 
             << "Matrix<unsigned int> calculate_confusion(void) method." << std::endl
             << "Number of outputs in neural network must be equal to number of targets in data set." << std::endl;

      throw std::logic_error(buffer.str().c_str());	  
   }

   #endif

   const InstancesInformation& instances_information = data_set_pointer->get_instances_information();

   const unsigned int testing_instances_number = instances_information.count_testing_instances_number();

   const Matrix<bool> binary_testing_target_data = calculate_binary_testing_target_data();
   const Matrix<bool> binary_testing_output_data = calculate_binary_testing_output_data();

   if(outputs_number == 1) // Binary classification 
   {
      Matrix<unsigned int> confusion(2, 2);
      unsigned int true_positive = 0;
	  unsigned int false_positive = 0;
	  unsigned int false_negative = 0;
	  unsigned int true_negative = 0;

      for(unsigned int i = 0; i < testing_instances_number; i++)
	  {
	     if(binary_testing_output_data[0][i] == true && binary_testing_target_data[0][i] == true)
		 {
		    true_positive++;
		 }
		 else if(binary_testing_output_data[0][i] == true && binary_testing_target_data[0][i] == false)
		 {
		    false_positive++;
		 }
		 else if(binary_testing_output_data[0][i] == false && binary_testing_target_data[0][i] == true)
		 {
		    false_negative++;
		 }
		 else if(binary_testing_output_data[0][i] == false && binary_testing_target_data[0][i] == false)
		 {
		    true_negative++;
		 }
		 else
		 {
            std::ostringstream buffer;

            buffer << "OpenNN Exception: PatternRecognitionTesting class." << std::endl 
                   << "Matrix<unsigned int> calculate_confusion(void) method." << std::endl
				   << "Unknown condition." << std::endl; 

            throw std::logic_error(buffer.str().c_str());	        
		 }

         confusion[0][0] = true_positive;
         confusion[0][1] = false_positive;
         confusion[1][0] = false_negative;
         confusion[1][1] = true_negative;
	  }

      return(confusion);	  
   }
   else
   {
      Matrix<unsigned int> confusion(outputs_number, outputs_number, 0);

      unsigned int predicted_class = 0;
	  unsigned int actual_class = 0;

      for(unsigned int i = 0; i < testing_instances_number; i++)
	  {         
	     for(unsigned int j = 0; j < outputs_number; j++)
		 {
		    if(binary_testing_output_data[i][j] == true)
			{
			   predicted_class = j;
			}
		    if(binary_testing_target_data[i][j] == true)
			{
			   actual_class = j;
			}			
		 }

		 confusion[predicted_class][actual_class] += 1;
	  }

      return(confusion);	  
   }
}


// Vector<double> calculate_binary_classification_test(void) method

/// This method returns the results of a binary classification test in a single vector.
/// The size of that vector is six.
/// The elements are:
/// <ul>
/// <li> Classification accuracy
/// <li> Error rate
/// <li> Sensitivity
/// <li> Specifity
/// <li> Positive likelihood
/// <li> Negative likelihood
/// </ul>

Vector<double> PatternRecognitionTesting::calculate_binary_classification_test(void) const
{
   // Control sentence (if debug)

   #ifdef _DEBUG 

   const MultilayerPerceptron* multilayer_perceptron_pointer = neural_network_pointer->get_multilayer_perceptron_pointer();

   const unsigned int inputs_number = multilayer_perceptron_pointer->count_inputs_number();

   if(!data_set_pointer)
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: PatternRecognitionTesting class." << std::endl 
             << "Vector<double> calculate_binary_classification_test(void) const." << std::endl
             << "Data set is NULL." << std::endl;

      throw std::logic_error(buffer.str().c_str());	  
   }

   const VariablesInformation& variables_information = data_set_pointer->get_variables_information();

   const unsigned int targets_number = variables_information.count_targets_number();

   const unsigned int outputs_number = multilayer_perceptron_pointer->count_outputs_number();

   // Control sentence
   
   if(inputs_number != variables_information.count_inputs_number())
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: PatternRecognitionTesting class." << std::endl 
             << "Vector<double> calculate_binary_classification_test(void) const." << std::endl
             << "Number of inputs in neural network is not equal to number of inputs in data set." << std::endl;

      throw std::logic_error(buffer.str().c_str());	  
   }
   else if(outputs_number != 1)
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: PatternRecognitionTesting class." << std::endl
             << "Vector<double> calculate_binary_classification_test(void) const." << std::endl
             << "Number of outputs in neural network must be one." << std::endl;

      throw std::logic_error(buffer.str().c_str());	  
   }
   else if(targets_number != 1)
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: PatternRecognitionTesting class." << std::endl
             << "Vector<double> calculate_binary_classification_test(void) const." << std::endl
             << "Number of targets in data set must be one." << std::endl;

      throw std::logic_error(buffer.str().c_str());	  
   }

   #endif

   // Confusion matrix

   const Matrix<unsigned int> confusion = calculate_confusion();

   const unsigned int true_positive = confusion[0][0];
   const unsigned int false_positive = confusion[0][1];
   const unsigned int false_negative = confusion[1][0];
   const unsigned int true_negative = confusion[1][1];

   // Classification accuracy

   const double classification_accuracy = (double)(true_positive + true_negative)/double(true_positive + true_negative + false_positive + false_negative);

   // Error rate

   const double error_rate = (double)(false_positive + false_negative)/(double)(true_positive + true_negative + false_positive + false_negative);

   // Sensitivity

   const double sensitivity = (double)true_positive/(double)(true_positive + false_negative);

   // Specifity

   const double specifity = (double)true_negative/(double)(true_negative + false_positive);

   // Positive likelihood

   const double positive_likelihood = sensitivity/(1.0 - specifity);

   // Negative likelihood

   const double negative_likelihood = specifity/(1.0 - sensitivity);

   Vector<double> binary_classification_test(6);
   binary_classification_test[0] = classification_accuracy;
   binary_classification_test[1] = error_rate;
   binary_classification_test[2] = sensitivity;
   binary_classification_test[3] = specifity;
   binary_classification_test[4] = positive_likelihood;
   binary_classification_test[5] = negative_likelihood;

   return(binary_classification_test);
}


// void save_binary_classification_test(const std::string&) const method

/// This method computes the binary classification test values and saves them into a data file. 
/// @param filename Name of binary classification tests file. 

void PatternRecognitionTesting::save_binary_classification_test(const std::string& filename) const
{
   std::ofstream file(filename.c_str()); 
    
   if(!file.is_open())
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: Matrix template." << std::endl
             << "void save_binary_classification_test(const std::string&) const method." << std::endl
             << "Cannot open binary classification test data file." << std::endl;

	  throw std::logic_error(buffer.str());
   }

   const Vector<double> binary_classification_test = calculate_binary_classification_test();

   file << "Binary classification test\n"
        << "Classification accuracy: " << binary_classification_test[0] << "\n" 
	    << "Error rate: " << binary_classification_test[1] << "\n" 
	    << "Sensitivity: " << binary_classification_test[2] << "\n" 
	    << "Specifity: " << binary_classification_test[3] << "\n" 
	    << "Positive likelihood: " << binary_classification_test[4] << "\n" 
	    << "Negative likelihood: " << binary_classification_test[5] << std::endl; 
 
   file.close();
}


// void save_confusion(const std::string&) const method

/// This method computes the confusion matrix and saves it into a data file. 
/// @param filename Name of confusion matrix file. 

void PatternRecognitionTesting::save_confusion(const std::string& filename) const
{
   std::ofstream file(filename.c_str()); 
    
   if(!file.is_open())
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: Matrix template." << std::endl
             << "void save_confusion(const std::string&) const method." << std::endl
             << "Cannot open confusion data file." << std::endl;

	  throw std::logic_error(buffer.str());
   }

   const Matrix<unsigned int> confusion = calculate_confusion();

   file << "Confusion\n"
        << confusion;
 
   file.close();
}

// std::string to_string(void) const method

/// This method returns a string representation of the current pattern recognition testing object. 

std::string PatternRecognitionTesting::to_string(void) const
{
   std::ostringstream buffer;

   buffer << "Pattern recognition testing\n"
          << "Display: " << display << "\n";

   return(buffer.str());
}


// TiXmlElement* to_XML(void) const method

/// This method serializes the pattern recognition testing object into a XML element of the TinyXML library. 
/// See the OpenNN manual for more information about the format of this element. 
/// @todo

TiXmlElement* PatternRecognitionTesting::to_XML(void) const
{
   return(NULL);
}


// void from_XML(TiXmlElement*) method

/// @todo

void PatternRecognitionTesting::from_XML(TiXmlElement*)
{
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

