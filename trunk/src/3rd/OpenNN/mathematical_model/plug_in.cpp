/****************************************************************************************************************/
/*                                                                                                              */
/*   OpenNN: Open Neural Networks Library                                                                       */
/*   www.opennn.cimne.com                                                                                       */
/*                                                                                                              */
/*   P L U G - I N   C L A S S                                                                                  */
/*                                                                                                              */
/*   Roberto Lopez                                                                                              */ 
/*   International Center for Numerical Methods in Engineering (CIMNE)                                          */
/*   Technical University of Catalonia (UPC)                                                                    */
/*   Barcelona, Spain                                                                                           */
/*   E-mail: rlopez@cimne.upc.edu                                                                               */ 
/*                                                                                                              */
/****************************************************************************************************************/

// System includes

#include<fstream>
#include<iostream>
#include<string>
#include<sstream>
#include <time.h>

// OpenNN includes

#include "../utilities/vector.h"
#include "../utilities/matrix.h"

#include "mathematical_model.h"
#include "plug_in.h"

#include "../neural_network/independent_parameters.h"

namespace OpenNN
{

// DEFAULT CONSTRUCTOR

/// Default constructor.
/// It constructs a default plug-in object, with zero independent and dependent variables. 

PlugIn::PlugIn(void) : MathematicalModel()
{                                            
   set_default();
}


// XML CONSTRUCTOR

/// XML constructor.
/// It creates a plug-in and loads its members from a Tiny XML element. 
/// @param plug_in_element Pointer to a XML element containing the plug-in members. 

PlugIn::PlugIn(TiXmlElement* plug_in_element)
: MathematicalModel(plug_in_element)
{                                            
}


// DESTRUCTOR
 
/// Destructor. 
/// It does not delete any object.  

PlugIn::~PlugIn(void)
{ 
}


// ASSIGNMENT OPERATOR

// PlugIn& operator = (const PlugIn&) method

/// Assignment operator. 
/// It assigns to this object the members of an existing plug-in object.
/// @param other_plug_in Plug-in object to be assigned.

PlugIn& PlugIn::operator = (const PlugIn& other_plug_in)
{
   if(this != &other_plug_in) 
   {
      input_method = other_plug_in.input_method;

      template_filename = other_plug_in.template_filename;
      input_filename = other_plug_in.input_filename;

      script_filename = other_plug_in.script_filename;

      output_filename = other_plug_in.output_filename;

      input_flags = other_plug_in.input_flags;

      output_rows_number = other_plug_in.output_rows_number;
      output_columns_number = other_plug_in.output_columns_number;
   }

   return(*this);
}


// EQUAL TO OPERATOR

// bool operator == (const PlugIn&) const method

/// Equal to operator. 
/// It compares this object with another object of the same class. 
/// It returns true if the members of the two objects have the same values, and false otherwise.
/// @ param other_plug_in Plug-in to be compared with.

bool PlugIn::operator == (const PlugIn& other_plug_in) const
{
   if(input_method == other_plug_in.input_method
   && template_filename == other_plug_in.template_filename
   && input_filename == other_plug_in.input_filename
   && script_filename == other_plug_in.script_filename
   && output_filename == other_plug_in.output_filename
   && input_flags == other_plug_in.input_flags
   && output_rows_number == other_plug_in.output_rows_number
   && output_columns_number == other_plug_in.output_columns_number)
   {
      return(true);
   }
   else
   {
      return(false);
   }
}


// METHODS

// const InputMethod& get_input_method(void) const method

/// This method returns the method for including the information into the input file. 

const PlugIn::InputMethod& PlugIn::get_input_method(void) const
{
   return(input_method);
}


// std::string write_input_method(void) const method

/// This method returns a string with the name of the method for including the information into the input file. 

std::string PlugIn::write_input_method(void) const
{
   switch(input_method)
   {
      case IndependentParametersInput:
      {
         return("IndependentParameters");
      }
      break;

      default:
      {
         std::ostringstream buffer;

         buffer << "OpenNN Exception: PlugIn class.\n"
                << "std::string get_input_method_name(void) const method.\n"
                << "Unknown inputs method.\n";
 
	     throw std::logic_error(buffer.str());
      }
      break;
   }   
}


// const std::string& get_template_filename(void) method

/// This method returns the name of the template file. 

const std::string& PlugIn::get_template_filename(void) const
{
   return(template_filename);
}


// const std::string& get_input_filename(void) method

/// This method returns the name of the input file. 

const std::string& PlugIn::get_input_filename(void) const
{
   return(input_filename);
}


// const std::string& get_script_filename(void) method

/// This method returns the name of the script file. 

const std::string& PlugIn::get_script_filename(void) const
{
   return(script_filename);
}


// const std::string& get_output_filename(void) method

/// This method returns the name of the output file. 

const std::string& PlugIn::get_output_filename(void) const
{
   return(output_filename);
}


// const Vector<std::string>& get_input_flags(void) const method

/// This method returns the vector of input file flags. 

const Vector<std::string>& PlugIn::get_input_flags(void) const
{
   return(input_flags);
}


// const std::string& get_input_flag(const unsigned int&) const method

/// This method returns a single input file flag. 
/// @param i Index of flag. 

const std::string& PlugIn::get_input_flag(const unsigned int& i) const
{
   return(input_flags[i]);
}


// void set_default(void) method

/// This method sets the following default values in this object:
/// <ul>
/// <li> Input method: Independent parameters input.
/// <li> Input filename: input.dat.
/// <li> Script filename: batch.bat.
/// <li> Output filename: output.dat.
/// <li> Display: true. 
/// </ul>

void PlugIn::set_default(void)
{
   input_method = IndependentParametersInput;

   template_filename = "template.dat";
   input_filename = "input.dat";

   script_filename = "batch.bat";

   output_filename = "output.dat";

   display = true;
}


// void set_input_method(const InputMethod&) method

/// This method sets the method for writting the input file. 
/// @param new_input_method Method for inputing the input file. 

void PlugIn::set_input_method(const InputMethod& new_input_method)
{
   input_method = new_input_method;
}


// void set_input_method(const std::string&) method

/// This method sets the method for writting the input file from a string. 
/// @param new_input_method Method for inputing the input file. 

void PlugIn::set_input_method(const std::string& new_input_method)
{
   if(new_input_method == "IndependentParameters")
   {
      set_input_method(IndependentParametersInput);
   }
   else
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: PlugIn class.\n"
             << "void set_input_method(const std::string&) method.\n"
			 << "Unknown plug-in method: " << new_input_method << ".\n";

	  throw std::logic_error(buffer.str());
   }
}


