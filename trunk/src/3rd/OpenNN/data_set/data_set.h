/****************************************************************************************************************/
/*                                                                                                              */
/*   OpenNN: Open Neural Networks Library                                                                       */
/*   www.opennn.cimne.com                                                                                       */
/*                                                                                                              */
/*   D A T A   S E T   C L A S S   H E A D E R                                                                  */
/*                                                                                                              */ 
/*   Roberto Lopez                                                                                              */ 
/*   International Center for Numerical Methods in Engineering (CIMNE)                                          */
/*   Technical University of Catalonia (UPC)                                                                    */
/*   Barcelona, Spain                                                                                           */
/*   E-mail: rlopez@cimne.upc.edu                                                                               */ 
/*                                                                                                              */
/****************************************************************************************************************/

#ifndef __DATASET_H__
#define __DATASET_H__

// System includes

#include <string>

// OpenNN includes

#include "../utilities/vector.h"
#include "../utilities/matrix.h"

#include "variables_information.h"
#include "instances_information.h"

// TinyXml includes

#include "../../tinyxml/tinyxml.h"


namespace OpenNN
{

/// This class represents the concept of data set for data modelling problems, 
/// such as function regression, pattern recognition and time series prediction.
/// It basically consists of a data matrix plus a variables and an instances information objects. 

class DataSet 
{

public:  

    // DEFAULT CONSTRUCTOR

    explicit DataSet(void);

   // INSTANCES AND VARIABLES CONSTRUCTOR

   explicit DataSet(const unsigned int&, const unsigned int&);

   // INSTANCES AND INPUT AND TARGET VARIABLES CONSTRUCTOR

   explicit DataSet(const unsigned int&, const unsigned int&, const unsigned int&);

   // XML CONSTRUCTOR

   explicit DataSet(TiXmlElement*);

   // FILE CONSTRUCTOR

   explicit DataSet(const std::string&);

   // COPY CONSTRUCTOR

   DataSet(const DataSet&);

   // DESTRUCTOR

   virtual ~DataSet(void);

   // ASSIGNMENT OPERATOR

   DataSet& operator = (const DataSet&);

   // EQUAL TO OPERATOR

   bool operator == (const DataSet&) const;

   // ENUMERATIONS

   /// Enumeration of available methods for scaling and unscaling the data.  
   
   enum ScalingUnscalingMethod{MinimumMaximum, MeanStandardDeviation};

   // METHODS

   // Get methods

   const std::string& get_data_filename(void) const;

   /// This method returns the number of instances in the data set.

   inline unsigned int get_instances_number(void) const
   {
      return(data.get_rows_number());   
   }

   /// This method returns the total number of variables in the data set.

   inline unsigned int get_variables_number(void) const
   {
      return(data.get_columns_number());   
   }

   const VariablesInformation& get_variables_information(void) const;
   VariablesInformation* get_variables_information_pointer(void);

   const InstancesInformation& get_instances_information(void) const;
   InstancesInformation* get_instances_information_pointer(void);

   const ScalingUnscalingMethod& get_scaling_unscaling_method(void) const;
   std::string write_scaling_unscaling_method(void) const;

   const bool& get_display(void) const;

   // Data methods

   const Matrix<double>& get_data(void) const;

   Matrix<double> arrange_training_data(void) const;
   Matrix<double> arrange_generalization_data(void) const;
   Matrix<double> arrange_testing_data(void) const;

   Matrix<double> arrange_input_data(void) const;
   Matrix<double> arrange_target_data(void) const;

   Matrix<double> arrange_training_input_data(void) const;
   Matrix<double> arrange_training_target_data(void) const;  
   Matrix<double> get_generalization_input_data(void) const;
   Matrix<double> get_generalization_target_data(void) const;
   Matrix<double> arrange_testing_input_data(void) const;
   Matrix<double> arrange_testing_target_data(void) const;

   // Instance methods

   Vector<double> get_instance(const unsigned int&) const;

   Vector<double> get_training_instance(const unsigned int&) const;
   Vector<double> get_generalization_instance(const unsigned int&) const;
   Vector<double> get_testing_instance(const unsigned int&) const;

