/****************************************************************************************************************/
/*                                                                                                              */
/*   OpenNN: Open Neural Networks Library                                                                       */
/*   www.opennn.cimne.com                                                                                       */
/*                                                                                                              */
/*   V A R I A B L E S   I N F O R M A T I O N   C L A S S                                                      */
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

#include "variables_information.h"

// TinyXml includes

#include "../../tinyxml/tinyxml.h"

namespace OpenNN
{

// DEFAULT CONSTRUCTOR

/// Default constructor. 
/// It creates a variables information object with zero variables. 
/// It also initializes the rest of class members to their default values.

VariablesInformation::VariablesInformation(void)
{
   set();  
}


// VARIABLES NUMBER CONSTRUCTOR

/// Variables number constructor. 
/// It creates a variables information object with a given number of variables.
/// All the variables are set as inputs but the last, which is set as targets. 
/// It also initializes the rest of class members to their default values.
/// @param new_variables_number Number of variables.

VariablesInformation::VariablesInformation(const unsigned int& new_variables_number)
{
   set(new_variables_number);
}


// INPUT AND TARGET VARIABLES NUMBERS CONSTRUCTOR

/// Variables number constructor. 
/// It creates a variables information object with a given number of input and target variables.
/// It also initializes the rest of class members to their default values.
/// @param new_inputs_number Number of input variables.
/// @param new_targets_number Number of target variables.

VariablesInformation::VariablesInformation(const unsigned int& new_inputs_number, const unsigned int& new_targets_number)
{
   set(new_inputs_number, new_targets_number);
}


// XML CONSTRUCTOR
/// XML constructor. 
/// It creates a variables information object and loads its members from a XML element. 
/// @param variables_information_element TinyXML element with the member data. 

VariablesInformation::VariablesInformation(TiXmlElement* variables_information_element)
{
   set(variables_information_element);
}


// COPY CONSTRUCTOR

/// Copy constructor. 
/// It creates a copy of an existing variables information object. 
/// @param other_variables_information Variables information object to be copied.

VariablesInformation::VariablesInformation(const VariablesInformation& other_variables_information)
{
   variables_number = other_variables_information.variables_number;

   // Input and target variables

   inputs_indices = other_variables_information.inputs_indices;
   targets_indices = other_variables_information.targets_indices;

   // Variables information

   names = other_variables_information.names;
   units = other_variables_information.units;
   descriptions = other_variables_information.descriptions;

   // Utilities

   display = other_variables_information.display;
}


// DESTRUCTOR

/// Destructor. 

VariablesInformation::~VariablesInformation(void)
{
}


// ASSIGNMENT OPERATOR

/// Assignment operator. 
/// It assigns to the current object the members of an existing variables information object.
/// @param other_variables_information Variables information object to be assigned.

VariablesInformation& VariablesInformation::operator=(const VariablesInformation& other_variables_information)
{
   if(this != &other_variables_information) 
   {
      variables_number = other_variables_information.variables_number;

      // Input and target variables

      inputs_indices = other_variables_information.inputs_indices;
      targets_indices = other_variables_information.targets_indices;

      // Variables information

      names = other_variables_information.names;
      units = other_variables_information.units;
      descriptions = other_variables_information.descriptions;

      // Utilities

      display = other_variables_information.display;
   }

   return(*this);
}


// EQUAL TO OPERATOR

// bool operator == (const VariablesInformation&) const method

/// Equal to operator. 
/// It compares this object with another object of the same class. 
/// It returns true if the members of the two objects have the same values, and false otherwise.
/// @ param other_variables_information Variables information object to be compared with.

bool VariablesInformation::operator == (const VariablesInformation& other_variables_information) const
{
   if(variables_number == other_variables_information.variables_number
   && inputs_indices == other_variables_information.inputs_indices
   && targets_indices == other_variables_information.targets_indices
   && names == other_variables_information.names
   && units == other_variables_information.units
   && descriptions == other_variables_information.descriptions
   && display == other_variables_information.display)
   {
      return(true);
   }
   else
   {
      return(false);
   }
}


// METHODS


// const Vector<unsigned int>& get_inputs_indices(void) const method

/// This method returns the indices of the input variables.

const Vector<unsigned int>& VariablesInformation::get_inputs_indices(void) const
{
   return(inputs_indices);
}


// const Vector<unsigned int>& get_targets_indices(void) const method

/// This method returns the indices of the target variables.

const Vector<unsigned int>& VariablesInformation::get_targets_indices(void) const
{
   return(targets_indices);
}


// const Vector<std::string>& get_names(void) const method

/// This method returns the names of all the variables in the data set.

const Vector<std::string>& VariablesInformation::get_names(void) const
{
   return(names);
}


// std::string& get_name(unsigned int) method

/// This method returns the name of a single variable in the data set.
/// @param i Index of variable. 

const std::string& VariablesInformation::get_name(const unsigned int& i) const
{
   // Control sentence (if debug)

   #ifdef _DEBUG 

   unsigned int variables_number = get_variables_number();

   if(i >= variables_number)
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: VariablesInformation class.\n"
             << "std::string& get_name(unsigned int) method.\n"
             << "Index of variable must be less than number of variables.\n";

	  throw std::logic_error(buffer.str());
   }

