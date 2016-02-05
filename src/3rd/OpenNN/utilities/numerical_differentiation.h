/****************************************************************************************************************/
/*                                                                                                              */
/*   OpenNN: Open Neural Networks Library                                                                       */
/*   www.opennn.cimne.com                                                                                       */
/*                                                                                                              */
/*   N U M E R I C A L   D I F F E R E N T I A T I O N   C L A S S   H E A D E R                                */
/*                                                                                                              */ 
/*   Roberto Lopez                                                                                              */ 
/*   International Center for Numerical Methods in Engineering (CIMNE)                                          */
/*   Technical University of Catalonia (UPC)                                                                    */
/*   Barcelona, Spain                                                                                           */
/*   E-mail: rlopez@cimne.upc.edu                                                                               */ 
/*                                                                                                              */
/****************************************************************************************************************/

#ifndef __NUMERICALDIFFERENTIATION_H__
#define __NUMERICALDIFFERENTIATION_H__

// OpenNN includes

#include "vector.h"
#include "matrix.h"

// TinyXml includes

#include "../../tinyxml/tinyxml.h"

   
namespace OpenNN
{

/// This class contains methods for numerical differentiation of functions. 
/// In particular it implements the forward and central differences methods for derivatives, Jacobians, Hessians or Hessian forms.

class NumericalDifferentiation 
{

public:

   // DEFAULT CONSTRUCTOR

   explicit NumericalDifferentiation(void);

   // COPY CONSTRUCTOR

   NumericalDifferentiation(const NumericalDifferentiation&);

   // DESTRUCTOR

   virtual ~NumericalDifferentiation(void);

   // ASSIGNMENT OPERATOR

   NumericalDifferentiation& operator = (const NumericalDifferentiation&);

   // EQUAL TO OPERATOR

   bool operator == (const NumericalDifferentiation&) const;

   /// Enumeration of available methods for numerical differentiation.

   enum NumericalDifferentiationMethod{ForwardDifferences, CentralDifferences};

   // METHODS

   const NumericalDifferentiationMethod& get_numerical_differentiation_method(void) const;
   std::string write_numerical_differentiation_method(void) const;
   
   const unsigned int& get_precision_digits(void) const;

   const bool& get_display(void) const;

   void set(const NumericalDifferentiation&);

   void set_numerical_differentiation_method(const NumericalDifferentiationMethod&);
   void set_numerical_differentiation_method(const std::string&);

   void set_precision_digits(const unsigned int&);

   void set_display(const bool&);

   void set_default(void);

   double calculate_h(const double&) const;

   Vector<double> calculate_h(const Vector<double>&) const;

   // Serialization methods

   TiXmlElement* to_XML(void) const;   
   void from_XML(TiXmlElement*);   
   

   // DERIVATIVE METHODS


   // double calculate_forward_differences_derivative(const T&, double (T::*f)(const double&) const , double) const method

   /// This method returns the derivative of a function using the forward differences method. 
   /// @param t  Object constructor containing the member method to differentiate.  
   /// @param f Pointer to the member method.
   /// @param x Differentiation point. 

   template<class T> 
   double calculate_forward_differences_derivative(const T& t, double (T::*f)(const double&) const, const double& x) const
   {
      const double y = (t.*f)(x);

      const double h = calculate_h(x);

	  const double y_forward = (t.*f)(x + h);
     
      const double d = (y_forward - y)/h;
     
      return(d);
   }


   // double calculate_central_differences_derivative(const T&, double (T::*f)(const double&) const , double) const method

   /// This method returns the derivative of a function using the central differences method. 
   /// @param t  Object constructor containing the member method to differentiate.  
   /// @param f Pointer to the member method.
   /// @param x Differentiation point. 

   template<class T>  
   double calculate_central_differences_derivative(const T& t, double (T::*f)(const double&) const , const double& x) const
   {
      const double h = calculate_h(x);

	  const double y_forward = (t.*f)(x+h);

	  const double y_backward = (t.*f)(x-h);
     
      const double d = (y_forward - y_backward)/(2.0*h);

      return(d);
   }


   // double calculate_derivative(const T&, double (T::*f)(const double&) const , double) const method

   /// This method returns the derivative of a function acording to the numerical differentiation method to be used. 
   /// @param t  Object constructor containing the member method to differentiate.  
   /// @param f Pointer to the member method.
   /// @param x Differentiation point. 

   template<class T> 
   double calculate_derivative(const T& t, double (T::*f)(const double&) const , const double& x) const
   {
      switch(numerical_differentiation_method)
      {
         case ForwardDifferences:
         {
            return(calculate_forward_differences_derivative(t, f, x));
      	 }
	     break;

         case CentralDifferences:
         {
            return(calculate_central_differences_derivative(t, f, x));
    	 }
   	     break;

         default:
         {
            std::ostringstream buffer;

            buffer << "OpenNN Exception: NumericalDifferentiation class.\n"
                   << "double calculate_derivative(const T&, double (T::*f)(const double&) const , const double&) const method.\n"
                   << "Unknown numerical differentiation method.\n";
 
            throw std::logic_error(buffer.str());
	     }
	     break;
      }
   }


   // Vector<double> calculate_forward_differences_derivative
   //(const T&, Vector<double> (T::*f)(const Vector<double>&) const, const Vector<double>&) const method

   /// This method returns the derivatives of a vector function using the forward differences method. 
   /// @param t  Object constructor containing the member method to differentiate.  
   /// @param f Pointer to the member method.
   /// @param x Input vector. 

   template<class T> 
   Vector<double> calculate_forward_differences_derivative(const T& t, Vector<double> (T::*f)(const Vector<double>&) const, const Vector<double>& x) const
   {
      const Vector<double> h = calculate_h(x);

	  const Vector<double> y = (t.*f)(x);

      const Vector<double> x_forward = x + h;     
	  const Vector<double> y_forward = (t.*f)(x_forward);

	  const Vector<double> d = (y_forward - y)/h;

      return(d);
   }


   // Vector<double> calculate_central_differences_derivative
   //(const T&, Vector<double> (T::*f)(const Vector<double>&) const, const Vector<double>&) const method

   /// This method returns the derivatives of a vector function using the central differences method. 
   /// @param t : Object constructor containing the member method to differentiate.  
   /// @param f: Pointer to the member method.
   /// @param x: Input vector. 

   template<class T> 
   Vector<double> calculate_central_differences_derivative(const T& t, Vector<double> (T::*f)(const Vector<double>&) const, const Vector<double>& x) const
   {
      const Vector<double> h = calculate_h(x);
     
      const Vector<double> x_forward = x + h;
      const Vector<double> x_backward = x - h;

	  const Vector<double> y_forward = (t.*f)(x_forward);
	  const Vector<double> y_backward = (t.*f)(x_backward);

	  const Vector<double> y = (t.*f)(x);

      const Vector<double> d = (y_forward - y_backward)/(h*2.0);

      return(d);
   }


   // Vector<double> calculate_derivative
   //(const T&, Vector<double> (T::*f)(const Vector<double>&) const, const Vector<double>&) const method

   /// This method returns the derivatives of a vector function acording to the numerical differentiation method to be used. 
   /// @param t : Object constructor containing the member method to differentiate.  
   /// @param f: Pointer to the member method.
   /// @param x: Input vector. 

   template<class T> 
   Vector<double> calculate_derivative(const T& t, Vector<double> (T::*f)(const Vector<double>&) const, const Vector<double>& x) const
   {
      switch(numerical_differentiation_method)
      {
         case ForwardDifferences:
         {
            return(calculate_forward_differences_derivative(t, f, x));
         }
	     break;

         case CentralDifferences:
         {
            return(calculate_central_differences_derivative(t, f, x));
    	 }
   	     break;

         default:
         {
            std::ostringstream buffer;

            buffer << "OpenNN Exception: NumericalDifferentiation class.\n"
                   << "Vector<double> calculate_derivative(const T&, Vector<double> (T::*f)(const Vector<double>&) const, const Vector<double>&) const method.\n"
                   << "Unknown numerical differentiation method.\n";
 
            throw std::logic_error(buffer.str());
	     }
	     break;
      }
   }


   // Vector<double> calculate_forward_differences_derivative
   //(const T&, Vector<double> (T::*f)(const unsigned int&, const Vector<double>&) const, const unsigned int&, const Vector<double>&) const method

   /// This method returns the derivatives of a vector function using the forward differences method. 
   /// The function to be differentiated is of the following form: Vector<double> f(const unsigned int&, const Vector<double>&) const. 
   /// @param t : Object constructor containing the member method to differentiate.  
   /// @param f: Pointer to the member method.
   /// @param dummy: Dummy integer for the method prototype. 
   /// @param x: Input vector. 

   template<class T> 
   Vector<double> calculate_forward_differences_derivative(const T& t, Vector<double> (T::*f)(const unsigned int&, const Vector<double>&) const, const unsigned int& dummy, const Vector<double>& x) const
   {
      const Vector<double> y = (t.*f)(dummy, x);

      const Vector<double> h = calculate_h(x);     
      const Vector<double> x_forward = x + h;     

	  const Vector<double> y_forward = (t.*f)(dummy, x_forward);

	  const Vector<double> d = (y_forward - y)/h;

      return(d);
   }


   // Vector<double> calculate_central_differences_derivative
   //(const T&, Vector<double> (T::*f)(const unsigned int&, const Vector<double>&) const, const unsigned int&, const Vector<double>&) const method

   /// This method returns the derivatives of a vector function using the central differences method. 
   /// The function to be differentiated is of the following form: Vector<double> f(const unsigned int&, const Vector<double>&) const. 
   /// @param t : Object constructor containing the member method to differentiate.  
   /// @param f: Pointer to the member method.
   /// @param dummy: Dummy integer for the method prototype. 
   /// @param x: Input vector. 

   template<class T> 
   Vector<double> calculate_central_differences_derivative(const T& t, Vector<double> (T::*f)(const unsigned int&, const Vector<double>&) const, const unsigned int& dummy, const Vector<double>& x) const
   {
      const Vector<double> h = calculate_h(x);     

      const Vector<double> x_forward = x + h;
      const Vector<double> x_backward = x - h;

	  const Vector<double> y_forward = (t.*f)(dummy, x_forward);
	  const Vector<double> y_backward = (t.*f)(dummy, x_backward);

      const Vector<double> d = (y_forward - y_backward)/(h*2.0);

      return(d);
   }


   // Vector<double> calculate_derivative
   //(const T&, Vector<double> (T::*f)(const unsigned int&, const Vector<double>&) const, const unsigned int&, const Vector<double>&) const method

   /// This method returns the derivatives of a vector function according to the numerical differentiation method to be used. 
   /// The function to be differentiated is of the following form: Vector<double> f(const unsigned int&, const Vector<double>&) const. 
   /// @param t : Object constructor containing the member method to differentiate.  
   /// @param f: Pointer to the member method.
   /// @param dummy: Dummy integer for the method prototype. 
   /// @param x: Input vector. 

   template<class T> 
   Vector<double> calculate_derivative(const T& t, Vector<double> (T::*f)(const unsigned int&, const Vector<double>&) const, const unsigned int& dummy, const Vector<double>& x) const
   {
      switch(numerical_differentiation_method)
      {
         case ForwardDifferences:
         {
            return(calculate_forward_differences_derivative(t, f, dummy, x));
         }
	     break;

         case CentralDifferences:
         {
           return(calculate_central_differences_derivative(t, f, dummy, x));
    	 }
   	     break;

         default:
         {
            std::ostringstream buffer;

            buffer << "OpenNN Exception: NumericalDifferentiation class.\n"
                   << "Vector<double> calculate_derivative(const T&, Vector<double> (T::*f)(const unsigned int&, const Vector<double>&) const, const unsigned int&, const Vector<double>&) const method.\n"
                   << "Unknown numerical differentiation method.\n";
 
            throw std::logic_error(buffer.str());
	     }
	     break;
      }
   }


   // SECOND DERIVATIVE METHODS


   // double calculate_forward_differences_second_derivative(const T&, double (T::*f)(const double&) const , const double&) const method

   /// This method returns the second derivative of a function using the forward differences method. 
   /// @param t : Object constructor containing the member method to differentiate.  
   /// @param f: Pointer to the member method.
   /// @param x: Differentiation point. 

   template<class T> 
   double calculate_forward_differences_second_derivative(const T& t, double (T::*f)(const double&) const, const double& x) const
   {   
      const double h = calculate_h(x);

      const double x_forward_2 = x + 2.0*h;

      const double y_forward_2 = (t.*f)(x_forward_2);

      const double x_forward = x + h;

      const double y_forward = (t.*f)(x_forward);

      const double y = (t.*f)(x);
       
      return((y_forward_2 - 2*y_forward + y)/pow(h, 2));
   }


   // double calculate_central_differences_second_derivative(const T&, double (T::*f)(const double&) const , const double&) const method

   /// This method returns the second derivative of a function using the central differences method. 
   /// @param t : Object constructor containing the member method to differentiate.  
   /// @param f: Pointer to the member method.
   /// @param x: Differentiation point. 

   template<class T> 
   double calculate_central_differences_second_derivative(const T& t, double (T::*f)(const double&) const , const double& x) const
   {
      const double h = calculate_h(x);

      const double x_forward_2 = x + 2.0*h;

      const double y_forward_2 = (t.*f)(x_forward_2);

      const double x_forward = x + h;

      const double y_forward = (t.*f)(x_forward);

      const double y = (t.*f)(x);

      const double x_backward = x - h;  

      const double y_backward = (t.*f)(x_backward);

      const double x_backward_2 = x - 2.0*h;

      const double y_backward_2 = (t.*f)(x_backward_2);
    
      const double d2 = (-y_forward_2 + 16.0*y_forward -30.0*y + 16.0*y_backward - y_backward_2)/(12.0*pow(h, 2));  

      return(d2);
   }


   // double calculate_second_derivative(const T&, double (T::*f)(const double&) const , const double&) const method

   /// This method returns the second derivative of a function acording to the numerical differentiation method to be used. 
   /// @param t : Object constructor containing the member method to differentiate.  
   /// @param f: Pointer to the member method.
   /// @param x: Differentiation point. 

   template<class T> 
   double calculate_second_derivative(const T& t, double (T::*f)(const double&) const , const double& x) const
   {
      switch(numerical_differentiation_method)
      {
         case ForwardDifferences:
         {
            return(calculate_forward_differences_second_derivative(t, f, x));
      	 }
	     break;

         case CentralDifferences:
         {
            return(calculate_central_differences_second_derivative(t, f, x));
    	 }
   	     break;

         default:
         {
            std::ostringstream buffer;

            buffer << "OpenNN Exception: NumericalDifferentiation class.\n"
                   << "double calculate_second_derivative(const T& t, double (T::*f)(const double&) const, const double&) const method.\n"
                   << "Unknown numerical differentiation method.\n";
 
            throw std::logic_error(buffer.str());
	     }
	     break;
      }
   }


   // Vector<double> calculate_forward_differences_second_derivative(const T&, Vector<double> (T::*f)(const Vector<double>&) const, const Vector<double>&) const method

   /// This method returns the second derivative of a vector function using the forward differences method. 
   /// @param t : Object constructor containing the member method to differentiate.  
   /// @param f: Pointer to the member method.
   /// @param x: Input vector. 

   template<class T> 
   Vector<double> calculate_forward_differences_second_derivative(const T& t, Vector<double> (T::*f)(const Vector<double>&) const, const Vector<double>& x) const
   {
      const Vector<double> y = (t.*f)(x);

      const Vector<double> h = calculate_h(x);

      const Vector<double> x_forward = x + h;
      const Vector<double> x_forward_2 = x + h*2.0;

      const Vector<double> y_forward = (t.*f)(x_forward);
      const Vector<double> y_forward_2 = (t.*f)(x_forward_2);

      return((y_forward_2 - y_forward*2.0 + y)/(h*h));
   }


