/****************************************************************************************************************/
/*                                                                                                              */
/*   OpenNN: Open Neural Networks Library                                                                       */
/*   www.opennn.cimne.com                                                                                       */
/*                                                                                                              */
/*   I N P U T S   O U T P U T S   I N F O R M A T I O N   C L A S S                                            */
/*                                                                                                              */
/*   Roberto Lopez                                                                                              */
/*   International Center for Numerical Methods in Engineering (CIMNE)                                          */
/*   Technical University of Catalonia (UPC)                                                                    */
/*   Barcelona, Spain                                                                                           */
/*   E-mail: rlopez@cimne.upc.edu                                                                               */
/*                                                                                                              */
/****************************************************************************************************************/

// System includes

#include <cmath>   
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>

// OpenNN includes

#include "inputs_outputs_information.h"

// TinyXml includes

#include "../../tinyxml/tinyxml.h"

namespace OpenNN
{

// DEFAULT CONSTRUCTOR

/// Default constructor. 
/// It creates a inputs-outputs information object with zero inputs and zero outputs.

InputsOutputsInformation::InputsOutputsInformation(void)
{
   set();
}


// INPUTS AND OUTPUTS NUMBERS CONSTRUCTOR

/// Inputs and outputs numbers constructor. 
/// It creates a inputs-outputs information object with given numbers of inputs and outputs.
/// This constructor initializes the members of the object to their default values. 
/// @param new_inputs_number Number of inputs. 
/// @param new_outputs_number Number of outputs. 

InputsOutputsInformation::InputsOutputsInformation(const unsigned int& new_inputs_number, const unsigned int& new_outputs_number)
{
   set(new_inputs_number, new_outputs_number);
}


// XML CONSTRUCTOR

/// XML constructor. 
/// It creates an inputs-outputs information object and loads its members from a XML element. 
/// @param inputs_outputs_information_element TinyXML element with the member data. 

InputsOutputsInformation::InputsOutputsInformation(TiXmlElement* inputs_outputs_information_element)
{
   from_XML(inputs_outputs_information_element);
}


// COPY CONSTRUCTOR

/// Copy constructor. 
/// It creates a copy of an existing inputs outputs information object. 
/// @param other_inputs_outputs_information Inputs outputs information object to be copied.

InputsOutputsInformation::InputsOutputsInformation(const InputsOutputsInformation& other_inputs_outputs_information)
{
   set(other_inputs_outputs_information);
}


// DESTRUCTOR

/// Destructor.

InputsOutputsInformation::~InputsOutputsInformation(void)
{
}


// ASSIGNMENT OPERATOR

/// Assignment operator. 
/// It assigns to this object the members of an existing inputs-outputs information object.
/// @param other_inputs_outputs_information Inputs-outputs information object to be assigned.

InputsOutputsInformation& InputsOutputsInformation::operator = (const InputsOutputsInformation& other_inputs_outputs_information)
{
   if(this != &other_inputs_outputs_information) 
   {
      inputs_name = other_inputs_outputs_information.inputs_name;

      inputs_units = other_inputs_outputs_information.inputs_units;

      inputs_description = other_inputs_outputs_information.inputs_description;
 
      outputs_name = other_inputs_outputs_information.outputs_name;

      outputs_units = other_inputs_outputs_information.outputs_units;

      outputs_description = other_inputs_outputs_information.outputs_description;

      display = other_inputs_outputs_information.display;
   }

   return(*this);
}


// METHODS


// EQUAL TO OPERATOR

// bool operator == (const InputsOutputsInformation&) const method

/// Equal to operator. 
/// It compares this object with another object of the same class. 
/// It returns true if the members of the two objects have the same values, and false otherwise.
/// @ param other_inputs_outputs_information Inputs-oputputs information object to be compared with.

bool InputsOutputsInformation::operator == (const InputsOutputsInformation& other_inputs_outputs_information) const
{
   if(inputs_name == other_inputs_outputs_information.inputs_name
   && inputs_units == other_inputs_outputs_information.inputs_units
   && inputs_description == other_inputs_outputs_information.inputs_description
   && outputs_name == other_inputs_outputs_information.outputs_name
   && outputs_units == other_inputs_outputs_information.outputs_units
   && outputs_description == other_inputs_outputs_information.outputs_description
   && display == other_inputs_outputs_information.display)
   {
      return(true);
   }
   else
   {
      return(false);
   }
}


// bool is_empty(void) const method

/// This method returns true if both the numbers of inputs and outputs are zero, and false otherwise. 

bool InputsOutputsInformation::is_empty(void) const
{
   const unsigned int inputs_number = count_inputs_number();
   const unsigned int outputs_number = count_outputs_number();

   if(inputs_number == 0 && outputs_number == 0)
   {
      return(true);   
   }
   else
   {
      return(false);
   }
}


// const Vector<std::string>& get_inputs_name(void) const method

/// This method returns the names of the input variables.
/// Such names are only used to give the user basic information about the problem at hand.

const Vector<std::string>& InputsOutputsInformation::get_inputs_name(void) const
{
   return(inputs_name);
}


// const std::string& get_input_name(const unsigned int&) const method

/// This method returns the name of a single input variable. 
/// Such a name is only used to give the user basic information about the problem at hand.
/// @param input_index Index of input variable.

const std::string& InputsOutputsInformation::get_input_name(const unsigned int& input_index) const
{
   // Control sentence (if debug)

   #ifdef _DEBUG 

   const unsigned int inputs_number = count_inputs_number();

   if(input_index >= inputs_number)
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: InputsOutputsInformation class.\n"
             << "const std::string get_input_name(const unsigned int&) const method.\n"
             << "Input variable index must be less than number of inputs.\n";

	  throw std::logic_error(buffer.str());
   }