   #endif

   return(names[i]);
}


// Vector<std::string> arrange_inputs_name(void) const method

/// This method returns the names of the input variables in the data set.

Vector<std::string> VariablesInformation::arrange_inputs_name(void) const
{
   const unsigned int inputs_number = count_inputs_number();

   Vector<std::string> inputs_name(inputs_number);

   unsigned int index;

   for(unsigned int i = 0; i < inputs_number; i++)
   {        
      index = inputs_indices[i];

	  inputs_name[i] = names[index]; 
   }

   return(inputs_name);
}


// Vector<std::string> arrange_targets_name(void) const method

/// This method returns the names of the target variables in the data set.

Vector<std::string> VariablesInformation::arrange_targets_name(void) const
{
   const unsigned int targets_number = count_targets_number();

   Vector<std::string> targets_name(targets_number);

   unsigned int index;

   for(unsigned int i = 0; i < targets_number; i++)
   {        
      index = targets_indices[i];

	  targets_name[i] = names[index];
   } 

   return(targets_name);
}


// const Vector<std::string>& get_units(void) const method

/// This method returns the units of all the variables in the data set. 

const Vector<std::string>& VariablesInformation::get_units(void) const
{
   return(units);
}


// const std::string& get_units(const unsigned int&) const method

/// This method returns the units of a single variable in the data set. 
/// @param i Index of variable. 

const std::string& VariablesInformation::get_unit(const unsigned int& i) const
{
   // Control sentence (if debug)

   #ifdef _DEBUG 

   unsigned int variables_number = get_variables_number();

   if(i >= variables_number)
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: VariablesInformation class.\n"
             << "const std::string& get_units(const unsigned int&) const method.\n"
             << "Index of variable must be less than number of variables.\n";

	  throw std::logic_error(buffer.str());
   }

   #endif

   return(units[i]);
}


// Vector<std::string> arrange_inputs_units(void) const method

/// This method returns the units of the input variables in the data set. 

Vector<std::string> VariablesInformation::arrange_inputs_units(void) const
{
   unsigned int variables_number = get_variables_number();

   Vector<std::string> inputs_units;

   if(units.size() == variables_number)
   {
      unsigned int inputs_number = count_inputs_number();

      inputs_units.set(inputs_number);
      unsigned int index;

   	  for(unsigned int i = 0; i < inputs_number; i++)
	  {        
		 index = inputs_indices[i];

	     inputs_units[i] = units[index];
      } 
   }

   return(inputs_units);
}


// Vector<std::string> arrange_targets_units(void) const method