   Vector<double> get_input_instance(const unsigned int&) const;
   Vector<double> get_target_instance(const unsigned int&) const;

   Vector<double> get_training_input_instance(const unsigned int&) const;
   Vector<double> get_training_target_instance(const unsigned int&) const;

   Vector<double> get_generalization_input_instance(const unsigned int&) const;
   Vector<double> get_generalization_target_instance(const unsigned int&) const;

   Vector<double> get_testing_input_instance(const unsigned int&) const;
   Vector<double> get_testing_target_instance(const unsigned int&) const;

   // Variable methods

   Vector<double> get_variable(const unsigned int&) const;

   // Set methods

   void set(void);
   void set(const unsigned int&, const unsigned int&);
   void set(const unsigned int&, const unsigned int&, const unsigned int&);
   void set(const DataSet&);
   void set(TiXmlElement*);
   void set(const std::string&);

   // Data methods

   void set_data(const Matrix<double>&);

   void set_instances_number(const unsigned int&);
   void set_variables_number(const unsigned int&);

   void set_data_filename(const std::string&);

   void set_scaling_unscaling_method(const ScalingUnscalingMethod&);
   void set_scaling_unscaling_method(const std::string&);

   // Utilities

   void set_display(const bool&);

   void set_default(void);

   // Instance methods

   void set_instance(const unsigned int&, const Vector<double>&);

   void set_training_instance(const unsigned int&, const Vector<double>&);
   void set_generalization_instance(const unsigned int&, const Vector<double>&);
   void set_testing_instance(const unsigned int&, const Vector<double>&);

   void set_input_instance(const unsigned int&, const Vector<double>&);
   void set_target_instance(const unsigned int&, const Vector<double>&);

   void set_training_input_instance(const unsigned int&, const Vector<double>&);
   void set_training_target_instance(const unsigned int&, const Vector<double>&);

   void set_generalization_input_instance(const unsigned int&, const Vector<double>&);
   void set_generalization_target_instance(const unsigned int&, const Vector<double>&);

   void set_testing_input_instance(const unsigned int&, const Vector<double>&); 
   void set_testing_target_instance(const unsigned int&, const Vector<double>&);

   // Data resizing methods

   void add_instance(const Vector<double>&);
   void subtract_instance(const unsigned int&);

   void append_variable(const Vector<double>&);
   void subtract_variable(const unsigned int&);

   void subtract_constant_variables(void);
   void subtract_repeated_instances(void);

   // Initialization methods

   void initialize_data(const double&);

   void initialize_data_normal(void);
 
   // Statistics methods

   Vector< Vector<double> > calculate_data_statistics(void) const;

   Vector< Vector<double> > calculate_training_instances_statistics(void) const;
   Vector< Vector<double> > calculate_generalization_instances_statistics(void) const;
   Vector< Vector<double> > calculate_testing_instances_statistics(void) const;

   Vector< Vector<double> > calculate_instances_statistics(void) const;

   Vector< Vector<double> > calculate_inputs_statistics(void) const;
   Vector< Vector<double> > calculate_targets_statistics(void) const;
   
   Vector< Vector<double> > calculate_inputs_targets_minimum_maximum(void) const;
   Vector< Vector<double> > calculate_inputs_targets_mean_standard_deviation(void) const;
   Vector< Vector<double> > calculate_inputs_targets_statistics(void) const;

   Vector<double> calculate_training_target_data_mean(void) const;
   Vector<double> calculate_generalization_target_data_mean(void) const;
   Vector<double> calculate_testing_target_data_mean(void) const;

   // Correlation methods

   Matrix<double> calculate_variables_correlation(void) const;

   // Histrogram methods

   Vector< Vector< Vector<double> > > calculate_data_histogram(const unsigned int&) const;
   Vector< Vector< Vector<double> > > calculate_data_histogram(void) const;

   // Data scaling

   void scale_data_minimum_maximum(const Vector<double>&, const Vector<double>&); 
   void scale_data_mean_standard_deviation(const Vector<double>&, const Vector<double>&);  