   if(inputs_name.size() == 0)
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: InputsOutputsInformation class.\n"
             << "const std::string get_input_name(const unsigned int&) const method.\n"
             << "Input variables size is zero.\n";

	  throw std::logic_error(buffer.str());
   }

   #endif

   return(inputs_name[input_index]);
}


// const Vector<std::string>& get_inputs_units(void) const method

/// This method returns the units of the input variables as strings. 
/// Such units are only used to give the user basic information about the problem at hand.

const Vector<std::string>& InputsOutputsInformation::get_inputs_units(void) const
{
   return(inputs_units);
}


// const std::string& get_input_units(const unsigned int&) const method

/// This method returns the units of a single input variable as a string. 
/// Such units are only used to give the user basic information about the problem at hand.
/// @param input_index Index of input variable.

const std::string& InputsOutputsInformation::get_input_units(const unsigned int& input_index) const
{
   // Control sentence (if debug)

   #ifdef _DEBUG 

   const unsigned int inputs_number = count_inputs_number();

   if(input_index >= inputs_number)
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: InputsOutputsInformation class.\n" 
             << "const std::string get_input_units(const unsigned int&) const method.\n"
             << "Index of input variable must be less than number of inputs.\n";

	  throw std::logic_error(buffer.str());
   }

   #endif

   return(inputs_units[input_index]);
}


// const Vector<std::string>& get_inputs_description(void) const method

/// This method returns the description of the input variables as strings. 
/// Such descriptions are only used to give the user basic information about the problem at hand.

const Vector<std::string>& InputsOutputsInformation::get_inputs_description(void) const
{
   return(inputs_description);
}


// const std::string get_input_description(const unsigned int&) const method

/// This method returns the description of a single input variable as a string. 
/// Such a description is only used to give the user basic information about the problem at hand.
/// @param input_index Index of input variable.

const std::string& InputsOutputsInformation::get_input_description(const unsigned int& input_index) const
{
   // Control sentence (if debug)

   #ifdef _DEBUG 

   const unsigned int inputs_number = count_inputs_number();

   if(input_index >= inputs_number)
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: InputsOutputsInformation class.\n" 
             << "const std::string& get_input_description(const unsigned int&) const method.\n"
             << "Index of input variable must be less than number of inputs.\n";

	  throw std::logic_error(buffer.str());
   }

   #endif

   return(inputs_description[input_index]);
}


// const Vector<std::string>& get_outputs_name(void) const method

/// This method returns the names of the output variables.
/// Such names are only used to give the user basic information about the problem at hand.

const Vector<std::string>& InputsOutputsInformation::get_outputs_name(void) const
{
   return(outputs_name);
}


// const std::string& get_output_name(const unsigned int&) const method

/// This method returns the name of a single output variable.
/// Such a name is only used to give the user basic information about the problem at hand.
/// @param output_index Index of output variable.

const std::string& InputsOutputsInformation::get_output_name(const unsigned int& output_index) const
{
   // Control sentence (if debug)

   #ifdef _DEBUG 

   const unsigned int outputs_number = count_outputs_number();

   if(output_index >= outputs_number)
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: InputsOutputsInformation class.\n"  
             << "const std::string get_output_name(const unsigned int&) const method.\n"
             << "Output variable index must be less than number of outputs.\n";

	  throw std::logic_error(buffer.str());
   }

   #endif

   return(outputs_name[output_index]);
}


// const Vector<std::string>& get_outputs_description(void) const method

/// This method returns the descriptions of the output variables as strings. 
/// Such descriptions are only used to give the user basic information about the problem at hand.

const Vector<std::string>& InputsOutputsInformation::get_outputs_description(void) const
{
   return(outputs_description);
}


// const std::string& get_output_description(const unsigned int&) const method

/// This method returns the description of a single input variable as a string. 
/// Such a description is only used to give the user basic information about the problem at hand.
/// @param output_index Index of output variable.

const std::string& InputsOutputsInformation::get_output_description(const unsigned int& output_index) const
{
   // Control sentence (if debug)

   #ifdef _DEBUG 

   const unsigned int outputs_number = count_outputs_number();

   if(output_index >= outputs_number)
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: InputsOutputsInformation class.\n" 
             << "const std::string& get_output_description(const unsigned int&) const method.\n"
             << "Index of output variable must be less than number of outputs.\n";

	  throw std::logic_error(buffer.str());
   }

   #endif

   return(outputs_description[output_index]);
}


// const Vector<std::string>& get_outputs_units(void) const method

/// This method returns the units of the output variables as strings. 
/// Such units are only used to give the user basic information about the problem at hand.

const Vector<std::string>& InputsOutputsInformation::get_outputs_units(void) const
{
   return(outputs_units);
}


// const std::string& get_output_units(const unsigned int&) const method

/// This method returns the units of a single output variable as a string. 
/// Such units are only used to give the user basic information about the problem at hand.
/// @param output_index Index of output variable.