/// This method returns the units of the target variables in the data set. 

Vector<std::string> VariablesInformation::arrange_targets_units(void) const
{
   const unsigned int variables_number = get_variables_number();

   Vector<std::string> targets_units;

   if(units.size() == variables_number)
   {
      const unsigned int targets_number = count_targets_number();

      targets_units.set(targets_number);
      unsigned int index;

   	  for(unsigned int i = 0; i < targets_number; i++)
	  {        
		 index = targets_indices[i];

	     targets_units[i] = units[index];
      } 
   }

   return(targets_units);
}


// const Vector<std::string>& get_descriptions(void) const method

/// This method returns the description of all the variables in the data set. 

const Vector<std::string>& VariablesInformation::get_descriptions(void) const
{
   return(descriptions);
}


// const std::string& get_description(const unsigned int&) const method

/// This method returns the description of a single variable in the data set. 
/// @param i Index of variable.

const std::string& VariablesInformation::get_description(const unsigned int& i) const
{
   // Control sentence (if debug)

   #ifdef _DEBUG 

   unsigned int variables_number = get_variables_number();

   if(i >= variables_number)
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: VariablesInformation class.\n"
             << "const std::string& get_description(const unsigned int&) const method.\n"
             << "Index of variable must be less than number of variables.\n";

	  throw std::logic_error(buffer.str());
   }

   #endif

   return(descriptions[i]);
}


// Vector<std::string> arrange_inputs_description(void) const method

/// This method returns the description of the input variables in the data set. 

Vector<std::string> VariablesInformation::arrange_inputs_description(void) const
{
   const unsigned int variables_number = get_variables_number();

   Vector<std::string> inputs_description;

   if(units.size() == variables_number)
   {
      const unsigned int inputs_number = count_inputs_number();
      inputs_description.set(inputs_number);
      unsigned int index;

   	  for(unsigned int i = 0; i < inputs_number; i++)
	  {        
		 index = inputs_indices[i];

	     inputs_description[i] = descriptions[index];
      } 
   }

   return(inputs_description);
}


// Vector<std::string> arrange_targets_descriptions(void) const method

/// This method returns the description of the target variables in the data set. 

Vector<std::string> VariablesInformation::arrange_targets_description(void) const
{
   unsigned int variables_number = get_variables_number();

   Vector<std::string> target_descriptions;

   if(descriptions.size() == variables_number)
   {
      unsigned int targets_number = count_targets_number();

      target_descriptions.set(targets_number);
      unsigned int index;

   	  for(unsigned int i = 0; i < targets_number; i++)
	  {        
		 index = targets_indices[i];

	     target_descriptions[i] = descriptions[index];
      } 
   }

   return(target_descriptions);
}


// Vector< Vector<std::string> > arrange_inputs_targets_information(void) const method

/// This method returns a vector of vectors with the inputs and target variables information.
/// The size of this vector is six. 
/// The contents of each element are:
/// <ul>
/// <li> Name of input variables.
/// <li> Units of input variables.
/// <li> Description of input variables.
/// <li> Name of target variables.
/// <li> Units of target variables. 
/// <li> Description of target variables. 
/// </ul>

