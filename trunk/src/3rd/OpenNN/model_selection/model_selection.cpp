/****************************************************************************************************************/
/*                                                                                                              */
/*   OpenNN: Open Neural Networks Library                                                                       */
/*   www.opennn.cimne.com                                                                                       */
/*                                                                                                              */
/*   M O D E L   S E L E C T I O N   C L A S S                                                                  */
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
#include <ctime>

// OpenNN includes


#include "../performance_functional/performance_functional.h"

#include "model_selection.h"

namespace OpenNN
{

// DEFAULT CONSTRUCTOR

/// Default constructor. 

ModelSelection::ModelSelection(void)
 : training_strategy_pointer(NULL)
{
   set_default();
}


// TRAINING STRATEGY CONSTRUCTOR

/// Training strategy constructor. 
/// @param new_training_strategy_pointer Pointer to a gradient descent object.

ModelSelection::ModelSelection(TrainingStrategy* new_training_strategy_pointer)
 : training_strategy_pointer(new_training_strategy_pointer)
{
   set_default();
}


// FILE CONSTRUCTOR

/// File constructor. 
/// @param filename Name of XML model selection file. 

ModelSelection::ModelSelection(const std::string& filename)
 : training_strategy_pointer(NULL)
{
   load(filename);
}


// DESTRUCTOR

/// Destructor. 

ModelSelection::~ModelSelection(void)
{
}


// METHODS

// TrainingStrategy* get_training_strategy_pointer(void) const method

/// This method returns a pointer to the training strategy object.

TrainingStrategy* ModelSelection::get_training_strategy_pointer(void) const
{
   return(training_strategy_pointer);   
}


// const Vector<unsigned int>& get_hidden_perceptrons_numbers(void) const method

/// This method returns the complexities of the neural networks given by the numbers of hidden perceptrons. 

const Vector<unsigned int>& ModelSelection::get_hidden_perceptrons_numbers(void) const
{
   return(hidden_perceptrons_numbers);
}


// const unsigned int& get_parameters_assays_number(void) const method

/// This method returns the number of trials for each network architecture. 

const unsigned int& ModelSelection::get_parameters_assays_number(void) const
{
   return(parameters_assays_number);
}


// const bool& get_reserve_parameters_data(void) method

/// This method returns true if the neural network parameters are to be reserved, and false otherwise. 

const bool& ModelSelection::get_reserve_parameters_data(void)
{
   return(reserve_parameters_data);
}


// const bool& get_reserve_evaluation_data(void) method

/// This method returns true if the performance functional evaluations are to be reserved, and false otherwise. 

const bool& ModelSelection::get_reserve_evaluation_data(void)
{
   return(reserve_evaluation_data);
}


// const bool& get_reserve_generalization_evaluation_data(void) method

/// This method returns true if the performance functional generalization evaluations are to be reserved, and false otherwise. 

const bool& ModelSelection::get_reserve_generalization_evaluation_data(void)
{
   return(reserve_generalization_evaluation_data);
}


// const bool& get_reserve_minimal_parameters(void) method

/// This method returns true if the parameters vector of the neural network with minimum performance functional evaluation is to be reserved, and false otherwise. 

const bool& ModelSelection::get_reserve_minimal_parameters(void)
{
   return(reserve_minimal_parameters);
}


// const bool& get_reserve_evaluation_data_statistics(void) method

/// This method returns true if the statistics of the evaluation data are to be inclued in the model selection results. 

const bool& ModelSelection::get_reserve_evaluation_data_statistics(void)
{
   return(reserve_evaluation_data_statistics);
}


// const bool& get_reserve_generalization_evaluation_data_statistics(void) method

/// This method returns true if the statistics of the generalization evaluation data are to be inclued in the model selection results. 

const bool& ModelSelection::get_reserve_generalization_evaluation_data_statistics(void)
{
   return(reserve_generalization_evaluation_data_statistics);
}


// const bool& get_reserve_model_order_selection_plot(void) method

/// This method returns true if the necessary data for plotting the model selection results is to be included in the results.

const bool& ModelSelection::get_reserve_model_order_selection_plot(void)
{
   return(reserve_model_order_selection_plot);
}


// const bool& get_display(void) const method

/// This method returns true if messages from this class can be displayed on the screen,
/// or false if messages from this class can't be displayed on the screen.

const bool& ModelSelection::get_display(void) const
{
   return(display);     
}


// void set_training_strategy_pointer(TrainingStrategy*) method

/// This method sets a new training strategy pointer.
/// @param new_training_strategy_pointer Pointer to a training strategy object.

void ModelSelection::set_training_strategy_pointer(TrainingStrategy* new_training_strategy_pointer)
{
   training_strategy_pointer = new_training_strategy_pointer;   
}


// void set_default(void) method

/// @todo

void ModelSelection::set_default(void)
{
//   set_assays_numbers(5, 3);

   display = true;
}


// void set_hidden_perceptrons_numbers(const Vector<unsigned int>&) method

/// This method sets the number of complexities to be compared in the model order selection process.
/// @param new_hidden_perceptrons_numbers Vector with different hidden layers sizes. 

void ModelSelection::set_hidden_perceptrons_numbers(const Vector<unsigned int>& new_hidden_perceptrons_numbers)
{
   hidden_perceptrons_numbers = new_hidden_perceptrons_numbers;
}


// void set_parameters_assays_number(unsigned int) method

/// This method sets the number of times that each different neural network is to be trained.
/// @param new_parameters_assays_number Number of assays for each set of parameters. 

void ModelSelection::set_parameters_assays_number(const unsigned int& new_parameters_assays_number) 
{
   parameters_assays_number = new_parameters_assays_number;
}


// void set_assays_numbers(const unsigned int&, const unsigned int&) method

/// This method sets the numbers of complexities and assays. 
/// @param new_complexity_assays_number Number of hidden neurons. 
/// @param new_parameters_assays_number Number of trainings for each different neural network. 

void ModelSelection::set_assays_numbers(const unsigned int& new_complexity_assays_number, const unsigned int& new_parameters_assays_number)
{
   // Control sentence (if debug)

   #ifdef _DEBUG 

   if(!training_strategy_pointer) 
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: ModelSelection class.\n"
             << "void set_assays_numbers(const unsigned int&, const unsigned int&) method.\n"
             << "Pointer to training strategy is NULL.\n";

      throw std::logic_error(buffer.str().c_str());
   }   