   // Vector<double> calculate_central_differences_second_derivative(const T&, Vector<double> (T::*f)(const Vector<double>&) const, const Vector<double>&) const method

   /// This method returns the second derivative of a vector function using the central differences method. 
   /// @param t : Object constructor containing the member method to differentiate.  
   /// @param f: Pointer to the member method.
   /// @param x: Input vector. 

   template<class T> 
   Vector<double> calculate_central_differences_second_derivative(const T& t, Vector<double> (T::*f)(const Vector<double>&) const, const Vector<double>& x) const
   {      
      const Vector<double> h = calculate_h(x);

      const Vector<double> x_forward = x + h;
      const Vector<double> x_forward_2 = x + h*2.0;

      const Vector<double> x_backward = x - h;
      const Vector<double> x_backward_2 = x - h*2.0;

      const Vector<double> y = (t.*f)(x);

      const Vector<double> y_forward = (t.*f)(x_forward);
      const Vector<double> y_forward_2 = (t.*f)(x_forward_2);

      const Vector<double> y_backward = (t.*f)(x_backward);
      const Vector<double> y_backward_2 = (t.*f)(x_backward_2);

      return((y_forward_2*-1.0 + y_forward*16.0 + y*-30.0 + y_backward*16.0 + y_backward_2*-1.0)/(h*h*12.0));
   }


   // Vector<double> calculate_second_derivative(const T&, Vector<double> (T::*f)(const Vector<double>&) const, const Vector<double>&) const method

   /// This method returns the second derivative of a vector function acording to the numerical differentiation method to be used. 
   /// @param t : Object constructor containing the member method to differentiate.  
   /// @param f: Pointer to the member method.
   /// @param x: Input vector. 

   template<class T> 
   Vector<double> calculate_second_derivative(const T& t, Vector<double> (T::*f)(const Vector<double>&) const, const Vector<double>& x) const
   {
      switch(numerical_differentiation_method)
      {
         case ForwardDifferences:
         {
            return(calculate_forward_differences_second_derivative(t, f, x));
      	 }
	     break;

         case CentralDifferences:
         {
            return(calculate_central_differences_second_derivative(t, f, x));
    	 }
   	     break;

         default:
         {
            std::ostringstream buffer;

            buffer << "OpenNN Exception: NumericalDifferentiation class.\n"
                   << "Vector<double> calculate_second_derivative(const T& t, Vector<double> (T::*f)(const Vector<double>&) const, const Vector<double>&) const method.\n"
                   << "Unknown numerical differentiation method.\n";
 
            throw std::logic_error(buffer.str());
	     }
	     break;
      }
   }


   // Vector<double> calculate_forward_differences_second_derivative(const T&, Vector<double> (T::*f)(const unsigned int&, const Vector<double>&) const, const unsigned int&, const Vector<double>&) const method

   /// This method returns the second derivatives of a vector function using the forward differences method. 
   /// The function to be differentiated is of the following form: Vector<double> f(const unsigned int&, const Vector<double>&) const. 
   /// @param t : Object constructor containing the member method to differentiate.  
   /// @param f: Pointer to the member method.
   /// @param dummy: Dummy integer for the method prototype. 
   /// @param x: Input vector. 

   template<class T> 
   Vector<double> calculate_forward_differences_second_derivative(const T& t, Vector<double> (T::*f)(const unsigned int&, const Vector<double>&) const, const unsigned int& dummy, const Vector<double>& x) const
   {
      const Vector<double> y = (t.*f)(dummy, x);

      const Vector<double> h = calculate_h(x);

      const Vector<double> x_forward = x + h;
      const Vector<double> x_forward_2 = x + h*2.0;

      const Vector<double> y_forward = (t.*f)(dummy, x_forward);
      const Vector<double> y_forward_2 = (t.*f)(dummy, x_forward_2);

      return((y_forward_2 - y_forward*2.0 + y)/(h*h));
   }


   // Vector<double> calculate_central_differences_second_derivative(const T&, Vector<double> (T::*f)(const unsigned int&, const Vector<double>&) const, const unsigned int&, const Vector<double>&) const method

   /// This method returns the second derivatives of a vector function using the central differences method. 
   /// The function to be differentiated is of the following form: Vector<double> f(const unsigned int&, const Vector<double>&) const. 
   /// @param t : Object constructor containing the member method to differentiate.  
   /// @param f: Pointer to the member method.
   /// @param dummy: Dummy integer for the method prototype. 
   /// @param x: Input vector. 

   template<class T> 
   Vector<double> calculate_central_differences_second_derivative(const T& t, Vector<double> (T::*f)(const unsigned int&, const Vector<double>&) const, const unsigned int& dummy, const Vector<double>& x) const
   {      
      const Vector<double> h = calculate_h(x);

      const Vector<double> x_forward = x + h;
      const Vector<double> x_forward_2 = x + h*2.0;

      const Vector<double> x_backward = x - h;
      const Vector<double> x_backward_2 = x - h*2.0;

      const Vector<double> y = (t.*f)(dummy, x);

      const Vector<double> y_forward = (t.*f)(dummy, x_forward);
      const Vector<double> y_forward_2 = (t.*f)(dummy, x_forward_2);

      const Vector<double> y_backward = (t.*f)(dummy, x_backward);
      const Vector<double> y_backward_2 = (t.*f)(dummy, x_backward_2);

      return((y_forward_2*-1.0 + y_forward*16.0 + y*-30.0 + y_backward*16.0 + y_backward_2*-1.0)/(h*h*12.0));
   }


   // Vector<double> calculate_second_derivative(const T&, Vector<double> (T::*f)(const unsigned int&, const Vector<double>&) const, const unsigned int&, const Vector<double>&) const method

   /// This method returns the second derivatives of a vector function acording to the numerical differentiation method to be used. 
   /// The function to be differentiated is of the following form: Vector<double> f(const unsigned int&, const Vector<double>&) const. 
   /// @param t : Object constructor containing the member method to differentiate.  
   /// @param f: Pointer to the member method.
   /// @param dummy: Dummy integer for the method prototype. 
   /// @param x: Input vector. 

   template<class T> 
   Vector<double> calculate_second_derivative(const T& t, Vector<double> (T::*f)(const unsigned int&, const Vector<double>&) const, const unsigned int& dummy, const Vector<double>& x) const
   {
      switch(numerical_differentiation_method)
      {
         case ForwardDifferences:
         {
            return(calculate_forward_differences_second_derivative(t, f, dummy, x));
      	 }
	     break;

         case CentralDifferences:
         {
            return(calculate_central_differences_second_derivative(t, f, dummy, x));
    	 }
   	     break;

         default:
         {
            std::ostringstream buffer;

            buffer << "OpenNN Exception: NumericalDifferentiation class.\n"
                   << "Vector<double> calculate_second_derivative(const T& t, Vector<double> (T::*f)(const unsigned int&, const Vector<double>&) const, const unsigned int&, const Vector<double>&) const method.\n"
                   << "Unknown numerical differentiation method.\n";
 
            throw std::logic_error(buffer.str());
	     }
	     break;
      }
   }


   // GRADIENT METHODS

   // Vector<double> calculate_forward_differences_gradient(const T&, double (T::*f)(const Vector<double>&) const, const Vector<double>&) const method

   /// This method returns the gradient of a function of several dimensions using the forward differences method. 
   /// The function to be differentiated is of the following form: double f(const Vector<double>&) const. 
   /// @param t : Object constructor containing the member method to differentiate.  
   /// @param f: Pointer to the member method.
   /// @param x: Input vector. 

   template<class T> 
   Vector<double> calculate_forward_differences_gradient(const T& t, double (T::*f)(const Vector<double>&) const, const Vector<double>& x) const
   {
      const unsigned int n = x.size();

      double h;

      double y = (t.*f)(x);
      
	  Vector<double> x_forward(x);
  
      double y_forward;

	  Vector<double> g(n);

      for(unsigned int i = 0; i < n; i++)
      {
         h = calculate_h(x[i]);
 
         x_forward[i] += h;
         y_forward = (t.*f)(x_forward);
         x_forward[i] -= h;

         g[i] = (y_forward - y)/h; 
      }

      return(g);
   }


   // Vector<double> calculate_central_differences_gradient(const T& t, double (T::*f)(const Vector<double>&) const, const Vector<double>& x) const method

   /// This method returns the gradient of a function of several dimensions using the central differences method. 
   /// The function to be differentiated is of the following form: double f(const Vector<double>&) const. 
   /// @param t : Object constructor containing the member method to differentiate.  
   /// @param f: Pointer to the member method.
   /// @param x: Input vector. 

   template<class T> 
   Vector<double> calculate_central_differences_gradient(const T& t, double (T::*f)(const Vector<double>&) const, const Vector<double>& x) const
   {      
      const unsigned int n = x.size();

      double h;

	  Vector<double> x_forward(x);
	  Vector<double> x_backward(x);
  
	  double y_forward;
      double y_backward;

      Vector<double> g(n);

      for(unsigned int i = 0; i < n; i++)
      {
         h = calculate_h(x[i]);

         x_forward[i] += h;
         y_forward = (t.*f)(x_forward);
         x_forward[i] -= h;

         x_backward[i] -= h;
         y_backward = (t.*f)(x_backward);
         x_backward[i] += h;

         g[i] = (y_forward - y_backward)/(2.0*h); 
      }

      return(g);
   }


   // Vector<double> calculate_gradient(const T&, double (T::*f)(const Vector<double>&) const, const Vector<double>&) const method

   /// This method returns the gradient of a function of several dimensions acording to the numerical differentiation method to be used. 
   /// The function to be differentiated is of the following form: double f(const Vector<double>&) const. 
   /// @param t : Object constructor containing the member method to differentiate.  
   /// @param f: Pointer to the member method.
   /// @param x: Input vector. 

   template<class T> 
   Vector<double> calculate_gradient(const T& t, double (T::*f)(const Vector<double>&) const, const Vector<double>& x) const
   {
      switch(numerical_differentiation_method)
      {
         case ForwardDifferences:
         {
            return(calculate_forward_differences_gradient(t, f, x));
      	 }
	     break;

         case CentralDifferences:
         {
            return(calculate_central_differences_gradient(t, f, x));
    	 }
   	     break;

         default:
         {
            std::ostringstream buffer;

            buffer << "OpenNN Exception: NumericalDifferentiation class.\n"
                   << "Vector<double> calculate_gradient(const T& t, double (T::*f)(const Vector<double>&) const, const Vector<double>&) const method.\n"
                   << "Unknown numerical differentiation method.\n";
 
            throw std::logic_error(buffer.str());
	     }
	     break;
      }
   }


   // Vector<double> calculate_forward_differences_gradient(const T&, double (T::*f)(const Vector<double>&), const Vector<double>&) const method

   /// This method returns the gradient of a function of several dimensions using the forward differences method. 
   /// The function to be differentiated is of the following form: double f(const Vector<double>&). 
   /// @param t : Object constructor containing the member method to differentiate.  
   /// @param f: Pointer to the member method.
   /// @param x: Input vector. 

   template<class T> 
   Vector<double> calculate_forward_differences_gradient(const T& t, double (T::*f)(const Vector<double>&), const Vector<double>& x) const
   {
      const unsigned int n = x.size();

      double h;

      double y = (t.*f)(x);
      
	  Vector<double> x_forward(x);
  
      double y_forward;

	  Vector<double> g(n);

      for(unsigned int i = 0; i < n; i++)
      {
         h = calculate_h(x[i]);
 
         x_forward[i] += h;
         y_forward = (t.*f)(x_forward);
         x_forward[i] -= h;

         g[i] = (y_forward - y)/h; 
      }

      return(g);
   }


   // Vector<double> calculate_central_differences_gradient(const T& t, double (T::*f)(const Vector<double>&), const Vector<double>& x) const method

   /// This method returns the gradient of a function of several dimensions using the central differences method. 
   /// The function to be differentiated is of the following form: double f(const Vector<double>&) const. 
   /// @param t : Object constructor containing the member method to differentiate.  
   /// @param f: Pointer to the member method.
   /// @param x: Input vector. 

   template<class T> 
   Vector<double> calculate_central_differences_gradient(const T& t, double (T::*f)(const Vector<double>&), const Vector<double>& x) const
   {      
      const unsigned int n = x.size();

      double h;

	  Vector<double> x_forward(x);
	  Vector<double> x_backward(x);
  
	  double y_forward;
      double y_backward;

      Vector<double> g(n);

      for(unsigned int i = 0; i < n; i++)
      {
         h = calculate_h(x[i]);

         x_forward[i] += h;
         y_forward = (t.*f)(x_forward);
         x_forward[i] -= h;

         x_backward[i] -= h;
         y_backward = (t.*f)(x_backward);
         x_backward[i] += h;

         g[i] = (y_forward - y_backward)/(2.0*h); 
      }

      return(g);
   }


   // Vector<double> calculate_gradient(const T&, double (T::*f)(const Vector<double>&), const Vector<double>&) const method

   /// This method returns the gradient of a function of several dimensions acording to the numerical differentiation method to be used. 
   /// The function to be differentiated is of the following form: double f(const Vector<double>&) const. 
   /// @param t : Object constructor containing the member method to differentiate.  
   /// @param f: Pointer to the member method.
   /// @param x: Input vector. 

   template<class T> 
   Vector<double> calculate_gradient(const T& t, double (T::*f)(const Vector<double>&), const Vector<double>& x) const
   {
      switch(numerical_differentiation_method)
      {
         case ForwardDifferences:
         {
            return(calculate_forward_differences_gradient(t, f, x));
      	 }
	     break;

         case CentralDifferences:
         {
            return(calculate_central_differences_gradient(t, f, x));
    	 }
   	     break;

         default:
         {
            std::ostringstream buffer;

            buffer << "OpenNN Exception: NumericalDifferentiation class.\n"
                   << "Vector<double> calculate_gradient(const T& t, double (T::*f)(const Vector<double>&), const Vector<double>&) const method.\n"
                   << "Unknown numerical differentiation method.\n";
 
            throw std::logic_error(buffer.str());
	     }
	     break;
      }
   }


   // Vector<double> calculate_forward_differences_gradient(const T&, double (T::*f)(const Vector<double>&, const Vector<double>&), const Vector<double>&, const Vector<double>&) const

   /// This method returns the gradient of a function of several dimensions using the forward differences method. 
   /// The function to be differentiated is of the following form: double f(const Vector<double>&, const Vector<double>&) const. 
   /// The first vector argument is dummy, differentiation is performed with respect to the second vector argument. 
   /// @param t : Object constructor containing the member method to differentiate.  
   /// @param f: Pointer to the member method.
   /// @param dummy: Dummy vector for the method prototype.
   /// @param x: Input vector. 

   template<class T> 
   Vector<double> calculate_forward_differences_gradient(const T& t, double (T::*f)(const Vector<double>&, const Vector<double>&) const, const Vector<double>& dummy, const Vector<double>& x) const
   {
      const unsigned int n = x.size();

      double h;

      const double y = (t.*f)(dummy, x);
      
      Vector<double> x_forward(x);
  
      double y_forward;

	  Vector<double> g(n);

      for(unsigned int i = 0; i < n; i++)
      {
         h = calculate_h(x[i]);
 
         x_forward[i] += h;
         y_forward = (t.*f)(dummy, x_forward);
         x_forward[i] -= h;

         g[i] = (y_forward - y)/h; 
      }

      return(g);
   }


   // Vector<double> calculate_central_differences_gradient(const T&, double (T::*f)(const Vector<double>&, const Vector<double>&) const, const Vector<double>&, const Vector<double>&) const method

   /// This method returns the gradient of a function of several dimensions using the central differences method. 
   /// The function to be differentiated is of the following form: double f(const Vector<double>&, const Vector<double>&) const. 
   /// The first vector argument is dummy, differentiation is performed with respect to the second vector argument. 
   /// @param t : Object constructor containing the member method to differentiate.  
   /// @param f: Pointer to the member method.
   /// @param dummy: Dummy vector for the method prototype.
   /// @param x: Input vector. 