Vector< Vector<std::string> > VariablesInformation::arrange_inputs_targets_information(void) const
{
   unsigned int variable_index;

   unsigned int variables_number = get_variables_number();

   unsigned int inputs_number = count_inputs_number();

   Vector<std::string> inputs_name;
   Vector<std::string> inputs_units;
   Vector<std::string> inputs_description;

   for(unsigned int i = 0; i < inputs_number; i++)
   {
      variable_index = inputs_indices[i];

	  if(names.size() == variables_number)
	  {
         inputs_name.set(inputs_number);
	     inputs_name[i] = names[variable_index];
	  }

	  if(units.size() == variables_number)
	  {
         inputs_units.set(inputs_number);
   	     inputs_units[i] = units[variable_index];
	  }

	  if(descriptions.size() == variables_number)
	  {
         inputs_description.set(inputs_number);
	     inputs_description[i] = descriptions[variable_index];  
	  }
   }

   unsigned int targets_number = count_targets_number();

   Vector<std::string> targets_name;
   Vector<std::string> targets_units;
   Vector<std::string> target_descriptions;

   for(unsigned int i = 0; i < targets_number; i++)
   {
      variable_index = targets_indices[i];

	  if(names.size() == variables_number)
	  {
         targets_name.set(targets_number);
	     targets_name[i] = names[variable_index];
	  }

	  if(units.size() == variables_number)
	  {
         targets_units.set(targets_number);
   	     targets_units[i] = units[variable_index];
	  }

	  if(descriptions.size() == variables_number)
	  {
         target_descriptions.set(targets_number);
	     target_descriptions[i] = descriptions[variable_index];  
	  }
   }

   Vector< Vector<std::string> > variables_information(6);

   variables_information[0] = inputs_name;
   variables_information[1] = inputs_units;
   variables_information[2] = inputs_description;

   variables_information[3] = targets_name;
   variables_information[4] = targets_units;
   variables_information[5] = target_descriptions;

   return(variables_information);
}


// const bool& get_display(void) const method

/// This method returns true if messages from this class can be displayed on the screen,
/// or false if messages from this class can't be displayed on the screen.

const bool& VariablesInformation::get_display(void) const
{
   return(display);   
}


// void set(void) method

/// This method sets a variables information object with zero variables. 

void VariablesInformation::set(void)
{
   set_variables_number(0); 

   display = true;
}


// void set(const unsigned int&) method

/// This method sets a new number of variables in the current object. 
/// All the variables are set as inputs, but the last one, wich is set as targets. 
/// @param new_variables_number Number of variables.

void VariablesInformation::set(const unsigned int& new_variables_number)
{
   set_variables_number(new_variables_number);

   display = true;
}


// void set(const unsigned int&, const unsigned int&) method

/// This method sets new numbers of of inputs and target variables in the current object. 
/// The total number of variables is the number of inputs plus the number of targets. 
/// The first variables are set as inputs, and last ones are set as targets. 
/// @param inputs_number Number of input variables.
/// @param targets_number Number of target variables.

void VariablesInformation::set(const unsigned int& inputs_number, const unsigned int& targets_number)
{
   set_variables_number(inputs_number+targets_number);

   inputs_indices.set(inputs_number);
   inputs_indices.initialize_sequential();

   targets_indices.set(targets_number);
   targets_indices.initialize_sequential();
   targets_indices += inputs_number;
}


// void set(TiXmlElement*) method

/// This method sets the variables information members from a XML element. 
/// @param variables_information_element Pointer to a Tiny XML element containing the member data. 

void VariablesInformation::set(TiXmlElement* variables_information_element)
{
   from_XML(variables_information_element);
}


// void set_inputs_indices(const Vector<unsigned int>&) method

/// This method sets new indices for the input variables from a vector of integers.
/// The size of that vector is the number of input variables.
/// This method does not check for repeated indices. 
/// @param new_inputs_indices Indices of input variables. 

void VariablesInformation::set_inputs_indices(const Vector<unsigned int>& new_inputs_indices)
{
   inputs_indices = new_inputs_indices;
}


// void set_targets_indices(const Vector<unsigned int>&) method

/// This method sets new indices for the target variables from a vector of integers.
/// The size of that vector is the number of target variables.
/// This method does not check for repeated indices. 
/// @param new_targets_indices Indices of target variables. 

void VariablesInformation::set_targets_indices(const Vector<unsigned int>& new_targets_indices)
{
   targets_indices = new_targets_indices;
}


// void set_input(void) method

/// This method sets all the variables in the data set as input variables. 

void VariablesInformation::set_input(void)
{
   unsigned int variables_number = get_variables_number();

   inputs_indices.set(0, 1, variables_number-1);
}