// void set_template_filename(const std::string&) method

/// This method sets the name of the template file. 
/// @param new_template_filename Name of template file. 

void PlugIn::set_template_filename(const std::string& new_template_filename)
{
   template_filename = new_template_filename;
}


// void set_input_filename(const std::string&) method

/// This method sets the name of the input file. 
/// @param new_input_filename Name of input file. 

void PlugIn::set_input_filename(const std::string& new_input_filename)
{
   input_filename = new_input_filename;
}


// void set_script_filename(const std::string&) method

/// This method sets the name of the script file. 
/// @param new_script_filename Name of script file. 

void PlugIn::set_script_filename(const std::string& new_script_filename)
{
   script_filename = new_script_filename;
}


// void set_output_filename(const std::string&) method

/// This method sets the name of the output file. 
/// @param new_output_filename Name of output file. 

void PlugIn::set_output_filename(const std::string& new_output_filename)
{
   output_filename = new_output_filename;
}


// void set_input_flags(const Vector<std::string>&) method

/// This method sets the flags in the input file. 
/// @param new_input_flags Flags strings. 

void PlugIn::set_input_flags(const Vector<std::string>& new_input_flags)
{
   input_flags = new_input_flags;
}


// void write_input_file(const NeuralNetwork&) const method

/// Thise method writes the input file with values obtained from the neural network. 
/// @param neural_network Neural network. 

void PlugIn::write_input_file(const NeuralNetwork& neural_network) const
{ 
   switch(input_method)
   {
      case IndependentParametersInput:
      {
         write_input_file_independent_parameters(neural_network);
      }
      break;

      default:
      {
         std::ostringstream buffer;

         buffer << "OpenNN Exception: PlugIn class.\n"
                << "void write_input_file(const NeuralNetwork&) const method.\n"
                << "Unknown input method.\n";
 
	     throw std::logic_error(buffer.str());
      }
      break;
   }   
}


// void write_input_file_independent_parameters(const NeuralNetwork&) const method

/// @todo

