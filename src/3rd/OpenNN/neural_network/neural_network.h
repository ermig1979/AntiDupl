/****************************************************************************************************************/
/*                                                                                                              */
/*   OpenNN: Open Neural Networks Library                                                                       */
/*   www.opennn.cimne.com                                                                                       */
/*                                                                                                              */
/*   N E U R A L   N E T W O R K   C L A S S   H E A D E R                                                      */
/*                                                                                                              */
/*   Roberto Lopez                                                                                              */
/*   International Center for Numerical Methods in Engineering (CIMNE)                                          */
/*   Technical University of Catalonia (UPC)                                                                    */
/*   Barcelona, Spain                                                                                           */
/*   E-mail: rlopez@cimne.upc.edu                                                                               */
/*                                                                                                              */
/****************************************************************************************************************/

#ifndef __NEURALNETWORK_H__
#define __NEURALNETWORK_H__

// System includes

#include <string>

// OpenNN includes

#include "../utilities/vector.h"
#include "../utilities/matrix.h"

#include "perceptron.h"
#include "perceptron_layer.h"
#include "multilayer_perceptron.h"
#include "scaling_layer.h"
#include "unscaling_layer.h"
#include "bounding_layer.h"
#include "probabilistic_layer.h"
#include "conditions_layer.h"
#include "independent_parameters.h"
#include "inputs_outputs_information.h"


namespace OpenNN
{

/// This class represents the concept of neural network in the OpenNN library.
/// A neural network here is defined as a multilayer perceptron extended with a scaling layer, an unscaling layer, 
/// a bounding layer, a probabilistic layer, a conditions layer and a set of independent parameters. 
/// This neural network is used to span a function space for the variational problem at hand. 

class NeuralNetwork
{

public:

   // DEFAULT CONSTRUCTOR

   explicit NeuralNetwork(void);

   // MULTILAYER PERCEPTRON CONSTRUCTOR

   explicit NeuralNetwork(const MultilayerPerceptron&);

   // MULTILAYER PERCEPTRON ARCHITECTURE CONSTRUCTOR

   explicit NeuralNetwork(const Vector<unsigned int>&);

   // ONE PERCEPTRON LAYER CONSTRUCTOR 

   explicit NeuralNetwork(const unsigned int&, const unsigned int&);

   // TWO PERCEPTRON LAYERS CONSTRUCTOR 

   explicit NeuralNetwork(const unsigned int&, const unsigned int&, const unsigned int&);

   // INDEPENDENT PARAMETERS CONSTRUCTOR

   //explicit NeuralNetwork(const IndependentParameters&);

   // INDEPENDENT PARAMETERS NUMBER CONSTRUCTOR

   explicit NeuralNetwork(const unsigned int&);

   // MULTILAYER PERCEPTRON AND INDEPENDENT PARAMETERS CONSTRUCTOR

   //explicit NeuralNetwork(const MultilayerPerceptron&, const IndependentParameters&);

   // MULTILAYER PERCEPTRON ARCHITECTURE AND INDEPENDENT PARAMETERS NUMBER CONSTRUCTOR

   //explicit NeuralNetwork(const Vector<unsigned int>&, const unsigned int&);

   // FILE CONSTRUCTOR

   explicit NeuralNetwork(const std::string&);

   // COPY CONSTRUCTOR

   NeuralNetwork(const NeuralNetwork&);

   // DESTRUCTOR

   virtual ~NeuralNetwork(void);

   // ASSIGNMENT OPERATOR

   NeuralNetwork& operator = (const NeuralNetwork&);

   // EQUAL TO OPERATOR

   bool operator == (const NeuralNetwork&) const;


   // GET METHODS

   MultilayerPerceptron* get_multilayer_perceptron_pointer(void) const;
   InputsOutputsInformation* get_inputs_outputs_information_pointer(void) const;
   ScalingLayer* get_scaling_layer_pointer(void) const;
   UnscalingLayer* get_unscaling_layer_pointer(void) const;   
   BoundingLayer* get_bounding_layer_pointer(void) const;
   ProbabilisticLayer* get_probabilistic_layer_pointer(void) const;
   ConditionsLayer* get_conditions_layer_pointer(void) const;
   IndependentParameters* get_independent_parameters_pointer(void) const;