   template<class T> 
   Vector<double> calculate_central_differences_gradient(const T& t, double (T::*f)(const Vector<double>&, const Vector<double>&) const, const Vector<double>& dummy, const Vector<double>& x) const
   {    
      const unsigned int n = x.size();

      double h;

      Vector<double> x_forward(x);
      Vector<double> x_backward(x);
  
      double y_forward;
      double y_backward;

      Vector<double> g(n);

      for(unsigned int i = 0; i < n; i++)
      {
         h = calculate_h(x[i]);
 
         x_forward[i] += h;
         y_forward = (t.*f)(dummy, x_forward);
         x_forward[i] -= h;

         x_backward[i] -= h;
         y_backward = (t.*f)(dummy, x_backward);
         x_backward[i] += h;

         g[i] = (y_forward - y_backward)/(2.0*h); 
      }

      return(g);
   }


   // Vector<double> calculate_gradient(const T&, double (T::*f)(const Vector<double>&, const Vector<double>&) const, const Vector<double>&, const Vector<double>&) const method

   /// This method returns the gradient of a function of several dimensions acording to the numerical differentiation method to be used. 
   /// The function to be differentiated is of the following form: double f(const Vector<double>&, const Vector<double>&) const. 
   /// The first vector argument is dummy, differentiation is performed with respect to the second vector argument. 
   /// @param t : Object constructor containing the member method to differentiate.  
   /// @param f: Pointer to the member method.
   /// @param dummy: Dummy vector for the method prototype.
   /// @param x: Input vector. 

   template<class T> 
   Vector<double> calculate_gradient(const T& t, double (T::*f)(const Vector<double>&, const Vector<double>&) const, const Vector<double>& dummy, const Vector<double>& x) const
   {
      switch(numerical_differentiation_method)
      {
         case ForwardDifferences:
         {
            return(calculate_forward_differences_gradient(t, f, dummy, x));
      	 }
	     break;

         case CentralDifferences:
         {
            return(calculate_central_differences_gradient(t, f, dummy, x));
    	 }
   	     break;

         default:
         {
            std::ostringstream buffer;

            buffer << "OpenNN Exception: NumericalDifferentiation class.\n"
                   << "Vector<double> calculate_gradient(const T& t, double (T::*f)(const Vector<double>&, const Vector<double>&) const, const Vector<double>&, const Vector<double>&) const method.\n"
                   << "Unknown numerical differentiation method.\n";
 
            throw std::logic_error(buffer.str());
	     }
	     break;
      }
   }


   // Vector<double> calculate_forward_differences_gradient(const T&, double (T::*f)(const unsigned int&, const Vector<double>&) const, const unsigned int&, const Vector<double>&) const method

   /// This method returns the gradient of a function of several dimensions using the forward differences method. 
   /// The function to be differentiated is of the following form: double f(const unsigned int&, const Vector<double>&) const. 
   /// The first integer argument is used for the function definition, differentiation is performed with respect to the second vector argument. 
   /// @param t : Object constructor containing the member method to differentiate.  
   /// @param f: Pointer to the member method.
   /// @param dummy: Dummy integer for the method prototype.
   /// @param x: Input vector. 

   template<class T> 
   Vector<double> calculate_forward_differences_gradient(const T& t, double (T::*f)(const unsigned int&, const Vector<double>&) const, const unsigned int& dummy, const Vector<double>& x) const
   {
      unsigned int n = x.size();

      double h;

      double y = (t.*f)(dummy, x);
      
	  Vector<double> x_forward(x);
  
      double y_forward;

	  Vector<double> g(n);

      for(unsigned int i = 0; i < n; i++)
      {
         h = calculate_h(x[i]);
 
         x_forward[i] += h;
         y_forward = (t.*f)(dummy, x_forward);
         x_forward[i] -= h;

         g[i] = (y_forward - y)/h; 
      }

      return(g);
   }


   // Vector<double> calculate_central_differences_gradient(const T&, double (T::*f)(const unsigned int&, const Vector<double>&) const, const unsigned int&, const Vector<double>&) const method

   /// This method returns the gradient of a function of several dimensions using the central differences method. 
   /// The function to be differentiated is of the following form: double f(const unsigned int&, const Vector<double>&) const. 
   /// The first integer argument is used for the function definition, differentiation is performed with respect to the second vector argument. 
   /// @param t : Object constructor containing the member method to differentiate.  
   /// @param f: Pointer to the member method.
   /// @param dummy: Dummy integer for the method prototype.
   /// @param x: Input vector. 

   template<class T> 
   Vector<double> calculate_central_differences_gradient(const T& t, double (T::*f)(const unsigned int&, const Vector<double>&) const, const unsigned int& dummy, const Vector<double>& x) const
   {      
      unsigned int n = x.size();

      double h;

	  Vector<double> x_forward(x);
	  Vector<double> x_backward(x);
  
	  double y_forward;
      double y_backward;

      Vector<double> g(n);

      for(unsigned int i = 0; i < n; i++)
      {
         h = calculate_h(x[i]);
 
         x_forward[i] += h;
         y_forward = (t.*f)(dummy, x_forward);
         x_forward[i] -= h;

         x_backward[i] -= h;
         y_backward = (t.*f)(dummy, x_backward);
         x_backward[i] += h;

         g[i] = (y_forward - y_backward)/(2.0*h); 
      }

      return(g);
   }


   // Vector<double> calculate_gradient(const T&, double (T::*f)(const unsigned int&, const Vector<double>&) const, const unsigned int&, const Vector<double>&) const method

   /// This method returns the gradient of a function of several dimensions acording to the numerical differentiation method to be used. 
   /// The function to be differentiated is of the following form: double f(const unsigned int&, const Vector<double>&) const. 
   /// The first integer argument is used for the function definition, differentiation is performed with respect to the second vector argument. 
   /// @param t : Object constructor containing the member method to differentiate.  
   /// @param f: Pointer to the member method.
   /// @param dummy: Dummy integer for the method prototype.
   /// @param x: Input vector. 

   template<class T> 
   Vector<double> calculate_gradient(const T& t, double (T::*f)(const unsigned int&, const Vector<double>&) const, const unsigned int& dummy, const Vector<double>& x) const
   {
      switch(numerical_differentiation_method)
      {
         case ForwardDifferences:
         {
            return(calculate_forward_differences_gradient(t, f, dummy, x));
      	 }
	     break;

         case CentralDifferences:
         {
            return(calculate_central_differences_gradient(t, f, dummy, x));
    	 }
   	     break;

         default:
         {
            std::ostringstream buffer;

            buffer << "OpenNN Exception: NumericalDifferentiation class.\n"
                   << "Vector<double> calculate_gradient(const T& t, double (T::*f)(const unsigned int&, const Vector<double>&) const, const unsigned int&, const Vector<double>&) const method.\n"
                   << "Unknown numerical differentiation method.\n";
 
            throw std::logic_error(buffer.str());
	     }
	     break;
      }
   }


   // HESSIAN METHODS


   // Matrix<double> calculate_forward_differences_Hessian(const T&, double (T::*f)(const Vector<double>&) const, const Vector<double>&) const method

   /// This method returns the Hessian matrix of a function of several dimensions using the forward differences method. 
   /// The function to be differentiated is of the following form: double f(const Vector<double>&) const. 
   /// @param t : Object constructor containing the member method to differentiate.  
   /// @param f: Pointer to the member method.
   /// @param x: Input vector. 

   template<class T> 
   Matrix<double> calculate_forward_differences_Hessian(const T& t, double (T::*f)(const Vector<double>&) const, const Vector<double>& x) const
   {
      unsigned int n = x.size();

      Matrix<double> H(n, n);

      double h_i;
      double h_j;

      double y = (t.*f)(x);

      Vector<double> x_forward_2i(x);
      Vector<double> x_forward_ij(x);
      Vector<double> x_forward_i(x);
      Vector<double> x_forward_j(x);

      double y_forward_2i;
      double y_forward_ij;
      double y_forward_i;
      double y_forward_j;

      for(unsigned int i = 0; i < n; i++)
      {
         h_i = calculate_h(x[i]);

         x_forward_i[i] += h_i;       
         y_forward_i = (t.*f)(x_forward_i);
         x_forward_i[i] -= h_i;       

         x_forward_2i[i] += 2.0*h_i;       
         y_forward_2i = (t.*f)(x_forward_2i);
         x_forward_2i[i] -= 2.0*h_i;       

         H[i][i] = (y_forward_2i - 2*y_forward_i + y)/pow(h_i, 2);  

         for(unsigned int j = i; j < n; j++)
         {
            h_j = calculate_h(x[j]);

            x_forward_j[j] += h_j;       
            y_forward_j = (t.*f)(x_forward_j);
            x_forward_j[j] -= h_j;       

            x_forward_ij[i] += h_i; 
            x_forward_ij[j] += h_j; 
            y_forward_ij = (t.*f)(x_forward_ij);   
            x_forward_ij[i] -= h_i; 
            x_forward_ij[j] -= h_j; 
            
            H[i][j] = (y_forward_ij - y_forward_i - y_forward_j + y)/(h_i*h_j);
         } 
      }

      for(unsigned int i = 0; i < n; i++)
      {
         for(unsigned int j = 0; j < i; j++)
         {
            H[i][j] = H[j][i];
         }
      }

      return(H);
   }


   // Matrix<double> calculate_central_differences_Hessian(const T&, double (T::*f)(const Vector<double>&) const, const Vector<double>&) const method

   /// This method returns the Hessian matrix of a function of several dimensions using the central differences method. 
   /// The function to be differentiated is of the following form: double f(const Vector<double>&) const. 
   /// @param t : Object constructor containing the member method to differentiate.  
   /// @param f: Pointer to the member method.
   /// @param x: Input vector. 

   template<class T> 
   Matrix<double> calculate_central_differences_Hessian(const T& t, double (T::*f)(const Vector<double>&) const, const Vector<double>& x) const
   {
      unsigned int n = x.size();

      double y = (t.*f)(x);

      Matrix<double> H(n, n);

      double h_i;
      double h_j;

      Vector<double> x_backward_2i(x);
      Vector<double> x_backward_i(x);

      Vector<double> x_forward_i(x);
      Vector<double> x_forward_2i(x);      

      Vector<double> x_backward_ij(x);
      Vector<double> x_forward_ij(x);

      Vector<double> x_backward_i_forward_j(x);
      Vector<double> x_forward_i_backward_j(x);

      double y_backward_2i;
      double y_backward_i;

      double y_forward_i;
      double y_forward_2i;
   
      double y_backward_ij;
      double y_forward_ij;

      double y_backward_i_forward_j;
      double y_forward_i_backward_j;

      for(unsigned int i = 0; i < n; i++)
      {
         h_i = calculate_h(x[i]);

         x_backward_2i[i] -= 2.0*h_i; 
         y_backward_2i = (t.*f)(x_backward_2i);
         x_backward_2i[i] += 2.0*h_i; 

         x_backward_i[i] -= h_i; 
         y_backward_i = (t.*f)(x_backward_i);
         x_backward_i[i] += h_i; 

         x_forward_i[i] += h_i; 
         y_forward_i = (t.*f)(x_forward_i);
         x_forward_i[i] -= h_i; 

         x_forward_2i[i] += 2.0*h_i; 
         y_forward_2i = (t.*f)(x_forward_2i);
         x_forward_2i[i] -= 2.0*h_i; 

         H[i][i] = (-y_forward_2i + 16.0*y_forward_i -30.0*y + 16.0*y_backward_i - y_backward_2i)/(12.0*pow(h_i, 2));  

         for(unsigned int j = i; j < n; j++)
         {
            h_j = calculate_h(x[j]);

            x_backward_ij[i] -= h_i;  
            x_backward_ij[j] -= h_j;  
            y_backward_ij = (t.*f)(x_backward_ij);   
            x_backward_ij[i] += h_i;  
            x_backward_ij[j] += h_j;  

            x_forward_ij[i] += h_i;  
            x_forward_ij[j] += h_j;  
            y_forward_ij = (t.*f)(x_forward_ij);   
            x_forward_ij[i] -= h_i;  
            x_forward_ij[j] -= h_j;  
            
            x_backward_i_forward_j[i] -= h_i;
            x_backward_i_forward_j[j] += h_j;
            y_backward_i_forward_j = (t.*f)(x_backward_i_forward_j);   
            x_backward_i_forward_j[i] += h_i;
            x_backward_i_forward_j[j] -= h_j;

            x_forward_i_backward_j[i] += h_i;
            x_forward_i_backward_j[j] -= h_j;
            y_forward_i_backward_j = (t.*f)(x_forward_i_backward_j);   
            x_forward_i_backward_j[i] -= h_i;
            x_forward_i_backward_j[j] += h_j;
 
            H[i][j] = (y_forward_ij - y_forward_i_backward_j - y_backward_i_forward_j + y_backward_ij)/(4.0*h_i*h_j);
         }
      }

      for(unsigned int i = 0; i < n; i++)
      {
         for(unsigned int j = 0; j < i; j++)
         {
            H[i][j] = H[j][i];
         }
      }

      return(H);
   }


   // Matrix<double> calculate_Hessian(const T&, double (T::*f)(const Vector<double>&) const, const Vector<double>&) const method

   /// This method returns the Hessian matrix of a function of several dimensions acording to the numerical differentiation method to be used. 
   /// The function to be differentiated is of the following form: double f(const Vector<double>&) const. 
   /// @param t : Object constructor containing the member method to differentiate.  
   /// @param f: Pointer to the member method.
   /// @param x: Input vector. 

   template<class T> 
   Matrix<double> calculate_Hessian(const T& t, double (T::*f)(const Vector<double>&) const, const Vector<double>& x) const
   {
      switch(numerical_differentiation_method)
      {
         case ForwardDifferences:
         {
            return(calculate_forward_differences_Hessian(t, f, x));
      	 }
	     break;

         case CentralDifferences:
         {
            return(calculate_central_differences_Hessian(t, f, x));
         }
   	     break;

         default:
         {
            std::ostringstream buffer;

            buffer << "OpenNN Exception: NumericalDifferentiation class.\n"
                   << "double calculate_Hessian(const T& t, double (T::*f)(const Vector<double>&) const, const Vector<double>&) const method.\n"
                   << "Unknown numerical differentiation method.\n";
 
            throw std::logic_error(buffer.str());
         }
	     break;
      }
   }


   // Matrix<double> calculate_forward_differences_Hessian(const T&, double (T::*f)(const Vector<double>&, const Vector<double>&) const, const Vector<double>& dummy, const Vector<double>& x) const method

   /// This method returns the Hessian matrix of a function of several dimensions using the forward differences method. 
   /// The function to be differentiated is of the following form: double f(const Vector<double>&, const Vector<double>&) const. 
   /// The first vector argument is dummy, differentiation is performed with respect to the second vector argument. 
   /// @param t : Object constructor containing the member method to differentiate.  
   /// @param f: Pointer to the member method.
   /// @param dummy: Dummy vector for the method prototype.
   /// @param x: Input vector. 

