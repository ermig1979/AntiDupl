/****************************************************************************************************************/
/*                                                                                                              */
/*   OpenNN: Open Neural Networks Library                                                                       */
/*   www.opennn.cimne.com                                                                                       */
/*                                                                                                              */
/*   I N S T A N C E S   I N F O R M A T I O N   C L A S S                                                      */
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
#include <algorithm>
#include <cstdlib>
#include <stdexcept>
#include <ctime>
#include <exception>

// OpenNN includes

#include "instances_information.h"

// TinyXml includes

#include "../../tinyxml/tinyxml.h"

namespace OpenNN
{


// DEFAULT CONSTRUCTOR

/// Default constructor. 
/// It creates a instances information object with zero instances. 
/// It also initializes the rest of class members to their default values.

InstancesInformation::InstancesInformation(void)
{
   set();  
}


// INSTANCES NUMBER CONSTRUCTOR

/// Instances number constructor. 
/// It creates a data set object with a given number of instances.
/// It also initializes the rest of class members to their default values.
/// @param new_instances_number Number of instances in the data set.

InstancesInformation::InstancesInformation(const unsigned int& new_instances_number)
{
   set(new_instances_number);
}


// XML CONSTRUCTOR

/// XML constructor. 
/// It creates a instances information object by loading the object members from a Tiny XML element. 
/// @param instances_information_element XML element from the Tiny XML library.

InstancesInformation::InstancesInformation(TiXmlElement* instances_information_element)
{
   set(instances_information_element);
}


// COPY CONSTRUCTOR

/// Copy constructor. 
/// It creates a copy of an existing instances information object. 
/// @param other_instances_information Instances information object to be copied.

InstancesInformation::InstancesInformation(const InstancesInformation& other_instances_information)
{
   instances_number = other_instances_information.instances_number;

   training_indices = other_instances_information.training_indices;
   generalization_indices = other_instances_information.generalization_indices;
   testing_indices = other_instances_information.testing_indices;

   display = other_instances_information.display;
}


// DESTRUCTOR

/// Destructor. 

InstancesInformation::~InstancesInformation(void)
{
}


// ASSIGNMENT OPERATOR

/// Assignment operator. 
/// It assigns to the current object the members of an existing instances information object.
/// @param other_instances_information Instances information object to be assigned.

InstancesInformation& InstancesInformation::operator=(const InstancesInformation& other_instances_information)
{
   if(this != &other_instances_information) 
   {
      instances_number = other_instances_information.instances_number;

      // Data splitting

      training_indices = other_instances_information.training_indices;
      generalization_indices = other_instances_information.generalization_indices;
      testing_indices = other_instances_information.testing_indices;

      // Utilities

      display = other_instances_information.display;
   }

   return(*this);
}


// EQUAL TO OPERATOR

// bool operator == (const InstancesInformation&) const method

/// Equal to operator. 
/// It compares this object with another object of the same class. 
/// It returns true if the members of the two objects have the same values, and false otherwise.
/// @ param other_instances_information Instances information object to be compared with.

bool InstancesInformation::operator == (const InstancesInformation& other_instances_information) const
{
   if(instances_number == other_instances_information.instances_number
   && training_indices == other_instances_information.training_indices
   && generalization_indices == other_instances_information.generalization_indices
   && testing_indices == other_instances_information.testing_indices
   && display == other_instances_information.display)
   {
      return(true);   
   }
   else
   {
      return(false);
   }
}

// METHODS

// const Vector<unsigned int>& get_training_indices(void) const method

/// This method returns the indices of the instances which will be used for training.

const Vector<unsigned int>& InstancesInformation::get_training_indices(void) const
{
   return(training_indices);
}


// const Vector<unsigned int>& get_generalization_indices(void) const method

/// This method returns the indices of the instances which will be used for generalization.

const Vector<unsigned int>& InstancesInformation::get_generalization_indices(void) const
{
   return(generalization_indices);
}


// const Vector<unsigned int>& get_testing_indices(void) const method

/// This method returns the indices of the instances which will be used for testing.

const Vector<unsigned int>& InstancesInformation::get_testing_indices(void) const
{
   return(testing_indices);
}


// const bool& get_display(void) const method

/// This method returns true if messages from this class can be displayed on the screen,
/// or false if messages from this class can't be displayed on the screen.

const bool& InstancesInformation::get_display(void) const
{
   return(display);   
}


// void set(void) method

/// This method sets a instances information object with zero instances. 

void InstancesInformation::set(void)
{
   set_instances_number(0);

   display = true;
}


// void set(const unsigned int&) method

/// This method sets a new number of instances in the instances information object. 
/// All the instances are set for training. 
/// @param new_instances_number Number of instances.

void InstancesInformation::set(const unsigned int& new_instances_number)
{
   set_instances_number(new_instances_number);

   display = true;
}


// void set(TiXmlElement*) method

/// This method sets the instances information members from a XML element. 
/// @param instances_information_element Pointer to a Tiny XML element containing the member data. 

void InstancesInformation::set(TiXmlElement* instances_information_element)
{
   set();
   from_XML(instances_information_element);
}


// void set_training_indices(const Vector<unsigned int>&) method

/// This method sets new indices for the training instances from a vector of integers.
/// The size of that vector is the number of training instances.
/// This method does not check for repeated indices. 
/// @param new_training_indices Indices of training instances. 

void InstancesInformation::set_training_indices(const Vector<unsigned int>& new_training_indices)
{
   // Control sentence (if debug)

   #ifdef _DEBUG 

   unsigned int size = new_training_indices.size();

   if(size > instances_number)
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: InstancesInformation class.\n"
             << "void set_training_indices(const Vector<double>&) method.\n"
             << "Size must be less or equal than number of instances.\n";

	  throw std::logic_error(buffer.str());
   }
   