// void set_target(void) method

/// This method sets all the variables in the data set as target variables. 

void VariablesInformation::set_target(void)
{
   unsigned int variables_number = get_variables_number();

   targets_indices.set(0, 1, variables_number-1);
}


// void set_default_indices(void) method

/// This method sets the default indices for the input and target variables:
/// <ul>
/// <li> Input indices: 0, ..., variables number-2.
/// <li> Target indices: variables number-1.
/// </ul>

void VariablesInformation::set_default_indices(void)
{
   unsigned int variables_number = get_variables_number();

   inputs_indices.set(variables_number-1);
   inputs_indices.initialize_sequential();

   targets_indices.set(1, variables_number-1);
}


// void set_names(const Vector<std::string>&) method

/// This method sets new names for the variables in the data set from a vector of strings. 
/// The size of that vector must be equal to the total number of variables. 
/// @param new_names Name of variables. 

void VariablesInformation::set_names(const Vector<std::string>& new_names)
{
   // Control sentence (if debug)

   #ifdef _DEBUG 

   unsigned int variables_number = get_variables_number();

   unsigned int size = new_names.size();

   if(size != variables_number)
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: VariablesInformation class.\n"
             << "void set_names(const Vector<std::string>&) method.\n"
             << "Size must be equal to number of variables.\n";

	  throw std::logic_error(buffer.str());
   }
   
   #endif

   names = new_names;
}


// void set_name(const unsigned int&, const std::string&) method

/// This method set the name of a single variable. 
/// If the vector of names is zero the rest of elements are initialized as empty strings. 
/// @param i Index of variable. 
/// @param new_name Name of variable. 

void VariablesInformation::set_name(const unsigned int& i, const std::string& new_name)
{
   unsigned int variables_number = get_variables_number();

   // Control sentence (if debug)

   #ifdef _DEBUG 

   if(i >= variables_number)
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: VariablesInformation class.\n"
             << "void set_name(const unsigned int&, const std::string&) method.\n"
             << "Index of variable must be less than number of variables.\n";

	  throw std::logic_error(buffer.str());
   }

   #endif

   if(names.size() != variables_number)
   {
      names.set(variables_number); 

   }

   // Set name of single variable

   names[i] = new_name;
}


// void set_units(const Vector<std::string>&) method

/// This method sets new units for the variables in the data set from a vector of strings. 
/// The size of that vector must be equal to the total number of variables. 
/// @param new_units Units of variables. 

void VariablesInformation::set_units(const Vector<std::string>& new_units)
{
   // Control sentence (if debug)

   #ifdef _DEBUG 

   unsigned int variables_number = get_variables_number();

   unsigned int size = new_units.size();

   if(size != variables_number)
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: VariablesInformation class.\n"
             << "void set_units(const Vector<std::string>&) method.\n"
             << "Size must be equal to number of variables.\n";

	  throw std::logic_error(buffer.str());
   }
   
   #endif

   units = new_units;
}


// void set_units(const unsigned int&, const std::string&) method

/// This method set the units of a single variable. 
/// If the vector of units is zero the rest of elements are initialized as empty strings. 
/// @param i Index of variable. 
/// @param new_units Units of variable. 

void VariablesInformation::set_units(const unsigned int& i, const std::string& new_units)
{
   unsigned int variables_number = get_variables_number();

   // Control sentence (if debug)

   #ifdef _DEBUG 

   if(i >= variables_number)
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: VariablesInformation class.\n"
             << "void set_units(const unsigned int&, const std::string&) method.\n"
             << "Index of variable must be less than number of variables.\n";

	  throw std::logic_error(buffer.str());
   }

   #endif

   if(units.size() != variables_number)
   {
      units.set(variables_number); 
   }

   // Set units of single variable

   units[i] = new_units;
}


// void set_descriptions(const Vector<std::string>&) method