   template<class T> 
   Matrix<double> calculate_forward_differences_Hessian(const T& t, double (T::*f)(const Vector<double>&, const Vector<double>&) const, const Vector<double>& dummy, const Vector<double>& x) const
   {
      unsigned int n = x.size();

      Matrix<double> H(n, n);

      double h_i;
      double h_j;

      double y = (t.*f)(dummy, x);

      Vector<double> x_forward_2i(x);
      Vector<double> x_forward_ij(x);
      Vector<double> x_forward_i(x);
      Vector<double> x_forward_j(x);

      double y_forward_2i;
      double y_forward_ij;
      double y_forward_i;
      double y_forward_j;

      for(unsigned int i = 0; i < n; i++)
      {
         h_i = calculate_h(x[i]);

         x_forward_i[i] += h_i;       
         y_forward_i = (t.*f)(dummy, x_forward_i);
         x_forward_i[i] -= h_i;       

         x_forward_2i[i] += 2.0*h_i;       
         y_forward_2i = (t.*f)(dummy, x_forward_2i);
         x_forward_2i[i] -= 2.0*h_i;       

         H[i][i] = (y_forward_2i - 2*y_forward_i + y)/pow(h_i, 2);  

         for(unsigned int j = i; j < n; j++)
         {
            h_j = calculate_h(x[j]);

            x_forward_j[j] += h_j;       
            y_forward_j = (t.*f)(dummy, x_forward_j);
            x_forward_j[j] -= h_j;       

            x_forward_ij[i] += h_i; 
            x_forward_ij[j] += h_j; 
            y_forward_ij = (t.*f)(dummy, x_forward_ij);   
            x_forward_ij[i] -= h_i; 
            x_forward_ij[j] -= h_j; 
            
            H[i][j] = (y_forward_ij - y_forward_i - y_forward_j + y)/(h_i*h_j);
         } 
      }

      for(unsigned int i = 0; i < n; i++)
      {
         for(unsigned int j = 0; j < i; j++)
         {
            H[i][j] = H[j][i];
         }
      }

      return(H);
   }


   // Matrix<double> calculate_central_differences_Hessian(const T&, double (T::*f)(const Vector<double>&, const Vector<double>&) const, const Vector<double>&, const Vector<double>&) const method

   /// This method returns the Hessian matrix of a function of several dimensions using the central differences method. 
   /// The function to be differentiated is of the following form: double f(const Vector<double>&, const Vector<double>&) const. 
   /// The first vector argument is dummy, differentiation is performed with respect to the second vector argument. 
   /// @param t : Object constructor containing the member method to differentiate.  
   /// @param f: Pointer to the member method.
   /// @param dummy: Dummy vector for the method prototype.
   /// @param x: Input vector. 

   template<class T> 
   Matrix<double> calculate_central_differences_Hessian(const T& t, double (T::*f)(const Vector<double>&, const Vector<double>&) const, const Vector<double>& dummy, const Vector<double>& x) const
   {
      unsigned int n = x.size();

      double y = (t.*f)(dummy, x);

      Matrix<double> H(n, n);

      double h_i;
      double h_j;

      Vector<double> x_backward_2i(x);
      Vector<double> x_backward_i(x);

      Vector<double> x_forward_i(x);
      Vector<double> x_forward_2i(x);      

      Vector<double> x_backward_ij(x);
      Vector<double> x_forward_ij(x);

      Vector<double> x_backward_i_forward_j(x);
      Vector<double> x_forward_i_backward_j(x);

      double y_backward_2i;
      double y_backward_i;

      double y_forward_i;
      double y_forward_2i;
   
      double y_backward_ij;
      double y_forward_ij;

      double y_backward_i_forward_j;
      double y_forward_i_backward_j;

      for(unsigned int i = 0; i < n; i++)
      {
         h_i = calculate_h(x[i]);

         x_backward_2i[i] -= 2.0*h_i; 
         y_backward_2i = (t.*f)(dummy, x_backward_2i);
         x_backward_2i[i] += 2.0*h_i; 

         x_backward_i[i] -= h_i; 
         y_backward_i = (t.*f)(dummy, x_backward_i);
         x_backward_i[i] += h_i; 

         x_forward_i[i] += h_i; 
         y_forward_i = (t.*f)(dummy, x_forward_i);
         x_forward_i[i] -= h_i; 

         x_forward_2i[i] += 2.0*h_i; 
         y_forward_2i = (t.*f)(dummy, x_forward_2i);
         x_forward_2i[i] -= 2.0*h_i; 

         H[i][i] = (-y_forward_2i + 16.0*y_forward_i -30.0*y + 16.0*y_backward_i - y_backward_2i)/(12.0*pow(h_i, 2));  

         for(unsigned int j = i; j < n; j++)
         {
            h_j = calculate_h(x[j]);

            x_backward_ij[i] -= h_i;  
            x_backward_ij[j] -= h_j;  
            y_backward_ij = (t.*f)(dummy, x_backward_ij);   
            x_backward_ij[i] += h_i;  
            x_backward_ij[j] += h_j;  

            x_forward_ij[i] += h_i;  
            x_forward_ij[j] += h_j;  
            y_forward_ij = (t.*f)(dummy, x_forward_ij);   
            x_forward_ij[i] -= h_i;  
            x_forward_ij[j] -= h_j;  
            
            x_backward_i_forward_j[i] -= h_i;
            x_backward_i_forward_j[j] += h_j;
            y_backward_i_forward_j = (t.*f)(dummy, x_backward_i_forward_j);   
            x_backward_i_forward_j[i] += h_i;
            x_backward_i_forward_j[j] -= h_j;

            x_forward_i_backward_j[i] += h_i;
            x_forward_i_backward_j[j] -= h_j;
            y_forward_i_backward_j = (t.*f)(dummy, x_forward_i_backward_j);   
            x_forward_i_backward_j[i] -= h_i;
            x_forward_i_backward_j[j] += h_j;
 
            H[i][j] = (y_forward_ij - y_forward_i_backward_j - y_backward_i_forward_j + y_backward_ij)/(4.0*h_i*h_j);
         }
      }

      for(unsigned int i = 0; i < n; i++)
      {
         for(unsigned int j = 0; j < i; j++)
         {
            H[i][j] = H[j][i];
         }
      }

      return(H);
   }


   // Matrix<double> calculate_Hessian(const T&, double (T::*f)(const Vector<double>&, const Vector<double>&) const, const Vector<double>&, const Vector<double>&) const method

   /// This method returns the Hessian matrix of a function of several dimensions according to the numerical differentiation method to be used. 
   /// The function to be differentiated is of the following form: double f(const Vector<double>&, const Vector<double>&) const. 
   /// The first vector argument is dummy, differentiation is performed with respect to the second vector argument. 
   /// @param t : Object constructor containing the member method to differentiate.  
   /// @param f: Pointer to the member method.
   /// @param dummy: Dummy vector for the method prototype.
   /// @param x: Input vector. 

   template<class T> 
   Matrix<double> calculate_Hessian(const T& t, double (T::*f)(const Vector<double>&, const Vector<double>&) const, const Vector<double>& dummy, const Vector<double>& x) const
   {
      switch(numerical_differentiation_method)
      {
         case ForwardDifferences:
         {
            return(calculate_forward_differences_Hessian(t, f, dummy, x));
      	 }
	     break;

         case CentralDifferences:
         {
            return(calculate_central_differences_Hessian(t, f, dummy, x));
         }
   	     break;

         default:
         {
            std::ostringstream buffer;

            buffer << "OpenNN Exception: NumericalDifferentiation class.\n"
                   << "double calculate_Hessian(const T& t, double (T::*f)(const Vector<double>&, const Vector<double>&) const, const Vector<double>&, const Vector<double>&) const method.\n"
                   << "Unknown numerical differentiation method.\n";
 
            throw std::logic_error(buffer.str());
         }
	     break;
      }
   }


   // Matrix<double> calculate_forward_differences_Hessian(const T&, double (T::*f)(const unsigned int&, const Vector<double>&) const, const unsigned int&, const Vector<double>&) const method

   /// This method returns the Hessian matrix of a function of several dimensions using the forward differences method. 
   /// The function to be differentiated is of the following form: double f(const unsigned int&, const Vector<double>&) const. 
   /// The first integer argument is dummy, differentiation is performed with respect to the second vector argument. 
   /// @param t : Object constructor containing the member method to differentiate.  
   /// @param f: Pointer to the member method.
   /// @param dummy: Dummy integer for the method prototype.
   /// @param x: Input vector. 

   template<class T> 
   Matrix<double> calculate_forward_differences_Hessian(const T& t, double (T::*f)(const unsigned int&, const Vector<double>&) const, const unsigned int& dummy, const Vector<double>& x) const
   {
      unsigned int n = x.size();

      Matrix<double> H(n, n);

      double h_i;
      double h_j;

      double y = (t.*f)(dummy, x);

      Vector<double> x_forward_2i(x);
      Vector<double> x_forward_ij(x);
      Vector<double> x_forward_i(x);
      Vector<double> x_forward_j(x);

      double y_forward_2i;
      double y_forward_ij;
      double y_forward_i;
      double y_forward_j;

      for(unsigned int i = 0; i < n; i++)
      {
         h_i = calculate_h(x[i]);

         x_forward_i[i] += h_i;       
         y_forward_i = (t.*f)(dummy, x_forward_i);
         x_forward_i[i] -= h_i;       

         x_forward_2i[i] += 2.0*h_i;       
         y_forward_2i = (t.*f)(dummy, x_forward_2i);
         x_forward_2i[i] -= 2.0*h_i;       

         H[i][i] = (y_forward_2i - 2*y_forward_i + y)/pow(h_i, 2);  

         for(unsigned int j = i; j < n; j++)
         {
            h_j = calculate_h(x[j]);

            x_forward_j[j] += h_j;       
            y_forward_j = (t.*f)(dummy, x_forward_j);
            x_forward_j[j] -= h_j;       

            x_forward_ij[i] += h_i; 
            x_forward_ij[j] += h_j; 
            y_forward_ij = (t.*f)(dummy, x_forward_ij);   
            x_forward_ij[i] -= h_i; 
            x_forward_ij[j] -= h_j; 
            
            H[i][j] = (y_forward_ij - y_forward_i - y_forward_j + y)/(h_i*h_j);
         } 
      }

      for(unsigned int i = 0; i < n; i++)
      {
         for(unsigned int j = 0; j < i; j++)
         {
            H[i][j] = H[j][i];
         }
      }

      return(H);
   }


   // Matrix<double> calculate_central_differences_Hessian(const T&, double (T::*f)(const unsigned int&, const Vector<double>&) const, const unsigned int&, const Vector<double>&) const method

   /// This method returns the Hessian matrix of a function of several dimensions using the central differences method. 
   /// The function to be differentiated is of the following form: double f(const unsigned int&, const Vector<double>&) const. 
   /// The first integer argument is dummy, differentiation is performed with respect to the second vector argument. 
   /// @param t : Object constructor containing the member method to differentiate.  
   /// @param f: Pointer to the member method.
   /// @param dummy: Dummy integer for the method prototype.
   /// @param x: Input vector. 

   template<class T> 
   Matrix<double> calculate_central_differences_Hessian(const T& t, double (T::*f)(const unsigned int&, const Vector<double>&) const, const unsigned int& dummy, const Vector<double>& x) const
   {
      unsigned int n = x.size();

      double y = (t.*f)(dummy, x);

      Matrix<double> H(n, n);

      double h_i;
      double h_j;

      Vector<double> x_backward_2i(x);
      Vector<double> x_backward_i(x);

      Vector<double> x_forward_i(x);
      Vector<double> x_forward_2i(x);      

      Vector<double> x_backward_ij(x);
	  Vector<double> x_forward_ij(x);

	  Vector<double> x_backward_i_forward_j(x);
      Vector<double> x_forward_i_backward_j(x);

      double y_backward_2i;
      double y_backward_i;

      double y_forward_i;
      double y_forward_2i;
   
      double y_backward_ij;
	  double y_forward_ij;

	  double y_backward_i_forward_j;
      double y_forward_i_backward_j;

      for(unsigned int i = 0; i < n; i++)
      {
         h_i = calculate_h(x[i]);

         x_backward_2i[i] -= 2.0*h_i; 
         y_backward_2i = (t.*f)(dummy, x_backward_2i);
         x_backward_2i[i] += 2.0*h_i; 

         x_backward_i[i] -= h_i; 
         y_backward_i = (t.*f)(dummy, x_backward_i);
         x_backward_i[i] += h_i; 

         x_forward_i[i] += h_i; 
         y_forward_i = (t.*f)(dummy, x_forward_i);
         x_forward_i[i] -= h_i; 

         x_forward_2i[i] += 2.0*h_i; 
         y_forward_2i = (t.*f)(dummy, x_forward_2i);
         x_forward_2i[i] -= 2.0*h_i; 

         H[i][i] = (-y_forward_2i + 16.0*y_forward_i -30.0*y + 16.0*y_backward_i - y_backward_2i)/(12.0*pow(h_i, 2));  

         for(unsigned int j = i; j < n; j++)
         {
            h_j = calculate_h(x[j]);

            x_backward_ij[i] -= h_i;  
            x_backward_ij[j] -= h_j;  
            y_backward_ij = (t.*f)(dummy, x_backward_ij);   
            x_backward_ij[i] += h_i;  
            x_backward_ij[j] += h_j;  

            x_forward_ij[i] += h_i;  
            x_forward_ij[j] += h_j;  
            y_forward_ij = (t.*f)(dummy, x_forward_ij);   
            x_forward_ij[i] -= h_i;  
            x_forward_ij[j] -= h_j;  
            
            x_backward_i_forward_j[i] -= h_i;
            x_backward_i_forward_j[j] += h_j;
            y_backward_i_forward_j = (t.*f)(dummy, x_backward_i_forward_j);   
            x_backward_i_forward_j[i] += h_i;
            x_backward_i_forward_j[j] -= h_j;

			x_forward_i_backward_j[i] += h_i;
			x_forward_i_backward_j[j] -= h_j;
            y_forward_i_backward_j = (t.*f)(dummy, x_forward_i_backward_j);   
			x_forward_i_backward_j[i] -= h_i;
			x_forward_i_backward_j[j] += h_j;
 
            H[i][j] = (y_forward_ij - y_forward_i_backward_j - y_backward_i_forward_j + y_backward_ij)/(4.0*h_i*h_j);
         }
      }

      for(unsigned int i = 0; i < n; i++)
      {
         for(unsigned int j = 0; j < i; j++)
         {
            H[i][j] = H[j][i];
         }
      }

      return(H);
   }


   // Matrix<double> calculate_Hessian(const T&, double (T::*f)(const unsigned int&, const Vector<double>&) const, const unsigned int&, const Vector<double>&) const method

   /// This method returns the Hessian matrix of a function of several dimensions according to the numerical differentiation method to be used. 
   /// The function to be differentiated is of the following form: double f(const unsigned int&, const Vector<double>&) const. 
   /// The first integer argument is dummy, differentiation is performed with respect to the second vector argument. 
   /// @param t : Object constructor containing the member method to differentiate.  
   /// @param f: Pointer to the member method.
   /// @param dummy: Dummy integer for the method prototype.
   /// @param x: Input vector. 

   template<class T> 
   Matrix<double> calculate_Hessian(const T& t, double (T::*f)(const unsigned int&, const Vector<double>&) const, const unsigned int& dummy, const Vector<double>& x) const
   {
      switch(numerical_differentiation_method)
      {
         case ForwardDifferences:
         {
            return(calculate_forward_differences_Hessian(t, f, dummy, x));
      	 }
	     break;

         case CentralDifferences:
         {
            return(calculate_central_differences_Hessian(t, f, dummy, x));
    	 }
   	     break;

         default:
         {
            std::ostringstream buffer;

            buffer << "OpenNN Exception: NumericalDifferentiation class.\n"
                   << "double calculate_Hessian(const T& t, double (T::*f)(const unsigned int&, const Vector<double>&) const, const unsigned int&, const Vector<double>&) const method.\n"
                   << "Unknown numerical differentiation method.\n";
 
            throw std::logic_error(buffer.str());
	     }
	     break;
      }
   }


   // JACOBIAN METHODS

   // Matrix<double> calculate_forward_differences_Jacobian(const T&, Vector<double> (T::*f)(const Vector<double>&) const, const Vector<double>&) const

   /// This method returns the Jacobian matrix of a function of many inputs and many outputs using the forward differences method. 
   /// The function to be differentiated is of the following form: Vector<double> f(const Vector<double>&, const Vector<double>&) const. 
   /// @param t : Object constructor containing the member method to differentiate.  
   /// @param f: Pointer to the member method.
   /// @param x: Input vector. 