void PlugIn::write_input_file_independent_parameters(const NeuralNetwork& neural_network) const
{
   const IndependentParameters* independent_parameters_pointer = neural_network.get_independent_parameters_pointer();

   #ifdef _DEBUG 

   if(!independent_parameters_pointer)
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: PlugIn class.\n"
             << "void write_input_file_independent_parameters(void) const method.\n"
             << "Pointer to independent parameters is null.\n";

	  throw std::logic_error(buffer.str());
   }

   #endif

   const Vector<double> independent_parameters = independent_parameters_pointer->get_parameters();

   //unsigned int input_flags_number = input_flags.size();

   //unsigned int independent_parameters_number = independent_parameters.size();

   //// Control sentence

   //if(input_flags_number != independent_parameters_number)
   //{
   //   buffer << "OpenNN Exception: PlugIn class.\n"
   //          << "void write_input_file_independent_parameters(void) const method.\n"
   //          << "Number of inputs flags must be equal to number of independent parameters.\n";

   //   throw std::logic_error(buffer.str());         
   //}

   //// Template file 

   //std::ifstream template_file(template_filename.c_str());

   //if(!template_file.is_open())
   //{
   //   buffer << "OpenNN Exception: PlugIn class.\n"
   //          << "void write_input_file_independent_parameters(void) const method.\n"
   //          << "Cannot open template file.\n";            
   //         
   //   throw std::logic_error(buffer.str().c_str());
   //}

   //std::string file_string;
   //std::string line;

   //while(getline(template_file, line))
   //{
   //   file_string += line;
   //   file_string += "\n";
   //}

   //template_file.close();

   //// Convert values to string

   //Vector<std::string> independent_parameters_string = independent_parameters.get_string_vector();

   //// Replace flags by values as many times flags are found in string

   //for(unsigned int i = 0; i < input_flags_number; i++)
   //{
   //   while(file_string.find(input_flags[i]) != std::string::npos)
   //   {
   //      size_t found = file_string.find(input_flags[i]);

   //      if(found != std::string::npos)
   //      {
   //         file_string.replace(file_string.find(input_flags[i]), input_flags[i].length(), independent_parameters_string[i]);
   //      }
   //   }
   //}

   //// Input file

   //std::ofstream input_file(input_filename.c_str());
   //
   //if(!input_file.is_open())
   //{
   //   buffer << "OpenNN Exception: PlugIn class.\n"
   //          << "void write_input_file(void) const method.\n"
   //          << "Cannot open inputs file.\n";            
   //         
   //   throw std::logic_error(buffer.str().c_str());
   //}

   //input_file << file_string << "\n";

   //input_file.close();
}


// void run_script(void) const

/// This method runs the script needed for executing the mathematical model. 

void PlugIn::run_script(void) const
{
   if(!script_filename.empty())
   {
      system(script_filename.c_str());
   }
   else
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: PlugIn class.\n"
             << "void run_script(void) const.\n"
             << "Batch filename is empty.\n";

      throw std::logic_error(buffer.str());
   }
}


// Matrix<double> read_output_file(void) const method

/// This method reads the output file from the mathematical model.
/// Here the output file only contains a data matrix. 

Matrix<double> PlugIn::read_output_file(void) const
{
   Matrix<double> data(output_filename);

   return(data);
}


// Matrix<double> read_output_file_header(void) const method

/// This method reads the output file from the mathematical model.
/// Here the output file contains a header file and a data matrix. 

Matrix<double> PlugIn::read_output_file_header(void) const
{
   // Open outputs file for reading

   std::ifstream output_file(output_filename.c_str());
   
   if(!output_file.is_open())
   {
      std::ostringstream buffer;

      buffer << "OpenNN Exception: PlugIn class.\n"
             << "Matrix<double> read_output_file_header(void) const method.\n"
             << "Cannot open outputs file.\n";            
            
      throw std::logic_error(buffer.str().c_str());
   }

   std::string header;
   getline(output_file, header);

   Matrix<double> data;

   output_file >> data;

   output_file.close();

   return(data);
}


// Matrix<double> calculate_solutions(const NeuralNetwork&) const method

/// This method returns the solutions to the mathematical model. 
/// which are given by the independent and the dependent variables. 
/// The process is as follows:
/// <ul>
/// <li> Write input file.
/// <li> Run script. 
/// <li> Read output file. 
/// </ul>

Matrix<double> PlugIn::calculate_solutions(const NeuralNetwork& neural_network) const
{
   write_input_file(neural_network);

   run_script();

   return(read_output_file());
}


// std::string to_string(void) const method

/// This method returns a string representation of the current plug-in object. 