   void scale_data(const Vector< Vector<double> >&);

   Vector< Vector<double> > scale_data(void);

   // Input variables scaling

   void scale_inputs_minimum_maximum(const Vector<double>&, const Vector<double>&);
   Vector< Vector<double> > scale_inputs_minimum_maximum(void);

   void scale_inputs_mean_standard_deviation(const Vector<double>&, const Vector<double>&);
   Vector< Vector<double> > scale_inputs_mean_standard_deviation(void);

   Vector< Vector<double> > scale_inputs(void);

   // Target variables scaling

   void scale_targets_minimum_maximum(const Vector<double>&, const Vector<double>&);
   Vector< Vector<double> > scale_targets_minimum_maximum(void);

   void scale_targets_mean_standard_deviation(const Vector<double>&, const Vector<double>&);
   Vector< Vector<double> > scale_targets_mean_standard_deviation(void);

   Vector< Vector<double> > scale_targets(void);

   // Inputs-targets scaling

   void scale_inputs_targets_minimum_maximum(const Vector<double>&, const Vector<double>&, const Vector<double>&, const Vector<double>&);
   Vector< Vector<double> > scale_inputs_targets_minimum_maximum(void);

   void scale_inputs_targets_mean_standard_deviation(const Vector<double>&, const Vector<double>&, const Vector<double>&, const Vector<double>&);
   Vector< Vector<double> > scale_inputs_targets_mean_standard_deviation(void);

   Vector< Vector<double> > scale_inputs_targets(void);

   // Data unscaling

   void unscale_data_minimum_maximum(const Vector<double>&, const Vector<double>&);
   void unscale_data_mean_standard_deviation(const Vector<double>&, const Vector<double>&);

   // Input variables unscaling

   void unscale_inputs_minimum_maximum(const Vector<double>&, const Vector<double>&);
   void unscale_inputs_mean_standard_deviation(const Vector<double>&, const Vector<double>&);

   // Target variables unscaling

   void unscale_targets_minimum_maximum(const Vector<double>&, const Vector<double>&);
   void unscale_targets_mean_standard_deviation(const Vector<double>&, const Vector<double>&);

   // Input-target variables unscaling

   void unscale_inputs_targets_minimum_maximum(const Vector< Vector<double> >&);
   void unscale_inputs_targets_mean_standard_deviation(const Vector< Vector<double> >&);

   // Pattern recognition methods

   Vector<unsigned int> calculate_target_class_distribution(void) const;

   // Serialization methods

   std::string to_string(void) const;

   void print(void) const;

   TiXmlElement* to_XML(void) const;
   void from_XML(TiXmlElement*);

   void save(const std::string&) const;
   void load(const std::string&);

   void print_data(void) const;

   void save_data(const std::string&) const;
   void load_data(const std::string&);

   // Task methods

//   TiXmlElement* report_data_XML(void) const;

//   TiXmlElement* report_variables_information_XML(void) const;
//   TiXmlElement* report_instances_information_XML(void) const;

//   TiXmlElement* calculate_data_statistics_XML(void) const;
  
//   TiXmlElement* calculate_training_instances_statistics_XML(void) const;
//   TiXmlElement* calculate_generalization_instances_statistics_XML(void) const;
//   TiXmlElement* calculate_testing_instances_statistics_XML(void) const;

//   TiXmlElement* split_instances_XML(void) const;

//   TiXmlElement* calculate_data_histogram_XML(void) const;

//   TiXmlElement* calculate_target_class_distribution_XML(void) const;

private:

   // MEMBERS

   /// Data filename 

   std::string data_filename;

   //std::string column_separator;

   //unsigned int header_lines_number;

   /// Data Matrix.

   Matrix<double> data;

   /// Variables information object (inputs and target variables).

   VariablesInformation variables_information;

   /// Instances information object (training, generalization and testing instances).

   InstancesInformation instances_information;

   /// Method for scaling and unscaling the data (minimum and maximum or mean and standard deviation). 

   ScalingUnscalingMethod scaling_unscaling_method;   

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