   template<class T> 
   Matrix<double> calculate_forward_differences_Jacobian(const T& t, Vector<double> (T::*f)(const Vector<double>&) const, const Vector<double>& x) const
   {
      Vector<double> y = (t.*f)(x); 

      double h;

      unsigned int n = x.size();
      unsigned int m = y.size();

      Vector<double> x_forward(x);
      Vector<double> y_forward(n);

      Matrix<double> J(m,n);

      for(unsigned int j = 0; j < n; j++)
	   {
         h = calculate_h(x[j]);

         x_forward[j] += h;
         y_forward = (t.*f)(x_forward);   
         x_forward[j] -= h;
         
	     for(unsigned int i = 0; i < m; i++)
		  {
		     J[i][j] = (y_forward[i] - y[i])/h;
		  }
	  }

      return(J);
   }


   // Matrix<double> calculate_central_differences_Jacobian(const T&, Vector<double> (T::*f)(const Vector<double>&) const, const Vector<double>&) const method

   /// This method returns the Jacobian matrix of a function of many inputs and many outputs using the central differences method. 
   /// The function to be differentiated is of the following form: Vector<double> f(const Vector<double>&, const Vector<double>&) const. 
   /// @param t : Object constructor containing the member method to differentiate.  
   /// @param f: Pointer to the member method.
   /// @param x: Input vector. 

   template<class T> 
   Matrix<double> calculate_central_differences_Jacobian(const T& t, Vector<double> (T::*f)(const Vector<double>&) const, const Vector<double>& x) const
   {
      Vector<double> y = (t.*f)(x); 

      double h;

      unsigned int n = x.size();
      unsigned int m = y.size();

      Vector<double> x_forward(x);
      Vector<double> x_backward(x);

	  Vector<double> y_forward(n);
	  Vector<double> y_backward(n);

      Matrix<double> J(m,n);

      for(unsigned int j = 0; j < n; j++)
	  {
         h = calculate_h(x[j]);

         x_backward[j] -= h;
         y_backward = (t.*f)(x_backward);   
         x_backward[j] += h;

         x_forward[j] += h;
         y_forward = (t.*f)(x_forward);   
         x_forward[j] -= h;
         
	     for(unsigned int i = 0; i < m; i++)
		 {
		    J[i][j] = (y_forward[i] - y_backward[i])/(2.0*h);
		 }
	  }

      return(J);
   }


   // Matrix<double> calculate_Jacobian(const T&, Vector<double> (T::*f)(const Vector<double>&) const, const Vector<double>&) const method

   /// This method returns the Jacobian matrix of a function of many inputs and many outputs according to the numerical differentiation method to be used. 
   /// The function to be differentiated is of the following form: Vector<double> f(const Vector<double>&, const Vector<double>&) const. 
   /// @param t : Object constructor containing the member method to differentiate.  
   /// @param f: Pointer to the member method.
   /// @param x: Input vector. 

   template<class T> 
   Matrix<double> calculate_Jacobian(const T& t, Vector<double> (T::*f)(const Vector<double>&) const, const Vector<double>& x) const
   {
      switch(numerical_differentiation_method)
      {
         case ForwardDifferences:
         {
            return(calculate_forward_differences_Jacobian(t, f, x));
      	 }
	     break;

         case CentralDifferences:
         {
            return(calculate_central_differences_Jacobian(t, f, x));
    	 }
   	     break;

         default:
         {
            std::ostringstream buffer;

            buffer << "OpenNN Exception: NumericalDifferentiation class.\n"
                   << "Matrix<double> calculate_Jacobian(const T&, Vector<double> (T::*f)(const Vector<double>&) const, const Vector<double>&) const method.\n"
                   << "Unknown numerical differentiation method.\n";
 
            throw std::logic_error(buffer.str());
	     }
	     break;
      }
   }


   // Matrix<double> calculate_forward_differences_Jacobian(const T&, Vector<double> (T::*f)(const Vector<double>&, const Vector<double>&) const, const Vector<double>&, const Vector<double>&) const

   /// This method returns the Jacobian matrix of a function of many inputs and many outputs using the forward differences method. 
   /// The function to be differentiated is of the following form: Vector<double> f(const Vector<double>&, const Vector<double>&) const. 
   /// @param t  Object constructor containing the member method to differentiate.  
   /// @param f Pointer to the member method.
   /// @param dummy Dummy vector for the method prototype.
   /// @param x Input vector. 

   template<class T> 
   Matrix<double> calculate_forward_differences_Jacobian(const T& t, Vector<double> (T::*f)(const Vector<double>&, const Vector<double>&) const, const Vector<double>& dummy, const Vector<double>& x) const
   {
      Vector<double> y = (t.*f)(dummy, x); 

      double h;

      unsigned int n = x.size();
      unsigned int m = y.size();

      Vector<double> x_forward(x);
      Vector<double> y_forward(n);

      Matrix<double> J(m,n);

      for(unsigned int j = 0; j < n; j++)
	   {
         h = calculate_h(x[j]);

         x_forward[j] += h;
         y_forward = (t.*f)(dummy, x_forward);   
         x_forward[j] -= h;
         
	     for(unsigned int i = 0; i < m; i++)
		  {
		     J[i][j] = (y_forward[i] - y[i])/h;
		  }
	  }

      return(J);
   }


   // Matrix<double> calculate_central_differences_Jacobian(const T&, Vector<double> (T::*f)(const Vector<double>&, const Vector<double>&) const, const Vector<double>&, const Vector<double>&) const method

   /// This method returns the Jacobian matrix of a function of many inputs and many outputs using the central differences method. 
   /// The function to be differentiated is of the following form: Vector<double> f(const Vector<double>&, const Vector<double>&) const. 
   /// @param t : Object constructor containing the member method to differentiate.  
   /// @param f: Pointer to the member method.
   /// @param dummy: Dummy vector for the method prototype. 
   /// @param x: Input vector. 

   template<class T> 
   Matrix<double> calculate_central_differences_Jacobian(const T& t, Vector<double> (T::*f)(const Vector<double>&, const Vector<double>&) const, const Vector<double>& dummy, const Vector<double>& x) const
   {
      Vector<double> y = (t.*f)(dummy, x); 

      double h;

      unsigned int n = x.size();
      unsigned int m = y.size();

      Vector<double> x_forward(x);
      Vector<double> x_backward(x);

	  Vector<double> y_forward(n);
	  Vector<double> y_backward(n);

      Matrix<double> J(m,n);

      for(unsigned int j = 0; j < n; j++)
	  {
         h = calculate_h(x[j]);

         x_backward[j] -= h;
         y_backward = (t.*f)(dummy, x_backward);   
         x_backward[j] += h;

         x_forward[j] += h;
         y_forward = (t.*f)(dummy, x_forward);   
         x_forward[j] -= h;
         
	     for(unsigned int i = 0; i < m; i++)
		 {
		    J[i][j] = (y_forward[i] - y_backward[i])/(2.0*h);
		 }
	  }

      return(J);
   }


   // Matrix<double> calculate_Jacobian(const T&, Vector<double> (T::*f)(const Vector<double>&, const Vector<double>&) const, const Vector<double>&, const Vector<double>&) const method

   /// This method returns the Jacobian matrix of a function of many inputs and many outputs according to the numerical differentiation method to be used. 
   /// The function to be differentiated is of the following form: Vector<double> f(const Vector<double>&, const Vector<double>&) const. 
   /// @param t : Object constructor containing the member method to differentiate.  
   /// @param f: Pointer to the member method.
   /// @param dummy: Dummy vector for the method prototype. 
   /// @param x: Input vector. 

   template<class T> 
   Matrix<double> calculate_Jacobian(const T& t, Vector<double> (T::*f)(const Vector<double>&, const Vector<double>&) const, const Vector<double>& dummy, const Vector<double>& x) const
   {
      switch(numerical_differentiation_method)
      {
         case ForwardDifferences:
         {
            return(calculate_forward_differences_Jacobian(t, f, dummy, x));
      	 }
	     break;

         case CentralDifferences:
         {
            return(calculate_central_differences_Jacobian(t, f, dummy, x));
    	 }
   	     break;

         default:
         {
            std::ostringstream buffer;

            buffer << "OpenNN Exception: NumericalDifferentiation class.\n"
                   << "Matrix<double> calculate_Jacobian(const T&, Vector<double> (T::*f)(const Vector<double>&, const Vector<double>&) const, const Vector<double>&, const Vector<double>&) const method.\n"
                   << "Unknown numerical differentiation method.\n";
 
            throw std::logic_error(buffer.str());
	     }
	     break;
      }
   }



   // Matrix<double> calculate_forward_differences_Jacobian(const T&, Vector<double> (T::*f)(const unsigned int&, const Vector<double>&) const, const unsigned int&, const Vector<double>&) const

   /// This method returns the Jacobian matrix of a function of many inputs and many outputs using the forward differences method. 
   /// The function to be differentiated is of the following form: double f(const unsigned int&, const Vector<double>&) const. 
   /// The first integer argument is dummy, differentiation is performed with respect to the second vector argument.  
   /// @param t : Object constructor containing the member method to differentiate.  
   /// @param f: Pointer to the member method.
   /// @param dummy: Dummy integer for the method prototype.
   /// @param x: Input vector. 

   template<class T> 
   Matrix<double> calculate_forward_differences_Jacobian(const T& t, Vector<double> (T::*f)(const unsigned int&, const Vector<double>&) const, const unsigned int& dummy, const Vector<double>& x) const
   {
      Vector<double> y = (t.*f)(dummy, x); 

      double h;

      unsigned int n = x.size();
      unsigned int m = y.size();

      Vector<double> x_forward(x);
      Vector<double> y_forward(n);

      Matrix<double> J(m,n);

      for(unsigned int j = 0; j < n; j++)
	  {
         h = calculate_h(x[j]);

         x_forward[j] += h;
         y_forward = (t.*f)(dummy, x_forward);   
         x_forward[j] -= h;
         
	     for(unsigned int i = 0; i < m; i++)
		 {
		    J[i][j] = (y_forward[i] - y[i])/h;
		 }
	  }

      return(J);
   }


   // Matrix<double> calculate_central_differences_Jacobian(const T&, Vector<double> (T::*f)(const unsigned int&, const Vector<double>&) const, const unsigned int&, const Vector<double>&) const

   /// This method returns the Jacobian matrix of a function of many inputs and many outputs using the central differences method. 
   /// The function to be differentiated is of the following form: double f(const unsigned int&, const Vector<double>&) const. 
   /// The first integer argument is dummy, differentiation is performed with respect to the second vector argument.  
   /// @param t : Object constructor containing the member method to differentiate.  
   /// @param f: Pointer to the member method.
   /// @param dummy: Dummy integer for the method prototype.
   /// @param x: Input vector. 

   template<class T> 
   Matrix<double> calculate_central_differences_Jacobian(const T& t, Vector<double> (T::*f)(const unsigned int&, const Vector<double>&) const, const unsigned int& dummy, const Vector<double>& x) const
   {
      Vector<double> y = (t.*f)(dummy, x); 

      double h;

      unsigned int n = x.size();
      unsigned int m = y.size();

      Vector<double> x_forward(x);
      Vector<double> x_backward(x);

	  Vector<double> y_forward(n);
	  Vector<double> y_backward(n);

      Matrix<double> J(m,n);

      for(unsigned int j = 0; j < n; j++)
	  {
         h = calculate_h(x[j]);

         x_backward[j] -= h;
         y_backward = (t.*f)(dummy, x_backward);   
         x_backward[j] += h;

         x_forward[j] += h;
         y_forward = (t.*f)(dummy, x_forward);   
         x_forward[j] -= h;
         
	     for(unsigned int i = 0; i < m; i++)
		 {
		    J[i][j] = (y_forward[i] - y_backward[i])/(2.0*h);
		 }
	  }

      return(J);
   }


   // Matrix<double> calculate_Jacobian(const T&, Vector<double> (T::*f)(const unsigned int&, const Vector<double>&) const, const unsigned int&, const Vector<double>&) const method

   /// This method returns the Jacobian matrix of a function of many inputs and many outputs according to the numerical differentiation method to be used. 
   /// The function to be differentiated is of the following form: double f(const unsigned int&, const Vector<double>&) const. 
   /// The first integer argument is dummy, differentiation is performed with respect to the second vector argument.  
   /// @param t : Object constructor containing the member method to differentiate.  
   /// @param f: Pointer to the member method.
   /// @param dummy: Dummy integer for the method prototype.
   /// @param x: Input vector. 

   template<class T> 
   Matrix<double> calculate_Jacobian(const T& t, Vector<double> (T::*f)(const unsigned int&, const Vector<double>&) const, const unsigned int& dummy, const Vector<double>& x) const
   {
      switch(numerical_differentiation_method)
      {
         case ForwardDifferences:
         {
            return(calculate_forward_differences_Jacobian(t, f, dummy, x));
      	 }
	     break;

         case CentralDifferences:
         {
            return(calculate_central_differences_Jacobian(t, f, dummy, x));
    	 }
   	     break;

         default:
         {
            std::ostringstream buffer;

            buffer << "OpenNN Exception: NumericalDifferentiation class.\n"
                   << "Matrix<double> calculate_Jacobian(const T&, Vector<double> (T::*f)(const unsigned int&, const Vector<double>&) const, const unsigned int&, const Vector<double>&) const method.\n"
                   << "Unknown numerical differentiation method.\n";
 
            throw std::logic_error(buffer.str());
	     }
	     break;
      }
   }


   // Matrix<double> calculate_forward_differences_Jacobian(const T&, Vector<double> (T::*f)(const unsigned int&, const Vector<double>&, const Vector<double>&) const, const unsigned int&, const Vector<double>&, const Vector<double>&) const method

   /// This method returns the Jacobian matrix of a function of many inputs and many outputs using the forward differences method. 
   /// The function to be differentiated is of the following form: double f(const unsigned int&, const Vector<double>&, const Vector<double>&) const. 
   /// The first and second arguments are dummy, differentiation is performed with respect to the third argument.  
   /// @param t : Object constructor containing the member method to differentiate.  
   /// @param f: Pointer to the member method.
   /// @param dummy_int: Dummy integer for the method prototype.
   /// @param dummy_vector: Dummy vector for the method prototype.
   /// @param x: Input vector. 

   template<class T> 
   Matrix<double> calculate_forward_differences_Jacobian
   (const T& t, Vector<double> (T::*f)(const unsigned int&, const Vector<double>&, const Vector<double>&) const, const unsigned int& dummy_int, const Vector<double>& dummy_vector, const Vector<double>& x) const
   {
      Vector<double> y = (t.*f)(dummy_int, dummy_vector, x); 

      double h;

      unsigned int n = x.size();
      unsigned int m = y.size();

      Vector<double> x_forward(x);
      Vector<double> y_forward(n);

      Matrix<double> J(m,n);

      for(unsigned int j = 0; j < n; j++)
	  {
         h = calculate_h(x[j]);

         x_forward[j] += h;
         y_forward = (t.*f)(dummy_int, dummy_vector, x_forward);   
         x_forward[j] -= h;
         
	     for(unsigned int i = 0; i < m; i++)
		 {
		    J[i][j] = (y_forward[i] - y[i])/h;
		 }
	  }

      return(J);
   }


   // Matrix<double> calculate_central_differences_Jacobian(const T&, Vector<double> (T::*f)(const unsigned int&, const Vector<double>&, const Vector<double>&) const, const unsigned int&, const Vector<double>&, const Vector<double>&) const method

   /// This method returns the Jacobian matrix of a function of many inputs and many outputs using the central differences method. 
   /// The function to be differentiated is of the following form: double f(const unsigned int&, const Vector<double>&, const Vector<double>&) const. 
   /// The first and second arguments are dummy, differentiation is performed with respect to the third argument.  
   /// @param t : Object constructor containing the member method to differentiate.  
   /// @param f: Pointer to the member method.
   /// @param dummy_int: Dummy integer for the method prototype.
   /// @param dummy_vector: Dummy vector for the method prototype.
   /// @param x: Input vector. 