   if(training_strategy_pointer->get_performance_functional_pointer() == NULL) 
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: ModelSelection class.\n"
             << "void set_assays_numbers(const unsigned int&, const unsigned int&) method.\n"
             << "Pointer to performance functional is NULL.\n";      

      throw std::logic_error(buffer.str().c_str());
   }   

   if(training_strategy_pointer->get_performance_functional_pointer()->get_neural_network_pointer() == NULL) 
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: ModelSelection class.\n"
             << "void set_assays_numbers(const unsigned int&, const unsigned int&) method.\n"
             << "Pointer to multilayer perceptron is NULL.\n";

      throw std::logic_error(buffer.str().c_str());
   }   

   #endif
   
   hidden_perceptrons_numbers.set(new_complexity_assays_number);

//   unsigned int inputs_number = training_strategy_pointer->get_performance_functional_pointer()->get_multilayer_perceptron_pointer()->count_inputs_number();
//   unsigned int outputs_number = training_strategy_pointer->get_performance_functional_pointer()->get_multilayer_perceptron_pointer()->count_outputs_number();

//   unsigned int minimum_hidden_layer_size = (int)sqrt((double)(inputs_number*inputs_number) + (double)(outputs_number*outputs_number));
//   unsigned int maximum_hidden_layer_size = 2*(int)sqrt((double)(inputs_number*inputs_number) + (double)(outputs_number*outputs_number));


   unsigned int minimum_hidden_layer_size = 1;
   unsigned int maximum_hidden_layer_size = 12;

   int hidden_layer_size_increment = (maximum_hidden_layer_size-minimum_hidden_layer_size)/new_complexity_assays_number;

   if(hidden_layer_size_increment < 1)
   {
      hidden_layer_size_increment = 1;
   }

   for(unsigned int i = 0; i < new_complexity_assays_number; i++)
   {
      hidden_perceptrons_numbers[i] = minimum_hidden_layer_size + hidden_layer_size_increment*i;  
   } 

   parameters_assays_number = new_parameters_assays_number;
}