/// This method sets new descriptions for the variables in the data set from a vector of strings. 
/// The size of that vector must be equal to the total number of variables. 
/// @param new_descriptions Description of variables. 

void VariablesInformation::set_descriptions(const Vector<std::string>& new_descriptions)
{
   // Control sentence (if debug)

   #ifdef _DEBUG 

   unsigned int variables_number = get_variables_number();

   unsigned int size = new_descriptions.size();

   if(size != variables_number)
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: VariablesInformation class.\n"
             << "void set_descriptions(const Vector<std::string>&) method.\n"
             << "Size must be equal to number of variables.\n";

	  throw std::logic_error(buffer.str());
   }
   
   #endif

   descriptions = new_descriptions;
}


// void set_description(const unsigned int&, const std::string&) method

/// This method set the description of a single variable. 
/// If the vector of descriptions is zero the rest of elements are initialized as empty strings. 
/// @param i Index of variable. 
/// @param new_description Description of variable. 

void VariablesInformation::set_description(const unsigned int& i, const std::string& new_description)
{
   unsigned int variables_number = get_variables_number();

   // Control sentence (if debug)

   #ifdef _DEBUG 

   if(i >= variables_number)
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: VariablesInformation class.\n"
             << "void set_description(const unsigned int&, const std::string&) method.\n"
             << "Index of variable must be less than number of variables.\n";

	  throw std::logic_error(buffer.str());
   }

   #endif

   if(descriptions.size() != variables_number)
   {
      descriptions.set(variables_number); 
   }

   // Set description of single variable

   descriptions[i] = new_description;
}


// void set_display(const bool&) method

/// This method sets a new display value. 
/// If it is set to true messages from this class are to be displayed on the screen;
/// if it is set to false messages from this class are not to be displayed on the screen.
/// @param new_display Display value.

void VariablesInformation::set_display(const bool& new_display)
{
   display = new_display;
}


// void set_variables_number(const unsigned int&) method

/// This method sets a new number of variables in the variables information object. 
/// All variables are set as inputs but the last one, which is set as targets. 
/// @param new_variables_number Number of variables. 

void VariablesInformation::set_variables_number(const unsigned int& new_variables_number)
{
   variables_number = new_variables_number;

   if(new_variables_number == 0)
   {
      inputs_indices.set();
      targets_indices.set();
   }
   else if(new_variables_number == 1)
   {
      inputs_indices.set(1, 0);
      targets_indices.set(1, 0);           
   }
   else
   {
      inputs_indices.set(new_variables_number-1);
      inputs_indices.initialize_sequential();

      targets_indices.set(1, new_variables_number-1);   
   }

   names.set(new_variables_number);
   units.set(new_variables_number);
   descriptions.set(new_variables_number);
}


// std::string to_string(void) const method

/// This method returns a string representation of the current variables information object. 

std::string VariablesInformation::to_string(void) const
{
   std::ostringstream buffer;

   buffer << "Variables information\n"
          << "Variables number: " << variables_number << "\n"
          << "Input indices: " << inputs_indices << "\n"
          << "Target indices: " << targets_indices << "\n";

   buffer << "Names\n";

   for(unsigned int i = 0; i < variables_number; i++)
   {
      buffer << "Variable " << i+1 << ": " << names[i] << "\n";
   }

   buffer << "Units\n";

   for(unsigned int i = 0; i < variables_number; i++)
   {
      buffer << "Variable " << i+1 << ": " << units[i] << "\n";
   }

   buffer << "Descriptions\n";

   for(unsigned int i = 0; i < variables_number; i++)
   {
      buffer << "Variable " << i+1 << ": " << descriptions[i] << "\n";
   }

   buffer << "Display: " << display << "\n";

   return(buffer.str());
}


// TiXmlElement* to_XML(void) const method

/// This method serializes the variables information object into a XML element of the TinyXML library. 
/// See the OpenNN manual for more information about the format of this element. 