const std::string& InputsOutputsInformation::get_output_units(const unsigned int& output_index) const
{
   // Control sentence (if debug)

   #ifdef _DEBUG 

   const unsigned int outputs_number = count_outputs_number();

   if(output_index >= outputs_number)
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: InputsOutputsInformation class.\n" 
             << "const std::string get_output_units(const unsigned int&) const method.\n"
             << "Index of output variable must be less than number of outputs.\n";

	  throw std::logic_error(buffer.str());
   }

   #endif

   return(outputs_units[output_index]);
}


// Vector< Vector<std::string> > get_information(void) method

/// This method returns all the available information about the inputs and output variables. 
/// The format is a vector of pointers to vectors of size six: 
/// <ul>
/// <li> Name of input variables.
/// <li> Name of output variables.
/// <li> Units of input variables.
/// <li> Units of output variables.
/// <li> Description of input variables.
/// <li> Description of output variables.
/// </ul>

Vector< Vector<std::string>* > InputsOutputsInformation::get_information(void)
{
   Vector< Vector<std::string>* > information(6);
 
   information[0] = &inputs_name;
   information[1] = &outputs_name;

   information[2] = &inputs_units;
   information[3] = &outputs_units;

   information[4] = &inputs_description;
   information[5] = &outputs_description;

   return(information);
}


// const bool& get_display(void) const method

/// This method returns true if messages from this class are to be displayed on the screen, or false if messages 
/// from this class are not to be displayed on the screen.

const bool& InputsOutputsInformation::get_display(void) const
{
   return(display);
}


// void set(void) method

/// This method sets zero inputs and outputs numbers.
/// It also sets the rest of members to their default values. 

void InputsOutputsInformation::set(void)
{
   set_inputs_number(0);

   set_outputs_number(0);

   set_default();
}


// void set(const unsigned int&, const unsigned int&) method

/// This method sets new number of inputs and outputs.
/// It also sets the rest of members to their default values. 
/// @param new_inputs_number Number of inputs. 
/// @param new_outputs_number Number of outputs. 

void InputsOutputsInformation::set(const unsigned int& new_inputs_number, const unsigned int& new_outputs_number)
{
   set_inputs_number(new_inputs_number);

   set_outputs_number(new_outputs_number);

   set_default();
}


// void set(const InputsOutputsInformation&) method

/// This method sets the members of this inputs-outputs information object with those from other inputs-outputs information object. 
/// @param other_inputs_outputs_information Inputs-outputs information object to be copied. 

void InputsOutputsInformation::set(const InputsOutputsInformation& other_inputs_outputs_information)
{
   inputs_name = other_inputs_outputs_information.inputs_name;

   inputs_units = other_inputs_outputs_information.inputs_units;

   inputs_description = other_inputs_outputs_information.inputs_description;

   outputs_name = other_inputs_outputs_information.outputs_name;

   outputs_units = other_inputs_outputs_information.outputs_units;

   outputs_description = other_inputs_outputs_information.outputs_description;

   display = other_inputs_outputs_information.display;
}


// void set(const Vector< Vector<std::string> >&) method

/// This method sets all the inputs and outputs information from a single vector. 
/// @param new_variables_information Inputs and outputs information. 
/// The format is a vector of 6 subvectors:
/// <ul>
/// <li> Inputs name.
/// <li> Inputs units.
/// <li> Inputs description.
/// <li> Outputs name.
/// <li> Outputs units.
/// <li> Outputs description.
/// </ul>

void InputsOutputsInformation::set(const Vector< Vector<std::string> >& new_variables_information)
{
   // Control sentence (if debug)

   #ifdef _DEBUG 

   const unsigned int new_variables_information_size = new_variables_information.size();

   if(new_variables_information_size != 6)
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: InputsOutputsInformation class.\n" 
             << "void set(const Vector< Vector<std::string> >&) method.\n"
             << "Size of variables information must be six.\n";

	  throw std::logic_error(buffer.str());
   }

   #endif
   
   set_inputs_name(new_variables_information[0]);
   set_inputs_units(new_variables_information[1]);
   set_inputs_description(new_variables_information[2]);

   set_outputs_name(new_variables_information[3]);
   set_outputs_units(new_variables_information[4]);
   set_outputs_description(new_variables_information[5]);
}


// void set_inputs_number(const unsigned int&) method

/// This method sets a new number of inputs.
/// @param new_inputs_number Number of inputs. 

void InputsOutputsInformation::set_inputs_number(const unsigned int& new_inputs_number) 
{
   inputs_name.set(new_inputs_number);
   inputs_units.set(new_inputs_number);
   inputs_description.set(new_inputs_number);
}


// void set_outputs_number(const unsigned int&) method

/// This method sets a new number of outputs.
/// @param new_outputs_number Number of outputs. 

void InputsOutputsInformation::set_outputs_number(const unsigned int& new_outputs_number)
{
   outputs_name.set(new_outputs_number);
   outputs_units.set(new_outputs_number);
   outputs_description.set(new_outputs_number);
}


// void set_default(void) method

/// This method sets the members of this object to their default values.

void InputsOutputsInformation::set_default(void)
{
   const unsigned int inputs_number = count_inputs_number();
   const unsigned int outputs_number = count_outputs_number();

   inputs_name.set(inputs_number, "");

   inputs_units.set(inputs_number, "");

   inputs_description.set(inputs_number, "");

   outputs_name.set(outputs_number, "");

   outputs_units.set(outputs_number, "");

   outputs_description.set(outputs_number, "");

   set_display(true);
}