   const bool& get_multilayer_perceptron_flag(void) const;
   const bool& get_scaling_layer_flag(void) const;
   const bool& get_unscaling_layer_flag(void) const;
   const bool& get_bounding_layer_flag(void) const;
   const bool& get_probabilistic_layer_flag(void) const;
   const bool& get_conditions_layer_flag(void) const;

   const bool& get_display(void) const;

   // SET METHODS

   void set(void);

   void set(const MultilayerPerceptron&);
   void set(const Vector<unsigned int>&);
   void set(const unsigned int&, const unsigned int&);
   void set(const unsigned int&, const unsigned int&, const unsigned int&);

//   void set(const IndependentParameters&);
   void set(const unsigned int&);

   void set(const std::string&);
   void set(const NeuralNetwork&);

   virtual void set_default(void);

   void set_multilayer_perceptron_pointer(MultilayerPerceptron*);
   void set_scaling_layer_pointer(ScalingLayer*);
   void set_unscaling_layer_pointer(UnscalingLayer*);
   void set_bounding_layer_pointer(BoundingLayer*);
   void set_probabilistic_layer_pointer(ProbabilisticLayer*);
   void set_conditions_layer_pointer(ConditionsLayer*);
   void set_inputs_outputs_information_pointer(InputsOutputsInformation*);
   void set_independent_parameters_pointer(IndependentParameters*);

   void set_multilayer_perceptron_flag(const bool&);
   void set_scaling_layer_flag(const bool&);
   void set_unscaling_layer_flag(const bool&);
   void set_scaling_unscaling_layers_flag(const bool&);
   void set_bounding_layer_flag(const bool&);
   void set_probabilistic_layer_flag(const bool&);
   void set_conditions_layer_flag(const bool&);

   void set_display(const bool&);


   // Pointer methods

   void construct_multilayer_perceptron(void);
   void construct_scaling_layer(void);
   void construct_unscaling_layer(void);
   void construct_bounding_layer(void);
   void construct_probabilistic_layer(void);
   void construct_conditions_layer(void);
   void construct_inputs_outputs_information(void);
   void construct_independent_parameters(void);

   void destruct_multilayer_perceptron(void);
   void destruct_scaling_layer(void);
   void destruct_unscaling_layer(void);
   void destruct_bounding_layer(void);
   void destruct_probabilistic_layer(void);
   void destruct_conditions_layer(void);
   void destruct_inputs_outputs_information(void);
   void destruct_independent_parameters(void);

   void delete_pointers(void);

   // Multilayer perceptron initialization methods

   void initialize_random(void);

   // Layers 

   unsigned int count_layers_number(void);

   // Parameters

   unsigned int count_parameters_number(void) const;
   Vector<double> arrange_parameters(void) const;      

   void set_parameters(const Vector<double>&);

   // Parameters initialization methods

   void initialize_parameters(const double&);

   void initialize_parameters_uniform(void);
   void initialize_parameters_uniform(const double&, const double&);
   void initialize_parameters_uniform(const Vector<double>&, const Vector<double>&);
   void initialize_parameters_uniform(const Vector< Vector<double> >&);

   void initialize_parameters_normal(void);
   void initialize_parameters_normal(const double&, const double&);
   void initialize_parameters_normal(const Vector<double>&, const Vector<double>&);
   void initialize_parameters_normal(const Vector< Vector<double> >&);

   // Parameters norm 

   double calculate_parameters_norm(void) const;

   // Output 

   Vector<double> calculate_outputs(const Vector<double>&) const;
   Matrix<double> calculate_Jacobian(const Vector<double>&) const;
   Vector< Matrix<double> > calculate_Hessian_form(const Vector<double>&) const;

   Vector<double> calculate_parameters_output(const Vector<double>&, const Vector<double>&) const;
   Matrix<double> calculate_parameters_Jacobian(const Vector<double>&, const Vector<double>&) const;
   Vector< Matrix<double> > calculate_parameters_Hessian_form(const Vector<double>&, const Vector<double>&) const;