   template<class T> 
   Matrix<double> calculate_central_differences_Jacobian
   (const T& t, Vector<double> (T::*f)(const unsigned int&, const Vector<double>&, const Vector<double>&) const, const unsigned int& dummy_int, const Vector<double>& dummy_vector, const Vector<double>& x) const
   {
      const unsigned int n = x.size();

      const Vector<double> y = (t.*f)(dummy_int, dummy_vector, x); 
      const unsigned int m = y.size();

      double h;

      Vector<double> x_forward(x);
      Vector<double> x_backward(x);

	  Vector<double> y_forward(n);
	  Vector<double> y_backward(n);

      Matrix<double> J(m,n);

      for(unsigned int j = 0; j < n; j++)
	  {
         h = calculate_h(x[j]);

         x_backward[j] -= h;
         y_backward = (t.*f)(dummy_int, dummy_vector, x_backward);   
         x_backward[j] += h;

         x_forward[j] += h;
         y_forward = (t.*f)(dummy_int, dummy_vector, x_forward);   
         x_forward[j] -= h;
         
	     for(unsigned int i = 0; i < m; i++)
		 {
		    J[i][j] = (y_forward[i] - y_backward[i])/(2.0*h);
		 }
	  }

      return(J);
   }


   // Matrix<double> calculate_Jacobian(const T&, Vector<double> (T::*f)(const unsigned int&, const Vector<double>&, const Vector<double>&) const, const unsigned int&, const Vector<double>&, const Vector<double>&) const method

   /// This method returns the Jacobian matrix of a function of many inputs and many outputs according to the numerical differentiation method to be used. 
   /// The function to be differentiated is of the following form: double f(const unsigned int&, const Vector<double>&, const Vector<double>&) const. 
   /// The first and second arguments are dummy, differentiation is performed with respect to the third argument.  
   /// @param t : Object constructor containing the member method to differentiate.  
   /// @param f: Pointer to the member method.
   /// @param dummy_int: Dummy integer for the method prototype.
   /// @param dummy_vector: Dummy vector for the method prototype.
   /// @param x: Input vector. 

   template<class T> 
   Matrix<double> calculate_Jacobian
   (const T& t, Vector<double> (T::*f)(const unsigned int&, const Vector<double>&, const Vector<double>&) const, const unsigned int& dummy_int, const Vector<double>& dummy_vector, const Vector<double>& x) const
   {
      switch(numerical_differentiation_method)
      {
         case ForwardDifferences:
         {
            return(calculate_forward_differences_Jacobian(t, f, dummy_int, dummy_vector, x));
      	 }
	     break;

         case CentralDifferences:
         {
            return(calculate_central_differences_Jacobian(t, f, dummy_int, dummy_vector, x));
    	 }
   	     break;

         default:
         {
            std::ostringstream buffer;

            buffer << "OpenNN Exception: NumericalDifferentiation class.\n"
                   << "Matrix<double> calculate_Jacobian\n"
                   << "(const T&, Vector<double> (T::*f)(const unsigned int&, const Vector<double>&, const Vector<double>&) const, const unsigned int&, const Vector<double>&, const Vector<double>&) const method.\n"
                   << "Unknown numerical differentiation method.\n";
 
            throw std::logic_error(buffer.str());
	     }
	     break;
      }
   }


   // Matrix<double> calculate_forward_differences_Jacobian(const T&, Vector<double> (T::*f)(const unsigned int&, const Vector<double>&, const Vector<double>&) const, const unsigned int&, const Vector<double>&, const Vector<double>&) const method

   /// This method returns the Jacobian matrix of a function of many inputs and many outputs using the forward differences method. 
   /// The function to be differentiated is of the following form: double f(const unsigned int&, const Vector<double>&, const Vector<double>&) const. 
   /// The first and second arguments are dummy, differentiation is performed with respect to the third argument.  
   /// @param t Object constructor containing the member method to differentiate.  
   /// @param f Pointer to the member method.
   /// @param dummy_int_1 Dummy integer for the method prototype.
   /// @param dummy_int_2 Dummy integer for the method prototype.
   /// @param x Input vector. 

   template<class T> 
   Matrix<double> calculate_forward_differences_Jacobian
   (const T& t, Vector<double> (T::*f)(const unsigned int&, const unsigned int&, const Vector<double>&) const, const unsigned int& dummy_int_1, const unsigned int& dummy_int_2, const Vector<double>& x) const
   {
      const Vector<double> y = (t.*f)(dummy_int_1, dummy_int_2, x); 

      const unsigned int n = x.size();
      const unsigned int m = y.size();

      double h;

      Vector<double> x_forward(x);
      Vector<double> y_forward(n);

      Matrix<double> J(m,n);

      for(unsigned int j = 0; j < n; j++)
	  {
         h = calculate_h(x[j]);

         x_forward[j] += h;
         y_forward = (t.*f)(dummy_int_1, dummy_int_2, x_forward);   
         x_forward[j] -= h;
         
	     for(unsigned int i = 0; i < m; i++)
		 {
		    J[i][j] = (y_forward[i] - y[i])/h;
		 }
	  }

      return(J);
   }


   // Matrix<double> calculate_central_differences_Jacobian(const T&, Vector<double> (T::*f)(const unsigned int&, const Vector<double>&, const Vector<double>&) const, const unsigned int&, const Vector<double>&, const Vector<double>&) const method

   /// This method returns the Jacobian matrix of a function of many inputs and many outputs using the central differences method. 
   /// The function to be differentiated is of the following form: double f(const unsigned int&, const Vector<double>&, const Vector<double>&) const. 
   /// The first and second arguments are dummy, differentiation is performed with respect to the third argument.  
   /// @param t : Object constructor containing the member method to differentiate.  
   /// @param f: Pointer to the member method.
   /// @param dummy_int_1: Dummy integer for the method prototype.
   /// @param dummy_int_2: Dummy integer for the method prototype.
   /// @param x: Input vector. 

   template<class T> 
   Matrix<double> calculate_central_differences_Jacobian
   (const T& t, Vector<double> (T::*f)(const unsigned int&, const unsigned int&, const Vector<double>&) const, const unsigned int& dummy_int_1, const unsigned int& dummy_int_2, const Vector<double>& x) const
   {
      const Vector<double> y = (t.*f)(dummy_int_1, dummy_int_2, x); 

      const unsigned int n = x.size();
      const unsigned int m = y.size();

      double h;

      Vector<double> x_forward(x);
      Vector<double> x_backward(x);

	  Vector<double> y_forward(n);
	  Vector<double> y_backward(n);

      Matrix<double> J(m,n);

      for(unsigned int j = 0; j < n; j++)
	  {
         h = calculate_h(x[j]);

         x_backward[j] -= h;
         y_backward = (t.*f)(dummy_int_1, dummy_int_2, x_backward);   
         x_backward[j] += h;

         x_forward[j] += h;
         y_forward = (t.*f)(dummy_int_1, dummy_int_2, x_forward);   
         x_forward[j] -= h;
         
	     for(unsigned int i = 0; i < m; i++)
		 {
		    J[i][j] = (y_forward[i] - y_backward[i])/(2.0*h);
		 }
	  }

      return(J);
   }


   // Matrix<double> calculate_Jacobian(const T&, Vector<double> (T::*f)(const unsigned int&, const Vector<double>&, const Vector<double>&) const, const unsigned int&, const Vector<double>&, const Vector<double>&) const method

   /// This method returns the Jacobian matrix of a function of many inputs and many outputs according to the numerical differentiation method to be used. 
   /// The function to be differentiated is of the following form: double f(const unsigned int&, const Vector<double>&, const Vector<double>&) const. 
   /// The first and second arguments are dummy, differentiation is performed with respect to the third argument.  
   /// @param t Object constructor containing the member method to differentiate.  
   /// @param f Pointer to the member method.
   /// @param dummy_int_1 Dummy integer for the method prototype.
   /// @param dummy_int_2 Dummy integer for the method prototype.
   /// @param x Input vector. 

   template<class T> 
   Matrix<double> calculate_Jacobian
   (const T& t, Vector<double> (T::*f)(const unsigned int&, const unsigned int&, const Vector<double>&) const, const unsigned int& dummy_int_1, const unsigned int& dummy_int_2, const Vector<double>& x) const
   {
      switch(numerical_differentiation_method)
      {
         case ForwardDifferences:
         {
            return(calculate_forward_differences_Jacobian(t, f, dummy_int_1, dummy_int_2, x));
      	 }
	     break;

         case CentralDifferences:
         {
            return(calculate_central_differences_Jacobian(t, f, dummy_int_1, dummy_int_2, x));
    	 }
   	     break;

         default:
         {
            std::ostringstream buffer;

            buffer << "OpenNN Exception: NumericalDifferentiation class.\n"
                   << "Matrix<double> calculate_Jacobian\n"
                   << "(const T&, Vector<double> (T::*f)(const unsigned int&, const unsigned int&, const Vector<double>&) const, const unsigned int&, const unsigned int&, const Vector<double>&) const method.\n"
                   << "Unknown numerical differentiation method.\n";
 
            throw std::logic_error(buffer.str());
	     }
	     break;
      }
   }


   // HESSIAN FORM METHODS


   // Vector< Matrix <double> > calculate_forward_differences_Hessian_form(const T&, Vector<double> (T::*f)(const Vector<double>&) const, const Vector<double>&) const method

   /// This method returns the Hessian form, as a vector of matrices, of a function of many inputs and many outputs using the forward differences method. 
   /// The function to be differentiated is of the following form: Vector<double> f(const Vector<double>&) const. 
   /// @param t : Object constructor containing the member method to differentiate.  
   /// @param f: Pointer to the member method.
   /// @param x: Input vector. 

   template<class T> 
   Vector< Matrix <double> > calculate_forward_differences_Hessian_form(const T& t, Vector<double> (T::*f)(const Vector<double>&) const, const Vector<double>& x) const
   {      
      Vector<double> y = (t.*f)(x);   

      unsigned int s = y.size();
      unsigned int n = x.size();

      double h_j;
      double h_k;

      Vector<double> x_forward_j(x);       
      Vector<double> x_forward_2j(x);       

      Vector<double> x_forward_k(x);       
      Vector<double> x_forward_jk(x);       

      Vector<double> y_forward_j(s);       
      Vector<double> y_forward_2j(s);       

      Vector<double> y_forward_k(s);       
      Vector<double> y_forward_jk(s);       

      Vector< Matrix<double> > H(s);

      for(unsigned int i = 0; i < s; i++)
      {
         H[i].set(n,n);

         for(unsigned int j = 0; j < n; j++)
         {
            h_j = calculate_h(x[j]);

            x_forward_j[j] += h_j;       
            y_forward_j = (t.*f)(x_forward_j);
            x_forward_j[j] -= h_j;       

            x_forward_2j[j] += 2.0*h_j;       
            y_forward_2j = (t.*f)(x_forward_2j);
            x_forward_2j[j] -= 2.0*h_j;       

            H[i][j][j] = (y_forward_2j[i] - 2.0*y_forward_j[i] + y[i])/pow(h_j, 2);  

            for(unsigned int k = j; k < n; k++)
			{
               h_k = calculate_h(x[k]);

               x_forward_k[k] += h_k;       
               y_forward_k = (t.*f)(x_forward_k);
               x_forward_k[k] -= h_k;       

               x_forward_jk[j] += h_j; 
               x_forward_jk[k] += h_k; 
               y_forward_jk = (t.*f)(x_forward_jk);   
               x_forward_jk[j] -= h_j; 
               x_forward_jk[k] -= h_k; 
            
               H[i][j][k] = (y_forward_jk[i] - y_forward_j[i] - y_forward_k[i] + y[i])/(h_j*h_k);
			}
		 }

         for(unsigned int j = 0; j < n; j++)
         {
            for(unsigned int k = 0; k < j; k++)
            {
               H[i][j][k] = H[i][k][j];
            }
         }
      }

      return(H);
   }


   // Vector< Matrix <double> > calculate_central_differences_Hessian_form(const T&, Vector<double> (T::*f)(const Vector<double>&) const, const Vector<double>&) const method

   /// This method returns the Hessian form, as a vector of matrices, of a function of many inputs and many outputs using the central differences method. 
   /// The function to be differentiated is of the following form: Vector<double> f(const Vector<double>&) const. 
   /// @param t : Object constructor containing the member method to differentiate.  
   /// @param f: Pointer to the member method.
   /// @param x: Input vector. 

   template<class T> 
   Vector< Matrix <double> > calculate_central_differences_Hessian_form(const T& t, Vector<double> (T::*f)(const Vector<double>&) const, const Vector<double>& x) const
   {
      Vector<double> y = (t.*f)(x);   

      unsigned int s = y.size();
      unsigned int n = x.size();

      double h_j;
      double h_k;

      Vector<double> x_backward_2j(x);
      Vector<double> x_backward_j(x);

      Vector<double> x_forward_j(x);
      Vector<double> x_forward_2j(x);      

      Vector<double> x_backward_jk(x);
	  Vector<double> x_forward_jk(x);

	  Vector<double> x_backward_j_forward_k(x);
      Vector<double> x_forward_j_backward_k(x);

      Vector<double> y_backward_2j;
      Vector<double> y_backward_j;

      Vector<double> y_forward_j;
      Vector<double> y_forward_2j;
   
      Vector<double> y_backward_jk;
	  Vector<double> y_forward_jk;

	  Vector<double> y_backward_j_forward_k;
      Vector<double> y_forward_j_backward_k;

      Vector< Matrix<double> > H(s);

      for(unsigned int i = 0; i < s; i++)
	  {
         H[i].set(n,n);

      	 for(unsigned int j = 0; j < n; j++)
         {
            h_j = calculate_h(x[j]);

            x_backward_2j[j] -= 2.0*h_j; 
            y_backward_2j = (t.*f)(x_backward_2j);
            x_backward_2j[j] += 2.0*h_j; 

            x_backward_j[j] -= h_j; 
            y_backward_j = (t.*f)(x_backward_j);
            x_backward_j[j] += h_j; 

            x_forward_j[j] += h_j; 
            y_forward_j = (t.*f)(x_forward_j);
            x_forward_j[j] -= h_j; 

            x_forward_2j[j] += 2.0*h_j; 
            y_forward_2j = (t.*f)(x_forward_2j);
            x_forward_2j[j] -= 2.0*h_j; 

            H[i][j][j] = (-y_forward_2j[i] + 16.0*y_forward_j[i] -30.0*y[i] + 16.0*y_backward_j[i] - y_backward_2j[i])/(12.0*pow(h_j, 2));  

            for(unsigned int k = j; k < n; k++)
            {
               h_k = calculate_h(x[k]);

               x_backward_jk[j] -= h_j;  
               x_backward_jk[k] -= h_k;  
               y_backward_jk = (t.*f)(x_backward_jk);   
               x_backward_jk[j] += h_j;  
               x_backward_jk[k] += h_k;  

               x_forward_jk[j] += h_j;  
               x_forward_jk[k] += h_k;  
               y_forward_jk = (t.*f)(x_forward_jk);   
               x_forward_jk[j] -= h_j;  
               x_forward_jk[k] -= h_k;  
            
               x_backward_j_forward_k[j] -= h_j;
               x_backward_j_forward_k[k] += h_k;
               y_backward_j_forward_k = (t.*f)(x_backward_j_forward_k);   
               x_backward_j_forward_k[j] += h_j;
               x_backward_j_forward_k[k] -= h_k;

			   x_forward_j_backward_k[j] += h_j;
			   x_forward_j_backward_k[k] -= h_k;
               y_forward_j_backward_k = (t.*f)(x_forward_j_backward_k);   
			   x_forward_j_backward_k[j] -= h_j;
			   x_forward_j_backward_k[k] += h_k;
 
               H[i][j][k] = (y_forward_jk[i] - y_forward_j_backward_k[i] - y_backward_j_forward_k[i] + y_backward_jk[i])/(4.0*h_j*h_k);
            }
         }
	  
         for(unsigned int j = 0; j < n; j++)
         {
            for(unsigned int k = 0; k < j; k++)
            {
               H[i][j][k] = H[i][k][j];
            }
         }
	  }

      return(H);
   }