// void set_inputs_name(const Vector<std::string>&) method

/// This method sets the names for the input variables.
/// Such values are only used to give the user basic information on the problem at hand.
/// @param new_inputs_name New names for the input variables.

void InputsOutputsInformation::set_inputs_name(const Vector<std::string>& new_inputs_name)
{
   // Control sentence (if debug)

   #ifdef _DEBUG 

   const unsigned int size = new_inputs_name.size();

   const unsigned int inputs_number = count_inputs_number();

   if(size != inputs_number)
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: InputsOutputsInformation class.\n" 
             << "void set_inputs_name(const Vector<std::string>&) method.\n"
             << "Size of name of input variables vector must be equal to number of inputs.\n";

	  throw std::logic_error(buffer.str());
   }

   #endif

   // Set name of input variables 

   inputs_name = new_inputs_name;
}


// void set_input_name(const unsigned int&, const std::string&) method

/// This method sets the name of a single input variable.
/// Such value is only used to give the user basic information on the problem at hand.
/// @param input_index Index of input variable.
/// @param new_input_name New name for the input variable with index i.

void InputsOutputsInformation::set_input_name(const unsigned int& input_index, const std::string& new_input_name)
{
   const unsigned int inputs_number = count_inputs_number();

   // Control sentence (if debug)

   #ifdef _DEBUG 

   if(input_index >= inputs_number)
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: InputsOutputsInformation class.\n"
             << "void set_input_name(const unsigned int&, const std::string&) method.\n"
             << "Index of input variable must be less than number of inputs.\n";

	  throw std::logic_error(buffer.str());
   }

   #endif

   if(inputs_name.size() != inputs_number)
   {
      inputs_name.set(inputs_number); 
   }

   // Set name of single input variable

   inputs_name[input_index] = new_input_name;
}


// void set_inputs_units(const Vector<std::string>&) method

/// This method sets new units for all the input variables.
/// Such values are only used to give the user basic information on the problem at hand.
/// @param new_inputs_units New units for the input variables.

void InputsOutputsInformation::set_inputs_units(const Vector<std::string>& new_inputs_units)
{
   // Control sentence (if debug)

   #ifdef _DEBUG 

   const unsigned int inputs_number = count_inputs_number();

   const unsigned int size = new_inputs_units.size();

   if(size != inputs_number)
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: InputsOutputsInformation class.\n" 
             << "void set_inputs_units(const Vector<std::string>&) method.\n"
             << "Size must be equal to number of input variables.\n";

	  throw std::logic_error(buffer.str());
   }

   #endif

   // Set units of input variables

   inputs_units = new_inputs_units;
}


// void set_input_units(const unsigned int&, const std::string&) method

/// This method sets new units for a single input variable.
/// Such value is only used to give the user basic information on the problem at hand.
/// @param input_index Index of input variable.
/// @param new_input_units New units for that input variable.

void InputsOutputsInformation::set_input_units(const unsigned int& input_index, const std::string& new_input_units)
{
   const unsigned int inputs_number = count_inputs_number();

   // Control sentence (if debug)

   #ifdef _DEBUG 

   if(input_index >= inputs_number)
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: InputsOutputsInformation class.\n"
             << "void set_input_units(const unsigned int&, const std::string&) method.\n"
             << "Index of input variable must be less than number of inputs.\n";

	  throw std::logic_error(buffer.str());
   }

   #endif

   if(inputs_units.size() != inputs_number)
   {
      inputs_units.set(inputs_number); 
   }

   // Set units of single input variable

   inputs_units[input_index] = new_input_units;
}


// void set_inputs_description(const Vector<std::string>&) method

/// This method sets new descriptions for all the input variables.
/// Such values are only used to give the user basic information on the problem at hand.
/// @param new_inputs_description New description for the input variables.

void InputsOutputsInformation::set_inputs_description(const Vector<std::string>& new_inputs_description)
{
   // Control sentence (if debug)

   #ifdef _DEBUG 

   const unsigned int inputs_number = count_inputs_number();

   const unsigned int size = new_inputs_description.size();

   if(size != inputs_number)
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: InputsOutputsInformation class.\n"  
             << "void set_inputs_description(const Vector<std::string>&) method.\n"
             << "Size must be equal to number of input variables.\n";

	  throw std::logic_error(buffer.str());
   }

   #endif

   // Set description of input variables

   inputs_description = new_inputs_description;
}


// void set_input_description(const unsigned int&, const std::string&) method

/// This method sets a new description for a single input variable.
/// Such value is only used to give the user basic information on the problem at hand.
///
/// @param input_index Index of input variable.
/// @param new_input_description New description for the input variable with index i.

void InputsOutputsInformation::set_input_description(const unsigned int& input_index, const std::string& new_input_description)
{
   const unsigned int inputs_number = count_inputs_number();

   // Control sentence (if debug)

   #ifdef _DEBUG 

   if(input_index >= inputs_number)
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: InputsOutputsInformation class.\n"
             << "void set_input_description(const unsigned int&, const std::string&) method.\n"
             << "Index of input variable must be less than number of inputs.\n";

	  throw std::logic_error(buffer.str());
   }

   #endif

   if(inputs_description.size() != inputs_number)
   {
      inputs_description.set(inputs_number); 
   }

   // Set description of single input variable

   inputs_description[input_index] = new_input_description;
}


// void set_outputs_name(const Vector<std::string>&) method