// void set_reserve_parameters_data(const bool&)

/// This method sets the reserve flag for the parameters data. 
/// @param new_reserve_parameters_data Flag value. 

void ModelSelection::set_reserve_parameters_data(const bool& new_reserve_parameters_data)
{
   reserve_parameters_data = new_reserve_parameters_data;
}


// void set_reserve_evaluation_data(const bool&)

/// This method sets the reserve flag for the evaluation data. 
/// @param new_reserve_evaluation_data Flag value. 

void ModelSelection::set_reserve_evaluation_data(const bool& new_reserve_evaluation_data)
{
   reserve_evaluation_data = new_reserve_evaluation_data;
}


// void set_reserve_generalization_evaluation_data(const bool&)

/// This method sets the reserve flag for the generalization evaluation data. 
/// @param new_reserve_generalization_evaluation_data Flag value. 

void ModelSelection::set_reserve_generalization_evaluation_data(const bool& new_reserve_generalization_evaluation_data)
{
   reserve_generalization_evaluation_data = new_reserve_generalization_evaluation_data;
}


// void set_reserve_minimal_parameters(const bool&)

/// This method sets the reserve flag for the minimal parameters. 
/// @param new_reserve_minimal_parameters Flag value. 

void ModelSelection::set_reserve_minimal_parameters(const bool& new_reserve_minimal_parameters)
{
   reserve_minimal_parameters = new_reserve_minimal_parameters;
}


// void set_reserve_evaluation_data_statistics(const bool&)

/// This method sets the reserve flag for the evaluation data statistics. 
/// @param new_reserve_evaluation_data_statistics Flag value. 

void ModelSelection::set_reserve_evaluation_data_statistics(const bool& new_reserve_evaluation_data_statistics)
{
   reserve_evaluation_data_statistics = new_reserve_evaluation_data_statistics;
}


// void set_reserve_generalization_evaluation_data_statistics(const bool&)

/// This method sets the reserve flag for the generalization evaluation data statistics. 
/// @param new_reserve_generalization_evaluation_data_statistics Flag value. 

void ModelSelection::set_reserve_generalization_evaluation_data_statistics(const bool& new_reserve_generalization_evaluation_data_statistics)
{
   reserve_generalization_evaluation_data_statistics = new_reserve_generalization_evaluation_data_statistics;
}


// void set_reserve_model_order_selection_plot(const bool&)

/// This method sets the reserve flag for the model order selection plot. 
/// @param new_reserve_model_order_selection_plot Flag value. 

void ModelSelection::set_reserve_model_order_selection_plot(const bool& new_reserve_model_order_selection_plot)
{
   reserve_model_order_selection_plot = new_reserve_model_order_selection_plot;
}


// void set_display(const bool&) method

/// This method sets a new display value. 
/// If it is set to true messages from this class are to be displayed on the screen;
/// if it is set to false messages from this class are not to be displayed on the screen.
/// @param new_display Display value.

void ModelSelection::set_display(const bool& new_display)
{
   display = new_display;
}


// void check(void) const method

/// This method checks that the different pointers needed for performing the model selection are not NULL.

