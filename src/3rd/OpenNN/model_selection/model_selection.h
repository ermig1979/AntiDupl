/****************************************************************************************************************/
/*                                                                                                              */
/*   OpenNN: Open Neural Networks Library                                                                       */
/*   www.opennn.cimne.com                                                                                       */
/*                                                                                                              */
/*   M O D E L   S E L E C T I O N   C L A S S   H E A D E R                                                    */
/*                                                                                                              */
/*   Roberto Lopez                                                                                              */ 
/*   International Center for Numerical Methods in Engineering (CIMNE)                                          */
/*   Technical University of Catalonia (UPC)                                                                    */
/*   Barcelona, Spain                                                                                           */
/*   E-mail: rlopez@cimne.upc.edu                                                                               */ 
/*                                                                                                              */
/****************************************************************************************************************/

#ifndef __MODELSELECTION_H__
#define __MODELSELECTION_H__

// OpenNN includes

#include "../utilities/vector.h"
#include "../utilities/matrix.h"
#include "../training_strategy/training_strategy.h"

// TinyXml includes

#include "../../tinyxml/tinyxml.h"

namespace OpenNN
{

/// This class represents the concept of model selection algorithm.
/// It is used for finding a network architecture with maximum generalization capabilities. 

class ModelSelection
{

public:  

   // DEFAULT CONSTRUCTOR

   explicit ModelSelection(void);

   // GRADIENT DESCENT CONSTRUCTOR

   explicit ModelSelection(TrainingStrategy*);

   // FILE CONSTRUCTOR

   explicit ModelSelection(const std::string&);

   // DESTRUCTOR

   virtual ~ModelSelection(void);

   // STRUCTURES

   ///
   /// This structure contains the results from the model selection process. 
   ///

   struct ModelSelectionResults
   {
      /// Parameters of the different neural networks. 

      Matrix< Vector<double> > parameters_data;

      /// Performance evaluation of the different neural networks. 

      Matrix<double> evaluation_data;

      /// Generalization evaluation of the different neural networks. 

      Matrix<double> generalization_evaluation_data;

	  /// Vector of parameters for the neural network with minimum generalization evaluation. 

	  Vector<double> minimal_parameters;

	  /// Statistics of the performance functional evaluation for the different neural networks.

	  Vector< Vector<double> > evaluation_data_statistics;

	  /// Statistics of the generalization evaluation for the different neural networks.

	  Vector< Vector<double> > generalization_evaluation_data_statistics; 
   };

   // METHODS

   // Get methods

   TrainingStrategy* get_training_strategy_pointer(void) const;

   const Vector<unsigned int>& get_hidden_perceptrons_numbers(void) const;
   const unsigned int& get_parameters_assays_number(void) const;

   const bool& get_reserve_parameters_data(void);
   const bool& get_reserve_evaluation_data(void);
   const bool& get_reserve_generalization_evaluation_data(void);
   const bool& get_reserve_minimal_parameters(void);
   const bool& get_reserve_evaluation_data_statistics(void);
   const bool& get_reserve_generalization_evaluation_data_statistics(void);
   const bool& get_reserve_model_order_selection_plot(void);

   const bool& get_display(void) const;

   // Set methods

   void set_training_strategy_pointer(TrainingStrategy*);

   void set_default(void);

   void set_hidden_perceptrons_numbers(const Vector<unsigned int>&);
   void set_parameters_assays_number(const unsigned int&);

   void set_assays_numbers(const unsigned int&, const unsigned int&);

   void set_reserve_parameters_data(const bool&);
   void set_reserve_evaluation_data(const bool&);
   void set_reserve_generalization_evaluation_data(const bool&);
   void set_reserve_minimal_parameters(const bool&);
   void set_reserve_evaluation_data_statistics(const bool&);
   void set_reserve_generalization_evaluation_data_statistics(const bool&);
   void set_reserve_model_order_selection_plot(const bool&);

   void set_display(const bool&);

   // Model order selection methods

   void check(void) const;

   ModelSelectionResults perform_model_inputs_selection(void) const;

   ModelSelectionResults perform_model_order_selection(void) const;

   ModelSelectionResults perform_model_selection(void) const;

   // Serialization methods

   TiXmlElement* to_XML(void) const;
   void from_XML(TiXmlElement*);

   void print(void) const;
   void save(const std::string&) const;
   void load(const std::string&);

//   TiXmlElement* get_model_order_selection_data_XML(const ModelSelectionData&) const;

private: 

   // MEMBERS

   /// Pointer to a training strategy object.

   TrainingStrategy* training_strategy_pointer;

   /// Indices for selecting the input variables. 

   Vector< Vector<unsigned int> > inputs_indices;

   /// Complexities for selecting the network architecture. 

   Vector<unsigned int> hidden_perceptrons_numbers;

   /// Number of trials for each neural network. 

   unsigned int parameters_assays_number;

   // Model selection results

   /// True if the parameters of all neural networks are to be reserved. 

   bool reserve_parameters_data;

   /// True if the evaluation of all neural networks are to be reserved. 

   bool reserve_evaluation_data;

   /// True if the generalization evaluation of all neural networks are to be reserved. 

   bool reserve_generalization_evaluation_data;

   /// True if the vector parameters of the neural network presenting minimum generalization evaluation is to be reserved. 

   bool reserve_minimal_parameters;

   /// True if the statistics of the different evaluations are to be reserved. 

   bool reserve_evaluation_data_statistics;

   /// True if the statistics of the different generalization evaluations are to be reserved. 

   bool reserve_generalization_evaluation_data_statistics;

   /// True if the data needed to plot the results from the model selection plot is to be reserved. 

   bool reserve_model_order_selection_plot;

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