/// This method sets the names of the output variables.
/// Such values are only used to give the user basic information on the problem at hand.
/// @param new_outputs_name New names for the output variables.

void InputsOutputsInformation::set_outputs_name(const Vector<std::string>& new_outputs_name)
{
   // Control sentence (if debug)

   #ifdef _DEBUG 

   const unsigned int size = new_outputs_name.size();

   const unsigned int outputs_number = count_outputs_number();

   if(size != outputs_number)
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: InputsOutputsInformation class.\n"
             << "void set_outputs_name(const Vector<std::string>&) method.\n" 
             << "Size of name of outputs vector must be equal to number of output variables.\n";

	  throw std::logic_error(buffer.str());
   }

   #endif

   // Set name of output variables

   outputs_name = new_outputs_name;
}


// void set_output_name(const unsigned int&, const std::string&) method

/// This method sets the name of a single output variable.
/// Such value is only used to give the user basic information on the problem at hand.
/// @param output_layer_index Index of output variable.
/// @param new_output_name New name for the output variable with index i.

void InputsOutputsInformation::set_output_name(const unsigned int& output_layer_index, const std::string& new_output_name)
{
   const unsigned int outputs_number = count_outputs_number();

   // Control sentence (if debug)

   #ifdef _DEBUG 

   if(output_layer_index >= outputs_number)
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: InputsOutputsInformation class.\n"
             << "void set_output_name(const unsigned int&, const std::string&) method.\n"
             << "Index of output variable must be less than number of outputs.\n";

	  throw std::logic_error(buffer.str());
   }

   #endif

   if(outputs_name.size() != outputs_number)
   {
      outputs_name.set(outputs_number); 
   }

   // Set name of single output variable

   outputs_name[output_layer_index] = new_output_name;
}


// void set_outputs_units(const Vector<std::string>&) method

/// This method sets new units for all the output variables.
/// Such values are only used to give the user basic information on the problem at hand.
/// @param new_outputs_units New units for the output variables.

void InputsOutputsInformation::set_outputs_units(const Vector<std::string>& new_outputs_units)
{
   // Control sentence (if debug)

   #ifdef _DEBUG 

   const unsigned int outputs_number = count_outputs_number();

   const unsigned int size = new_outputs_units.size();

   if(size != outputs_number)
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: InputsOutputsInformation class.\n"
             << "void set_outputs_units(const Vector<std::string>&) method.\n"
             << "Size must be equal to number of output variables.\n";

	  throw std::logic_error(buffer.str());
   }

   #endif

   // Set units of output variables

   outputs_units = new_outputs_units;
}


// void set_output_units(const unsigned int&, const std::string&) method

/// This method sets new units for a single output variable.
/// Such value is only used to give the user basic information on the problem at hand.
/// @param output_index Index of output variable.
/// @param new_output_units New units for that output variable.

void InputsOutputsInformation::set_output_units(const unsigned int& output_index, const std::string& new_output_units)
{
   const unsigned int outputs_number = count_outputs_number();

   // Control sentence (if debug)

   #ifdef _DEBUG 

   if(output_index >= outputs_number)
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: InputsOutputsInformation class.\n"
             << "void set_output_units(const unsigned int&, const std::string&) method.\n" 
             << "Index of output variable must be less than number of outputs.\n";

	  throw std::logic_error(buffer.str());
   }

   #endif

   if(outputs_units.size() != outputs_number)
   {
      outputs_units.set(outputs_number); 
   }

   // Set units of single output variable

   outputs_units[output_index] = new_output_units;
}


// void set_outputs_description(const Vector<std::string>&) method

/// This method sets new descriptions for all the output variables.
/// Such values are only used to give the user basic information on the problem at hand.
/// @param new_outputs_description New description for the output variables.

void InputsOutputsInformation::set_outputs_description(const Vector<std::string>& new_outputs_description)
{
   // Control sentence (if debug)

   #ifdef _DEBUG 

   const unsigned int size = new_outputs_description.size();

   const unsigned int outputs_number = count_outputs_number();

   if(size != outputs_number)
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: InputsOutputsInformation class.\n" 
             << "void set_outputs_description(const Vector<std::string>&) method.\n"
             << "Size must be equal to number of outputs.\n";

	  throw std::logic_error(buffer.str());
   }

   #endif

   // Set description of output variables

   outputs_description = new_outputs_description;
}


// void set_output_description(const unsigned int&, const std::string&) method

/// This method sets a new description for a single output variable.
/// Such value is only used to give the user basic information on the problem at hand.
/// @param output_index Index of output variable.
/// @param new_output_description New description for the output variable with index i.

void InputsOutputsInformation::set_output_description(const unsigned int& output_index, const std::string& new_output_description)
{
   const unsigned int outputs_number = count_outputs_number();

   // Control sentence (if debug)

   #ifdef _DEBUG 

   if(output_index >= outputs_number)
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: InputsOutputsInformation class.\n"
             << "void set_output_description(const unsigned int&, const std::string&) method.\n" 
             << "Index of output variable must be less than number of outputs.\n";

	  throw std::logic_error(buffer.str());
   }

   #endif

   if(outputs_description.size() != outputs_number)
   {
      outputs_description.set(outputs_number); 
   }

   // Set description of single output variable

   outputs_description[output_index] = new_output_description;
}


// void set_information(const Vector< Vector<std::string> >&) method