   #endif

   training_indices = new_training_indices;
}


// void set_generalization_indices(const Vector<unsigned int>&) method

/// This method sets new indices for the generalization instances from a vector of integers.
/// The size of that vector is the number of generalization instances.
/// This method does not check for repeated indices. 
/// @param new_generalization_indices Indices of generalization instances. 

void InstancesInformation::set_generalization_indices(const Vector<unsigned int>& new_generalization_indices)
{
   // Control sentence (if debug)

   #ifdef _DEBUG 

   unsigned int size = new_generalization_indices.size();

   if(size > instances_number)
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: InstancesInformation class.\n"
             << "void set_generalization_indices(const Vector<double>&) method.\n"
             << "Size must be less or equal than number of instances.\n";

	  throw std::logic_error(buffer.str());
   }
   
   #endif

   generalization_indices = new_generalization_indices;
}


// void set_testing_indices(const Vector<unsigned int>&) method

/// This method sets new indices for the testing instances from a vector of integers.
/// The size of that vector is the number of testing instances.
/// This method does not check for repeated indices. 
/// @param new_testing_indices Indices of testing instances. 

void InstancesInformation::set_testing_indices(const Vector<unsigned int>& new_testing_indices)
{
   // Control sentence (if debug)

   #ifdef _DEBUG 

   unsigned int size = new_testing_indices.size();

   if(size > instances_number)
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: InstancesInformation class.\n"
             << "void set_testing_indices(const Vector<double>&) method.\n"
             << "Size must be less or equal than number of instances.\n";

	  throw std::logic_error(buffer.str());
   }
   
   #endif

   testing_indices = new_testing_indices;
}


// void set_training(void) method

/// This method sets all the instances in the data set for training. 

void InstancesInformation::set_training(void)
{
   training_indices.set(0, 1, instances_number-1);
   generalization_indices.set(0);
   testing_indices.set(0);
}


// void set_generalization(void) method

/// This method sets all the instances in the data set for generalization. 

void InstancesInformation::set_generalization(void)
{
   training_indices.set(0);
   generalization_indices.set(0, 1, instances_number-1);
   testing_indices.set(0);
}


// void set_testing(void) method

/// This method sets all the instances in the data set for testing. 

void InstancesInformation::set_testing(void)
{
   training_indices.set(0);
   generalization_indices.set(0);
   testing_indices.set(0, 1, instances_number-1);
}