void ModelSelection::check(void) const
{
   // Training algorithm stuff

  std::ostringstream buffer;

   if(!training_strategy_pointer) 
   {
      buffer << "OpenNN Exception: ModelSelection class.\n"
             << "void check(void) const method.\n"
             << "Pointer to training strategy is NULL.\n";

      throw std::logic_error(buffer.str().c_str());
   }   

   // Performance functional stuff

   const PerformanceFunctional* performance_functional_pointer = training_strategy_pointer->get_performance_functional_pointer();

   if(!performance_functional_pointer) 
   {
      buffer << "OpenNN Exception: ModelSelection class.\n"
             << "void check(void) const method.\n"
             << "Pointer to performance functional is NULL.\n";      

      throw std::logic_error(buffer.str().c_str());
   }   

   // Neural network stuff

   const NeuralNetwork* neural_network_pointer = performance_functional_pointer->get_neural_network_pointer(); 

   if(!neural_network_pointer) 
   {
      buffer << "OpenNN Exception: ModelSelection class.\n"
             << "void check(void) const method.\n"
             << "Pointer to neural network is NULL.\n";

      throw std::logic_error(buffer.str().c_str());
   }   

   const MultilayerPerceptron* multilayer_perceptron_pointer = neural_network_pointer->get_multilayer_perceptron_pointer();

   if(!multilayer_perceptron_pointer) 
   {
      buffer << "OpenNN Exception: ModelSelection class.\n"
             << "void check(void) const method.\n"
             << "Pointer to multilayer perceptron is NULL.\n";

      throw std::logic_error(buffer.str().c_str());
   }   

}


// ModelSelectionResults perform_model_inputs_selection(void) const method

/// @todo

ModelSelection::ModelSelectionResults ModelSelection::perform_model_inputs_selection(void) const
{
   std::ostringstream buffer;

   buffer << "OpenNN Exception: ModelSelection class.\n"
          << "ModelSelectionResults perform_model_inputs_selection(void) const method.\n"
          << "This method is under development.\n";

   throw std::logic_error(buffer.str().c_str());
/*
   #ifdef _DEBUG 

   check();

   #endif

   // Training strategy stuff

   // Performance functional stuff

   const PerformanceFunctional* performance_functional_pointer = training_strategy_pointer->get_performance_functional_pointer();

   // Neural network stuff

   NeuralNetwork* neural_network_pointer = performance_functional_pointer->get_neural_network_pointer(); 

   MultilayerPerceptron* multilayer_perceptron_pointer = neural_network_pointer->get_multilayer_perceptron_pointer();

   #ifdef _DEBUG 

   const unsigned int layers_number = multilayer_perceptron_pointer->count_layers_number();

   if(layers_number != 2) 
   {
     std::ostringstream buffer;

      buffer << "OpenNN Exception: ModelSelection class.\n"
             << "ModelSelectionResults perform_model_inputs_selection(void) method.\n"
             << "Number of layers in multilayer perceptron must be two.\n";

      throw std::logic_error(buffer.str().c_str());
   }   

   #endif

   // Model order selection stuff

   const unsigned int inputs_assays_number = hidden_perceptrons_numbers.size();

   ModelSelectionResults model_selection_results;
   model_selection_results.parameters_data.set(inputs_assays_number, parameters_assays_number);
   model_selection_results.evaluation_data.set(inputs_assays_number, parameters_assays_number);
   model_selection_results.generalization_evaluation_data.set(inputs_assays_number, parameters_assays_number);

   time_t beginning_time, current_time;
   time(&beginning_time);
   double elapsed_time;

   unsigned int assay_count = 0;

   for(unsigned int i = 0; i < inputs_assays_number; i++)
   {
      //multilayer_perceptron_pointer->set_inputs_number(1);

      for(unsigned int j = 0; j < parameters_assays_number; j++)
	  {
         try
		 {
            assay_count++; 

			// Multilayer perceptron

            multilayer_perceptron_pointer->initialize_parameters_normal();

			// Performance functional 

			// Training algorithm

   	        training_strategy_pointer->perform_training();
	   
			// Model order selection

		    model_selection_results.parameters_data[i][j] = multilayer_perceptron_pointer->arrange_parameters();

		    model_selection_results.evaluation_data[i][j] = training_strategy_pointer->get_performance_functional_pointer()->calculate_evaluation();
		    model_selection_results.generalization_evaluation_data[i][j] = training_strategy_pointer->get_performance_functional_pointer()->calculate_generalization_evaluation();			

            time(&current_time);
            elapsed_time = difftime(current_time, beginning_time);

			if(display)
			{
               std::cout << "Assay " << assay_count << ";\n"
                         << "Hidden layer size: " << hidden_perceptrons_numbers[i] << "\n"
                         << "Parameters set: " << j+1 << "\n"
						 << "Evaluation: " << model_selection_results.evaluation_data[i][j] << "\n"
						 << "Generalization evaluation: " << model_selection_results.generalization_evaluation_data[i][j] << "\n"
						 << "Elapsed time: " << elapsed_time << std::endl;
			}
		 }
         catch(std::logic_error& e)
         {
            std::cerr << e.what() << std::endl;
         }
      }
   }

   const Vector<unsigned int> minimal_indices = model_selection_results.generalization_evaluation_data.calculate_minimal_indices();

   const Vector<double> minimal_parameters = model_selection_results.parameters_data[minimal_indices [0]][minimal_indices[1]];

   multilayer_perceptron_pointer->set_parameters(minimal_parameters);

   return(model_selection_results);
*/
}