/// This method sets all the possible information about the inputs and output variables.
/// The format is a vector of vectors of size six: 
/// <ul>
/// <li> Name of input variables.
/// <li> Name of output variables.
/// <li> Units of input variables.
/// <li> Units of output variables.
/// <li> Description of input variables.
/// <li> Description of output variables.
/// </ul>
/// @param new_information Input and output variables information. 

void InputsOutputsInformation::set_information(const Vector< Vector<std::string> >& new_information)
{
   // Control sentence (if debug)

   #ifdef _DEBUG 

   const unsigned int size = new_information.size();

   if(size != 6)
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: InputsOutputsInformation class.\n"
             << "void set_information(const Vector< Vector<std::string> >&) method.\n" 
             << "Size of vector must be 6.\n";

	  throw std::logic_error(buffer.str());
   }

   #endif

   // Set all information

   set_inputs_name(new_information[0]);
   set_inputs_units(new_information[1]);
   set_inputs_description(new_information[2]);

   set_outputs_name(new_information[3]);
   set_outputs_units(new_information[4]);
   set_outputs_description(new_information[5]);
}


// void set_display(const bool&) method

/// This method sets a new display value. 
/// If it is set to true messages from this class are to be displayed on the screen;
/// if it is set to false messages from this class are not to be displayed on the screen.
/// @param new_display Display value.

void InputsOutputsInformation::set_display(const bool& new_display)
{
   display = new_display;
}


// void initialize_random(void) method

/// @todo

void InputsOutputsInformation::initialize_random(void)
{

}


// Vector<std::string> write_default_inputs_name(void) const method

/// This method returns the default names for the input variables:
/// <ul>
/// <li> input_1
/// <li> ...
/// <li> input_n
/// </ul>

Vector<std::string> InputsOutputsInformation::write_default_inputs_name(void) const
{
   const unsigned int inputs_number = count_inputs_number();

   Vector<std::string> default_inputs_name(inputs_number);

   std::ostringstream buffer;
  
   for(unsigned int i = 0; i < inputs_number; i++)
   {
      if(inputs_name[i] != "")
      {
         default_inputs_name[i] = inputs_name[i];
      }
      else
      {
         buffer.str("");

         buffer << "input_" << i+1;

         default_inputs_name[i] = buffer.str();
      }
   }

   return(default_inputs_name);
}


// Vector<std::string> write_default_outputs_name(void) const method

/// This method returns the default names for the output variables:
/// <ul>
/// <li> output_1
/// <li> ...
/// <li> output_n
/// </ul>

Vector<std::string> InputsOutputsInformation::write_default_outputs_name(void) const
{
   const unsigned int outputs_number = count_outputs_number();

   Vector<std::string> default_outputs_name(outputs_number);

   std::ostringstream buffer;
      
   for(unsigned int i = 0; i < outputs_number; i++)
   {
      if(outputs_name[i] != "")
      {
         default_outputs_name[i] = outputs_name[i];
      }
      else
      {
         buffer.str("");
          
         buffer << "output_" << i+1;

         default_outputs_name[i] = buffer.str();
      }
   }

   return(default_outputs_name);  
}


// std::string to_string(void) const method

/// This method returns a string representation of the current inputs-outputs information object. 

std::string InputsOutputsInformation::to_string(void) const
{
   std::ostringstream buffer;

   buffer << "Inputs outputs information\n"
          << "Inputs name:" << inputs_name << "\n"
          << "Inputs units:" << inputs_units << "\n"
          << "Inputs description:" << inputs_description << "\n"
          << "Outputs name:" << outputs_name << "\n"
          << "Outputs units:" << outputs_units << "\n"
          << "Outputs description:" << outputs_description << "\n"
          << "Display:" << display << "\n";

   return(buffer.str());
}


// TiXmlElement* to_XML(void) const method

/// This method serializes the inputs-outputs information object into a XML element of the TinyXML library. 
/// See the OpenNN manual for more information about the format of this element. 