// void set_default_instances_indices(void) method

/// This method splits the instances indices with the sequential method.

void InstancesInformation::set_default_instances_indices(void)
{
   split_sequential_indices();
}


// void set_display(const bool&) method

/// This method sets a new display value. 
/// If it is set to true messages from this class are to be displayed on the screen;
/// if it is set to false messages from this class are not to be displayed on the screen.
/// @param new_display Display value.

void InstancesInformation::set_display(const bool& new_display)
{
   display = new_display;
}


// void set_instances_number(unsigned int) method

/// This method sets a new number of instances in the data set. 
/// All instances are also set for training. 
/// @param new_instances_number Number of instances. 

void InstancesInformation::set_instances_number(const unsigned int& new_instances_number)
{
   instances_number = new_instances_number;

   training_indices.set(new_instances_number);
   training_indices.initialize_sequential();

   generalization_indices.set();

   testing_indices.set();
}


// TiXmlElement* to_XML(void) const method

/// This method serializes the instances information object into a XML element of the TinyXML library. 
/// See the OpenNN manual for more information about the format of this element. 

TiXmlElement* InstancesInformation::to_XML(void) const
{
   std::ostringstream buffer;

   // Instances information 

   TiXmlElement* instances_information_element = new TiXmlElement("InstancesInformation");
   instances_information_element->SetAttribute("Version", 4);

   // Instances number 

   TiXmlElement* instances_number_element = new TiXmlElement("InstancesNumber");
   instances_information_element->LinkEndChild(instances_number_element);

   buffer.str("");
   buffer << instances_number;

   TiXmlText* instances_number_text = new TiXmlText(buffer.str().c_str());
   instances_number_element->LinkEndChild(instances_number_text);

   // Training indices

   if(!training_indices.empty())
   {
      TiXmlElement* training_indices_element = new TiXmlElement("TrainingIndices");
      instances_information_element->LinkEndChild(training_indices_element);

	  Vector<unsigned int> new_training_indices = training_indices + 1;

      buffer.str("");
      buffer << new_training_indices;

      TiXmlText* training_indices_text = new TiXmlText(buffer.str().c_str());
      training_indices_element->LinkEndChild(training_indices_text);
   }

   // Generalization indices

   if(!generalization_indices.empty())
   {
      TiXmlElement* generalization_indices_element = new TiXmlElement("GeneralizationIndices");
      instances_information_element->LinkEndChild(generalization_indices_element);

	  Vector<unsigned int> new_generalization_indices = generalization_indices + 1;

      buffer.str("");
      buffer << new_generalization_indices;

      TiXmlText* generalization_indices_text = new TiXmlText(buffer.str().c_str());
      generalization_indices_element->LinkEndChild(generalization_indices_text);
   }

   // Testing indices

   if(!testing_indices.empty())
   {
      TiXmlElement* testing_indices_element = new TiXmlElement("TestingIndices");
      instances_information_element->LinkEndChild(testing_indices_element);

	  Vector<unsigned int> new_testing_indices = testing_indices + 1;

      buffer.str("");
      buffer << new_testing_indices;

      TiXmlText* testing_indices_text = new TiXmlText(buffer.str().c_str());
      testing_indices_element->LinkEndChild(testing_indices_text);
   }

   return(instances_information_element);
}


// void from_XML(TiXmlElement*) method

/// This method deserializes a TinyXML element into this instances information object. 
/// @param instances_information_element Pointer to a XML element containing the member data. 