std::string PlugIn::to_string(void) const
{
   std::ostringstream buffer; 

   buffer << "Plug-in\n" 
          << "Independent variables number: " << independent_variables_number << "\n" 
          << "Dependent variables number: " << dependent_variables_number << "\n"
          << "Input method: " << input_method << "\n"
          << "Template filename: " << template_filename << "\n"
          << "Input filename: " <<  input_filename << "\n"
          << "Script filename: " << script_filename << "\n"
          << "Output filename: " << output_filename << "\n"
          << "Input flags: " << input_flags << "\n"
          << "Output rows number: " << output_rows_number << "\n"
          << "Output columns number: " <<  output_columns_number << "\n"
          << "Display: " << display << "\n";

   return(buffer.str());
}


// TiXmlElement* to_XML(void) const method

/// This method serializes the plug-in object into a XML element of the TinyXML library. 
/// See the OpenNN manual for more information about the format of this element. 

TiXmlElement* PlugIn::to_XML(void) const   
{
   std::ostringstream buffer;

   TiXmlElement* plug_in_element = new TiXmlElement("PlugIn");
   plug_in_element->SetAttribute("Version", 4);

   // Independent variables number 

   {
      TiXmlElement* independent_variables_number_element = new TiXmlElement("IndependentVariablesNumber");
      plug_in_element->LinkEndChild(independent_variables_number_element);

      buffer.str("");
      buffer << independent_variables_number;

      TiXmlText* independent_variables_number_text = new TiXmlText(buffer.str().c_str());
      independent_variables_number_element->LinkEndChild(independent_variables_number_text);
   }

   // Dependent variables number 

   {
      TiXmlElement* dependent_variables_number_element = new TiXmlElement("DependentVariablesNumber");
      plug_in_element->LinkEndChild(dependent_variables_number_element);

      buffer.str("");
      buffer << dependent_variables_number;

      TiXmlText* dependent_variables_number_text = new TiXmlText(buffer.str().c_str());
      dependent_variables_number_element->LinkEndChild(dependent_variables_number_text);
   }

   // Input method

   {
      TiXmlElement* input_method_element = new TiXmlElement("InputMethod");
      plug_in_element->LinkEndChild(input_method_element);

      std::string input_method_name = write_input_method();

      TiXmlText* input_method_text = new TiXmlText(input_method_name.c_str());
      input_method_element->LinkEndChild(input_method_text);
   }

   // Template filename

   {
      TiXmlElement* template_filename_element = new TiXmlElement("TemplateFilename");
      plug_in_element->LinkEndChild(template_filename_element);

      TiXmlText* template_filename_text = new TiXmlText(template_filename.c_str());
      template_filename_element->LinkEndChild(template_filename_text);
   }

   // Input filename 

   {
      TiXmlElement* input_filename_element = new TiXmlElement("InputFilename");
      plug_in_element->LinkEndChild(input_filename_element);

      TiXmlText* input_filename_text = new TiXmlText(input_filename.c_str());
      input_filename_element->LinkEndChild(input_filename_text);
   }

   // Batch filename 

   {
      TiXmlElement* script_filename_element = new TiXmlElement("BatchFilename");
      plug_in_element->LinkEndChild(script_filename_element);

      TiXmlText* script_filename_text = new TiXmlText(script_filename.c_str());
      script_filename_element->LinkEndChild(script_filename_text);
   }

   // Output filename 

   {
      TiXmlElement* output_filename_element = new TiXmlElement("OutputFilename");
      plug_in_element->LinkEndChild(output_filename_element);

      TiXmlText* output_filename_text = new TiXmlText(output_filename.c_str());
      output_filename_element->LinkEndChild(output_filename_text);
   }

   // Input flags

   {
      TiXmlElement* input_flags_element = new TiXmlElement("InputFlags");
      plug_in_element->LinkEndChild(input_flags_element);

      buffer.str("");
      buffer << input_flags;

      TiXmlText* input_flags_text = new TiXmlText(buffer.str().c_str());
      input_flags_element->LinkEndChild(input_flags_text);
   }

   // Output rows number

   {
      TiXmlElement* output_rows_number_element = new TiXmlElement("OutputRowsNumber");
      plug_in_element->LinkEndChild(output_rows_number_element);

      buffer.str("");
      buffer << output_rows_number;

      TiXmlText* output_rows_number_text = new TiXmlText(buffer.str().c_str());
      output_rows_number_element->LinkEndChild(output_rows_number_text);
   }

   // Output columns number

   {
      TiXmlElement* output_columns_number_element = new TiXmlElement("OutputColumnsNumber");
      plug_in_element->LinkEndChild(output_columns_number_element);

      buffer.str("");
      buffer << output_columns_number;

      TiXmlText* output_columns_number_text = new TiXmlText(buffer.str().c_str());
      output_columns_number_element->LinkEndChild(output_columns_number_text);
   }

   // Display

   {
      TiXmlElement* display_element = new TiXmlElement("Display");
      plug_in_element->LinkEndChild(display_element);

      buffer.str("");
      buffer << display;

      TiXmlText* display_text = new TiXmlText(buffer.str().c_str());
      display_element->LinkEndChild(display_text);
   }

   return(plug_in_element);   
}