   Matrix<double> calculate_output_data(const Matrix<double>&) const;

   // Inputs outputs information and statistics

   Vector< Vector<std::string> > arrange_inputs_outputs_information(void);

   void set_inputs_outputs_information(const Vector< Vector<std::string> >&);

   // Inputs outputs statistics

   Vector< Vector<double> > arrange_inputs_outputs_statistics(void);

   void set_inputs_outputs_statistics(const Vector< Vector<double> >&);

   void set_inputs_outputs_minimums_maximums(const Vector< Vector<double> >&);
   void set_inputs_outputs_means_standard_deviations(const Vector< Vector<double> >&);

   void set_inputs_scaling_outputs_unscaling_methods(const std::string&);

   // Serialization methods

   std::string to_string(void) const;
 
   virtual TiXmlElement* to_XML(void) const;
   virtual void from_XML(TiXmlElement*);

   void print(void) const;
   void save(const std::string&) const;
   void save_parameters(const std::string&) const;

   virtual void load(const std::string&);
   void load_parameters(const std::string&);

   void save_data(const std::string&) const;

   // Expression methods

   std::string write_expression(void) const;

   void save_expression(const std::string&);

   // Parameters

//   TiXmlElement* report_parameters_number_XML(void) const;
//   TiXmlElement* report_parameters_XML(void) const;
//   TiXmlElement* calculate_parameters_norm_XML(void) const;

//   TiXmlElement* report_Hinton_diagram_XML(void) const;

   // Output

//   TiXmlElement* calculate_output_XML(const Vector<double>&) const;   
//   TiXmlElement* calculate_output_data_XML(const Matrix<double>&) const;

//   TiXmlElement* calculate_Jacobian_XML(const Vector<double>&) const;
//   TiXmlElement* calculate_Jacobian_data_XML(const Matrix<double>&) const;

//   TiXmlElement* calculate_Hessian_form_XML(const Vector<double>&) const;
//   TiXmlElement* calculate_Hessian_form_data_XML(const Matrix<double>&) const;

//   TiXmlElement* calculate_parameters_output_XML(const Vector<double>&) const;   
//   TiXmlElement* calculate_parameters_output_data_XML(const Matrix<double>&) const;

//   TiXmlElement* calculate_parameters_Jacobian_XML(const Vector<double>&, const Vector<double>&) const;
//   TiXmlElement* calculate_parameters_Jacobian_data_XML(const Matrix<double>&, const Vector<double>&) const;

//   TiXmlElement* calculate_parameters_Hessian_form_XML(const Vector<double>&, const Vector<double>&) const;
//   TiXmlElement* calculate_parameters_Hessian_form_data_XML(const Matrix<double>&, const Vector<double>&) const;

   // Expression

//   TiXmlElement* report_expression_XML(void) const;


protected:

   // MEMBERS

   /// Pointer to a multilayer perceptron object.

   MultilayerPerceptron* multilayer_perceptron_pointer;

   /// Pointer to a scaling layer object.

   ScalingLayer* scaling_layer_pointer;

   /// Pointer to an unscaling layer object.

   UnscalingLayer* unscaling_layer_pointer;

   /// Pointer to a bounding layer object.

   BoundingLayer* bounding_layer_pointer;

   /// Pointer to a probabilistic layer.

   ProbabilisticLayer* probabilistic_layer_pointer;

   /// Pointer to a conditions object.

   ConditionsLayer* conditions_layer_pointer;

   /// Pointer to an inputs-outputs information object.

   InputsOutputsInformation* inputs_outputs_information_pointer;

   /// Pointer to an independent parameters object

   IndependentParameters* independent_parameters_pointer;

   /// Flag for using a multilayer perceptron.

   bool multilayer_perceptron_flag;

   /// Flag for using the scaling layer.

   bool scaling_layer_flag;

   /// Flag for using the unscaling layer.

   bool unscaling_layer_flag;

   /// Flag for using the bounding layer.

   bool bounding_layer_flag;

   /// Flag for using the probabilistic layer.

   bool probabilistic_layer_flag;

   /// Flag for using the conditions layer. 

   bool conditions_layer_flag;

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