void InstancesInformation::from_XML(TiXmlElement* instances_information_element)
{
   if(!instances_information_element)
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: InstancesInformation class.\n"
             << "void from_XML(TiXmlElement*) method.\n"
             << "Pointer to instances information element is NULL.\n";

	  throw std::logic_error(buffer.str());   
   }

   // Instances number 

   TiXmlElement* instances_number_element = instances_information_element->FirstChildElement("InstancesNumber");

   if(instances_number_element)
   {        
      unsigned int new_instances_number = atoi(instances_number_element->GetText());           
  
      try
      {
         set_instances_number(new_instances_number);
      }
      catch(std::exception& e)
      {
         std::cout << e.what() << std::endl;		 
      }      
   }

   // Training indices

   TiXmlElement* training_indices_element = instances_information_element->FirstChildElement("TrainingIndices");

   if(training_indices_element)
   {        
      Vector<unsigned int> new_training_indices;
      new_training_indices.parse(training_indices_element->GetText());
	  new_training_indices -= 1;

      try
      {
         set_training_indices(new_training_indices);
      }
      catch(std::exception& e)
      {
         std::cout << e.what() << std::endl;		 
      }
   }

   // Generalization indices

   TiXmlElement* generalization_indices_element = instances_information_element->FirstChildElement("GeneralizationIndices");

   if(generalization_indices_element)
   {        
      Vector<unsigned int> new_generalization_indices;
      new_generalization_indices.parse(generalization_indices_element->GetText());
	  new_generalization_indices -= 1;

      try
      {
         set_generalization_indices(new_generalization_indices);
      }
      catch(std::exception& e)
      {
         std::cout << e.what() << std::endl;		 
      }
   }

   // Testing indices

   TiXmlElement* testing_indices_element = instances_information_element->FirstChildElement("TestingIndices");

   if(testing_indices_element)
   {        
      Vector<unsigned int> new_testing_indices;
      new_testing_indices.parse(testing_indices_element->GetText());
	  new_testing_indices -= 1;

      try
      {
         set_testing_indices(new_testing_indices);
      }
      catch(std::exception& e)
      {
         std::cout << e.what() << std::endl;		 
      }
   }
}


// void split_given_indices(const Vector<double>&, const Vector<double>&, const Vector<double>&) method

/// This method sets new training, generalization and testing indices from given vectors. 
/// @param new_training_indices Indices of instances to be used for training. 
/// @param new_generalization_indices Indices of instances to be used for generalization. 
/// @param new_testing_indices Indices of instances to be used for testing. 

void InstancesInformation::split_given_indices
(const Vector<unsigned int>& new_training_indices, const Vector<unsigned int>& new_generalization_indices, const Vector<unsigned int>& new_testing_indices)
{
   const unsigned int new_training_instances_number = new_training_indices.size();
   const unsigned int new_generalization_instances_number = new_generalization_indices.size();
   const unsigned int new_testing_instances_number = new_testing_indices.size();

   const unsigned int new_instances_number = new_training_instances_number+new_generalization_instances_number+new_testing_instances_number;

   if(display && new_instances_number != instances_number)
   {
      std::cout << "OpenNN Warning: InstancesInformation class.\n" 
                << "void split_given_indices(const Vector<double>&, const Vector<double>&, const Vector<double>&) method.\n"
                << "New number of instances is not equal to number of instances.\n";
   }
     
   training_indices = new_training_indices;
   generalization_indices = new_generalization_indices;
   testing_indices = new_testing_indices;
}


// void split_random_indices(const double&, const double&, const double&) method

/// This method creates new training, generalization and testing indices at random. 
/// @param training_instances_ratio Ratio of training instances in the data set.
/// @param generalization_instances_ratio Ratio of generalization instances in the data set.
/// @param testing_instances_ratio Ratio of testing instances in the data set.

