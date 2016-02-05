
/*                                                                                                              */
/*   OpenNN: Open Neural Networks Library                                                                       */
/*   www.opennn.cimne.com                                                                                       */
/*                                                                                                              */
/*   I N P U T S   O U T P U T S   I N F O R M A T I O N   C L A S S   H E A D E R                              */
/*                                                                                                              */
/*   Roberto Lopez                                                                                              */
/*   International Center for Numerical Methods in Engineering (CIMNE)                                          */
/*   Technical University of Catalonia (UPC)                                                                    */
/*   Barcelona, Spain                                                                                           */
/*   E-mail: rlopez@cimne.upc.edu                                                                               */
/*                                                                                                              */
/****************************************************************************************************************/

#ifndef __INPUTSOUTPUTSINFORMATION_H__
#define __INPUTSOUTPUTSINFORMATION_H__

// System includes

#include <string>

// OpenNN includes

#include "../utilities/vector.h"
#include "../utilities/matrix.h"

// TinyXml includes

#include "../../tinyxml/tinyxml.h"

namespace OpenNN
{

/// This class is used to store some information about the inputs and output variables of a neural network.
/// That information basically consists on the names, units and descriptions of the inputs and the output variables. 

class InputsOutputsInformation
{

public:

   // DEFAULT CONSTRUCTOR

   explicit InputsOutputsInformation(void);

   // INPUTS AND OUTPUTS NUMBER CONSTRUCTOR

   explicit InputsOutputsInformation(const unsigned int&, const unsigned int&);


   ///explicit InputsOutputsInformation(const Vector< Vector<std::string> >&);

   // XML CONSTRUCTOR

   explicit InputsOutputsInformation(TiXmlElement*);


   // COPY CONSTRUCTOR

   InputsOutputsInformation(const InputsOutputsInformation&);

   // DESTRUCTOR

   virtual ~InputsOutputsInformation(void);

   // ASSIGNMENT OPERATOR

   InputsOutputsInformation& operator = (const InputsOutputsInformation&);

   // EQUAL TO OPERATOR

   bool operator == (const InputsOutputsInformation&) const;

   // METHOD

   bool is_empty(void) const;

   /// This method returns the number of inputs in the multilayer perceptron

   inline unsigned int count_inputs_number(void) const
   {
      return(inputs_name.size());
   }

   /// This method returns the number of outputs neurons in the multilayer perceptron 

   inline int count_outputs_number(void) const
   {
      return(outputs_name.size());
   }

   // Input variables information

   const Vector<std::string>& get_inputs_name(void) const;
   const std::string& get_input_name(const unsigned int&) const;

   const Vector<std::string>& get_inputs_units(void) const;
   const std::string& get_input_units(const unsigned int&) const;

   const Vector<std::string>& get_inputs_description(void) const;
   const std::string& get_input_description(const unsigned int&) const;

   // Output variables information

   const Vector<std::string>& get_outputs_name(void) const;
   const std::string& get_output_name(const unsigned int&) const;

   const Vector<std::string>& get_outputs_units(void) const;
   const std::string& get_output_units(const unsigned int&) const;

   const Vector<std::string>& get_outputs_description(void) const;
   const std::string& get_output_description(const unsigned int&) const;

   // Variables information

   Vector< Vector<std::string>* > get_information(void);

   // Display messages

   const bool& get_display(void) const;

   // SET METHODS

   void set(void);
   void set(const unsigned int&, const unsigned int&);
   void set(const Vector< Vector<std::string> >&);
   void set(const InputsOutputsInformation&);

   void set_inputs_number(const unsigned int&);
   void set_outputs_number(const unsigned int&);

   virtual void set_default(void);

   // Input variables information

   void set_inputs_name(const Vector<std::string>&);
   void set_input_name(const unsigned int&, const std::string&);

   void set_inputs_units(const Vector<std::string>&);
   void set_input_units(const unsigned int&, const std::string&);

   void set_inputs_description(const Vector<std::string>&);
   void set_input_description(const unsigned int&, const std::string&);

   // Output variables information

   void set_outputs_name(const Vector<std::string>&);
   void set_output_name(const unsigned int&, const std::string&);

   void set_outputs_units(const Vector<std::string>&);
   void set_output_units(const unsigned int&, const std::string&);

   void set_outputs_description(const Vector<std::string>&);
   void set_output_description(const unsigned int&, const std::string&);

   // Variables information

   void set_information(const Vector< Vector<std::string> >&);

   void set_display(const bool&);

   void initialize_random(void);


   Vector<std::string> write_default_inputs_name(void) const; 
   Vector<std::string> write_default_outputs_name(void) const; 

   // Serialization methods

   std::string to_string(void) const;

   virtual TiXmlElement* to_XML(void) const;
   virtual void from_XML(TiXmlElement*);

protected:

   // MEMBERS

   /// Name of input variables.

   Vector<std::string> inputs_name;

   /// Units of input variables.

   Vector<std::string> inputs_units;

   /// Description of input variables.

   Vector<std::string> inputs_description;

   /// Name of output variables.

   Vector<std::string> outputs_name;

   /// Units of output variables.

   Vector<std::string> outputs_units;

   /// Description of output variables.

   Vector<std::string> outputs_description;

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