TiXmlElement* InputsOutputsInformation::to_XML(void) const
{
   const unsigned int inputs_number = count_inputs_number();
   const unsigned int outputs_number = count_outputs_number();

   std::ostringstream buffer;

   TiXmlElement* variables_information_element = new TiXmlElement("InputsOutputsInformation");
   variables_information_element->SetAttribute("Version", 4);

   // Inputs name
   {
      TiXmlElement* inputs_name_element = new TiXmlElement("InputsName");
      variables_information_element->LinkEndChild(inputs_name_element);

      Vector<TiXmlElement*> elements(inputs_number);
      Vector<TiXmlText*> texts(inputs_number);

      for(unsigned int i = 0; i < inputs_number; i++)
      {
         elements[i] = new TiXmlElement("InputName");
         elements[i]->SetAttribute("Index", i+1);
         inputs_name_element->LinkEndChild(elements[i]);
      
         texts[i] = new TiXmlText(inputs_name[i].c_str());
         elements[i]->LinkEndChild(texts[i]);
      }
   }

   // Inputs units 
   {
      TiXmlElement* inputs_units_element = new TiXmlElement("InputsUnits");
      variables_information_element->LinkEndChild(inputs_units_element);

      Vector<TiXmlElement*> elements(inputs_number);
      Vector<TiXmlText*> texts(inputs_number);

      for(unsigned int i = 0; i < inputs_number; i++)
      {
         elements[i] = new TiXmlElement("InputUnits");
         elements[i]->SetAttribute("Index", i+1);
         inputs_units_element->LinkEndChild(elements[i]);
      
         texts[i] = new TiXmlText(inputs_units[i].c_str());
         elements[i]->LinkEndChild(texts[i]);
      }
   }

   // Inputs description
   {
      TiXmlElement* inputs_description_element = new TiXmlElement("InputsDescription");
      variables_information_element->LinkEndChild(inputs_description_element);

      Vector<TiXmlElement*> elements(inputs_number);
      Vector<TiXmlText*> texts(inputs_number);

      for(unsigned int i = 0; i < inputs_number; i++)
      {
         elements[i] = new TiXmlElement("InputDescription");
         elements[i]->SetAttribute("Index", i+1);
         inputs_description_element->LinkEndChild(elements[i]);
      
         texts[i] = new TiXmlText(inputs_description[i].c_str());
         elements[i]->LinkEndChild(texts[i]);
      }
   }

   // Outputs name
   {
      TiXmlElement* outputs_name_element = new TiXmlElement("OutputsName");
      variables_information_element->LinkEndChild(outputs_name_element);

      TiXmlElement* element;
      TiXmlText* text;

      for(unsigned int i = 0; i < outputs_number; i++)
      {
         element = new TiXmlElement("OutputName");
         element->SetAttribute("Index", i+1);

         outputs_name_element->LinkEndChild(element);
      
         text = new TiXmlText(outputs_name[i].c_str());
         element->LinkEndChild(text);
      }
   }

   // Outputs units 
   {
      TiXmlElement* outputs_units_element = new TiXmlElement("OutputsUnits");
      variables_information_element->LinkEndChild(outputs_units_element);

      TiXmlElement* element = NULL;
      TiXmlText* text = NULL;

      for(unsigned int i = 0; i < outputs_number; i++)
      {
         element = new TiXmlElement("OutputUnits");
         element->SetAttribute("Index", i+1);

         outputs_units_element->LinkEndChild(element);
      
         text = new TiXmlText(outputs_units[i].c_str());
         element->LinkEndChild(text);
      }
   }
   
   // Outputs description
   {
      TiXmlElement* outputs_description_element = new TiXmlElement("OutputsDescription");
      variables_information_element->LinkEndChild(outputs_description_element);

      Vector<TiXmlElement*> elements(outputs_number);
      Vector<TiXmlText*> texts(outputs_number);

      for(unsigned int i = 0; i < outputs_number; i++)
      {
         elements[i] = new TiXmlElement("OutputDescription");
         elements[i]->SetAttribute("Index", i+1);

         outputs_description_element->LinkEndChild(elements[i]);
      
         texts[i] = new TiXmlText(outputs_description[i].c_str());
         elements[i]->LinkEndChild(texts[i]);
      }
   }

   // Display
   {
      TiXmlElement* display_element = new TiXmlElement("Display");
      variables_information_element->LinkEndChild(display_element);

      buffer.str("");
      buffer << display;

      TiXmlText* display_text = new TiXmlText(buffer.str().c_str());
      display_element->LinkEndChild(display_text);
   }

   return(variables_information_element);
}


// void from_XML(TiXmlElement*) method

/// This method deserializes a TinyXML element into this inputs-outputs information object. 
/// @param inputs_outputs_information_element Pointer to a XML element containing the member data. 