   // Vector< Matrix<double> > calculate_Hessian_form(const T&, Vector<double> (T::*f)(const Vector<double>&) const, const Vector<double>&) const method

   /// This method returns the Hessian form, as a vector of matrices, of a function of many inputs and many outputs according to the numerical differentiation method to be used. 
   /// The function to be differentiated is of the following form: Vector<double> f(const Vector<double>&) const. 
   /// @param t : Object constructor containing the member method to differentiate.  
   /// @param f: Pointer to the member method.
   /// @param x: Input vector. 

   template<class T> 
   Vector< Matrix<double> > calculate_Hessian_form(const T& t, Vector<double> (T::*f)(const Vector<double>&) const, const Vector<double>& x) const
   {
      switch(numerical_differentiation_method)
      {
         case ForwardDifferences:
         {
            return(calculate_forward_differences_Hessian_form(t, f, x));
      	 }
	     break;

         case CentralDifferences:
         {
            return(calculate_central_differences_Hessian_form(t, f, x));
         }
   	     break;

         default:
         {
            std::ostringstream buffer;

            buffer << "OpenNN Exception: NumericalDifferentiation class.\n"
                   << "double calculate_Hessian_form(const T& t, Vector<double> (T::*f)(const Vector<double>&), const Vector<double>&) const method.\n"
                   << "Unknown numerical differentiation method.\n";
 
            throw std::logic_error(buffer.str());
	     }
	     break;
      }
   }


   // Vector< Matrix <double> > calculate_forward_differences_Hessian_form
   // (const T& t, Vector<double> (T::*f)(const Vector<double>&, const Vector<double>&) const, const Vector<double>&, const Vector<double>&) const

   /// This method returns the Hessian form, as a vector of matrices, of a function of many inputs and many outputs using the forward differences method. 
   /// The function to be differentiated is of the following form: Vector<double> f(const unsigned int&, const Vector<double>&, const Vector<double>&) const. 
   /// The first and second arguments are dummy, differentiation is performed with respect to the third argument. 
   /// @param t : Object constructor containing the member method to differentiate.  
   /// @param f: Pointer to the member method.
   /// @param dummy_vector: Dummy vector for the method prototype. 
   /// @param x: Input vector. 

   template<class T> 
   Vector< Matrix <double> > calculate_forward_differences_Hessian_form
   (const T& t, Vector<double> (T::*f)(const Vector<double>&, const Vector<double>&) const, const Vector<double>& dummy_vector, const Vector<double>& x) const
   {      
      Vector<double> y = (t.*f)(dummy_vector, x);   

      unsigned int s = y.size();
      unsigned int n = x.size();

      double h_j;
      double h_k;

      Vector<double> x_forward_j(x);       
      Vector<double> x_forward_2j(x);       

      Vector<double> x_forward_k(x);       
      Vector<double> x_forward_jk(x);       

      Vector<double> y_forward_j(s);       
      Vector<double> y_forward_2j(s);       

      Vector<double> y_forward_k(s);       
      Vector<double> y_forward_jk(s);       

      Vector< Matrix<double> > H(s);

      for(unsigned int i = 0; i < s; i++)
      {
         H[i].set(n,n);

         for(unsigned int j = 0; j < n; j++)
         {
            h_j = calculate_h(x[j]);

            x_forward_j[j] += h_j;       
            y_forward_j = (t.*f)(dummy_vector, x_forward_j);
            x_forward_j[j] -= h_j;       

            x_forward_2j[j] += 2.0*h_j;       
            y_forward_2j = (t.*f)(dummy_vector, x_forward_2j);
            x_forward_2j[j] -= 2.0*h_j;       

            H[i][j][j] = (y_forward_2j[i] - 2.0*y_forward_j[i] + y[i])/pow(h_j, 2);  

	        for(unsigned int k = j; k < n; k++)
		    {
               h_k = calculate_h(x[k]);

               x_forward_k[k] += h_k;       
               y_forward_k = (t.*f)(dummy_vector, x_forward_k);
               x_forward_k[k] -= h_k;       

               x_forward_jk[j] += h_j; 
               x_forward_jk[k] += h_k; 
               y_forward_jk = (t.*f)(dummy_vector, x_forward_jk);   
               x_forward_jk[j] -= h_j; 
               x_forward_jk[k] -= h_k; 
            
               H[i][j][k] = (y_forward_jk[i] - y_forward_j[i] - y_forward_k[i] + y[i])/(h_j*h_k);
			}
		 }

         for(unsigned int j = 0; j < n; j++)
         {
            for(unsigned int k = 0; k < j; k++)
            {
               H[i][j][k] = H[i][k][j];
            }
         }
      }

      return(H);
   }


   // Vector< Matrix <double> > calculate_central_differences_Hessian_form
   // (const T&, Vector<double> (T::*f)(const unsigned int&, const Vector<double>&, const Vector<double>&) const, const Vector<double>&, const Vector<double>&) const method

   /// This method returns the Hessian form, as a vector of matrices, of a function of many inputs and many outputs using the central differences method. 
   /// The function to be differentiated is of the following form: Vector<double> f(const unsigned int&, const Vector<double>&, const Vector<double>&) const. 
   /// The first and second arguments are dummy, differentiation is performed with respect to the third argument. 
   /// @param t : Object constructor containing the member method to differentiate.  
   /// @param f: Pointer to the member method.
   /// @param dummy_vector: Dummy vector for the method prototype. 
   /// @param x: Input vector. 

   template<class T> 
   Vector< Matrix <double> > calculate_central_differences_Hessian_form
   (const T& t, Vector<double> (T::*f)(const Vector<double>&, const Vector<double>&) const, const Vector<double>& dummy_vector, const Vector<double>& x) const
   {
      Vector<double> y = (t.*f)(dummy_vector, x);   

      unsigned int s = y.size();
      unsigned int n = x.size();

      double h_j;
      double h_k;

      Vector<double> x_backward_2j(x);
      Vector<double> x_backward_j(x);

      Vector<double> x_forward_j(x);
      Vector<double> x_forward_2j(x);      

      Vector<double> x_backward_jk(x);
      Vector<double> x_forward_jk(x);

	  Vector<double> x_backward_j_forward_k(x);
      Vector<double> x_forward_j_backward_k(x);

      Vector<double> y_backward_2j;
      Vector<double> y_backward_j;

      Vector<double> y_forward_j;
      Vector<double> y_forward_2j;
   
      Vector<double> y_backward_jk;
	  Vector<double> y_forward_jk;

	  Vector<double> y_backward_j_forward_k;
      Vector<double> y_forward_j_backward_k;

      Vector< Matrix<double> > H(s);

      for(unsigned int i = 0; i < s; i++)
	  {
         H[i].set(n,n);

      	 for(unsigned int j = 0; j < n; j++)
         {
            h_j = calculate_h(x[j]);

            x_backward_2j[j] -= 2.0*h_j; 
            y_backward_2j = (t.*f)(dummy_vector, x_backward_2j);
            x_backward_2j[j] += 2.0*h_j; 

            x_backward_j[j] -= h_j; 
            y_backward_j = (t.*f)(dummy_vector, x_backward_j);
            x_backward_j[j] += h_j; 

            x_forward_j[j] += h_j; 
            y_forward_j = (t.*f)(dummy_vector, x_forward_j);
            x_forward_j[j] -= h_j; 

            x_forward_2j[j] += 2.0*h_j; 
            y_forward_2j = (t.*f)(dummy_vector, x_forward_2j);
            x_forward_2j[j] -= 2.0*h_j; 

            H[i][j][j] = (-y_forward_2j[i] + 16.0*y_forward_j[i] -30.0*y[i] + 16.0*y_backward_j[i] - y_backward_2j[i])/(12.0*pow(h_j, 2));  

            for(unsigned int k = j; k < n; k++)
            {
               h_k = calculate_h(x[k]);

               x_backward_jk[j] -= h_j;  
               x_backward_jk[k] -= h_k;  
               y_backward_jk = (t.*f)(dummy_vector, x_backward_jk);   
               x_backward_jk[j] += h_j;  
               x_backward_jk[k] += h_k;  

               x_forward_jk[j] += h_j;  
               x_forward_jk[k] += h_k;  
               y_forward_jk = (t.*f)(dummy_vector, x_forward_jk);   
               x_forward_jk[j] -= h_j;  
               x_forward_jk[k] -= h_k;  
            
               x_backward_j_forward_k[j] -= h_j;
               x_backward_j_forward_k[k] += h_k;
               y_backward_j_forward_k = (t.*f)(dummy_vector, x_backward_j_forward_k);   
               x_backward_j_forward_k[j] += h_j;
               x_backward_j_forward_k[k] -= h_k;

			   x_forward_j_backward_k[j] += h_j;
			   x_forward_j_backward_k[k] -= h_k;
               y_forward_j_backward_k = (t.*f)(dummy_vector, x_forward_j_backward_k);   
			   x_forward_j_backward_k[j] -= h_j;
			   x_forward_j_backward_k[k] += h_k;
 
               H[i][j][k] = (y_forward_jk[i] - y_forward_j_backward_k[i] - y_backward_j_forward_k[i] + y_backward_jk[i])/(4.0*h_j*h_k);
            }
         }
	  
         for(unsigned int j = 0; j < n; j++)
         {
            for(unsigned int k = 0; k < j; k++)
            {
               H[i][j][k] = H[i][k][j];
            }
         }
	  }

      return(H);
   }


   // Vector< Matrix<double> > calculate_Hessian_form
   // (const T& t, Vector<double> (T::*f)(const Vector<double>&, const Vector<double>&) const, const Vector<double>&, const Vector<double>&) const method

   /// This method returns the Hessian form, as a vector of matrices, of a function of many inputs and many outputs according to the numerical differentiation method to be used. 
   /// The function to be differentiated is of the following form: Vector<double> f(const unsigned int&, const Vector<double>&, const Vector<double>&) const. 
   /// The first and second arguments are dummy, differentiation is performed with respect to the third argument. 
   /// @param t : Object constructor containing the member method to differentiate.  
   /// @param f: Pointer to the member method.
   /// @param dummy_vector: Dummy vector for the method prototype. 
   /// @param x: Input vector. 

   template<class T> 
   Vector< Matrix<double> > calculate_Hessian_form
   (const T& t, Vector<double> (T::*f)(const Vector<double>&, const Vector<double>&) const, const Vector<double>& dummy_vector, const Vector<double>& x) const
   {
      switch(numerical_differentiation_method)
      {
         case ForwardDifferences:
         {
            return(calculate_forward_differences_Hessian_form(t, f, dummy_vector, x));
      	 }
	     break;

         case CentralDifferences:
         {
            return(calculate_central_differences_Hessian_form(t, f, dummy_vector, x));
    	 }
   	     break;

         default:
         {
            std::ostringstream buffer;

            buffer << "OpenNN Exception: NumericalDifferentiation class.\n"
                   << "Vector< Matrix<double> > calculate_Hessian_form\n"
                   << "(const T& t, Vector<double> (T::*f)(const Vector<double>&, const Vector<double>&) const, const Vector<double>&, const Vector<double>&) const method.\n"
                   << "Unknown numerical differentiation method.\n";
 
            throw std::logic_error(buffer.str());
	     }
	     break;
      }
   }


   // Vector< Matrix <double> > calculate_forward_differences_Hessian_form(const T&, Vector<double> (T::*f)(const unsigned int&, const Vector<double>&) const, const unsigned int&, const Vector<double>&) const method

   /// This method returns the Hessian form, as a vector of matrices, of a function of many inputs and many outputs using the forward differences method. 
   /// The function to be differentiated is of the following form: Vector<double> f(const unsigned int&, const Vector<double>&) const. 
   /// The first argument is dummy, differentiation is performed with respect to the second argument. 
   /// @param t : Object constructor containing the member method to differentiate.  
   /// @param f: Pointer to the member method.
   /// @param dummy: Dummy integer for the method prototype. 
   /// @param x: Input vector. 

   template<class T> 
   Vector< Matrix <double> > calculate_forward_differences_Hessian_form(const T& t, Vector<double> (T::*f)(const unsigned int&, const Vector<double>&) const, const unsigned int& dummy, const Vector<double>& x) const
   {      
      Vector<double> y = (t.*f)(dummy, x);   

      unsigned int s = y.size();
      unsigned int n = x.size();

      double h_j;
      double h_k;

      Vector<double> x_forward_j(x);       
      Vector<double> x_forward_2j(x);       

      Vector<double> x_forward_k(x);       
      Vector<double> x_forward_jk(x);       

      Vector<double> y_forward_j(s);       
      Vector<double> y_forward_2j(s);       

      Vector<double> y_forward_k(s);       
      Vector<double> y_forward_jk(s);       

      Vector< Matrix<double> > H(s);

      for(unsigned int i = 0; i < s; i++)
      {
         H[i].set(n,n);

         for(unsigned int j = 0; j < n; j++)
		   {
            h_j = calculate_h(x[j]);

            x_forward_j[j] += h_j;       
            y_forward_j = (t.*f)(dummy, x_forward_j);
            x_forward_j[j] -= h_j;       

            x_forward_2j[j] += 2.0*h_j;       
            y_forward_2j = (t.*f)(dummy, x_forward_2j);
            x_forward_2j[j] -= 2.0*h_j;       

            H[i][j][j] = (y_forward_2j[i] - 2.0*y_forward_j[i] + y[i])/pow(h_j, 2);  

	         for(unsigned int k = j; k < n; k++)
			   {
               h_k = calculate_h(x[k]);

               x_forward_k[k] += h_k;       
               y_forward_k = (t.*f)(dummy, x_forward_k);
               x_forward_k[k] -= h_k;       

               x_forward_jk[j] += h_j; 
               x_forward_jk[k] += h_k; 
               y_forward_jk = (t.*f)(dummy, x_forward_jk);   
               x_forward_jk[j] -= h_j; 
               x_forward_jk[k] -= h_k; 
            
               H[i][j][k] = (y_forward_jk[i] - y_forward_j[i] - y_forward_k[i] + y[i])/(h_j*h_k);
            }
		 }

         for(unsigned int j = 0; j < n; j++)
         {
            for(unsigned int k = 0; k < j; k++)
            {
               H[i][j][k] = H[i][k][j];
            }
         }
      }

      return(H);
   }


   // Vector< Matrix <double> > calculate_central_differences_Hessian_form(const T&, Vector<double> (T::*f)(const unsigned int&, const Vector<double>&) const, const unsigned int&, const Vector<double>&) const method

   /// This method returns the Hessian form, as a vector of matrices, of a function of many inputs and many outputs using the central differences method. 
   /// The function to be differentiated is of the following form: Vector<double> f(const unsigned int&, const Vector<double>&) const. 
   /// The first argument is dummy, differentiation is performed with respect to the second argument. 
   /// @param t : Object constructor containing the member method to differentiate.  
   /// @param f: Pointer to the member method.
   /// @param dummy: Dummy integer for the method prototype. 
   /// @param x: Input vector. 

