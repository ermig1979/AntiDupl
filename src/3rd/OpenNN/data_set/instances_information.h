/****************************************************************************************************************/
/*                                                                                                              */
/*   OpenNN: Open Neural Networks Library                                                                       */
/*   www.opennn.cimne.com                                                                                       */
/*                                                                                                              */
/*   I N S T A N C E S   I N F O R M A T I O N   C L A S S   H E A D E R                                        */
/*                                                                                                              */ 
/*   Roberto Lopez                                                                                              */ 
/*   International Center for Numerical Methods in Engineering (CIMNE)                                          */
/*   Technical University of Catalonia (UPC)                                                                    */
/*   Barcelona, Spain                                                                                           */
/*   E-mail: rlopez@cimne.upc.edu                                                                               */ 
/*                                                                                                              */
/****************************************************************************************************************/

#ifndef __INSTANCESINFORMATION_H__
#define __INSTANCESINFORMATION_H__

// System includes

#include <string>

// OpenNN includes

#include "../utilities/vector.h"
#include "../utilities/matrix.h"

// TinyXml includes

#include "../../tinyxml/tinyxml.h"


namespace OpenNN
{

/// This class is used to store information about the instances of a data set. 
/// Instances in a data set can be used for training, generalization and testing.    

class InstancesInformation 
{

public:  

   // DEFAULT CONSTRUCTOR

   explicit InstancesInformation(void);

   // INSTANCES NUMBER CONSTRUCTOR

   explicit InstancesInformation(const unsigned int&);

   // XML CONSTRUCTOR

   explicit InstancesInformation(TiXmlElement*);


   // COPY CONSTRUCTOR

   InstancesInformation(const InstancesInformation&);


   // DESTRUCTOR

   virtual ~InstancesInformation(void);

   // ASSIGNMENT OPERATOR

   InstancesInformation& operator = (const InstancesInformation&);

   // EQUAL TO OPERATOR

   bool operator == (const InstancesInformation&) const;

   // METHODS

   /// This method returns the number of instances in the data set.

   const unsigned int& get_instances_number(void) const
   {
      return(instances_number);
   }

   /// This method returns the number of instances in the data set which will be used for training.

   inline unsigned int count_training_instances_number(void) const
   {
      return(training_indices.size());      
   }

   /// This method returns the number of instances in the data set which will be used for generalization.

   inline unsigned int count_generalization_instances_number(void) const
   {
      return(generalization_indices.size());
   }

   /// This method returns the number of instances in the data set which will be used for testing.

   inline unsigned int count_testing_instances_number(void) const
   {
      return(testing_indices.size());
   }

   // Instances methods

   const Vector<unsigned int>& get_training_indices(void) const;
   const Vector<unsigned int>& get_generalization_indices(void) const;
   const Vector<unsigned int>& get_testing_indices(void) const;

   const bool& get_display(void) const;

   // Set methods

   void set(void);
   void set(const unsigned int&);
   void set(TiXmlElement*);

   // Data methods

   void set_instances_number(const unsigned int&);

   // Instances methods

   void set_training_indices(const Vector<unsigned int>&);
   void set_generalization_indices(const Vector<unsigned int>&);
   void set_testing_indices(const Vector<unsigned int>&);

   void set_training(void);
   void set_generalization(void);
   void set_testing(void);

   void set_default_instances_indices(void);

   void set_display(const bool&);

   // Data splitting methods
  
   void split_given_indices(const Vector<unsigned int>&, const Vector<unsigned int>&, const Vector<unsigned int>&);

   void split_random_indices(const double&, const double&, const double&);
   void split_random_indices(void);

   void split_sequential_indices(const double&, const double&, const double&);
   void split_sequential_indices(void);

   // Serialization methods

   std::string to_string(void) const;

   TiXmlElement* to_XML(void) const;
   void from_XML(TiXmlElement*);

private:

   // MEMBERS

   unsigned int instances_number;

   /// Indices of instances for training.

   Vector<unsigned int> training_indices;

   /// Indices of instances for generalization.

   Vector<unsigned int> generalization_indices;

   /// Indices of instances for testing.

   Vector<unsigned int> testing_indices;

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