// ModelSelectionResults perform_model_order_selection(void) method

/// @todo

ModelSelection::ModelSelectionResults ModelSelection::perform_model_order_selection(void) const
{
   std::ostringstream buffer;

   buffer << "OpenNN Exception: ModelSelection class.\n"
          << "ModelSelectionResults perform_model_order_selection(void) method.\n"
          << "This method is under development.\n";

   throw std::logic_error(buffer.str().c_str());
/*
   // Training algorithm stuff

   #ifdef _DEBUG 

   check();

   #endif

   // Performance functional stuff

   const PerformanceFunctional* performance_functional_pointer = training_strategy_pointer->get_performance_functional_pointer();

   DataSet* data_set_pointer = performance_functional_pointer->get_objective_term_pointer()->get_data_set_pointer();

   VariablesInformation* variables_information_pointer = data_set_pointer->get_variables_information_pointer();

   // Neural network stuff

   NeuralNetwork* neural_network_pointer = performance_functional_pointer->get_neural_network_pointer(); 

   MultilayerPerceptron* multilayer_perceptron_pointer = neural_network_pointer->get_multilayer_perceptron_pointer();

   #ifdef _DEBUG 

   const unsigned int layers_number = multilayer_perceptron_pointer->count_layers_number();

   if(layers_number != 2) 
   {
     std::ostringstream buffer;

      buffer << "OpenNN Exception: ModelSelection class.\n"
             << "ModelSelectionResults perform_model_order_selection(void) method.\n"
             << "Number of layers in multilayer perceptron must be two.\n";

      throw std::logic_error(buffer.str().c_str());
   }   

   #endif

   // Model order selection stuff

   const unsigned int complexity_assays_number = hidden_perceptrons_numbers.size();

   ModelSelectionResults model_order_selection_results;

   if(reserve_parameters_data)
   {
      model_order_selection_results.parameters_data.set(complexity_assays_number, parameters_assays_number);
   }  
	 
   if(reserve_evaluation_data)
   {
      model_order_selection_results.evaluation_data.set(complexity_assays_number, parameters_assays_number);
   }

   if(reserve_generalization_evaluation_data)
   {
      model_order_selection_results.generalization_evaluation_data.set(complexity_assays_number, parameters_assays_number);
   }

   time_t beginning_time, current_time;
   time(&beginning_time);
   double elapsed_time;

   unsigned int assay_count = 0;

   for(unsigned int i = 0; i < complexity_assays_number; i++)
   {
	  variables_information_pointer->set_inputs_indices(inputs_indices[i]);

      //multilayer_perceptron_pointer->set_layer_perceptrons_number(0, hidden_perceptrons_numbers[i]);

      for(unsigned int j = 0; j < parameters_assays_number; j++)
	  {
         try
		 {
            assay_count++; 

			// Multilayer perceptron

            multilayer_perceptron_pointer->initialize_parameters_normal();

			// Performance functional 

			// Training algorithm

   	        training_strategy_pointer->perform_training();
	   
			// Model order selection

            if(reserve_parameters_data)
            {
		       model_order_selection_results.parameters_data[i][j] = multilayer_perceptron_pointer->arrange_parameters();
			}

            if(reserve_parameters_data)
            {
		       model_order_selection_results.evaluation_data[i][j] = training_strategy_pointer->get_performance_functional_pointer()->calculate_evaluation();
			}

            if(reserve_parameters_data)
            {
		       model_order_selection_results.generalization_evaluation_data[i][j] = training_strategy_pointer->get_performance_functional_pointer()->calculate_generalization_evaluation();			
			}

            time(&current_time);
            elapsed_time = difftime(current_time, beginning_time);

			if(display)
			{
               std::cout << "Assay " << assay_count << ";\n"
                         << "Hidden layer size: " << hidden_perceptrons_numbers[i] << "\n"
                         << "Parameters set: " << j+1 << "\n"
						 << "Evaluation: " << model_order_selection_results.evaluation_data[i][j] << "\n"
						 << "Generalization performance: " << model_order_selection_results.generalization_evaluation_data[i][j] << "\n"
						 << "Elapsed time: " << elapsed_time << std::endl;
			}
		 }
         catch(std::logic_error& e)
         {
            std::cerr << e.what() << std::endl;
         }
      }
   }


   Vector<unsigned int> minimal_indices = model_order_selection_results.generalization_evaluation_data.calculate_minimal_indices();

   Vector<double> minimal_parameters = model_order_selection_results.parameters_data[minimal_indices [0]][minimal_indices[1]];

   multilayer_perceptron_pointer->set_parameters(minimal_parameters);

   if(reserve_minimal_parameters)
   {
   }

// Vector< Vector<double> > calculate_performance_data_statistics(const ModelSelectionResults&) const method
//   return(model_order_selection_data.performance_data.calculate_transpose().calculate_statistics());  

// Vector< Vector<double> > calculate_generalization_evaluation_data_statistics(const ModelSelectionResults&) const method
   //   return(model_order_selection_data.generalization_evaluation_data.calculate_transpose().calculate_statistics());  

//   unsigned int complexities_number = hidden_perceptrons_numbers.size();

//   file << "# Performance data\n\n";

//   for(unsigned int i = 0; i < complexities_number; i++)
//   {    
//      file << hidden_perceptrons_numbers[i] << " ";

//      for(unsigned int j = 0; j < parameters_assays_number; j++)
//	  {
//	     file << model_order_selection_data.performance_data[i][j] << " ";
//	  }

//	  file << "\n";
//   }

//   file << "\n";

//   file << "# Validation performance data\n\n";

//   for(unsigned int i = 0; i < complexities_number; i++)
//   {    
//      file << hidden_perceptrons_numbers[i] << " ";

//      for(unsigned int j = 0; j < parameters_assays_number; j++)
//	  {
//	     file << model_order_selection_data.generalization_evaluation_data[i][j] << " ";
//	  }

//	  file << "\n";
//   }


   return(model_order_selection_results);
*/
}