TiXmlElement* VariablesInformation::to_XML(void) const
{
   std::ostringstream buffer;

   // Variables information

   TiXmlElement* variables_information_element = new TiXmlElement("VariablesInformation");
   variables_information_element->SetAttribute("Version", 4);

   // Variables number

   TiXmlElement* variables_number_element = new TiXmlElement("VariablesNumber");
   variables_information_element->LinkEndChild(variables_number_element);     

   buffer.str("");
   buffer << variables_number;

   TiXmlText* variables_number_text = new TiXmlText(buffer.str().c_str());
   variables_number_element->LinkEndChild(variables_number_text);

   // Variables name 

   if(!names.empty())
   {
      TiXmlElement* names_element = new TiXmlElement("Names");
      variables_information_element->LinkEndChild(names_element);     

      for(unsigned int i = 0; i < variables_number; i++)
      {
         TiXmlElement* element = new TiXmlElement("Name");
         element->SetAttribute("Index", i+1);
         names_element->LinkEndChild(element);
      
         TiXmlText* text = new TiXmlText(names[i].c_str());
         element->LinkEndChild(text);
      }
   }
   
   // Variables units 

   if(!units.empty())
   {
      TiXmlElement* units_element = new TiXmlElement("Units");
      variables_information_element->LinkEndChild(units_element);

      for(unsigned int i = 0; i < variables_number; i++)
      {
         TiXmlElement* element = new TiXmlElement("Unit");
         element->SetAttribute("Index", i+1);
         units_element->LinkEndChild(element);
      
         TiXmlText* text = new TiXmlText(units[i].c_str());
         element->LinkEndChild(text);
      }
   }

   // Variables description

   if(!descriptions.empty())
   {
      TiXmlElement* descriptions_element = new TiXmlElement("Descriptions");
      variables_information_element->LinkEndChild(descriptions_element);

      for(unsigned int i = 0; i < variables_number; i++)
      {
         TiXmlElement* element = new TiXmlElement("Description");
         element->SetAttribute("Index", i+1);
         descriptions_element->LinkEndChild(element);
      
         TiXmlText* text = new TiXmlText(descriptions[i].c_str());
         element->LinkEndChild(text);
      }
   }

   // Input variables indices

   if(!inputs_indices.empty())
   {
      TiXmlElement* inputs_indices_element = new TiXmlElement("InputIndices");
      variables_information_element->LinkEndChild(inputs_indices_element);

	  Vector<unsigned int> new_inputs_indices = inputs_indices + 1;

      buffer.str("");
      buffer << new_inputs_indices;

      TiXmlText* inputs_indices_text = new TiXmlText(buffer.str().c_str());
      inputs_indices_element->LinkEndChild(inputs_indices_text);
   }

   // Target variables indices

   if(!targets_indices.empty())
   {
      TiXmlElement* targets_indices_element = new TiXmlElement("TargetIndices");
      variables_information_element->LinkEndChild(targets_indices_element);

	  Vector<unsigned int> new_targets_indices = targets_indices + 1;

      buffer.str("");
      buffer << new_targets_indices;

      TiXmlText* targets_indices_text = new TiXmlText(buffer.str().c_str());
      targets_indices_element->LinkEndChild(targets_indices_text);
   }

   return(variables_information_element);
}


// void from_XML(TiXmlElement*) method

/// This method deserializes a TinyXML element into this variables information object. 
/// @param variables_information_element Pointer to a XML element containing the member data. 