   template<class T> 
   Vector< Matrix <double> > calculate_central_differences_Hessian_form(const T& t, Vector<double> (T::*f)(const unsigned int&, const Vector<double>&) const, const unsigned int& dummy, const Vector<double>& x) const
   {
      Vector<double> y = (t.*f)(dummy, x);   

      unsigned int s = y.size();
      unsigned int n = x.size();

      double h_j;
      double h_k;

      Vector<double> x_backward_2j(x);
      Vector<double> x_backward_j(x);

      Vector<double> x_forward_j(x);
      Vector<double> x_forward_2j(x);      

      Vector<double> x_backward_jk(x);
      Vector<double> x_forward_jk(x);

	  Vector<double> x_backward_j_forward_k(x);
      Vector<double> x_forward_j_backward_k(x);

      Vector<double> y_backward_2j;
      Vector<double> y_backward_j;

      Vector<double> y_forward_j;
      Vector<double> y_forward_2j;
   
      Vector<double> y_backward_jk;
	  Vector<double> y_forward_jk;

	  Vector<double> y_backward_j_forward_k;
      Vector<double> y_forward_j_backward_k;

      Vector< Matrix<double> > H(s);

      for(unsigned int i = 0; i < s; i++)
	  {
         H[i].set(n,n);

      	 for(unsigned int j = 0; j < n; j++)
         {
            h_j = calculate_h(x[j]);

            x_backward_2j[j] -= 2.0*h_j; 
            y_backward_2j = (t.*f)(dummy, x_backward_2j);
            x_backward_2j[j] += 2.0*h_j; 

            x_backward_j[j] -= h_j; 
            y_backward_j = (t.*f)(dummy, x_backward_j);
            x_backward_j[j] += h_j; 

            x_forward_j[j] += h_j; 
            y_forward_j = (t.*f)(dummy, x_forward_j);
            x_forward_j[j] -= h_j; 

            x_forward_2j[j] += 2.0*h_j; 
            y_forward_2j = (t.*f)(dummy, x_forward_2j);
            x_forward_2j[j] -= 2.0*h_j; 

            H[i][j][j] = (-y_forward_2j[i] + 16.0*y_forward_j[i] -30.0*y[i] + 16.0*y_backward_j[i] - y_backward_2j[i])/(12.0*pow(h_j, 2));  

            for(unsigned int k = j; k < n; k++)
            {
               h_k = calculate_h(x[k]);

               x_backward_jk[j] -= h_j;  
               x_backward_jk[k] -= h_k;  
               y_backward_jk = (t.*f)(dummy, x_backward_jk);   
               x_backward_jk[j] += h_j;  
               x_backward_jk[k] += h_k;  

               x_forward_jk[j] += h_j;  
               x_forward_jk[k] += h_k;  
               y_forward_jk = (t.*f)(dummy, x_forward_jk);   
               x_forward_jk[j] -= h_j;  
               x_forward_jk[k] -= h_k;  
            
               x_backward_j_forward_k[j] -= h_j;
               x_backward_j_forward_k[k] += h_k;
               y_backward_j_forward_k = (t.*f)(dummy, x_backward_j_forward_k);   
               x_backward_j_forward_k[j] += h_j;
               x_backward_j_forward_k[k] -= h_k;

			   x_forward_j_backward_k[j] += h_j;
			   x_forward_j_backward_k[k] -= h_k;
               y_forward_j_backward_k = (t.*f)(dummy, x_forward_j_backward_k);   
			   x_forward_j_backward_k[j] -= h_j;
			   x_forward_j_backward_k[k] += h_k;
 
               H[i][j][k] = (y_forward_jk[i] - y_forward_j_backward_k[i] - y_backward_j_forward_k[i] + y_backward_jk[i])/(4.0*h_j*h_k);
            }
         }
	  
         for(unsigned int j = 0; j < n; j++)
         {
            for(unsigned int k = 0; k < j; k++)
            {
               H[i][j][k] = H[i][k][j];
            }
         }
	  }

      return(H);
   }


   // Vector< Matrix<double> > calculate_Hessian_form(const T&, Vector<double> (T::*f)(const unsigned int&, const Vector<double>&) const, const unsigned int&, const Vector<double>&) const method

   /// This method returns the Hessian form, as a vector of matrices, of a function of many inputs and many outputs according to the numerical differentiation method to be used. 
   /// The function to be differentiated is of the following form: Vector<double> f(const unsigned int&, const Vector<double>&) const. 
   /// The first argument is dummy, differentiation is performed with respect to the second argument. 
   /// @param t : Object constructor containing the member method to differentiate.  
   /// @param f: Pointer to the member method.
   /// @param dummy: Dummy integer for the method prototype. 
   /// @param x: Input vector. 

   template<class T> 
   Vector< Matrix<double> > calculate_Hessian_form(const T& t, Vector<double> (T::*f)(const unsigned int&, const Vector<double>&) const, const unsigned int& dummy, const Vector<double>& x) const
   {
      switch(numerical_differentiation_method)
      {
         case ForwardDifferences:
         {
            return(calculate_forward_differences_Hessian_form(t, f, dummy, x));
      	 }
	     break;

         case CentralDifferences:
         {
            return(calculate_central_differences_Hessian_form(t, f, dummy, x));
    	 }
   	     break;

         default:
         {
            std::ostringstream buffer;

            buffer << "OpenNN Exception: NumericalDifferentiation class.\n"
                   << "double calculate_Hessian_form(const T& t, Vector<double> (T::*f)(const unsigned int&, const Vector<double>&), const unsigned int&, const Vector<double>&) const method.\n"
                   << "Unknown numerical differentiation method.\n";
 
            throw std::logic_error(buffer.str());
	     }
	     break;
      }
   }


   // Vector< Matrix <double> > calculate_forward_differences_Hessian_form
   // (const T& t, Vector<double> (T::*f)(const unsigned int&, const Vector<double>&, const Vector<double>&) const, const unsigned int&, const Vector<double>&, const Vector<double>&) const

   /// This method returns the Hessian form, as a vector of matrices, of a function of many inputs and many outputs using the forward differences method. 
   /// The function to be differentiated is of the following form: Vector<double> f(const unsigned int&, const Vector<double>&, const Vector<double>&) const. 
   /// The first and second arguments are dummy, differentiation is performed with respect to the third argument. 
   /// @param t : Object constructor containing the member method to differentiate.  
   /// @param f: Pointer to the member method.
   /// @param dummy_int: Dummy integer for the method prototype. 
   /// @param dummy_vector: Dummy vector for the method prototype. 
   /// @param x: Input vector. 

   template<class T> 
   Vector< Matrix <double> > calculate_forward_differences_Hessian_form
   (const T& t, Vector<double> (T::*f)(const unsigned int&, const Vector<double>&, const Vector<double>&) const, const unsigned int& dummy_int, const Vector<double>& dummy_vector, const Vector<double>& x) const
   {      
      Vector<double> y = (t.*f)(dummy_int, dummy_vector, x);   

      unsigned int s = y.size();
      unsigned int n = x.size();

      double h_j;
      double h_k;

      Vector<double> x_forward_j(x);       
      Vector<double> x_forward_2j(x);       

      Vector<double> x_forward_k(x);       
      Vector<double> x_forward_jk(x);       

      Vector<double> y_forward_j(s);       
      Vector<double> y_forward_2j(s);       

      Vector<double> y_forward_k(s);       
      Vector<double> y_forward_jk(s);       

      Vector< Matrix<double> > H(s);

      for(unsigned int i = 0; i < s; i++)
      {
         H[i].set(n,n);

         for(unsigned int j = 0; j < n; j++)
         {
            h_j = calculate_h(x[j]);

            x_forward_j[j] += h_j;       
            y_forward_j = (t.*f)(dummy_int, dummy_vector, x_forward_j);
            x_forward_j[j] -= h_j;       

            x_forward_2j[j] += 2.0*h_j;       
            y_forward_2j = (t.*f)(dummy_int, dummy_vector, x_forward_2j);
            x_forward_2j[j] -= 2.0*h_j;       

            H[i][j][j] = (y_forward_2j[i] - 2.0*y_forward_j[i] + y[i])/pow(h_j, 2);  

	        for(unsigned int k = j; k < n; k++)
		    {
               h_k = calculate_h(x[k]);

               x_forward_k[k] += h_k;       
               y_forward_k = (t.*f)(dummy_int, dummy_vector, x_forward_k);
               x_forward_k[k] -= h_k;       

               x_forward_jk[j] += h_j; 
               x_forward_jk[k] += h_k; 
               y_forward_jk = (t.*f)(dummy_int, dummy_vector, x_forward_jk);   
               x_forward_jk[j] -= h_j; 
               x_forward_jk[k] -= h_k; 
            
               H[i][j][k] = (y_forward_jk[i] - y_forward_j[i] - y_forward_k[i] + y[i])/(h_j*h_k);
			}
		 }

         for(unsigned int j = 0; j < n; j++)
         {
            for(unsigned int k = 0; k < j; k++)
            {
               H[i][j][k] = H[i][k][j];
            }
         }
      }

      return(H);
   }


   // Vector< Matrix <double> > calculate_central_differences_Hessian_form
   // (const T&, Vector<double> (T::*f)(const unsigned int&, const Vector<double>&, const Vector<double>&) const, const unsigned int&, const Vector<double>&, const Vector<double>&) const method

   /// This method returns the Hessian form, as a vector of matrices, of a function of many inputs and many outputs using the central differences method. 
   /// The function to be differentiated is of the following form: Vector<double> f(const unsigned int&, const Vector<double>&, const Vector<double>&) const. 
   /// The first and second arguments are dummy, differentiation is performed with respect to the third argument. 
   /// @param t : Object constructor containing the member method to differentiate.  
   /// @param f: Pointer to the member method.
   /// @param dummy_int: Dummy integer for the method prototype. 
   /// @param dummy_vector: Dummy vector for the method prototype. 
   /// @param x: Input vector. 

   template<class T> 
   Vector< Matrix <double> > calculate_central_differences_Hessian_form
   (const T& t, Vector<double> (T::*f)(const unsigned int&, const Vector<double>&, const Vector<double>&) const, const unsigned int& dummy_int, const Vector<double>& dummy_vector, const Vector<double>& x) const
   {
      const Vector<double> y = (t.*f)(dummy_int, dummy_vector, x);   

      unsigned int s = y.size();
      unsigned int n = x.size();

      double h_j;
      double h_k;

      Vector<double> x_backward_2j(x);
      Vector<double> x_backward_j(x);

      Vector<double> x_forward_j(x);
      Vector<double> x_forward_2j(x);      

      Vector<double> x_backward_jk(x);
      Vector<double> x_forward_jk(x);

	  Vector<double> x_backward_j_forward_k(x);
      Vector<double> x_forward_j_backward_k(x);

      Vector<double> y_backward_2j;
      Vector<double> y_backward_j;

      Vector<double> y_forward_j;
      Vector<double> y_forward_2j;
   
      Vector<double> y_backward_jk;
	  Vector<double> y_forward_jk;

	  Vector<double> y_backward_j_forward_k;
      Vector<double> y_forward_j_backward_k;

      Vector< Matrix<double> > H(s);

      for(unsigned int i = 0; i < s; i++)
	  {
         H[i].set(n,n);

      	 for(unsigned int j = 0; j < n; j++)
         {
            h_j = calculate_h(x[j]);

            x_backward_2j[j] -= 2.0*h_j; 
            y_backward_2j = (t.*f)(dummy_int, dummy_vector, x_backward_2j);
            x_backward_2j[j] += 2.0*h_j; 

            x_backward_j[j] -= h_j; 
            y_backward_j = (t.*f)(dummy_int, dummy_vector, x_backward_j);
            x_backward_j[j] += h_j; 

            x_forward_j[j] += h_j; 
            y_forward_j = (t.*f)(dummy_int, dummy_vector, x_forward_j);
            x_forward_j[j] -= h_j; 

            x_forward_2j[j] += 2.0*h_j; 
            y_forward_2j = (t.*f)(dummy_int, dummy_vector, x_forward_2j);
            x_forward_2j[j] -= 2.0*h_j; 

            H[i][j][j] = (-y_forward_2j[i] + 16.0*y_forward_j[i] -30.0*y[i] + 16.0*y_backward_j[i] - y_backward_2j[i])/(12.0*pow(h_j, 2));  

            for(unsigned int k = j; k < n; k++)
            {
               h_k = calculate_h(x[k]);

               x_backward_jk[j] -= h_j;  
               x_backward_jk[k] -= h_k;  
               y_backward_jk = (t.*f)(dummy_int, dummy_vector, x_backward_jk);   
               x_backward_jk[j] += h_j;  
               x_backward_jk[k] += h_k;  

               x_forward_jk[j] += h_j;  
               x_forward_jk[k] += h_k;  
               y_forward_jk = (t.*f)(dummy_int, dummy_vector, x_forward_jk);   
               x_forward_jk[j] -= h_j;  
               x_forward_jk[k] -= h_k;  
            
               x_backward_j_forward_k[j] -= h_j;
               x_backward_j_forward_k[k] += h_k;
               y_backward_j_forward_k = (t.*f)(dummy_int, dummy_vector, x_backward_j_forward_k);   
               x_backward_j_forward_k[j] += h_j;
               x_backward_j_forward_k[k] -= h_k;

			   x_forward_j_backward_k[j] += h_j;
			   x_forward_j_backward_k[k] -= h_k;
               y_forward_j_backward_k = (t.*f)(dummy_int, dummy_vector, x_forward_j_backward_k);   
			   x_forward_j_backward_k[j] -= h_j;
			   x_forward_j_backward_k[k] += h_k;
 
               H[i][j][k] = (y_forward_jk[i] - y_forward_j_backward_k[i] - y_backward_j_forward_k[i] + y_backward_jk[i])/(4.0*h_j*h_k);
            }
         }
	  
         for(unsigned int j = 0; j < n; j++)
         {
            for(unsigned int k = 0; k < j; k++)
            {
               H[i][j][k] = H[i][k][j];
            }
         }
	  }

      return(H);
   }


   // Vector< Matrix<double> > calculate_Hessian_form
   // (const T& t, Vector<double> (T::*f)(const unsigned int&, const Vector<double>&, const Vector<double>&) const, const unsigned int&, const Vector<double>&, const Vector<double>&) const method

   /// This method returns the Hessian form, as a vector of matrices, of a function of many inputs and many outputs according to the numerical differentiation method to be used. 
   /// The function to be differentiated is of the following form: Vector<double> f(const unsigned int&, const Vector<double>&, const Vector<double>&) const. 
   /// The first and second arguments are dummy, differentiation is performed with respect to the third argument. 
   /// @param t : Object constructor containing the member method to differentiate.  
   /// @param f: Pointer to the member method.
   /// @param dummy_int: Dummy integer for the method prototype. 
   /// @param dummy_vector: Dummy vector for the method prototype. 
   /// @param x: Input vector. 

   template<class T> 
   Vector< Matrix<double> > calculate_Hessian_form
   (const T& t, Vector<double> (T::*f)(const unsigned int&, const Vector<double>&, const Vector<double>&) const, const unsigned int& dummy_int, const Vector<double>& dummy_vector, const Vector<double>& x) const
   {
      switch(numerical_differentiation_method)
      {
         case ForwardDifferences:
         {
            return(calculate_forward_differences_Hessian_form(t, f, dummy_int, dummy_vector, x));
      	 }
	     break;

         case CentralDifferences:
         {
            return(calculate_central_differences_Hessian_form(t, f, dummy_int, dummy_vector, x));
    	 }
   	     break;

         default:
         {
            std::ostringstream buffer;

            buffer << "OpenNN Exception: NumericalDifferentiation class.\n"
                   << "Vector< Matrix<double> > calculate_Hessian_form\n"
                   << "(const T& t, Vector<double> (T::*f)(const unsigned int&, const Vector<double>&, const Vector<double>&) const, const unsigned int&, const Vector<double>&, const Vector<double>&) const method.\n"
                   << "Unknown numerical differentiation method.\n";
 
            throw std::logic_error(buffer.str());
	     }
	     break;
      }
   }


private:

   /// Numerical differentiation method variable. 

   NumericalDifferentiationMethod numerical_differentiation_method;

   /// Number of precision digits. 

   unsigned int precision_digits;

   /// Flag for displaying warning messages from this class. 

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