// void from_XML(TiXmlElement*) method

/// This method deserializes a TinyXML element into this plug-in object. 
/// @param plug_in_element Pointer to a XML element containing the member data. 
/// @todo

void PlugIn::from_XML(TiXmlElement* plug_in_element)  
{
   if(plug_in_element)
   {
      // Independent variables number
      {
         TiXmlElement* element = plug_in_element->FirstChildElement("IndependentVariablesNumber");

         if(element)
         {
            const char* text = element->GetText();     

            if(text)
            {
               try
               {
                  set_independent_variables_number(atoi(text));
               }
               catch(std::exception& e)
               {
                  std::cout << e.what() << std::endl;		 
               }
            }
         }
      }

      // Dependent variables number
      {
         TiXmlElement* element = plug_in_element->FirstChildElement("DependentVariablesNumber");

         if(element)
         {
            const char* text = element->GetText();     

            if(text)
            {
               try
               {
                  set_dependent_variables_number(atoi(text));
               }
               catch(std::exception& e)
               {
                  std::cout << e.what() << std::endl;		 
               }
            }
         }
      }

      // Input method
      {
         TiXmlElement* input_method_element = plug_in_element->FirstChildElement("InputMethod");

         if(input_method_element)
         {
            const char* input_method_text = input_method_element->GetText();

            if(input_method_text)
            {
               try
               {
                  set_input_method(input_method_text);
               }
               catch(std::exception& e)
               {
                  std::cout << e.what() << std::endl;		 
               }
            }
         }
      }

      // Template filename
      {
         TiXmlElement* template_filename_element = plug_in_element->FirstChildElement("TemplateFilename");

         if(template_filename_element)
         {
            const char* template_filename_text = template_filename_element->GetText();

            if(template_filename_text)
            {
               try
               {
                  set_template_filename(template_filename_text);
               }
               catch(std::exception& e)
               {
                  std::cout << e.what() << std::endl;		 
               }
            }
         }
      }

      // Input filename
      {
         TiXmlElement* input_filename_element = plug_in_element->FirstChildElement("InputFilename");

         if(input_filename_element)
         {
            const char* input_filename_text = input_filename_element->GetText();

            if(input_filename_text)
            {
               try
               {
                  set_input_filename(input_filename_text);
               }
               catch(std::exception& e)
               {
                  std::cout << e.what() << std::endl;		 
               }
            }
         }
      }

      // Batch filename 
      {
         TiXmlElement* script_filename_element = plug_in_element->FirstChildElement("BatchFilename");

         if(script_filename_element)
         {
            const char* script_filename_text = script_filename_element->GetText();

            if(script_filename_text)
            {
               try
               {
                  set_script_filename(script_filename_text);
               }
               catch(std::exception& e)
               {
                  std::cout << e.what() << std::endl;		 
               }
            }
         }
      }

      // Output filename 
      {
         TiXmlElement* output_filename_element = plug_in_element->FirstChildElement("OutputFilename");

         if(output_filename_element)
         {
            const char* output_filename_text = output_filename_element->GetText();

            if(output_filename_text)
            {
               try
               {
                  set_output_filename(output_filename_text);
               }
               catch(std::exception& e)
               {
                  std::cout << e.what() << std::endl;		 
               }
            }
         }
      }
/*
      // Input flags 
      {
         TiXmlElement* input_flags_element = plug_in_element->FirstChildElement("InputFlags");

         if(input_flags_element)
         {
            const char* input_flags_text = input_flags_element->GetText();

            if(input_flags_text)
            {
               Vector<std::string> new_input_flags;
               new_input_flags.parse(input_flags_text); 

               try
               {
                  set_input_flags(new_input_flags);
               }
               catch(std::exception& e)
               {
                  std::cout << e.what() << std::endl;		 
               }
            }
         }
      }
*/
      // Display
      {
         TiXmlElement* display_element = plug_in_element->FirstChildElement("Display");

         if(display_element)
         {
            const char* display_text = display_element->GetText();     

            if(display_text)
            {
               try
               {
                  std::string display_string(display_text);

                  set_display(display_string != "0");
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