void VariablesInformation::from_XML(TiXmlElement* variables_information_element)
{
   if(!variables_information_element)
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: VariablesInformation class.\n"
             << "void from_XML(TiXmlElement*) method.\n"
             << "Pointer to variables information element is NULL.\n";

	  throw std::logic_error(buffer.str());   
   }

   int index = 0;

   // Variables number 

   TiXmlElement* variables_number_element = variables_information_element->FirstChildElement("VariablesNumber");

   if(variables_number_element)
   {        
      unsigned int new_variables_number = atoi(variables_number_element->GetText());           
  
      try
      {
         set_variables_number(new_variables_number);
      }
      catch(std::exception& e)
      {
         std::cout << e.what() << std::endl;		 
      }
   }

   // Input indices

   TiXmlElement* inputs_indices_element = variables_information_element->FirstChildElement("InputIndices");

   if(inputs_indices_element)
   {        
      Vector<unsigned int> new_inputs_indices;
      new_inputs_indices.parse(inputs_indices_element->GetText());
	  new_inputs_indices -= 1;

      try
      {
         set_inputs_indices(new_inputs_indices);
      }
      catch(std::exception& e)
      {
         std::cout << e.what() << std::endl;		 
      }
   }

   // Target indices

   TiXmlElement* targets_indices_element = variables_information_element->FirstChildElement("TargetIndices");

   if(targets_indices_element)
   {        
      Vector<unsigned int> new_targets_indices;
      new_targets_indices.parse(targets_indices_element->GetText());
	  new_targets_indices -= 1;

      try
      {
         set_targets_indices(new_targets_indices);
      }
      catch(std::exception& e)
      {
         std::cout << e.what() << std::endl;		 
      }
   }

   // Variables name

   TiXmlElement* names_element = variables_information_element->FirstChildElement("Names");

   if(names_element)
   {        
      Vector<std::string> new_names(variables_number);
      
      TiXmlElement* name_element = names_element->FirstChildElement("Name");

      if(name_element)
      {
         name_element->QueryIntAttribute("Index", &index);

         if(name_element->GetText())
		 {
         new_names[index-1] = name_element->GetText();
      }

      for( ; name_element; name_element=name_element->NextSiblingElement())
      {
         name_element->QueryIntAttribute("Index", &index);

         if(name_element->GetText())
	 {
               new_names[index-1] = name_element->GetText();
	 }
         }
      }

      try
      {
         set_names(new_names);
      }
      catch(std::exception& e)
      {
         std::cout << e.what() << std::endl;		 
      }
   }

   // Variables units

   TiXmlElement* units_element = variables_information_element->FirstChildElement("Units");

   if(units_element)
   {        
      Vector<std::string> new_units(variables_number);
      
      TiXmlElement* variable_units_element = units_element->FirstChildElement("Unit");

      if(variable_units_element)
      {
         variable_units_element->QueryIntAttribute("Index", &index);

		 if(variable_units_element->GetText())
		 {
            new_units[index-1] = variable_units_element->GetText();
		 }

         for( ; variable_units_element; variable_units_element=variable_units_element->NextSiblingElement())
         {
            variable_units_element->QueryIntAttribute("Index", &index);

			if(variable_units_element->GetText())
			{
               new_units[index-1] = variable_units_element->GetText();
			}
         }
      }

      try
      {
         set_units(new_units);
      }
      catch(std::exception& e)
      {
         std::cout << e.what() << std::endl;		 
      }
   }

   // Variables description

   TiXmlElement* descriptions_element = variables_information_element->FirstChildElement("Descriptions");

   if(descriptions_element)
   {        
      Vector<std::string> new_descriptions(variables_number);
      
      TiXmlElement* variable_description_element = descriptions_element->FirstChildElement("Description");

      if(variable_description_element)
      {
         variable_description_element->QueryIntAttribute("Index", &index);

		 if(variable_description_element->GetText())
		 {
            new_descriptions[index-1] = variable_description_element->GetText();
		 }

         for( ; variable_description_element; variable_description_element=variable_description_element->NextSiblingElement())
         {
            variable_description_element->QueryIntAttribute("Index", &index);

			if(variable_description_element->GetText())
			{
               new_descriptions[index-1] = variable_description_element->GetText();
			}
         }
      }

      try
      {
         set_descriptions(new_descriptions);
      }
      catch(std::exception& e)
      {
         std::cout << e.what() << std::endl;		 
      }
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