// ModelSelectionResults perform_model_selection(void) const method

/// @todo

ModelSelection::ModelSelectionResults ModelSelection::perform_model_selection(void) const
{
   std::ostringstream buffer;

   buffer << "OpenNN Exception: ModelSelection class.\n"
          << "ModelSelectionResults perform_model_selection(void) method.\n"
          << "This method is under development.\n";

   throw std::logic_error(buffer.str().c_str());

//   ModelSelectionResults model_selection_results;

//   return(model_selection_results);
}


// TiXmlElement* to_XML(void) const method

/// This method serializes the model selection object into a XML element of the TinyXML library. 
/// See the OpenNN manual for more information about the format of this element. 

TiXmlElement* ModelSelection::to_XML(void) const
{
   std::ostringstream buffer; 

   // Model order selection

   TiXmlElement* model_order_selection_element = new TiXmlElement("ModelSelection");
   model_order_selection_element->SetAttribute("Version", 4);

   // Hidden layer sizes

   TiXmlElement* hidden_perceptrons_numbers_element = new TiXmlElement("HiddenLayerSizes");
   model_order_selection_element->LinkEndChild(hidden_perceptrons_numbers_element);

   buffer.str("");
   buffer << hidden_perceptrons_numbers;

   TiXmlText* hidden_perceptrons_numbers_text = new TiXmlText(buffer.str().c_str());
   hidden_perceptrons_numbers_element->LinkEndChild(hidden_perceptrons_numbers_text);

   // ParametersAssaysNumber

   TiXmlElement* parameters_assays_number_element = new TiXmlElement("ParametersAssaysNumber");
   model_order_selection_element->LinkEndChild(parameters_assays_number_element);

   buffer.str("");
   buffer << parameters_assays_number;

   TiXmlText* parameters_assays_number_text = new TiXmlText(buffer.str().c_str());
   parameters_assays_number_element->LinkEndChild(parameters_assays_number_text);

   return(model_order_selection_element);
}