void InstancesInformation::split_random_indices
(const double& training_instances_ratio, const double& generalization_instances_ratio, const double& testing_instances_ratio)
{
   const double total_ratio = training_instances_ratio + generalization_instances_ratio + testing_instances_ratio;

   // Get number of instances for training, generalization and testing

   const unsigned int generalization_instances_number = (unsigned int)(generalization_instances_ratio*instances_number/total_ratio);
   const unsigned int testing_instances_number = (unsigned int)(testing_instances_ratio*instances_number/total_ratio);
   const unsigned int training_instances_number = instances_number - generalization_instances_number - testing_instances_number;

   const unsigned int sum_instances_number = training_instances_number + generalization_instances_number + testing_instances_number;

   if(sum_instances_number != instances_number)
   {
      std::ostringstream buffer;

      buffer << "OpenNN Warning: InstancesInformation class.\n" 
                << "void split_random_indices(double, double, double) method.\n"
                << "Sum of numbers of training, generalization and testing instances is not equal to number of instances.\n";
   }

   // Resize training, generalization and testing data set objects

   generalization_indices.set(generalization_instances_number);
   testing_indices.set(testing_instances_number);
   training_indices.set(training_instances_number);

   Vector<int> indices(0, 1, instances_number-1);
   std::random_shuffle(indices.begin(), indices.end());

   // Training indices

   for(unsigned int i = 0; i < training_instances_number; i++)
   {
      training_indices[i] = indices[i];
   }

   std::sort(training_indices.begin(), training_indices.end());

   // Generalization indices

   for(unsigned int i = 0; i < generalization_instances_number; i++)           
   {
      generalization_indices[i] = indices[training_instances_number+i];   
   }

   std::sort(generalization_indices.begin(), generalization_indices.end());

   // Testing indices

   for(unsigned int i = 0; i < testing_instances_number; i++)           
   {
      testing_indices[i] = indices[training_instances_number+generalization_instances_number+i];   
   }

   std::sort(testing_indices.begin(), testing_indices.end());
}


// void split_random_indices(void) method

/// This method creates new training, generalization and testing indices at random. 
/// It uses the following default values:
/// <ul>
/// <li> Training instances ratio: 0.6
/// <li> Generalization instances ratio: 0.2
/// <li> Testing instances ratio: 0.2
/// </ul>

void InstancesInformation::split_random_indices(void)
{
   split_random_indices(0.6, 0.2, 0.2);
}


// void split_sequential_indices(const double&, const double&, const double&) method

/// This method creates new training, generalization and testing indices with sequential indices. 
/// @param training_instances_ratio Ratio of training instances in the data set.
/// @param generalization_instances_ratio Ratio of generalization instances in the data set.
/// @param testing_instances_ratio Ratio of testing instances in the data set.

void InstancesInformation::split_sequential_indices(const double& training_instances_ratio, const double& generalization_instances_ratio, const double& testing_instances_ratio)
{
   const double total_ratio = training_instances_ratio + generalization_instances_ratio + testing_instances_ratio;

   // Get number of instances for training, generalization and testing

   const unsigned int generalization_instances_number = (unsigned int)(generalization_instances_ratio*instances_number/total_ratio);
   const unsigned int testing_instances_number = (unsigned int)(testing_instances_ratio*instances_number/total_ratio);
   const unsigned int training_instances_number = instances_number - generalization_instances_number - testing_instances_number;

   const unsigned int sum_instances_number = training_instances_number + generalization_instances_number + testing_instances_number;

   if(sum_instances_number != instances_number)
   {
      std::ostringstream buffer;

      buffer << "OpenNN Warning: InstancesInformation class.\n" 
             << "void split_random_indices(double, double, double) method.\n"
             << "Sum of numbers of training, generalization and testing instances is not equal to number of instances.\n";
   
   }

   // Training indices

   training_indices.set(0, 1, training_instances_number-1);

   // Generalization indices

   generalization_indices.set(training_instances_number, 1, training_instances_number+ generalization_instances_number-1);

   // Testing indices

   testing_indices.set(training_instances_number+generalization_instances_number, 1, instances_number-1);
}


// void split_sequential_indices(void) method

/// This method creates new training, generalization and testing indices with sequential indices. 
/// It uses the following default values:
/// <ul>
/// <li> Training instances ratio: 0.6
/// <li> Generalization instances ratio: 0.2
/// <li> Testing instances ratio: 0.2
/// </ul>

void InstancesInformation::split_sequential_indices(void)
{
   split_sequential_indices(0.6, 0.2, 0.2);
}


// std::string to_string(void) const method

/// This method returns a string representation of the current instances information object. 

std::string InstancesInformation::to_string(void) const
{
   std::ostringstream buffer;

   buffer << "InstancesInformation\n"
          << "Training indices: " << training_indices << "\n"
          << "Generalization indices: " << generalization_indices << "\n"
          << "Testing indices: " << testing_indices << "\n"
          << "Display: " << display << "\n";

   return(buffer.str());
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
