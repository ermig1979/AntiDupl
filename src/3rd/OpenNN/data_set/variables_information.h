/****************************************************************************************************************/
/*                                                                                                              */
/*   OpenNN: Open Neural Networks Library                                                                       */
/*   www.opennn.cimne.com                                                                                       */
/*                                                                                                              */
/*   V A R I A B L E S   I N F O R M A T I O N   C L A S S   H E A D E R                                        */
/*                                                                                                              */ 
/*   Roberto Lopez                                                                                              */ 
/*   International Center for Numerical Methods in Engineering (CIMNE)                                          */
/*   Technical University of Catalonia (UPC)                                                                    */
/*   Barcelona, Spain                                                                                           */
/*   E-mail: rlopez@cimne.upc.edu                                                                               */ 
/*                                                                                                              */
/****************************************************************************************************************/

#ifndef __VARIABLESINFORMATION_H__
#define __VARIABLESINFORMATION_H__

// System includes

#include <string>

// OpenNN includes

#include "../utilities/vector.h"
#include "../utilities/matrix.h"

// TinyXml includes

#include "../../tinyxml/tinyxml.h"


namespace OpenNN
{

/// This class is used to store information about the variables of a data set. 
/// Variables in a data set can be used as inputs and targets.
/// This class also stores information about the name, unit and description of all the variables.

class VariablesInformation
{

public:  

   // DEFAULT CONSTRUCTOR

   explicit VariablesInformation(void);

   // VARIABLES NUMBER CONSTRUCTOR

   explicit VariablesInformation(const unsigned int&);

   // INPUT AND TARGET VARIABLES NUMBER

   explicit VariablesInformation(const unsigned int&, const unsigned int&);

   // XML CONSTRUCTOR

   explicit VariablesInformation(TiXmlElement*);

   // COPY CONSTRUCTOR

   VariablesInformation(const VariablesInformation&);

   // DESTRUCTOR

   virtual ~VariablesInformation(void);

   // ASSIGNMENT OPERATOR

   VariablesInformation& operator = (const VariablesInformation&);

   // EQUAL TO OPERATOR

   bool operator == (const VariablesInformation&) const;

   // METHODS

   /// This method returns the total number of variables in the data set.

   inline const unsigned int& get_variables_number(void) const
   {
      return(variables_number);   
   }

   /// This method returns the number of input variables of the data set.

   inline unsigned int count_inputs_number(void) const
   {
      return(inputs_indices.size());   
   }

   /// This method returns the number of target variables of the data set.

   inline unsigned int count_targets_number(void) const
   {
      return(targets_indices.size());   
   }

   // Variables methods

   const Vector<unsigned int>& get_inputs_indices(void) const;
   const Vector<unsigned int>& get_targets_indices(void) const; 

   // Information methods

   const Vector<std::string>& get_names(void) const;
   const std::string& get_name(const unsigned int&) const;

   const Vector<std::string>& get_units(void) const;
   const std::string& get_unit(const unsigned int&) const;

   const Vector<std::string>& get_descriptions(void) const;
   const std::string& get_description(const unsigned int&) const;

   const bool& get_display(void) const;

   // Set methods

   void set(void);
   void set(const unsigned int&);
   void set(const unsigned int&, const unsigned int&);
   void set(TiXmlElement*);

   // Data methods

   void set_variables_number(const unsigned int&);

   // Variables methods

   void set_inputs_indices(const Vector<unsigned int>&);
   void set_targets_indices(const Vector<unsigned int>&);

   void set_input(void);
   void set_target(void);

   void set_default_indices(void);

   // Information methods

   void set_names(const Vector<std::string>&);
   void set_name(const unsigned int&, const std::string&);

   void set_units(const Vector<std::string>&);
   void set_units(const unsigned int&, const std::string&);

   void set_descriptions(const Vector<std::string>&);
   void set_description(const unsigned int&, const std::string&);

   void set_display(const bool&);


   Vector<std::string> arrange_inputs_units(void) const;
   Vector<std::string> arrange_targets_units(void) const;

   Vector<std::string> arrange_inputs_name(void) const;
   Vector<std::string> arrange_targets_name(void) const;

   Vector<std::string> arrange_inputs_description(void) const;
   Vector<std::string> arrange_targets_description(void) const;

   Vector< Vector<std::string> > arrange_inputs_targets_information(void) const;

   // Serialization methods

   std::string to_string(void) const;

   TiXmlElement* to_XML(void) const;
   void from_XML(TiXmlElement*);


private:

   // MEMBERS

   /// Number of variables

   unsigned int variables_number;

   /// Indices of input variables.

   Vector<unsigned int> inputs_indices;

   /// Indices of target variables.

   Vector<unsigned int> targets_indices;

   /// Name of variables.

   Vector<std::string> names;

   /// Units of variables.

   Vector<std::string> units;

   /// Description of variables.

   Vector<std::string> descriptions;

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