void InputsOutputsInformation::from_XML(TiXmlElement* inputs_outputs_information_element)
{
   if(inputs_outputs_information_element)
   {
      int input_index = 0; // unsigned int does not work

      // Inputs number
      {
         TiXmlElement* inputs_number_element = inputs_outputs_information_element->FirstChildElement("InputsName");

         if(inputs_number_element)
         {    
            TiXmlElement* input_name_element = inputs_number_element->FirstChildElement("InputName");

            if(input_name_element)
            {
               input_name_element->QueryIntAttribute("Index", &input_index);

               for( ; input_name_element; input_name_element=input_name_element->NextSiblingElement())
               {
                  input_name_element->QueryIntAttribute("Index", &input_index);
               }
            }
         }
      }

      const unsigned int inputs_number = input_index;

      // Inputs name 
      {
         TiXmlElement* inputs_name_element = inputs_outputs_information_element->FirstChildElement("InputsName");

         if(inputs_name_element)
         {
            Vector<std::string> new_inputs_name(inputs_number);
      
            TiXmlElement* input_name_element = inputs_name_element->FirstChildElement("InputName");

            if(input_name_element)
            {
               input_name_element->QueryIntAttribute("Index", &input_index);

			   if(input_name_element->GetText())
			   {  
                  new_inputs_name[input_index-1] = input_name_element->GetText();
			   }

               for( ; input_name_element; input_name_element=input_name_element->NextSiblingElement())
               {
                  input_name_element->QueryIntAttribute("Index", &input_index);
				  
				  if(input_name_element->GetText())
				  {
                     new_inputs_name[input_index-1] = input_name_element->GetText();
				  }
               }
            }

            try
            {
               set_inputs_name(new_inputs_name);
            }
            catch(std::exception& e)
            {
               std::cout << e.what() << std::endl;		 
            }
         }
      }

      // Inputs units
      {
         TiXmlElement* inputs_units_element = inputs_outputs_information_element->FirstChildElement("InputsUnits");

         if(inputs_units_element)
         {
            Vector<std::string> new_inputs_units(inputs_number);
      
            TiXmlElement* input_units_element = inputs_units_element->FirstChildElement("InputUnits");

            if(input_units_element)
            {
               input_units_element->QueryIntAttribute("Index", &input_index);

			   if(input_units_element->GetText())
			   {
                  new_inputs_units[input_index-1] = input_units_element->GetText();
			   }

               for( ; input_units_element; input_units_element=input_units_element->NextSiblingElement())
               {
                  input_units_element->QueryIntAttribute("Index", &input_index);

				  if(input_units_element->GetText())
				  {
                     new_inputs_units[input_index-1] = input_units_element->GetText();
				  }
               }
            }

            try
            {
               set_inputs_units(new_inputs_units);
            }
            catch(std::exception& e)
            {
               std::cout << e.what() << std::endl;		 
            }
         }
      }

      // Inputs description
      {
         TiXmlElement* inputs_description_element = inputs_outputs_information_element->FirstChildElement("InputsDescription");

         if(inputs_description_element)
         {
            Vector<std::string> new_inputs_description(inputs_number);
      
            TiXmlElement* input_description_element = inputs_description_element->FirstChildElement("InputDescription");

            if(input_description_element)
            {
               input_description_element->QueryIntAttribute("Index", &input_index);

			   if(input_description_element->GetText())
			   {
                  new_inputs_description[input_index-1] = input_description_element->GetText();
			   } 

               for( ; input_description_element; input_description_element=input_description_element->NextSiblingElement())
               {
                  input_description_element->QueryIntAttribute("Index", &input_index);

				  if(input_description_element->GetText())
				  {
                     new_inputs_description[input_index-1] = input_description_element->GetText();
				  }
               }
            }

            try
            {
               set_inputs_description(new_inputs_description);
            }
            catch(std::exception& e)
            {
               std::cout << e.what() << std::endl;		 
            }
         }
      }

	  int output_index = 0;

      // Count outputs number
      {
         TiXmlElement* inputs_number_element = inputs_outputs_information_element->FirstChildElement("OutputsName");

         if(inputs_number_element)
         {    
            TiXmlElement* input_name_element = inputs_number_element->FirstChildElement("OutputName");

            if(input_name_element)
            {
               input_name_element->QueryIntAttribute("Index", &output_index);

               for( ; input_name_element; input_name_element=input_name_element->NextSiblingElement())
               {
                  input_name_element->QueryIntAttribute("Index", &output_index);
               }
            }
         }
      }

      const unsigned int outputs_number = output_index;

      // Outputs name 
      {
         TiXmlElement* outputs_name_element = inputs_outputs_information_element->FirstChildElement("OutputsName");

         if(outputs_name_element)
         {
            Vector<std::string> new_outputs_name(outputs_number);
      
            TiXmlElement* output_name_element = outputs_name_element->FirstChildElement("OutputName");

            if(output_name_element)
            {
               output_name_element->QueryIntAttribute("Index", &output_index);
               new_outputs_name[output_index-1] = output_name_element->GetText();

               for( ; output_name_element; output_name_element=output_name_element->NextSiblingElement())
               {
                  output_name_element->QueryIntAttribute("Index", &output_index);
                  new_outputs_name[output_index-1] = output_name_element->GetText();
               }
            }

            try
            {
               set_outputs_name(new_outputs_name);
            }
            catch(std::exception& e)
            {
               std::cout << e.what() << std::endl;		 
            }
         }
      }

      // Outputs units
      {
         TiXmlElement* outputs_units_element = inputs_outputs_information_element->FirstChildElement("OutputsUnits");

         if(outputs_units_element)
         {
            Vector<std::string> new_outputs_units(outputs_number);
      
            TiXmlElement* output_units_element = outputs_units_element->FirstChildElement("OutputUnits");

            if(output_units_element)
            {
               output_units_element->QueryIntAttribute("Index", &output_index);

			   if(output_units_element->GetText())
			   {
                  new_outputs_units[output_index-1] = output_units_element->GetText();
			   }

               for( ; output_units_element; output_units_element=output_units_element->NextSiblingElement())
               {
                  output_units_element->QueryIntAttribute("Index", &output_index);

				  if(output_units_element->GetText())
				  { 
                     new_outputs_units[output_index-1] = output_units_element->GetText();
				  }
               }
            }

            try
            {
               set_outputs_units(new_outputs_units);
            }
            catch(std::exception& e)
            {
               std::cout << e.what() << std::endl;		 
            }
         }
      }

      // Outputs description
      {
         TiXmlElement* outputs_description_element = inputs_outputs_information_element->FirstChildElement("OutputsDescription");

         if(outputs_description_element)
         {
            Vector<std::string> new_outputs_description(outputs_number);
      
            TiXmlElement* output_description_element = outputs_description_element->FirstChildElement("OutputDescription");

            if(output_description_element)
            {
               output_description_element->QueryIntAttribute("Index", &output_index);

			   if(output_description_element->GetText())
			   {
                  new_outputs_description[output_index-1] = output_description_element->GetText();
			   }

               for( ; output_description_element; output_description_element=output_description_element->NextSiblingElement())
               {
                  output_description_element->QueryIntAttribute("Index", &output_index);

                  if(output_description_element->GetText())
				  { 
                     new_outputs_description[output_index-1] = output_description_element->GetText();
				  }
               }
            }

            try
            {
               set_outputs_description(new_outputs_description);
            }
            catch(std::exception& e)
            {
               std::cout << e.what() << std::endl;		 
            }
         }
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