// void from_XML(TiXmlElement*) method

/// @todo

void ModelSelection::from_XML(TiXmlElement*)
{
}


// void print(void) method

/// This method prints to the screen the XML representation of this model selection object. 

void ModelSelection::print(void) const
{
   std::cout << to_XML();
}


// void save(const std::string&) const method

/// This method saves the model selection members to a XML file. 
/// @param filename Name of model selection XML file. 

void ModelSelection::save(const std::string& filename) const
{
   TiXmlDocument document;

   // Declaration

   TiXmlDeclaration* declaration = new TiXmlDeclaration("1.0", "", "");
   document.LinkEndChild(declaration);

   // Model selection element

   TiXmlElement* model_selection_element = to_XML();
   document.LinkEndChild(model_selection_element);

   // Save

   document.SaveFile(filename.c_str());
}


// void load(const std::string&) method

/// This method loads the model selection members from a XML file. 
/// @param filename Name of model selection XML file. 

void ModelSelection::load(const std::string& filename)
{
   std::ostringstream buffer;

   TiXmlDocument document(filename.c_str());

   if (!document.LoadFile())
   {
      buffer << "OpenNN Exception: ModelSelection class.\n"
             << "void load(const std::string&) method.\n"
             << "Cannot load XML file " << filename << ".\n";

      throw std::logic_error(buffer.str());
   }

   // Root
   
   TiXmlElement* model_order_selection_element = document.FirstChildElement("ModelSelection");

   if(!model_order_selection_element)
   {
      buffer << "OpenNN Exception: ModelSelection class.\n"
             << "void load(const std::string&) method.\n"
             << "Invalid model order selection XML root element.\n";

      throw std::logic_error(buffer.str());
   }
    
   // Hidden layer sizes
   
   TiXmlElement* hidden_perceptrons_numbers_element = model_order_selection_element->FirstChildElement("HiddenLayerSizes");

   if(hidden_perceptrons_numbers_element)
   {
      Vector<unsigned int> new_hidden_perceptrons_numbers;
      new_hidden_perceptrons_numbers.parse(hidden_perceptrons_numbers_element->GetText());

      try
      {
         set_hidden_perceptrons_numbers(new_hidden_perceptrons_numbers);
      }
      catch(std::exception& e)
      {
         std::cout << e.what() << std::endl;		 
      }
   }

   // Parameters assays number
   
   TiXmlElement* parameters_assays_number_element = model_order_selection_element->FirstChildElement("ParametersAssaysNumber");

   if(parameters_assays_number_element)
   {
      parameters_assays_number = atoi(parameters_assays_number_element->GetText());           
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
